# 孙鑫-MFC笔记十--修改应用程序外观 - 工作笔记 - CSDN博客





2012年05月15日 00:05:25[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6122










## 修改应用程序的外观

窗口创建之前修改：

要改变一个框架窗口的外观和大小，我们应该在CMainFrame这个类当中的PreCreateWindow函数当中完成。

PreCreateWindow这个函数有一个参数cs，这个参数的类型为CREATESTRUCT这个结构体。



如果要想改变窗口的外观和大小，我们只需要去修改CREATESTRUCT这个结构体当中，相应的成员变量值。

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)

{

if( !CFrameWnd::PreCreateWindow(cs) )

return FALSE;

// TODO: Modify the Window class or styles here by modifying

// the CREATESTRUCT cs

cs.cx=300;//高度设为300

cs.cy=200;//宽度设为200



cs.style=WS_OVERLAPPEDWINDOW;//改变窗口标题的必要步骤(MSDN查阅window styles)

cs.lpszName="比他帅";//改变窗口标题

return TRUE;

}



窗口创建之后修改：


用SetWindowLong(设置窗口类型)在窗口创建之后，改变窗口的外观

在窗口创建之后，改变外观。可以在CMainFrame这个类当中的OnCreate去编写代码。



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)

{

if (CFrameWnd::OnCreate(lpCreateStruct) == -1)

return -1;


if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP

| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||

!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))

{

TRACE0("Failed to create toolbar\n");

return -1; // fail to create

}

if (!m_wndStatusBar.Create(this) ||

!m_wndStatusBar.SetIndicators(indicators,

sizeof(indicators)/sizeof(UINT)))

{

TRACE0("Failed to create status bar\n");

return -1; // fail to create

}

// TODO: Delete these three lines if you don't want the toolbar to

// be dockable

m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

EnableDocking(CBRS_ALIGN_ANY);

DockControlBar(&m_wndToolBar);



//获取原有窗口的类型，在这样的基础上，进行修改

SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) & ~WS_MAXIMIZEBOX);

//GetWindowLong获取指定窗口信息，& ~WS_MAXIMIZEBOX取反与操作，作用是去掉WS_MAXIMIZEBOX

//WS_MAXIMIZEBOX最大化框变灰.(MSDN查阅window styles)

return 0;

}



=======================================================



## 修改窗口的图标，光标和背景

窗口创建之前：



对于窗口的图标，光标和背景，是在设计窗口类的时候指定的，窗口类的设计和注册，是由MFC底层代码自

动帮助我们完成的，我们不能也不应该修改MFC的底层代码。



那么要如何才能改变窗口的图标，光标和背景呢？

虽然不能修改MFC的底层代码，但是我们可以编写自己的窗口类，进行注册，让随后的窗口按照我们自己编

写的窗口类去创建。



下面可以在CMainFrame这个类当中的PreCreateWindow函数当中，编写自己的窗口类。

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)

{

if( !CFrameWnd::PreCreateWindow(cs) )

return FALSE;

// TODO: Modify the Window class or styles here by modifying

// the CREATESTRUCT cs

WNDCLASS wndcls;

wndcls.cbClsExtra=0;//类额外的内存，我们不需要

wndcls.cbWndExtra=0;//窗口额外的内存，我们也不需要

wndcls.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);//背景,获取一个黑色的背景画刷

wndcls.hCursor=LoadCursor(NULL,IDC_HELP);//光标

wndcls.hIcon=LoadIcon(NULL,IDI_ERROR);//图标


wndcls.hInstance=AfxGetInstanceHandle();

//全局函数AfxGetInstanceHandle获取当前应用程序的句柄

wndcls.lpfnWndProc=::DefWindowProc;

//窗口过程,这里我们不想改变任何东西,所以用了缺省的DefWindowProc,这里要用全局的api函数,以免混淆

wndcls.lpszClassName="sunxin.org";//类的名字

