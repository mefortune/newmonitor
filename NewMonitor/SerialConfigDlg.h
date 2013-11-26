#pragma once
#include "afxwin.h"


// CSerialConfigDlg dialog

class CSerialConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSerialConfigDlg)

public:
	CSerialConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSerialConfigDlg();

// Dialog Data
	enum { IDD = IDD_SERIAL_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_serial_sel;
	virtual void OnOK();
};
