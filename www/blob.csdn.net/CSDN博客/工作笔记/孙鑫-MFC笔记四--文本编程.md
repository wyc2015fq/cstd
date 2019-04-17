# 孙鑫-MFC笔记四--文本编程 - 工作笔记 - CSDN博客





2012年05月03日 08:49:48[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5878标签：[编程																[timer																[byte																[callback																[pascal																[mfc](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[基础编程](https://blog.csdn.net/App_12062011/article/category/952065)





1,创建插入符：

void CreateSolidCaret( int nWidth, int nHeight );//创建插入符

void CreateCaret( CBitmap* pBitmap );//创建位图插入符

void ShowCaret( );//显示插入符

void HideCaret( );//隐藏插入符

static void PASCAL SetCaretPos( POINT point );//移动插入符号

说明：

1）创建插入符要在窗口创建完成之后，CreateSolidCaret函数创建的插入符被初始化为隐藏，所以需要调用ShowCaret()将其显示。

2）使用CreateCaret函数创建位图插入符的时候，不能使用局部的位图对象关联位图资源。（与资源相关联的C++对象，当它析构的时候会同时把与它相关联的资源销毁。）

2，获取当前字体信息的度量：CDC::GetTextMetrics

BOOL GetTextMetrics( LPTEXTMETRIC lpMetrics ) const;

说明：

typedef struct tagTEXTMETRIC {  /* tm */

    int  tmHeight;//字体高度。Specifies the height (ascent + descent) of characters.

    int  tmAscent;//基线以上的字体高度 升序高度

    int  tmDescent;//基线以下的字体高度 降序高度 字体的高度是升序高度加上降序高度

    int  tmInternalLeading;

    int  tmExternalLeading;

    int  tmAveCharWidth;//字符平均宽度

    int  tmMaxCharWidth;

    int  tmWeight;

    BYTE tmItalic;

    BYTE tmUnderlined;

    BYTE tmStruckOut;

    BYTE tmFirstChar;

    BYTE tmLastChar;

    BYTE tmDefaultChar;

    BYTE tmBreakChar;

    BYTE tmPitchAndFamily;

    BYTE tmCharSet;

    int  tmOverhang;

    int  tmDigitizedAspectX;

    int  tmDigitizedAspectY;

} TEXTMETRIC;

3，OnDraw函数:

virtual void OnDraw( CDC* pDC )

当窗口（从无到有或尺寸大小改变等）要求重绘的时候，会发送WM_PAIN消息，调用OnDraw函数进行重绘。



4,获取字符串的高度和宽度（区别字符串的长度）：

CDC::GetTextExtent

CSize GetTextExtent( LPCTSTR lpszString, int nCount ) const;

CSize GetTextExtent( const CString& str ) const;

说明：

CSize类似于Windows中的SIZE

typedef struct tagSIZE {

    int cx;//the x-extent 

    int cy;//the y-extent 

} SIZE;

5，路径层：划分自己的操作区域。通过BeginPath和EndPath来实现。

BOOL BeginPath( );

//在这作图定义路径层剪切区域

BOOL EndPath( );

BOOL SelectClipPath( int nMode );//调用这个函数来使当前路径层剪切区域与新剪切区域进行互操作。   

//在这覆盖作图（包含前定义的路径层区域）定义新的剪切区域

说明：

1）SelectClipPath Selects the current path as a clipping region for the device context, combining the new region with any existing clipping region by using the specified mode. The device context identified must contain a closed path.

////

nMode：RGN_AND，RGN_COPY，RGN_DIFF，RGN_OR，RGN_XOR

RGN_AND   The new clipping region includes the intersection (overlapping areas) of the current clipping region and the current path.

RGN_COPY   The new clipping region is the current path.

RGN_DIFF   The new clipping region includes the areas of the current clipping region, and those of the current path are excluded.

RGN_OR   The new clipping region includes the union (combined areas) of the current clipping region and the current path.

RGN_XOR   The new clipping region includes the union of the current clipping region and the current path, but without the overlapping areas.

2）应用：当作图的时候，如果想要在整幅图形其中的某个部分和其它部分有所区别，我们可以把这部分图形放到路径层当中，然后指定调用指定互操作模式调用SelectClipPath( int nMode )函数来使路径层和覆盖在其上新绘图剪切区域进行互操作，达到特殊效果。

6，关于文本字符串一些函数：

COLORREF GetBkColor( ) const;//得到背景颜色

virtual COLORREF SetBkColor( COLORREF crColor );//设置背景颜色

BOOL SetTextBkColor( COLORREF cr );//设置文本背景颜色

virtual COLORREF SetTextColor( COLORREF crColor );//设置文本颜色

virtual BOOL TextOut( int x, int y, LPCTSTR lpszString, int nCount );//输出文本

BOOL TextOut( int x, int y, const CString& str );

CString Left( int nCount ) const;//得到字符串左边nCount个字符

int GetLength( ) const;//得到字符串长度

7,字体CFont::CFont 

CFont( );//构造函数

//Constructs a CFont object. The resulting object must be initialized with CreateFont, CreateFontIndirect, CreatePointFont, or CreatePointFontIndirect before it can be used.

选用字体事例代码组：

CClientDC dc(this);

CFont font;//构造字体对象

font.CreatePointFont(300,"华文行楷",NULL);//初始化字体对象，与字体资源相关联

CFont *pOldFont=dc.SelectObject(&font);//将新字体选入DC

...

dc.SelectObject(pOldFont);//恢复原字体

说明：

1）构造字体对象时候，必须初始化。（初始化是将字体对象与字体资源相关联）。

2）初始化对象时候，选用的字体也可以是系统字体，但不一定都有效，据测试选用。

8，在MFC中CEditView 和 cRichEditView类已经完成了初步的文字处理。可以让应用程序的View类以CEditView 和 cRichEditView类为基类。

9，平滑变色

CDC::TextOut()是一个字母一个字母的输出，达不到平滑效果。

CDC::DrawText()：将文字的输出局限于一个矩形区域，超出矩形区域的文字都被截断。利用这一特点，可每隔些时间增加矩形大小，从而可实现人眼中的平滑效果。

CWnd::SetTimer()：设置定时器。按设定的时间定时发送WM_TIMER消息。

说明：

UINT SetTimer( UINT nIDEvent, UINT nElapse, void (CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT, DWORD) );

//nIDEvent定时器标示，nElapse消息发送间隔时间，void (CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT, DWORD)设置回调函数，如果设置则由设置的回调函数处理WM_TIMER消息，如果没有设置回调函数设为NULL，这发送的WM_TIMER消息压入消息队列，交由相关联的窗口处理（添加WM_TIMER消息处理函数OnTimer()）。

afx_msg void OnTimer( UINT nIDEvent );

//响应WM_TIMER消息，nIDEvent为消息对应定时器标示（可以设置不同的定时器发送WM_TIMER消息）


     在view类中，Create 是虚函数由框架调用，是用来“生成一个窗口的子窗口”。 而OnCreate 函数是用来“表示一个窗口正在生成”。一个窗口创建（Create）之后，会向操作系统发送WM_CREATE消息，OnCreate()函数主要是用来响应此消息的。因为在MFC里面用一种消息映射的机制来响应消息，也就是可以用函数来响应相应的消息。就拿CMainFrame类来说，当窗口创建后会产生WM_CREATE消息，我们可以在OnCreate函数里实现我们要在窗口里面增加的东西，例如按扭，状态栏，工具栏等。这些子窗口一般是定义成类中的一个成员变量，因为要保证生命周期。一般以m_开头来表示成员(member)。onCreate不产生窗口，只是在窗口显示前设置窗口的属性。Create负责注册并产生窗口。](https://so.csdn.net/so/search/s.do?q=pascal&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)




