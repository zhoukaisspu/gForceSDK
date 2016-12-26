#pragma once


// CButtGattView 视图

class CButtGattView : public CListView
{
	DECLARE_DYNCREATE(CButtGattView)
	CListCtrl& m_list;
protected:
	CButtGattView();           // 动态创建所使用的受保护的构造函数
	virtual ~CButtGattView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


