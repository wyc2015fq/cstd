# Visual C++/MFC入门教程  - 深之JohnChen的专栏 - CSDN博客

2007年03月15日 10:33:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6665标签：[c++																[mfc																[windows																[文档																[dialog																[command](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[windows界面编程](https://blog.csdn.net/byxdaz/article/category/694534)


Visual C++/MFC入门教程 

VC开发指南

1.1 如何学好VC

　　这个问题很多朋友都问过我，当然流汗是必须的，但同时如果按照某种思路进行有计划的学习就会起到更好的效果。万事开头难，为了帮助朋友们更快的掌握VC开发，下面我将自己的一点体会讲一下：

　　1、需要有好的C/C++基础。正所谓“磨刀不误砍柴工”，最开始接触VC时不要急于开始Windows程序开发，而是应该进行一些字符界面程序的编写。这样做的目的主要是增加对语言的熟悉程度，同时也训练自己的思维和熟悉一些在编程中常犯的错误。更重要的是理解并能运用C++的各种特性，这些在以后的开发中都会有很大的帮助，特别是利用MFC进行开发的朋友对C++一定要能熟练运用。 

　　2、理解Windows的消息机制，窗口句柄和其他GUI句柄的含义和用途。了解和MFC各个类功能相近的API函数。

　　3、一定要理解MFC中消息映射的作用。

　　4、训练自己在编写代码时不使用参考书而是使用Help Online。

　　5、记住一些常用的消息名称和参数的意义。

　　6、学会看别人的代码。

　　7、多看书，少买书，买书前一定要慎重。

　　8、闲下来的时候就看参考书。

　　9、多来我的主页。^O^

　　后面几条是我个人的一点意见，你可以根据需要和自身的情况选用适用于自己的方法。

　　此外我将一些我在选择参考书时的原则：

　　对于初学者：应该选择一些内容比较全面的书籍，并且书籍中的内容应该以合理的方式安排，在使用该书时可以达到循序渐进的效果，书中的代码要有详细的讲解。尽量买翻译的书，因为这些书一般都比较易懂，而且语言比较轻松。买书前一定要慎重如果买到不好用的书可能会对自己的学习积极性产生打击。

　　对于已经掌握了VC的朋友：这种程度的开发者应该加深自己对系统原理，技术要点的认识。需要选择一些对原理讲解的比较透彻的书籍，这样一来才会对新技术有更多的了解，最好书中对技术的应用有一定的阐述。尽量选择示范代码必较精简的书，可以节约银子。

　　此外最好涉猎一些辅助性的书籍。
1.2 理解Windows消息机制

Windows系统是一个消息驱动的OS，什么是消息呢？我很难说得清楚，也很难下一个定义（谁在嘘我），我下面从不同的几个方面讲解一下，希望大家看了后有一点了解。 

1、消息的组成：一个消息由一个消息名称（UINT），和两个参数（WPARAM，LPARAM）。当用户进行了输入或是窗口的状态发生改变时系统都会发送消息到某一个窗口。例如当菜单转中之后会有WM_COMMAND消息发送，WPARAM的高字中（HIWORD(wParam)）是命令的ID号，对菜单来讲就是菜单ID。当然用户也可以定义自己的消息名称，也可以利用自定义消息来发送通知和传送数据。 

2、谁将收到消息：一个消息必须由一个窗口接收。在窗口的过程（WNDPROC）中可以对消息进行分析，对自己感兴趣的消息进行处理。例如你希望对菜单选择进行处理那么你可以定义对WM_COMMAND进行处理的代码，如果希望在窗口中进行图形输出就必须对WM_PAINT进行处理。 

3、未处理的消息到那里去了：M$为窗口编写了默认的窗口过程，这个窗口过程将负责处理那些你不处理消息。正因为有了这个默认窗口过程我们才可以利用Windows的窗口进行开发而不必过多关注窗口各种消息的处理。例如窗口在被拖动时会有很多消息发送，而我们都可以不予理睬让系统自己去处理。 

4、窗口句柄：说到消息就不能不说窗口句柄，系统通过窗口句柄来在整个系统中唯一标识一个窗口，发送一个消息时必须指定一个窗口句柄表明该消息由那个窗口接收。而每个窗口都会有自己的窗口过程，所以用户的输入就会被正确的处理。例如有两个窗口共用一个窗口过程代码，你在窗口一上按下鼠标时消息就会通过窗口一的句柄被发送到窗口一而不是窗口二。

5、示例：下面有一段伪代码演示如何在窗口过程中处理消息 

LONG yourWndProc(HWND hWnd,UINT uMessageType,WPARAM wP,LPARAM)
{
switch(uMessageType)
{//使用SWITCH语句将各种消息分开
case(WM_PAINT):
doYourWindow(...);//在窗口需要重新绘制时进行输出
break;
case(WM_LBUTTONDOWN):
doYourWork(...);//在鼠标左键被按下时进行处理
break;
default:
callDefaultWndProc(...);//对于其它情况就让系统自己处理
break;
}
}

接下来谈谈什么是消息机制：系统将会维护一个或多个消息队列，所有产生的消息都回被放入或是插入队列中。系统会在队列中取出每一条消息，根据消息的接收句柄而将该消息发送给拥有该窗口的程序的消息循环。每一个运行的程序都有自己的消息循环，在循环中得到属于自己的消息并根据接收窗口的句柄调用相应的窗口过程。而在没有消息时消息循环就将控制权交给系统所以Windows可以同时进行多个任务。下面的伪代码演示了消息循环的用法： 

while(1)
{
id=getMessage(...);
if(id == quit)
break;
translateMessage(...);
}

当该程序没有消息通知时getMessage就不会返回，也就不会占用系统的CPU时间。 图示消息投递模式 

在16位的系统中系统中只有一个消息队列，所以系统必须等待当前任务处理消息后才可以发送下一消息到相应程序，如果一个程序陷如死循环或是耗时操作时系统就会得不到控制权。这种多任务系统也就称为协同式的多任务系统。Windows3.X就是这种系统。

而32位的系统中每一运行的程序都会有一个消息队列，所以系统可以在多个消息队列中转换而不必等待当前程序完成消息处理就可以得到控制权。这种多任务系统就称为抢先式的多任务系统。Windows95/NT就是这种系统。 

1.3 利用Visual C++/MFC开发Windows程序的优势

MFC借助C++的优势为Windows开发开辟了一片新天地，同时也借助ApplicationWizzard使开发者摆脱离了那些每次都必写基本代码，借助ClassWizard和消息映射使开发者摆脱了定义消息处理时那种混乱和冗长的代码段。更令人兴奋的是利用C++的封装功能使开发者摆脱Windows中各种句柄的困扰，只需要面对C++中的对象，这样一来使开发更接近开发语言而远离系统。（但我个人认为了解系统原理对开发很有帮助） 

正因为MFC是建立在C++的基础上，所以我强调C/C++语言基础对开发的重要性。利用C++的封装性开发者可以更容易理解和操作各种窗口对象；利用C++的派生性开发者可以减少开发自定义窗口的时间和创造出可重用的代码；利用虚拟性可以在必要时更好的控制窗口的活动。而且C++本身所具备的超越C语言的特性都可以使开发者编写出更易用，更灵活的代码。 

在MFC中对消息的处理利用了消息映射的方法，该方法的基础是宏定义实现，通过宏定义将消息分派到不同的成员函数进行处理。下面简单讲述一下这种方法的实现方法： 

代码如下
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd) 
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE() 
//}}AFX_MSG_MAP
ON_COMMAND(ID_FONT_DROPDOWN, DoNothing)
END_MESSAGE_MAP()
经过编译后，代码被替换为如下形式（这只是作讲解，实际情况比这复杂得多）：
//BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd) 
CMainFrame::newWndProc(...)
{
switch(...)
{
//{{AFX_MSG_MAP(CMainFrame)
// ON_WM_CREATE() 
case(WM_CREATE):
OnCreate(...);
break;
//}}AFX_MSG_MAP
// ON_COMMAND(ID_FONT_DROPDOWN, DoNothing)
case(WM_COMMAND):
if(HIWORD(wP)==ID_FONT_DROPDOWN)
{
DoNothing(...);
}
break;
//END_MESSAGE_MAP()
}
}

newWndProc就是窗口过程只要是该类的实例生成的窗口都使用该窗口过程。 

所以了解了Windows的消息机制在加上对消息映射的理解就很容易了解MFC开发的基本思路了。 

1.4 利用MFC进行开发的通用方法介绍 

以下是我在最初学习VC时所常用的开发思路和方法，希望能对初学VC的朋友有所帮助和启发。

1、开发需要读写文件的应用程序并且有简单的输入和输出可以利用单文档视结构。

2、开发注重交互的简单应用程序可以使用对话框为基础的窗口，如果文件读写简单这可利用CFile进行。

3、开发注重交互并且文件读写复杂的的简单应用程序可以利用以CFormView为基础视的单文档视结构。

4、利用对话框得到用户输入的数据，在等级提高后可使用就地输入。

5、在对多文档要求不强烈时尽量避免多文档视结构，可以利用分隔条产生单文档多视结构。

6、在要求在多个文档间传递数据时使用多文档视结构。

7、学会利用子窗口，并在自定义的子窗口包含多个控件达到封装功能的目的。

8、尽量避免使用多文档多视结构。

9、不要使用多重继承并尽量减少一个类中封装过多的功能。

1.5 MFC中常用类，宏，函数介绍

常用类

CRect：用来表示矩形的类，拥有四个成员变量：top left bottom right。分别表是左上角和右下角的坐标。可以通过以下的方法构造： 

CRect( int l, int t, int r, int b ); 指明四个坐标 

CRect( const RECT& srcRect ); 由RECT结构构造 

CRect( LPCRECT lpSrcRect ); 由RECT结构构造 

CRect( POINT point, SIZE size ); 有左上角坐标和尺寸构造 

CRect( POINT topLeft, POINT bottomRight ); 有两点坐标构造 

下面介绍几个成员函数： 

int Width( ) const; 得到宽度 
int Height( ) const; 得到高度 
CSize Size( ) const; 得到尺寸 
CPoint& TopLeft( ); 得到左上角坐标 
CPoint& BottomRight( ); 得到右下角坐标 
CPoint CenterPoint( ) const; 得当中心坐标 
此外矩形可以和点（CPoint）相加进行位移，和另一个矩形相加得到“并”操作后的矩形。 

CPoint：用来表示一个点的坐标，有两个成员变量：x y。 可以和另一个点相加。

CString：用来表示可变长度的字符串。使用CString可不指明内存大小，CString会根据需要自行分配。下面介绍几个成员函数： 

GetLength 得到字符串长度 
GetAt 得到指定位置处的字符 
operator + 相当于strcat 
void Format( LPCTSTR lpszFormat, ... ); 相当于sprintf 
Find 查找指定字符，字符串 
Compare 比较 
CompareNoCase 不区分大小写比较 
MakeUpper 改为小写 
MakeLower 改为大写 

CStringArray：用来表示可变长度的字符串数组。数组中每一个元素为CString对象的实例。下面介绍几个成员函数： 

Add 增加CString 
RemoveAt 删除指定位置CString对象 
RemoveAll 删除数组中所有CString对象 
GetAt 得到指定位置的CString对象 
SetAt 修改指定位置的CString对象 
InsertAt 在某一位置插入CString对象 

常用宏

RGB

TRACE

ASSERT

VERIFY

常用函数

CWindApp* AfxGetApp();

HINSTANCE AfxGetInstanceHandle( );

HINSTANCE AfxGetResourceHandle( );

int AfxMessageBox( LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0 );用于弹出一个消息框

2.1 和GUI有关的各种对象

在Windows中有各种GUI对象（不要和C++对象混淆），当你在进行绘图就需要利用这些对象。而各种对象都拥有各种属性，下面分别讲述各种GUI对象和拥有的属性。

字体对象CFont用于输出文字时选用不同风格和大小的字体。可选择的风格包括：是否为斜体，是否为粗体，字体名称，是否有下划线等。颜色和背景色不属于字体的属性。关于如何创建和使用字体在2.2 在窗口中输出文字中会详细讲解。

刷子CBrush对象决定填充区域时所采用的颜色或模板。对于一个固定色的刷子来讲它的属性为颜色，是否采用网格和网格的类型如水平的，垂直的，交叉的等。你也可以利用8*8的位图来创建一个自定义模板的刷子，在使用这种刷子填充时系统会利用位图逐步填充区域。关于如何创建和使用刷子在2.3 使用刷子，笔进行绘图中会详细讲解。

画笔CPen对象在画点和画线时有用。它的属性包括颜色，宽度，线的风格，如虚线，实线，点划线等。关于如何创建和使用画笔在2.3 使用刷子，笔进行绘图中会详细讲解。

位图CBitmap对象可以包含一幅图像，可以保存在资源中。关于如何使用位图在2.4 在窗口中绘制设备相关位图，图标，设备无关位图中会详细讲解。

还有一种特殊的GUI对象是多边形，利用多边形可以很好的限制作图区域或是改变窗口外型。关于如何创建和使用多边形在2.6 多边形和剪贴区域中会详细讲解。

在Windows中使用GUI对象必须遵守一定的规则。首先需要创建一个合法的对象，不同的对象创建方法不同。然后需要将该GUI对象选入DC中，同时保存DC中原来的GUI对象。如果选入一个非法的对象将会引起异常。在使用完后应该恢复原来的对象，这一点特别重要，如果保存一个临时对象在DC中，而在临时对象被销毁后可能引起异常。有一点必须注意，每一个对象在重新创建前必须销毁，下面的代码演示了这一种安全的使用方法： 

OnDraw(CDC* pDC)
{
CPen pen1,pen2;
pen1.CreatePen(PS_SOLID,2,RGB(128,128,128));//创建对象
pen2.CreatePen(PS_SOLID,2,RGB(128,128,0));//创建对象
CPen* pPenOld=(CPen*)pDC->SelectObject(&pen1);//选择对象进DC
drawWithPen1...
(CPen*)pDC->SelectObject(&pen2);//选择对象进DC
drawWithPen2...
pen1.DeleteObject();//再次创建前先销毁
pen1.CreatePen(PS_SOLID,2,RGB(0,0,0));//再次创建对象
(CPen*)pDC->SelectObject(&pen1);//选择对象进DC
drawWithPen1...
pDC->SelectObject(pOldPen);//恢复
}

此外系统中还拥有一些库存GUI对象，你可以利用CDC::SelectStockObject(SelectStockObject( int nIndex )选入这些对象，它们包括一些固定颜色的刷子，画笔和一些基本字体。 

BLACK_BRUSH Black brush.

DKGRAY_BRUSH Dark gray brush.

GRAY_BRUSH Gray brush.

HOLLOW_BRUSH Hollow brush.

LTGRAY_BRUSH Light gray brush.

NULL_BRUSH Null brush.

WHITE_BRUSH White brush.

BLACK_PEN Black pen.

NULL_PEN Null pen.

WHITE_PEN White pen.

ANSI_FIXED_FONT ANSI fixed system font.

ANSI_VAR_FONT ANSI variable system font.

DEVICE_DEFAULT_FONT Device-dependent font.

OEM_FIXED_FONT OEM-dependent fixed font.

SYSTEM_FONT The system font. By default, Windows uses the system font to draw menus, dialog-box controls, and other text. In Windows versions 3.0 and later, the system font is proportional width; earlier versions of Windows use a fixed-width system font.

SYSTEM_FIXED_FONT The fixed-width system font used in Windows prior to version 3.0. This object is available for compatibility with earlier versions of Windows.

DEFAULT_PALETTE Default color palette. This palette consists of the 20 static colors in the system palette. 
这些对象留在DC中是安全的，所以你可以利用选入库存对象来作为恢复DC中GUI对象。 

大家可能都注意到了绘图时都需要一个DC对象，DC（Device Context设备环境）对象是一个抽象的作图环境，可能是对应屏幕，也可能是对应打印机或其它。这个环境是设备无关的，所以你在对不同的设备输出时只需要使用不同的设备环境就行了，而作图方式可以完全不变。这也就是Windows耀眼的一点设备无关性。如同你将对一幅画使用照相机或复印机将会产生不同的输出，而不需要对画进行任何调整。DC的使用会穿插在本章中进行介绍。

2.2 在窗口中输出文字

在这里我假定读者已经利用ApplicationWizard生成了一个SDI界面的程序代码。接下来的你只需要在CView派生类的OnDraw成员函数中加入绘图代码就可以了。在这里我需要解释一下OnDraw函数的作用，OnDraw函数会在窗口需要重绘时自动被调用，传入的参数CDC* pDC对应的就是DC环境。使用OnDraw的优点就在于在你使用打印功能的时候传入OnDraw的DC环境将会是打印机绘图环境，使用打印预览时传入的是一个称为CPreviewDC的绘图环境，所以你只需要一份代码就可以完成窗口/打印预览/打印机绘图三重功能。利用Windows的设备无关性和M$为打印预览所编写的上千行代码你可以很容易的完成一个具有所见即所得的软件。

输出文字一般使用CDC::BOOL TextOut( int x, int y, const CString& str )和CDC::int DrawText( const CString& str, LPRECT lpRect, UINT nFormat )两个函数，对TextOut来讲只能输出单行的文字，而DrawText可以指定在一个矩形中输出单行或多行文字，并且可以规定对齐方式和使用何种风格。nFormat可以是多种以下标记的组合（利用位或操作）以达到选择输出风格的目的。 

DT_BOTTOM底部对齐 Specifies bottom-justified text. This value must be combined with DT_SINGLELINE.

DT_CALCRECT计算指定文字时所需要矩形尺寸 Determines the width and height of the rectangle. If there are multiple lines of text, DrawText will use the width of the rectangle pointed to by lpRect and extend the base of the rectangle to bound the last line of text. If there is only one line of text, DrawText will modify the right side of the rectangle so that it bounds the last character in the line. In either case, DrawText returns the height of the formatted text, but does not draw the text.

DT_CENTER中部对齐 Centers text horizontally.

DT_END_ELLIPSIS or DT_PATH_ELLIPSIS Replaces part of the given string with ellipses, if necessary, so that the result fits in the specified rectangle. The given string is not modified unless the DT_MODIFYSTRING flag is specified. 
You can specify DT_END_ELLIPSIS to replace characters at the end of the string, or DT_PATH_ELLIPSIS to replace characters in the middle of the string. If the string contains backslash (/) characters, DT_PATH_ELLIPSIS preserves as much as possible of the text after the last backslash.

DT_EXPANDTABS Expands tab characters. The default number of characters per tab is eight.

DT_EXTERNALLEADING Includes the font抯 external leading in the line height. Normally, external leading is not included in the height of a line of text.

DT_LEFT左对齐 Aligns text flush-left.

DT_MODIFYSTRING Modifies the given string to match the displayed text. This flag has no effect unless the DT_END_ELLIPSIS or DT_PATH_ELLIPSIS flag is specified. 
Note Some uFormat flag combinations can cause the passed string to be modified. Using DT_MODIFYSTRING with either DT_END_ELLIPSIS or DT_PATH_ELLIPSIS may cause the string to be modified, causing an assertion in the CString override.

DT_NOCLIP Draws without clipping. DrawText is somewhat faster when DT_NOCLIP is used.

DT_NOPREFIX禁止使用&前缀 Turns off processing of prefix characters. Normally, DrawText interprets the ampersand (&) mnemonic-prefix character as a directive to underscore the character that follows, and the two-ampersand (&&) mnemonic-prefix characters as a directive to print a single ampersand. By specifying DT_NOPREFIX, this processing is turned off.

DT_PATH_ELLIPSIS 

DT_RIGHT右对齐 Aligns text flush-right.

DT_SINGLELINE单行输出 Specifies single line only. Carriage returns and linefeeds do not break the line.

DT_TABSTOP设置TAB字符所占宽度 Sets tab stops. The high-order byte of nFormat is the number of characters for each tab. The default number of characters per tab is eight.

DT_TOP定部对齐 Specifies top-justified text (single line only).

DT_VCENTER中部对齐 Specifies vertically centered text (single line only).

DT_WORDBREAK每行只在单词间被折行 Specifies word-breaking. Lines are automatically broken between words if a word would extend past the edge of the rectangle specified by lpRect. A carriage return杔inefeed sequence will also break the line. 

在输出文字时如果希望改变文字的颜色，你可以利用CDC::SetTextColor( COLORREF crColor )进行设置，如果你希望改变背景色就利用CDC::SetBkColor( COLORREF crColor )，很多时候你可能需要透明的背景色你可以利用CDC::SetBkMode( int nBkMode )设置，可接受的参数有 

OPAQUE Background is filled with the current background color before the text, hatched brush, or pen is drawn. This is the default background mode.

TRANSPARENT Background is not changed before drawing. 

接下来讲讲如何创建字体，你可以创建的字体有两种：库存字体CDC::CreateStockObject( int nIndex )和自定义字体。
在创建非库存字体时需要填充一个LOGFONT结构并使用CFont::CreateFontIndirect(const LOGFONT* lpLogFont )（可以参考文章在同一系统中显示GB字符和BIG5字符），或使用CFont::CreateFont( int nHeight, int nWidth, int nEscapement, int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline, BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision, BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily, LPCTSTR lpszFacename )其中的参数和LOGFONT中的分量有一定的对应关系。下面分别讲解参数的意义：

nHeight 字体高度（逻辑单位）等于零为缺省高度，否则取绝对值并和可用的字体高度进行匹配。
nWidth 宽度（逻辑单位）如果为零则使用可用的横纵比进行匹配。
nEscapement 出口矢量与X轴间的角度
nOrientation 字体基线与X轴间的角度
nWeight 字体粗细，可取以下值
Constant Value 
FW_DONTCARE 0 
FW_THIN 100 
FW_EXTRALIGHT 200 
FW_ULTRALIGHT 200 
FW_LIGHT 300 
FW_NORMAL 400 
FW_REGULAR 400 
FW_MEDIUM 500 
FW_SEMIBOLD 600 
FW_DEMIBOLD 600 
FW_BOLD 700 
FW_EXTRABOLD 800 
FW_ULTRABOLD 800 
FW_BLACK 900 
FW_HEAVY 900 

bItalic 是否为斜体
bUnderline 是否有下划线
cStrikeOut 是否带删除线
nCharSet 指定字符集合，可取以下值
Constant Value 
ANSI_CHARSET 0 
DEFAULT_CHARSET 1 
SYMBOL_CHARSET 2 
SHIFTJIS_CHARSET 128 
OEM_CHARSET 255 

nOutPrecision 输出精度
OUT_CHARACTER_PRECIS OUT_STRING_PRECIS 
OUT_DEFAULT_PRECIS OUT_STROKE_PRECIS 
OUT_DEVICE_PRECIS OUT_TT_PRECIS 
OUT_RASTER_PRECIS 

nClipPrecision 剪辑精度，可取以下值
CLIP_CHARACTER_PRECIS CLIP_MASK 
CLIP_DEFAULT_PRECIS CLIP_STROKE_PRECIS 
CLIP_ENCAPSULATE CLIP_TT_ALWAYS 
CLIP_LH_ANGLES 

nQuality 输出质量，可取以下值

DEFAULT_QUALITY Appearance of the font does not matter.

DRAFT_QUALITY Appearance of the font is less important than when PROOF_QUALITY is used. For GDI raster fonts, scaling is enabled. Bold, italic, underline, and strikeout fonts are synthesized if necessary.

PROOF_QUALITY Character quality of the font is more important than exact matching of the logical-font attributes. For GDI raster fonts, scaling is disabled and the font closest in size is chosen. Bold, italic, underline, and strikeout fonts are synthesized if necessary. 
nPitchAndFamily 字体间的间距
lpszFacename 指定字体名称，为了得到系统所拥有的字体可以利用EmunFontFamiliesEx。（可以参考文章在同一系统中显示GB字符和BIG5字符）

此外可以利用CFontDialog来得到用户选择的字体的LOGFONT数据。

最后我讲一下文本坐标的计算，利用CDC::GetTextExtent( const CString& str )可以得到字符串的在输出时所占用的宽度和高度，这样就可以在手工输出多行文字时使用正确的行距。另外如果需要更精确的对字体高度和宽度进行计算就需要使用CDC::GetTextMetrics( LPTEXTMETRIC lpMetrics ) 该函数将会填充TEXTMETRIC结构，该结构中的分量可以非常精确的描述字体的各种属性。

2.3 使用点，刷子，笔进行绘图

在Windows中画点的方法很简单，只需要调用COLORREF CDC::SetPixel( int x, int y, COLORREF crColor )就可以在指定点画上指定颜色，同时返回原来的颜色。COLORREF CDC::GetPixel( int x, int y)可以得到指定点的颜色。在Windows中应该少使用画点的函数，因为这样做的执行效率比较低。

刷子和画笔在Windows作图中是使用最多的GUI对象，本节在讲解刷子和画笔使用方法的同时也讲述一写基本作图函数。

在画点或画线时系统使用当前DC中的画笔，所以在创建画笔后必须将其选入DC才会在绘图时产生效果。画笔可以通过CPen对象来产生，通过调用CPen::CreatePen( int nPenStyle, int nWidth, COLORREF crColor )来创建。其中nPenStyle指名画笔的风格，可取如下值： 

PS_SOLID 实线 Creates a solid pen.

PS_DASH 虚线，宽度必须为一 Creates a dashed pen. Valid only when the pen width is 1 or less, in device units.

PS_DOT 点线，宽度必须为一 Creates a dotted pen. Valid only when the pen width is 1 or less, in device units.

PS_DASHDOT 点划线，宽度必须为一 Creates a pen with alternating dashes and dots. Valid only when the pen width is 1 or less, in device units.

PS_DASHDOTDOT 双点划线，宽度必须为一 Creates a pen with alternating dashes and double dots. Valid only when the pen width is 1 or less, in device units.

PS_NULL 空线，使用时什么也不会产生 Creates a null pen.

PS_ENDCAP_ROUND 结束处为圆形 End caps are round.

PS_ENDCAP_SQUARE 结束处为方形 End caps are square.

nWidth和crColor为线的宽度和颜色。 

刷子是在画封闭曲线时用来填充的颜色，例如当你画圆形或方形时系统会用当前的刷子对内部进行填充。刷子可利用CBrush对象产生。通过以下几种函数创建刷子： 

BOOL CreateSolidBrush( COLORREF crColor ); 创建一种固定颜色的刷子 
BOOL CreateHatchBrush( int nIndex, COLORREF crColor ); 创建指定颜色和网格的刷子，nIndex可取以下值： 
HS_BDIAGONAL Downward hatch (left to right) at 45 degrees

HS_CROSS Horizontal and vertical crosshatch

HS_DIAGCROSS Crosshatch at 45 degrees

HS_FDIAGONAL Upward hatch (left to right) at 45 degrees

HS_HORIZONTAL Horizontal hatch

HS_VERTICAL Vertical hatch 
BOOL CreatePatternBrush( CBitmap* pBitmap ); 创建以8*8位图为模板的刷子 

在选择了画笔和刷子后就可以利用Windows的作图函数进行作图了，基本的画线函数有以下几种 

CDC::MoveTo( int x, int y ); 改变当前点的位置 
CDC::LineTo( int x, int y ); 画一条由当前点到参数指定点的线 
CDC::BOOL Arc( LPCRECT lpRect, POINT ptStart, POINT ptEnd ); 画弧线 
CDC::BOOL Polyline( LPPOINT lpPoints, int nCount ); 将多条线依次序连接 
基本的作图函数有以下几种： 
CDC::BOOL Rectangle( LPCRECT lpRect ); 矩形 
CDC::RoundRect( LPCRECT lpRect, POINT point ); 圆角矩形 
CDC::Draw3dRect( int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight ); 3D边框 
CDC::Chord( LPCRECT lpRect, POINT ptStart, POINT ptEnd ); 扇形 
CDC::Ellipse( LPCRECT lpRect ); 椭圆形 
CDC::Pie( LPCRECT lpRect, POINT ptStart, POINT ptEnd ); 
CDC::Polygon( LPPOINT lpPoints, int nCount ); 多边形 
对于矩形，圆形或类似的封闭曲线，系统会使用画笔绘制边缘，使用刷子填充内部。如果你不希望填充或是画出边缘，你可以选入空刷子（NULL_PEN）或是（NULL_BRUSH）空笔。 

下面的代码创建一条两象素宽的实线并选入DC。并进行简单的作图： 
{
...
CPen pen;
pen.CreatePen(PS_SOLID,2,RGB(128,128,128));
CPen* pOldPen=(CPen*)dc.SelectObject(&pen);
dc.SelectStockObject(NULL_BRUSH);//选入空刷子
dc.Rectangle(CRect(0,0,20,20));//画矩形
...

2.4 在窗口中绘制设备相关位图，图标，设备无关位图

在Windows中可以将预先准备好的图像复制到显示区域中，这种内存拷贝执行起来是非常快的。在Windows中提供了两种使用图形拷贝的方法：通过设备相关位图（DDB）和设备无关位图（DIB）。

DDB可以用MFC中的CBitmap来表示，而DDB一般是存储在资源文件中，在加载时只需要通过资源ID号就可以将图形装入。BOOL CBitmap::LoadBitmap( UINT nIDResource )可以装入指定DDB，但是在绘制时必须借助另一个和当前绘图DC兼容的内存DC来进行。通过CDC::BitBlt( int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop )绘制图形，同时指定光栅操作的类型。BitBlt可以将源DC中位图复制到目的DC中，其中前四个参数为目的区域的坐标，接下来是源DC指针，然后是源DC中的起始坐标，由于BitBlt为等比例复制，所以不需要再次指定长宽，（StretchBlt可以进行缩放）最后一个参数为光栅操作的类型，可取以下值： 

BLACKNESS 输出区域为黑色 Turns all output black.

DSTINVERT 反色输出区域 Inverts the destination bitmap.

MERGECOPY 在源和目的间使用AND操作 Combines the pattern and the source bitmap using the Boolean AND operator.

MERGEPAINT 在反色后的目的和源间使用OR操作 Combines the inverted source bitmap with the destination bitmap using the Boolean OR operator.

NOTSRCCOPY 将反色后的源拷贝到目的区 Copies the inverted source bitmap to the destination.

PATINVERT 源和目的间进行XOR操作 Combines the destination bitmap with the pattern using the Boolean XOR operator.

SRCAND 源和目的间进行AND操作 Combines pixels of the destination and source bitmaps using the Boolean AND operator.

SRCCOPY 复制源到目的区 Copies the source bitmap to the destination bitmap.

SRCINVERT 源和目的间进行XOR操作 Combines pixels of the destination and source bitmaps using the Boolean XOR operator.

SRCPAINT 源和目的间进行OR操作 Combines pixels of the destination and source bitmaps using the Boolean OR operator.

WHITENESS 输出区域为白色 Turns all output white. 
下面用代码演示这种方法： 
CYourView::OnDraw(CDC* pDC)
{
CDC memDC;//定义一个兼容DC
memDC.CreateCompatibleDC(pDC);//创建DC
CBitmap bmpDraw;
bmpDraw.LoadBitmap(ID_BMP);//装入DDB
CBitmap* pbmpOld=memDC.SelectObject(&bmpDraw);//保存原有DDB，
并选入新DDB入DC
pDC->BitBlt(0,0,20,20,&memDC,0,0,SRCCOPY);//将源DC中(0,0,20,20)
复制到目的DC(0,0,20,20)
pDC->BitBlt(20,20,40,40,&memDC,0,0,SRCAND);//将源DC中(0,0,20,20)
和目的DC(20,20,40,40)中区域进行AND操作
memDC.SelectObject(pbmpOld);//选入原DDB
}

（图标并不是一个GDI对象，所以不需要选入DC）在MFC中没有一个专门的图标类，因为图标的操作比较简单，使用HICON CWinApp::LoadIcon( UINT nIDResource )或是HICON CWinApp::LoadStandardIcon( LPCTSTR lpszIconName ) 装入后就可以利用BOOL CDC::DrawIcon( int x, int y, HICON hIcon )绘制。由于在图标中可以指定透明区域，所以在某些需要使用非规则图形而且面积不大的时候使用图标会比较简单。下面给出简单的代码： 

OnDraw(CDC* pDC)
{
HICON hIcon1=AfxGetApp()->LoadIcon(IDI_I1);
HICON hIcon2=AfxGetApp()->LoadIcon(IDI_I2);
pDC->DrawIcon(0,0,hIcon1);
pDC->DrawIcon(0,40,hIcon2);
DestroyIcon(hIcon1);
DestroyIcon(hIcon2);
}

同样在MFC也没有提供一个DIB的类，所以在使用DIB位图时我们需要自己读取位图文件中的头信息， 并读入数据，并利用API函数StretchDIBits绘制。位图文件以BITMAPFILEHEADER结构开始，然后是BITMAPINFOHEADER 结构和调色版信息和数据，其实位图格式是图形格式中最简单的一种，而且也是Windows可以理解的一种。我不详细 讲解DIB位图的结构，提供一个CDib类供大家使用，这个类包含了基本的功能如：Load,Save,Draw。

2.5 使用各种映射方式

所谓的映射方式简单点讲就是坐标的安排方式，系统默认的映射方式为MM_TEXT即X坐标向右增加，Y坐标向下增加，(0,0)在屏幕左上方，DC中的每一点就是屏幕上的一个象素。也许你会认为这种方式下是最好理解的，但是一个点和象素对应的关系在屏幕上看来是正常的，但到了打印机上就会很不正常。因为我们作图是以点为单位并且打印机的分辨率远远比显示器高（800DPI 800点每英寸）所以在打印机上图形看起来就会很小。这样就需要为打印另做一套代码而加大了工作量。如果每个点对应0.1毫米那么在屏幕上的图形就会和打印出来的图形一样大小。

通过int CDC::SetMapMode( int nMapMode )可以指定映射方式，可用的有以下几种： 

MM_HIENGLISH 每点对应0.001英寸 Each logical unit is converted to 0.001 inch. Positive x is to the right; positive y is up.

MM_HIMETRIC 每点对应0.001毫米 Each logical unit is converted to 0.01 millimeter. Positive x is to the right; positive y is up.

MM_LOENGLISH 每点对应0.01英寸 Each logical unit is converted to 0.01 inch. Positive x is to the right; positive y is up.

MM_LOMETRIC 每点对应0.001毫米 Each logical unit is converted to 0.1 millimeter. Positive x is to the right; positive y is up.

MM_TEXT 象素对应 Each logical unit is converted to 1 device pixel. Positive x is to the right; positive y is down.

以上几种映射默认的原点在屏幕左上方。除MM_TEXT外都为X坐标向右增加，Y坐标向上增加，和自然坐标是一致的。所以在作图是要注意什么时候应该使用负坐标。而且以上的映射都是X-Y等比例的，即相同的长度在X，Y轴上显示的长度都是相同的。 

另外的一种映射方式为MM_ANISOTROPIC，这种方式可以规定不同的长宽比例。在设置这中映射方式后必须调用CSize CDC::SetWindowExt( SIZE size )和CSize CDC::SetViewportExt( SIZE size )来设定长宽比例。系统会根据两次设定的长宽的比值来确定长宽比例。下面给出一段代码比较映射前后的长宽比例： 

OnDraw(CDC* pDC)
{
CRect rcC1(200,0,400,200);
pDC->FillSolidRect(rcC1,RGB(0,0,255));
pDC->SetMapMode(MM_ANISOTROPIC );
CSize sizeO;
sizeO=pDC->SetWindowExt(5,5);
TRACE("winExt %d %d/n",sizeO.cx,sizeO.cy);
sizeO=pDC->SetViewportExt(5,10);
TRACE("ViewExt %d %d/n",sizeO.cx,sizeO.cy);
CRect rcC(0,0,200,200);
pDC->FillSolidRect(rcC,RGB(0,128,0));
}

上面代码在映射后画出的图形将是一个长方形。 

最后讲讲视原点（viewport origin）,你可以通过调用CPoint CDC::SetViewportOrg( POINT point )重新设置原点的位置，这就相对于对坐标进行了位移。例如你将原点设置在(20,20)那么原来的(0,0)就变成了(-20,-20)。

2.6 多边形和剪贴区域

多边形也是一个GDI对象，同样遵守其他GDI对象的规则，只是通常都不将其选入DC中。在MFC中多边形有CRgn表示。多边形用来表示一个不同与矩形的区域，和矩形具有相似的操作。如：检测某点是否在内部，并操作等。此外还得到一个包含此多边形的最小矩形。下面介绍一下多边形类的成员函数： 

CreateRectRgn 由矩形创建一个多边形 
CreateEllipticRgn 由椭圆创建一个多边形 
CreatePolygonRgn 创建一个有多个点围成的多边形 
PtInRegion 某点是否在内部 
CombineRgn 两个多边形相并 
EqualRgn 两个多边形是否相等 

在本节中讲演多边形的意义在于重新在窗口中作图时提高效率。因为引发窗口重绘的原因是某个区域失效，而失效的区域用多边形来表示。假设窗口大小为500*400当上方的另一个窗口从(0,0,10,10)移动到(20,20,30,30)这时(0,0,10,10)区域就失效了，而你只需要重绘这部分区域而不是所有区域，这样你程序的执行效率就会提高。

通过调用API函数int GetClipRgn( HDC hdc, HRGN hrgn)就可以得到失效区域，但是一般用不着那么精确而只需得到包含该区域的最小矩形就可以了，所以可以利用int CDC::GetClipBox( LPRECT lpRect )完成这一功能。

3.1 文档 视图 框架窗口间的关系和消息传送规律

在MFC中M$引入了文档-视结构的概念，文档相当于数据容器，视相当于查看数据的窗口或是和数据发生交互的窗口。（这一结构在MFC中的OLE，ODBC开发时又得到更多的拓展）因此一个完整的应用一般由四个类组成：CWinApp应用类，CFrameWnd窗口框架类，CDocument文档类，CView视类。（VC6中支持创建不带文档-视的应用）

在程序运行时CWinApp将创建一个CFrameWnd框架窗口实例，而框架窗口将创建文档模板，然后有文档模板创建文档实例和视实例，并将两者关联。一般来讲我们只需对文档和视进行操作，框架的各种行为已经被MFC安排好了而不需人为干预，这也是M$设计文档-视结构的本意，让我们将注意力放在完成任务上而从界面编写中解放出来。

在应用中一个视对应一个文档，但一个文档可以包含多个视。一个应用中只用一个框架窗口，对多文档界面来讲可能有多个MDI子窗口。每一个视都是一个子窗口，在单文档界面中父窗口即是框架窗口，在多文档界面中父窗口为MDI子窗口。一个多文档应用中可以包含多个文档模板，一个模板定义了一个文档和一个或多个视之间的对应关系。同一个文档可以属于多个模板，但一个模板中只允许定义一个文档。同样一个视也可以属于多个文档模板。（不知道我说清楚没有）

接下来看看如何在程序中得到各种对象的指针： 

全局函数AfxGetApp可以得到CWinApp应用类指针 
AfxGetApp()->m_pMainWnd为框架窗口指针 
在框架窗口中：CFrameWnd::GetActiveDocument得到当前活动文档指针 
在框架窗口中：CFrameWnd::GetActiveView得到当前活动视指针 
在视中：CView::GetDocument得到对应的文档指针 
在文档中：CDocument::GetFirstViewPosition，CDocument::GetNextView用来遍历所有和文档关联的视。 
在文档中：CDocument::GetDocTemplate得到文档模板指针 
在多文档界面中：CMDIFrameWnd::MDIGetActive得到当前活动的MDI子窗口 

一般来讲用户输入消息（如菜单选择，鼠标，键盘等）会先发往视，如果视未处理则会发往框架窗口。所以定义消息映射时定义在视中就可以了，如果一个应用同时拥有多个视而当前活动视没有对消息进行处理则消息会发往框架窗口。

3.2 接收用户输入

在视中接收鼠标输入：

鼠标消息是我们常需要处理的消息，消息分为：鼠标移动，按钮按下/松开，双击。利用ClassWizard可以轻松的添加这几种消息映射，下面分别讲解每种消息的处理。

WM_MOUSEMOVE对应的函数为OnMouseMove( UINT nFlags, CPoint point )，nFlags表明了当前一些按键的消息，你可以通过“位与”操作进行检测。 

MK_CONTROL Ctrl键是否被按下 Set if the CTRL key is down.

MK_LBUTTON 鼠标左键是否被按下 Set if the left mouse button is down.

MK_MBUTTON 鼠标中间键是否被按下 Set if the middle mouse button is down.

MK_RBUTTON 鼠标右键是否被按下 Set if the right mouse button is down.

MK_SHIFT Shift键是否被按下 Set if the SHIFT key is down. 
point表示当前鼠标的设备坐标，坐标原点对应视左上角。 

WM_LBUTTONDOWN/WM_RBUTTONDOWN（鼠标左/右键按下）对应的函数为OnLButtonDown/OnRButtonDown( UINT nFlags, CPoint point )参数意义和OnMouseMove相同。

WM_LBUTTONUP/WM_RBUTTONUP（鼠标左/右键松开）对应的函数为OnLButtonUp/OnRButtonUp( UINT nFlags, CPoint point )参数意义和OnMouseMove相同。

WM_LBUTTONDBLCLK/WM_RBUTTONDBLCLK（鼠标左/右键双击）对应的函数为OnLButtonDblClk/OnRButtonDblClk( UINT nFlags, CPoint point )参数意义和OnMouseMove相同。

下面我用一段伪代码来讲解一下这些消息的用法： 

代码的作用是用鼠标拉出一个矩形
global BOOL fDowned;//是否在拉动
global CPoint ptDown;//按下位置
global CPoint ptUp;//松开位置

OnLButtonDown(UINT nFlags, CPoint point)
{
fDowned=TRUE;
ptUp=ptDown=point;
DrawRect();
...
}

OnMouseMove(UINT nFlags, CPoint point)
{
if(fDowned)
{
DrawRect();//恢复上次所画的矩形
ptUp=point;
DrawRect();//画新矩形
}
}
OnLButtonUp(UINT nFlags, CPoint point)
{
if(fDowned)
{
DrawRect();//恢复上次所画的矩形
ptUp=point;
DrawRect();//画新矩形
fDowned=FALSE;
}
}

DrawRect()
{//以反色屏幕的方法画出ptDown,ptUp标记的矩形
CClientDC dc(this);
MakeRect(ptDown,ptUp);
SetROP(NOT);
Rect();
}

坐标间转换：在以上的函数中point参数对应的都是窗口的设备坐标，我们应该将设备坐标和逻辑坐标相区别，在图32_g1由于窗口使用了滚动条，所以传入的设备坐标是对应于当前窗口左上角的坐标，没有考虑是否滚动，而逻辑坐标必须考虑滚动后对应的坐标，所以我以黄线虚拟的表达一个逻辑坐标的区域。可以看得出同一点在滚动后的坐标值是不同的，这一规则同样适用于改变了映射方式的窗口，假设你将映射方式设置为每点为0.01毫米，那么设备坐标所对应的逻辑坐标也需要重新计算。进行这种转换需要写一段代码，所幸的是系统提供了进行转换的功能DC的DPtoLP，LPtoDP，下面给出代码完成由设备坐标到逻辑坐标的转换。 

CPoint CYourView::FromDP(CPoint point)
{
CClientDC dc(this);
CPoint ptRet=point;
dc.PrepareDC();//必须先准备DC，这在使用滚动时让DC重新计算坐标

//如果你作图设置了不同的映射方式，则在下面需要设置
dc.SetMapMode(...)
//
dc.DPtoLP(&ptRet);//DP->LP进行转换
return ptRet;
}

在图32_g1中以蓝线标记的是屏幕区域，红线标记的客户区域。利用ScreenToClient，ClientToScreen可以将坐标在这两个区域间转换。

在视中接收键盘输入：

键盘消息有三个：键盘被按下/松开，输入字符。其中输入字符相当于直接得到用户输入的字符这在不需要处理按键细节时使用，而键盘被按下/松开在按键状态改变时发送。

WM_CHAR对应的函数为OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )，其中nChar为被按下的字符，nRepCnt表明在长时间为松开时相当于的按键次数，nFlags中的不同位代表不同的含义，在这里一般不使用。

WM_KEYDOWN/WM_KEYUP所对应的函数为OnKeyDown/OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags )nChar代表按键的虚拟码值，如VK_ALT为ALT键，VK_CONTROL为Ctrl键。nFlags各位的含义如下： Value Description 
0? Scan code (OEM-dependent value). 
8 Extended key, such as a function key or a key on the numeric keypad (1 if it is an extended key). 
9?0 Not used. 
11?2 Used internally by Windows. 
13 Context code (1 if the ALT key is held down while the key is pressed; otherwise 0). 
14 Previous key state (1 if the key is down before the call, 0 if the key is up). 
15 Transition state (1 if the key is being released, 0 if the key is being pressed). 

3.3 使用菜单

利用菜单接受用户命令是一中很简单的交互方法，同时也是一种很有效的方法。通常菜单作为一中资源存储在文件中，因此我们可以在设计时就利用资源**编辑**器设计好一个菜单。关于使用VC设计菜单我就不再多讲了，但你在编写菜单时应该尽量在属性对话框的底部提示（Prompt）处输入文字，这虽然不是必要的，但MFC在有状态栏和工具条的情况下会使用该文字，文字的格式为“状态栏出说明/n工具条提示”。图33_g1

我们要面临的任务是如何知道用户何时选择了菜单，他选的是什么菜单项。当用户选择了一个有效的菜单项时系统会向应用发送一个WM_COMMAND消息，在消息的参数中表明来源。在MFC中我们只需要进行一次映射，将某一菜单ID映射到一处理函数，图33_g2。在这里我们在CView的派生类中处理菜单消息，同时我对同一ID设置两个消息映射，接下来将这两种映射的作用。

ON_COMMAND 映射的作用为在用户选择该菜单时调用指定的处理函数。如：ON_COMMAND(IDM_COMMAND1, OnCommand1)会使菜单被选择时调用OnCommand1成员函数。

ON_UPDATE_COMMAND_UI(IDM_COMMAND1, OnUpdateCommand1) 映射的作用是在菜单被显示时通过调用指定的函数来进行确定其状态。在这个处理函数中你可以设置菜单的允许/禁止状态，其显示字符串是什么，是否在前面打钩。函数的参数为CCmdUI* pCmdUI，CCmdUI是MFC专门为更新命令提供的一个类，你可以调用 

Enable 设置允许/禁止状态 
SetCheck 设置是否在前面打钩 
SetText 设置文字 

下面我讲解一个例子：我在CView派生类中有一个变量m_fSelected，并且在视中处理两个菜单的消息，当IDM_COMMAND1被选时，对m_fSelected进行逻辑非操作，当IDM_COMMAND2被选中时出一提示；同时IDM_COMMAND1根据m_fSelected决定菜单显示的文字和是否在前面打上检查符号，IDM_COMMAND2根据m_fSelected的值决定菜单的允许/禁止状态。下面是代码和说明： 

void CMenuDView::OnCommand1() 
{
m_fSelected=!m_fSelected;
TRACE("command1 selected/n");
}

void CMenuDView::OnUpdateCommand1(CCmdUI* pCmdUI) 
{
pCmdUI->SetCheck(m_fSelected);//决定检查状态
pCmdUI->SetText(m_fSelected?"当前被选中":"当前未被选中");//决定所显示的文字
}

void CMenuDView::OnUpdateCommand2(CCmdUI* pCmdUI) 
{//决定是否为允许
pCmdUI->Enable(m_fSelected);
}

void CMenuDView::OnCommand2() 
{//选中时给出提示
AfxMessageBox("你选了command2");
}

接下来再讲一些通过代码操纵菜单的方法，在MFC中有一个类CMenu用来处理和菜单有关的功能。在生成一个CMenu对象时你需要从资源中装如菜单，通过调用BOOL CMenu::LoadMenu( UINT nIDResource )进行装入，然后你就可以对菜单进行动态的修改，所涉及到的函数有： 

CMenu* GetSubMenu( int nPos ) 一位置得到子菜单的指针，因为一个CMenu对象只能表示一个弹出菜单，如果菜单中的某一项也为弹出菜单，就需要通过该函数获取指针。 
BOOL AppendMenu( UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL ) 在末尾添加一项，nFlag为MF_SEPARATOR表示增加一个分隔条，这样其他两个参数将会被忽略；为MF_STRING表示添加一个菜单项uIDNewItem为该菜单的ID命令值；为MF_POPUP表示添加一个弹出菜单项，这时uIDNewItem为另一菜单的句柄HMENU。lpszNewItem为菜单文字说明。 
BOOL InsertMenu( UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL )用于在指定位置插入一菜单，位置由变量nPosition指明。如果nFlags包含MF_BYPOSITION则表明插入在nPosition位置，如果包含MF_BYCOMMAND表示插入在命令ID为nPosition的菜单处。 
BOOL ModifyMenu( UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL )用于修改某一位置的菜单，如果nFlags包含MF_BYPOSITION则表明修改nPosition位置的菜单，如果包含MF_BYCOMMAND表示修改命令ID为nPosition处的菜单。 
BOOL RemoveMenu( UINT nPosition, UINT nFlags )用于删除某一位置的菜单。如果nFlags包含MF_BYPOSITION则表明删除nPosition位置的菜单，如果包含MF_BYCOMMAND表示删除命令ID为nPosition处的菜单。 
BOOL AppendMenu( UINT nFlags, UINT nIDNewItem, const CBitmap* pBmp ) 和 BOOL InsertMenu( UINT nPosition, UINT nFlags, UINT nIDNewItem, const CBitmap* pBmp )可以添加一位图菜单，但这样的菜单在选中时只是反色显示，并不美观。（关于使用自绘OwnerDraw菜单请参考我翻译的一篇文章自绘菜单类） 

视图中是没有菜单的，在框架窗口中才有，所以只有用AfxGetApp()->m_pMainWnd->GetMenu()才能得到应用的菜单指针。

最后我讲一下如何在程序中弹出一个菜单，你必须先装入一个菜单资源，你必需得到一个弹出菜单的指针然后调用BOOL TrackPopupMenu( UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = NULL )弹出菜单，你需要指定(x,y)为菜单弹出的位置，pWnd为接收命令消息的窗口指针。下面有一段代码说明方法，当然为了处理消息你应该在pWnd指明的窗口中对菜单命令消息进行映射。 

CMenu menu;
menu.LoadMenu(IDR_POPUP);
CMenu* pM=menu.GetSubMenu(0);
CPoint pt;
GetCursorPos(&pt);
pM->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);

另一种做法是通过CMenu::CreatePopupMenu()建立一个弹出菜单，然后使用TrackPopupMenu弹出菜单。使用CreatePopupMenu创建的菜单也可以将其作为一个弹出项添加另一个菜单中。下面的伪代码演示了如何创建一个弹出菜单并进行修改后弹出： 

CMenu menu1,menu2;
menu1.CreatePopupMenu
menu1.InsertMenu(1)
menu1.InsertMenu(2)
menu1.InsertMenu(3)

menu2.CreatePopupMenu
menu2.AppendMenu(MF_POPUP,1,menu1.Detach()) 将弹出菜单加入 or InsertMenu...
menu2.InsertMenu("string desc");
menu.TrackPopupMenu(...)

3.4 文档，视，框架之间相互作用

一般来说用户的输入/输出基本都是通过视进行，但一些例外的情况下可能需要和框架直接发生作用，而在多视的情况下如何在视之间传递数据。

在使用菜单时大家会发现当一个菜单没有进行映射处理时为禁止状态，在多视的情况下菜单的状态和处理映射是和当前活动视相联系的，这样MFC可以保证视能正确的接收到各种消息，但有时候也会产生不便。有一个解决办法就是在框架中对消息进行处理，这样也可以保证当前文档可以通过框架得到当前消息。

在用户进行输入后如何使视的状态得到更新？这个问题在一个文档对应一个视图时是不存在的，但是现在有一个文档对应了两个视图，当在一个视上进行了输入时如何保证另一个视也得到通知呢？MFC的做法是利用文档来处理的，因为文档管理着当前和它联系的视，由它来通知各个视是最合适的。让我们同时看两个函数： 

void CView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint ) 
void CDocument::UpdateAllViews( CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL ) 
当文档的UpdateAllViews被调用时和此文档相关的所有视的OnUpdate都会被调用，而参数lHint和pHint都会被传递。这样一来发生改变视就可以通知其他的兄弟了。那么还有一个问题：如何在OnUpdate中知道是那个视图发生了改变呢，这就可以利用pHint参数，只要调用者将this指针赋值给参数就可以了，当然完全可以利用该参数传递更复杂的结构。 

视的初始化，当一个文档被打开或是新建一个文档时视图的CView::OnInitialUpdate()会被调用，你可以通过重载该函数对视进行初始化，并在结束前调用父类的OnInitialUpdate，因为这样可以保证OnUpdate会被调用。

文档中内容的清除，当文档被关闭时（比如退出或是新建前上一个文档清除）void CDocument::DeleteContents ()会被调用，你可以通过重载该函数来进行清理工作。

在单文档结构中上面两点尤其重要，因为软件运行文档对象和视对象只会被产生并删除一次。所以应该将上面两点和C++对象构造和构析分清楚。

最后将一下文档模板（DocTemplate）的作用，文档模板分为两类单文档模板和多文档模板，分别由CSingleDocTemplate和CMultiDocTemplate表示，模板的作用在于记录文档，视，框架之间的对应关系。还有一点就是模板可以记录应用程序可以打开的文件的类型，当打开文件时会根据文档模板中的信息选择正确的文档和视。模板是一个比较抽想的概念，一般来说是不需要我们直接进行操作的。

当使用者通过视修改了数据时，应该调用GetDocument()->SetModifiedFlag(TRUE)通知文档数据已经被更新，这样在关闭文档时会自动询问用户是否保存数据。

3.5 利用序列化进行文件读写

在很多应用中我们需要对数据进行保存，或是从介质上读取数据，这就涉及到文件的操作。我们可以利用各种文件存取方法完成这些工作，但MFC中也提供了一种读写文件的简单方法——“序列化”。序列化机制通过更高层次的接口功能向开发者提供了更利于使用和透明于字节流的文件操纵方法，举一个例来讲你可以将一个字串写入文件而不需要理会具体长度，读出时也是一样。你甚至可以对字符串数组进行操作。在MFC提供的可自动分配内存的类的支持下你可以更轻松的读/写数据。你也可以根据需要编写你自己的具有序列化功能的类。

序列化在最低的层次上应该被需要序列化的类支持，也就是说如果你需要对一个类进行序列化，那么这个类必须支持序列化。当通过序列化进行文件读写时你只需要该类的序列化函数就可以了。

怎样使类具有序列化功能呢？你需要以下的工作： 

该类从CObject派生。 
在类声明中包括DECLARE_SERIAL宏定义。 
提供一个缺省的构造函数。 
在类中实现Serialze函数 
使用IMPLEMENT_SERIAL指明类名和版本号 

下面的代码建立了一个简单身份证记录的类，同时也能够支持序列化。 

in H
struct strPID
{
char szName[10];
char szID[16];
struct strPID* pNext;
};
class CAllPID : public CObject
{
public:
DECLARE_SERIAL(CAllPID)
CAllPID();
~CAllPID();

public:// 序列化相关 
struct strPID* pHead;
//其他的成员函数
void Serialize(CArchive& ar);
};

in CPP
IMPLEMENT_SERIAL(CAllPID,CObject,1) // version is 1，版本用于读数据时的检测
void CAllPID::Serialize(CArchive& ar)
{
int iTotal;
if(ar.IsStoring())
{//保存数据
iTotal=GetTotalID();//得到链表中的记录数量
arr<26;i++)
ar<<&(((BYTE*)pItem)+i);//写一个strPID中所有的数据
}
}
else
{//读数据
ar>>iTotal;
for(int i=0;i26;j++)
ar>>*(((BYTE*)pID)+j);//读一个strPID中所有的数据
//修改链表
}
}
}

当然上面的代码很不完整，但已经可以说明问题。这样CAllPID就是一个可以支持序列化的类，并且可以根据记录的数量动态分配内存。在序列化中我们使用了CArchive类，该类用于在序列化时提供读写支持，它重载了<<和>>运算符号，并且提供Read和Write函数对数据进行读写。 

下面看看如何在文档中使用序列化功能，你只需要修改文档类的Serialize(CArchive& ar)函数，并调用各个进行序列化的类的Serial进行数据读写就可以了。当然你也可以在文档类的内部进行数据读写，下面的代码利用序列化功能读写数据： 

class CYourDoc : public CDocument
{
void Serialize(CArchive& ar);
CString m_szDesc;
CAllPID m_allPID;
......
}

void CYourDoc::Serialize(CArchive& ar)
{
if (ar.IsStoring())
{//由于CString对CArchive定义了<<和>>操作符号，所以可以直接利用>>和<<
ar<>m_szDesc;
}
m_allPID.Serialize(ar);//调用数据类的序列化函数

} 
3.6 MFC中所提供的各种视类介绍

MFC中提供了丰富的视类供开发者使用，下面对各个类进行介绍：

CView类是最基本的视类只支持最基本的操作。

CScrollView类提供了滚动的功能，你可以利用void CScrollView::SetScrollSizes( int nMapMode, SIZE sizeTotal, const SIZE& sizePage = sizeDefault, const SIZE& sizeLine = sizeDefault )设置滚动尺寸，和坐标映射模式。但是在绘图和接收用户输入时需要对坐标进行转换。请参见3.2 接收用户输入。

CFormView类提供用户在资源文件中定义界面的能力，并可以将子窗口和变量进行绑定。通过UpdateData函数让数据在变量和子窗口间交换。

**CTreeView**类利用TreeCtrl界面作为视界面，通过调用CTreeCtrl& **CTreeView**::GetTreeCtrl( ) const得到CTreeCtrl的引用。

CListView类利用ListCtrl界面作为视界面，通过调用CTreeCtrl& **CTreeView**::GetTreeCtrl( ) const得到CListCtrl的引用。

CEditView类利用Edit接收用户输入，它具有输入框的一切功能。通过调用CEdit& CEditView::GetEditCtrl( ) const得到Edit&的引用。void CEditView::SetPrinterFont( CFont* pFont )可以设置打印字体。

CRichEditView类作为Rich Text Edit（富文本输入）的视类，提供了可以按照格式显示文本的能力，在使用时需要CRichEditDoc的支持。

4.1 Button

按钮窗口（控件）在MFC中使用CButton表示，CButton包含了三种样式的按钮，Push Button，Check Box，Radio Box。所以在利用CButton对象生成按钮窗口时需要指明按钮的风格。

创建按钮：BOOL CButton::Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );其中lpszCaption是按钮上显示的文字，dwStyle为按钮风格，除了Windows风格可以使用外（如WS_CHILD|WS_VISUBLE|WS_BORDER）还有按钮专用的一些风格。 

