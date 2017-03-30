/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
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

#ifndef _NPI_EVT_H_
#define _NPI_EVT_H_

#include "CommonDef.h"
#include "npi_tl.h"
#include "gap.h"
#include "gatt.h"
#pragma pack(1)
/*---------------------------
/*           EVT            /
/*-------------------------*/
typedef struct {
	UINT8 type;
	UINT8 event_code;
	UINT8 len;
	UINT8 event_lo;
	UINT8 event_hi;
	UINT8 status;
	UINT8 data[MAX_RX_SIZE];
} sNpiEvt;

typedef struct {
	UINT8 type;
	UINT8 event_code;
	UINT8 len;
	UINT8 packets;
	UINT8 op_lo;
	UINT8 op_hi;
	UINT8 status;
	UINT8 data[MAX_RX_SIZE];
} sHciEvt;

#define HCI_EXT_LL_EVENT                                                    0x0400
#define HCI_EXT_L2CAP_EVENT                                                 0x0480
#define HCI_EXT_ATT_EVENT                                                   0x0500
#define HCI_EXT_GATT_EVENT                                                  0x0580
#define HCI_EXT_GAP_EVENT                                                   0x0600
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
#define L2CAP_DATA_EVT                   (HCI_EXT_L2CAP_EVENT | 0x70)//0x04F0 !< L2CAP DATA

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
/*      L2CAP Event Struct   /
/*-------------------------*/
typedef enum {
	L2CAP_REJECT_CMD_NOT_UNDERSTOOD,
	L2CAP_REJECT_SIGNAL_MTU_EXCEED,
	L2CAP_REJECT_INVALID_CID
} eRejectReason;

typedef struct {
	UINT8 status;           //refer eRetStatus
	UINT8 handle_lo;
	UINT8 handle_hi;
	UINT8 reason_lo;        //refer eRejectReason
	UINT8 reason_hi;
} sL2capCmdRejEvt;

typedef void(*PCallBack)(const PUINT8 pdata, UINT16 len);

class NPI_EVT : public Runnable
{
private:
	NPI_Dict < UINT16, PCallBack, TOTAL_OPCODE_COUNTS > m_EvtCB;
public:
	NPI_EVT(HANDLE hdl);
	~NPI_EVT();
	void Run();
	BOOL RegistCallBack(const HANDLE, UINT16 evt);

private:
	HANDLE comHdl;
	INT16 GetPosFromMsg(INT16 msg);
	void Hci_Status_Event(const PUINT8 pBuf, UINT16 len);
	void Gap_Status_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_Decrypt_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapStatus_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_RdLocSuppFeat_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_RdBdaddr_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_RdRssi_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_LE_SetEvtMsk_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_LE_RdLocSuppFeat_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_RdWhiteListSize_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_LE_ConnUpdate_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_LE_TestEnd_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_LE_RemConnParaReqRep_Event(const PUINT8 pBuf, UINT16 len);
	void Hci_LE_RemConnParaReqNegRep_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_CmdReject_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_DiscRsp_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_InfoRsp_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_ParamUpdateRsp_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_ConnRsp_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_ChannelEstablish_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_ChannelTerminate_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_OutOfCredit_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_PeerCredThreshold_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_SendSduDone_Event(const PUINT8 pBuf, UINT16 len);
	void L2cap_Data_Event(const PUINT8 pBuf, UINT16 len);
	void Att_Error_Event(const PUINT8 pBuf, UINT16 len);
	void Att_ExchangeMtu_Event(const PUINT8 pBuf, UINT16 len);
	void Att_FindInfo_Event(const PUINT8 pBuf, UINT16 len);
	void Att_FindByTypeValue_Event(const PUINT8 pBuf, UINT16 len);
	void Att_ReadByType_Event(const PUINT8 pBuf, UINT16 len);
	void Att_Read_Event(const PUINT8 pBuf, UINT16 len);
	void Att_ReadBlob_Event(const PUINT8 pBuf, UINT16 len);
	void Att_ReadMulti_Event(const PUINT8 pBuf, UINT16 len);
	void Att_ReadByGrpType_Event(const PUINT8 pBuf, UINT16 len);
	void Att_Write_Event(const PUINT8 pBuf, UINT16 len);
	void Att_PrepareWrite_Event(const PUINT8 pBuf, UINT16 len);
	void Att_ExecuteWrite_Event(const PUINT8 pBuf, UINT16 len);
	void Att_HanddlValueNoti_Event(const PUINT8 pBuf, UINT16 len);
	void Att_HandleValueInd_Event(const PUINT8 pBuf, UINT16 len);
	void Att_HandleValueCfm_Event(const PUINT8 pBuf, UINT16 len);
	void Att_MtuUpdated_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapDevInitDone_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapDevDiscovery_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapAdvDataUpdateDone_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapMakeDiscoverableDone_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapEndDiscoverableDone_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapLinkEstablished_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapLinkTerminated_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapLinkParamUpdate_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapRandomAddrChanged_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapSignatureUpdated_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapAuthComplete_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapPasskeyNeeded_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapSlaveReqSec_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapDevInfo_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapBondComp_Event(const PUINT8 pBuf, UINT16 len);
	void HciExt_GapPairReq_Event(const PUINT8 pBuf, UINT16 len);
};

#pragma pack()

#endif