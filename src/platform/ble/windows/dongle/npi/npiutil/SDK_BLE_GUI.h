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

// SDK_BLE_GUI.h : main header file for the SDK_BLE_GUI application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "ButtGattView.h"
#include "CmdView.h"
#include "LogView.h"
#include "npi_cmd.h"
#include "GuiEvtHdl.h"
#include "gap.h"
#include "gatt.h"

#define MAX_CON_DEV        8
typedef struct {
	UINT16 conHdl;
	UINT8 enable;
	UINT8 conFlag;
	UINT8 bdaddr[6];
} sBleState;

// CSDK_BLE_GUIApp:
// See SDK_BLE_GUI.cpp for the implementation of this class
//
class CSDK_BLE_GUIApp : public CWinApp
{
public:
	CSDK_BLE_GUIApp();
	NPI_CMD* m_cmdHandle;
	CButtGattView* m_gattView;
	CCmdView* m_cmdView;
	CLogView* m_logView;
	sBleState m_state[MAX_CON_DEV];
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	afx_msg void OnNewDevice();
	afx_msg void OnCloseDevice();
	DECLARE_MESSAGE_MAP()
};

extern CSDK_BLE_GUIApp theApp;
