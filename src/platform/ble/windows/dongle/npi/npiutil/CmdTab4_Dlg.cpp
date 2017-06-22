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
// CmdTab4_Dlg.cpp
//

#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "CmdTab4_Dlg.h"
#include "afxdialogex.h"


// CmdTab4_Dlg dialog

IMPLEMENT_DYNAMIC(CmdTab4_Dlg, CDialogEx)

CmdTab4_Dlg::CmdTab4_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmdTab4_Dlg::IDD, pParent)
{

}

CmdTab4_Dlg::~CmdTab4_Dlg()
{
}

void CmdTab4_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB4_TREE, m_cmdTree);
	DDX_Control(pDX, IDC_TAB4_PROPERTYGRID, m_gridCtrl);
}


BEGIN_MESSAGE_MAP(CmdTab4_Dlg, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TAB4_TREE, &CmdTab4_Dlg::OnSelchangedTab4Tree)
//	ON_NOTIFY(NM_CLICK, IDC_TAB4_TREE, &CmdTab4_Dlg::OnClickTab4Tree)
	ON_NOTIFY(NM_DBLCLK, IDC_TAB4_TREE, &CmdTab4_Dlg::OnDblclkTab4Tree)
END_MESSAGE_MAP()


// CmdTab4_Dlg message handle


BOOL CmdTab4_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// init command tree
	HTREEITEM h_gap = m_cmdTree.InsertItem(_T("GAP"));
	m_cmdTree.InsertItem(_T("GAP_DeviceInit"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_ConfigDeviceAddr"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_MakeDiscoverable"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_EndDiscoverable"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_UpdateAdvertisingData"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_PasskeyUpdate"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_SlaveSecurityRequest"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_Signable"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_TerminateAuth"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_UpdateLinkParamReq"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_SetParam"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_GetParam"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_ResolvePrivateAddr"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_SetAdvToken"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_RemoveAdvToken"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_UpdateAdvTokens"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_UpdateAdvTokens"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_Bond"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_BondSetParam"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_BondGetParam"), h_gap);

	HTREEITEM h_gatt = m_cmdTree.InsertItem(_T("GATT"));
	m_cmdTree.InsertItem(_T("GATT_DiscAllPrimaryServices"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_DiscPrimaryServiceByUUID"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_FindIncludedServices"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_DiscAllChar"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_DiscCharByUUID"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_DiscAllCharDesc"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_ReadCharVal"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_ReadUseCharUUID"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_ReadLongCharValue"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_ReadMultiCharValues"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_WriteCharValue"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_WriteLongCharValue"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_ReliableWrites"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_ReadCharDesc"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_ReadLongCharDesc"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_WriteCharDesc"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_WriteLongCharDesc"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_AddService"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_DelService"), h_gatt);
	m_cmdTree.InsertItem(_T("GATT_AddAttribute"), h_gatt);

	HDITEM item;
	item.cxy = 200;
	item.mask = HDI_WIDTH;
	m_gridCtrl.GetHeaderCtrl().SetItem(0, new HDITEM(item));

	return TRUE;
}

void CmdTab4_Dlg::OnSelchangedTab4Tree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM item = m_cmdTree.GetSelectedItem();
	CString text = m_cmdTree.GetItemText(item);

	m_gridCtrl.RemoveAll();

	if (text == _T("GAP_DeviceInit")) {
		this->Show_GAP_DeviceInit();
	}
	if (text == _T("GAP_ConfigDeviceAddr")) {
		this->Show_GAP_ConfigDeviceAddr();
	}
	if (text == _T("GAP_ConfigDeviceAddr")) {
		this->Show_GAP_ConfigDeviceAddr();
	}
	if (text == _T("GAP_Bond")) {
		this->Show_GAP_Bond();
	}
	*pResult = 0;
}

//void CmdTab4_Dlg::OnClickTab4Tree(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	*pResult = 0;
//}

