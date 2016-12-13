
#ifndef _NPI_EVT_H_
#define _NPI_EVT_H_

#include"Thread.h"
#include"npi_cmd.h"
#include"com.h"

#pragma pack(1)
/*---------------------------
/*			EVT				/
/*-------------------------*/
typedef struct {
	UINT8 type;
	UINT8 event_code;
	UINT8 len;
	UINT8 event_lo;
	UINT8 event_hi;
	UINT8 status;
	UINT8 data[MAX_RX_SIZE];
}sNpiEvt;

typedef struct {
	UINT8 type;
	UINT8 event_code;
	UINT8 len;
	UINT8 packets;
	UINT8 op_lo;
	UINT8 op_hi;
	UINT8 status;
	UINT8 data[MAX_RX_SIZE];
}sHciEvt;

#define HCI_EXT_LL_EVENT							0x0400
#define HCI_EXT_L2CAP_EVENT							0x0480
#define HCI_EXT_ATT_EVENT							0x0500
#define HCI_EXT_GATT_EVENT						    0x0580
#define HCI_EXT_GAP_EVENT						    0x0600
/*
** HCI Event Codes
*/

// BT Events
#define HCI_DISCONNECTION_COMPLETE_EVENT_CODE             0x05
#define HCI_ENCRYPTION_CHANGE_EVENT_CODE                  0x08
#define HCI_READ_REMOTE_INFO_COMPLETE_EVENT_CODE          0x0C
#define HCI_COMMAND_COMPLETE_EVENT_CODE                   0x0E
#define HCI_COMMAND_STATUS_EVENT_CODE                     0x0F
#define HCI_BLE_HARDWARE_ERROR_EVENT_CODE                 0x10
#define HCI_NUM_OF_COMPLETED_PACKETS_EVENT_CODE           0x13
#define HCI_DATA_BUFFER_OVERFLOW_EVENT                    0x1A
#define HCI_KEY_REFRESH_COMPLETE_EVENT_CODE               0x30
#define HCI_APTO_EXPIRED_EVENT_CODE                       0x57

// LE Event Code (for LE Meta Events)
#define HCI_LE_EVENT_CODE                                 0x0E

// LE Meta Event Codes
#define HCI_BLE_CONNECTION_COMPLETE_EVENT                 0x01
#define HCI_BLE_ADV_REPORT_EVENT                          0x02
#define HCI_BLE_CONN_UPDATE_COMPLETE_EVENT                0x03
#define HCI_BLE_READ_REMOTE_FEATURE_COMPLETE_EVENT        0x04
#define HCI_BLE_LTK_REQUESTED_EVENT                       0x05
// V4.1
#define HCI_BLE_REMOTE_CONN_PARAM_REQUEST_EVENT           0x06
// V4.2
#define HCI_BLE_DATA_LENGTH_CHANGE_EVENT                  0x07
#define HCI_BLE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE_EVENT 0x08
#define HCI_BLE_GENERATE_DHKEY_COMPLETE_EVENT             0x09
#define HCI_BLE_ENHANCED_CONNECTION_COMPLETE_EVENT        0x0A
#define HCI_BLE_DIRECT_ADVERTISING_REPORT_EVENT           0x0B
// V5.0
#define HCI_BLE_PHY_UPDATE_COMPLETE_EVENT                 0x0C

// Vendor Specific Event Code
#define HCI_VE_EVENT_CODE                                 0xFF

