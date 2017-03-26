#include "Database.h"
#include "GFBLETypes.h"
using namespace tinyxml2;
using namespace std;

GF_CHAR int2char(GF_UINT8 data)
{
	GF_CHAR result = '#';

	if (0 <= data  && data <= 9)
	{
		result = data + '0';
	}
	else if (10 <= data && data <= 16)
	{
		result = (data - 10) + 'A';
	}

	return result;
}

GF_VOID arry2str(GF_PUINT8 array, GF_UINT32 length, char* result)
{
	GF_UINT32 i = 0;

	for (i = 0; i < length; i++)
	{
		result[2 * i] = int2char((array[i] & 0xF0) >> 4);
		result[2 * i + 1] = int2char(array[i] & 0x0F);
	}
	result[2 * length ] = '\0';
}

GF_UINT8 char2int8(const char* data)
{
	GF_UINT8 result = '#';

	if ((*data) >= '0' && (*data) <= '9')
	{
		result = (*data) - '0';
	}

	if ((*data) >= 'A' && (*data) <= 'F')
	{
		result = (*data) - 'A' + 10;
	}

	return result;
}

GF_VOID str2arry(const char* str, GF_PUINT8 array)
{
	int i = 0;
	int length = strlen(str);

	for (i = 0; i < length/2; i++)
	{
		array[i] = (char2int8(str + 2 * i) << 4);
		array[i] += char2int8(str + 2 * i + 1);
	}
}

GF_CDatabase::GF_CDatabase(GF_PUINT8 address, GF_UINT8 length)
{
	GF_STATUS status = GF_OK;
	mFilename = (char*)malloc(length * 2 + 1);
	arry2str(address, length, mFilename);

	mTag = MODUAL_TAG_DB;
	mDoc = new OYMXMLDocument;
	status = mDoc->LoadFile((const char*)mFilename);
	if (status == GF_OK)
	{
		LOGDEBUG(mTag, "LoadFile success!!! \n");
	} 
	else
	{
		LOGDEBUG(mTag, "LoadFile fail, file not exist!!! \n");
	}
}

GF_CDatabase::~GF_CDatabase()
{
	free(mFilename);
	//do not need to delete mDoc.
}


GF_STATUS GF_CDatabase::DeleteNode(const char* NodeName)
{
	GF_STATUS status = GF_FAIL;
	status = mDoc->LoadFile((const char*)mFilename);
	if (status == GF_FAIL)
	{
		LOGDEBUG(mTag, "Load File Error!! \n");
		return status;
	}

	XMLElement *DeletetEle = mDoc->RootElement();
	XMLElement *Temp;
	while (DeletetEle != NULL)
	{
		if (0 == memcmp(DeletetEle->Name(), NodeName, strlen(NodeName)))
		{
			break;
		}
		else
		{
			Temp = DeletetEle->NextSiblingElement();
			DeletetEle = Temp;
		}
	}

	if (DeletetEle != NULL)
	{
		XMLNode *DeleteNode = DeletetEle->ToElement();

		LOGDEBUG(mTag, "name of deleting is %s \n", DeletetEle->Name());

		mDoc->DeleteNode(DeleteNode);
		mDoc->SaveFile((const char*)mFilename);
		status = GF_OK;
	}
	else
	{
		LOGDEBUG(mTag, "No Node found to delete!! \n");
	}

	return status;
}

GF_STATUS GF_CDatabase::DeleteGattService()
{
	GF_STATUS status = GF_FAIL;
	status = DeleteNode(GATTSERVICE);

	return status;
}

GF_STATUS GF_CDatabase::DeleteLinkKey()
{
	GF_STATUS status = GF_FAIL;
	status = DeleteNode(SECURITYINFOMATION);

	return status;
}

