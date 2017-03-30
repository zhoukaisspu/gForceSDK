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
#include "stdafx.h"
#include "GuiEvtHdl.h"
#include "npi_evt.h"
#include "com.h"
#include "log.h"
//Gap function declaration
static void discoveryDone(const PUINT8 pdata, UINT16 len);
static void deviceInfo(const PUINT8 pdata, UINT16 len);
static void linkEstablished(const PUINT8 pdata, UINT16 len);
static void linkTerminated(const PUINT8 pdata, UINT16 len);
static void authComplete(const PUINT8 pdata, UINT16 len);
//Att function declaration
static void attReadRsp(const PUINT8 pdata, UINT16 len);



void RegistEvtCallBack()
{
	//GAP Callback
	((NPI_EVT*)theApp.m_cmdHandle->m_evt)->RegistCallBack(discoveryDone,
	                HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT);
	((NPI_EVT*)theApp.m_cmdHandle->m_evt)->RegistCallBack(deviceInfo,
	                HCI_EXT_GAP_DEVICE_INFO_EVENT);
	((NPI_EVT*)theApp.m_cmdHandle->m_evt)->RegistCallBack(linkEstablished,
	                HCI_EXT_GAP_LINK_ESTABLISHED_EVENT);
	((NPI_EVT*)theApp.m_cmdHandle->m_evt)->RegistCallBack(linkTerminated,
	                HCI_EXT_GAP_LINK_TERMINATED_EVENT);
	((NPI_EVT*)theApp.m_cmdHandle->m_evt)->RegistCallBack(authComplete,
	                HCI_EXT_GAP_AUTH_COMPLETE_EVENT);
	//ATT Callback
	((NPI_EVT*)theApp.m_cmdHandle->m_evt)->RegistCallBack(attReadRsp,
	                ATT_READ_EVENT);
}


/************************
/*GAP CALLBACK FUNCTION
/***********************/
//ref struct:GAP_DEVICE_DISCOVERY_EVENT
static void discoveryDone(const PUINT8 pdata, UINT16 len)
{

	u_GapMsg* msg = (u_GapMsg*)pdata;
	UINT8 numDevs = msg->DiscDone_Evt.numDevs;
	gapDevRec_t* dev;
	wchar_t devNum[5] = {0};
	wchar_t slvAdr[20] = {0};
	wchar_t dst[20] = {0};
	//UINT8 temp[8] = {0};
	int i;

	if (msg->DiscDone_Evt.status != BLE_SUCCESS) {
		::LogW(L"Discovery Error:%x!", msg->DiscDone_Evt.status);
		theApp.m_cmdView->m_page1.m_discScan.ModifyStyle(WS_DISABLED, 0);
		return;
	}
	_itow_s(numDevs, (wchar_t*)devNum, 5, 10);
	theApp.m_cmdView->m_page1.m_discDevFound.SetWindowText((LPCTSTR)devNum);
	theApp.m_cmdView->m_page1.m_discScan.ModifyStyle(WS_DISABLED, 0);

	theApp.m_cmdView->m_page1.m_esSlvBDACbx.ResetContent();
	for (i = 0; i < numDevs; i++) {
		dev = &msg->DiscDone_Evt.DevList[i];
		//convert bdaddr to unicode
		hex2wstr(dst, dev->addr, 6);
		//add to combobox
		theApp.m_cmdView->m_page1.m_esSlvBDACbx.AddString(dst);
		theApp.m_cmdView->m_page1.m_esSlvBDACbx.SetCurSel
		(theApp.m_cmdView->m_page1.m_esSlvBDACbx.GetCount() - 1);
	}
}

static void deviceInfo(const PUINT8 pdata, UINT16 len)
{
	/*nothing*/
}

