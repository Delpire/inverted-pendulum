using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Management;
using System.Diagnostics;
using System.ComponentModel;
using System.Windows.Forms;

namespace EvaUI
{
    public class MainController
    {
        private IMainView view;
        private GloLink link;
        private bool linkConnected = false;
        private bool capturingData = false;

        private System.Threading.Timer linkStatsTimer;
        private int linkStatsTimerInterval = 500; // milliseconds
        private int lastBytesSent;
        private int lastBytesReceived;

        private BackgroundWorker captureWaitingWorker = new BackgroundWorker();

        // Startup time of application used for creating unique run output directory.
        private static DateTime startTime = DateTime.Now;

        private string currentOutputDirectory;
        private string sessionDirectoryName;
        private string sessionDirectory;
        private const string rootOutputName = "EvaOutput";

        private const int defaultCaptureDuration = 3;
        private const int defaultCaptureRate = 100;
        private const int minCaptureRate = 1;
        private const int maxCaptureRate = 1000;

        private LogFile logFile;

        private List<float[]> captureData = new List<float[]>();

        public bool drivingModeEnabled = false;

        public MainController(IMainView view)
        {
            this.view = view;

            // Create log file and then initially setup all output paths.
            string dateTimeText = String.Format("{0:yyyy-MM-dd_hh-mm-ss-tt}", startTime);
            string logFileName = String.Format("log_{0}.txt", dateTimeText);
            sessionDirectoryName = String.Format("session_{0}", dateTimeText);
            logFile = new LogFile("./", logFileName);
            UpdateOutputPaths(view.getOutputDirectory());

            // Setup view.
            requestNewPortList(); // to keep user from having to request them.
            view.setNumberTransferPackets(0);
            view.setNumberReceivedPackets(0);
            view.setNumberTransferBytes(0);
            view.setNumberReceivedBytes(0);
            view.CaptureDuration = defaultCaptureDuration;
            view.CaptureSampleRate = defaultCaptureRate;
            view.TiltAngle = 0;

            // Setup driving mode by enabling it and then flipping it back.
            drivingModeEnabled = true;
            changeDrivingMode();

            // Try to see if last connected com port is avaiable to select.
            string lastPortName = Properties.Settings.Default.LastPortName;
            view.selectPortName(lastPortName);

            // Setup link.
            link = new GloLink();
            link.PacketReceived += new PacketReceivedEventHandler(link_PacketReceived);

            // Setup timers.
            linkStatsTimer = new System.Threading.Timer(LinkStatsTimerCallback, null, 0, linkStatsTimerInterval);
            captureWaitingWorker.DoWork += new DoWorkEventHandler(captureWaitingWorker_DoWork);
            captureWaitingWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(captureWaitingWorker_RunWorkerCompleted);
        }

        private void captureWaitingWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            Thread.Sleep((int)e.Argument);
        }

