#ifndef _PFXEDITCTRL_H_
#define _PFXEDITCTRL_H_ 
// BUG
// 2005-10-10
// 问题描述：
//     在一些黑白打印机上试图显示彩色文本时可能无法显示（注：这个BUG 我已经
//     无时间去处理，请使用的朋友自己去解决）
// 问题分析：
//     这是因为黑白打印机在颜色上不支持选择的颜色，这个问题引伸到在16色或
//     256色的屏幕显示模式下也有同样的问题
// 解决办法：
//     建立一颜色表，对打印机是否支持某颜色进行检查，用最匹配的颜色来调整
// 解决结果：
//     2005-11-07日问题得到解决
// 代码修改
// 2005-11-07
//      2005-10-10至2005-11-07重新对CPfxEditHash，CPfxEditTypeInfo，
//      CPfxEditPrintInfo，CPfxEditAlloc，CPfxEditHeap，CPfxEditData等类作
//      了调整，使他们用统一的内存分配源；为CPfxEditTypeInfo和
//      CPfxEditPrintInfo建立颜色索引表
#pragma once 
//#include <afxole.h>
//#include <afxplex_.h>
// 如果你需要编译为UNICODE版，你需要修改一些用到PFX_EC_MAX_MB_LEN的地方
// 我没有试过在_UNICODE模式下编译，但已经为在_UNICODE模式下编译做准备
#ifdef _UNICODE
#error CPfxEditCtrl NOT SUPPORT UNICODE!
#endif
class CPfxEditDoc;
class CPfxEditView;
#define PFX_EC_SIZEOF(x)         (sizeof(x) / sizeof(x[0]))
#define PFX_EC_SIZEOF_NO_NULL(x) ((sizeof(x) / sizeof(x[0])) - 1)
#define PFX_EC_ROUND(x, y)       ((((static_cast<size_t>(x)) + (static_cast<size_t>(y) - 1)) / (static_cast<size_t>(y))) * static_cast<size_t>(y))
#define PFX_EC_ROUND4(x)         (PFX_EC_ROUND(x, 4)) 
// 默认颜色配置
#define PFX_EC_CLR_RULER                      (COLORREF)::GetSysColor(COLOR_WINDOWTEXT)
#define PFX_EC_CLR_LINE_NUMBER                (COLORREF)::GetSysColor(COLOR_WINDOWTEXT)
#define PFX_EC_CLR_TEXT                       (COLORREF)::GetSysColor(COLOR_WINDOWTEXT)
#define PFX_EC_CLR_TEXT_SELECTED              (COLORREF)::GetSysColor(COLOR_HIGHLIGHTTEXT)
#define PFX_EC_CLR_WHITESPACE                 (COLORREF)0xFFCC66
#define PFX_EC_CLR_LINE_COMMENT1              (COLORREF)RGB(0, 128, 0)
#define PFX_EC_CLR_LINE_COMMENT2              (COLORREF)RGB(0, 128, 0)
#define PFX_EC_CLR_BLOCK_COMMENT1             (COLORREF)RGB(0, 128, 0)
#define PFX_EC_CLR_BLOCK_COMMENT2             (COLORREF)RGB(0, 128, 0)
#define PFX_EC_CLR_QUOTED_STRING1             (COLORREF)RGB(128, 128, 128)
#define PFX_EC_CLR_QUOTED_STRING2             (COLORREF)RGB(128, 128, 128)
#define PFX_EC_CLR_BK_TEXT                    (COLORREF)::GetSysColor(COLOR_WINDOW)
#define PFX_EC_CLR_BK_TEXT_SELECTED           (COLORREF)::GetSysColor(COLOR_HIGHLIGHT)
#define PFX_EC_CLR_BK_RULER                   (COLORREF)::GetSysColor(COLOR_3DFACE)
#define PFX_EC_CLR_BK_LINE_NUMBER             (COLORREF)::GetSysColor(COLOR_3DFACE)
#define PFX_EC_CLR_BK_BOOKMARK                (COLORREF)0xFFCC66
#define PFX_EC_CLR_BK_ACTIVE_LINE             (COLORREF)0x00FFFF 
// 目前不支持GB18030，VC7中的MB_LEN_MAX等于5，这会导致不能正常工作！
const LONG PFX_EC_MAX_MB_LEN = 2;
const LONG PFX_EC_MAX_TAB_SIZE = 64;
const LONG PFX_EC_MAX_LINE_LENGTH = 8192;
const LONG PFX_EC_TASKMEM_CONST_SIZE = 4096 * 8; // 32K
const LONG PFX_EC_TASKMEM_ALLOC_SIZE = 4096 * 4; // 16K
// 撤消/恢复描述
const WORD PFX_EC_UNDO_UNKNOWN = 0;
const WORD PFX_EC_UNDO_TYPING = 1;
const WORD PFX_EC_UNDO_REMOVE = 2;
const WORD PFX_EC_UNDO_INSERT = 3;
const WORD PFX_EC_UNDO_CUT = 4;
const WORD PFX_EC_UNDO_PASTE = 5;
const WORD PFX_EC_UNDO_REPLACE = 6;
const WORD PFX_EC_UNDO_REPLACE_ALL = 7;
const WORD PFX_EC_UNDO_DRAGDROP_COPY = 8;
const WORD PFX_EC_UNDO_DRAGDROP_MOVE = 9;
const WORD PFX_EC_UNDO_CASE_LOWER = 10;
const WORD PFX_EC_UNDO_CASE_UPPER = 11;
const WORD PFX_EC_UNDO_CASE_INVERT = 12;
const WORD PFX_EC_UNDO_CASE_CAPITALIZE = 13;
const WORD PFX_EC_UNDO_SPACES_TO_TABS = 14;
const WORD PFX_EC_UNDO_SPACES_TO_TABS_LEADING = 15;
const WORD PFX_EC_UNDO_TABS_TO_SPACES = 16;
const WORD PFX_EC_UNDO_TRIM_TRAILING_SPACES = 17;
// 文件转换器
const LONG PFX_EC_FILE_CONVERTER_AUTO_DETECT = 0;
const LONG PFX_EC_FILE_CONVERTER_ANSI = 1;
const LONG PFX_EC_FILE_CONVERTER_UTF8 = 2;
const LONG PFX_EC_FILE_CONVERTER_UNICODE = 3;
const LONG PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN = 4;
// 文档格式
const LONG PFX_EC_LINE_BREAK_TYPE_DOS = 0;
const LONG PFX_EC_LINE_BREAK_TYPE_UNIX = 1;
const LONG PFX_EC_LINE_BREAK_TYPE_MAC = 2;
const LONG PFX_EC_LINE_BREAK_SIZE_DOS = 2;
const LONG PFX_EC_LINE_BREAK_SIZE_UNIX = 1;
const LONG PFX_EC_LINE_BREAK_SIZE_MAC = 1;
// 换行标志
const LONG PFX_EC_WRAP_NONE = 0;
const LONG PFX_EC_WRAP_WINDOWEDGE = 1;
const LONG PFX_EC_WRAP_COLUMNS = 2;
// 粘贴方式
const LONG PFX_EC_PASTE_NORMAL = 0;
const LONG PFX_EC_PASTE_COLUMN_ALIGN = 1;
const LONG PFX_EC_PASTE_COLUMN_APPEND = 2;
// 打印范围
const LONG PFX_EC_PRINT_PAGES_ALL = 0;
const LONG PFX_EC_PRINT_PAGES_ODD = 1;
const LONG PFX_EC_PRINT_PAGES_EVEN = 2;
// 打印页眉和脚注的参数，要插入一个“&”可以使用“&&”
// 当前页
#define PFX_EC_PRINT_MACRO_CURRENT_PAGE       _T("&p") 
// 总页数
#define PFX_EC_PRINT_MACRO_TOTAL_PAGES        _T("&P") 
// 文件名
#define PFX_EC_PRINT_MACRO_FILE_NAME          _T("&f") 
// 路径名
#define PFX_EC_PRINT_MACRO_PATH_NAME          _T("&F") 
// 日期
#define PFX_EC_PRINT_MACRO_DATE               _T("&D") 
// 时间
#define PFX_EC_PRINT_MACRO_TIME               _T("&T") 
// 制表符
#define PFX_EC_PRINT_MACRO_TAB                _T("&t") 
// 回车换行
#define PFX_EC_PRINT_MACRO_CRLF               _T("&b") 
// 向左对齐
#define PFX_EC_PRINT_MACRO_ALIGN_LEFT         _T("&l") 
// 向右对齐
#define PFX_EC_PRINT_MACRO_ALIGN_RIGHT        _T("&r") 
// 居中对齐
#define PFX_EC_PRINT_MACRO_ALIGN_CENTER       _T("&c") 
// 事件通知消息，请参考CPfxEditDoc::OnEvent()函数
// 消息：PFX_EN_READ_ONLY_DOCUMENT
// 描述：文档只读不能修改
// LPARAM = 无意义
const LONG PFX_EN_READ_ONLY_DOCUMENT = 0x0001;
// 消息：PFX_EN_SEARCH_PASSED_END_OF_FILE
// 描述：搜索已达尽头（可能是末尾或开头）
// LPARAM = EOF越过末尾，否则越过开头
const LONG PFX_EN_SEARCH_PASSED_EOF = 0x0002;
// 消息：PFX_EN_WORD_WRAPPING
// 描述：正在计算自动换行
// LPARAM = 已经完成的百分比
// 返回0计算换行，返回非零终止换行计算
const LONG PFX_EN_WORD_WRAPPING = 0x0003;
// 错误消息
// 控件内部错误
const LONG PFX_EE_INTERNAL_ERROR = 0x0100;
// 系统未知错误
const LONG PFX_EE_UNKNOWN_ERROR = 0x0101;
// 消息：PFX_EE_OUT_OF_MEMORY
// 描述：系统内存不足
// LPARAM = 0
const LONG PFX_EE_OUT_OF_MEMORY = 0x0102;
// 消息：PFX_EE_OUT_OF_RESOURCE
// 描述：系统资源不足
// LPARAM = 0
const LONG PFX_EE_OUT_OF_RESOURCE = 0x0103;
// 消息：PFX_EE_CONVERT_ERR_ON_LOADING
// 描述：装入文件时对字符的转换出错
// LPARAM = 0
const LONG PFX_EE_CONVERT_ERR_ON_LOADING = 0x0104;
// 消息：PFX_EE_CONVERT_ERR_ON_SAVING
// 描述：保存文件时对字符的转换出错
// LPARAM = 0
const LONG PFX_EE_CONVERT_ERR_ON_SAVING = 0x0105;
// CPfxEditAlloc 用于CRT堆内的静态内存分配器
class CPfxEditAlloc {
public:
  class CAllocator {
  public:
    struct CNode {
      CNode* pNext;
    };
    struct CPlex {
      size_t nUsed;
      CPlex* pNext;
      CPlex* pPrev;
      CNode* pNodeFree;
      void* data() {
        return this + 1;
      }
    };
    size_t m_nMaxUnits;  // 每块内存最大能分配的单元数
    size_t m_nUnitSize;  // 单元大小（字节）
    size_t m_nBlockSize; // 每块内存的大小（字节）
    size_t m_nBlocks;    // 当前共有多少块内存
    CPlex* m_pBlocks;    // 内存块链头（双链表）
  public:
    CAllocator( size_t nUnitSize, size_t nMaxUnits = 64 );
    ~CAllocator() {
      FreeAll();
    }
  public:
    void* Alloc();
    void Free( void* lp );
    void FreeAll();
    size_t GetUnitSize() {
      return m_nUnitSize;
    }
  };
  CAllocator m_b1; // 8
  CAllocator m_b2; // 16
  CAllocator m_b3; // 24
  CAllocator m_b4; // 32
  CAllocator m_b5; // 64
  CAllocator m_b6; // 128
  CAllocator m_b7; // 256
  CAllocator m_b8; // 512
public:
  CPfxEditAlloc();
  ~CPfxEditAlloc();
public:
  void* Alloc( size_t cb );
  void Free( void* lp );
  void FreeAll();
};
// CPfxEditHeap 用于堆栈上的动态内存分配器
class CPfxEditHeap {
public:
  class CAllocator {
    class CNode {
    public:
      size_t nRef;
      size_t nSize;
      CNode* pNext;
      CNode* pPrev;
      void* data() {
        return reinterpret_cast<void*>( this + 1 );
      }
    };
    CNode* m_pNodes;
    size_t m_nNodes;
  public:
    CAllocator() {
      m_pNodes = NULL;
      m_nNodes = 0;
    }
    ~CAllocator() {
      FreeAll();
    }
  public:
    LPVOID Alloc( size_t cb );
    void Free( void* lp );
    void FreeAll();
  };
public:
  // 当前用户要求的缓冲区大小
  size_t m_nDataSize;
  // 当前实际分配的缓冲区大小(m_nBufferSize >= m_nDataSize)
  size_t m_nBufferSize;
  // 缓冲区地址
  void* m_pBuffer;
public:
  CPfxEditHeap() {
    m_pBuffer = NULL;
    m_nBufferSize = m_nDataSize = 0;
  }
  ~CPfxEditHeap() {
    Free();
  }
public:
  // 获取缓冲区地址
  void* GetBuffer() const {
    return m_pBuffer;
  }
  // 获取建议的缓冲区大小，使用这个大小可以加快速度
  size_t GetBufferConstSize() const {
    return PFX_EC_TASKMEM_CONST_SIZE;
  }
  // 分配
  void* Alloc( size_t cb );
  // 重新调整缓冲区大小并将原来的数据复制到新的缓冲区中
  void* ReAlloc( size_t cb );
  // 释放
  void Free();
};
// CPfxEditHash
const LONG PFX_EC_KEYWORD_MAP_HASH_TABLE_SIZE = 256; // HASH表长度
const LONG PFX_EC_KEYWORD_MAP_MAX_KEYWORD_LEN = 256; // 允许的最大语法关键字长度，超过该数值将被截断
class CPfxEditHash {
#pragma pack(push, 1)
  class CAssoc {
  public:
    CAssoc* pNext;
    LONG nKeyClass;
    LONG cchKey;
    TCHAR szKey[ 1 ];
  };
#pragma pack(pop)
  class CAssocInfo {
  public:
    UINT nHash;
    LPCTSTR pszKey;
    LONG cchKey;
    CAssoc* pAssoc;
  };
public:
  CPfxEditHash();
  ~CPfxEditHash();
public:
  // 找到返回COLOR INDEX，否则返回-1
  LONG Lookup( LPCTSTR pszKey, LONG cch );
  LONG GetCount() const;
  BOOL IsMatchCase() const;
  void SetMatchCase( BOOL bCase );
  void SetAt( LONG nKeyClass, LPCTSTR pszKey );
  void RemoveAt( LPCTSTR pszKey );
  void RemoveAll();
public:
  BOOL m_bMatchCase; // 关键字是否需要匹配大小写
  LONG m_nCount; // 关键字数目
  CAssoc* m_arrHashTable[ PFX_EC_KEYWORD_MAP_HASH_TABLE_SIZE ];
  TCHAR m_szKeyLower[ PFX_EC_KEYWORD_MAP_MAX_KEYWORD_LEN + 1 ];
public:
  // Hash数值管理
  UINT HashKey( LPCTSTR pKey, LONG cch ) const;
  BOOL GetAssocAt( LPCTSTR pKey, LONG cch, CAssocInfo& retVal );
};
__forceinline UINT CPfxEditHash::HashKey( LPCTSTR pKey, LONG cch ) const {
  UINT nHash = 0;
  LPCTSTR lp = pKey + min( cch, PFX_EC_KEYWORD_MAP_HASH_TABLE_SIZE );
  while ( pKey < lp ) {
    nHash = ( nHash << 5 ) + nHash + *pKey++;
  }
  return ( nHash % PFX_EC_KEYWORD_MAP_HASH_TABLE_SIZE );
}
__forceinline LONG CPfxEditHash::Lookup( LPCTSTR pszKey, LONG cch ) {
  CAssocInfo i;
  return ( GetAssocAt( pszKey, cch, i ) ? i.pAssoc->nKeyClass : -1 );
}
inline BOOL CPfxEditHash::IsMatchCase() const {
  return m_bMatchCase;
}
inline LONG CPfxEditHash::GetCount() const {
  return m_nCount;
}
inline void CPfxEditHash::SetMatchCase( BOOL bOn ) {
  m_bMatchCase = bOn;
}
// 保存了颜色和与之对应的名称，例如：
// {
//   {RGB(0, 255, 0), _T("书签颜色")},
//   {RGB(0, 0, 255), _T("C++关键字")},
// };
// 使用这个结构有比较大的灵活性，方便以后可以扩展其它特性，例如某种KeyClass
// 使用粗体，斜体等都可以在这里添加成员
class CPfxEditKeyClass {
public:
  bool m_bEmpty;
  COLORREF m_color;
  CString m_strClassName;
public:
  CPfxEditKeyClass() {
    SetEmpty();
  }
  CPfxEditKeyClass( LPCTSTR pszClassName, COLORREF color ) {
    m_bEmpty = false;
    m_color = color;
    m_strClassName = pszClassName;
  }
public:
  BOOL IsEmpty() const {
    return m_bEmpty;
  }
  BOOL IsClass( LPCTSTR pszClassName ) const {
    return ( ( pszClassName != NULL ) && ( IsEmpty() ) && ( m_strClassName == pszClassName ) );
  }
  void SetEmpty() {
    m_bEmpty = true;
    m_color = CLR_INVALID;
    m_strClassName.Empty();
  }
  CPfxEditKeyClass& operator=( const CPfxEditKeyClass& src ) {
    m_bEmpty = src.m_bEmpty;
    m_color = src.m_color;
    m_strClassName = src.m_strClassName;
    return *this;
  }
  bool operator==( const CPfxEditKeyClass& src ) {
    return ( ( ( m_bEmpty && src.m_bEmpty ) || ( !m_bEmpty && !src.m_bEmpty ) )
             && ( m_color == src.m_color )
             && ( m_strClassName == src.m_strClassName ) );
  }
  bool operator!=( const CPfxEditKeyClass& src ) {
    return ( ( ( m_bEmpty && !src.m_bEmpty ) || ( !m_bEmpty && src.m_bEmpty ) )
             || ( m_color != src.m_color ) || ( m_strClassName != src.m_strClassName ) );
  }
};
class CPfxEditMarkerPair // 这是变长度结构体
{
public:
  LPCTSTR pszOn;
  LONG cchOn;
  WCHAR wLowerOn;
  WCHAR wUpperOn;
  LPCTSTR pszOff;
  LONG cchOff;
  WCHAR wLowerOff;
  WCHAR wUpperOff;
  CPfxEditMarkerPair* pNext;
  CPfxEditMarkerPair* pPrev;
  TCHAR szKey[ sizeof( int ) ];
};
enum EPfxEditPredefinedKeyClass
{
  kcLineNumber = 0,
  kcRuler,
  kcText,
  kcTextSelected,
  kcWhitespace,
  kcBkText,
  kcBkLineNumber,
  kcBkRuler,
  kcBkTextSelected,
  kcBkActiveLine,
  kcBkBookmark,
  kcHeaderAndFooter,
  kcQuotedString1,
  kcQuotedString2,
  kcLineComment1,
  kcLineComment2,
  kcBlockComment1,
  kcBlockComment2,
  kcPredefinedKeyClassNumber = 18,
  kcKeyClassArraySize = 36,
  kcUserCustomKeyClassNumber = ( kcKeyClassArraySize - kcPredefinedKeyClassNumber ),
};
enum EPfxEditMarkerPair
{
  // 行注释
  mpLineComment1 = 0,
  mpLineComment2,
  // 块注释开始和结束标识
  mpBlockComment1,
  mpBlockComment2,
  // 定义字符串的开始和结束标识
  mpQuotedString1,
  mpQuotedString2,
  // 配对标识符，例如“{}”，“[]”，“()”等
  mpBraceStrings,
  mpMarkerPairNumber = 0x07,
};
class CKeyClassArray {
public:
  CPfxEditKeyClass m_arrClasses[ kcKeyClassArraySize ];
public:
  CKeyClassArray();
  ~CKeyClassArray();
public:
  // 判断给定的索引号是否是有效的关键字组
  BOOL IsValidClass( LONG nIndex ) const {
    return ( nIndex >= 0 ) && ( nIndex < PFX_EC_SIZEOF( m_arrClasses ) );
  }
  COLORREF GetClassColor( LONG nIndex ) const {
    ASSERT( IsValidClass( nIndex ) );
    return m_arrClasses[ nIndex ].m_color;
  }
  void SetClassColor( LONG nIndex, COLORREF color ) {
    ASSERT( IsValidClass( nIndex ) );
    m_arrClasses[ nIndex ].m_color = color;
  }
  // 获取关键字分组信息
  POSITION GetClassFirst() const;
  void GetClassNext( POSITION& rPosition, CPfxEditKeyClass& theClass ) const;
  // 添加关键字-颜色定义对，返回已保存的索引号
  LONG SetClass( const CPfxEditKeyClass& theClass );
  LPCTSTR GetClassName( LONG nIndex ) const;
  void SetClassName( LONG nIndex, LPCTSTR pszClassName );
  // 删除一单元
  void RemoveClass( LONG nIndex );
  void RemoveClass( LPCTSTR pszClassName );
  // 复制
  CKeyClassArray& operator=( const CKeyClassArray& src );
};
class CMarkerPairArr {
public:
  CPfxEditMarkerPair* m_arrMarkers[ mpMarkerPairNumber ];
public:
  CMarkerPairArr();
  ~CMarkerPairArr();
public:
  BOOL IsValidMarker( LONG nIndex ) const {
    return ( nIndex >= 0 ) && ( nIndex < PFX_EC_SIZEOF( m_arrMarkers ) );
  }
  CPfxEditMarkerPair* GetMarker( EPfxEditMarkerPair id ) const {
    return ( IsValidMarker( id ) ? m_arrMarkers[ id ] : NULL );
  }
  // 全部释放
  void RemoveAll();
  // 释放一节点链
  void FreeMarkerChain( CPfxEditMarkerPair* pMarker );
  // 复制一节点链
  CPfxEditMarkerPair* CopyMarkerChain( CPfxEditMarkerPair* pMarker );
  // 创建新节点
  CPfxEditMarkerPair* CreateMarker( LPCTSTR pszOn, LPCTSTR pszOff );
  // 复制
  CMarkerPairArr& operator=( const CMarkerPairArr& src );
};
// CPfxEditTypeInfo 保存语法信息
class CPfxEditTypeInfo {
public:
public:
  CPfxEditTypeInfo();
  virtual ~CPfxEditTypeInfo();
public:
  BOOL m_bDrawHilighted;
  BOOL m_bDrawRuler;
  BOOL m_bDrawLineNumber;
  BOOL m_bDrawLineBreak;
  BOOL m_bDrawWhitespace;
  BOOL m_bEnableDragDrop;
  BOOL m_bExpandTabToSpaceChars;
  BOOL m_bHomeKeyGotoFirstNonWhitespaceChar;
  BOOL m_bSyncTextBkColorWithSystem;
  BOOL m_bSyncActiveLineBkColorWithTextBkColor;
  // 制表符的长度
  LONG m_nTabSize;
  // 自动缩进长度
  LONG m_nIndentSize;
  // 换行方法
  LONG m_nWrapStyle;
  // 换行宽度
  LONG m_nWrapWidth;
  // 关键字与颜色的映射表
  CPfxEditHash m_mapKeywords;
  // 关键字组信息（颜色表）
  CKeyClassArray m_arrKeyClasses;
  // 配对字符串
  CMarkerPairArr m_arrMarkerPair;
  // 转义字符
  WCHAR m_wEscape;
  // 单词定界符
  WCHAR m_arrDelimiters[ 64 ];
public:
  CPfxEditTypeInfo& operator=( const CPfxEditTypeInfo& src );
  // 获取单词定界符
  CString GetDelimiters() const;
  static LPCTSTR GetDelimitersDefault();
  BOOL IsEscape( WCHAR wc ) const;
  BOOL IsMatchCase() const;
  BOOL IsDrawHilighted() const;
  void SetDrawHilighted( BOOL bOn );
  // 设置关键字是否为匹配大小写
  void SetMatchCase( BOOL bCase );
  // 设置单词定界符
  void SetDelimiters( LPCTSTR pszDelimiters );
  // 设置转义字符（对于C++来说是_T("\\")）
  void SetEscape( LPCTSTR pszEscape );
  // 关键字组信息（颜色表）
  // 判断给定的索引号是否是有效的关键字组
  BOOL IsValidKeyClass( LONG nIndex ) const;
  // 获取关键字分组信息
  POSITION GetKeyClassFirst() const;
  void GetKeyClassNext( POSITION& rPosition, CPfxEditKeyClass& theClass ) const;
  // 添加关键字-颜色定义对，返回已保存的索引号
  LONG SetKeyClass( const CPfxEditKeyClass& theClass );
  void RemoveKeyClass( LONG nIndex );
  void RemoveKeyClass( LPCTSTR pszClassName );
  COLORREF GetKeyClassColor( LONG Index ) const;
  void SetKeyClassColor( LONG nIndex, COLORREF color );
  LPCTSTR GetKeyClassName( LONG nIndex ) const;
  void SetKeyClassName( LONG nIndex, LPCTSTR pszClassName );
  // 关键字管理
  // 获取关键字数量
  LONG GetKeywordCount() const;
  // 添加关键字，颜色索引必须是SetKeyClass()的非负数返回值
  void SetKeyword( LONG nKeyClass, LPCTSTR pszKeyword );
  // 语法配置信息
  BOOL HasMarkerPair( EPfxEditMarkerPair id ) const;
  // 设置配对标识符，例如“{}”，“[]”，“()”等
  CPfxEditMarkerPair* GetMarkerPair( EPfxEditMarkerPair id ) const;
  void SetMarkerPair( EPfxEditMarkerPair id, LPCTSTR pszOn, LPCTSTR pszOff );
  void AppendMarkerPair( EPfxEditMarkerPair id, LPCTSTR pszOn, LPCTSTR pszOff );
};
inline BOOL CPfxEditTypeInfo::IsMatchCase() const {
  return m_mapKeywords.IsMatchCase();
}
inline BOOL CPfxEditTypeInfo::IsDrawHilighted() const {
  return m_bDrawHilighted;
}
inline BOOL CPfxEditTypeInfo::IsEscape( WCHAR wc ) const {
  return ( wc == m_wEscape );
}
inline LPCTSTR CPfxEditTypeInfo::GetDelimitersDefault() {
  return _T( ",.?!;:/<>(){}[]\"\'" );
}
inline void CPfxEditTypeInfo::SetMatchCase( BOOL bOn ) {
  m_mapKeywords.SetMatchCase( bOn );
}
inline void CPfxEditTypeInfo::SetDrawHilighted( BOOL bOn ) {
  m_bDrawHilighted = bOn;
}
inline LONG CPfxEditTypeInfo::GetKeywordCount() const {
  return m_mapKeywords.GetCount();
}
inline void CPfxEditTypeInfo::SetKeyword( LONG nKeyClass, LPCTSTR pszKeyword ) {
  m_mapKeywords.SetAt( nKeyClass, pszKeyword );
}
inline BOOL CPfxEditTypeInfo::IsValidKeyClass( LONG nIndex ) const {
  return m_arrKeyClasses.IsValidClass( nIndex );
}
inline POSITION CPfxEditTypeInfo::GetKeyClassFirst() const {
  return m_arrKeyClasses.GetClassFirst();
}
inline void CPfxEditTypeInfo::GetKeyClassNext( POSITION& rPosition, CPfxEditKeyClass& theClass ) const {
  m_arrKeyClasses.GetClassNext( rPosition, theClass );
  return ;
}
inline LONG CPfxEditTypeInfo::SetKeyClass( const CPfxEditKeyClass& theClass ) {
  return m_arrKeyClasses.SetClass( theClass );
}
inline void CPfxEditTypeInfo::RemoveKeyClass( LONG nIndex ) {
  m_arrKeyClasses.RemoveClass( nIndex );
}
inline void CPfxEditTypeInfo::RemoveKeyClass( LPCTSTR pszClassName ) {
  m_arrKeyClasses.RemoveClass( pszClassName );
}
inline COLORREF CPfxEditTypeInfo::GetKeyClassColor( LONG nIndex ) const {
  ASSERT( IsValidKeyClass( nIndex ) );
  return m_arrKeyClasses.GetClassColor( nIndex );
}
inline void CPfxEditTypeInfo::SetKeyClassColor( LONG nIndex, COLORREF color ) {
  ASSERT( IsValidKeyClass( nIndex ) );
  m_arrKeyClasses.SetClassColor( nIndex, color );
}
inline LPCTSTR CPfxEditTypeInfo::GetKeyClassName( LONG nIndex ) const {
  ASSERT( IsValidKeyClass( nIndex ) );
  return m_arrKeyClasses.GetClassName( nIndex );
}
inline void CPfxEditTypeInfo::SetKeyClassName( LONG nIndex, LPCTSTR pszClassName ) {
  ASSERT( IsValidKeyClass( nIndex ) );
  m_arrKeyClasses.SetClassName( nIndex, pszClassName );
}
inline BOOL CPfxEditTypeInfo::HasMarkerPair( EPfxEditMarkerPair id ) const {
  return m_arrMarkerPair.GetMarker( id ) != NULL;
}
inline CPfxEditMarkerPair* CPfxEditTypeInfo::GetMarkerPair( EPfxEditMarkerPair id ) const {
  return m_arrMarkerPair.GetMarker( id );
}
// CPfxEditPrintInfo 保存打印信息
class CPfxEditPrintInfo {
public:
  // 本次操作是否用于打印预览（参考）
  BOOL m_bPreview;
  // 是否打印语法高亮，只有当当前文档是程序文件才有效
  BOOL m_bPrintHilighted;
  // 绘制行号（可选设置）
  BOOL m_bPrintLineNumber;
  // 打印标题分隔线
  BOOL m_bPrintHeaderSeparator;
  // 打印脚注分隔线
  BOOL m_bPrintFooterSeparator;
  // 打印机和屏幕显示DC，派生类不应修改这两个成员的属性
  HDC m_pdcPrinter;
  HDC m_pdcDisplay;
  // 显示字体
  HFONT m_pFontDisplay;
  // 自动换行（可选设置，参见SetWrapStyle()）
  LONG m_nWrapStyle;
  // 打印全部或只打印奇数页或偶数页（可选设置）
  LONG m_nPrintRange;
  // 从该页开始打印，这是一个参考值，该数值是从打印对话框返回的，如果
  // 设置的数值超出了有效范围，该成员被替换有效值（可选设置）
  LONG m_nFromPage;
  // 至该页结束，这是一个参考值，该数值是从打印对话框返回的，如果设置
  // 的数值超出了最大值，该成员被替换成最大值（可选设置）
  LONG m_nToPage;
  // 页眉和正文的距离
  LONG m_nSpacingHeaderAndText;
  // 页脚与正文的距离
  LONG m_nSpacingFooterAndText;
  // 系统本地时间（可选设置）
  SYSTEMTIME m_time;
  // 打印纸可打印范围，逻辑单元（参考）
  CRect m_rectDraw;
  // 文本缩进量，单位：逻辑单元（可选设置）
  CRect m_rectMargin;
  // 打印参考字体，默认地它等于显示字体，派生类可以用新的字体据柄代替
  // 它但不能删除它，因为它是屏幕显示用的字体。如果派生类不修改它，则
  // CPfxEditView 会根据打印DC和显示DC之间的比例来创建打印字体。如果派
  // 生类用自己的字体据柄替代了该字体据柄，派生类不应该保存该字体，因
  // 为CPfxEditView一打印完毕就删除该字体（可选设置）
  CFont m_fontText;
  // 标题和脚注字体，等于NULL时使用打印字体，派生类可以指定新的字体据
  // 柄，CPfxEditView一打印完毕就删除该字体（可选设置）
  CFont m_fontHeaderAndFooter;
  // 用于绘制水平分隔线，将其置NULL则不绘制。CPfxEditView 一打印完毕就
  // 删除该资源（可选设置）
  CPen m_penSeparateLine;
  // 用于绘制边框，将其置NULL则不绘制，CPfxEditView 一打印完毕就删除该
  // 资源（可选设置）
  CPen m_penMargin;
  // 标题格式化字符串（可选设置）
  CString m_strHeader;
  // 脚注格式化字符串（可选设置）
  CString m_strFooter;
  // 文件名（可选设置）
  CString m_strPathName;
  CPfxEditKeyClass m_arrKeyClasses[ kcKeyClassArraySize ];
  COLORREF COLOR( LONG nIndex ) const {
    return m_arrKeyClasses[ nIndex ].m_color;
  }
public:
  // 打印DC对显示DC的缩放比例系数
  FLOAT m_fRatioX;
  FLOAT m_fRatioY;
  // 打印机在当前设置下每水平英寸包含的逻辑单元数
  FLOAT m_fLogUnitsPerInchX;
  // 打印机在当前设置下每垂直英寸包含的逻辑单元数
  FLOAT m_fLogUnitsPerInchY;
  // 打印机在当前设置下每水平CM包含的逻辑单元数
  FLOAT m_fLogUnitsPerCmX;
  // 打印机在当前设置下每垂直CM包含的逻辑单元数
  FLOAT m_fLogUnitsPerCmY;
public:
  CPfxEditPrintInfo();
  virtual ~CPfxEditPrintInfo();
public:
  BOOL IsPreview() const;
  BOOL IsPrintHilighted() const;
  BOOL IsPrintHeader() const;
  BOOL IsPrintFooter() const;
  BOOL IsPrintMargin() const;
  BOOL IsPrintLineNumber() const;
  BOOL IsPrintHeaderSeparator() const;
  BOOL IsPrintFooterSeparator() const;
  // 直接使用已有的字体作为打印字体
  BOOL SetPrinterFont( HFONT pFont );
  // 用映射显示字体的方法创建打印字体
  BOOL SetMapDisplayFont();
  void SetDC( HDC hdcDisplay, HDC hdcPrinter );
  void SetPrintPagesAll();
  void SetPrintPagesEven();
  void SetPrintPagesOdd();
  // 显示DC逻辑单元映射成打印DC逻辑单元
  LONG PrinterLogicUnitFromDisplayX( LONG x );
  LONG PrinterLogicUnitFromDisplayY( LONG y );
  void PrinterLogicUnitFromDisplay( CRect& rect );
  void PrinterLogicUnitFromDisplay( CPoint& rpt );
  // 将英寸转换成逻辑单元
  LONG PrinterLogicUnitFromInchX( FLOAT fInch );
  LONG PrinterLogicUnitFromInchY( FLOAT fInch );
  // 将厘米转换成逻辑单元
  LONG PrinterLogicUnitFromCmX( FLOAT fCM );
  LONG PrinterLogicUnitFromCmY( FLOAT fCM );
  HGDIOBJ FontText() const;
  HGDIOBJ FontHeaderAndFooter() const;
  HGDIOBJ PenSeparateLine() const;
  HGDIOBJ PenMargin() const;
  HFONT GetFontText() const;
  HFONT GetFontHeaderAndFooter() const;
  HPEN GetPenSeparateLine() const;
  HPEN GetPenMargin() const;
};
inline BOOL CPfxEditPrintInfo::IsPreview() const {
  return m_bPreview;
}
inline BOOL CPfxEditPrintInfo::IsPrintHilighted() const {
  return m_bPrintHilighted;
}
inline BOOL CPfxEditPrintInfo::IsPrintHeader() const {
  return !m_strHeader.IsEmpty();
}
inline BOOL CPfxEditPrintInfo::IsPrintFooter() const {
  return !m_strFooter.IsEmpty();
}
inline BOOL CPfxEditPrintInfo::IsPrintMargin() const {
  return m_penMargin != NULL;
}
inline BOOL CPfxEditPrintInfo::IsPrintLineNumber() const {
  return m_bPrintLineNumber;
}
inline BOOL CPfxEditPrintInfo::IsPrintHeaderSeparator() const {
  return m_bPrintHeaderSeparator;
}
inline BOOL CPfxEditPrintInfo::IsPrintFooterSeparator() const {
  return m_bPrintFooterSeparator;
}
inline void CPfxEditPrintInfo::SetPrintPagesAll() {
  m_nPrintRange = PFX_EC_PRINT_PAGES_ALL;
}
inline void CPfxEditPrintInfo::SetPrintPagesEven() {
  m_nPrintRange = PFX_EC_PRINT_PAGES_EVEN;
}
inline void CPfxEditPrintInfo::SetPrintPagesOdd() {
  m_nPrintRange = PFX_EC_PRINT_PAGES_ODD;
}
inline HFONT CPfxEditPrintInfo::GetFontText() const {
  return ( m_fontText );
}
inline HFONT CPfxEditPrintInfo::GetFontHeaderAndFooter() const {
  return ( m_fontHeaderAndFooter );
}
inline HPEN CPfxEditPrintInfo::GetPenSeparateLine() const {
  return ( m_penSeparateLine );
}
inline HPEN CPfxEditPrintInfo::GetPenMargin() const {
  return ( m_penMargin );
}
inline HGDIOBJ CPfxEditPrintInfo::FontText() const {
  return ( HGDIOBJ ) m_fontText;
}
inline HGDIOBJ CPfxEditPrintInfo::FontHeaderAndFooter() const {
  return ( HGDIOBJ ) m_fontHeaderAndFooter;
}
inline HGDIOBJ CPfxEditPrintInfo::PenSeparateLine() const {
  return ( HGDIOBJ ) m_penSeparateLine;
}
inline HGDIOBJ CPfxEditPrintInfo::PenMargin() const {
  return ( HGDIOBJ ) m_penMargin;
}
#pragma warning (disable: 4244)
inline LONG CPfxEditPrintInfo::PrinterLogicUnitFromDisplayX( LONG x ) {
  ASSERT( m_fRatioX != 0 );
  return ( LONG ) ( m_fRatioX * x );
}
inline LONG CPfxEditPrintInfo::PrinterLogicUnitFromDisplayY( LONG y ) {
  ASSERT( m_fRatioY != 0 );
  return ( LONG ) ( m_fRatioY * y );
}
inline LONG CPfxEditPrintInfo::PrinterLogicUnitFromInchX( FLOAT fInch ) {
  return ( LONG ) ( fInch * m_fLogUnitsPerInchX );
}
inline LONG CPfxEditPrintInfo::PrinterLogicUnitFromInchY( FLOAT fInch ) {
  return ( LONG ) ( fInch * m_fLogUnitsPerInchY );
}
inline LONG CPfxEditPrintInfo::PrinterLogicUnitFromCmX( FLOAT fCM ) {
  return ( LONG ) ( fCM * m_fLogUnitsPerCmX );
}
inline LONG CPfxEditPrintInfo::PrinterLogicUnitFromCmY( FLOAT fCM ) {
  return ( LONG ) ( fCM * m_fLogUnitsPerCmY );
}
#pragma warning (default: 4244) 
// CPfxEditData
class CPfxEditData {
public:
  CPfxEditData();
  ~CPfxEditData();
public:
  LONG m_nCharWidthTab;        // 正文字符TAB字符宽度
  LONG m_nCharWidthSpace;      // 正文字符空格字符宽度
  LONG m_nCharWidthLineBreak;  // 正文字符虚拟换行符宽度
  LONG m_nCharWidthMaxNum;     // 正文字符中最大数字字符宽度
  LONG m_nCharWidthAve;        // 正文字符平均宽度
  LONG m_nCharWidthMax;        // 正文字符最大宽度
  LONG m_nCharHeight;          // 正文字符高度
  LONG m_nLineHeight;          // 行高 = (m_nCharHeight + m_nSpacingLines)
  LONG m_nSpacingLines;        // 行间距
  LONG m_nSpacingLineNumberAndText; // 行号区与正文区的间距
  LONG m_nRulerCharHeight;     // 标尺字符高度
  LONG m_nRulerCharWidthAve;   // 标尺字符平均宽度
  LONG m_nRulerHeight;         // 标尺高度
  CSize m_sizeCaret;           // 当前插入符号尺寸
  // 它是用来标识是否可以实施列块粘贴，如果它等于剪贴板当前的据柄，则
  // 表明当前剪贴板数据是CPfxEditView放上去的，可以进行列块粘贴，否则
  // 就不行，所以一定不要删除该据柄
  HGLOBAL m_hDataClipboardCopied;
  DWORD m_nDataClipboardCopied; // 复制到剪贴板的数据大小
  // OLE拖放据柄的标识，用来标识数据对象。如果“放”在CPfxEditView内，
  // 则该标识用来指示是否进行列块操作
  HGLOBAL m_hDataDragDropCopied;
  HFONT m_hFontText;            // 共享资源，由属主删除
  HCURSOR m_hCurText;             // 用于编辑区
  HCURSOR m_hCurLineNumber;       // 用于行号
  HCURSOR m_hCurColSelect;        // 指示可以进行列块操作
  HCURSOR m_hCurStandard;         // 系统标准光标
  CPfxEditAlloc m_alloc;
  CPfxEditHeap::CAllocator m_heap;
  CPfxEditTypeInfo m_defTypeInfo;
  DWORD m_arrLeadByte[ UCHAR_MAX ]; // 为了数据对准使用DWORD
public:
  void* MemAlloc( size_t cb ) {
    return m_alloc.Alloc( cb );
  }
  void MemFree( void* lp ) {
    m_alloc.Free( lp );
  }
};
extern CPfxEditData PFX_EC_DATA;
// CPfxEditLineInfo
struct CPfxEditLineInfo {
  // 行标志，指示该行是否有书签，是否是子行等
  DWORD dwFlags;
  // 主行行号
  LONG nIndex;
  // 逻辑列。如果是主行，它等于零，如果是子行，它等于该子行第一个字符到主行行首的逻辑偏移量（包含TAB扩展）
  LONG nFirstChar;
  // 行长度，如果是主行且没有子行它等于nActLen
  LONG nLen;
  // 如果是主行它是整行（包括子行）的长度，如果是子行它是到主行的行偏移量
  LONG nActLen;
  // 一行中选择文本的起始和终点位置，nSelTo可能超过本行长度，表明选择内容包括换行符
  LONG nSelFrom;
  LONG nSelTo;
  // 指向数据的指针
  LPTSTR pData;
  // 行缓冲区所在的内存块的指针。注意：它仅由内存管理模块使用，正常情况下你不应该使用
  LPVOID pMemBlock;
};
// CPfxEditUndoInfo
struct CPfxEditUndoInfo {
  // 操作类型
  WORD fAction;
  // 操作名称
  WORD fUndoName;
  // X等于从主行行首计起的字符偏移量，Y等于绝对行号
  POINT ptFrom;
  POINT ptTo;
  // 缓冲区的长度
  LONG nLen;
  // 如果是插入操作，它是缓冲区的指针，否则应该为NULL
  LPTSTR pData;
  // 如果是插入操作，它是缓冲区所在内存块的指针，否则应该为NULL。注意：它仅由内存管理模块使用，正常情况下你不应该使用
  LPVOID pMemBlock;
};
struct CPfxEditDocMemBlock {
  // 有多少TCHAR空间被丢弃
  LONG nUnused;
  // 当丢弃的空间达到这个数目时就要执行压缩操作
  LONG nMaxUnused;
  // 剩余多少TCHAR空间可用
  LONG nRemain;
  // 该内存块总的长度
  LONG nLength;
  // 剩余的可用空间指针
  LPTSTR pRemain;
  // 链表的下一块
  CPfxEditDocMemBlock* pNext;
  // 链表的上一块
  CPfxEditDocMemBlock* pPrev;
};
struct CPfxEditDocLineInfo {
  // 行数组单元数
  LONG nli;
  // 行数组剩余可用的单元数
  LONG nliRemain;
  // 如果要增加行信息数组的长度，每次应该增加的数目
  LONG nliGrowBy;
  // 行数组数据指针
  CPfxEditLineInfo* pli;
  // 内存池指针
  CPfxEditDocMemBlock* pmb;
};
struct CPfxEditDocUndoInfo {
  // 当前已用的单元数
  LONG nui;
  // 当前剩余的单元数
  LONG nuiRemain;
  // 每次增加的缓冲区长度
  LONG nuiGrowBy;
  // 最大UNDO次数
  LONG nuiMax;
  // 当前UNDO位置
  LONG nPosCur;
  // 标记为文档未修改状态的UNDO位置
  LONG nPosUnmodified;
  // UNDO名称
  WORD nGroupName;
  // 锁定计数
  BYTE nGroupLockCount;
  // 是否是一UNDO组的第一个记录
  BOOL bGroupBegin;
  // UNDO线性链指针
  CPfxEditUndoInfo* pui;
  // 内存池指针
  CPfxEditDocMemBlock* pmb;
};
class CPfxEditDocFile {
  HANDLE m_hMapping;
  LONG m_nMappingSize;
  LPVOID m_pMappingAddress;
public:
  CPfxEditDocFile();
  ~CPfxEditDocFile();
public:
  BOOL MapFile( HANDLE hFile, LONG nSize );
  BOOL MapMemory( LONG nSize ) {
    return MapFile( ( HANDLE ) 0xFFFFFFFF, nSize );
  }
  LONG GetSize() const {
    return m_nMappingSize;
  }
  LPVOID GetBuffer() const {
    return m_pMappingAddress;
  }
  void SetSize( LONG nSize ) {
    if ( ( nSize >= 0 ) && ( nSize < m_nMappingSize ) ) {
      m_nMappingSize = nSize;
    }
  }
  void Close();
};
class CPfxEditDocUtf8Converter {
public:
  // 检测给定的缓冲区是否是正确的UTF8编码数据
  BOOL IsUtf8FromUnicode( LPCSTR pUtf8, LONG cch ) const;
  // 获取给定的UTF8字符长度
  LONG Utf8CharSize( BYTE b ) const {
    // 下列字节串用来表示一个字符. 用到哪个串取决于该字符在 Unicode 中的序号.
    // U+00000000-U+0000007F: 0xxxxxxx
    // U+00000080-U+000007FF: 110xxxxx 10xxxxxx
    // U+00000800-U+0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
    // U+00010000-U+001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    // U+00200000-U+03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    // U+04000000-U+7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    // U+00000000-U+0000007F: 0xxx-xxxx
    // 0000-0000->0111-1111 = 0x00->0x7F
    if ( b <= 0x7F ) {
      return 1;
    }
    // U+00000080-U+000007FF: 110x-xxxx
    // 1100-0000->1101-1111 = 0xC0->0xDF
    else if ( ( b >= 0xC0 ) && ( b <= 0xDF ) ) {
      return 2;
    }
    // U+00000800-U+0000FFFF: 1110-xxxx
    // 1110-0000->1110-1111 = 0xE0->0xEF
    else if ( ( b >= 0xE0 ) && ( b <= 0xEF ) ) {
      return 3;
    } else // 不支持UCS4编码！
    {
      return 0;
    }
  }
  // 当UTF8字符转换为UNICODE时所需的缓冲区长度（单位：字符）
  LONG Utf8ToUnicodeCharSize( BYTE b ) const {
    UNUSED( b );
    return 1; // 不支持UCS4！
  }
  // 将UTF8字符转换为UNICODE字符或者计算需要多大的输出缓冲区
  LONG Utf8ToUnicode( LPCSTR pUtf8, LONG cchUtf8, LPWSTR pUnicode, LONG cchUnicode ) const;
  // 将UNICODE字符转换为UTF8字符所需的缓冲区长度（单位：字节）
  LONG UnicodeToUtf8CharSize( WORD w ) const {
    // 下列字节串用来表示一个字符. 用到哪个串取决于该字符在 Unicode 中的序号.
    // U+00000000-U+0000007F: 0xxxxxxx
    // U+00000080-U+000007FF: 110xxxxx 10xxxxxx
    // U+00000800-U+0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
    // U+00010000-U+001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    // U+00200000-U+03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    // U+04000000-U+7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    if ( w < 0x0080 ) {
      return 1; // ANSI CHAR
    }
    if ( w < 0x0800 ) {
      return 2;
    } else {
      return 3;
    }
  }
  // 将UNICODE字符转换为UTF8字符或者计算需要多大的输出缓冲区
  LONG UnicodeToUtf8( LPCWSTR pUnicode, LONG cchUnicode, LPSTR pUtf8, LONG cchUtf8 ) const;
};
// CPfxEditDoc
class CPfxEditDoc : public CDocument {
public:
  CPfxEditDoc();           // public constructor used by dynamic creation
  // Attributes
public:
  BOOL IsCanUndo() const;
  BOOL IsCanRedo() const;
  BOOL IsCanReload() const;
  BOOL IsReadOnly() const;
  BOOL IsValidLine( LONG nLine ) const;
  BOOL IsValidChar( LONG nLine, LONG nChar ) const;
public:
  LONG GetValidLine( LONG nLine ) const;
  LONG GetValidChar( LONG nLine, LONG nChar ) const;
  LONG GetOpenConverter() const;
  LONG GetFileConverter() const;
  LONG GetLineBreakSize() const;
  LONG GetLineBreakType() const;
  LONG GetBookmarkCount() const;
  LONG GetDocSize() const;
  LONG GetLineCount() const;
  LONG GetLineUpperBound() const;
  LONG GetLineLength( LONG nLine ) const;
  LONG GetLineData( LONG nLine, LPTSTR pBuffer, LONG nBufMax ) const;
  LONG GetUndoName() const;
  LONG GetRedoName() const;
  // Operations
public:
  void SetReadOnly( BOOL bOn );
  void SetLineBreakType( LONG nType );
  void SetFileConverter( LONG nConverter );
  void SetOpenConverter( LONG nConverter );
  BOOL SetTypeInfoPtr( CPfxEditTypeInfo* pTypeInfo );
  const CPfxEditTypeInfo* GetTypeInfoPtr() const;
  BOOL Reload();
  void UpdateAllViews( BOOL bRecalcWrap );
  // Overrides
public:
  // ClassWizard generated virtual function overrides
public:
  virtual void Serialize( CArchive& ar ); // overridden for document i/o
  virtual void DeleteContents();
  virtual void SetModifiedFlag( BOOL bModified );
  virtual void OnChangedViewList();
  virtual BOOL OnNewDocument();
  virtual BOOL OnOpenDocument( LPCTSTR pszPathName );
  virtual BOOL OnSaveDocument( LPCTSTR pszPathName );
  // 编辑控件和（派生类）使用者交换事件处理信息
  // UINT nCode    - 请参考PFX_EN_XX消息
  // LPARAM lParam - 与该nCode消息相关的附加参数
  virtual LONG OnEvent( UINT nCode, LPARAM lParam );
  // 如果是语法高亮编辑，可以根据文件类型装入语法相关的设置等
  virtual BOOL OnInitDocument( LPCTSTR pszPathName );
  // Implementation
public:
  virtual ~CPfxEditDoc();
public:
  BOOL m_bDrawRuler;       // 是否显示标尺
  BOOL m_bDrawLineNumber;  // 是否显示行号
  BOOL m_bDrawWhitespace;  // 是否显示空白字符
  BOOL m_bDrawLineBreak;   // 是否绘制换行符
  BOOL m_bReadOnly;        // 文档是否为只读
  // 缓冲区（包括行缓冲和UNDO缓冲区）是否已经被锁定，不允许执行压缩操
  // 作，这种情况发生在一次批处理中，例如“全部替换”操作，如果在一次
  // 较大的批处理中频繁地执行缓冲区压缩，必定会导致速度很慢，所以压缩
  // 操作将会在以后进行
  BOOL m_bLockedBufferCompact;
  // 禁止换行进度消息传递给OnEvent()
  BOOL m_bLockedWordWrapEvent;
  LONG m_nLineLastParsed;  // 语法高亮时最后一个已分析的行
  // 语法分析时最后一已分析行但该行被新近的编辑操作所丢弃
  LONG m_nLineLastParsedButCanceled;
  LONG m_nWrapStyle;       // 换行方法
  LONG m_nWrapWidth;       // 换行宽度
  LONG m_nLineBreakSize;   // 换行符的长度，单位：TCHAR
  LONG m_nLineBreakType;   // 换行符的类型DOS、Mac、Unix，其实是文档的类型
  LONG m_nOpenConverter;   // 打开时文件转换器
  LONG m_nSaveConverter;   // 保存时文件转换器
  // 文档参数
  LONG m_nBookmarks;       // 文档中当前的书签数
  LONG m_nDocLineCount;    // 主行行数
  LONG m_nDocSize;         // 文档当前大小，单位：TCHAR
  CRect m_rectIndent;       // 文本区各边缩进量，目前定义了left、top
public:
  // 最多只允许两个视图，而且建议这两个视图的布局是两行一列。之所以建
  // 议使用这种布局，是因为涉及到文本换行问题，使用一行两列或两行两列
  // 都会产生矛盾
  // |======================|
  // |                      |
  // | 视图1                |
  // |                      |
  // |======================|
  // |                      |
  // | 视图2                |
  // |                      |
  // |======================|
  CPfxEditView* m_arrViews[ 2 ];
  CPfxEditDocLineInfo m_clsLine;
  CPfxEditDocUndoInfo m_clsUndo;
public:
  BOOL IsDrawRuler() const;
  BOOL IsDrawLineNumber() const;
  BOOL IsDrawLineBreak() const;
  BOOL IsDrawWhitespace() const;
  BOOL IsWrapping() const;
  BOOL IsWrapAtWindowEdge() const;
  BOOL IsWrapAfterColumns() const;
  LONG GetWrapStyle() const;
  LONG GetWrapWidth() const;
  LONG GetLineBreakSizeFromTypeA( LONG nType ) const;
  LPCSTR GetLineBreakDataFromTypeA( LONG nType ) const;
  LONG GetLineBreakSizeFromTypeW( LONG nType ) const;
  LPCWSTR GetLineBreakDataFromTypeW( LONG nType ) const;
#ifndef _UNICODE
#define GetLineBreakSizeFromType GetLineBreakSizeFromTypeA
#define GetLineBreakDataFromType GetLineBreakDataFromTypeA
#else
#define GetLineBreakSizeFromType GetLineBreakSizeFromTypeW
#define GetLineBreakDataFromType GetLineBreakDataFromTypeW
#endif
public:
  // 事件通知信息...
  LONG EventSendMsg( UINT nCode, LPARAM lParam );
public:                           // 文本信息管理...
  BOOL TextIsSOL( const CPoint& rpt ) const;
  BOOL TextIsEOL( const CPoint& rpt ) const;
  BOOL TextIsSOF( const CPoint& rpt ) const;
  BOOL TextIsEOF( const CPoint& rpt ) const;
  BOOL TextIsFirstLine( LONG nLine ) const;
  BOOL TextIsLastLine( LONG nLine ) const;
  BOOL TextIsValidLine( LONG nLine ) const;
  BOOL TextIsValidChar( LONG nLine, LONG nChar ) const;
  LONG TextVerifyLine( LONG nLine ) const;
  LONG TextVerifyChar( LONG nLine, LONG nChar ) const;
  CPfxEditLineInfo* TextGetLineAt( LONG nIndex ) const;
  LONG TextGetLineLength( LONG nLine ) const;
  LPTSTR TextGetLineData( LONG nLine ) const;
  DWORD TextGetLineCookies( LONG nLine ) const;
  LONG TextGetLineSubLines( LONG nLine ) const;
  LONG TextGetLineCount() const;
  LONG TextGetLineUpperBound() const;
  LONG TextGetBlockSize( const CPoint& rptFrom, const CPoint& rptTo, LONG nLineBreakType );
  LONG TextGetBlockData( const CPoint& rptFrom, const CPoint& rptTo, LPTSTR pBuffer, LONG nBufMax, LONG nLineBreakType );
  LONG TextGetThisMainLine( LONG nLine ) const;
  LONG TextGetPrevMainLine( LONG nLine ) const;
  LONG TextGetNextMainLine( LONG nLine ) const;
  // 文本坐标转换...
  void TextAbsLineToSubLine( CPoint& rpt ) const;
  void TextAbsLineToMainLine( CPoint& rpt ) const;
  LONG TextAbsLineToOffset( const CPoint& rpt ) const;
  void TextMainLineToAbsLine( CPoint& rpt ) const;
  void TextMainLineToSubLine( CPoint& rpt ) const;
  LONG TextMainLineToOffset( const CPoint& rpt ) const;
  void TextSubLineToAbsLine( CPoint& rpt ) const;
  void TextSubLineToMainLine( CPoint& rpt ) const;
  LONG TextSubLineToOffset( const CPoint& rpt ) const;
  void TextOffsetToAbsLine( LONG nOffset, CPoint& rpt ) const;
  void TextOffsetToSubLine( LONG nOffset, CPoint& rpt ) const;
  void TextOffsetToMainLine( LONG nOffset, CPoint& rpt ) const;
  // 这是PfxEditCtrl唯一的数据插入和删除函数
  void TextInsertChars( CPoint& rpt, LPCTSTR pData, LONG cch, CPfxEditView* pView );
  void TextRemoveChars( CPoint& rptFrom, CPoint& rptTo, CPfxEditView* pView );
public:
  CPfxEditLineInfo* TextInsertLine( LONG nIndex, LONG nCount = 1 );
  void TextInsertLine( LONG nIndex, LPCTSTR pLine, LONG nLen, DWORD dwFlags );
  void TextRemoveLine( LONG nIndex, LONG nCount = 1 );
  void TextRemoveLine( LONG nIndex );
  void TextUpdateDocSize( LONG cchChanged );
  void TextUpdateLineIndex( LONG nFromLine, LONG nBaseIndex );
  void TextUpdateLineInfo( CPfxEditLineInfo* pliHead, LONG nFirstModifiedLine, LONG nModifiedLines, LONG nAdjustedLines, CPoint& rptCurPos );
  void TextGetBuffer( CPfxEditLineInfo* pli, LONG nNewLen );
  void TextConstructEmptyLineInfo( CPfxEditLineInfo* pli, LONG nCount = 1 ) const;
public:                           // 撤消恢复管理...
  void UndoRemove( CPfxEditUndoInfo* pui, CPoint& rptRetPos, BOOL bRedo );
  void UndoInsert( CPfxEditUndoInfo* pui, CPoint& rptRetPos, BOOL bRedo );
  void UndoAddInsert( CPoint ptFrom, CPoint ptTo );
  void UndoAddRemove( CPoint ptFrom, CPoint ptTo );
  void UndoSetGroup( BOOL bBegin, WORD fUndoName );
  void UndoSetUnmodified();
  void UndoGetBuffer( CPfxEditUndoInfo* pui, LONG nLen );
  WORD UndoGetFlags( WORD wDefaultFlags );
  LONG UndoGetCount() const;
  LONG UndoGetRedoCount() const;
  CPfxEditUndoInfo* UndoAddRecord();
  CPfxEditUndoInfo* UndoGetInfo() const;
  CPfxEditUndoInfo* UndoGetRedoInfo() const;
public:                           // 局部缓冲池管理...
  void BufferLineCleanup();
  void BufferLineGetSpace( LONG nLen, TCHAR** ppBuffer, CPfxEditDocMemBlock** ppMemBlock );
  void BufferLineCompact( CPfxEditDocMemBlock* pCurMemBlock, LONG nNewUnused );
  void BufferLineRemoveBlock( CPfxEditDocMemBlock* pMemBlock );
  CPfxEditDocMemBlock* BufferLineAllocBlock( LONG nBlockSize );
  void BufferUndoCleanup();
  void BufferUndoGetSpace( LONG nLen, TCHAR** ppBuffer, CPfxEditDocMemBlock** ppMemBlock );
  void BufferUndoCompact( CPfxEditDocMemBlock* pCurMemBlock, LONG nNewUnused );
  void BufferUndoRemoveBlock( CPfxEditDocMemBlock* pMemBlock );
  CPfxEditDocMemBlock* BufferUndoAllocBlock( LONG nBlockSize );
public:                           // 数据装载和保存...
  BOOL FileIsAnsi( LPCSTR pData, LONG cch ) const;
  BOOL FileIsUtf8( LPCSTR pData, LONG cch ) const;
  BOOL FileIsUnicode( LPCSTR pData, LONG cch ) const;
  BOOL FileIsUnicodeBigEndian( LPCSTR pData, LONG cch ) const;
  LONG FileGetConverterType( LPCSTR pData, LONG cch ) const;
  void FileLoadAnsi( CPfxEditDocFile& file ); // throw (CMemoryException)
  void FileLoadUtf8( CPfxEditDocFile& file ); // throw (CMemoryException)
  void FileLoadUnicode( CPfxEditDocFile& file ); // throw (CMemoryException)
  void FileLoadUnicodeBigEndian( CPfxEditDocFile& file ); // throw (CMemoryException)
  void FileSaveAnsi( CArchive& ar ); // throw (CMemoryException)
  void FileSaveUtf8( CArchive& ar ); // throw (CMemoryException)
  void FileSaveUnicode( CArchive& ar ); // throw (CMemoryException)
  void FileSaveUnicodeBigEndian( CArchive& ar ); // throw (CMemoryException)
public:
  void SetLockBufferCompact( BOOL bOn );
  void SetLockWordWrapEvent( BOOL bOn );
  BOOL IsLockedBufferCompact() const;
  BOOL IsLockedWordWrapEvent() const;
  // Generated message map functions
public:
  friend class CPfxEditView;
};
inline BOOL CPfxEditDoc::TextIsSOL( const CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  return ( rpt.x == 0 );
}
inline BOOL CPfxEditDoc::TextIsEOL( const CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  return ( rpt.x >= TextGetLineAt( rpt.y ) ->nLen );
}
inline BOOL CPfxEditDoc::TextIsSOF( const CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  return ( ( rpt.y == 0 ) && ( rpt.x == 0 ) );
}
inline BOOL CPfxEditDoc::TextIsEOF( const CPoint& rpt ) const {
  ASSERT( TextIsValidLine( rpt.y ) );
  return ( ( rpt.y == TextGetLineUpperBound() ) && ( rpt.x == TextGetLineAt( rpt.y ) ->nLen ) );
}
inline BOOL CPfxEditDoc::TextIsValidLine( LONG nLine ) const {
  ASSERT( TextGetLineCount() > 0 );
  return ( ( nLine >= 0 ) && ( nLine < TextGetLineCount() ) );
}
inline BOOL CPfxEditDoc::TextIsValidChar( LONG nLine, LONG nChar ) const {
  ASSERT( TextIsValidLine( nLine ) );
  return ( ( nChar >= 0 ) && ( nChar <= TextGetLineAt( nLine ) ->nLen ) );
}
inline BOOL CPfxEditDoc::TextIsFirstLine( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  return ( nLine == 0 );
}
inline BOOL CPfxEditDoc::TextIsLastLine( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  return ( nLine >= TextGetLineUpperBound() );
}
inline LONG CPfxEditDoc::TextGetLineCount() const {
  return m_clsLine.nli;
}
inline LONG CPfxEditDoc::TextGetLineUpperBound() const {
  return m_clsLine.nli - 1;
}
inline LONG CPfxEditDoc::TextGetLineLength( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  return TextGetLineAt( nLine ) ->nLen;
}
inline LPTSTR CPfxEditDoc::TextGetLineData( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  return TextGetLineAt( nLine ) ->pData;
}
inline DWORD CPfxEditDoc::TextGetLineCookies( LONG nLine ) const {
  ASSERT( TextIsValidLine( nLine ) );
  return TextGetLineAt( nLine ) ->dwFlags;
}
inline CPfxEditLineInfo* CPfxEditDoc::TextGetLineAt( LONG nIndex ) const {
  ASSERT( nIndex >= 0 && nIndex < m_clsLine.nli );
  return &m_clsLine.pli[ nIndex ];
}
inline LONG CPfxEditDoc::UndoGetCount() const {
  return m_clsUndo.nPosCur;
}
inline LONG CPfxEditDoc::UndoGetRedoCount() const {
  return ( m_clsUndo.nui - m_clsUndo.nPosCur );
}
inline void CPfxEditDoc::UndoSetUnmodified() {
  m_clsUndo.nPosUnmodified = m_clsUndo.nPosCur;
}
inline BOOL CPfxEditDoc::IsWrapping() const {
  return ( m_nWrapStyle != PFX_EC_WRAP_NONE );
}
inline BOOL CPfxEditDoc::IsWrapAtWindowEdge() const {
  return ( m_nWrapStyle == PFX_EC_WRAP_WINDOWEDGE );
}
inline BOOL CPfxEditDoc::IsWrapAfterColumns() const {
  return ( m_nWrapStyle == PFX_EC_WRAP_COLUMNS );
}
inline BOOL CPfxEditDoc::IsDrawLineBreak() const {
  return m_bDrawLineBreak;
}
inline BOOL CPfxEditDoc::IsDrawWhitespace() const {
  return m_bDrawWhitespace;
}
inline BOOL CPfxEditDoc::IsDrawRuler() const {
  return m_bDrawRuler;
}
inline BOOL CPfxEditDoc::IsDrawLineNumber() const {
  return m_bDrawLineNumber;
}
inline BOOL CPfxEditDoc::IsCanUndo() const {
  return !IsReadOnly() && ( UndoGetCount() > 0 );
}
inline BOOL CPfxEditDoc::IsCanRedo() const {
  return !IsReadOnly() && ( UndoGetRedoCount() > 0 );
}
inline BOOL CPfxEditDoc::IsCanReload() const {
  return !GetPathName().IsEmpty();
}
inline BOOL CPfxEditDoc::IsReadOnly() const {
  return m_bReadOnly;
}
inline BOOL CPfxEditDoc::IsLockedBufferCompact() const {
  return m_bLockedBufferCompact;
}
inline BOOL CPfxEditDoc::IsLockedWordWrapEvent() const {
  return m_bLockedWordWrapEvent;
}
inline LONG CPfxEditDoc::GetWrapStyle() const {
  return m_nWrapStyle;
}
inline LONG CPfxEditDoc::GetWrapWidth() const {
  return m_nWrapWidth;
}
inline LONG CPfxEditDoc::GetLineBreakSize() const {
  return m_nLineBreakSize;
}
inline LONG CPfxEditDoc::GetLineBreakType() const {
  return m_nLineBreakType;
}
inline LONG CPfxEditDoc::GetOpenConverter() const {
  return m_nOpenConverter;
}
inline LONG CPfxEditDoc::GetFileConverter() const {
  return m_nSaveConverter;
}
inline LONG CPfxEditDoc::GetBookmarkCount() const {
  return m_nBookmarks;
}
inline LONG CPfxEditDoc::GetDocSize() const {
  return m_nDocSize;
}
inline LONG CPfxEditDoc::GetLineCount() const {
  return m_nDocLineCount;
}
inline LONG CPfxEditDoc::GetLineUpperBound() const {
  return m_nDocLineCount - 1;
}
inline void CPfxEditDoc::SetLockBufferCompact( BOOL bOn ) {
  m_bLockedBufferCompact = bOn;
}
inline void CPfxEditDoc::SetLockWordWrapEvent( BOOL bOn ) {
  m_bLockedWordWrapEvent = bOn;
}
inline void CPfxEditDoc::SetReadOnly( BOOL bOn ) {
  m_bReadOnly = bOn;
}
inline void CPfxEditDoc::SetOpenConverter( LONG nConverter ) {
  ASSERT( ( nConverter == PFX_EC_FILE_CONVERTER_AUTO_DETECT )
          || ( nConverter == PFX_EC_FILE_CONVERTER_ANSI )
          || ( nConverter == PFX_EC_FILE_CONVERTER_UTF8 )
          || ( nConverter == PFX_EC_FILE_CONVERTER_UNICODE )
          || ( nConverter == PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN ) );
  m_nOpenConverter = nConverter;
}
inline void CPfxEditDoc::SetFileConverter( LONG nConverter ) {
  ASSERT( ( nConverter == PFX_EC_FILE_CONVERTER_ANSI )
          || ( nConverter == PFX_EC_FILE_CONVERTER_UTF8 )
          || ( nConverter == PFX_EC_FILE_CONVERTER_UNICODE )
          || ( nConverter == PFX_EC_FILE_CONVERTER_UNICODE_BIG_ENDIAN ) );
  m_nSaveConverter = nConverter;
}
// CPfxEditView
// 下面是内部使用的符号定义
const TCHAR PFX_EC_CHR_CR = ( '\r' );
const TCHAR PFX_EC_CHR_LF = ( '\n' );
const TCHAR PFX_EC_CHR_TAB = ( '\t' );
const TCHAR PFX_EC_CHR_SPACE = ( VK_SPACE );
const LONG PFX_EC_INVALID_LINE = -1;
const LONG PFX_EC_INVALID_CHAR = -1;
const LONG PFX_EC_INVALID_SELECT = -1;
// 位置转换标志(HT Hit Test)
const LONG PFX_EC_HT_ERROR = 0;
const LONG PFX_EC_HT_RULER = 1;
const LONG PFX_EC_HT_LINE_NUMBER = 2;
const LONG PFX_EC_HT_TEXT = 3;
// 选择模式(SM Selection Mode)
const LONG PFX_EC_SM_NORMAL = 0;
const LONG PFX_EC_SM_WORD = 1;
const LONG PFX_EC_SM_COLUMN = 2;
// 更新标志(UT Update Type)
const DWORD PFX_EC_UT_LINES = 0x00000001L; // 仅仅刷新指定的行
const DWORD PFX_EC_UT_LAYOUT = 0x00000002L; // 仅仅更新布局
const DWORD PFX_EC_UT_HORZ = 0x00000004L; // 刷新指定的行和重新计算水平滚动条
const DWORD PFX_EC_UT_VERT = 0x00000008L; // 刷新指定的行和重新计算垂直滚动条
const DWORD PFX_EC_UT_ALL = 0x00000010L; // 需要重新计算布局和全面刷新
// 撤消/恢复描述(UF Undo Flags)
const WORD PFX_EC_UF_UNKNOWN = 0x0000;
const WORD PFX_EC_UF_BEGIN_GROUP = 0x0001;
const WORD PFX_EC_UF_GROUP = 0x0002;
const WORD PFX_EC_UF_INSERT = 0x0004;
const WORD PFX_EC_UF_REMOVE = 0x0008;
// 行标志(LF Line Flags)
const DWORD PFX_EC_LF_NONE = 0x00000000L;
const DWORD PFX_EC_LF_SUBLINE = 0x00010000L;
const DWORD PFX_EC_LF_HARDRETURN = 0x00020000L;
const DWORD PFX_EC_LF_BOOKMARK = 0x00040000L;
const DWORD PFX_EC_LF_COOKIE_NONE = 0x00000000L;
const DWORD PFX_EC_LF_COOKIE_QUOTED_STRING1 = 0x01000000L;
const DWORD PFX_EC_LF_COOKIE_QUOTED_STRING2 = 0x02000000L;
const DWORD PFX_EC_LF_COOKIE_LINE_COMMENT1 = 0x04000000L;
const DWORD PFX_EC_LF_COOKIE_LINE_COMMENT2 = 0x08000000L;
const DWORD PFX_EC_LF_COOKIE_BLOCK_COMMENT1 = 0x10000000L;
const DWORD PFX_EC_LF_COOKIE_BLOCK_COMMENT2 = 0x20000000L;
const DWORD PFX_EC_LF_COOKIE_ALL = 0x3F000000L;
#define PFX_EC_IS_EMPTY_LINE(pli)             (NULL == pli->pData)
#define PFX_EC_IS_EMPTY_UNDO(pui)             (NULL == pui->pData)
#define PFX_EC_IS_SUBLINE(pli)                (pli->dwFlags & PFX_EC_LF_SUBLINE)
#define PFX_EC_IS_DRAW_BOOKMARK(pli)          (pli->dwFlags & PFX_EC_LF_BOOKMARK)
#define PFX_EC_IS_DRAW_HARDRETURN(pli)        (pli->dwFlags & PFX_EC_LF_HARDRETURN)
#define PFX_EC_IS_KEY_DOWN(k)                 (::GetKeyState(k) & 0x8000)
#ifdef _MBCS
#define PFX_EC_IS_LEAD_BYTE(c)                (PFX_EC_DATA.m_arrLeadByte[static_cast<BYTE>(c)])
#else
#define PFX_EC_IS_LEAD_BYTE(c)                (false)
#endif
struct CPfxEditViewFindReplace {
  // 匹配大小写
  BOOL bMatchCase;
  // 匹配整个字符串
  BOOL bMatchWholeWord;
  // 匹配规则表达式
  BOOL bRegExp;
  // （查找使用）向下搜索
  BOOL bDirectDown;
  // （查找使用）找到文档尽头时是否回绕
  BOOL bWrapAtEndOfFile;
  // （查找使用）全部标记，执行该操作将会忽略指定位置从文档开始搜索到
  // 结尾，并将找到的行标记，这是用于查找操作的成员
  BOOL bMarkAll;
  // （替换使用）是否在选择范围内替换，这是用于替换操作的成员，如果该
  // 成员设置为TRUE，则忽略ptFrom和ptTo操作范围在选择范围内，如果当前
  // 没有选择范围则操作失败
  BOOL bInSelection;
  // （替换使用）是否忽略查找对象将ptFrom至ptTo的内容用pszReplaceWith
  // 替换掉，这种操作将不需要执行查找
  BOOL bIgnoreFindWhat;
  // 要搜索字符串对象的长度（单位：TCHAR），如果等于-1由搜索函数自动计算
  LONG cchFindWhat;
  // 搜索对象，如果不是以NULL结尾则应该给出长度，否则假定以NULL结尾
  LPCTSTR pszFindWhat;
  // 替换字符串的长度，等于-1由搜索函数自动计算
  LONG cchReplaceWith;
  // 替换对象，如果不是以NULL结尾则应该给出长度，否则假定以NULL结尾
  LPCTSTR pszReplaceWith;
  // 指定操作的起始位置
  CPoint ptFrom;
  // 操作的结束位置
  CPoint ptTo;
  // 用来转换大小写的缓冲区
  CString strCaseBuffer;
};
struct CPfxEditViewPrintContext : public CPfxEditPrintInfo {
  LONG m_nCharWidthTab;
  LONG m_nCharWidthSpace;
  LONG m_nCharWidthMax;
  LONG m_nTabSize;
  LONG m_nFontHeight;
  LONG m_nLineHeight;
  LONG m_nHeaderAndFooterLineHeight;
  // 行间距
  LONG m_nSpacingLines;
  LONG m_nTotalPages;
  LONG m_nMaxLines;
  LONG m_nLinesPerPage;
  // 语法高亮打印时最后一已经分析过的行
  LONG m_nLineLastParsed;
  LONG m_nCurPage;
  LONG m_nCurPageFirstLine;
  LONG m_nCurPageLastLine;
  LONG m_nWidthSeparateLine;
  LONG m_nWidthMarginLine;
  // 绘制时用于传送参数和避免频繁使用SetTextColor()
  COLORREF m_clrCurText;
  // 当前的背景颜色
  COLORREF m_clrCurTextBkgnd;
  CRect m_rectLine;
  CPoint m_ptCurPos;
  // 打印区域
  // 标题区域，不打印标题时等于空
  CRect m_rectHeader;
  // 脚注区域，不打印脚注时等于空
  CRect m_rectFooter;
  // 正文区域
  CRect m_rectText;
  // 行号区域，不打印行号时等于空
  CRect m_rectLineNumber;
  // 指向正文数据的行信息缓冲区
  CPfxEditHeap m_cHeap;
};
struct CPfxEditViewPaintContext {
  // 当前的文本输出点坐标
  CPoint m_ptCurPos;
  BOOL m_bDrawRuler;
  BOOL m_bDrawLineNumber;
  BOOL m_bDrawWhitespace;
  BOOL m_bDrawLineBreak;
  LONG m_nFirstUpdateLine;
  LONG m_nLastUpdateLine;
  LONG m_nCharWidthSpace;
  LONG m_nCharWidthTab;
  LONG m_nCharWidthMax;
  LONG m_nCharWidthLineBreak;
  LONG m_nTabSize;
  LONG m_nFontHeight;
  LONG m_nLineHeight;
  LONG m_nSpacingLines;
  LONG m_nTextHorzOffset;
  CRect m_rectUpdate;
  CRect m_rectRuler;
  CRect m_rectLineNumber;
  CRect m_rectText;
  CRect m_rectColSelect;
  CRect m_rectIndent;
  CSize m_sizeBitmap;
  CPen m_penNormal;
  CPen m_penSelect;
  HPEN m_pPenWhitespace;
  // 当前的前景颜色（避免频繁使用SetTextColor()）
  COLORREF m_clrCurText;
  // 当前的背景颜色
  COLORREF m_clrCurTextBkgnd;
  // 颜色表（目前因为只使用颜色属性，所以简单的用COLORREF数组，
  // 如果以后还支持粗体/斜体等属性则可以定义一个新的结构来存放
  // 信息，但不要用标准的CPfxEditKeyClass，因为开销很大
  COLORREF m_arrColors[ kcKeyClassArraySize ];
  COLORREF COLOR( LONG nIndex ) const {
    return m_arrColors[ nIndex ];
  }
};
struct CPfxEditViewWrapLineHint {
  // 显示DC或打印DC
  HDC m_hDC;
  // 最大字符宽度
  LONG m_nCharWidthMax;
  // TAB字符宽度
  LONG m_nCharWidthTab;
  // 空格字符宽度
  LONG m_nCharWidthSpace;
  // 制表符宽度
  LONG m_nTabSize;
  // 如果PFX_EC_WRAP_WINDOWEDGE它是逻辑单元数，如果是
  // PFX_EC_WRAP_COLUMNS它是列数
  LONG m_nWidth;
  // 包括主行和子行在内的总行数
  LONG m_nliCur;
  // 当前以CPfxEditLineInfo大小为单位的最大缓冲区长度
  LONG m_nliMax;
  // 如果空间不足，每次增加的以CPfxEditLineInfo大小为单位的数量
  LONG m_nGrowBy;
};
// 下面的字符是各种标识符的首个字符，它们用在过滤字符串时探测
struct CPfxEditViewMarkerChar {
  WCHAR m_cLowerQuotedString1;
  WCHAR m_cUpperQuotedString1;
  WCHAR m_cLowerQuotedString2;
  WCHAR m_cUpperQuotedString2;
  WCHAR m_cLowerLineComment1;
  WCHAR m_cUpperLineComment1;
  WCHAR m_cLowerLineComment2;
  WCHAR m_cUpperLineComment2;
  WCHAR m_cLowerBlockComment1;
  WCHAR m_cUpperBlockComment1;
  WCHAR m_cLowerBlockComment2;
  WCHAR m_cUpperBlockComment2;
};
struct CPfxEditViewBraceHint {
  // EditMatchBrace使用
  BOOL m_bMatchCase;  // 返回是否匹配大小写
  BOOL m_bDirectDown; // 返回搜索方向
  CString m_strOn;        // 返回搜索方向上的ON串
  CString m_strOff;       // 返回搜索方向上的OFF串
  CPoint m_ptFindStart;   // 返回绝对主行位置的开始搜索点
  // MatchBraceGetString()和MatchBraceGetHint()使用
  LONG m_nMaxBraceString; // 最长的BRACE字符串长度
  CString m_strBrace;     // 返回待检查的STRING
  CPoint m_ptStrStart;    // 返回相对子行位置的STRING起点
  CPoint m_ptStrEnd;      // 返回相对子行位置的STRING终点
};
class CPfxEditView : public CView {
public:
  // 用于管理UNDO/REDO操作，主要是用来设定一个UNDO组的边界
  class CPfxEditViewUndoManager {
  public:
    CPfxEditView* m_pOwner;
  public:
    CPfxEditViewUndoManager( CPfxEditView* pOwner, WORD fUndoName ) {
      ASSERT_VALID( pOwner );
      m_pOwner = pOwner;
      m_pOwner->SetUndoGroup( true, fUndoName );
    }
    ~CPfxEditViewUndoManager() {
      m_pOwner->SetUndoGroup( false, PFX_EC_UNDO_UNKNOWN );
    }
  };
  // 用来处理长时间的操作
  class CPfxEditViewWorkingState {
    LONG m_nWrapStyle;
    LONG m_nWrapWidth;
    CPfxEditView* m_pOwner;
    BOOL m_bWrapRestore;
  public:
    CPfxEditViewWorkingState( CPfxEditView* pOwner, BOOL bCompactBuffer ) {
      ASSERT_VALID( pOwner );
      m_pOwner = ( bCompactBuffer ? pOwner : NULL );
      if ( m_pOwner != NULL ) {
        m_bWrapRestore = pOwner->IsWrapping();
        if ( m_bWrapRestore ) {
          m_nWrapStyle = pOwner->GetWrapStyle();
          m_nWrapWidth = pOwner->GetWrapWidth();
          pOwner->SetViewWrapping( PFX_EC_WRAP_NONE, 0 );
        }
        pOwner->SetLockBufferCompact( true );
        pOwner->SetLockWordWrapEvent( true );
      }
    }
    ~CPfxEditViewWorkingState() {
      if ( m_pOwner != NULL ) {
        if ( m_bWrapRestore ) {
          m_pOwner->SetViewWrapping( m_nWrapStyle, m_nWrapWidth );
        }
        m_pOwner->SetLockBufferCompact( false );
        m_pOwner->SetLockWordWrapEvent( false );
      }
    }
  };
  class CPfxEditViewClipboard {
  public:
    CPfxEditViewClipboard( CControlUI* pOwner, UINT uFormat, BOOL bGet = true );
    ~CPfxEditViewClipboard();
  public:
    LPCTSTR GetClipboardData();
    LPTSTR GetBuffer( LONG nBufLen, BOOL bAppend );
    BOOL SetClipboardData();
    BOOL Open();
    void Close();
    BOOL UnLock( HGLOBAL hGlobal );
    HGLOBAL Detach();
    BOOL Attach( HGLOBAL hGlobal );
    LPTSTR Lock( HGLOBAL hGlobal ) {
      ASSERT( hGlobal != NULL );
      return static_cast<LPTSTR>( ::GlobalLock( hGlobal ) );
    }
    HGLOBAL GetHandle() {
      return m_hCBData;
    }
  public:
    CControlUI* m_hWndOwner;
    HGLOBAL m_hCBData;
    UINT m_uFormat;
    BOOL m_bOpened;
    BOOL m_bForGet;
  };
class CPfxEditViewDataSource : public COleDataSource {
    CPfxEditView* m_pOwner;
    // Constructors
  public:
    CPfxEditViewDataSource( CPfxEditView* pOwner ) {
      ASSERT_VALID( pOwner );
      m_pOwner = pOwner;
      pOwner->SetDragging( true );
      pOwner->SetDragDropHandled( false );
    }
  public:
    // Operations
    // Overides
    virtual BOOL OnRenderGlobalData( LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal );
    // Implementation
  public:
    virtual ~CPfxEditViewDataSource();
  };
  COleDropTarget m_odt;             // OLE拖放对象
  CPfxEditDoc* m_pdb;               // CPfxEditDoc指针(DocBuffer)
  CPfxEditTypeInfo* m_pti;          // 语法高亮配置信息
  CPfxEditViewPrintContext* m_ppc;  // 打印配置信息
  BOOL m_bOverride;            // 是否处于覆盖状态
  BOOL m_bTrackingMouse;       // 正在跟踪鼠标位置
  BOOL m_bTrackingColSelect;   // 追踪列块选择范围
  BOOL m_bDragging;            // 拖放编辑正在进行
  BOOL m_bDragDropHandled;     // 拖放数据被自己接收
  BOOL m_bDelayRecalcWrapping; // 在激活的时候是否需要重新计算换行
  LONG m_nSelMode;             // 选择模式
  LONG m_nSelSize;             // 已选择文本的大小（单位：字节）
  LONG m_nSelLineFrom;         // 选择区的第一行序号
  LONG m_nSelLineTo;           // 选择区的最后行序号
  CPoint m_ptFirstVisible;     // 首个可见点，X为逻辑单元单位，Y为行单位
  CPoint m_ptEditPos;          // 当前位置，它忽略TAB字符扩展，是绝对位置
  CPoint m_ptLogiPos;          // 当前位置，它识别TAB扩展
  CPoint m_ptTextPos;          // 当前文本位置，单位：逻辑单元
  CPoint m_ptCaretRefer;       // 插入位置的参考点，文本区坐标
  // 可见区域。cx等于水平逻辑单元。cy等于可见行行数，它等于(文本区高度 / 行高)
  CSize m_sizeVisible;
  CRect m_rectClient;          // 客户区
  CRect m_rectRuler;           // 显示标尺的区域
  CRect m_rectText;            // 显示文本的区域
  CRect m_rectLineNumber;      // 显示行号的区域
  CRect m_rectColSelect;       // 列块选择时的追踪区域
  CRect m_rectSelWordModeBaseRange;
public:
  BEGIN_MESSAGE_MAP()
  //ON_WM_MOUSEWHEEL()
  //ON_WM_KEYDOWN()
  ON_WM_SETFOCUS()
  ON_WM_KILLFOCUS()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONDBLCLK()
  //ON_WM_RBUTTONDOWN()
  ON_WM_CHAR()
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONUP()
  //ON_WM_MOUSELEAVE()
  END_MESSAGE_MAP( CContainerUI )
  CPfxEditView(); // public constructor used by dynamic creation
  virtual ~CPfxEditView();
  CPfxEditDoc* GetDocument() {
    return m_pdb;
  }
  virtual void SetPos( RECT rc ) {
    CContainerUI::SetPos( rc );
    OnSize( true, rc.right - rc.left, rc.bottom - rc.top );
  }
  void SetCapture() {
    GetManager() ->SetCapture();
  }
  BOOL SetCaretPos( POINT pt ) {
    return ::SetCaretPos( pt.x + m_rcItem.left, pt.y + m_rcItem.top );
  }
  BOOL SetScrollInfo( int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE ) {
    return 0;
  }
  BOOL ShowScrollBar( UINT nBar, BOOL bShow = TRUE ) {
    return 0;
  }
  BOOL Invalidate( bool AA ) {
    return CView::Invalidate();
  }
  BOOL UpdateWindow() {
    return CView::Invalidate();
  }
  BOOL InvalidateRect( CONST RECT *lpRect = NULL, BOOL bErase = 0 ) {
    return CView::Invalidate();
  }
  // 是否按语法高亮方式显示
  BOOL IsDrawHilighted() const;
  // 是否显示标尺
  BOOL IsDrawRuler() const;
  // 是否显示行号
  BOOL IsDrawLineNumber() const;
  // 是否显示空白字符
  BOOL IsDrawWhitespace() const;
  // 是否显示换行符
  BOOL IsDrawLineBreak() const;
  // 指定行是否有书签
  BOOL IsDrawBookmark( LONG nLine = -1 ) const;
  // 背景颜色是否与系统同步
  BOOL IsSyncTextBkColorWithSystem() const;
  // 活动行背景颜色是否与文本背景颜色同步（不特出显示）
  BOOL IsSyncActiveLineBkColorWithTextBkColor() const;
  // 是否有选择文本
  BOOL IsTextSelected() const;
  // 是否可以撤消
  BOOL IsCanUndo() const;
  // 是否可以恢复
  BOOL IsCanRedo() const;
  // 是否可以删除
  BOOL IsCanDelete() const;
  // 是否可以剪切
  BOOL IsCanCut() const;
  // 是否可以复制
  BOOL IsCanCopy() const;
  // 判断是否可以粘贴
  BOOL IsCanPaste() const;
  // 判断是否可以列块粘贴
  BOOL IsCanColumnPaste() const;
  // 判断是否可以列块选择
  BOOL IsCanColumnSelect() const;
  BOOL IsWrapping() const;
  BOOL IsWrapAtWindowEdge() const;
  BOOL IsWrapAfterColumns() const;
  BOOL IsOverride() const;
  BOOL IsReadOnly() const;
  BOOL IsColumnSelect() const;
  BOOL IsExpandTabToSpaceChars() const;
  BOOL IsDragDropEnabled() const;
  BOOL IsHomeKeyGotoFirstNonWhitespaceChar() const;
  BOOL IsFirstSiblingView() const;
  // 测试指定的字符是否是单词定界符
  BOOL IsDelimiter( WCHAR wc ) const;
  BOOL IsDelimiter( LPCTSTR pCur, LPCTSTR pEnd ) const;
  // 测试指定的字符是否是空白字符
  BOOL IsWhitespace( WCHAR wc ) const;
  LONG GetLineCount() const;
  LONG GetLineUpperBound() const;
  LONG GetLineLength( LONG nLine ) const;
  LONG GetLineData( LONG nLine, LPTSTR pBuffer, LONG nBufMax ) const;
  LONG GetUndoName() const;
  LONG GetRedoName() const;
  LONG GetDocSize() const;
  LONG GetTabSize() const;
  LONG GetCurrentChar() const;
  // 获取当前的编辑位置
  CPoint GetCurrentPosition( BOOL bLogicalPos = false ) const;
  // 获取当前选择范围
  BOOL GetSelRange( CPoint& rptFrom, CPoint& rptTo ) const;
  // 当列块选择时它不包含换行符，然而当将选择文本复制到剪贴板时却需要
  // 包含换行符。所以如果要向用户反映选择大小时将 bCopyBufferSize设置
  // 为FALSE，而在获取容纳选择文本的缓冲区大小时需将bCopyBufferSize设
  // 置为TRUE，bCopyBufferSize只影响列块选择，正常选择会忽略它。例子：
  // 当执行复制操作时判断需要多大的剪贴板空间来存放数据时应该使用
  // GetSelSize(PFX_EC_LINE_BREAK_TYPE_DOS, true)
  LONG GetSelSize( LONG nLineBreakType, BOOL bCopyBufferSize = false ) const;
  LONG GetSelData( LONG nLineBreakType, LPTSTR pBuffer, LONG nMaxBuf ) const;
  // 获取当前的自动换行设置
  LONG GetWrapStyle() const;
  LONG GetWrapWidth() const;
  // 获取当前文档的换行符设置
  LONG GetLineBreakSize() const;
  LONG GetLineBreakType() const;
  // 将窗口坐标转换成编辑位置（行，列）
  CPoint TransViewPosToEditPos( CPoint pt ) const;
  // 将编辑位置（行，列）转换成窗口坐标
  CPoint TransEditPosToViewPos( CPoint pt ) const;
  // 将一维的字符偏移量转换为二维的编辑位置
  CPoint TransCharOfsToEditPos( LONG nOffset ) const;
  // 将二维的编辑位置转换为一维的字符偏移量
  LONG TransEditPosToCharOfs( CPoint pt ) const;
  // Operations
  // 将文档设置为只读不允许编辑
  void SetReadOnly( BOOL bOn );
  // 将文档设置为已经修改过的状态，在关闭文档时会提问是否保存
  void SetModifiedFlag( BOOL bChanged );
  // 进入列块选择模式
  void SetColumnSelect();
  // 是否进入覆盖模式
  void SetOverride( BOOL bOn );
  // 设置当前位置
  void SetCurrentPosition( LONG nLine, LONG nChar );
  // 设置文档的换行符类型，会影响文档保存时的换行符格式
  void SetLineBreakType( LONG nLineBreakType );
  // 设置是否显示换行符
  void SetViewLineBreak( BOOL bOn );
  // 设置是否显示空白字符
  void SetViewWhitespace( BOOL bOn );
  // 设置是否显示标尺
  void SetViewRuler( BOOL bOn );
  // 设置是否显示行号
  void SetViewLineNumber( BOOL bOn );
  // 设置是否进入自动换行模式
  BOOL SetViewWrapping( UINT nStyle, LONG nWidth );
  // 撤消
  BOOL EditUndo();
  // 恢复
  BOOL EditRedo();
  // 复制
  BOOL EditCopy( BOOL bAppend );
  // 剪切
  BOOL EditCut( BOOL bAppend );
  // 粘贴
  BOOL EditPaste( LONG nLine, LONG nChar, LONG nType = PFX_EC_PASTE_NORMAL );
  // 删除
  BOOL EditRemove( LONG nFromLine, LONG nFromChar, LONG nToLine, LONG nToChar );
  // 插入
  BOOL EditInsert( LONG nLine, LONG nChar, LPCTSTR pData, LONG cch );
  // 选择
  BOOL EditSelect( LONG nFromLine, LONG nFromChar, LONG nToLine, LONG nToChar );
  // 查找
  BOOL EditFind( CPoint& rptFrom,                             // [IN/OUT]范围起点
                 CPoint& rptTo,                               // [OUT]范围终点
                 LPCTSTR pszFindWhat,                         // 查找对象，长度由cchFindWhat给出
                 LONG cchFindWhat,                            // 查找对象的长度，如果等于-1表示由函数自动计算长度，这时应以NULL结尾
                 BOOL bMatchCase,                             // 是否匹配大小写
                 BOOL bMatchWholeWord,                        // 是否单词匹配
                 BOOL bRegExp,                                // 是否使用正则表达式（这时忽略单词匹配）
                 BOOL bDirectDown,                            // 是否向下查找
                 BOOL bMarkAll,                               // 是否用书签标记找到的对象所在行
                 BOOL bWrapAtEndOfFile );      // 是否在文件尽头折回，它只当起点或终点在文档尽头才起作用
  // 替换
  BOOL EditReplace( CPoint& rptFrom,                          // [IN/OUT]范围起点
                    CPoint& rptTo,                            // [IN/OUT]范围终点
                    LPCTSTR pszFindWhat,                      // 查找对象，长度由cchFindWhat给出
                    LONG cchFindWhat,                         // 查找对象的长度，如果等于-1表示由函数自动计算长度，这时应以NULL结尾
                    LPCTSTR pszReplaceWith,                   // 替换对象，长度由cchReplaceWith给出
                    LONG cchReplaceWith,                      // 替换对象的长度，如果等于-1表示由函数自动计算长度，这时应以NULL结尾
                    BOOL bMatchCase,                          // 是否匹配大小写
                    BOOL bMatchWholeWord,                     // 是否单词匹配
                    BOOL bRegExp,                             // 是否使用正则表达式（这时忽略单词匹配）
                    BOOL bIgnoreFindWhat,                     // 是否忽略查找对象而将指定范围内的文本直接由替换对象取代
                    BOOL bInSelection );       // 是否只在选择范围内进行并忽略起点和终点，如果没有选择内容则失败
  // 查找与当前编辑位置所在符号配对的符号，例如匹配括号功能，标识符对由
  // CPfxEditTypeInfo中的 m_pBraceStrings指定，一般由语法配置文件读入
  BOOL EditMatchBrace();
  // 空格字符转换成制表符
  // BOOL bLeading 是否只将行首的空格转换而忽略其它的
  BOOL EditSpacesToTabs( BOOL bLeading );
  // 制表符转换成空格字符
  BOOL EditTabsToSpaces();
  // 删除行末空白字符
  BOOL EditTrimTrailingSpaces();
  // 将全部或选择范围内的字符转换成小写或大写
  BOOL EditCaseChange( BOOL bLower );
  // 将全部或选择范围内的字符反转大小写
  BOOL EditCaseInvert();
  // 将全部或选择范围内的单词首字母大写
  BOOL EditCaseCapitalize();
  void BookmarkToggle( BOOL bOn, LONG nLine = -1, BOOL bUpdate = true );
  LONG BookmarkGetNext( LONG nLine, BOOL bWrap );
  LONG BookmarkGetPrev( LONG nLine, BOOL bWrap );
  void BookmarkClearAll();
  LONG BookmarkGetCount() const;
public:
  virtual void OnImmKeyChars( LPCTSTR pData, LONG cch ) {
    OnKeyChar( pData, cch );
  }
  // 键盘输入处理函数组，也可以作通用接口模拟键盘输入，所有操作都相对于当前位置进行
  void OnKeyChar( LPCTSTR pData, LONG cch );
  void OnKeyTab();
  void OnKeyReturn();
  void OnKeyInsert();
  void OnKeyBackSpace( BOOL bWord );
  void OnKeyDelete( BOOL bWord );
  void OnKeyHome( BOOL bSelect, BOOL bGotoSOF );
  void OnKeyEnd( BOOL bSelect, BOOL bGotoEOF );
  void OnKeyPageUp( BOOL bSelect );
  void OnKeyPageDown( BOOL bSelect );
  void OnKeyMoveUp( BOOL bSelect, BOOL bScroll );
  void OnKeyMoveDown( BOOL bSelect, BOOL bScroll );
  void OnKeyMoveLeft( BOOL bSelect, BOOL bWord );
  void OnKeyMoveRight( BOOL bSelect, BOOL bWord );
public:
  // 设置所有CPfxEditView的显示字体，由调用者负责启动刷新
  static BOOL SetFont( HFONT hFont );
  // 以逻辑单元数指出行间距，由调用者负责启动刷新
  static void SetSpacingLines( LONG nSpacing );
  // 设置行号与正文的距离，单位：逻辑单位，由调用者负责启动刷新
  static void SetSpacingLineNumberAndText( LONG nSpacing );
  static HFONT GetFont();
  static LONG GetSpacingLines();
  static LONG GetSpacingLineNumberAndText();
  static LONG GetLineHeight();
  static BOOL IsLeadByte( UINT c );
public:
  // 当前是否调用了SetTracking()进行鼠标跟踪，例如进行选择操作时
  BOOL IsTracking() const;
  // 是否处于OLE拖放
  BOOL IsDragging() const;
  // OLE拖放操作是否被自己处理了，而不是由其它窗口处理了
  BOOL IsDragDropHandled() const;
  // 测试指定的位置是否落在选择范围内
  BOOL IsInsideSelect( CPoint ptView ) const;
  // 当前是否处于确定列块选择范围的状态
  BOOL IsColumnSelectTracking() const;
  // 判断是否可以列块粘贴（或OLE拖放时实施列块移动或复制）
  BOOL IsDataColumnModeRendered( BOOL bDragDrop, HGLOBAL hGlobal ) const;
  // 在被激活的时候是否需要重新计算换行信息，在 MDI类型的程序中，用户
  // 可以在不同的 MDI子窗口撤换，就会造成频繁的换行计算操作，因此需要
  // 临时锁死换行计算，以避免频繁的重复计算操作
  BOOL IsDelayRecalcWrapping() const;
  // 根据窗口坐标判断指定点落在那个部分：行号、正文、标尺？
  LONG GetHitTestCode( CPoint pt ) const;
  // 返回用于确定插入符水平坐标的数值，特别地用在上下方向键和
  // PAGEUP, PAGEDOWN等输入处理
  LONG GetCaretReferPoint() const;
  // 获取兄弟视图的指针，没有兄弟视图返回NULL
  CPfxEditView* GetSiblingView() const;
  // 获取当前第一可见行
  LONG GetFirstVisibleLine() const;
  // 获取当前最后可见行（不包括被剪切的行，即最后一不能完全显示的行）
  LONG GetLastVisibleLine() const;
  // 获取一行中文本到行起始垂直坐标的偏移量，它用于定位文本和插入符的
  // 垂直位置，当行间距为零时它等于零，否则不为零
  LONG GetLineTextVertOffset() const;
  // 标记OLE拖放操作已经被自己处理了
  void SetDragDropHandled( BOOL bOn );
  // 标记需要延迟重新计算换行（在OnSetFocus()中重新计算）
  void SetDelayRecalcWrapping( BOOL bOn );
  // 标记开始进入OLE拖放状态
  void SetDragging( BOOL bOn );
  // 标记开始捕获鼠标
  void SetTracking( BOOL bOn );
  // 标记开始跟踪列块选择范围
  void SetColumnSelectTracking( BOOL bOn );
  // 选择...
  CPoint SelGetParam() const;
  // 选择指定位置所在单词
  void SelWord( CPoint pt );
  // 选择当前行整行
  void SelLine( LONG nLine );
  // 从当前位置选择到指定点
  void SelRange( CPoint pt );
  // 列块选择
  void SelRange( CRect& rect );
  // 取消选择
  void SelNone();
  // 取消选择并且删除当前选择内容
  void SelNoneAndRemoveSelected();
  // 取消选择并更新屏幕
  void SelNoneAndUpdateCurrentPosition( LONG nAction );
  // 字符处理...
  // rptFrom是相对的子行位置
  void CharsInsert( CPoint& rptFrom, LPCTSTR pData, LONG cch, BOOL bUpdate );
  // rptFrom和rptTo是相对的子行位置
  void CharsRemove( CPoint& rptFrom, CPoint& rptTo, BOOL bUpdate );
  // rptFrom和rptTo是相对的子行位置
  void CharsReplace( CPoint& rptFrom, CPoint& rptTo, LPCTSTR pData, LONG cch, BOOL bUpdate );
  // 匹配标识符、括号等...
  BOOL MatchBraceGetString( CPoint pt, BOOL bPrevStr, CPfxEditViewBraceHint& hint );
  BOOL MatchBraceGetHint( CPfxEditViewBraceHint& hint );
  BOOL MatchBraceSearchUp( CPfxEditViewBraceHint& hint, CPoint& rpt );
  BOOL MatchBraceSearchDown( CPfxEditViewBraceHint& hint, CPoint& rpt );
  // 大小写转换...
  // 改变大小写
  BOOL CaseChange( LONG nLine, LONG nStart, LONG nEnd, BOOL bLower );
  // 反转大小写
  BOOL CaseInvert( LONG nLine, LONG nStart, LONG nEnd );
  // 首字符大写
  BOOL CaseCapitalize( LONG nLine, LONG nStart, LONG nEnd );
  // 如果要向缓冲区填充指定个空格，计算该缓冲区的长度（可能用TAB来代替空格）
  LONG FillBufferWithWhitespacesCalcBufferSize( LONG nStartLogChar,
      LONG nSpacesToFill );
  // 使用空格或TAB填充指定的缓冲区
  void FillBufferWithWhitespacesDoFill( LONG nStartLogChar, LONG nSpacesToFill, LPTSTR pBuffer, LONG nBufLen );
  // 给定一字符串和文本宽度，计算在这宽度内可以容纳多少个字符，它用于
  // 文本坐标到编辑坐标的转换，例如按下鼠标时确定插入符的位置
  void CalcCharsToFit( LPCTSTR& rpCharRet, LONG& rxTextPos, HDC dc, LPCTSTR pStart, LPCTSTR pEnd, LONG nTestPos, LONG nTestLen, LONG nLogChar ) const;
  // 检查一字符串的某个位置是否落在一MBCS字符的中间，以避免在复制指定
  // 数量的字节时将MBCS字符的前半部复制
  LONG CalcCharsToCopy( LPCTSTR pData, LONG cch ) const;
  // 计算从行开头到第一个非空白字符之间的空白数
  LONG CalcLeadingWhitespaces( CPoint pt ) const;
  // 从一字符串的某个位置开始计算到指定的结束位置，累加之间的逻辑字符
  // 数（即TAB扩展）
  LONG CalcLogicalChar( LPCTSTR pStart, LPCTSTR pEnd, LONG nTabSize, LONG nLogChar ) const;
  // 计算字符串逻辑单元宽度，返回宽度值
  LONG CalcStringWidth( LPCTSTR pData, LONG cch, LONG& nLogChar, HDC hDC ) const;
  // LONG nLine - 绝对行位置
  // LONG nLogCharToTest - 将该逻辑字符位置（TAB扩展）转换成绝对字符位置（指针读写位置）
  LONG CalcAbsCharPosFromLogCharPos( LONG nLine, LONG nLogCharToTest ) const;
  // 坐标转换...
  LONG TransLinePosToTextPos( LONG nLine ) const;
  LONG TransCharPosToTextPos( CPoint& rpt ) const;
  LONG TransTextPosToLinePos( LONG yTextPos ) const;
  LONG TransTextPosToCharPos( LONG nLine, LONG& rxTextPos ) const;
  LONG TransTextPosToViewPosX( LONG xTextPos ) const;
  LONG TransTextPosToViewPosY( LONG yTextPos ) const;
  LONG TransViewPosToTextPosX( LONG xViewPos ) const;
  LONG TransViewPosToTextPosY( LONG yViewPos ) const;
  // 参数校验...
  void VerifyCurrentPosition(); // 检查当前位置是否有效
  void VerifyPointerPosition( CPoint& rpt ) const; // 检查X是否在一个MBCS字符的中间
  BOOL EnsureVisible( LONG nLine, LONG xTextPos );
  void EnsureVisibleAndUpdateCurrentPosition();
  void EnsureVisibleAndSetAsCurrentPosition( CPoint pt );
  void EnsureVisibleAndUpdateLines( LONG nLine, LONG xTextPos, LONG nFirstUpdateLine, LONG nLastUpdateLine );
  // 滚动...
  LONG ScrollHorz( UINT nSBCode );
  LONG ScrollVert( UINT nSBCode );
  // OLE拖放操作...
  HGLOBAL DragDropRender( BOOL bDragDrop, BOOL bAppend );
  BOOL DragDropPaste( HGLOBAL hGlobal, CPoint pt );
  void DragDropRemove( CPoint& rpt );
  // 粘贴...
  void PasteNormal( CPoint pt, LPCTSTR pData, LONG cch );
  void PasteColumnAlign( CPoint pt, LPCTSTR pData, LONG cch );
  void PasteColumnAppend( CPoint pt, LPCTSTR pData, LONG cch );
  // 查找替换...
  BOOL SearchPreFind( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchDoFind( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchFindMarkAll( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchFindNormalUp( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchFindNormalDown( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchFindWholeWordUp( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchFindWholeWordDown( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchFindRegExp( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchPreReplace( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchDoReplace( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchReplaceNormal( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchReplaceWholeWord( CPfxEditViewFindReplace& rFindReplace );
  BOOL SearchReplaceRegExp( CPfxEditViewFindReplace& rFindReplace );
  LPCTSTR SearchFindNormalStringCase( LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat );
  LPCTSTR SearchFindNormalStringNoCase( LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat );
  LPCTSTR SearchFindWholeWordStringCase( LPCTSTR pStart, LPCTSTR pEnd, LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat );
  LPCTSTR SearchFindWholeWordStringNoCase( LPCTSTR pStart, LPCTSTR pEnd, LPCTSTR pData, LONG cch, LPCTSTR pszFindWhat, LONG cchFindWhat );
  // 位置移动...
  LPCTSTR GetPrevCharPos( LPCTSTR pCur, LPCTSTR pStart ) const;
  LPCTSTR GetNextCharPos( LPCTSTR pCur, LPCTSTR pEnd ) const;
  LPCTSTR GetPrevWordPos( LPCTSTR pCur, LPCTSTR pStart ) const;
  LPCTSTR GetNextWordPos( LPCTSTR pCur, LPCTSTR pEnd ) const;
  LPCTSTR GetPrevNonWhitespacePos( LPCTSTR pCur, LPCTSTR pStart ) const;
  LPCTSTR GetNextNonWhitespacePos( LPCTSTR pCur, LPCTSTR pEnd ) const;
  LPCTSTR GetPrevDelimiterPos( LPCTSTR pCur, LPCTSTR pStart ) const;
  LPCTSTR GetNextDelimiterPos( LPCTSTR pCur, LPCTSTR pEnd ) const;
  LONG MoveLeft( CPoint& rpt, BOOL bWord, BOOL bCalcTextPos = true ) const;
  LONG MoveRight( CPoint& rpt, BOOL bWord, BOOL bCalcTextPos = true ) const;
  // 语法分析...
  BOOL ParserIsKeyword( LPCTSTR pData, LONG cch ) const;
  BOOL ParserIsEscape( WCHAR wc ) const;
  BOOL ParserIsEscapedLine( LPCTSTR pStart, LPCTSTR pEnd ) const;
  // 检查一给定位置是否落在注释或引用字符串等块的范围内
  // CPoint - 相对子行位置
  BOOL ParserIsInsideBlock( CPoint pt ) const;
  BOOL ParserIsMarkerStart( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, CPfxEditMarkerPair* pMarker ) const;
  BOOL ParserIsMarkerClose( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, CPfxEditMarkerPair* pMarker ) const;
  BOOL ParserIsMarkerString( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, LPCTSTR pString, LONG nStrLen, BOOL bCase ) const;
  void ParserGetFirstCharOfMarkerStart( CPfxEditViewMarkerChar& mc ) const;
  // 返回指定块定义符结束部分的位置，如果在当前行中找不到则返回NULL
  LPCTSTR ParserGetMarkerCloseEndPosition( LPCTSTR pStart, LPCTSTR pCur, LPCTSTR pEnd, BOOL bCurrentAddressPointToMarkerStart, CPfxEditMarkerPair* pMarker ) const;
  BOOL ParserPreParse( LONG nLineToEndParse, LONG nLineUpperBound, LONG nLineLastParsedButCanceled, LONG& rnLineLastParsed, CPfxEditLineInfo* pliHead ) const;
  // 分析一行的数据返回分析结果
  DWORD ParserParseLine( LPCTSTR pData, LONG nLen, DWORD dwCookie ) const;
  // 绘制...
  void DrawRuler( HDC hdcSrc, const CPfxEditViewPaintContext& pc );
  void DrawLineNumber( LONG nLine, HDC hDC, const CPfxEditViewPaintContext& pc );
  void DrawLineBkgnd( LONG nLine, const CPfxEditLineInfo* pli, LONG x, HDC hDC, const CPfxEditViewPaintContext& pc );
  void DrawLineBreak( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc );
  void DrawVisualEOL( HDC hDC, LONG x, LONG y, LONG cx, LONG cy );
  void DrawVisualEOF( HDC hDC, LONG x, LONG y, LONG cx, LONG cy );
  void DrawVisualTab( HDC hDC, LONG x, LONG y, LONG cx, LONG cy );
  void DrawVisualSpace( HDC hDC, LONG x, LONG y, LONG cx, LONG cy );
  // 普通文本绘制...
  void DrawPlainText( HDC hdcSrc, CPfxEditViewPaintContext& pc );
  void DrawBk( HDC hDC, const CPfxEditViewPaintContext& pc );
  void DrawPlainTextNormal( LPCTSTR pData, LONG nLen, LONG& rnLogChar, HDC hDC, CPfxEditViewPaintContext& pc );
  void DrawPlainTextSelection( LONG nLine, const CPfxEditLineInfo* pli, LONG& rnLogChar, HDC hDC, CPfxEditViewPaintContext& pc );
  // 语法高亮绘制...
  void DrawHilighted( HDC hdcSrc, CPfxEditViewPaintContext& pc );
  void DrawHilightedNormal( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc );
  void DrawHilightedSelection( LONG nLine, const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPaintContext& pc );
  void DrawHilightedString1( LPCTSTR pData, LONG nLen, HDC hDC, CPfxEditViewPaintContext& pc );
  void DrawHilightedString2( LPCTSTR pData, LONG nLen, LONG& rnLogChar, COLORREF color, HDC hDC, CPfxEditViewPaintContext& pc );
  LPCTSTR DrawHilightedStringBaseOnCookie( LPCTSTR pData, LONG nLen, LONG& rnLogChar, DWORD dwCookie, HDC hDC, CPfxEditViewPaintContext& pc );
  // 打印...
  void PrintCalcLayout( HDC hdcPrinter, CPfxEditViewPrintContext& pc );
  void PrintFormatText( HDC hdcPrinter, CPfxEditViewPrintContext& pc );
  void PrintLineNumber( LONG nLine, HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintMargin( const CRect& rectDraw, HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintHeader( HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintFooter( HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintHeaderFooterText( const CString& strText, const CRect& rectDraw, HDC hDC, CPfxEditViewPrintContext& pc );
  // 普通文本打印...
  void PrintPlainText( HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintPlainTextLine( const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPrintContext& pc );
  // 语法高亮打印...
  void PrintHilighted( HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintHilightedLine( const CPfxEditLineInfo* pli, HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintHilightedString1( LPCTSTR pData, LONG nLen, HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintHilightedString2( LPCTSTR pData, LONG nLen, LONG& rnLogChar, COLORREF color, HDC hDC, CPfxEditViewPrintContext& pc );
  LPCTSTR PrintHilightedStringBaseOnCookie( LPCTSTR pData, LONG nLen, LONG& rnLogChar, DWORD dwCookie, HDC hDC, CPfxEditViewPrintContext& pc );
  void PrintCheckColors( CPfxEditViewPrintContext& pc );
  virtual void PrintReleaseResource();
  // 自动换行操作...
  // 在指定的逻辑单元处换行
  void WrapAtUnits( LONG nWidth );
  // 在指定的列数处换行
  void WrapColumns( LONG nWidth );
  // 恢复不换行
  void WrapRestore();
  LONG WrapLineSingle( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli );
  LONG WrapLineMultiple( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, LONG nCount, LONG& rnLineToRemember );
  void WrapLineAtUnits( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, CPfxEditViewWrapLineHint& wl );
  void WrapLineColumns( CPfxEditHeap& Heap, const CPfxEditLineInfo* pli, CPfxEditViewWrapLineHint& wl );
  // 滚动条...
  void ScrollBarUpdate( LONG nBar );
  void ScrollBarSetPos( LONG nBar, LONG nPos );
  // 插入符...
  void CaretRebuild();
  void CaretDestroy();
  void CaretSetPos( CPoint pt );
  // 光标...
  void UpdateCursor( CPoint pt );
  // 刷新...
  void UpdateAllViews( DWORD dwFlags, LONG nFirstUpdateLine, LONG nLastUpdateLine );
  void UpdateThisView( CPfxEditView* pView, DWORD dwFlags, LONG nFirstUpdateLine, LONG nLastUpdateLine );
  void UpdateLayout();
  void UpdateCurrentPosition( LONG xTextPos, BOOL bUpdateCurCharPosition, BOOL bUpdateCaretReferPoint );
  void UpdateLines( LONG nFirstUpdateLine, LONG nLastUpdateLine );
  void UpdateLinesSelected( LONG nOldSelLineFrom, LONG nOldSelLineTo );
  // Overridable
  virtual BOOL OnSetupPrintInfo( CPfxEditPrintInfo* pPrintInfo );
  virtual void OnInitialUpdate();
  virtual DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint pt );
  virtual DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint pt );
  virtual void OnDragLeave();
  virtual BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint pt );
  virtual void OnPrepareDC( HDC hDC, CPrintInfo* pInfo = NULL );
  virtual void OnBeginPrinting( HDC hDC, CPrintInfo* pInfo );
  virtual void OnPrint( HDC hDC, CPrintInfo* pInfo );
  virtual void OnEndPrinting( HDC hDC, CPrintInfo* pInfo );
  void OnInitialize();
  // Implementation
  void SetUndoGroup( BOOL bBegin, WORD fUndoName );
  void SetLockBufferCompact( BOOL bOn );
  void SetLockWordWrapEvent( BOOL bOn );
  // MSG(CPfxEditView)
  void OnMouseMove( UINT nFlags, CPoint pt );
  BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
  void DoPaint( HDC hDC, const RECT& rcPaint );
  void OnSize( UINT nType, INT cx, INT cy );
  void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
  void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
  void OnSetFocus();
  void OnKillFocus();
  void OnCaptureChanged( CControlUI *pWnd );
  void OnLButtonDown( UINT nFlags, CPoint pt );
  void OnLButtonUp( UINT nFlags, CPoint pt );
  void OnLButtonDblClk( UINT nFlags, CPoint pt );
  INT OnCreate();
  void OnRButtonDown( UINT nFlags, CPoint pt );
  void OnDestroy();
  // MSG
  LRESULT OnIMEComposition( WPARAM wParam, LPARAM lParam );
  // friend classes that call public CPfxEditView overridables
  friend class CPfxEditDoc;
  friend class CPfxEditViewDataSource;
  friend class CPfxEditViewUndoManager;
  friend class CPfxEditViewWorkingState;
};
#include "pfxeditctrl.inl"
#endif // _PFXEDITCTRL_H_
