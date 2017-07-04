/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
#ifndef _NPI_CMD_H_
#define _NPI_CMD_H_

#include "CommonDef.h"
#include "com.h"
#include "gap.h"
/*Dongle Type Define*/
#define CC26XX
//#define CC13XX
//#define CC254X

#define LL_STATUS_ERROR_PEER_TERM       0x13 // Remote User Terminated Connection
#pragma pack (1)
/*---------------------------
/*           CMD            /
/*-------------------------*/
#define CMD_HEAD_LEN 4

typedef enum {
	NPI_DISABLE = 0,
	NPI_ENABLE,
} eEnDisMode;

typedef enum {
	NPI_FALSE = 0,
	NPI_TRUE,
} eBoolMode;

typedef enum {
	NPI_NO = 0,
	NPI_YES,
} eYesNoMode;

/*---------HCI Extended Command------------*/
typedef enum {
	HCI_EXT_RX_GAIN_STD,
	HCI_EXT_RX_GAIN_HIGH
} eRxGain;

typedef enum {
#if defined( CC26XX ) || defined( CC13XX )
	HCI_EXT_TX_POWER_MINUS_21_DBM = 0,
	HCI_EXT_TX_POWER_MINUS_18_DBM,
	HCI_EXT_TX_POWER_MINUS_15_DBM,
	HCI_EXT_TX_POWER_MINUS_12_DBM,
	HCI_EXT_TX_POWER_MINUS_9_DBM,
	HCI_EXT_TX_POWER_MINUS_6_DBM,
	HCI_EXT_TX_POWER_MINUS_3_DBM,
	HCI_EXT_TX_POWER_0_DBM,
	HCI_EXT_TX_POWER_1_DBM,
	HCI_EXT_TX_POWER_2_DBM,
	HCI_EXT_TX_POWER_3_DBM,
	HCI_EXT_TX_POWER_4_DBM,
	HCI_EXT_TX_POWER_5_DBM = 12
#else // CC254x
	HCI_EXT_TX_POWER_MINUS_23_DBM = 0,
	HCI_EXT_TX_POWER_MINUS_6_DBM,
	HCI_EXT_TX_POWER_0_DBM,
	HCI_EXT_TX_POWER_4_DBM = 3
#endif // CC26XX/CC13XX
} eTxPwr;

typedef enum {
	HCI_EXT_DISABLE_ONE_PKT_PER_EVT = 0,
	HCI_EXT_ENABLE_ONE_PKT_PER_EVT
} eOnePktPerEvtCtrl;

typedef enum {
	HCI_EXT_DISABLE_CLK_DIVIDE_ON_HALT = 0,
	HCI_EXT_ENABLE_CLK_DIVIDE_ON_HALT
} eClkDivOnHalt;

typedef enum {
	HCI_EXT_NV_NOT_IN_USE = 0,
	HCI_EXT_NV_IN_USE
} eNvUsage;

typedef struct {
	UINT8 key[16];
	UINT8 data[16];
} sDecrypt;

typedef enum {
	HCI_EXT_DISABLE_FAST_TX_RESP_TIME = 0,
	HCI_EXT_ENABLE_FAST_TX_RESP_TIME
} eFastTxRespTime;

typedef enum {
	HCI_EXT_TX_MODULATED_CARRIER = 0,
	HCI_EXT_TX_UNMODULATED_CARRIER
} eCwModem;

typedef struct {
	eCwModem cw;
	UINT8 tx_channel;
} sModemTestTx;

typedef enum {
	HCI_PTM_SET_FREQ_TUNE_DOWN = 0,
	HCI_PTM_SET_FREQ_TUNE_UP
} eFreqTune;

typedef enum {
	HCI_EXT_PM_IO_PORT_P0 = 0,
	HCI_EXT_PM_IO_PORT_P1,
	HCI_EXT_PM_IO_PORT_P2,
	HCI_EXT_PM_IO_PORT_NONE = 0xFF
} ePmloPort;

typedef enum {
	HCI_EXT_PM_IO_PORT_PIN0 = 0,
	HCI_EXT_PM_IO_PORT_PIN1,
	HCI_EXT_PM_IO_PORT_PIN2,
	HCI_EXT_PM_IO_PORT_PIN3,
	HCI_EXT_PM_IO_PORT_PIN4,
	HCI_EXT_PM_IO_PORT_PIN5,
	HCI_EXT_PM_IO_PORT_PIN6,
	HCI_EXT_PM_IO_PORT_PIN7
} ePmloPortPin;

