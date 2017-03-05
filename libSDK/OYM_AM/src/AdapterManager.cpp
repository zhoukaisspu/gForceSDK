#include "stdafx.h"
#include "AdapterManager.h"
#include "RemoteDevice.h"

OYM_AdapterManager::OYM_AdapterManager() :OYM_CallBack(ADAPTER_MANAGER_EVENT, ADAPTER_MANAGER_CALLBACK_INDEX)
{
	mInterface = new OYM_NPI_Interface;
	mLog = new OYM_Log(MODUAL_TAG_AM, sizeof(MODUAL_TAG_AM));
	mDS = new OYM_Discovery_Service(mInterface, this);
	mIsConnecting = OYM_FALSE;
	mIsScanning = OYM_FALSE;
}

OYM_AdapterManager::~OYM_AdapterManager()
{
	delete mLog;
	delete mDS;
	delete mInterface;
	mInterface = NULL;
	mDS = NULL;
	mLog = NULL;
}

OYM_STATUS OYM_AdapterManager::Init()
{
	OYM_STATUS result = OYM_FAIL;

	if (mInterface != NULL)
	{
		result = mInterface->Init();
	}
	else
	{
		return result;
	}
	
	if (result == OYM_SUCCESS)
	{
		//register to receive event form NIF
		mInterface->RegisterCallback(this);
		mInterface->InitDevice();
	}
	else
	{
		return result;
	}
	
	if (mDS != NULL)
	{
		result = mDS->Init();
	}

	return result;
}

OYM_STATUS OYM_AdapterManager::Deinit()
{
	OYM_STATUS result = OYM_FAIL;

	if (mDS != NULL)
	{
		result = mDS->Deinit();
	}

	if (mInterface != NULL)
	{
		result = mInterface->UnRegisterCallback(this);
	}

	if (mInterface != NULL)
	{
		result = mInterface->Deinit();
	}
	return result;
}

/*RSSI_Threshold is used to filter device by RSSI
* if RSSI_Threshold = 0, all device will be sent to client,
* if RSSI_Threshold = x, only device with rssi > x will be sent to client.
*/
OYM_STATUS OYM_AdapterManager::StartScan(OYM_UINT8 RSSI_Threshold)
{
	/*cannot start to scaning while scanning or connecting is on the way.*/
	if (mIsScanning == OYM_TRUE || mIsConnecting == OYM_TRUE)
	{
		return OYM_ERR_CODE_SCAN_BUSY;
	}

	if (mDS != NULL)
	{
		//start to scan
		mIsScanning = OYM_TRUE;
		return mDS->StartScan();
	}
	else
	{
		return OYM_ERR_CODE_NO_RESOURCE;
	}
}

OYM_STATUS OYM_AdapterManager::StopScan()
{
	if (mDS != NULL)
	{
		//start to scan
		mIsScanning = OYM_FALSE;
		return mDS->StopScan();
	}
	else
	{
		return OYM_FAIL;
	}
}

OYM_STATUS OYM_AdapterManager::OnDeviceFound(BLE_DEVICE new_device)
{
	OYM_STATUS result = OYM_SUCCESS;
	LOGDEBUG("OnNewDeviceFound... \n");
	if (mClientCallback != NULL)
	{
		mClientCallback->onScanResult(&new_device);
	}
	return result;
}

OYM_STATUS OYM_AdapterManager::ScanFinished()
{
	OYM_STATUS result = OYM_FAIL;
	LOGDEBUG("OnScanFinished... \n");
	mIsScanning = OYM_FALSE;

	if (mClientCallback != NULL)
	{
		mClientCallback->onScanFinished();
	}
	return result;
}

OYM_STATUS OYM_AdapterManager::Connect(OYM_PUINT8 addr, UINT8 addr_type, OYM_BOOL is_direct_conn)
{
	OYM_STATUS result = OYM_FAIL;
	if (is_direct_conn == OYM_TRUE && mIsConnecting == OYM_TRUE)
	{
		return OYM_FALSE;
	}

	OYM_RemoteDevice *device = new OYM_RemoteDevice(mInterface, addr_type, addr, this);
	if (device != NULL)
	{
		device->Init();
	}

	if (mInterface != NULL)
	{
		if (is_direct_conn == OYM_TRUE)
		{
			result = mInterface->Connect(addr, addr_type, OYM_FALSE);
			mConnectingDevice.push_front(device);
			mIsConnecting = OYM_TRUE;
		}
	}
	return result;
}

