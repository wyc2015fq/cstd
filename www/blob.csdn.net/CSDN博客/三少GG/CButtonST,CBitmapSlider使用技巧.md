# CButtonST,CBitmapSlider使用技巧 - 三少GG - CSDN博客
2012年03月15日 19:44:27[三少GG](https://me.csdn.net/scut1135)阅读数：1615
**[CButtonST使用技巧（一）](http://www.cnblogs.com/wqj1212/archive/2008/03/17/1109429.html)**
[下载CButtonST示例源代码](http://www.vckbase.com/code/downcode.asp?id=1556)
[下载CButtonST类的源代码](http://www.vckbase.com/code/downcode.asp?id=1750)
本篇主要包括了以下的效果：
1. [在按钮上加入Icon，使Icon和文字同时显示](http://www.vckbase.com/document/viewdoc/?id=518#F1)
2. [显示平面按钮](http://www.vckbase.com/document/viewdoc/?id=518#F2)
3. [使按钮上的图标可变](http://www.vckbase.com/document/viewdoc/?id=518#F3)
4. [设置按钮在不同状态下的底色和文字颜色](http://www.vckbase.com/document/viewdoc/?id=518#F4)
5. [设置图标和文字的位置](http://www.vckbase.com/document/viewdoc/?id=518#F5)
6. [根据Icon的形状设置按钮的形状](http://www.vckbase.com/document/viewdoc/?id=518#F6)
7. [添加Tooltips](http://www.vckbase.com/document/viewdoc/?id=518#F7)
以下是Basic Features的演示界面：
![](http://www.vckbase.com/document/journal/vckbase17/images/CButtonST2.gif)
![](http://www.vckbase.com/document/image/paragraph.gif)**效果一：在按钮上加入Icon，使Icon和文字同时显示 **
假设按钮ID为IDC_BUTTON1
1. 添加成员变量
CButtonST	m_btn;2. 添加Icon资源，设其ID设为IDI_ICON1
3. 在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
                        m_btn.SetIcon(IDI_ICON1);
                        m_btn.SetFlat(FALSE);注意:为达到最好效果，请根据Icon的大小调整按钮的大小。
实例：演示程序中的Standar 按钮。
![](http://www.vckbase.com/document/image/paragraph.gif)效果二：显示平面按钮
只需要在效果一的基础上把m_btn.SetFlat(FALSE);语句去掉。
![](http://www.vckbase.com/document/image/paragraph.gif)效果三：使按钮上的图标可变
假设按钮ID为IDC_BUTTON1
1. 添加成员变量CButtonST	m_btn;2. 添加两个Icon资源，ID设为IDI_ICON1和IDI_ICON2
IDI_ICON1是普通状态的图标，IDI_ICON2是按下时的图标
3. 在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
                        m_btn.SetIcon(IDI_ICON2,IDI_ICON1);实例：演示程序中的Halloween 按钮。 
![](http://www.vckbase.com/document/image/paragraph.gif)效果四：设置按钮在不同状态下的底色和文字颜色
假设按钮ID为IDC_BUTTON1
1. 添加成员变量CButtonST	m_btn;2. 添加Icon资源，设其ID设为IDI_ICON1
3. 在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
                        m_btn.SetIcon(IDI_ICON1);
                        m_btn.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208));
                        m_btn.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(208,208,208));
                        m_btn.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);提示：SetColor函数和OffsetColor函数的第一个参数表示按钮的各个状态，前景色就是文字的颜色，它们的取值表示：BTNST_COLOR_BK_IN	//鼠标放在按钮内时的背景色
                        BTNST_COLOR_FG_IN,	//鼠标放在按钮内时的前景色
                        BTNST_COLOR_BK_OUT,	//普通状态时的背景色
                        BTNST_COLOR_FG_OUT,	//普通状态时的前景色
                        BTNST_COLOR_BK_FOCUS,	//按钮被按下后的背景色
                        BTNST_COLOR_FG_FOCUS,	//按钮被按下后的前景色实例：演示程序中的48×48 icon 按钮、Zip按钮等。
![](http://www.vckbase.com/document/image/paragraph.gif)效果五：设置图标和文字的位置
假设按钮ID为IDC_BUTTON1
1.添加成员变量CButtonST	m_btn;2.添加Icon资源，ID设为IDI_ICON1
3.在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
                        m_btn.SetIcon(IDI_ICON1);
                        m_btn.SetAlign(CButtonST::ST_ALIGN_VERT);
                        提示：SetAlign函数的第一个参数表示位置信息，缺省情况下，文字在图标右边ST_ALIGN_HORIZ	// 文字在右
                        ST_ALIGN_VERT	//文字在下
                        ST_ALIGN_HORIZ_RIGHT// 文字在左
                        实例：演示程序中的Search 按钮。
![](http://www.vckbase.com/document/image/paragraph.gif)效果六：根据Icon的形状设置按钮的形状
假设按钮ID为IDC_BUTTON1
1.添加成员变量CButtonST	m_btn;2.添加Icon资源，设其ID设为IDI_ICON1
3.在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
                        m_btn.SetIcon(IDI_ICON1);
                        m_btn.DrawBorder(FALSE);
                        m_btn.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(208,208,208));
                        m_btn.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(208,208,208));
                        m_btn.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(208,208,208));注意:为达到最好效果，请根据Icon的大小调整按钮的大小。
并根据实际情况设置背景颜色。请事先将按钮的文字去掉。
![](http://www.vckbase.com/document/image/paragraph.gif)效果七：添加Tooltips
假设按钮ID为IDC_BUTTON1
1.添加成员变量CButtonST	m_btn;2.添加Icon资源，设其ID设为IDI_ICON1
3. 在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
                        m_btn.SetIcon(IDI_ICON1);
                        m_btn.SetTooltipText(_T("This is a tooltip.\r\nMultiline!"));
**CButtonST使用技巧（二）**
上一篇文章《[CButtonST使用技巧（一）](http://www.vckbase.com/document/viewdoc.asp?id=518)》介绍了CButtonST使用的7种技巧，本篇主要包括了以下的效果：
1. [改变鼠标进入按钮时的形状(超链接效果)](http://www.vckbase.com/document/viewdoc/?id=519#F1)
2. [菜单按钮](http://www.vckbase.com/document/viewdoc/?id=519#F2)
3. [位图按钮](http://www.vckbase.com/document/viewdoc/?id=519#F3)
4. [按钮焦点](http://www.vckbase.com/document/viewdoc/?id=519#F4)
5. [CheckBox按钮](http://www.vckbase.com/document/viewdoc/?id=519#F5)
6. [透明按钮(按钮背景和窗口背景一样)](http://www.vckbase.com/document/viewdoc/?id=519#F6)
7. [特殊的PictureBox](http://www.vckbase.com/document/viewdoc/?id=519#F7)
![](http://www.vckbase.com/document/image/paragraph.gif) 效果一：改变鼠标进入按钮时的形状(超链接效果)
假设按钮ID为IDC_BUTTON1
1. 添加成员变量CButtonST    m_btn;2. 添加Icon资源，设其ID设为IDI_ICON1
3. 在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
m_btn.SetIcon(IDI_ICON1);
m_btn.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
m_btn.SetURL("www.scut.edu.cn");
m_btn.SetTooltipText("www.scut.edu.cn");
m_btn.SetBtnCursor(IDC_CURSOR1);
![](http://www.vckbase.com/document/image/paragraph.gif) 效果二：菜单按钮
假设按钮ID为IDC_BUTTON1
1.添加成员变量
CButtonST    m_btn;2. 添加Icon资源，设其ID设为IDI_ICON1
3. 添加Menu，ID设为IDR_MENU
4. 在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
m_btn.SetIcon(IDI_ICON1);
m_btn.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
m_btn.SetMenu(IDR_MENU, m_hWnd);注意:菜单的效果类似与右键菜单，所有在设置菜单选项时只设置一列就够了。另外菜单的宽度与菜单的文字长度有关，可以用空格占位的办法以达到最佳效果。
在菜单中引入位图的方法
1.添加ToolBar，依次使用Menu菜单项ID作为ToolBar按钮的ID。
2.ToolBar上的按钮图标将显示在相应的菜单项中。
将上面代码中的m_btn.SetMenu(IDR_MENU, m_hWnd);
改为m_btn.SetMenu(IDR_MENU, m_hWnd, TRUE, IDR_TOOLBAR);
IDR_TOOLBAR为相应ToolBar的ID。
![](http://www.vckbase.com/document/image/paragraph.gif) 效果三：位图按钮
假设按钮ID为IDC_BUTTON1
1.添加成员变量
CButtonST    m_btn;2.添加位图资源，ID设为IDB_BITMAP1
3.在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
m_btn.SetBitmaps(IDB_BITMAP1,RGB(0,0,0));
m_btn.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);提示: 上面的SetBitmaps函数会将图片中颜色值为RGB(0,0,0)的点设为透明。
![](http://www.vckbase.com/document/image/paragraph.gif) 效果四：按钮焦点
假设按钮ID为IDC_BUTTON1
1.添加成员变量
CButtonST    m_btn;2. 添加Icon资源，设其ID设为IDI_ICON1
3. 在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
m_btn.SetIcon(IDI_ICON1);
m_btn.SetAlign(CButtonST::ST_ALIGN_VERT);
m_btn.DrawFlatFocus(TRUE);
![](http://www.vckbase.com/document/image/paragraph.gif) 效果五：CheckBox按钮
假设按钮ID为IDC_BUTTON1
1.添加成员变量
CButtonST    m_btn;2. 添加Icon资源，ID设为IDI_ICON1和IDI_ICON2
3.在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_CHECK1,this);
m_btn.SetIcon(IDI_ICON1,IDI_ICON2);
m_btn.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
提示:
IDI_ICON1为选中时候显示的图标
IDI_ICON2为为选中时候的图标
![](http://www.vckbase.com/document/image/paragraph.gif) 效果六：透明按钮
假设按钮ID为IDC_BUTTON1
1.添加成员变量
CButtonST    m_btn;2. 添加Icon资源，ID设为IDI_ICON1
3.在OnInitDialog函数中初始化按钮m_btn.SubclassDlgItem(IDC_BUTTON1,this);
m_btn.SetIcon(IDI_ICON1);
m_btn.DrawTransparent(TRUE);
![](http://www.vckbase.com/document/image/paragraph.gif) 效果七：特殊的PictureBox
利用CButtonST的特殊显示风格，完全可以把它当作一个PictureBox控件使用。这时一般会把按钮的属性设置为disable。
实例：演示程序的About页中使用了CButtonST来代替PictureBox。
本篇将向大家介绍两个**CButtonST的派生类。**
![](http://img.jcwcn.com/attachment/portal/2007-3/27/0732710151664976.gif) 一、CWinXPButtonST类
CWinXPButtonST类 是一个CButtonST的派生类。它的特点是使用了WindowsXP中的圆角、黑色边框的风格。它包括WinXPButtonST.h和WinXPButtonST.cpp两个文件，而由于它是从CButtonST派生而来的，所以在引用它时,也要一并引入CButtonST的文件。
使用：
1. 引入文件，添加文件
2. 导入文件后，在SdtAfx.h文件中添加#include "WinXPButtonST.h"3. 添加成员变量CWinXPButtonST	  m_btn2;4. 假设按钮ID为IDC_BUTTON
添加Icon资源，ID设为IDI_ICON1
5. 在OnInitDialog函数中初始化按钮
m_btn2.SubclassDlgItem(IDC_BUTTON,this);m_btn2.SetIcon(IDI_ICON1);m_btn2.SetRounded(TRUE);注意：但是它存在一个不足的地方，就是按钮的圆角是画出来的，按钮的形状仍然是原来的矩形。所以如果在有背景色的窗口中使用该按钮的话就会显得相当碍眼。
如图所示，在黑色的背景色中显示出难看的尖角：
![](http://img.jcwcn.com/attachment/portal/2007-3/27/0732710151637868.gif)
解决的办法是通过SetColor函数使得按钮的底色与窗口的底色一致。 
![](http://img.jcwcn.com/attachment/portal/2007-3/27/0732710151664976.gif) 二、CShadeButtonST类
CShadeButtonST类 是CButtonST的另一个派生类。它的特点是支持更为多样化的背景，通过SetShade函数可以为按钮设置9种不通的背景效果。它包括四个文件，它们分别是CeXDib.h、CeXDib.cpp、ShadeButtonST.h和ShadeButtonST.cpp。在引用它时,同样要一并引入CButtonST的文件。
使用：
1.引入文件，添加文件
2.导入文件后，在SdtAfx.h文件中添加
#include "ShadeButtonST.h"3.添加成员变量CShadeButtonST	m_btn3;4.假设按钮ID为IDC_BUTTON3
添加Icon资源，ID设为IDI_ICON1
5.在OnInitDialog函数中初始化按钮m_btn3.SubclassDlgItem(IDC_BUTTON3,this);m_btn3.SetIcon(IDI_ICON1);m_btn3.SetShade(CShadeButtonST::SHS_METAL);下面给出各种背景效果和对应的参数值。
![](http://img.jcwcn.com/attachment/portal/2007-3/27/0732710151657864.gif)
![](http://img.jcwcn.com/attachment/portal/2007-3/27/0732710151697345.gif)
5  **CButtonST类公共接口函数**
以下是CButtonST类的所有公共成员函数（函数名按音序排列先后）：
**CButtonST::DrawBorder**
DWORD DrawBorder(BOOL bDrawBorder = TRUE, BOOL bRepaint = TRUE)
注释：
因为CButtonST的默认显示风格是平面按钮。调用此函数用来显示按钮边框。
参数：
bDrawBorder: 值为TRUE则显示按钮边框。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行完毕。
**CButtonST::DrawFlatFocus**
DWORD DrawFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = TRUE)
注释：
此函数只对平面按钮有效，调用此函数可以在按钮中显示焦点。
参数：
bDrawFlatFocus : 值为TRUE则会在平面按钮上显示焦点。
bRepaint : 值为TRUE则马上重画按钮。
返回值：
BTNST_OK: 表示函数执行成功。
**CButtonST::DrawTransparent**
void DrawTransparent(BOOL bRepaint = FALSE)
注释：
调用此函数来激活透明显示模式。注意，此操作是不可逆的，而且必须在按钮实例创建之后调用。函数将会在内存中保留一份窗口背景的拷贝，因此请真的有需要时才调用此函数，以免浪费内存空间。透明模式一般是在带有背景的窗口中使用。
参数：
bRepaint : 为TRUE则马上重绘按钮。
**CButtonST::OffsetColor**
DWORD OffsetColor(BYTE byColorIndex, short shOffset, BOOL bRepaint = TRUE)
注释：
调用此函数可以设置按钮在各种状态下显示指定的底色。函数会根据所传递的偏移量参数以及按钮原来的默认底色产生一个新的颜色值。通过这样的方法可以使得按钮在不同状态下的颜色变化更为自然。
参数：
byColorIndex : 状态索引值，它的取值必须是以下枚举值之一。
取值注释
BTNST_COLOR_BK_IN 鼠标指针位于按钮上面时的底色
BTNST_COLOR_FG_IN 鼠标指针位于按钮上面时的前景色
BTNST_COLOR_BK_OUT 鼠标指针不在按钮上面时的底色
BTNST_COLOR_FG_OUT 鼠标指针不在按钮上面时的前景色
BTNST_COLOR_BK_FOCUS 焦点位于按钮上面时的底色
BTNST_COLOR_FG_FOCUS 焦点位于按钮上面时的前景色
*前景色一般是指文字的颜色
shOffsetColor: 指定颜色的偏移量，它的取值范围是[-255,255]。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK : 函数执行成功。
BTNST_INVALIDINDEX : 由 byColorIndex 指定状态索引值无效。
BTNST_BADPARAM : 根据偏移量生成的值超出RGB的颜色取值范围。
**CButtonST::SetAlign**
DWORD SetAlign(BYTE byAlign, BOOL bRepaint = TRUE)
注释：
调用此函数可以设定按钮上图标（或位图）于按钮上文字的相对位置。
参数：
byAlign: 位置参数，它的取值必须是以下枚举量之一。
取值注释
ST_ALIGN_HORIZ 图标/图片在左，文字在右
ST_ALIGN_VERT 图标/图片在上，文字在下
ST_ALIGN_HORIZ_RIGHT 图标/图片在右，文字在左
默认值为ST_ALIGN_HORIZ. 
bRepaint: 为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDALIGN: 位置参数无效。
**CButtonST::SetAlwaysTrack**
DWORD SetAlwaysTrack(BOOL bAlwaysTrack = TRUE)
注释：
此函数只对平面按钮有效。调用此函数可以设定按钮是否显示高亮颜色。
参数：
bAlwaysTrack: 值为TRUE时，无论前窗口是否为激活窗口，只要鼠标指针移动到按钮上面都会发按钮的显示状态发生变化。默认值为TRUE。
值为FALSE时，如果窗口未被激活，按钮的显示不会发生变化。
返回值：
BTNST_OK : 函数执行成功。
**CButtonST::SetBitmaps**
DWORD SetBitmaps(int nBitmapIn, COLORREF crTransColorIn, int nBitmapOut = NULL, COLORREF crTransColorOut = 0)
DWORD SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut = NULL, COLORREF crTransColorOut = 0)
注释：
调用此函数可以在按钮上显示指定的图片（Icon）。如果对同一个按钮重复调用了此函数的话，那么按钮只显示最后一次指定的图片。
参数：
nBitmapIn: 图片的ID值。当鼠标指针位于按钮之上时显示该图片。
crTransColorIn: 颜色值。ID为nBitmapIn的图片中与该颜色相同的区域将显示透明。
nBitmapOut: 图片的ID值。当鼠标指针不在按钮之上时显示该图片。
crTransColorOut: 颜色值。ID为nBitmapOut的图片中与该颜色相同的区域将显示透明。
hBitmapIn: 图片的句柄。当鼠标指针位于按钮之上时显示该图片。
crTransColorIn: 颜色值。由hBitmapIn指定的图片中与该颜色相同的区域将显示透明。
hBitmapOut: 图片的ID值。当鼠标指针不在按钮之上时显示该图片。
crTransColorOut: 颜色值。由hBitmapOut指定的图片中与该颜色相同的区域将显示透明。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDRESOURCE: 指定资源读取失败。
BTNST_FAILEDMASK: 颜色提取失败。
**CButtonST::SetBtnCursor**
DWORD SetBtnCursor(int nCursorId = NULL, BOOL bRepaint = TRUE)
注释：
这个函数可以让鼠标在移动到按钮上面的时候显示指定的鼠标指针。
参数：
nCursorId: 鼠标指针的ID值。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDRESOURCE: 读取指定资源失败。
**CButtonST::SetCheck**
DWORD SetCheck(int nCheck, BOOL bRepaint = TRUE)
注释：
当按钮被当作CheckBox使用的时候，用来设置按钮的状态。
参数：
nCheck : 1表示选中; 0表示没选中
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
**CButtonST::SetColor**
DWORD SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE)
注释：
调用此函数可以设置按钮在各种状态下显示指定的底色。
参数：
byColorIndex : 状态索引值。具体含义可以参见OffsetColor函数中的参数(byColorIndex)说明。
crColor : 颜色值。设置按钮指定状态的底色。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDINDEX: 无效的状态索引值。
**CButtonST::SetDefaultColors**
DWORD SetDefaultColors(BOOL bRepaint = TRUE)
注释：
调用此函数之后，程序将使用默认颜色来设定按钮在各个状态下的颜色。这些颜色值可能会因为Windows版本的不同而不同。
参数：
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
**CButtonST::SetFlat**
DWORD SetFlat(BOOL bFlat = TRUE, BOOL bRepaint = TRUE)
注释：
调用此函数来设置按钮的显示风格。
参数：
bFlat: 值为TRUE则显示为平面按钮; 值为FALSE则显示为标准的Windows按钮
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
**CButtonST::SetIcon**
DWORD SetIcon(int nIconIn, int nIconOut = NULL)
DWORD SetIcon(HICON hIconIn, HICON hIconOut = NULL)
注释：
调用此函数可以在按钮上显示指定的图标（Icon）。如果对同一个按钮重复调用了此函数的话，那么按钮只显示最后一次指定的图标。
参数：
nIconIn : 图标的ID值，当鼠标指针移动到按钮上面的时候显示该图标。如果该参数被赋值为 NULL ，则分配给这个按钮的所有图标资源将被释放。
nIconOut: 图标的ID值，当鼠标指针不在按钮上面的时候显示该图标。调用函数的时候可以忽略该参数。
hIconIn : 图标的句柄，当鼠标指针移动到按钮上面的时候显示该图标。如果该参数被赋值为 NULL ，则分配给这个按钮的所有图标资源将被释放。
hIconOut: 图标的句柄，当鼠标指针不在按钮上面的时候显示该图标。调用函数的时候可以忽略该参数。
返回值：
BTNST_OK: 表示函数执行成功。
BTNST_INVALIDRESOURCE: 表示函数执行失败，例如无法读取指定的图标资源。
**CButtonST::SetMenu**
DWORD SetMenu(UINT nMenu, HWND hParentWnd, BOOL bWinXPStyle = TRUE, UINT nToolbarID = NULL, CSize sizeToolbarIcon = CSize(16, 16), COLORREF crToolbarBk = RGB(255, 0, 255), BOOL bRepaint = TRUE)
DWORD SetMenu(UINT nMenu, HWND hParentWnd, BOOL bRepaint = TRUE)
注释：
要调用此函数必须与BCMenu类一起使用。函数将为按钮指派一个菜单，当点击按钮的时候会弹出所指派的菜单。此函数的调用与宏有关，如果在代码中声明了BTNST_USE_BCMENU 宏，则应该调用SetMenu函数的第一种形式，否则应该调用SetMenu的后一种形式。
参数：
nMenu : 菜单的ID值。
hParentWnd: 菜单所属的窗口句柄。菜单选项将触发该窗口内的时间。
bWinXPStyle: 值为TRUE则会以XP风格显示菜单, 值为FALSE则会以标准风格显示菜单。
nToolbarID: 工具条的ID值。程序会将工具条中与菜单项ID值相同图片加入到菜单中。
sizeToolbarIcon: 指定工具条中每一个图标的大小，它是一个CSize 类型的实例。注意请确保工具条中所有图标的大小一致。
crToolbarBk: 颜色值。工具条图片中与该颜色相同的区域将显示为透明。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDRESOURCE: 无法读取指定资源。
**CButtonST::TooltipText**
void SetTooltipText(int nText, BOOL bActivate = TRUE)
void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE)
注释：
调用此函数可以为按钮设定提示信息。
参数：
nText: 字符串的ID值。
lpszText: 所要显示的字符串。
nActivate: 值为TRUE表示激活提示功能, 值为FALSE表示不激活提示功能。
**CButtonST::SetURL**
DWORD SetURL(LPCTSTR lpszURL = NULL)
注释：
调用此函数后，单击按钮将打开参数所指定的连接。
参数：
lpszURL: 包含链接信息的字符串。
返回值：
BTNST_OK: 函数执行成功。
CBitmapSlider 位图进度条控件
老外写的一个自绘控件类，能做出MediaPlayer那样的进度条效果，配合SkinMagic你就能做出一个非常棒的浏览器了
CBitmapSlider类传在如下地址：
[http://www.codeproject.com/Articles/4850/CBitmapSlider](http://www.codeproject.com/Articles/4850/CBitmapSlider)
怎么使用，里面的注释都有，自己看看
我自己附加点说明吧，一般这么用
CBitmapSlider
 m_sliderVol; CBitmapSlider m_sliderProg;
m_sliderVol.MoveWindow( CRect(247, 112, 304, 127), TRUE ); m_sliderVol.SetBitmapChannel( IDB_MP_CHANNEL, IDB_MP_CHANNEL_ACTIVE ); m_sliderVol.SetBitmapThumb( IDB_MP_THUMB, IDB_MP_THUMB_ACTIVE,
 TRUE ); m_sliderVol.DrawFocusRect( FALSE );
m_sliderProg.MoveWindow( CRect(13, 144, 313, 152), TRUE ); m_sliderProg.SetBitmapChannel( IDB_PROG_CHANNEL, IDB_PROG_CHANNEL_ACT ); m_sliderProg.SetBitmapThumb( IDB_PROG_THUMB, IDB_PROG_THUMB_ACTIVE,
 TRUE ); m_sliderProg.DrawFocusRect( FALSE );
这样就完成了初始化，那些图片的ID都是自定义的资源ID，这2个主要的接口从你传入的图片ID进行初始化类中的CBitmap m_bmChannel和CBitmap m_bmThumb，你看源代码就知道了
其他的初始化的接口函数在实际中用得不算很多