void CmdTab4_Dlg::OnDblclkTab4Tree(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM item = m_cmdTree.GetSelectedItem();
	CString text = m_cmdTree.GetItemText(item);

	gapRole_t role;
	UINT8 maxScan = 0;
	UINT8 irk[16] = { 0 };
	UINT8 csrk[16] = { 0 };
	UINT8 signCnt[4] = { 0 };
	CString str;
	WCHAR buf[20] = { 0 };
	UINT16 conHandle;
	if (theApp.m_cmdHandle == NULL){
		return;
	}
	//get connection handle
	theApp.m_cmdView->m_conHdl.GetWindowText(buf, 10);
	conHandle = (UINT16)wcstol(buf, NULL, 16);

	if (text == _T("GAP_DeviceInit")) {
		role.data = 0;
		CMFCPropertyGridProperty* pProp0 = m_gridCtrl.GetProperty(0);
		str = pProp0->GetValue();
		if (str == _T("Enable")) {
			role.role.broadcast = 1;
		}
		CMFCPropertyGridProperty* pProp1 = m_gridCtrl.GetProperty(1);
		str = pProp1->GetValue();
		if (str == _T("Enable")) {
			role.role.observer = 1;
		}
		CMFCPropertyGridProperty* pProp2 = m_gridCtrl.GetProperty(2);
		str = pProp2->GetValue();
		if (str == _T("Enable")) {
			role.role.peripheral = 1;
		}
		CMFCPropertyGridProperty* pProp3 = m_gridCtrl.GetProperty(3);
		str = pProp3->GetValue();
		if (str == _T("Enable")) {
			role.role.central = 1;
		}
		CMFCPropertyGridProperty* pProp4 = m_gridCtrl.GetProperty(4);
		str = pProp3->GetValue();
		//maxScan = (UINT8)atoi(str);
		//maxScan = wcstod(str);
		theApp.m_cmdHandle->GAP_DeviceInit(role, maxScan, irk, csrk, signCnt);
	}
	if (text == _T("GAP_ConfigDeviceAddr")) {

	}
	if (text == _T("GAP_ConfigDeviceAddr")) {

	}
	if (text == _T("GAP_Bond")){
		CMFCPropertyGridProperty* pProp0 = m_gridCtrl.GetProperty(0);
		str = pProp0->GetValue();
		eEnDisMode auth = NPI_ENABLE;
		if (str == _T("No")) {
			auth = NPI_DISABLE;
		}
		/*Get LTK*/
		CMFCPropertyGridProperty* pProp1 = m_gridCtrl.GetProperty(1);
		str = pProp1->GetValue();
		UINT8 ltk[128] = { 0 };
		size_t LtkLen = wstr2hex(str, (PUINT8)ltk, 128, _T("FF:"));
		/*Get DIV*/
		CMFCPropertyGridProperty* pProp2 = m_gridCtrl.GetProperty(2);
		str = pProp2->GetValue();
		LONG div = wcstol(str, NULL, 16);
		/*Get RAND*/
		CMFCPropertyGridProperty* pProp3 = m_gridCtrl.GetProperty(3);
		str = pProp3->GetValue();
		UINT8 rand[128] = { 0 };
		wstr2hex(str, (PUINT8)rand, 128, _T("FF:"));
		theApp.m_cmdHandle->GAP_Bond(conHandle, auth, ltk, (UINT16)div, rand, LtkLen);
	}
	*pResult = 0;
}