        private void captureWaitingWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (capturingData)
            {
                stopDataCapture();
            }
        }

        private void LinkStatsTimerCallback(Object o)
        {
            int numberBytesSent = link.NumberBytesSent;
            int numberBytesReceived = link.NumberBytesReceived;

            double timerIntervalSeconds = linkStatsTimerInterval / 1000.0;

            int bytesSentPerSecond = (int)((numberBytesSent - lastBytesSent) / timerIntervalSeconds);
            int bytesReceivedPerSecond = (int)((numberBytesReceived - lastBytesReceived) / timerIntervalSeconds);

            view.setNumberTransferPackets(link.NumberPacketsSent);
            view.setNumberReceivedPackets(link.NumberPacketsReceived);
            view.setNumberTransferBytes(bytesSentPerSecond);
            view.setNumberReceivedBytes(bytesReceivedPerSecond);

            lastBytesSent = numberBytesSent;
            lastBytesReceived = numberBytesReceived;
        }

        public void changeDrivingMode()
        {
            if (!drivingModeEnabled)
            {
                view.setDrivingModeButtonText("Driving Enabled");
                view.setDrivingModeButtonColor(Color.LightGreen);
            }
            else // driving mode disabled
            {
                view.setDrivingModeButtonText("Driving Disabled");
                view.setDrivingModeButtonColor(Color.LightPink);
            }

            // toggle driving mode status
            drivingModeEnabled = !drivingModeEnabled;
        }

        public bool handleDrivingKey(Keys key, bool justCheck=false)
        {
            if (!drivingModeEnabled)
            {
                return false; // never handle if not in driving mode.
            }

            bool handled = true;

            try
            {
                MovementCommand command = tryConvertKeyToMovementCommand(key);

                if (!justCheck)
                {
                    sendMovementCommand(command);
                }
            }
            catch (ArgumentException)
            {
                handled = false;
            }

            return handled;
        }

        private MovementCommand tryConvertKeyToMovementCommand(Keys key)
        {
            if (key == Keys.Up) { return MovementCommand.Forward; }
            else if (key == Keys.Down) { return MovementCommand.Reverse; }
            else if (key == Keys.Right) { return MovementCommand.TurnRight; }
            else if (key == Keys.Left)  { return MovementCommand.TurnLeft; }
            else if (key == Keys.Space) { return MovementCommand.Stop; }
            else
            {
                throw new ArgumentException("Key press not found.");
            }
        }

        public bool changePortConnectionStatus(string portName)
        {
            if (String.IsNullOrWhiteSpace(portName))
            {
                logAndDisplayMessage("No port selected", MessageSource.UI);
                return false;
            }

            if (linkConnected)
            {
                disconnectPort(portName);
            }
            else // link disconnected so need to try to connect 
            {
                connectPort(portName);
            }

            // Successfully changed port connection status.
            return true;
        }

        private void disconnectPort(string portName)
        {
            link.disconnect();
            linkConnected = false;
            view.setConnectButtonText("Connect");
            logAndDisplayMessage("Port closed", MessageSource.UI);
        }

        private void connectPort(string portName)
        {
            try
            {
                link.connect(portName);
                linkConnected = true;
            }
            catch (System.IO.IOException e)
            {
                logAndDisplayMessage(String.Format("Failed to open \"{0}\"\n{1}", portName, e.Message), MessageSource.UI);
                link.disconnect();
                linkConnected = false;
            }

            if (linkConnected)
            {
                logAndDisplayMessage(String.Format("Opened port \"{0}\"", portName), MessageSource.UI);
                view.setConnectButtonText("Disconnect");

                // Save port name for next time form is opened.
                Properties.Settings.Default.LastPortName = portName;
                Properties.Settings.Default.Save();
            }
        }

        public void changeCaptureStatus()
        {
            if (capturingData)
            {
                stopDataCapture();
            }
            else // not yet capturing data so need to start
            {
                startDataCapture();
            }
        }

        private void stopDataCapture()
        {
            GloCaptureCommand stopCommand = new GloCaptureCommand() { isStart = 0 };
            sendGloObject(ref stopCommand, Glo.IDs.CaptureCommand);

            capturingData = false;
            view.setCaptureButtonText("Start");
            logAndDisplayMessage("Capture stopped", MessageSource.UI);

            // write out all received data
            writeDataToFile();
        }

        private void startDataCapture()
        {
            captureData.Clear();

            if (verifyCaptureParameters())
            {
                GloCaptureCommand startCommand = new GloCaptureCommand() 
                {
                    isStart = 1,
                    frequency = (UInt16)view.CaptureSampleRate,
                };
                sendGloObject(ref startCommand, Glo.IDs.CaptureCommand);

                capturingData = true;
                view.setCaptureButtonText("Stop");
                logAndDisplayMessage("Capture started", MessageSource.UI);

                captureWaitingWorker.RunWorkerAsync(view.CaptureDuration * 1000);
            }
        }

        public void requestNewPortList()
        {
            logAndDisplayMessage("Refreshing ports", MessageSource.UI);

            view.setPortList(SerialPort.GetPortNames());
        }

        public void sendMovementCommand(MovementCommand command)
        { 
            // Create new command packet to send.
            GloObject<GloDrivingCommand> gloCommand = new GloObject<GloDrivingCommand>(Glo.IDs.DrivingCommand);
            gloCommand.data.movementType = (UInt32)command;

            link.send(gloCommand);
        }

        private void sendGloObject<T>(ref T gloData, Glo.IDs ID) where T : struct
        {
            GloObject<T> gloObject = new GloObject<T>(ID) { data = gloData };

            try
            {
                link.send<T>(gloObject);
            }
            catch (TimeoutException)
            {
                logAndDisplayMessage("Port write timed out", MessageSource.UI);
            }
        }

        private bool verifyCaptureParameters()
        {
            if (view.CaptureDuration <= 0)
            {
                view.popUpMessage("Duration must be greater than zero");
                view.CaptureDuration = 0;
                return false;
            }

            if (view.CaptureSampleRate < minCaptureRate)
            {
                view.popUpMessage(String.Format("Minimum capture rate: {0}", minCaptureRate));
                view.CaptureSampleRate = minCaptureRate;
                return false;
            }

            if (view.CaptureSampleRate > maxCaptureRate)
            {
                view.popUpMessage(String.Format("Maximum capture rate: {0}", maxCaptureRate));
                view.CaptureSampleRate = maxCaptureRate;
                return false;
            }

            return true; // all parameters ok
        }

        void link_PacketReceived(object sender, PacketReceivedEventArgs e)
        {
            switch (e.ID)
            {
                case (byte)Glo.IDs.AssertMessage:
                    GloObject<GloAssertMessage> assertMessage = new GloObject<GloAssertMessage>(Glo.IDs.AssertMessage);
                    assertMessage.data = GloLink.ByteArrayToStructure<GloAssertMessage>(e.Data);
                    logAndDisplayMessage(assertMessage.data.text, MessageSource.RobotAssert);
                    break;

                case (byte)Glo.IDs.DebugMessage:
                    GloObject<GloDebugMessage> debugMessage = new GloObject<GloDebugMessage>(Glo.IDs.DebugMessage);
                    debugMessage.data = GloLink.ByteArrayToStructure<GloDebugMessage>(e.Data);
                    logAndDisplayMessage(debugMessage.data.text, MessageSource.RobotDebug);
                    break;

                case (byte)Glo.IDs.CaptureData:

                    if (capturingData)
                    {
                        GloObject<GloCaptureData> testData = new GloObject<GloCaptureData>(Glo.IDs.CaptureData);
                        testData.data = GloLink.ByteArrayToStructure<GloCaptureData>(e.Data);

                        captureData.Add(new float[] { (float)testData.data.time, (float)(testData.data.tiltAngle * 180.0 / Math.PI) });
                    }

                    break;

                case (byte)Glo.IDs.StatusData:
                    GloObject<GloStatusData> statusData = new GloObject<GloStatusData>(Glo.IDs.StatusData);
                    statusData.data = GloLink.ByteArrayToStructure<GloStatusData>(e.Data);

                    view.TiltAngle = statusData.data.tiltAngle * 180.0 / Math.PI;

                    break;

                default:
                    logAndDisplayMessage("Received unhandled packet with ID: " + e.ID, MessageSource.UI);
                    break;
            }
        }

        public void OutputDirectoryChanged(string newDirectory)
        {
            if (Directory.Exists(newDirectory))
            {
                UpdateOutputPaths(newDirectory);
            }
            else 
            {
                logAndDisplayMessage(String.Format("Output directory does not exist: {0}", newDirectory), MessageSource.UI);
            }
        }

        public void openSessionDirectory()
        {
            Process.Start(sessionDirectory);
        }

        private void UpdateOutputPaths(string newBaseDirectory)
        { 
            // Default to current working directory if base directory isn't valid.
            if (String.IsNullOrWhiteSpace(newBaseDirectory))
            {
                newBaseDirectory = "./";
            }

            // Create sub directory at specified output base directory.
            currentOutputDirectory = Path.Combine(newBaseDirectory, rootOutputName);

            if (!Directory.Exists(currentOutputDirectory))
            {
                Directory.CreateDirectory(currentOutputDirectory);
            }

            // Create session directory at current output directory.
            sessionDirectory = Path.Combine(currentOutputDirectory, sessionDirectoryName);

            if (!Directory.Exists(sessionDirectory))
            {
                Directory.CreateDirectory(sessionDirectory);
            }

            logFile.UpdatePath(sessionDirectory);
        }

        private string makeFilePathUnique(string path)
        {
            string originalDirectory = Path.GetDirectoryName(path);
            string originalFileName = Path.GetFileNameWithoutExtension(path);
            string originalExtension = Path.GetExtension(path);
            int i = 1; // number to append to file name
            while (File.Exists(path))
            {
                string newFileName = String.Format("{0}_{1}", originalFileName, i);

                path = Path.Combine(originalDirectory, newFileName);
                path = Path.ChangeExtension(path, originalExtension);

                i++;
            }

            return path;
        }

        private void writeDataToFile()
        {
            if (captureData.Count < 1) 
            {
                logAndDisplayMessage("No capture data received", MessageSource.UI);
                return;
            }

            string fileName = view.DataCaptureFileName;

            bool generateFileName = view.GenerateFileName;

            if (!generateFileName && String.IsNullOrWhiteSpace(fileName))
            {
                logAndDisplayMessage("Generating file name since none was provided", MessageSource.UI);
                view.GenerateFileName = true;
                generateFileName = true;
            }

            if (generateFileName)
            {
                string dateTimeText = String.Format("{0:yyyy-MM-dd_hh-mm-ss-tt}", DateTime.Now);
                fileName = String.Format("data_{0}", dateTimeText);
                // update text box so user can see generated name
                view.DataCaptureFileName = Path.GetFileNameWithoutExtension(fileName);
            }

            string matlabFileName = Path.ChangeExtension(fileName, ".mat");
            string csvFileName = Path.ChangeExtension(fileName, ".csv");

            string matlabFilePath = Path.Combine(sessionDirectory, matlabFileName);
            string csvFilePath = Path.Combine(sessionDirectory, csvFileName);

            matlabFilePath = makeFilePathUnique(matlabFilePath);
            csvFilePath = makeFilePathUnique(csvFilePath);

            // The column names associated with each field.
            // **These names can't be spaces.  They will be the variable names in the matlab data file.
            string[] columnNames = new string[] { "Time", "TiltAngle" };

            // First write data out to comma-separated value (CSV) file.
            try
            {
                DataLogFile dataLogFile = new DataLogFile(csvFilePath);
                dataLogFile.Write(captureData, columnNames);
                logAndDisplayMessage("Created: " + Path.GetFileName(csvFilePath), MessageSource.UI);
            }
            catch (Exception e)
            {
                logAndDisplayMessage("Error creating CSV data file: \n" + e.Message, MessageSource.UI);
            }

            // Need to reference data by columns since each column element is the same field.
            float[][] columnedData = new float[captureData[0].Length][];
            for (int i = 0; i < columnedData.Length; ++i)
            {
                columnedData[i] = new float[captureData.Count];
            }

            // 'r'and 'c' = row and column index of original data matrix
            for (int r = 0; r < captureData.Count; ++r)
            {
                for (int c = 0; c < captureData[0].Length; ++c)
                {
                    columnedData[c][r] = (captureData[r])[c];
                }
            }

            try
            {
                MatlabDataFile mlDataFile = new MatlabDataFile(matlabFilePath);
                mlDataFile.Write(columnedData.ToList(), columnNames.ToList());
                logAndDisplayMessage("Created: " + Path.GetFileName(matlabFilePath), MessageSource.UI);
            }
            catch (Exception e)
            {
                logAndDisplayMessage("Error creating Matlab data file: \n" + e.Message, MessageSource.UI);
            }
        }

        private void logAndDisplayMessage(string message, MessageSource source)   
        {
            Color textColor = MessageSourceColor.Dict[source];

            string formattedMessage = String.Format("[{0}] {1}", DateTime.Now.ToString("HH:mm"), message);

            view.displayMessage(formattedMessage, textColor);

            // Protect against any log messages before the log is setup.  These messages are just setup messages anyways and shouldn't be critical.
            if (logFile != null)
            {
                try
                {
                    logFile.Write(formattedMessage);
                }
                catch (DirectoryNotFoundException)
                {
                    view.displayMessage("Log cannot be written. Output path invalid.", MessageSourceColor.Dict[MessageSource.UI]);
                }
            }
        }
    }
}
