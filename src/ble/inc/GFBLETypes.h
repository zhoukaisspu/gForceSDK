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
#ifndef __GF_TYPES_H__
#define __GF_TYPES_H__
#pragma once

#include <Windows.h>

#include "gfTypes.h"
using namespace gf;

typedef void				GF_VOID;
typedef GF_INT				GF_BOOL;
#define GF_FALSE			((GF_BOOL)0)
#define GF_TRUE				(GF_BOOL)(!(GF_FALSE))

#define GF_OK				((GF_INT)0)
#define GF_FAIL				((GF_INT)1)
#define GF_SUCCEEDED		(GF_OK == status)

#define BT_ADDRESS_SIZE		6
#define GF_AUTH_FAIL		0x1000

typedef enum {
	GF_ERR_CODE_SCAN_BUSY = 0x60,
	GF_ERR_CODE_NO_RESOURCE = 0x61,
} GF_ERROR_CODE;

#define BLE_DEVICE_NAME_LENGTH 30
struct GF_BLEDevice{
	GF_UINT8   addr_type;
	GF_UINT8	addr[BT_ADDRESS_SIZE];
	GF_CHAR	dev_name[BLE_DEVICE_NAME_LENGTH];
	GF_UINT8   rssi;
	GF_BLEDevice(GF_UINT8 bytes[], GF_UINT8 type)
	{
		for (int i = BT_ADDRESS_SIZE - 1; i >= 0; --i) {
			this->addr[i] = bytes[i];
		}
		addr_type = type;
	}

};

#define	LOGTYPE_CONSOLE 1
#define	LOGTYPE_MFC     2
#define	LOGTYPE_FILE    3
#define	LOGTYPE_MAX     3

class GF_CCallBack
{
public:
	GF_CCallBack(GF_UINT32 eventmask, GF_UINT8 index) :mEventMask(eventmask), mIndex(index){}
	virtual GF_STATUS OnDeviceFound(GF_BLEDevice new_device) = 0;
	virtual GF_STATUS OnEvent(GF_UINT32 event, GF_PUINT8 data, GF_UINT16 length) = 0;

	GF_UINT32 GetEventMask()
	{
		return mEventMask;
	}

	GF_UINT8 GetIndex()
	{
		return mIndex;
	}

private:
	GF_UINT32 mEventMask;//which event is intent to process.
	GF_UINT8 mIndex;
};

typedef enum {
	State_Idle = 0,
	State_Scanning = 1,
	State_Connecting = 2,
}GF_HubState;

struct GF_ConnectedDevice
{
	GF_UINT8 address_type;
	GF_UINT8 address[BT_ADDRESS_SIZE];
	GF_UINT16 handle;
	GF_UINT16 conn_int;
	GF_UINT16 superTO;
	GF_UINT16 slavelatency;
	GF_UINT16 MTUsize;
};
#endif