GF_STATUS GF_CDatabase::LoadLinkKey(GF_PUINT8 keysize, GF_PUINT8 key, GF_PUINT16 div, GF_PUINT8 rand)
{
	GF_STATUS result = GF_FAIL;
	GF_STATUS status = GF_OK;
	
	if (mDoc == NULL) 
	{
		result = GF_FAIL;
	}
	
	//file do not exist.
	status = mDoc->LoadFile((const char*)mFilename);
	if (status == GF_OK)
	{
		LOGDEBUG(mTag, "LoadFile success!!! \n");
		XMLElement *rootEle = mDoc->RootElement();	
		XMLElement *Temp;
		while (rootEle != NULL)
		{
			if (0 == memcmp(rootEle->Name(), SECURITYINFOMATION, strlen(SECURITYINFOMATION)))
			{
				LOGDEBUG(mTag, "SecurityInfomation found \n");
				result = GF_OK;
				XMLElement *child = rootEle->FirstChildElement();
				XMLElement *next_child;
				const XMLAttribute *attr;
				while (child != NULL)
				{
					if (0 == memcmp(child->Name(), KEYSIZE, strlen(KEYSIZE)))
					{
						attr = child->FirstAttribute();
						*keysize = attr->IntValue();

						attr = attr->Next();
						*div = attr->IntValue();
					}
					else if (0 == memcmp(child->Name(), RAND, strlen(RAND)))
					{
						attr = child->FirstAttribute();
						LOGDEBUG(mTag, "attribute name = %s attribute value = %s\n", attr->Name(), attr->Value());
						str2arry(attr->Value(), rand);
					}
					else if (0 == memcmp(child->Name(), LTK, strlen(LTK)))
					{
						attr = child->FirstAttribute();
						LOGDEBUG(mTag, "attribute name = %s attribute value = %s\n", attr->Name(), attr->Value());
						str2arry(attr->Value(), key);
					}

					next_child = child->NextSiblingElement();
					child = next_child;
				}
				LOGDEBUG(mTag, "Load link key done!! \n");
			}

			Temp = rootEle->NextSiblingElement();
			rootEle = Temp;
		}
	}
	else
	{
		LOGDEBUG(mTag, "LoadFile fail, file not exist!!! \n");
		result = GF_FAIL;
	}

	return result;
}

GF_STATUS GF_CDatabase::SaveLinkKey(GF_UINT8 keysize, GF_PUINT8 key, GF_UINT16 div, GF_PUINT8 rand)
{
	GF_STATUS status = GF_FAIL;
	GF_UINT8 size = (min(keysize, 8))*2 + 1;
	char data[100];
	memset(data, 0, size);
	LOGDEBUG(mTag, "start to save linkkey... \n");

	//load file first
	mDoc->LoadFile((const char*)mFilename);

	XMLElement * SecurityInfomation = mDoc->NewElement(SECURITYINFOMATION);
	mDoc->LinkEndChild(SecurityInfomation);

	XMLElement * KeySize = mDoc->NewElement(KEYSIZE);
	KeySize->SetAttribute(KEYSIZE, keysize);
	KeySize->SetAttribute(DIV, div);
	SecurityInfomation->LinkEndChild(KeySize);

	XMLElement * Rand = mDoc->NewElement(RAND);
	arry2str(rand, 8, data);
	Rand->SetAttribute(RAND, data);
	SecurityInfomation->LinkEndChild(Rand);

	XMLElement * Key = mDoc->NewElement(LTK);
	arry2str(key, keysize, data);
	Key->SetAttribute(LTK, data);
	SecurityInfomation->LinkEndChild(Key);

	mDoc->SaveFile((const char*)mFilename);

	LOGDEBUG(mTag, "linkkey successful... \n");
	return GF_OK;
}

