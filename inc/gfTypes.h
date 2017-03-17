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

#if defined(UNICODE) || defined(_UNICODE)

#define tstring std::wstring
#define tchar wchar_t
#define tcout std::wcout
#define tcin std::wcin
#ifndef _T
#define _T(x) L##x
#endif

#else // UNICODE

$define tstring std::string
#define tchar char
#define tcout std::cout
#define tcin std::cin
#ifndef _T
#define __T(x) x
#endif

#endif // UNICODE

//----------------------------------------------------------
// gfwPtr, gfsPtr
//----------------------------------------------------------
template <typename T>
using gfwPtr = std::weak_ptr<T>;
template <typename T>
using gfsPtr = std::shared_ptr<T>;

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
} GF_EVENT;


enum class DongleState{
	Idle,
	Scanning,
	Connecting,
	Unknown
} ;

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


} // namespace oym
