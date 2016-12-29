/**
  @headerfile:       gatt.h
*/
#ifndef GATT_H
#define GATT_H
#pragma pack(1)
/*********************************************************************
*		ATT     CONSTANTS
*/

// The Exchanging MTU Size is defined as the maximum size of any packet 
// transmitted between a client and a server. A higher layer specification
// defines the default ATT MTU value. The ATT MTU value should be within
// the range 23 to 517 inclusive.
#define ATT_MTU_SIZE                     23 //!< Minimum ATT MTU size
#define ATT_MAX_MTU_SIZE                 517            //!< Maximum ATT MTU size

/** @} End ATT_MSG_EVENT_DEFINES */

/*** Opcode fields: bitmasks ***/
// Method (bits 5-0)
#define ATT_METHOD_BITS                  0x3f

// Command Flag (bit 6)
#define ATT_CMD_FLAG_BIT                 0x40

// Authentication Signature Flag (bit 7)
#define ATT_AUTHEN_SIG_FLAG_BIT          0x80

// Size of 16-bit Bluetooth UUID
#define ATT_BT_UUID_SIZE                 2

// Size of 128-bit UUID
#define ATT_UUID_SIZE                    16

// ATT Response or Confirmation timeout
#define ATT_MSG_TIMEOUT                  30

// Authentication Signature status for received PDU; it's TRUE or FALSE for PDU to be sent
#define ATT_SIG_NOT_INCLUDED             0x00 // Signature not included
#define ATT_SIG_VALID                    0x01 // Included signature valid
#define ATT_SIG_INVALID                  0x02 // Included signature not valid

/*********************************************************************
* Error Response: Error Code
*/

/** @defgroup ATT_ERR_CODE_DEFINES ATT Error Codes
* @{
*/
typedef enum {
	ATT_ERR_INVALID_HANDLE = 0x01, //!< Attribute handle value given was not valid on this attribute server
	ATT_ERR_READ_NOT_PERMITTED,       //0x02 //!< Attribute cannot be read
	ATT_ERR_WRITE_NOT_PERMITTED,      //0x03 //!< Attribute cannot be written
	ATT_ERR_INVALID_PDU,              //0x04 //!< The attribute PDU was invalid
	ATT_ERR_INSUFFICIENT_AUTHEN,      //0x05 //!< The attribute requires authentication before it can be read or written
	ATT_ERR_UNSUPPORTED_REQ,          //0x06 //!< Attribute server doesn't support the request received from the attribute client
	ATT_ERR_INVALID_OFFSET,           //0x07 //!< Offset specified was past the end of the attribute
	ATT_ERR_INSUFFICIENT_AUTHOR,      //0x08 //!< The attribute requires an authorization before it can be read or written
	ATT_ERR_PREPARE_QUEUE_FULL,       //0x09 //!< Too many prepare writes have been queued
	ATT_ERR_ATTR_NOT_FOUND,           //0x0a //!< No attribute found within the given attribute handle range
	ATT_ERR_ATTR_NOT_LONG,            //0x0b //!< Attribute cannot be read or written using the Read Blob Request or Prepare Write Request
	ATT_ERR_INSUFFICIENT_KEY_SIZE,    //0x0c //!< The Encryption Key Size used for encrypting this link is insufficient
	ATT_ERR_INVALID_VALUE_SIZE,       //0x0d //!< The attribute value length is invalid for the operation
	ATT_ERR_UNLIKELY,                 //0x0e //!< The attribute request that was requested has encountered an error that was very unlikely, and therefore could not be completed as requested
	ATT_ERR_INSUFFICIENT_ENCRYPT,     //0x0f //!< The attribute requires encryption before it can be read or written
	ATT_ERR_UNSUPPORTED_GRP_TYPE,     //0x10 //!< The attribute type is not a supported grouping attribute as defined by a higher layer specification
	ATT_ERR_INSUFFICIENT_RESOURCES,   //0x11 //!< Insufficient Resources to complete the request
	ATT_ERR_INVALID_VALUE = 0x80
} eAttErrCode;

