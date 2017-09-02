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
  * \file gforce_export.h
  * \brief API collection for C# calling
  *
  * \version 0.1
  * \date 2017.7.5
  */
#pragma once

#ifdef WIN32

#ifdef GFORCE4CS_EXPORTS
#define GFORCE4CS_API __declspec(dllexport)
#else
#define GFORCE4CS_API __declspec(dllimport)
#endif

#else // #ifdef WIN32

#ifdef GFORCE4CS_EXPORTS
#define GFORCE4CS_API extern "C" __attribute__ ((visibility ("default")))
#else
#define GFORCE4CS_API __attribute__ ((visibility ("hidden")))
#endif

#endif


#include "gfTypes.h"

  /// \brief The callback function pointer type for device enumeration
  ///
  /// \param handle A device handle indicates the enumeration result
  /// \return The return value tells gForceSDK if client wants to continue
  ///			True if client wants to continue enumeration, otherwise stop it
typedef bool(*FunEnumDevice)(gf::GF_HANDLE handle);

/// \brief The function pointer type for receiving gForceSDK log to a 3rd party envrionment
///
/// \param level Log output level.
/// \param log The log content
/// \remark In Windows, level represents enum GF_LOG_LEVEL.\n
///			In Android, level represents enum android_LogPriority.
typedef void(*FunClientLog)(gf::GF_UINT level, const gf::GF_PCHAR log);

#pragma pack(push,1)
/// \brief The C style version of class HubListener
///
struct ListenerCalls
{
	/// \brief This callback is called when the Hub finishes scanning devices.
	/// \remark
	///     This callback may be called after a series of onDeviceFound()
	///     are called.
	void(*onScanFinished)() = nullptr;
	/// \brief This callback is called when the state of the hub changed
	/// \param state
	///     An enumerated value of HubState which indicates the state of the
	///     hub.
	void(*onStateChanged)(gf::GF_UINT state) = nullptr;
	/// \brief
	///     This callback is called when the hub finds a device during
	///     scanning.
	/// \param device The handle of a Device that was found.
	void(*onDeviceFound)(gf::GF_HANDLE device) = nullptr;
	/// \brief
	///     This callback is called when a previously found but not
	///     connected device has been dropped by the Hub.
	/// \param device
	///     The handle of a Device that was previously found and
	///     passed to the application.
	void(*onDeviceDiscard)(gf::GF_HANDLE device) = nullptr;
	/// \brief
	///     This callback is called when a device has been connected to
	///     the hub successfully.
	/// \param device
	///     The handle of a Device that the hub has connected to.
	void(*onDeviceConnected)(gf::GF_HANDLE device) = nullptr;
	/// \brief
	///     This callback is called when a device has been disconnected from
	///     connection state or failed to connect to
	/// \param device
	///     The handle of a Device that was disconnected.
	/// \param reason The reason of why device disconnected.
	void(*onDeviceDisconnected)(gf::GF_HANDLE device, gf::GF_UINT reason) = nullptr;
	/// \brief
	///     This callback is called when the orientation of the device has
	///     changed.
	/// \param device
	///     The handle of a Device sending data.
	/// \param w
	/// \param x
	/// \param y
	/// \param z
	void(*onOrientationData)(gf::GF_HANDLE device, float w, float x, float y, float z) = nullptr;
	/// \brief This callback is called when the gesture data is recevied
	/// \param device The handle of a Device
	/// \param gest The Gesture object.
	void(*onGestureData)(gf::GF_HANDLE device, gf::GF_UINT gest) = nullptr;
	/// \brief
	///     This callback is called when the device status has been changed
	///     device.
	/// \param device The handle of a Device sending data
	/// \param status What status of the device has been changed
	/// \sa gf::HubListener::onDeviceStatusChanged
	void(*onDeviceStatusChanged)(gf::GF_HANDLE device, gf::GF_UINT status) = nullptr;
	/// \brief
	///     This callback is called when the extend data is recevied
	/// \param device The handle of a Device sending data
	/// \param dataType The data type carried
	/// \param dataLength Length of data
	/// \param data The pointer to the data buffer
	/// \sa gf::HubListener::onExtendDeviceData
	void(*onExtendDeviceData)(gf::GF_HANDLE device, gf::GF_UINT dataType, gf::GF_INT dataLength, const gf::GF_UINT8* data) = nullptr;
};
#pragma pack(pop)

// Log from client

/// \brief Re-direct log output
///
/// \param logMethod the client log out method
/// \remark
///             The default log out is the stdio, this method allows client to redirect it
///             Set the param to nullptr to reset log method
GFORCE4CS_API void gf_set_log_method(FunClientLog logMethod);

// hub

