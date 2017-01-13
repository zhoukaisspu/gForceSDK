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