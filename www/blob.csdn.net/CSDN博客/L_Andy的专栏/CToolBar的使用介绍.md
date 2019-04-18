# CToolBar的使用介绍 - L_Andy的专栏 - CSDN博客

2013年10月22日 09:52:52[卡哥](https://me.csdn.net/L_Andy)阅读数：1612
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



<1>

CToolBar控件是MFC提供的一个类，实现了一般窗口应用中的工具条。通常用于SDI或者MDI程序中，对话框程序中通常不使用工具条。

一、代码中的工具条

工具条是一个CToolBar类对象，通常作为成员对象嵌入程序的CMainFrame类中，也就是说嵌入主框架窗口中。因此，MFC生成框架窗口的时候同时生成工具条，销毁框架窗口的时候同时销毁工具条。下面是一个多文档 (MDI) 程序的代码片断，给出了工具条相关的代码：

class CMainFrame : public CMDIFrameWnd

{

   // ...

protected:  // control bar embedded members

   CToolBar    m_wndToolBar;

protected:

   //{{AFX_MSG(CMainFrame)

   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()

};

工具条的创建过程发生在CMainFrame::OnCreate阶段。MFC在创建框架窗口以后，在显示窗口之前调用这个OnCreate函数，AppWizard通常在这个函数中做如下的工作：

1、调用CToolBar对象的Create成员函数，来生成内嵌的CToolBarCtrl对象。

2、调用LoadToolBar函数加载工具条资源。

3、调用函数来启动工具条的停靠、浮动、提示等功能，具体详见后面“停靠和浮动”的内容。这些调用的代码是可选的，如果不需要这些功能，可以删除它们。

二、编辑工具条资源

通过AppWizard生成的工具条是基于MFC4.0中定义的RT_TOOLBAR类型的资源。工具条的按钮可以用工具条编辑器来修改，诸如添加、删除、排列按钮等操作。

要把一个工具条按钮和一个命令相关联，必须给按钮一个命令ID，例如ID_MYCOMMAND，可以在工具条按钮的属性页中添加这个命令，然后用ClassWizard生成命令的响应函数。

三、多个工具条

AppWizard仅仅提供了一个工具条，如果需要多个，则可以根据第一个工具条的代码创建多个自己的工具条。首先创建工具条资源，然后在框架窗口类中嵌入新的CToolBar对象，最后在OnCreate函数中生成它。

四、工具条按钮

通常工具条显示一些代表不同命令的按钮，工具条的按钮和菜单项类似，都会产生命令，而且通常情况下工具条按钮和菜单项的ID相同，两者的功能相同，用户可以在使用程序的时候，自愿选择。

五、停靠和浮动

MFC的工具条可以固定停靠在父窗口的一侧，或者通过拖拉停靠在父窗口的任何一侧，或者以小窗口的形式浮动在父窗口的上面。

1、允许停靠

要在一个框架窗口中停靠工具条，该窗口必须允许停靠，调用CFrameWnd::EnableDocking ，该函数通过一个DWORD参数给出框架窗口的那一侧允许停靠。准备好停靠的目标以后，工具条也要做同样的准备，调用CControlBar::EnableDocking 启动工具条的停靠功能，给出可以停靠的边。如果工具条允许停靠的边和框架窗口允许停靠的边没有相同的，工具条就会浮动在框架窗口的上面。因此如果想永久的把工具条做成浮动的，就把参数设置成0，然后调用CFrameWnd::FloatControlBar。

准备好上述工作以后，框架调用CFrameWnd::DockControlBar 开始启动工具条的停靠。可以在任何时候调用这个函数，但通常是在初始化阶段调用。

一个停靠的工具条离开框架窗口的边以后，就浮动的，也可以直接调用CFrameWnd::FloatControlBar 把工具条设置成浮动的。给出浮动的位置和样子。当用户把一个停靠的工具条拖拽离开框架窗口的边的时候，框架也调用这个函数，和DockControlBar函数一样，你也可以在初始化阶段调用它。

MFC对停靠工具条不提供某些功能的支持，例如自定义工具条。

从Visual C++ 4.0版本之后，开始提供对动态改变工具条尺寸的支持，一般来讲，工具条是一个水平的长条形，但是也可以改变它的形状，例如垂直的长条，或者多行。工具条在生成的时候，有一个属性：

CBRS_SIZE_DYNAMIC 或者 CBRS_SIZE_FIXED   。如果定义了前者，则工具条在浮动状态的时候，可以通过拖拽它的边改变它的形状。如果定义了后者，则不论工具条是停靠状态还是浮动状态，它的形状是不变的。

可以通过函数CToolBar::GetButtonStyle获取工具条按钮的状态和属性，属性决定了按钮如何响应用户的单击，状态决定了按钮的换行。比如说对于一个FIXED属性的工具条，想让它的按钮分两行显示，从第三个按钮（包括分割条）开始，可以这么做：

在框架窗口的OnCreate函数中，生成工具条以后，加入下列代码：

 //获取第三个按钮（或者分割条）的属性

 UINT nStyle = m_wndToolBar.GetButtonStyle( 3 );

 //将它设置成换行

 nStyle |= TBBS_WRAPPED;

 m_wndToolBar.SetButtonStyle( 3, nStyle );

 // 将工具条属性设置成 fixed

 m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |

       CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

六、工具条提示

MFC的工具条还可以提供提示功能，就是当用户把鼠标移动在工具条按钮上面的时候，弹出一个小的窗口，给出描述该按钮用途的一小段文字。用AppWizard生成的工具条，自动具备这个提示功能。

要在程序中激活提示功能，需要两个条件：

1、在CToolBar::Create函数中明确给出CBRS_TOOLTIPS 风格，或者用SetBarStyle函数设置这个风格。

2、在资源里面添加提示字符串，用/n隔开，字符串的ID和该按钮的相同。或者在编辑工具条按钮的时候，打开它们的属性栏，在Properties 一栏输入提示字符串。如果一个工具条包含子控件，只要控件的ID不是-1，而且字符串资源里面有对应的字符串，子控件也可以有提示功能。

这里要说一下 CBRS_FLYBY 属性，一般来说当工具条的按钮被按下的时候，状态条就会显示关于该按钮的一个说明文字，如果定义了这个属性，则当鼠标位于按钮之上的时候，就会显示这个提示，不用按下按钮。

七、CToolBar类和CToolBarCtrl类的关系

一般通过CToolBar类管理工具条，从MFC4.0开始，CToolBar类就封装了Windows提供的工具条控件，完全可以用该类的成员函数操作工具条，或者获取一个内嵌的CToolBarCtrl对象的引用，调用它的函数。

要访问CToolBar对象中的内嵌工具条控件，可以调用CToolBar::GetToolBarCtrl函数。返回一个CToolBarCtrl对象的引用。然后可以用这个引用对象调用CToolBarCtrl类的成员函数。

                                                              <2>

一、首先来看工具条类的派生关系

工具条类的派生关系如下：

//---------------------------

 CObject

          |_____CCmdTarget

                   |______CWnd

                            |____CControlBar

                                    |_____CToolBar

//----------------------------

由于这样的继承关系，工具条类具有下列的特点和功能：

（1）从CObject继承来的功能

   a. 串行化支持

   b. 运行时刻的类信息

   c. 对象诊断输出

   d. 兼容 collection 类

如果在继承类的实现和声明中，使用了一些预定义宏，则派生类就可以实现CObject类的许多优点，

包括：

   a.一级宏 DECLARE_DYNAMIC和IMPLEMENT_DYNAMIC。使用该宏，将允许在运行时刻访问类的名字和它在继承表中的位置，也就意味着可以用来进行诊断。 

   b.二级宏 DECLARE_SERIAL 和 IMPLEMENT_SERIAL。该宏涵盖了一级宏的所有功能，并允许对象可以通过archive进行串行化。

（2）从CCmdTarget继承来的功能

CCmdTarget是MFC中所有具备消息循环的类的基类，主要包括CView, CWinApp, CDocument, CWnd, 和CFrameWnd。

除了消息循环，该类还提供了：

   a. 漏斗鼠标

   b. Dispatch映射，用于ole自动化中的IDispatch 函数，为VB调用提供了接口

（3）从CWnd类继承的功能

 CWnd类提供可所有MFC库中窗口类的基本功能，CWnd类与windows下的窗口是有所区别的，但是两者有紧密的联系：CWnd类的构造函数和析构函数生成和销毁一个CWnd对象，窗口对象是CWnd类的Create函数生成的，在它的虚析构函数中销毁窗口对象。 DestroyWindow 函数也可以销毁窗口对象，但是不是CWnd对象。

 CWnd类以及它的消息映射机制隐藏了WndProc函数，所有Windows消息被自动通过消息映射表转发到相应的OnMessage成员函数。应该在派生类中重载OnMessage成员函数来处理某个特定的消息。这意味着工具条可以有自己的消息映射表。

 除此以外，CWnd还提供了很多功能，比如绘图，输出文字，设置菜单等等，但是某些功能在工具条中不能用，后面将通过剖析其源代码进行分析。

（4）从CControlBar继承的功能

 CControlBar 是控制条类的基类，包括 CStatusBar, CToolBar, CDialogBar, CReBar 和COleResizeBar. 一个控制条窗口通常位于一个框架窗口的左侧或者右侧。它可以包含一些子项，有些是

窗口控件，比如列表框和编辑框，负责产生和响应Windows 消息，有些是非窗口控件，比如状态条面板和位图由应用程序和框架负责管理。

 控制条窗口通常是一个框架窗口的子窗口，和客户区视窗口或者MDI客户区处于同一个地位。因此控制条

使用父窗口的客户区矩形信息来定位自己。而且负责把剩余的未使用的客户区大小通知父窗口。

二、使用CToolBar类

 CToolbar对象具有一行位图按钮和可选的分割条，这些按钮的行为和下压按钮，check-box按钮和单选按钮一样。CToolBar对象通常嵌入 CFrameWnd 或者 CMDIFrameWnd对象里面。

 CToolBar类的成员函数GetToolBarCtrl允许你使用Windows通用控件支持的特性来设计工具条或者添加额外的功能。CToolBar的成员函数已经提供了Windows通用控件的大多数功能，如果你调用GetToolBarCtrl，会返回一个CToolBarCtrl对象的引用，利用它可以实现 Windows 95工具条的许多特性。

Visual C++ 提供了两种生成工具条的方法：

要生成一个工具条，应该遵循下面的步骤：

1、生成一个工具条资源

2、构造工具条对象

3、调用 Create (或者 CreateEx) 函数生成一个工具条窗口，并把它关联到 CToolBar对象。

4、调用 LoadToolBar 函数加载工具条的资源。

或则采用下面的步骤：

1、构造一个工具条对象。

2、调用 Create (或者 CreateEx) 函数生成一个工具条窗口，并把它关联到 CToolBar对象。

3、调用 LoadBitmap 函数来加载包含工具条按钮图片的位图。

4、调用 SetButtons 函数来设置工具条按钮的样式并且把每个按钮与位图中的某个图片相关联。

一个工具条中的所有图片都来自于一个位图，该位图应该至少为每个按钮提供一个图片，所有的图片必需是相同的尺寸，缺省定义是 16 象素宽，15 象素高。在位图中，图片必需一个挨着一个。

 SetButtons 函数使用一个指向 控制ID的数组指针和一个给出id数目的整数。该函数把工具条每个按钮的id设置成数组中对应的值，并且为每个按钮分配一个图片。如果数组中包含ID_SEPARATOR的项,则不会给它关联图片。

 通常图片的顺序就是他们在屏幕上画的顺序，但是你可以通过函数SetButtonInfo来改变这个顺序。

 一个工具条的所有按钮都是相同的尺寸。缺省定义是 24 x 22 象素，迎合了 Windows 软件设计接口规范的要求。在图片和按钮之外的其它多余空间是用来生成一个边框。

 每个按钮有一个图片。不同的按钮状态和属性下 (压下，抬起，禁用，激活，以及中间态)的图片都从那个图片产生。虽然图片可以用任何颜色，但是带有灰色阴影的黑色图片效果最好。

 工具条上的按钮初始状态都是下压按钮。然而，它们可以改成check-box 按钮或者单选按钮。Check-box 按钮具有三个状态：选中，清除和中间态，单选按钮只有两个状态：选中和清除。

 要设置一个按钮或者分隔条的属性，可以通过函数 GetButtonStyle获得属性，然后调用 SetButtonStyle进行设置，而不用SetButtons。 SetButtonStyle函数在运行时刻改变按钮的属性十分好用。

 要在按钮上显示文字，可以通过函数 GetButtonText获取文字，调用 SetButtonText设置文字。

  要生成一个 check-box 按钮，只需加上 TBBS_CHECKBOX属性，或者在 ON_UPDATE_COMMAND_UI 处理函数中使用CCmdUI对象的SetCheck函数设置。只要调用SetCheck函数就会自动把下压按钮改变成为check-box按钮。

 要生成一个单选按钮，可以在ON_UPDATE_COMMAND_UI处理函数中，调用CCmdUI对象的SetRadio函数。为了提供一组单选按钮的排它功能，你必须为该组的所有按钮都生成ON_UPDATE_COMMAND_UI处理函数。

三、关于通用控制条的认识（来自技术备忘录31:控制条）

本文所论及的内容包括MFC中的控制条类:

 CControlBar, CStatusBar, CToolBar, CDialogBar, 和 CDockBar.

（一）CControlBar

 ControlBar 派生自 CWnd，通常定位于框架窗口的顶端或者底部。它可以包含窗口类的子项，比如 CDialogBar，或者非窗口类的子项，比如CToolBar和 CStatusBar。

 控制条支持的样式包括：

 CBRS_TOP   (缺省的)位于顶部

 CBRS_BOTTOM  放置在底部

 CBRS_NOALIGN 在父窗口尺寸变化的时候，不要重新定位控制条

 从 CControlBar派生的类提供了更加有趣的功能：

 （a） 状态条CStatusBar ，它的子项是包含文字的面板

 （b） 工具条CToolBar ，对于工具条来说，子项是排列成一行的位图按钮

 （c)  对话框条CDialogBar 象一个框架一样的工具条，包括标准windows控件 (从一个对话框模板资源生成)

 （d)  CDockBar,可以认为它是为其它控制条类提供对接区域的一个区域。该类的成员函数和变量在将来可能还要变化。

  记住：所有的控制条对象必需是某个框架窗口的子窗口。通常它们被作为框架客户区(例如，一个MDI客户区或者一个视窗口)的同级窗口。对于一个控制条来说，它的子窗口ID是非常重要的。仅仅当它的ID的值位于AFX_IDW_CONTROLBAR_FIRST和AFX_IDW_CONTROLBAR_LAST之间时，控制条的缺省布局才能起作用。

