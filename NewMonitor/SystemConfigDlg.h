#pragma once
#include "afxwin.h"


// CSystemConfigDlg dialog

class CSystemConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemConfigDlg)

public:
	CSystemConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSystemConfigDlg();

// Dialog Data
	enum { IDD = IDD_SYSTEM_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_device_range_1;
	int m_device_range_2;
	int m_device_range_3;
	int m_cord_x;
	int m_cord_y1;
	int m_cord_y2;
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
