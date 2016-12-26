#pragma once


// CmdTab3_Dlg 对话框

class CmdTab3_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmdTab3_Dlg)

public:
	CmdTab3_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CmdTab3_Dlg();

// 对话框数据
	enum { IDD = IDD_TAB3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
