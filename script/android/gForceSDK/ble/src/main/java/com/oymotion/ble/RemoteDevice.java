/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
package com.oymotion.ble;

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
import java.util.Queue;
import java.util.LinkedList;

/*RemoteDevice is a abstract of Remote Device.*/
public class RemoteDevice {
    private final static String TAG = RemoteDevice.class.getSimpleName();
    private Context mContext;

    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothManager mBluetoothManager;
    private BluetoothDevice mDevice;
    private BluetoothGatt mBluetoothGatt;
    private List<BluetoothGattService> mGattservices = new ArrayList<BluetoothGattService>();
    private BluetoothGattCharacteristic mCommandCharacteristic = null;
	private BluetoothGattCharacteristic mControlCommandCharacteristic = null;
	private BluetoothGattCharacteristic mDataNotifyCharacteristic = null;
	private byte mProtocolType;
    private BLEService mBleService;

	private final static byte PROTOCOLTYPE_SIMPLEPROFILE = 0x00;
	private final static byte PROTOCOLTYPE_GFORCEDATAPROTOCOL = 0x01;
	private final static byte PROTOCOLTYPE_OADSERVICE = 0x02;
	private final static byte PROTOCOLTYPE_INVALID = (byte)0xFF;

    private String mAddress;
    private int mHandle = 0;
    private final static int INVALID_HANDLE = 0xFFFF;

    /*UUID TBD*/
    private final String gForce_Data_Protocol_UUID = "f000ffd0-0451-4000-b000-000000000000";
    private final String gForce_Control_Command_Characteristic_UUID = "f000ffe1-0451-4000-b000-000000000000";
    private final String gForce_Data_Notify_Characteristic_UUID = "f000ffe2-0451-4000-b000-000000000000";
	private final String gForce_OAD_Service_UUID = "f000ffc0-0451-4000-b000-000000000000";
	private final String gForce_Simple_Profile_UUID = "0000fff0-0000-1000-8000-00805f9b34fb";

    private int mConnectionState = BluetoothProfile.STATE_DISCONNECTED;

	private Queue<PendingGattOperation> mPendingGattOperations;
	private boolean mGattBusy;
	private final static byte GATT_CHARACTERISTIC_READ_REQUEST = 0x00;
	private final static byte GATT_CHARACTERISTIC_WRITE_REQUEST = 0x01;
	private final static byte GATT_DESCRIPTOR_READ_REQUEST = 0x02;
	private final static byte GATT_DESCRIPTOR_WRITE_REQUEST = 0x03;

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

