using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace gf
{

    public class Device
    {
        public const int MAX_DEVICE_STR_LEN = 64;

        public enum Gesture
        {
            Relax = 0x00,
            Gist = 0x01,
            SpreadFingers = 0x02,
            WaveTowardPalm = 0x03,
            WaveBackwardPalm = 0x04,
            TuckFingers = 0x05,
            Shoot = 0x06,
            Undefined = 0xFF
        };

        public enum ConnectionStatus
        {
            Disconnected,
            Disconnecting,
            Connecting,
            Connected
        };
        public Device(IntPtr handle)
        {
            hD = handle;
        }

        public uint getAddrType()
        {
            return libgforce.device_get_addr_type(hD);
        }

        public RetCode getAddress(out string address)
        {
            StringBuilder str = new StringBuilder(MAX_DEVICE_STR_LEN);
            RetCode ret = libgforce.device_get_address(hD, str, (uint)str.Capacity);
            if (RetCode.GF_SUCCESS == ret)
            {
                address = str.ToString();
            }
            else
            {
                address = "";
            }
            return ret;
        }

        public RetCode getName(out string name)
        {
            StringBuilder str = new StringBuilder(MAX_DEVICE_STR_LEN);
            RetCode ret = libgforce.device_get_name(hD, str, (uint)str.Capacity);
            if (RetCode.GF_SUCCESS == ret)
            {
                name = str.ToString();
            }
            else
            {
                name = "";
            }
            return ret;
        }

        public ConnectionStatus getConnectionStatus()
        {
            return libgforce.device_get_connection_status(hD);
        }

        public RetCode connect()
        {
            return libgforce.device_connect(hD);
        }

        public RetCode disconnect()
        {
            return libgforce.device_disconnect(hD);
        }

        private IntPtr hD;
    }
}
