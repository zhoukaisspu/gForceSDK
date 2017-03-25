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
