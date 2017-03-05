// App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "app.h"

#include <OYM_NIF.h>
#include <DiscoveryService.h>
#include "AdapterManager.h"

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

class GattClient:public IClientCallback
{
public:
	GattClient(OYM_AdapterManager* am)
	{
		mAM = am;

		if (mAM != NULL)
		{
			mAM->Init();
		}
	}

	void onScanResult(BLE_DEVICE* ble_device)
	{
		printf("\n onScanResult \n");
		Device* device = new Device;
		device->address_type = ble_device->addr_type;
		memcpy(device->address, ble_device->addr, 6);
		memcpy(device->name, ble_device->dev_name, 50);
		device->state = 0;

		mAvailabeDevice.push_back(device);
	}

	void onScanFinished()
	{
		Device* device;
		printf("\n onScanFinished \n");
		if (mAvailabeDevice.size() != 0)
		{
			device = mAvailabeDevice.front();
			mAM->Connect(device->address, device->address_type, OYM_TRUE);
		}
	}

	void onDeviceConnected(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 conn_int, OYM_UINT16 superTO, OYM_UINT16 slavelatency, OYM_UINT16 MTU_Size)
	{
		printf("\n onDeviceConnected \n");
	}

	void onDeviceDisonnected(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 reason)
	{
		printf("\n onDeviceDisonnected \n");
	}

	void onMTUSizeChanged(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 mtu_size)
	{
		printf("\n onMTUSizeChanged \n");
	}

	void onConnectionParmeterUpdated(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT16 conn_int, OYM_UINT16 superTO, OYM_UINT16 slavelatency)
	{
		printf("\n onConnectionParmeterUpdated \n");
	}

	void onChracteristicValueRead(OYM_STATUS status, OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data)
	{
		printf("\n onChracteristicValueRead \n");
	}

	void onComDestory()
	{
		printf("\n onComDestory \n");
	}
	
	void onNotificationReceived(OYM_UINT16 handle, OYM_UINT8 length, OYM_PUINT8 data)
	{
		printf("\n onNotificationReceived \n");
	}

private:
	OYM_AdapterManager* mAM;
	list<Device*> mAvailabeDevice;
};

int _tmain(int charc, char* argv[]) {
	OYM_STATUS status;
	OYM_UINT8 loop = 0;
	OYM_AdapterManager* am = new OYM_AdapterManager();
	GattClient gattclient(am);

	am->RegisterClientCallback(&gattclient);
#if 1
	status = am->StartScan(0);
	if (!OYM_SUCCEEDED)
	{
		return OYM_FAIL;
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
