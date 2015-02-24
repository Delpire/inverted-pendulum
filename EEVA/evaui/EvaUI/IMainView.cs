using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace EvaUI
{
    public interface IMainView
    {
        void setController(MainController controller);

        void displayMessage(string message, Color textColor);

        void popUpMessage(string message);

        void setPortList(string[] portNames);

        bool selectPortName(string portName);

        void setConnectButtonText(string text);

        void setNumberTransferPackets(int numberPackets);

        void setNumberReceivedPackets(int numberPackets);

        void setNumberReceivedBytes(int numberBytes);

        void setNumberTransferBytes(int numberBytes);

        void setCaptureButtonText(string text);

        void setDrivingModeButtonText(string text);

        void setDrivingModeButtonColor(Color color);

        string getOutputDirectory();

        bool GenerateFileName { get; set; }

        string DataCaptureFileName { get; set; }

        int CaptureSampleRate { get; set; }

        int CaptureDuration { get; set; }

        double TiltAngle { set; }
    }
}
