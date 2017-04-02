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
/*!
 * \file Device.h
 * \brief The abstract of a gForce device
 *
 * \version 0.1
 * \date 2017.4.3
 */
#pragma once

#include "gfTypes.h"

namespace gf
{
	class DeviceListener;

	/// \class Device
	/// \brief
	///             The abstract of a gForce device.
	///
	///             All devices are created and maintained by the Hub instance
	///
	class Device
	{
	public:
		/// \brief Get the device address type
		///
		/// \return The device address type is defined in BLE.
		virtual GF_UINT8 getAddrType() const = 0;

		/// \brief Get the string type of device address
		///
		///             The address is represented like DD:DD:DD:DD:DD:DD
		/// \return The human-readable tstring type of device address
		virtual tstring getAddress() const = 0;

		/// \brief Get the device name
		///
		/// \return The tstring type of device name
		virtual tstring getName() const = 0;

		/// \brief Identify a physical device using vibration or light, if the device support
		///
		/// \remark
		///             This method is not implemented yet and can be changed in the future.
		///
		/// \return A GF_RET_CODE type of return value to indicate if the identify command is sent or not.
		virtual GF_RET_CODE identify(int msec = 1000) = 0;

		/// \brief Retrieves the connection status of the device
		///
		/// \return DeviceConnectionStatus type of device status
		virtual DeviceConnectionStatus getConnectionStatus() const = 0;

		/// \brief Set the wear position of the device
		///
		/// \remark
		///             This method is not implemented yet and can be changed in the future.
		virtual GF_RET_CODE setPostion(DevicePosition pos) = 0;

		/// \brief Get the wear position of the device set before
		///
		/// \remark
		///             This method is not implemented yet and can be changed in the future.
		virtual DevicePosition getPosition() const = 0;

		/// \brief Connect to the device
		///
		/// \param directConn - bool
		///
		/// \return GF_RET_CODE type to indicate if the command sent to device or not
		/// \remark The return value is not represent the connection state.\n
		///         The method may not work during Hub scanning.\n
		///         See HubListener::onDeviceConnected and HubListener::onDeviceDisconnected also.
		virtual GF_RET_CODE connect(bool directConn = true) = 0;

		/// \brief Disconnect from the device
		///
		/// \return GF_RET_CODE type to indicate if the command sent to device or not
		/// \remark The return value is not represent the connection state.\n
		///         The method may not work during Hub scanning.\n
		///         See HubListener::onDeviceConnected and HubListener::onDeviceDisconnected also.
		virtual GF_RET_CODE disconnect() = 0;

		/// \brief Cancel connecting action to the device
		///
		/// \return GF_RET_CODE type to indicate if the command sent to device or not
		/// \remark The return value is not represent the connection state,
		///         The method may not work during Hub scanning.\n
		///         See HubListener::onDeviceConnected and HubListener::onDeviceDisconnected also.
		virtual GF_RET_CODE cancelConnect() = 0;

	protected:
		/// \brief Virtual deconstructor
		///
		virtual ~Device() {}
	};

} // namespace gf