wndcls.lpszMenuName=NULL;//菜单的名字

wndcls.style=CS_HREDRAW | CS_VREDRAW;//窗口类的类型，水平重画，垂直重画

RegisterClass(&wndcls);//注册窗口类

cs.lpszClass="sunxin.org";//启用自己设计的窗口类


return TRUE;

}



编译后，发现光标和背景没有改变(框架窗口CMainFrame只能改变图标)。

要想改变他们必须在view类的PreCreateWindow函数中改变。

BOOL CStyleView::PreCreateWindow(CREATESTRUCT& cs)

{

// TODO: Modify the Window class or styles here by modifying

// the CREATESTRUCT cs

cs.lpszClass="sunxin.org";//启用自己设计的窗口类

return CView::PreCreateWindow(cs);

}

综上，在MFC程序中，如果想要修改应用程序的图标，则应该在框架类中修改，因为框架类才有标题栏，所以才能修改位于该标题栏上的图标；如果想要修改应用程序的背景和光标，应该在视类窗口中改变。



-----------------------------------------------------------------



我们发现只修改那么点东西，就要重写窗口类，挺麻烦的。

为了方便我们，MFC就提供了全局函数AfxRegisterWndClass用来直接修改它们。

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)

{

if( !CFrameWnd::PreCreateWindow(cs) )

return FALSE;

// TODO: Modify the Window class or styles here by modifying

// the CREATESTRUCT cs

cs.lpszClass=AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,0,0,

LoadIcon(NULL,IDI_WARNING));

//4个参数分别为窗口类的类型，光标，背景，图标

return TRUE;

}



BOOL CStyleView::PreCreateWindow(CREATESTRUCT& cs)

{

// TODO: Modify the Window class or styles here by modifying

// the CREATESTRUCT cs

cs.lpszClass=AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,

LoadCursor(NULL,IDC_HELP),(HBRUSH)GetStockObject(BLACK_BRUSH),0);

//4个参数分别为窗口类的类型，光标，背景，图标

return CView::PreCreateWindow(cs);

}



----------------------------------------------------------------

窗口创建之后修改：

在窗口创建之后(CMainFrame里的OnCreate函数)，修改它的图标



利用全局API函数SetClassLong，它可以改变WNDCLASSEX成员变量的值

DWORD SetClassLong(

HWND hWnd, //要改变的窗口句柄

int nIndex, //索引值(要改变的光标,图标......)

LONG dwNewLong //所设置的新的值

); 



在CMainFrame里的OnCreate函数

SetClassLong(m_hWnd,GCL_HICON,(LONG)LoadIcon(NULL,IDI_ERROR));



------------------------------------------



在窗口创建之后(在view类增加消息处理WM_CREATE),修改光标和背景

int CStyleView::OnCreate(LPCREATESTRUCT lpCreateStruct)

{

if (CView::OnCreate(lpCreateStruct) == -1)

return -1;


// TODO: Add your specialized creation code here

SetClassLong(m_hWnd,GCL_HBRBACKGROUND,(LONG)GetStockObject(BLACK_BRUSH));//修改背景

SetClassLong(m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL,IDC_HELP));//修改光标

return 0;

}



============================================



## 下面完成一个不断变化的图标





就是运用预先准备好的图标，然后每隔一段时间，显示一副新的图标。

让这几个图标不断来回循环的显示，这样就形成了动画的效果。



这个功能，我们利用定时器，和函数SetClassLong完成。

我们每隔一秒钟，调用一次SetClassLong，SetClassLong可以在窗口创建之后，去改变窗口的图标。

这样每隔一秒钟显示下一副图标，形成动画的效果。





1.先准备3副图标，把这3副图标放在程序(不是vc而是你写的程序)的res里面。

2.在VC里面选择"插入"->"资源"。之后找到那3副图标，全部导入。



3.定义一个图标句柄数组，用来存放3副图标的句柄。

