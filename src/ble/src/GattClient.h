#ifndef __GATTCLIENT_H__
#define __GATTCLIENT_H__

#include "GFBLETypes.h"
#include "list"
using namespace std;

#define GF_ATT_PROPERTY_READ 0x02

/*define UUID type*/
#define GF_UUID_16 0
#define GF_UUID_128 1
#define GF_UUID_32 2

#define GF_ERROR_CODE_ATTR_NOT_FOUND 0x0A
#define GF_ATT_ERROR_MSG_ERROR_CODE_OFFSET 7

#define GF_ATT_PROCEDURE_SUCCESS 0
#define GF_ATT_PRODECURE_COMPLETE 0x1A

#define GF_ATT_READ_BY_TYPE_RES_STATUS_OFFSET 0
#define GF_ATT_READ_BY_TYPE_RES_PDU_LEN_OFFSET (GF_ATT_READ_BY_TYPE_RES_STATUS_OFFSET+3)
#define GF_ATT_READ_BY_TYPE_RES_PAIR_LEN_OFFSET (GF_ATT_READ_BY_TYPE_RES_PDU_LEN_OFFSET+1)

#define GF_ATT_READ_BY_TYPE_RES_HANDLE_OFFSET (GF_ATT_READ_BY_TYPE_RES_PAIR_LEN_OFFSET+1)
#define GF_ATT_READ_BY_TYPE_RES_START_HANDLE_OFFSET (GF_ATT_READ_BY_TYPE_RES_HANDLE_OFFSET+2)
#define GF_ATT_READ_BY_TYPE_RES_END_HANDLE_OFFSET (GF_ATT_READ_BY_TYPE_RES_START_HANDLE_OFFSET+2)
#define GF_ATT_READ_BY_TYPE_RES_UUID_OFFSET (GF_ATT_READ_BY_TYPE_RES_END_HANDLE_OFFSET+2)

#define GF_ATT_READ_VALUE_RESP_STATUS_OFFSET 0
#define GF_ATT_READ_VALUE_RESP_LEN_OFFSET (GF_ATT_READ_VALUE_RESP_STATUS_OFFSET + 3)
#define GF_ATT_READ_VALUE_RESP_DATA_OFFSET (GF_ATT_READ_VALUE_RESP_LEN_OFFSET + 1)

typedef struct
{
	GF_UINT8 type;
	union{
		GF_UINT8 uuid128[16];
		GF_UINT16 uuid16;
		GF_UINT8 uuid32[4];
	}value;
}GF_UUID;

class GF_CCharacteristicDescriptor
{
public:
	GF_CCharacteristicDescriptor(GF_UINT16 handle, GF_UINT8 index)
	{
		mIndex = index;
		mHandle = handle;
		mData = NULL;
	}
	
	GF_UINT16 mHandle;
	GF_UUID mUUID;
	GF_UINT8 mDataLen;
	GF_PUINT8 mData;
	GF_UINT8 mIndex;
};

class GF_CCharacteristicValue
{
public:
	GF_CCharacteristicValue()
	{
	
	}

	GF_CCharacteristicValue(GF_UINT16 handle)
	{
		mHandle = handle;
		mData = NULL;
	}
	
	GF_UINT16 mHandle;
	GF_UINT8 mLength;
	GF_PUINT8 mData;
};

class GF_CCharacteristic
{
public:
	GF_CCharacteristic()
	{
	
	}

	~GF_CCharacteristic();

	GF_CCharacteristic(GF_UINT16 handle, GF_UINT8 property, GF_UINT16 valueHandle, GF_UINT8 index)
	{
		mHandle = handle;
		mProperty = property;
		mValueHandle = valueHandle;
		mIndex = index;
		mAttriValue = new GF_CCharacteristicValue(mValueHandle);
		mNumOfDescriptor = 0;
		mCurrentDescriptor = 0;
	}
	GF_STATUS AddDescriptorIntoCharacteristic(GF_PUINT8 data, GF_UINT8 pair_len);
	GF_CCharacteristicDescriptor* FindDescriptorByUUID(GF_UUID uuid);
	GF_CCharacteristicDescriptor* FindDescriptorByIndex(GF_UINT8 index);

	GF_UINT16 mHandle;
	GF_UINT8 mProperty;
	GF_UINT16 mValueHandle;
	GF_UUID mUUID;
	GF_UINT8 mIndex;

	GF_CCharacteristicValue* mAttriValue;
	GF_UINT8 mNumOfDescriptor;
	GF_UINT8 mCurrentDescriptor;
	list<GF_CCharacteristicDescriptor*> mDescriptor;
};

class GF_CIncludeService
{
public:
	GF_CIncludeService(GF_UINT16 handle, GF_UINT16 startHandle, GF_UINT16 endHandle, GF_UINT8 index)
	{
		mHandle = handle;
		mStartHandle = startHandle;
		mEndHandle = endHandle;
		mIndex = index;
	}

	GF_UINT16 mHandle;
	GF_UINT16 mStartHandle;
	GF_UINT16 mEndHandle;
	GF_UUID mUUID;
	GF_UINT8 mIndex;
};

class GF_CPrimaryService
{
public:
	GF_CPrimaryService(GF_UINT16 sHandle, GF_UINT16 eHandle, GF_UINT8 index);
	~GF_CPrimaryService();
	GF_STATUS AddCharacterIntoPriService(GF_PUINT8 data, GF_UINT8 pair_len);
	GF_STATUS AddIncSvcIntoPriService(GF_PUINT8 data, GF_UINT8 pair_len);

	GF_CCharacteristic* FindCharacteristicbyIndex(GF_UINT8 index);
	GF_CIncludeService* FindIncluSvcbyIndex(GF_UINT8 index);
	
	GF_UINT16 mStartHandle;
	GF_UINT16 mEndHandle;
	GF_UUID mUUID;
	GF_UINT8 mIndex;
	GF_UINT8 mCurrentCharateristic;
	
	GF_UINT8 mNumOfIncludedService;
	list<GF_CIncludeService*> mIncludeService;
	
	GF_UINT8 mNumOfCharacteristic;
	list<GF_CCharacteristic*> mCharacteristic;
};

class GF_CService
{
public:
	GF_CPrimaryService* FindPriSvcbyIndex(GF_UINT8 index);
	GF_CPrimaryService* GetNextAvailablePriSvc();
	GF_STATUS AddPriSvcIntoService(GF_PUINT8 data, GF_UINT8 pair_len);
	
	GF_CService();
	~GF_CService();
	GF_UINT8 mNumOfPrivateService;
	GF_UINT8 mCurrentPriService;
	list<GF_CPrimaryService*> mPrivateService;
};
#endif