void CmdTab4_Dlg::Show_GAP_DeviceInit(void)
{
	CMFCPropertyGridProperty* pProp1 = new CMFCPropertyGridProperty(
	        _T("broadcastProfileRole"),
	        _T("Disable"),
	        _T("Broadcast Profile Role(1 Bit)-Bit mask - GAP Profile role."));
	pProp1->AddOption(_T("Enable"));
	pProp1->AllowEdit(FALSE);
	m_gridCtrl.AddProperty(pProp1);

	CMFCPropertyGridProperty* pProp2 = new CMFCPropertyGridProperty(
	        _T("observerProfileRole"),
	        _T("Disable"),
	        _T("Observer Profile Role(1 Bit)-Bit mask - GAP Profile role."));
	pProp2->AddOption(_T("Enable"));
	pProp2->AllowEdit(FALSE);
	m_gridCtrl.AddProperty(pProp2);

	CMFCPropertyGridProperty* pProp3 = new CMFCPropertyGridProperty(
	        _T("peripheralProfileRole"),
	        _T("Disable"),
	        _T("Peripheral Profile Role(1 Bit)-Bit mask - GAP Profile role."));
	pProp3->AddOption(_T("Enable"));
	pProp3->AllowEdit(FALSE);
	m_gridCtrl.AddProperty(pProp3);

	CMFCPropertyGridProperty* pProp4 = new CMFCPropertyGridProperty(
	        _T("centralProfileRole"),
	        _T("Disable"),
	        _T("Central Profile Role(1 Bit)-Bit mask - GAP Profile role."));
	pProp4->AddOption(_T("Enable"));
	pProp4->AllowEdit(FALSE);
	m_gridCtrl.AddProperty(pProp4);

	CMFCPropertyGridProperty* pProp5 = new CMFCPropertyGridProperty(
	        _T("maxScanResponses"),
	        _T("5"),
	        _T("(1 Byte) - The maximun responses we can receive during a device discovery."));
	m_gridCtrl.AddProperty(pProp5);

	CMFCPropertyGridProperty* pProp6 = new CMFCPropertyGridProperty(
	        _T("irk"),
	        _T("00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00"),
	        _T("(16 Bytes) - Identify Resolving Key -0 if generate the key."));
	m_gridCtrl.AddProperty(pProp6);

	CMFCPropertyGridProperty* pProp7 = new CMFCPropertyGridProperty(
	        _T("csrk"),
	        _T("00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00"),
	        _T("(16 Bytes) - Connection Signature Resolving Key -0 if generate the key."));
	m_gridCtrl.AddProperty(pProp7);

	CMFCPropertyGridProperty* pProp8 = new CMFCPropertyGridProperty(
	        _T("signCounter"),
	        _T("1"),
	        _T("(4 Bytes) - 32bit signature counter."));
	m_gridCtrl.AddProperty(pProp8);

}

void CmdTab4_Dlg::Show_GAP_ConfigDeviceAddr(void)
{
	CMFCPropertyGridProperty* pProp1 = new CMFCPropertyGridProperty(
	        _T("addrType"),
	        _T("Pubic"),
	        _T("(1 Byte)-Address Type."));
	pProp1->AddOption(_T("Static"));
	pProp1->AddOption(_T("PrivateNonResolve"));
	pProp1->AddOption(_T("PrivateResolve"));
	pProp1->AllowEdit(FALSE);
	m_gridCtrl.AddProperty(pProp1);

	CMFCPropertyGridProperty* pProp2 = new CMFCPropertyGridProperty(
	        _T("addr"),
	        _T("00:00:00:00:00:00"),
	        _T("(6 Bytes)-BDA of the intended address."));
	m_gridCtrl.AddProperty(pProp2);

}

void CmdTab4_Dlg::Show_GAP_Bond(void)
{
	CMFCPropertyGridProperty* pProp1 = new CMFCPropertyGridProperty(
		_T("authenticated"),
		_T("No"),
		_T("(1 Byte)-Yes if bond was authenticated."));
	pProp1->AddOption(_T("Yes"));
	pProp1->AllowEdit(FALSE);
	m_gridCtrl.AddProperty(pProp1);

	CMFCPropertyGridProperty* pProp2 = new CMFCPropertyGridProperty(
		_T("secInfo_LTK"),
		_T("00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00"),
		_T("(16 Bytes)-Long Term Key."));
	m_gridCtrl.AddProperty(pProp2);

	CMFCPropertyGridProperty* pProp3 = new CMFCPropertyGridProperty(
		_T("secInfo_DIV"),
		_T("0"),
		_T("(2 Bytes)-Diversifier."));
	m_gridCtrl.AddProperty(pProp3);

	CMFCPropertyGridProperty* pProp4 = new CMFCPropertyGridProperty(
		_T("secInfo_RAND"),
		_T("11:22:33:44:55:66:77:88"),
		_T("(8 Bytes)-LTK Random Paring."));
	m_gridCtrl.AddProperty(pProp4);
}