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
// define the device manager as hub
#pragma once

#include "gfTypes.h"
#include <vector>

namespace gf
{
	class HubListener;

	class Hub
	{
	public:

		// module management
		virtual GF_RET_CODE init(GF_UINT8 comPort = 0) = 0;
		virtual GF_RET_CODE deinit() = 0;
		virtual WorkMode getWorkMode() const = 0;
		virtual WorkMode setWorkMode(WorkMode newMode) = 0;
		// get status, version, etc.
		virtual HubState getStatus() = 0;
		virtual tstring getDescString() const = 0;

		// setup listener
		virtual GF_RET_CODE registerListener(const gfwPtr<HubListener>& listener) = 0;
		virtual GF_RET_CODE unRegisterListener(const gfwPtr<HubListener>& listener) = 0;

		virtual GF_RET_CODE startScan(GF_UINT8 rssiThreshold = 0) = 0;
		virtual GF_RET_CODE stopScan() = 0;

		virtual GF_SIZE getNumOfDevices() const = 0;
		virtual GF_SIZE getNumOfConnectedDevices() const = 0;
		virtual void enumDevices(FunEnumDevice funEnum, bool bConnectedOnly = true) = 0;
		virtual WPDEVICE findDevice(GF_UINT8 addrType, tstring address) = 0;
		// set up virtual device, client can combine two or more gdevices positioning in
		//   defferent location into one virtual device. client still can receive gesture
		//   data of these devices seperately, and can get one combined gesture
		//   data if it happens
		virtual GF_RET_CODE createVirtualDevice(int numDevices, std::vector<WPDEVICE> realDevices, WPDEVICE& newDevice) = 0;

		// removing method releaseDevice, virtual device can be destoryed by calling disconnect
		//virtual int releaseDevice(SPTR_DEV& device) = 0;

		virtual GF_RET_CODE poll(Event& event) = 0;

	protected:
		Hub() {}
		virtual ~Hub() {}
		friend class HubManager;
	};

} // namespace gf
