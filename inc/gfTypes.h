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


#include <iostream>
#include <string>
#include <memory>

/// \namespace gf
namespace gf {

	typedef char				GF_CHAR;
	typedef char*				GF_PCHAR;
	typedef const char*			GF_CPCHAR;
	typedef unsigned int		GF_UINT;
	typedef int					GF_INT;
	typedef float				GF_FLOAT;

	typedef unsigned char		GF_UINT8;
	typedef unsigned char*		GF_PUINT8;
	typedef unsigned short		GF_UINT16;
	typedef unsigned short*		GF_PUINT16;
	typedef unsigned int		GF_UINT32;
	typedef unsigned long long	GF_UINT64;

	typedef size_t				GF_SIZE;
	typedef GF_INT				GF_STATUS;


	const static GF_UINT16 INVALID_HANDLE = 0xFFFF;

#if defined(UNICODE) || defined(_UNICODE)

#define tstring std::wstring
#define tchar wchar_t
#if ! defined(WIN32) || ! defined(_T)
#define _T(x) L ## x
#endif

#else // UNICODE

#define tstring std::string
#define tchar char
#if ! defined(WIN32) || ! defined(_T)
#define __T(x) x
#endif

#endif // UNICODE

	//----------------------------------------------------------
	// gfwPtr, gfsPtr
	//----------------------------------------------------------
	template <typename T>
	using gfwPtr = std::weak_ptr < T > ;
	template <typename T>
	using gfsPtr = std::shared_ptr < T > ;

	class Device;
	typedef gfwPtr<Device> WPDEVICE;
	typedef void(*FunEnumDevice)(WPDEVICE);

	// Freerun mode: callbacks are called in the callers' threads
	// ClientThread mode: callbacks are called in the client thread given by method Hub::run
	enum class WorkMode {
		Freerun,
		ClientThread,
	};

	enum class GF_RET_CODE : GF_UINT32{
		GF_SUCCESS = 0,
		GF_ERROR,
		GF_ERROR_BAD_PARAM,
		GF_ERROR_BAD_STATE,
		GF_ERROR_NOT_SUPPORT,
		GF_ERR_SCAN_BUSY,
		GF_ERR_NO_RESOURCE,
		GF_ERROR_TIMEOUT,
	};


	typedef enum {
		GF_EVT_EVENTBASE,
		GF_EVT_DEVICE_RECENTER = GF_EVT_EVENTBASE,
		GF_EVT_DATA_GESTURE,
		GF_EVT_DATA_QUATERNION,
	} GF_EVENT_TYPE;
	
	enum class Gesture : GF_UINT8 {
		Relax = 0x00,
		Gist = 0x01,
		SpreadFingers = 0x02,
		WaveTowardPalm = 0x03,
		WaveBackwardPalm = 0x04,
		TuckFingers = 0x05,
		Shoot = 0x06,
		Unknown = 0xFF
	};

	enum class HubState{
		Idle,
		Scanning,
		Connecting,
		Unknown
	};

	enum class DeviceConnectionStatus {
		Disconnected,
		Disconnecting,
		Connecting,
		Connected
	};

	enum class DevicePosition {
		NotAssigned,
		RightArm,
		LeftArm,
		RightWrist,
		LeftWrist,
		Virtual
	};

	enum class AttributeHandle : GF_UINT16 {
		Max,
	};
} // namespace gf
