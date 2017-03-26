#pragma once


// CLogView View

class CLogView : public CRichEditView
{
	DECLARE_DYNCREATE(CLogView)
	CRichEditCtrl& m_richEdit;
protected:
	CLogView();           
	virtual ~CLogView();

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
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void On1SelAll();
	afx_msg void On1Copy();
	afx_msg void On1ClrLog();
};