BS_AUTOCHECKBOX 检查框，按钮的状态会自动改变 Same as a check box, except that a check mark appears in the check box when the user selects the box; the check mark disappears the next time the user selects the box.

BS_AUTORADIOBUTTON 圆形选择按钮，按钮的状态会自动改变 Same as a radio button, except that when the user selects it, the button automatically highlights itself and removes the selection from any other radio buttons with the same style in the same group.

BS_AUTO3STATE 允许按钮有三种状态即：选中，未选中，未定 Same as a three-state check box, except that the box changes its state when the user selects it.

BS_CHECKBOX 检查框 Creates a small square that has text displayed to its right (unless this style is combined with the BS_LEFTTEXT style).

BS_DEFPUSHBUTTON 默认普通按钮 Creates a button that has a heavy black border. The user can select this button by pressing the ENTER key. This style enables the user to quickly select the most likely option (the default option).

BS_LEFTTEXT 左对齐文字 When combined with a radio-button or check-box style, the text appears on the left side of the radio button or check box.

BS_OWNERDRAW 自绘按钮 Creates an owner-drawn button. The framework calls the DrawItem member function when a visual aspect of the button has changed. This style must be set when using the CBitmapButton class.

BS_PUSHBUTTON 普通按钮 Creates a pushbutton that posts a WM_COMMAND message to the owner window when the user selects the button.

