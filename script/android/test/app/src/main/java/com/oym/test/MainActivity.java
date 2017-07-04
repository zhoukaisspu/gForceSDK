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
package com.oym.test;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Button;
import android.view.View;
import android.util.Log;
import android.text.method.ScrollingMovementMethod;

import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;

import com.oym.libble.GlobalContext;

public class MainActivity extends AppCompatActivity {
    private final static String TAG = "Test App";
    private static boolean IsScaning = false;
    TextView textview;
    private Button Start_Scan;
    private ListView listview;
    final List<String> adapterData = new ArrayList<String>();
    ArrayAdapter arrayadapter;
	Handler mHandler;

    private List<BTDevice> mDevices = new ArrayList<BTDevice>();
	private final static int INVALID_HANDLE = 0xFFFF;

	private final static int MESSAGE_DEVICE_CONNECTED = 0;
	private final static int MESSAGE_DEVICE_DISCONNECTED = 1;
	private final static int MESSAGE_NOTIFICATION_RECEIVED = 2;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");
        setContentView(R.layout.activity_main);

        listview = (ListView) findViewById(R.id.listview);
        arrayadapter = new ArrayAdapter<String>(this, android.R.layout.simple_expandable_list_item_1, adapterData);
        listview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Log.d(TAG, "position " + position);
                Log.d(TAG, "id " + id);
                String deviceName = (String)arrayadapter.getItem(position);
                Log.d(TAG, "device Name " + deviceName);
                for (BTDevice device : mDevices) {
                    if (deviceName.contains(device.getName())) {
                        if (device.getState() == BTDevice.STATE_DISCONNECTED) {
                            textview.append("\n Start connect device: \n" + device.getAddress());
                            connectNative(getBytesFromAddress(device.getAddress()));
							adapterData.remove(position);
							adapterData.add(position, device.getName() + "    (Connecting)");
							listview.setAdapter(arrayadapter);
                            break;
                        } else if (device.getState() == BTDevice.STATE_CONNECTED){
                            textview.append("\n Start disconnect device \n" + device.getAddress() + "(handle=" + device.getHandle() + ")");
                            disconnectNative(device.getHandle());
                            adapterData.remove(position);
                            adapterData.add(position, device.getName() + "    (Disconnecting)");
                            listview.setAdapter(arrayadapter);
                            break;
                        }
                    }
                }

            }
        });



        Start_Scan = (Button) findViewById(R.id.button);
        Start_Scan.setClickable(true);
        Start_Scan.setOnClickListener(listener);

        //Example of a call to a native method
        //TextView tv = (TextView) findViewById(R.id.sample_text);
        textview = (TextView) findViewById(R.id.sample_text);
        textview.setMovementMethod(ScrollingMovementMethod.getInstance());

		GlobalContext.setApplicationContext(this);
        initNative();
        IsScaning = false;

		 mHandler = new Handler(){
            @Override
            public void handleMessage(Message msg){
                String address;
                BTDevice device;
                int index;
                super.handleMessage(msg);
				switch (msg.what) {
	                case MESSAGE_DEVICE_CONNECTED:
                        Log.d(TAG, "receive message MESSAGE_DEVICE_CONNECTED");
                        address = (String)msg.obj;
                        device = findRemoteDeviceByAddress(address);
                        Log.d(TAG, "address 1 = " + address);
                        Log.d(TAG, "name = " + device.getName());
                        for (index = 0; index < adapterData.size(); index++) {
                            Log.d(TAG, "address 2 = " + adapterData.get(index));
                            if (adapterData.get(index).contains(device.getName())){
                                Log.d(TAG, "MESSAGE_DEVICE_CONNECTED index = " + index);
                                break;
                            }
                        }
                        if (index < adapterData.size()) {
	                	    adapterData.remove(index);
						    adapterData.add(index, device.getName() + "    (Connected)");
						    listview.setAdapter(arrayadapter);
                        }

                        textview.append("\n device connected: \n" + device.getAddress() + "(handle=" + device.getHandle() + ")");
	                	break; 
					case MESSAGE_DEVICE_DISCONNECTED:
                        int handle = (int)msg.obj;
                        Log.d(TAG, "receive message MESSAGE_DEVICE_DISCONNECTED with handle = " + handle);
                        device = findRemoteDeviceByHandle(handle);
                        if (device == null) {
                            Log.d(TAG, "device not found!!!");
                            return;
                        }

                        Log.d(TAG, "adapterData.size() = " + adapterData.size());
                        for (index = 0; index < adapterData.size(); index++) {
                            if (adapterData.get(index).contains(device.getName())){
                                Log.d(TAG, "MESSAGE_DEVICE_CONNECTED index = " + index);
                                break;
                            }
                        }

                        if (index < adapterData.size()) {
                            adapterData.remove(index);
                            adapterData.add(index, device.getName() + "    (Disconnected)");
                            listview.setAdapter(arrayadapter);
                        }
                        textview.append("\n device disconnected: \n" + device.getAddress());
                        device.setHandle(INVALID_HANDLE);
                        break;

                }
            }
        };
    }

    Button.OnClickListener listener = new Button.OnClickListener() {//创建监听对象
        public void onClick(View v) {
            if (false == IsScaning) {
                String strTmp = "\n StartScan... \n";
                //textview.setText(strTmp);
                textview.append(strTmp);
                clearDevices();
                listview.setAdapter(arrayadapter);
                startScanNative();
                IsScaning = true;
                String stopScan = "StopScan";
                Start_Scan.setText(stopScan.toCharArray(), 0, stopScan.length());
            } else {
                String strTmp = "\n StopScan... \n";
                textview.append(strTmp);

                stopScanNative();
                IsScaning = false;
                String startScan = "StartScan";
                Start_Scan.setText(startScan.toCharArray(), 0, startScan.length());
            }
        }
    };

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy");
        if (true == IsScaning) {
            stopScanNative();
        }

        cleanupNative();
        super.onDestroy();
    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause");
        super.onPause();
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume");
        super.onResume();
    }

    public void onScanFinished() {
        Log.d(TAG, "onScanFinished");
        String strTmp = "\n ScanFinished \n";
        textview.append(strTmp);

        IsScaning = false;
        String startScan = "StartScan";
        Start_Scan.setText(startScan.toCharArray(), 0, startScan.length());
    }

    public void onScanResult(byte[] address, byte[] device_name, int rssi) {
        String addr;
        addr = getAddressStringFromByte(address);
        Log.d(TAG, "onScanResult address: " + addr);
        BTDevice device = findRemoteDeviceByAddress(addr);
        if (device != null) {
            Log.d(TAG, "device already existed.");
            return;
        }
        String dev_name = new String(device_name);
        adapterData.add(dev_name);
        listview.setAdapter(arrayadapter);
        String text = "Device Found with address: \n" + addr + " \n with RSSI:" + rssi;
        textview.append(text);

        Log.d(TAG, "onScanResult address: " + dev_name);
        Log.d(TAG, "onScanResult rssi: " + rssi);

        BTDevice deviceFound = new BTDevice(dev_name, addr, (byte)0xFF, rssi);
        mDevices.add(deviceFound);
    }

    public void onDeviceConnected(byte[] address, int handle) {
        String addr;
        addr = getAddressStringFromByte(address);
        Log.d(TAG, "onDeviceConnected address: " + addr + " with handle = " + handle);

        //listview.setAdapter(arrayadapter);
        //String text = "Device connected \n with address:" + addr + "\n with handle = " + handle;
        //textview.append(text);
        BTDevice device = findRemoteDeviceByAddress(addr);
        if (device != null) {
            Log.d(TAG, "device found");
            device.setHandle(handle);
            device.setState(BTDevice.STATE_CONNECTED);
        }

		Message msg = new Message();
		msg.what = MESSAGE_DEVICE_CONNECTED;
		msg.obj = addr;
		mHandler.sendMessage(msg);
    }

    public void onDeviceDisconnected(byte[] address, int handle, byte reason) {
        String addr;
        addr = getAddressStringFromByte(address);
        Log.d(TAG, "onDeviceDisconnected address: " + addr + " with handle = " + handle);

        BTDevice device = findRemoteDeviceByHandle(handle);
        if (device != null) {
            Log.d(TAG, "device found");
            device.setState(BTDevice.STATE_DISCONNECTED);
        }

		Message msg = new Message();
		msg.what = MESSAGE_DEVICE_DISCONNECTED;
		msg.obj = handle;
		mHandler.sendMessage(msg);
    }


	public void onMTUSizeChanged(int handle, int mtu) {
        Log.d(TAG, "onMTUSizeChanged on handle = " + handle + "MTU Size :" + mtu);
    }

    public void onNotificationReceived(int handle, byte[] notification) {
        String noti;
        noti = new String(notification);
        Log.d(TAG, "onNotificationReceived on handle = " + handle + "data :" + noti);
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("ble-lib");
    }

    static {
        classInitNative();
    }

    public void clearDevices() {
        List<BTDevice> tempDevices = new ArrayList<BTDevice>();
        List<String> tempData = new ArrayList<String>();
        for (BTDevice device: mDevices) {
            if (device.getState() != BTDevice.STATE_DISCONNECTED) {
                tempDevices.add(device);

                for (String name : adapterData) {
                    if (name.contains(device.getName())) {
                        tempData.add(name);
                    }
                }
            }
        }

        Log.d(TAG, "clearDevices tempDevices size = " + tempDevices.size() + "tempData size = " + tempData.size());
        mDevices.clear();
        mDevices.addAll(tempDevices);

        adapterData.clear();
        adapterData.addAll(tempData);
    }

    private class BTDevice {
        private String deviceName;
        private String address;
        private int handle;
        private int rssi;
        private int state;
        public static final int STATE_DISCONNECTED = 0;
        public static final int STATE_CONNECTED = 1;

        public BTDevice(String name, String addr, int hd, int r)
        {
            deviceName = name;
            address = addr;
            handle = hd;
            rssi = r;
            state = STATE_DISCONNECTED;
        }

        public String getName() {
            return deviceName;
        }

        public String getAddress() {
            return address;
        }

        public int getState() {
            return state;
        }

        public void setState(int st) { state = st;}

        public int getRSSI() {
            return rssi;
        }

        public int getHandle() {
            return handle;
        }

        public void setHandle(int hd) {
             handle = hd;
        }
    }

    public static String getAddressStringFromByte(byte[] address) {
        if (address == null || address.length != 6) {
            return null;
        }

        return String.format("%02X:%02X:%02X:%02X:%02X:%02X",
                address[0], address[1], address[2], address[3], address[4],
                address[5]);
    }

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

    public BTDevice findRemoteDeviceByAddress(String address) {
        for (BTDevice device : mDevices) {
            if (device.getAddress().equals(address)) {
                return device;
            }
        }

        return null;
    }

    public BTDevice findRemoteDeviceByHandle(int handle) {
        for (BTDevice device : mDevices) {
            if (device.getHandle() == handle) {
                return device;
            }
        }

        return null;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native boolean initNative();
    public native boolean cleanupNative();
    public native boolean startScanNative();
    public native boolean stopScanNative();
    public native static void classInitNative();
    public native boolean connectNative(byte[] address);
    public native boolean cancelConnectNative(byte[] address);
    public native boolean disconnectNative(int handle);
	public native byte getHubStateNative();
	public native byte getConnectedDevNumNative();
    public native boolean writeCharecteristicNative(byte len, byte[] data);
}
