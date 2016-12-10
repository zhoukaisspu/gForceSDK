
#ifndef _NPI_EVT_H_
#define _NPI_EVT_H_

#include"Thread.h"
#include"com.h"

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

typedef enum {
	HCI_SUCCESS = 0x00,
	HCI_ERROR_CODE_UNKNOWN_HCI_CMD = 0x01,
	HCI_ERROR_CODE_UNKNOWN_CONN_ID = 0x02,
	HCI_ERROR_CODE_HW_FAILURE = 0x03,
	HCI_ERROR_CODE_PAGE_TIMEOUT = 0x04,
	HCI_ERROR_CODE_AUTH_FAILURE = 0x05,
	HCI_ERROR_CODE_PIN_KEY_MISSING = 0x06,
	HCI_ERROR_CODE_MEM_CAP_EXCEEDED = 0x07,
	HCI_ERROR_CODE_CONN_TIMEOUT = 0x08,
	HCI_ERROR_CODE_CONN_LIMIT_EXCEEDED = 0x09,
	HCI_ERROR_CODE_SYNCH_CONN_LIMIT_EXCEEDED = 0x0A,
	HCI_ERROR_CODE_ACL_CONN_ALREADY_EXISTS = 0x0B,
	HCI_ERROR_CODE_CMD_DISALLOWED = 0x0C,
	HCI_ERROR_CODE_CONN_REJ_LIMITED_RESOURCES = 0x0D,
	HCI_ERROR_CODE_CONN_REJECTED_SECURITY_REASONS = 0x0E,
	HCI_ERROR_CODE_CONN_REJECTED_UNACCEPTABLE_BDADDR = 0x0F,
	HCI_ERROR_CODE_CONN_ACCEPT_TIMEOUT_EXCEEDED = 0x10,
	HCI_ERROR_CODE_UNSUPPORTED_FEATURE_PARAM_VALUE = 0x11,
	HCI_ERROR_CODE_INVALID_HCI_CMD_PARAMS = 0x12,
	HCI_ERROR_CODE_REMOTE_USER_TERM_CONN = 0x13,
	HCI_ERROR_CODE_REMOTE_DEVICE_TERM_CONN_LOW_RESOURCES = 0x14,
	HCI_ERROR_CODE_REMOTE_DEVICE_TERM_CONN_POWER_OFF = 0x15,
	HCI_ERROR_CODE_CONN_TERM_BY_LOCAL_HOST = 0x16,
	HCI_ERROR_CODE_REPEATED_ATTEMPTS = 0x17,
	HCI_ERROR_CODE_PAIRING_NOT_ALLOWED = 0x18,
	HCI_ERROR_CODE_UNKNOWN_LMP_PDU = 0x19,
	HCI_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE = 0x1A,
	HCI_ERROR_CODE_SCO_OFFSET_REJ = 0x1B,
	HCI_ERROR_CODE_SCO_INTERVAL_REJ = 0x1C,
	HCI_ERROR_CODE_SCO_AIR_MODE_REJ = 0x1D,
	HCI_ERROR_CODE_INVALID_LMP_PARAMS = 0x1E,
	HCI_ERROR_CODE_UNSPECIFIED_ERROR = 0x1F,
	HCI_ERROR_CODE_UNSUPPORTED_LMP_PARAM_VAL = 0x20,
	HCI_ERROR_CODE_ROLE_CHANGE_NOT_ALLOWED = 0x21,
	HCI_ERROR_CODE_LMP_LL_RESP_TIMEOUT = 0x22,
	HCI_ERROR_CODE_LMP_ERR_TRANSACTION_COLLISION = 0x23,
	HCI_ERROR_CODE_LMP_PDU_NOT_ALLOWED = 0x24,
	HCI_ERROR_CODE_ENCRYPT_MODE_NOT_ACCEPTABLE = 0x25,
	HCI_ERROR_CODE_LINK_KEY_CAN_NOT_BE_CHANGED = 0x26,
	HCI_ERROR_CODE_REQ_QOS_NOT_SUPPORTED = 0x27,
	HCI_ERROR_CODE_INSTANT_PASSED = 0x28,
	HCI_ERROR_CODE_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED = 0x29,
	HCI_ERROR_CODE_DIFFERENT_TRANSACTION_COLLISION = 0x2A,
	HCI_ERROR_CODE_RESERVED1 = 0x2B,
	HCI_ERROR_CODE_QOS_UNACCEPTABLE_PARAM = 0x2C,
	HCI_ERROR_CODE_QOS_REJ = 0x2D,
	HCI_ERROR_CODE_CHAN_ASSESSMENT_NOT_SUPPORTED = 0x2E,
	HCI_ERROR_CODE_INSUFFICIENT_SECURITY = 0x2F,
	HCI_ERROR_CODE_PARAM_OUT_OF_MANDATORY_RANGE = 0x30,
	HCI_ERROR_CODE_RESERVED2 = 0x31,
	HCI_ERROR_CODE_ROLE_SWITCH_PENDING = 0x32,
	HCI_ERROR_CODE_RESERVED3 = 0x33,
	HCI_ERROR_CODE_RESERVED_SLOT_VIOLATION = 0x34,
	HCI_ERROR_CODE_ROLE_SWITCH_FAILED = 0x35,
	HCI_ERROR_CODE_EXTENDED_INQUIRY_RESP_TOO_LARGE = 0x36,
	HCI_ERROR_CODE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST = 0x37,
	HCI_ERROR_CODE_HOST_BUSY_PAIRING = 0x38,
	HCI_ERROR_CODE_CONN_REJ_NO_SUITABLE_CHAN_FOUND = 0x39,
	HCI_ERROR_CODE_CONTROLLER_BUSY = 0x3A,
	HCI_ERROR_CODE_UNACCEPTABLE_CONN_PARAMETERS = 0x3B,
	HCI_ERROR_CODE_DIRECTED_ADV_TIMEOUT = 0x3C,
	HCI_ERROR_CODE_CONN_TERM_MIC_FAILURE = 0x3D,
	HCI_ERROR_CODE_CONN_FAILED_TO_ESTABLISH = 0x3E,
	HCI_ERROR_CODE_MAC_CONN_FAILED = 0x3F,
	HCI_ERROR_CODE_COARSE_CLOCK_ADJUST_REJECTED = 0x40
}eHciRetSta;

