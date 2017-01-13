/**
@headerfile:       gatt.h
*/
#ifndef GAP_H
#define GAP_H
#pragma pack(1)
/*---------------------------
/*      Gap Event  Struct   /
/*-------------------------*/
typedef UINT16 gapParamIDs_t;

typedef union{
	UINT8 data;
	struct{
		UINT8 broadcast : 1;
		UINT8 observer : 1;
		UINT8 peripheral : 1;
		UINT8 central : 1;
		UINT8 reserve : 4;
	}role;
}gapRole_t;

typedef struct {
	UINT8 opcode;                    //!< GAP type of command. Ref: @ref GAP_MSG_EVENT_DEFINES
} gapEventHdr_t;

typedef struct {
	//!< GAP_RANDOM_ADDR_CHANGED_EVENT
	UINT8 status;
	UINT8 addrType;                     //!< Address type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 newRandomAddr[BLE_ADDR_LEN];    //!< the new calculated private addr
} gapRandomAddrEvent_t;

typedef struct {
	/** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
	UINT16 intervalMin;
	/** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
	UINT16 intervalMax;
	/** Number of LL latency connection events (0x0000 - 0x03e8) */
	UINT16 latency;
	/** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
	UINT16 timeout;
} gapPeriConnectParams_t;

typedef struct {
	//!< GAP_DEVICE_INIT_DONE_EVENT
	UINT8 status;
	UINT8 devAddr[BLE_ADDR_LEN];          //!< Device's BD_ADDR
	UINT16 dataPktLen;                  //!< HC_LE_Data_Packet_Length
	UINT8 numDataPkts;                  //!< HC_Total_Num_LE_Data_Packets
	UINT8 irk[IRK_LEN];
	UINT8 csrk[CSRK_LEN];
} gapDeviceInitDoneEvent_t;

typedef struct {
	//!< GAP_SIGNATURE_UPDATED_EVENT
	UINT8 status;
	UINT8 addrType;                     //!< Device's address type for devAddr
	UINT8 devAddr[BLE_ADDR_LEN];          //!< Device's BD_ADDR, could be own address
	UINT32 signCounter;                 //!< new Signed Counter
} gapSignUpdateEvent_t;

typedef struct {
	//!< GAP_DEVICE_INFO_EVENT
	UINT8 status;
	UINT8 eventType;          //!< Advertisement Type: @ref GAP_ADVERTISEMENT_REPORT_TYPE_DEFINES
	UINT8 addrType;           //!< address type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 addr[BLE_ADDR_LEN];   //!< Address of the advertisement or SCAN_RSP
	UINT8 rssi;                //!< Advertisement or SCAN_RSP RSSI
	UINT8 dataLen;            //!< Length (in bytes) of the data field (evtData)
	UINT8* pEvtData;          //!< Data field of advertisement or SCAN_RSP
} gapDeviceInfoEvent_t;

typedef struct {
	UINT8 eventType;        //!< Indicates advertising event type used by the advertiser: @ref GAP_ADVERTISEMENT_REPORT_TYPE_DEFINES
	UINT8 addrType;         //!< Address Type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 addr[BLE_ADDR_LEN]; //!< Device's Address
} gapDevRec_t;

typedef struct {
	//!< GAP_DEVICE_DISCOVERY_EVENT
	UINT8 status;
	UINT8 numDevs;         //!< Number of devices found during scan
	gapDevRec_t DevList[1]; //!< array of device records
} gapDevDiscEvent_t;

typedef struct {
	UINT8 eventType;          //!< Advertise Event Type: @ref GAP_ADVERTISEMENT_TYPE_DEFINES
	UINT8 initiatorAddrType;  //!< Initiator's address type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 initiatorAddr[BLE_ADDR_LEN];  //!< Initiator's addr - used only with connectable directed eventType (ADV_EVTTYPE_CONNECTABLE_DIRECTED).
	UINT8 channelMap;         //!< Channel Map: Bit mask @ref GAP_ADVCHAN_DEFINES
	UINT8 filterPolicy;       //!< Filer Policy: @ref GAP_FILTER_POLICY_DEFINES. Ignored when directed advertising is used.
} gapAdvertisingParams_t;

typedef struct {
	//!< GAP_ADV_DATA_UPDATE_DONE_EVENT
	UINT8 adType;          //!< TRUE if advertising data, FALSE if SCAN_RSP
} gapAdvDataUpdateEvent_t;

typedef struct {
	//!< GAP_LINK_ESTABLISHED_EVENT
	UINT8 status;
	UINT8 devAddrType;         //!< Device address type: @ref GAP_ADDR_TYPE_DEFINES
	UINT8 devAddr[BLE_ADDR_LEN]; //!< Device address of link
	UINT16 connectionHandle;   //!< Connection Handle from controller used to ref the device
	UINT8 connRole;            //!< Connection formed as Master or Slave
	UINT16 connInterval;       //!< Connection Interval
	UINT16 connLatency;        //!< Connection Latency
	UINT16 connTimeout;        //!< Connection Timeout
	UINT8 clockAccuracy;       //!< Clock Accuracy
} gapEstLinkReqEvent_t;

typedef struct {
	UINT8 opcode;             //!< GAP_LINK_PARAM_UPDATE_EVENT
	UINT8 status;             //!< bStatus_t
	UINT16 connectionHandle;  //!< Connection handle of the update
	UINT16 connInterval;      //!< Requested connection interval
	UINT16 connLatency;       //!< Requested connection latency
	UINT16 connTimeout;       //!< Requested connection timeout
} gapLinkUpdateEvent_t;

typedef struct {
	//!< GAP_LINK_TERMINATED_EVENT
	UINT8 status;
	UINT16 connectionHandle; //!< connection Handle
	UINT8 reason;            //!< termination reason from LL
} gapTerminateLinkEvent_t;

typedef struct {
	//!< GAP_PASSKEY_NEEDED_EVENT
	UINT8 status;
	UINT8 deviceAddr[BLE_ADDR_LEN]; //!< address of device to pair with, and could be either public or random.
	UINT16 connectionHandle;      //!< Connection handle
	UINT8 uiInputs;               //!< Pairing User Interface Inputs - Ask user to input passcode
	UINT8 uiOutputs;              //!< Pairing User Interface Outputs - Display passcode
} gapPasskeyNeededEvent_t;

typedef struct {
	UINT8 enable;
	UINT8 keySize;                  //!< LTK Key Size (7-16 bytes)
	UINT8 ltk[PASSKEY_LEN];              //!< Long Term Key (LTK)
	UINT16 div;                     //!< LTK Diversifier
	UINT8 rand[B_RANDOM_NUM_SIZE];  //!< LTK random number
} smSecurityInfo_t;

typedef struct {
	UINT8 enable;
	UINT8 irk[PASSKEY_LEN];          //!< Identity Resolving Key (IRK)
	UINT8 bd_addr[BLE_ADDR_LEN];  //!< The advertiser may set this to zeroes to not disclose its BD_ADDR (public address).
} smIdentityInfo_t;

typedef struct {
	UINT8 enable;
	UINT8  srk[PASSKEY_LEN]; //!< Signature Resolving Key (CSRK)
	UINT32 signCounter; //!< Sign Counter
} smSigningInfo_t;

typedef struct {
	//!< GAP_AUTHENTICATION_COMPLETE_EVENT
	UINT8 status;
	UINT16 connectionHandle;         //!< Connection Handle from controller used to ref the device
	UINT8 authState;                 //!< TRUE if the pairing was authenticated (MITM)
	smSecurityInfo_t
	pSecurityInfo; //!< BOUND - security information from this device
	smSecurityInfo_t
	pDevSecInfo;   //!< BOUND - security information from connected device
	smIdentityInfo_t pIdentityInfo; //!< BOUND - identity information
	smSigningInfo_t pSigningInfo;   //!< Signing information
} gapAuthCompleteEvent_t;

typedef struct {
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

typedef struct {
	UINT8 ioCaps;               //!< I/O Capabilities (ie.
	UINT8 oobAvailable;         //!< True if Out-of-band key available
	UINT8 oob[PASSKEY_LEN];          //!< Out-Of-Bounds key
	UINT8 authReq;              //!< Authentication Requirements
	keyDist_t keyDist;          //!< Key Distribution mask
	UINT8 maxEncKeySize;        //!< Maximum Encryption Key size (7-16 bytes)
} smLinkSecurityReq_t;

typedef struct {
	UINT16 connectionHandle;      //!< Connection Handle from controller,
	smLinkSecurityReq_t  secReqs; //!< Pairing Control info
} gapAuthParams_t;

typedef struct {
	//!< GAP_SLAVE_REQUESTED_SECURITY_EVENT
	UINT8 status;
	UINT16 connectionHandle;      //!< Connection Handle
	UINT8 deviceAddr[BLE_ADDR_LEN]; //!< address of device requesting security
	UINT8 authReq;                //!< Authentication Requirements: Bit 2: MITM, Bits 0-1: bonding (0 - no bonding, 1 - bonding)
} gapSlaveSecurityReqEvent_t;

typedef struct {
	//!< GAP_BOND_COMPLETE_EVENT
	UINT16 connectionHandle; //!< connection Handle
} gapBondCompleteEvent_t;

typedef struct {
	UINT8 ioCap;         //!< Pairing Request ioCap field
	UINT8 oobDataFlag;   //!< Pairing Request OOB Data Flag field
	UINT8 authReq;       //!< Pairing Request Auth Req field
	UINT8 maxEncKeySize; //!< Pairing Request Maximum Encryption Key Size field
	keyDist_t keyDist;   //!< Pairing Request Key Distribution field
} gapPairingReq_t;

typedef struct {
	//!< GAP_PAIRING_REQ_EVENT
	UINT8 status;
	UINT16 connectionHandle; //!< connection Handle
	gapPairingReq_t pairReq; //!< The Pairing Request fields received.
} gapPairingReqEvent_t;

typedef struct {
	UINT8 adType;     //!< ADTYPE value: @ref GAP_ADTYPE_DEFINES
	UINT8 attrLen;    //!< Number of bytes in the attribute data
	UINT8* pAttrData; //!< pointer to Attribute data
} gapAdvDataToken_t;

/*---------------------------
/*      Gap Event Union    /
/*-------------------------*/
typedef union {
	gapDeviceInitDoneEvent_t  InitDone_Evt;
	gapDevDiscEvent_t  DiscDone_Evt;
	gapEstLinkReqEvent_t  EstLink_Evt;
	gapTerminateLinkEvent_t  TerLink_Evt;
	gapRandomAddrEvent_t  RanAddr_Evt;
	gapSignUpdateEvent_t  SigUpdate_Evt;
	gapAuthCompleteEvent_t  AuthComp_Evt;
	gapPasskeyNeededEvent_t  PassKeyNeed_Evt;
	gapSlaveSecurityReqEvent_t  SlaveSec_Evt;
	gapDeviceInfoEvent_t  DevInfo_Evt;
	gapPairingReqEvent_t  PairReq_Evt;
} u_GapMsg;

#pragma pack()
#endif