GF_STATUS GF_CDatabase::SaveIncludedService(XMLElement *PriSvc, GF_CIncludeService* IncluSvc)
{
	if (PriSvc == NULL)
	{
		return GF_FAIL;
	}

	XMLElement *IncludeService = mDoc->NewElement(INCLUDEDSERVICE);
	IncludeService->SetAttribute(INDEX, IncluSvc->mIndex);
	IncludeService->SetAttribute(ATTRIHANDLE, IncluSvc->mHandle);
	SaveUUID(IncludeService, IncluSvc->mUUID);	
	IncludeService->SetAttribute(START_HANDLE, IncluSvc->mStartHandle);
	IncludeService->SetAttribute(END_HANDLE, IncluSvc->mEndHandle);
	PriSvc->LinkEndChild(IncludeService);

	return GF_OK;
}

GF_STATUS GF_CDatabase::SaveCharacteristicDescriptor(XMLElement *Charc, GF_CCharacteristicDescriptor* Descriptor)
{
	char *data;
	if (Charc == NULL || Descriptor == NULL)
	{
		return GF_FAIL;
	}

	XMLElement *CharcDescrip = mDoc->NewElement(CHARACTERISTICDES);
	CharcDescrip->SetAttribute(INDEX, Descriptor->mIndex);
	CharcDescrip->SetAttribute(ATTRIHANDLE, Descriptor->mHandle);
	SaveUUID(CharcDescrip, Descriptor->mUUID);

	if (Descriptor->mUUID.value.uuid16 != 0x2902 && Descriptor->mData != NULL)
	{
		data = (char*)malloc((Descriptor->mDataLen * 2) + 1);
		arry2str(Descriptor->mData, Descriptor->mDataLen, data);
		CharcDescrip->SetAttribute(VALUE, data);
		free(data);
	}
	Charc->LinkEndChild(CharcDescrip);
	
	return GF_OK;
}

GF_STATUS GF_CDatabase::SaveChracteristicValue(XMLElement *Charc, GF_CCharacteristicValue* value)
{
	char *data;
	if (Charc == NULL || value == NULL)
	{
		LOGDEBUG(mTag, "SaveChracteristicValue with NULL pointer \n");
		return GF_FAIL;
	}

	XMLElement *CharcValue = mDoc->NewElement(CHARACTERISTICVALUE);
	CharcValue->SetAttribute(ATTRIHANDLE, value->mHandle);

	if (value->mData != NULL)
	{
		data = (char*)malloc((value->mLength * 2) + 1);
		arry2str(value->mData, value->mLength, data);
		CharcValue->SetAttribute(VALUE, data);
	}

	Charc->LinkEndChild(CharcValue);
	return GF_OK;
}

GF_STATUS GF_CDatabase::SaveCharacteristic(XMLElement *PriSvc, GF_CCharacteristic* Characteristic)
{
	if (PriSvc == NULL || Characteristic == NULL)
	{
		LOGDEBUG(mTag, "SaveCharacteristic with NULL pointer \n");
		return GF_FAIL;
	}

	XMLElement *Charc = mDoc->NewElement(CHARACTERISTIC);
	Charc->SetAttribute(INDEX, Characteristic->mIndex);
	Charc->SetAttribute(ATTRIHANDLE, Characteristic->mHandle);
	SaveUUID(Charc, Characteristic->mUUID);
	Charc->SetAttribute(PROPERTY, Characteristic->mProperty);
	Charc->SetAttribute(VALUE_HANDLE, Characteristic->mValueHandle);
	Charc->SetAttribute(NUM_CHACDES, Characteristic->mNumOfDescriptor);
	
	PriSvc->LinkEndChild(Charc);

	if (Characteristic->mProperty & GF_ATT_PROPERTY_READ)
	{
		SaveChracteristicValue(Charc, Characteristic->mAttriValue);
	}

	for (int i = 0; i < Characteristic->mNumOfDescriptor; i++)
	{
		GF_CCharacteristicDescriptor* Descriptor = Characteristic->FindDescriptorByIndex(i);
		SaveCharacteristicDescriptor(Charc, Descriptor);
	}

	return GF_OK;
}
GF_STATUS GF_CDatabase::SaveUUID(XMLElement * Item, GF_UUID uuid)
{
	char *data = (char*)malloc(128);
	memset(data, 0, 128);
	GF_UINT8 array[2] = {0x00, 0x00};
	if (Item == NULL)
	{
		free(data);
		return GF_FAIL;
	}

	if (uuid.type == GF_UUID_16)
	{
		array[0] = (uuid.value.uuid16 >> 8) & 0xFF;
		array[1] = uuid.value.uuid16 & 0xFF;
		arry2str(array, 2, data);
		Item->SetAttribute(UUID, data);
	}
	else if (uuid.type == GF_UUID_32)
	{
		arry2str(uuid.value.uuid32, 4, data);
		Item->SetAttribute(UUID, data);
	}
	else
	{
		arry2str(uuid.value.uuid128, 16, data);
		Item->SetAttribute(UUID, data);
	}

	free(data);

	return GF_OK;
}

