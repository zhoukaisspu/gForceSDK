#include"stdafx.h"
#include "GattClient.h"

/*used as GATT client. the sequence of gatt service discovery
*1. discovery all primary service
*2. do for every primary service:
*   2.1 find included service of primary service
*   2.2 find all characteristic of primary service
*       do for every characteristic
*		2.2.1 read characteristic value
*		2.2.2 find characteristic descriptor if exit
*3. process characteristic descriptor(read write)
*/

OYM_CHAR_DESCRIPTOR* OYM_CHARACTERISTIC::FindDescriptorByUUID(OYM_UUID uuid)
{
	OYM_CHAR_DESCRIPTOR* result = NULL;
	list<OYM_CHAR_DESCRIPTOR*>::iterator ii;

	if (mNumOfDescriptor == 0)
	{
		return result;
	}
	else
	{
		for (ii = mDescriptor.begin(); ii != mDescriptor.end(); ii++)
		{
			if (uuid.type == OYM_UUID_16 && ((*ii)->mUUID.type == OYM_UUID_16))
			{
				if ((*ii)->mUUID.value.uuid16 == uuid.value.uuid16)
				{
					return (*ii);
				}
			}
			else if (uuid.type == OYM_UUID_128 && ((*ii)->mUUID.type == OYM_UUID_128))
			{
				if (0 == memcpy(uuid.value.uuid128, (*ii)->mUUID.value.uuid128, 16))
				{
					return (*ii);
				};
			}
		}
	}

	return result;
}

OYM_STATUS OYM_CHARACTERISTIC::AddDescriptorIntoCharacteristic(OYM_PUINT8 data, OYM_UINT8 pair_len)
{
	OYM_STATUS result = OYM_SUCCESS;
	OYM_UINT16 handle = data[0] + (data[1] << 8);
	OYM_CHAR_DESCRIPTOR* descriptor = new OYM_CHAR_DESCRIPTOR(handle, mNumOfDescriptor);
	if (pair_len == 4)
	{
		descriptor->mUUID.type = OYM_UUID_16;
		descriptor->mUUID.value.uuid16 = data[2] + (data[3] << 8);
	}
	else if (pair_len == 18)
	{
		descriptor->mUUID.type = OYM_UUID_128;
		memcpy(descriptor->mUUID.value.uuid128, data + 2, 16);
	}

	mDescriptor.push_front(descriptor);
	mNumOfDescriptor += 1;

	return result;
}

OYM_STATUS OYM_PRISERVICE::AddCharacterIntoPriService(OYM_PUINT8 data, OYM_UINT8 pair_len)
{
	OYM_STATUS result = OYM_SUCCESS;
	OYM_UINT16 start_handle;
	OYM_UINT8 property;
	OYM_UINT16 value_handle;
	
	start_handle = data[0] + (data[1] << 8);
	property = data[2];
	value_handle = data[3] + (data[4] << 8);
	//LOGDEBUG("OYM_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG with start_handle = 0x%04x, property = 0x%02x, value_handle = 0x%02x\n", start_handle, property, value_handle);
	OYM_CHARACTERISTIC* characteristic = new OYM_CHARACTERISTIC(start_handle, property, value_handle, mNumOfCharacteristic);

	if (pair_len == 7)
	{
		characteristic->mUUID.type = OYM_UUID_16;
		characteristic->mUUID.value.uuid16 = data[5] + (data[6] << 8);
		//LOGDEBUG("OYM_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with uuid16 = 0x%04x\n", characteristic->mUUID.value.uuid16);
	}
	else if (pair_len == 21)
	{
		characteristic->mUUID.type = OYM_UUID_128;
		memcpy(characteristic->mUUID.value.uuid128, data + 5, 16);
	}
	else if (pair_len == 9)
	{
		characteristic->mUUID.type = OYM_UUID_32;
		memcpy(characteristic->mUUID.value.uuid32, data + 5, 4);
	}
	else
	{
		//LOGDEBUG("OYM_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with wrong pair length = %d\n", pair_len);
	}

	mNumOfCharacteristic++;
	mCharacteristic.push_front(characteristic);
	return result;
}

