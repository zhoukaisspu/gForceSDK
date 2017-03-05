#ifndef __OYM_TYPES_H__
#define __OYM_TYPES_H__
#pragma once

#include <Windows.h>

typedef void				OYM_VOID;
typedef char				OYM_CHAR;
typedef char*				OYM_PCHAR;
typedef const char*			OYM_CPCHAR;
typedef unsigned int		OYM_UINT;
typedef int					OYM_INT;
typedef long				OYM_LONG;
typedef unsigned long		OYM_ULONG;
typedef float				OYM_FLOAT;

typedef unsigned char		OYM_UINT8;
typedef unsigned char*		OYM_PUINT8;
typedef unsigned short		OYM_UINT16;
typedef unsigned short*		OYM_PUINT16;
typedef unsigned int		OYM_UINT32;
typedef unsigned long long	OYM_UINT64;

typedef OYM_INT				OYM_BOOL;
#define OYM_FALSE			((OYM_BOOL)0)
#define OYM_TRUE			(OYM_BOOL)(!(OYM_FALSE))

typedef size_t				OYM_SIZE;
typedef HANDLE				OYM_HANDLE;


typedef OYM_INT				OYM_STATUS;
#define OYM_SUCCESS			((OYM_INT)0)
#define OYM_FAIL			((OYM_INT)1)
#define OYM_SUCCEEDED		(OYM_SUCCESS == status)

#define BT_ADDRESS_SIZE		6
typedef OYM_UINT8			BT_Address_Type;

typedef enum {
	OYM_ERR_CODE_SCAN_BUSY = 0x60,
	OYM_ERR_CODE_NO_RESOURCE = 0x61,
} OYM_ERROR_CODE;

#define BLE_DEVICE_NAME_LENGTH 30
struct BLE_DEVICE{
	OYM_UINT8   addr_type;
	OYM_UINT8	addr[BT_ADDRESS_SIZE];
	OYM_CHAR	dev_name[BLE_DEVICE_NAME_LENGTH];
	OYM_UINT8   rssi;
	BLE_DEVICE(OYM_UINT8 bytes[], OYM_UINT8 type)
	{
		for (int i = BT_ADDRESS_SIZE - 1; i >= 0; --i) {
			this->addr[i] = bytes[i];
		}
		addr_type = type;
	}

};
#endif