typedef struct {
	ePmloPort port;
	ePmloPortPin portPin;
} sPmloPort;

typedef enum {
	HCI_EXT_PER_RESET = 0,
	HCI_EXT_PER_READ,
} ePerTestCmd;

typedef struct {
	UINT16 handle;
	ePerTestCmd cmd;
} sPerTestCmd;

typedef enum {
	HCI_EXT_HALT_DURING_RF_DISABLE = 0,
	HCI_EXT_HALT_DURING_RF_ENSABLE,
} eHaltDurRF;

typedef enum {
	HCI_EXT_OVER_RIDE_SL_DIS = 0,
	HCI_EXT_OVER_RIDE_SL_EN,
} eOverrideSL;

typedef enum {
	HCI_EXT_SET_USER_REVISION = 0,
	HCI_EXT_READ_BUILD_REVISION,
} eBuildRevMode;

typedef enum {
	HCI_EXT_RESET_SYSTEM_HARD = 0,
	HCI_EXT_RESET_SYSTEM_SOFT,
} eResetType;

/*-----------L2CAP Command--------------*/
typedef enum {
	CONNECTIONLESS_MTU = 1,
	EXTENDED_FEATURES,
	FIXED_CHANNELS
} eInfoType;

typedef enum {
	L2CAP_CONN_SUCCESS = 0,                         //!< Connection successful
	L2CAP_CONN_PSM_NOT_SUPPORTED = 2,       //!< Connection refused ?LE_PSM not supported
	L2CAP_CONN_NO_RESOURCES = 4,            //!< Connection refused ?no resources available
	L2CAP_CONN_INSUFFICIENT_AUTHEN,         //!< Connection refused ?insufficient authentication
	L2CAP_CONN_INSUFFICIENT_AUTHOR,         //!< Connection refused ?insufficient authorization
	L2CAP_CONN_INSUFFICIENT_KEY_SIZE,       //!< Connection refused ?insufficient encryption key size
	L2CAP_CONN_INSUFFICIENT_ENCRYPT         //!< Connection refused ?insufficient encryption
} eL2capConRes;
/*-----------ATT Command--------------*/


typedef enum {
	Cancel_All_Prepared_Writes,
	Immediately_Write_All_Pending_Prepared_Values
} eAttExeWrFlag;
/*-----------GATT Command--------------*/
typedef struct {
	UINT8 value_len;
	UINT8 hdl_lo;
	UINT8 hdl_hi;
	UINT8 offset_lo;
	UINT8 offset_hi;
	PUINT8 pvalue;
} sGattWriteVal;

typedef enum {
	PRIMARY_SERVICE = 0x2800,
	SECONDARY_SERVICE = 0x2801
} sGattServiceUUID;
/*-----------GAP Command--------------*/
#define GAP_CENTRAL_ROLE                0x08
#define GAP_PREFERAL_ROLE               0x04
#define GAP_OBSERVE_ROLE                0x02
#define GAP_BROADCAST_ROLE              0x01

typedef enum {
	ADDRTYPE_PUBLIC,
	ADDRTYPE_STATIC,
	ADDRTYPE_PRIVATE_NONRESOLVE,
	ADDRTYPE_PRIVATE_RESOLVE
} eGapAddrType;

typedef enum {
	DEVDISC_MODE_NONDISCOVERABLE,   //!< No discoverable setting
	DEVDISC_MODE_GENERAL,                   //!< General Discoverable devices
	DEVDISC_MODE_LIMITED,                   //!< Limited Discoverable devices
	DEVDISC_MODE_ALL                                //!< Not filtered
} eGapDiscMode;

typedef enum {
	GAP_ADTYPE_ADV_IND =               0x00,  //!< Connectable undirected advertisement
	GAP_ADTYPE_ADV_HDC_DIRECT_IND =    0x01,  //!< Connectable high duty cycle directed advertisement
	GAP_ADTYPE_ADV_SCAN_IND =          0x02,  //!< Scannable undirected advertisement
	GAP_ADTYPE_ADV_NONCONN_IND =       0x03,  //!< Non-Connectable undirected advertisement
	GAP_ADTYPE_ADV_LDC_DIRECT_IND =    0x04  //!< Connectable low duty cycle directed advertisement
} eGapEventType;

