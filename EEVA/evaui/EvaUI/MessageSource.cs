using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace EvaUI
{
    public enum MessageSource
    { 
        UI,
        RobotDebug,
        RobotAssert,
    }

    public static class MessageSourceColor
    {
        public static Dictionary<MessageSource, Color> Dict = new Dictionary<MessageSource, Color>() 
        {
            { MessageSource.UI, Color.Black },
            { MessageSource.RobotDebug, Color.Blue },
            { MessageSource.RobotAssert, Color.Red },
        };
    }
}
