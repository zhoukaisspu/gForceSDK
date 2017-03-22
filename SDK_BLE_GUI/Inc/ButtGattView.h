#pragma once
// CButtGattView view

class CButtGattView : public CListView
{
	DECLARE_DYNCREATE(CButtGattView)
	CListCtrl& m_list;
protected:
	CButtGattView();        
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


