using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace EvaUI
{
    static class Glo
    {
        public enum IDs
        {
            MotionCommands,
            RawAccels,
            RawAnalog,
            GyrosAccels,
            RollPitchYaw,
            Quaternion,
            ThetaZero,
            Odometry,
            RollPidParams,
            PitchPidParams,
            Modes,
            FlashParams,
            AssertMessage,
            DebugMessage,
            CaptureData,
            DrivingCommand,
            CaptureCommand,
            StatusData,        
        };
    }
    
    class GloObjectBase
    {
        public byte ID;
        public ushort instance;
    }

    class GloObject<T> : GloObjectBase
    {
        public T data;

        public GloObject(byte ID)
        {
            this.ID = ID;
        }

        public GloObject(Glo.IDs ID)
        {
            this.ID = (byte)ID;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    struct GloAssertMessage
    {
        public UInt32 type;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 200)]
        public string text;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct GloDebugMessage
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 200)]
        public string text;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct GloCaptureData
    {
        public double time; // Seconds
        public float tiltAngle; // Radians
    }

    [StructLayout(LayoutKind.Sequential)]
    struct GloDrivingCommand
    {
        public UInt32 movementType;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct GloCaptureCommand
    {
        public byte isStart; // false (0) if should stop sending data
        public byte pad0;
        public UInt16 frequency; // Hz
    }

    [StructLayout(LayoutKind.Sequential)]
    struct GloStatusData
    {
        public float tiltAngle; // Radians
    }
}
