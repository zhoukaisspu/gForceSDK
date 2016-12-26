
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
