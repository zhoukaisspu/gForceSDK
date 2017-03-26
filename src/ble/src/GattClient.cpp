#include "GattClient.h"
#include <assert.h>
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
GF_CCharacteristic::~GF_CCharacteristic()
{
	GF_CCharacteristicDescriptor* temp_c;
	GF_UINT8 i;
	for (i = 0; i < mDescriptor.size(); i++)
	{
		temp_c = mDescriptor.front();
		mDescriptor.pop_front();
		if (temp_c->mData != NULL)
		{
			delete temp_c->mData;
			temp_c->mData = NULL;
		}
		delete temp_c;
	}

	if (mAttriValue != NULL)
	{
		if (mAttriValue->mData != NULL)
		{
			delete mAttriValue->mData;
			mAttriValue->mData = NULL;
		}
		delete mAttriValue;
		mAttriValue = NULL;
	}
}

GF_CCharacteristicDescriptor* GF_CCharacteristic::FindDescriptorByUUID(GF_UUID uuid)
{
	GF_CCharacteristicDescriptor* result = NULL;
	list<GF_CCharacteristicDescriptor*>::iterator ii;

	if (mNumOfDescriptor == 0)
	{
		return result;
	}
	else
	{
		for (ii = mDescriptor.begin(); ii != mDescriptor.end(); ii++)
		{
			if (uuid.type == GF_UUID_16 && ((*ii)->mUUID.type == GF_UUID_16))
			{
				if ((*ii)->mUUID.value.uuid16 == uuid.value.uuid16)
				{
					return (*ii);
				}
			}
			else if (uuid.type == GF_UUID_128 && ((*ii)->mUUID.type == GF_UUID_128))
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

GF_CCharacteristicDescriptor* GF_CCharacteristic::FindDescriptorByIndex(GF_UINT8 index)
{
	GF_CCharacteristicDescriptor* result = NULL;
	list<GF_CCharacteristicDescriptor*>::iterator ii;

	if (mNumOfDescriptor == 0)
	{
		return result;
	}
	else
	{
		for (ii = mDescriptor.begin(); ii != mDescriptor.end(); ii++)
		{
			if (((*ii)->mIndex == index))
			{
				return (*ii);
			}
		}
	}

	return result;
}

GF_STATUS GF_CCharacteristic::AddDescriptorIntoCharacteristic(GF_PUINT8 data, GF_UINT8 pair_len)
{
	GF_STATUS result = GF_OK;
	GF_UINT16 handle = data[0] + (data[1] << 8);
	GF_CCharacteristicDescriptor* descriptor = new GF_CCharacteristicDescriptor(handle, mNumOfDescriptor);
	assert(descriptor);
	
	if (pair_len == 4)
	{
		descriptor->mUUID.type = GF_UUID_16;
		descriptor->mUUID.value.uuid16 = data[2] + (data[3] << 8);
	}
	else if (pair_len == 18)
	{
		descriptor->mUUID.type = GF_UUID_128;
		memcpy(descriptor->mUUID.value.uuid128, data + 2, 16);
	}

	mDescriptor.push_front(descriptor);
	mNumOfDescriptor += 1;

	return result;
}

GF_STATUS GF_CPrimaryService::AddCharacterIntoPriService(GF_PUINT8 data, GF_UINT8 pair_len)
{
	GF_STATUS result = GF_OK;
	GF_UINT16 start_handle;
	GF_UINT8 property;
	GF_UINT16 value_handle;
	
	start_handle = data[0] + (data[1] << 8);
	property = data[2];
	value_handle = data[3] + (data[4] << 8);
	//LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG with start_handle = 0x%04x, property = 0x%02x, value_handle = 0x%02x\n", start_handle, property, value_handle);
	GF_CCharacteristic* characteristic = new GF_CCharacteristic(start_handle, property, value_handle, mNumOfCharacteristic);
	assert(characteristic);
	
	if (pair_len == 7)
	{
		characteristic->mUUID.type = GF_UUID_16;
		characteristic->mUUID.value.uuid16 = data[5] + (data[6] << 8);
		//LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with uuid16 = 0x%04x\n", characteristic->mUUID.value.uuid16);
	}
	else if (pair_len == 21)
	{
		characteristic->mUUID.type = GF_UUID_128;
		memcpy(characteristic->mUUID.value.uuid128, data + 5, 16);
	}
	else if (pair_len == 9)
	{
		characteristic->mUUID.type = GF_UUID_32;
		memcpy(characteristic->mUUID.value.uuid32, data + 5, 4);
	}
	else
	{
		//LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with wrong pair length = %d\n", pair_len);
	}

	mNumOfCharacteristic++;
	mCharacteristic.push_front(characteristic);
	return result;
}

GF_STATUS GF_CPrimaryService::AddIncSvcIntoPriService(GF_PUINT8 data, GF_UINT8 pair_len)
{
	GF_STATUS result = GF_OK;
	GF_UINT16 handle;
	GF_UINT16 start_handle;
	GF_UINT16 end_handle;
	
	handle = data[0] + (data[1] << 8);
	start_handle = data[2] + (data[3] << 8);;
	end_handle = data[4] + (data[5] << 8);
	//LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG with start_handle = 0x%04x, property = 0x%02x, value_handle = 0x%02x\n", start_handle, property, value_handle);
	GF_CIncludeService* incService = new GF_CIncludeService(handle, start_handle, end_handle, mNumOfIncludedService);
	if ( incService == NULL)
	{
		assert(GF_FALSE);
	}
	
	if (pair_len == 8)
	{
		incService->mUUID.type = GF_UUID_16;
		incService->mUUID.value.uuid16 = data[6] + (data[7] << 8);
		//LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with uuid16 = 0x%04x\n", characteristic->mUUID.value.uuid16);
	}
	else if (pair_len == 22)
	{
		incService->mUUID.type = GF_UUID_128;
		memcpy(incService->mUUID.value.uuid128, data + 6, 16);
	}
	else if (pair_len == 10)
	{
		incService->mUUID.type = GF_UUID_32;
		memcpy(incService->mUUID.value.uuid32, data + 6, 4);
	}
	else
	{
		//LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with wrong pair length = %d\n", pair_len);
		assert(GF_FALSE);
	}

	mNumOfIncludedService++;
	mIncludeService.push_front(incService);
	return result;
}

GF_CIncludeService* GF_CPrimaryService::FindIncluSvcbyIndex(GF_UINT8 index)
{
	list<GF_CIncludeService*>::iterator ii;
	for (ii = mIncludeService.begin(); ii != mIncludeService.end(); ii++)
	{
		if ((*ii)->mIndex == index)
		{
			return (*ii);
		}
	}

	return NULL;
}

GF_CCharacteristic* GF_CPrimaryService::FindCharacteristicbyIndex(GF_UINT8 index)
{
	list<GF_CCharacteristic*>::iterator ii;
	for (ii = mCharacteristic.begin(); ii != mCharacteristic.end(); ii++)
	{
		if ((*ii)->mIndex == index)
		{
			return (*ii);
		}
	}

	return NULL;
}

GF_CPrimaryService::GF_CPrimaryService(GF_UINT16 sHandle, GF_UINT16 eHandle, GF_UINT8 index)
{
	mStartHandle = sHandle;
	mEndHandle = eHandle;
	mIndex = index;
	mNumOfIncludedService = 0;
	mNumOfCharacteristic = 0;
	mCurrentCharateristic = 0;
}

GF_CPrimaryService::~GF_CPrimaryService()
{
	GF_CCharacteristic* temp_c;
	GF_CIncludeService* temp_i;
	GF_UINT8 i;
	for (i = 0; i < mCharacteristic.size(); i++)
	{
		temp_c = mCharacteristic.front();
		mCharacteristic.pop_front();
		delete temp_c;
	}

	for (i = 0; i < mIncludeService.size(); i++)
	{
		temp_i = mIncludeService.front();
		mIncludeService.pop_front();
		delete temp_i;
	}
}

GF_CPrimaryService* GF_CService::GetNextAvailablePriSvc()
{
	GF_CPrimaryService* result = NULL;
	GF_CPrimaryService* service;

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

GF_STATUS GF_CService::AddPriSvcIntoService(GF_PUINT8 data, GF_UINT8 pair_len)
{
	GF_STATUS result = GF_OK;
	
	GF_UINT16 start_handle = data[0] + (data[1] << 8);
	GF_UINT16 end_handle = data[2] + (data[3] << 8);
	
	GF_CPrimaryService* service = new GF_CPrimaryService(start_handle, end_handle, mNumOfPrivateService);
	if ( service == NULL)
	{
		assert(GF_FALSE);
	}
	
	if (pair_len == 6)
	{
		service->mUUID.type = GF_UUID_16;
		service->mUUID.value.uuid16 = data[4] + (data[5] << 8);
		//LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with uuid16 = 0x%04x\n", characteristic->mUUID.value.uuid16);
	}
	else if (pair_len == 20)
	{
		service->mUUID.type = GF_UUID_128;
		memcpy(service->mUUID.value.uuid128, data + 4, 16);
	}
	else if (pair_len == 8)
	{
		service->mUUID.type = GF_UUID_32;
		memcpy(service->mUUID.value.uuid32, data + 4, 4);
	}
	else
	{
		//LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with wrong pair length = %d\n", pair_len);
		assert(GF_FALSE);
	}

	mPrivateService.push_front(service);
	mNumOfPrivateService++;
	return result;
}


GF_CPrimaryService* GF_CService::FindPriSvcbyIndex(GF_UINT8 index)
{
	list<GF_CPrimaryService*>::iterator ii;
	for (ii = mPrivateService.begin(); ii != mPrivateService.end(); ii++)
	{
		if ((*ii)->mIndex == index)
		{
			return (*ii);
		}
	}

	return NULL;
}

GF_CService::GF_CService()
{
	mNumOfPrivateService = 0;
	mCurrentPriService = 0;
}

GF_CService::~GF_CService()
{
	GF_CPrimaryService* temp;
	GF_UINT8 i;
	for (i = 0; i < mPrivateService.size(); i++)
	{
		temp = mPrivateService.front();
		mPrivateService.pop_front();
		delete temp;
	}

	mNumOfPrivateService = 0;
	mCurrentPriService = 0;
}