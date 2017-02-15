#include"stdafx.h"
#include "OYM_Database.h"
#include "oym_types.h"

OYM_CHAR int2char(OYM_UINT8 data)
{
	OYM_CHAR result = '#';

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

OYM_VOID arry2str(OYM_PUINT8 array, OYM_UINT32 length, char* result)
{
	OYM_UINT32 i = 0;

	for (i = 0; i < length; i++)
	{
		result[2 * i] = int2char((array[i] & 0xF0) >> 4);
		result[2 * i + 1] = int2char(array[i] & 0x0F);
	}
	result[2 * length ] = '\0';
}

OYM_UINT8 char2int8(const char* data)
{
	OYM_UINT8 result = '#';
	//printf("input: %c   ", *data);
	if ((*data) >= '0' && (*data) <= '9')
	{
		result = (*data) - '0';
	}

	if ((*data) >= 'A' && (*data) <= 'F')
	{
		result = (*data) - 'A' + 10;
	}

	//printf("output: %x \n", result);
	return result;
}

OYM_VOID str2arry(const char* str, OYM_PUINT8 array)
{
	int i = 0;
	int length = strlen(str);

	for (i = 0; i < length/2; i++)
	{
		array[i] = (char2int8(str + 2 * i) << 4);
		array[i] += char2int8(str + 2 * i + 1);
		printf("%02x", array[i]);
	}

	printf("\n");
}

OYM_Database::OYM_Database(OYM_PUINT8 address, OYM_UINT8 length)
{
	OYM_STATUS status = OYM_SUCCESS;
	mFilename = (char*)malloc(length * 2 + 1);
	arry2str(address, length, mFilename);

	mDoc = new OYMXMLDocument;
	status = mDoc->LoadFile((const char*)mFilename);
	if (status == OYM_SUCCESS)
	{
		printf("LoadFile success!!! \n");
	} 
	else
	{
		printf("LoadFile fail, file not exist!!! \n");
	}
}

OYM_Database::~OYM_Database()
{
	free(mFilename);

	//do not need to delete mDoc.
}


OYM_STATUS OYM_Database::DeleteNode(const char* NodeName)
{
	OYM_STATUS status = OYM_FAIL;
	status = mDoc->LoadFile((const char*)mFilename);
	if (status == OYM_FAIL)
	{
		printf("Load File Error!! \n");
		return status;
	}

	XMLElement *DeletetEle = mDoc->RootElement();
	XMLElement *Temp;
	while (DeletetEle != NULL)
	{
		printf("name of Node is %s \n", DeletetEle->Name());
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

		printf("name of deleting is %s \n", DeletetEle->Name());

		mDoc->DeleteNode(DeleteNode);
		mDoc->SaveFile((const char*)mFilename);
		status = OYM_SUCCESS;
	}
	else
	{
		printf("No Node found to delete!! \n");
	}

	return status;
}

OYM_STATUS OYM_Database::DeleteGattService()
{
	OYM_STATUS status = OYM_FAIL;
	status = DeleteNode(GATTSERVICE);

	return status;
}

OYM_STATUS OYM_Database::DeleteLinkKey()
{
	OYM_STATUS status = OYM_FAIL;
	status = DeleteNode(SECURITYINFOMATION);

	return status;
}

OYM_STATUS OYM_Database::LoadLinkKey(OYM_PUINT8 keysize, OYM_PUINT8 key, OYM_PUINT16 div, OYM_PUINT8 rand)
{
	OYM_STATUS result = OYM_FAIL;
	OYM_STATUS status = OYM_SUCCESS;
	
	if (mDoc == NULL) 
	{
		result = OYM_FAIL;
	}
	
	//file do not exist.
	status = mDoc->LoadFile((const char*)mFilename);
	if (status == OYM_SUCCESS)
	{
		printf("LoadFile success!!! \n");
		XMLElement *rootEle = mDoc->RootElement();	
		XMLElement *Temp;
		while (rootEle != NULL)
		{
			if (0 == memcmp(rootEle->Name(), SECURITYINFOMATION, strlen(SECURITYINFOMATION)))
			{
				printf("SecurityInfomation found \n");
				result = OYM_SUCCESS;
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
						printf("First child Element attribute name = %s attribute value = %s\n", attr->Name(), attr->Value());
						str2arry(attr->Value(), rand);
					}
					else if (0 == memcmp(child->Name(), LTK, strlen(LTK)))
					{
						attr = child->FirstAttribute();
						printf("First child Element attribute name = %s attribute value = %s\n", attr->Name(), attr->Value());
						str2arry(attr->Value(), key);
					}

					next_child = child->NextSiblingElement();
					child = next_child;
				}
				printf("Load link key done!! \n");
			}

			Temp = rootEle->NextSiblingElement();
			rootEle = Temp;
		}
	}
	else
	{
		printf("LoadFile fail, file not exist!!! \n");
		result = OYM_FAIL;
	}

	return result;
}

