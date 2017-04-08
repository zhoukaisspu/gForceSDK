using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace gf
{
    class gForce
    {
        [DllImport("gforce", EntryPoint = "hub_init")]
        public static extern int hub_init(int comport);
        [DllImport("gforce", EntryPoint = "hub_deinit")]
        public static extern int hub_deinit();
        [DllImport("gforce", EntryPoint = "hub_register_listener")]
        public static extern int hub_register_listener();
        [DllImport("gforce", EntryPoint = "hub_unregister_listener")]
        public static extern int hub_unregister_listener();
        [DllImport("gforce", EntryPoint = "hub_start_scan")]
        public static extern int hub_start_scan();
        [DllImport("gforce", EntryPoint = "hub_stop_scan")]
        public static extern int hub_stop_scan();
        [DllImport("gforce", EntryPoint = "hub_get_num_devices")]
        public static extern int hub_get_num_devices();
        [DllImport("gforce", EntryPoint = "hub_get_num_devices_connected")]
        public static extern int hub_get_num_devices_connected();
        [DllImport("gforce", EntryPoint = "hub_enum_devices")]
        public static extern int hub_enum_devices();
        [DllImport("gforce", EntryPoint = "hub_set_workmode")]
        public static extern void hub_set_workmode();
        [DllImport("gforce", EntryPoint = "hub_get_workmode")]
        public static extern int hub_get_workmode();
        [DllImport("gforce", EntryPoint = "hub_run")]
        public static extern int hub_run();
    }

}
