#ifndef __GATTCLIENT_H__
#define __GATTCLIENT_H__

#include "oym_types.h"
#include "list"
using namespace std;

#define OYM_ATT_PROPERTY_READ 0x02

/*define UUID type*/
#define OYM_UUID_16 0
#define OYM_UUID_128 1
#define OYM_UUID_32 2

#define OYM_ERROR_CODE_ATTR_NOT_FOUND 0x0A
#define OYM_ATT_ERROR_MSG_ERROR_CODE_OFFSET 7

#define OYM_ATT_PROCEDURE_SUCCESS 0
#define OYM_ATT_PRODECURE_COMPLETE 0x1A

#define OYM_ATT_READ_BY_TYPE_RES_STATUS_OFFSET 0
#define OYM_ATT_READ_BY_TYPE_RES_PDU_LEN_OFFSET (OYM_ATT_READ_BY_TYPE_RES_STATUS_OFFSET+3)
#define OYM_ATT_READ_BY_TYPE_RES_PAIR_LEN_OFFSET (OYM_ATT_READ_BY_TYPE_RES_PDU_LEN_OFFSET+1)

#define OYM_ATT_READ_BY_TYPE_RES_HANDLE_OFFSET (OYM_ATT_READ_BY_TYPE_RES_PAIR_LEN_OFFSET+1)
#define OYM_ATT_READ_BY_TYPE_RES_START_HANDLE_OFFSET (OYM_ATT_READ_BY_TYPE_RES_HANDLE_OFFSET+2)
#define OYM_ATT_READ_BY_TYPE_RES_END_HANDLE_OFFSET (OYM_ATT_READ_BY_TYPE_RES_START_HANDLE_OFFSET+2)
#define OYM_ATT_READ_BY_TYPE_RES_UUID_OFFSET (OYM_ATT_READ_BY_TYPE_RES_END_HANDLE_OFFSET+2)

#define OYM_ATT_READ_VALUE_RESP_STATUS_OFFSET 0
#define OYM_ATT_READ_VALUE_RESP_LEN_OFFSET (OYM_ATT_READ_VALUE_RESP_STATUS_OFFSET + 3)
#define OYM_ATT_READ_VALUE_RESP_DATA_OFFSET (OYM_ATT_READ_VALUE_RESP_LEN_OFFSET + 1)

typedef struct
{
	OYM_UINT8 type;
	union{
		OYM_UINT8 uuid128[16];
		OYM_UINT16 uuid16;
		OYM_UINT8 uuid32[4];
	}value;
}OYM_UUID;

class OYM_CHAR_DESCRIPTOR
{
public:
	OYM_CHAR_DESCRIPTOR(OYM_UINT16 handle, OYM_UINT8 index)
	{
		mIndex = index;
		mHandle = handle;
	}
	
	OYM_UINT16 mHandle;
	OYM_UUID mUUID;
	OYM_UINT8 mDataLen;
	OYM_UINT8 mData[10];
	OYM_UINT8 mIndex;
};

class OYM_CHAR_VALUE
{
public:
	OYM_CHAR_VALUE()
	{
	
	}

	OYM_CHAR_VALUE(OYM_UINT16 handle)
	{
		mHandle = handle;
	}
	
	OYM_UINT16 mHandle;
	OYM_UINT8 mLength;
	OYM_PUINT8 mData;
};

class OYM_CHARACTERISTIC
{
public:
	OYM_CHARACTERISTIC()
	{
	
	}

	OYM_CHARACTERISTIC(OYM_UINT16 handle, OYM_UINT8 property, OYM_UINT16 valueHandle, OYM_UINT8 index)
	{
		mHandle = handle;
		mProperty = property;
		mValueHandle = valueHandle;
		mIndex = index;
		mAttriValue = new OYM_CHAR_VALUE(mValueHandle);
		mNumOfDescriptor = 0;
		mCurrentDescriptor = 0;
	}
	OYM_STATUS AddDescriptorIntoCharacteristic(OYM_PUINT8 data, OYM_UINT8 pair_len);
	OYM_CHAR_DESCRIPTOR* FindDescriptorByUUID(OYM_UUID uuid);
	OYM_CHAR_DESCRIPTOR* FindDescriptorByIndex(OYM_UINT8 index);

	OYM_UINT16 mHandle;
	OYM_UINT8 mProperty;
	OYM_UINT16 mValueHandle;
	OYM_UUID mUUID;
	OYM_UINT8 mIndex;

	OYM_CHAR_VALUE* mAttriValue;
	OYM_UINT8 mNumOfDescriptor;
	OYM_UINT8 mCurrentDescriptor;
	list<OYM_CHAR_DESCRIPTOR*> mDescriptor;
};

class OYM_INCSERVICE
{
public:
	OYM_INCSERVICE(OYM_UINT16 handle, OYM_UINT16 startHandle, OYM_UINT16 endHandle, OYM_UINT8 index)
	{
		mHandle = handle;
		mStartHandle = startHandle;
		mEndHandle = endHandle;
		mIndex = index;
	}

	OYM_UINT16 mHandle;
	OYM_UINT16 mStartHandle;
	OYM_UINT16 mEndHandle;
	OYM_UUID mUUID;
	OYM_UINT8 mIndex;
};

class OYM_PRISERVICE
{
public:
	OYM_PRISERVICE(OYM_UINT16 sHandle, OYM_UINT16 eHandle, OYM_UINT8 index);
	OYM_STATUS AddCharacterIntoPriService(OYM_PUINT8 data, OYM_UINT8 pair_len);
	OYM_STATUS AddIncSvcIntoPriService(OYM_PUINT8 data, OYM_UINT8 pair_len);

	OYM_CHARACTERISTIC* FindCharacteristicbyIndex(OYM_UINT8 index);
	OYM_INCSERVICE* FindIncluSvcbyIndex(OYM_UINT8 index);
	
	OYM_UINT16 mStartHandle;
	OYM_UINT16 mEndHandle;
	OYM_UUID mUUID;
	OYM_UINT8 mIndex;
	OYM_UINT8 mCurrentCharateristic;
	
	OYM_UINT8 mNumOfIncludedService;
	list<OYM_INCSERVICE*> mIncludeService;
	
	OYM_UINT8 mNumOfCharacteristic;
	list<OYM_CHARACTERISTIC*> mCharacteristic;
};

class OYM_Service
{
public:
	OYM_PRISERVICE* FindPriSvcbyIndex(OYM_UINT8 index);
	OYM_PRISERVICE* GetNextAvailablePriSvc();
	OYM_STATUS AddPriSvcIntoService(OYM_PUINT8 data, OYM_UINT8 pair_len);
	
	OYM_Service();
	OYM_UINT8 mNumOfPrivateService;
	OYM_UINT8 mCurrentPriService;
	list<OYM_PRISERVICE*> mPrivateService;
};
#endif