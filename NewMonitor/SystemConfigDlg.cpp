// SystemConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewMonitor.h"
#include "SystemConfigDlg.h"
#include "afxdialogex.h"

#include "NewMonitorDoc.h"
// CSystemConfigDlg dialog

IMPLEMENT_DYNAMIC(CSystemConfigDlg, CDialogEx)

CSystemConfigDlg::CSystemConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemConfigDlg::IDD, pParent)
	, m_device_range_1(0)
	, m_device_range_2(0)
	, m_device_range_3(0)
	, m_cord_x(2)
	, m_cord_y1(0)
	, m_cord_y2(1)
{

}

CSystemConfigDlg::~CSystemConfigDlg()
{
}

void CSystemConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_device_range_1);
	DDX_Text(pDX, IDC_EDIT2, m_device_range_2);
	DDX_Text(pDX, IDC_EDIT3, m_device_range_3);
	DDX_Radio(pDX, IDC_RADIOA1, m_cord_x);
	DDX_Radio(pDX, IDC_RADIOB1, m_cord_y1);
	DDX_Radio(pDX, IDC_RADIOC1, m_cord_y2);
}


BEGIN_MESSAGE_MAP(CSystemConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// CSystemConfigDlg message handlers


void CSystemConfigDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();
	pDoc->_system_settings.device_type[0] = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1))->GetCurSel();
	pDoc->_system_settings.device_type[1] = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO2))->GetCurSel();
	pDoc->_system_settings.device_type[2] = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO3))->GetCurSel();

	pDoc->_system_settings.device_range[0] = m_device_range_1;
	pDoc->_system_settings.device_range[1] = m_device_range_2;
	pDoc->_system_settings.device_range[2] = m_device_range_3;

	pDoc->_system_settings.coordinate_type[0] = m_cord_x;
	pDoc->_system_settings.coordinate_type[1] = m_cord_y1;
	pDoc->_system_settings.coordinate_type[2] = m_cord_y2;

	CDialogEx::OnOK();
}


BOOL CSystemConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1))->InsertString(0, L"0 ~ 5V");
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1))->InsertString(1, L"1 ~ 5V");
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO1))->SetCurSel(pDoc->_system_settings.device_type[0]);
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO2))->InsertString(0, L"0 ~ 5V");
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO2))->InsertString(1, L"1 ~ 5V");
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO2))->SetCurSel(pDoc->_system_settings.device_type[1]);
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO3))->InsertString(0, L"0 ~ 5V");
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO3))->InsertString(1, L"1 ~ 5V");
	static_cast<CComboBox*>(GetDlgItem(IDC_COMBO3))->SetCurSel(pDoc->_system_settings.device_type[2]);

	m_device_range_1 = pDoc->_system_settings.device_range[0];
	m_device_range_2 = pDoc->_system_settings.device_range[1];
	m_device_range_3 = pDoc->_system_settings.device_range[2];

	m_cord_x = pDoc->_system_settings.coordinate_type[0];
	m_cord_y1 = pDoc->_system_settings.coordinate_type[1];
	m_cord_y2 = pDoc->_system_settings.coordinate_type[2];

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
