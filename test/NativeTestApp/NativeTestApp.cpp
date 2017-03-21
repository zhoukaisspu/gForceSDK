// NativeTestApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define TAG "NativeTestApp"

#include "AdapterFactory.h"
#include "Device.h"
#include "AdapterListener.h"
#include "DeviceListener.h"
#include "LogUtils.h"
#include "../Utils.h"

#include <atomic>
#include <list>
#include <algorithm>

using namespace std;
using namespace oym;

#define DEFAULT_RSSI 0xA0

atomic<bool> bExiting = false;

BOOL ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
	case CTRL_C_EVENT:
		// handle the ctrl-c signal.
		GF_LOGI("ctrl-c event\n");
		bExiting = true;
		break;
	case CTRL_CLOSE_EVENT:
		// ctrl-close: confirm that the user wants to exit.
		GF_LOGI("ctrl-close event");
		bExiting = true;
		break;
	case CTRL_BREAK_EVENT:
		// pass other signals to the next handler.
		GF_LOGI("ctrl-break event");
		bExiting = true;
		break;
	case CTRL_LOGOFF_EVENT:
		GF_LOGI("ctrl-logoff event");
		bExiting = true;
		break;
	case CTRL_SHUTDOWN_EVENT:
		GF_LOGI("ctrl-shutdown event");
		bExiting = true;
		break;
	default:;
	}
	if (bExiting)
		return TRUE;
	else
		return FALSE;
}

void printHelp()
{
	GF_LOGI("\tPress Ctrl+C to exit.\n");
	GF_LOGI("g:\tGet adapter status.");
	GF_LOGI("s:\tStart to scan.");
	GF_LOGI("S:\tStop to scan.");
	GF_LOGI("e:\tEnum devices.");
	GF_LOGI("c:\tConnect to device.");
	GF_LOGI("C:\tCancel connecting to device.");
	GF_LOGI("d:\tDisconnect device.\n\n");
}

list<gfsPtr<Device>> listDev;
void enumDevice(WPDEVICE dev)
{
	auto sp = dev.lock();
	if (nullptr == sp)
	{
		GF_LOGW("%s: empty device???", __FUNCTION__);
	}
	else
	{
		GF_LOGI("Dev: addrtype: %u, address: %s, name: %s, connstatus: %u, position:%u",
			(GF_UINT)sp->getAddrType(), utils::tostring(sp->getAddress()).c_str(), utils::tostring(sp->getName()).c_str(),
			static_cast<GF_UINT>(sp->getConnectionStatus()), static_cast<GF_UINT>(sp->getPosition()));
		listDev.push_back(sp);
	}
}

void handleCmd(gfsPtr<Adapter>& pAdapter, string cmd)
{
	if (cmd.length() == 0)
		return;

	GF_LOGI("Command %s received.", cmd.c_str());
	switch (cmd[0])
	{
	case 'g':
		GF_LOGI("adapter status is: %u", static_cast<GF_UINT>(pAdapter->getStatus()));
		break;
	case 's':
		listDev.clear();
		pAdapter->startScan(DEFAULT_RSSI);
		break;
	case 'S':
		pAdapter->stopScan();
		break;
	case 'e':
	{
		GF_LOGI("Total %u devices found, %u of them are connected.",
			pAdapter->getNumOfDevices(), pAdapter->getNumOfConnectedDevices());
		listDev.clear();
		pAdapter->enumDevices(enumDevice, false);
		break;
	}
	case 'c':
	{
		auto itor = listDev.begin();
		if (itor != listDev.end())
			(*itor)->connect();
		break;
	}
	case 'C':
	{
		auto itor = listDev.begin();
		if (itor != listDev.end())
			(*itor)->cancelConnect();
		break;
	}
	case 'd':
	{
		auto itor = listDev.begin();
		if (itor != listDev.end())
			(*itor)->disconnect();
		break;
	}
	default:;
		GF_LOGW("Invalid command %s.", cmd.c_str());
	}
}