OYM_STATUS OYM_AdapterManager::CancelConnect(OYM_PUINT8 addr, OYM_UINT8 addr_type)
{
	LOGDEBUG("start to CancelConnect... \n");
	OYM_STATUS result = OYM_FAIL;
	list<OYM_RemoteDevice*>::iterator ii;
	if (mInterface != NULL)
	{
		result = mInterface->Disconnect(0xFFFE);
		mIsConnecting = OYM_FALSE;
		result = OYM_SUCCESS;
	}
	for (ii = mConnectingDevice.begin(); ii != mConnectingDevice.end(); ii++)
	{
		LOGDEBUG("mConnectingDevice.size() = %d \n", mConnectingDevice.size());
		if (((*ii)->mAddrType == addr_type) && memcmp((*ii)->mAddr, addr, BT_ADDRESS_SIZE) == 0)
		{
			mConnectingDevice.erase(ii);
		}
	}
	return result;
}

OYM_STATUS OYM_AdapterManager::Disconnect(OYM_UINT16 handle)
{
	LOGDEBUG("start to disconnect device... \n");
	OYM_STATUS result = OYM_FAIL;
	if (mInterface != NULL)
	{
		result = mInterface->Disconnect(handle);
	}
	return result;
}

OYM_STATUS OYM_AdapterManager::OnConnectEvent(OYM_PUINT8 data, OYM_UINT16 length)
{
	OYM_STATUS result = OYM_FAIL;
	list<OYM_RemoteDevice*>::iterator ii;
	OYM_UINT8 status = data[0];
	OYM_UINT8 addr_type = data[1];
	if (status != OYM_SUCCESS)
	{
		LOGDEBUG("OnConnectEvent with error status = %d \n", status);
		for (ii = mConnectingDevice.begin(); ii != mConnectingDevice.end(); ii++)
		{
			LOGDEBUG("mConnectingDevice.size() = %d \n", mConnectingDevice.size());
			if (((*ii)->mAddrType == addr_type) && memcmp((*ii)->mAddr, data + 2, BT_ADDRESS_SIZE) == 0)
			{
				mConnectingDevice.erase(ii);
				mIsConnecting = OYM_FALSE;
			}
		}
		return OYM_FAIL;
	}

	LOGDEBUG("OnConnectEvent LOGDEBUG with length = %d \n" , length);
	for (OYM_UINT16 i = 0; i < length; i++)
	{
		LOGDEBUG("the data of [%d]th bytes is 0x%02x \n", i, data[i]);
	}
	LOGDEBUG("mConnectingDevice.size() = %d \n", mConnectingDevice.size());
	
	for (ii = mConnectingDevice.begin(); ii != mConnectingDevice.end(); ii++)
	{
		LOGDEBUG("mConnectingDevice.size() = %d \n", mConnectingDevice.size());
		if (((*ii)->mAddrType == addr_type) && memcmp((*ii)->mAddr, data + 2, BT_ADDRESS_SIZE) == 0)
		{
			(*ii)->Init();
			mConnectedDevice.push_front(*ii);
			//mConnectingDevice.erase(ii);
			(*ii)->ProcessMessage(OYM_DEVICE_EVENT_DEVICE_CONNECTED, data, length);

		}
	}

	return result;
}

OYM_RemoteDevice* OYM_AdapterManager::GetDeviceByHandle(OYM_UINT16 handle)
{
	list<OYM_RemoteDevice*>::iterator ii;
	for (ii = mConnectedDevice.begin(); ii != mConnectedDevice.end(); ii++)
	{
		if (((*ii)->GetHandle()) == handle)
		{
			return (*ii);
		}
	}

	return NULL;
}

