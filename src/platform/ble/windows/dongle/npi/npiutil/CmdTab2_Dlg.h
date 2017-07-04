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
#pragma once
#include "afxwin.h"


// CmdTab2_Dlg dialog

class CmdTab2_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmdTab2_Dlg)

public:
	CmdTab2_Dlg(CWnd* pParent = NULL);   // construct
	virtual ~CmdTab2_Dlg();

// dialog data
	enum { IDD = IDD_TAB2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_crSubCBX;
	virtual BOOL OnInitDialog();
	CEdit m_crCharValHdl;
	CEdit m_crStaHdl;
	CEdit m_crEndHdl;
	CEdit m_crCharUUID;
	CEdit m_crValEdit;
//	CStatic m_crHdlEdit;
	afx_msg void OnBnClickedTab2CrRead();
	afx_msg void OnBnClickedTab2CrAscii();
	afx_msg void OnBnClickedTab2CrDec();
	afx_msg void OnBnClickedTab2CrHex();
	afx_msg void OnSelchangeTab2CrSpCBx();
	int m_crRadio;
	CButton m_crRdButton;
	afx_msg void OnChangeTab2CrValue();
	int m_cwRadio;
//	CStatic m_cwHdlEdit;
	CEdit m_cwCharValHdl;
	CEdit m_cwValEdit;
	CButton m_cwWrButton;
	afx_msg void OnBnClickedTab2CwWrite();
	afx_msg void OnBnClickedTab2CwAscii();
	afx_msg void OnBnClickedTab2CwDec();
	afx_msg void OnBnClickedTab2CwHex();
	CString m_crValStr;
	CString m_cwValStr;
	CEdit m_mtuEdit;
	afx_msg void OnBnClickedTab2MtuBut();
	afx_msg void OnBnClickedTab2CrSave();
};