class AdapterListenerImp : public AdapterListener
{
	virtual void onScanfinished()
	{
		GF_LOGD("%s", __FUNCTION__);
	}
	virtual void onDeviceFound(WPDEVICE device)
	{
		auto ptr = device.lock();
		GF_LOGI("%s: Name: %s", __FUNCTION__, (nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
	}
	virtual void onDeviceDiscard(WPDEVICE device)
	{
		auto ptr = device.lock();
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
		//listDev.erase(remove_if(listDev.begin(), listDev.end(),
		//	[&ptr](decltype(*listDev.begin()) it){ return (ptr == it); }), listDev.end());

		for (auto itor = listDev.begin(); itor != listDev.end();)
		{
			if (ptr == (*itor))
				listDev.erase(itor++);
			else
				++itor;
		}
	}
	virtual void onDeviceSpecialized(WPDEVICE oldPtr, WPDEVICE newPtr)
	{
		auto spold = oldPtr.lock();
		auto spnew = newPtr.lock();
		GF_LOGI("%s: AdapterState: old device: %s", __FUNCTION__, (nullptr == spold ? "__empty__" : utils::tostring(spold->getName()).c_str()));
		GF_LOGI("%s: AdapterState: new device: %s", __FUNCTION__, (nullptr == spnew ? "__empty__" : utils::tostring(spnew->getName()).c_str()));
		//listDev.erase(remove_if(listDev.begin(), listDev.end(),
		//	[&spold](decltype(*listDev.begin()) it){ return (spold == it); }), listDev.end());

		for (auto itor = listDev.begin(); itor != listDev.end();)
		{
			if (spold == (*itor))
				listDev.erase(itor++);
			else
				++itor;
		}
		if (nullptr != spnew)
			listDev.push_back(spnew);
	}
	virtual void onStateChanged(AdapterState state)
	{
		GF_LOGI("%s: AdapterState: %u", __FUNCTION__, static_cast<GF_UINT>(state));
	}
};

class DeviceListenerImpl : public DeviceListener
{
	virtual void onDeviceConnected(Device* device, GF_STATUS status)
	{
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == device ? "__empty__" : utils::tostring(device->getName()).c_str()));
	}
	virtual void onDeviceDisonnected(Device* device, GF_STATUS status, GF_UINT8 reason)
	{
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == device ? "__empty__" : utils::tostring(device->getName()).c_str()));
	}
	virtual void onMTUSizeChanged(Device* device, GF_STATUS status, GF_UINT16 mtu_size)
	{
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == device ? "__empty__" : utils::tostring(device->getName()).c_str()));
	}
	virtual void onConnectionParmeterUpdated(Device* device, GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
	{
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == device ? "__empty__" : utils::tostring(device->getName()).c_str()));
	}
	virtual void onEvent(Device* device, GF_EVENT event, GF_UINT8 length, GF_PUINT8 data)
	{
		GF_LOGD("%s: device: %s, event: %u", __FUNCTION__,
			(nullptr == device ? "__empty__" : utils::tostring(device->getName()).c_str()), (GF_UINT)event);

		if (event == GF_EVT_DATA_GESTURE)
		{
			GF_LOGD("Gesture data received: %u", data[0]);
		}
	}
	virtual void onChracteristicUpdated(Device* device, GF_STATUS status, AttributeHandle attribute_handle, GF_UINT8 length, GF_PUINT8 data)
	{
		GF_UINT16 attr = static_cast<GF_UINT16>(attribute_handle);

		if (attr < static_cast<GF_UINT16>(AttributeHandle::Max) && 'S' == charTypes[attr])
		{
			string str((char*)data, length);
			GF_LOGI("%s: device: %s, status: %u, characteristic %d received: %s", __FUNCTION__,
				(nullptr == device ? "__empty__" : utils::tostring(device->getName()).c_str()), status, attr, str.c_str());
		}
	}
};

int _tmain()
{
	printHelp();
	gfsPtr<Adapter> pAdapter = AdapterFactory::getAdapterInstance(_T("TestApp"));

	if (nullptr == pAdapter) {
		GF_LOGE("failed to get adapter.");
		return 0;
	}

	gfsPtr<AdapterListener> listener = dynamic_pointer_cast<AdapterListener>(make_shared<AdapterListenerImp>());
	pAdapter->registerListener(listener);
	pAdapter->registerListener(dynamic_pointer_cast<AdapterListener>(make_shared<AdapterListenerImp>()));
	//listener = nullptr;

	if (GF_SUCCESS != pAdapter->init())
	{
		GF_LOGE("failed to init adapter.");
		return 0;
	}
	GF_LOGI("adapter status is: %u", static_cast<GF_UINT>(pAdapter->getStatus()));
	GF_LOGI("adapter string is: %s", utils::tostring(pAdapter->getDescString()).c_str());


	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, TRUE))
	{
		do {
			string cmd;
			cin >> cmd;
			handleCmd(pAdapter, cmd);
		} while (!bExiting);
	}
	else
	{
		GF_LOGE("Error: could not set control handler.");
	}

	listDev.clear();
	pAdapter->unRegisterListener(listener);
	pAdapter->deinit();

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, FALSE);
	return 0;
}

