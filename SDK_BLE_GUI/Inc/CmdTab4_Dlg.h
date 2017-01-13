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
public:
	afx_msg void OnDblclkTab4Tree(NMHDR *pNMHDR, LRESULT *pResult);
};