BS_RADIOBUTTON 圆形选择按钮 Creates a small circle that has text displayed to its right (unless this style is combined with the BS_LEFTTEXT style). Radio buttons are usually used in groups of related but mutually exclusive choices.

BS_3STATE 允许按钮有三种状态即：选中，未选中，未定 Same as a check box, except that the box can be dimmed as well as checked. The dimmed state typically is used to show that a check box has been disabled. 
rect为窗口所占据的矩形区域，pParentWnd为父窗口指针，nID为该窗口的ID值。 

获取/改变按钮状态：对于检查按钮和圆形按钮可能有两种状态，选中和未选中，如果设置了BS_3STATE或BS_AUTO3STATE风格就可能出现第三种状态：未定，这时按钮显示灰色。通过调用int CButton::GetCheck( ) 得到当前是否被选中，返回0：未选中，1：选中，2：未定。调用void CButton::SetCheck( int nCheck );设置当前选中状态。

处理按钮消息：要处理按钮消息需要在父窗口中进行消息映射，映射宏为ON_BN_CLICKED( id, memberFxn )id为按钮的ID值，就是创建时指定的nID值。处理函数原型为afx_msg void memberFxn( );

4.2 Static Box

静态文本控件的功能比较简单，可作为显示字符串，图标，位图用。创建一个窗口可以使用成员函数： 
BOOL CStatic::Create( LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff ); 
其中dwStyle将指明该窗口的风格，除了子窗口常用的风格WS_CHILD,WS_VISIBLE外，你可以针对静态控件指明专门的风格。 

SS_CENTER,SS_LEFT,SS_RIGHT 指明字符显示的对齐方式。 
SS_GRAYRECT 显示一个灰色的矩形 
SS_NOPREFIX 如果指明该风格，对于字符&将直接显示，否则&将作为转义符，&将不显示而在其后的字符将有下划线，如果需要直接显示&必须使用&&表示。 
SS_BITMAP 显示位图 
SS_ICON 显示图标 
SS_CENTERIMAGE 图象居中显示 

控制显示的文本利用成员函数SetWindowText/GetWindowText用于设置/得到当前显示的文本。

控制显示的图标利用成员函数SetIcon/GetIcon用于设置/得到当前显示的图标。

控制显示的位图利用成员函数SetBitmap/GetBitmap用于设置/得到当前显示的位图。下面一段代码演示如何创建一个显示位图的静态窗口并设置位图

CStatic* pstaDis=new CStatic;
pstaDis->Create("",WS_CHILD|WS_VISIBLE|SS_BITMAP|SSCENTERIMAGE,
CRect(0,0,40,40),pWnd,1);
CBitmap bmpLoad;
bmpLoad.LoadBitmap(IDB_TEST);
pstaDis->SetBitmap(bmpLoad.Detach());

4.3 Edit Box

Edit窗口是用来接收用户输入最常用的一个控件。创建一个输入窗口可以使用成员函数： 
BOOL CEdit::Create( LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff ); 
其中dwStyle将指明该窗口的风格，除了子窗口常用的风格WS_CHILD,WS_VISIBLE外，你可以针对输入控件指明专门的风格。 

ES_AUTOHSCROLL,ES_AUTOVSCROLL 指明输入文字超出显示范围时自动滚动。 
ES_CENTER,ES_LEFT,ES_RIGHT 指定对齐方式 
ES_MULTILINE 是否允许多行输入 
ES_PASSWORD 是否为密码输入框，如果指明该风格则输入的文字显示为* 
ES_READONLY 是否为只读 
ES_UPPERCASE,ES_LOWERCASE 显示大写/小写字符 

控制显示的文本利用成员函数SetWindowText/GetWindowText用于设置/得到当前显示的文本。

通过GetLimitText/SetLimitText可以得到/设置在输入框中输入的字符数量。

由于在输入时用户可能选择某一段文本，所以通过void CEdit::GetSel( int& nStartChar, int& nEndChar )得到用户选择的字符范围，通过调用void CEdit::SetSel( int nStartChar, int nEndChar, BOOL bNoScroll = FALSE )可以设置当前选择的文本范围，如果指定nStartChar=0 nEndChar=-1则表示选中所有的文本。void ReplaceSel( LPCTSTR lpszNewText, BOOL bCanUndo = FALSE )可以将选中的文本替换为指定的文字。

此外输入框还有一些和剪贴板有关的功能，void Clear( );删除选中的文本，void Copy( );可将选中的文本送入剪贴板，void Paste( );将剪贴板中内容插入到当前输入框中光标位置，void Cut( );相当于Copy和Clear结合使用。

最后介绍一下输入框几种常用的消息映射宏： 

ON_EN_CHANGE 输入框中文字更新后产生 
ON_EN_ERRSPACE 输入框无法分配内存时产生 
ON_EN_KILLFOCUS / ON_EN_SETFOCUS 在输入框失去/得到输入焦点时产生 
使用以上几种消息映射的方法为定义原型如：afx_msg void memberFxn( );的函数，并且定义形式如ON_Notification( id, memberFxn )的消息映射。如果在对话框中使用输入框，Class Wizard会自动列出相关的消息，并能自动产生消息映射代码。 

4.5 List Box/Check List Box

ListBox窗口用来列出一系列的文本，每条文本占一行。创建一个列表窗口可以使用成员函数： 
BOOL CListBox::Create( LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff ); 
其中dwStyle将指明该窗口的风格，除了子窗口常用的风格WS_CHILD,WS_VISIBLE外，你可以针对列表控件指明专门的风格。 

LBS_MULTIPLESEL 指明列表框可以同时选择多行 
LBS_EXTENDEDSEL 可以通过按下Shift/Ctrl键选择多行 
LBS_SORT 所有的行按照字母顺序进行排序 

在列表框生成后需要向其中加入或是删除行，可以利用： 
int AddString( LPCTSTR lpszItem )添加行， 
int DeleteString( UINT nIndex )删除指定行， 
int InsertString( int nIndex, LPCTSTR lpszItem )将行插入到指定位置。 
void ResetContent( )可以删除列表框中所有行。 
通过调用int GetCount( )得到当前列表框中行的数量。 

如果需要得到/设置当前被选中的行，可以调用int GetCurSel( )/int SetCurSel(int iIndex)。如果你指明了选择多行的风格，你就需要先调用int GetSelCount( )得到被选中的行的数量，然后int GetSelItems( int nMaxItems, LPINT rgIndex )得到所有选中的行，参数rgIndex为存放被选中行的数组。通过调用int GetLBText( int nIndex, LPTSTR lpszText )得到列表框内指定行的字符串。

此外通过调用int FindString( int nStartAfter, LPCTSTR lpszItem )可以在当前所有行中查找指定的字符传的位置，nStartAfter指明从那一行开始进行查找。 
int SelectString( int nStartAfter, LPCTSTR lpszItem )可以选中包含指定字符串的行。

在MFC 4.2版本中添加了CCheckListBox类，该类是由CListBox派生并拥有CListBox的所有功能，不同的是可以在每行前加上一个检查框。必须注意的是在创建时必须指明LBS_OWNERDRAWFIXED或LBS_OWNERDRAWVARIABLE风格。

通过void SetCheckStyle( UINT nStyle )/UINT GetCheckStyle( )可以设置/得到检查框的风格，关于检查框风格可以参考4.1 Button中介绍。通过void SetCheck( int nIndex, int nCheck )/int GetCheck( int nIndex )可以设置和得到某行的检查状态，关于检查框状态可以参考4.1 Button中介绍。

最后介绍一下列表框几种常用的消息映射宏： 

ON_LBN_DBLCLK 鼠标双击 
ON_EN_ERRSPACE 输入框无法分配内存时产生 
ON_EN_KILLFOCUS / ON_EN_SETFOCUS 在输入框失去/得到输入焦点时产生 
ON_LBN_SELCHANGE 选择的行发生改变 
使用以上几种消息映射的方法为定义原型如：afx_msg void memberFxn( );的函数，并且定义形式如ON_Notification( id, memberFxn )的消息映射。如果在对话框中使用列表框，Class Wizard会自动列出相关的消息，并能自动产生消息映射代码。 

4.6 Combo Box

组合窗口是由一个输入框和一个列表框组成。创建一个组合窗口可以使用成员函数： 
BOOL CListBox::Create( LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff ); 
其中dwStyle将指明该窗口的风格，除了子窗口常用的风格WS_CHILD,WS_VISIBLE外，你可以针对列表控件指明专门的风格。 

CBS_DROPDOWN 下拉式组合框 
CBS_DROPDOWNLIST 下拉式组合框，但是输入框内不能进行输入 
CBS_SIMPLE 输入框和列表框同时被显示 
LBS_SORT 所有的行按照字母顺序进行排序 

由于组合框内包含了列表框，所以列表框的功能都能够使用，如可以利用： 
int AddString( LPCTSTR lpszItem )添加行， 
int DeleteString( UINT nIndex )删除指定行， 
int InsertString( int nIndex, LPCTSTR lpszItem )将行插入到指定位置。 
void ResetContent( )可以删除列表框中所有行。 
通过调用int GetCount( )得到当前列表框中行的数量。 

如果需要得到/设置当前被选中的行的位置，可以调用int GetCurSel( )/int SetCurSel(int iIndex)。通过调用int GetLBText( int nIndex, LPTSTR lpszText )得到列表框内指定行的字符串。

此外通过调用int FindString( int nStartAfter, LPCTSTR lpszItem )可以在当前所有行中查找指定的字符传的位置，nStartAfter指明从那一行开始进行查找。 
int SelectString( int nStartAfter, LPCTSTR lpszItem )可以选中包含指定字符串的行。

此外输入框的功能都能够使用，如可以利用： 
DWORD GetEditSel( ) /BOOL SetEditSel( int nStartChar, int nEndChar )得到或设置输入框中被选中的字符位置。 
BOOL LimitText( int nMaxChars )设置输入框中可输入的最大字符数。 
输入框的剪贴板功能Copy,Clear,Cut,Paste动可以使用。 

最后介绍一下列表框几种常用的消息映射宏： 

ON_CBN_DBLCLK 鼠标双击 
ON_CBN_DROPDOWN 列表框被弹出 
ON_CBN_KILLFOCUS / ON_CBN_SETFOCUS 在输入框失去/得到输入焦点时产生 
ON_CBN_SELCHANGE 列表框中选择的行发生改变 
ON_CBN_EDITUPDATE 输入框中内容被更新 
使用以上几种消息映射的方法为定义原型如：afx_msg void memberFxn( );的函数，并且定义形式如ON_Notification( id, memberFxn )的消息映射。如果在对话框中使用组合框，Class Wizard会自动列出相关的消息，并能自动产生消息映射代码。 

4.7 Tree Ctrl

树形控件TreeCtrl和下节要讲的列表控件 ListCtrl在系统中大量被使用，例如Windows资源管理器就是一个典型的例子。

树形控件可以用于树形的结构，其中有一个根接点(Root)然后下面有许多子结点，而每个子结点上有允许有一个或多个或没有子结点。MFC中使用CTreeCtrl类来封装树形控件的各种操作。通过调用
BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );创建一个窗口，dwStyle中可以使用以下一些树形控件的专用风格： 

