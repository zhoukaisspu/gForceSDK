package com.oymotion.ble;

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
import android.util.Log;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;
import android.os.ParcelUuid;
import android.os.Handler;

public class BLEScanner {
    private final static String TAG = BLEScanner.class.getSimpleName();
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothManager mBluetoothManager;
    private BluetoothLeScanner mScanner;
    private Context mContext;

    private final UUID[] serviceUuids = null;
    private BLEService mBleService;

    private int mScanningState;

    private Handler mHandler;

    private static final int SANNING_IDLE = 0;
    private static final int SANNING_BUSY = 1;

    public BLEScanner(BLEService service) {
        mBleService = service;
        mScanningState = SANNING_IDLE;
        mContext = GlobalContext.getApplicationContext();
        mHandler = new Handler();
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
                mScanner = mBluetoothAdapter.getBluetoothLeScanner();
                if (mScanner == null) {
                    Log.e(TAG, "Unable to initialize BluetoothLeScanner.");
                } else {
                    Log.e(TAG, "initialize BluetoothLeScanner successful.");
                }
            }
        }
    }

    private Runnable mRunnable= new Runnable() {
        public void run() {
            Log.i(TAG, "scan timeout");
            stopScan();
        }
    };

	public boolean isScanBusy() {
		return (mScanningState == SANNING_BUSY);
	}

    public boolean startScan() {
        Log.i(TAG, "start scan....");

        if (mScanningState != SANNING_IDLE) {
            Log.e(TAG, "Scanning is busy!");
            return false;
        }

        if (mBluetoothAdapter != null && mScanner == null) {
            mScanner = mBluetoothAdapter.getBluetoothLeScanner();
        }

        if (mScanner != null) {
            mScanner.startScan(filters, settings, scanCallback);

            /*automatic stop scan in 10s.*/
            mHandler.postDelayed(mRunnable, 10000);

            mScanningState = SANNING_BUSY;
            return true;
        } else {
            Log.e(TAG, "unable to attach BluetoothLEScanner.");
            return false;
        }
    }

    public boolean stopScan() {
        Log.e(TAG, "stop scan....");

        if (mScanningState != SANNING_BUSY) {
            Log.e(TAG, "Scanning is not busy!");
            return false;
        }

        if (!(mBluetoothAdapter != null && mBluetoothAdapter.isEnabled())) {
            Log.e(TAG, "bluetooth has been turned off, no need to stop scan!");
            mScanningState = SANNING_IDLE;
            return false;
        }

        if (mBluetoothAdapter != null && mScanner == null) {
            mScanner = mBluetoothAdapter.getBluetoothLeScanner();
        }

        if (mScanner != null) {
            mScanner.stopScan(scanCallback);
            mHandler.removeCallbacks(mRunnable);
        }

        mScanningState = SANNING_IDLE;

        if (mBleService != null ) {
            mHandler.postDelayed(new Runnable(){
                public void run() {
                    mBleService.onScanFinished();
                }
            }, 1);
        }

        return true;
    }

    BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {
                @Override
                public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
                    Log.d(TAG, "onLeScan...");
                    Log.d(TAG, "Device Name:" + device.getName());
                    if (device.getName() == null) {
                        /*device name is not available, do not process it.*/
                        return;
                    }
                    byte[] name = device.getName().getBytes();
                    for (int i = 0; i < name.length; i++)
                    {
                        String hex = Integer.toHexString(name[i] & 0xFF);
                        Log.d(TAG, "name:" + hex);
                    }

                    String mAddress = device.getAddress();
                    Log.d(TAG, "Device Address:" + mAddress);
                    byte[] address = getBytesFromAddress(mAddress);
                    for (int i = 0; i < address.length; i++)
                    {
                        String hex = Integer.toHexString(address[i] & 0xFF);
                        Log.d(TAG, "addrsss:" + hex);
                    }

                    Log.d(TAG, "Device RSSI:" + rssi);

                    if (mBleService != null ){
                        mBleService.onDeviceFound(address, name, (byte)rssi);
                    }
                }
            };

    ScanCallback scanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            if (callbackType != ScanSettings.CALLBACK_TYPE_ALL_MATCHES) {
                // Should not happen.
                Log.e(TAG, "LE Scan has already started");
                return;
            }

            //Log.d(TAG, "Ad Flags:" + result.getScanRecord().getAdvertiseFlags());
            //Log.d(TAG, "Device Name:" + result.getScanRecord().getDeviceName());
            //SparseArray<byte[]> md = result.getScanRecord().getManufacturerSpecificData();

            //String manufactureData = new String();
            ScanRecord scanRecord = result.getScanRecord();
            Log.d(TAG, "Scan Record:" + scanRecord.toString());

            if (scanRecord == null) {
                return;
            }

            if (serviceUuids != null) {
                List<ParcelUuid> uuids = new ArrayList<ParcelUuid>();
                for (UUID uuid : serviceUuids) {
                    uuids.add(new ParcelUuid(uuid));
                }
                List<ParcelUuid> scanServiceUuids = scanRecord.getServiceUuids();
                if (scanServiceUuids == null || !scanServiceUuids.containsAll(uuids)) {
                    Log.d(TAG, "uuids does not match");
                    return;
                }
            }

            mLeScanCallback.onLeScan(result.getDevice(), result.getRssi(),
                    scanRecord.getBytes());
        }
    };

    ScanSettings settings = new ScanSettings.Builder()
            .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY).build();

    List<ScanFilter> filters = new ArrayList<ScanFilter>();

    public static byte[] getBytesFromAddress(String address) {
        int i, j = 0;
        byte[] output = new byte[6];

        for (i = 0; i < address.length(); i++) {
            if (address.charAt(i) != ':') {
                output[j] = (byte) Integer.parseInt(address.substring(i, i + 2), 16);
                j++;
                i++;
            }
        }

        return output;
    }

    public static String getAddressStringFromByte(byte[] address) {
        if (address == null || address.length != 6) {
            return null;
        }

        return String.format("%02X:%02X:%02X:%02X:%02X:%02X",
                address[0], address[1], address[2], address[3], address[4],
                address[5]);
    }
}
