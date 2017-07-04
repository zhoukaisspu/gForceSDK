/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
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
// CmdTab2_Dlg.cpp
//

#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "CmdTab2_Dlg.h"
#include "afxdialogex.h"


// CmdTab2_Dlg dialog

IMPLEMENT_DYNAMIC(CmdTab2_Dlg, CDialogEx)

CmdTab2_Dlg::CmdTab2_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmdTab2_Dlg::IDD, pParent)
	, m_crRadio(0)
	, m_cwRadio(0)
{

}

CmdTab2_Dlg::~CmdTab2_Dlg()
{
}

void CmdTab2_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2_CR_SP, m_crSubCBX);
	DDX_Control(pDX, IDC_TAB2_CR_CVH, m_crCharValHdl);
	DDX_Control(pDX, IDC_TAB2_CR_SH, m_crStaHdl);
	DDX_Control(pDX, IDC_TAB2_CR_CU, m_crCharUUID);
	DDX_Control(pDX, IDC_TAB2_CR_EH, m_crEndHdl);
	DDX_Control(pDX, IDC_TAB2_CR_VALUE, m_crValEdit);
	DDX_Radio(pDX, IDC_TAB2_CR_ASCII, m_crRadio);
	DDX_Control(pDX, IDC_TAB2_CR_READ, m_crRdButton);
	DDX_Radio(pDX, IDC_TAB2_CW_ASCII, m_cwRadio);
	DDX_Control(pDX, IDC_TAB2_CW_CVH, m_cwCharValHdl);
	DDX_Control(pDX, IDC_TAB2_CW_VALUE, m_cwValEdit);
	DDX_Control(pDX, IDC_TAB2_CW_WRITE, m_cwWrButton);
	DDX_Control(pDX, IDC_TAB2_MTU_EDIT, m_mtuEdit);
}


BEGIN_MESSAGE_MAP(CmdTab2_Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_TAB2_CR_READ, &CmdTab2_Dlg::OnBnClickedTab2CrRead)
	ON_BN_CLICKED(IDC_TAB2_CR_ASCII, &CmdTab2_Dlg::OnBnClickedTab2CrAscii)
	ON_BN_CLICKED(IDC_TAB2_CR_DEC, &CmdTab2_Dlg::OnBnClickedTab2CrDec)
	ON_BN_CLICKED(IDC_TAB2_CR_HEX, &CmdTab2_Dlg::OnBnClickedTab2CrHex)
	ON_CBN_SELCHANGE(IDC_TAB2_CR_SP, &CmdTab2_Dlg::OnSelchangeTab2CrSpCBx)
	ON_EN_CHANGE(IDC_TAB2_CR_VALUE, &CmdTab2_Dlg::OnChangeTab2CrValue)
	ON_BN_CLICKED(IDC_TAB2_CW_WRITE, &CmdTab2_Dlg::OnBnClickedTab2CwWrite)
	ON_BN_CLICKED(IDC_TAB2_CW_ASCII, &CmdTab2_Dlg::OnBnClickedTab2CwAscii)
	ON_BN_CLICKED(IDC_TAB2_CW_DEC, &CmdTab2_Dlg::OnBnClickedTab2CwDec)
	ON_BN_CLICKED(IDC_TAB2_CW_HEX, &CmdTab2_Dlg::OnBnClickedTab2CwHex)
	ON_BN_CLICKED(IDC_TAB2_MTU_BUT, &CmdTab2_Dlg::OnBnClickedTab2MtuBut)
	ON_BN_CLICKED(IDC_TAB2_CR_SAVE, &CmdTab2_Dlg::OnBnClickedTab2CrSave)
END_MESSAGE_MAP()


// CmdTab2_Dlg message handle
BOOL CmdTab2_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_crSubCBX.SetCurSel(0);

	m_crCharValHdl.SetWindowText(_T("0x0001"));
	m_crCharUUID.EnableWindow(FALSE);
	m_crCharUUID.SetWindowText(_T("00:2A"));
	m_crStaHdl.EnableWindow(FALSE);
	m_crStaHdl.SetWindowText(_T("0x0001"));
	m_crEndHdl.EnableWindow(FALSE);
	m_crEndHdl.SetWindowText(_T("0xFFFE"));

	m_cwCharValHdl.SetWindowText(_T("0x0001"));
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CmdTab2_Dlg::OnSelchangeTab2CrSpCBx()
{
	switch (m_crSubCBX.GetCurSel()) {
	case 0:
	case 2:
		m_crCharValHdl.EnableWindow(TRUE);
		m_crCharUUID.EnableWindow(FALSE);
		m_crStaHdl.EnableWindow(FALSE);
		m_crEndHdl.EnableWindow(FALSE);
		break;
	case 1:
	case 3:
		m_crCharValHdl.EnableWindow(FALSE);
		m_crCharUUID.EnableWindow(TRUE);
		m_crStaHdl.EnableWindow(TRUE);
		m_crEndHdl.EnableWindow(TRUE);
		break;
	default:
		break;
	}
}


