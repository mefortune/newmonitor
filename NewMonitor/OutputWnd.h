
#pragma once
#include <string>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// COutputList ����

class COutputList : public CMFCListCtrl
{
// ����
public:
	COutputList();

// ʵ��
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelChangeData(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};


class COutputWnd : public CDockablePane
{
// ����
public:
	COutputWnd();

	void UpdateFonts();

// ����
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputData;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

protected:
	std::wstring ConvertVectorToString(const std::vector<char>& data);

// ʵ��
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFreshData();
	
	DECLARE_MESSAGE_MAP()
};