/*** Reserved for future use: 0x12 - 0x7F ***/

/*** Application error code defined by a higher layer specification: 0x80-0x9F ***/

#define ATT_ERR_INVALID_VALUE            0x80 //!< The attribute value is invalid for the operation

/** @} End ATT_ERR_CODE_DEFINES */

/*********************************************************************
* Find Information Response: UUID Format
*/
// Handle and 16-bit Bluetooth UUID
#define ATT_HANDLE_BT_UUID_TYPE          0x01

// Handle and 128-bit UUID
#define ATT_HANDLE_UUID_TYPE             0x02

/*********************************************************************
* Read Multiple Request: Handles
*/
// Minimum number of handles in a single Read Multiple Request
#define ATT_MIN_NUM_HANDLES              2

/*********************************************************************
* Execute Write Request: Flags
*/
// Cancel all prepared writes
#define ATT_CANCEL_PREPARED_WRITES       0x00

// Immediately write all pending prepared values
#define ATT_WRITE_PREPARED_VALUES        0x01

#if defined ( TESTMODES )
// ATT Test Modes
#define ATT_TESTMODE_OFF               0 // Test mode off
#define ATT_TESTMODE_UNAUTHEN_SIG      1 // Do not authenticate incoming signature
#endif

/*********************************************************************
* Size of mandatory fields of ATT requests
*/

// Length of opcode field: Opcode (1)
#define ATT_OPCODE_SIZE                        1

// Length of Exchange MTU Request: client receive MTU size (2)
#define ATT_EXCHANGE_MTU_REQ_SIZE              2

// Length of Exchange MTU Response: server receive MTU size (2)
#define ATT_EXCHANGE_MTU_RSP_SIZE              2

// Length of Error Response: Command opcode in error (1) + Attribute handle in error (2) + Status code (1)
#define ATT_ERROR_RSP_SIZE                     4  

// Length of Find Information Request's fixed fields: First handle number (2) + Last handle number (2)
#define ATT_FIND_INFO_REQ_FIXED_SIZE           4

// Length of Find Information Response's fixed field: Format (1)
#define ATT_FIND_INFO_RSP_FIXED_SIZE           1
#define ATT_FIND_INFO_RSP_HDR_SIZE             ( ATT_OPCODE_SIZE + ATT_FIND_INFO_RSP_FIXED_SIZE )

// Length of Find By Type Value Request's fixed fields: Start handle (2) + End handle (2) + Type (2)
#define ATT_FIND_BY_TYPE_VALUE_REQ_FIXED_SIZE  6
#define ATT_FIND_BY_TYPE_VALUE_REQ_HDR_SIZE    ( ATT_OPCODE_SIZE + ATT_FIND_BY_TYPE_VALUE_REQ_FIXED_SIZE )

// Length of Read By Type Request's fixed fields: First handle number (2) + Last handle number (2)
#define ATT_READ_BY_TYPE_REQ_FIXED_SIZE        4
#define ATT_READ_BY_TYPE_REQ_HDR_SIZE          ( ATT_OPCODE_SIZE + ATT_READ_BY_TYPE_REQ_FIXED_SIZE )

// Length of Read By Type Response's fixed fields: Length (1)
#define ATT_READ_BY_TYPE_RSP_FIXED_SIZE        1
#define ATT_READ_BY_TYPE_RSP_HDR_SIZE          ( ATT_OPCODE_SIZE + ATT_READ_BY_TYPE_RSP_FIXED_SIZE )

// Length of Read Request: Attribute Handle (2)
#define ATT_READ_REQ_SIZE                      2

// Length of Read By Type Response's fixed fields: Length (1)
#define ATT_READ_BY_GRP_TYPE_RSP_FIXED_SIZE    1
#define ATT_READ_BY_GRP_TYPE_RSP_HDR_SIZE      ( ATT_OPCODE_SIZE + ATT_READ_BY_GRP_TYPE_RSP_FIXED_SIZE )

// Length of Write Request's fixed field: Attribute Handle (2)
#define ATT_WRITE_REQ_FIXED_SIZE               2