OYM_STATUS  OYM_AdapterManager::ConfigMtuSize(OYM_UINT16 conn_handle, OYM_UINT16 MTU_Size)
{
	OYM_RemoteDevice* device = GetDeviceByHandle(conn_handle);
	if (device != NULL)
	{
		return device->ExchangeMTUSize(MTU_Size);
	}
	else
	{
		return OYM_FAIL;
	}
}

OYM_STATUS OYM_AdapterManager::ConnectionParameterUpdate(OYM_UINT16 conn_handle, OYM_UINT16 conn_interval_min, OYM_UINT16 conn_interval_max, OYM_UINT16 slave_latence, OYM_UINT16 supervision_timeout)
{
	OYM_RemoteDevice* device = GetDeviceByHandle(conn_handle);
	if (device != NULL)
	{
		return device->ConnectionParameterUpdateRequest(conn_interval_min, conn_interval_max, slave_latence, supervision_timeout);
	}
	else
	{
		return OYM_FAIL;
	}

}

OYM_STATUS OYM_AdapterManager::WriteCharacteristic(OYM_UINT16 conn_handle, OYM_UINT16 attribute_handle, OYM_UINT8 data_length, OYM_PUINT8 data)
{
	OYM_RemoteDevice* device = GetDeviceByHandle(conn_handle);
	if (device != NULL)
	{
		return device->WriteCharacteristicValue(attribute_handle, data_length, data);
	}
	else
	{
		return OYM_FAIL;
	}
}

OYM_STATUS OYM_AdapterManager::ReadCharacteristic(OYM_UINT16 conn_handle, OYM_UINT16 attribute_handle)
{
	OYM_RemoteDevice* device = GetDeviceByHandle(conn_handle);
	if (device != NULL)
	{
		return device->ReadCharacteristicValue(attribute_handle);
	}
	else
	{
		return OYM_FAIL;
	}
}