在CMainFrame增加成员变量。变量类型HICON，名称m_hIcons[3]，权限为private



4.在CMainFrame的OnCreate上，加载这3个图标



 m_hIcons[0]=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1));

 m_hIcons[1]=LoadIcon(theApp.m_hInstance,MAKEINTRESOURCE(IDI_ICON2));//利用全局对象获取当前实例句柄

 m_hIcons[2]=LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDI_ICON3));//利用全局函数获取当前应用程序对象指针，然后获取内部数据成员



SetTimer(1,1000,NULL);//设置定时器



//AfxGetInstanceHandle()

//因为要用我们自己的图标，所以利用全局函数AfxGetInstanceHandle设置为程序当前的实例句柄

//MAKEINTRESOURCE(IDI_ICON1)

//MAKEINTRESOURCE，将资源ID号转换为字符指针类型





5.在CMainFrame增加消息处理WM_TIMER

void CMainFrame::OnTimer(UINT nIDEvent)

{

// TODO: Add your message handler code here and/or call default

static int index=0;//定义一个索引，初始为0

SetClassLong(m_hWnd,GCL_HICON,(LONG)m_hIcons[index]);

index=++index%3;//取模，我们需要它在0到2之间不停变化，以实现图标的转换

CFrameWnd::OnTimer(nIDEvent);

}

//要把一个值始终保持在一个范围内，最好的办法，就是取模(取余)

//取余运算；0除3余0；1除3余1；2除3余2 ；3除3余0。





=====================================================================



**工具栏(CToolBar)的编程**





创建工具栏有2种方式:

(第一种)

1.Create a toolbar resource.创建工具栏资源

2.Construct the CToolBar object.构建CToolBar的对象

3.Call the Create (or CreateEx) function to create the Windows toolbar

调用Create或者CreateEx创建工具栏，并且和CToolBar对象关联起来。

4.Call LoadToolBar to load the toolbar resource. 调用LoadToolBar加载工具栏资源



(第二种)

1.Construct the CToolBar object.构造CToolBar对象

2.Call the Create (or CreateEx) function to create the Windows toolbar and attach it to the

CToolBar object.

调用Create或者CreateEx创建工具栏，并且和CToolBar对象关联起来。



3.Call LoadBitmap to load the bitmap that contains the toolbar button images.

调用LoadBitmap加载一个工具栏位图(.bmp)的图象



4.Call SetButtons to set the button style and associate each button with an image in the

bitmap.

调用SetButtons设置按钮的样式，和位图的每一幅图象关联



-----------------------------------------



创建自己的工具栏(CToolBar)



1.在资源编辑器"插入"一个工具栏,创建3个工具栏资源(按钮)



2.构建CToolBar的对象

在CMainFrame增加成员变量，类型CToolBar，名称m_newToolBar,权限为protected



3.调用Create或者CreateEx创建工具栏，并且和CToolBar对象关联起来。

4.调用LoadToolBar加载工具栏资源

在CMainFrame的OnCreate函数中处理::



拷贝以下的代码::

if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP

| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||

!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))

{

TRACE0("Failed to create toolbar\n");

return -1; // fail to create

}



拷贝代码后修改代码::

if (!m_newToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_RIGHT

| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||

!m_newToolBar.LoadToolBar(IDR_TOOLBAR1))

{

TRACE0("Failed to create toolbar\n");

return -1; // fail to create

}

m_newToolBar.EnableDocking(CBRS_ALIGN_ANY);//让工具栏可以停靠

DockControlBar(&m_newToolBar);//让工具栏停靠在主框架窗口上



m_wndToolBar要改为m_newToolBar

CBRS_TOP改为CBRS_RIGHT//将工具栏停靠在右边

LoadToolBar(IDR_MAINFRAME)改为LoadToolBar(IDR_TOOLBAR1)



----------------------------



接上程序，在"查看"上面新建一个菜单(标记菜单)，让工具栏显示或者隐藏