#define AFX_IDW_CONTROLBAR_FIRST        0xE800

#define AFX_IDW_CONTROLBAR_LAST         0xE8FF

注意，虽然一共有256个控制条ID，前面的32个是做特殊用途的，它们直接被打印预览结构支持。

 CControlBar类提供了

 A.在框架窗口里面定位工具条的标准实现。

 B.分配控制条项队列

 C.支持派生类的实现

 C++ 控制条对象通常作为成员嵌入一个CFrameWnd派生类对象中。而且当父窗口销毁的时候，它也被清除。如果你想在堆中分配一个控制条对象，你可以将它的 m_bAutoDestruct成员设置成 TRUE，从而当HWND销毁的时候允许它 “delete this”。

 重要提示：如果你派生了自己的控制条对象，则必须重载Create函数，在其中设置 m_dwStyle数据成员，例如:

// CMyControlBar is derived from CControlBar

BOOL CMyControlBar::Create( CWnd* pParentWnd, DWORD dwStyle, UINT nID )

{

   m_dwStyle = dwStyle;

   .

   .

   .

}

（二）控制条的布局原理

 控制条的布局原理非常简单，框架窗口按照 Z-顺序发送消息 WM_SIZEPARENT给所有控制条子窗口，并跟随这个消息传递一个父窗口客户区矩形的指针。控制条使用这个矩形信息来定位自己，并且把父窗口的客户区尺寸减小，剩下的是正常的客户区域，用于定位主要的客户区窗口，通常是一个MDI客户区，视或者拆分窗口。

 详细情况可以参考 CWnd::RepositionBars 和 CFrameWnd::RecalcLayout 两个函数。

 MFC 私有窗口消息，包括WM_SIZEPARENT,都在技术备忘录24中说明。

