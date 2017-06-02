package com.oymotion.libble;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.os.RemoteException;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

/*RemoteDevice is a abstract of Remote Device.*/
public class RemoteDevice {
    private final static String TAG = RemoteDevice.class.getSimpleName();
    private Context mContext;

    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothManager mBluetoothManager;
    private BluetoothDevice mDevice;
    private BluetoothGatt mBluetoothGatt;
    private List<BluetoothGattService> mGattservices = new ArrayList<BluetoothGattService>();
    private BLEService mBleService;

    private String mAddress;
    private int mHandle = 0;
    private final static int INVALID_HANDLE = 0xFFFF;

    private int mConnectionState = BluetoothProfile.STATE_DISCONNECTED;

    public RemoteDevice(String address, BLEService service) {
        mBleService = service;
        mAddress = address;
        mConnectionState = BluetoothProfile.STATE_DISCONNECTED;
        mContext = GlobalContext.getApplicationContext();
        mBluetoothManager = (BluetoothManager) mContext.getSystemService(Context.BLUETOOTH_SERVICE);
        if (mBluetoothManager == null) {
            Log.e(TAG, "Unable to initialize BluetoothManager.");
        } else {
            Log.e(TAG, "initialize BluetoothManager successful.");
            mBluetoothAdapter = mBluetoothManager.getAdapter();
            if (mBluetoothAdapter == null) {
                Log.e(TAG, "Unable to initialize mBluetoothAdapter.");
            } else {
                Log.e(TAG, "initialize BluetoothAdapter successful.");
            }
        }
    }

    public String getAddress() {
        return mAddress;
    }

    public int getDeviceState() {
        return mConnectionState;
    }

    public void setHandle(int handle) {
        mHandle = handle;
    }

    public int getHandle() {
        return mHandle;
    }

    public boolean connect(boolean autoConnect) {
        boolean result;
        Log.d(TAG, "Trying connect to remote device:" + mAddress);
        if (mBluetoothAdapter == null) {
            Log.w(TAG, "BluetoothAdapter not initialized.");
            return false;
        }

        if (mAddress == null || mConnectionState != BluetoothProfile.STATE_DISCONNECTED) {
            Log.w(TAG, "Remote device is in error state = " + mConnectionState);
            return false;
        }

        if (mBluetoothGatt != null) {
            /**
             * Connect back to remote device.
             *
             * <This method is used to re-connect to a remote device after the
             * connection has been dropped. If the device is not in range, the
             * re-connection will be triggered once the device is back in range.
             */
            try {
                result = mBluetoothGatt.connect();
            } catch (Exception e) {
                Log.w(TAG, "connect catch exception + " + e);
                result = false;
            }

            /*result == false means re-connect fail, happen on below case.
            * 1. connect remote device successful.
            * 2. disable & enable bluetooth.
            * 3. reconnect remote device.
            * */
            if (result) {
                mConnectionState = BluetoothProfile.STATE_CONNECTING;
                return true;
            }
        }

        mDevice = mBluetoothAdapter.getRemoteDevice(mAddress);
        if (mDevice == null) {
            Log.w(TAG, "Device not found.  Unable to connect.");
            return false;
        }
        /*autoConnect indicate Whether to directly connect to the remote device (false)
         *or to automatically connect as soon as the remote device becomes available (true).
         * */
        mBluetoothGatt = mDevice.connectGatt(mContext, autoConnect, mGattCallback);
        mConnectionState = BluetoothProfile.STATE_CONNECTING;

        return true;
    }

    public boolean disconnect() {
        Log.d(TAG, "Trying disconnect to remote device:" + mAddress);
        if (mBluetoothAdapter == null) {
            Log.w(TAG, "BluetoothAdapter not initialized.");
            return false;
        }

        if (mAddress == null || mConnectionState == BluetoothProfile.STATE_DISCONNECTED) {
            Log.w(TAG, "Remote device is in error state = " + mConnectionState);
            return false;
        }

		if (mBluetoothGatt != null) {
        	mBluetoothGatt.disconnect();
		} else {
			Log.w(TAG, "Remote device is in error state, never connect before.");
		}
		
        mConnectionState = BluetoothProfile.STATE_DISCONNECTING;

        return true;
    }