1.在"查看"上面增加一个菜单项，ID为IDM_VIEW_NEWTOOL，标题为"新的工具栏"

2.对新建的菜单项进行命令响应(COMMAND)



3.利用函数ShowControlBar显示，隐藏工具栏

void ShowControlBar( CControlBar* pBar, BOOL bShow, BOOL bDelay );

第二个参数bShow，BOOL型

If TRUE, specifies that the control bar is to be shown. If FALSE, specifies that the control

bar is to be hidden.如果为真显示工具栏，为假隐藏



方法2

void CMainFrame::OnViewNewtool()

{

// TODO: Add your command handler code here

ShowControlBar(&m_newToolBar,!m_newToolBar.IsWindowVisible(),FALSE);

//IsWindowVisible如果窗口可视的话返回真，我们因为要把它隐藏起来，所以用逻辑非操作符!

}



4.给菜单项加上复选标记

首先是为菜单项进行命令响应(UPDATE_COMMAND_UI)

void CMainFrame::OnUpdateViewNewtool(CCmdUI* pCmdUI)

{

// TODO: Add your command update UI handler code here

pCmdUI->SetCheck(m_newToolBar.IsWindowVisible());

//IsWindowVisible为真就打上标记

}



=========================================================================================



### 状态栏(CStatusBar)的编程







在状态栏上添加时间显示



1.定义字符串资源，在资源编辑器的String_Table中添加IDS_TIMER,标题为时钟

IDS_PROGRESS, 标题为进度栏

2.在CMainFrame的static UINT indicators[](指示器数组ID_SEPARATOR的后面)添加IDS_TIMER，

IDS_PROGRESS。



3.在状态栏上显示系统时间，在CMainFrame的OnCreate函数中：

//CTime::GetCurrentTime

//Returns a CTime object that represents the current time.返回一个CTime对象，表明当前系统时间

CTime t=CTime::GetCurrentTIme();//获取当前系统时间，它返回一个CTime对象

CString str=t.Format("%H:%M:%S");//将当前系统时间格式化(获取表示时间的字符串对象)

CClientDC dc(this);

CSize sz=dc.GetTextExtent(str);//作用见下面的解释(1)

m_wndStatusBar.SetPaneInfo(1,IDS_TIMER,SBPS_NORMAL,sz.cx);//作用见下面的解释(1)

m_wndStatusBar.SetPaneText(1,str);//将表示时间的字符串对象，输出到状态栏



//BOOL SetPaneText( int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE );

//参数分别为，状态栏面板的索引，字符串文本



//解释(1)

//调整显示宽度,因为编译后发现窗格太小，并没用足够位置显示到时钟的秒数

//利用函数void SetPaneInfo( int nIndex, UINT nID, UINT nStyle, int cxWidth );设置窗格信息

//CClientDC dc(this);

//CSize sz=dc.GetTextExtent(str);//得到字符串显示(表示时间)的宽度

//m_wndStatusBar.SetPaneInfo(1,IDS_TIMER,SBPS_NORMAL,sz.cx);//设置窗格信息



4.让时间动态显示

把步骤3的代码放在OnTimer函数上



---------------------------------------------------------------



创建一个进度栏(CProgressCtrl)





1.在CMainFrame增加成员变量，类型CProgressCtrl，名称为m_Progress,权限为protected

2.创建进度栏，在CMainFrame::OnCreate()中

m_Progress.Create(WS_CHILD | WS_VISIBLE,CRect(100,100,200,120),this,123);



3.设置进度栏位置(利用CProgressCtrl::SetPos)，在CMainFrame::OnCreate()中

m_Progress.SetPos(50);



4.创建垂直进度栏

m_Progress.Create(WS_CHILD | WS_VISIBLE | PBS_VERTICAL,CRect(100,100,120,200),this,123);



5.进度栏放在状态栏窗格



首先要获取要放置窗格的矩形区域(利用CStatusBar::GetItemRect函数)；

