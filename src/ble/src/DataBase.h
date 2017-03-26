#ifndef __GF_CDatabase_H__
#define __GF_CDatabase_H__
#include <LogPrint.h>
#include "GFBLETypes.h"
#include "tinyxml2.h"
#include "GattClient.h"
using namespace tinyxml2;
using namespace std;

#define MODUAL_TAG_DB "DataBase"

#define SECURITYINFOMATION  "SecurityInfomation"
#define KEYSIZE				"KeySize"
#define LTK					"Key"
#define RAND				"Rand"
#define DIV					"Div"
#define GATTSERVICE			"GATTService"

//#define PRIMARYSERVICE 0x2800
#define PRIMARYSERVICE       "PrimaryService"
#define INCLUDEDSERVICE      "IncludedService"
#define CHARACTERISTIC       "Characteristic"
#define CHARACTERISTICDES	 "CharacteristicDescriptor"
#define CHARACTERISTICVALUE  "CharacteristicValue"
#define ATTRITYPE		     "AttributeType"
#define ATTRIHANDLE			 "Handle"
#define INDEX				 "Index"
#define UUID				 "UUID"
#define START_HANDLE		 "StartHandle"
#define END_HANDLE			 "EndHandle"
#define NUM_INC_SVC			 "NumofIncludeService"
#define NUM_CHAC			 "NumofCharacterristic"
#define NUM_CHACDES			 "NumofCharacterristicDescriptor"
#define PROPERTY			 "Property"
#define VALUE_HANDLE		 "ValueHandle"
#define VALUE				 "Value"

class GF_CDatabase
{
public:
	GF_CDatabase(GF_PUINT8 address, GF_UINT8 length);
	~GF_CDatabase();

	GF_STATUS LoadLinkKey(GF_PUINT8 keysize, GF_PUINT8 key, GF_PUINT16 div, GF_PUINT8 rand);
	GF_STATUS SaveLinkKey(GF_UINT8 keysize, GF_PUINT8 key, GF_UINT16 div, GF_PUINT8 rand);

	GF_STATUS SaveService(GF_CService* service);
	GF_STATUS SavePrimaryService(XMLElement * GattSvc, GF_CPrimaryService* PrimaryService);
	GF_STATUS SaveCharacteristic(XMLElement *PriSvc, GF_CCharacteristic* Characteristic);
	GF_STATUS SaveCharacteristicDescriptor(XMLElement *Charc, GF_CCharacteristicDescriptor*Descriptor);
	GF_STATUS SaveChracteristicValue(XMLElement *Charc, GF_CCharacteristicValue* value);
	GF_STATUS SaveIncludedService(XMLElement *PriSvc, GF_CIncludeService* IncluSvc);
	GF_STATUS SaveUUID(XMLElement * Item, GF_UUID uuid);

	GF_STATUS LoadService(GF_CService* service);

	GF_STATUS DeleteLinkKey();
	GF_STATUS DeleteGattService();
	GF_STATUS DeleteNode(const char* NodeName);
private:

	void GetUUIDFromChar(GF_UUID *uuid, const char* value);
	XMLAttribute* GetAttibuteByName(XMLElement *element, const char* name);
	void GetCharacteristicDescriptor(XMLElement *chara, GF_CCharacteristic* charcteristic);
	void GetCharacteristicValue(XMLElement *chara, GF_CCharacteristic* charcteristic);
	GF_CCharacteristic* LoadCharacteristic(XMLElement *chara);
	void GetCharacteristic(GF_CPrimaryService* service, XMLElement *primaryservice);
	GF_CIncludeService* LoadIncludedService(XMLElement *includeservice);
	void GetIncludedService(GF_CPrimaryService* service, XMLElement *element);
	GF_CPrimaryService* LoadPrimaryService(XMLElement *primaryservice);
	XMLElement* FindElementByName(XMLElement *element, const char* name);
	char* mFilename;
	OYMXMLDocument* mDoc;
	GF_CPCHAR mTag;
};
#endif