// LE Vendor Specific LL Extension Events
#define HCI_EXT_SET_RX_GAIN_EVENT                         0x0400
#define HCI_EXT_SET_TX_POWER_EVENT                        0x0401
#define HCI_EXT_ONE_PKT_PER_EVT_EVENT                     0x0402
#define HCI_EXT_CLK_DIVIDE_ON_HALT_EVENT                  0x0403
#define HCI_EXT_DECLARE_NV_USAGE_EVENT                    0x0404
#define HCI_EXT_DECRYPT_EVENT                             0x0405
#define HCI_EXT_SET_LOCAL_SUPPORTED_FEATURES_EVENT        0x0406
#define HCI_EXT_SET_FAST_TX_RESP_TIME_EVENT               0x0407
#define HCI_EXT_MODEM_TEST_TX_EVENT                       0x0408
#define HCI_EXT_MODEM_HOP_TEST_TX_EVENT                   0x0409
#define HCI_EXT_MODEM_TEST_RX_EVENT                       0x040A
#define HCI_EXT_END_MODEM_TEST_EVENT                      0x040B
#define HCI_EXT_SET_BDADDR_EVENT                          0x040C
#define HCI_EXT_SET_SCA_EVENT                             0x040D
#define HCI_EXT_ENABLE_PTM_EVENT                          0x040E // Not a supported HCI command! Application only.
#define HCI_EXT_SET_FREQ_TUNE_EVENT                       0x040F
#define HCI_EXT_SAVE_FREQ_TUNE_EVENT                      0x0410
#define HCI_EXT_SET_MAX_DTM_TX_POWER_EVENT                0x0411
#define HCI_EXT_MAP_PM_IO_PORT_EVENT                      0x0412
#define HCI_EXT_DISCONNECT_IMMED_EVENT                    0x0413
#define HCI_EXT_PER_EVENT                                 0x0414
#define HCI_EXT_PER_BY_CHAN_EVENT                         0x0415 // Not a supported HCI command! Application only.
#define HCI_EXT_EXTEND_RF_RANGE_EVENT                     0x0416
#define HCI_EXT_ADV_EVENT_NOTICE_EVENT                    0x0417 // Not a supported HCI command! Application only.
#define HCI_EXT_CONN_EVENT_NOTICE_EVENT                   0x0418 // Not a supported HCI command! Application only.
#define HCI_EXT_HALT_DURING_RF_EVENT                      0x0419
#define HCI_EXT_OVERRIDE_SL_EVENT                         0x041A
#define HCI_EXT_BUILD_REVISION_EVENT                      0x041B
#define HCI_EXT_DELAY_SLEEP_EVENT                         0x041C
#define HCI_EXT_RESET_SYSTEM_EVENT                        0x041D
#define HCI_EXT_OVERLAPPED_PROCESSING_EVENT               0x041E
#define HCI_EXT_NUM_COMPLETED_PKTS_LIMIT_EVENT            0x041F
#define HCI_EXT_GET_CONNECTION_INFO_EVENT                 0x0420

#define HCI_EXT_LL_TEST_MODE_EVENT                        0x0470

/*------------------GAP Events-----------------*/
#define HCI_EXT_GAP_DEVICE_INIT_DONE_EVENT          ( HCI_EXT_GAP_EVENT | 0x00 )
#define HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT          ( HCI_EXT_GAP_EVENT | 0x01 )
#define HCI_EXT_GAP_ADV_DATA_UPDATE_DONE_EVENT      ( HCI_EXT_GAP_EVENT | 0x02 )
#define HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_EVENT    ( HCI_EXT_GAP_EVENT | 0x03 )
#define HCI_EXT_GAP_END_DISCOVERABLE_DONE_EVENT     ( HCI_EXT_GAP_EVENT | 0x04 )
#define HCI_EXT_GAP_LINK_ESTABLISHED_EVENT          ( HCI_EXT_GAP_EVENT | 0x05 )
#define HCI_EXT_GAP_LINK_TERMINATED_EVENT           ( HCI_EXT_GAP_EVENT | 0x06 )
#define HCI_EXT_GAP_LINK_PARAM_UPDATE_EVENT         ( HCI_EXT_GAP_EVENT | 0x07 )
#define HCI_EXT_GAP_RANDOM_ADDR_CHANGED_EVENT       ( HCI_EXT_GAP_EVENT | 0x08 )
#define HCI_EXT_GAP_SIGNATURE_UPDATED_EVENT         ( HCI_EXT_GAP_EVENT | 0x09 )
#define HCI_EXT_GAP_AUTH_COMPLETE_EVENT             ( HCI_EXT_GAP_EVENT | 0x0A )
#define HCI_EXT_GAP_PASSKEY_NEEDED_EVENT            ( HCI_EXT_GAP_EVENT | 0x0B )
#define HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_EVENT  ( HCI_EXT_GAP_EVENT | 0x0C )
#define HCI_EXT_GAP_DEVICE_INFO_EVENT               ( HCI_EXT_GAP_EVENT | 0x0D )
#define HCI_EXT_GAP_BOND_COMPLETE_EVENT             ( HCI_EXT_GAP_EVENT | 0x0E )
#define HCI_EXT_GAP_PAIRING_REQ_EVENT               ( HCI_EXT_GAP_EVENT | 0x0F )

