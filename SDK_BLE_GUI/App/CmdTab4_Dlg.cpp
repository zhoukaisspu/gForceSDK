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
}


BEGIN_MESSAGE_MAP(CmdTab4_Dlg, CDialogEx)
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
	m_cmdTree.InsertItem(_T("GAP_EndDiscoverable"),	h_gap);
	m_cmdTree.InsertItem(_T("GAP_UpdateAdvertisingData"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_Authenticate"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_PasskeyUpdate"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_SlaveSecurityRequest"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_Signable"), h_gap);
	m_cmdTree.InsertItem(_T("GAP_Bond"), h_gap);
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
	m_cmdTree.InsertItem(_T("GATT_ExchangeMTU"), h_gatt);
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

	return TRUE;  
}