// Length of Read Blob Request: Attribute Handle (2) + Value Offset (2)
#define ATT_READ_BLOB_REQ_SIZE                 4

// Length of Write Request's fixed field: Attribute Handle (2)
#define ATT_WRITE_REQ_FIXED_SIZE               2
#define ATT_WRITE_REQ_HDR_SIZE                 ( ATT_OPCODE_SIZE + ATT_WRITE_REQ_FIXED_SIZE )

// Length of Prepare Write Request's fixed fields: Attribute Handle (2) + Value Offset (2)
#define ATT_PREPARE_WRITE_REQ_FIXED_SIZE       4
#define ATT_PREPARE_WRITE_REQ_HDR_SIZE         ( ATT_OPCODE_SIZE + ATT_PREPARE_WRITE_REQ_FIXED_SIZE )

// Length of Prepare Write Response's fixed size: Attribute Handle (2) + Value Offset (2)
#define ATT_PREPARE_WRITE_RSP_FIXED_SIZE       4

// Length of Execute Write Request: Flags (1)
#define ATT_EXECUTE_WRITE_REQ_SIZE             1

// Length of Handle Value Indication's fixed size: Attribute Handle (2)
#define ATT_HANDLE_VALUE_IND_FIXED_SIZE        2
#define ATT_HANDLE_VALUE_IND_HDR_SIZE          ( ATT_OPCODE_SIZE + ATT_HANDLE_VALUE_IND_FIXED_SIZE )

// Length of Authentication Signature field
#define ATT_AUTHEN_SIG_LEN                     12

/*********************************************************************
* VARIABLES
*/

extern CONST UINT8 btBaseUUID[ATT_UUID_SIZE];

/*********************************************************************
* MACROS
*/

#define ATT_WRITE_COMMAND( method, cmd ) ( ( (method) == ATT_WRITE_REQ ) && ( (cmd) == TRUE ) )

/*********************************************************************
* ATT Find Info Response macros
*/
// Handle and 16-bit Bluetooth UUID pair indexes
#define ATT_BT_PAIR_HANDLE_IDX( i )        ( (i) * (2 + ATT_BT_UUID_SIZE) )
#define ATT_BT_PAIR_UUID_IDX( i )          ( ATT_BT_PAIR_HANDLE_IDX( (i) ) + 2 )

#define ATT_BT_PAIR_HANDLE( info, i )      ( BUILD_UINT16( (info)[ATT_BT_PAIR_HANDLE_IDX((i))], \
                                                           (info)[ATT_BT_PAIR_HANDLE_IDX((i))+1] ) )
#define ATT_BT_PAIR_UUID( info, i )        ( BUILD_UINT16( (info)[ATT_BT_PAIR_UUID_IDX((i))], \
                                                           (info)[ATT_BT_PAIR_UUID_IDX((i))+1] ) )

// Handle and 128-bit UUID pair indexes
#define ATT_PAIR_HANDLE_IDX( i )           ( (i) * (2 + ATT_UUID_SIZE) )
#define ATT_PAIR_UUID_IDX( i )             ( ATT_PAIR_HANDLE_IDX( (i) ) + 2 )

#define ATT_PAIR_HANDLE( info, i )         ( BUILD_UINT16( (info)[ATT_PAIR_HANDLE_IDX((i))], \
                                                           (info)[ATT_PAIR_HANDLE_IDX((i))+1] ) )

/*********************************************************************
* ATT Find By Type Value Response macros
*/
// Attribute Handle and Group End Handle pair indexes
#define ATT_ATTR_HANDLE_IDX( i )           ( (i) * (2 + 2) )
#define ATT_GRP_END_HANDLE_IDX( i )        ( ATT_ATTR_HANDLE_IDX( (i) ) + 2 )

#define ATT_ATTR_HANDLE( info, i )         ( BUILD_UINT16( (info)[ATT_ATTR_HANDLE_IDX((i))], \
                                                           (info)[ATT_ATTR_HANDLE_IDX((i))+1] ) )
