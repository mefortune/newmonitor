
// NewMonitorView.h : CNewMonitorView ��Ľӿ�
//

#pragma once


class CNewMonitorView : public CView
{
protected: // �������л�����
	CNewMonitorView();
	DECLARE_DYNCREATE(CNewMonitorView)

// ����
public:
	CNewMonitorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CNewMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // NewMonitorView.cpp �еĵ��԰汾
inline CNewMonitorDoc* CNewMonitorView::GetDocument() const
   { return reinterpret_cast<CNewMonitorDoc*>(m_pDocument); }
#endif

