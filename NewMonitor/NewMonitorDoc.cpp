
// NewMonitorDoc.cpp : CNewMonitorDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "NewMonitor.h"
#endif

#include "NewMonitorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewMonitorDoc

IMPLEMENT_DYNCREATE(CNewMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CNewMonitorDoc, CDocument)
END_MESSAGE_MAP()


// CNewMonitorDoc 构造/析构

CNewMonitorDoc::CNewMonitorDoc()
{
	// TODO:  在此添加一次性构造代码
}

CNewMonitorDoc::~CNewMonitorDoc()
{
}

CNewMonitorDoc * CNewMonitorDoc::GetDoc()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	return (CNewMonitorDoc *)pFrame->GetActiveDocument();
}

BOOL CNewMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  在此添加重新初始化代码
	// (SDI 文档将重用该文档
	_system_settings._com_port = theApp.GetInt(L"SERIALPORT", 1);

	_system_settings.device_type[0] = theApp.GetInt(L"DEVICETYPE0", 0);
	_system_settings.device_type[1] = theApp.GetInt(L"DEVICETYPE1", 0);
	_system_settings.device_type[2] = theApp.GetInt(L"DEVICETYPE2", 0);

	_system_settings.device_range[0] = theApp.GetInt(L"DEVICERANGE0", 0);
	_system_settings.device_range[1] = theApp.GetInt(L"DEVICERANGE1", 0);
	_system_settings.device_range[2] = theApp.GetInt(L"DEVICERANGE2", 0);

	_system_settings.coordinate_type[0] = theApp.GetInt(L"COORDINATE0", 0);
	_system_settings.coordinate_type[1] = theApp.GetInt(L"COORDINATE1", 0);
	_system_settings.coordinate_type[2] = theApp.GetInt(L"COORDINATE2", 0);
	return TRUE;
}

// CNewMonitorDoc 序列化

void CNewMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CNewMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CNewMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CNewMonitorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CNewMonitorDoc 诊断

#ifdef _DEBUG
void CNewMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNewMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNewMonitorDoc 命令
