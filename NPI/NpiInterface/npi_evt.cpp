#include"stdafx.h"
#include"npi_evt.h"
#include"com.h"


NPI_EVT::NPI_EVT()
{
	memset((void *)m_EvtCB, 0, TOTAL_MSG_COUNT*sizeof(m_EvtCB[0]));
}

NPI_EVT::~NPI_EVT()
{
}

void NPI_EVT::Run()
{
	MSG	msg;
	PUINT8  buf;
	UINT16	size;
	INT16	pos;
	while (1) { 
		if (GetMessage(&msg, 0, 0, 0)) //get msg from message queue
		{
			buf = (PUINT8)msg.wParam;
			size = (UINT16)msg.lParam;

			pos = GetPosFromMsg(msg.message);
			if ((pos > 0) && (pos < TOTAL_MSG_COUNT)){
				m_EvtCB[pos] == NULL ? NULL : m_EvtCB[pos](buf, size);//callback
			}

			switch (msg.message) {
				case HCI_DECRYPT_MSG:
					HciExt_Decrypt_Event(buf, size);
					break;
				case HCI_GAP_STATUS_MSG:
					HciExt_GapStatus_Event(buf, size);
					break;
				case HCI_READ_LOCAL_SUPPORT_FEAT_MSG:
					Hci_RdLocSuppFeat_Event(buf, size);
					break;
				case HCI_READ_BDADDR_MSG:
					Hci_RdBdaddr_Event(buf, size);
					break;
				case HCI_READ_RSSI_MSG:
					Hci_RdRssi_Event(buf, size);
					break;
				case HCI_LE_SET_EVENT_MASK_MSG:
					Hci_LE_SetEvtMsk_Event(buf, size);
					break;
				case HCI_LE_READ_LOCAL_SUPPORT_FEAT_MSG:
					Hci_LE_RdLocSuppFeat_Event(buf, size);
					break;
				case HCI_LE_READ_WHITE_LIST_SIZE_MSG:
					Hci_RdWhiteListSize_Event(buf, size);
					break;
				case HCI_LE_CONNECTION_UPDATE_MSG:
					Hci_LE_ConnUpdate_Event(buf, size);
					break;
				case HCI_LE_TEST_END_MSG:
					Hci_LE_TestEnd_Event(buf, size);
					break;
				case HCI_LE_REM_CONN_PARAM_REQ_REP_MSG:
					Hci_LE_RemConnParaReqRep_Event(buf, size);
					break;
				case HCI_LE_REM_CONN_PARAM_REQ_NEG_REP_MSG:
					Hci_LE_RemConnParaReqNegRep_Event(buf, size);
					break;
				case L2CAP_COMMAND_REJECT_MSG:
					L2cap_CmdReject_Event(buf, size);
					break;
				case L2CAP_DISCONNECT_RSP_MSG:
					L2cap_DiscRsp_Event(buf, size);
					break;
				case L2CAP_INFO_RSP_MSG:
					L2cap_InfoRsp_Event(buf, size);
					break;
				case L2CAP_PARAM_UPDATE_RSP_MSG:
					L2cap_ParamUpdateRsp_Event(buf, size);
					break;
				case L2CAP_CONNECT_RSP_MSG:
					L2cap_ConnRsp_Event(buf, size);
					break;
				case L2CAP_CHANNEL_ESTABLISHED_MSG:
					L2cap_ChannelEstablish_Event(buf, size);
					break;
				case L2CAP_CHANNEL_TERMINATED_MSG:
					L2cap_ChannelTerminate_Event(buf, size);
					break;
				case L2CAP_OUT_OF_CREDIT_MSG:
					L2cap_OutOfCredit_Event(buf, size);
					break;
				case L2CAP_PEER_CREDIT_THRESHOLD_MSG:
					L2cap_PeerCredThreshold_Event(buf, size);
					break;
				case L2CAP_SEND_SDU_DONE_MSG:
					L2cap_SendSduDone_Event(buf, size);
					break;
				case L2CAP_DATA_MSG:
					L2cap_Data_Event(buf, size);
					break;
				case ATT_ERROR_MSG:
					Att_Error_Event(buf, size);
					break;
				case ATT_EXCHANGE_MTU_MSG:
					Att_ExchangeMtu_Event(buf, size);
					break;
				case ATT_FIND_INFO_MSG:
					Att_FindInfo_Event(buf, size);
					break;
				case ATT_FIND_BY_TYPE_VALUE_MSG:
					Att_FindByTypeValue_Event(buf, size);
					break;
				case ATT_READ_BY_TYPE_MSG:
					Att_ReadByType_Event(buf, size);
					break;
				case ATT_READ_MSG:
					Att_Read_Event(buf, size);
					break;
				case ATT_READ_BLOB_MSG:
					Att_ReadBlob_Event(buf, size);
					break;
				case ATT_READ_MULTI_MSG:
					Att_ReadMulti_Event(buf, size);
					break;
				case ATT_READ_BY_GRP_TYPE_MSG:
					Att_ReadByGrpType_Event(buf, size);
					break;
				case ATT_WRITE_MSG:
					Att_Write_Event(buf, size);
					break;
				case ATT_PREPARE_WRITE_MSG:
					Att_PrepareWrite_Event(buf, size);
					break;
				case ATT_EXECUTE_WRITE_MSG:
					Att_ExecuteWrite_Event(buf, size);
					break;
				case ATT_HANDLE_VALUE_NOTI_MSG:
					Att_HanddlValueNoti_Event(buf, size);
					break;
				case ATT_HANDLE_VALUE_IND_MSG:
					Att_HandleValueInd_Event(buf, size);
					break;
				case ATT_HANDLE_VALUE_CFM_MSG:
					Att_HandleValueCfm_Event(buf, size);
					break;
				case ATT_MTU_UPDATED_MSG:
					Att_MtuUpdated_Event(buf, size);
					break;
				case HCI_EXT_GAP_DEVICE_INIT_DONE_MSG:
					HciExt_GapDevInitDone_Event(buf, size);
					break;
				case HCI_EXT_GAP_DEVICE_DISCOVERY_MSG:
					HciExt_GapDevDiscovery_Event(buf, size);
					break;
				case HCI_EXT_GAP_ADV_DATA_UPDATE_DONE_MSG:
					HciExt_GapAdvDataUpdateDone_Event(buf, size);
					break;
				case HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_MSG:
					HciExt_GapMakeDiscoverableDone_Event(buf, size);
					break;
				case HCI_EXT_GAP_END_DISCOVERABLE_DONE_MSG:
					HciExt_GapEndDiscoverableDone_Event(buf, size);
					break;
				case HCI_EXT_GAP_LINK_ESTABLISHED_MSG:
					HciExt_GapLinkEstablished_Event(buf, size);
					break;
				case HCI_EXT_GAP_LINK_TERMINATED_MSG:
					HciExt_GapLinkTerminated_Event(buf, size);
					break;
				case HCI_EXT_GAP_LINK_PARAM_UPDATE_MSG:
					HciExt_GapLinkParamUpdate_Event(buf, size);
					break;
				case HCI_EXT_GAP_RANDOM_ADDR_CHANGED_MSG:
					HciExt_GapRandomAddrChanged_Event(buf, size);
					break;
				case HCI_EXT_GAP_SIGNATURE_UPDATED_MSG:
					HciExt_GapSignatureUpdated_Event(buf, size);
					break;
				case HCI_EXT_GAP_AUTH_COMPLETE_MSG:
					HciExt_GapAuthComplete_Event(buf, size);
					break;
				case HCI_EXT_GAP_PASSKEY_NEEDED_MSG:
					HciExt_GapPasskeyNeeded_Event(buf, size);
					break;
				case HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_MSG:
					HciExt_GapSlaveReqSec_Event(buf, size);
					break;
				case HCI_EXT_GAP_DEVICE_INFO_MSG:
					HciExt_GapDevInfo_Event(buf, size);
					break;
				case HCI_EXT_GAP_BOND_COMPLETE_MSG:
					HciExt_GapBondComp_Event(buf, size);
					break;
				case HCI_EXT_GAP_PAIRING_REQ_MSG:
					HciExt_GapPairReq_Event(buf, size);
					break;
				default:
					LogE(L"NPI_EVT£ºErr msg type=%4X !\n", msg.message);
					break;
			}
		}
	}
}
INT16 NPI_EVT::GetPosFromMsg(INT16 msg){

	INT16 pos = -1;

	if ((msg >= HCI_MSG_HEAD)
		&& (msg <= HCI_MSG_HEAD + HCI_MSG_COUNT)){
		pos = msg - HCI_MSG_HEAD;
	}
	else if ((msg >= L2CAP_MSG_HEAD)
		&& (msg <= L2CAP_MSG_HEAD + L2CAP_MSG_COUNT)){
		pos = HCI_MSG_COUNT + msg - L2CAP_MSG_HEAD;
	}
	else if ((msg >= ATT_MSG_HEAD)
		&& (msg <= ATT_MSG_HEAD + ATT_MSG_COUNT)){
		pos = HCI_MSG_COUNT + L2CAP_MSG_COUNT + msg - ATT_MSG_HEAD;
	}
	else if ((msg >= GAP_MSG_HEAD)
		&& (msg <= GAP_MSG_HEAD + GAP_MSG_COUNT)){
		pos = HCI_MSG_COUNT + L2CAP_MSG_COUNT + ATT_MSG_COUNT + msg - GAP_MSG_HEAD;
	}
	else{
		return -1;
	}
	return pos;
}
BOOL NPI_EVT::RegistCallBack(void(*pFun)(const PUINT8 pBuf, UINT16 len), UINT16 msg){

	INT16 pos;

	pos = GetPosFromMsg(msg);
	if ((pos > 0) && (pos < TOTAL_MSG_COUNT)){
		m_EvtCB[pos] = pFun;//callback
	}
	else{
		return FALSE;
	}

	return TRUE;
}

