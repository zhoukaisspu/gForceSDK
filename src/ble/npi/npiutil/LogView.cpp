// LogView.cpp : 实现文件
//

#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "LogView.h"


// CLogView

IMPLEMENT_DYNCREATE(CLogView, CRichEditView)

CLogView::CLogView() :m_richEdit(GetRichEditCtrl())
{
	theApp.m_logView = this;
}

CLogView::~CLogView()
{
}

BEGIN_MESSAGE_MAP(CLogView, CRichEditView)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_1_SEL_ALL, &CLogView::On1SelAll)
	ON_COMMAND(ID_1_COPY, &CLogView::On1Copy)
	ON_COMMAND(ID_1_CLR_LOG, &CLogView::On1ClrLog)
END_MESSAGE_MAP()


// CLogView debug
#ifdef _DEBUG
void CLogView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CLogView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLogView message handle
void CLogView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();
	//m_richEdit.ModifyStyle(0, Multiline);
	m_richEdit.SetReadOnly();
}


void CLogView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMenu popEditmenu;
	popEditmenu.LoadMenu(IDR_MENU_RICHEDIT);
	CMenu *pPopup = popEditmenu.GetSubMenu(0);

	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	CRichEditView::OnRButtonUp(nFlags, point);
}


void CLogView::On1SelAll()
{
	m_richEdit.SetSel(0, -1);
}


void CLogView::On1Copy()
{
	m_richEdit.Copy();
}


void CLogView::On1ClrLog()
{
	//m_richEdit.Clear();
	m_richEdit.SetWindowText(L"");
}
