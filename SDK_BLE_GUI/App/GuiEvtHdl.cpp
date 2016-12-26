#include "stdafx.h"
#include "GuiEvtHdl.h"
#include "npi_evt.h"
#include "com.h"

static void discoveryDone(PUINT8 pdata, UINT16 len);
static void deviceInfo(PUINT8 pdata, UINT16 len);
static void linkEstablished(PUINT8 pdata, UINT16 len);
static void linkTerminated(PUINT8 pdata, UINT16 len);


void RegistEvtCallBack()
{
	theApp.m_cmdHandle->m_evt->RegistCallBack(discoveryDone, HCI_EXT_GAP_DEVICE_DISCOVERY_MSG);
	theApp.m_cmdHandle->m_evt->RegistCallBack(deviceInfo, HCI_EXT_GAP_DEVICE_INFO_MSG);
	theApp.m_cmdHandle->m_evt->RegistCallBack(linkEstablished, HCI_EXT_GAP_LINK_ESTABLISHED_MSG);
	theApp.m_cmdHandle->m_evt->RegistCallBack(linkTerminated, HCI_EXT_GAP_LINK_TERMINATED_MSG);
}

//ref struct:GAP_DEVICE_DISCOVERY_EVENT
static void discoveryDone(PUINT8 pdata, UINT16 len){

	UINT8 numDevs = pdata[0];
	gapDevRec_t *dev;
	wchar_t devNum[5];
	wchar_t slvAdr[20];
	UINT8 temp[8] = {0};
	int i;

	_itow_s(numDevs, (wchar_t *)devNum, 5, 10);
	theApp.m_cmdView->m_discPage.m_discDevFound.SetWindowText((LPCTSTR)devNum);
	theApp.m_cmdView->m_discPage.m_discScan.ModifyStyle(WS_DISABLED, 0);

	for (i = 0; i < numDevs; i++){
		dev = (gapDevRec_t *)&pdata[1 + i*sizeof(gapDevRec_t)];
		//convert bdaddr to unicode
		memcpy_s(temp, 8, dev->addr, 6);
		swprintf_s(slvAdr, _T("%llX"), *(long long *)temp);
		theApp.m_cmdView->m_discPage.m_esSlvBDACbx.ResetContent();
		//theApp.m_cmdView->m_discPage.m_esSlvBDACbx.SetEditSel(0, -1);
		//theApp.m_cmdView->m_discPage.m_esSlvBDACbx.Clear();
		//add to combobox
		theApp.m_cmdView->m_discPage.m_esSlvBDACbx.AddString(slvAdr);//_wcsupr_s(slvAdr);
		theApp.m_cmdView->m_discPage.m_esSlvBDACbx.SetCurSel
			(theApp.m_cmdView->m_discPage.m_esSlvBDACbx.GetCount() - 1);
	}
}

static void deviceInfo(PUINT8 pdata, UINT16 len){

}

static void linkEstablished(PUINT8 pdata, UINT16 len){
	gapEstLinkReqEvent_t *pEstEvt = (gapEstLinkReqEvent_t *)pdata;
	wchar_t slvAdr[14];
	wchar_t handle[5];
	wchar_t hdlBuf[10];
	UINT8 temp[8] = { 0 };
	//update state
	theApp.m_cmdView->m_discPage.m_tlConStStatic.SetWindowText(_T("Connected"));
	//update bd address
	memcpy_s(temp, 8, pEstEvt->devAddr, 6);
	swprintf_s(slvAdr, _T("%llX"), *(long long *)temp);
	theApp.m_cmdView->m_discPage.m_tlConBDA.SetWindowText(slvAdr);
	//update connect handle
	_itow_s(pEstEvt->connectionHandle, handle, 16);
	swprintf_s(hdlBuf, 10, _T("0x"));
	wcscat_s(hdlBuf, 10 - wcslen(handle), handle);
	theApp.m_cmdView->m_discPage.m_tlHandleEdit.SetWindowText(hdlBuf);

}

static void linkTerminated(PUINT8 pdata, UINT16 len){
	//update state
	theApp.m_cmdView->m_discPage.m_tlConStStatic.SetWindowTextW(_T("Disconnected"));
	//update bd address
	theApp.m_cmdView->m_discPage.m_tlConBDA.SetWindowText(_T("None"));
	//update handle
	theApp.m_cmdView->m_discPage.m_tlHandleEdit.SetWindowText(_T("0xFFFE"));
}