#define HCI_EXT_GAP_CMD_STATUS_EVENT                ( HCI_EXT_GAP_EVENT | 0x7F )//0x067F

/*------------------L2CAP Events-----------------*/
#define L2CAP_COMMAND_REJECT_EVT         (HCI_EXT_L2CAP_EVENT | 0x01)//0x0481 !< Command Reject
#define L2CAP_DISCONNECT_RSP_EVT         (HCI_EXT_L2CAP_EVENT | 0x07)//0x0487 !< Disconnection Response
#define L2CAP_INFO_RSP_EVT               (HCI_EXT_L2CAP_EVENT | 0x0b)//0x048b !< Information Response - not supported
#define L2CAP_PARAM_UPDATE_RSP_EVT       (HCI_EXT_L2CAP_EVENT | 0x13)//0x0493 !< Connection Parameter Update Response
#define L2CAP_CONNECT_RSP_EVT            (HCI_EXT_L2CAP_EVENT | 0x15)//0x0495 !< LE Credit Based Connection Response

#define L2CAP_CHANNEL_ESTABLISHED_EVT    (HCI_EXT_L2CAP_EVENT | 0x60)//0x04E0 !< Channel Established Event
#define L2CAP_CHANNEL_TERMINATED_EVT     (HCI_EXT_L2CAP_EVENT | 0x61)//0x04E1 !< Channel Terminated Event
#define L2CAP_OUT_OF_CREDIT_EVT          (HCI_EXT_L2CAP_EVENT | 0x62)//0x04E2 !< Out Of Credit Event
#define L2CAP_PEER_CREDIT_THRESHOLD_EVT  (HCI_EXT_L2CAP_EVENT | 0x63)//0x04E3 !< Peer Credit Threshold Event
#define L2CAP_SEND_SDU_DONE_EVT          (HCI_EXT_L2CAP_EVENT | 0x64)//0x04E4 !< Send SDU Done Event
#define L2CAP_DATA_EVT					 (HCI_EXT_L2CAP_EVENT | 0x70)//0x04F0 !< L2CAP DATA

/*------------------ATT Events-----------------*/
#define ATT_ERROR_EVENT                    (HCI_EXT_ATT_EVENT | 0x01) //!< ATT Error Response
#define ATT_EXCHANGE_MTU_EVENT             (HCI_EXT_ATT_EVENT | 0x03) //!< ATT Exchange MTU Response
#define ATT_FIND_INFO_EVENT                (HCI_EXT_ATT_EVENT | 0x05) //!< ATT Find Information Response
#define ATT_FIND_BY_TYPE_VALUE_EVENT       (HCI_EXT_ATT_EVENT | 0x07) //!< ATT Find By Type Value Response
#define ATT_READ_BY_TYPE_EVENT             (HCI_EXT_ATT_EVENT | 0x09) //!< ATT Read By Type Response
#define ATT_READ_EVENT                     (HCI_EXT_ATT_EVENT | 0x0B) //!< ATT Read Response
#define ATT_READ_BLOB_EVENT                (HCI_EXT_ATT_EVENT | 0x0D) //!< ATT Read Blob Response
#define ATT_READ_MULTI_EVENT               (HCI_EXT_ATT_EVENT | 0x0F) //!< ATT Read Multiple Response
#define ATT_READ_BY_GRP_TYPE_EVENT         (HCI_EXT_ATT_EVENT | 0x11) //!< ATT Read By Group Type Response
#define ATT_WRITE_EVENT                    (HCI_EXT_ATT_EVENT | 0x13) //!< ATT Write Response
#define ATT_PREPARE_WRITE_EVENT            (HCI_EXT_ATT_EVENT | 0x17) //!< ATT Prepare Write Response
#define ATT_EXECUTE_WRITE_EVENT            (HCI_EXT_ATT_EVENT | 0x19) //!< ATT Execute Write Response
#define ATT_HANDLE_VALUE_NOTI              (HCI_EXT_ATT_EVENT | 0x1B) //!< ATT Handle Value Notification
#define ATT_HANDLE_VALUE_IND               (HCI_EXT_ATT_EVENT | 0x1D) //!< ATT Handle Value Indication
#define ATT_HANDLE_VALUE_CFM               (HCI_EXT_ATT_EVENT | 0x1E) //!< ATT Handle Value Confirmation

