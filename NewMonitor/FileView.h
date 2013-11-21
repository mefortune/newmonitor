
#pragma once

#include "ViewTree.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// ����
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// ����
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

protected:
	void FillFileView();

private:
// ʵ��
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnQueryInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddTable();
	afx_msg void OnUpdateAddTable(CCmdUI* pCmdUI);
	afx_msg void OnDelTable();
	afx_msg void OnUpdateDelTable(CCmdUI* pCmdUI);
	afx_msg void OnCloseDatabase();
	afx_msg void OnUpdateCloseDatabase(CCmdUI* pCmdUI);
	afx_msg void OnEditTable();
	afx_msg void OnUpdateEditTable(CCmdUI* pCmdUI);
	afx_msg void OnEditInfo();
	afx_msg void OnUpdateEditInfo(CCmdUI* pCmdUI);
	afx_msg void OnFreshExplorer();
	DECLARE_MESSAGE_MAP()
};
