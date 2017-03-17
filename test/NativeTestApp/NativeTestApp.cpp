// NativeTestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DongleManager.h"
#include "Device.h"
#include "DongleListener.h"
#include "DeviceListener.h"

#include <atomic>
#include <list>

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
		printf("ctrl-c event\n\n");
		bExiting = true;
		break;
	case CTRL_CLOSE_EVENT:
		// ctrl-close: confirm that the user wants to exit.
		printf("ctrl-close event\n\n");
		bExiting = true;
		break;
	case CTRL_BREAK_EVENT:
		// pass other signals to the next handler.
		printf("ctrl-break event\n\n");
		bExiting = true;
		break;
	case CTRL_LOGOFF_EVENT:
		printf("ctrl-logoff event\n\n");
		bExiting = true;
		break;
	case CTRL_SHUTDOWN_EVENT:
		printf("ctrl-shutdown event\n\n");
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
	tcout << _T("\n\nPress Ctrl+C to exit.\n") << endl;
	tcout << _T("g:\tGet dongle status.") << endl;
	tcout << _T("s:\tStart to scan.") << endl;
	tcout << _T("S:\tStop to scan.") << endl;
	tcout << _T("e:\tEnum devices.") << endl;
	tcout << _T("c:\tConnect to device.") << endl;
	tcout << _T("C:\tCancel connecting to device.") << endl;
	tcout << _T("d:\tDisconnect device.") << endl;
	tcout << endl << endl << endl;
}

list<gfsPtr<Device>> listDev;
void enumDevice(WPDEVICE dev)
{
	auto sp = dev.lock();
	if (nullptr == sp)
	{
		tcout << __FUNCTION__ << ": empty device???" << endl;
	}
	else
	{
		tcout << "Dev: addrtype: " << sp->getAddrType() << ", address: " << sp->getAddress() << ", name: " << sp->getName() << ", connstatus: "
			<< static_cast<int>(sp->getConnectionStatus()) << ", position: " << static_cast<int>(sp->getPosition())
			<< endl;
		listDev.push_back(sp);
	}
}

void handleCmd(gfsPtr<Dongle>& pDongle, string cmd)
{
	if (cmd.length() == 0)
		return;

	printf("Command %s received.\n", cmd.c_str());
	switch (cmd[0])
	{
	case 'g':
		tcout << _T("dongle status is: ") << static_cast<int>(pDongle->getStatus()) << endl;
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
		printf("Total %d devices found, %d of them are connected.\n",
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
	}
}


class DongleListenerImp : public DongleListener
{
	virtual void onScanfinished()
	{
		tcout << __FUNCTION__ << endl;
	}
	virtual void onDeviceFound(gfwPtr<Device> dev)
	{
		auto ptr = dev.lock();
		tcout << __FUNCTION__ << _T(": Name: ") << (nullptr == ptr ? _T("__empty__") : ptr->getName().c_str()) << endl;
	}
	virtual void onDeviceSpecialized(gfwPtr<Device> oldPtr, gfwPtr<Device> newPtr)
	{
		auto spold = oldPtr.lock();
		auto spnew = newPtr.lock();
		tcout << __FUNCTION__ << _T(": old device: ") << (nullptr == spold ? _T("__empty__") : spold->getName().c_str()) << endl;
		tcout << __FUNCTION__ << _T(": new device: ") << (nullptr == spnew ? _T("__empty__") : spnew->getName().c_str()) << endl;
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
	virtual void onStateChanged(DongleState dongState)
	{
		tcout << __FUNCTION__ << ": DongleState: " << static_cast<int>(dongState) << endl;
	}
};

int _tmain()
{
	printHelp();
	gfsPtr<Dongle> pDongle = DongleManager::getDongleInstance(_T("TestApp"));

	if (nullptr == pDongle) {
		tcout << _T("failed to get dongle.") << endl;
		return 0;
	}

	gfsPtr<DongleListener> listener = dynamic_pointer_cast<DongleListener>(make_shared<DongleListenerImp>());
	gfwPtr<DongleListener> wptest = listener;
	pDongle->registerListener(listener);
	pDongle->registerListener(wptest);
	//listener = nullptr;

	if (GF_SUCCESS != pDongle->init())
	{
		tcout << _T("failed to init dongle.") << endl;
		return 0;
	}
	tcout << _T("dongle status is: ") << static_cast<int>(pDongle->getStatus()) << endl;
	tcout << _T("dongle string is: ") << pDongle->getDescString().c_str() << endl;


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
		tcout << _T("Error: could not set control handler.") << endl;
	}

	listDev.clear();
	pDongle->deinit();

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, FALSE);
	return 0;
}

