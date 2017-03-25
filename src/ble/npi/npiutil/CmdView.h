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


