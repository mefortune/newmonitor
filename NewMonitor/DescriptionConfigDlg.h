#pragma once


// CDescriptionConfigDlg �Ի���

class CDescriptionConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDescriptionConfigDlg)

public:
	CDescriptionConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDescriptionConfigDlg();

// �Ի�������
	enum { IDD = IDD_EDITDESCRIPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_dataname;
	CString m_datadescription;
};
