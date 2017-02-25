#include"stdafx.h"
#include"npi_evt.h"
#include"com.h"
#include"log.h"

NPI_EVT::NPI_EVT(HANDLE hdl)
{
	comHdl = hdl;
}

NPI_EVT::~NPI_EVT()
{
	comHdl = NULL;
}

void NPI_EVT::Run()
{
	UINT16 msg_type = 0;
	PCallBack pFunc;
	HANDLE hThread = ((Com *)comHdl)->evtThread;
	HANDLE hEvent = ((Com *)comHdl)->evtThread->GetEvent();
	while (1) {
		sEvt* pEvt = ((NPI_RX*)((Com*)comHdl)->m_rx)->Get_Queue()->Pop();
		if (pEvt->type == HCI_EXIT_PACKET){
			CloseHandle(hEvent);
			::ReleaseSemaphore(g_semhdl, 1, NULL);
			delete pEvt;
			return;
		}
		if (pEvt->evtCode == HCI_LE_EVENT_CODE) {
			sHciEvt* pHciEvt = (sHciEvt*)pEvt;
			UINT16 opCode = BUILD_UINT16(pHciEvt->op_lo, pHciEvt->op_hi);
			pFunc = m_EvtCB.get(opCode);
			if (pFunc) {
				pFunc(&pHciEvt->status, pHciEvt->len - 3);
			}
		} else {
			sNpiEvt* pNpiEvt = (sNpiEvt*)pEvt;
			UINT16 event = BUILD_UINT16(pEvt->data[0], pEvt->data[1]);
			pFunc = m_EvtCB.get(event);
			if (pFunc) {
				pFunc(&pNpiEvt->status, pNpiEvt->len - 2);
			}
		}
		delete pEvt;
	}
}
INT16 NPI_EVT::GetPosFromMsg(INT16 msg)
{

	INT16 pos = -1;

	if ((msg >= HCI_MSG_HEAD)
	    && (msg <= HCI_MSG_HEAD + HCI_MSG_COUNT)) {
		pos = msg - HCI_MSG_HEAD;
	} else if ((msg >= L2CAP_MSG_HEAD)
	           && (msg <= L2CAP_MSG_HEAD + L2CAP_MSG_COUNT)) {
		pos = HCI_MSG_COUNT + msg - L2CAP_MSG_HEAD;
	} else if ((msg >= ATT_MSG_HEAD)
	           && (msg <= ATT_MSG_HEAD + ATT_MSG_COUNT)) {
		pos = HCI_MSG_COUNT + L2CAP_MSG_COUNT + msg - ATT_MSG_HEAD;
	} else if ((msg >= GAP_MSG_HEAD)
	           && (msg <= GAP_MSG_HEAD + GAP_MSG_COUNT)) {
		pos = HCI_MSG_COUNT + L2CAP_MSG_COUNT + ATT_MSG_COUNT + msg - GAP_MSG_HEAD;
	} else {
		return -1;
	}
	return pos;
}
BOOL NPI_EVT::RegistCallBack(const HANDLE hdl,
                             UINT16 evt)
{
	m_EvtCB.put(evt, (PCallBack)hdl);
	return TRUE;
}

/*---------------------------
/*              HCI Extended Event      /
/*-------------------------*/
void NPI_EVT::Hci_Status_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/

	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}
/*---------------------------
/*              Gap Status Event        /
/*-------------------------*/
void NPI_EVT::Gap_Status_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/

	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

/*---------------------------
/*              HCI Extended Event      /
/*-------------------------*/
void NPI_EVT::HciExt_Decrypt_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/

	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

/*---------------------------
/*              HCI Event                       /
/*-------------------------*/
void NPI_EVT::Hci_RdLocSuppFeat_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_RdBdaddr_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_RdRssi_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_SetEvtMsk_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_RdLocSuppFeat_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_RdWhiteListSize_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_ConnUpdate_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_TestEnd_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_RemConnParaReqRep_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

void NPI_EVT::Hci_LE_RemConnParaReqNegRep_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sHciEvt, status));
}

/*---------------------------
/*              L2cap Event                     /
/*-------------------------*/
void NPI_EVT::L2cap_CmdReject_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));

}

void NPI_EVT::L2cap_DiscRsp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_InfoRsp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_ParamUpdateRsp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_ConnRsp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_ChannelEstablish_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_ChannelTerminate_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_OutOfCredit_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_PeerCredThreshold_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_SendSduDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::L2cap_Data_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_Error_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ExchangeMtu_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_FindInfo_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_FindByTypeValue_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ReadByType_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_Read_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ReadBlob_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ReadMulti_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ReadByGrpType_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_Write_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_PrepareWrite_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_ExecuteWrite_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_HanddlValueNoti_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_HandleValueInd_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_HandleValueCfm_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::Att_MtuUpdated_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

/*---------------------------
/*              GAP Event                       /
/*-------------------------*/
void NPI_EVT::HciExt_GapStatus_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapDevInitDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapDevDiscovery_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapAdvDataUpdateDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapMakeDiscoverableDone_Event(const PUINT8 pBuf,
                UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapEndDiscoverableDone_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapLinkEstablished_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapLinkTerminated_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapLinkParamUpdate_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapRandomAddrChanged_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapSignatureUpdated_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapAuthComplete_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapPasskeyNeeded_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapSlaveReqSec_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapDevInfo_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapBondComp_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

void NPI_EVT::HciExt_GapPairReq_Event(const PUINT8 pBuf, UINT16 len)
{
	/*User App Code Block*/


	/*Do not Modify*/
	delete(pBuf - offsetof(sNpiEvt, status));
}