#define ATT_GRP_END_HANDLE( info, i )      ( BUILD_UINT16( (info)[ATT_GRP_END_HANDLE_IDX((i))], \
                                                           (info)[ATT_GRP_END_HANDLE_IDX((i))+1] ) )

/*********************************************************************
* ATT Read Multiple Request macros
*/
// Attribute Handle index
#define ATT_HANDLE_IDX( i )                ( (i) * 2 )

#define ATT_HANDLE( handles, i )           ( BUILD_UINT16( (handles)[ATT_HANDLE_IDX((i))], \
                                                           (handles)[ATT_HANDLE_IDX((i))+1] ) )

/*********************************************************************
* TYPEDEFS
*/

/**
* Attribute Protocol PDU format.
*/
typedef struct
{
	UINT8 sig;      //!< Authentication Signature status (not included (0), valid (1), invalid (2))
	UINT8 cmd;      //!< Command Flag
	UINT8 method;   //!< Method
	UINT16 len;     //!< Length of Attribute Parameters
	UINT8 *pParams; //!< Attribute Parameters
} attPacket_t;

/**
* Attribute Type format (2 or 16 octet UUID).
*/
typedef struct
{
	UINT8 len;                 //!< Length of UUID (2 or 16)
	UINT8 uuid[ATT_UUID_SIZE]; //!< 16 or 128 bit UUID
} attAttrType_t;

/**
* Attribute Type format (2-octet Bluetooth UUID).
*/
typedef struct
{
	UINT8 len;                    //!< Length of UUID (2)
	UINT8 uuid[ATT_BT_UUID_SIZE]; //!< 16 bit UUID
} attAttrBtType_t;

/**
* Error Response format.
*/
typedef struct
{
	UINT8 status;
	UINT8 reqOpcode; //!< Request that generated this error response
	UINT16 handle;   //!< Attribute handle that generated error response
	UINT8 errCode;   //!< Reason why the request has generated error response
} attErrorRsp_t;

/**
* Exchange MTU Request format.
*/
typedef struct
{
	UINT16 clientRxMTU; //!< Client receive MTU size 
} attExchangeMTUReq_t;

/**
* Exchange MTU Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 conHandle;   //!connection handle
	UINT8  pduLen;
	UINT16 serverRxMTU; //!< Server receive MTU size 
} attExchangeMTURsp_t;

/**
* Find Information Request format.
*/
typedef struct
{
	UINT16 startHandle;       //!< First requested handle number (must be first field)
	UINT16 endHandle;         //!< Last requested handle number
} attFindInfoReq_t;

/**
* Handle and its 16-bit Bluetooth UUIDs.
*/
typedef struct
{
	UINT16 handle;                //!< Handle
	UINT8 uuid[ATT_BT_UUID_SIZE]; //!< 2-octet Bluetooth UUID
} attHandleBtUUID_t;

/**
* Handle and its 128-bit UUID.
*/
typedef struct
{
	UINT16 handle;             //!< Handle
	UINT8 uuid[ATT_UUID_SIZE]; //!< 16-octect UUID
} attHandleUUID_t;

/**
* Find Information Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 conHandle;   //!connection handle
	UINT8  pduLen;
	UINT8 format;   //!< Format of information data
	union{
		attHandleBtUUID_t uuid2[1];
		attHandleUUID_t uuid16[1];
	}Info;//!< Information data whose format is determined by format field (4 to ATT_MTU_SIZE-2)
} attFindInfoRsp_t;

/**
* Find By Type Value Request format.
*/
typedef struct
{
	UINT16 startHandle;   //!< First requested handle number (must be first field)
	UINT16 endHandle;     //!< Last requested handle number
	attAttrBtType_t type; //!< 2-octet UUID to find
	UINT16 len;           //!< Length of value
	UINT8 *pValue;        //!< Attribute value to find (0 to ATT_MTU_SIZE-7)
} attFindByTypeValueReq_t;

/**
* Handles Information format.
*/
typedef struct
{
	UINT16 handle;       //!< Found attribute handle
	UINT16 grpEndHandle; //!< Group end handle
} attHandlesInfo_t;

