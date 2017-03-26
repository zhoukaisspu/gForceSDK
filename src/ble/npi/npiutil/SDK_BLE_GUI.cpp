
// SDK_BLE_GUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SDK_BLE_GUI.h"
#include "MainFrm.h"
#include "log.h"

#include "NewDeviceDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSDK_BLE_GUIApp

BEGIN_MESSAGE_MAP(CSDK_BLE_GUIApp, CWinApp)
	ON_COMMAND(ID_NEW_DEV, &CSDK_BLE_GUIApp::OnNewDevice)
	ON_COMMAND(ID_CLOSE_DEV, &CSDK_BLE_GUIApp::OnCloseDevice)
	ON_COMMAND(ID_APP_ABOUT, &CSDK_BLE_GUIApp::OnAppAbout)
END_MESSAGE_MAP()


// CSDK_BLE_GUIApp construction

CSDK_BLE_GUIApp::CSDK_BLE_GUIApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(
	        System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SDK_BLE_GUI.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_cmdHandle = NULL;

	memset(m_state, 0x00, sizeof(m_state));
	m_state[0].conHdl = 0xFFFE;
	m_state[0].enable = NPI_TRUE;
}

// The one and only CSDK_BLE_GUIApp object

CSDK_BLE_GUIApp theApp;


// CSDK_BLE_GUIApp initialization

BOOL CSDK_BLE_GUIApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame) {
		return FALSE;
	}
	m_pMainWnd = pFrame;

	CCreateContext cc;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
	                  WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
	                  &cc);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CSDK_BLE_GUIApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	return CWinApp::ExitInstance();
}

// CSDK_BLE_GUIApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CSDK_BLE_GUIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// App command to run the dialog
void CSDK_BLE_GUIApp::OnNewDevice()
{
	NewDeviceDlg newDevDlg;
	newDevDlg.DoModal();
}

// App command to run the dialog
void CSDK_BLE_GUIApp::OnCloseDevice()
{
	if (theApp.m_cmdHandle) {
		theApp.m_cmdHandle->DisConnect();
		theApp.m_cmdHandle = NULL;
	}
}
// CSDK_BLE_GUIApp message handlers

#define RED_COLOR RGB(255, 0, 0)
#define BLUE_COLOR RGB(0, 0, 255)
#define GREEN_COLOR RGB(0, 255, 0)
CHARFORMAT2 cf;

void setColor(UINT8 color)
{
	theApp.m_logView->m_richEdit.GetSelectionCharFormat(cf);
	cf.dwMask = CFM_COLOR | CFM_SIZE;// | CFM_BOLD | CFM_BACKCOLOR;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	if (color == 0)
	{
		cf.crTextColor = GREEN_COLOR;//green
	}
	else if (color == 1)
	{
		cf.crTextColor = RED_COLOR;//red
	}
	else if (color == 2)
	{
		cf.crTextColor = BLUE_COLOR;//blue
	}
	cf.yHeight = 200;
}

void setData(WCHAR *buf)
{
	theApp.m_logView->m_richEdit.SetSel(-1, -1);
	theApp.m_logView->m_richEdit.SetSelectionCharFormat(cf);
	theApp.m_logView->m_richEdit.ReplaceSel((LPCTSTR)buf);
}

void setMessageBox(WCHAR *buf)
{
	AfxMessageBox(buf);
}

