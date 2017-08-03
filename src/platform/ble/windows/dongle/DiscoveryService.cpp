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
#include "DiscoveryService.h"
#include <iostream>
#include <list>
using namespace std;

/*DiscoveryService is used to scan LE device, and filter device by RSSI, device name and so on,
* report the filted device to upper level.
*/

GF_CDiscoveryService::GF_CDiscoveryService(GF_CNpiInterface* minterface, GF_CCallBack* callback) : GF_CCallBack(DISCOVERY_SERVICE_EVENT_MASK, DISCOVERY_SERVICE_CALLBACK_INDEX)
{
	mInterface = minterface;
	mCallback = callback;
	mTag = MODUAL_TAG_DS;
	mIsScanning = GF_FALSE;
}

GF_CDiscoveryService::~GF_CDiscoveryService()
{
	mInterface = NULL;
	mCallback = NULL;
}

GF_STATUS GF_CDiscoveryService::Init()
{
	//register callback to receive event form NIF
	if (mInterface != NULL)
	{
		mInterface->RegisterCallback(this);
		return GF_OK;
	}
	else
	{
		return GF_FAIL;
	}
}

GF_STATUS GF_CDiscoveryService::Deinit()
{
	if (mIsScanning == GF_TRUE)
	{
		this->StopScan();
	}

	if (mInterface != NULL)
	{
		mInterface->UnRegisterCallback(this);
	}

	return GF_OK;
}

GF_STATUS GF_CDiscoveryService::StartScan(GF_UINT8 RSSI_Threshold)
{
	GF_STATUS result = GF_OK;
	LOGDEBUG(mTag, "StartScan... \n");

	if (mInterface == NULL)
	{
		LOGERROR(mTag, "StartScan failed, interface not available... \n");
		return GF_ERR_CODE_NO_RESOURCE;
	}

	if (mIsScanning == GF_TRUE)
	{
		LOGWARNING(mTag, "StartScan failed, scan is ongoing... \n");
		return GF_ERR_CODE_SCAN_BUSY;
	}
	
	result = mInterface->StartLEScan();
	if (result == GF_OK)
	{
		mIsScanning = GF_TRUE;
		mRSSI_Threshold = RSSI_Threshold;
	}
	LOGDEBUG(mTag, "StartScan... with result = %d\n", result);
	return result;
}

GF_STATUS GF_CDiscoveryService::StopScan()
{
	GF_STATUS result = GF_OK;
	LOGDEBUG(mTag, "StopScan... \n");

	if (mInterface == NULL)
	{
		LOGERROR(mTag, "StopScan failed, interface not available... \n");
		return GF_ERR_CODE_NO_RESOURCE;
	}

	if (mIsScanning == GF_FALSE)
	{
		LOGWARNING(mTag, "StopScan failed, scan is not ongoing... \n");
		return GF_ERR_CODE_SCAN_BUSY;
	}

	result = mInterface->StopLEScan();
	if (result == GF_OK)
	{
		mIsScanning = GF_FALSE;
	}

	return result;
}

GF_STATUS GF_CDiscoveryService::ProcessScanFinished()
{
	LOGDEBUG(mTag, "Scan finished... \n");
	mAvalbalDevice.clear();

	if (mIsScanning == GF_TRUE)
	{
		//LOGDEBUG(mTag, "restart scan... \n");
		mIsScanning = GF_FALSE;
		if ((mCallback != NULL) && (0 != (mCallback->GetEventMask() & EVENT_MASK_INTERNAL_SCAN_FINISHED)))
		{
			mCallback->OnEvent(EVENT_MASK_INTERNAL_SCAN_FINISHED, NULL, 0);
		}
	}

	return GF_OK;
}

