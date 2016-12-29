#include "stdafx.h"
#include "GuiEvtHdl.h"
#include "npi_evt.h"
#include "com.h"

//Gap function declaration
static void discoveryDone(const PUINT8 pdata, UINT16 len);
static void deviceInfo(const PUINT8 pdata, UINT16 len);
static void linkEstablished(const PUINT8 pdata, UINT16 len);
static void linkTerminated(const PUINT8 pdata, UINT16 len);
//Att function declaration
static void attReadRsp(const PUINT8 pdata, UINT16 len);



void RegistEvtCallBack()
{
	//GAP Callback
	theApp.m_cmdHandle->m_evt->RegistCallBack(discoveryDone, HCI_EXT_GAP_DEVICE_DISCOVERY_MSG);
	theApp.m_cmdHandle->m_evt->RegistCallBack(deviceInfo, HCI_EXT_GAP_DEVICE_INFO_MSG);
	theApp.m_cmdHandle->m_evt->RegistCallBack(linkEstablished, HCI_EXT_GAP_LINK_ESTABLISHED_MSG);
	theApp.m_cmdHandle->m_evt->RegistCallBack(linkTerminated, HCI_EXT_GAP_LINK_TERMINATED_MSG);

	//ATT Callback
	theApp.m_cmdHandle->m_evt->RegistCallBack(attReadRsp, ATT_READ_MSG);
}


/************************
/*GAP CALLBACK FUNCTION
/***********************/
//ref struct:GAP_DEVICE_DISCOVERY_EVENT
static void discoveryDone(const PUINT8 pdata, UINT16 len){

	u_GapMsg *msg = (u_GapMsg *)pdata;
	UINT8 numDevs = msg->DiscDone_Evt.numDevs;
	gapDevRec_t *dev;
	wchar_t devNum[5];
	wchar_t slvAdr[20];
	UINT8 temp[8] = {0};
	int i;

	if (msg->DiscDone_Evt.status != BLE_SUCCESS){
		::LogW(L"Discovery Error:%x!", msg->DiscDone_Evt.status);
		return;
	}
	_itow_s(numDevs, (wchar_t *)devNum, 5, 10);
	theApp.m_cmdView->m_page1.m_discDevFound.SetWindowText((LPCTSTR)devNum);
	theApp.m_cmdView->m_page1.m_discScan.ModifyStyle(WS_DISABLED, 0);

	for (i = 0; i < numDevs; i++){
		dev = &msg->DiscDone_Evt.DevList[i*sizeof(gapDevRec_t)];
		//convert bdaddr to unicode
		memcpy_s(temp, 8, dev->addr, 6);
		swprintf_s(slvAdr, _T("%llX"), *(long long *)temp);
		theApp.m_cmdView->m_page1.m_esSlvBDACbx.ResetContent();
		//theApp.m_cmdView->m_page1.m_esSlvBDACbx.SetEditSel(0, -1);
		//theApp.m_cmdView->m_page1.m_esSlvBDACbx.Clear();
		//add to combobox
		theApp.m_cmdView->m_page1.m_esSlvBDACbx.AddString(slvAdr);//_wcsupr_s(slvAdr);
		theApp.m_cmdView->m_page1.m_esSlvBDACbx.SetCurSel
			(theApp.m_cmdView->m_page1.m_esSlvBDACbx.GetCount() - 1);
	}
}

static void deviceInfo(const PUINT8 pdata, UINT16 len){
	/*nothing*/
}

