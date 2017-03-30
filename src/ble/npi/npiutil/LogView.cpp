/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
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
// LogView.cpp : 
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
