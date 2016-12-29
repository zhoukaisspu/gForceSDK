#pragma once


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
};