static void linkEstablished(const PUINT8 pdata, UINT16 len){
	u_GapMsg *msg = (u_GapMsg *)pdata;
	wchar_t slvAdr[14];
	wchar_t hdlBuf[10];
	UINT8 temp[8] = { 0 };
	if (msg->EstLink_Evt.status != BLE_SUCCESS){
		::LogW(L"Link Established Error:%x!", msg->EstLink_Evt.status);
		return;
	}
	//update state
	theApp.m_cmdView->m_page1.m_tlConStStatic.SetWindowText(_T("Connected"));
	//update bd address
	memcpy_s(temp, 8, msg->EstLink_Evt.devAddr, 6);
	swprintf_s(slvAdr, 14, _T("%llX"), *(long long *)temp);
	theApp.m_cmdView->m_page1.m_tlConBDA.SetWindowText(slvAdr);
	//update connect handle
	swprintf_s(hdlBuf, 10, _T("0x%04X"), msg->EstLink_Evt.connectionHandle);
	theApp.m_cmdView->m_page1.m_tlHandleEdit.SetWindowText(hdlBuf);
	theApp.m_cmdView->m_page2.m_crHdlEdit.SetWindowText(hdlBuf);
	theApp.m_cmdView->m_page2.m_cwHdlEdit.SetWindowText(hdlBuf);

}

static void linkTerminated(const PUINT8 pdata, UINT16 len){
	u_GapMsg *msg = (u_GapMsg *)pdata;
	if (msg->TerLink_Evt.status != BLE_SUCCESS){
		::LogW(L"Link Terminate Error:%x!", msg->TerLink_Evt.status);
		return;
	}
	//update state
	theApp.m_cmdView->m_page1.m_tlConStStatic.SetWindowTextW(_T("Disconnected"));
	//update bd address
	theApp.m_cmdView->m_page1.m_tlConBDA.SetWindowText(_T("None"));
	//update handle
	theApp.m_cmdView->m_page1.m_tlHandleEdit.SetWindowText(_T("0xFFFE"));
}

/************************
/*ATT CALLBACK FUNCTION
/***********************/
static void attReadRsp(const PUINT8 pdata, UINT16 len){
	u_AttMsg *msg = (u_AttMsg *)pdata;
	wchar_t temp[512];
	wchar_t hdlBuf[20];
	long hdlInList, hdlInCmd;
	int i,pos=0;

	if (msg->readRsp.status != BLE_SUCCESS){
		::LogW(L"Att Resp Error:%x!", msg->readRsp.status);
		return;
	}
	int nIndex = theApp.m_gattView->m_list.GetItemCount();
	theApp.m_cmdView->m_page2.m_crCharValHdl.GetWindowText(hdlBuf, 20);
	hdlInCmd = wcstol(hdlBuf, NULL, 16);
	for (i = 0; i < nIndex; i++){
		theApp.m_gattView->m_list.GetItemText(i, 1, temp, 20);
		hdlInList = wcstol(temp, NULL, 16);
		if (hdlInCmd < hdlInList){
			break;
		}
		if (hdlInCmd == hdlInList){
			//update value
			if (msg->readRsp.pduLen > 0){
				hex2wstr(temp, msg->readRsp.value, msg->readRsp.pduLen);
				theApp.m_gattView->m_list.SetItemText(i, 4, temp);
				theApp.m_cmdView->m_page2.m_crRadio = 2;
				theApp.m_cmdView->m_page2.m_crValStr = temp;
				theApp.m_cmdView->m_page2.m_crValEdit.SetWindowText(temp);
			}
			return;
		}
	}
	//add connection handle
	CString itemAdd;
	itemAdd.Format(_T("0x%04X"), msg->readRsp.conHdl);
	theApp.m_gattView->m_list.InsertItem(i,itemAdd);
	//add handle
	theApp.m_gattView->m_list.SetItemText(i, 1, hdlBuf);
	//add value
	if (msg->readRsp.pduLen > 0){
		hex2wstr(temp, msg->readRsp.value, msg->readRsp.pduLen);
		theApp.m_gattView->m_list.SetItemText(i, 4, temp);
		theApp.m_cmdView->m_page2.m_crRadio = 2;
		theApp.m_cmdView->m_page2.m_crValStr = temp;
		theApp.m_cmdView->m_page2.m_crValEdit.SetWindowText(temp);
	}
}