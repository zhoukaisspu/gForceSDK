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
#include "stdafx.h"
#include "gforce.h"
#include <atomic>

using namespace gf;
using namespace std;


atomic<bool> bExiting = false;

BOOL ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
	case CTRL_C_EVENT:
		// handle the ctrl-c signal.
		bExiting = true;
		break;
	case CTRL_CLOSE_EVENT:
		// ctrl-close: confirm that the user wants to exit.
		bExiting = true;
		break;
	case CTRL_BREAK_EVENT:
		// pass other signals to the next handler.
		bExiting = true;
		break;
	case CTRL_LOGOFF_EVENT:
		bExiting = true;
		break;
	case CTRL_SHUTDOWN_EVENT:
		bExiting = true;
		break;
	default:;
	}
	if (bExiting)
		return TRUE;
	else
		return FALSE;
}

// The GForceHandle implements HubListener,
//     operates gForce device and receives data
class GForceHandle : public HubListener
{
public:
	GForceHandle(gfsPtr<Hub>& pHub)
		: mHub(pHub)
	{
	}
	/// This callback is called when the Hub finishes scanning devices.
	virtual void onScanFinished() override
	{
		cout << __FUNCTION__ << " has been called." << endl;
		if (nullptr == mDevice)
		{
			// if no device found, we do scan again
			mHub->startScan();
		}
		else
		{
			// or if there already is a device found and it's not
			//     in connecting or connected state, try to connect it.
			DeviceConnectionStatus status = mDevice->getConnectionStatus();
			if (DeviceConnectionStatus::Connected != status &&
				DeviceConnectionStatus::Connecting != status)
			{
				mDevice->connect();
			}
		}
	}

	/// This callback is called when the state of the hub changed
	virtual void onStateChanged(HubState state) override
	{
		cout << __FUNCTION__ << " has been called. New state is " << static_cast<GF_UINT32>(state) << endl;
		// if the hub is disconnected (such as unplugged), then set the flag of exiting the app.
		if (HubState::Disconnected == state)
		{
			mLoop = false;
		}
	}

	/// This callback is called when the hub finds a device.
	virtual void onDeviceFound(WPDEVICE device) override
	{
		// In the sample app, we only connect to one device, so once we got one, we stop scanning.
		cout << __FUNCTION__ << " has been called." << endl;
		auto sp = device.lock();
		if (nullptr != sp)
		{
			// only search the first connected device if we connected it before
			if (nullptr == mDevice || sp == mDevice)
			{
				mDevice = sp;
				mHub->stopScan();
			}
		}
	}

	/// This callback is called a device has been connected successfully
	virtual void onDeviceConnected(WPDEVICE device) override
	{
		cout << __FUNCTION__ << " has been called." << endl;
	}

	/// This callback is called when a device has been disconnected from
	///                                 connection state or failed to connect to
	virtual void onDeviceDisconnected(WPDEVICE device, GF_UINT8 reason) override
	{
		// if connection lost, we will try to reconnect again.
		cout << __FUNCTION__ << " has been called. reason: " << static_cast<GF_UINT32>(reason) << endl;
		auto sp = device.lock();
		if (nullptr != sp && sp == mDevice)
		{
			mDevice->connect();
		}
	}

	/// This callback is called when the quaternion data is received
	virtual void onOrientationData(WPDEVICE device, const Quaternion& rotation) override
	{
		// print the quaternion data
		cout << __FUNCTION__ << " has been called. " << rotation.toString() << endl;
	}

	/// This callback is called when the gesture data is recevied
	virtual void onGestureData(WPDEVICE device, Gesture gest) override
	{
		// a gesture event coming.
		auto sp = device.lock();
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
			gesture = "Undefined: ";
			string s;
			stringstream ss(s);
			ss << static_cast<int>(gest);
			gesture += ss.str();
		}
		}
		cout << __FUNCTION__ << " has been called. " << gesture << endl;
	}

	/// This callback is called when the button on gForce is pressed by user
	virtual void onReCenter(WPDEVICE device) override
	{
		cout << __FUNCTION__ << " has been called." << endl;
	}

	// Indicates if we want to exit app
	bool getLoop()
	{
		return mLoop;
	}

private:
	// Indicates if we will keep message polling
	bool mLoop = true;
	// keep a instance of hub.
	gfsPtr<Hub> mHub;
	// keep a device to operate
	gfsPtr<Device> mDevice;
};

int _tmain(int argc, _TCHAR* argv[])
{
	GF_RET_CODE retCode = GF_RET_CODE::GF_SUCCESS;

	// get the hub instance from hub factory
	tstring identifer(_T("FetchGForceHandle sample app"));
	auto pHub = HubManager::getHubInstance(identifer);

	// set work mode to WorkMode::Polling, then the client has
	//     to call Hub::run to receive callbacks
	pHub->setWorkMode(WorkMode::Polling);

	// create the listener implementation and register to hub
	gfsPtr<GForceHandle> gforceHandle = make_shared<GForceHandle>(pHub);
	gfsPtr<HubListener> listener = static_pointer_cast<HubListener>(gforceHandle);
	retCode = pHub->registerListener(listener);
	cout << "registerListener " << ((retCode == GF_RET_CODE::GF_SUCCESS) ? "SUCCESS" : "FAIL") << endl;

	// Initialize hub. Could be failed in below cases:
	//   1. The hub is not plugged in the USB port.
	//   2. Other apps are connected to the hub already.
	retCode = pHub->init();
	if (GF_RET_CODE::GF_SUCCESS != retCode)
	{
		cout << "hub init failed: " << static_cast<GF_UINT32>(retCode) << endl;
		return 1;
	}

	// start to scan devices
	retCode = pHub->startScan();
	if (GF_RET_CODE::GF_SUCCESS != retCode)
	{
		cout << "scan starting failed: " << static_cast<GF_UINT32>(retCode) << endl;
		goto exit;
	}

	// set console handler to receive Ctrl+C command so that we can exit the app by Ctrl+C.
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, TRUE))
	{
		do
		{
			// set up 50ms timeout so we can handle console commands
			GF_UINT32 period = 50; // ms

			// Hub::run could be failed in the below cases:
			//   1. other threads have already been launching it.
			//   2. WorkMode is not set to WorkMode::Polling.
			// A return of GF_RET_CODE::GF_ERROR_TIMEOUT means no error but period expired.
			retCode = pHub->run(period);
			if (GF_RET_CODE::GF_SUCCESS != retCode &&
				GF_RET_CODE::GF_ERROR_TIMEOUT != retCode)
			{
				cout << "Method run() failed: " << static_cast<GF_UINT32>(retCode) << endl;
				break;
			}
		}
		// Loop ends while the hub is unplugged or Ctrl+C is pressed.
		while (gforceHandle->getLoop() && !bExiting);
	}
	else
	{
		cout << "Error: could not set control handler." << endl;
	}

exit:
	// Clean execution envionment while exiting
	pHub->unRegisterListener(listener);
	pHub->deinit();

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, FALSE);

	return 0;
}