OYM_STATUS OYM_AdapterManager::OnEvent(OYM_UINT32 event, OYM_PUINT8 data, OYM_UINT16 length)
{
	OYM_STATUS result = OYM_SUCCESS;
	OYM_DEVICE_EVENT message = OYM_DEVICE_EVENT_INVALID;
	list<OYM_RemoteDevice*>::iterator ii;
	OYM_UINT8 handle_offset;

	switch(event)
	{
		case EVENT_MASK_ATT_EXCHANGE_MTU_MSG:
		{
			message = OYM_DEVICE_EVENT_ATT_EXCHANGE_MTU_MSG;
			handle_offset = 1;
			break;
		}
		case EVENT_MASK_ATT_READ_BY_GRP_TYPE_MSG:
		{
			message = OYM_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_ATT_READ_BY_TYPE_MSG:
		{
			message = OYM_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_ATT_ERROR_MSG:
		{
			message = OYM_DEVICE_EVENT_ATT_ERROR_MSG;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_ATT_READ_RESP_MSG:
		{
			message = OYM_DEVICE_EVENT_ATT_READ_RESP_MSG;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_ATT_READ_BLOB_RESP_MSG:
		{
			message = OYM_DEVICE_EVENT_ATT_READ_BLOB_RESP_MSG;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_ATT_READ_BY_INFO_MSG:
		{
			message = OYM_DEVICE_EVENT_ATT_READ_BY_INFO_MSG;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_ATT_WRITE_MSG:
		{
			message = OYM_DEVICE_EVENT_ATT_WRITE_MSG;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_ATT_NOTI_MSG:
		{
			LOGDEBUG("--------->notification received! \n");
			for (OYM_UINT16 i = 0; i < length; i++)
			{
				//LOGDEBUG("the notificationdata of [%d]th bytes is 0x%02x \n", i, data[i]);
			}
			handle_offset = 1;
			OYM_UINT16 handle = data[handle_offset] + (data[handle_offset + 1] << 8);
			OYM_RemoteDevice* device = GetDeviceByHandle(handle);
			OYM_DEVICE_STATE state = device->GetState();
			if (state == OYM_DEVICE_STATE_CONNECTED && mClientCallback != NULL)
			{
				mClientCallback->onNotificationReceived(handle, length-3, data + 3);
			}
			break;
		}

		case EVENT_MASK_SLAVE_REQUESTED_SECURITY_MSG:
		{
			message = OYM_DEVICE_EVENT_SLAVE_SECURY_REQUEST;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_AUTH_COMPLETE_MSG:
		{
			message = OYM_DEVICE_EVENT_AUTH_COMPLETE;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_BOND_COMPLETE_MSG:
		{
			message = OYM_DEVICE_EVENT_BOND_COMPLETE;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_LINK_PARA_UPDATE_MSG:
		{
			message = OYM_DEVICE_EVENT_LINK_PARA_UPDATE;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_GAP_LINK_ESTABLISHED_MSG:
		{
			OnConnectEvent(data, length);
			return OYM_SUCCESS;
		}

		case EVENT_MASK_GAP_LINK_TERMINATED_MSG:
		{
			message = OYM_DEVICE_EVENT_EVICE_DISCONNECTED;
			handle_offset = 1;
			break;
		}

		case EVENT_MASK_INTERNAL_SCAN_FINISHED:
		{
			ScanFinished();
			break;
		}

		default:
			break;
	}

	if (message != OYM_DEVICE_EVENT_INVALID)
	{
		LOGDEBUG("OnEvent with length = %d \n", length);
		for (OYM_UINT16 i = 0; i < length; i++)
		{
			LOGDEBUG("the data of [%d]th bytes is 0x%02x \n", i, data[i]);
		}

		OYM_UINT16 handle = data[handle_offset] + (data[handle_offset + 1] << 8);
		OYM_RemoteDevice* device = GetDeviceByHandle(handle);
		if (device != NULL)
		{
			if (event == EVENT_MASK_GAP_LINK_TERMINATED_MSG)
			{
				device->DeInit();
				OYM_UINT8 reason = data[3];
				if (mClientCallback != NULL)
				{
					mClientCallback->onDeviceDisonnected(OYM_SUCCESS, handle, reason);
				}
			}
			else
			{
				LOGDEBUG("call ProcessMessage \n");
				device->ProcessMessage(message, data, length);

				if(event == EVENT_MASK_ATT_EXCHANGE_MTU_MSG)
				{
					OYM_DEVICE_STATE state = device->GetState();
					if (state == OYM_DEVICE_STATE_CONNECTED && mClientCallback != NULL)
					{
						mClientCallback->onMTUSizeChanged(data[0], handle, device->GetMTUSize());
					}
				}
				else if(event == EVENT_MASK_LINK_PARA_UPDATE_MSG)
				{
					OYM_DEVICE_STATE state = device->GetState();
					if (state == OYM_DEVICE_STATE_CONNECTED && mClientCallback != NULL)
					{
						mClientCallback->onConnectionParmeterUpdated(data[0], handle, device->GetConnectInterval(), device->GetSupervisionTimeout(), device->GetSlaveLatency());
					}
				}
				else if(event == OYM_DEVICE_EVENT_ATT_READ_RESP_MSG)
				{
					OYM_DEVICE_STATE state = device->GetState();
					if (state == OYM_DEVICE_STATE_CONNECTED && mClientCallback != NULL)
					{
						mClientCallback->onChracteristicValueRead(data[0], handle, data[OYM_ATT_READ_VALUE_RESP_LEN_OFFSET], data + OYM_ATT_READ_VALUE_RESP_DATA_OFFSET);
					}
				}
			}
		}

	} 
	else
	{
		if (event == EVENT_MASK_GAP_STATUS_MSG)
		{
			LOGDEBUG("EVENT_MASK_GAP_STATUS_MSG with length = %d \n", length);
			for (OYM_UINT16 i = 0; i < length; i++)
			{
				LOGDEBUG("the data of [%d]th bytes is 0x%02x \n", i, data[i]);
			}
		}
		else if (event == EVENT_MASK_DEVICE_CONNECTED)
		{
			OYM_UINT16 handle = data[0] + (data[1] << 8);
			OYM_RemoteDevice* device = GetDeviceByHandle(handle);
			if (mClientCallback != NULL)
			{
				mClientCallback->onDeviceConnected(OYM_SUCCESS, handle, device->GetConnectInterval(), device->GetSupervisionTimeout(), device->GetSlaveLatency(), device->GetMTUSize());
			}
		}
	}

	return result;
}

