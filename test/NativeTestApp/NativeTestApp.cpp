// NativeTestApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define TAG "NativeTestApp"

#include "DongleManager.h"
#include "Device.h"
#include "DongleListener.h"
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
	GF_LOGI("g:\tGet dongle status.");
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

void handleCmd(gfsPtr<Dongle>& pDongle, string cmd)
{
	if (cmd.length() == 0)
		return;

	GF_LOGI("Command %s received.", cmd.c_str());
	switch (cmd[0])
	{
	case 'g':
		GF_LOGI("dongle status is: %u", static_cast<GF_UINT>(pDongle->getStatus()));
		break;
	case 's':
		listDev.clear();
		pDongle->startScan(DEFAULT_RSSI);
		break;
	case 'S':
		pDongle->stopScan();
		break;
	case 'e':
	{
		GF_LOGI("Total %u devices found, %u of them are connected.",
			pDongle->getNumOfDevices(), pDongle->getNumOfConnectedDevices());
		listDev.clear();
		pDongle->enumDevices(enumDevice, false);
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


class DongleListenerImp : public DongleListener
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
		GF_LOGI("%s: DongleState: old device: %s", __FUNCTION__, (nullptr == spold ? "__empty__" : utils::tostring(spold->getName()).c_str()));
		GF_LOGI("%s: DongleState: new device: %s", __FUNCTION__, (nullptr == spnew ? "__empty__" : utils::tostring(spnew->getName()).c_str()));
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
	virtual void onStateChanged(DongleState state)
	{
		GF_LOGI("%s: DongleState: %u", __FUNCTION__, static_cast<GF_UINT>(state));
	}
};

class DeviceListenerImpl : public DeviceListener
{
	virtual void onDeviceConnected(WPDEVICE device, GF_STATUS status)
	{
		auto ptr = device.lock();
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
	}
	virtual void onDeviceDisonnected(WPDEVICE device, GF_STATUS status, GF_UINT8 reason)
	{
		auto ptr = device.lock();
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
	}
	virtual void onMTUSizeChanged(WPDEVICE device, GF_STATUS status, GF_UINT16 mtu_size)
	{
		auto ptr = device.lock();
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
	}
	virtual void onConnectionParmeterUpdated(WPDEVICE device, GF_STATUS status, GF_UINT16 conn_int, GF_UINT16 superTO, GF_UINT16 slavelatency)
	{
		auto ptr = device.lock();
		GF_LOGD("%s: device: %s", __FUNCTION__, (nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
	}
	virtual void onEvent(WPDEVICE device, GF_EVENT event, GF_CPCHAR data)
	{
		auto ptr = device.lock();
		GF_LOGD("%s: device: %s, event: %u", __FUNCTION__,
			(nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()), (GF_UINT)event);
	}
};

int _tmain()
{
	printHelp();
	gfsPtr<Dongle> pDongle = DongleManager::getDongleInstance(_T("TestApp"));

	if (nullptr == pDongle) {
		GF_LOGE("failed to get dongle.");
		return 0;
	}

	gfsPtr<DongleListener> listener = dynamic_pointer_cast<DongleListener>(make_shared<DongleListenerImp>());
	pDongle->registerListener(listener);
	pDongle->registerListener(dynamic_pointer_cast<DongleListener>(make_shared<DongleListenerImp>()));
	//listener = nullptr;

	if (GF_SUCCESS != pDongle->init())
	{
		GF_LOGE("failed to init dongle.");
		return 0;
	}
	GF_LOGI("dongle status is: %u", static_cast<GF_UINT>(pDongle->getStatus()));
	GF_LOGI("dongle string is: %s", utils::tostring(pDongle->getDescString()).c_str());


	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, TRUE))
	{
		do {
			string cmd;
			cin >> cmd;
			handleCmd(pDongle, cmd);
		} while (!bExiting);
	}
	else
	{
		GF_LOGE("Error: could not set control handler.");
	}

	listDev.clear();
	pDongle->unRegisterListener(listener);
	pDongle->deinit();

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, FALSE);
	return 0;
}

