#pragma once


#include <iostream>
#include <string>
#include <memory>

namespace oym {

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
#ifndef _T
#define _T(x) L ## x
#endif

#else // UNICODE

#define tstring std::string
#define tchar char
#ifndef _T
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


	typedef enum {
		GF_SUCCESS = 0,
		GF_ERROR,
		GF_ERROR_BAD_PARAM,
		GF_ERROR_BAD_STATE,
		GF_ERROR_NOT_SUPPORT,
		GF_ERR_SCAN_BUSY,
		GF_ERR_NO_RESOURCE,
	} GF_RET_CODE;


	typedef enum {
		GF_EVT_EVENTBASE,
		GF_EVT_DEVICE_RECENTER,
		GF_EVT_DATA_GESTURE,
		GF_EVT_DATA_QUATERNION,
	} GF_EVENT;


	enum class AdapterState{
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

	// define Characteristic read/write handle
	enum class AttributeHandle : GF_UINT16 {
		GATTPrimServiceDeclaration1 = 0x0001,
		GATTCharacteristicDeclaration1,
		DeviceName,
		GATTCharacteristicDeclaration2,
		Appearance,
		GATTCharacteristicDeclaration3,
		PreferredConnectParamters,
		GATTPrimServiceDeclaration2,
		GATTPrimServiceDeclaration3,
		GATTCharacteristicDeclaration4,
		SystemID,
		GATTCharacteristicDeclaration5,
		ModelNumberStr,
		GATTCharacteristicDeclaration6,
		SerialNumberStr,
		GATTCharacteristicDeclaration7,
		FirmwareRevStr,
		GATTCharacteristicDeclaration8,
		HardwareRevStr,
		GATTCharacteristicDeclaration9,
		SoftwareRevStr,
		GATTCharacteristicDeclaration10,
		ManufactureNameStr,
		GATTCharacteristicDeclaration11,
		IEEE11073_20601,
		GATTCharacteristicDeclaration12,
		PnPID,
		GATTPrimServiceDeclaration4,
		GATTCharacteristicDeclaration13,
		Max
	};

	static const char charTypes[] =
		"BBSBBBBBBBBBSBSBSBSBSBSBBBBBBN";

} // namespace oym
