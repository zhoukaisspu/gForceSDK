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
#include "afxcmn.h"


// CmdTab1_Dlg dialog

class CmdTab1_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmdTab1_Dlg)

public:
	CmdTab1_Dlg(CWnd* pParent = NULL);
	virtual ~CmdTab1_Dlg();

// dialog data
	enum { IDD = IDD_TAB1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//Discovery
	CButton m_discActiveScan;
	CButton m_discCancel;
	CComboBox m_discMode;
	CButton m_discScan;
	CButton m_discWhiteList;
	CStatic m_discDevFound;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedDiscScan();
	afx_msg void OnBnClickedDiscCancel();
	//connection settings
	CEdit m_csMinIntEdit;
	CStatic m_csMinIntStatic;
	afx_msg void OnChangeConsetMinint();
	CSpinButtonCtrl m_csMinIntSpin;
	CEdit m_csMaxIntEdit;
	CSpinButtonCtrl m_csMaxIntSpin;
	CStatic m_csMaxIntStatic;
	afx_msg void OnChangeConsetMaxint();
	CEdit m_csSlvLatEdit;
	CSpinButtonCtrl m_csSlvLatSpin;
	afx_msg void OnChangeConsetSlaLat();
	CEdit m_csSupToEdit;
	CSpinButtonCtrl m_csSupToSpin;
	CStatic m_csSupToStatic;
	afx_msg void OnChangeConsetSupto();
	afx_msg void OnBnClickedCsButLoad();
	afx_msg void OnBnClickedIdcCsButGet();
	afx_msg void OnBnClickedCsButSet();
	CComboBox m_esAdTypeCbx;
	CComboBox m_esSlvBDACbx;
	CButton m_esWLChbx;
	afx_msg void OnBnClickedEsButEstab();
//	afx_msg void OnBnClickedEsButCancel();
//	CEdit m_tlHandleEdit;
	afx_msg void OnBnClickedTlButTermin();

//	CStatic m_tlConStStatic;
//	CStatic m_tlConBDA;
};
