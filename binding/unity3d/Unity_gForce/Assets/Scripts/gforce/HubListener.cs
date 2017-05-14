namespace gf
{
    public class HubListener
    {
        public virtual void onScanFinished() { }
        public virtual void onStateChanged(Hub.HubState state) { }
        public virtual void onDeviceFound(Device device) { }
        public virtual void onDeviceDiscard(Device device) { }
        public virtual void onDeviceConnected(Device device) { }
        public virtual void onDeviceDisconnected(Device device, int reason) { }
        public virtual void onOrientationData(Device device, float w, float x, float y, float z) { }
        public virtual void onGestureData(Device device, Device.Gesture gest) { }
        public virtual void onReCenter(Device device) { }
    }
}