OYM_STATUS OYM_PRISERVICE::AddIncSvcIntoPriService(OYM_PUINT8 data, OYM_UINT8 pair_len)
{
	OYM_STATUS result = OYM_SUCCESS;
	OYM_UINT16 handle;
	OYM_UINT16 start_handle;
	OYM_UINT16 end_handle;
	
	handle = data[0] + (data[1] << 8);
	start_handle = data[2] + (data[3] << 8);;
	end_handle = data[4] + (data[5] << 8);
	//LOGDEBUG("OYM_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG with start_handle = 0x%04x, property = 0x%02x, value_handle = 0x%02x\n", start_handle, property, value_handle);
	OYM_INCSERVICE* incService = new OYM_INCSERVICE(handle, start_handle, end_handle, mNumOfIncludedService);
	if (pair_len == 7)
	{
		incService->mUUID.type = OYM_UUID_16;
		incService->mUUID.value.uuid16 = data[6] + (data[6] << 8);
		//LOGDEBUG("OYM_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with uuid16 = 0x%04x\n", characteristic->mUUID.value.uuid16);
	}
	else if (pair_len == 21)
	{
		incService->mUUID.type = OYM_UUID_128;
		memcpy(incService->mUUID.value.uuid128, data + 6, 16);
	}
	else if (pair_len == 9)
	{
		incService->mUUID.type = OYM_UUID_32;
		memcpy(incService->mUUID.value.uuid32, data + 6, 4);

	}
	else
	{
		//LOGDEBUG("OYM_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with wrong pair length = %d\n", pair_len);
	}

	mNumOfIncludedService++;
	mIncludeService.push_front(incService);
	return result;
}

OYM_CHARACTERISTIC* OYM_PRISERVICE::FindCharacteristicbyIndex(OYM_UINT8 index)
{
	list<OYM_CHARACTERISTIC*>::iterator ii;
	for (ii = mCharacteristic.begin(); ii != mCharacteristic.end(); ii++)
	{
		if ((*ii)->mIndex == index)
		{
			return (*ii);
		}
	}

	return NULL;
}

OYM_PRISERVICE::OYM_PRISERVICE(OYM_UINT16 sHandle, OYM_UINT16 eHandle, OYM_UINT8 index)
{
	mStartHandle = sHandle;
	mEndHandle = eHandle;
	mIndex = index;
	mNumOfIncludedService = 0;
	mNumOfIncludedService = 0;
}

OYM_PRISERVICE* OYM_Service::GetNextAvailablePriSvc()
{
	OYM_PRISERVICE* result = NULL;
	OYM_PRISERVICE* service;

	if ( mCurrentPriService < (mNumOfPrivateService - 1))
	{
		mCurrentPriService++;
		service = FindPriSvcbyIndex(mCurrentPriService);
		if ( service != NULL)
		{
			if (service->mStartHandle < service->mEndHandle)
			{
				return service;
			}
			else
			{
				return GetNextAvailablePriSvc();
			}
		}
	}
	else
	{
		return NULL;
	}

	return result;
}

OYM_STATUS OYM_Service::AddPriSvcIntoService(OYM_PUINT8 data, OYM_UINT8 pair_len)
{
	OYM_STATUS result = OYM_SUCCESS;
	
	OYM_UINT16 start_handle = data[0] + (data[1] << 8);
	OYM_UINT16 end_handle = data[2] + (data[3] << 8);
	
	OYM_PRISERVICE* service = new OYM_PRISERVICE(start_handle, end_handle, mNumOfPrivateService);

	if (pair_len == 6)
	{
		service->mUUID.type = OYM_UUID_16;
		service->mUUID.value.uuid16 = data[6] + (data[6] << 8);
		//LOGDEBUG("OYM_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with uuid16 = 0x%04x\n", characteristic->mUUID.value.uuid16);
	}
	else if (pair_len == 20)
	{
		service->mUUID.type = OYM_UUID_128;
		memcpy(service->mUUID.value.uuid128, data + 6, 16);
	}
	else if (pair_len == 8)
	{
		service->mUUID.type = OYM_UUID_32;
		memcpy(service->mUUID.value.uuid32, data + 6, 4);

	}
	else
	{
		//LOGDEBUG("OYM_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with wrong pair length = %d\n", pair_len);
	}

	mPrivateService.push_front(service);
	mNumOfPrivateService++;
	return result;
}


OYM_PRISERVICE* OYM_Service::FindPriSvcbyIndex(OYM_UINT8 index)
{
	list<OYM_PRISERVICE*>::iterator ii;
	for (ii = mPrivateService.begin(); ii != mPrivateService.end(); ii++)
	{
		if ((*ii)->mIndex == index)
		{
			return (*ii);
		}
	}

	return NULL;
}

OYM_Service::OYM_Service()
{
	mNumOfPrivateService = 0;
	mCurrentPriService = 0;
}