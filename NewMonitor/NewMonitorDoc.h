
// NewMonitorDoc.h : CNewMonitorDoc ��Ľӿ�
//


#pragma once

#include <vector>

class CNewMonitorDoc : public CDocument
{
protected: // �������л�����
	CNewMonitorDoc();
	DECLARE_DYNCREATE(CNewMonitorDoc)

// ����
public:

// ����
public:
	static CNewMonitorDoc * GetDoc();
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CNewMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:


protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