（三）工具条

工具条支持两种按钮: 下压按钮和check box按钮。通过使用check box按钮组加上适当的ON_UPDATE_COMMAND_UI处理，可以得到单选按钮组的功能。

工具条中的所有按钮的图片都取自同一个位图。每个按钮尺寸相同（缺省尺寸是24x22象素大小），每个按钮的图片尺寸相同而且相连， 图片的缺省大小是16x15象素，所以如果一个工具条具有10个按钮，则需要位图的尺寸是160象素宽，15 象素高。

工具条按钮的所有状态的图片都是通过一定的算法从那一个图片中转换得到。因此，虽然理论上按钮图片可以使用任何颜色，但是对于这个算法来说，如果原始图片使用灰色阴影效果比较好。

项              Windows 颜色           缺省RGB值 

工具条背景      COLOR_BTNFACE          RGB(192, 192, 192) 

工具条左/上边框 COLOR_BTNHIGHLIGHT     RGB(255, 255, 255) 

工具条右/下边框 COLOR_BTNSHADOW        RGB(128, 128, 128)

对工具条的 CCmdUI 支持：

工具条按钮的更新是通过空闲时刻调用 ON_UPDATE_COMMAND_UI 完成的。通过这个函数可以:

1、允许/禁止工具条按钮。