OYM_STATUS OYM_Database::SaveLinkKey(OYM_UINT8 keysize, OYM_PUINT8 key, OYM_UINT16 div, OYM_PUINT8 rand)
{
	OYM_STATUS status = OYM_FAIL;
	OYM_UINT8 size = (min(keysize, 8))*2 + 1;
	char data[100];
	memset(data, 0, size);

	//load file first
	status = mDoc->LoadFile((const char*)mFilename);
	if (status != OYM_SUCCESS)
	{
		free(data);
		return status;
	}
	else
	{
		DeleteLinkKey();
	}

	status = mDoc->LoadFile((const char*)mFilename);
	XMLElement * SecurityInfomation = mDoc->NewElement(SECURITYINFOMATION);
	mDoc->LinkEndChild(SecurityInfomation);

	printf("start to save linkkey... \n");
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

	printf("linkkey successful... \n");
	return OYM_SUCCESS;
}

OYM_STATUS OYM_Database::SaveIncludedService(XMLElement *PriSvc, OYM_INCSERVICE* IncluSvc)
{
	if (PriSvc == NULL)
	{
		return OYM_FAIL;
	}

	XMLElement *IncludeService = mDoc->NewElement(INCLUDEDSERVICE);
	IncludeService->SetAttribute(INDEX, IncluSvc->mIndex);
	IncludeService->SetAttribute(HANDLE, IncluSvc->mHandle);
	SaveUUID(IncludeService, IncluSvc->mUUID);	
	IncludeService->SetAttribute(START_HANDLE, IncluSvc->mStartHandle);
	IncludeService->SetAttribute(END_HANDLE, IncluSvc->mEndHandle);
	PriSvc->LinkEndChild(IncludeService);

	return OYM_SUCCESS;
}

OYM_STATUS OYM_Database::SaveCharacteristicDescriptor(XMLElement *Charc, OYM_CHAR_DESCRIPTOR* Descriptor)
{
	char *data;
	if (Charc == NULL)
	{
		return OYM_FAIL;
	}

	XMLElement *CharcDescrip = mDoc->NewElement(CHARACTERISTICDES);
	CharcDescrip->SetAttribute(INDEX, Descriptor->mIndex);
	CharcDescrip->SetAttribute(HANDLE, Descriptor->mHandle);
	SaveUUID(CharcDescrip, Descriptor->mUUID);

	if (Descriptor->mUUID.value.uuid16 != 0x2902)
	{
		data = (char*)malloc((Descriptor->mDataLen * 2) + 1);
		arry2str(Descriptor->mData, Descriptor->mDataLen, data);
		CharcDescrip->SetAttribute(VALUE, data);
		free(data);
	}
	Charc->LinkEndChild(CharcDescrip);
	return OYM_SUCCESS;
}

OYM_STATUS OYM_Database::SaveChracteristicValue(XMLElement *Charc, OYM_CHAR_VALUE* value)
{
	char *data;
	XMLElement *CharcValue = mDoc->NewElement(CHARACTERISTICVALUE);
	CharcValue->SetAttribute(HANDLE, value->mHandle);

	data = (char*)malloc((value->mLength * 2) + 1);
	arry2str(value->mData, value->mLength, data);
	CharcValue->SetAttribute(VALUE, data);

	Charc->LinkEndChild(CharcValue);
	return OYM_SUCCESS;
}

