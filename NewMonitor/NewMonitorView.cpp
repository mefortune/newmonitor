
// NewMonitorView.cpp : CNewMonitorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CNewMonitorView 构造/析构

CNewMonitorView::CNewMonitorView()
{
	// TODO:  在此处添加构造代码

}

CNewMonitorView::~CNewMonitorView()
{
}

BOOL CNewMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CNewMonitorView 绘制

void CNewMonitorView::OnDraw(CDC* /*pDC*/)
{
	CNewMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
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


// CNewMonitorView 诊断

#ifdef _DEBUG
void CNewMonitorView::AssertValid() const
{
	CView::AssertValid();
}

void CNewMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNewMonitorDoc* CNewMonitorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewMonitorDoc)));
	return (CNewMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CNewMonitorView 消息处理程序
