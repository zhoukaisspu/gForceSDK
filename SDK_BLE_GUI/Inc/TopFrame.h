#pragma once


// CTopFrame 框架

class CTopFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CTopFrame)
	CSplitterWnd m_split;
protected:
	CTopFrame();           // 动态创建所使用的受保护的构造函数
	virtual ~CTopFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