2、设置按钮为check状态。注意调用SetCheck函数会把按钮变成check box 按钮。

3、SetRadio: 设置类似单选按钮的功能。

工具条按钮会象正常的按钮一样发送WM_COMMAND消息，通常被一个ON_COMMAND函数处理。

工具条按钮一共有四种样式（代表四种状态）：

TBBS_CHECKED

TBBS_INDETERMINATE

TBBS_DISABLED

TBBS_PRESSED

正式的按钮状态定义是六种，它们是：

Up = 0

Mouse Down = TBBS_PRESSED (| any other style)

Disabled = TBBS_DISABLED

Down = TBBS_CHECKED

Down Disabled = TBBS_CHECKED | TBBS_DISABLED

Indeterminate = TBBS_INDETERMINATE

                                                                    <3>

下面将通过研究工具条的源代码探讨几个问题。

1、工具条类的数据

CToolBar类有下列数据成员，均为保护成员。并在构造函数中设置它们的缺省值如下：

 CMapStringToPtr* m_pStringMap = NULL;  // used as CMapStringToUInt

 HRSRC m_hRsrcImageWell = NULL;  // 用于加载资源图片

 HINSTANCE m_hInstImageWell= NULL;  // 加载图片的进程句柄

 HBITMAP m_hbmImageWell = NULL;  // 保存已经加载的工具条位图

 BOOL m_bDelayedButtonLayout = TRUE;  // 用于控制工具条按钮布局