typedef enum {
	GAP_FILTER_POLICY_ALL =        0x00, //!< Allow Scan Request from Any, Allow Connect Request from Any (default).
	GAP_FILTER_POLICY_WHITE_SCAN = 0x01, //!< Allow Scan Request from White List Only, Allow Connect from Any
	GAP_FILTER_POLICY_WHITE_CON =  0x02, //!< Allow Scan Request from Any, Connect from White List Only
	GAP_FILTER_POLICY_WHITE =      0x03 //!< Allow Scan Request and Connect from White List Only
} eGapFilterPol;

typedef enum {
	GAP_SCAN_RSP_DATA_TYPE = 0,
	GAP_ADVERTISEMENT_DATA
} eGapAdvDataType;

typedef enum {
	DISPLAY_ONLY =             0x00,  //!< Display Only Device
	DISPLAY_YES_NO =           0x01,  //!< Display and Yes and No Capable
	KEYBOARD_ONLY =            0x02,  //!< Keyboard Only
	NO_INPUT_NO_OUTPUT =       0x03,  //!< No Display or Input Device
	KEYBOARD_DISPLAY =         0x04  //!< Both Keyboard and Display Capable
} eGapIOCaps;

typedef struct {
	UINT8 bonding : 2;    //!< Bonding flags
	UINT8 mitm : 1;       //!< Man-In-The-Middle (MITM)
	UINT8 sc : 1;         //!< LE Secure Connection
	UINT8 reserved : 4;   //!< Reserved - don't use
} sAuthReq;

typedef union {
	UINT8 oper;
	sAuthReq req;
} uAuthReq;

typedef struct {
	UINT8 sEncKey : 1;    //!< Set to distribute slave encryption key
	UINT8 sIdKey : 1;     //!< Set to distribute slave identity key
	UINT8 sSign : 1;      //!< Set to distribute slave signing key
	UINT8 sLinkKey : 1;   //!< Set to derive slave link key from slave LTK
	UINT8 mEncKey : 1;    //!< Set to distribute master encryption key
	UINT8 mIdKey : 1;     //!< Set to distribute master identity key
	UINT8 mSign : 1;      //!< Set to distribute master signing key
	UINT8 mLinkKey : 1;   //!< Set to derive master link key from master LTK
} sKeyDist;

typedef union {
	UINT8 oper;
	sKeyDist dist;
} uKeyDist;

typedef struct {
	eGapIOCaps sec_ioCaps;
	eBoolMode sec_oobFlag;
	UINT8 sec_oobData[OOB_DATA_LEN];
	uAuthReq sec_authReq;
	UINT8 sec_maxEncKeySize;
	uKeyDist sec_keyDist;
	eEnDisMode pair_en;
	eGapIOCaps pair_ioCaps;
	eBoolMode pair_oobFlag;
	uAuthReq pair_authReq;
	UINT8 pair_maxEncKeySize;
	uKeyDist pair_keyDist;
} sGapAuth;

