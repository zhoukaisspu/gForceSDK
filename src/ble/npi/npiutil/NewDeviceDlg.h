#pragma once
#include "afxwin.h"


// NewDeviceDlg dialog

class NewDeviceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewDeviceDlg)

public:
	NewDeviceDlg(CWnd* pParent = NULL);
	virtual ~NewDeviceDlg();

// data
	enum { IDD = IDD_NEWDEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_port;
	CComboBox m_baud;
	CComboBox m_flow;
	CComboBox m_parity;
	CComboBox m_stopbit;
	CComboBox m_databit;
	afx_msg void OnBnClickedOk();
};
