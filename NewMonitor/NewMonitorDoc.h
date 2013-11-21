
// NewMonitorDoc.h : CNewMonitorDoc ��Ľӿ�
//
#include <string>
#include <map>
#include <vector>
#include <tuple>

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

public:
	struct SYSTEM_SETTINGS{
		// com settings
		std::string _com_port{"COM4"};
	}_system_settings;

	struct EXPLORE_STATUS{
		unsigned		_sel_status{ 0 };   //0: no sel, 1:file, 2: table
		std::wstring	_sel_filename;
		unsigned		_sel_tableid;

		std::map<std::wstring/*filename*/, std::map<int, std::tuple<std::wstring/*displayname*/, std::wstring/*description*/>>> _file_map;
	}_explore_status;
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