TVS_HASLINES 在父/子结点之间绘制连线 
TVS_LINESATROOT 在根/子结点之间绘制连线 
TVS_HASBUTTONS 在每一个结点前添加一个按钮，用于表示当前结点是否已被展开 
TVS_EDITLABELS 结点的显示字符可以被**编辑**
TVS_SHOWSELALWAYS 在失去焦点时也显示当前选中的结点 
TVS_DISABLEDRAGDROP 不允许Drag/Drop 
TVS_NOTOOLTIPS 不使用ToolTip显示结点的显示字符 
在树形控件中每一个结点都有一个句柄（HTREEITEM），同时添加结点时必须提供的参数是该结点的父结点句柄，（其中根Root结点只有一个，既不可以添加也不可以删除）利用
HTREEITEM InsertItem( LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST );可以添加一个结点，pszItem为显示的字符，hParent代表父结点的句柄，当前添加的结点会排在hInsertAfter表示的结点的后面，返回值为当前创建的结点的句柄。下面的代码会建立一个如下形式的树形结构： +--- Parent1 +--- Child1_1 +--- Child1_2 +--- Child1_3 +--- Parent2 +--- Parent3 /*假设m_tree为一个CTreeCtrl对象，而且该窗口已经创建*/ HTREEITEM hItem,hSubItem; hItem = m_tree.InsertItem("Parent1",TVI_ROOT); 在根结点上添加Parent1 hSubItem = m_tree.InsertItem("Child1_1",hItem); //在Parent1上添加一个子结点 hSubItem = m_tree.InsertItem("Child1_2",hItem,hSubItem);//在Parent1上添加一个子结点，排在Child1_1后面 hSubItem = m_tree.InsertItem("Child1_3",hItem,hSubItem); hItem = m_tree.InsertItem("Parent2",TVI_ROOT,hItem); hItem = m_tree.InsertItem("Parent3",TVI_ROOT,hItem); 如果你希望在每个结点前添加一个小图标，就必需先调用CImageList* SetImageList( CImageList * pImageList, int nImageListType );指明当前所使用的ImageList，nImageListType为TVSIL_NORMAL。在调用完成后控件中使用图片以设置的ImageList中图片为准。然后调用
HTREEITEM InsertItem( LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);添加结点，nImage为结点没被选中时所使用图片序号，nSelectedImage为结点被选中时所使用图片序号。下面的代码演示了ImageList的设置。 /*m_list 为CImageList对象 IDB_TREE 为16*(16*4)的位图，每个图片为16*16共4个图标*/ m_list.Create(IDB_TREE,16,4,RGB(0,0,0)); m_tree.SetImageList(&m_list,TVSIL_NORMAL); m_tree.InsertItem("Parent1",0,1);//添加， 选中时显示图标1，未选中时显示图标0 

此外CTreeCtrl还提供了一些函数用于得到/修改控件的状态。 
HTREEITEM GetSelectedItem( );将返回当前选中的结点的句柄。BOOL SelectItem( HTREEITEM hItem );将选中指明结点。 
BOOL GetItemImage( HTREEITEM hItem, int& nImage, int& nSelectedImage ) / BOOL SetItemImage( HTREEITEM hItem, int nImage, int nSelectedImage )用于得到/修改某结点所使用图标索引。 
CString GetItemText( HTREEITEM hItem ) /BOOL SetItemText( HTREEITEM hItem, LPCTSTR lpszItem );用于得到/修改某一结点的显示字符。 
BOOL DeleteItem( HTREEITEM hItem );用于删除某一结点，BOOL DeleteAllItems( );将删除所有结点。 

此外如果想遍历树可以使用下面的函数： 
HTREEITEM GetRootItem( );得到根结点。 
HTREEITEM GetChildItem( HTREEITEM hItem );得到子结点。 
HTREEITEM GetPrevSiblingItem/GetNextSiblingItem( HTREEITEM hItem );得到指明结点的上/下一个兄弟结点。 
HTREEITEM GetParentItem( HTREEITEM hItem );得到父结点。 

树形控件的消息映射使用ON_NOTIFY宏，形式如同：ON_NOTIFY( wNotifyCode, id, memberFxn )，wNotifyCode为通知代码，id为产生该消息的窗口ID，memberFxn为处理函数，函数的原型如同void OnXXXTree(NMHDR* pNMHDR, LRESULT* pResult)，其中pNMHDR为一数据结构，在具体使用时需要转换成其他类型的结构。对于树形控件可能取值和对应的数据结构为： 

TVN_SELCHANGED 在所选中的结点发生改变后发送，所用结构：NMTREEVIEW 
TVN_ITEMEXPANDED 在某结点被展开后发送，所用结构：NMTREEVIEW 
TVN_BEGINLABELEDIT 在开始**编辑**结点字符时发送，所用结构：NMTVDISPINFO 
TVN_ENDLABELEDIT 在结束**编辑**结点字符时发送，所用结构：NMTVDISPINFO 
TVN_GETDISPINFO 在需要得到某结点信息时发送，（如得到结点的显示字符）所用结构：NMTVDISPINFO 
关于ON_NOTIFY有很多内容，将在以后的内容中进行详细讲解。 

关于动态提供结点所显示的字符：首先你在添加结点时需要指明lpszItem参数为：LPSTR_TEXTCALLBACK。在控件显示该结点时会通过发送TVN_GETDISPINFO来取得所需要的字符，在处理该消息时先将参数pNMHDR转换为LPNMTVDISPINFO，然后填充其中item.pszText。但是我们通过什么来知道该结点所对应的信息呢，我的做法是在添加结点后设置其lParam参数，然后在提供信息时利用该参数来查找所对应的信息。下面的代码说明了这种方法： char szOut[8][3]={"No.1","No.2","No.3"}; //添加结点 HTREEITEM hItem = m_tree.InsertItem(LPSTR_TEXTCALLBACK,...) m_tree.SetItemData(hItem, 0 ); hItem = m_tree.InsertItem(LPSTR_TEXTCALLBACK,...) m_tree.SetItemData(hItem, 1 ); //处理消息 void CParentWnd::OnGetDispInfoTree(NMHDR* pNMHDR, LRESULT* pResult) { TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR; pTVDI->item.pszText=szOut[pTVDI->item.lParam];//通过lParam得到 需要显示的字符在数组中的位置 *pResult = 0; } 

关于**编辑**结点的显示字符：首先需要设置树形控件的TVS_EDITLABELS风格，在开始**编辑**时该控件将会发送TVN_BEGINLABELEDIT，你可以通过在处理函数中返回TRUE来取消接下来的**编辑**，在**编辑**完成后会发送TVN_ENDLABELEDIT，在处理该消息时需要将参数pNMHDR转换为LPNMTVDISPINFO，然后通过其中的item.pszText得到**编辑**后的字符，并重置显示字符。如果**编辑**在中途中取消该变量为NULL。下面的代码说明如何处理这些消息： //处理消息 TVN_BEGINLABELEDIT void CParentWnd::OnBeginEditTree(NMHDR* pNMHDR, LRESULT* pResult) { TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR; if(pTVDI->item.lParam==0);//判断是否取消该操作 *pResult = 1; else *pResult = 0; } //处理消息 TVN_BEGINLABELEDIT void CParentWnd::OnBeginEditTree(NMHDR* pNMHDR, LRESULT* pResult) { TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR; if(pTVDI->item.pszText==NULL);//判断是否已经取消取消**编辑** m_tree.SetItemText(pTVDI->item.hItem,pTVDI->pszText);//重置显示字符 *pResult = 0; } 上面讲述的方法所进行的消息映射必须在父窗口中进行（同样WM_NOTIFY的所有消息都需要在父窗口中处理）。 

4.8 List Ctrl

列表控件可以看作是功能增强的ListBox，它提供了四种风格，而且可以同时显示一列的多中属性值。MFC中使用CListCtrl类来封装列表控件的各种操作。通过调用
BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );创建一个窗口，dwStyle中可以使用以下一些列表控件的专用风格： 

LVS_ICON LVS_SMALLICON LVS_LIST LVS_REPORT 这四种风格决定控件的外观，同时只可以选择其中一种，分别对应：大图标显示，小图标显示，列表显示，详细报表显示 
LVS_EDITLABELS 结点的显示字符可以被**编辑**，对于报表风格来讲可**编辑**的只为第一列。 
LVS_SHOWSELALWAYS 在失去焦点时也显示当前选中的结点 
LVS_SINGLESEL 同时只能选中列表中一项 
首先你需要设置列表控件所使用的ImageList，如果你使用大图标显示风格，你就需要以如下形式调用： 
CImageList* SetImageList( CImageList* pImageList, LVSIL_NORMAL); 
如果使用其它三种风格显示而不想显示图标你可以不进行任何设置，否则需要以如下形式调用： 
CImageList* SetImageList( CImageList* pImageList, LVSIL_SMALL); 

通过调用int InsertItem( int nItem, LPCTSTR lpszItem );可以在列表控件中nItem指明位置插入一项，lpszItem为显示字符。除LVS_REPORT风格外其他三种风格都只需要直接调用InsertItem就可以了，但如果使用报表风格就必须先设置列表控件中的列信息。

通过调用int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat , int nWidth, int nSubItem);可以插入列。iCol为列的位置，从零开始，lpszColumnHeading为显示的列名，nFormat为显示对齐方式，nWidth为显示宽度，nSubItem为分配给该列的列索引。 

在有多列的列表控件中就需要为每一项指明其在每一列中的显示字符，通过调用 
BOOL SetItemText( int nItem, int nSubItem, LPTSTR lpszText );可以设置每列的显示字符。nItem为设置的项的位置，nSubItem为列位置，lpszText为显示字符。下面的代码演示了如何设置多列并插入数据： m_list.SetImageList(&m_listSmall,LVSIL_SMALL);//设置ImageList m_list.InsertColumn(0,"Col 1",LVCFMT_LEFT,300,0);//设置列 m_list.InsertColumn(1,"Col 2",LVCFMT_LEFT,300,1); m_list.InsertColumn(2,"Col 3",LVCFMT_LEFT,300,2); m_list.InsertItem(0,"Item 1_1");//插入行 m_list.SetItemText(0,1,"Item 1_2");//设置该行的不同列的显示字符 m_list.SetItemText(0,2,"Item 1_3"); 

此外CListCtrl还提供了一些函数用于得到/修改控件的状态。 
COLORREF GetTextColor( )/BOOL SetTextColor( COLORREF cr );用于得到/设置显示的字符颜色。 
COLORREF GetTextBkColor( )/BOOL SetTextBkColor( COLORREF cr );用于得到/设置显示的背景颜色。 
void SetItemCount( int iCount );用于得到添加进列表中项的数量。 
BOOL DeleteItem(int nItem);用于删除某一项，BOOL DeleteAllItems( );将删除所有项。 
BOOL SetBkImage(HBITMAP hbm, BOOL fTile , int xOffsetPercent, int yOffsetPercent);用于设置背景位图。 
CString GetItemText( int nItem, int nSubItem );用于得到某项的显示字符。 

列表控件的消息映射同样使用ON_NOTIFY宏，形式如同：ON_NOTIFY( wNotifyCode, id, memberFxn )，wNotifyCode为通知代码，id为产生该消息的窗口ID，memberFxn为处理函数，函数的原型如同void OnXXXList(NMHDR* pNMHDR, LRESULT* pResult)，其中pNMHDR为一数据结构，在具体使用时需要转换成其他类型的结构。对于列表控件可能取值和对应的数据结构为： 

LVN_BEGINLABELEDIT 在开始某项**编辑**字符时发送，所用结构：NMLVDISPINFO 
LVN_ENDLABELEDIT 在结束某项**编辑**字符时发送，所用结构：NMLVDISPINFO 
LVN_GETDISPINFO 在需要得到某项信息时发送，（如得到某项的显示字符）所用结构：NMLVDISPINFO 
关于ON_NOTIFY有很多内容，将在以后的内容中进行详细讲解。 

关于动态提供结点所显示的字符：首先你在项时需要指明lpszItem参数为：LPSTR_TEXTCALLBACK。在控件显示该结点时会通过发送TVN_GETDISPINFO来取得所需要的字符，在处理该消息时先将参数pNMHDR转换为LPNMLVDISPINFO，然后填充其中item.pszText。通过item中的iItem,iSubItem可以知道当前显示的为那一项。下面的代码演示了这种方法： char szOut[8][3]={"No.1","No.2","No.3"}; //添加结点 m_list.InsertItem(LPSTR_TEXTCALLBACK,...) m_list.InsertItem(LPSTR_TEXTCALLBACK,...) //处理消息 void CParentWnd::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) { LV_DISPINFO* pLVDI = (LV_DISPINFO*)pNMHDR; pLVDI->item.pszText=szOut[pTVDI->item.iItem];//通过iItem得到需要 显示的字符在数组中的位置 *pResult = 0; } 

关于**编辑**某项的显示字符：（在报表风格中只对第一列有效）首先需要设置列表控件的LVS_EDITLABELS风格，在开始**编辑**时该控件将会发送LVN_BEGINLABELEDIT，你可以通过在处理函数中返回TRUE来取消接下来的**编辑**，在**编辑**完成后会发送LVN_ENDLABELEDIT，在处理该消息时需要将参数pNMHDR转换为LPNMLVDISPINFO，然后通过其中的item.pszText得到**编辑**后的字符，并重置显示字符。如果**编辑**在中途中取消该变量为NULL。下面的代码说明如何处理这些消息： //处理消息 LVN_BEGINLABELEDIT void CParentWnd::OnBeginEditList(NMHDR* pNMHDR, LRESULT* pResult) { LV_DISPINFO* pLVDI = (LV_DISPINFO*)pNMHDR; if(pLVDI->item.iItem==0);//判断是否取消该操作 *pResult = 1; else *pResult = 0; } //处理消息 LVN_BEGINLABELEDIT void CParentWnd::OnBeginEditList(NMHDR* pNMHDR, LRESULT* pResult) { LV_DISPINFO* pLVDI = (LV_DISPINFO*)pNMHDR; if(pLVDI->item.pszText==NULL);//判断是否已经取消取消**编辑** m_list.SetItemText(pLVDI->item.iItem,0,pLVDI->pszText);//重置显示字符 *pResult = 0; } 上面讲述的方法所进行的消息映射必须在父窗口中进行（同样WM_NOTIFY的所有消息都需要在父窗口中处理）。 

如何得到当前选中项位置：在列表控件中没有一个类似于ListBox中GetCurSel()的函数，但是可以通过调用GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);得到选中项位置。 

4.9 Tab Ctrl

Tab属性页控件可以在一个窗口中添加不同的页面，然后在页选择发生改变时得到通知。MFC中使用CTabCtrl类来封装属性页控件的各种操作。通过调用
BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );创建一个窗口，dwStyle中可以使用以下一些属性页控件的专用风格： 

TCS_BUTTONS 使用按钮来表示页选择位置 
TCS_MULTILINE 分行显示页选择位置 
TCS_SINGLELINE 只使用一行显示页选择位置 
在控件创建后必需向其中添加页面才可以使用，添加页面的函数为： 
BOOL InsertItem( int nItem, LPCTSTR lpszItem );nItem为位置，从零开始，lpszItem为页选择位置上显示的文字。如果你希望在页选择位置处显示一个图标，你可以调用 
BOOL InsertItem( int nItem, LPCTSTR lpszItem, int nImage );nImage指明所使用的图片位置。（在此之前必须调用CImageList * SetImageList( CImageList * pImageList );设置正确的ImageList） 

此外CTabCtrl还提供了一些函数用于得到/修改控件的状态。 
int GetCurSel( )/int SetCurSel( int nItem );用于得到/设置当前被选中的页位置。 
BOOL DeleteItem( int nItem )/BOOL DeleteAllItems( );用于删除指定/所有页面。 
void RemoveImage( int nImage );用于删除某页选择位置上的图标。 

属性页控件的消息映射同样使用ON_NOTIFY宏，形式如同：ON_NOTIFY( wNotifyCode, id, memberFxn )，wNotifyCode为通知代码，id为产生该消息的窗口ID，memberFxn为处理函数，函数的原型如同void OnXXXTab(NMHDR* pNMHDR, LRESULT* pResult)，其中pNMHDR为一数据结构，在具体使用时需要转换成其他类型的结构。对于列表控件可能取值和对应的数据结构为： 

TCN_SELCHANGE 在当前页改变后发送，所用结构：NMHDR 
TCN_SELCHANGING 在当前页改变时发送可以通过返回TRUE来禁止页面的改变，所用结构：NMHDR 

一般来讲在当前页发生改变时需要隐藏当前的一些子窗口，并显示其它的子窗口。下面的伪代码演示了如何使用属性页控件： 

CParentWnd::OnCreate(...)
{
m_tab.Create(...);
m_tab.InsertItem(0,"Option 1");
m_tab.InsertItem(1,"Option 2");
Create a edit box as the m_tab's Child
Create a static box as the m_tab's Child
edit_box.ShowWindow(SW_SHOW); // edit box在属性页的第一页
static_box.ShowWindow(SW_HIDE); // static box在属性页的第二页
}
void CParentWnd::OnSelectChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{//处理页选择改变后的消息
if(m_tab.GetCurSel()==0)
{//根据当前页显示/隐藏不同的子窗口
edit_box.ShowWindow(SW_SHOW);
static_box.ShowWindow(SW_HIDE);
}
else
{//
edit_box.ShowWindow(SW_HIDE);
static_box.ShowWindow(SW_SHOW);
}
}

4.A Tool Bar

工具条也是常用的控件。MFC中使用CToolBar类来封装工具条控件的各种操作。通过调用
BOOL Create( CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR );创建一个窗口，dwStyle中可以使用以下一些工具条控件的专用风格： 

CBRS_TOP 工具条在父窗口的顶部 
TCBRS_BOTTOM 工具条在父窗口的底部 
CBRS_FLOATING 工具条是浮动的 
创建一个工具条的步骤如下：先使用Create创建窗口，然后使用BOOL LoadToolBar( LPCTSTR lpszResourceName );直接从资源中装入工具条，或者通过装入位图并指明每个按钮的ID，具体代码如下： 

UINT uID[5]={IDM_1,IDM_2,IDM_3,IDM_4,IDM_5};
m_toolbar.Create(pParentWnd);
m_toolbar.LoadBitmap(IDB_TOOLBAR);
m_toolbar.SetSizes(CSize(20,20),CSize(16,16));//设置按钮大尺寸
和按钮上位图的尺寸
m_toolbar.SetButtons(uID,5);

AppWizard在生成代码时也会同时生成工具条的代码，同时还可以支持停靠功能。所以一般是不需要直接操作工具条对象。 

工具条上的按钮被按下时发送给父窗口的消息和菜单消息相同，所以可以使用ON_COMMAND宏进行映射，同样工具条中的按钮也支持ON_UPDATE_COMMAND_UI的相关操作，如SetCheck,Enable，你可以将按钮的当作菜单上的一个具有相同ID菜单项。

在以后的章节4.D 利用AppWizard创建并使用ToolBar StatusBar Dialog Bar会给出使用的方法。 

4.B Status Bar

状态条用于显示一些提示字符。MFC中使用CStatusBar类来封装状态条控件的各种操作。通过调用
BOOL Create( CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, UINT nID = AFX_IDW_STATUS_BAR );创建一个窗口，dwStyle中可以使用以下一些状态条控件的专用风格： 

CBRS_TOP 状态条在父窗口的顶部 
TCBRS_BOTTOM 状态条在父窗口的底部 
创建一个状态条的步骤如下：先使用Create创建窗口，然后调用BOOL SetIndicators( const UINT* lpIDArray, int nIDCount );设置状态条上各部分的ID，具体代码如下： 

UINT uID[2]={ID_SEPARATOR,ID_INDICATOR_CAPS};
m_stabar.Create(pParentWnd);
m_stabar.SetIndicators(uID,2);

通过CString GetPaneText( int nIndex )/BOOL SetPaneText( int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE )可以得到/设置状态条上显示的文字。 

Tip：在创建状态条时最好将状态条中所有的部分ID（除MFC自定义的几个用于状态条的ID外）都设置为ID_SEPARATOR，在生成后调用
void SetPaneInfo( int nIndex, UINT nID, UINT nStyle, int cxWidth );改变其风格，ID和宽度。 

AppWizard在生成代码时也会同时生成状态条的代码。所以一般是不需要直接创建状态条对象。此外状态条上会自动显示菜单上的命令提示（必须先在资源中定义），所以也不需要人为设置显示文字。 

状态条支持ON_UPDATE_COMMAND_UI的相关操作，如SetText，Enable。

在以后的章节4.D 利用AppWizard创建并使用ToolBar StatusBar Dialog Bar会给出使用的方法。 

4.C Dialog Bar

Dialog Bar类似一个静态的附在框架窗口上的对话框，由于Dialog Bar可以使用资源**编辑**器进行**编辑**所以使用起来就很方便，在设计时就可以对Dialog Bar上的子窗口进行定位。用于显示一些提示字符。MFC中使用CDialogBar类来Dialog Bar控件的各种操作。通过调用
BOOL Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID );创建一个窗口，nIDTemplate为对话框资源，nID为该Dialog Bar对应的窗口ID，nStyle中可以使用以下一些状态条控件的专用风格： 

CBRS_TOP Dialog Bar在父窗口的顶部 
TCBRS_BOTTOM Dialog Bar在父窗口的底部 
CBRS_LEFT Dialog Bar在父窗口的左部 
CBRS_RIGHT Dialog Bar在父窗口的右部 
对于Dialog Bar的所产生消息需要在父窗口中进行映射和处理，例如Dialog Bar上的按钮，需要在父窗口中进行ON_BN_CLICKED或ON_COMMAND映射，Dialog Bar上的输入框可以在父窗口中进行ON_EN_CHANGE，ON_EN_MAXTEXT等输入框对应的映射。 

Dialog Bar支持ON_UPDATE_COMMAND_UI的相关操作，如SetText，Enable。

在以后的章节4.D 利用AppWizard创建并使用ToolBar StatusBar Dialog Bar会给出使用的方法。 

4.D 利用AppWizard创建并使用ToolBar StatusBar Dialog Bar

运行时程序界面如界面图，该程序拥有一个工具条用于显示两个命令按钮，一个用于演示如何使按钮处于检查状态，另一个根据第一个按钮的状态来禁止/允许自身。（设置检查状态和允许状态都通过OnUpdateCommand实现）此外Dialog Bar上有一个输入框和按钮，这两个子窗口的禁止/允许同样是根据工具条上的按钮状态来确定，当按下Dialog Bar上的按钮时将显示输入框中的文字内容。状态条的第一部分用于显示各种提示，第二部分用于利用OnUpdateCommand显示当前时间。同时在程序中演示了如何设置菜单项的命令解释字符（将在状态条的第一部分显示）和如何设置工具条的提示字符（利用一个小的ToolTip窗口显示）。

生成应用：利用AppWizard生成一个MFC工程，图例，并设置为单文档界面图例，最后选择工具条，状态条和ReBar支持，图例

修改菜单：利用资源**编辑**器删除多余的菜单并添加一个新的弹出菜单和三个子菜单，图例，分别是： 名称 ID 说明字符 
Check IDM_CHECK SetCheck Demo/nSetCheck Demo 
Disable IDM_DISABLE Disable Demo/nDisable Demo 
ShowText on DialogBar IDM_SHOW_TXT ShowText on DialogBar Demo/nShowText on DialogBar 

/n前的字符串将显示在状态条中作为命令解释，/n后的部分将作为具有相同ID的工具条按钮的提示显示在ToolTip窗口中。 

修改Dialog Bar：在Dialog Bar中添加一个输入框和按钮，按钮的ID为IDM_SHOW_TXT与一个菜单项具有相同的ID，这样可以利用映射菜单消息来处理按钮消息（当然使用不同ID值也可以利用ON_COMMAND来映射Dialog Bar上的按钮消息，但是ClassWizard没有提供为Dialog Bar上按钮进行映射的途径，只能手工添加消息映射代码）。图例

修改工具条：在工具条中添加两个按钮，ID值为IDM_CHECK和IDM_DISABLE和其中两个菜单项具有相同的ID值。图例

利用ClassWizard为三个菜单项添加消息映射和更新命令。图例

修改MainFrm.h文件 

//添加一个成员变量来记录工具条上Check按钮的检查状态。
protected:
BOOL m_fCheck;
//手工添加状态条第二部分用于显示时间的更新命令，
和用于禁止/允许输入框的更新命令
//{{AFX_MSG(CMainFrame)
afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
afx_msg void OnCheck();
afx_msg void OnUpdateCheck(CCmdUI* pCmdUI);
afx_msg void OnDisable();
afx_msg void OnUpdateDisable(CCmdUI* pCmdUI);
afx_msg void OnShowTxt();
afx_msg void OnUpdateShowTxt(CCmdUI* pCmdUI);
//}}AFX_MSG
//上面的部分为ClassWizard自动产生的代码
afx_msg void OnUpdateTime(CCmdUI* pCmdUI); //显示时间
afx_msg void OnUpdateInput(CCmdUI* pCmdUI); //禁止/允许输入框

修改MainFrm.cpp文件 

