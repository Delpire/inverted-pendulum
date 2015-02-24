using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace EvaUI
{
    public partial class MainForm : Form, IMainView, IMessageFilter
    {
        MainController controller;

        Point tiltPanelP1 = new Point(0, 0);
        Point tiltPanelP2 = new Point(0, 0);
        Pen tiltPanelPen = new Pen(Color.Black, 2);
        Point tiltReferenceP2 = new Point(0, 0);
        Pen tiltReferencePen = new Pen(Color.Black, 1);

        bool keyRepeating = false;

        public MainForm()
        {
            InitializeComponent();
            outputDirectoryTextBox.Text = Properties.Settings.Default.OutputDirectory;

            tiltReferencePen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;

            Application.AddMessageFilter(this);
            this.FormClosed += (s, e) => Application.RemoveMessageFilter(this);
        }

        public void setController(MainController controller)
        {
            this.controller = controller;
        }

        public void displayMessage(string message, Color textColor)
        {
            if (messagesTextBox.InvokeRequired)
            {
                messagesTextBox.BeginInvoke(new Action<string, Color>(displayMessage), new object[] { message, textColor });
                return;
            }

            // An exception is sometimes thrown when something is being written to text box when form is exited.
            // this should protect against that.
            if (messagesTextBox.IsDisposed) { return; }

            messagesTextBox.AppendText(message, textColor);
            messagesTextBox.ScrollToCaret(); // autoscroll to show new text
        }

        public void popUpMessage(string message)
        {
            MessageBox.Show(message);
        }

        public void setDrivingModeButtonText(string text)
        {
            drivingModeEnableButton.Text = text;
        }

        public void setDrivingModeButtonColor(Color color)
        {
            drivingModeEnableButton.BackColor = color;
        }

        public void setPortList(string[] portNames)
        {
            // If user has an item selected then save off index so it can be restored after referesh.
            // Default to 0 (1st item) if no item is selected.
            int selectedIndex = portBox.SelectedIndex > 0 ? portBox.SelectedIndex : 0;

            portBox.Items.Clear();

            foreach (string name in portNames)
            {
                portBox.Items.Add(name);
            }

            // Make sure old selected index is still valid.
            selectedIndex = Math.Min(selectedIndex, portBox.Items.Count - 1);

            portBox.SelectedIndex = selectedIndex;
        }

        public bool selectPortName(string portName)
        {
            if (String.IsNullOrWhiteSpace(portName)) { return false; }

            bool containsName = portBox.Items.Contains(portName);

            if (containsName)
            {
                portBox.SelectedIndex = portBox.Items.IndexOf(portName);
            }

            return containsName;
        }

        public void setConnectButtonText(string newText)
        {
            this.connectButton.Text = newText;
        }

        public void setNumberTransferPackets(int numberPackets)
        {
            if (transferedPacketsTextBox.InvokeRequired)
            {
                transferedPacketsTextBox.BeginInvoke(new Action<int>(setNumberTransferPackets), new object[] { numberPackets });
                return;
            }

            this.transferedPacketsTextBox.Text = numberPackets.ToString();
        }

        public void setNumberReceivedPackets(int numberPackets)
        {
            if (receivedPacketsTextBox.InvokeRequired)
            {
                receivedPacketsTextBox.BeginInvoke(new Action<int>(setNumberReceivedPackets), new object[] { numberPackets });
                return;
            }

            receivedPacketsTextBox.Text = numberPackets.ToString();
        }

        public void setNumberTransferBytes(int numberBytes)
        {
            if (transferredBytesTextBox.InvokeRequired)
            {
                transferredBytesTextBox.BeginInvoke(new Action<int>(setNumberTransferBytes), new object[] { numberBytes });
                return;
            }

            this.transferredBytesTextBox.Text = numberBytes.ToString();
        }

        public void setNumberReceivedBytes(int numberBytes)
        {
            if (receivedBytesTextBox.InvokeRequired)
            {
                receivedBytesTextBox.BeginInvoke(new Action<int>(setNumberReceivedBytes), new object[] { numberBytes });
                return;
            }

            receivedBytesTextBox.Text = numberBytes.ToString();
        }

        public void setCaptureButtonText(string newText)
        {
            startCaptureButton.Text = newText;
        }

        public string getOutputDirectory()
        {
            return outputDirectoryTextBox.Text;
        }

        public bool GenerateFileName
        {
            get { return generateFileNameCheckBox.Checked; }
            set { generateFileNameCheckBox.Checked = value; }
        }

        public string DataCaptureFileName
        {
            get { return dataFileNameTextBox.Text; }
            set { dataFileNameTextBox.Text = value; }
        }

        public int CaptureSampleRate
        {
            get { return (int)captureRateTextBox.Value; }
            set { captureRateTextBox.Value = (decimal)value; }
        }

        public int CaptureDuration
        {
            get { return (int)captureDurationTextBox.Value; }
            set { captureDurationTextBox.Value = (decimal)value; }
        }

        public double TiltAngle
        {
            set 
            {
                if (tiltAngleTextBox.InvokeRequired)
                {
                    tiltAngleTextBox.BeginInvoke((MethodInvoker)delegate
                    {
                        tiltAngleTextBox.Text = String.Format("{0:00.00}", value);

                        // Invalidate panel to redraw line.
                        tiltPanel.Invalidate();
                    });
                    return;
                }

                tiltAngleTextBox.Text = String.Format("{0:00.00}", value);

                // Invalidate panel to redraw line.
                tiltPanel.Invalidate();
            }
        }

        private void refreshPortsButton_Click(object sender, EventArgs e)
        {
            controller.requestNewPortList();
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            object selectedItem = portBox.SelectedItem;

            string selectedPortName = String.Empty;
 
            if (selectedItem != null)
            {
                selectedPortName = portBox.GetItemText(selectedItem);
            }

            controller.changePortConnectionStatus(selectedPortName);
        }

        public bool PreFilterMessage(ref Message m)
        {
            if (m.Msg == 0x101)
            {
                keyRepeating = false;
            }
            return false;
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            bool justCheck = keyRepeating;
            keyRepeating = true;

            bool handled = controller.handleDrivingKey(keyData, justCheck);

            if (handled)
            {
                // This keystroke was handled, don't pass to the control with the focus
                return true;
            }
            else
            {
                // Pass on keystroke to control with focus.
                return base.ProcessCmdKey(ref msg, keyData);
            }
        }

        private void outputDirectoryBrowseButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog directoryBrowser = new FolderBrowserDialog();
            string currentDirectory = Properties.Settings.Default.OutputDirectory;
            directoryBrowser.SelectedPath = currentDirectory;

            if (DialogResult.OK == directoryBrowser.ShowDialog())
            {
                string newDirectory = directoryBrowser.SelectedPath;
                Properties.Settings.Default.OutputDirectory = newDirectory;
                Properties.Settings.Default.Save();
                outputDirectoryTextBox.Text = newDirectory;

                controller.OutputDirectoryChanged(outputDirectoryTextBox.Text);
            }
        }

        private void startCaptureButton_Click(object sender, EventArgs e)
        {
            controller.changeCaptureStatus();
        }

        private void openSessionDirectoryButton_Click(object sender, EventArgs e)
        {
            controller.openSessionDirectory();
        }

        private void clearMessagesButton_Click(object sender, EventArgs e)
        {
            messagesTextBox.Clear();
        }

        private void tiltImagePanel_Paint(object sender, PaintEventArgs e)
        {
            tiltPanelP1.X = tiltPanel.Width / 2;
            tiltPanelP1.Y = tiltPanel.Height;
            
            // Get the x and y components based on tilt angle
            double tiltRadians = double.Parse(tiltAngleTextBox.Text) * Math.PI / 180.0;
            int x = (int)(tiltPanel.Height * Math.Sin(tiltRadians));
            int y = (int)(tiltPanel.Height * Math.Cos(tiltRadians));

            // Account for the fact that 0 is the top part of the image and we want to center horizontally.
            tiltPanelP2.X = x + (tiltPanel.Width / 2);
            tiltPanelP2.Y = tiltPanel.Height - y; 

            // Reference point is at 0 degrees.
            tiltReferenceP2.X = tiltPanelP1.X;
            tiltReferenceP2.Y = 0;

            e.Graphics.DrawLine(tiltPanelPen, tiltPanelP1, tiltPanelP2);
            e.Graphics.DrawLine(tiltReferencePen, tiltPanelP1, tiltReferenceP2);
        }

        private void drivingModeEnableButton_Click(object sender, EventArgs e)
        {
            controller.changeDrivingMode();
        }
    }

    public static class RichTextBoxExtensions
    {
        public static void AppendText(this RichTextBox box, string text, Color color)
        {
            box.SelectionStart = box.TextLength;
            box.SelectionLength = 0;

            box.SelectionColor = color;

            if (box.Text.Length == 0)
            {
                box.Text = text;
            }
            else
            {
                box.AppendText("\r\n" + text);
            }

            box.SelectionColor = box.ForeColor;
        }
    }
}
