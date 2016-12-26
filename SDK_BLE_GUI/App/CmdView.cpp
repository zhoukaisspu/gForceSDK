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
}

BEGIN_MESSAGE_MAP(CCmdView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CMD, &CCmdView::OnSelchangeTabCmd)
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
	m_discPage.Create(IDD_TAB1_DLG, &m_tab);
	m_discPage.MoveWindow(&rc);
	m_discPage.ShowWindow(SW_SHOW);   

	//Creat read/write page
	m_rwPage.Create(IDD_TAB2_DLG, &m_tab);
	m_rwPage.MoveWindow(&rc);
	m_rwPage.ShowWindow(SW_HIDE);

	//Creat pair/bond page
	m_pairPage.Create(IDD_TAB3_DLG, &m_tab);
	m_pairPage.MoveWindow(&rc);
	m_pairPage.ShowWindow(SW_HIDE);

	//Creat command page
	m_cmdPage.Create(IDD_TAB4_DLG, &m_tab);
	m_cmdPage.MoveWindow(&rc);
	m_cmdPage.ShowWindow(SW_HIDE);
	
}

void CCmdView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	RECT rc;

	if (m_tab.m_hWnd == NULL)
		return;      // Return if window is not created yet.
	// Get size of dialog window.
	GetClientRect(&rc);
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
	m_discPage.MoveWindow(&rc);
	m_rwPage.MoveWindow(&rc);
	m_pairPage.MoveWindow(&rc);
	m_cmdPage.MoveWindow(&rc);
}


void CCmdView::OnSelchangeTabCmd(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_discPage.ShowWindow(SW_HIDE);
	m_rwPage.ShowWindow(SW_HIDE);
	m_pairPage.ShowWindow(SW_HIDE);
	m_cmdPage.ShowWindow(SW_HIDE);
	switch (m_tab.GetCurSel())
	{
		case 0:
			m_discPage.ShowWindow(SW_SHOW);
			break;
		case 1:
			m_rwPage.ShowWindow(SW_SHOW);
			break;
		case 2:
			m_pairPage.ShowWindow(SW_SHOW);
			break;
		case 3:
			m_cmdPage.ShowWindow(SW_SHOW);
			break;
		default:
			break;
	}
	*pResult = 0;
}
