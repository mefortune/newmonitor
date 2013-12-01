
// NewMonitorDoc.cpp : CNewMonitorDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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


// CNewMonitorDoc ����/����

CNewMonitorDoc::CNewMonitorDoc()
{
	// TODO:  �ڴ����һ���Թ������
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

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�
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

// CNewMonitorDoc ���л�

void CNewMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CNewMonitorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CNewMonitorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CNewMonitorDoc ���

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


// CNewMonitorDoc ����
