﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace gf
{
    public enum RetCode
    {
        /// Method returns successfully.
        GF_SUCCESS,
        /// Method returns with a generic error.
        GF_ERROR,
        /// Given parameters are not match required.
        GF_ERROR_BAD_PARAM,
        /// Method call is not allowed by the inner state.
        GF_ERROR_BAD_STATE,
        /// Method is not supported at this time.
        GF_ERROR_NOT_SUPPORT,
        /// Hub is busying on device scan and cannot fulfill the call.
        GF_ERR_SCAN_BUSY,
        /// Insufficient resource to perform the call.
        GF_ERR_NO_RESOURCE,
        /// A preset timer is expired.
        GF_ERROR_TIMEOUT,
    };

    public sealed class Hub
    {
        public enum WorkMode
        {
            /// Callbacks are called in the message senders' threads
            /// \remark
            /// 1. Callbacks are called in various threads.
            /// 2. No need to call Hub::run, the method will return immediately in this mode.
            Freerun,
            /// Callbacks are called in the client thread given by method Hub::run
            /// \remark
            /// Client has to call Hub::run to pull messages (represent as callbacks),
            /// otherwise messages are blocking in an inner queue without been handled.
            Polling,
        };

        public enum HubState
        {
            Idle,
            Scanning,
            Connecting,
            Disconnected,
            Unknown
        };

        public enum LogLevel
        {
            GF_LOG_VERBOSE,
            GF_LOG_DEBUG,
            GF_LOG_INFO,
            GF_LOG_WARN,
            GF_LOG_ERROR,
            GF_LOG_FATAL,
            GF_LOG_MAX
        };

        public static Hub Instance
        {
            get
            {
                return instance;
            }
        }

        public delegate void logFn(LogLevel level, string value);
        public delegate bool deviceEnumFn(Device dev);

        public void setClientLogMethod(logFn fun)
        {
            libgforce.gf_set_log_method(fun);
        }

        public RetCode init(uint comport)
        {
            RetCode ret = libgforce.hub_init(comport);
            if (ret == RetCode.GF_SUCCESS)
            {
                // register listener
            }
            return RetCode.GF_SUCCESS;
        }

        public RetCode deinit()
        {
            return libgforce.hub_deinit();
        }

        public void setWorkMode(WorkMode newMode)
        {
            libgforce.hub_set_workmode(newMode);
        }

        public WorkMode getWorkMode()
        {
            return libgforce.hub_get_workmode();
        }

        public HubState getStatus()
        {
            return libgforce.hub_get_status();
        }

        public RetCode registerListener(HubListener ls)
        {
            RetCode ret = RetCode.GF_SUCCESS;
            if (mListeners.Add(ls))
            {
                if (mListeners.Count() == 1)
                {
                    mListenerDele = new libgforce.ListenerDelegate();
                    mListenerDele.onScanfinishedFn = new libgforce.onScanfinished(Hub.onScanfinishedImpl);
                    mListenerDele.onStateChangedFn = new libgforce.onStateChanged(Hub.onStateChangedImpl);
                    mListenerDele.onDeviceFoundFn = new libgforce.onDeviceFound(Hub.onDeviceFoundImpl);
                    mListenerDele.onDeviceDiscardFn = new libgforce.onDeviceDiscard(Hub.onDeviceDiscardImpl);
                    mListenerDele.onDeviceConnectedFn = new libgforce.onDeviceConnected(Hub.onDeviceConnectedImpl);
                    mListenerDele.onDeviceDisconnectedFn = new libgforce.onDeviceDisconnected(Hub.onDeviceDisconnectedImpl);
                    mListenerDele.onOrientationDataFn = new libgforce.onOrientationData(Hub.onOrientationDataImpl);
                    mListenerDele.onGestureDataFn = new libgforce.onGestureData(Hub.onGestureDataImpl);
                    mListenerDele.onReCenterFn = new libgforce.onReCenter(Hub.onReCenterImpl);

                    //int i = 0;
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onScanfinishedFn, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onStateChangedFn, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onDeviceFoundFn, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onDeviceDiscardFn, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onDeviceConnectedFn, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onDeviceDisconnectedFn, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onOrientationDataFn, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onGestureDataFn, GCHandleType.Pinned);
                    //mListenerDeleUnmanaged[i++] = GCHandle.Alloc(mListenerDele.onReCenterFn, GCHandleType.Pinned);

                    ret = libgforce.hub_register_listener(ref mListenerDele);
                }
            }
            else
            {
                // if already added, do nothing but return GF_SUCCESS


                // debug
                ret = libgforce.hub_register_listener(ref mListenerDele);
            }
            return ret;
        }

        public RetCode unregisterListener(HubListener ls)
        {
            RetCode ret = RetCode.GF_SUCCESS;
            if (mListeners.Remove(ls))
            {
                if (mListeners.Count() == 0)
                {
                    ret = libgforce.hub_unregister_listener(ref mListenerDele);
                    //if (null != mListenerDeleUnmanaged)
                    //{
                    //    foreach (var gch in mListenerDeleUnmanaged)
                    //        if (gch.IsAllocated)
                    //            gch.Free();
                    //}
                }
            }
            else
            {
                // if not exists
                ret = RetCode.GF_ERROR;
            }
            return ret;
        }

        public RetCode startScan()
        {
            return libgforce.hub_start_scan();
        }

        public RetCode stopScan()
        {
            return libgforce.hub_stop_scan();
        }

        public uint getNumDevices(bool bConnectedOnly)
        {
            return libgforce.hub_get_num_devices(bConnectedOnly);
        }

        public RetCode enumDevices(deviceEnumFn enumFn, bool bConnectedOnly)
        {
            if (null == enumFn)
                return RetCode.GF_ERROR_BAD_PARAM;
            mClientEnumFn = enumFn;
            libgforce.gfDeviceEnumFn gfEnumFn = new libgforce.gfDeviceEnumFn(onDeviceEnum);
            return libgforce.hub_enum_devices(gfEnumFn, bConnectedOnly);
        }

        public RetCode run(uint ms)
        {
            return libgforce.hub_run(ms);
        }

        private deviceEnumFn mClientEnumFn = null;
        private static readonly Hub instance = new Hub("c# app");

        static Hub()
        {
        }

        private Hub(string identifier)
        {
            libgforce.hub_instance(identifier);
        }

        private static bool onDeviceEnum(IntPtr hDevice)
        {
            if (IntPtr.Zero == hDevice)
                return false;
            if (null == instance.mClientEnumFn)
                return false;
            // try to add new device
            Device d = new Device(hDevice);
            try
            {
                instance.mDevices.Add(hDevice, d);
            }
            catch
            {
                // already exists
                d = (Device)instance.mDevices[hDevice];
            }
            return instance.mClientEnumFn(d);
        }

        private HashSet<HubListener> mListeners = new HashSet<HubListener>();
        private Hashtable mDevices = new Hashtable();

        private libgforce.ListenerDelegate mListenerDele;

        // 10 is the function numbers + the structure itself
        //GCHandle[] mListenerDeleUnmanaged = new GCHandle[10];
        private static void onScanfinishedImpl()
        {
            foreach (HubListener l in Instance.mListeners)
                l.onScanFinished();
        }
        private static void onStateChangedImpl(Hub.HubState state)
        {
            foreach (HubListener l in Instance.mListeners)
                l.onStateChanged(state);
        }
        private static void onDeviceFoundImpl(IntPtr hDevice)
        {
            if (IntPtr.Zero == hDevice)
                return;
            // try to add new device
            Device d = new Device(hDevice);
            try
            {
                instance.mDevices.Add(hDevice, d);
            }
            catch
            {
                // already exists
                d = (Device)instance.mDevices[hDevice];
            }
            foreach (HubListener l in instance.mListeners)
                l.onDeviceFound(d);
        }
        private static void onDeviceDiscardImpl(IntPtr hDevice)
        {
            Device d = null;
            try
            {
                d = (Device)instance.mDevices[hDevice];
            }
            catch
            {
                // cannot find
                return;
            }
            foreach (HubListener l in instance.mListeners)
                l.onDeviceDiscard(d);

            // remove data
            instance.mDevices.Remove(hDevice);
        }
        private static void onDeviceConnectedImpl(IntPtr hDevice)
        {
            Device d = null;
            try
            {
                d = (Device)instance.mDevices[hDevice];
            }
            catch
            {
                // cannot find
                return;
            }
            foreach (HubListener l in instance.mListeners)
                l.onDeviceConnected(d);
        }
        private static void onDeviceDisconnectedImpl(IntPtr hDevice, int reason)
        {
            Device d = null;
            try
            {
                d = (Device)instance.mDevices[hDevice];
            }
            catch
            {
                // cannot find
                return;
            }
            foreach (HubListener l in instance.mListeners)
                l.onDeviceDisconnected(d, reason);
        }
        private static void onOrientationDataImpl(IntPtr hDevice, float w, float x, float y, float z)
        {
            Device d = null;
            try
            {
                d = (Device)instance.mDevices[hDevice];
            }
            catch
            {
                // cannot find
                return;
            }
            foreach (HubListener l in instance.mListeners)
                l.onOrientationData(d, w, x, y, z);
        }
        private static void onGestureDataImpl(IntPtr hDevice, Device.Gesture gest)
        {
            Device d = null;
            try
            {
                d = (Device)instance.mDevices[hDevice];
            }
            catch
            {
                // cannot find
                return;
            }
            foreach (HubListener l in instance.mListeners)
                l.onGestureData(d, gest);
        }
        private static void onReCenterImpl(IntPtr hDevice)
        {
            Device d = null;
            try
            {
                d = (Device)instance.mDevices[hDevice];
            }
            catch
            {
                // cannot find
                return;
            }
            foreach (HubListener l in instance.mListeners)
                l.onReCenter(d);
        }
    }

}