	public boolean exchangeMtuSize(int mtu) {
		if (mBluetoothGatt != null && (mConnectionState == BluetoothProfile.STATE_CONNECTED)) {
			mBluetoothGatt.requestMtu(mtu);
		} else {
			Log.w(TAG, "Remote device is in error state.");
            return false;
		}

		return true;
	}

    /* connection state change and services discovery callback.*/
    private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
        @Override
        // Callback indicating when GATT client has connected/disconnected to/from a remote GATT server.
        /*
        **gatt	GATT client
        **status	Status of the connect or disconnect operation. GATT_SUCCESS if the operation succeeds.
        **newState	Returns the new connection state. Can be one of STATE_DISCONNECTED or STATE_CONNECTED
         */
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            Log.w(TAG, "onConnectionStateChange with status:" + status);

            if (newState == BluetoothProfile.STATE_CONNECTED && status == BluetoothGatt.GATT_SUCCESS) {
                if (mConnectionState != BluetoothProfile.STATE_CONNECTING) {
                    Log.i(TAG, "ConnectState changed unexpected!!");
                }
                mConnectionState = BluetoothProfile.STATE_CONNECTED;
				mHandle = mBleService.allocDeviceHandle();

				Log.i(TAG, "Connected to GATT server.");
				/*notify upper level with device connected*/
				mBleService.onDeivceConnected(mAddress);
                
                //Attempts to discover services after successful connection.
                /*This is an asynchronous operation. Once service discovery is completed,
                * the BluetoothGattCallback#onServicesDiscovered callback is
                * triggered. If the discovery was successful, the remote services can be
                * retrieved using the {getServices} function.*/
                Log.i(TAG, "Attempting to start service discovery");
                
                if ( false == mBluetoothGatt.discoverServices()) {
                    Log.e(TAG, "discovery service failed.");
                }
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
            	Log.i(TAG, "Disconnected from GATT server.");
				mConnectionState = BluetoothProfile.STATE_DISCONNECTED;
				mBleService.onDeivceDisconnected(mAddress);
				mBleService.freeDeviceHandle(mHandle);
                mHandle = INVALID_HANDLE;
                Log.i(TAG, "mHandle = " + mHandle + " of " + mAddress);
            }
    }

        @Override
        // Callback invoked when the list of remote services, characteristics and descriptors for the remote device have been updated,
        // ie new services have been discovered.
        /*
        **gatt	GATT client invoked discoverServices()
        **status	GATT_SUCCESS if the remote device has been explored successfully.
        **/
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            Log.i(TAG, "onServicesDiscovered finished with status: " + status);
            if (status == BluetoothGatt.GATT_SUCCESS && gatt.equals(mBluetoothGatt)) {
                Log.i(TAG, "onServicesDiscovered finished successful!");

                /**
                 * Retrieves a list of supported GATT services on the connected device. This should be
                 * invoked only after {BluetoothGatt#discoverServices()} completes successfully.
                 *
                 * @return A {@code List} of supported services.
                 */
                mGattservices = mBluetoothGatt.getServices();
                if (mGattservices == null) {
                    Log.e(TAG, "Retrieve gatt service failed!");
                } else {
                    printGattServices();
                }
            } else {
                Log.e(TAG, "onServicesDiscovered finished with error!");
            }
        }

        @Override
        // Callback triggered as a result of a remote characteristic notification.
        /*
        **gatt	GATT client the characteristic is associated with
        **characteristic	Characteristic that has been updated as a result of a remote notification event.
        */
        public void onCharacteristicChanged(BluetoothGatt gatt,
                                            BluetoothGattCharacteristic characteristic) {
            byte[] data = characteristic.getValue();
            String Notify = String.format("%02X:%02X:%02X:%02X:%02X:%02X",
                    data[0], data[1], data[2], data[3], data[4], data[5]);

            Log.d(TAG, "onNotification received:" + Notify);
			mBleService.onNotificationReceived(mAddress, data);
        }

		@Override
		/**
	     * Callback indicating the MTU for a given device connection has changed.
	     *
	     * This callback is triggered in response to the
	     * {@link BluetoothGatt#requestMtu} function, or in response to a connection
	     * event.
	     *
	     * @param gatt GATT client invoked {@link BluetoothGatt#requestMtu}
	     * @param mtu The new MTU size
	     * @param status {@link BluetoothGatt#GATT_SUCCESS} if the MTU has been changed successfully
	     */
	    public void onMtuChanged(BluetoothGatt gatt, int mtu, int status) {
	    	Log.d(TAG, "onMtuChanged mtu:" + mtu);
	    	mBleService.onMTUSizeChanged(mAddress, mtu);
	    }
    };

    private void printGattServices() {
        int i = 0;
        for (BluetoothGattService service:mGattservices){
            Log.i(TAG, i + " th service:");
            Log.i(TAG, "    the uuid is :" + service.getUuid().toString());
            if (service.getType() == 0){
                Log.i(TAG, "    Primary Service:" + "th instance is:" + service.getInstanceId());
            } else {
                Log.i(TAG, "    Secondary Service:" + "th instance is:" + service.getInstanceId());
            }

            List<BluetoothGattCharacteristic> gattCharacteristics = service.getCharacteristics();
            Log.i(TAG, " size of characteristics is " + gattCharacteristics.size());
            // Loops through available Characteristics.
            int j = 0;
            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                Log.i(TAG, j + " th characteristics:");
                Log.i(TAG, "    the uuid is :" + gattCharacteristic.getUuid().toString());
                Log.i(TAG, "    the instance is:" + gattCharacteristic.getInstanceId());
                Log.i(TAG, "    the property is:" + gattCharacteristic.getProperties());
                Log.i(TAG, "    the permissions is:" + gattCharacteristic.getPermissions());
                //String value = new String(gattCharacteristic.getValue());
                //Log.i(TAG, "    the value is:" + value);

                List<BluetoothGattDescriptor> descriptors = gattCharacteristic.getDescriptors();
                Log.i(TAG, "    the size of descriptors is :" + descriptors.size());
                for(BluetoothGattDescriptor des : descriptors) {
                    Log.i(TAG, "    the descriptors uuid is :" + des.getUuid().toString());
                    String CCC_uuid = "00002902-0000-1000-8000-00805f9b34fb";
                    if (des.getUuid().toString().equals(CCC_uuid)) {
                        if ((gattCharacteristic.getProperties() | BluetoothGattCharacteristic.PROPERTY_NOTIFY) > 0) {
                            Log.i(TAG, " enable ccc...  ");
                            mBluetoothGatt.setCharacteristicNotification(gattCharacteristic, true);
                            des.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
                            mBluetoothGatt.writeDescriptor(des);
                        }
                    }
                }
                j++;
            }

            List<BluetoothGattService> includedservices = service.getIncludedServices();
            Log.i(TAG, " size of IncludedServices is " + includedservices.size());
            if (includedservices.size() != 0) {
                int k;
                Log.i(TAG, i + " th included service:");
                for (BluetoothGattService includeser : includedservices) {
                    Log.i(TAG, "    the uuid is :" + includeser.getUuid().toString());
                    if (includeser.getType() == 0){
                        Log.i(TAG, "    Primary Service:" + "th instance is:" + includeser.getInstanceId());
                    } else {
                        Log.i(TAG, "    Secondary Service:" + "th instance is:" + includeser.getInstanceId());
                    }
                }
            }
            i++;
        }
    }
}