GF_STATUS GF_CDatabase::SavePrimaryService(XMLElement * GattSvc, GF_CPrimaryService* PrimaryService)
{
	XMLElement * PriSvc = mDoc->NewElement(PRIMARYSERVICE);

	PriSvc->SetAttribute(INDEX, PrimaryService->mIndex);
	SaveUUID(PriSvc, PrimaryService->mUUID);
	PriSvc->SetAttribute(START_HANDLE, PrimaryService->mStartHandle);
	PriSvc->SetAttribute(END_HANDLE, PrimaryService->mEndHandle);
	PriSvc->SetAttribute(NUM_INC_SVC, PrimaryService->mNumOfIncludedService);
	PriSvc->SetAttribute(NUM_CHAC, PrimaryService->mNumOfCharacteristic);
	GattSvc->LinkEndChild(PriSvc);

	for (int i = 0; i < PrimaryService->mNumOfIncludedService; i++)
	{
		GF_CIncludeService* IncluSvc = PrimaryService->FindIncluSvcbyIndex(i);
		SaveIncludedService(PriSvc, IncluSvc);
	}

	for (int i = 0; i < PrimaryService->mNumOfCharacteristic; i++)
	{
		GF_CCharacteristic* Characteristic = PrimaryService->FindCharacteristicbyIndex(i);
		SaveCharacteristic(PriSvc, Characteristic);
	}

	return GF_OK;
}

GF_STATUS GF_CDatabase::SaveService(GF_CService* service)
{
	GF_STATUS status = GF_FAIL;
	//load file first
	status = mDoc->LoadFile((const char*)mFilename);
	if (status != GF_OK)
	{
		return GF_FAIL;
	}

	XMLElement * GattSvc = mDoc->NewElement(GATTSERVICE);
	mDoc->LinkEndChild(GattSvc);

	GF_CPrimaryService* PrimaryService;
	for (int i = 0; i < service->mNumOfPrivateService; i++ )
	{
		LOGDEBUG(mTag, "SaveService index = %d \n", i);
		PrimaryService = service->FindPriSvcbyIndex(i);
		SavePrimaryService(GattSvc, PrimaryService);
	}

	mDoc->SaveFile((const char*)mFilename);
	return GF_OK;
}

void GF_CDatabase::GetUUIDFromChar(GF_UUID *uuid, const char* value)
{
	UINT8 len = strlen(value);
	UINT8 data[2];

	if (len == 0x04)
	{
		uuid->type = GF_UUID_16;
		str2arry(value, data);
		uuid->value.uuid16 = data[1] + (data[0] << 8);
	}
	else if (len == 32)
	{
		uuid->type = GF_UUID_128;
		str2arry(value, uuid->value.uuid128);
	}

	else
	{
		uuid->type = GF_UUID_32;
		str2arry(value, uuid->value.uuid32);
	}
}

XMLAttribute* GF_CDatabase::GetAttibuteByName(XMLElement *element, const char* name)
{
	XMLAttribute* result = NULL;
	const XMLAttribute* a = 0;
	for (a = element->FirstAttribute(); a; a = a->Next()) {
		if (XMLUtil::StringEqual(a->Name(), name))
			return (XMLAttribute*)a;
	}

	return result;
}

