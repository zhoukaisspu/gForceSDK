// ButtGattView.cpp
#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "ButtGattView.h"


// CButtGattView

IMPLEMENT_DYNCREATE(CButtGattView, CListView)

CButtGattView::CButtGattView() :m_list(GetListCtrl())
{
	theApp.m_gattView = this;
}

CButtGattView::~CButtGattView()
{
}

BEGIN_MESSAGE_MAP(CButtGattView, CListView)
END_MESSAGE_MAP()


// CButtGattView debug

#ifdef _DEBUG
void CButtGattView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CButtGattView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CButtGattView message handle
void CButtGattView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	m_list.ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, (LPCTSTR)L"ConHandle", LVCFMT_LEFT, 150);
	m_list.InsertColumn(1, (LPCTSTR)L"Handle", LVCFMT_LEFT, 150);
	m_list.InsertColumn(2, (LPCTSTR)L"Uuid", LVCFMT_LEFT, 150);
	m_list.InsertColumn(3, (LPCTSTR)L"Uuid Description", LVCFMT_LEFT, 500);
	m_list.InsertColumn(4, (LPCTSTR)L"Value", LVCFMT_LEFT, 150);
	m_list.InsertColumn(5, (LPCTSTR)L"Value Description", LVCFMT_LEFT, 500);
	m_list.InsertColumn(6, (LPCTSTR)L"Properties", LVCFMT_LEFT, 200);
}
