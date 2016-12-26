// CmdTab1_Dlg.cpp
//

#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "CmdTab1_Dlg.h"
#include "afxdialogex.h"


// CmdTab1_Dlg dialog

IMPLEMENT_DYNAMIC(CmdTab1_Dlg, CDialogEx)

CmdTab1_Dlg::CmdTab1_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmdTab1_Dlg::IDD, pParent)
{

}

CmdTab1_Dlg::~CmdTab1_Dlg()
{
}

void CmdTab1_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//Discovery
	DDX_Control(pDX, IDC_DISC_AS, m_discActiveScan);
	DDX_Control(pDX, IDC_DISC_CANCEL, m_discCancel);
	DDX_Control(pDX, IDC_DISC_MODE, m_discMode);
	DDX_Control(pDX, IDC_DISC_SCAN, m_discScan);
	DDX_Control(pDX, IDC_DISC_WL, m_discWhiteList);
	DDX_Control(pDX, IDC_DISC_DEVFOUND, m_discDevFound);
	//Connection Settings
	DDX_Control(pDX, IDC_CONSET_MININT, m_csMinIntEdit);
	DDX_Control(pDX, IDC_CONSET_MININT_STATIC, m_csMinIntStatic);
	DDX_Control(pDX, IDC_MININT_SPIN, m_csMinIntSpin);
	DDX_Control(pDX, IDC_CONSET_MAXINT, m_csMaxIntEdit);
	DDX_Control(pDX, IDC_MAXINT_SPIN, m_csMaxIntSpin);
	DDX_Control(pDX, IDC_CONSET_MAXINT_STATIC, m_csMaxIntStatic);
	DDX_Control(pDX, IDC_CONSET_SLA_LAT, m_csSlvLatEdit);
	DDX_Control(pDX, IDC_SLA_LAT_SPIN, m_csSlvLatSpin);
	DDX_Control(pDX, IDC_CONSET_SUPTO, m_csSupToEdit);
	DDX_Control(pDX, IDC_SUPTO_SPIN, m_csSupToSpin);
	DDX_Control(pDX, IDC_CONSET_SUPTO_STATIC, m_csSupToStatic);
	//Establish Link
	DDX_Control(pDX, IDC_ES_COMBOX_ADTYPE, m_esAdTypeCbx);
	DDX_Control(pDX, IDC_ES_COMBOX_SLVBDA, m_esSlvBDACbx);
	DDX_Control(pDX, IDC_ES_CBOX_WL, m_esWLChbx);
	//Terminate Link
	DDX_Control(pDX, IDC_TL_HDL_EDIT, m_tlHandleEdit);
	DDX_Control(pDX, IDC_TL_CONSTATE, m_tlConStStatic);
	DDX_Control(pDX, IDC_TL_CON_BDA, m_tlConBDA);
}


BEGIN_MESSAGE_MAP(CmdTab1_Dlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DISC_SCAN, &CmdTab1_Dlg::OnBnClickedDiscScan)
	ON_BN_CLICKED(IDC_DISC_CANCEL, &CmdTab1_Dlg::OnBnClickedDiscCancel)
	ON_EN_CHANGE(IDC_CONSET_MININT, &CmdTab1_Dlg::OnChangeConsetMinint)
	ON_EN_CHANGE(IDC_CONSET_MAXINT, &CmdTab1_Dlg::OnChangeConsetMaxint)
	ON_EN_CHANGE(IDC_CONSET_SLA_LAT, &CmdTab1_Dlg::OnChangeConsetSlaLat)
	ON_EN_CHANGE(IDC_CONSET_SUPTO, &CmdTab1_Dlg::OnChangeConsetSupto)
	ON_BN_CLICKED(IDC_CS_BUT_LOAD, &CmdTab1_Dlg::OnBnClickedCsButLoad)
	ON_BN_CLICKED(IDC_CS_BUT_GET, &CmdTab1_Dlg::OnBnClickedIdcCsButGet)
	ON_BN_CLICKED(IDC_CS_BUT_SET, &CmdTab1_Dlg::OnBnClickedCsButSet)
	ON_BN_CLICKED(IDC_ES_BUT_ESTAB, &CmdTab1_Dlg::OnBnClickedEsButEstab)
	ON_BN_CLICKED(IDC_ES_BUT_CANCEL, &CmdTab1_Dlg::OnBnClickedEsButCancel)
	ON_BN_CLICKED(IDC_TL_BUT_TERMIN, &CmdTab1_Dlg::OnBnClickedTlButTermin)
END_MESSAGE_MAP()


// CmdTab1_Dlg message handle