void CmdTab2_Dlg::OnBnClickedTab2CrRead()
{
	WCHAR buf[128];
	UINT16 conHdl;
	UINT16 startHdl;
	UINT16 endHdl;
	WCHAR charVal[128] = { 0 };
	size_t charValLen;
	UINT8 uuid[20] = { 0 };
	size_t uuidLen;

	if (theApp.m_cmdHandle) {
		theApp.m_cmdView->m_conHdl.GetWindowText(buf, 10);
		conHdl = (UINT16)wcstol(buf, NULL, 16); //get connection handle
		//get char value handle
		m_crCharValHdl.GetWindowText(buf, 128);
		charValLen = wstr2hex(buf, (PUINT8)charVal, 128, _T("0xFFFF;"));
		//get start handle
		m_crStaHdl.GetWindowText(buf, 10);
		startHdl = (UINT16)wcstol(buf, NULL, 16);
		//get end handle
		m_crEndHdl.GetWindowText(buf, 10);
		endHdl = (UINT16)wcstol(buf, NULL, 16);
		//get uuid
		m_crCharUUID.GetWindowText(buf, 128);
		uuidLen = wstr2hex(buf, (PUINT8)uuid, 20, _T("FF:"));

		switch (m_crSubCBX.GetCurSel()) {
		case 0:
			if (charValLen != 1) {
				AfxMessageBox(_T("Format Error!\nThe format is 0x0000!"));
			} else {
				theApp.m_cmdHandle->GATT_ReadCharVal(conHdl, charVal[0]);
			}
			break;
		case 1:
			if ((uuidLen != ATT_BT_UUID_SIZE) && (uuidLen != ATT_UUID_SIZE)) {
				AfxMessageBox(
				        _T("Format Error!\nThe format is 00:00,and length is either 2 or 16!"));
			} else {
				theApp.m_cmdHandle->GATT_ReadUseCharUUID(conHdl, startHdl, endHdl, uuid,
				                uuidLen);
			}
			break;
		case 2:
			if (charValLen == 0) {
				AfxMessageBox(_T("Format Error!\nThe format is 0x0000;0x0000!"));
			} else {
				theApp.m_cmdHandle->GATT_ReadMultiCharValues(conHdl, (PUINT8)charVal,
				                charValLen * 2);
			}
			break;
		case 3:
			if ((uuidLen != ATT_BT_UUID_SIZE) && (uuidLen != ATT_UUID_SIZE)) {
				AfxMessageBox(
				        _T("Format Error!\nThe format is 00:00,and length is either 2 or 16!"));
			} else {
				theApp.m_cmdHandle->GATT_DiscCharByUUID(conHdl, startHdl, endHdl, uuid,
				                                        uuidLen);
			}
			break;
		default:
			break;
		}
	}
}

void CmdTab2_Dlg::OnBnClickedTab2CrAscii()
{
	UINT8 dst[256] = { 0 };
	m_crRadio = 0;
	wstr2hex(m_crValStr.GetString(), dst, 256, L"FF:");
	string str_a = (char*)dst;
	m_crValEdit.SetWindowText(ANSIToUnicode(str_a).c_str());
}


void CmdTab2_Dlg::OnBnClickedTab2CrDec()
{
	wchar_t src[512] = {0};
	UINT8 dst[256] = {0};
	char** out = NULL;
	size_t len;
	int lastSel = m_crRadio;
	len = wstr2hex(m_crValStr.GetString(), dst, 256, L"FF:");

	if ((lastSel == 2) && (len <= sizeof(LONG))) {
		ULONG x = dst[0] + (dst[1] << 8) + (dst[2] << 16) + (dst[3] << 24);
		wsprintf(src, L"%d", x);
		m_crRadio = 1;
		m_crValEdit.SetWindowText(src);
	} else {
		AfxMessageBox(L"Cann't convert to decimal!");
		this->UpdateData(FALSE);
	}
}


void CmdTab2_Dlg::OnBnClickedTab2CrHex()
{
	m_crRadio = 2;
	m_crValEdit.SetWindowText(m_crValStr.GetString());
}

void CmdTab2_Dlg::OnChangeTab2CrValue()
{
	this->UpdateData(FALSE);

}

