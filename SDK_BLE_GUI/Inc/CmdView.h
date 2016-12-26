#pragma once
#include "afxcmn.h"
#include "CmdTab1_Dlg.h"
#include "CmdTab2_Dlg.h"
#include "CmdTab3_Dlg.h"
#include "CmdTab4_Dlg.h"

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
	CmdTab1_Dlg m_discPage;
	CmdTab2_Dlg m_rwPage;
	CmdTab3_Dlg m_pairPage;
	CmdTab4_Dlg m_cmdPage;
	int m_tabCurSel;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTabCmd(NMHDR *pNMHDR, LRESULT *pResult);
};


