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
 * \file gfTypes.h
 * \brief The basic type definitions using in gForceSDK
 *
 * \version 0.1
 * \date 2017.4.3
 */
#pragma once


#include <iostream>
#include <string>
#include <memory>

#if defined(WIN32)
#include "tchar.h"
#endif

namespace gf {

	/// type definition
	typedef char				GF_CHAR;
	/// type definition
	typedef char*				GF_PCHAR;
	/// type definition
	typedef const char*			GF_CPCHAR;
	/// type definition
	typedef unsigned int		GF_UINT;
	/// type definition
	typedef int					GF_INT;
	/// type definition
	typedef float				GF_FLOAT;

	/// type definition
	typedef unsigned char		GF_UINT8;
	/// type definition
	typedef unsigned char*		GF_PUINT8;
	/// type definition
	typedef unsigned short		GF_UINT16;
	/// type definition
	typedef unsigned short*		GF_PUINT16;
	/// type definition
	typedef unsigned int		GF_UINT32;
	/// type definition
	typedef unsigned long long	GF_UINT64;

	/// type definition
	typedef size_t				GF_SIZE;
	/// type definition
	typedef GF_INT				GF_STATUS;

	typedef void*				GF_HANDLE;

#if defined(UNICODE) || defined(_UNICODE)

	/// std::string type definition for ANSI/UNICODE compatible 
	using tstring = std::wstring;
	/// char type definition for ANSI/UNICODE compatible 
	typedef wchar_t tchar;
#if ! defined(WIN32) || ! defined(_T)
	///
#define _T(x) L ## x
#endif

#else // UNICODE

	/// std::string type definition for ANSI/UNICODE convenience 
	using tstring = std::string;
	/// char type definition for ANSI/UNICODE convenience 
	typedef char tchar;
#if ! defined(WIN32) || ! defined(_T)
	/// ANSI/UNICODE convenience
#define _T(x) x
#endif

#endif // UNICODE

	/// The weak pointer
	template <typename T>
	using gfwPtr = std::weak_ptr < T > ;
	/// The strong pointer
	template <typename T>
	using gfsPtr = std::shared_ptr < T > ;

	class Device;
	/// the weak pointer to Device
	typedef gfwPtr<Device> WPDEVICE;
	/// the strong pointer to Device
	typedef gfsPtr<Device> SPDEVICE;

	/// \brief Defines how callbacks are called in threads
	///
	enum class WorkMode {
		/// Callbacks are called in the message senders' threads
		/// \remark
		/// 1. Callbacks are called in various threads.
		/// 2. No need to call Hub::run, the method will return immediately in this mode.
		Freerun,
		/// Callbacks are called in the client thread given by method Hub::run
		/// \remark
		/// Client has to call Hub::run to pull messages (represent as callbacks),
		/// otherwise messages are blocking in an inner queue without been handled.
		Polling,
	};

	/// \brief Defines possible return values of methods
	///
	enum class GF_RET_CODE : GF_UINT32{
		/// Method returns successfully.
		GF_SUCCESS = 0,
		/// Method returns with a generic error.
		GF_ERROR,
		/// Given parameters are not match required.
		GF_ERROR_BAD_PARAM,
		/// Method call is not allowed by the inner state.
		GF_ERROR_BAD_STATE,
		/// Method is not supported at this time.
		GF_ERROR_NOT_SUPPORT,
		/// Hub is busying on device scan and cannot fulfill the call.
		GF_ERR_SCAN_BUSY,
		/// Insufficient resource to perform the call.
		GF_ERR_NO_RESOURCE,
		/// A preset timer is expired.
		GF_ERROR_TIMEOUT,
	};

	/// \brief Gesture types predefined in the gForce device
	///
	enum class Gesture : GF_UINT8 {
		Relax = 0x00,
		Gist = 0x01,
		SpreadFingers = 0x02,
		WaveTowardPalm = 0x03,
		WaveBackwardPalm = 0x04,
		TuckFingers = 0x05,
		Shoot = 0x06,
		Undefined = 0xFF
	};

	/// \brief Possible Hub states
	///
	enum class HubState{
		Idle,
		Scanning,
		Connecting,
		Disconnected,
		Unknown
	};

	/// \brief Possible gForce device connection status
	///
	enum class DeviceConnectionStatus {
		Disconnected,
		Disconnecting,
		Connecting,
		Connected
	};

	/// \brief Define where the gForce device is weared.
	///
	/// \remark Not implmented yet.
	enum class DevicePosition {
		NotAssigned,
		RightArm,
		LeftArm,
		RightWrist,
		LeftWrist,
		Virtual
	};

} // namespace gf
