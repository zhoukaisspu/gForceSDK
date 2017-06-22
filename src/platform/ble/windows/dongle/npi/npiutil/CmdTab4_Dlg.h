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
#pragma once
#include "afxcmn.h"
#include "afxpropertygridctrl.h"


// CmdTab4_Dlg dialog

class CmdTab4_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmdTab4_Dlg)

public:
	CmdTab4_Dlg(CWnd* pParent = NULL);
	virtual ~CmdTab4_Dlg();

// dialog data
	enum { IDD = IDD_TAB4_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_cmdTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTab4Tree(NMHDR* pNMHDR, LRESULT* pResult);
	CMFCPropertyGridCtrl m_gridCtrl;
//	afx_msg void OnClickTab4Tree(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void Show_GAP_DeviceInit(void);
	void Show_GAP_ConfigDeviceAddr(void);
	void Show_GAP_Bond(void);
public:
	afx_msg void OnDblclkTab4Tree(NMHDR* pNMHDR, LRESULT* pResult);
};
