using System;
using System.IO.Ports;
using System.Text;
using System.Threading;
using System.Collections.Concurrent;

namespace EvaUI
{
    public sealed class SerialPortConnection : IDisposable
    {
        public delegate void DataReceivedDelegate(byte[] dataBytes);

        DataReceivedDelegate dataReceivedDelegate;

        private readonly SerialPort serialPort;

        private BlockingCollection<byte[]> receiveBuffer = new BlockingCollection<byte[]>();
        public BlockingCollection<byte[]> ReceiveBuffer { get { return receiveBuffer; } }

        public bool Disposed { get; private set; }

        public SerialPortConnection(
            DataReceivedDelegate dataReceivedDelegate,
            string comPort = "Com1",
            int baud = 115200, 
            Parity parity = Parity.None,
            int dataBits = 8,
            StopBits stopBits = StopBits.One,
            int receiveBufferSize = 10000)
        {
            Disposed = false;

            this.dataReceivedDelegate = dataReceivedDelegate;

            this.serialPort = new SerialPort(comPort, baud, parity, dataBits, stopBits)
            {
                ReadTimeout = 1000,
                WriteTimeout = 1000,
            };

            this.serialPort.DataReceived += new SerialDataReceivedEventHandler(serialPort_DataReceived);
        }

        public void Open()
        {
            if (Disposed)
            {
                throw new ObjectDisposedException(this.GetType().Name, "Cannot use a disposed object.");
            }

            serialPort.Open();
            serialPort.DiscardInBuffer();
        }

        public void Write(byte[] buffer, int length)
        {
            serialPort.Write(buffer, 0, length);
        }

        public void Dispose()
        {
            if (Disposed)
            {
                throw new ObjectDisposedException(this.GetType().Name, "Cannot use a disposed object.");
            }

            this.serialPort.DataReceived -= new SerialDataReceivedEventHandler(serialPort_DataReceived);

            if (serialPort.IsOpen)
            {
                this.serialPort.DiscardInBuffer();
                this.serialPort.Close();
                this.serialPort.Dispose();
            }

            Disposed = true;
        }

        void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            // Initialize a buffer to hold the received data 
            byte[] buffer = new byte[serialPort.BytesToRead]; 
 
            serialPort.Read(buffer, 0, buffer.Length);

            receiveBuffer.Add(buffer);
        }
    }
}