		mProtocolType = PROTOCOLTYPE_INVALID;
		mGattBusy = false;
        mPendingGattOperations = new LinkedList<PendingGattOperation>();
    }

    public String getAddress() {
        return mAddress;
    }

    public int getDeviceState() {
		Log.i(TAG, "getDeviceState mConnectionState = " + mConnectionState);
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
        Log.d(TAG, "Trying connect to remote device:" + mAddress + "mConnectionState = " + mConnectionState);
        if (mBluetoothAdapter == null) {
            Log.w(TAG, "BluetoothAdapter not initialized.");
            return false;
        }

        if ((mAddress == null) || (mConnectionState != BluetoothProfile.STATE_DISCONNECTED)) {
            Log.w(TAG, "Remote device is in error mConnectionState = " + mConnectionState);
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
            	/*change the status to connecting before initial connect, 
            	* and if initial connect fail, change state back to disconnected.*/
            	mConnectionState = BluetoothProfile.STATE_CONNECTING;
                result = mBluetoothGatt.connect();

				/*result == false means re-connect fail, happen on below case.
	            * 1. connect remote device successful.
	            * 2. disable & enable bluetooth.
	            * 3. reconnect remote device.
	            * */
	            if (!result) {
	                mConnectionState = BluetoothProfile.STATE_DISCONNECTED;
					Log.i(TAG, "connect1 mConnectionState = " + mConnectionState);
	                return false;
	            } else {
					return true;
				}
            } catch (Exception e) {
                Log.w(TAG, "connect catch exception + " + e);
				mConnectionState = BluetoothProfile.STATE_DISCONNECTED;
                result = false;
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
        mConnectionState = BluetoothProfile.STATE_CONNECTING;
		Log.i(TAG, "connect2 mConnectionState = " + mConnectionState);
        mBluetoothGatt = mDevice.connectGatt(mContext, autoConnect, mGattCallback);
		
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
			mConnectionState = BluetoothProfile.STATE_DISCONNECTING;
			Log.i(TAG, "disconnect mConnectionState = " + mConnectionState);
        	mBluetoothGatt.disconnect();
		} else {
			Log.w(TAG, "Remote device is in error state, never connect before.");
			return false;
		}

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

	public boolean writeCharacteristic(byte[] data) {
        boolean result = false;
        String datatowrite = BLEScanner.getAddressStringFromByte(data);
        Log.w(TAG, "WriteCharacteristic with data:" + datatowrite);
        if (mBluetoothGatt != null && (mConnectionState == BluetoothProfile.STATE_CONNECTED)) {
            if (mCommandCharacteristic == null) {
                return false;
            }
            mCommandCharacteristic.setValue(data);
            result = mBluetoothGatt.writeCharacteristic(mCommandCharacteristic);
        } else {
            Log.w(TAG, "Remote device is in error state.");
            return false;
        }

        return result;
    }

	public byte getDeviceProtocolSupported()
	{
		if (mConnectionState == BluetoothProfile.STATE_DISCONNECTED) {
            Log.w(TAG, "Remote device is in error state = " + mConnectionState);
            return PROTOCOLTYPE_INVALID;
        }

		return mProtocolType;
	}

	public boolean sendControlCommand(byte[] data) {
        boolean result = false;
        Log.w(TAG, "sendControlCommand with data:" + data[0]);
        if (mBluetoothGatt != null && (mConnectionState == BluetoothProfile.STATE_CONNECTED)) {
            if (mControlCommandCharacteristic == null) {
				Log.w(TAG, "mControlCommandCharacteristic is null.");
                return false;
            } else {
				result = sendGattRequest(GATT_CHARACTERISTIC_WRITE_REQUEST, mControlCommandCharacteristic, null, data);
			}
        } else {
            Log.w(TAG, "Remote device is in error state.");
            return false;
        }

        return result;
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
					Log.e(TAG, "onConnectionStateChange1 mConnectionState = " + mConnectionState);
                }
                mConnectionState = BluetoothProfile.STATE_CONNECTED;
				Log.i(TAG, "onConnectionStateChange2 mConnectionState = " + mConnectionState);
				mHandle = mBleService.allocDeviceHandle();
                
                //Attempts to discover services after successful connection.
                /*This is an asynchronous operation. Once service discovery is completed,
                * the BluetoothGattCallback#onServicesDiscovered callback is
                * triggered. If the discovery was successful, the remote services can be
                * retrieved using the {getServices} function.*/
                Log.i(TAG, "Attempting to start service discovery");
                
                if ( false == mBluetoothGatt.discoverServices()) {
                    Log.e(TAG, "discovery service failed.");
                    /*todo... how to handle this case?*/
                }
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
            	Log.i(TAG, "Disconnected from GATT server.");
				
				mConnectionState = BluetoothProfile.STATE_DISCONNECTED;
				Log.w(TAG, "Device state = " + mConnectionState);
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
					checkProtocolType();
                }
            } else {
                Log.e(TAG, "onServicesDiscovered finished with error!");
            }

            Log.i(TAG, "Connected to GATT server.");
            /*notify upper level with device connected*/
            mBleService.onDeivceConnected(mAddress);
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
            if (data.length >= 4) {
                String Notify = String.format("%02X:%02X:%02X:%02X", data[0], data[1], data[2], data[3]);
                Log.i(TAG, "onCharacteristicChanged with data: " + Notify);
            }

            Log.i(TAG, "onCharacteristicChanged on characteristic UUID:  " + characteristic.getUuid().toString());

			/*data from control command characteristic, means it is control response.*/
			if (characteristic.getUuid().toString().equals(gForce_Control_Command_Characteristic_UUID)) {
                Log.i(TAG, "receive control response.");
				mBleService.onControlResponseReceived(mAddress, data);
			} else if (characteristic.getUuid().toString().equals(gForce_Data_Notify_Characteristic_UUID)) {
                Log.i(TAG, "receive data notify.");
				byte[] predata = {0x15, 0x27, 0x00};
				byte[] notification = new byte[predata.length + data.length];
				System.arraycopy(predata, 0, notification, 0, predata.length);
				System.arraycopy(data, 0, notification, predata.length, data.length);
				mBleService.onNotificationReceived(mAddress, notification);
			} else {
                Log.i(TAG, "receive notification.");
				byte[] predata = {0x15, 0x27, 0x00};
				byte[] notification = new byte[predata.length + data.length];
				System.arraycopy(predata, 0, notification, 0, predata.length);
				System.arraycopy(data, 0, notification, predata.length, data.length);
				mBleService.onNotificationReceived(mAddress, notification);
			}
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

		@Override
		/**
	     * Callback reporting the result of a characteristic read operation.
	     *
	     * @param gatt GATT client invoked {@link BluetoothGatt#readCharacteristic}
	     * @param characteristic Characteristic that was read from the associated
	     *                       remote device.
	     * @param status {@link BluetoothGatt#GATT_SUCCESS} if the read operation
	     *               was completed successfully.
	     */
	    public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic,
	                                     int status) {
	    	Log.d(TAG, "onCharacteristicRead status:" + status);
			mGattBusy = false;
			if (!mPendingGattOperations.isEmpty()) {
				dequeueGattRequest();
			}
	    }

		@Override
		/**
	     * Callback indicating the result of a characteristic write operation.
	     *
	     * <p>If this callback is invoked while a reliable write transaction is
	     * in progress, the value of the characteristic represents the value
	     * reported by the remote device. An application should compare this
	     * value to the desired value to be written. If the values don't match,
	     * the application must abort the reliable write transaction.
	     *
	     * @param gatt GATT client invoked {@link BluetoothGatt#writeCharacteristic}
	     * @param characteristic Characteristic that was written to the associated
	     *                       remote device.
	     * @param status The result of the write operation
	     *               {@link BluetoothGatt#GATT_SUCCESS} if the operation succeeds.
	     */
	    public void onCharacteristicWrite(BluetoothGatt gatt,
	                                      BluetoothGattCharacteristic characteristic, int status) {
	    	Log.d(TAG, "onCharacteristicWrite status:" + status);
			mGattBusy = false;
			if (!mPendingGattOperations.isEmpty()) {
				dequeueGattRequest();
			}
	    }

		@Override
		/**
	     * Callback reporting the result of a descriptor read operation.
	     *
	     * @param gatt GATT client invoked {@link BluetoothGatt#readDescriptor}
	     * @param descriptor Descriptor that was read from the associated
	     *                   remote device.
	     * @param status {@link BluetoothGatt#GATT_SUCCESS} if the read operation
	     *               was completed successfully
	     */
	    public void onDescriptorRead(BluetoothGatt gatt, BluetoothGattDescriptor descriptor,
	                                 int status) {
	    	Log.d(TAG, "onDescriptorRead status:" + status);
			mGattBusy = false;
			if (!mPendingGattOperations.isEmpty()) {
				dequeueGattRequest();
			}
	    }

		@Override
		/**
	     * Callback indicating the result of a descriptor write operation.
	     *
	     * @param gatt GATT client invoked {@link BluetoothGatt#writeDescriptor}
	     * @param descriptor Descriptor that was writte to the associated
	     *                   remote device.
	     * @param status The result of the write operation
	     *               {@link BluetoothGatt#GATT_SUCCESS} if the operation succeeds.
	     */
	    public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor,
	                                  int status) {
	    	Log.d(TAG, "onDescriptorWrite status:" + status);
			mGattBusy = false;
			if (!mPendingGattOperations.isEmpty()) {
				dequeueGattRequest();
			}
	    }

		@Override
		/**
	     * Callback invoked when a reliable write transaction has been completed.
	     *
	     * @param gatt GATT client invoked {@link BluetoothGatt#executeReliableWrite}
	     * @param status {@link BluetoothGatt#GATT_SUCCESS} if the reliable write
	     *               transaction was executed successfully
	     */
	    public void onReliableWriteCompleted(BluetoothGatt gatt, int status) {
	    	Log.d(TAG, "onReliableWriteCompleted status:" + status);
			mGattBusy = false;
			if (!mPendingGattOperations.isEmpty()) {
				dequeueGattRequest();
			}
	    }
    };

	private BluetoothGattCharacteristic findCharacteristicByUUID(BluetoothGattService service, String uuid) {
		Log.i(TAG, "findCharacteristicByUUID" + uuid);
		for (BluetoothGattCharacteristic gattCharacteristic : service.getCharacteristics()) {
			if (gattCharacteristic.getUuid().toString().equals(uuid)) {
				return gattCharacteristic;
			}
		}

		return null;
	}

	private void checkProtocolType() {
		Log.i(TAG, "checkProtocolType");
		for (BluetoothGattService service : mGattservices) {
			/*find gForce data protocol service by match UUID*/
			if (service.getUuid().toString().equals(gForce_Data_Protocol_UUID)) {
				mControlCommandCharacteristic = findCharacteristicByUUID(service, gForce_Control_Command_Characteristic_UUID);
				mDataNotifyCharacteristic = findCharacteristicByUUID(service, gForce_Data_Notify_Characteristic_UUID);

				if ((mControlCommandCharacteristic != null) && (mDataNotifyCharacteristic != null)) {
					mProtocolType = PROTOCOLTYPE_GFORCEDATAPROTOCOL;
					return;
				}
			} else if (service.getUuid().toString().equals(gForce_OAD_Service_UUID)) {
				mProtocolType = PROTOCOLTYPE_OADSERVICE;
				return;
			} else if (service.getUuid().toString().equals(gForce_Simple_Profile_UUID)) {
				mProtocolType = PROTOCOLTYPE_SIMPLEPROFILE;
				return;
			}
		}

		mProtocolType = PROTOCOLTYPE_INVALID;
	}

    private void printGattServices() {
        int i = 0;
        for (BluetoothGattService service:mGattservices) {
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
							sendGattRequest(GATT_DESCRIPTOR_WRITE_REQUEST, gattCharacteristic, des, BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
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

	public class PendingGattOperation {
		public byte mOpcode;
		public BluetoothGattCharacteristic mCharacteristic;
		public BluetoothGattDescriptor mDescriptor;
		public byte[] data;

		public PendingGattOperation(byte command, BluetoothGattCharacteristic cha, BluetoothGattDescriptor des, byte[] rapameter) {
			mOpcode = command;
			mCharacteristic = cha;
			mDescriptor = des;
			data = rapameter;
		}
	}

	private boolean enqueueGattRequest(byte command, BluetoothGattCharacteristic cha, BluetoothGattDescriptor des, byte[] parameter) {
		Log.e(TAG, "enqueueGattRequest");
		PendingGattOperation pending = new PendingGattOperation(command, cha, des, parameter);
		mPendingGattOperations.offer(pending);
		return true;
	}

	private boolean sendGattRequest(byte command, BluetoothGattCharacteristic cha, BluetoothGattDescriptor des, byte[] parameter)
	{
		Log.e(TAG, "sendGattRequest mGattBusy = " + mGattBusy);
		synchronized (mPendingGattOperations) {
			if (mGattBusy == false) {
				/*send the gatt request directly, and set mGattBusy to true.*/
				switch (command) {
					case GATT_CHARACTERISTIC_READ_REQUEST:
					{
						if (true == mBluetoothGatt.readCharacteristic(cha)){
							mGattBusy = true;
						} else {
							Log.e(TAG, "readCharacteristic fail");
							return false;
						}
						break;

					}
					case GATT_CHARACTERISTIC_WRITE_REQUEST:
					{
						cha.setValue(parameter);
						if (true == mBluetoothGatt.writeCharacteristic(cha)){
							mGattBusy = true;
						} else {
							Log.e(TAG, "GATT_CHARACTERISTIC_WRITE_REQUEST fail");
							return false;
						}
						break;
					}
					case GATT_DESCRIPTOR_READ_REQUEST:
					{
						if (true == mBluetoothGatt.readDescriptor(des)){
							mGattBusy = true;
						} else {
							Log.e(TAG, "GATT_DESCRIPTOR_READ_REQUEST fail");
							return false;
						}
						break;

					}
					case GATT_DESCRIPTOR_WRITE_REQUEST:
					{
						des.setValue(parameter);
						if (true == mBluetoothGatt.writeDescriptor(des)){
							mGattBusy = true;
						} else {
							Log.e(TAG, "GATT_DESCRIPTOR_WRITE_REQUEST fail");
							return false;
						}
						break;
					}

					default:
						return false;
				}
			}else {
				/*if gatt request is busy now, add this request to pending list.*/
				enqueueGattRequest(command, cha, des, parameter);
				return true;
			}
			
		}

		return true;
	}

	private boolean dequeueGattRequest() {
		Log.e(TAG, "dequeueGattRequest mGattBusy = " + mGattBusy);
		boolean result;
		synchronized (mPendingGattOperations) {
			PendingGattOperation pendingGattReqeust;
			if (null != (pendingGattReqeust = mPendingGattOperations.poll()) && (false == mGattBusy)) {
				result = sendGattRequest(pendingGattReqeust.mOpcode, pendingGattReqeust.mCharacteristic, pendingGattReqeust.mDescriptor, pendingGattReqeust.data);
			} else {
				Log.e(TAG, "No pending Gatt request.");
				return false;
			}
		}

		return result;
	}
}