//修改状态条上各部分ID
#define ID_TIME 0x705 //作为状态条上第二部分ID
static UINT indicators[] =
{
ID_SEPARATOR, // status line indicator
ID_SEPARATOR, //先设置为ID_SEPARATOR，
在状态条创建后再进行修改
};
//修改消息映射
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_COMMAND(IDM_CHECK, OnCheck)
ON_UPDATE_COMMAND_UI(IDM_CHECK, OnUpdateCheck)
ON_COMMAND(IDM_DISABLE, OnDisable)
ON_UPDATE_COMMAND_UI(IDM_DISABLE, OnUpdateDisable)
ON_COMMAND(IDM_SHOW_TXT, OnShowTxt)
ON_UPDATE_COMMAND_UI(IDM_SHOW_TXT, OnUpdateShowTxt)
//}}AFX_MSG_MAP
//以上部分为ClassWizard自动生成代码
ON_UPDATE_COMMAND_UI(ID_TIME, OnUpdateTime) ////显示时间
ON_UPDATE_COMMAND_UI(IDC_INPUT_TEST, OnUpdateInput) 
//禁止/允许输入框
//修改OnCreate函数，重新设置状态条第二部分ID值
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
....
// by wenyy 修改状态条上第二部分信息
m_wndStatusBar.SetPaneInfo(1,ID_TIME,SBPS_NORMAL,60);
//set the width
return 0;
}
//修改经过映射的消息处理函数代码
void CMainFrame::OnCheck() 
{
//在Check按钮被按下时改变并保存状态
m_fCheck=!m_fCheck;
}

void CMainFrame::OnUpdateCheck(CCmdUI* pCmdUI) 
{
//Check按钮是否设置为检查状态
pCmdUI->SetCheck(m_fCheck);
}

void CMainFrame::OnDisable() 
{
//Disable按钮被按下
AfxMessageBox("you press disable test");
}

void CMainFrame::OnUpdateDisable(CCmdUI* pCmdUI) 
{
//根据Check状态决定自身禁止/允许状态
pCmdUI->Enable(m_fCheck);
}

void CMainFrame::OnShowTxt() 
{
//得到Dialog Bar上输入框中文字并显示
CEdit* pE=(CEdit*)m_wndDlgBar.GetDlgItem(IDC_INPUT_TEST);
CString szO;
pE->GetWindowText(szO);
AfxMessageBox(szO);
}

void CMainFrame::OnUpdateShowTxt(CCmdUI* pCmdUI) 
{
//Dialog Bar上按钮根据Check状态决定自身禁止/允许状态
pCmdUI->Enable(m_fCheck);
}

void CMainFrame::OnUpdateInput(CCmdUI* pCmdUI) 
{
//Dialog Bar上输入框根据Check状态决定自身禁止/允许状态
pCmdUI->Enable(m_fCheck);
}

void CMainFrame::OnUpdateTime(CCmdUI* pCmdUI) 
{
//根据当前时间设置状态条上第二部分文字
CTime timeCur=CTime::GetCurrentTime();
char szOut[20];
sprintf( szOut, "%02d:%02d:%02d", timeCur.GetHour(), 
timeCur.GetMinute(),timeCur.GetSecond());
pCmdUI->SetText(szOut);
}

4.E General Window

从VC提供的MFC类派生图中我们可以看出窗口的派生关系，派生图，所有的窗口类都是由CWnd派生。所有CWnd的成员函数在其派生类中都可以使用。本节介绍一些常用的功能给大家。 

改变窗口状态：
BOOL EnableWindow( BOOL bEnable = TRUE );可以设置窗口的禁止/允许状态。BOOL IsWindowEnabled( );可以查询窗口的禁止/允许状态。 
BOOL ModifyStyle( DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 )/BOOL ModifyStyleEx( DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 );可以修改窗口的风格，而不需要调用SetWindowLong 
BOOL IsWindowVisible( ) 可以检查窗口是否被显示。 
BOOL ShowWindow( int nCmdShow );将改变窗口的显示状态，nCmdShow可取如下值： 

SW_HIDE 隐藏窗口 
SW_MINIMIZE SW_SHOWMAXIMIZED 最小化窗口 
SW_RESTORE 恢复窗口 
SW_SHOW 显示窗口 
SW_SHOWMINIMIZED 最大化窗口 

改变窗口位置：
void MoveWindow( LPCRECT lpRect, BOOL bRepaint = TRUE );可以移动窗口。
void GetWindowRect( LPRECT lpRect ) ;可以得到窗口的矩形位置。
BOOL IsIconic( ) ;可以检测窗口是否已经缩为图标。
BOOL SetWindowPos( const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags );可以改变窗口的Z次序，此外还可以移动窗口位置。 

使窗口失效，印发重绘：
void Invalidate( BOOL bErase = TRUE );使整个窗口失效，bErase将决定窗口是否产生重绘。
void InvalidateRect( LPCRECT lpRect, BOOL bErase = TRUE )/void InvalidateRgn( CRgn* pRgn, BOOL bErase = TRUE );将使指定的矩形/多边形区域失效。 

窗口查找： 
static CWnd* PASCAL FindWindow( LPCTSTR lpszClassName, LPCTSTR lpszWindowName );可以以窗口的类名和窗口名查找窗口。任一参数设置为NULL表对该参数代表的数据进行任意匹配。如FindWindow("MyWnd",NULL)表明查找类名为MyWnd的所有窗口。 
BOOL IsChild( const CWnd* pWnd ) 检测窗口是否为子窗口。 
CWnd* GetParent( ) 得到父窗口指针。 
CWnd* GetDlgItem( int nID ) 通过子窗口ID得到窗口指针。 
int GetDlgCtrlID( ) 得到窗口ID值。 
static CWnd* PASCAL WindowFromPoint( POINT point );将从屏幕上某点坐标得到包含该点的窗口指针。 
static CWnd* PASCAL FromHandle( HWND hWnd );通过HWND构造一个CWnd*指针，但该指针在空闲时会被删除，所以不能保存供以后使用。 

时钟：
UINT SetTimer( UINT nIDEvent, UINT nElapse, void (CALLBACK EXPORT* lpfnTimer)(HWND, UINT, UINT, DWORD) );可以创建一个时钟，如果lpfnTimer回调函数为NULL，窗口将会收到WM_TIMER消息，并可以在afx_msg void OnTimer( UINT nIDEvent );中安排处理代码 
BOOL KillTimer( int nIDEvent );删除一个指定时钟。 

可以利用重载来添加消息处理的虚函数： 
afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );窗口被创建时被调用 
afx_msg void OnDestroy( );窗口被销毁时被调用 
afx_msg void OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );需要得到窗口尺寸时被调用 
afx_msg void OnSize( UINT nType, int cx, int cy );窗口改变大小后被调用 
afx_msg void OnMove( int x, int y );窗口被移动后时被调用 
afx_msg void OnPaint( );窗口需要重绘时时被调用，你可以填如绘图代码，对于视图类不需要重载OnPaint，所有绘图代码应该在OnDraw中进行 
afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );接收到字符输入时被调用 
afx_msg void OnKeyDown/OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );键盘上键被按下/放开时被调用 
afx_msg void OnLButtonDown/OnRButtonDown( UINT nFlags, CPoint point );鼠标左/右键按下时被调用 
afx_msg void OnLButtonUp/OnRButtonUp( UINT nFlags, CPoint point );鼠标左/右键放开时被调用 
afx_msg void OnLButtonDblClk/OnRButtonDblClk( UINT nFlags, CPoint point );鼠标左/右键双击时被调用 
afx_msg void OnMouseMove( UINT nFlags, CPoint point );鼠标在窗口上移动时被调用 

5.1 使用资源**编辑**器**编辑**对话框

在Windows开发中弹出对话框是一种常用的输入/输出手段，同时**编辑**好的对话框可以保存在资源文件中。Visual C++提供了对话框**编辑**工具，利用**编辑**工具可以方便的添加各种控件到对话框中，而且利用ClassWizard可以方便的生成新的对话框类和映射消息。

首先资源列表中按下右键，可以在弹出菜单中选择“插入对话框”，如图1。然后再打开该对话框进行**编辑**，你会在屏幕上看到一个控件板，如图2。你可以将所需要添加的控件拖到对话框上，或是先选中后再在对话框上用鼠标画出所占的区域。

接下来我们在对话框上产生一个输入框，和一个用于显示图标的图片框。之后我们使用鼠标右键单击产生的控件并选择其属性，如图3。我们可以在属性对话框中**编辑**控件的属性同时也需要指定控件ID，如图4，如果在选择对话框本身的属性那么你可以选择对话框的一些属性，包括字体，外观，是否有系统菜单等等。最后我们**编辑**图片控件的属性，如图5，我们设置控件的属性为显示图标并指明一个图标ID。

接下来我们添加一些其他的控件，最后的效果如图6。按下Ctrl-T可以测试该对话框。此外在对话框中还有一个有用的特性，就是可以利用Tab键让输入焦点在各个控件间移动，要达到这一点首先需要为控件设置在Tab键按下时可以接受焦点移动的属性Tab Stop，如果某一个控件不打算利用这一特性，你需要清除这一属性。然后从菜单“Layout”选择Tab Order来确定焦点移动顺序，如图7。使用鼠标依此点击控件就可以重新规定焦点移动次序。最后按下Ctrl-T进行测试。

最后我们需要为对话框产生新的类，ClassWizard可以替我们完成大部分的工作，我们只需要填写几个参数就可以了。在**编辑**好的对话框上双击，然后系统回询问是否添加新的对话框，选择是并在接下来的对话框中输入类名就可以了。ClassWizard会为你产生所需要的头文件和CPP文件。然后在需要使用的地方包含相应的头文件，对于有模式对话框使用DoModal()产生，对于无模式对话框使用Create()产生。相关代码如下； 

void CMy51_s1View::OnCreateDlg() 
{//产生无模式对话框
CTestDlg *dlg=new CTestDlg;
dlg->Create(IDD_TEST_DLG);
dlg->ShowWindow(SW_SHOW);
}

void CMy51_s1View::OnDoModal() 
{//产生有模式对话框
CTestDlg dlg;
int iRet=dlg.DoModal();
TRACE("dlg return %d/n",iRet);
}

下载例子。如果你在调试这个程序时你会发现程序在退出后会有内存泄漏，这是因为我没有释放无模式对话框所使用的内存，这一问题会在以后的章节5.3 创建无模式对话框中专门讲述。 

关于在使用对话框时Enter键和Escape键的处理：在使用对话框是你会发现当你按下Enter键或Escape键都会退出对话框，这是因为Enter键会引起CDialog::OnOK()的调用，而Escape键会引起CDialog::OnCancel()的调用。而这两个调用都会引起对话框的退出。在MFC中这两个成员函数都是虚拟函数，所以我们需要进行重载，如果我们不希望退出对话框那么我们可以在函数中什么都不做，如果需要进行检查则可以添加检查代码，然后调用父类的OnOK()或OnCancel()。相关代码如下； 

void CTestDlg::OnOK()
{
AfxMessageBox("你选择确定");
CDialog::OnOK();
}

void CTestDlg::OnCancel()
{
AfxMessageBox("你选择取消");
CDialog::OnCancel();
}

5.2 创建有模式对话框

使用有模式对话框时在对话框弹出后调用函数不会立即返回，而是等到对话框销毁后才会返回（请注意在对话框弹出后其他窗口的消息依然会被传递）。所以在使用对话框时其他窗口都不能接收用户输入。创建有模式对话框的方法是调用CDialog::DoModal()。下面的代码演示了这种用法： 

CYourView::OnOpenDlg()
{
CYourDlg dlg;
int iRet=dlg.DoModal();
}

CDialog::DoModal()的返回值为IDOK，IDCANCEL。表明操作者在对话框上选择“确认”或是“取消”。由于在对话框销毁前DoModal不会返回，所以可以使用局部变量来引用对象。在退出函数体后对象同时也会被销毁。而对于无模式对话框则不能这样使用，下节5.3 创建无模式对话框中会详细讲解。 

你需要根据DoModal()的返回值来决定你下一步的动作，而得到返回值也是使用有模式对话框的一个很大原因。

使用有模式对话框需要注意一些问题，比如说不要在一些反复出现的事件处理过程中生成有模式对话框，比如说在定时器中产生有模式对话框，因为在上一个对话框还未退出时，定时器消息又会引起下一个对话框的弹出。

同样的在你的对话框类中为了向调用者返回不同的值可以调用CDialog::OnOK()或是CDialog::OnCancel()以返回IDOK或IDCANCEL，如果你希望返回其他的值，你需要调用 
CDialog::EndDialog( int nResult );其中nResult会作为DoModal()调用的返回值。

下面的代码演示了如何使用自己的函数来退出对话框： 

void CMy52_s1View::OnLButtonDown(UINT nFlags, CPoint point) 
{//创建对话框并得到返回值
CView::OnLButtonDown(nFlags, point);
CTestDlg dlg;
int iRet=dlg.DoModal();
CString szOut;
szOut.Format("return value %d",iRet);
AfxMessageBox(szOut);
}
//重载OnOK,OnCancel
void CTestDlg::OnOK()
{//什么也不做
}
void CTestDlg::OnCancel()
{//什么也不做
}
//在对话框中对三个按钮消息进行映射
void CTestDlg::OnExit1() 
{
CDialog::OnOK();
}
void CTestDlg::OnExit2() 
{
CDialog::OnCancel();
}
void CTestDlg::OnExit3() 
{
CDialog::EndDialog(0XFF);
}

由于重载了OnOK和OnCancel所以在对话框中按下Enter键或Escape键时都不会退出，只有按下三个按钮中的其中一个才会返回。 

此外在对话框被生成是会自动调用BOOL CDialog::OnInitDialog()，你如果需要在对话框显示前对其中的控件进行初始化，你需要重载这个函数，并在其中填入相关的初始化代码。利用ClassWizard可以方便的产生一些默认代码，首先打开ClassWizard，选择相应的对话框类，在右边的消息列表中选择WM_INITDIALOG并双击，如图，ClassWizard会自动产生相关代码，代码如下： 

BOOL CTestDlg::OnInitDialog() 
{
/*先调用父类的同名函数*/
CDialog::OnInitDialog();
/*填写你的初始化代码*/ 
return TRUE; 
}

VC小知识总结:

（1） **如何通过代码获得应用程序主窗口的 指针?**
主窗口的 指针保存在CWinThread::m_pMainWnd中,调用AfxGetMainWnd实现。
AfxGetMainWnd() ->ShowWindow(SW_SHOWMAXMIZED)
//使程序最大化.

（2） **确定应用程序的路径**
Use GetModuleFileName 获得应用程序的路径，然后去掉可执行文件名。
Example:
TCHAR
exeFullPath[MAX_PATH] // MAX_PATH在API中定义了吧，好象是
128
GetModuleFileName(NULL,exeFullPath,MAX_PATH)

（3） **如何在程序中获得其他程序的 图标?**
两种方法:
(1) SDK函数 SHGetFileInfo 或使用 ExtractIcon获得图标资源的 handle,
(2) SDK函数 SHGetFileInfo 获得有关文件的很多信息,如大小图标,属性, 类型等.
Example(1):
在程序窗口左上角显示 NotePad图标.
void CSampleView:
OnDraw(CDC * pDC)
{
if( :: SHGetFileInfo(_T("c://pwin95//notepad.exe"),0,
&stFileInfo,sizeof(stFileInfo),SHGFI_ICON))
{
pDC ->DrawIcon(10,10,stFileInfo.hIcon)
}
}
Example(2):同样功能,Use ExtractIcon Function
void CSampleView:: OnDraw(CDC *pDC)
{
HICON hIcon=:: ExtractIcon(AfxGetInstanceHandle(),_T
("NotePad.exe"),0)
if (hIcon &&hIcon!=(HICON)-1)
pDC->DrawIcon(10,10,hIcon)
}
    说明: 获得notepad.exe的路径正规上来说用GetWindowsDirectory函数得到, 如果是调用 win95下的画笔，应该用访问注册表的方法获得其路径，要作成一个比较考究的程序，考虑应该全面点.

（4） **获得各种目录信息**
Windows目录: Use "GetWindowsDirectory"
Windows下的system目录: Use "GetSystemDirectory"
temp目录: Use "GetTempPath"
当前目录: Use "GetCurrentDirectory"

请注意前两个函数的第一个参数为目录变量名，后一个为缓冲区后两个相反.

（5） **如何自定义消息**
1) 手工定义消息，可以这么写
#define WM_MY_MESSAGE(WM_USER+100),
MS 推荐的至少是 WM_USER+100

(2)写消息处理函数,用
WPARAM,LPARAM返回LRESULT.
LRESULT CMainFrame::OnMyMessage(WPARAM wparam,LPARAM lParam)

{
temp目录: Use "GetTempPath"
//加入你的处理函数 irectory"
}

（6） **如何改变窗口的图标?**
向窗口发送 WM_SECTION消息。
Example:
HICON hIcon=AfxGetApp() ->LoadIcon(IDI_ICON)
ASSERT(hIcon)
AfxGetMainWnd() ->SendMessage(WM_SECTION,TRUE,(LPARAM)hIcon)

（7） **如何改变窗口的缺省风格?**
重载 CWnd:: PreCreateWindow 并修改CREATESTRUCT结构来指定窗口风格和其他创建信息.
Example: Delete "Max" Button and Set Original
Window's Position and Size

BOOL CMainFrame:: PreCreateWindow
(CREATESTRUCT &cs)
{
cs.style &=~WS_MAXINIZEMOX

cs.x=cs.y=0
cs.cx=GetSystemMetrics(SM_CXSCREEN/2)
cs.cy=GetSystemMetrics(SM_CYSCREEN/2)

return CMDIFramewnd ::PreCreateWindow(cs)
}

（8） **如何将窗口居中显示?**
Call Function CWnd::
Center Windows

Example(1):
Center Window( ) //Relative to it's parent
// Relative
to Screen
Example(2):
Center Window(CWnd:: GetDesktopWindow( ))
//Relative to
Application's MainWindow
AfxGetMainWnd( ) ->
Center Window( )

（9） **如何让窗口和 MDI窗口一启动就最大化和最小化?**
先说窗口。
在 InitStance 函数中设定 m_nCmdShow的取值.
m_nCmdShow=SW_SHOWMAXMIZED //最大化
m_nCmdShow=SW_SHOWMINMIZED //最小化
m_nCmdShow=SW_SHOWNORMAL //正常方式

MDI窗口:
如果是创建新的应用程序,可以用MFC AppWizard 的Advanced 按钮并在MDI子窗口风格组中检测最大化或最小化还可以重载 MDI Window 的PreCreateWindow函数，设置WS_MAXMIZE or WS_MINMIZE

如果从 CMDIChildWnd派生,调用 OnInitialUpdate函数中的 CWnd::Show Window来指定 MDI Child Window的风格。

（10） **如何限制窗口的大小?**
也就是 FixedDialog形式。 Windows发送 WM_GETMAXMININFO消息来跟踪, 响应它,在 OnGetMAXMININFO 中写代码:

（11） **如何使窗口不可见？**
很简单,用SW_HIDE 隐藏窗口，可以结合 FindWindow,ShowWindow控制.

（12） **如何创建一个字回绕的CEditView**
重载CWnd : : PreCreateWindow和修改CREATESTRUCT结构，关闭CEditView对象的ES_AUTOHSCROLL和WS_HSCROLL风格位， 由于CEditView : : PreCreateWindow显示设置cs. style，调用基类函数后要修改cs . style。

BOOL CSampleEDitView : : PreCreateWindow (CREATESTRUCT&cs)
{
//First call basse class function .
BOOL bResutl =CEditView : : PreCreateWindow (cs)

// Now specify the new window style .
cs.style &= ~ (ES_AUTOHSCROLL ｜WS_HSCROLL)
return bResult
}

（13） **如何使程序保持极小状态?**
这么办: 在恢复程序窗体大小时，Windows会发送WM_QUERY-OPEN消息，用 ClassWizard设置成员函数
OnQueryOpen() ,add following code:

Bool CMainFrame:: OnQueryOpen( )
{
Return false
}

（14） **移动窗口**
调用CWnd : : SetWindowPos并指定SWP_NOSIZE标志。目的位置与父窗口有关（顶层窗口与屏幕有关）。调用CWnd : : MoveWindow时必须要指定窗口的大小。
//Move window to positoin 100 , 100 of its parent window .
SetWindowPos (NULL, 100 , 100 , 0 , 0 , SWP_NOSIZE ｜SWP_NOAORDER)

（15） **通用控件的显示窗口**
MFC提供了几个CView派生的视窗类， 封装了通用控件的功能，但仍然使用工作框文档显示窗口体系结构：CEditView封装了编辑控件，CTreeView保持了树列表控件，CListView封装了列表显示窗口控件，CRichEditView可以处理多种编辑控件。

（16） **重置窗口的大小**
调用CWnd: : SetWindowPos并指定SWP_NOMOVE标志， 也可调用CWnd : : MoveWindow 但必须指定窗口的位置。
// Get the size of the window .
Crect reWindow
GetWindowRect (reWindow )

//Make the window twice as wide and twice as tall .
SetWindowPos (NULL , 0 , 0 , reWindow . Width ( ) *2,

reWindow . Height () * 2,
SWP_NOMOVE ｜SWP_NOZORDER )

（17） **如何单击除了窗口标题栏以外的区域使窗口移动**
当窗口需要确定鼠标位置时Windows向窗口发送WM_NCHITTEST信息，可以处理该信息使Windows认为鼠标在窗口标题上。对于对话框和基于对话的应用程序，可以使用ClassWizard处理该信息并调用基类函数， 如果函数返回HTCLIENT 则表明鼠标在客房区域，返回HTCAPTION表明鼠标在Windows的标题栏中。
UINT CSampleDialog : : OnNcHitTest (Cpoint point )
{
UINT nHitTest =Cdialog: : OnNcHitTest (point )
return (nHitTest = =HTCLIENT)? HTCAPTION : nHitTest
}

上述技术有两点不利之处，
其一是在窗口的客户区域双击时，窗口将极大；
其二， 它不适合包含几个视窗的主框窗口。
还有一种方法，当用户按下鼠标左键使主框窗口认为鼠标在其窗口标题上，使用ClassWizard在视窗中处理WM_LBUTTODOWN信息并向主框窗口发送一个WM_NCLBUTTONDOWN信息和一个单击测试HTCAPTION。
void CSampleView : : OnLButtonDown (UINT nFlags , Cpoint point
)
{
CView : : OnLButtonDow (nFlags , pont )

//Fool frame window into thinking somene clicked
on
its caption bar .
GetParentFrame ( ) —> PostMessage (
WM_NCLBUTTONDOWN ,
HTCAPTION , MAKELPARAM (poitn .x , point .y) )

}
该技术也适用于对话框和基于对的应用程序，只是不必调用
CWnd: :GetParentFrame 。
void CSampleDialog : : OnLbuttonDown (UINT nFlags, Cpoint point )
{
Cdialog : : OnLButtonDow (nFlags, goint )
//Fool dialog into thinking simeone clicked on its
caption bar .
PostMessage (WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARM (point.x
, point. y
) )
}

（18） **如何改变视窗的背景颜色**
Windows向窗口发送一个WM_ERASEBKGND消息通知该窗口擦除背景，可以使用ClassWizard重载该消息的缺省处理程序来擦除背景（实际是画），并返回TRUE以防止Windows擦除窗口。
//Paint area that needs to be erased.
BOOL CSampleView : : OnEraseBkgnd (CDC* pDC)
{
// Create a pruple brush.
CBrush Brush (RGB (128 , 0 , 128) )

// Select the brush into the device context .
CBrush* pOldBrush = pDC—>SelcetObject (&brush)

// Get the area that needs to be erased .
CRect reClip
pDC—>GetCilpBox (&rcClip)
//Paint the area.
pDC—> PatBlt (rcClip.left , rcClip.top , rcClip.Width ( ) , rcClip.Height( ) , PATCOPY )

//Unselect brush out of device context .
pDC—>SelectObject (pOldBrush )

// Return nonzero to half fruther processing .
return TRUE
}

