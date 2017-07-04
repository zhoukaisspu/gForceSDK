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
// CmdTab3_Dlg.cpp
//

#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "CmdTab3_Dlg.h"
#include "afxdialogex.h"


// CmdTab3_Dlg dialog

IMPLEMENT_DYNAMIC(CmdTab3_Dlg, CDialogEx)

CmdTab3_Dlg::CmdTab3_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmdTab3_Dlg::IDD, pParent)
{

}

CmdTab3_Dlg::~CmdTab3_Dlg()
{
}

void CmdTab3_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB3_IP_BE, m_ipBondEn);
	DDX_Control(pDX, IDC_TAB3_IP_AE, m_ipAuthEn);
	DDX_Control(pDX, IDC_TAB3_IP_SPR, m_ipSendPair);
	DDX_Control(pDX, IDC_TAB3_PI_PK, m_piPassKey);
	DDX_Control(pDX, IDC_TAB3_PI_SPK, m_piSendPK);
	DDX_Control(pDX, IDC_TAB3_USLTK_AB, m_ltkAuthBond);
	DDX_Control(pDX, IDC_TAB3_USLTK_LD, m_ltkDiv);
	DDX_Control(pDX, IDC_TAB3_USLTK_LR, m_ltkRand);
	DDX_Control(pDX, IDC_TAB3_USLTK_LTK, m_ltkEdit);
	DDX_Control(pDX, IDC_TAB3_USLTK_EL, m_ltkEncLink);
}


BEGIN_MESSAGE_MAP(CmdTab3_Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_TAB3_IP_SPR, &CmdTab3_Dlg::OnBnClickedTab3SendPairReq)
	ON_BN_CLICKED(IDC_TAB3_PI_SPK, &CmdTab3_Dlg::OnBnClickedTab3SendPassKey)
	ON_BN_CLICKED(IDC_TAB3_USLTK_EL, &CmdTab3_Dlg::OnBnClickedTab3EncLink)
END_MESSAGE_MAP()


// CmdTab3_Dlg message handle
BOOL CmdTab3_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void CmdTab3_Dlg::OnBnClickedTab3SendPairReq()
{
	sGapAuth auth;
	UINT16 conHandle;
	WCHAR buf[128];
	if (theApp.m_cmdHandle) {
		memset(&auth, 0x00, sizeof(auth));
		//get connection handle
		theApp.m_cmdView->m_conHdl.GetWindowText(buf, 10);
		conHandle = (UINT16)wcstol(buf, NULL, 16);
		//Bond enabled
		if (m_ipBondEn.GetCheck() == BST_CHECKED) {
			auth.sec_authReq.req.bonding = NPI_TRUE;
		}
		//MITM enabled
		if (m_ipAuthEn.GetCheck() == BST_CHECKED) {
			auth.sec_authReq.req.mitm = NPI_TRUE;
		}
		auth.sec_ioCaps = KEYBOARD_DISPLAY;
		auth.sec_oobFlag = NPI_FALSE;
		auth.sec_maxEncKeySize = 0x10;
		auth.sec_keyDist.oper = 0x77;

		auth.pair_en = NPI_DISABLE;
		auth.pair_ioCaps = NO_INPUT_NO_OUTPUT;
		auth.pair_oobFlag = NPI_FALSE;
		auth.pair_authReq.req.bonding = NPI_TRUE;
		auth.pair_maxEncKeySize = 0x10;
		auth.pair_keyDist.oper = 0x77;

		theApp.m_cmdHandle->GAP_Authenticate(conHandle, &auth);
	}
}


void CmdTab3_Dlg::OnBnClickedTab3SendPassKey()
{

}


void CmdTab3_Dlg::OnBnClickedTab3EncLink()
{
	UINT16 conHdl;
	eEnDisMode authFlag = NPI_DISABLE;
	UINT8 ltk[16] = { 0 };
	UINT8 random[8] = { 0 };
	WCHAR wBuf[128];
	UINT16 div;
	if (theApp.m_cmdHandle) {
		//get connection handle
		theApp.m_cmdView->m_conHdl.GetWindowText(wBuf, 10);
		conHdl = (UINT16)wcstol(wBuf, NULL, 16);
		//get Auth Bond Flag
		if (m_ltkAuthBond.GetCheck() == BST_CHECKED) {
			authFlag = NPI_ENABLE;
		}
		//get ltk
		m_ltkEdit.GetWindowText(wBuf, 128);
		if (wstr2hex(wBuf, ltk, 16, _T("FF:"))) {
			//get div
			m_ltkDiv.GetWindowText(wBuf, 10);
			div = (UINT16)wcstol(wBuf, NULL, 16);
			//get random(8 bytes)
			if (wstr2hex(wBuf, random, 8, _T("FF:"))) {
				theApp.m_cmdHandle->GAP_Bond(conHdl, authFlag, ltk, div, random, 16);
			} else {
				AfxMessageBox(_T("Convert LTK Random Error!"));
			}
		} else {
			AfxMessageBox(_T("Convert LTK Error!"));
		}


	}
}

