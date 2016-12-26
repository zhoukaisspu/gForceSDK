// NewDeviceDlg.cpp : implementation of the NewDeviceDlg class
//

#include "stdafx.h"
#include "SDK_BLE_GUI.h"
#include "NewDeviceDlg.h"
#include "afxdialogex.h"

#include "npi_cmd.h"
#include "MainFrm.h"
// NewDeviceDlg DIALOG

IMPLEMENT_DYNAMIC(NewDeviceDlg, CDialogEx)

NewDeviceDlg::NewDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NewDeviceDlg::IDD, pParent)
{

}

NewDeviceDlg::~NewDeviceDlg()
{
}

void NewDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_port);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_baud);
	DDX_Control(pDX, IDC_COMBO_FLOW, m_flow);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_parity);
	DDX_Control(pDX, IDC_COMBO_STOPBIT, m_stopbit);
	DDX_Control(pDX, IDC_COMBO_DATABIT, m_databit);

	m_port.SetCurSel(2);
	m_baud.SetCurSel(5);
	m_flow.SetCurSel(0);
	m_parity.SetCurSel(0);
	m_stopbit.SetCurSel(0);
	m_databit.SetCurSel(1);
}


BEGIN_MESSAGE_MAP(NewDeviceDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewDeviceDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// NewDeviceDlg message handle


void NewDeviceDlg::OnBnClickedOk()
{
	
	UINT8 port[] = { 1, 2, 3, 4, 5 };
	DWORD baud[] = { 9600, 57600, 38400, 19200, 14400, 115200 };
	UINT8 parity[] = { NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY };
	UINT8 stopBit[] = { ONESTOPBIT, TWOSTOPBITS };
	UINT8 dataBit[] = { 7, 8, 9 };
	UINT8 key[16] = { 0 };
	UINT8 signCount[4] = {1, 0, 0, 0};
	TRACE("Port=%d\n", port[m_port.GetCurSel()]);
	TRACE("Baud=%d\n", baud[m_baud.GetCurSel()]);
	TRACE("Flow=%d\n", m_flow.GetCurSel());
	TRACE("Parity=%d\n", parity[m_parity.GetCurSel()]);
	TRACE("StopBit=%d\n", stopBit[m_stopbit.GetCurSel()]);
	TRACE("DataBits=%d\n", dataBit[m_databit.GetCurSel()]);
	/*NPI*/
	if (!theApp.m_cmdHandle){
		theApp.m_cmdHandle = new NPI_CMD(port[m_port.GetCurSel()], baud[m_baud.GetCurSel()], \
			parity[m_parity.GetCurSel()], dataBit[m_databit.GetCurSel()], stopBit[m_stopbit.GetCurSel()]);

		if (!theApp.m_cmdHandle->Connect()){
			AfxMessageBox(L"Can't open port!");
		}
		else{
			RegistEvtCallBack();
			theApp.m_cmdHandle->GAP_DeviceInit(0x08, 0x05, key, key, signCount);
		}
	}

	CDialogEx::OnOK();
}