BOOL CmdTab1_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_discMode.SetCurSel(3);
	m_csMinIntEdit.SetWindowText(_T("80"));
	m_csMinIntSpin.SetBuddy(&m_csMinIntEdit);
	m_csMinIntSpin.SetRange(6, 3200);
	m_csMaxIntEdit.SetWindowText(_T("80"));
	m_csMaxIntSpin.SetBuddy(&m_csMaxIntEdit);
	m_csMaxIntSpin.SetRange(6, 3200);
	m_csSlvLatEdit.SetWindowText(_T("0"));
	m_csSlvLatSpin.SetBuddy(&m_csSlvLatEdit);
	m_csSlvLatSpin.SetRange(0, 499);
	m_csSupToEdit.SetWindowText(_T("2000"));
	m_csSupToSpin.SetBuddy(&m_csSupToEdit);
	m_csSupToSpin.SetRange(10, 3200);

	m_esAdTypeCbx.SetCurSel(0);
	m_esSlvBDACbx.SetCurSel(0);

	m_tlHandleEdit.SetWindowText(_T("0xFFFE"));
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CmdTab1_Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}


void CmdTab1_Dlg::OnBnClickedDiscScan()
{
	eGapDiscMode discMode;
	eEnDisMode activeScan, whiteList;

	discMode = (eGapDiscMode)m_discMode.GetCurSel();
	m_discActiveScan.GetCheck() == BST_CHECKED ? \
		activeScan = NPI_ENABLE : activeScan = NPI_DISABLE;
	m_discWhiteList.GetCheck() == BST_CHECKED ? \
		whiteList = NPI_ENABLE : whiteList = NPI_DISABLE;

	if (theApp.m_cmdHandle){
		theApp.m_cmdHandle->GAP_DeviceDiscoveryRequest(discMode, activeScan, whiteList);
		m_discDevFound.SetWindowText(_T("0"));
		m_discScan.ModifyStyle(0, WS_DISABLED);
	}
}


void CmdTab1_Dlg::OnBnClickedDiscCancel()
{
	if (theApp.m_cmdHandle){
		theApp.m_cmdHandle->GAP_DeviceDiscoveryCancel();
	}
}


void CmdTab1_Dlg::OnChangeConsetMinint()
{
	CString cs;
	DOUBLE num;
	m_csMinIntEdit.GetWindowText(cs);
	num = (DOUBLE)StrToInt(cs);
	if (num < 6){
		num = 6;
		m_csMinIntEdit.SetWindowText(_T("6"));
	}
	else if (num > 3200){
		num = 3200;
		m_csMinIntEdit.SetWindowText(_T("3200"));
	}
	else{
	}
	num = num*1.25;
	wchar_t buf[20] = { 0 };
	swprintf_s(buf, 20, _T("("));
	swprintf_s(&buf[wcslen(buf)], 20 - wcslen(buf), _T("%.2f"), num);
	wcscat_s(&buf[0], 20 - wcslen(buf), _T("ms)"));
	m_csMinIntStatic.SetWindowText((LPCTSTR)buf);
}


void CmdTab1_Dlg::OnChangeConsetMaxint()
{
	CString cs;
	DOUBLE num;
	m_csMaxIntEdit.GetWindowText(cs);
	num = (DOUBLE)StrToInt(cs);
	if (num < 6){
		num = 6;
		m_csMaxIntEdit.SetWindowText(_T("6"));
	}
	else if (num > 3200){
		num = 3200;
		m_csMaxIntEdit.SetWindowText(_T("3200"));
	}
	else{
	}
	num = num*1.25;
	wchar_t buf[20] = { 0 };
	swprintf_s(buf, 20, _T("("));
	swprintf_s(&buf[wcslen(buf)], 20 - wcslen(buf), _T("%.2f"), num);
	wcscat_s(&buf[0], 20 - wcslen(buf), _T("ms)"));
	m_csMaxIntStatic.SetWindowText((LPCTSTR)buf);
}


void CmdTab1_Dlg::OnChangeConsetSlaLat()
{
	CString cs;
	DOUBLE num;
	m_csMaxIntEdit.GetWindowText(cs);
	num = (DOUBLE)StrToInt(cs);
	if (num < 0){
		num = 0;
		m_csMaxIntEdit.SetWindowText(_T("0"));
	}
	else if (num > 499){
		num = 499;
		m_csMaxIntEdit.SetWindowText(_T("499"));
	}
	else{
	}
}