void GF_CDatabase::GetCharacteristicDescriptor(XMLElement *chara, GF_CCharacteristic* charcteristic)
{
	GF_UINT8 index = 0;
	GF_UINT16 handle = 0;
	XMLElement *chrades = chara;
	XMLElement *temp;
	const XMLAttribute* attribute;
	const char* value;

	while (chrades != NULL)
	{
		if (0 == memcmp(chrades->Name(), CHARACTERISTICDES, strlen(CHARACTERISTICDES)))
		{
			attribute = chrades->FirstAttribute();

			//get index
			attribute = GetAttibuteByName(chrades, INDEX);
			if (attribute != NULL)
			{
				index = attribute->IntValue();
			}

			//get handle
			attribute = GetAttibuteByName(chrades, ATTRIHANDLE);
			if (attribute != NULL)
			{
				handle = attribute->IntValue();
				LOGDEBUG(mTag, "descriptor handle = %d \n", handle);
			}

			GF_CCharacteristicDescriptor * descriptor = new GF_CCharacteristicDescriptor(handle, index);
			//get uuid
			attribute = GetAttibuteByName(chrades, UUID);
			if (attribute != NULL)
			{
				value = attribute->Value();
				GetUUIDFromChar(&(descriptor->mUUID), value);
			}
#if 0
			if (!((descriptor->mUUID.type == GF_UUID_16) && (descriptor->mUUID.value.uuid16 == 0x2902)))
			{
				attribute = GetAttibuteByName(chara, VALUE);
				if (attribute != NULL)
				{
					value = attribute->Value();
					descriptor->mDataLen = (strlen(value) / 2);
					str2arry(value, descriptor->mData);
				}
			}
#endif
			charcteristic->mDescriptor.push_front(descriptor);
		}

		temp = chrades->NextSiblingElement();
		chrades = temp;
	}

	return;
}

void GF_CDatabase::GetCharacteristicValue(XMLElement *chara, GF_CCharacteristic* charcteristic)
{
	XMLElement *chracvalue = chara;
	XMLElement *temp;
	const XMLAttribute* attribute;
	const char* value;
	if (charcteristic == NULL)
	{
		return;
	}

	while (chracvalue != NULL)
	{
		if (0 == memcmp(chracvalue->Name(), CHARACTERISTICVALUE, strlen(CHARACTERISTICVALUE)))
		{
			attribute = chracvalue->FirstAttribute();

			attribute = GetAttibuteByName(chara, VALUE_HANDLE);
			if (attribute != NULL)
			{
				charcteristic->mAttriValue->mHandle = attribute->IntValue();
			}
			
			attribute = GetAttibuteByName(chara, VALUE);
			if (attribute != NULL)
			{
				value = attribute->Value();
				//LOGDEBUG(mTag, "GetCharacteristicValue  value = %x\n", value);
				if (value != NULL)
				{
					charcteristic->mAttriValue->mLength = (strlen(value) / 2);
					charcteristic->mAttriValue->mData = (GF_PUINT8)malloc(charcteristic->mAttriValue->mLength);
					str2arry(value, charcteristic->mAttriValue->mData);
				}
				else
				{
					charcteristic->mAttriValue->mLength = 0;
					charcteristic->mAttriValue->mData = NULL;
				}
			}

			return;
		}

		temp = chracvalue->NextSiblingElement();
		chracvalue = temp;
	}
}

