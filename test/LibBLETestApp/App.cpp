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
// App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "app.h"
#include "ClientCallbackInterface.h"
#include "AdapterManagerInterface.h"
#include "list"
using namespace std;

bool ctrlhandler( DWORD fdwctrltype )
{
    switch( fdwctrltype )
    {
	    // handle the ctrl-c signal.
	    case CTRL_C_EVENT:
	        printf( "ctrl-c event\n\n" );
	        return( true );
	    // ctrl-close: confirm that the user wants to exit.
	    case CTRL_CLOSE_EVENT:
	        printf( "ctrl-close event\n\n" );
	        return( false );
	    // pass other signals to the next handler.
	    case CTRL_BREAK_EVENT:
	        printf( "ctrl-break event\n\n" );
	        return false;
	    case CTRL_LOGOFF_EVENT:
	        printf( "ctrl-logoff event\n\n" );
	        return false;
	    case CTRL_SHUTDOWN_EVENT:
	        printf( "ctrl-shutdown event\n\n" );
	        return false;
	    default:
	        return false;
    }
}

class GattClient:public GF_CClientCallback
{
public:
	GattClient(GF_CAdapterManagerInterface* am)
	{
		mAMInterface = am;

		if (mAMInterface != NULL)
		{
			mAMInterface->Init(COM_PORT_NUM);
		}
	}

	void onScanResult(GF_BLEDevice* GF_BLEDevice)
	{
		printf("\n onScanResult \n");
		GF_CDevice* device = new GF_CDevice;
		device->address_type = GF_BLEDevice->addr_type;
		memcpy(device->address, GF_BLEDevice->addr, 6);
		memcpy(device->name, GF_BLEDevice->dev_name, 50);
		device->state = 0;

		mAvailabeDevice.push_back(device);
	}

	void onScanFinished()
	{
		GF_CDevice* device;
		printf("\n onScanFinished \n");
		if (mAvailabeDevice.size() != 0)
		{
			device = mAvailabeDevice.front();
			mAMInterface->Connect(device->address, device->address_type, GF_TRUE);
		}
	}

	void onDeviceConnected(GF_STATUS status, GF_ConnectedDevice *device)
	{
		printf("\n onDeviceConnected \n");
	}

	void onDeviceDisonnected(GF_STATUS status, GF_UINT16 handle, GF_UINT8 reason)
	{
		printf("\n onDeviceDisonnected \n");
	}

	void onMTUSizeChanged(GF_STATUS status, GF_UINT16 handle, GF_UINT16 mtu_size)
	{
		printf("\n onMTUSizeChanged \n");
	}

	void onConnectionParmeterUpdated(GF_STATUS status, GF_UINT16 handle, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
	{
		printf("\n onConnectionParmeterUpdated \n");
	}

	void onChracteristicValueRead(GF_STATUS status, GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data)
	{
		printf("\n onChracteristicValueRead \n");
	}

	void onComDestory()
	{
		printf("\n onComDestory \n");
	}
	
	void onNotificationReceived(GF_UINT16 handle, GF_UINT8 length, GF_PUINT8 data)
	{
		printf("\n onNotificationReceived \n");
	}

private:
	GF_CAdapterManagerInterface* mAMInterface;
	list<GF_CDevice*> mAvailabeDevice;
};

int _tmain(int charc, char* argv[]) {
	GF_STATUS status;
	GF_UINT8 loop = 0;
	GF_CAdapterManagerInterface* amInterface = GF_CAdapterManagerInterface::GetInstance();
	if (amInterface == NULL)
	{
		return 10;
	}
	GattClient gattclient(amInterface);

	amInterface->RegisterClientCallback(&gattclient);
#if 1
	status = amInterface->StartScan(100);
	if (!GF_SUCCEEDED)
	{
		return GF_FAIL;
		printf("main thread! status = %d\n", status);
	}
#endif

	if( SetConsoleCtrlHandler( (PHANDLER_ROUTINE) ctrlhandler, true ) )
    {
        while( 1 )
		{ 
			Sleep(2000);
			loop++;
			printf("loop = %d \n", loop);
		}
    }
	else
	{
    	printf( "\nerror: could not set control handler");
	}
	
	return 0;
}