static void linkEstablished(const PUINT8 pdata, UINT16 len)
{
	u_GapMsg* msg = (u_GapMsg*)pdata;
	wchar_t hdlBuf[10] = {0};
	wchar_t wBuf[10] = {0};
	wchar_t slvAdr[20] = { 0 };
	UINT8 temp[10] = { 0 };
	if (msg->EstLink_Evt.status != BLE_SUCCESS) {
		::LogW(L"Link Established Error:%x!", msg->EstLink_Evt.status);
		return;
	}
	int i;
	for (i = 0; i < MAX_CON_DEV; i++) {
		if ((theApp.m_state[i].enable == NPI_FALSE)
		    && (theApp.m_state[i].conFlag == FALSE)) {
			theApp.m_state[i].conHdl = msg->EstLink_Evt.connectionHandle;
			theApp.m_state[i].enable = NPI_TRUE;
			theApp.m_state[i].conFlag = NPI_TRUE;
			memcpy_s(theApp.m_state[i].bdaddr, BLE_ADDR_LEN, msg->EstLink_Evt.devAddr,
			         BLE_ADDR_LEN);
			//update connect handle
			swprintf_s(hdlBuf, 10, _T("0x%04X"), msg->EstLink_Evt.connectionHandle);

			if (i <= theApp.m_cmdView->m_conHdl.GetCount()) {
				theApp.m_cmdView->m_conHdl.DeleteString(i);
			}
			theApp.m_cmdView->m_conHdl.AddString(hdlBuf);
			//update state
			theApp.m_cmdView->m_conState.SetWindowText(_T("Connected"));
			//update bd address
			memcpy_s(temp, 10, theApp.m_state[i].bdaddr, BLE_ADDR_LEN);
			swprintf_s(slvAdr, 14, _T("%llX"), *(long long*)temp);
			theApp.m_cmdView->m_conBDA.SetWindowText(slvAdr);
			break;
		}
	}
	theApp.m_cmdView->m_conHdl.SetCurSel(theApp.m_cmdView->m_conHdl.GetCount() - 1);
}

static void linkTerminated(const PUINT8 pdata, UINT16 len)
{
	u_GapMsg* msg = (u_GapMsg*)pdata;
	if (msg->TerLink_Evt.status != BLE_SUCCESS) {
		::LogW(L"Link Terminate Error:%x!", msg->TerLink_Evt.status);
		return;
	}
	int i;
	for (i = 0; i < MAX_CON_DEV; i++) {
		if (theApp.m_state[i].conHdl == msg->TerLink_Evt.connectionHandle) {
			theApp.m_state[i].enable = NPI_FALSE;
			theApp.m_state[i].conFlag = NPI_FALSE;
			theApp.m_cmdView->m_conState.SetWindowText(_T("Disconnected"));
			//clear bd address
			theApp.m_cmdView->m_conBDA.SetWindowText(_T("None"));
		}
	}
}

static void authComplete(const PUINT8 pdata, UINT16 len)
{
	u_GapMsg* msg = (u_GapMsg*)pdata;
	wchar_t wBuf[128] = { 0 };
	//Long Term Key
	hex2wstr(wBuf, msg->AuthComp_Evt.pDevSecInfo.ltk, LTK_LEN);
	theApp.m_cmdView->m_page3.m_ltkEdit.SetWindowText(wBuf);
	//LTK Diversifier
	wsprintf(wBuf, _T("%4X"), msg->AuthComp_Evt.pDevSecInfo.div);
	theApp.m_cmdView->m_page3.m_ltkDiv.SetWindowText(wBuf);
	//LTK Random
	hex2wstr(wBuf, msg->AuthComp_Evt.pDevSecInfo.rand, B_RANDOM_NUM_SIZE);
	theApp.m_cmdView->m_page3.m_ltkRand.SetWindowText(wBuf);
}

/************************
/*ATT CALLBACK FUNCTION
/***********************/
static void attReadRsp(const PUINT8 pdata, UINT16 len)
{
	u_AttMsg* msg = (u_AttMsg*)pdata;
	wchar_t temp[512];
	wchar_t hdlBuf[20];
	long hdlInList, hdlInCmd;
	int i, pos = 0;

	if (msg->readRsp.status != BLE_SUCCESS) {
		::LogW(L"Att Resp Error:%x!", msg->readRsp.status);
		return;
	}
	int nIndex = theApp.m_gattView->m_list.GetItemCount();
	theApp.m_cmdView->m_page2.m_crCharValHdl.GetWindowText(hdlBuf, 20);
	hdlInCmd = wcstol(hdlBuf, NULL, 16);
	for (i = 0; i < nIndex; i++) {
		theApp.m_gattView->m_list.GetItemText(i, 1, temp, 20);
		hdlInList = wcstol(temp, NULL, 16);
		if (hdlInCmd < hdlInList) {
			break;
		}
		if (hdlInCmd == hdlInList) {
			//update value
			if (msg->readRsp.pduLen > 0) {
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
	theApp.m_gattView->m_list.InsertItem(i, itemAdd);
	//add handle
	theApp.m_gattView->m_list.SetItemText(i, 1, hdlBuf);
	//add value
	if (msg->readRsp.pduLen > 0) {
		hex2wstr(temp, msg->readRsp.value, msg->readRsp.pduLen);
		theApp.m_gattView->m_list.SetItemText(i, 4, temp);
		theApp.m_cmdView->m_page2.m_crRadio = 2;
		theApp.m_cmdView->m_page2.m_crValStr = temp;
		theApp.m_cmdView->m_page2.m_crValEdit.SetWindowText(temp);
	}
}