/*---------------------------
/*		HCI Extended Event	/
/*-------------------------*/
void NPI_EVT::HciExt_Decrypt_Event(const PUINT8 pBuf, UINT16 len) {
	/*User App Code Block*/

	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

/*---------------------------
/*		HCI Event			/
/*-------------------------*/
void NPI_EVT::Hci_RdLocSuppFeat_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_RdBdaddr_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_RdRssi_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_SetEvtMsk_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_RdLocSuppFeat_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_RdWhiteListSize_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_ConnUpdate_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_TestEnd_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_RemConnParaReqRep_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_RemConnParaReqNegRep_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sHciEvt, status));
}

/*---------------------------
/*		L2cap Event			/
/*-------------------------*/
void NPI_EVT::L2cap_CmdReject_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));

}

void NPI_EVT::L2cap_DiscRsp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_InfoRsp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_ParamUpdateRsp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_ConnRsp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_ChannelEstablish_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_ChannelTerminate_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_OutOfCredit_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_PeerCredThreshold_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_SendSduDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_Data_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_Error_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ExchangeMtu_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_FindInfo_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_FindByTypeValue_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ReadByType_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_Read_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ReadBlob_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ReadMulti_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ReadByGrpType_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_Write_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_PrepareWrite_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ExecuteWrite_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_HanddlValueNoti_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_HandleValueInd_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_HandleValueCfm_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_MtuUpdated_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

/*---------------------------
/*		GAP Event			/
/*-------------------------*/
void NPI_EVT::HciExt_GapStatus_Event(const PUINT8 pBuf, UINT16 len) {
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapDevInitDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapDevDiscovery_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapAdvDataUpdateDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapMakeDiscoverableDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapEndDiscoverableDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapLinkEstablished_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapLinkTerminated_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapLinkParamUpdate_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapRandomAddrChanged_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapSignatureUpdated_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapAuthComplete_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapPasskeyNeeded_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapSlaveReqSec_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapDevInfo_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapBondComp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapPairReq_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete (pBuf - offsetof(sNpiEvt, status));
}