//

// 注：

//

// 在工具条析构的时候会自动销毁 m_hbmImageWell和 m_pStringMap两个对象。

//

 CSize m_sizeImage;  //按钮图片的缺省大小

 m_sizeImage.cx = 16;

 m_sizeImage.cy = 15;

 CSize m_sizeButton; //按钮的缺省尺寸

 m_sizeButton.cx = 23;

 m_sizeButton.cy = 22;

//下面的数据定义位于CControlBar类中，是public,可以改变它们的值，获得边界尺寸怪异的工具条

//方法是：从CToolBar 派生一个新的工具条类，然后在构造函数里面改变这几个数据的值。

//

// int m_cxLeftBorder, m_cxRightBorder;

// int m_cyTopBorder, m_cyBottomBorder;

//

//这里仅改变两个值 ，为了更易于加把手

 m_cyTopBorder = 3;

 m_cyBottomBorder = 3;

}

2、 关于Create和CreateEx

工具条的缺省构造函数是CToolBar(),因此声明一个对象以后要生成它。这两种生成方式有什么不同呢？

看它们的定义：

BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR);

BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT, 

 DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP, CRect rcBorders = CRect(0, 0, 0, 0),

 UINT nID = AFX_IDW_TOOLBAR);

实际上 Create函数最终也是调用CreateEx:

BOOL CToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)

{

 return CreateEx(pParentWnd, 0, dwStyle,

  CRect(m_cxLeftBorder, m_cyTopBorder, m_cxRightBorder, m_cyBottomBorder), nID);

}

因此，我们看到如果用Create函数生成工具条，仅仅是风格稍有不同，后者采用了平滑按钮，不画底边。：

#define TBSTYLE_BUTTON          0x0000

