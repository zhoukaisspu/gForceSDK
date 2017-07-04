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
#include "afxcmn.h"
#include "afxpropertygridctrl.h"


// CmdTab4_Dlg dialog

class CmdTab4_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmdTab4_Dlg)

public:
	CmdTab4_Dlg(CWnd* pParent = NULL);
	virtual ~CmdTab4_Dlg();

// dialog data
	enum { IDD = IDD_TAB4_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_cmdTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTab4Tree(NMHDR* pNMHDR, LRESULT* pResult);
	CMFCPropertyGridCtrl m_gridCtrl;
//	afx_msg void OnClickTab4Tree(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void Show_GAP_DeviceInit(void);
	void Show_GAP_ConfigDeviceAddr(void);
	void Show_GAP_Bond(void);
public:
	afx_msg void OnDblclkTab4Tree(NMHDR* pNMHDR, LRESULT* pResult);
};