#define ATT_FLOW_CTRL_VIOLATED_EVENT       (HCI_EXT_ATT_EVENT | 0x7E) //!< Sent when ATT flow control is violated on a connection. 
#define ATT_MTU_UPDATED_EVENT              (HCI_EXT_ATT_EVENT | 0x7F) //!< Sent when MTU is updated for a connection. 

/*---------------------------
/*	L2CAP Event	Struct		/
/*-------------------------*/
typedef enum {
	L2CAP_REJECT_CMD_NOT_UNDERSTOOD,
	L2CAP_REJECT_SIGNAL_MTU_EXCEED,
	L2CAP_REJECT_INVALID_CID
}eRejectReason;

typedef struct{
	UINT8 status;		//refer eRetStatus
	UINT8 handle_lo;
	UINT8 handle_hi;
	UINT8 reason_lo;	//refer eRejectReason
	UINT8 reason_hi;
}sL2capCmdRejEvt;

/*---------------------------
/*	ATT Event	Struct		/
/*-------------------------*/
typedef struct {
	UINT8 status;		//refer eRetStatus
	UINT8 handle_lo;
	UINT8 handle_hi;
	UINT8 pdu_len;
	UINT8 mtu_lo;
	UINT8 mtu_hi;
}sAttExMtuRsp;

/*---------------------------
/*	Gap Event	Struct		/
/*-------------------------*/
typedef UINT16 gapParamIDs_t;

typedef struct
{
	UINT8 opcode;                    //!< GAP type of command. Ref: @ref GAP_MSG_EVENT_DEFINES
} gapEventHdr_t;

typedef struct
{
	//!< GAP_RANDOM_ADDR_CHANGED_EVENT
	UINT8 addrType;                     //!< Address type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 newRandomAddr[BLE_ADDR_LEN];    //!< the new calculated private addr
} gapRandomAddrEvent_t;