typedef enum {
	// Timers
	TGAP_GEN_DISC_ADV_MIN =          0,//!< Minimum time to remain advertising, when in Discoverable mode (mSec).  Setting this parameter to 0 turns off the timeout (default).
	TGAP_LIM_ADV_TIMEOUT =           1, //!< Maximum time to remain advertising, when in Limited Discoverable mode. In seconds (default 180 seconds)
	TGAP_GEN_DISC_SCAN =             2, //!< Minimum time to perform scanning, when performing General Discovery proc (mSec)
	TGAP_LIM_DISC_SCAN =             3,//!< Minimum time to perform scanning, when performing Limited Discovery proc (mSec)
	TGAP_CONN_EST_ADV_TIMEOUT =      4,//!< Advertising timeout, when performing Connection Establishment proc (mSec)
	TGAP_CONN_PARAM_TIMEOUT =        5, //!< Link Layer connection parameter update notification timer, connection parameter update proc (mSec)
	// Constants
	TGAP_LIM_DISC_ADV_INT_MIN =     6,//!< Minimum advertising interval, when in limited discoverable mode (n * 0.625 mSec)
	TGAP_LIM_DISC_ADV_INT_MAX =     7,//!< Maximum advertising interval, when in limited discoverable mode (n * 0.625 mSec)
	TGAP_GEN_DISC_ADV_INT_MIN =     8,//!< Minimum advertising interval, when in General discoverable mode (n * 0.625 mSec)
	TGAP_GEN_DISC_ADV_INT_MAX =     9,//!< Maximum advertising interval, when in General discoverable mode (n * 0.625 mSec)
	TGAP_CONN_ADV_INT_MIN =        10,//!< Minimum advertising interval, when in Connectable mode (n * 0.625 mSec)
	TGAP_CONN_ADV_INT_MAX =        11,//!< Maximum advertising interval, when in Connectable mode (n * 0.625 mSec)
	TGAP_CONN_SCAN_INT =           12,//!< Scan interval used during Link Layer Initiating state, when in Connectable mode (n * 0.625 mSec)
	TGAP_CONN_SCAN_WIND =          13,//!< Scan window used during Link Layer Initiating state, when in Connectable mode (n * 0.625 mSec)
	TGAP_CONN_HIGH_SCAN_INT =      14,//!< Scan interval used during Link Layer Initiating state, when in Connectable mode, high duty scan cycle scan parameters (n * 0.625 mSec)
	TGAP_CONN_HIGH_SCAN_WIND =     15,//!< Scan window used during Link Layer Initiating state, when in Connectable mode, high duty scan cycle scan parameters (n * 0.625 mSec)
	TGAP_GEN_DISC_SCAN_INT =       16,//!< Scan interval used during Link Layer Scanning state, when in General Discovery proc (n * 0.625 mSec)
	TGAP_GEN_DISC_SCAN_WIND =      17,//!< Scan window used during Link Layer Scanning state, when in General Discovery proc (n * 0.625 mSec)
	TGAP_LIM_DISC_SCAN_INT =       18,//!< Scan interval used during Link Layer Scanning state, when in Limited Discovery proc (n * 0.625 mSec)
	TGAP_LIM_DISC_SCAN_WIND =      19,//!< Scan window used during Link Layer Scanning state, when in Limited Discovery proc (n * 0.625 mSec)
	TGAP_CONN_EST_ADV =            20,//!< Advertising interval, when using Connection Establishment proc (n * 0.625 mSec). Obsolete - Do not use.
	TGAP_CONN_EST_INT_MIN =        21,//!< Minimum Link Layer connection interval, when using Connection Establishment proc (n * 1.25 mSec)
	TGAP_CONN_EST_INT_MAX =        22,//!< Maximum Link Layer connection interval, when using Connection Establishment proc (n * 1.25 mSec)
	TGAP_CONN_EST_SCAN_INT =       23,//!< Scan interval used during Link Layer Initiating state, when using Connection Establishment proc (n * 0.625 mSec)
	TGAP_CONN_EST_SCAN_WIND =      24,//!< Scan window used during Link Layer Initiating state, when using Connection Establishment proc (n * 0.625 mSec)
	TGAP_CONN_EST_SUPERV_TIMEOUT = 25,//!< Link Layer connection supervision timeout, when using Connection Establishment proc (n * 10 mSec)
	TGAP_CONN_EST_LATENCY =        26,//!< Link Layer connection slave latency, when using Connection Establishment proc (in number of connection events)
	TGAP_CONN_EST_MIN_CE_LEN =     27,//!< Local informational parameter about min len of connection needed, when using Connection Establishment proc (n * 0.625 mSec)
	TGAP_CONN_EST_MAX_CE_LEN =     28,//!< Local informational parameter about max len of connection needed, when using Connection Establishment proc (n * 0.625 mSec)
	TGAP_PRIVATE_ADDR_INT =        29,//!< Minimum Time Interval between private (resolvable) address changes. In minutes (default 15 minutes)
	TGAP_CONN_PAUSE_CENTRAL =      30,//!< Central idle timer. In seconds (default 1 second)
	TGAP_CONN_PAUSE_PERIPHERAL =   31,//!< Minimum time upon connection establishment before the peripheral starts a connection update procedure. In seconds (default 5 seconds)
	// Proprietary
	TGAP_SM_TIMEOUT =              32,//!< SM Message Timeout (milliseconds). Default 30 seconds.
	TGAP_SM_MIN_KEY_LEN =          33,//!< SM Minimum Key Length supported. Default 7.
	TGAP_SM_MAX_KEY_LEN =          34,//!< SM Maximum Key Length supported. Default 16.
	TGAP_FILTER_ADV_REPORTS =      35,//!< Filter duplicate advertising reports. Default TRUE.
	TGAP_SCAN_RSP_RSSI_MIN =       36,//!< Minimum RSSI required for scan responses to be reported to the app. Default -127.
	TGAP_REJECT_CONN_PARAMS =      37,//!< Whether or not to reject Connection Parameter Update Request received on Central device. Default FALSE.
	TGAP_GAP_TESTCODE =          38,//!< GAP TestCodes - puts GAP into a test mode
	TGAP_SM_TESTCODE =           39,//!< SM TestCodes - puts SM into a test mode
	TGAP_AUTH_TASK_ID =          40,//!< Task ID override for Task Authentication control (for stack internal use only)
	TGAP_PARAMID_MAX =           41,//!< ID MAX-valid Parameter ID
	TGAP_GATT_TESTCODE =         100 , //!< GATT TestCodes - puts GATT into a test mode (paramValue maintained by GATT)
	TGAP_ATT_TESTCODE =          101 , //!< ATT TestCodes - puts ATT into a test mode (paramValue maintained by ATT)
	TGAP_GGS_TESTCODE =          102 , //!< GGS TestCodes - puts GGS into a test mode (paramValue maintained by GGS)
	TGAP_L2CAP_TESTCODE =        103 , //!< L2CAP TestCodes - puts L2CAP into a test mode (paramValue maintained by L2CAP)
} eGapParamIDs;

