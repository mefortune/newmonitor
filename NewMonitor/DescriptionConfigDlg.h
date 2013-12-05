#pragma once


// CDescriptionConfigDlg 对话框

class CDescriptionConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDescriptionConfigDlg)

public:
	CDescriptionConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDescriptionConfigDlg();

// 对话框数据
	enum { IDD = IDD_EDITDESCRIPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_dataname;
	CString m_datadescription;
};