（19） **如何改变窗口标题**
调用CWnd : : SetWindowText可以改变任何窗口（包括控件）的标题。
//Set title for application's main frame window .
AfxGetMainWnd ( ) —> SetWindowText (_T("Application title") )

//Set title for View's MDI child frame window .
GetParentFrame ( ) —> SetWindowText ("_T ("MDI Child Frame new title")
)

//Set title for dialog's push button control.
GetDigitem (IDC_BUTTON) —> SetWindowText (_T ("Button new title ") )
如果需要经常修改窗口的标题（注：控件也是窗口），应该考虑使用半文档化的函数AfxSetWindowText。该函数在AFXPRIV.H中说明，在WINUTIL.CPP中实现，在联机帮助中找不到它，它在AFXPRIV.H中半文档化， 在以后发行的MFC中将文档化。
AfxSetWindowText的实现如下：
voik AFXAPI AfxSetWindowText (HWND hWndCtrl , LPCTSTR IpszNew )
{
itn nNewLen= Istrlen (Ipaznew)
TCHAR szOld [256]
//fast check to see if text really changes (reduces
flash in the
controls )
if (nNewLen >_contof (szOld)
｜｜ : : GetWindowText (hWndCrtl, szOld , _countof (szOld) !=nNewLen
｜｜ Istrcmp (szOld , IpszNew)! = 0
{
//change it
: : SetWindowText(hWndCtrl , IpszNew )
}
}

（20） **如何防止主框窗口在其说明中显示活动的文档名**
创建主框窗口和MDI子窗口进通常具有FWS_ADDTOTITLE风格位，如果不希望在说明中自动添加文档名， 必须禁止该风格位， 可以使用ClassWizard重置
CWnd: : PreCreateWindow并关闭FWS_ADDTOTITLE风格。
BOOL CMainFrame : : PreCreateWindow (CREATESTRUCT&cs)
{
//Turn off FWS_ADDTOTITLE in main frame .
cs.styel & = ~FWS_ADDTOTITLE 　
return CMDIFrameWnd : : PreCreateWindow (cs )
}
关闭MDI子窗口的FWS _ADDTOTITLE风格将创建一个具有空标题的窗口，可以调用CWnd: : SetWindowText来设置标题。记住自己设置标题时要遵循接口风格指南。

（21） **如何获取有关窗口正在处理的当前消息的信息**
调用CWnd: : GetCurrentMessage可以获取一个MSG指针。例如，可以使用ClassWizard将几个菜单项处理程序映射到一个函数中，然后调用GetCurrentMessage来确定所选中的菜单项。
viod CMainFrame : : OnCommmonMenuHandler ( )
{
//Display selected menu item in debug window .
TRACE ("Menu item %u was selected . /n" ,

（22） **如何在代码中获取工具条和状态条的指针**
缺省时， 工作框创建状态条和工具条时将它们作为主框窗口的子窗口，状态条有一个AFX_IDW_STATUS_BAR标识符，工具条有一个AFX_IDW_TOOLBAR标识符，下例说明了如何通过一起调用CWnd: : GetDescendantWindow和AfxGetMainWnd来获取这些子窗口的指针：
//Get pointer to status bar .
CStatusBar * pStatusBar = (CStatusBar *) AfxGetMainWnd ( )
—> GetDescendantWindow(AFX_IDW_STUTUS_BAR)

//Get pointer to toolbar .
CToolBar * pToolBar = (CToolBar * ) AfxGetMainWnd ( )
—> GetDescendantWindow(AFX_IDW_TOOLBAR)

（23） **如何使能和禁止工具条的工具提示**
如果设置了CBRS_TOOLTIPS风格位，工具条将显示工具提示，要使能或者禁止工具提示，需要设置或者清除该风格位。下例通过调用CControlBar : : GetBarStyle和CControlBar : : SetBarStyle建立一个完成此功能的成员函数：
void CMainFrame : : EnableToolTips ( BOOL bDisplayTips )
{
ASSERT_VALID (m_wndToolBar)

DWORD dwStyle = m _wndToolBar.GetBarStyle ( )

if (bDisplayTips) dwStyle ｜=CBRS_TOOLTIPS

else
dwStyle & = ~CBRS_TOOLTIPS

m_wndToolBar.SetBarStyle (dwStyle )
}

（24） **如何创建一个不规则形状的窗口**
可以使用新的SDK函数SetWindowRgn。该函数将绘画和鼠标消息限定在窗口的一个指定的区域，实际上使窗口成为指定的不规则形状。 使用AppWizard创建一个基于对的应用程序并使用资源编辑器从主对话资源中删除所在的缺省控件、标题以及边界。
给对话类增加一个CRgn数据成员，以后要使用该数据成员建立窗口区域。
Class CRoundDlg : public CDialog
{
…
private :
Crgn m_rgn : // window region
…
}
修改OnInitDialog函数建立一个椭圆区域并调用SetWindowRgn将该区域分配给窗口：
BOOL CRoundDlg : : OnInitDialog ( )
{
CDialog : : OnInitDialog ( )

//Get size of dialog .
CRect rcDialog
GetClientRect (rcDialog )

// Create region and assign to window .
m_rgn . CreateEllipticRgn (0 , 0 , rcDialog.Width( ) , rcDialog.Height ( ) )
SetWindowRgn (GetSafeHwnd ( ) , (HRGN) m_ rgn ,TRUE )

return TRUE
}

通过建立区域和调用SetWindowRgn，已经建立一个不规则形状的窗口，下面的例子程序是修改OnPaint函数使窗口形状看起来象一个球形体。
voik CRoundDlg : : OnPaint ( )
{
CPaintDC de (this) // device context for painting
.
//draw ellipse with out any border
dc. SelecStockObject (NULL_PEN)
//get the RGB colour components of the sphere color
COLORREF color= RGB( 0 , 0 , 255)
BYTE byRed =GetRValue (color)
BYTE byGreen = GetGValue (color)
BYTE byBlue = GetBValue (color)

// get the size of the view window
Crect rect
GetClientRect (rect)

// get minimun number of units
int nUnits =min (rect.right , rect.bottom )

//calculate he horiaontal and vertical step size
float fltStepHorz = (float) rect.right /nUnits
float fltStepVert = (float) rect.bottom /nUnits

int nEllipse = nUnits/3 // calculate how many to
draw
int nIndex
// current ellipse that is being draw

CBrush brush
// bursh used for ellipse fill color
CBrush *pBrushOld // previous
brush that was selected into dc
//draw ellipse , gradually moving towards upper-right
corner
for (nIndex = 0 nIndes < + nEllipse nIndes++)
{
//creat solid brush
brush . CreatSolidBrush (RGB ( ( (nIndex*byRed ) /nEllipse ).
( ( nIndex * byGreen ) /nEllipse ), ( (nIndex * byBlue)
/nEllipse ) ) )

//select brush into dc
pBrushOld= dc .SelectObject (&brhsh)

//draw ellipse
dc .Ellipse ( (int) fltStepHorz * 2, (int) fltStepVert * nIndex ,
rect. right -( (int) fltStepHorz * nIndex )+ 1,
rect . bottom -( (int) fltStepVert * (nIndex *2) ) +1)

//delete the brush
brush.DelecteObject ( )
}
}

最后，处理WM_NCHITTEST消息，使当击打窗口的任何位置时能移动窗口。
UINT CRoundDlg : : OnNchitTest (Cpoint point )
{
//Let user move window by clickign anywhere on thewindow .
UINT nHitTest = CDialog : : OnNcHitTest (point)
rerurn (nHitTest = = HTCLIENT)? HTCAPTION: nHitTest

}

（25） **如何获取应用程序的 实例句柄?**
应用程序的实例句柄保存在CWinApp m_hInstance 中,可以这么调用AfxGetInstancdHandle获得句柄.
Example: HANDLE hInstance=AfxGetInstanceHandle()

（26） **如何编程结束应用程序?**
这是个很简单又是编程中经常要遇到的问题.
向窗口发送 WM_CLOSE消息,调用 CWnd::OnClose成员函数.允许对用户提示是否保存修改过的数据.
Example: AfxGetMainWindow()->SendMessage(WM_CLOSE)

还可以创建一个自定义的函数 Terminate Window
void Terminate Window(LPCSTR pCaption)
{
CWnd *pWnd=Cwnd::FindWindow(NULL,pCaption)

if (pWnd)

pWnd ->SendMessage(WM_CLOSE)
}

    说明: FindWindow函数不是提倡的做法，因为它无法处理标题栏自动改变，比如我们要检测 Notepad是不是已运行而事先不知道Notepad的标题栏,这时FindWindow就无能为力了，可以通过枚举 windows任务列表的办法来实现。在机械出版社"Windows 95 API开发人员指南"一书有比较详细的介绍,这里就不再多说乐。

（27） **如何创建和使用无模式对话框**
MFC将模式和无模式对话封装在同一个类中，但是使用无模式对话需要几个对话需要几个额处的步骤。首先，使用资源编辑器建立对话资源并使用ClassWizard创建一个CDialog的派生类。模式和无模式对话的中止是不一样的：模式对话通过调用CDialog : : EndDialog 来中止，无模式对话则是调用CWnd: : DestroyWindow来中止的，函数CDialog : : OnOK和CDialog : : OnCancel调用EndDialog ,所以需要调用DestroyWindow并重置无模式对话的函数。
void CSampleDialog : : OnOK ( )
{
// Retrieve and validate dialog data .
if (! UpdateData (TRUE) )
{
// the UpdateData rountine
will set focus to correct item TRACEO (" UpdateData failed during dialog termination ./n")
return
}

//Call DestroyWindow instead of EndDialog .
DestroyWindow ( )
}

void CSampleDialog : : OnCancel ( )
{
//Call DestroyWindow instead of EndDialog .
DestroyWindow ( )
}

其次，需要正确删除表示对话的C++对象。对于模式对来说，这很容易，需要创建函数返回后即可删除C++对象；无模式对话不是同步的，创建函数调用后立即返回，因而用户不知道何时删除C++对象。撤销窗口时工作框调用CWnd : : PostNcDestroy，可以重置该函数并执行清除操作，诸如删除this指针。
void CSampleDialog : : PostNcDestroy ( )
{
// Declete the C++ object that represents this dialog.
delete this

最后，要创建无模式对话。可以调用CDialog : : DoModal创建一个模式对放，要创建一个无模式对话则要调用CDialog: : Create。下面的例子说明 了应用程序是如何创建无模式对话的： 象；无模式对话不是同步的，创建函数调用后立即返回，
void CMainFrame : : OnSampleDialog ( )
{
//Allocate a modeless dialog object .
CSampleDilog * pDialog =new CSampleDialog
ASSERT_VALID (pDialog) Destroy ( )

//Create the modeless dialog . represents this dialog.
BOOL bResult = pDialog —> Creste (IDD_IDALOG)
ASSERT (bResult )
}

（28） **如何防止主框窗口在其说明中显示活动的文档名**
创建主框窗口和MDI子窗口进通常具有FWS_ADDTOTITLE风格位，如果不希望在说明中自动添加文档名， 必须禁止该风格位， 可以使用ClassWizard重置
CWnd: : PreCreateWindow并关闭FWS_ADDTOTITLE风格。
BOOL CMainFrame : : PreCreateWindow (CREATESTRUCT&cs)
{
//Turn off FWS_ADDTOTITLE in main frame .
cs.styel & = ~FWS_ADDTOTITLE 　
return CMDIFrameWnd : : PreCreateWindow (cs )
}
关闭MDI子窗口的FWS _ADDTOTITLE风格将创建一个具有空标题的窗口，可以调用CWnd: : SetWindowText来设置标题。记住自己设置标题时要遵循接口风格指南。

（29） **如何在代码中获取工具条和状态条的指针**
缺省时， 工作框创建状态条和工具条时将它们作为主框窗口的子窗口，状态条有一个AFX_IDW_STATUS_BAR标识符，工具条有一个AFX_IDW_TOOLBAR标识符，下例说明了如何通过一起调用CWnd: : GetDescendantWindow和AfxGetMainWnd来获取这些子窗口的指针：
//Get pointer to status bar .
CStatusBar * pStatusBar = (CStatusBar *) AfxGetMainWnd ( )
—> GetDescendantWindow(AFX_IDW_STUTUS_BAR)

//Get pointer to toolbar .
CToolBar * pToolBar = (CToolBar * ) AfxGetMainWnd ( )
—> GetDescendantWindow(AFX_IDW_TOOLBAR)

（30） **怎样加载其他的应用程序?**
三个SDK函数 winexec, shellexecute,createprocess可以使用。
WinExec最简单，两个参数，前一个指定路径，后一个指定显示方式.后一个参数值得说一下，比如泥用 SW_SHOWMAXMIZED方式去加载一个无最大化按钮的程序，就是Neterm,calc等等，就不会出现正常的窗体，但是已经被加到任务列表里了。

ShellExecute较 WinExex灵活一点，可以指定工作目录,下面的Example就是直接打开 c:/temp/1.txt,而不用加载与 txt文件关联的应用程序,很多安装程序完成后都会打开一个窗口，来显示Readme or Faq,我猜就是这么作的啦.

ShellExecute(NULL,NULL,_T("1.txt"),NULL,_T("c://temp"),SW_SHOWMAXMIZED)

CreateProcess最复杂，一共有十个参数，不过大部分都可以用NULL代替，它可以指定进程的安全属性，继承信息，类的优先级等等.来看个很简单的Example:
STARTUPINFO stinfo
//启动窗口的信息
PROCESSINFO procinfo //进程的信息

CreateProcess(NULL,_T("notepad.exe"),NULL,NULL.FALSE,
NORMAL_PRIORITY_

CLASS,NULL,NULL, &stinfo,&procinfo)

（31） **如何在代码中获取工具条和状态条的指针**
缺省时， 工作框创建状态条和工具条时将它们作为主框窗口的子窗口，状态条有一个AFX_IDW_STATUS_BAR标识符，工具条有一个AFX_IDW_TOOLBAR标识符，下例说明了如何通过一起调用CWnd: : GetDescendantWindow和AfxGetMainWnd来获取这些子窗口的指针：
//Get pointer to status bar .
CStatusBar * pStatusBar = (CStatusBar *) AfxGetMainWnd ( )
—> GetDescendantWindow(AFX_IDW_STUTUS_BAR)

（32） **如何使能和禁止工具条的工具提示**
如果设置了CBRS_TOOLTIPS风格位，工具条将显示工具提示，要使能或者禁止工具提示，需要设置或者清除该风格位。下例通过调用CControlBar : : GetBarStyle和CControlBar : : SetBarStyle建立一个完成此功能的成员函数：
void CMainFrame : : EnableToolTips ( BOOL bDisplayTips )
{
ASSERT_VALID (m_wndToolBar)

DWORD dwStyle = m _wndToolBar.GetBarStyle ( )

if (bDisplayTips) dwStyle ｜=CBRS_TOOLTIPS

else
dwStyle & = ~CBRS_TOOLTIPS

m_wndToolBar.SetBarStyle (dwStyle )
}

//Get pointer to toolbar .
CToolBar * pToolBar = (CToolBar * ) AfxGetMainWnd ( )
—> GetDescendantWindow(AFX_IDW_TOOLBAR)

（33） **如何设置工具条标题**
工具条是一个窗口，所以可以在调用CWnd : : SetWindowText来设置标题，例子如下：
int CMainFrame : : OnCreate (LPCREATESTRUCT lpCreateStruct )
{
…
// Set the caption of the toolbar .
m_wndToolBar.SetWindowText (_T "Standdard")

（34） **如何使窗口始终在最前方?**
BringWindowToTop(Handle)
SetWindowPos函数，指定窗口的 最顶风格,用WS_EX_TOPMOST扩展窗口的风格

Example:
void ToggleTopMost(
CWnd *pWnd)
{
ASSERT_VALID(pWnd)

pWnd ->SetWindowPos(pWnd-> GetStyle( ) &WS_EX_TOPMOST)?

&wndNoTopMOST: &wndTopMost,0,0,0,0,SSP_NOSIZE|WSP_NOMOVE)
}

（35） **如何在对话框中显示一个位图**
这要归功于Win 32先进的静态控件和Microsoft的资源编辑器，在对话框中显示位图是很容易的， 只需将图形控件拖到对话中并选择适当属性即可，用户也可以显示图标、位图以及增强型元文件。

（36） **如何改变对话或窗体视窗的背景颜色**
调用CWinApp : : SetDialogBkColor可以改变所有应用程序的背景颜色。第一个参数指定了背景颜色，第二个参数指定了文本颜色。下例将应用程序对话设置为蓝色背景和黄色文本。
BOOL CSampleApp : : InitInstance ( )
{
…

//use blue dialog with yellow text .
SetDialogBkColor (RGB (0, 0, 255 ), RGB ( 255 ,255 , 0 ) )

…
}

需要重画对话（或对话的子控件）时，Windows向对话发送消息WM_CTLCOLOR，通常用户可以让Windows选择绘画背景的刷子，也可重置该消息指定刷子。下例说明了创建一个红色背景对话的步骤。

首先，给对话基类增加一人成员变量
CBursh :class CMyFormView : public CFormView
{
…

private :
CBrush m_ brush // background brush

…
}

其次， 在类的构造函数中将刷子初始化为所需要的背景颜色。
CMyFormView : : CMyFormView ( )
{
// Initialize background brush .
m_brush .CreateSolidBrush (RGB ( 0, 0, 255) )
}

最后，使用ClassWizard处理WM_CTLCOLOR消息并返回一个用来绘画对话背景的刷子句柄。注意：由于当重画对话控件时也要调用该函数，所以要检测nCtlColor参量。
HBRUSH CMyFormView : : OnCtlColor (CDC* pDC , CWnd*pWnd , UINT nCtlColor
)

{
// Determine if drawing a dialog box . If we are, return +handle to
//our own background brush . Otherwise let windows handle it .
if (nCtlColor = = CTLCOLOR _ DLG )
return (HBRUSH) m_brush.GetSafeHandle ( )
return CFormView : : OnCtlColor (pDC, pWnd , nCtlColor
)
}

（37） **如何获取一个对话控件的指针**
有两种方法。其一，调用CWnd: : GetDlgItem，获取一个CWnd*指针调用成员函数。下例调用GetDlgItem，将返回值传给一个CSpinButtonCtrl*以便调用CSpinButtonCtrl : : SetPos 函数：
BOOL CSampleDialog : : OnInitDialog ( )
{
CDialog : : OnInitDialog ( )

//Get pointer to spin button .
CSpinButtonCtrl * pSpin - ( CSpinButtonCtrl *) GetDlgItem(IDC_SPIN)
ASSERT _ VALID (pSpin)
//Set spin button's default position .
pSpin —> SetPos (10)

return TRUE
}

其二， 可以使用ClassWizard将控件和成员变量联系起来。在ClassWizard中简单地选择Member Variables标签，然后选择Add Variable …按钮。如果在对话资源编辑器中，按下Ctrl键并双击控件即可转到Add Member Variable对话。

（38） **如何禁止和使能控件**
控件也是窗口，所以可以调用CWnd : : EnableWindow使能和禁止控件。
//Disable button controls .
m_wndOK.EnableWindow (FALSE )
m_wndApply.EnableWindow (FALSE )

（39） **如何改变控件的字体**
由于控件是也是窗口，用户可以调用CWnd: : SetFont指定新字体。该函数用一个Cfont指针，要保证在控件撤消之前不能撤消字体对象。下例将下压按钮的字体改为8点Arial字体：
//Declare font object in class declaration (.H file ).
private : Cfont m_font
// Set font in class implementation (.Cpp file ). Note m_wndButton is a
//member variable added by ClassWizard.DDX routines hook the member
//variable to a dialog button contrlo.
BOOL CSampleDialog : : OnInitDialog ( )
{
…
//Create an 8-point Arial font
m_font . CreateFont (MulDiv (8 , -pDC
—> GetDeviceCaps(LOGPIXELSY) ,72). 0 , 0 , 0 , FW_NORMAL , 0 , 0,0, ANSI_CHARSER, OUT_STROKE_PRECIS ,

CLIP_STROKE _PRECIS , DRAFT _QUALITY
VARIABLE_PITCH ｜FF_SWISS, _T("Arial") )

//Set font for push button .
m_wndButton . SetFont (&m _font )

…
}

（40） **如何在OLE控件中使用OLE_COLOR数据类型**
诸如COleControl : : GetFortColor和COleControl : : GetBackColor等函数返回OLE _COLOR数据类型的颜色，而GDI对象诸如笔和刷子使用的是COLORREF数据类型，调用COleControl : : TranslateColor可以很容易地将OLE_COLOR类型改为COLORREF类型。下例创建了一个当前背景颜色的刷子：

void CSampleControl : : OnDraw (CDC* pdc
const Crect& rcBounds , const Crect& rcInvalid
)
{
//Create a brush of the cuttent background color.
CBrush brushBack (TranslateColor (GetBackColor () ) )

//Paint the background using the current backgroundcolor .
pdc—> FilllRect (rcBounds , &brushBack)

//other drawign commands

…
}

（41） **在不使用通用文件打开对话的情况下如何显示一个文件列表**
调用CWnd: : DlgDirList或者CWnd: : DlgDirListComboBox，Windows 将自动地向列表框或组合框填充可用的驱动器名或者指定目录中的文件，下例将Windows目录中的文件填充在组合框中：
BOOL CSampleDig : : OnInitDialog ( )
{
CDialog : : OnInitDialog ( )
TCHAR szPath [MAX_PATH] = {"c://windows"}
int nReslt = DlgDirListComboBox (szPath, IDC_COMBO , IDC_CURIDIR, DDL_READWRITE ｜DDL_READONLY｜DDL_HIDDEN｜ DDL_SYSTEM｜DDL_ARCHIVE)
return TRUE
}

（42） **为什么旋转按钮控件看起来倒转**
需要调用CSpinCtrl : : SetRange 设置旋转按钮控件的范围，旋转按钮控件的缺省上限为0，缺省下限为100，这意味着增加时旋转按控件的值由100变为0。下例将旋转按钮控件的范围设置为0到100：
BOOL CAboutDlg : : OnInitDialog ( )
{
CDialog : : OnInitDialog ( )

//set the lower and upper limit of the spin button
m_wndSpin . SetRange ( 0 ,100 )

return TRUE
}

Visual C++ 4.0 Print对话中的Copise旋转按钮控件也有同样的问题：按下Up按钮时拷贝的数目减少，而按下Down 按钮时拷贝的数目增加。

（43） **为什么旋转按钮控件不能自动地更新它下面的编辑控件**
如果使用旋转按钮的autu buddy特性， 则必须保证在对话的标记顺序中buddy窗口优先于旋转按钮控件。从Layout菜单中选择Tab Order菜单项（或者按下Crtl+D）可以设置对话的标签顺序。

（44） **如何用位图显示下压按钮**
Windows 95按钮有几处新的创建风格，尤其是BS_BITMAP和BS_ICON，要想具有位图按钮，创建按钮和调用CButton : : SetBitmap或CButton : : SetIcon时要指定BS_BITMAP或BS_ICON风格。

首先，设置按钮的图标属性。然后，当对话初始化时调用CButton: : SetIcon。注意：下例用图标代替位图，使用位图时要小心，因为不知道背景所有的颜色——并非每个人都使用浅灰色。

BOOL CSampleDlg : : OnInitDialog ( )
{
CDialog : : OnInitDialog ( )

//set the images for the push buttons .
BOOL CSampleDlg : : OnInitDialog ( )
{
CDialog : : OnInitDialog ( )

//set the images for the push buttons .
m_wndButton1.SetIcon (AfxGetApp ( ) —> LoadIcon (IDI _ IPTION1))
m_wndButton2.SetIcon (AfxGetApp ( ) —> LoadIcon (IDI _ IPTION2))
m_wndButton3.SetIcon (AfxGetApp ( ) —> LoadIcon (IDI _ IPTION3))

return TRUE
}

（45） **如何一个创建三态下压按钮**
可以使用新的BS_PUSHBUTTON 风格位和检测框以及按钮来创建一个三态下压按钮。这很容易，只需将检测框和按钮拖拉到对话中并指定属性Push—like即可。不用任何附加程序就可以成为三态下压按钮。

（46） **如何动态创建控件**
分配一个控件对象的实例并调用其Create成员函数。开发者最容易忽略两件事：忘记指定WS_VISBLE标签和在栈中分配控件对象。下例动态地创建一个下压按钮控件：
//In class declaration (.H file ).
private : CButton* m _pButton

//In class implementation (.cpp file ) .
m_pButton =new CButton
ASSERT_VALID (m_pButton)
m_pButton —>Create (_T ("Button Title ") , WS_CHILD ｜WS_VISIBLE ｜BS_PUSHBUTTON. Crect ( 0, 0, 100 , 24) , this , IDC _MYBUTTON )

（47） **如何限制编辑框中的准许字符**
如果用户在编辑控件中只允许接收数字，可以使用一个标准的编辑控件并指定新的创建标志ES_NUMBERS,它是Windows 95新增加的标志，该标志限制 编辑控件只按收数字字符。如果用户需要复杂的编辑控件，可以使用Microsoft 的屏蔽编辑控件，它是一个很有用的OLE定制控件。
如果希望不使用OLE 定制控件自己处理字符，可以派生一个CEdit类并处理WM_CHAR消息，然后从编辑控件中过滤出特定的字符。首先，使用ClassWizard建立一个 CEdit的派生类，其次，在对话类中指定一个成员变量将编辑控件分类在OnInitdialog 中调用CWnd: : SubclassDlgItem .

//In your dialog class declaration (.H file )
private : CMyEdit m_wndEdit // Instance of your new edit control .

//In you dialog class implementation (.CPP file )
BOOL CSampleDialog : : OnInitDialog ( )
{
…

//Subclass the edit lontrod .
m_wndEdit .SubclassDlgItem (IDC_EDIT,this)

…
}

使用ClassWizard处理WM_CHAR消息，计算nChar参量并决定所执行的操作，用户可以确定是否修改、传送字符。下例说明了如何显示字母字符，如果字符是字母字符，则调用CWnd OnChar，否则不调用OnChar.
//Only display alphabetic dharacters .
void CMyEdit : : OnChar (UINT nChar , UINT nRepCnt , UITN nFlags )
{
//Determine if nChar is an alphabetic character.
if (: : IsCharAlpha ( ( TCHAR) nChar ) )
CEdit : : OnChar (nChar, nRepCnt , nFlags )
}

如果要修改字符，则不能仅仅简单地用修改过的nChar调用CEdit: : OnChar，然后CEdit: : OnChar调用CWnd: : Default获取原来的wParam 和lParam 的值，这样是不行的。要修改一个字符，需要首先修改nChar，然后用修改过的nChar调用CWnd: : DefWindowProc。下例说明了如何将字符转变为大写：
//Make all characters uppercase
void CMyEdit : : OnChar (UINT nChar , UINT nRepCnt , UINT nFlags )
{
//Make sure character is uppercase .
if (: : IsCharAlpha ( .( TCHAR) nChar)
nChar=: : CharUpper(nChar )

//Bypass default OnChar processing and directly call
//default window proc.
DefWindProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ))
}

（48） **如何改变控件的颜色**
有两种方法。其一，可以在父类中指定控件的颜色，或者利用MFC4.0新的消息反射在控件类中指定颜色。 当控件需要重新着色时，工作框调用父窗口（通常是对话框）的CWnd: : OnCrtlColor,可以在父窗口类中重置该函数并指定控件的新的绘画属性。例如，下述代码将对话中的所有编辑控件文本颜色改为红色：
HBRUSH CAboutDig : : OnCtlColor (CDC * pDCM , CWnd * pWnd , UINT nCtlColor)

{
HBRUSH hbr = CDialog : : OnCtlColor (pDC, pWnd , nCtlColor )

//Draw red text for all edit controls .
if (nCtlColor= = CTLCOLOR_EDIT )
pDC —> SetTextColor (RGB (255, 0 , 0 , ) )

return hbr
}

然而，由于每个父窗口必须处理通知消息并指定每个控件的绘画属性，所以，这种方法不是完全的面向对象的方法。控件处理该消息并指定绘画属性更合情合理。消息反射允许用户这样做。通知消息首先发送给父窗口，如果父窗口没有处理则发送给控件。创建一个定制彩色列表框控件必须遵循下述步骤。

首先，使用ClassWizard 创建一个CListBox 的派生类并为该类添加下述数据成员。
class CMyListBox publilc CListBox
{
…
private
COLORREF m_clrFor // foreground color
COLORREF m_clrBack //background color
Cbrush m_brush //background brush
…
}
其次，在类的构造函数中，初始化数据中。
CMyListBox : : CMyListBox ()
{
//Initialize data members .
m_clrFore =RGB (255 , 255 , 0) //yellow text
m_clrBack=RGB (0 , 0 , 255) // blue background
m_brush . CreateSolidBrush (m _clrBack )
}

最后，使用ClassWizard处理反射的WM_CTLCOLOR(=WM_CTLCOLOR)消息并指定新的绘画属性。
HBRUSH CMyListBox : : CtlColor (CDC* pDC, UINT nCtlColor )
{
pDC—>SetTextColor (m_clrFore)
pDC—>SetBkColor (m_clrBack)

return (HBRUSH) m_brush.GetSafeHandle ()
}
现在，控件可以自己决定如何绘画，与父窗口无关。

（49） **当向列表框中添加多个项时如何防止闪烁**
调用CWnd::SetRedraw 清除重画标志可以禁止CListBox（或者窗口）重画。当向列表框添加几个项时，用户可以清除重画标志，然后添加项，最后恢复重画标志。为确保重画列表框的新项，调用SetRedraw (TRUE) 之后调用CWnd::Invalidate。

//Disable redrawing.
pListBox->SetRedraw (FALSE)

//Fill in the list box gere
//Enable drwing and make sure list box is redrawn.
pListBox->SetRedraw (TRUE)
pListBox->Invalidate ()

（50） **如何向编辑控件中添加文本**
由于没有CEdit:: AppendText函数，用户只好自己做此项工作。调用CEdit:: SetSel移动到编辑控件末尾，然后调用CEdit:: ReplaceSel添加文本。下例是AppendText 的一种实现方法：

void CMyEdit:: AppendText (LPCSTR pText)
{
int nLen=GetWindowTextLength ()
SetFocus ()
SetSel (nLen, nLen)

ReplaceSel (pText)
}

（51） **如何访问预定义的GDI对象**
可以通过调用CDC:: SlectStockObject使用Windows的几个预定义的对象，诸如刷子、笔以及字体。下例使用了Windows预定义的笔和刷子GDI对象在视窗中画一个椭圆。
//Draw ellipse using stock black pen and gray brush.
void CSampleView:: OnDraw (CDC* pDC)
{
//Determine size of view.
CRect rcView
GetClientRect (rcView)

//Use stock black pen and stock gray brush to draw ellipse.
pDC->SelectStockObject (BLACK_PEN)
pDC->SelectStockObject (GRAY_BRUSH)
//Draw the ellipse.
pDC->Ellipse (reView)
}

也可以调用新的SDK函数GetSysColorBrush获取一个系统颜色刷子，下例用背景色在视窗中画一个椭圆：
void CsampleView:: OnDraw (CDC* pDC)
{
//Determine size of view.
CRect rcView
GetClientRect (rcView)

//Use background color for tooltips brush.
CBrush * pOrgBrush=pDC->SelectObject ( CBrush ::FromHandle( ::GetSysColorBrush (COLOR_INFOBK)))

//Draw the ellipse.
pDC->Ellipse (rcView)

//Restore original brush.
pDC->SelectObject (pOrgBrush)
}

（52） **如何获取GDI对象的属性信息**
可以调用GDIObject:: GetObject。这个函数将指定图表设备的消息写入到缓冲区。下例创建了几个有用的辅助函数。
//Determine if font is bold.
BOOL IsFontBold (const CFont&font)
{
LOGFONT stFont
font.GetObject (sizeof (LOGFONT), &stFont)
return (stFont.lfBold)? TRUE: FALSE
}

//Return the size of a bitmap.
CSize GetBitmapSize (const CBitmap&bitmap)
{
BITMAP stBitmap
bitmap.GetObject (sizeof (BITMAP), &stBitmap)
return CSize (stBitmap.bmWidth, stBitmap.bmHeight)
}

//Create a pen with the same color as a brush.
BOOL CreatePenFromBrush (Cpen&pen, cost Cbrush&brush)
{
LOGBRUSH stBrush
brush.Getobject (sizeof (LOGBRUSH), &stBrush)
return pen. Createpen (PS_SOLID, 0, stBrush.ibColor)
}

（53） **如何实现一个橡皮区矩形**
CRectTracker是一个很有用的类，可以通过调用CRectTracker::TrackRubberBand 响应WM_LBUTTONDOWN消息来创建一个橡皮区矩形。
下例表明使用CRectTracker移动和重置视窗中的蓝色椭圆的大小是很容易的事情。

首先，在文件档中声明一个CRectTracker数据成员：
class CSampleView : Public CView
{
…
public :
CrectTracker m_tracker
…
}

其次，在文档类的构造函数中初始化CRectTracker 对象：
CSampleDoc:: CSampleDOC ()
{
//Initialize tracker position, size and style.
m_tracker.m_rect.SetRect (0, 0, 10, 10)
m_tracker.m_nStyle=CRectTracker:: resizeInside | CRectTracker ::dottedLine
}

然后，在OnDraw函数中画椭圆和踪迹矩形：
void CSampleView:: OnDraw (CDC* pDC)
{
CSampleDoc* pDoc=GetDocument ()
ASSERT_VALID (pDoc)

//Select blue brush into device context.
CBrush brush (RGB (0, 0, 255))
CBrush* pOldBrush=pDC->SelectObject (&brush)

//draw ellipse in tracking rectangle.
Crect rcEllipse
pDoc->m_tracker.GetTrueRect (rcEllipse)
pDC->Ellipse (rcEllipse)

//Draw tracking rectangle.
pDoc->m_tracker.Draw (pDC)
//Select blue brush out of device context.
pDC->Selectobject (pOldBrush)
}

最后，使用ClassWizard处理WM_LBUTTONDOWN消息，并增加下述代码。该段代码根据鼠标击键情况可以拖放、移动或者重置椭圆的大小。
void CSampleView::OnLButtonDown (UINT nFlags, CPoint point)
{
//Get pointer to document.
CSampleDoc* pDoc=GetDocument ()
ASSERT_VALID (pDoc)

//If clicked on ellipse, drag or resize it.Otherwise create a
//rubber-band rectangle nd create a new ellipse.
BOOL bResult=pDoc->m_tracker.HitTest (point)!= CRectTracker::hitNothing

//Tracker rectangle changed so update views.
if (bResult)
{
pDoc->m_tracker.Track (this,point,TRue)
pDoc->SetModifiedFlag ()
pDoc->UpdateAllViews (NULL)
}

else
pDoc->m-tracker.TrackRubberBand(this,point,TRUE)
CView:: onLButtonDown (nFlags,point)
}

（54） **如何更新翻转背景颜色的文本**
调用CDC:: SetBkmode并传送OPAQUE用当前的背景颜色填充背景，或者调用CDC::SetBkMode并传送TRANSPAARENT使背景保持不变，这两种方法都可以设置背景模式。下例设置背景模式为TRANSPARENT，可以两次更新串，用花色带黑阴影更新文本。黑色串在红色串之后，但由于设置了背景模式仍然可见。

void CSampleView:: OnDraw (CDC* pDC)
{
//Determint size of view.
CRect rcView
GetClientRect (rcVieew)

//Create sample string to display.
CString str (_T ("Awesome Shadow Text．．．"))
//Set the background mode to transparent.
pDC->SetBKMode (TRANSPARENT)

//Draw black shadow text.
rcView.OffsetRect (1, 1)
pDc->SetTextColor (RGB (0, 0, 0))
pDC->DrawText (str, str.GetLength (), rcView, DT_SINGLELINE | DT_CENTER | DT_VCENTER)

//Draw red text.
rcView.OffsetRect (-1,-1)
pDc->SetTextColor (RGB (255, 0, 0))
pDC->DrawText (str, str.GetLength (), rcView, DT_SINGLELINE | DT_CENTER | DT_VCENTER)

}

（55） **如何创建一个具有特定点大小的字体**
可以指定字体逻辑单位的大小，但有时指定字体的点的大小可能会更方便一些。可以如下将字体的点转换为字体的高度：

int nHeigth=mulDiv (nPointSize, -dc.GetDeviceCaps (LOGPIXELSY), 72)
下例创建了一个8点的Apial字体：
…
CClientDC dc (AqfxGetMainWnd ())

m_font. CreateFont (MulDiv (8, -dc.GetDeviceCaps (LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH | FF-SWISS,_T("Arial"))

（56） **如何计算一个串的大小**
函数CDC:: Det text Extent 根据当前选择的字体计算一个串的高度和宽度。如果使用的不是系统字体而是其他字体，则在调用GetTextExtent之前将字体选进设备上下文中是很重要的，否则计算高度和宽度时将依据系统字体，由此得出的结果当然是不正确的。下述样板程序当改变下压按钮的标题时动态调整按钮的大小，按钮的大小由按钮的字体和标题的大小而定。响应消息WM_SETTEXT时调用OnSetText，该消息使用ON_MESSAE宏指令定义的用户自定义消息。

LRESULT CMyButton:: OnSettext (WPARAM wParam, LPARAM lParam)
{
//Pass message to window procedure.
LRESULT bResult=CallWindowProc (*GetSuperWndProcAddr(), m_hWnd, GetCurrentMessage() ->message,wParam,lParam)
//Get title of push button.
CString strTitle
GetWindowText (strTitle)

//Select current font into device context.
CDC* pDC=GetDc ()
CFont*pFont=GetFont ()
CFont*pOldFont=pDC->SelectObject (pFont)

//Calculate size of title.
CSize size=pDC->GetTextExent (strTitle,strTitle.GetLength())

//Adjust the button's size based on its title.
//Add a 5-pixel border around the button.
SetWindowPos (NULL, 0, 0, size.cx+10, size.cy+10, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE)
//Clean up.
pDC->SelectFont (pOldFont)
ReleaseDC (pDC)

return bResult
}

（57） **如何显示旋转文本**
只要用户使用TrueType或者GDI笔或字体就可以显示旋转文本(有些硬件设备也支持旋转光栅字体）。LOGFONT结构中的ifEscapement成员指定了文本行和x轴的角度，角度的单位是十分之一度而不是度，例如，ifEscapement为450表示字体旋转45度。为确保所有的字体沿坐标系统的同一方向旋转，一定要设置ifEscapement成员的CLIP_LH_ANGLES位，否则，有些字体可能反向旋转。下例使用了14点Arial字体每间隔15度画一个串。
void CSampleView:: OnDraw (CDC* pDC)
{
//Determine the size of the window.
CRect rcClient
GetClientRect (rcClient)

//Create sample string.
CString str (_T ("Wheeee．．．I am rotating!"))
//Draw transparent, red text.
pDC->SetBkMode (TRANSPARENT)
pDC->SetTextColor (RGB (255,0,0))
CFont font
//font object
LOGFONT stFont //font definition
//Set font attributes that will not change.
memset (&stFont, 0, sizeof (LOGFONT))
stFont.ifheight=MulDiv (14, -pDC->GetDeviceCaps(LOGPIXELSY), 72)
stFont.ifWeight=FW_NORMAL
stFont.ifClipPrecision=LCIP_LH_ANGLES
strcpy (stFont.lfFaceName, "Arial")

//Draw text at 15degree intervals.
for (int nAngle=0 nAngle<3600 nAngle+=150)
{
//Specify new angle.
stFont.lfEscapement=nAngle

//Create and select font into dc.
font.CreateFontIndirect(&stfont)
CFont* pOldFont=pDC ->SelectObject(&font)

//Draw the text.
pDC->SelectObject(pOldFont)
font.DelectObjext()
}
}

（58） **如何正确显示包含标签字符的串**
调用GDI文本绘画函数时需要展开标签字符，这可以通过调用CDC:: TabbedTextOut或者CDC:: DrawText并指定DT_EXPANDTABS标志来完成。TabbedTextOut函数允许指定标签位的数组，下例指定每20设备单位展开一个标签：

void CSampleView:: OnDraw (CDC* pDC)
{
CTestDoc* pDoc=GetDocument ()
ASSERT_VALID (pDoC)

CString str
str.Format (_T ("Cathy/tNorman/tOliver"))
int nTabStop=20 //tabs are every 20 pixels
pDC->TabbedtextOut (10, 10, str, 1, &nTabStop, 10)
}

（59） **如何快速地格式化一个CString对象**
调用CString:: Format，该函数和printf函数具有相同的参数，下例说明了如何使用Format函数：

//Get size of window.
CRect rcWindow
GetWindowRect (rcWindow)
//Format message string.
CString strMessage
strMessage.Format (_T ("Window Size (%d, %d)"),

rcWindow.Width (), rcWindow.Height ())

//Display the message.
MessageBox (strmessage)

（60） **串太长时如何在其末尾显示一个省略号**
调用CDC:: DrawText并指定DT_END_ELLIPSIS标志，这样就可以用小略号取代串末尾的字符使其适合于指定的边界矩形。如果要显示路径信息，指定DT_END_ELLIPSIS标志并省略号取代串中间的字符。

void CSampleView:: OnDraw (CDC* pDC)
{
CTestDoc* pDoc=GetDocument ()
ASSERT_VALID (pDoc)

//Add ellpsis to end of string if it does not fit
pDC->Drawtext (CString ("This is a long string"), CRect (10, 10, 80, 30), DT_LEFT | DT_END_ELLIPSIS)

//Add ellpsis to middle of string if it does not fit
pDC->DrawText (AfxgetApp () ->m_pszhelpfilePath, CRect (10, 40, 200, 60), DT_LEFT | DT_PATH_ELLIPSIS)
}

（61） **为什么即使调用EnableMenuItem菜单项后，菜单项还处于禁止状态**
需要将CFrameWnd:: m_bAutomenuEnable设置为FALSE，如果该数据成员为TRUE（缺省值），工作框将自动地禁止没有ON_UPDATE_COMMAND_UI或者ON_COMMAND的菜单项。

//Disable MFC from automatically disabling menu items.
m_bAuoMenuEnable=FALSE
//Now enable the menu item.
CMenu* pMenu=GetMenu ()
ASSERT_VALID (pMenu)

pMenu->EnableMenuItem (ID_MENU_ITEM,MF_BYCOMMAND | MF_ENABLED)

（62） **如何给系统菜单添加一个菜单项**
给系统菜单添加一个菜单项需要进行下述三个步骤：
首先，使用Resource Symbols对话（在View菜单中选择Resource Symbols．．．可以显示该对话）定义菜单项ID，该ID应大于0x0F而小于0xF000；
其次，调用CWnd::GetSystemMenu获取系统菜单的指针并调用CWnd:: Appendmenu将菜单项添加到菜单中。下例给系统菜单添加两个新的
int CMainFrame:: OnCreate (LPCREATESTRUCT lpCreateStruct)
{
…
//Make sure system menu item is in the right range.
ASSERT (IDM_MYSYSITEM &0xFFF0)==IDM_MYSYSITEM)
ASSERT (IDM-MYSYSITEM<0xF000)

//Get pointer to system menu.
CMenu* pSysmenu=GetSystemmenu (FALSE)
ASSERT_VALID (pSysMenu)
//Add a separator and our menu item to system menu.
CString StrMenuItem (_T ("New menu item"))
pSysMenu->Appendmenu (MF_SEPARATOR)
pSysMenu->AppendMenu (MF_STRING, IDM_MYSYSITEM, strMenuitem)

…
}

现在，选择系统菜单项时用户应进行检测。使用ClassWizard处理WM_SYSCOMMAND消息并检测用户菜单的nID参数：
void CMainFrame:: OnSysCommand (UINT nID,LPARAM lParam)
{
//Determine if our system menu item was selected.
if ( (nID & 0xFFF0)==IDM_MYSYSITEM)
{
//TODO-process system menu item
}

else
CMDIFrameWnd ::OnSysCommand (nID, lParam)
}
最后，一个设计良好的UI应用程序应当在系统菜单项加亮时在状态条显示一个帮助信息，这可以通过增加一个包含系统菜单基ID的串表的入口来实现。

（63） **如何确定顶层菜单所占据的菜单行数**
这可以通过简单的减法和除法来实现。首先，用户需要计算主框窗口的高度和客户区；其次，从主框窗口的高度中减去客户区、框边界以及标题的高度；最后，除以菜单栏的高度。下例成员函数是一个计算主框菜单所占据的行数的代码实现。

int CMainFrame:: GetMenuRows ()
{
CRect rcFrame,rcClient
GetWindowRect (rcFrame)
GetClientRect (rcClient)
return (rcFrame.Height () -rcClient.Height () - :: GetSystemMetrics(SM_CYCAPTION) - (:: getSystemMetrics(SM_CYFRAME) *2)) / :: GetSystemMetrics(SM_CYMENU)
}

（64） **在用户环境中如何确定系统显示元素的颜色**
调用SDK函数GetSysColor可以获取一个特定显示元素的颜色。下例说明了如何在MFC函数CMainFrameWnd:: OnNcPaint中调用该函数设置窗口标题颜色。

void CMiniFrameWnd:: OnNcPaint ()
{
…
dc.SetTextColor (:: GetSysColor (m_bActive ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT))
…

（65） **如何查询和设置系统参数**
在Windows 3.1 SDK中介绍过SDK函数SystemParametersInfo，调用该函数可以查询和设置系统参数，诸如按键的重复速率设置、鼠标双击延迟时间、图标字体以及桌面覆盖位图等等。

//Create a font that is used for icon titles.
LOGFONT stFont
∶: SystemParametersInfo (SPIF_GETICONTITLELOGFONT, sizeof (LOGFONT), &stFont, SPIF_SENDWININICHANGE)
m_font.CreateFontIndirect (&stFont)

//Change the wallpaper to leaves.bmp.
∶ : SystemParametersInfo (SPI_SETDESKWALLPAPER, 0, _T (" forest.bmp"), SPIF_UPDATEINIFILE)

（66） **如何确定当前屏幕分辨率**
调用SDK函数GetSystemMetrics，该函数可以检索有关windows显示信息，诸如标题大小、边界大小以及滚动条大小等等。

//Initialize CSize object with screen size.
CSize sizeScreen (GetSystemMetrics (SM_CXSCREEN),
GetSystemMetrics (SM_CYSCREEN))

（67） **如何使用一个预定义的Windows光标**
调用CWinApp:: LoadStandardCursor并传送光标标识符。
BOOL CSampleDialog:: OnSetCursor (CWnd* pWnd,
UINT nHitTest, UINT
message)
{
//Display wait cursor if busy.
if (m_bBusy)
{
SetCursor (AfxGetApp () ->LoadStandardCursor (IDC_WAIT))
return TRUE
}

return CDialog:: OnSetCursor (pWnd. nHitTest,message)
}

（68） **如何检索原先的Task Manager应用程序使用的任务列表**
原先的Task Manager应用程序显示顶层窗口的列表。为了显示该列表，窗口必须可见、包含一个标题以及不能被其他窗口拥有。调用CWnd:: GetWindow可以检索顶层窗口的列表，调用IsWindowVisible、GetWindowTextLength以及GetOwner可以确定窗口是否应该在列表中。下例将把TaskManager窗口的标题填充到列表中。

void GetTadkList (CListBox&list)
{
CString strCaption
//Caption of window.

list.ResetContent ()
//Clear list box.

//Get first Window in window list.
ASSERT_VALID (AfxGetMainWnd ())
CWnd* pWnd=AfxGetMainWnd () ->GetWindow (GW_HWNDFIRST)

//Walk window list.
while (pWnd)
{
// I window visible, has a caption, and does not have an owner?
if (pWnd ->IsWindowVisible()
&& pWnd ->GetWindowTextLength ()
&&! pWnd ->GetOwner ())
{

//Add caption o window to list box.

pWnd ->GetWindowText (strCaption)

list.AddString (strCaption)
}
//Get next window in window list.
pWnd=pWnd ->GetWindow(GW_HWNDNEXT)
}
}

（69） **如何确定Windows和Windows系统目录**
有两个SDK函数可以完成该功能。GetWindowsDirectory和GetSystemDirectory，下例说明了如何使用这两个函数：

TCHAR szDir [MAX_PATH]
//Get the full path of the windows directory.
∶ : GetWindowsDirectory (szDir, MAX_PATH)
TRACE ("Windows directory %s/n", szDir)
//Get the full path of the windows system directory.
∶ : GetSystemDirectory (szDir, MAX_PATH)
TRACE ("Windows system directory %s/n", szDir)

（70） **在哪儿创建临文件**
调用SDK函数GetTemPath可以确定临时文件的目录，该函数首先为临时路径检测TMP环境变量：如果没有指定TMP，检测TMP环境变量，然后返回到当前目录。下例说明了如何创建一个临时文件。

…
//get unique temporary file.
CString strFile
GetUniqueTempName (strFile)
TRY
{
//Create file and write data.Note that file is closed
//in the destructor of the CFile object.
CFile file (strFile,CFile ::modeCreate | CFile:: modeWrite)

//write data
}

CATCH (CFileException, e)
{
//error opening file
}
END_CATCH
…

Void GetuniqueTempName (CString& strTempName)
{
//Get the temporary files directory.
TCHAR szTempPath [MAX_PATH]
DWORD dwResult=:: GetTempPath (MAX_PATH, szTempPath)
ASSERT (dwResult)

//Create a unique temporary file.
TCHAR szTempFile [MAX_PATH]
UINT nResult=GetTempFileName (szTempPath, _T ("~ex"),0,szTempfile)
ASSERT (nResult)

strTempName=szTempFile
}

（71） **我怎样才能建立一个等待光标?**
调 用 BeginWaitCursor 函 数 来 启 动 等 待 光 标，调 用 EndWaitCursor 函 数 来 结 束 等 待 光 标。要 注 意，二 者 都 要 调 用 app 的 成 员 函 数，如 下 所 示:

    AfxGetApp()->BeginWaitCursor();
    // 要做的事
    AfxGetApp()->EndWaitCursor();

（72） **我在MDI框架中有个 form 视窗。它有个取消按钮，我需要当用户按取消按钮时可关闭form视窗。我应该如何关闭该文档?**
调 用 OnCloseDocument 函 数。

（73） **如何访问桌面窗口**
静态函数CWnd:: GetDesktopWindow 返回桌面窗口的指针。下例说明了MFC函数CFrameWnd::BeginModalStae是如何使用该函数进入内部窗口列表的。

void CFrameWnd::BeginModalState ()
{
…
//first count all windows that need to be disabled
UINT nCount=0
HWND hWnd= :: GetWindow (:: GetDesktopWindow(), GW_CHILD)
while (hWnd!=NULL)
{
if (:: IsWindowEnabled (hwnd)
&& CWnd::FromHandlePermanent (hWnd)!=NULL
&& AfxIsDescendant (pParent->m_hWnd, hWnd)
&& :: SendMessage (hWnd, WM_DISABLEMODAL, 0, 0)==0)
{
++nCount
}
hWnd=:: GetWindow (hWnd, GW_HWNDNEXT)
}
…

（74） **什么是COLORREF? 我该怎样用它?**
COLORREF 是 一 个 32-bit 整 型 数 值，它 代 表 了 一 种 颜 色。你 可 以 使 用 RGB 函 数 来 初 始 化 COLORREF。例 如：

    COLORREF color = RGB(0, 255, 0);
RGB 函 数 接 收 三 个 0-255 数 值，一 个 代 表 红 色， 一 个 代 表 绿 色， 一 个 代 表 蓝 色。在 上 面的 例 子 中， 红 色 和 蓝 色 值 都 为 0，所 以 在 该 颜 色 中 没 有 红 色 和 蓝 色。绿 色 为 最 大 值 255。所 以 该 颜 色 为 绿 色。0,0,0 为 黑 色，255,255,255 为 白 色。

另 一 种 初 始 化 COLORREF 的 方 法 如 下 所 示：

    CColorDialog colorDialog;
    COLORREF color;

    if( colorDialog.DoModal() == IDOK )
    {
        color = colorDialog.GetColor();
    }
这 段 代 码 使 用 了 MFC 中 的 颜 色 对 话 框，它 需 要 文 件。

（75） **AppWizard所产生的STDAFX文件是干什么用的?**
它 主 要 是 协 助 产 生 预 编 译 头 文 件 的。通 常 你 是 不 需 要 修 改 它 的。

（76） **我在我的程序中是了CDWordArray。我向它添加了约10,000个整数，这使得它变得非常非常慢。为什么会这么糟?**
CDWordArray 是 很 好 用 的，只 是 因 为 你 没 有 指 定 数 组 的最大尺寸。因 此，当 你 添 加 新 元 素 时，该 类 会 从 堆 中 重 新 分 配 空 间。不 幸 的 是，该 类 会 在 每 次 插 入 新 元 素 时 都 为 数 组 重 新 分 配 空 间。如 果 你 向 它 添 加 了 很 多 新 元 素，所 有 这 些 分 配 和 复 制 数 组 的 操 作 会 就 会 使 它 变 慢。解 决 该 问 题 的 方 法 是，你 可 以 使 用 SetSize 函 数 的 第 二 个 参 数 来 改 变 这 种 重 新 分 配 的 频 率。例 如，如 果 你 把 该 参 数 设 置 为 500，则 每 次 数 组 空 间 超 出 时 它 才 重 新 分 配 并 添 加 500 个 新 空 间，而 不 是 1 个。这 样 一 来，你 就 可 以 不 用 重 新 分 配 而 添 加 了 另 外 499 个 元 素 空 间，这 也 会 大 大 提 高 程 序 的 运 行 速 度。

（77） **我该如何改变MDI框架窗口的子窗口的大小以使在窗口以一定的大小打开?**
在 视 中 的 OnInitialUpdate 函 数 中 调 用 GetParentFrame 函 数。GetParentFrame 会 返 回 一 指 向 一 保 存 有 该 视 的 框 架 窗 口 的 指 针。然 后 调 用 在 框 架 窗 口 上 调 用 MoveWindow。

（78） **在我的程序的某些部分，我可以调用 MessageBox 函数来建立一个信息对话框，例如在视类中。但是，在其它部分我却不能，如文档类中。为什么？我怎样才能在我的应用程序类中建立一个信息对话框？**
MessageBox 函 数 来 自 CWnd 类，所 以 你 只 能 在 从 CWnd 继 承 的 类 ( 如 CView ) 中 调 用 它。但 是，MFC 也 提 供 了 AfxMessageBox 函 数，你 可 以 在 任 何 地 方 调 用 它。

（79） **我需要在我的程序中设置全局变量，以使文档中的所有类都能访问。我应该吧它放到哪儿?**
把 该 变 量 放 到 该 应 用 程 序 类 的 头 文 件 中 的 attribute 处。然 后，在 程 序 的 任 何 地 方，你 都 可 以 用 下 面 的 方 法 来 访 问 该 变 量：

    CMyApp *app = (CMyApp *)AfxGetApp();
    app->MyGlobalVariable = ...

（80） **我听说MFC可以发现内存漏洞，我怎样使用该特性?**
如 果 你 在 Debug 菜 单 中 的 Go 选 项 ( 不 是 Project 菜 单 中 的 Execute 选 项 ) 来 运 行 你 的 应 用 程 序，MFC 应 该 在 程 序 终 止 时 报 告 内 存 漏 洞。如 果 没 有，那 么 试 试 运 行 MFC Tracer 工 具 程 序 ( 在 VC++ 程 序 组 中 )，并 启 动 跟 踪。然 后 返 回 应 用 程 序。

（81） **我怎样才能在我的应用程序中循环浏览已经打开的文档?**
使用CDocTemplate中未公开的GetFirstDocPosition()和GetNextDoc()函数。 

（82）**才能在我的应用程序中循环浏览已经打开的视?**
使 用 CDocument 中 未 公 开 的 GetFirstViewPosition() 和 GetNextView() 函 数。

（83）**数PreCreateWindow是干什么用的?**
PreCreateWindow 允 许 你 在 调 用 CreateWindow 之 前 来 改 变 窗 口 属 性。

（84）**该怎样防止MFC在窗口标题栏上把文档名预置成应用程序名?**
在 PreCreateWindow 函 数 中 删 除 FWS_PREFIXTITLE 标 志 的 窗 口 样 式：

    cs.style &= ~FWS_PREFIXTITLE;

（85） **我应该怎样防止MFC在窗口标题栏上添加文档名?**
在 PreCreateWindow 函 数 中 删 除 FWS_ADDTOTITLE 标 志 的 窗 口 样 式：

    cs.style &= ~FWS_ADDTOTITLE ;

（86） **我应该如何改变视窗口的大小?**
因 为 视 窗 口 实 际 上 是 框 架 窗 口 的 子 窗 口，所 以 你 必 须 改 变 框 架 窗 口 的 大 小，而 不 是 改 变 视 窗 口。使 用 CView 类 中 的 GetParentFrame() 函 数 获 得 指 向 框 架 窗 口 的 指 针，然 后 调 用 MoveWindow() 函 数 来 改 变 框 架 的 大 小。这 会 使 变 尺 寸 的 视 充 满 框 架 窗 口。

（87） **我有一无模式对话框。我怎样才能在窗口退出时删除CDialog对象?**
把“delete this”加 到 PostNcDestroy 中。这 主 要 用 在 需 要 自 动 删 除 对 象 的 场 合。

（88） **为什么把“delete this”放在PostNcDestroy中而不是OnNcDestroy?**
OnNcDestroy 只 被 已 建 立 的 窗 口 调 用。如 果 建 立 窗 口 失 败 ( 如 PreCreateWindow )，则 没 有 窗 口 处 来 发 送 WM_NCDESTROY 消 息。PostNcDestroy 是 在 对 象 窗 口 被 完 全 删 除，在 OnNcDestroy 后，甚 至 在 窗 口 建 立 失 败 之 后 调 用 的。

（89） **File菜单中的MRU列表是从哪儿来的？列表中的名字放在哪儿了？我怎样才能改变列表中项目的最大值？**
在 应 用 程 序 类 的 InitInstance 函 数 中 对 LoadStdProfileSettings 的 调 用 中。该 调 用 接 受 一 个 参 数 ( 在 缺 省 情 况 下 如 果 没 有 传 递 值 则 为 4 )。MRU 文 件 名 是 从 INI 文 件 中 调 用 的。如 果 你 有 带 有 ID_FILE_MRU_FILE1 的 ID 的 菜 单 选 项，它 会 为 调 入 的 MRU 列 表 所 替 换。如 果 你 改 变 传 递 给 LoadStdProfileSettings 的 数 值 ( 最 大 为 16 )，则 你 就 改 变 了 所 装 如 文 件 名 的 最 大 值。

（90） **我在菜单中添加了新的项。但是，当我选该项时，在状态栏上没有出现任何提示信息。为什么?**
打 开 资 源 文 件 中 的 菜 单 模 板。打 开 新 菜 单 选 项 的 属 性 对 话 框。在 对 话 框 的 底 部 的 Prompt 编 辑 框 中 ，你 可 以 如 下 指 定 状 态 栏 上 的 提 示 信 息 和 工 具 栏 上 的 提 示 信 息 ( 如 果 你 已 经 建 立 的 工 具 栏 按 钮 ):

    Status bar string/nFlying tag

（91） **我怎样才能在应用程序的缺省系统菜单中加上一些东西?**
系 统 菜 单 与 其 它 菜 单 类 似，你 可 以 添 加 或 删 除 项 目，这 需 要 使 用 CMenu 类 的 成 员 函 数。下 面 的 代 码 在 你 的 系 统 菜 单 后 面 添 加 一 个 新 菜 单 项：

    CMenu *sysmenu;
    sysmenu = m_pMainWnd->GetSystemMenu(FALSE);
    sysmenu->AppendMenu(MF_STRING, 1000, "xxx");
参 见 MFC 帮 助 文 件 中 的 CMenu 类。

（92） **我建立了一个对话框。但是当我显示该对话框时，第一个编辑框总是不能获得焦点，我必须单击它来使它获得焦点。我怎样才能使第一个编辑框在对话框打开时就获得焦点?**
打 开 资 源 编 辑 器 中 的 对 话 框 模 板。在 Layout 菜单 中 选 择 Tab Order 选 项。按 你 的 需 求 单 击 对 话 框 中 的 控 制 来 重 新 排 列 这 些 控 制 的 tab 顺 序。

（93） **我怎样才能使一个窗口具有“always on top”特性?**
在 调 用 OnFileNew 后，在 你 的 InitInstance 函 数 中 加 上 下 面 的 代 码：

m_pMainWnd->SetWindowPos(&CWnd::wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

(94)   我要为我的form view添加文档模板。我先建立了对话框模板，然后使用ClassWizard建立了基于CFormView的新类，它也是从CDocument继承来的。我还建立了相应的资源并在InitInstance中添加了新的文档模板。但是，当我试图运行该程序时，出现了Assertion信息。为什么?

form 的 对 话 框 模 板 需 要 些 特 殊 设 置 以 便 可 用 于 CFromView。确 保 这 些 设 置 的 最 简 单 方 法 是 使 用 AppWizard 来 建 立 CFormView 应 用 程 序，并 查 看 AppWizard 所 建 立 的 对 话 框 模 板 所 选 择 的Styles Properties。你 会 发 现 该 对 话 框 模 板 具 有 下 列 样 式：没 有 标 题 栏、不 可 见 和“Child”。把 你 的 form view 的 对 话 框 属 性 变 成 这 样 就 可 以 了。

(95)   我在一对话框中有一列表框，我需要tabbed列表框中的项目。但是，当我处理含有tab字符(用AddString添加的)的列表项时，tab被显示成小黑块而没有展开。哪儿出错了?

在 对 话 框 模 版 中，打 开 列 表 框 的 属 性。确 保 选 择 了“Use Tabstops” 样 式。然 后，确 保 在 对 话 框 类 中 OnInitDialog 函 数 中 调 用 SetTabStops。

(96)  我建立了一个应用程序，并使用了CRecordset类。但是，当我运行该程序时，它试图要访问数据库，并给出“Internal Application Error”对话框。我应该怎样做?

通 常 情 况 下，当 你 的 程 序 中 向 数 据 库 发 送 信 息 的 SQL 语 句 出 现 问 题 时 才 出 现 该 对 话 框。例 如，参 见 下 面 的 例 子：

    set.m_strFilter = "(ZipCode = '27111')";
如 果 ZipCode 列 被 定 义 为 字 符 串 时 不 会 出 现 问 题，如 果 定 义 为 long，则 会 出 现“Internal Application Error”对 话 框，这 是 由 于 类 型 不 匹 配 的 缘 故。如 果 你 删 除 27111 的 单 引 号，则 不 会 出 现 问 题。当 你 看 到“Internal Application Error”时，最 好 检 查 一 下 试 图 要 发 送 给 数 据 库 的 SQL 语 句。

(97)   我用ClassWizard建立了一个类。但是，我把名字取错了，我想把它从项目中删除，应该如何做?

在 ClassWizard 对 话 框 关 闭 后，用 文 件 管 理 器 删 除 新 类 的 H 和 CPP 文 件。然 后 打 开 ClassWizard，它 会 提 示 丢 失 了 两 个 文 件，并 询 问 你 该 如 何 做。你 可 以 选 择 从 项 目 中 删 除 这 两 个 问 的 按 钮。

(98)     当我打开应用程序中的窗口时，我要传递该窗口的矩形尺寸。该矩形指定了窗口的外围大小，但是当我调用GetClientRect时，所得到的尺寸要比所希望的值要小(因为工具栏和窗口边框的缘故)。有其它方法来计算窗口的尺寸吗?

参 见 CWnd::CalcWindowRect。

(99)   我在文档类中设置了一个整型变量。但是，当我试图把该变量写入Serialize函数中的archive文件中时，出现了类型错误。而文档中的其它变量没有问题。为什么?

archive 类 只 重 载 某 些 类 型 的 >> 和 << 操 作 符。“int”类 型 没 有 在 其 中，也 许 是 因 为 int 变 量 在 Windows 3.1 与 Windows NT/95 有 所 不 同 的 缘 故 吧。“long”类 型 得 到 了 支 持，所 以 你 可 以 把 int 类 型 改 成 long 型。参 见 MFC 帮 助 文 件 中 CArchive 类。

(100)  如何控制菜单的大小?
我用MFC的CMenu生成了一个动态菜单(例如File,Edit,View...Help), 我想控制这个菜单的大小(长+高).

方法一:查找 WM_MEASUREITEM 和 MEASUREITEMSTRUCT.
方法二:查询系统::GetSystemMetric(SM_CXMENUSIZE).

     /* 你可以通过如下代码来获得文本的大小:
        (A)获得被使用的字体 */

       NONCLIENTMETRICS ncm;
     HFONT hFontMenu;
     SIZE size;
     size.cy = size.cy = 0;

     memset(&ncm, 0, sizeof(NONCLIENTMETRICS));
     ncm.cbSize = sizeof(NONCLIENTMETRICS);
     if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0))
     {
          hFontMenu = CreateFontIndirect(&ncm.lfMenuFont);
          /*
          (B) 获得菜单项的文本: */
          char szText[_MAX_PATH];

          pMenu->GetMenuString(0, szText, _MAX_PATH, MF_BYPOSITION);
          /*
          然后,获得菜单项文本的高度: */
          HFONT hFontOld;
          HDC hDC;

          hDC = ::GetDC(NULL);
          hFontOld = (HFONT) ::SelectObject(hDC, hFontMenu);
          GetTextExtentPoint32(hDC, szText, lstrlen(szText), &size);
          SelectObject(hDC, hFontOld);
          ::ReleaseDC(NULL, hDC);
     }
     /*此时,size.cy即为高度,size.cx为宽度,你可以给菜单加上自定义的高度和宽度,通过比较,我发现宽度为4

比较合适。*/

(101)  改变LVIS_SELECTED的状态颜色?
我想将CListCtrl项和CTreeCtrl项在LVIS_SELECTED状态时的颜色变灰.

方法一:查找函数CustomDraw,它是IE4提供的公共控制,允许有你自己的代码.
方法二:生成一个draw控件,然后在DrawItem中处理文本颜色.

(102)   如何只存储文档的某一部分?
我只想存储文档的某一部分,能否象使用文件一样使用文档?(也就是有定位函数).将每个CArchive类设置为CFile类的派生类,这样你就能使用Seek等成员函数.

(103)   保存工具条菜单有bug吗?

使用浮动菜单条时,SaveBarState和LoadBarState出现了问题.如果菜单是浮动的,重起应用程序时它会出现在左上角,而它固定在屏幕其它位置时,下一次启动就会出现在该位置,这是什么原因?你试试这个PToolBar->Create(this,...,ID_MYTOOLBAR);
你的工具条需要包括id,而不是象默认的工具条那样.

(104)   Tip of the day的bug

我创建了一个简单的mdi应用程序,使用.BSF(自定义的文档扩展名)作为它的文档我保存一个foo.bsf文档后,可以在资源管理器中双击该文件打开mdi应用程序同时打开foo.bsf文档.但当我给mdi应用程序加上a tip of the day组件之后,从资源管理器中双击foo.bsf后,就会给我一个警告:ASSERT(::IsWindow(m_hWnd)),然后mdi应用程序就死那了.

当从dde启动应用程序(例如:双击相关文档)时,"Tip of the Day"是有bug的.你可以看看函数"ShowTipAtStartup",它在"InitInstance"中调用,可以看到tip of the day作为一个模式对话框显示,在处理其它消息时它一直进行消息循环你可心修改ShowTipAtStartup使其从dde启动时不出现tip of the day.
void CTipOfApp::ShowTipAtStartup(void)
        {
                // CG: This function added by 'Tip of the Day' component.

                CCommandLineInfo cmdInfo;
                ParseCommandLine(cmdInfo);

                if (
                        cmdInfo.m_bShowSplash &&
                        cmdInfo.m_nShellCommand != CCommandLineInf:FileDDE
                        )
                {
                        CTipDlg dlg;
                        if (dlg.m_bStartup)
                                dlg.DoModal();
                }
        }
如果还有其它bug,你可以设定cmdInfo.m_nShellCommand的过滤.

(105)   如何可以让我的程序可以显示在其它的窗口上面?

让用户选择"总是在最上面"最好是在系统菜单里加入一个选项.可以通过修改WM_SYSCOMMAND消息来发送用户的选择.菜单的命令标识(id)会作为一个参数传给OnSysCommand().要定义标识(id),将如下代码加入到CMainFrame.CPP中:

    #define WM_ALWAYSONTOP WM_USER + 1
将"总在最上面"的菜单项加入到系统菜单中,将如下代码加入到函数CMainFrame::OnCreate()中:

      CMenu* pSysMenu = GetSystemMenu(FALSE);
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, WM_ALWAYSONTOP,
                     "&Always On Top");
使用ClassWizard,加入对WM_SYSCOMMAND消息的处理,你应该改变消息过滤器,使用系统可以处理这个消息.
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
    switch ( nID )
    {
    case WM_ALWAYSONTOP:

        if ( GetExStyle() & WS_EX_TOPMOST )
        {
            SetWindowPos(&wndNoTopMost, 0, 0, 0, 0,
                SWP_NOSIZE | SWP_NOMOVE);
            GetSystemMenu(FALSE)->CheckMenuItem(WM_ALWAYSONTOP,
                MF_UNCHECKED);
        }
        else
        {
            SetWindowPos(&wndTopMost, 0, 0, 0, 0,
                SWP_NOSIZE | SWP_NOMOVE);
            GetSystemMenu(FALSE)->CheckMenuItem(WM_ALWAYSONTOP,MF_CHECKED);
        }

        break;

    default:
        CFrameWnd::OnSysCommand(nID, lParam);
    }
}

(106)    如何控制窗口框架的最大最小尺寸?

要控制一个框架的的最大最小尺寸,你需要做两件事情.在CFrameWnd的继承类中处理消息WM_GETMINMAXINFO,结构MINMAXINFO设置了整个窗口类的限制,因此记住要考虑工具条,卷动条等等的大小.

// 最大最小尺寸的象素点 - 示例
#define MINX 200
#define MINY 300
#define MAXX 300
#define MAXY 400

void CMyFrameWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    CRect rectWindow;
    GetWindowRect(&rectWindow);

    CRect rectClient;
    GetClientRect(&rectClient);

      // get offset of toolbars, scrollbars, etc.
    int nWidthOffset = rectWindow.Width() - rectClient.Width();
    int nHeightOffset = rectWindow.Height() - rectClient.Height();

    lpMMI->ptMinTrackSize.x = MINX + nWidthOffset;
    lpMMI->ptMinTrackSize.y = MINY + nHeightOffset;
    lpMMI->ptMaxTrackSize.x = MAXX + nWidthOffset;
    lpMMI->ptMaxTrackSize.y = MAXY + nHeightOffset;
}
第二步,在CFrameWnd的继承类的PreCreateWindow函数中去掉WS_MAXIMIZEBOX消息,否则在最大化时你将得不到预料的结果.

BOOL CMyFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style &= ~WS_MAXIMIZEBOX;
    return CFrameWnd::PreCreateWindow(cs);
}

(107)    如何改变窗口框架的颜色?

MDI框架的客户区被另一个窗口的框架所覆盖.为了改变客户区的背景色,你需要重画这个客户窗口.为了做到这点,你要处理消息WM_ERASEBKND产生一个新类,从CWnd继承,姑且称之为CMDIClient.给它加上一个成员变量,
#include "MDIClient.h"
class CMainFrame : public CMDIFrameWnd
{
...
protected:
CMDIClient m_wndMDIClient;
}
在CMainFrame中重载CMDIFrameWnd::OnCreateClient
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    if ( CMDIFrameWnd::OnCreateClient(lpcs, pContext) )
    {
        m_wndMDIClient.SubclassWindow(m_hWndMDIClient);
        return TRUE;
    }
    else
        return FALSE;
}
然后就可以加入对消息WM_ERASEBKGND的处理了.

(108)    如何将应用程序窗口置于屏幕正中?

要将你的应用程序窗口放置在屏幕正中央,只须在MainFrame的OnCreate函数中加入:
CenterWindow( GetDesktopWindow() );