typedef struct
{
	/** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
	UINT16 intervalMin;
	/** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
	UINT16 intervalMax;
	/** Number of LL latency connection events (0x0000 - 0x03e8) */
	UINT16 latency;
	/** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
	UINT16 timeout;
} gapPeriConnectParams_t;

typedef struct
{
	//!< GAP_DEVICE_INIT_DONE_EVENT
	UINT8 devAddr[BLE_ADDR_LEN];          //!< Device's BD_ADDR
	UINT16 dataPktLen;                  //!< HC_LE_Data_Packet_Length
	UINT8 numDataPkts;                  //!< HC_Total_Num_LE_Data_Packets
	UINT8 irk[IRK_LEN];
	UINT8 csrk[CSRK_LEN];
} gapDeviceInitDoneEvent_t;

typedef struct
{
	//!< GAP_SIGNATURE_UPDATED_EVENT
	UINT8 addrType;                     //!< Device's address type for devAddr
	UINT8 devAddr[BLE_ADDR_LEN];          //!< Device's BD_ADDR, could be own address
	UINT32 signCounter;                 //!< new Signed Counter
} gapSignUpdateEvent_t;

typedef struct
{
	//!< GAP_DEVICE_INFO_EVENT
	UINT8 eventType;          //!< Advertisement Type: @ref GAP_ADVERTISEMENT_REPORT_TYPE_DEFINES
	UINT8 addrType;           //!< address type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 addr[BLE_ADDR_LEN];   //!< Address of the advertisement or SCAN_RSP
	INT8 rssi;                //!< Advertisement or SCAN_RSP RSSI
	UINT8 dataLen;            //!< Length (in bytes) of the data field (evtData)
	UINT8 *pEvtData;          //!< Data field of advertisement or SCAN_RSP
} gapDeviceInfoEvent_t;

typedef struct
{
	UINT8 eventType;        //!< Indicates advertising event type used by the advertiser: @ref GAP_ADVERTISEMENT_REPORT_TYPE_DEFINES
	UINT8 addrType;         //!< Address Type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 addr[BLE_ADDR_LEN]; //!< Device's Address
} gapDevRec_t;

typedef struct
{
	//!< GAP_DEVICE_DISCOVERY_EVENT
	UINT8 numDevs;         //!< Number of devices found during scan
	gapDevRec_t *DevList; //!< array of device records
} gapDevDiscEvent_t;

typedef struct
{
	UINT8 eventType;          //!< Advertise Event Type: @ref GAP_ADVERTISEMENT_TYPE_DEFINES
	UINT8 initiatorAddrType;  //!< Initiator's address type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 initiatorAddr[BLE_ADDR_LEN];  //!< Initiator's addr - used only with connectable directed eventType (ADV_EVTTYPE_CONNECTABLE_DIRECTED).
	UINT8 channelMap;         //!< Channel Map: Bit mask @ref GAP_ADVCHAN_DEFINES
	UINT8 filterPolicy;       //!< Filer Policy: @ref GAP_FILTER_POLICY_DEFINES. Ignored when directed advertising is used.
} gapAdvertisingParams_t;

typedef struct
{
	 //!< GAP_ADV_DATA_UPDATE_DONE_EVENT
	UINT8 adType;          //!< TRUE if advertising data, FALSE if SCAN_RSP
} gapAdvDataUpdateEvent_t;

typedef struct
{
	//!< GAP_LINK_ESTABLISHED_EVENT
	UINT8 devAddrType;         //!< Device address type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 devAddr[BLE_ADDR_LEN]; //!< Device address of link
	UINT16 connectionHandle;   //!< Connection Handle from controller used to ref the device
	UINT8 connRole;            //!< Connection formed as Master or Slave
	UINT16 connInterval;       //!< Connection Interval
	UINT16 connLatency;        //!< Connection Latency
	UINT16 connTimeout;        //!< Connection Timeout
	UINT8 clockAccuracy;       //!< Clock Accuracy
} gapEstLinkReqEvent_t;

typedef struct
{
	UINT8 opcode;             //!< GAP_LINK_PARAM_UPDATE_EVENT
	UINT8 status;             //!< bStatus_t
	UINT16 connectionHandle;  //!< Connection handle of the update
	UINT16 connInterval;      //!< Requested connection interval
	UINT16 connLatency;       //!< Requested connection latency
	UINT16 connTimeout;       //!< Requested connection timeout
} gapLinkUpdateEvent_t;

typedef struct
{
	//!< GAP_LINK_TERMINATED_EVENT
	UINT16 connectionHandle; //!< connection Handle
	UINT8 reason;            //!< termination reason from LL
} gapTerminateLinkEvent_t;

typedef struct
{
	//!< GAP_PASSKEY_NEEDED_EVENT
	UINT8 deviceAddr[BLE_ADDR_LEN]; //!< address of device to pair with, and could be either public or random.
	UINT16 connectionHandle;      //!< Connection handle
	UINT8 uiInputs;               //!< Pairing User Interface Inputs - Ask user to input passcode
	UINT8 uiOutputs;              //!< Pairing User Interface Outputs - Display passcode
} gapPasskeyNeededEvent_t;

typedef struct
{
	UINT8 ltk[PASSKEY_LEN];              //!< Long Term Key (LTK)
	UINT16 div;                     //!< LTK Diversifier
	UINT8 rand[B_RANDOM_NUM_SIZE];  //!< LTK random number
	UINT8 keySize;                  //!< LTK Key Size (7-16 bytes)
} smSecurityInfo_t;

typedef struct
{
	UINT8 irk[PASSKEY_LEN];          //!< Identity Resolving Key (IRK)
	UINT8 bd_addr[BLE_ADDR_LEN];  //!< The advertiser may set this to zeroes to not disclose its BD_ADDR (public address).
} smIdentityInfo_t;

typedef struct
{
	UINT8  srk[PASSKEY_LEN]; //!< Signature Resolving Key (CSRK)
	UINT32 signCounter; //!< Sign Counter
} smSigningInfo_t;

typedef struct
{
	UINT8 opcode;                    //!< GAP_AUTHENTICATION_COMPLETE_EVENT
	UINT16 connectionHandle;         //!< Connection Handle from controller used to ref the device
	UINT8 authState;                 //!< TRUE if the pairing was authenticated (MITM)
	smSecurityInfo_t *pSecurityInfo; //!< BOUND - security information from this device
	smSigningInfo_t *pSigningInfo;   //!< Signing information
	smSecurityInfo_t *pDevSecInfo;   //!< BOUND - security information from connected device
	smIdentityInfo_t *pIdentityInfo; //!< BOUND - identity information
} gapAuthCompleteEvent_t;

typedef struct
{
	unsigned int sEncKey : 1;    //!< Set to distribute slave encryption key
	unsigned int sIdKey : 1;     //!< Set to distribute slave identity key
	unsigned int sSign : 1;      //!< Set to distribute slave signing key
	unsigned int sLinkKey : 1;   //!< Set to derive slave link key from slave LTK
	unsigned int sReserved : 4;  //!< Reserved for slave - don't use
	unsigned int mEncKey : 1;    //!< Set to distribute master encryption key
	unsigned int mIdKey : 1;     //!< Set to distribute master identity key
	unsigned int mSign : 1;      //!< Set to distribute master signing key
	unsigned int mLinkKey : 1;   //!< Set to derive master link key from master LTK
	unsigned int mReserved : 4;  //!< Reserved for master - don't use
} keyDist_t;

typedef struct
{
	UINT8 ioCaps;               //!< I/O Capabilities (ie.
	UINT8 oobAvailable;         //!< True if Out-of-band key available
	UINT8 oob[PASSKEY_LEN];          //!< Out-Of-Bounds key
	UINT8 authReq;              //!< Authentication Requirements
	keyDist_t keyDist;          //!< Key Distribution mask
	UINT8 maxEncKeySize;        //!< Maximum Encryption Key size (7-16 bytes)
} smLinkSecurityReq_t;

typedef struct
{
	UINT16 connectionHandle;      //!< Connection Handle from controller,
	smLinkSecurityReq_t  secReqs; //!< Pairing Control info
} gapAuthParams_t;

typedef struct
{
	 //!< GAP_SLAVE_REQUESTED_SECURITY_EVENT
	UINT16 connectionHandle;      //!< Connection Handle
	UINT8 deviceAddr[BLE_ADDR_LEN]; //!< address of device requesting security
	UINT8 authReq;                //!< Authentication Requirements: Bit 2: MITM, Bits 0-1: bonding (0 - no bonding, 1 - bonding)
} gapSlaveSecurityReqEvent_t;

typedef struct
{
	//!< GAP_BOND_COMPLETE_EVENT
	UINT16 connectionHandle; //!< connection Handle
} gapBondCompleteEvent_t;

typedef struct
{
	UINT8 ioCap;         //!< Pairing Request ioCap field
	UINT8 oobDataFlag;   //!< Pairing Request OOB Data Flag field
	UINT8 authReq;       //!< Pairing Request Auth Req field
	UINT8 maxEncKeySize; //!< Pairing Request Maximum Encryption Key Size field
	keyDist_t keyDist;   //!< Pairing Request Key Distribution field
} gapPairingReq_t;

typedef struct
{
	//!< GAP_PAIRING_REQ_EVENT
	UINT16 connectionHandle; //!< connection Handle
	gapPairingReq_t pairReq; //!< The Pairing Request fields received.
} gapPairingReqEvent_t;

typedef struct
{
	UINT8 adType;     //!< ADTYPE value: @ref GAP_ADTYPE_DEFINES
	UINT8 attrLen;    //!< Number of bytes in the attribute data
	UINT8 *pAttrData; //!< pointer to Attribute data
} gapAdvDataToken_t;


class NPI_EVT :public Runnable {
public:
	NPI_EVT();
	~NPI_EVT();
	void Run();

private:
	void HciExt_Decrypt_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapStatus_Event(PUINT8 pBuf, UINT16 len);
	void Hci_RdLocSuppFeat_Event(PUINT8 pBuf, UINT16 len);
	void Hci_RdBdaddr_Event(PUINT8 pBuf, UINT16 len);
	void Hci_RdRssi_Event(PUINT8 pBuf, UINT16 len);
	void Hci_LE_SetEvtMsk_Event(PUINT8 pBuf, UINT16 len);
	void Hci_LE_RdLocSuppFeat_Event(PUINT8 pBuf, UINT16 len);
	void Hci_RdWhiteListSize_Event(PUINT8 pBuf, UINT16 len);
	void Hci_LE_ConnUpdate_Event(PUINT8 pBuf, UINT16 len);
	void Hci_LE_TestEnd_Event(PUINT8 pBuf, UINT16 len);
	void Hci_LE_RemConnParaReqRep_Event(PUINT8 pBuf, UINT16 len);
	void Hci_LE_RemConnParaReqNegRep_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_CmdReject_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_DiscRsp_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_InfoRsp_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_ParamUpdateRsp_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_ConnRsp_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_ChannelEstablish_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_ChannelTerminate_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_OutOfCredit_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_PeerCredThreshold_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_SendSduDone_Event(PUINT8 pBuf, UINT16 len);
	void L2cap_Data_Event(PUINT8 pBuf, UINT16 len);
	void Att_Error_Event(PUINT8 pBuf, UINT16 len);
	void Att_ExchangeMtu_Event(PUINT8 pBuf, UINT16 len);
	void Att_FindInfo_Event(PUINT8 pBuf, UINT16 len);
	void Att_FindByTypeValue_Event(PUINT8 pBuf, UINT16 len);
	void Att_ReadByType_Event(PUINT8 pBuf, UINT16 len);
	void Att_Read_Event(PUINT8 pBuf, UINT16 len);
	void Att_ReadBlob_Event(PUINT8 pBuf, UINT16 len);
	void Att_ReadMulti_Event(PUINT8 pBuf, UINT16 len);
	void Att_ReadByGrpType_Event(PUINT8 pBuf, UINT16 len);
	void Att_Write_Event(PUINT8 pBuf, UINT16 len);
	void Att_PrepareWrite_Event(PUINT8 pBuf, UINT16 len);
	void Att_ExecuteWrite_Event(PUINT8 pBuf, UINT16 len);
	void Att_HanddlValueNoti_Event(PUINT8 pBuf, UINT16 len);
	void Att_HandleValueInd_Event(PUINT8 pBuf, UINT16 len);
	void Att_HandleValueCfm_Event(PUINT8 pBuf, UINT16 len);
	void Att_MtuUpdated_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapDevInitDone_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapDevDiscovery_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapAdvDataUpdateDone_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapMakeDiscoverableDone_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapEndDiscoverableDone_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapLinkEstablished_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapLinkTerminated_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapLinkParamUpdate_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapRandomAddrChanged_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapSignatureUpdated_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapAuthComplete_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapPasskeyNeeded_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapSlaveReqSec_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapDevInfo_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapBondComp_Event(PUINT8 pBuf, UINT16 len);
	void HciExt_GapPairReq_Event(PUINT8 pBuf, UINT16 len);
};

#pragma pack()

#endif