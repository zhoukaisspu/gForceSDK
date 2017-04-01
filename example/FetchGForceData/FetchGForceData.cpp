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

class GForceData : public HubListener
{
public:
	GForceData(gfsPtr<Hub>& pHub)
		: mHub(pHub)
	{
	}
	/// This callback is called when the Hub finishes scanning devices.
	///
	/// \remark
	///             This callback may be called after a series of onDeviceFound()
	///     are called.
	virtual void onScanfinished()
	{
		cout << __FUNCTION__ << " has been called." << endl;
		if (nullptr == mDevice)
		{
			// if no device found, scan again
			mHub->startScan();
		}
		else
		{
			DeviceConnectionStatus status = mDevice->getConnectionStatus();
			if (DeviceConnectionStatus::Connected != status &&
				DeviceConnectionStatus::Connecting != status)
			{
				mDevice->connect();
			}
		}
	}

	/// This callback is called when the state of the hub changed
	/// \param state
	///             An enumerate of HubState which indicates the state of the hub.
	virtual void onStateChanged(HubState state)
	{
		cout << __FUNCTION__ << " has been called. New state is " << static_cast<GF_UINT32>(state) << endl;
		if (HubState::Unknown == state)
			mLoop = false;
	}

	/// This callback is called when the hub finds a device.
	///
	/// \param device The pointer to Device that was found.
	virtual void onDeviceFound(WPDEVICE device)
	{
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

	/// This callback is called when a previous connected device has
	/// disconnected from the hub.
	///
	/// \param device The pointer to Device that was previously found and
	///                               passed to the application.
	///
	virtual void onDeviceDiscard(WPDEVICE device)
	{
		cout << __FUNCTION__ << " has been called." << endl;
	}

	/// This callback is called a device has been connected successfully
	///
	/// \param device The pointer to Device that was connected
	///
	virtual void onDeviceConnected(WPDEVICE device)
	{
		cout << __FUNCTION__ << " has been called." << endl;
	}

	/// This callback is called when a device has been disconnected from
	///                                 connection state or failed to connect to
	///
	/// \param device The pointer to Device that was disconnected
	///
	/// \reason The reason of disconnection
	virtual void onDeviceDisconnected(WPDEVICE device, GF_UINT8 reason)
	{
		cout << __FUNCTION__ << " has been called. reason: " << static_cast<GF_UINT32>(reason) << endl;
		auto sp = device.lock();
		if (nullptr != sp && sp == mDevice)
		{
			mDevice->connect();
		}
	}

	/// This callback is called when the quaternion data is received
	///
	/// \param device The pointer to Device that is sending the data
	///
	/// \param rotation The Quaternion type of quaternion data received
	virtual void onOrientationData(WPDEVICE device, const Quaternion<float>& rotation)
	{
		cout << __FUNCTION__ << " has been called. " << rotation.toString() << endl;
	}

	/// This callback is called when the gesture data is recevied
	///
	/// \param device The pointer to Device that is sending the data
	///
	/// \param gest The Gesture type of data received
	virtual void onGestureData(WPDEVICE device, Gesture gest)
	{
		cout << __FUNCTION__ << " has been called." << endl;
	}

	/// This callback is called when the button on gForce is pressed by user
	///
	/// \param device The pointer to Device sending the data
	///
	virtual void onReCenter(WPDEVICE device)
	{
		cout << __FUNCTION__ << " has been called." << endl;
	}

	bool getLoop()
	{
		return mLoop;
	}

private:
	bool mLoop = true;
	gfsPtr<Hub> mHub;
	gfsPtr<Device> mDevice;
};

int _tmain(int argc, _TCHAR* argv[])
{
	GF_RET_CODE retCode = GF_RET_CODE::GF_SUCCESS;

	tstring identifer(_T("FetchGForceData sample app"));
	auto pHub = HubManager::getHubInstance(identifer);

	pHub->setWorkMode(WorkMode::ClientThread);

	gfsPtr<GForceData> gforceData = make_shared<GForceData>(pHub);
	gfsPtr<HubListener> listener = static_pointer_cast<HubListener>(gforceData);
	retCode = pHub->registerListener(listener);
	cout << "registerListener " << ((retCode == GF_RET_CODE::GF_SUCCESS) ? "SUCCESS" : "FAIL") << endl;

	retCode = pHub->init();
	if (GF_RET_CODE::GF_SUCCESS != retCode)
	{
		cout << "hub init failed: " << static_cast<GF_UINT32>(retCode) << endl;
		return 1;
	}

	retCode = pHub->startScan();
	if (GF_RET_CODE::GF_SUCCESS != retCode)
	{
		cout << "scan starting failed: " << static_cast<GF_UINT32>(retCode) << endl;
		goto exit;
	}

	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, TRUE))
	{
		do
		{
			GF_UINT32 period = 100; // ms
			retCode = pHub->run(period);
			if (GF_RET_CODE::GF_SUCCESS != retCode &&
				GF_RET_CODE::GF_ERROR_TIMEOUT != retCode)
			{
				cout << "Method run() failed: " << static_cast<GF_UINT32>(retCode) << endl;
				break;
			}
		} while (gforceData->getLoop() && !bExiting);
	}
	else
	{
		cout << "Error: could not set control handler." << endl;
	}

exit:
	pHub->unRegisterListener(listener);
	pHub->deinit();

	return 0;
}

