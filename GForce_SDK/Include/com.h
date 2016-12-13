#include "windows.h"
#include"log.h"
#include"Thread.h"

#include <atlstr.h>
#ifndef _COM_H_
#define _COM_H_

#define MAX_RX_SIZE 255
#define MAX_TX_SIZE 255

#define EVT_HEADER_LEN 3
#define BLE_ADDR_LEN            6
#define IRK_LEN                         16
#define CSRK_LEN                        16
#define SIGN_COUNT_LEN          4
#define OOB_DATA_LEN            16
#define PASSKEY_LEN                     16
#define LTK_LEN                         16
#define LTK_RAND_PAIR_LEN       16

#define B_RANDOM_NUM_SIZE   8
#define FEATURES_LEN            8
#define ADV_CHANNEL_37          0x01
#define ADV_CHANNEL_38          0x02
#define ADV_CHANNEL_39          0x04
#define ADV_CHANNEL_ALL         (ADV_CHANNEL_37 | ADV_CHANNEL_38 | ADV_CHANNEL_39)

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
} eHciRetSta;

typedef enum {
	/*** Generic Status Return Values ***/
	SUCCESS = 0x00,
	FAILURE = 0x01,
	INVALIDPARAMETER = 0x02,
	INVALID_TASK = 0x03,
	MSG_BUFFER_NOT_AVAIL = 0x04,
	INVALID_MSG_POINTER = 0x05,
	INVALID_EVENT_ID = 0x06,
	INVALID_INTERRUPT_ID = 0x07,
	NO_TIMER_AVAIL = 0x08,
	NV_ITEM_UNINIT = 0x09,
	NV_OPER_FAILED = 0x0A,
	INVALID_MEM_SIZE = 0x0B,
	NV_BAD_ITEM_LEN = 0x0C,

	bleNotReady = 0x10,  //!< Not ready to perform task
	bleAlreadyInRequestedMode = 0x11,  //!< Already performing that task
	bleIncorrectMode = 0x12,  //!< Not setup properly to perform that task
	bleMemAllocError = 0x13,  //!< Memory allocation error occurred
	bleNotConnected = 0x14,  //!< Can't perform function when not in a connection
	bleNoResources = 0x15,  //!< There are no resource available
	blePending = 0x16,  //!< Waiting
	bleTimeout = 0x17,  //!< Timed out performing function
	bleInvalidRange = 0x18,  //!< A parameter is out of range
	bleLinkEncrypted = 0x19,  //!< The link is already encrypted
	bleProcedureComplete = 0x1A,  //!< The Procedure is completed
	bleInvalidMtuSize = 0x1B,  //!< MTU size is out of range

	// GAP Status Return Values
	bleGAPUserCanceled = 0x30,  //!< The user canceled the task
	bleGAPConnNotAcceptable = 0x31,  //!< The connection was not accepted
	bleGAPBondRejected = 0x32,  //!< The bound information was rejected.

	// ATT Status Return Values
	bleInvalidPDU = 0x40,  //!< The attribute PDU is invalid
	bleInsufficientAuthen = 0x41,  //!< The attribute has insufficient authentication
	bleInsufficientEncrypt = 0x42,  //!< The attribute has insufficient encryption
	bleInsufficientKeySize = 0x43, //!< The attribute has insufficient encryption key size

	INVALID_RET_STATUS = 0xFF
} eRetStatus;

#define RX_STATUS_MSG                                           WM_USER+1
//#define LOG_MSG                                                               WM_USER+2
/*------HCI Message-------*/
#define HCI_DECRYPT_MSG                                         WM_USER+3
#define HCI_GAP_STATUS_MSG                                      WM_USER+4
#define HCI_READ_LOCAL_SUPPORT_FEAT_MSG         WM_USER+5
#define HCI_READ_BDADDR_MSG                                     WM_USER+6
#define HCI_READ_RSSI_MSG                                       WM_USER+7
#define HCI_LE_SET_EVENT_MASK_MSG                       WM_USER+8
#define HCI_LE_READ_LOCAL_SUPPORT_FEAT_MSG      WM_USER+9
#define HCI_LE_READ_WHITE_LIST_SIZE_MSG         WM_USER+10
#define HCI_LE_CONNECTION_UPDATE_MSG            WM_USER+11
#define HCI_LE_TEST_END_MSG                                     WM_USER+12
#define HCI_LE_REM_CONN_PARAM_REQ_REP_MSG       WM_USER+13
#define HCI_LE_REM_CONN_PARAM_REQ_NEG_REP_MSG   WM_USER+14

