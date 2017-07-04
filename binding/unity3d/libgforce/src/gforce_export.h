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
/// \remark In Windows, level represents enum GF_LOG_LEVEL.
///			In Android, level represents enum android_LogPriority.
typedef void(*FunClientLog)(gf::GF_UINT level, const gf::GF_PCHAR log);

#pragma pack(push,1)
/// \brief The C style version of class HubListener
///
struct ListenerCalls
{
	void(*onScanFinished)() = nullptr;
	void(*onStateChanged)(gf::GF_UINT state) = nullptr;
	void(*onDeviceFound)(gf::GF_HANDLE device) = nullptr;
	void(*onDeviceDiscard)(gf::GF_HANDLE device) = nullptr;
	void(*onDeviceConnected)(gf::GF_HANDLE device) = nullptr;
	void(*onDeviceDisconnected)(gf::GF_HANDLE device, gf::GF_UINT reason) = nullptr;
	void(*onOrientationData)(gf::GF_HANDLE device, float w, float x, float y, float z) = nullptr;
	void(*onGestureData)(gf::GF_HANDLE device, gf::GF_UINT gest) = nullptr;
	void(*onReCenter)(gf::GF_HANDLE device) = nullptr;
};
#pragma pack(pop)

// Log from client

/// \brief Re-direct log output
///
/// \param FunClientLog the client log out method
/// \remark
///             The default log out is the stdio, this method allows client to redirect it
///             Set the param to nullptr to reset log method
GFORCE4CS_API void gf_set_log_method(FunClientLog);

// hub

/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_instance(const gf::GF_PCHAR identifier);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_init(gf::GF_UINT comport);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_deinit();
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_set_workmode(gf::GF_UINT newMode);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_get_workmode();
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_get_status();
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_register_listener(ListenerCalls* lis);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_unregister_listener(ListenerCalls* lis);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_start_scan();
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_stop_scan();
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_SIZE hub_get_num_devices(bool bConnectedOnly);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_enum_devices(FunEnumDevice fn, bool bConnectedOnly);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT hub_run(gf::GF_UINT ms);

// device

/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT device_get_addr_type(gf::GF_HANDLE device);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT device_get_address(gf::GF_HANDLE device, gf::GF_PCHAR addr, gf::GF_SIZE buflen);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT device_get_name(gf::GF_HANDLE device, gf::GF_PCHAR name, gf::GF_SIZE buflen);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT device_get_connection_status(gf::GF_HANDLE device);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT device_connect(gf::GF_HANDLE device);
/// \brief Get the Hub instance
///
/// \param sIdentifier a tstring to tell who wants to use the Hub instance
/// \return The Hub instance
/// \remark
///             The life cycle of the Hub instance starts in the first call to this method and until the app exits
GFORCE4CS_API gf::GF_UINT device_disconnect(gf::GF_HANDLE device);