/**
* Find By Type Value Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 conHandle;   //!connection handle
	UINT8  pduLen;
	UINT16 handle[1];//!< List of 1 or more handles information (4 to ATT_MTU_SIZE-1)
} attFindByTypeValueRsp_t;

/**
* Read By Type Request format.
*/
typedef struct
{
	UINT16 startHandle; //!< First requested handle number (must be first field)
	UINT16 endHandle;   //!< Last requested handle number
	attAttrType_t type; //!< Requested type (2 or 16 octet UUID)
} attReadByTypeReq_t;

/**
* Read By Type Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 conHdl;
	UINT8 pduLen;
	UINT8 len;       //!< Length of each attribute handle, end group handle and value set
	UINT8 data[1]; //!< List of 1 or more attribute handle-value pairs (2 to ATT_MTU_SIZE-2)
} attReadByTypeRsp_t;

/**
* Read Request format.
*/
typedef struct
{
	UINT16 handle; //!< Handle of the attribute to be read (must be first field)
} attReadReq_t;

/**
* Read Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 conHdl;
	UINT8  pduLen;    //!< Length of value
	UINT8  value[1]; //!< Value of the attribute with the handle given (0 to ATT_MTU_SIZE-1)
} attReadRsp_t;

/**
* Read Blob Req format.
*/
typedef struct
{
	UINT16 handle; //!< Handle of the attribute to be read (must be first field)
	UINT16 offset; //!< Offset of the first octet to be read
} attReadBlobReq_t;

/**
* Read Blob Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 len;    //!< Length of value
	UINT8 *pValue; //!< Part of the value of the attribute with the handle given (0 to ATT_MTU_SIZE-1)
} attReadBlobRsp_t;

/**
* Read Multiple Request format.
*/
typedef struct
{
	UINT8 *pHandles;   //!< Set of two or more attribute handles (4 to ATT_MTU_SIZE-1) - must be first field
	UINT16 numHandles; //!< Number of attribute handles
} attReadMultiReq_t;

/**
* Read Multiple Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 conHdl;
	UINT8  pduLen;    //!< Length of value
	UINT8  value[1]; //!< Set of two or more values (0 to ATT_MTU_SIZE-1)
} attReadMultiRsp_t;

/**
* Read By Group Type Request format.
*/
typedef struct
{
	UINT16 startHandle; //!< First requested handle number (must be first field)
	UINT16 endHandle;   //!< Last requested handle number
	attAttrType_t type; //!< Requested group type (2 or 16 octet UUID)
} attReadByGrpTypeReq_t;

/**
* Read By Group Type Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 conHdl;
	UINT8 pduLen;
	UINT8 len;       //!< Length of each attribute handle, end group handle and value set
	UINT8 data[1]; //!< List of 1 or more attribute handle, end group handle and value (4 to ATT_MTU_SIZE-2)
} attReadByGrpTypeRsp_t;

/**
* Write Request format.
*/
typedef struct
{
	UINT16 handle; //!< Handle of the attribute to be written (must be first field)
	UINT16 len;    //!< Length of value
	UINT8 *pValue; //!< Value of the attribute to be written (0 to ATT_MTU_SIZE-3)
	UINT8 sig;     //!< Authentication Signature status (not included (0), valid (1), invalid (2))
	UINT8 cmd;     //!< Command Flag
} attWriteReq_t;

/**
* Prepare Write Request format.
*/
typedef struct
{
	UINT16 handle; //!< Handle of the attribute to be written (must be first field)
	UINT16 offset; //!< Offset of the first octet to be written
	UINT16 len;    //!< Length of value
	UINT8 *pValue; //!< Part of the value of the attribute to be written (0 to ATT_MTU_SIZE-5) - must be allocated
} attPrepareWriteReq_t;

/**
* Prepare Write Response format.
*/
typedef struct
{
	UINT8 status;
	UINT16 handle; //!< Handle of the attribute that has been read
	UINT16 offset; //!< Offset of the first octet to be written
	UINT16 len;    //!< Length of value
	UINT8 *pValue; //!< Part of the value of the attribute to be written (0 to ATT_MTU_SIZE-5)
} attPrepareWriteRsp_t;