GF_CCharacteristic* GF_CDatabase::LoadCharacteristic(XMLElement *chara)
{
	GF_UINT16 value_handle = 0;
	GF_UINT16 handle = 0;
	GF_UINT8 index = 0;
	GF_UINT8 property = 0;
	GF_UINT8 num_of_characteristic_descriptor = 0;
	XMLAttribute* attribute;
	const char* value;

	if (chara == NULL)
	{
		return NULL;
	}

	//get value handle
	attribute = GetAttibuteByName(chara, VALUE_HANDLE);
	if (attribute != NULL)
	{
		value_handle = attribute->IntValue();
	}

	//get index
	attribute = GetAttibuteByName(chara, INDEX);
	if (attribute != NULL)
	{
		index = attribute->IntValue();
	}

	//get handle
	attribute = GetAttibuteByName(chara, ATTRIHANDLE);
	if (attribute != NULL)
	{
		handle = attribute->IntValue();
	}

	//get perporty
	attribute = GetAttibuteByName(chara, PROPERTY);
	if (attribute != NULL)
	{
		property = attribute->IntValue();
	}

	//get num of characteristic descriptor
	attribute = GetAttibuteByName(chara, NUM_CHACDES);
	if (attribute != NULL)
	{
		num_of_characteristic_descriptor = attribute->IntValue();
	}

	GF_CCharacteristic* charcteristic = new GF_CCharacteristic(handle, property, value_handle, index);
	//get uuid
	attribute = GetAttibuteByName(chara, UUID);
	if (attribute != NULL)
	{
		value = attribute->Value();
		GetUUIDFromChar(&(charcteristic->mUUID), value);
	}
	charcteristic->mNumOfDescriptor = num_of_characteristic_descriptor;

	if (charcteristic->mProperty & GF_ATT_PROPERTY_READ)
	{
		GetCharacteristicValue(chara->FirstChildElement(), charcteristic);
	}

	if (charcteristic->mNumOfDescriptor != 0)
	{
		GetCharacteristicDescriptor(chara->FirstChildElement(), charcteristic);
	}

	return charcteristic;
}

void GF_CDatabase::GetCharacteristic(GF_CPrimaryService* service, XMLElement *primaryservice)
{
	XMLElement *child = primaryservice;
	XMLElement *temp;

	while (child != NULL)
	{
		if (0 == memcmp(child->Name(), CHARACTERISTIC, strlen(CHARACTERISTIC)))
		{
			GF_CCharacteristic* characterisc = LoadCharacteristic(child);
			if (characterisc != NULL)
			{
				service->mCharacteristic.push_front(characterisc);
			}
		}

		temp = child->NextSiblingElement();
		child = temp;
	}
}

GF_CIncludeService* GF_CDatabase::LoadIncludedService(XMLElement *includeservice)
{
	GF_UINT16 start_handle = 0;
	GF_UINT16 end_handle = 0;
	GF_UINT16 handle = 0;
	GF_UINT8 index = 0;
	XMLAttribute* attribute;
	const char* value;
	//get start handle
	attribute = GetAttibuteByName(includeservice, START_HANDLE);
	if (attribute != NULL)
	{
		start_handle = attribute->IntValue();
	}

	//get end handle
	attribute = GetAttibuteByName(includeservice, END_HANDLE);
	if (attribute != NULL)
	{
		end_handle = attribute->IntValue();
	}

	//get index
	attribute = GetAttibuteByName(includeservice, INDEX);
	if (attribute != NULL)
	{
		index = attribute->IntValue();
	}

	//get num of included service
	attribute = GetAttibuteByName(includeservice, ATTRIHANDLE);
	if (attribute != NULL)
	{
		handle = attribute->IntValue();
	}

	GF_CIncludeService* included_service = new GF_CIncludeService(handle, start_handle, end_handle, index);
	//get uuid
	attribute = GetAttibuteByName(includeservice, UUID);
	if (attribute != NULL)
	{
		value = attribute->Value();
		GetUUIDFromChar(&(included_service->mUUID), value);
	}

	return included_service;
}

void GF_CDatabase::GetIncludedService(GF_CPrimaryService* service, XMLElement *element)
{
	XMLElement *child = element;
	XMLElement *temp;
	LOGDEBUG(mTag, "get included service \n");
	
	while (child != NULL)
	{
		if (0 == memcmp(child->Name(), INCLUDEDSERVICE, strlen(INCLUDEDSERVICE)))
		{
			GF_CIncludeService* includedsevice = LoadIncludedService(child);
			service->mIncludeService.push_front(includedsevice);
		}

		temp = child->NextSiblingElement();
		child = temp;
	}
}