#define TBSTYLE_FLAT            0x0800

#define CBRS_TOP            (CBRS_ALIGN_TOP|CBRS_BORDER_BOTTOM)

类似的按钮样式还包括：

#define TBSTYLE_BUTTON          0x0000

#define TBSTYLE_SEP             0x0001

#define TBSTYLE_CHECK           0x0002

#define TBSTYLE_GROUP           0x0004

#define TBSTYLE_CHECKGROUP      (TBSTYLE_GROUP | TBSTYLE_CHECK)

#if (_WIN32_IE >= 0x0300)

#define TBSTYLE_DROPDOWN        0x0008

#endif

#if (_WIN32_IE >= 0x0400)

#define TBSTYLE_AUTOSIZE        0x0010 // automatically calculate the cx of the button

#define TBSTYLE_NOPREFIX        0x0020 // if this button should not have accel prefix

#endif

#define TBSTYLE_TOOLTIPS        0x0100

#define TBSTYLE_WRAPABLE        0x0200

#define TBSTYLE_ALTDRAG         0x0400

#if (_WIN32_IE >= 0x0300)

#define TBSTYLE_FLAT            0x0800

#define TBSTYLE_LIST            0x1000

#define TBSTYLE_CUSTOMERASE     0x2000

#endif

#if (_WIN32_IE >= 0x0400)

#define TBSTYLE_REGISTERDROP    0x4000

#define TBSTYLE_TRANSPARENT     0x8000

#define TBSTYLE_EX_DRAWDDARROWS 0x00000001

#endif

另外一点要注意的是，从CreateEx函数中可以得知，工具条窗口的窗口类名字是：

"ToolbarWindow32"

3、关于改变按钮的尺寸，改变后所有按钮还是同一个尺寸。

工具条类的SetSizes函数可以用来改变按钮的尺寸，但是要注意：

 // button must be big enough to hold image

 //   + 7 pixels on x

 //   + 6 pixels on y

 ASSERT(sizeButton.cx >= sizeImage.cx + 7);

 ASSERT(sizeButton.cy >= sizeImage.cy + 6);

4、关于改变工具条的高度

工具条类的SetHeight函数可以用来改变整个工具条的高度，但是要注意的是，调用这个函数以后，工具条按钮自动垂直居中，

如果前面设置了m_cyTopBorder 和 m_cyBottomBorder，它们的值将会改变。

5、关于LoadToolBar

通常，工具条对象Create完成以后，就会调用LoadToolBar函数加载工具条，例如：

 if (!m_wndToolBar.Create(this) ||

  !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))

 {

  TRACE0("Failed to create toolbar/n");

  return -1;  

 }

这个加载过程有些细节很有意思，值得看看：

首先是一个结构，这个结构位于编译后的工具条资源的开头

struct CToolBarData

{

 WORD wVersion; //版本号 ==1

 WORD wWidth;

 WORD wHeight;

 WORD wItemCount; //给出后面有几项

 //WORD aItems[wItemCount] 

 WORD* items()

  { return (WORD*)(this+1); }

};

注意，该结构没有使用可变长度数组，而是添加了一个函数，直接返回指向结构数据后面一个字节的指针，这种方法值得借鉴。

BOOL CToolBar::LoadToolBar(LPCTSTR lpszResourceName)

{

 ASSERT_VALID(this);

 ASSERT(lpszResourceName != NULL);

 // 首先定位位图资源

 HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);

 HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);

 if (hRsrc == NULL) return FALSE;

 //加载资源数据

 HGLOBAL hGlobal = LoadResource(hInst, hRsrc);

 if (hGlobal == NULL) return FALSE;

 //锁定资源，并强制转化成结构指针

 CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);

 if (pData == NULL) return FALSE;

 ASSERT(pData->wVersion == 1);

 //这里，它不直接使用pData->items()，而是new了一块内存，为什么？

 //另外，通过函数SetButtons的代码，我们可以知道这一串WORD数据

 //给出的是每个按钮的ID

 UINT* pItems = new UINT[pData->wItemCount];

 for (int i = 0; i < pData->wItemCount; i++)

  pItems[i] = pData->items()[i];

 BOOL bResult = SetButtons(pItems, pData->wItemCount);

 delete[] pItems;

 //设置按钮的尺寸

 if (bResult)

 {

  // set new sizes of the buttons

  CSize sizeImage(pData->wWidth, pData->wHeight);

  //注意这里，它都增加了7个象素

  CSize sizeButton(pData->wWidth + 7, pData->wHeight + 7);

  SetSizes(sizeButton, sizeImage);

  //这个时候才加载位图资源，可见位图资源和RT_TOOLBAR资源没有在一起

  bResult = LoadBitmap(lpszResourceName);

 }

 UnlockResource(hGlobal);

 FreeResource(hGlobal);

 return bResult;

}

