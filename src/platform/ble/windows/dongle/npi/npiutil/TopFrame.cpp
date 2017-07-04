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
// TopFrame.cpp : 
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


// CTopFrame 


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
