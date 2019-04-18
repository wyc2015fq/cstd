# vc笔记（三） - L_Andy的专栏 - CSDN博客

2012年12月19日 10:40:51[卡哥](https://me.csdn.net/L_Andy)阅读数：1091


张明奇（卡哥）

（1）如何自定义消息
1) 手工定义消息，可以这么写

#define WM_MY_MESSAGE(WM_USER+100),

MS 推荐的至少是 WM_USER+100

(2)写消息处理函数,用

WPARAM,LPARAM返回LRESULT.

LRESULT CMainFrame::OnMyMessage(WPARAM wparam,LPARAM lParam)

{

temp目录: Use "GetTempPath"

//加入你的处理函数 irectory"

}

（10）如何限制窗口的大小?
也就是 FixedDialog形式。 Windows发送 WM_GETMAXMININFO消息来跟踪, 响应它,在 OnGetMAXMININFO 中写代码:

（11）如何使窗口不可见？
很简单,用SW_HIDE 隐藏窗口，可以结合 FindWindow,ShowWindow控制.

（2）**如何通过代码获得应用程序主窗口的指针?**
主窗口的指针保存在CWinThread::m_pMainWnd中,调用AfxGetMainWnd实现。

AfxGetMainWnd() ->ShowWindow(SW_SHOWMAXMIZED)

//使程序最大化.

（2）确定应用程序的路径
Use GetModuleFileName 获得应用程序的路径，然后去掉可执行文件名。

Example:

TCHAR

exeFullPath[MAX_PATH] // MAX_PATH在API中定义了吧，好象是

128

GetModuleFileName(NULL,exeFullPath,MAX_PATH)

（3）如何在程序中获得其他程序的图标?
两种方法:

(1) SDK函数 SHGetFileInfo 或使用 ExtractIcon获得图标资源的 handle,

(2) SDK函数 SHGetFileInfo 获得有关文件的很多信息,如大小图标,属性, 类型等.

Example(1):

在程序窗口左上角显示 NotePad图标.

void CSampleView:

OnDraw(CDC * pDC)