typedef enum {
	GAP_ADTYPE_FLAGS = 0x01,//!< Discovery Mode: @ref GAP_ADTYPE_FLAGS_MODES
	GAP_ADTYPE_16BIT_MORE = 0x02,//!< Service: More 16-bit UUIDs available
	GAP_ADTYPE_16BIT_COMPLETE = 0x03,//!< Service: Complete list of 16-bit UUIDs
	GAP_ADTYPE_32BIT_MORE = 0x04,//!< Service: More 32-bit UUIDs available
	GAP_ADTYPE_32BIT_COMPLETE = 0x05,//!< Service: Complete list of 32-bit UUIDs
	GAP_ADTYPE_128BIT_MORE = 0x06,//!< Service: More 128-bit UUIDs available
	GAP_ADTYPE_128BIT_COMPLETE = 0x07,//!< Service: Complete list of 128-bit UUIDs
	GAP_ADTYPE_LOCAL_NAME_SHORT = 0x08,//!< Shortened local name
	GAP_ADTYPE_LOCAL_NAME_COMPLETE = 0x09,//!< Complete local name
	GAP_ADTYPE_POWER_LEVEL = 0x0A,//!< TX Power Level: 0xXX: -127 to +127 dBm
	GAP_ADTYPE_OOB_CLASS_OF_DEVICE = 0x0D,//!< Simple Pairing OOB Tag: Class of device (3 octets)
	GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC = 0x0E,//!< Simple Pairing OOB Tag: Simple Pairing Hash C (16 octets)
	GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR = 0x0F,//!< Simple Pairing OOB Tag: Simple Pairing Randomizer R (16 octets)
	GAP_ADTYPE_SM_TK = 0x10,//!< Security Manager TK Value
	GAP_ADTYPE_SM_OOB_FLAG = 0x11,//!< Security Manager OOB Flags
	GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE = 0x12,//!< Min and Max values of the connection interval (2 octets Min, 2 octets Max) (0xFFFF indicates no conn interval min or max)
	GAP_ADTYPE_SIGNED_DATA = 0x13,//!< Signed Data field
	GAP_ADTYPE_SERVICES_LIST_16BIT = 0x14,//!< Service Solicitation: list of 16-bit Service UUIDs
	GAP_ADTYPE_SERVICES_LIST_128BIT = 0x15,//!< Service Solicitation: list of 128-bit Service UUIDs
	GAP_ADTYPE_SERVICE_DATA = 0x16,//!< Service Data - 16-bit UUID
	GAP_ADTYPE_PUBLIC_TARGET_ADDR = 0x17,//!< Public Target Address
	GAP_ADTYPE_RANDOM_TARGET_ADDR = 0x18,//!< Random Target Address
	GAP_ADTYPE_APPEARANCE = 0x19,//!< Appearance
	GAP_ADTYPE_ADV_INTERVAL = 0x1A,//!< Advertising Interval
	GAP_ADTYPE_LE_BD_ADDR = 0x1B,//!< LE Bluetooth Device Address
	GAP_ADTYPE_LE_ROLE = 0x1C,//!< LE Role
	GAP_ADTYPE_SIMPLE_PAIRING_HASHC_256 = 0x1D,//!< Simple Pairing Hash C-256
	GAP_ADTYPE_SIMPLE_PAIRING_RANDR_256 = 0x1E,//!< Simple Pairing Randomizer R-256
	GAP_ADTYPE_SERVICE_DATA_32BIT = 0x20,//!< Service Data - 32-bit UUID
	GAP_ADTYPE_SERVICE_DATA_128BIT = 0x21,//!< Service Data - 128-bit UUID
	GAP_ADTYPE_3D_INFO_DATA = 0x3D,//!< 3D Information Data
	GAP_ADTYPE_MANUFACTURER_SPECIFIC = 0xFF //!< Manufacturer Specific Data: first 2 octets contain the Company Identifier Code followed by the additional manufacturer specific data
} eGapAdvTokenType;

