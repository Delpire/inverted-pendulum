using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;

namespace EvaUI
{
    class GloLink
    {
        private SerialPortConnection connection;

        private char[] messageFrame = new char[] { '<', '{', '(' };

        // Receive fields
        private int parseState;
        private byte objectID;
        private byte numberBodyBytes;
        private byte bodyIndex;
        private UInt16 instance;
        public int NumberPacketsReceived { get; private set; }
        public int NumberBytesReceived { get; private set; }
        private byte[] gloObjectData = new byte[255];
    
        // Transfer fields 
        public int NumberBytesSent { get; private set; }
        public int NumberPacketsSent { get; private set; }
        private byte[] transferBuffer = new byte[264];

        private BackgroundWorker parsingWorker;

        public event PacketReceivedEventHandler PacketReceived;

        public GloLink()
        {
        }

        void parsingWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                if (connectionOpen())
                {
                    // This blocks if nothing is in the queue
                    byte[] buffer = connection.ReceiveBuffer.Take();

                    parseData(buffer);
                }
            }
        }

        public void connect(string portName)
        {
            // If we still have a connection open then can't open a new one.
            if (connectionOpen())
            {
                throw new System.IO.IOException("Connection still open.");
            }          

            connection = new SerialPortConnection(parseData, portName);

            // This probably isn't the best way to handle this.  But the do work method blocks
            // and if the connection is dropped then it doesn't ever become unblocked 
            // (even if cancel is requested). Creating a new worker gets around that problem.
            parsingWorker = new BackgroundWorker();
            parsingWorker.DoWork += new DoWorkEventHandler(parsingWorker_DoWork);
            parsingWorker.RunWorkerAsync();

            connection.Open();
        }

        public bool disconnect()
        {
            if (connectionOpen())
            {
                connection.Dispose();
                return true;
            }

            return false;
        }

        public void send<T>(GloObject<T> obj) where T : struct
        {
            if (!connectionOpen())
            {
                return;
            }

            byte[] dataBytes = GloLink.StructureToByteArray(ref obj.data);

            transferBuffer[0] = Convert.ToByte(messageFrame[0]);
            transferBuffer[1] = Convert.ToByte(messageFrame[1]);
            transferBuffer[2] = Convert.ToByte(messageFrame[2]);
            transferBuffer[3] = obj.ID;
            transferBuffer[4] = Convert.ToByte((obj.instance<<8)>>8);
            transferBuffer[5] = Convert.ToByte(obj.instance >> 8);
            transferBuffer[6] = Convert.ToByte(dataBytes.Length);
            transferBuffer[dataBytes.Length + 7] = 0x0D; 
            transferBuffer[dataBytes.Length + 8] = 0x0A; //CR LF
 
            dataBytes.CopyTo(transferBuffer, 7);

            // how much space is actually used up for current packet
            int packetLength = dataBytes.Length + 9;

            connection.Write(transferBuffer, packetLength);

            NumberPacketsSent++;
            NumberBytesSent += packetLength;
        }

        public void parseData(byte[] dataBytes)
        { 
            // set to true if we received a new complete packet
            bool packetPending = false;

            NumberBytesReceived += dataBytes.Length;

            foreach (byte inByte in dataBytes)
            {
                switch (parseState)
                {
                    case -1:  // looking for first frame byte
                        if (inByte == messageFrame[0]) { advanceParse(); }
                        else { resetParse(); }
                        break;
                    case 0:  // looking for 2nd frame byte
                        if (inByte == messageFrame[1]) { advanceParse(); }
                        else { resetParse(); }
                        break;
                    case 1:  // looking for 3rd frame byte
                        if (inByte == messageFrame[2]) { advanceParse(); }
                        else { resetParse(); throw new Exception("Invalid frame: " + inByte.ToString()); }
                        break;
                    case 2:  // pulling out id
                        objectID = inByte;
                        advanceParse();
                        break;
                    case 3:  // pulling out byte one of instance
                        instance = inByte;
                        advanceParse();
                        break;
                    case 4:  // pulling out byte two of instance
                        instance += (ushort)(((ushort)inByte) << 8);
                        advanceParse();
                        break;
                    case 5:  // pulling out length of data
                        numberBodyBytes = inByte;
                        advanceParse();
                        break;
                    case 6:  // pulling out body
                        gloObjectData[bodyIndex] = inByte;
                        bodyIndex++;

                        // if this is last byte then advance parsing state
                        if (bodyIndex >= numberBodyBytes) { advanceParse(); }
                        break;
                    case 7: // looking for CR (0x0D) -- should add CRC check for serial
                        if (inByte == 0x0D) { advanceParse(); }
                        else { resetParse(); }
                        break;
                    case 8: // looking for LF (0x0A) 
                        if (inByte == 0x0A)
                        {
                            packetPending = true;
                        }

                        resetParse();

                        break;
                    default: // safety reset
                        resetParse();
                        break;
                }

                if (packetPending)
                {
                    packetPending = false;
                    NumberPacketsReceived++;
                    if (PacketReceived != null)
                    {
                        PacketReceivedEventArgs args = new PacketReceivedEventArgs();
                        args.ID = objectID;
                        args.Instance = instance;
                        args.Data = gloObjectData;
                        PacketReceived(this, args);
                    }
                }
            }
        }

        private void advanceParse()
        {
            parseState++;
        }

        private void resetParse()
        {
            bodyIndex = 0;
            parseState = -1;
        }

        private bool connectionOpen()
        {
            return (connection != null) && (!connection.Disposed);
        }

        public static byte[] StructureToByteArray<T>(ref T structData) where T : struct
        {
            int length = Marshal.SizeOf(structData);

            byte[] byteArray = new byte[length];

            IntPtr dataPointer = Marshal.AllocHGlobal(length);

            Marshal.StructureToPtr(structData, dataPointer, true);

            Marshal.Copy(dataPointer, byteArray, 0, length);

            Marshal.FreeHGlobal(dataPointer);

            return byteArray;
        }

        public static T ByteArrayToStructure<T>(byte[] bytes) where T : struct
        {
            GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            T structData = (T)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(T));
            handle.Free();
            return structData;
        }
    }

    public class PacketReceivedEventArgs : EventArgs
    {
        public byte ID { get; set; }
        public ushort Instance { get; set; }
        public byte[] Data { get; set; }
    }

    public delegate void PacketReceivedEventHandler(Object sender, PacketReceivedEventArgs e);
}