{

if( :: SHGetFileInfo(_T("c:""pwin95""notepad.exe"),0,

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

    说明: 获得notepad.exe的路径正规上来说用GetWindowsDirectory函数得到, 如果是调用 win95下的画笔，应该用访问注册表的方法获得其路径，要作成一个比较考究的程序，考虑应该全面点

（4）获得各种目录信息
Windows目录: Use "GetWindowsDirectory"

Windows下的system目录: Use "GetSystemDirectory"

temp目录: Use "GetTempPath"

当前目录: Use "GetCurrentDirectory"

请注意前两个函数的第一个参数为目录变量名，后一个为缓冲区后两个相反.

（6）如何改变窗口的图标?
向窗口发送 WM_SECTION消息。

Example:

HICON hIcon=AfxGetApp() ->LoadIcon(IDI_ICON)

ASSERT(hIcon)

AfxGetMainWnd() ->SendMessage(WM_SECTION,TRUE,(LPARAM)hIcon)

（7）如何改变窗口的缺省风格?
重载 CWnd:: PreCreateWindow 并修改CREATESTRUCT结构来指定窗口风格和其他创建信息.

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

（8）如何将窗口居中显示?
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

AfxGetMainWnd( ) ->CenterWindow( )
（9）如何让窗口和 MDI窗口一启动就最大化和最小化?
先说窗口。

在 InitStance 函数中设定 m_nCmdShow的取值.

m_nCmdShow=SW_SHOWMAXMIZED //最大化

m_nCmdShow=SW_SHOWMINMIZED //最小化

m_nCmdShow=SW_SHOWNORMAL //正常方式

MDI窗口:

如果是创建新的应用程序,可以用MFC AppWizard 的Advanced 按钮并在MDI子窗口风格组中检测最大化或最小化还可以重载 MDI Window 的PreCreateWindow函数，设置WS_MAXMIZE or WS_MINMIZE

如果从 CMDIChildWnd派生,调用 OnInitialUpdate函数中的 CWnd::Show Window来指定 MDI Child Window的风格。

（12）如何创建一个字回绕的CEditView
重载CWnd : : PreCreateWindow和修改CREATESTRUCT结构，关闭CEditView对象的ES_AUTOHSCROLL和WS_HSCROLL风格位，由于CEditView
 : : PreCreateWindow显示设置cs. style，调用基类函数后要修改cs . style。

BOOL CSampleEDitView : : PreCreateWindow (CREATESTRUCT&cs)

{

//First call basse class function .

BOOL bResutl =CEditView : : PreCreateWindow (cs)

// Now specify the new window style .

cs.style &= ~ (ES_AUTOHSCROLL ｜WS_HSCROLL)

return bResult

}

（13）如何使程序保持极小状态?
这么办: 在恢复程序窗体大小时，Windows会发送消息WM_QUERY-OPEN，用 ClassWizard设置成员函数

OnQueryOpen() ,add following code:

Bool CMainFrame:: OnQueryOpen( )

{

Return false

}

（14）移动窗口
调用CWnd : : SetWindowPos并指定SWP_NOSIZE标志。目的位置与父窗口有关（顶层窗口与屏幕有关）。调用CWnd : : MoveWindow时必须要指定窗口的大小。

//Move window to positoin 100 , 100 of its parent window .

SetWindowPos (NULL, 100 , 100 , 0 , 0 , SWP_NOSIZE ｜SWP_NOAORDER)

（15）通用控件的显示窗口
MFC提供了几个CView派生的视窗类，封装了通用控件的功能，但仍然使用工作框文档显示窗口体系结构：CEditView封装了编辑控件，CTreeView保持了树列表控件，CListView封装了列表显示窗口控件，CRichEditView可以处理多种编辑控件。

（16）重置窗口的大小
调用CWnd: : SetWindowPos并指定SWP_NOMOVE标志，也可调用CWnd : : MoveWindow 但必须指定窗口的位置。

// Get the size of the window .

Crect reWindow

GetWindowRect (reWindow )

//Make the window twice as wide and twice as tall .

SetWindowPos (NULL , 0 , 0 , reWindow . Width ( ) *2,

reWindow . Height () * 2,

SWP_NOMOVE ｜SWP_NOZORDER )

（17）如何单击除了窗口标题栏以外的区域使窗口移动
当窗口需要确定鼠标位置时Windows向窗口发送WM_NCHITTEST信息，可以处理该信息使Windows认为鼠标在窗口标题上。对于对话框和基于对话的应用程序，可以使用ClassWizard处理该信息并调用基类函数，如果函数返回HTCLIENT 则表明鼠标在客房区域，返回HTCAPTION表明鼠标在Windows的标题栏中。

UINT CSampleDialog : : OnNcHitTest (Cpoint point )

{

UINT nHitTest =Cdialog: : OnNcHitTest (point )

return (nHitTest = =HTCLIENT)? HTCAPTION : nHitTest

}

上述技术有两点不利之处，

其一是在窗口的客户区域双击时，窗口将极大；

其二，它不适合包含几个视窗的主框窗口。

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

CWnd: :GetParentFrame 。

void CSampleDialog : : OnLbuttonDown (UINT nFlags, Cpoint point )

{

Cdialog : : OnLButtonDow (nFlags, goint )

//Fool dialog into thinking simeone clicked on its

caption bar .

PostMessage (WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARM (point.x

, point. y

) )

}

（18）如何改变视窗的背景颜色
Windows向窗口发送一个WM_ERASEBKGND消息通知该窗口擦除背景，可以使用ClassWizard重载该消息的缺省处理程序来擦除背景（实际是画），并返回TRUE以防止Windows擦除窗口。

//Paint area that needs to be erased.

BOOL CSampleView : : OnEraseBkgnd (CDC* pDC)

{

// Create a pruple brush.

CBrush Brush (RGB (128 , 0 , 128) )

// Select the brush into the device context .

CBrush* pOldBrush = pDC—>SelcetObject (&Brush)

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

　注释：函数功能，该函数得到一个能够完整包含当前可见区域的最小矩形的大小。该可见区域由当前的剪切区域定义或由剪切路径所定义或者由任何重叠的窗口所定义。

　函数原型：int GetClipBox(HDC hdc, LPRECT lprc)；

函数功能：该函数使用当前选入指定设备环境中的刷子绘制给定的矩形区域。通过使用给出的光栅操作来对该刷子的颜色和表面颜色进行组合。

　函数原型：BOOL PatBlt(HDC hdc, int nXLeft, int nYLeft, int nWidth, int nHeight, DWORD dwRop)；

（19）如何改变窗口标题
调用CWnd : : SetWindowText可以改变任何窗口（包括控件）的标题。

//Set title for application's main frame window .

AfxGetMainWnd ( ) —> SetWindowText (_T("Application title") )

//Set title for View's MDI child frame window .

GetParentFrame ( ) —> SetWindowText ("_T ("MDI Child Frame new title")

)

//Set title for dialog's push button control.

GetDigitem (IDC_BUTTON) —> SetWindowText (_T ("Button new title ") )

如果需要经常修改窗口的标题（注：控件也是窗口），应该考虑使用半文档化的函数AfxSetWindowText。该函数在AFXPRIV.H中说明，在WINUTIL.CPP中实现，在联机帮助中找不到它，它在AFXPRIV.H中半文档化，在以后发行的MFC中将文档化。

AfxSetWindowText的实现如下：

voik AFXAPI AfxSetWindowText (HWND hWndCtrl , LPCTSTR IpszNew )

{

itn nNewLen= Istrlen (Ipaznew)

TCHAR szOld [256]

//fast check to see if text really changes (reduces

flash in the

controls )

if (nNewLen >_contof (szOld)｜｜ : : GetWindowText (hWndCrtl, szOld , _countof (szOld) !=nNewLen｜｜ Istrcmp (szOld , IpszNew)! = 0

{

//change it

: : SetWindowText(hWndCtrl , IpszNew )

}

}

（20）如何防止主框窗口在其说明中显示活动的文档名
创建主框窗口和MDI子窗口进通常具有FWS_ADDTOTITLE风格位，如果不希望在说明中自动添加文档名，必须禁止该风格位，可以使用ClassWizard重置

CWnd: : PreCreateWindow并关闭FWS_ADDTOTITLE风格。

BOOL CMainFrame : : PreCreateWindow (CREATESTRUCT&cs)

{

//Turn off FWS_ADDTOTITLE in main frame .

cs.styel & = ~FWS_ADDTOTITLE 　

return CMDIFrameWnd : : PreCreateWindow (cs )

}

关闭MDI子窗口的FWS _ADDTOTITLE风格将创建一个具有空标题的窗口，可以调用CWnd: : SetWindowText来设置标题。记住自己设置标题时要遵循接口风格指南。

（21）如何获取有关窗口正在处理的当前消息的信息
调用CWnd: : GetCurrentMessage可以获取一个MSG指针。例如，可以使用ClassWizard将几个菜单项处理程序映射到一个函数中，然后调用GetCurrentMessage来确定所选中的菜单项。

void CMainFrame : : OnCommmonMenuHandler ( )

{

//Display selected menu item in debug window .

TRACE ("Menu item %u was selected . "n" ,GetCurrentMessage()->wParam);}

（22）如何在代码中获取工具条和状态条的指针
缺省时，工作框创建状态条和工具条时将它们作为主框窗口的子窗口，状态条有一个AFX_IDW_STATUS_BAR标识符，工具条有一个AFX_IDW_TOOLBAR标识符，下例说明了如何通过一起调用CWnd: : GetDescendantWindow和AfxGetMainWnd来获取这些子窗口的指针：

//Get pointer to status bar .

CStatusBar * pStatusBar = (CStatusBar *) AfxGetMainWnd ( )

—> GetDescendantWindow(AFX_IDW_STUTUS_BAR)

//Get pointer to toolbar .

CToolBar * pToolBar = (CToolBar * ) AfxGetMainWnd ( )

—> GetDescendantWindow(AFX_IDW_TOOLBAR)

（23）如何使能和禁止工具条的工具提示
如果设置了CBRS_TOOLTIPS风格位，工具条将显示工具提示，要使能或者禁止工具提示，需要设置或者清

除该风格位。下例通过调用CControlBar : : GetBarStyle和CControlBar : : SetBarStyle建立一个完成此功能的成员函数：

void CMainFrame : : EnableToolTips ( BOOL bDisplayTips )

{

ASSERT_VALID (m_wndToolBar)

DWORD dwStyle = m _wndToolBar.GetBarStyle ( )

if (bDisplayTips) dwStyle ｜=CBRS_TOOLTIPS

else

dwStyle & = ~CBRS_TOOLTIPS

m_wndToolBar.SetBarStyle (dwStyle )

}

（24）如何创建一个不规则形状的窗口
可以使用新的SDK函数SetWindowRgn。该函数将绘画和鼠标消息限定在窗口的一个指定的区域，实际上使窗口成为指定的不规则形状。使用AppWizard创建一个基于对的应用程序并使用资源编辑器从主对话资源中删除所在的缺省控件、标题以及边界。

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

for (nIndex = 0;nIndex< nEllipse;nIndex++)

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

（25）如何获取应用程序的实例句柄?
应用程序的实例句柄保存在CWinApp m_hInstance 中,可以这么调用AfxGetInstancdHandle获得句柄.

Example: HANDLE hInstance=AfxGetInstanceHandle()

（26）如何编程结束应用程序?
这是个很简单又是编程中经常要遇到的问题.

向窗口发送 WM_CLOSE消息,调用 CWnd::OnClose成员函数.允许对用户提示是否保存修改过的数据.

Example: AfxGetMainWindow()->SendMessage(WM_CLOSE)

还可以创建一个自定义的函数 Terminate Window

void Terminate Window(LPCSTR pCaption)

{

CWnd *pWnd=Cwnd::FindWindow(NULL,pCaption)

if (pWnd)

pWnd ->SendMessage(WM_CLOSE)

}

    说明: FindWindow函数不是提倡的做法，因为它无法处理标题栏自动改变，比如我们要检测 Notepad是不是已运行而事先不知道Notepad的标题栏,这时FindWindow就无能为力了，可以通过枚举 windows任务列表的办法来实现。在机械出版社"Windows 95 API开发人员指南"一书有比较详细的介绍,这里就不再多说乐。

（27）如何创建和使用无模式对话框
MFC将模式和无模式对话封装在同一个类中，但是使用无模式对话需要几个对话需要几个额处的步骤。首先，使用资源编辑器建立对话资源并使用ClassWizard创建一个CDialog的派生类。模式和无模式对话的中止是不一样的：模式对话通过调用CDialog : : EndDialog 来中止，无模式对话则是调用CWnd: : DestroyWindow来中止的，函数CDialog : : OnOK和CDialog : : OnCancel调用EndDialog ,所以需要调用DestroyWindow并重置无模式对话的函数。

void CSampleDialog : : OnOK ( )

{

// Retrieve and validate dialog data .

if (! UpdateData (TRUE) )

{

// the UpdateData rountine

will set focus to correct item TRACEO (" UpdateData failed during dialog termination ."n")

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

最后，要创建无模式对话。可以调用CDialog : : DoModal创建一个模式对放，要创建一个无模式对话则要调用CDialog: : Create。下面的例子说明了应用程序是如何创建无模式对话的：象；无模式对话不是同步的，创建函数调用后立即返回，

void CMainFrame : : OnSampleDialog ( )

{

//Allocate a modeless dialog object .

CSampleDilog * pDialog =new CSampleDialog

ASSERT_VALID (pDialog) Destroy ( )

//Create the modeless dialog . represents this dialog.

BOOL bResult = pDialog —> Creste (IDD_IDALOG)

ASSERT (bResult )

}

（28）如何防止主框窗口在其说明中显示活动的文档名
创建主框窗口和MDI子窗口进通常具有FWS_ADDTOTITLE风格位，如果不希望在说明中自动添加文档名，必须禁止该风格位，可以使用ClassWizard重置

CWnd: : PreCreateWindow并关闭FWS_ADDTOTITLE风格。

BOOL CMainFrame : : PreCreateWindow (CREATESTRUCT&cs)

{

//Turn off FWS_ADDTOTITLE in main frame .

cs.styel & = ~FWS_ADDTOTITLE 　

return CMDIFrameWnd : : PreCreateWindow (cs )

}

关闭MDI子窗口的FWS _ADDTOTITLE风格将创建一个具有空标题的窗口，可以调用CWnd: : SetWindowText来设置标题。记住自己设置标题时要遵循接口风格指南。

（29）如何在代码中获取工具条和状态条的指针
缺省时，工作框创建状态条和工具条时将它们作为主框窗口的子窗口，状态条有一个AFX_IDW_STATUS_BAR标识符，工具条有一个AFX_IDW_TOOLBAR标识符，下例说明了如何通过一起调用CWnd: : GetDescendantWindow和AfxGetMainWnd来获取这些子窗口的指针：

//Get pointer to status bar .

CStatusBar * pStatusBar = (CStatusBar *) AfxGetMainWnd ( )—> GetDescendantWindow(AFX_IDW_STUTUS_BAR)

//Get pointer to toolbar .

CToolBar * pToolBar = (CToolBar * ) AfxGetMainWnd ( )—> GetDescendantWindow(AFX_IDW_TOOLBAR)
（30）怎样加载其他的应用程序?
三个SDK函数 winexec, shellexecute,createprocess可以使用。

WinExec最简单，两个参数，前一个指定路径，后一个指定显示方式.后一个参数值得说一下，比如泥用 SW_SHOWMAXMIZED方式去加载一个无最大化按钮的程序，就是Neterm,calc等等，就不会出现正常的窗体，但是已经被加到任务列表里了。

ShellExecute较 WinExex灵活一点，可以指定工作目录,下面的Example就是直接打开 c:"temp"1.txt,而不用加载与 txt文件关联的应用程序,很多安装程序完成后都会打开一个窗口，来显示Readme or Faq,我猜就是这么作的啦.

ShellExecute(NULL,NULL,_T("1.txt"),NULL,_T("c:""temp"),SW_SHOWMAXMIZED)

CreateProcess最复杂，一共有十个参数，不过大部分都可以用NULL代替，它可以指定进程的安全属性，继承信息，类的优先级等等.来看个很简单的Example:

STARTUPINFO stinfo

//启动窗口的信息

PROCESSINFO procinfo //进程的信息

CreateProcess(NULL,_T("notepad.exe"),NULL,NULL.FALSE,

NORMAL_PRIORITY_

CLASS,NULL,NULL, &stinfo,&procinfo)

（31）如何在代码中获取工具条和状态条的指针
缺省时，工作框创建状态条和工具条时将它们作为主框窗口的子窗口，状态条有一个AFX_IDW_STATUS_BAR标识符，工具条有一个AFX_IDW_TOOLBAR标识符，下例说明了如何通过一起调用CWnd: : GetDescendantWindow和AfxGetMainWnd来获取这些子窗口的指针：

//Get pointer to status bar .

CStatusBar * pStatusBar = (CStatusBar *) AfxGetMainWnd ( )—> GetDescendantWindow(AFX_IDW_STUTUS_BAR)

（32）如何使能和禁止工具条的工具提示
如果设置了CBRS_TOOLTIPS风格位，工具条将显示工具提示，要使能或者禁止工具提示，需要设置或者清除该风格位。下例通过调用CControlBar : : GetBarStyle和CControlBar : : SetBarStyle建立一个完成此功能的成员函数：

void CMainFrame : : EnableToolTips ( BOOL bDisplayTips )

{

ASSERT_VALID (m_wndToolBar)

DWORD dwStyle = m _wndToolBar.GetBarStyle ( )

if (bDisplayTips) dwStyle ｜=CBRS_TOOLTIPS

else

dwStyle & = ~CBRS_TOOLTIPS

m_wndToolBar.SetBarStyle (dwStyle )

}

//Get pointer to toolbar .

CToolBar * pToolBar = (CToolBar * ) AfxGetMainWnd ( )—> GetDescendantWindow(AFX_IDW_TOOLBAR)

（33）如何设置工具条标题
工具条是一个窗口，所以可以在调用CWnd : : SetWindowText来设置标题，例子如下：

int CMainFrame : : OnCreate (LPCREATESTRUCT lpCreateStruct )

{

…

// Set the caption of the toolbar .

m_wndToolBar.SetWindowText (_T "Standdard")

（34）如何使窗口始终在最前方?
BringWindowToTop(Handle)

SetWindowPos函数，指定窗口的最顶风格,用WS_EX_TOPMOST扩展窗口的风格

Example:

void ToggleTopMost(

CWnd *pWnd)

{

ASSERT_VALID(pWnd)

pWnd ->SetWindowPos(pWnd-> GetStyle( ) &WS_EX_TOPMOST)?

&wndNoTopMOST: &wndTopMost,0,0,0,0,SSP_NOSIZE|WSP_NOMOVE)

}

（35）如何在对话框中显示一个位图
这要归功于Win 32先进的静态控件和Microsoft的资源编辑器，在对话框中显示位图是很容易的，只需将图形控件拖到对话中并选择适当属性即可，用户也可以显示图标、位图以及增强型元文件。

（36）如何改变对话或窗体视窗的背景颜色
调用CWinApp : : SetDialogBkColor可以改变所有应用程序的背景颜色。第一个参数指定了背景颜色，第二个参数指定了文本颜色。下例将应用程序对话设置为蓝色背景和黄色文本。

BOOL CSampleApp : : InitInstance ( )

{

…

//use blue dialog with yellow text .

SetDialogBkColor (RGB (0, 0, 255 ), RGB ( 255 ,255 , 0 ) )

…

}

需要重画对话（或对话的子控件）时，Windows向对话发送消息WM_CTLCOLOR，通常用户可以让Windows选择绘画背景的刷子，也可重置该消息指定刷子。下例说明了创建一个红色背景对话的步骤。

首先，给对话基类增加一个成员变量

CBursh :class CMyFormView : public CFormView

{

…

private :

CBrush m_ brush // background brush

…

}

其次，在类的构造函数中将刷子初始化为所需要的背景颜色。

CMyFormView : : CMyFormView ( )

{

// Initialize background brush .

m_brush .CreateSolidBrush (RGB ( 0, 0, 255) )

}

最后，使用ClassWizard处理WM_CTLCOLOR消息并返回一个用来绘画对话背景的刷子句柄。注意：由于当重画对话控件时也要调用该函数，所以要检测nCtlColor参量。

HBRUSH CMyFormView : : OnCtlColor (CDC* pDC , CWnd*pWnd , UINT nCtlColor)

{

// Determine if drawing a dialog box . If we are, return +handle to

//our own background brush . Otherwise let windows handle it .

if (nCtlColor = = CTLCOLOR _ DLG )

return (HBRUSH) m_brush.GetSafeHandle ( )

return CFormView : : OnCtlColor (pDC, pWnd , nCtlColor)

}

（37）如何获取一个对话控件的指针
有两种方法。其一，调用CWnd: : GetDlgItem，获取一个CWnd*指针调用成员函数。下例调用GetDlgItem，将返回值传给一个CSpinButtonCtrl*以便调用CSpinButtonCtrl : : SetPos 函数：

BOOL CSampleDialog : : OnInitDialog ( )

{

CDialog : : OnInitDialog ( )

//Get pointer to spin button .

CSpinButtonCtrl * pSpin =( CSpinButtonCtrl *) GetDlgItem(IDC_SPIN)

ASSERT_VALID (pSpin)

//Set spin button's default position .

pSpin —> SetPos (10)

return TRUE

}

其二，可以使用ClassWizard将控件和成员变量联系起来。在ClassWizard中简单地选择Member Variables标签，然后选择Add Variable …按钮。如果在对话资源编辑器中，按下Ctrl键并双击控件即可转到Add Member Variable对话。

（38）如何禁止和使能控件
控件也是窗口，所以可以调用CWnd : : EnableWindow使能和禁止控件。

//Disable button controls .

m_wndOK.EnableWindow (FALSE )

m_wndApply.EnableWindow (FALSE )

（39）如何改变控件的字体

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

VARIABLE_PITCH ｜FF_SWISS, _T("Arial") )

//Set font for push button .

m_wndButton . SetFont (&m _font )

…

}

（40）如何在OLE控件中使用OLE_COLOR数据类型
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

（41）在不使用通用文件打开对话的情况下如何显示一个文件列表
调用CWnd: : DlgDirList或者CWnd: : DlgDirListComboBox，Windows 将自动地向列表框或组合框填充可用的驱动器名或者指定目录中的文件，下例将Windows目录中的文件填充在组合框中：

BOOL CSampleDig : : OnInitDialog ( )

{

CDialog : : OnInitDialog ( )

TCHAR szPath [MAX_PATH] = {"c:""windows"}

int nReslt = DlgDirListComboBox (szPath, IDC_COMBO , IDC_CURIDIR, DDL_READWRITE ｜DDL_READONLY｜DDL_HIDDEN｜ DDL_SYSTEM｜DDL_ARCHIVE)

return TRUE

}

（42）为什么旋转按钮控件看起来倒转
需要调用CSpinCtrl : : SetRange 设置旋转按钮控件的范围，旋转按钮控件的缺省上限为0，缺省下限为100，这意味着增加时旋转按控件的值由100变为0。下例将旋转按钮控件的范围设置为0到100：

BOOL CAboutDlg : : OnInitDialog ( )

{

CDialog : : OnInitDialog ( )

//set the lower and upper limit of the spin button

m_wndSpin . SetRange ( 0 ,100 )

return TRUE

}

Visual C++ 4.0 Print对话中的Copise旋转按钮控件也有同样的问题：按下Up按钮时拷贝的数目减少，而按下Down 按钮时拷贝的数目增加。

（43）为什么旋转按钮控件不能自动地更新它下面的编辑控件
如果使用旋转按钮的autu buddy特性，则必须保证在对话的标记顺序中buddy窗口优先于旋转按钮控件。从Layout菜单中选择Tab Order菜单项（或者按下Crtl+D）可以设置对话的标签顺序。

（44）如何用位图显示下压按钮
Windows 95按钮有几处新的创建风格，尤其是BS_BITMAP和BS_ICON，要想具有位图按钮，创建按钮和调用CButton : : SetBitmap或CButton : : SetIcon时要指定BS_BITMAP或BS_ICON风格。

首先，设置按钮的图标属性。然后，当对话初始化时调用CButton: : SetIcon。注意：下例用图标代替位图，使用位图时要小心，因为不知道背景所有的颜色——并非每个人都使用浅灰色。

BOOL CSampleDlg : : OnInitDialog ( )

{

CDialog : : OnInitDialog ( )

//set the images for the push buttons .

m_wndButton1.SetIcon (AfxGetApp ( ) —> LoadIcon (IDI _ IPTION1))

m_wndButton2.SetIcon (AfxGetApp ( ) —> LoadIcon (IDI _ IPTION2))

m_wndButton3.SetIcon (AfxGetApp ( ) —> LoadIcon (IDI _ IPTION3))

return TRUE

}

（45）如何一个创建三态下压按钮

可以使用新的BS_PUSHBUTTON 风格位和检测框以及按钮来创建一个三态下压按钮。这很容易，只需将检测框和按钮拖拉到对话中并指定属性Push—like即可。不用任何附加程序就可以成为三态下压按钮。

（46）如何动态创建控件
分配一个控件对象的实例并调用其Create成员函数。开发者最容易忽略两件事：忘记指定WS_VISBLE标签和在栈中分配控件对象。下例动态地创建一个下压按钮控件：

//In class declaration (.H file ).

private : CButton* m _pButton

//In class implementation (.cpp file ) .

m_pButton =new CButton

ASSERT_VALID (m_pButton)

m_pButton —>Create (_T ("Button Title ") , WS_CHILD ｜WS_VISIBLE ｜BS_PUSHBUTTON. Crect ( 0, 0, 100 , 24) , this , IDC _MYBUTTON )

（47）如何限制编辑框中的准许字符
如果用户在编辑控件中只允许接收数字，可以使用一个标准的编辑控件并指定新的创建标志ES_NUMBERS,它是Windows 95新增加的标志，该标志限制编辑控件只按收数字字符。如果用户需要复杂的编辑控件，可以使用Microsoft 的屏蔽编辑控件，它是一个很有用的OLE定制控件。

如果希望不使用OLE 定制控件自己处理字符，可以派生一个CEdit类并处理WM_CHAR消息，然后从编辑控件中过滤出特定的字符。首先，使用ClassWizard建立一个 CEdit的派生类，其次，在对话类中指定一个成员变量将编辑控件分类在OnInitdialog 中调用CWnd: : SubclassDlgItem .

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

如果要修改字符，则不能仅仅简单地用修改过的nChar调用CEdit: : OnChar，然后CEdit: : OnChar调用CWnd: : Default获取原来的wParam 和lParam 的值，这样是不行的。要修改一个字符，需要首先修改nChar，然后用修改过的nChar调用CWnd: : DefWindowProc。下例说明了如何将字符转变为大写：

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

（48）如何改变控件的颜色
有两种方法。其一，可以在父类中指定控件的颜色，或者利用MFC4.0新的消息反射在控件类中指定颜色。当控件需要重新着色时，工作框调用父窗口（通常是对话框）的CWnd: : OnCrtlColor,可以在父窗口类中重置该函数并指定控件的新的绘画属性。例如，下述代码将对话中的所有编辑控件文本颜色改为红色：

HBRUSH CAboutDig : : OnCtlColor (CDC * pDCM , CWnd * pWnd , UINT nCtlColor)

{

HBRUSH hbr = CDialog : : OnCtlColor (pDC, pWnd , nCtlColor )

//Draw red text for all edit controls .

if (nCtlColor= = CTLCOLOR_EDIT )

pDC —> SetTextColor (RGB (255, 0 , 0 , ) )

return hbr

}

然而，由于每个父窗口必须处理通知消息并指定每个控件的绘画属性，所以，这种方法不是完全的面向对象的方法。控件处理该消息并指定绘画属性更合情合理。消息反射允许用户这样做。通知消息首先发送给父窗口，如果父窗口没有处理则发送给控件。创建一个定制彩色列表框控件必须遵循下述步骤。

首先，使用ClassWizard 创建一个CListBox 的派生类并为该类添加下述数据成员。

class CmyListBox：publilc CListBox

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

（49）当向列表框中添加多个项时如何防止闪烁
调用CWnd::SetRedraw 清除重画标志可以禁止CListBox（或者窗口）重画。当向列表框添加几个项时，用户可以清除重画标志，然后添加项，最后恢复重画标志。为确保重画列表框的新项，调用SetRedraw (TRUE) 之后调用CWnd::Invalidate。

//Disable redrawing.

pListBox->SetRedraw (FALSE)

//Fill in the list box gere

//Enable drwing and make sure list box is redrawn.

pListBox->SetRedraw (TRUE)

pListBox->Invalidate ()

（50）如何向编辑控件中添加文本
由于没有CEdit:: AppendText函数，用户只好自己做此项工作。调用CEdit:: SetSel移动到编辑控件末尾，然后调用CEdit:: ReplaceSel添加文本。下例是AppendText 的一种实现方法：

void CMyEdit:: AppendText (LPCSTR pText)

{

int nLen=GetWindowTextLength ()

SetFocus ()

SetSel (nLen, nLen)

ReplaceSel (pText)

}

（51）如何访问预定义的GDI对象
可以通过调用CDC:: SelectStockObject使用Windows的几个预定义的对象，诸如刷子、笔以及字体。下例使用了Windows预定义的笔和刷子GDI对象在视窗中画一个椭圆。

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

（52）如何获取GDI对象的属性信息
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

（53）如何实现一个橡皮区矩形
CRectTracker是一个很有用的类，可以通过调用CRectTracker::TrackRubberBand 响应WM_LBUTTONDOWN消息来创建一个橡皮区矩形。

下例表明使用CRectTracker移动和重置视窗中的蓝色椭圆的大小是很容易的事情。

首先，在文件档中声明一个CRectTracker数据成员：

class CSampleView : Public CView

{

…

public :

CrectTracker m_tracker

…

}

其次，在文档类的构造函数中初始化CRectTracker 对象：

CSampleDoc:: CSampleDOC ()

{

//Initialize tracker position, size and style.

m_tracker.m_rect.SetRect (0, 0, 10, 10)

m_tracker.m_nStyle=CRectTracker:: resizeInside | CRectTracker ::dottedLine

}

然后，在OnDraw函数中画椭圆和踪迹矩形：

void CSampleView:: OnDraw (CDC* pDC)

{

CSampleDoc* pDoc = GetDocument ()

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

（54）如何更新翻转背景颜色的文本
调用CDC:: SetBkmode并传送OPAQUE用当前的背景颜色填充背景，或者调用CDC::SetBkMode并传送TRANSPARENT使背景保持不变，这两种方法都可以设置背景模式。下例设置背景模式为TRANSPARENT，可以两次更新串，用花色带黑阴影更新文本。黑色串在红色串之后，但由于设置了背景模式仍然可见。

void CSampleView:: OnDraw (CDC* pDC)

{

//Determint size of view.

CRect rcView

GetClientRect (rcView)

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

（55）如何创建一个具有特定点大小的字体

可以指定字体逻辑单位的大小，但有时指定字体的点的大小可能会更方便一些。可以如下将字体的点转换为字体的高度：

int nHeigth=mulDiv (nPointSize, -dc.GetDeviceCaps (LOGPIXELSY), 72)

下例创建了一个8点的Apial字体：

…

CClientDC dc (AfxGetMainWnd ())

m_font. CreateFont (MulDiv (8, -dc.GetDeviceCaps (LOGPIXELSY), 72), 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH | FF-SWISS,_T("Arial"))

（56）如何计算一个串的大小
函数CDC:: Det text Extent 根据当前选择的字体计算一个串的高度和宽度。如果使用的不是系统字体而是其他字体，则在调用GetTextExtent之前将字体选进设备上下文中是很重要的，否则计算高度和宽度时将依据系统字体，由此得出的结果当然是不正确的。下述样板程序当改变下压按钮的标题时动态调整按钮的大小，按钮的大小由按钮的字体和标题的大小而定。响应消息WM_SETTEXT时调用OnSetText，该消息使用ON_MESSAE宏指令定义的用户自定义消息。

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

（57）如何显示旋转文本
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

for (int nAngle=0;nAngle<3600;nAngle+=150)

{

//Specify new angle.

stFont.lfEscapement=nAngle

//Create and select font into dc.

font.CreateFontIndirect(&stfont)

CFont* pOldFont=pDC ->SelectObject(&font)

//Draw the text.

pDC->SelectObject(pOldFont)

font.DelectObject()

}

}

（58）如何正确显示包含标签字符的串
调用GDI文本绘画函数时需要展开标签字符，这可以通过调用CDC:: TabbedTextOut或者CDC:: DrawText并指定DT_EXPANDTABS标志来完成。TabbedTextOut函数允许指定标签位的数组，下例指定每20设备单位展开一个标签：

void CSampleView:: OnDraw (CDC* pDC)

{

CTestDoc* pDoc=GetDocument ()

ASSERT_VALID (pDoC)

CString str

str.Format (_T ("Cathy"tNorman"tOliver"))

int nTabStop=20 //tabs are every 20 pixels

pDC->TabbedtextOut (10, 10, str, 1, &nTabStop, 10)

}

（59）如何快速地格式化一个CString对象
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

（60）串太长时如何在其末尾显示一个省略号
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

（61）为什么即使调用EnableMenuItem菜单项后，菜单项还处于禁止状态
需要将CFrameWnd:: m_bAutomenuEnable设置为FALSE，如果该数据成员为TRUE（缺省值），工作框将自动地禁止没有ON_UPDATE_COMMAND_UI或者ON_COMMAND的菜单项。

//Disable MFC from automatically disabling menu items.

m_bAuoMenuEnable=FALSE

//Now enable the menu item.

CMenu* pMenu=GetMenu ()

ASSERT_VALID (pMenu)

pMenu->EnableMenuItem (ID_MENU_ITEM,MF_BYCOMMAND | MF_ENABLED)

（62）如何给系统菜单添加一个菜单项
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

（63）如何确定顶层菜单所占据的菜单行数
这可以通过简单的减法和除法来实现。首先，用户需要计算主框窗口的高度和客户区；其次，从主框窗口的高度中减去客户区、框边界以及标题的高度；最后，除以菜单栏的高度。下例成员函数是一个计算主框菜单所占据的行数的代码实现。

int CMainFrame:: GetMenuRows ()

{

CRect rcFrame,rcClient

GetWindowRect (rcFrame)

GetClientRect (rcClient)

return (rcFrame.Height () -rcClient.Height () - :: GetSystemMetrics(SM_CYCAPTION) - (:: GetSystemMetrics(SM_CYFRAME) *2)) / :: GetSystemMetrics(SM_CYMENU)

}

（64）在用户环境中如何确定系统显示元素的颜色
调用SDK函数GetSysColor可以获取一个特定显示元素的颜色。下例说明了如何在MFC函数CMainFrameWnd:: OnNcPaint中调用该函数设置窗口标题颜色。

注释：The framework calls this member function when the nonclient area needs to be painted.

void CMiniFrameWnd:: OnNcPaint ()

{

…

dc.SetTextColor (:: GetSysColor (m_bActive ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT))

…

（65）如何查询和设置系统参数
在Windows 3.1 SDK中介绍过SDK函数SystemParametersInfo，调用该函数可以查询和设置系统参数，诸如按键的重复速率设置、鼠标双击延迟时间、图标字体以及桌面覆盖位图等等

//Create a font that is used for icon titles.

LOGFONT stFont

∶: SystemParametersInfo (SPIF_GETICONTITLELOGFONT, sizeof (LOGFONT), &stFont, SPIF_SENDWININICHANGE)

注释：Initializes a **CFont** object with the characteristics given in a**LOGFONT** structure pointed to by lpLogFont.

m_font.CreateFontIndirect (&stFont)

//Change the wallpaper to leaves.bmp.

∶ : SystemParametersInfo (SPI_SETDESKWALLPAPER, 0, _T (" forest.bmp"), SPIF_UPDATEINIFILE)

（66）如何确定当前屏幕分辨率
调用SDK函数GetSystemMetrics，该函数可以检索有关windows显示信息，诸如标题大小、边界大小以及滚动条大小等等。

//Initialize CSize object with screen size.

CSize sizeScreen (GetSystemMetrics (SM_CXSCREEN),

GetSystemMetrics (SM_CYSCREEN))

（67）如何使用一个预定义的Windows光标
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

（68）如何检索原先的Task Manager应用程序使用的任务列表
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

（69）如何确定Windows和Windows系统目录
有两个SDK函数可以完成该功能。GetWindowsDirectory和GetSystemDirectory，下例说明了如何使用这两个函数：

TCHAR szDir [MAX_PATH]

//Get the full path of the windows directory.

∶ : GetWindowsDirectory (szDir, MAX_PATH)

TRACE ("Windows directory %s\n", szDir)

//Get the full path of the windows system directory.

∶ : GetSystemDirectory (szDir, MAX_PATH)

TRACE ("Windows system directory %s\n", szDir)

（70）在哪儿创建临文件
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

（71）我怎样才能建立一个等待光标?
调用 BeginWaitCursor 函数来启动等待光标，调用 EndWaitCursor 函数来结束等待光标。要注意，二者都要调用 app 的成员函数，如下所示:

    AfxGetApp()->BeginWaitCursor();

    // 要做的事

    AfxGetApp()->EndWaitCursor();

（72）我在MDI框架中有个 form 视窗。它有个取消按钮，我需要当用户按取消按钮时可关闭form视窗。我应该如何关闭该文档?
调用 OnCloseDocument 函数。

（73）如何访问桌面窗口
静态函数CWnd:: GetDesktopWindow 返回桌面窗口的指针。下例说明了MFC函数CFrameWnd::BeginModalState是如何使用该函数进入内部窗口列表的。

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

（74）什么是COLORREF? 我该怎样用它?
COLORREF是一个32-bit整型数值，它代表了一种颜色。你可以使用RGB函数来初始化COLORREF。例如：

    COLORREF color = RGB(0, 255, 0);

RGB函数接收三个0-255数值，一个代表红色，一个代表绿色，一个代表蓝色。在上面的例子中，红色和蓝色值都为0，所以在该颜色中没有红色和蓝色。绿色为最大值255。所以该颜色为绿色。0,0,0为黑色，255,255,255 为白色。

另一种初始化COLORREF的方法如下所示：

    CColorDialog colorDialog;

    COLORREF color;

    if( colorDialog.DoModal() == IDOK )

    {

        color = colorDialog.GetColor();

    }

这段代码使用了MFC中的颜色对话框，它需要文件。

（75）AppWizard所产生的STDAFX文件是干什么用的?
它主要是协助产生预编译头文件的。通常你是不需要修改它的。

（76）我在我的程序中是了CDWordArray。我向它添加了约10,000个整数，这使得它变得非常非常慢。为什么会这么糟?
CDWordArray是很好用的，只是因为你没有指定数组的最大尺寸。因此，当你添加新元素时，该类会从堆中重新分配空间。不幸的是，该类会在每次插入新元素时都为数组重新分配空间。如果你向它添加了很多新元素，所有这些分配和复制数组的操作会就会使它变慢。解决该问题的方法是，你可以使用SetSize函数的第二个参数来改变这种重新分配的频率。例如，如果你把该参数设置为500，则每次数组空间超出时它才重新分配并添加500个新空间，而不是1 个。这样一来，你就可以不用重新分配而添加了另外499个元素空间，这也会大大提高程序的运行速度。

（77）我该如何改变MDI框架窗口的子窗口的大小以使在窗口以一定的大小打开?
在视中的OnInitialUpdate函数中调用GetParentFrame函数。GetParentFrame会返回一指向一保存有该视的框架窗口的指针。然后调用在框架窗口上调用MoveWindow。

（78）在我的程序的某些部分，我可以调用 MessageBox 函数来建立一个信息对话框，例如在视类中。但是，在其它部分我却不能，如文档类中。为什么？我怎样才能在我的应用程序类中建立一个信息对话框？
MessageBox函数来自CWnd 类，所以你只能在从CWnd继承的类(如CView )中调用它。但是，MFC也提供了AfxMessageBox函数，你可以在任何地方调用它。

（79）我需要在我的程序中设置全局变量，以使文档中的所有类都能访问。我应该把它放到哪儿?
把该变量放到该应用程序类的头文件中的attribute处。然后，在程序的任何地方，你都可以用下面的方法来访问该变量：

    CMyApp *app = (CMyApp *)AfxGetApp();

    app->MyGlobalVariable = ...

（80）我听说MFC可以发现内存漏洞，我怎样使用该特性?
如果你在Debug菜单中的Go选项(不是Project菜单中的Execute选项)来运行你的应用程序，MFC 应该在程序终止时报告内存漏洞。如果没有，那么试试运行MFC Tracer工具程序(在VC++程序组中)，并启动跟踪。然后返回应用程序。

（81）我怎样才能在我的应用程序中循环浏览已经打开的文档?
使用CDocTemplate中未公开的GetFirstDocPosition()和GetNextDoc()函数。

（82）才能在我的应用程序中循环浏览已经打开的视?
使用CDocument 中未公开的GetFirstViewPosition() 和GetNextView() 函数。

（83）函数PreCreateWindow是干什么用的?
PreCreateWindow允许你在调用 CreateWindow 之前来改变窗口属性。

（84）该怎样防止MFC在窗口标题栏上把文档名预置成应用程序名?
在PreCreateWindow函数中删除FWS_PREFIXTITLE标志的窗口样式：

    cs.style &= ~FWS_PREFIXTITLE;

（85）我应该怎样防止MFC在窗口标题栏上添加文档名?
在PreCreateWindow函数中删除FWS_ADDTOTITLE 标志的窗口样式：

    cs.style &= ~FWS_ADDTOTITLE ;

（86）我应该如何改变视窗口的大小?
因为视窗口实际上是框架窗口的子窗口，所以你必须改变框架窗口的大小，而不是改变视窗口。使用CView类中的GetParentFrame()函数获得指向框架窗口的指针，然后调用MoveWindow()函数来改变框架的大小。这会使变尺寸的视充满框架窗口。

（87）我有一无模式对话框。我怎样才能在窗口退出时删除CDialog对象?
把“delete this”加到PostNcDestroy中。这主要用在需要自动删除对象的场合。

（88）为什么把“delete this”放在PostNcDestroy中而不是OnNcDestroy?
OnNcDestroy只被已建立的窗口调用。如果建立窗口失败(如PreCreateWindow )，则没有窗口处来发送WM_NCDESTROY消息。PostNcDestroy是在对象窗口被完全删除，在OnNcDestroy后，甚至在窗口建立失败之后调用的。

（89）File菜单中的MRU列表是从哪儿来的？列表中的名字放在哪儿了？我怎样才能改变列表中项目的最大值？
在应用程序类的InitInstance函数中对LoadStdProfileSettings的调用中。该调用接受一个参数(在缺省情况下如果没有传递值则为4 )。MRU文件名是从INI文件中调用的。如果你有带有ID_FILE_MRU_FILE1的ID的菜单选项，它会为调入的MRU列表所替换。如果你改变传递给 LoadStdProfileSettings的数值(最大为16 )，则你就改变了所装如文件名的最大值。

（90）我在菜单中添加了新的项。但是，当我选该项时，在状态栏上没有出现任何提示信息。为什么?
打开资源文件中的菜单模板。打开新菜单选项的属性对话框。在对话框的底部的Prompt 编辑框中，你可以如下指定状态栏上的提示信息和工具栏上的提示信息(如果你已经建立的工具栏按钮):

    Status bar string"nFlying tag

（91）我怎样才能在应用程序的缺省系统菜单中加上一些东西?
系统菜单与其它菜单类似，你可以添加或删除项目，这需要使用CMenu类的成员函数。下面的代码在你的系统菜单后面添加一个新菜单项：

    CMenu *sysmenu;

    sysmenu = m_pMainWnd->GetSystemMenu(FALSE);

    sysmenu->AppendMenu(MF_STRING, 1000, "xxx");

参见MFC帮助文件中的CMenu 类。

（92）我建立了一个对话框。但是当我显示该对话框时，第一个编辑框总是不能获得焦点，我必须单击它来使它获得焦点。我怎样才能使第一个编辑框在对话框打开时就获得焦点?
打开资源编辑器中的对话框模板。在Layout菜单中选择Tab Order 选项。按你的需求单击对话框中的控制来重新排列这些控制的tab顺序。

（93）我怎样才能使一个窗口具有“always on top”特性?
在调用OnFileNew后，在你的InitInstance函数中加上下面的代码：

m_pMainWnd->SetWindowPos(&CWnd::wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

（94）我要为我的form view添加文档模板。我先建立了对话框模板，然后使用ClassWizard建立了基于CFormView的新类，它也是从CDocument继承来的。我还建立了相应的资源并在InitInstance中添加了新的文档模板。但是，当我试图运行该程序时，出现了Assertion信息。为什么?

form的对话框模板需要些特殊设置以便可用于CFromView。确保这些设置的最简单方法是使用 AppWizard来建立CFormView应用程序，并查看AppWizard 所建立的对话框模板所选择的Styles Properties。你会发现该对话框模板具有下列样式：没有标题栏、不可见和“Child”。把你的form view的对话框属性变成这样就可以了。
（95）我在一对话框中有一列表框，我需要tabbed列表框中的项目。但是，当我处理含有tab字符(用AddString添加的)的列表项时，tab被显示成小黑块而没有展开。哪儿出错了?

在对话框模版中，打开列表框的属性。确保选择了“Use Tabstops” 样式。然后，确保在对话框类中OnInitDialog函数中调用SetTabStops。
（96）我建立了一个应用程序，并使用了CRecordset类。但是，当我运行该程序时，它试图要访问数据库，并给出“Internal Application Error”对话框。我应该怎样做?

通常情况下，当你的程序中向数据库发送信息的SQL语句出现问题时才出现该对话框。例如，参见下面的例子：

    set.m_strFilter = "(ZipCode = '27111')";

如果 ZipCode 列被定义为字符串时不会出现问题，如果定义为long，则会出现“Internal Application Error”对话框，这是由于类型不匹配的缘故。如果你删除27111的单引号，则不会出现问题。当你看到“Internal Application Error”时，最好检查一下试图要发送给数据库的SQL语句。
（97）我用ClassWizard建立了一个类。但是，我把名字取错了，我想把它从项目中删除，应该如何做?

在ClassWizard对话框关闭后，用文件管理器删除新类的H 和CPP文件。然后打开ClassWizard，它会提示丢失了两个文件，并询问你该如何做。你可以选择从项目中删除这两个问的按钮。

（98）当我打开应用程序中的窗口时，我要传递该窗口的矩形尺寸。该矩形指定了窗口的外围大小，但是当我调用GetClientRect时，所得到的尺寸要比所希望的值要小(因为工具栏和窗口边框的缘故)。有其它方法来计算窗口的尺寸吗?

参见CWnd::CalcWindowRect。
（99）我在文档类中设置了一个整型变量。但是，当我试图把该变量写入Serialize函数中的archive文件中时，出现了类型错误。而文档中的其它变量没有问题。为什么?

archive 类只重载某些类型的>>和<<操作符。“int”类型没有在其中，也许是因为int变量在Windows 3.1与Windows NT/95有所不同的缘故吧。“long”类型得到了支持，所以你可以把int类型改成long型。参见MFC帮助文件中CArchive类。
（100）如何控制菜单的大小?
我用MFC的CMenu生成了一个动态菜单(例如File,Edit,View...Help), 我想控制这个菜单的大小(长+高).

方法一:查找 WM_MEASUREITEM 和 MEASUREITEMSTRUCT.

方法二:查询系统::GetSystemMetric(SM_CXMENUSIZE).

     /* 你可以通过如下代码来获得文本的大小:

        (A)获得被使用的字体 */

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

          (B) 获得菜单项的文本: */

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

（101）改变LVIS_SELECTED的状态颜色?
我想将CListCtrl项和CTreeCtrl项在LVIS_SELECTED状态时的颜色变灰.

方法一:查找函数CustomDraw,它是IE4提供的公共控制,允许有你自己的代码.

方法二:生成一个draw控件,然后在DrawItem中处理文本颜色.

（102）如何只存储文档的某一部分?
我只想存储文档的某一部分,能否象使用文件一样使用文档?(也就是有定位函数).将每个CArchive类设置为CFile类的派生类,这样你就能使用Seek等成员函数.

（103）保存工具条菜单有bug吗?

使用浮动菜单条时,SaveBarState和LoadBarState出现了问题.如果菜单是浮动的,重起应用程序时它会出现在左上角,而它固定在屏幕其它位置时,下一次启动就会出现在该位置,这是什么原因?你试试这个PToolBar->Create(this,...,ID_MYTOOLBAR);

你的工具条需要包括id,而不是象默认的工具条那样.

（104）Tip of the day的bug

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
（105）如何可以让我的程序可以显示在其它的窗口上面?
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
（106）如何控制窗口框架的最大最小尺寸?

要控制一个框架的的最大最小尺寸,你需要做两件事情.在CFrameWnd的继承类中处理消息WM_GETMINMAXINFO,结构MINMAXINFO设置了整个窗口类的限制,因此记住要考虑工具条,卷动条等等的大小.

// 最大最小尺寸的象素点 - 示例

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
（107）如何改变窗口框架的颜色?

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
（108）如何将应用程序窗口置于屏幕正中?

要将你的应用程序窗口放置在屏幕正中央,只须在MainFrame的OnCreate函数中加入:

CenterWindow( GetDesktopWindow() )

