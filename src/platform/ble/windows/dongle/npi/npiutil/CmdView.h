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
#include "CmdTab1_Dlg.h"
#include "CmdTab2_Dlg.h"
#include "CmdTab3_Dlg.h"
#include "CmdTab4_Dlg.h"
#include "afxwin.h"

// CCmdView FormView

class CCmdView : public CFormView
{
	DECLARE_DYNCREATE(CCmdView)

protected:
	CCmdView();
	virtual ~CCmdView();

public:
	enum { IDD = IDD_CMDVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CTabCtrl m_tab;
	CmdTab1_Dlg m_page1;
	CmdTab2_Dlg m_page2;
	CmdTab3_Dlg m_page3;
	CmdTab4_Dlg m_page4;
	int m_tabCurSel;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTabCmd(NMHDR* pNMHDR, LRESULT* pResult);
	CComboBox m_conHdl;
	afx_msg void OnBnClickedViewTerm();
	CEdit m_conState;
	CEdit m_conBDA;
	afx_msg void OnSelchangeViewConhdl();
};


