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

	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dwStyle);

	m_list.InsertColumn(0, (LPCTSTR)L"ConHandle", LVCFMT_LEFT, 150);
	m_list.InsertColumn(1, (LPCTSTR)L"Handle", LVCFMT_LEFT, 150);
	m_list.InsertColumn(2, (LPCTSTR)L"Uuid", LVCFMT_LEFT, 150);
	m_list.InsertColumn(3, (LPCTSTR)L"Uuid Description", LVCFMT_LEFT, 300);
	m_list.InsertColumn(4, (LPCTSTR)L"Value", LVCFMT_LEFT, 600);
	m_list.InsertColumn(5, (LPCTSTR)L"Value Description", LVCFMT_LEFT, 300);
	m_list.InsertColumn(6, (LPCTSTR)L"Properties", LVCFMT_LEFT, 200);

}