6、关于函数AfxLoadSysColorBitmap

这个函数是工具条类使用的，是一个未公开的API函数，定义如下：

HBITMAP AFXAPI AfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc, BOOL bMono = FALSE);

LoadBitmap函数里面使用这个函数加载工具条按钮的位图资源，然后调用AddReplaceBitmap设置资源句柄。在这个函数中

使用了位图资源前面的16个系统颜色的调色板对位图资源中的颜色做了一下处理，因此位图资源不能超过256色，必须有调色板。

如果你已经有了自己的真彩色HBITMAP资源，直接可以通过函数SetBitmap设置也可以。但是要注意，你必须保证程序

运行期间，你的HBITMAP资源一直有效。因为在函数SetBitmap中，做了如下的设置：

 m_hInstImageWell = NULL;

 m_hRsrcImageWell = NULL;

AddReplaceBitmap函数在添加新的位图资源的同时，调用函数AfxDeleteObject删除了原来的位图资源。

7、关于SetButtonStyle

工具条按钮定义了若干类型，分别是：

TBBS_BUTTON   标准下压按钮（缺省情况）

TBBS_SEPARATOR   分隔条

TBBS_CHECKBOX 自动复选框按钮

TBBS_GROUP    开始一组按钮

TBBS_CHECKGROUP   表示一组自动复选框按钮的首个按钮

8、细谈SetButtonInfo函数及其用途。

SetButtonInfo用于设置某个按钮，它的接口定义如下：

下面是它的几个接口函数说明：

void CToolBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage)

注意CToolBarCtrl类也有这个函数，但是它们的定义是完全不同的，实际上，CToolBar::SetButtonInfo的代码如下：

void CToolBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage)

{

 ASSERT_VALID(this);

 TBBUTTON button;

 _GetButton(nIndex, &button);

 TBBUTTON save;

 memcpy(&save, &button, sizeof(save));

 button.idCommand = nID;

 button.iBitmap = iImage;

 button.fsStyle = (BYTE)LOWORD(nStyle);

 button.fsState = (BYTE)HIWORD(nStyle);

 if (memcmp(&save, &button, sizeof(save)) != 0)

 {

  _SetButton(nIndex, &button);

  m_bDelayedButtonLayout = TRUE;

 }

}

msdn介绍说这个函数的功能是设置工具条按钮的 ID, style, 以及图片的编号。因此，通常这个函数有三个方面的用途：

（1）用于在工具条中添加其他控件

需要注意的是，当给出的nStyle是TBBS_SEPARATOR的时候，iImage是指它的宽度，而不是图片的编号。这个功能在向工具条中添加其它控件（比如编辑控件、组合框控件）的时候非常有用，它可以改变按钮的位置，为添加的控件留出空间。这个在后面将谈到。

（2）用于改变工具条中按钮的样式

例如：

    m_wndToolBar.SetButtonInfo(i, ID_FILE_SAVE, TBBS_CHECKBOX, 2);

该代码将工具条按钮ID_FILE_SAVE设置成CHECKBOX按钮，即单击该按钮显示按下，再次单击显示抬起。

例如：

    int i = m_wndToolBar.CommandToIndex( ID_BUTTON1);

    m_wndToolBar.SetButtonInfo(i, ID_BUTTON1, TBBS_GROUP|TBBS_CHECKBOX      , 9);

    m_wndToolBar.SetButtonInfo(i+1, ID_BUTTON2, TBBS_GROUP |TBBS_CHECKBOX  , 10);

    m_wndToolBar.SetButtonInfo(i+2, ID_BUTTON3, TBBS_GROUP |TBBS_CHECKBOX  , 11);

该代码设置三个相邻的按钮为互斥按钮，即同一时刻只能有一个按钮按下。

