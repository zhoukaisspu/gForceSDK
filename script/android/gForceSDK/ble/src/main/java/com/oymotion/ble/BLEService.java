package com.oymotion.ble;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanRecord;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.util.Log;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import android.os.ParcelUuid;
import android.util.SparseArray;

import android.util.Log;

public class BLEService {
    private final static String TAG = BLEService.class.getSimpleName();
    private static BLEService mSingletonBleService;
	/*used to store all device that we try to connect*/
    private ArrayList<RemoteDevice> mRemoteDevices = new ArrayList<RemoteDevice>();
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothManager mBluetoothManager;
    private Context mContext;
    private BLEScanner mBLEScanner;
	private static final int MAX_DEVICE = 8;
	private boolean[] mHanleArray = new boolean[MAX_DEVICE];
	/*test flag for android emulator*/
    private static int TEST = 0;
    private static int handle = 0;

	/*Reveive bluetooth adapter state change broadcast.*/
    private IntentFilter mIntentFilter;
    private int mBluetoothState;

	/*lock*/
	private Object mObject = new Object();

    private BLEService() {
        Log.d(TAG, "BLEService constructor...");
        mContext = GlobalContext.getApplicationContext();
		if (mContext == null) {
			Log.e(TAG, "GlobalContext is not available!");
			return;
		}
		
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
                mBLEScanner = new BLEScanner(this);
                mIntentFilter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
                mContext.registerReceiver(mReceiver, mIntentFilter);
                mBluetoothState = BluetoothAdapter.STATE_OFF;
            }
        }

        initialDeviceHandle();
    }

    /*used to monitor Bluetooth Adapter state.*/
    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR);
            Log.d(TAG, "Bluetooth state:" + mBluetoothState + "--->" + state);
            /*Bluetooth has been disabled.*/
            if (state == BluetoothAdapter.STATE_OFF && mBluetoothState != BluetoothAdapter.STATE_OFF) {
                //onBluetoothStateOFF();
                initialDeviceHandle();
                //resetDevices();
        	}

        	mBluetoothState = state;
    	}
    };

    public static BLEService GetInstance() {
        Log.d(TAG, "GetInstance");
        synchronized (BLEService.class) {
			if (null == mSingletonBleService) {
            	mSingletonBleService = new BLEService();
        	}
		}
        return mSingletonBleService;
    }

    public boolean startScan() {
        Log.d(TAG, "startScan...");

        if ( false == checkBluetoothState()) {
            Log.e(TAG, "turn on bluetooth fail!.");
            return false;
        }

        if(mBluetoothAdapter == null || mBLEScanner == null)
        {
            Log.e(TAG, "mBluetoothAdapter is not available!.");
            if (TEST == 1) {
                new Handler().postDelayed(new Runnable(){
                    public void run() {
                        byte[] deviceFound= {0x1A, 0x22, 0x34, 0x45, 0x56, 0x67, 0x58, 0x39, 0x11};
                        onDeviceFoundNative(deviceFound);
                    }
                }, 2000);

                new Handler().postDelayed(new Runnable(){
                    public void run() {
                        byte[] deviceFound= {0x18, 0x72, 0x34, 0x45, 0x46, 0x67, 0x66, 0x22, 0x11};
                        onDeviceFoundNative(deviceFound);
                    }
                }, 3000);

                new Handler().postDelayed(new Runnable(){
                    public void run() {
                        onScanFinishedNative();
                    }
                }, 5000);

                return false;
            }
        }

        /*double check Bluetooth is on.*/
        if (!mBluetoothAdapter.isEnabled())
        {
            Log.e(TAG, "bluetooth isn't enabled. Please open it first.");
            return false;
        }

        if (mBLEScanner != null) {
            return mBLEScanner.startScan();
        }
		
        return false;
    }

    public boolean stopScan() {
        Log.d(TAG, "stopScan...");

        if(mBluetoothAdapter == null)
        {
            Log.e(TAG, "mBluetoothAdapter is not available!.");
            return false;
        }

        /*
        if (!mBluetoothAdapter.isEnabled())
        {
            Log.e(TAG, "bluetooth isn't enabled. Please open it first.");
            return false;
        }*/

        if (mBLEScanner != null) {
            return mBLEScanner.stopScan();
        } else {
            Log.e(TAG, "mBLEScanner is not available!.");
            new Handler().postDelayed(new Runnable(){
                public void run() {
                    onScanFinishedNative();
                }
            }, 2000);
            return false;
        }
    }

    public boolean connectDevice(byte[] addr, boolean autoConnect) {
        final String address = BLEScanner.getAddressStringFromByte(addr);
        Log.d(TAG, "connectDevice:" + address);
		if (TEST == 1) {

            new Handler().postDelayed(new Runnable() {
                public void run() {
                    byte[] deviceFound = {0x1A, 0x22, 0x34, 0x45, 0x56, 0x67, 0x58, 0x39, 0x11};
                    onDeviceConnectedNative(BLEScanner.getBytesFromAddress(address), handle);
                }
            }, 2000);

            handle++;

            return false;
        }

        RemoteDevice remoteDevice = findRemoteDeviceByAddress(address);
		if (remoteDevice == null) {
			/*the first time to connect that device*/
			remoteDevice = new RemoteDevice(address, this);
			if (remoteDevice == null) {
				return false;
			}
			synchronized (mRemoteDevices) {
				mRemoteDevices.add(remoteDevice);
			}
		}

		return remoteDevice.connect(autoConnect);
    }

    public boolean cancelConnect(byte[] addr) {
        String address = BLEScanner.getAddressStringFromByte(addr);
        Log.d(TAG, "cancelConnect:" + address);

        RemoteDevice remoteDevice = findRemoteDeviceByAddress(address);
        if (remoteDevice != null) {
            return remoteDevice.disconnect();
        }
        else {
            Log.d(TAG, address  + "not in connecting state!");
        }

        return false;
    }

    public boolean disconnect(final int handle) {
        Log.d(TAG, "disconnect:" + handle);
		
		if (TEST == 1) {
            new Handler().postDelayed(new Runnable() {
                public void run() {
                    byte[] address = {0x1A, 0x22, 0x34, 0x45, 0x56, 0x67};
                    onDeviceDisconnectedNative(address, handle, (byte)0x16);
                }
            }, 2000);

            return false;
        }

        RemoteDevice remoteDevice = findRemoteDeviceByHandle(handle);
        if (remoteDevice != null) {
            return remoteDevice.disconnect();
        }
        else {
            Log.d(TAG, "connection with handle =  " + handle + " not in connected state!");
        }

        return false;
    }

    public boolean configMTUSize(final int handle, int mtu_size) {
        RemoteDevice remoteDevice = findRemoteDeviceByHandle(handle);
        if (remoteDevice != null && BluetoothAdapter.STATE_CONNECTED == remoteDevice.getDeviceState()) {
            return remoteDevice.exchangeMtuSize(mtu_size);
        }
        else {
            Log.d(TAG, "connection with handle =  " + handle + " not in connected state!");
            return false;
        }
    }

    public boolean WriteCharacteristic(final int handle, int length, byte[] data) {
        Log.d(TAG, "WriteCharacteristic");
        RemoteDevice remoteDevice = findRemoteDeviceByHandle(handle);
        if (remoteDevice != null && BluetoothAdapter.STATE_CONNECTED == remoteDevice.getDeviceState()) {
            return remoteDevice.WriteCharacteristic(data);
        }
        else {
            Log.d(TAG, "connection with handle =  " + handle + " not in connected state!");
            return false;
        }
    }
	
    public byte getConnectedDeviceNumber() {
		byte result = 0;
        synchronized (mRemoteDevices) {
            for (RemoteDevice device : mRemoteDevices) {
                if (BluetoothAdapter.STATE_CONNECTED == device.getDeviceState()) {
                    result++;
                }
            }
        }
		return result;
	}

	public byte getHubState() {
		byte result = 0;
		if (mBLEScanner.isScanBusy()) {
			result = 1; //scannint
		}
		
		return result;
	}

	public int getHandleByIndex(byte index) {
		int handle = 0;
        RemoteDevice remoteDevice;
        synchronized (mRemoteDevices) {
            remoteDevice = mRemoteDevices.get(index);
            if (remoteDevice != null) {
                handle = remoteDevice.getHandle();
            }
        }
		return handle;
	}

	public String getAddressByIndex(byte index) {
		String adderss = null;
        RemoteDevice remoteDevice;
        synchronized (mRemoteDevices) {
            remoteDevice = mRemoteDevices.get(index);
            if (remoteDevice != null) {
                adderss = remoteDevice.getAddress();
            }
        }
		return adderss;
	}

	/*new device found*/
    public void onDeviceFound(byte[] address, byte[] name, byte rssi) {
		synchronized (mObject) {
	        int length = 1 + address.length + name.length;
	        byte[] device = new byte[length];
	        System.arraycopy(address, 0, device, 0, address.length);
	        System.arraycopy(name, 0, device, 6, name.length);
	        device[length-1] = rssi;

	        onDeviceFoundNative(device);
		}
    }

	/*scan finished*/
    public void onScanFinished() {
    	synchronized (mObject) {
        	onScanFinishedNative();
		}
    }

	public void onDeivceConnected(String addr) {
		synchronized (mObject) {
            RemoteDevice device = findRemoteDeviceByAddress(addr);
			byte[] address = BLEScanner.getBytesFromAddress(device.getAddress());
			onDeviceConnectedNative(address, device.getHandle());
		}
	}

	public void onDeivceDisconnected(String addr) {
            synchronized (mObject) {
            RemoteDevice device = findRemoteDeviceByAddress(addr);
			byte[] address = BLEScanner.getBytesFromAddress(device.getAddress());
			/*can not fetch disconnection complete reason here.*/
			byte reason = 0x08;//connection timeout
			onDeviceDisconnectedNative(address, device.getHandle(), reason);
		}
	}

	public void onMTUSizeChanged(String addr, int mtu_size) {
		synchronized (mObject) {
            RemoteDevice device = findRemoteDeviceByAddress(addr);
			byte status = 0x00;
			onMTUSizeChangedNative(status, device.getHandle(), mtu_size);
		}
	}

	public void onNotificationReceived(String addr, byte[] data) {
		synchronized (mObject) {
            RemoteDevice device = findRemoteDeviceByAddress(addr);
			onNotificationReceivedNative(data, device.getHandle());
		}
	}

	public void freeDeviceHandle(int handle) {
		synchronized (mObject) {
            if(handle < MAX_DEVICE) {
			    mHanleArray[handle] = false;
            }
		}
	}

	public int allocDeviceHandle() {
		synchronized (mObject) {
			int i = 0;
			for (i = 0; i < MAX_DEVICE; i++) {
				if (mHanleArray[i] == false) {
					mHanleArray[i] = true;
					return i;
				}
			}
		}
		return MAX_DEVICE;
	}

	private void initialDeviceHandle() {
		synchronized (mObject) {
			int i = 0;
			for (i = 0; i < MAX_DEVICE; i++) {
				mHanleArray[i] = false;
			}
		}
	}

	/*reset device state while bluetooth state changed to off.*/
	private void resetDevices() {

    }


    public RemoteDevice findRemoteDeviceByAddress(String address) {
		synchronized (mRemoteDevices) {
			Log.d(TAG, "findRemoteDeviceByAddress with size = :" + mRemoteDevices.size());
			for (RemoteDevice device : mRemoteDevices) {
				Log.d(TAG, "findRemoteDeviceByAddress with address = " + device.getAddress());
                Log.d(TAG, "findRemoteDeviceByAddress with handle = " + device.getHandle());
				if (device.getAddress().equals(address)) {
					return device;
				}
			}
		}
		
        return null;
    }

    public RemoteDevice findRemoteDeviceByHandle(int handle) {
		synchronized (mRemoteDevices) {
	        Log.d(TAG, "findRemoteDeviceByHandle with size = " + mRemoteDevices.size());
	        for (RemoteDevice device : mRemoteDevices) {
                Log.d(TAG, "findRemoteDeviceByHandle with address = " + device.getAddress());
                Log.d(TAG, "findRemoteDeviceByHandle with handle = " + device.getHandle());
	            if (handle == device.getHandle()) {
                    Log.d(TAG, "findRemoteDeviceByHandle with address = " + device.getAddress());
	                return device;
	            }
	        }
		}
		
        return null;
    }

    /*check bluetooth adapter state, if bluetooth adapter is STATE_OFF, try to turn on bluetooth.
    * if bluetooth is ready for use, return true;
    * else return false.
    * */
    private boolean checkBluetoothState() {
        Log.d(TAG, "checkBluetoothState");
        int btState = BluetoothAdapter.STATE_OFF;
        int turnOnBTTimeout = 8; //timeout is 8s

        if (TEST == 1) {
            return true;
        }

        if (mBluetoothAdapter != null) {
            btState = mBluetoothAdapter.getState();
            if (btState == BluetoothAdapter.STATE_TURNING_OFF) {
                /*Bluetooth Adapter is turning off, error state.*/
                return false;
            }
            if (btState == BluetoothAdapter.STATE_ON) {
                /*Bluetooth Adapter is ready for use.*/
                return true;
            } else  {
                /*Bluetooth Adapter is turning on, wait for */
                if (btState == BluetoothAdapter.STATE_OFF) {
                    /*try to turn on bluetooth.*/
                    mBluetoothAdapter.enable();
                }

                 /*wait for bluetooth is turned on.*/
                 for (int i = 0; i < 8; i++) {
                     if (mBluetoothAdapter.getState() == BluetoothAdapter.STATE_ON) {
                         /*Bluetooth Adapter is ready for use.*/
                         Log.d(TAG, "turn on bluetooth successful!");
                         return true;
                     }

                     /*sleep 1s wait for BT turning on*/
                     try {
                         Thread.sleep(1000);
                     } catch(Exception e) {
                         Log.d(TAG, "checkBluetoothState catch exception!");
                     }
                 }
            }
        }

        return false;

    }

    public native void onDeviceFoundNative(byte[] device);
    public native void onScanFinishedNative();
    public native void onDeviceConnectedNative(byte[] address, int handle);
    public native void onDeviceDisconnectedNative(byte[] address, int handle, byte reason);
    public native void onMTUSizeChangedNative(byte status, int handle, int mtu);
    public native void onNotificationReceivedNative(byte[] data, int handle);
    //public native void onBluetoothStateOFF();
}


