// TopFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "TopFrame.h"

#include "LogView.h"
#include "CmdView.h"
// CTopFrame

IMPLEMENT_DYNCREATE(CTopFrame, CFrameWnd)

CTopFrame::CTopFrame()
{

}

CTopFrame::~CTopFrame()
{
}


BEGIN_MESSAGE_MAP(CTopFrame, CFrameWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTopFrame 消息处理程序


void CTopFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	CRect clientRect;
	GetClientRect(&clientRect);

	if (m_split){
		m_split.SetColumnInfo(0, (int)(clientRect.Width()*0.6), 200);
		m_split.RecalcLayout();
	}
}


BOOL CTopFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_split.CreateStatic(this, 1, 2);
	m_split.CreateView(0, 0, RUNTIME_CLASS(CLogView), CSize(0, 0), NULL);
	m_split.CreateView(0, 1, RUNTIME_CLASS(CCmdView), CSize(0, 0), NULL);

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