OYM_STATUS OYM_Database::SaveCharacteristic(XMLElement *PriSvc, OYM_CHARACTERISTIC* Characteristic)
{
	if (PriSvc == NULL)
	{
		return OYM_FAIL;
	}

	XMLElement *Charc = mDoc->NewElement(CHARACTERISTIC);
	Charc->SetAttribute(INDEX, Characteristic->mIndex);
	Charc->SetAttribute(HANDLE, Characteristic->mHandle);
	SaveUUID(Charc, Characteristic->mUUID);
	Charc->SetAttribute(PROPERTY, Characteristic->mProperty);
	Charc->SetAttribute(VALUE_HANDLE, Characteristic->mValueHandle);
	Charc->SetAttribute(NUM_CHACDES, Characteristic->mNumOfDescriptor);
	
	PriSvc->LinkEndChild(Charc);

	if (Characteristic->mProperty & OYM_ATT_PROPERTY_READ)
	{
		SaveChracteristicValue(Charc, Characteristic->mAttriValue);
	}

	for (int i = 0; i < Characteristic->mNumOfDescriptor; i++)
	{
		OYM_CHAR_DESCRIPTOR* Descriptor = Characteristic->FindDescriptorByIndex(i);
		SaveCharacteristicDescriptor(Charc, Descriptor);
	}

	return OYM_SUCCESS;
}
OYM_STATUS OYM_Database::SaveUUID(XMLElement * Item, OYM_UUID uuid)
{
	char *data = (char*)malloc(128);
	memset(data, 0, 128);
	OYM_UINT8 array[2] = {0x00, 0x00};
	if (Item == NULL)
	{
		free(data);
		return OYM_FAIL;
	}

	if (uuid.type == OYM_UUID_16)
	{
		array[0] = (uuid.value.uuid16 >> 8) & 0xFF;
		array[1] = uuid.value.uuid16 & 0xFF;
		arry2str(array, 2, data);
		Item->SetAttribute(UUID, data);
	}
	else if (uuid.type == OYM_UUID_32)
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

	return OYM_SUCCESS;
}

OYM_STATUS OYM_Database::SavePrimaryService(XMLElement * GattSvc, OYM_PRISERVICE* PrimaryService)
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
		OYM_INCSERVICE* IncluSvc = PrimaryService->FindIncluSvcbyIndex(i);
		SaveIncludedService(PriSvc, IncluSvc);
	}

	for (int i = 0; i < PrimaryService->mNumOfCharacteristic; i++)
	{
		OYM_CHARACTERISTIC* Characteristic = PrimaryService->FindCharacteristicbyIndex(i);
		SaveCharacteristic(PriSvc, Characteristic);
	}

	return OYM_SUCCESS;
}

OYM_STATUS OYM_Database::SaveService(OYM_Service* service)
{
	OYM_STATUS status = OYM_FAIL;
	//load file first
	status = mDoc->LoadFile((const char*)mFilename);
	if (status != OYM_SUCCESS)
	{
		return OYM_FAIL;
	}

	XMLElement * GattSvc = mDoc->NewElement(GATTSERVICE);
	mDoc->LinkEndChild(GattSvc);

	OYM_PRISERVICE* PrimaryService;
	for (int i = 0; i < service->mNumOfPrivateService; i++ )
	{
		printf("SaveService index = %d", i);
		PrimaryService = service->FindPriSvcbyIndex(i);
		SavePrimaryService(GattSvc, PrimaryService);
	}

	mDoc->SaveFile((const char*)mFilename);
	return OYM_SUCCESS;
}

void GetUUIDFromChar(OYM_UUID *uuid, const char* value)
{
	UINT8 len = strlen(value);
	UINT8 data[2];
	printf("string = %s \n", value);
	printf("len = %d \n", len);

	if (len == 0x04)
	{
		uuid->type = OYM_UUID_16;
		str2arry(value, data);
		uuid->value.uuid16 = data[1] + (data[0] << 8);
	}
	else if (len == 32)
	{
		uuid->type = OYM_UUID_128;
		str2arry(value, uuid->value.uuid128);
	}

	else
	{
		uuid->type = OYM_UUID_32;
		str2arry(value, uuid->value.uuid32);
	}
}