GF_CPrimaryService* GF_CDatabase::LoadPrimaryService(XMLElement *primaryservice)
{	
	GF_UINT16 start_handle = 0;
	GF_UINT16 end_handle = 0;
	GF_UINT8 index = 0;
	GF_UINT8 num_of_included_service = 0;
	GF_UINT8 num_of_characteristic = 0;
	XMLAttribute* attribute;
	const char* value;
	//get start handle
	attribute = GetAttibuteByName(primaryservice, START_HANDLE);
	if (attribute != NULL)
	{
		start_handle = attribute->IntValue();
	}

	//get end handle
	attribute = GetAttibuteByName(primaryservice, END_HANDLE);
	if (attribute != NULL)
	{
		end_handle = attribute->IntValue();
	}

	//get index
	attribute = GetAttibuteByName(primaryservice, INDEX);
	if (attribute != NULL)
	{
		index = attribute->IntValue();
	}

	//get num of included service
	attribute = GetAttibuteByName(primaryservice, NUM_INC_SVC);
	if (attribute != NULL)
	{
		num_of_included_service = attribute->IntValue();
	}

	//get num of characteristic
	attribute = GetAttibuteByName(primaryservice, NUM_CHAC);
	if (attribute != NULL)
	{
		num_of_characteristic = attribute->IntValue();
	}

	GF_CPrimaryService* service = new GF_CPrimaryService(start_handle, end_handle, index);
	//get uuid
	attribute = GetAttibuteByName(primaryservice, UUID);
	if (attribute != NULL)
	{
		value = attribute->Value();
		GetUUIDFromChar(&(service->mUUID), value);
	}
	service->mNumOfIncludedService = num_of_included_service;
	service->mNumOfCharacteristic = num_of_characteristic;
	
	if (num_of_included_service != 0)
	{
		GetIncludedService(service, primaryservice->FirstChildElement());
	}

	if (num_of_characteristic != 0)
	{
		GetCharacteristic(service, primaryservice->FirstChildElement());
	}

	return service;
}

XMLElement* GF_CDatabase::FindElementByName(XMLElement *element, const char* name)
{
	XMLElement *Element = element;
	XMLElement *Temp;
	XMLElement *result = NULL;
	while (Element != NULL)
	{
		LOGDEBUG(mTag, "name of Node is %s \n", Element->Name());
		if (0 == memcmp(Element->Name(), name, strlen(name)))
		{
			result = Element;
			break;
		}
		else
		{
			Temp = Element->NextSiblingElement();
			Element = Temp;
		}
	}

	return result;
}

GF_STATUS GF_CDatabase::LoadService(GF_CService* service)
{
	GF_STATUS status = GF_FAIL;
	UINT8 index = 0;
	status = mDoc->LoadFile((const char*)mFilename);
	if (status != GF_OK)
	{
		return GF_FAIL;
	}

	XMLElement *Root = mDoc->RootElement();
	XMLElement *Gatt_Service = FindElementByName(Root, GATTSERVICE);

	if (Gatt_Service == NULL)
	{
		return GF_FAIL;
	}

	XMLElement *PrimaryService = Gatt_Service->FirstChildElement();
	XMLElement *TempPrimaryService;
	while (PrimaryService != NULL)
	{
		LOGDEBUG(mTag, "Process the %d th primaryservice \n", index);
		GF_CPrimaryService* priSvc = LoadPrimaryService(PrimaryService);
		service->mPrivateService.push_front(priSvc);

		TempPrimaryService = PrimaryService->NextSiblingElement();
		PrimaryService = TempPrimaryService;
		index++;
	}

	service->mNumOfPrivateService = index;

	return GF_OK;
}