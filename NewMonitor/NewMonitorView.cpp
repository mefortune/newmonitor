
// NewMonitorView.cpp : CNewMonitorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "NewMonitor.h"
#endif

#include "NewMonitorDoc.h"
#include "NewMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewMonitorView

IMPLEMENT_DYNCREATE(CNewMonitorView, CView)

BEGIN_MESSAGE_MAP(CNewMonitorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CNewMonitorView ����/����

CNewMonitorView::CNewMonitorView()
{
	// TODO:  �ڴ˴���ӹ������

}

CNewMonitorView::~CNewMonitorView()
{
}

BOOL CNewMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CNewMonitorView ����

void CNewMonitorView::OnDraw(CDC* /*pDC*/)
{
	CNewMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CNewMonitorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNewMonitorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CNewMonitorView ���

#ifdef _DEBUG
void CNewMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CNewMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNewMonitorDoc* CNewMonitorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewMonitorDoc)));
	return (CNewMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CNewMonitorView ��Ϣ�������
