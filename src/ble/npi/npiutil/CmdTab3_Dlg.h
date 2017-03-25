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