XMLAttribute* GetAttibuteByName(XMLElement *element, const char* name)
{
	XMLAttribute* result = NULL;
	const XMLAttribute* a = 0;
	for (a = element->FirstAttribute(); a; a = a->Next()) {
		if (XMLUtil::StringEqual(a->Name(), name))
			return (XMLAttribute*)a;
	}

	return result;
}

void GetCharacteristicDescriptor(XMLElement *chara, OYM_CHARACTERISTIC* charcteristic)
{
	OYM_UINT8 index = 0;
	OYM_UINT16 handle = 0;
	XMLElement *chrades = chara;
	XMLElement *temp;
	const XMLAttribute* attribute;
	const char* value;

	while (chrades != NULL)
	{
		printf(" %s \n", chrades->Name());
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
			attribute = GetAttibuteByName(chrades, HANDLE);
			if (attribute != NULL)
			{
				handle = attribute->IntValue();
				printf("descriptor handle = %d \n", handle);
			}

			OYM_CHAR_DESCRIPTOR * descriptor = new OYM_CHAR_DESCRIPTOR(handle, index);
			//get uuid
			attribute = GetAttibuteByName(chrades, UUID);
			if (attribute != NULL)
			{
				value = attribute->Value();
				GetUUIDFromChar(&(descriptor->mUUID), value);
			}
#if 0
			if (!((descriptor->mUUID.type == OYM_UUID_16) && (descriptor->mUUID.value.uuid16 == 0x2902)))
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

void GetCharacteristicValue(XMLElement *chara, OYM_CHARACTERISTIC* charcteristic)
{
	XMLElement *chracvalue = chara;
	XMLElement *temp;
	const XMLAttribute* attribute;
	const char* value;

	printf("GetCharacteristicValue \n");
	while (chracvalue != NULL)
	{
		printf("Node name is %s", chracvalue->Name());
		if (0 == memcmp(chracvalue->Name(), CHARACTERISTICVALUE, strlen(CHARACTERISTICVALUE)))
		{
			attribute = chracvalue->FirstAttribute();

			attribute = GetAttibuteByName(chara, VALUE_HANDLE);
			if (attribute != NULL)
			{
				charcteristic->mAttriValue->mHandle = attribute->IntValue();
			}
			
			attribute = GetAttibuteByName(chara, VALUE);
			value = attribute->Value();
			charcteristic->mAttriValue->mLength = (strlen(value) / 2);
			charcteristic->mAttriValue->mData = (OYM_PUINT8)malloc(charcteristic->mAttriValue->mLength);
			str2arry(value, charcteristic->mAttriValue->mData);

			return;
		}

		temp = chracvalue->NextSiblingElement();
		chracvalue = temp;
	}
}

OYM_CHARACTERISTIC* LoadCharacteristic(XMLElement *chara)
{
	OYM_UINT16 value_handle = 0;
	OYM_UINT16 handle = 0;
	OYM_UINT8 index = 0, i;
	OYM_UINT8 property = 0;
	OYM_UINT8 num_of_characteristic_descriptor = 0;
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
	attribute = GetAttibuteByName(chara, HANDLE);
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

	OYM_CHARACTERISTIC* charcteristic = new OYM_CHARACTERISTIC(handle, property, value_handle, index);
	//get uuid
	attribute = GetAttibuteByName(chara, UUID);
	if (attribute != NULL)
	{
		value = attribute->Value();
		GetUUIDFromChar(&(charcteristic->mUUID), value);
	}
	charcteristic->mNumOfDescriptor = num_of_characteristic_descriptor;

	if (charcteristic->mProperty & OYM_ATT_PROPERTY_READ)
	{
		GetCharacteristicValue(chara->FirstChildElement(), charcteristic);
	}

	if (charcteristic->mNumOfDescriptor != 0)
	{
		GetCharacteristicDescriptor(chara->FirstChildElement(), charcteristic);
	}

	return charcteristic;
}

void GetCharacteristic(OYM_PRISERVICE* service, XMLElement *primaryservice)
{
	XMLElement *child = primaryservice;
	XMLElement *temp;

	printf("GetCharacteristic \n");
	while (child != NULL)
	{
		printf("Node name is %s \n", child->Name());
		if (0 == memcmp(child->Name(), CHARACTERISTIC, strlen(CHARACTERISTIC)))
		{
			OYM_CHARACTERISTIC* characterisc = LoadCharacteristic(child);
			if (characterisc != NULL)
			{
				service->mCharacteristic.push_front(characterisc);
			}
		}

		temp = child->NextSiblingElement();
		child = temp;
	}
}

OYM_INCSERVICE* LoadIncludedService(XMLElement *includeservice)
{
	OYM_UINT16 start_handle = 0;
	OYM_UINT16 end_handle = 0;
	OYM_UINT16 handle = 0;
	OYM_UINT8 index = 0, i;
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
	attribute = GetAttibuteByName(includeservice, HANDLE);
	if (attribute != NULL)
	{
		handle = attribute->IntValue();
	}

	OYM_INCSERVICE* included_service = new OYM_INCSERVICE(handle, start_handle, end_handle, index);
	//get uuid
	attribute = GetAttibuteByName(includeservice, UUID);
	if (attribute != NULL)
	{
		value = attribute->Value();
		GetUUIDFromChar(&(included_service->mUUID), value);
	}

	return included_service;
}

void GetIncludedService(OYM_PRISERVICE* service, XMLElement *element)
{
	XMLElement *child = element;
	XMLElement *temp;
	printf("get included service \n");
	
	while (child != NULL)
	{
		printf("Node name = %s \n", child->Name());
		if (0 == memcmp(child->Name(), INCLUDEDSERVICE, strlen(INCLUDEDSERVICE)))
		{
			OYM_INCSERVICE* includedsevice = LoadIncludedService(child);
			service->mIncludeService.push_front(includedsevice);
		}

		temp = child->NextSiblingElement();
		child = temp;
	}
}

OYM_PRISERVICE* LoadPrimaryService(XMLElement *primaryservice)
{	
	OYM_UINT16 start_handle = 0;
	OYM_UINT16 end_handle = 0;
	OYM_UINT8 index = 0, i;
	OYM_UINT8 num_of_included_service = 0;
	OYM_UINT8 num_of_characteristic = 0;
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

	OYM_PRISERVICE* service = new OYM_PRISERVICE(start_handle, end_handle, index);
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

XMLElement *FindElementByName(XMLElement *element, const char* name)
{
	XMLElement *Element = element;
	XMLElement *Temp;
	XMLElement *result = NULL;
	while (Element != NULL)
	{
		printf("name of Node is %s \n", Element->Name());
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

OYM_STATUS OYM_Database::LoadService(OYM_Service* service)
{
	OYM_STATUS status = OYM_FAIL;
	UINT8 index = 0;
	status = mDoc->LoadFile((const char*)mFilename);
	if (status != OYM_SUCCESS)
	{
		return OYM_FAIL;
	}

	XMLElement *Root = mDoc->RootElement();
	XMLElement *Gatt_Service = FindElementByName(Root, GATTSERVICE);

	if (Gatt_Service == NULL)
	{
		return OYM_FAIL;
	}

	XMLElement *PrimaryService = Gatt_Service->FirstChildElement();
	XMLElement *TempPrimaryService;
	while (PrimaryService != NULL)
	{
		printf("Process the %d th primaryservice\n", index);
		OYM_PRISERVICE* priSvc = LoadPrimaryService(PrimaryService);
		service->mPrivateService.push_front(priSvc);

		TempPrimaryService = PrimaryService->NextSiblingElement();
		PrimaryService = TempPrimaryService;
		index++;
	}

	service->mNumOfPrivateService = index;

	return OYM_SUCCESS;
}