/*
* pursose : process adv data to check whether report new device to upper level
* return value:
*	LE_ADV_PROCESS_OK     //report new devie to upper level
*   LE_ADV_UNDISCOVERABEL //the device is not discoverabel, discard it
*   LE_ADV_WAIT_FOR_DEVICE_NAME //wait for device name
*/
GF_STATUS GF_CDiscoveryService::CheckAdvData(GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = LE_ADV_WAIT_FOR_DEVICE_NAME;
	GF_UINT8 adv_type;
	GF_UINT16 remaining = length, offset = 0;
	GF_UINT8 len;
	GF_PUINT8 p = data;
	GF_UINT8 ad_flag;

	while (remaining > 0)
	{
		len = p[offset];
		/*if length of adv structure is more then remaining length, means some error happend, return avoid overflow.*/
		if (len > remaining)
		{
			break;
		}

		offset += 1; //skip len
		remaining--;
		adv_type = p[offset];
		LOGDEBUG(mTag, "adv_type %d length = %d\n", adv_type, len);
		switch (adv_type)
		{
			case BLE_AD_TYPE_FLAG:
				ad_flag = p[offset + 1];
				if (0 == (ad_flag & (BLE_LIMIT_DISC_FLAG | BLE_GEN_DISC_FLAG)))
				{
					return LE_ADV_UNDISCOVERABEL;
				}
				break;
			case BLE_AD_TYPE_16SRV_PART:
				//not process it for now
				break;
			case BLE_AD_TYPE_NAME_CMPL:
				result = LE_ADV_PROCESS_OK;
				break;
			case BLE_AD_TYPE_APPEARANCE:
				//not process it for now
				break;
		}

		offset += len;
		remaining -= len;
	}

	return result;
}

GF_BOOL GF_CDiscoveryService::GetDeviceName(GF_PCHAR out, GF_PUINT8 data, GF_UINT16 length)
{
	
	GF_BOOL result = GF_FALSE;
	GF_UINT8 adv_type;
	GF_UINT16 remaining = length, offset = 0;
	GF_UINT8 len;
	GF_PUINT8 p = data;

	while (remaining > 0)
	{
		len = p[offset];
		/*if length of adv structure is more then remaining length, means some error happend, return avoid overflow.*/
		if (len > remaining)
		{
			break;
		}

		offset += 1; //skip len
		remaining--;
		adv_type = p[offset];
		LOGDEBUG(mTag, "adv_type %d length = %d\n", adv_type, len);
		if (adv_type == BLE_AD_TYPE_NAME_CMPL)
		{
			//if (0 == memcmp(DEVICE_NAME_PATTERN, p + offset + 1, DEVICE_NAME_PATTERN_LENGTH))
			if (1)
			{
				memcpy(out, p + offset + 1, len - 1);
				out[len-1] = '\0';
				result = GF_TRUE;
			}
			else
			{
				result = GF_FALSE;
			}
			
			/*return here if we have found the device name.*/
			return result;
		}

		offset += len;
		remaining -= len;
	}

	return result;
}