typedef enum {
	/*** Generic Status Return Values ***/
	SUCCESS =                  0x00,
	FAILURE =                  0x01,
	INVALIDPARAMETER =         0x02,
	INVALID_TASK =             0x03,
	MSG_BUFFER_NOT_AVAIL =     0x04,
	INVALID_MSG_POINTER =      0x05,
	INVALID_EVENT_ID =         0x06,
	INVALID_INTERRUPT_ID =     0x07,
	NO_TIMER_AVAIL =           0x08,
	NV_ITEM_UNINIT =           0x09,
	NV_OPER_FAILED =           0x0A,
	INVALID_MEM_SIZE =         0x0B,
	NV_BAD_ITEM_LEN =          0x0C,

	bleNotReady =					0x10,  //!< Not ready to perform task
	bleAlreadyInRequestedMode =     0x11,  //!< Already performing that task
	bleIncorrectMode =              0x12,  //!< Not setup properly to perform that task
	bleMemAllocError =              0x13,  //!< Memory allocation error occurred
	bleNotConnected =               0x14,  //!< Can't perform function when not in a connection
	bleNoResources =                0x15,  //!< There are no resource available
	blePending =                    0x16,  //!< Waiting
	bleTimeout =                    0x17,  //!< Timed out performing function
	bleInvalidRange =               0x18,  //!< A parameter is out of range
	bleLinkEncrypted =              0x19,  //!< The link is already encrypted
	bleProcedureComplete =          0x1A,  //!< The Procedure is completed
	bleInvalidMtuSize =             0x1B,  //!< MTU size is out of range

	// GAP Status Return Values 
	bleGAPUserCanceled =			0x30,  //!< The user canceled the task
	bleGAPConnNotAcceptable =       0x31,  //!< The connection was not accepted
	bleGAPBondRejected =            0x32,  //!< The bound information was rejected.

	// ATT Status Return Values 
	bleInvalidPDU =                 0x40,  //!< The attribute PDU is invalid
	bleInsufficientAuthen =         0x41,  //!< The attribute has insufficient authentication
	bleInsufficientEncrypt =        0x42,  //!< The attribute has insufficient encryption
	bleInsufficientKeySize =        0x43, //!< The attribute has insufficient encryption key size

	INVALID_RET_STATUS =			0xFF
}eRetStatus;

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
#endif