/**
* Execute Write Request format.
*/
typedef struct
{
	UINT8 flags; //!< 0x00 - cancel all prepared writes.
	//!< 0x01 - immediately write all pending prepared values.
} attExecuteWriteReq_t;

/**
* Handle Value Notification format.
*/
typedef struct
{
	UINT8 status;
	UINT16 handle; //!< Handle of the attribute that has been changed (must be first field)
	UINT16 len;    //!< Length of value
	UINT8 *pValue; //!< Current value of the attribute (0 to ATT_MTU_SIZE-3)
} attHandleValueNoti_t;

/**
* Handle Value Indication format.
*/
typedef struct
{
	UINT8 status;
	UINT16 handle; //!< Handle of the attribute that has been changed (must be first field)
	UINT16 len;    //!< Length of value
	UINT8 *pValue; //!< Current value of the attribute (0 to ATT_MTU_SIZE-3)
} attHandleValueInd_t;

/**
* The following two ATT events are generated locally (not received OTA) by
* the ATT Server or Client.
*/

/**
* ATT Flow Control Violated Event message format.  This message is sent to the
* app by the local ATT Server or Client when a sequential ATT Request-Response
* or Indication-Confirmation protocol flow control is violated for a connection.
* All subsequent ATT Requests and Indications received by the local ATT Server
* and Client respectively will be dropped.
*
* This message is to inform the app (that has registered with GAP by calling
* GAP_RegisterForMsgs()) in case it wants to drop the connection.
*/
typedef struct
{
	UINT8 status;
	UINT8 opcode;        //!< opcode of message that caused flow control violation
	UINT8 pendingOpcode; //!< opcode of pending message
} attFlowCtrlViolatedEvt_t;

/**
* ATT MTU Updated Event message format.  This message is sent to the app
* by the local ATT Server or Client when the ATT MTU size is updated for a
* connection. The default ATT MTU size is 23 octets.
*
* This message is to inform the app (that has registered with GAP by calling
* GAP_RegisterForMsgs()) about the new ATT MTU size negotiated for a connection.
*/
typedef struct
{
	UINT8 status;
	UINT16 MTU; //!< new MTU size
} attMtuUpdatedEvt_t;

/*********************************************************************
* CONSTANTS
*/

/** @defgroup GATT_PERMIT_BITMAPS_DEFINES GATT Attribute Access Permissions Bit Fields
* @{
*/

#define GATT_PERMIT_READ                 0x01 //!< Attribute is Readable
#define GATT_PERMIT_WRITE                0x02 //!< Attribute is Writable
#define GATT_PERMIT_AUTHEN_READ          0x04 //!< Read requires Authentication
#define GATT_PERMIT_AUTHEN_WRITE         0x08 //!< Write requires Authentication
#define GATT_PERMIT_AUTHOR_READ          0x10 //!< Read requires Authorization
#define GATT_PERMIT_AUTHOR_WRITE         0x20 //!< Write requires Authorization
#define GATT_PERMIT_ENCRYPT_READ         0x40 //!< Read requires Encryption
#define GATT_PERMIT_ENCRYPT_WRITE        0x80 //!< Write requires Encryption

/** @} End GATT_PERMIT_BITMAPS_DEFINES */


/** @defgroup GATT_NUM_PREPARE_WRITES_DEFINES GATT Maximum Number of Prepare Writes
* @{
*/

#if !defined( GATT_MAX_NUM_PREPARE_WRITES )
#define GATT_MAX_NUM_PREPARE_WRITES      5 //!< GATT Maximum number of attributes that Attribute Server can prepare for writing per Attribute Client
#endif

/** @} End GATT_NUM_PREPARE_WRITES_DEFINES */


/** @defgroup GATT_ENCRYPT_KEY_SIZE_DEFINES GATT Encryption Key Size Limits
* @{
*/

