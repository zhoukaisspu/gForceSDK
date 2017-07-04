/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
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
	///    The abstract class of a gForce device.
	/// \remark
    ///     All Device objects are created and maintained by the Hub instance
	///
	class Device
	{
	public:
		/// \brief Gets the device address type.
		/// \return The device address type is defined in BLE.
		virtual GF_UINT8 getAddrType() const = 0;

		/// \brief Gets the string type of device address.
		/// \return
        ///     The human-readable tstring type of device address, e.g.
        ///     `DD:DD:DD:DD:DD:DD`
		virtual tstring getAddress() const = 0;

		/// \brief Gets the device name.
		///
		/// \return The tstring type of device name
		virtual tstring getName() const = 0;

		/// \brief
        ///     Identifies a physical device using vibration or light, if the
        ///     device supports.
		/// \remark
		///     This method is not implemented yet and can be changed in the
        ///     future.
		/// \return
        ///     A GF_RET_CODE type of return value to indicate if the identify
        ///     command is sent or not.
		virtual GF_RET_CODE identify(int msec = 1000) = 0;

		/// \brief Retrieves the connection status of the device.
		/// \return DeviceConnectionStatus type of device status.
		virtual DeviceConnectionStatus getConnectionStatus() const = 0;

		/// \brief Sets the wear position of the device
		/// \remark
		///     This method is not implemented yet and can be changed in the
        ///     future.
        /// \sa
        ///  Device::getPostion()
		virtual GF_RET_CODE setPostion(DevicePosition pos) = 0;

		/// \brief Get the wear position of the device set before
		///
		/// \remark
		///     This method is not implemented yet and can be changed in the
        ///     future.
        /// \sa
        ///  Device::setPostion()
		virtual DevicePosition getPosition() const = 0;

		/// \brief Connects to the device
		/// \param directConn - bool
		/// \return
        ///     GF_RET_CODE type to indicate if the command has been sent to
        ///     device or not.
		/// \remark
        ///     The return value is not represent the connection state.
		///     The method may not work during Hub scanning.
		/// \sa
        ///     Device::disconnect()
        ///     Device::cancelConnect()
        ///     HubListener::onDeviceConnected()
        ///     HubListener::onDeviceDisconnected()
		virtual GF_RET_CODE connect(bool directConn = true) = 0;

		/// \brief Disconnects from the device
        /// \return
        ///     GF_RET_CODE type to indicate if the command has been sent to
        ///     device or not.
		/// \remark
        ///     The return value is not represent the connection state.
		///     The method may not work during Hub scanning.
		/// \sa
        ///     Device::connect()
        ///     Device::cancelConnect()
        ///     HubListener::onDeviceConnected()
        ///     HubListener::onDeviceDisconnected()
		virtual GF_RET_CODE disconnect() = 0;

		/// \brief Cancels connecting to the device
        /// \return
        ///     GF_RET_CODE type to indicate if the command has been sent to
        ///     device or not.
		/// \remark
        ///     The return value is not represent the connection state.
		///     The method may not work during Hub scanning.
		/// \sa
        ///     Device::connect()
        ///     Device::disconnect()
        ///     HubListener::onDeviceConnected()
        ///     HubListener::onDeviceDisconnected()
		virtual GF_RET_CODE cancelConnect() = 0;

	protected:
		/// \brief Virtual deconstructor
		///
		virtual ~Device() {}
	};

} // namespace gf
