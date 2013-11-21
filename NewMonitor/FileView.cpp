
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "NewMonitor.h"
#include "NewMonitorDoc.h"
#include "include\datamanager.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TVN_SELCHANGED, 4, &CFileView::OnSelChange)
	ON_COMMAND(ID_DATA_ADDTABLE, &CFileView::OnAddTable)
	ON_UPDATE_COMMAND_UI(ID_DATA_ADDTABLE, &CFileView::OnUpdateAddTable)
	ON_COMMAND(ID_DATA_CLOSEDATABASE, &CFileView::OnCloseDatabase)
	ON_UPDATE_COMMAND_UI(ID_DATA_CLOSEDATABASE, &CFileView::OnUpdateCloseDatabase)
	ON_COMMAND(IDU_FRESHEXPLORER, &CFileView::OnFreshExplorer)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像: 
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	HTREEITEM cur_sel = m_wndFileView.GetSelectedItem();
	std::wstring cur_sel_name{};		//文件节点名
	int	cur_sel_id = 0;					//表节点ID

	if (cur_sel != NULL){
		if (m_wndFileView.GetParentItem(cur_sel) == NULL){		//文件节点
			cur_sel_name = m_wndFileView.GetItemText(cur_sel).GetString();
		}
		else{													//表节点
			cur_sel_name = m_wndFileView.GetItemText(m_wndFileView.GetParentItem(cur_sel)).GetString();
			cur_sel_id = m_wndFileView.GetItemData(cur_sel);
		}
	}

	CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();
	pDoc->_explore_status._sel_status = 0;


	m_wndFileView.DeleteAllItems();
	for (auto file_pair : pDoc->_explore_status._file_map){
		HTREEITEM hRoot = m_wndFileView.InsertItem(file_pair.first.c_str(), 0, 0);
		m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
		
		if (file_pair.first == cur_sel_name && cur_sel_id == 0){
			m_wndFileView.SelectItem(hRoot);
		}

		for (auto table : file_pair.second){
			HTREEITEM hItem = m_wndFileView.InsertItem(std::get<1>(table).c_str(), 1, 1, hRoot);
			m_wndFileView.SetItemData(hItem, std::get<0>(table));
			if (file_pair.first == cur_sel_name && std::get<0>(table) == cur_sel_id){
				m_wndFileView.SelectItem(hItem);
			}
		}
		m_wndFileView.Expand(hRoot, TVE_EXPAND);
	}
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项: 
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图:  %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pnmtv = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM cur_sel = pnmtv->itemNew.hItem;
	m_wndFileView.EnsureVisible(cur_sel);

	CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();
	if (m_wndFileView.GetParentItem(cur_sel) == NULL)		//文件节点
	{
		
		pDoc->_explore_status._sel_status = 1;
		pDoc->_explore_status._sel_filename = m_wndFileView.GetItemText(cur_sel).GetString();
	}	
	else{													//表节点
		pDoc->_explore_status._sel_status = 2;
		pDoc->_explore_status._sel_filename = m_wndFileView.GetItemText(m_wndFileView.GetParentItem(cur_sel)).GetString();
		pDoc->_explore_status._sel_tableid = m_wndFileView.GetItemData(cur_sel);
	}

	*pResult = 0;
}
void CFileView::OnAddTable()
{
	
}

void CFileView::OnUpdateAddTable(CCmdUI* pCmdUI)
{
	CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();
	switch (pDoc->_explore_status._sel_status)
	{
	case 0:
		pCmdUI->Enable(false);
		break;
	case 1:
		pCmdUI->Enable(true);
		break;
	case 2:
		pCmdUI->Enable(false);
		break;
	}
}
void CFileView::OnCloseDatabase()
{

}

void CFileView::OnUpdateCloseDatabase(CCmdUI* pCmdUI)
{
	CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();
	switch (pDoc->_explore_status._sel_status)
	{
	case 0:
		pCmdUI->Enable(false);
		break;
	case 1:
		pCmdUI->Enable(true);
		break;
	case 2:
		pCmdUI->Enable(false);
		break;
	}
}
void CFileView::OnFreshExplorer()
{
	FillFileView();
}