#define GATT_MIN_ENCRYPT_KEY_SIZE          7  //!< GATT Minimum Encryption Key Size
#define GATT_MAX_ENCRYPT_KEY_SIZE          16 //!< GATT Maximum Encryption Key Size

/** @} End GATT_ENCRYPT_KEY_SIZE_DEFINES */


/** @defgroup GATT_MAX_ATTR_SIZE_DEFINES GATT Maximum Attribute Value Length
* @{
*/

#define GATT_MAX_ATTR_SIZE               512 //!< GATT Maximum length of an attribute value

/** @} End GATT_MAX_ATTR_SIZE_DEFINES */

// GATT Base Method
#define GATT_BASE_METHOD                 0x40

// Attribute handle definitions
#define GATT_INVALID_HANDLE              0x0000 // Invalid attribute handle
#define GATT_MIN_HANDLE                  0x0001 // Minimum attribute handle
#define GATT_MAX_HANDLE                  0xFFFF // Maximum attribute handle

#define GATT_MAX_MTU                     0xFFFF // Maximum MTU size

/*********************************************************************
* VARIABLES
*/

/*********************************************************************
* MACROS
*/

// Attribute Access Permissions
#define gattPermitRead( a )              ( (a) & GATT_PERMIT_READ )
#define gattPermitWrite( a )             ( (a) & GATT_PERMIT_WRITE )
#define gattPermitAuthenRead( a )        ( (a) & GATT_PERMIT_AUTHEN_READ )
#define gattPermitAuthenWrite( a )       ( (a) & GATT_PERMIT_AUTHEN_WRITE )
#define gattPermitAuthorRead( a )        ( (a) & GATT_PERMIT_AUTHOR_READ )
#define gattPermitAuthorWrite( a )       ( (a) & GATT_PERMIT_AUTHOR_WRITE )
#define gattPermitEncryptRead( a )       ( (a) & GATT_PERMIT_ENCRYPT_READ )
#define gattPermitEncryptWrite( a )      ( (a) & GATT_PERMIT_ENCRYPT_WRITE )

/*********************************************************************
* TYPEDEFS
*/
/**
* GATT Find By Type Value Request format.
*/
typedef struct
{
	UINT16 startHandle;  //!< First requested handle number (must be first field)
	UINT16 endHandle;    //!< Last requested handle number
	attAttrType_t value; //!< Primary service UUID value (2 or 16 octets)
} gattFindByTypeValueReq_t;

/**
* GATT Read By Type Request format.
*/
typedef struct
{
	UINT8 discCharsByUUID;  //!< Whether this is a GATT Discover Characteristics by UUID sub-procedure
	attReadByTypeReq_t req; //!< Read By Type Request
} gattReadByTypeReq_t;

/**
* GATT Write Long Request format. Do not change the order of the members.
*/
typedef struct
{
	UINT8 reliable;           //!< Whether reliable writes requested (always FALSE for Write Long)
	attPrepareWriteReq_t req; //!< ATT Prepare Write Request
	UINT16 lastOffset;        //!< Offset of last Prepare Write Request sent
} gattWriteLongReq_t;

/**
* GATT Reliable Writes Request format. Do not change the order of the members.
*/
typedef struct
{
	UINT8 reliable;              //!< Whether reliable writes requested (always TRUE for Reliable Writes)
	attPrepareWriteReq_t *pReqs; //!< Array of Prepare Write Requests (must be allocated)
	UINT8 numReqs;               //!< Number of Prepare Write Requests
	UINT8 index;                 //!< Index of last Prepare Write Request sent
	UINT8 flags;                 //!< 0x00 - cancel all prepared writes.
	//!< 0x01 - immediately write all pending prepared values.
} gattReliableWritesReq_t;

/**
* GATT Attribute Type format.
*/
typedef struct {
	UINT8 len;         //!< Length of UUID (2 or 6)
	const UINT8* uuid; //!< Pointer to UUID
} gattAttrType_t;

