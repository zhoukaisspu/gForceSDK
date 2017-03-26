#pragma once
#include "GFBLETypes.h"
//define com port number
#define COM_PORT_NUM 3

#define LOCAL_GATT_CLIENT_MTU_SIZE 23

#define STD_OUT

class GF_CDevice
{
public:
	GF_UINT8 address_type;
	GF_UINT8 address[6];
	GF_UINT8 name[50];
	GF_UINT8 cod;
	GF_UINT8 state;
};
