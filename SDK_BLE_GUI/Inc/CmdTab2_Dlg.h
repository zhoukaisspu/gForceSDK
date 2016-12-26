#pragma once


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
};
