#include"stdafx.h"
#include "DiscoveryService.h"
#include <iostream>
#include <list>
using namespace std;

/*DiscoveryService is used to scan LE device, and filter device by RSSI, device name and so on,
* report the filted device to upper level.
*/

OYM_Discovery_Service::OYM_Discovery_Service(OYM_NPI_Interface* minterface, OYM_CallBack* callback) : OYM_CallBack(DISCOVERY_SERVICE_EVENT_MASK)
{
	mInterface = minterface;
	mCallback = callback;
	mLog = new OYM_Log(MODUAL_TAG_DS, sizeof(MODUAL_TAG_DS));
	mNeedScan = OYM_FALSE;
}

OYM_Discovery_Service::~OYM_Discovery_Service()
{
	delete mLog;
}

OYM_STATUS OYM_Discovery_Service::Init()
{
	//register callback to receive event form NIF
	mInterface->RegisterCallback(this);
	
	return OYM_SUCCESS;
}

OYM_STATUS OYM_Discovery_Service::Deinit()
{
	if (mNeedScan == OYM_TRUE)
	{
		this->StopScan();
	}

	mInterface = NULL;

	return OYM_SUCCESS;
}

OYM_STATUS OYM_Discovery_Service::StartScan()
{
	OYM_STATUS result;
	LOGDEBUG("StartScan... \n");

	if (mInterface == NULL)
	{
		LOGERROR("StartScan failed, interface not available... \n");
		return OYM_ERR_CODE_NO_RESOURCE;
	}

	if (mNeedScan == OYM_TRUE)
	{
		LOGWARNING("StartScan failed, scan is ongoing... \n");
		return OYM_ERR_CODE_SCAN_BUSY;
	}
	
	result = mInterface->StartLEScan();
	if (result == OYM_SUCCESS)
	{
		mNeedScan = OYM_TRUE;
	}

	return result;
}

OYM_STATUS OYM_Discovery_Service::StopScan()
{
	OYM_STATUS result;
	if (mInterface == NULL)
	{
		LOGERROR("StartScan failed, interface not available... \n");
		return OYM_ERR_CODE_NO_RESOURCE;
	}

	if (mNeedScan == OYM_FALSE)
	{
		LOGWARNING("StartScan failed, scan is not ongoing... \n");
		return OYM_ERR_CODE_SCAN_BUSY;
	}

	result = mInterface->StopLEScan();
	if (result == OYM_SUCCESS)
	{
		mNeedScan = OYM_FALSE;
	}

	return result;
}

OYM_STATUS OYM_Discovery_Service::OnScanFinished()
{
	LOGDEBUG("Scan finished... \n");
	if (mNeedScan == OYM_TRUE)
	{
		//LOGDEBUG("restart scan... \n");
		mNeedScan = OYM_FALSE;
		if (0 != (mCallback->GetEventMask() & EVENT_MASK_INTERNAL_SCAN_FINISHED))
		{
			mCallback->OnScanFinished();
		}
	}

	return OYM_SUCCESS;
}