/*------L2CAP Message-----*/
#define L2CAP_COMMAND_REJECT_MSG                        WM_USER+0x100
#define L2CAP_DISCONNECT_RSP_MSG                        WM_USER+0x101
#define L2CAP_INFO_RSP_MSG                                      WM_USER+0x102
#define L2CAP_PARAM_UPDATE_RSP_MSG                      WM_USER+0x103
#define L2CAP_CONNECT_RSP_MSG                           WM_USER+0x104
#define L2CAP_CHANNEL_ESTABLISHED_MSG           WM_USER+0x105
#define L2CAP_CHANNEL_TERMINATED_MSG            WM_USER+0x106
#define L2CAP_OUT_OF_CREDIT_MSG                         WM_USER+0x107
#define L2CAP_PEER_CREDIT_THRESHOLD_MSG         WM_USER+0x108
#define L2CAP_SEND_SDU_DONE_MSG                         WM_USER+0x109
#define L2CAP_DATA_MSG                                          WM_USER+0x10a
/*------ATT Message-----*/
#define ATT_ERROR_MSG                                           WM_USER+0x200
#define ATT_EXCHANGE_MTU_MSG                            WM_USER+0x201
#define ATT_FIND_INFO_MSG                                       WM_USER+0x202
#define ATT_FIND_BY_TYPE_VALUE_MSG                      WM_USER+0x203
#define ATT_READ_BY_TYPE_MSG                            WM_USER+0x204
#define ATT_READ_MSG                                            WM_USER+0x205
#define ATT_READ_BLOB_MSG                                       WM_USER+0x206
#define ATT_READ_MULTI_MSG                                      WM_USER+0x207
#define ATT_READ_BY_GRP_TYPE_MSG                        WM_USER+0x208
#define ATT_WRITE_MSG                                           WM_USER+0x209
#define ATT_PREPARE_WRITE_MSG                           WM_USER+0x20a
#define ATT_EXECUTE_WRITE_MSG                           WM_USER+0x20b
#define ATT_HANDLE_VALUE_NOTI_MSG                       WM_USER+0x20c
#define ATT_HANDLE_VALUE_IND_MSG                        WM_USER+0x20d
#define ATT_HANDLE_VALUE_CFM_MSG                        WM_USER+0x20e
#define ATT_FLOW_CTRL_VIOLATED_MSG                      WM_USER+0x20f
#define ATT_MTU_UPDATED_MSG                                     WM_USER+0x210
/*------GAP Message-----*/
#define HCI_EXT_GAP_DEVICE_INIT_DONE_MSG          WM_USER+0x300
#define HCI_EXT_GAP_DEVICE_DISCOVERY_MSG          WM_USER+0x301
#define HCI_EXT_GAP_ADV_DATA_UPDATE_DONE_MSG      WM_USER+0x302
#define HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_MSG    WM_USER+0x303
#define HCI_EXT_GAP_END_DISCOVERABLE_DONE_MSG     WM_USER+0x304
#define HCI_EXT_GAP_LINK_ESTABLISHED_MSG          WM_USER+0x305
#define HCI_EXT_GAP_LINK_TERMINATED_MSG           WM_USER+0x306
#define HCI_EXT_GAP_LINK_PARAM_UPDATE_MSG         WM_USER+0x307
#define HCI_EXT_GAP_RANDOM_ADDR_CHANGED_MSG       WM_USER+0x308
#define HCI_EXT_GAP_SIGNATURE_UPDATED_MSG         WM_USER+0x309
#define HCI_EXT_GAP_AUTH_COMPLETE_MSG             WM_USER+0x30a
#define HCI_EXT_GAP_PASSKEY_NEEDED_MSG            WM_USER+0x30b
#define HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_MSG  WM_USER+0x30c
#define HCI_EXT_GAP_DEVICE_INFO_MSG               WM_USER+0x30d
#define HCI_EXT_GAP_BOND_COMPLETE_MSG             WM_USER+0x30e
#define HCI_EXT_GAP_PAIRING_REQ_MSG               WM_USER+0x30f

#define LOG_MSG                                                                   WM_USER+0x400
class Com: public Runnable
{
public:
	DWORD com_threadID;
	DWORD logThreadID;
	DWORD rxThreadID;
	DWORD evtThreadID;
	HANDLE com_file;
	UINT8 ret_status;
	Com();
	Com(int nPort, int nBaud = 115200, int nParity = 0, int nByteSize = 8,
	    int nStopBit = ONESTOPBIT);
	~Com();
	int Connect();
	DWORD write(PUINT8 buf, UINT8 size);
	void Run();

private:
	OVERLAPPED osWait;
	DCB dcb;
	COMMTIMEOUTS timeouts;
	int port;
	HANDLE hThread;
	CThread* logThread;
	CThread* rxThread;
	CThread* evtThread;
};


#endif