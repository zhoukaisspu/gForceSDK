#ifndef __OYM_DATABASE_H__
#define __OYM_DATABASE_H__
#include <OYM_Log.h>
#include "oym_types.h"
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
#define HANDLE				 "Handle"
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

class OYM_Database
{
public:
	OYM_Database(OYM_PUINT8 address, OYM_UINT8 length);
	~OYM_Database();

	OYM_STATUS LoadLinkKey(OYM_PUINT8 keysize, OYM_PUINT8 key, OYM_PUINT16 div, OYM_PUINT8 rand);
	OYM_STATUS SaveLinkKey(OYM_UINT8 keysize, OYM_PUINT8 key, OYM_UINT16 div, OYM_PUINT8 rand);

	OYM_STATUS SaveService(OYM_Service* service);
	OYM_STATUS SavePrimaryService(XMLElement * GattSvc, OYM_PRISERVICE* PrimaryService);
	OYM_STATUS SaveCharacteristic(XMLElement *PriSvc, OYM_CHARACTERISTIC* Characteristic);
	OYM_STATUS SaveCharacteristicDescriptor(XMLElement *Charc, OYM_CHAR_DESCRIPTOR*Descriptor);
	OYM_STATUS SaveChracteristicValue(XMLElement *Charc, OYM_CHAR_VALUE* value);
	OYM_STATUS SaveIncludedService(XMLElement *PriSvc, OYM_INCSERVICE* IncluSvc);
	OYM_STATUS SaveUUID(XMLElement * Item, OYM_UUID uuid);

	OYM_STATUS LoadService(OYM_Service* service);

	OYM_STATUS DeleteLinkKey();
	OYM_STATUS DeleteGattService();
	OYM_STATUS DeleteNode(const char* NodeName);
private:

	void GetUUIDFromChar(OYM_UUID *uuid, const char* value);
	XMLAttribute* GetAttibuteByName(XMLElement *element, const char* name);
	void GetCharacteristicDescriptor(XMLElement *chara, OYM_CHARACTERISTIC* charcteristic);
	void GetCharacteristicValue(XMLElement *chara, OYM_CHARACTERISTIC* charcteristic);
	OYM_CHARACTERISTIC* LoadCharacteristic(XMLElement *chara);
	void GetCharacteristic(OYM_PRISERVICE* service, XMLElement *primaryservice);
	OYM_INCSERVICE* LoadIncludedService(XMLElement *includeservice);
	void GetIncludedService(OYM_PRISERVICE* service, XMLElement *element);
	OYM_PRISERVICE* LoadPrimaryService(XMLElement *primaryservice);
	XMLElement* FindElementByName(XMLElement *element, const char* name);
	char* mFilename;
	OYMXMLDocument* mDoc;
	OYM_Log* mLog;
};
#endif