/*-----------HCI Command--------------*/
typedef enum {
	PUBLIC_DEVICE,
	RANDOM_DEVICE
} eHciAddrType;


/*-----------Class declare--------------*/
class NPI_CMD: public Com
{
public:
	NPI_CMD();
	NPI_CMD(UINT8 port);
	NPI_CMD(UINT8 nPort, DWORD nBaud, UINT8 nParity, UINT8 nByteSize,
	        UINT8 nStopBit);
	~NPI_CMD();

	/*---------HCI Extended CMD---------*/
	BOOL HciExt_SetRxGain(eRxGain rxGain);
	BOOL HciExt_SetTxPower(eTxPwr txPower);
	BOOL HciExt_ClkDivOnHalt(eClkDivOnHalt ctrl);
	BOOL HciExt_DecNvUsage(eNvUsage ctrl);
	BOOL HciExt_Decrypt(PUINT8 pkey, PUINT8 pdata);
	BOOL HciExt_SetLocalFeatures(PUINT8 ctrl);
	BOOL HciExt_SetFastTxRespTime(eFastTxRespTime ctrl);
	BOOL HciExt_ModemTestTx(eCwModem cw, UINT8 channel);
	BOOL HciExt_ModemHopTestTx(void);
	BOOL HciExt_ModemTestRx(UINT8 channel);
	BOOL HciExt_EndModemTest(void);
	BOOL HciExt_SetBdAddr(PUINT8 addr);
	BOOL HciExt_SetSCA(UINT16 sca);
	BOOL HciExt_SetFreqTune(eFreqTune tune);
	BOOL HciExt_SaveFreqTune(void);
	BOOL HCIExt_SetMaxDtmTxPower(eTxPwr tx_pwr);
	BOOL HCIExt_MapPmIoPort(ePmloPort port, ePmloPortPin port_pin);
	BOOL HCIExt_DisconnectImmed(UINT16 handle);
	BOOL HCIExt_PER(UINT16 handle, ePerTestCmd perCmd);
	BOOL HCIExt_ExtendRfRange(void);
	BOOL HCIExt_HaltDuringRf(eHaltDurRF halt);
	BOOL HCIExt_OverrideSL(eOverrideSL state);
	BOOL HCIExt_BuildRevision(eBuildRevMode mode, UINT16 num);
	BOOL HCIExt_DelaySleep(UINT16 delay);
	BOOL HCIExt_ResetSystem(eResetType type);
	BOOL HCIExt_OverlappedProcessing(eEnDisMode mode);
	BOOL HCIExt_NumComplPktsLimit(UINT8 limit, eEnDisMode mode);

	/*---------L2CAP Command---------*/
	BOOL L2CAP_DisconnectReq(UINT16 cid);
	BOOL L2CAP_InfoReq(UINT16 handle, eInfoType type);
	BOOL L2CAP_ConnParamUpdateReq(UINT16 handle, UINT16 intervalMin,
	                              UINT16 intervalMax, UINT16 slaveLatency, UINT16 timeout);
	BOOL L2CAP_ConnectReq(UINT16 handle, UINT16 PSM, UINT16 peerPSM);
	BOOL L2CAP_FlowCtrlCredit(UINT16 CID, UINT16 peerCredits);
	BOOL L2CAP_Data(UINT16 CID, PUINT8 pSDU, UINT8 len);
	BOOL L2CAP_RegisterPsm(UINT16 psm, UINT16 mtu, UINT16 InitialPeerCrs,
	                       UINT16 PeerCrThreshld, UINT8 MaxNumChannels, UINT8 SecurityVrfctn);
	BOOL L2CAP_DeregisterPsm(UINT16 psm);
	BOOL L2CAP_PsmChannels(UINT16 psm);

