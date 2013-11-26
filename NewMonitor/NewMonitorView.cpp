
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
#include <cstdint>
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewMonitorView

IMPLEMENT_DYNCREATE(CNewMonitorView, CView)

BEGIN_MESSAGE_MAP(CNewMonitorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDU_FRESHMAP, OnFreshMap)
	ON_COMMAND(ID_IMAGE_ARRAY, OnImageArray)
	ON_COMMAND(ID_IMAGE_LARGE, OnImageLarge)
	ON_COMMAND(ID_IMAGE_SMALL, OnImageSmall)
	ON_CONTROL(CVN_ViewPortChanged, 1, OnViewPortChanged)

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


int CNewMonitorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口: 
	if (!m_chart.Create(NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE, rectDummy, this, 1))
	{
		TRACE0("未能创建图表窗口\n");
		return -1;      // 未能创建
	}
	return 0;
}


void CNewMonitorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_chart.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CNewMonitorView::OnFreshMap()
{
	m_chart.updateViewPort(true, true);
}

void CNewMonitorView::DrawChart()
{
	CNewMonitorDoc *pDoc = GetDocument();

	if (pDoc->_explore_status._sel_status != 2){
		return;
	}
	const std::vector<char>& data = std::get<1>(pDoc->_explore_status._data_info[pDoc->_explore_status._sel_data_id]);
	std::tm data_time = std::get<2>(pDoc->_explore_status._data_info[pDoc->_explore_status._sel_data_id]);

	std::vector<std::tuple<double, double, double>> parsed_data = ParseData(data, 0, 1);
	RECT client_rect;
	GetClientRect(&client_rect);
	XYChart *c = new XYChart(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
	c->setPlotArea(50, 50, client_rect.right - client_rect.left - 100, client_rect.bottom - client_rect.top - 100);
	
	std::vector<double> value_1, value_2, value_3;

	if (pDoc->_system_settings.coordinate_type[0] == 0){
		std::sort(parsed_data.begin(), parsed_data.end(),
			[](const std::tuple<double, double, double>&a, const std::tuple<double, double, double>&b){
			return std::get<2>(a) < std::get<2>(b);
		});
		for (auto data : parsed_data){
			value_1.push_back(std::get<0>(data));
			value_2.push_back(std::get<1>(data));
			value_3.push_back(std::get<2>(data));
		}
		if (pDoc->_system_settings.coordinate_type[1] == 0){
			auto line = c->addLineLayer(DoubleArray(value_1.data(), value_1.size()),
				0xc00000);
			line->setXData(DoubleArray(value_3.data(), value_3.size()));
			line->setLineWidth(3);
		}
		else if (pDoc->_system_settings.coordinate_type[1] == 1){
			auto line = c->addLineLayer(DoubleArray(value_2.data(), value_2.size()),
				0xc00000);
			line->setXData(DoubleArray(value_3.data(), value_3.size()));
			line->setLineWidth(3);
		}
		if (pDoc->_system_settings.coordinate_type[2] == 0){
			auto line = c->addLineLayer(DoubleArray(value_1.data(), value_1.size()),
				0xc00000);
			line->setXData(DoubleArray(value_3.data(), value_3.size()));
			line->setUseYAxis2();
			line->setLineWidth(3);
		}
		else if (pDoc->_system_settings.coordinate_type[2] == 1){
			auto line = c->addLineLayer(DoubleArray(value_2.data(), value_2.size()),
				0xc00000);
			line->setXData(DoubleArray(value_3.data(), value_3.size()));
			line->setUseYAxis2();
			line->setLineWidth(3);
		}
	}

	m_chart.syncDateAxisWithViewPort("x", c->xAxis());
	delete m_chart.getChart();
	m_chart.setChart(c);

}
void CNewMonitorView::UpdateImageMap()
{
	if (nullptr == m_chart.getImageMapHandler() && m_chart.getChart() != nullptr)
	{
		m_chart.setImageMap(m_chart.getChart()->getHTMLImageMap("", "",
			"title='[{dataSetName}]  value {value|2}'"));
	}

}
std::vector<std::tuple<double, double, double>> CNewMonitorView::ParseData(const std::vector<char>& data, short type1, short type2) // type: 0->压力  1->位移 2->流量
{
	std::vector<std::tuple<double, double, double>> temp{};
	if (data.size() % 4 != 0){
		return temp;
	}

	CNewMonitorDoc *pDoc = GetDocument();
	unsigned int low_val_1 = pDoc->_system_settings.device_type[type1] == 0 ? 0 : 819;
	unsigned int low_val_2 = pDoc->_system_settings.device_type[type2] == 0 ? 0 : 819;

	double start_time = 0.0;
	for (std::vector<char>::const_iterator it = data.begin(); it != data.end(); it += 4, start_time += 0.1){
		uint16_t value_1 = MAKEWORD(*it, *(it + 1));
		uint16_t value_2 = MAKEWORD(*(it + 2), *(it + 3));

		double parsed_value_1 = (value_1 - low_val_1) * 1.0 / (4095 - low_val_1) * pDoc->_system_settings.device_range[type1];
		double parsed_value_2 = (value_2 - low_val_2) * 1.0 / (4095 - low_val_2) * pDoc->_system_settings.device_range[type2];
		temp.push_back(std::make_tuple(parsed_value_1, parsed_value_2, start_time));
	}

	return temp;
}

void CNewMonitorView::OnImageArray()
{
	m_chart.setMouseUsage(Chart::MouseUsageScroll);
}
void CNewMonitorView::OnImageLarge()
{
	m_chart.setMouseUsage(Chart::MouseUsageZoomIn);
}
void CNewMonitorView::OnImageSmall()
{
	m_chart.setMouseUsage(Chart::MouseUsageZoomOut);
}

void CNewMonitorView::OnViewPortChanged()
{
	if (m_chart.needUpdateChart())
		DrawChart();
	if (m_chart.needUpdateImageMap())
		UpdateImageMap();
}