/// \brief Get the Hub instance
///
/// \param identifier a client identifier
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_instance(const gf::GF_PCHAR identifier);
/// \brief Initialize the Hub instance
///
/// \param comport Specify the COM port the physical Hub is connecting with.
///                0 means to enumuate COM ports and find one automatically.
/// \return The result of initalization.
/// \remark Possible conditions that initialize may fail.
///   1. The hub is not plugged in the USB port.
///   2. Other apps are connected to the hub already.
GFORCE4CS_API gf::GF_UINT hub_init(gf::GF_UINT comport);
/// \brief De-initialize the Hub instance
///
/// \return The result of de-initalization.
/// \remark During de-initialization, all gForce devices will be disconnected and dropped.
GFORCE4CS_API gf::GF_UINT hub_deinit();
/// \brief Set work mode
///
/// \return The current work mode
/// \remark Default work mode is Polling.
/// \sa
///         hub_run\n
///         gf::WorkMode
GFORCE4CS_API gf::GF_UINT hub_set_workmode(gf::GF_UINT newMode);
/// \brief Get work mode
///
/// \return The current work mode
/// \remark Default work mode is Polling
/// \sa
///         hub_run\n
///         gf::WorkMode
GFORCE4CS_API gf::GF_UINT hub_get_workmode();
/// \brief Get hub state
///
/// \return The current hub state
GFORCE4CS_API gf::GF_UINT hub_get_status();
/// \brief Register listener to the Hub
///
/// \param lis The client listener to be registered
/// \return The result of registration.
GFORCE4CS_API gf::GF_UINT hub_register_listener(ListenerCalls* lis);
/// \brief Un-register HubListener
///
/// \param lis The client listener to be un-registered
/// \return The result of un-registration.
GFORCE4CS_API gf::GF_UINT hub_unregister_listener(ListenerCalls* lis);
/// \brief Start gForce scan
///
/// \return GF_RET_CODE type to indicate if the command sent to hub or not
GFORCE4CS_API gf::GF_UINT hub_start_scan();
/// \brief Stop gForce scan
///
/// \return GF_RET_CODE type to indicate if the command succeeded or not
GFORCE4CS_API gf::GF_UINT hub_stop_scan();
/// \brief Get the number of all devices found
///
/// \param bConnectedOnly true if only want to get number of connected devices,\n
/// false if want to get all devices.
/// \return The Number of all devices
GFORCE4CS_API gf::GF_SIZE hub_get_num_devices(bool bConnectedOnly);
/// \brief Enumurate devices
///
/// \param fn The enumurate function
/// \param bConnectedOnly true if only want to get connected devices,\n
/// false if want to get all devices.
/// \remark The return value of funEnum tells gForce if client wants to continue next enumerate
/// true to continue enumerate, otherwise stop it.
GFORCE4CS_API gf::GF_UINT hub_enum_devices(FunEnumDevice fn, bool bConnectedOnly);
/// \brief run message polling
///
/// \param ms The method will return after ms milliseconds. No matter messages are processed or not.
/// \return Possible return values:
///       1. GF_RET_CODE::GF_ERROR_BAD_STATE: the method already been called in other threads, or not in proper WorkMode.
///       2. GF_RET_CODE::GF_ERROR_TIMEOUT: ms milliseconds expired.
///       3. GF_RET_CODE::GF_SUCCESS: return with no error.
/// \sa gf::WorkMode\n
///     hub_set_workmode\n
///     hub_get_workmode
GFORCE4CS_API gf::GF_UINT hub_run(gf::GF_UINT ms);

// device

/// \brief Gets the device address type.
///
/// \param device the device handle
/// \return The device address type is defined in BLE.
GFORCE4CS_API gf::GF_UINT device_get_addr_type(gf::GF_HANDLE device);
/// \brief Gets the device address string.
///
/// \param device the device handle
/// \param addr the buffer to receive human-readable address string
/// \param buflen length of addr in bytes
/// \return GF_RET_CODE type to indicate if the command succeeded or not
/// \remark
///     The human-readable string type of device address, e.g.
///     `DD:DD:DD:DD:DD:DD`
GFORCE4CS_API gf::GF_UINT device_get_address(gf::GF_HANDLE device, gf::GF_PCHAR addr, gf::GF_SIZE buflen);
/// \brief Gets the device name.
///
/// \param device the device handle
/// \param name the buffer to receive string of device name
/// \param buflen length of name in bytes
/// \return GF_RET_CODE type to indicate if the command succeeded or not
GFORCE4CS_API gf::GF_UINT device_get_name(gf::GF_HANDLE device, gf::GF_PCHAR name, gf::GF_SIZE buflen);
/// \brief Gets the device RSSI.
///
/// \param device the device handle
/// \return the RSSI value
/// \remark Currently the return value indicates the RSSI while device is found during scan
GFORCE4CS_API gf::GF_UINT device_get_rssi(gf::GF_HANDLE device);
/// \brief Retrieves the connection status of the device.
///
/// \param device the device handle
/// \return DeviceConnectionStatus type of device status.
GFORCE4CS_API gf::GF_UINT device_get_connection_status(gf::GF_HANDLE device);
/// \brief Connects to the device
///
/// \param device the device handle
/// \return
///     GF_RET_CODE type to indicate if the command has been sent to
///     device or not.
/// \remark
///     The return value is not represent the connection state.
///     The method may not work during Hub scanning.
/// \sa
///     device_disconnect()\n
///     ListenerCalls::onDeviceConnected()\n
///     ListenerCalls::onDeviceDisconnected()
GFORCE4CS_API gf::GF_UINT device_connect(gf::GF_HANDLE device);
/// \brief Disconnects from the device
///
/// \param device the device handle
/// \return
///     GF_RET_CODE type to indicate if the command has been sent to
///     device or not.
/// \remark
///     The return value is not represent the connection state.
///     The method may not work during Hub scanning.
/// \sa
///     device_connect()\n
///     ListenerCalls::onDeviceConnected()\n
///     ListenerCalls::onDeviceDisconnected()
GFORCE4CS_API gf::GF_UINT device_disconnect(gf::GF_HANDLE device);

