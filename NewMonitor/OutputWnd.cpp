
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "NewMonitorDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDU_FRESHDATA, OnFreshData)
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口: 
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_3D_ROUNDED, rectDummy, this, 1))
	{
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}

	// 创建输出窗格: 
	const DWORD dwStyle = WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS | LVS_SINGLESEL;

	if (!m_wndOutputData.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// 将列表窗口附加到选项卡: 
	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputData, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	m_wndOutputData.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_wndOutputData.InsertColumn(0, L"序号", LVCFMT_CENTER, -1, -1);
	m_wndOutputData.InsertColumn(1, L"数据段", LVCFMT_CENTER, -1, -1);
	m_wndOutputData.InsertColumn(2, L"数据时间", LVCFMT_CENTER, -1, -1);
	m_wndOutputData.InsertColumn(3, L"数据", LVCFMT_CENTER, -1, -1);

	return 0;
}

void COutputWnd::OnFreshData()
{
	CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();

	m_wndOutputData.DeleteAllItems();
	if (pDoc->_explore_status._sel_status == 2){
		wchar_t item_text[30];
		int elem_index = 0;
		for (auto data_pair : pDoc->_explore_status._data_info){
			swprintf_s(item_text, L"%d", data_pair.first);
			int index = m_wndOutputData.InsertItem(elem_index, item_text);
			m_wndOutputData.SetItemData(index, data_pair.first);
			swprintf_s(item_text, L"%d", std::get<0>(data_pair.second));
			m_wndOutputData.SetItemText(index, 1, item_text);
			swprintf_s(item_text, L"%02d年%02d月%02d日 %02d:%02d:%02d", std::get<2>(data_pair.second).tm_year % 100,
				std::get<2>(data_pair.second).tm_mon, std::get<2>(data_pair.second).tm_mday,
				std::get<2>(data_pair.second).tm_hour, std::get<2>(data_pair.second).tm_min,
				std::get<2>(data_pair.second).tm_sec);
			m_wndOutputData.SetItemText(index, 2, item_text);
			m_wndOutputData.SetItemText(index, 3, ConvertVectorToString(std::get<1>(data_pair.second)).c_str());

			++elem_index;
		}
	}
}


std::wstring COutputWnd::ConvertVectorToString(const std::vector<char>& data)
{
	std::wstring temp;
	for (auto c : data){
		wchar_t low = static_cast<short>(static_cast<unsigned char>(c & 0x0F)) >= 10 ?
			L'A' + static_cast<short>(static_cast<unsigned char>(c & 0x0F)) - 10 : L'0' + static_cast<short>(static_cast<unsigned char>(c & 0x0F));
		wchar_t high = static_cast<short>(static_cast<unsigned char>(c & 0xF0) >> 4) >= 10 ?
			L'A' + static_cast<short>(static_cast<unsigned char>(c & 0xF0) >> 4) - 10 : L'0' + static_cast<short>(static_cast<unsigned char>(c & 0xF0) >> 4);

		temp += high;
		temp += low;
		temp += L' ';
	}
	return temp;
}
void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区: 
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	RECT client_rect;
	m_wndOutputData.GetClientRect(&client_rect);
	m_wndOutputData.SetColumnWidth(0, 50);
	m_wndOutputData.SetColumnWidth(1, 80);
	m_wndOutputData.SetColumnWidth(2, 150);
	m_wndOutputData.SetColumnWidth(3, client_rect.right - client_rect.left - 280);
}


void COutputWnd::UpdateFonts()
{
	m_wndOutputData.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_WM_WINDOWPOSCHANGING()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &OnSelChangeData)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
	SetFocus();
}
void COutputList::OnSelChangeData(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ((pNMListView->uChanged & LVIF_STATE)
		&& (pNMListView->uNewState & LVNI_SELECTED))
	{
		CNewMonitorDoc* pDoc = CNewMonitorDoc::GetDoc();
		if (pDoc->_explore_status._sel_status == 2){
			pDoc->_explore_status._sel_data_id = GetItemData(pNMListView->iItem);

			CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
			pFrame->SendMessage(WM_COMMAND, MAKELONG(IDU_FRESHMAP, 0), NULL);
		}
	}
}
