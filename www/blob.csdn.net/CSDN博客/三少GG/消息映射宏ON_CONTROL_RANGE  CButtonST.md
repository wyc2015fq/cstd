# 消息映射宏ON_CONTROL_RANGE || CButtonST - 三少GG - CSDN博客
2012年05月14日 18:08:36[三少GG](https://me.csdn.net/scut1135)阅读数：1483标签：[null																[command																[工具																[windows																[byte																[include](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
**[消息映射宏之-ON_CONTROL_RANGE](http://blog.21ic.com/user1/5983/archives/2009/64673.html)**
|单选钮和复选框都可以响应BN_CLICKED消息，如果一组中有多个单选钮，分别创建消息处理函数就比较麻烦了，利用MFC的消息映射宏ON_CONTROL_RANGE可以避免这种麻烦，该映射宏把多个ID连续的控件发出的消息映射到同一个处理函数上，这样，我们只要编写一个消息处理函数就可以对一组单选钮的BN_CLICKED消息作出响应，ClassWizard不支持该宏，必须手工创建。ON_CONTROL_RANGE消息映射宏的第一个参数是控件消息码，第二和第三个参数分别指明了一组连续的控件ID中的头一个和最后一个ID，最后一个参数是消息处理函数名。注意，起始、 终止ID号必须是连续的，如果不能确定该组控件的ID是否是连续，请用View->Resource Symbols…命令检查控件的ID值，如果发现两个ID是不连续的，读者可以改变对ID的定义值使之连续，但要注意改动后的值不要与别的ID值发生冲突。　　在BEGIN_MESSAGE_MAP 和 END_MESSAGE_MAP之间 //}}AFX_MSG_MAP之后加入ON_CONTROL_RANGE(BN_CLICKED,IDC_CHK1,IDC_CHK2,OnCHKClicked)然后在对话框类的头文件的AFX_MSG 块中声明消息处理函数，……//{{AFX_MSG(CMyView)……//}}AFX_MSG　afx_msg void OnCHKClicked (UINT nCmdID);　DECLARE_MESSAGE_MAP()最后，在对话框类所在CPP文件的最后插入消息处理函数，如下所示：CRadioAndCheckButtonDialog:: OnCHKClicked，　void CRadioAndCheckButtonDialog:: OnCHKClicked (UINT nCmdID) 　　{　……switch(nID){case IDC_CHK1:AfxMessageBox("IDC_CHK1");break;case IDC_CHK2:AfxMessageBox("IDC_CHK2");break;……}　   http://jingyan.baidu.com/article/656db918b3f344e381249c19.html# VC ON_CONTROL_RANGE多个控件响应一个方法- 浏览： 720- 更新： 2011-04-09 17:04摘要：关于VC ON_CONTROL_RANGE多个控件响应一个方法的深入研究。![](http://hiphotos.baidu.com/exp/pic/item/95afee1f0a617ca2e0fe0b84.jpg)## 步骤/方法- 　　分三个步骤-       在头文件中声明函数例如　　afx_msg void onNum(UINT uID)-       在.cpp文件中添加函数体　　void CCalculatorDlg::OnNum(UINT uID)　　{　　UINT index=uID-IDC_NUM_0;　　CString num;　　num.Format(_T("%d"),index);　　AfxMessageBox(num);　　}-      添加消息映射　　ON_CONTROL_RANGE(BN_CLICKED,IDC_NUM_0,IDC_NUM_9,OnNum)　　第一个参数 事件　　第二个参数 控件的第一个id　　第三个参数 空间的最后一个id　　第四个参数 响应事件的方法ON_COMMAND_RANGE和ON_CONTROL_RANGE宏有什么区别呢？2011-05-25 8:45ON_COMMAND 以及 ON_COMMAND_RANGE 是用于处理菜单项或工具栏按钮的命令消息的.对于复选框这样的控件，请使用ON_CONTROL_RANGE1. 打开 resource.h，将你的复选框控件ID进行连续编号，例如1001，1002... 2. 在对话框类的头文件中添加公有成员函数：afx_msg void OnCheckBox(UINT uID) ; 3. 在对话框类的cpp文件中BEGIN_MESSAGE_MAP(CTestDlg, CDialog) 的 //}}AFX_MSG_MAP下添加代码 ON_CONTROL_RANGE(BN_CLICKED,1001,1003,OnCheckBox) 4. 然后在cpp文件中添加函数void CTestDlg::OnCheckBox(UINT uID) {} 5. 在函数内判断 uID，来确定用户单击了哪项复选框，并作相应处理。|ON_COMMAND 以及 ON_COMMAND_RANGE 是用于处理菜单项或工具栏按钮的命令消息的.对于复选框这样的控件，请使用ON_CONTROL_RANGE1. 打开 resource.h，将你的复选框控件ID进行连续编号，例如1001，1002... 2. 在对话框类的头文件中添加公有成员函数：afx_msg void OnCheckBox(UINT uID) ; 3. 在对话框类的cpp文件中BEGIN_MESSAGE_MAP(CTestDlg, CDialog) 的 //}}AFX_MSG_MAP下添加代码 ON_CONTROL_RANGE(BN_CLICKED,1001,1003,OnCheckBox) 4. 然后在cpp文件中添加函数void CTestDlg::OnCheckBox(UINT uID) {} 5. 在函数内判断 uID，来确定用户单击了哪项复选框，并作相应处理。|
|----|----|
|ON_COMMAND 以及 ON_COMMAND_RANGE 是用于处理菜单项或工具栏按钮的命令消息的.对于复选框这样的控件，请使用ON_CONTROL_RANGE1. 打开 resource.h，将你的复选框控件ID进行连续编号，例如1001，1002... 2. 在对话框类的头文件中添加公有成员函数：afx_msg void OnCheckBox(UINT uID) ; 3. 在对话框类的cpp文件中BEGIN_MESSAGE_MAP(CTestDlg, CDialog) 的 //}}AFX_MSG_MAP下添加代码 ON_CONTROL_RANGE(BN_CLICKED,1001,1003,OnCheckBox) 4. 然后在cpp文件中添加函数void CTestDlg::OnCheckBox(UINT uID) {} 5. 在函数内判断 uID，来确定用户单击了哪项复选框，并作相应处理。| |
[](http://blog.renren.com/share/240815773/7750056620)
# **[关于CButtonST类](http://blog.renren.com/share/240815773/7750056620)**
### 来源：[邱子鉴的日志](http://blog.renren.com/share/240815773/7750056620)
之前，一直以为VC++下只能做出那种跟Windows 2000界面差不多的按钮风格，既单调又不美观。今天才发现原来还有CButtonST类，而且做出的按钮是相当的漂亮。现在就把CButtonST类的一些函数摘列如下：
CButtonST类主要包括BtnST.h、BtnST.cpp、BCMenu.h和BCMenu.cpp四个文件。如果打算使用CButtonST的窗口不止一个，那么我建议你在VC++的Workspace | FileView中导入以上的四个文件之后，然后在SdtAfx.h文件中添加合适的include语句，如#include "BtnST.h"。
CButtonST类的所有公共成员函数（函数名按音序排列先后）：
**CButtonST::DrawBorder**
DWORD DrawBorder(BOOL bDrawBorder = TRUE, BOOL bRepaint = TRUE)
注释：
因为CButtonST的默认显示风格是平面按钮。调用此函数用来显示按钮边框。
参数：
bDrawBorder: 值为TRUE则显示按钮边框。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行完毕。
**CButtonST::DrawFlatFocus**
DWORD DrawFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = TRUE)
注释：
此函数只对平面按钮有效，调用此函数可以在按钮中显示焦点。
参数：
bDrawFlatFocus : 值为TRUE则会在平面按钮上显示焦点。
bRepaint : 值为TRUE则马上重画按钮。
返回值：
BTNST_OK: 表示函数执行成功。
**CButtonST::DrawTransparent**
void DrawTransparent(BOOL bRepaint = FALSE)
注释：
调用此函数来激活透明显示模式。注意，此操作是不可逆的，而且必须在按钮实例创建之后调用。函数将会在内存中保留一份窗口背景的拷贝，因此请真的有需要时才调用此函数，以免浪费内存空间。透明模式一般是在带有背景的窗口中使用。
参数：
bRepaint : 为TRUE则马上重绘按钮。
**CButtonST::OffsetColor**
DWORD OffsetColor(BYTE byColorIndex, short shOffset, BOOL bRepaint = TRUE)
注释：
调用此函数可以设置按钮在各种状态下显示指定的底色。函数会根据所传递的偏移量参数以及按钮原来的默认底色产生一个新的颜色值。通过这样的方法可以使得按钮在不同状态下的颜色变化更为自然。
参数：
byColorIndex : 状态索引值，它的取值必须是以下枚举值之一。
取值注释
BTNST_COLOR_BK_IN 鼠标指针位于按钮上面时的底色
BTNST_COLOR_FG_IN 鼠标指针位于按钮上面时的前景色
BTNST_COLOR_BK_OUT 鼠标指针不在按钮上面时的底色
BTNST_COLOR_FG_OUT 鼠标指针不在按钮上面时的前景色
BTNST_COLOR_BK_FOCUS 焦点位于按钮上面时的底色
BTNST_COLOR_FG_FOCUS 焦点位于按钮上面时的前景色
*前景色一般是指文字的颜色
shOffsetColor: 指定颜色的偏移量，它的取值范围是[-255,255]。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK : 函数执行成功。
BTNST_INVALIDINDEX : 由 byColorIndex 指定状态索引值无效。
BTNST_BADPARAM : 根据偏移量生成的值超出RGB的颜色取值范围。
**CButtonST::SetAlign**
DWORD SetAlign(BYTE byAlign, BOOL bRepaint = TRUE)
注释：
调用此函数可以设定按钮上图标（或位图）于按钮上文字的相对位置。
参数：
byAlign: 位置参数，它的取值必须是以下枚举量之一。
取值注释
ST_ALIGN_HORIZ 图标/图片在左，文字在右
ST_ALIGN_VERT 图标/图片在上，文字在下
ST_ALIGN_HORIZ_RIGHT 图标/图片在右，文字在左
默认值为ST_ALIGN_HORIZ. 
bRepaint: 为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDALIGN: 位置参数无效。
**CButtonST::SetAlwaysTrack**
DWORD SetAlwaysTrack(BOOL bAlwaysTrack = TRUE)
注释：
此函数只对平面按钮有效。调用此函数可以设定按钮是否显示高亮颜色。
参数：
bAlwaysTrack: 值为TRUE时，无论前窗口是否为激活窗口，只要鼠标指针移动到按钮上面都会发按钮的显示状态发生变化。默认值为TRUE。
值为FALSE时，如果窗口未被激活，按钮的显示不会发生变化。
返回值：
BTNST_OK : 函数执行成功。
**CButtonST::SetBitmaps**
DWORD SetBitmaps(int nBitmapIn, COLORREF crTransColorIn, int nBitmapOut = NULL, COLORREF crTransColorOut = 0)
DWORD SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut = NULL, COLORREF crTransColorOut = 0)
注释：
调用此函数可以在按钮上显示指定的图片（Icon）。如果对同一个按钮重复调用了此函数的话，那么按钮只显示最后一次指定的图片。
参数：
nBitmapIn: 图片的ID值。当鼠标指针位于按钮之上时显示该图片。
crTransColorIn: 颜色值。ID为nBitmapIn的图片中与该颜色相同的区域将显示透明。
nBitmapOut: 图片的ID值。当鼠标指针不在按钮之上时显示该图片。
crTransColorOut: 颜色值。ID为nBitmapOut的图片中与该颜色相同的区域将显示透明。
hBitmapIn: 图片的句柄。当鼠标指针位于按钮之上时显示该图片。
crTransColorIn: 颜色值。由hBitmapIn指定的图片中与该颜色相同的区域将显示透明。
hBitmapOut: 图片的ID值。当鼠标指针不在按钮之上时显示该图片。
crTransColorOut: 颜色值。由hBitmapOut指定的图片中与该颜色相同的区域将显示透明。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDRESOURCE: 指定资源读取失败。
BTNST_FAILEDMASK: 颜色提取失败。
**CButtonST::SetBtnCursor**
DWORD SetBtnCursor(int nCursorId = NULL, BOOL bRepaint = TRUE)
注释：
这个函数可以让鼠标在移动到按钮上面的时候显示指定的鼠标指针。
参数：
nCursorId: 鼠标指针的ID值。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDRESOURCE: 读取指定资源失败。
**CButtonST::SetCheck**
DWORD SetCheck(int nCheck, BOOL bRepaint = TRUE)
注释：
当按钮被当作CheckBox使用的时候，用来设置按钮的状态。
参数：
nCheck : 1表示选中; 0表示没选中
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
**CButtonST::SetColor**
DWORD SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE)
注释：
调用此函数可以设置按钮在各种状态下显示指定的底色。
参数：
byColorIndex : 状态索引值。具体含义可以参见OffsetColor函数中的参数(byColorIndex)说明。
crColor : 颜色值。设置按钮指定状态的底色。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDINDEX: 无效的状态索引值。
**CButtonST::SetDefaultColors**
DWORD SetDefaultColors(BOOL bRepaint = TRUE)
注释：
调用此函数之后，程序将使用默认颜色来设定按钮在各个状态下的颜色。这些颜色值可能会因为Windows版本的不同而不同。
参数：
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
**CButtonST::SetFlat**
DWORD SetFlat(BOOL bFlat = TRUE, BOOL bRepaint = TRUE)
注释：
调用此函数来设置按钮的显示风格。
参数：
bFlat: 值为TRUE则显示为平面按钮; 值为FALSE则显示为标准的Windows按钮
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
**CButtonST::SetIcon**
DWORD SetIcon(int nIconIn, int nIconOut = NULL)
DWORD SetIcon(HICON hIconIn, HICON hIconOut = NULL)
注释：
调用此函数可以在按钮上显示指定的图标（Icon）。如果对同一个按钮重复调用了此函数的话，那么按钮只显示最后一次指定的图标。
参数：
nIconIn : 图标的ID值，当鼠标指针移动到按钮上面的时候显示该图标。如果该参数被赋值为 NULL ，则分配给这个按钮的所有图标资源将被释放。
hIconOut: 图标的ID值，当鼠标指针不在按钮上面的时候显示该图标。调用函数的时候可以忽略该参数。
hIconIn : 图标的句柄，当鼠标指针移动到按钮上面的时候显示该图标。如果该参数被赋值为 NULL ，则分配给这个按钮的所有图标资源将被释放。
hIconOut: 图标的句柄，当鼠标指针不在按钮上面的时候显示该图标。调用函数的时候可以忽略该参数。
返回值：
BTNST_OK: 表示函数执行成功。
BTNST_INVALIDRESOURCE: 表示函数执行失败，例如无法读取指定的图标资源。
**CButtonST::SetMenu**
DWORD SetMenu(UINT nMenu, HWND hParentWnd, BOOL bWinXPStyle = TRUE, UINT nToolbarID = NULL, CSize sizeToolbarIcon = CSize(16, 16), COLORREF crToolbarBk = RGB(255, 0, 255), BOOL bRepaint = TRUE)
DWORD SetMenu(UINT nMenu, HWND hParentWnd, BOOL bRepaint = TRUE)
注释：
要调用此函数必须与BCMenu类一起使用。函数将为按钮指派一个菜单，当点击按钮的时候会弹出所指派的菜单。此函数的调用与宏有关，如果在代码中声明了BTNST_USE_BCMENU 宏，则应该调用SetMenu函数的第一种形式，否则应该调用SetMenu的后一种形式。
参数：
nMenu : 菜单的ID值。
hParentWnd: 菜单所属的窗口句柄。菜单选项将触发该窗口内的时间。
bWinXPStyle: 值为TRUE则会以XP风格显示菜单, 值为FALSE则会以标准风格显示菜单。
nToolbarID: 工具条的ID值。程序会将工具条中与菜单项ID值相同图片加入到菜单中。
sizeToolbarIcon: 指定工具条中每一个图标的大小，它是一个CSize 类型的实例。注意请确保工具条中所有图标的大小一致。
crToolbarBk: 颜色值。工具条图片中与该颜色相同的区域将显示为透明。
bRepaint: 值为TRUE则马上重绘按钮。
返回值：
BTNST_OK: 函数执行成功。
BTNST_INVALIDRESOURCE: 无法读取指定资源。
**CButtonST::TooltipText**
void SetTooltipText(int nText, BOOL bActivate = TRUE)
void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE)
注释：
调用此函数可以为按钮设定提示信息。
参数：
nText: 字符串的ID值。
lpszText: 所要显示的字符串。
nActivate: 值为TRUE表示激活提示功能, 值为FALSE表示不激活提示功能。
**CButtonST::SetURL**
DWORD SetURL(LPCTSTR lpszURL = NULL)
注释：
调用此函数后，单击按钮将打开参数所指定的连接。
参数：
lpszURL: 包含链接信息的字符串。
返回值：
BTNST_OK: 函数执行成功。
摘自《VC知识库》
源地址：[http://blog.renren.com/GetEntry.do?id=742753411&owner=243139491](http://blog.renren.com/share/240815773/7750056620)