	/*---------ATT Command---------*/
	BOOL ATT_ExchangeMTUReq(UINT16 con_hdl, UINT16 mtu);
	BOOL ATT_FindInfoReq(UINT16 con_hdl, UINT16 sta_hdl, UINT16 end_hdl);
	BOOL ATT_FindByTypeValueReq(UINT16 con_hdl, UINT16 sta_hdl,
	                            UINT16 end_hdl, UINT16 type, PUINT8 pvalue, UINT8 len);
	BOOL ATT_ReadByTypeReq(UINT16 con_hdl, UINT16 sta_hdl, UINT16 end_hdl,
	                       PUINT8 pvalue, UINT8 len);
	BOOL ATT_ReadReq(UINT16 con_hdl, UINT16 att_hdl);
	BOOL ATT_ReadBlobReq(UINT16 con_hdl, UINT16 att_hdl, UINT16 offset);
	BOOL ATT_ReadMultiReq(UINT16 con_hdl, PUINT8 att_hdl, UINT8 len);
	BOOL ATT_ReadByGroupTypeReq(UINT16 con_hdl, UINT16 sta_hdl,
	                            UINT16 end_hdl, PUINT8 pvalue, UINT8 len);
	BOOL ATT_WriteReq(UINT16 con_hdl, eYesNoMode sig, eYesNoMode cmd,
	                  UINT16 att_hdl, PUINT8 pvalue, UINT8 len);
	BOOL ATT_PrepareWriteReq(UINT16 con_hdl, UINT16 att_hdl, UINT16 offset,
	                         PUINT8 pvalue, UINT8 len);
	BOOL ATT_ExecuteWriteReq(UINT16 con_hdl, eAttExeWrFlag flag);

	/*---------GATT Command---------*/
	BOOL GATT_ExchangeMTU(UINT16 con_hdl, UINT16 mtu);
	BOOL GATT_DiscAllPrimaryServices(UINT16 con_hdl);
	BOOL GATT_DiscPrimaryServiceByUUID(UINT16 con_hdl, PUINT8 buf, UINT8 len);
	BOOL GATT_FindIncludedServices(UINT16 con_hdl, UINT16 sta_hdl,
	                               UINT16 end_hdl);
	BOOL GATT_DiscAllChar(UINT16 con_hdl, UINT16 sta_hdl, UINT16 end_hdl);
	BOOL GATT_DiscCharByUUID(UINT16 con_hdl, UINT16 sta_hdl, UINT16 end_hdl,
	                         PUINT8 uuid, UINT8 len);
	BOOL GATT_DiscAllCharDesc(UINT16 con_hdl, UINT16 sta_hdl, UINT16 end_hdl);
	BOOL GATT_ReadCharVal(UINT16 con_hdl, UINT16 hdl);
	BOOL GATT_ReadUseCharUUID(UINT16 con_hdl, UINT16 sta_hdl, UINT16 end_hdl,
	                          PUINT8 uuid, UINT8 len);
	BOOL GATT_ReadLongCharValue(UINT16 con_hdl, UINT16 hdl, UINT16 offset);
	BOOL GATT_ReadMultiCharValues(UINT16 con_hdl, PUINT8 att_hdl, UINT8 len);
	BOOL GATT_WriteCharValue(UINT16 con_hdl, UINT16 att_hdl, PUINT8 data,
	                         UINT8 len);
	BOOL GATT_WriteLongCharValue(UINT16 con_hdl, UINT16 att_hdl,
	                             UINT16 offset, PUINT8 data, UINT8 len);
	BOOL GATT_ReliableWrites(UINT16 con_hdl, UINT8 num_req,
	                         sGattWriteVal* pval);
	BOOL GATT_ReadCharDesc(UINT16 con_hdl, UINT16 hdl);
	BOOL GATT_ReadLongCharDesc(UINT16 con_hdl, UINT16 hdl, UINT16 offset);
	BOOL GATT_WriteCharDesc(UINT16 con_hdl, UINT16 offset, PUINT8 val,
	                        UINT8 len);
	BOOL GATT_WriteLongCharDesc(UINT16 con_hdl, UINT16 att_hdl, UINT16 offset,
	                            PUINT8 val, UINT8 len);
	BOOL GATT_AddService(sGattServiceUUID uuid, UINT16 num_attrs,
	                     UINT8 enc_keySize);
	BOOL GATT_DelService(UINT16 handle);
	BOOL GATT_AddAttribute(PUINT8 uuid, UINT8 len, UINT8 permission);