TBBS_GROUP |TBBS_CHECKBOX  等价于  TBBS_CHECKGROUP

例如：

    int i = m_wndToolBar.CommandToIndex( ID_BUTTON1);

    m_wndToolBar.SetButtonInfo(i, ID_BUTTON1, TBBS_SEPARATOR  , 0);

上述代码将一个按钮设置成分隔条，并设置它的宽度为0，这可以用于动态隐藏工具条按钮

需要的时候，还可以把它重新显示回来：

    int i = m_wndToolBar.CommandToIndex( ID_BUTTON1);

    m_wndToolBar.SetButtonInfo(i, ID_BUTTON1, TBBS_BUTTON  , 9);

当然，nStyle参数不仅仅可以用msdn上面提到的几种，比如CToolBarCtrl::SetButtonInfo就有更为强大的功能，这个以后会谈到。

（3）改变工具条按钮的图片。有的时候想在程序运行过程中，根据某些条件动态改变按钮的图片，

就可以用这个函数。方法如下：

(1) 在资源里面添加一个位图文件IDB_BITMAP1,注意它的高度和工具条位图的高度保持一致，然后里面可以有几个按钮图片。

(2) 工具条生成以后，把这些图片添加到工具条的图片列表：

 CToolBarCtrl &toolctrl = m_wndToolBar.GetToolBarCtrl();

 int TotalImage =toolctrl.AddBitmap(N, IDB_BITMAP1);  //N给出位图中图片的个数

    这个时候TotalImage里面就保存了工具条里面已经保存的按钮图片的个数，它比实际按钮个数要多，你可以

把这个值保存在CMainFrame类里面，供以后使用

(3) 在需要更换工具条按钮图片的时候，这样写：

 int index = m_wndToolBar.CommandToIndex(ID_FILE_SAVE);

 m_wndToolBar.SetButtonInfo(index, ID_FILE_SAVE, TBBS_BUTTON, TotalImage-1);

则将ID_FILE_SAVE按钮的图片设置成图片列表中的最后一个。

9、CToolBar 类的public函数说明

 void SetSizes(SIZE sizeButton, SIZE sizeImage); //设置按钮尺寸，图片尺寸

 void SetHeight(int cyHeight);   //设置工具条的高度

 BOOL LoadToolBar(LPCTSTR lpszResourceName); //加载工具条

 BOOL LoadToolBar(UINT nIDResource);

 BOOL LoadBitmap(LPCTSTR lpszResourceName); //加载位图，这两个函数基本不用了

 BOOL LoadBitmap(UINT nIDResource);

 //设置工具条位图，可以用来改变工具条按钮的位图

 BOOL SetBitmap(HBITMAP hbmImageWell);  

 //如果没有使用LoadToolBar,而使用LoadBitmap，则需要用这个函数设置按钮

 BOOL SetButtons(const UINT* lpIDArray, int nIDCount); 

 //根据ID得到按钮的位置编号

 int CommandToIndex(UINT nIDFind) const; 

 //根据按钮的位置得到它的ID

 UINT GetItemID(int nIndex) const;

 //返回指定按钮的矩形区域坐标，单位是象素，相对于工具条左上角

 virtual void GetItemRect(int nIndex, LPRECT lpRect) const;

 //获取指定按钮的属性

 UINT GetButtonStyle(int nIndex) const;

 //设置指定按钮的属性

 void SetButtonStyle(int nIndex, UINT nStyle);

 //获取指定按钮的信息，包括ID，类型，相关联的图片编号

 void GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const;

 //设置指定按钮的信息

 void SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage);

 //设置指定按钮的文字

 BOOL SetButtonText(int nIndex, LPCTSTR lpszText);

 //获取指定按钮的文字

 CString GetButtonText(int nIndex) const;

 void GetButtonText(int nIndex, CString& rString) const;

 //获取工具条内的CToolBarCtrl

 CToolBarCtrl& GetToolBarCtrl() const;

 //设置父窗口。由于通常在生成工具条的同时指定了父窗口，因此这个函数很少用到。

 void SetOwner(CWnd* pOwnerWnd);

 //用指定位图替换工具条已有位图，如果原来工具条没有设置位图，则添加这个位图。

 BOOL AddReplaceBitmap(HBITMAP hbmImageWell);

