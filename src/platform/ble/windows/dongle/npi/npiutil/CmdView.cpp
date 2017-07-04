/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
// CmdView.cpp :
//

#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "CmdView.h"


// CCmdView

IMPLEMENT_DYNCREATE(CCmdView, CFormView)

CCmdView::CCmdView()
	: CFormView(CCmdView::IDD)
	, m_tabCurSel(0)
{
	theApp.m_cmdView = this;
}

CCmdView::~CCmdView()
{
}

void CCmdView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CMD, m_tab);
	DDX_Control(pDX, IDC_VIEW_CONHDL, m_conHdl);
	DDX_Control(pDX, IDC_VIEW_CONSTA, m_conState);
	DDX_Control(pDX, IDC_VIEW_BDA, m_conBDA);
}

BEGIN_MESSAGE_MAP(CCmdView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CMD, &CCmdView::OnSelchangeTabCmd)
	ON_BN_CLICKED(IDC_VIEW_TERM, &CCmdView::OnBnClickedViewTerm)
	ON_CBN_SELCHANGE(IDC_VIEW_CONHDL, &CCmdView::OnSelchangeViewConhdl)
END_MESSAGE_MAP()


// CCmdView debug

#ifdef _DEBUG
void CCmdView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCmdView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCmdView message handle

void CCmdView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_tab.InsertItem(0, L"Discover/Connect");
	m_tab.InsertItem(1, L"Read/Write");
	m_tab.InsertItem(2, L"Pairing/Bonding");
	m_tab.InsertItem(3, L"Commands");
	m_tab.SetCurSel(m_tabCurSel);

	CRect rc;
	m_tab.GetClientRect(&rc);
	rc.top += 24;
	rc.bottom -= 2;
	rc.left += 2;
	rc.right -= 4;
	//Creat discovery page
	m_page1.Create(IDD_TAB1_DLG, &m_tab);
	m_page1.MoveWindow(&rc);
	m_page1.ShowWindow(SW_SHOW);

	//Creat read/write page
	m_page2.Create(IDD_TAB2_DLG, &m_tab);
	m_page2.MoveWindow(&rc);
	m_page2.ShowWindow(SW_HIDE);

	//Creat pair/bond page
	m_page3.Create(IDD_TAB3_DLG, &m_tab);
	m_page3.MoveWindow(&rc);
	m_page3.ShowWindow(SW_HIDE);

	//Creat command page
	m_page4.Create(IDD_TAB4_DLG, &m_tab);
	m_page4.MoveWindow(&rc);
	m_page4.ShowWindow(SW_HIDE);

}

void CCmdView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	RECT rc;

	if (m_tab.m_hWnd == NULL) {
		return;        // Return if window is not created yet.
	}
	// Get size of dialog window.
	GetClientRect(&rc);
	rc.top += 50;
	// Adjust the rectangle to fit the tab control into the
	// dialog's client rectangle.
	m_tab.AdjustRect(FALSE, &rc);
	// Move the tab control to the new position and size.
	m_tab.MoveWindow(&rc, TRUE);

	m_tab.GetClientRect(&rc);
	rc.top += 24;
	rc.bottom -= 2;
	rc.left += 2;
	rc.right -= 4;
	m_page1.MoveWindow(&rc);
	m_page2.MoveWindow(&rc);
	m_page3.MoveWindow(&rc);
	m_page4.MoveWindow(&rc);
}


void CCmdView::OnSelchangeTabCmd(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_page1.ShowWindow(SW_HIDE);
	m_page2.ShowWindow(SW_HIDE);
	m_page3.ShowWindow(SW_HIDE);
	m_page4.ShowWindow(SW_HIDE);
	switch (m_tab.GetCurSel()) {
	case 0:
		m_page1.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_page2.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_page3.ShowWindow(SW_SHOW);
		break;
	case 3:
		m_page4.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	*pResult = 0;
}

void CCmdView::OnBnClickedViewTerm()
{
	WCHAR buf[10];
	UINT16 handle;
	if (theApp.m_cmdHandle) {
		m_conHdl.GetWindowText(buf, 10);
		handle = (UINT16)wcstol(buf, NULL, 16);
		theApp.m_cmdHandle->GAP_TerminateLinkRequest(handle);
	}
}

void CCmdView::OnSelchangeViewConhdl()
{
	wchar_t slvAdr[20] = {0};
	UINT8 temp[10] = { 0 };
	int pos = theApp.m_cmdView->m_conHdl.GetCurSel();
	//update state
	if (theApp.m_state[pos].conFlag == NPI_TRUE) {
		theApp.m_cmdView->m_conState.SetWindowText(_T("Connected"));
		//update bd address
		memcpy_s(temp, 10, theApp.m_state[pos].bdaddr, BLE_ADDR_LEN);
		swprintf_s(slvAdr, 14, _T("%llX"), *(long long*)temp);
		theApp.m_cmdView->m_conBDA.SetWindowText(slvAdr);
	} else {
		theApp.m_cmdView->m_conState.SetWindowText(_T("Disconnected"));
		//clear bd address
		theApp.m_cmdView->m_conBDA.SetWindowText(_T("None"));
	}


}