/*
* pursose : process adv data to check whether report new device to upper level
* return value:
*	LE_ADV_PROCESS_OK     //report new devie to upper level
*   LE_ADV_UNDISCOVERABEL //the device is not discoverabel, discard it
*   LE_ADV_WAIT_FOR_DEVICE_NAME //wait for device name
*/
OYM_STATUS OYM_Discovery_Service::CheckAdvData(OYM_PUINT8 data, OYM_UINT16 length)
{
	OYM_STATUS result = LE_ADV_WAIT_FOR_DEVICE_NAME;
	OYM_UINT8 adv_type;
	OYM_UINT16 remaining = length, offset = 0;
	OYM_UINT8 len;
	OYM_PUINT8 p = data;
	OYM_UINT8 ad_flag;

	while (remaining > 0)
	{
		len = p[offset];
		offset += 1; //skip len
		remaining--;
		adv_type = p[offset];
		LOGDEBUG("adv_type %d length = %d\n", adv_type, len);
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

OYM_BOOL OYM_Discovery_Service::GetDeviceName(OYM_PCHAR out, OYM_PUINT8 data, OYM_UINT16 length)
{
	
	OYM_BOOL result = OYM_FALSE;
	OYM_UINT8 adv_type;
	OYM_UINT16 remaining = length, offset = 0;
	OYM_UINT8 len;
	OYM_PUINT8 p = data;

	while (remaining > 0)
	{
		len = p[offset];
		offset += 1; //skip len
		remaining--;
		adv_type = p[offset];
		LOGDEBUG("adv_type %d length = %d\n", adv_type, len);
		if (adv_type == BLE_AD_TYPE_NAME_CMPL)
		{
			if (0 == memcmp(DEVICE_NAME_PATTERN, p + offset + 1, DEVICE_NAME_PATTERN_LENGTH))
			{
				memcpy(out, p + offset + 1, len - 1);
				out[len-1] = '\0';
				result = OYM_TRUE;
			}
			else
			{
				result = OYM_FALSE;
			}
			
		}

		offset += len;
		remaining -= len;
	}

	return result;
}

OYM_STATUS OYM_Discovery_Service::OnScanResult(OYM_PUINT8 data, OYM_UINT16 length)
{
	OYM_UINT16 i;
	list<BLE_DEVICE>::iterator ii;
	OYM_UINT8 adv_event_type = data[LE_ADV_EVENT_TYPE_OFFSET];//data[0]
	OYM_UINT8 bt_addr_type = data[LE_ADV_ADDRESS_TYPE_OFFSET]; //data[1]
	BLE_DEVICE bt_address(data + LE_ADV_ADDRESS_OFFSET, bt_addr_type); //data[2]
	OYM_UINT8 rssi = data[LE_ADV_RSSI_OFFSET]; //data[8]
	OYM_UINT8 adv_data_len = data[LE_ADV_DATA_LEN_OFFSET]; //data[9]
	OYM_PUINT8 adv_data = data + LE_ADV_DATA_OFFSET; //data[10]

	LOGDEBUG("the length of event is %d \n", length);

	OYM_UINT8 result = LE_ADV_WAIT_FOR_DEVICE_NAME;
	for (i = 0; i < length; i++)
	{
		LOGDEBUG("the data of [%d]th bytes is 0x%02x \n", i, data[i]);
	}

	if (LE_CONN_ADV == adv_event_type)
	{
		LOGDEBUG("the adv event type is Connectable Undirect Advertisement with RSSI %d\n", rssi);
		LOGDEBUG("the address  is 0x%02x:%02x:%02x:%02x:%02x:%02x \n", bt_address.addr[5], bt_address.addr[4], bt_address.addr[3], bt_address.addr[2], bt_address.addr[1], bt_address.addr[0]);
		if (rssi < LE_ADV_RSSI_THRESHOLD)
		{
			LOGDEBUG("The RSSI is too low. \n");
			return OYM_FAIL;
		}
		else
		{
			bt_address.rssi = rssi;
		}

		result = CheckAdvData(adv_data, adv_data_len);
		if (LE_ADV_UNDISCOVERABEL == result)
		{
			LOGDEBUG("device is not discoverabel... \n");
			return OYM_FAIL;
		}
		else if (LE_ADV_WAIT_FOR_DEVICE_NAME == result)
		{
			LOGDEBUG("wait for scan response... \n");
			mAvalbalDevice.push_front(bt_address);
			return OYM_SUCCESS;
		}
		else if (LE_ADV_PROCESS_OK == result)
		{
			LOGDEBUG("All things are OK, send device to upper level... \n");
			if (GetDeviceName(bt_address.dev_name, adv_data, adv_data_len) == OYM_TRUE)
			{
				LOGDEBUG("device name is:%s \n", bt_address.dev_name);
				if (0 != (mCallback->GetEventMask() & EVENT_MASK_INTERNAL_DEVICE_FOUND))
				{
					mCallback->OnDeviceFound((bt_address));
				}
				return OYM_SUCCESS;
			}
		}
	}
	else if (LE_SCAN_RESPONSE == adv_event_type)
	{
		LOGDEBUG("the adv event type is Scan Responsen with RSSI %d\n", rssi);
		LOGDEBUG("the address  is 0x%02x:%02x:%02x:%02x:%02x:%02x \n", bt_address.addr[5], bt_address.addr[4], bt_address.addr[3], bt_address.addr[2], bt_address.addr[1], bt_address.addr[0]);
		if (rssi < LE_ADV_RSSI_THRESHOLD)
		{
			LOGDEBUG("The RSSI is too low. \n");
			return OYM_FAIL;
		}

		//process scan response for device name
		result = CheckAdvData(adv_data, adv_data_len);
		LOGDEBUG("TCheckAdvData result is %d\n", result);
	}

	if (result == LE_ADV_PROCESS_OK)
	{
		//report to uplevel for new device.
		LOGDEBUG("all things are already, report to upper level... \n");
		for (ii = mAvalbalDevice.begin(); ii != mAvalbalDevice.end(); ii++)
		{
			if (((*ii).addr_type == bt_address.addr_type) && (0 == memcmp((*ii).addr, bt_address.addr, BT_ADDRESS_SIZE)))
			{
				memset((*ii).dev_name, 0, BLE_DEVICE_NAME_LENGTH);
				if ( GetDeviceName((*ii).dev_name, adv_data, adv_data_len) == OYM_TRUE)
				{
					LOGDEBUG("device name is:%s \n", (*ii).dev_name);
					if (0 != (mCallback->GetEventMask() & EVENT_MASK_INTERNAL_DEVICE_FOUND))
					{
						mCallback->OnDeviceFound((*ii));
					}
				}
			}
		}
	}

	return OYM_SUCCESS;
}