/**
* GATT Attribute format.
*/
typedef struct attAttribute_t {
	gattAttrType_t type; //!< Attribute type (2 or 16 octet UUIDs)
	UINT8 permissions;   //!< Attribute permissions
	UINT16 handle;       //!< Attribute handle - assigned internally by attribute server
	UINT8* const
		pValue; //!< Attribute value - encoding of the octet array is defined in
	//!< the applicable profile. The maximum length of an attribute
	//!< value shall be 512 octets.
} gattAttribute_t;

/**
* GATT Service format.
*/
typedef struct {
	UINT16 numAttrs; //!< Number of attributes in attrs
	UINT8 encKeySize;//!< Minimum encryption key size required by service (7-16 bytes)

	/** Array of attribute records.
	*  NOTE: The list must start with a Service attribute followed by
	*        all attributes associated with this Service attribute.
	*/
	gattAttribute_t* attrs;
} gattService_t;


typedef union
{
	// Request messages
	attExchangeMTUReq_t exchangeMTUReq;              //!< ATT Exchange MTU Request
	attFindInfoReq_t findInfoReq;                    //!< ATT Find Information Request
	attFindByTypeValueReq_t findByTypeValueReq;      //!< ATT Find By Type Value Request
	attReadByTypeReq_t readByTypeReq;                //!< ATT Read By Type Request
	attReadReq_t readReq;                            //!< ATT Read Request
	attReadBlobReq_t readBlobReq;                    //!< ATT Read Blob Request
	attReadMultiReq_t readMultiReq;                  //!< ATT Read Multiple Request
	attReadByGrpTypeReq_t readByGrpTypeReq;          //!< ATT Read By Group Type Request
	attWriteReq_t writeReq;                          //!< ATT Write Request
	attPrepareWriteReq_t prepareWriteReq;            //!< ATT Prepare Write Request
	attExecuteWriteReq_t executeWriteReq;            //!< ATT Execute Write Request
	gattFindByTypeValueReq_t gattFindByTypeValueReq; //!< GATT Find By Type Value Request
	gattReadByTypeReq_t gattReadByTypeReq;           //!< GATT Read By Type Request
	gattWriteLongReq_t gattWriteLongReq;             //!< GATT Long Write Request
	gattReliableWritesReq_t gattReliableWritesReq;   //!< GATT Reliable Writes Request

	// Response messages
	attErrorRsp_t errorRsp;                          //!< ATT Error Response
	attExchangeMTURsp_t exchangeMTURsp;              //!< ATT Exchange MTU Response
	attFindInfoRsp_t findInfoRsp;                    //!< ATT Find Information Response
	attFindByTypeValueRsp_t findByTypeValueRsp;      //!< ATT Find By Type Value Response
	attReadByTypeRsp_t readByTypeRsp;                //!< ATT Read By Type Response
	attReadRsp_t readRsp;                            //!< ATT Read Response
	attReadBlobRsp_t readBlobRsp;                    //!< ATT Read Blob Response
	attReadMultiRsp_t readMultiRsp;                  //!< ATT Read Multiple Response
	attReadByGrpTypeRsp_t readByGrpTypeRsp;          //!< ATT Read By Group Type Response
	attPrepareWriteRsp_t prepareWriteRsp;            //!< ATT Prepare Write Response

	// Indication and Notification messages
	attHandleValueNoti_t handleValueNoti;            //!< ATT Handle Value Notification
	attHandleValueInd_t handleValueInd;              //!< ATT Handle Value Indication

	// Locally-generated event messages
	attFlowCtrlViolatedEvt_t flowCtrlEvt;            //!< ATT Flow Control Violated Event
	attMtuUpdatedEvt_t mtuEvt;                       //!< ATT MTU Updated Event
} u_GattMsg,u_AttMsg;

/**
* GATT OSAL GATT_MSG_EVENT message format. This message is used to forward an
* incoming attribute protocol/profile message up to upper layer application.
*/
typedef struct {
	//!< GATT_MSG_EVENT and status
	UINT16 connHandle;    //!< Connection message was received on
	UINT8 method;         //!< Type of message
	u_GattMsg msg;        //!< Attribute protocol/profile message
} gattMsgEvent_t;
#pragma pack()
#endif