void CmdTab2_Dlg::OnBnClickedTab2CwWrite()
{
	WCHAR wBuf[256] = { 0 };
	UINT8 buf[256] = { 0 };
	WCHAR charVal[128] = { 0 };
	size_t charValLen;
	size_t dataLen;
	size_t i = 0;
	wstring str_w;
	string str_a;
	LONG data_l;
	if (theApp.m_cmdHandle) {
		theApp.m_cmdView->m_conHdl.GetWindowText(wBuf, 256);
		UINT16 conHdl = (UINT16)wcstol(wBuf, NULL, 16); //get connection handle
		//get char value handle
		m_cwCharValHdl.GetWindowText(wBuf, sizeof(wBuf));
		charValLen = wstr2hex(wBuf, (PUINT8)charVal, 128, _T("0xFFFF;"));

		if (charValLen != 1) {
			AfxMessageBox(_T("Char Value Handle Format Error!\nThe format is 0x0000!"));
		} else {
			//get char value
			m_cwValEdit.GetWindowText(wBuf, sizeof(buf));
			str_w = wBuf;
			str_a = UnicodeToANSI(str_w);
			switch (m_cwRadio) {
			case 0:
				theApp.m_cmdHandle->GATT_WriteCharValue(conHdl, charVal[0],
				                                        (PUINT8)str_a.c_str(), str_a.length());
				break;
			case 1:
				data_l = wcstol(wBuf, NULL, 10);
				while (data_l) {
					buf[i++] = UINT8(data_l & 0x000000FF);
					data_l = (data_l >> 8);
				}
				theApp.m_cmdHandle->GATT_WriteCharValue(conHdl, charVal[0], buf, i);
				break;
			case 2:
				dataLen = wstr2hex(wBuf, buf, 256, L"FF:");
				if (!dataLen) {
					AfxMessageBox(_T("Format Error!\nThe format is 00:00"));
				} else {
					theApp.m_cmdHandle->GATT_WriteCharValue(conHdl, charVal[0], buf, dataLen);
				}
				break;
			default:
				break;
			}
		}

	}
}


void CmdTab2_Dlg::OnBnClickedTab2CwAscii()
{
	wchar_t src[512] = { 0 };
	UINT8 dst[256] = { 0 };
	m_cwRadio = 0;
	m_cwValEdit.GetWindowText(src, 512);
	wstr2hex(src, dst, 256, L"FF:");
	string str_a = (char*)dst;
	m_cwValEdit.SetWindowText(ANSIToUnicode(str_a).c_str());
}


void CmdTab2_Dlg::OnBnClickedTab2CwDec()
{
	wchar_t src[512] = { 0 };
	UINT8 dst[256] = { 0 };
	char** out = NULL;
	size_t len;
	int lastSel = m_cwRadio;
	m_cwValEdit.GetWindowText(src, 512);
	len = wstr2hex(src, dst, 256, L"FF:");

	if ((lastSel == 2) && (len <= sizeof(LONG))) {
		ULONG x = dst[0] + (dst[1] << 8) + (dst[2] << 16) + (dst[3] << 24);
		wsprintf(src, L"%d", x);
		m_cwRadio = 1;
		m_cwValEdit.SetWindowText(src);
	} else {
		AfxMessageBox(L"Cann't convert to decimal!");
		this->UpdateData(FALSE);
	}
}


void CmdTab2_Dlg::OnBnClickedTab2CwHex()
{
	wchar_t src[512] = { 0 };
	wchar_t temp[512] = { 0 };
	int i = 0;
	UINT8 dst[256] = { 0 };
	m_cwValEdit.GetWindowText(src, 512);
	int lastSel = m_cwRadio;
	m_cwRadio = 2;
	if (lastSel == 1) {
		//dec to hex
		long x = wcstol(src, NULL, 10);
		while (x) {
			dst[i++] = UINT8(x & 0x000000FF);
			x = (x >> 8);
		}
		hex2wstr(src, dst, i);
		m_cwValEdit.SetWindowText(src);
	} else {
		//ascii to hex
		wstring str_w = src;
		string str_a = UnicodeToANSI(str_w);
		hex2wstr(temp, (PUINT8)str_a.c_str(), str_a.length());
		m_cwValEdit.SetWindowText(temp);
	}
}


void CmdTab2_Dlg::OnBnClickedTab2MtuBut()
{
	UINT16 mtu;
	UINT16 conHdl;
	WCHAR wBuf[256] = {0};

	if (theApp.m_cmdHandle) {
		//get connection handle
		theApp.m_cmdView->m_conHdl.GetWindowText(wBuf, 256);
		conHdl = (UINT16)wcstol(wBuf, NULL, 16);
		//get mtu size
		m_mtuEdit.GetWindowText(wBuf, 256);
		mtu = (UINT16)wcstol(wBuf, NULL, 10);

		theApp.m_cmdHandle->GATT_ExchangeMTU(conHdl, mtu);
	}
}

void CmdTab2_Dlg::OnBnClickedTab2CrSave()
{
	BOOL isOpen = FALSE;        //SAVE
	WCHAR wBuf[256] = { 0 };
	CString defaultDir = _T("D:\\");
	CString fileName = _T("char.txt");
	CString filter = _T("File (*.txt)");
	CFileDialog openFileDlg(isOpen, defaultDir, fileName,
	                        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"D:\\char.txt";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath;
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
		CFile file;
		if (file.Open(filePath, CFile::modeCreate || CFile::modeReadWrite)) {
			string str_a = UnicodeToANSI(m_crValStr.GetString());
			file.Write(str_a.c_str(), m_crValStr.GetLength());
			file.Close();
		}
	}
}
