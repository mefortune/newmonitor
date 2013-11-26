// SerialConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewMonitor.h"
#include "SerialConfigDlg.h"
#include "afxdialogex.h"

#include "NewMonitorDoc.h"

// CSerialConfigDlg dialog

IMPLEMENT_DYNAMIC(CSerialConfigDlg, CDialogEx)

CSerialConfigDlg::CSerialConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialConfigDlg::IDD, pParent)
{

}

CSerialConfigDlg::~CSerialConfigDlg()
{
}

void CSerialConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERIALSEL, m_serial_sel);
}


BEGIN_MESSAGE_MAP(CSerialConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// CSerialConfigDlg message handlers


BOOL CSerialConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_serial_sel.InsertString(0, L"COM1");
	m_serial_sel.InsertString(1, L"COM2");
	m_serial_sel.InsertString(2, L"COM3");
	m_serial_sel.InsertString(3, L"COM4");
	
	CNewMonitorDoc *pDoc = CNewMonitorDoc::GetDoc();
	
	m_serial_sel.SetCurSel(pDoc->_system_settings._com_port - 1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSerialConfigDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	int index = m_serial_sel.GetCurSel();

	CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();
	
	pDoc->_system_settings._com_port = index + 1;
	CDialogEx::OnOK();
}
