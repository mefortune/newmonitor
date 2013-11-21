
// NewMonitorDoc.h : CNewMonitorDoc 类的接口
//
#include <string>
#include <map>
#include <vector>
#include <tuple>

class CNewMonitorDoc : public CDocument
{
protected: // 仅从序列化创建
	CNewMonitorDoc();
	DECLARE_DYNCREATE(CNewMonitorDoc)

// 特性
public:

// 操作
public:
	static CNewMonitorDoc * GetDoc();
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
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

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
