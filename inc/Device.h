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
#pragma once

#include "gfTypes.h"

/// \namespace gf
namespace gf
{
	class DeviceListener;

	/// \class Device
	/// \brief
	///             The abstract of a gForce device.
	///             All devices are created and maintained by the Hub instance
	///
	class Device
	{
	public:
		/// This method provides the device address type
		///
		/// \param state
		///             An enumerate of HubState which indicates the state of the hub.
		virtual GF_UINT8 getAddrType() const = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual tstring getAddress() const = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual tstring getName() const = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		// identify a device by vibration or light, if the device support
		virtual GF_RET_CODE identify(int msec = 1000) = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual DeviceConnectionStatus getConnectionStatus() const = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual GF_RET_CODE setPostion(DevicePosition pos) = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual DevicePosition getPosition() const = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual GF_RET_CODE connect(bool directConn = true) = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual GF_RET_CODE disconnect() = 0;

		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///             This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual GF_RET_CODE cancelConnect() = 0;

	protected:
		virtual ~Device() {}
	};

} // namespace gf