GF_STATUS GF_CDiscoveryService::ProcessScanResult(GF_PUINT8 data, GF_UINT16 length)
{
	GF_UINT16 i;
	list<GF_BLEDevice>::iterator ii;
	GF_UINT8 adv_event_type = data[LE_ADV_EVENT_TYPE_OFFSET];//data[0]
	GF_UINT8 bt_addr_type = data[LE_ADV_ADDRESS_TYPE_OFFSET]; //data[1]
	GF_BLEDevice bt_address(data + LE_ADV_ADDRESS_OFFSET, bt_addr_type); //data[2]
	GF_UINT8 rssi = data[LE_ADV_RSSI_OFFSET]; //data[8]
	GF_UINT8 adv_data_len = data[LE_ADV_DATA_LEN_OFFSET]; //data[9]
	GF_PUINT8 adv_data = data + LE_ADV_DATA_OFFSET; //data[10]

	LOGDEBUG(mTag, "the length of event is %d \n", length);

	GF_UINT8 result = LE_ADV_WAIT_FOR_DEVICE_NAME;
	for (i = 0; i < length; i++)
	{
		LOGDEBUG(mTag, "the data of [%d]th bytes is 0x%02x \n", i, data[i]);
	}

	if (LE_CONN_ADV == adv_event_type)
	{
		LOGDEBUG(mTag, "the adv event type is Connectable Undirect Advertisement with RSSI %d\n", rssi);
		LOGDEBUG(mTag, "the address  is 0x%02x:%02x:%02x:%02x:%02x:%02x \n", bt_address.addr[5], bt_address.addr[4], bt_address.addr[3], bt_address.addr[2], bt_address.addr[1], bt_address.addr[0]);
		if (rssi < mRSSI_Threshold)
		{
			LOGDEBUG(mTag, "The RSSI is too low. \n");
			return GF_FAIL;
		}
		else
		{
			bt_address.rssi = rssi;
		}

		result = CheckAdvData(adv_data, adv_data_len);
		if (LE_ADV_UNDISCOVERABEL == result)
		{
			LOGDEBUG(mTag, "device is not discoverabel... \n");
			return GF_FAIL;
		}
		else if (LE_ADV_WAIT_FOR_DEVICE_NAME == result)
		{
			LOGDEBUG(mTag, "wait for scan response... \n");
			mAvalbalDevice.push_front(bt_address);
			return GF_OK;
		}
		else if (LE_ADV_PROCESS_OK == result)
		{
			LOGDEBUG(mTag, "All things are OK, send device to upper level... \n");
			if (GetDeviceName(bt_address.dev_name, adv_data, adv_data_len) == GF_TRUE)
			{
				LOGDEBUG(mTag, "device name is:%s \n", bt_address.dev_name);
				if ((mCallback != NULL) && (0 != (mCallback->GetEventMask() & EVENT_MASK_INTERNAL_DEVICE_FOUND)))
				{
					mCallback->OnDeviceFound((bt_address));
				}
				return GF_OK;
			}
		}
	}
	else if (LE_SCAN_RESPONSE == adv_event_type)
	{
		LOGDEBUG(mTag, "the adv event type is Scan Responsen with RSSI %d\n", rssi);
		LOGDEBUG(mTag, "the address  is 0x%02x:%02x:%02x:%02x:%02x:%02x \n", bt_address.addr[5], bt_address.addr[4], bt_address.addr[3], bt_address.addr[2], bt_address.addr[1], bt_address.addr[0]);
		if (rssi < mRSSI_Threshold)
		{
			LOGDEBUG(mTag, "The RSSI is too low. \n");
			return GF_FAIL;
		}

		//process scan response for device name
		result = CheckAdvData(adv_data, adv_data_len);
		LOGDEBUG(mTag, "CheckAdvData result is %d\n", result);
	}

	if (result == LE_ADV_PROCESS_OK)
	{
		//report to uplevel for new device.
		LOGDEBUG(mTag, "all things are already, report to upper level... \n");
		for (ii = mAvalbalDevice.begin(); ii != mAvalbalDevice.end(); ii++)
		{
			if (((*ii).addr_type == bt_address.addr_type) && (0 == memcmp((*ii).addr, bt_address.addr, BT_ADDRESS_SIZE)))
			{
				memset((*ii).dev_name, 0, BLE_DEVICE_NAME_LENGTH);
				if (GetDeviceName((*ii).dev_name, adv_data, adv_data_len) == GF_TRUE)
				{
					LOGDEBUG(mTag, "device name is:%s \n", (*ii).dev_name);
					if ((mCallback != NULL) && (0 != (mCallback->GetEventMask() & EVENT_MASK_INTERNAL_DEVICE_FOUND)))
					{
						mCallback->OnDeviceFound((*ii));
					}
				}
			}
		}
	}

	return GF_OK;
}

GF_STATUS GF_CDiscoveryService::OnEvent(GF_UINT32 event, GF_PUINT8 data, GF_UINT16 length)
{
	if (EVENT_MASK_GAP_SCAN_RESULT == event)
	{
		return ProcessScanResult(data, length);
	}
	else if (EVENT_MASK_GAP_SCAN_FINISHED == event)
	{
		return ProcessScanFinished();
	}
	else
	{
		/*not expected event.*/
		return GF_FAIL;
	}
}