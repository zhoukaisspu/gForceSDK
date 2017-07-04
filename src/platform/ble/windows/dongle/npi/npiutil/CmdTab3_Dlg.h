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


// CmdTab3_Dlg dialog

class CmdTab3_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmdTab3_Dlg)

public:
	CmdTab3_Dlg(CWnd* pParent = NULL);
	virtual ~CmdTab3_Dlg();

// dialog data
	enum { IDD = IDD_TAB3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CStatic m_staConHdl;
	CButton m_ipBondEn;
	CButton m_ipAuthEn;
	CButton m_ipSendPair;
	CEdit m_piPassKey;
	CButton m_piSendPK;
	CButton m_ltkAuthBond;
	CEdit m_ltkDiv;
	CEdit m_ltkRand;
	CEdit m_ltkEdit;
	CButton m_ltkEncLink;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTab3SendPairReq();
	afx_msg void OnBnClickedTab3SendPassKey();
	afx_msg void OnBnClickedTab3EncLink();
};