然后将窗格的区域大小，作为我们进度栏的区域大小。



//以上即为实现代码，但是放在OnCreate函数中不能实现：因为OnCreate()返回前状态栏的位置还没有确定

//，所以无法获得各个子窗格位置,只能用自定义消息办法响应。



自定义消息

1)在MainFrm.h添加(class CMainFrame : public CFrameWnd前面添加)

#define UM_PROGRESS WM_USER+1 //WM_USER加多小随你，这样就可以判断为自定义消息



消息响应函数原形的声明

2)在MainFrm.h中添加(DECLARE_MESSAGE_MAP()前面添加)

afx_msg void OnProgress();



消息映射

3)MainFrm.cpp中添加(END_MESSAGE_MAP()前面添加)

ON_MESSAGE(UM_PROGRESS,OnProgress)



实现消息响应函数

4)

void CMainFrame::OnProgress()

{

CRect rect;

m_wndStatusBar.GetItemRect(2,&rect);//获取状态栏要放置窗格的矩形区域

m_Progress.Create(WS_CHILD | WS_VISIBLE,rect,&m_wndStatusBar,123);

m_Progress.SetPos(50);

}



在OnCreate()函数中发送消息

5)

PostMessage(UM_PROGRESS);//注意不要用SendMessage();因为SendMessage是将消息直接发送给消息响应函数，处理完毕之后，SendMessage函数才返回。PostMessage是将消息发送给消息队列，然后立即返回。之后，程序通告GetMessage取出消息。



当窗口尺寸发生改变，状态栏的举行区域也要发生变化，所以当窗口尺寸发生改变时，重新获取窗格的区域

我们知道，当窗口尺寸发生改变会发送WM_PAINT消息.只需要在WM_PAINT的响应函数当中添加刚才的代码

6)

把OnCreate()中的PostMessage()删除使之失效.

在CMainFrame增加消息处理WM_PAINT



void CMainFrame::OnPaint()

{

CPaintDC dc(this); // device context for painting


// TODO: Add your message handler code here

CRect rect;

m_wndStatusBar.GetItemRect(2,&rect);//获取状态栏要放置窗格的矩形区域

if(!m_Progress.m_hWnd)

m_Progress.Create(WS_CHILD | WS_VISIBLE,rect,&m_wndStatusBar,123);

else

m_Progress.MoveWindow(rect);


m_Progress.SetPos(50);

// Do not call CFrameWnd::OnPaint() for painting messages

}



--------



让进度栏前进

在void CMainFrame::OnTimer

m_Progress.StepIt();



--------------------------------------------------------------------------------------------



将鼠标移动的坐标，适时显示到状态栏上





1.捕获鼠标移动的消息

在view类增加消息处理添加WM_MOUSEMOVE



2.在OnMouseMove()中

void CStyleView::OnMouseMove(UINT nFlags, CPoint point)

{

// TODO: Add your message handler code here and/or call default

CString str;

str.Format("x=%d,y=%d",point.x,point.y);//要输出坐标到状态栏，先将它初始化

((CMainFrame*)GetParent())->m_wndStatusBar.SetWindowText(str);

//m_wndStatusBar变量要改为公用public

CView::OnMouseMove(nFlags, point);

}



//记得要包含CMainFrame头文件。#include "MainFrm.h"

//m_wndStatusBar变量要改为公用public





======================================================================================



**给程序增加启动画面**





"工程"->"增加到工程"->"Component and Control"(组件和控件)->"Visual C++ Components"(vc组件)

->Splash screen



我们发现。。。

系统自动增加了CSplashWnd类

而且在资源编辑器给我们增加了一副位图(Bitmap)，就是该启动画面



在CSplashWnd::OnCreate()中有SetTimer(1,750,NULL);设置750ms的启动画面显示间隔

当然你可以把750改大点，3000，那就是3秒后再隐藏启动画面