void CmdTab1_Dlg::OnChangeConsetSupto()
{
	CString cs;
	DOUBLE num;
	m_csSupToEdit.GetWindowText(cs);
	num = (DOUBLE)StrToInt(cs);
	if (num < 10){
		num = 10;
		m_csSupToEdit.SetWindowText(_T("10"));
	}
	else if (num > 3200){
		num = 3200;
		m_csSupToEdit.SetWindowText(_T("3200"));
	}
	else{
	}
	num = num*1.25;
	wchar_t buf[20] = { 0 };
	swprintf_s(buf, 20, _T("("));
	swprintf_s(&buf[wcslen(buf)], 20 - wcslen(buf), _T("%.2f"), num);
	wcscat_s(&buf[0], 20 - wcslen(buf), _T("ms)"));
	
	m_csSupToStatic.SetWindowText((LPCTSTR)buf);
}


void CmdTab1_Dlg::OnBnClickedCsButLoad()
{
	m_csMinIntEdit.SetWindowText(_T("80"));
	m_csMaxIntEdit.SetWindowText(_T("80"));
	m_csSlvLatEdit.SetWindowText(_T("0"));
	m_csSupToEdit.SetWindowText(_T("2000"));
}


void CmdTab1_Dlg::OnBnClickedIdcCsButGet()
{
	if (theApp.m_cmdHandle){
		theApp.m_cmdHandle->GAP_GetParam(TGAP_CONN_EST_INT_MIN);
		theApp.m_cmdHandle->GAP_GetParam(TGAP_CONN_EST_INT_MAX);
		theApp.m_cmdHandle->GAP_GetParam(TGAP_CONN_EST_LATENCY);
		theApp.m_cmdHandle->GAP_GetParam(TGAP_CONN_EST_SUPERV_TIMEOUT);
	}
}


void CmdTab1_Dlg::OnBnClickedCsButSet()
{
	CString cs;
	UINT16 minInt, maxInt, slvLat, supTo;
	if (theApp.m_cmdHandle){
		m_csMinIntEdit.GetWindowText(cs);
		minInt = StrToInt(cs);
		m_csMaxIntEdit.GetWindowText(cs);
		maxInt = StrToInt(cs);
		m_csSlvLatEdit.GetWindowText(cs);
		slvLat = StrToInt(cs);
		m_csSupToEdit.GetWindowText(cs);
		supTo = StrToInt(cs);
		theApp.m_cmdHandle->GAP_SetParam(TGAP_CONN_EST_INT_MIN, minInt);
		theApp.m_cmdHandle->GAP_SetParam(TGAP_CONN_EST_INT_MAX, maxInt);
		theApp.m_cmdHandle->GAP_SetParam(TGAP_CONN_EST_LATENCY, slvLat);
		theApp.m_cmdHandle->GAP_SetParam(TGAP_CONN_EST_SUPERV_TIMEOUT, supTo);
	}
}


void CmdTab1_Dlg::OnBnClickedEsButEstab()
{
	CString addr_str;
	CStringA addr;
	int len;
	eEnDisMode whiteList;
	eGapAddrType addrType;
	LPCWSTR addr_w16;
	long long addr_ll;
	if (theApp.m_cmdHandle){
		//check white list
		m_esWLChbx.GetCheck() == BST_CHECKED ? \
			whiteList = NPI_ENABLE : whiteList = NPI_DISABLE;
		//check address type
		addrType = (eGapAddrType)m_esAdTypeCbx.GetCurSel();
		//check addr and convert to long int
		len = m_esSlvBDACbx.GetLBTextLen(m_esSlvBDACbx.GetCurSel());
		m_esSlvBDACbx.GetLBText(m_esSlvBDACbx.GetCurSel(), addr_str.GetBuffer(len));
		addr_str.ReleaseBuffer();
		addr_w16 = addr_str.GetString();
		addr_ll = wcstoll(addr_w16, NULL, 16);
		if (addr_ll){
			theApp.m_cmdHandle->GAP_EstablishLinkRequest(NPI_DISABLE, whiteList, addrType, (PUINT8)&addr_ll);
		}
		else{
			AfxMessageBox(_T("Slave BDA Error!"));
		}

	}
}


void CmdTab1_Dlg::OnBnClickedEsButCancel()
{
	WCHAR buf[10];
	UINT16 handle;
	if (theApp.m_cmdHandle){
		m_tlHandleEdit.GetWindowText(buf, 10);
		handle = (UINT16)wcstol(buf, NULL, 16);
		theApp.m_cmdHandle->GAP_TerminateLinkRequest(handle);
	}
}


void CmdTab1_Dlg::OnBnClickedTlButTermin()
{
	WCHAR buf[10];
	UINT16 handle;
	if (theApp.m_cmdHandle){
		m_tlHandleEdit.GetWindowText(buf, 10);
		handle = (UINT16)wcstol(buf, NULL, 16);
		theApp.m_cmdHandle->GAP_TerminateLinkRequest(handle);
	}
}
