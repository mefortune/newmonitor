// DescriptionConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewMonitor.h"
#include "DescriptionConfigDlg.h"
#include "afxdialogex.h"


// CDescriptionConfigDlg �Ի���

IMPLEMENT_DYNAMIC(CDescriptionConfigDlg, CDialogEx)

CDescriptionConfigDlg::CDescriptionConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDescriptionConfigDlg::IDD, pParent)
	, m_dataname(_T(""))
	, m_datadescription(_T(""))
{

}

CDescriptionConfigDlg::~CDescriptionConfigDlg()
{
}

void CDescriptionConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DATANAME, m_dataname);
	DDX_Text(pDX, IDC_DATADESCRIPTION, m_datadescription);
}


BEGIN_MESSAGE_MAP(CDescriptionConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// CDescriptionConfigDlg ��Ϣ�������
