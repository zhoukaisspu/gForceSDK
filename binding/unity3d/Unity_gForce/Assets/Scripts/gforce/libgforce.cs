using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace gf
{
    internal static class libgforce
    {
        // listener callbacks
        public delegate void onScanfinished();
        public delegate void onStateChanged(Hub.HubState state);
        public delegate void onDeviceFound(System.IntPtr hDevice);
        public delegate void onDeviceDiscard(System.IntPtr hDevice);
        public delegate void onDeviceConnected(System.IntPtr hDevice);
        public delegate void onDeviceDisconnected(System.IntPtr hDevice, int reason);
        public delegate void onOrientationData(System.IntPtr hDevice, float w, float x, float y, float z);
        public delegate void onGestureData(System.IntPtr hDevice, Device.Gesture gest);
        public delegate void onReCenter(System.IntPtr hDevice);

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
        public struct ListenerDelegate
        {
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onScanfinished onScanfinishedFn;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onStateChanged onStateChangedFn;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onDeviceFound onDeviceFoundFn;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onDeviceDiscard onDeviceDiscardFn;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onDeviceConnected onDeviceConnectedFn;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onDeviceDisconnected onDeviceDisconnectedFn;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onOrientationData onOrientationDataFn;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onGestureData onGestureDataFn;
            [MarshalAs(UnmanagedType.FunctionPtr)]
            public onReCenter onReCenterFn;
        }

        [DllImport("gforce", EntryPoint = "gf_set_log_method")]
        public static extern void gf_set_log_method(Hub.logFn lf);
        [DllImport("gforce", EntryPoint = "hub_instance")]
        public static extern RetCode hub_instance(string identifier);
        [DllImport("gforce", EntryPoint = "hub_init")]
        public static extern RetCode hub_init(uint comport);
        [DllImport("gforce", EntryPoint = "hub_deinit")]
        public static extern RetCode hub_deinit();
        [DllImport("gforce", EntryPoint = "hub_set_workmode")]
        public static extern void hub_set_workmode(Hub.WorkMode wm);
        [DllImport("gforce", EntryPoint = "hub_get_workmode")]
        public static extern Hub.WorkMode hub_get_workmode();
        [DllImport("gforce", EntryPoint = "hub_get_status")]
        public static extern Hub.HubState hub_get_status();
        [DllImport("gforce", EntryPoint = "hub_register_listener")]
        public static extern RetCode hub_register_listener(ref ListenerDelegate listener);
        [DllImport("gforce", EntryPoint = "hub_unregister_listener")]
        public static extern RetCode hub_unregister_listener(ref ListenerDelegate listener);
        [DllImport("gforce", EntryPoint = "hub_start_scan")]
        public static extern RetCode hub_start_scan();
        [DllImport("gforce", EntryPoint = "hub_stop_scan")]
        public static extern RetCode hub_stop_scan();
        [DllImport("gforce", EntryPoint = "hub_get_num_devices")]
        public static extern uint hub_get_num_devices(bool bConnectedOnly);
        public delegate bool gfDeviceEnumFn(IntPtr hDevice);
        [DllImport("gforce", EntryPoint = "hub_enum_devices")]
        public static extern RetCode hub_enum_devices(gfDeviceEnumFn enumFn, bool bConnectedOnly);
        [DllImport("gforce", EntryPoint = "hub_run")]
        public static extern RetCode hub_run(uint ms);

        // Device methods
        [DllImport("gforce", EntryPoint = "device_get_addr_type")]
        public static extern uint device_get_addr_type(IntPtr hDevice);
        [DllImport("gforce", EntryPoint = "device_get_address")]
        public static extern RetCode device_get_address(IntPtr hDevice, [Out, MarshalAs(UnmanagedType.LPStr)]StringBuilder addr, uint buflen);
        [DllImport("gforce", EntryPoint = "device_get_name")]
        public static extern RetCode device_get_name(IntPtr hDevice, [Out, MarshalAs(UnmanagedType.LPStr)]StringBuilder name, uint buflen);
        [DllImport("gforce", EntryPoint = "device_get_connection_status")]
        public static extern Device.ConnectionStatus device_get_connection_status(IntPtr hDevice);
        [DllImport("gforce", EntryPoint = "device_connect")]
        public static extern RetCode device_connect(IntPtr hDevice);
        [DllImport("gforce", EntryPoint = "device_disconnect")]
        public static extern RetCode device_disconnect(IntPtr hDevice);
        /*
         */
    }

}