	/*---------GAP Command---------*/
	BOOL GAP_DeviceInit(gapRole_t role, UINT8 max_scanRes, PUINT8 irk,
	                    PUINT8 csrk, PUINT8 sign_count);
	BOOL GAP_ConfigDeviceAddr(eGapAddrType addr_type, PUINT8 addr);
	BOOL GAP_DeviceDiscoveryRequest(eGapDiscMode disc_mdoe,
	                                eEnDisMode act_scan, eEnDisMode white_list);
	BOOL GAP_DeviceDiscoveryCancel(void);
	BOOL GAP_MakeDiscoverable(eGapEventType evt_type, eGapAddrType addr_type,
	                          PUINT8 addr, UINT8 chl_map, eGapFilterPol filter_pol);
	BOOL GAP_UpdateAdvertisingData(eGapAdvDataType adv_type, UINT8 len,
	                               PUINT8 adv_Data);
	BOOL GAP_EndDiscoverable(void);
	BOOL GAP_EstablishLinkRequest(eEnDisMode high_duty, eEnDisMode white_list,
	                              eGapAddrType addr_type, PUINT8 addr);
	BOOL GAP_TerminateLinkRequest(UINT16 handle);
	BOOL GAP_Authenticate(UINT16 con_hdl, sGapAuth* pAuth);
	BOOL GAP_PasskeyUpdate(UINT16 con_hdl, PUINT8 key);
	BOOL GAP_SlaveSecurityRequest(UINT16 con_hdl, uAuthReq auth);
	BOOL GAP_Signable(UINT16 con_hdl, PUINT8 csrk, PUINT8 sign_count);
	BOOL GAP_Bond(UINT16 con_hdl, eEnDisMode auth, PUINT8 ltk, UINT16 div,
	              PUINT8 rand, UINT8 ltk_size);
	BOOL GAP_TerminateAuth(UINT16 con_hdl);
	BOOL GAP_UpdateLinkParamReq(UINT16 con_hdl, UINT16 int_min,
	                            UINT16 int_max, UINT16 latency, UINT16 timeout);
	BOOL GAP_SetParam(eGapParamIDs paraId, UINT16 val);
	BOOL GAP_GetParam(eGapParamIDs paraId);
	BOOL GAP_ResolvePrivateAddr(PUINT8 irk, PUINT8 addr);
	BOOL GAP_SetAdvToken(eGapAdvTokenType type, UINT8 len, PUINT8 adv_data);
	BOOL GAP_RemoveAdvToken(eGapAdvTokenType type);
	BOOL GAP_UpdateAdvTokens(void);
	BOOL GAP_BondSetParam(UINT8 paramId, UINT8 len, PUINT16 value);
	BOOL GAP_BondGetParam(UINT8 paramId);

	/*---------Util Command---------*/
	BOOL UTIL_Reset(eResetType type);
	BOOL UTIL_NVRead(UINT8 nv_id, UINT8 data_len);
	BOOL UTIL_NVWrite(UINT8 nv_id, UINT8 data_len, PUINT8 data);
	/*---------Hci Command---------*/
	BOOL HCI_ReadLocalSupportedFeatures(void);
	BOOL HCI_ReadBdAddr(void);
	BOOL HCI_ReadRSSI(UINT16 con_hdl);
	BOOL HCI_LEReadLocalSupportedFeatures(void);
	BOOL HCI_LEReadWhiteListSize(void);
	BOOL HCI_LEClearWhiteList(void);
	BOOL HCI_LEAddDeviceToWhiteList(eHciAddrType type, PUINT8 addr);
	BOOL HCI_LERemoveDeviceFromWhiteList(eHciAddrType type, PUINT8 addr);
	BOOL HCI_LEConnectionUpdate(UINT16 handle, UINT16 int_min, UINT16 int_max,
	                            UINT16 latency, UINT16 timeout, UINT16 min_len, UINT16 max_len);
	BOOL HCI_LEReceiverTest(UINT8 rx_chl);
	BOOL HCI_LETransmitterTest(UINT8 tx_chl, UINT8 len, PUINT8 data);
	BOOL HCI_LETestEnd(void);



	/*---------HCI Command---------*/
private:
	DWORD WaitFor_Msg(void);

};


#pragma pack ()
#endif