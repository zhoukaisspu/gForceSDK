#pragma once
#include "afxcmn.h"


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
};
