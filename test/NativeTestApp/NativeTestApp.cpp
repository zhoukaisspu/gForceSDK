/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
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
// NativeTestApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define TAG "NativeTestApp"

#include "HubManager.h"
#include "Device.h"
#include "HubListener.h"
#include "LogUtils.h"
#include "../Utils.h"

#include <atomic>
#include <list>
#include <algorithm>
#include <thread>

using namespace std;
using namespace gf;

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
	GF_LOGI("i:\tInit hub.");
	GF_LOGI("u:\tDeinit hub.");
	GF_LOGI("g:\tGet hub status.");
	GF_LOGI("s:\tStart to scan.");
	GF_LOGI("S:\tStop to scan.");
	GF_LOGI("e:\tEnum devices.");
	GF_LOGI("c:\tConnect to device.");
	GF_LOGI("C:\tCancel connecting to device.");
	GF_LOGI("d:\tDisconnect device.");
	GF_LOGI("p:\tPolling mode.\n\n");
	GF_LOGI("q:\tExit.\n\n");
}

list<gfsPtr<Device>> listDev;
gfsPtr<HubListener> listener;

class HubListenerImp : public HubListener
{
	virtual void onScanFinished() override
	{

		GF_LOGD("ThreadId: %s: %s", utils::threadIdToString(this_thread::get_id()).c_str(), __FUNCTION__);
	}
	virtual void onStateChanged(HubState state) override
	{
		GF_LOGI("ThreadId: %s: %s: HubState: %u", utils::threadIdToString(this_thread::get_id()).c_str(),
			__FUNCTION__, static_cast<GF_UINT>(state));
	}
	virtual void onDeviceFound(WPDEVICE device) override
	{
		auto ptr = device.lock();
		GF_LOGI("ThreadId: %s: %s: Name: %s", utils::threadIdToString(this_thread::get_id()).c_str(), __FUNCTION__,
			(nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
	}
	virtual void onDeviceDiscard(WPDEVICE device) override
	{
		auto ptr = device.lock();
		GF_LOGD("ThreadId: %s: %s: device: %s", utils::threadIdToString(this_thread::get_id()).c_str(), __FUNCTION__,
			(nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
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
	virtual void onDeviceConnected(WPDEVICE device) override
	{
		auto ptr = device.lock();
		GF_LOGD("ThreadId: %s: %s: device: %s", utils::threadIdToString(this_thread::get_id()).c_str(), __FUNCTION__,
			(nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
	}
	virtual void onDeviceDisconnected(WPDEVICE device, GF_UINT8 reason) override
	{
		auto ptr = device.lock();
		GF_LOGD("ThreadId: %s: %s: device: %s, reason: %u", utils::threadIdToString(this_thread::get_id()).c_str(), __FUNCTION__,
			(nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()), reason);
	}
	virtual void onOrientationData(WPDEVICE device, const Quaternion& rotation) override
	{
		auto now = chrono::steady_clock::now();
		chrono::duration<GF_UINT32, milli> duration(500);
		if (mLastPrinted + duration > now)
			return;

		mLastPrinted = now;
		auto ptr = device.lock();
		GF_LOGD("ThreadId: %s: %s, Quaternion: %s, Eulerian angle: %s",
			utils::threadIdToString(this_thread::get_id()).c_str(),
			(nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()),
			rotation.toString().c_str(), rotation.toEuler().toString().c_str());
	}
	virtual void onGestureData(WPDEVICE device, Gesture gest) override
	{
		auto ptr = device.lock();
		string gesture;
		switch (gest)
		{
		case Gesture::Relax:
			gesture = "Relax";
			break;
		case Gesture::Fist:
			gesture = "Fist";
			break;
		case Gesture::SpreadFingers:
			gesture = "SpreadFingers";
			break;
		case Gesture::WaveTowardPalm:
			gesture = "WaveTowardPalm";
			break;
		case Gesture::WaveBackwardPalm:
			gesture = "WaveBackwardPalm";
			break;
		case Gesture::TuckFingers:
			gesture = "TuckFingers";
			break;
		case Gesture::Shoot:
			gesture = "Shoot";
			break;
		case Gesture::Undefined:
		default:
		{
			gesture = "Unknown: ";
			string s;
			stringstream ss(s);
			ss << static_cast<int>(gest);
			gesture += ss.str();
		}
		}
		GF_LOGD("ThreadId: %s: %s: Device: %s, Gesture data received: %s", utils::threadIdToString(this_thread::get_id()).c_str(), __FUNCTION__,
			(nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()), gesture.c_str());
	}
	virtual void onReCenter(WPDEVICE device) override
	{
		auto ptr = device.lock();
		GF_LOGD("ThreadId: %s: %s: Gesture position re-centered. device = %s", utils::threadIdToString(this_thread::get_id()).c_str(), __FUNCTION__,
			(nullptr == ptr ? "__empty__" : utils::tostring(ptr->getName()).c_str()));
	}

private:
	chrono::steady_clock::time_point mLastPrinted = chrono::steady_clock::now();
};

bool enumDevice(WPDEVICE dev)
{
	auto sp = dev.lock();
	ASSERT_VALID_PTR(sp);
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

	// don't want to break enumerate, so always return true
	return true;
}

void handleCmd(gfsPtr<Hub>& pHub, string cmd)
{
	if (cmd.length() == 0)
		return;

	GF_LOGI("Command %s received.", cmd.c_str());
	switch (cmd[0])
	{
	case 'i':
		if (GF_RET_CODE::GF_SUCCESS != pHub->init())
		{
			GF_LOGE("failed to init hub.");
		}
		else
		{
			pHub->registerListener(listener);
			GF_LOGD("done init hub.");
		}
		break;
	case 'u':
		listDev.clear();
		if (GF_RET_CODE::GF_SUCCESS != pHub->deinit())
		{
			GF_LOGE("failed to deinit hub.");
		}
		else
		{
			GF_LOGD("done deinit hub.");
		}
		break;
	case 'g':
		GF_LOGI("hub status is: %u", static_cast<GF_UINT>(pHub->getState()));
		break;
	case 's':
		listDev.clear();
		pHub->startScan(DEFAULT_RSSI);
		break;
	case 'S':
		pHub->stopScan();
		break;
	case 'e':
	{
		GF_LOGI("Total %u devices found, %u of them are connected.",
			pHub->getNumOfDevices(false), pHub->getNumOfDevices(true));
		listDev.clear();
		function<bool(WPDEVICE)> enumFn(enumDevice);
		pHub->enumDevices(enumFn, false);
		break;
	}
	case 'c':
	{
		for (auto& itor : listDev)
		{
			if (DeviceConnectionStatus::Disconnected == itor->getConnectionStatus())
			{
				itor->connect();
				break;
			}
		}
		break;
	}
	case 'C':
	{
		for (auto& itor : listDev)
		{
			if (DeviceConnectionStatus::Connecting == itor->getConnectionStatus())
			{
				itor->cancelConnect();
				break;
			}
		}
		break;
	}
	case 'd':
	{
		for (auto& itor : listDev)
		{
			if (DeviceConnectionStatus::Connected == itor->getConnectionStatus())
			{
				itor->disconnect();
				break;
			}
		}
		break;
	}
	case 'p':
	{
		// entering polling mode
		pHub->setWorkMode(WorkMode::Polling);
		GF_LOGI("Hub work mode is %d now.", static_cast<GF_INT>(pHub->getWorkMode()));
		GF_LOGI("Main thread id is %s.\n", utils::threadIdToString(this_thread::get_id()).c_str());

		int tryit = 5;
		while (tryit--)
		{
			pHub->run(2000);
		}
		pHub->setWorkMode(WorkMode::Freerun);
		break;
	}
	case 'q':
	case 'Q':
		bExiting = true;
		break;
	default:;
		GF_LOGW("Invalid command %s.", cmd.c_str());
	}
}



int _tmain()
{
	printHelp();
	gfsPtr<Hub> pHub = HubManager::getHubInstance(_T("TestApp"));

	if (nullptr == pHub) {
		GF_LOGE("failed to get hub.");
		return 0;
	}
	
	pHub->setWorkMode(WorkMode::Freerun);
	GF_LOGI("Hub work mode is %d now.", static_cast<GF_INT>(pHub->getWorkMode()));
	GF_LOGI("Main thread id is %s.\n", utils::threadIdToString(this_thread::get_id()).c_str());

	listener = static_pointer_cast<HubListener>(make_shared<HubListenerImp>());
	pHub->registerListener(listener);
	pHub->registerListener(static_pointer_cast<HubListener>(make_shared<HubListenerImp>()));
	//listener = nullptr;

	if (GF_RET_CODE::GF_SUCCESS != pHub->init())
	{
		GF_LOGE("failed to init hub.");
		return 0;
	}
	GF_LOGI("hub state is: %u", static_cast<GF_UINT>(pHub->getState()));
	GF_LOGI("hub string is: %s", utils::tostring(pHub->getDescString()).c_str());


	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, TRUE))
	{
		do {
			string cmd;
			cin >> cmd;
			handleCmd(pHub, cmd);
		} while (!bExiting);
	}
	else
	{
		GF_LOGE("Error: could not set control handler.");
	}

	listDev.clear();
	pHub->unRegisterListener(listener);
	pHub->deinit();
	GF_LOGD("deinit done here.");

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, FALSE);
	return 0;
}

