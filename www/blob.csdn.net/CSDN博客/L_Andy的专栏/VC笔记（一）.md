# VC笔记（一） - L_Andy的专栏 - CSDN博客

2012年10月15日 09:54:25[卡哥](https://me.csdn.net/L_Andy)阅读数：2164标签：[dll																[mfc																[null																[windows																[buffer																[microsoft](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[VC++笔记](https://blog.csdn.net/L_Andy/article/category/1925951)


张明奇（卡哥）

********vc中debug版程序正常但release版崩溃******* 

I. 内存分配问题 

1. 变量未初始化。 

为debug中会自动给变量初始化found=FALSE,而在release版中 

则不会。所以尽可能的给变量、类或结构初始化。 

2. 数据溢出的问题

如：char buffer[10]; 

int counter; 

lstrcpy(buffer, "abcdefghik"); 

在debug版中buffer的NULL覆盖了counter的高位，但是除非counter>16M,什么问题也没 

有。但是在release版中，counter可能被放在寄存器中，这样NULL就覆盖了buffer下面 

的空间，可能就是函数的返回地址，这将导致ACCESS ERROR。 

3. DEBUG版和RELEASE版的内存分配方式是不同的 。如果你在DEBUG版中申请 

ele 为 6*sizeof(DWORD)=24bytes,实际上分配给你的是32bytes（debug版以32bytes 

为单位分配）， 而在release版，分配给你的就是24bytes（release版以8bytes为单位 

），所以在debug版中如果你写ele[6],可能不会有什么问题，而在release版中，就有A 

CCESS VIOLATE。 

II. ASSERT和VERIFY 

1. ASSERT在Release版本中是不会被编译的。 

假如你在这些语句中加了程序中必须要有的代 

码 

比如 

ASSERT(pNewObj = new CMyClass); 

pNewObj->MyFunction(); 

这种时候Release版本中的pNewObj不会分配到空间 

所以执行到下一个语句的时候程序会报该程序执行了非法操作的错误。这时可以用VERIFY 

III. 参数问题： 

自定义消息的处理函数，必须定义如下： 

afx_msg LRESULT OnMyMessage(WPARAM, LPARAM); 

返回值必须是HRESULT型，否则Debug会过，而Release出错 

IV. 内存分配

保证数据创建和清除的统一性：如果一个DLL提供一个能够创建数据的函数，那么这个D 

LL同时应该提供一个函数销毁这些数据。数据的创建和清除应该在同一个层次上。 

V. DLL的灾难

人们将不同版本DLL混合造成的不一致性形象的称为 “动态连接库的地狱“(DLL Hell) 

如果你的程序使用你自己的DLL时请注意： 

1. 不能将debug和release版的DLL混合在一起使用。debug都是debug版，releas

e版都是release版。 

解决办法是将debug和release的程序分别放在主程序的debug和release目录下 

2. 千万不要以为静态连接库会解决问题，那只会使情况更糟糕 

VI. RELEASE板中的调试 ： 

1. 将ASSERT() 改为 VERIFY() 。找出定义在"#ifdef _DEBUG"中的代码，如果 

在RELEASE版本中需要这些代码请将他们移到定义外。查找TRACE(...)中代码，因为这些 

代码在RELEASE中也不被编译。 请认真检查那些在RELEASE中需要的代码是否并没有被便 

宜。 

2. 变量的初始化所带来的不同，在不同的系统，或是在DEBUG/RELEASE版本间 

都存在这样的差异，所以请对变量进行初始化。 

3. 是否在编译时已经有了警告?请将警告级别设置为3或4,然后保证在编译时没 

有警告出现. 

VII. 将Project Settings" 中 "C++/C " 项目下优化选项改为Disbale（Debug）。编 

译器的优化可能导致许多意想不到的错误，请参考[url]http://www.pgh.net/~newcomer/deb[/url] 

ug_release.htm 

1. 此外对RELEASE版本的软件也可以进行调试，请做如下改动： 

在"Project Settings" 中 "C++/C " 项目下设置 "category" 为 "General" 并且将"D 

ebug Info"设置为 "Program Database"。 

在"Link"项目下选中"Generate Debug Info"检查框。 

"Rebuild All" 

如此做法会产生的一些限制： 

无法获得在MFC DLL中的变量的值。 

必须对该软件所使用的所有DLL工程都进行改动。 

************************************************************* 

*******句柄是一个标识符，是拿来标识对象或者项目***** 

的，它就象我们的姓名一样，每个人都会有一个，不同的人的姓名不一样，但是，也可 

能有一个名字和你一样的人。从数据类型上来看它只是一个16位的无符号整数。应用程 

序几乎总是通过调用一个WINDOWS函数来获得一个句柄，之后其他的WINDOWS函数就可以 

使用该句柄，以引用相应的对象 

通用的句柄，就是HANDLE，比如下面的语句： 

HINSTANCE hInstance； 

可以改成： 

HANDLE hInstance； 

一般情况下我们在编程的时候，给应用程序分配的 

内存都是可以移动的或者是可以丢弃的，这样能使有限的内存资源充分利用，所以，在 

某一个时候我们分配的那块内存的地址是不确定的，因为他是可以移动的，所以得先锁 

定那块内存块，这儿应用程序需要调用API函数GlobalLock函数来锁定句柄。如下： 

lpMem=GlobalLock(hMem); 

这样应用程序才能存取这块内存。 

********************************************************** 
要使程序的主窗口不可见，并且不在任务栏上出现任务按钮，要做到这两点，需分 

别设置主边框窗口的风格和扩展风格： 

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 

{ 

cs.style =WS_POPUP;//使主窗口不可见 

cs.dwExStyle |=WS_EX_TOOLWINDOW;//不显示任务按钮 

return CFrameWnd::PreCreateWindow(cs); 

} 

************************************************* 
**CString str = "Hello\r\nHow are you\r\n"; 

: 为什么pDC->TextOut(0, 0, str);显示时不能够自动换行？

如果你的视图类是CEditView,这样做是行的通的 

但是,如果在CView里面,这样就不行,这也很容易理解 

CView里面没有行的概念,它就像按象素编址的白纸(一个二维平面) 

在白纸上换行,显然是行不通的,因为它不知道下一行的具体位置 

************************************* 
*我想在子线程中直接给视图窗口绘图，而不调用视图类的函数OnDraw()， 

在子线程中获得资源， 

可以在子线程的构造函数中传入CMyThread（CWnd* pWnd，HDC hDC，。。） 

这样，在主线程中创建子线程时 mythread（this，m_pDC->m_hDC) 

其中m_pDC=m_pDC=new CClientDC(this); 

********************************** 

*我在程序中需要生成随机数， 

: 但是rand（）生成的是0～指定的数之间的随机数 

: 例如我想生成10～20之间的5个随机数，应该如何做？ 

int result[5]; 

srand(); 

for(int i=0;i<5;i++) 

{ 

result[i]=rand()%11+10; 

srand(); 

} 

************************************** 

*VC中让程序发出声因

beep() 

******************************** 

*获取当前屏幕分辨率

int nScreenWidth=GetSystemMetrics(SM_CXSCREEN); 

int nScreenHeight=GetSystemMetrics(SM_CYSCREEN); 

************************************ 

*把消息从一个view发到另外一个view 
在CWinApp里面保存view的指针。然后再任意地方就可以通过AfxGetApp()调用获得 

view的指针。That is ok! 

*********************************** 

*如何在视图类函数中得到HDC句柄

得到可以用GetDC，释放用ReleaseDC，句柄是CDC的成员变量 

返回的是CDC对象，但里面有一个成员变量，好像是m_hCDC之类的 

是句柄 

********************************* 

*注册控件

regsvr32 

-************************************** 

*CTypePtrArray跟CObArray本质区别

CTypePtrArray有两种基本框架类型,CObArray和CPtrArray,里面的数据类型可以是对象 

或者指向对象的指针;所以,它是一个模板类,它的可串行化依赖于它到底是以CObArray 

还是以CPtrArray为基本框架,它是类型安全的. 

CObArray,顾名思义,是对象的数组,所以里面是存放对象的.它不是模板类,可串行化, 

不是类型安全的. 

************************************** 

*vc中的extern "c" 如何使用？ 

假设某个C 函数的声明如下： 

void foo(int x, int y); 

该函数被C 编译器编译后在库中的名字为_foo，而C++编译器则会产生像_foo_int_int 

之类的名字用来支持函数重载和类型安全连接。由于编译后的名字不同，C++程序不能 

直接调用C 函数。C++提供了一个C 连接交换指定符号extern“C”来解决这个问题。 

例如： 

extern “C” 

{ 

void foo(int x, int y); 

. // 其它函数 

} 

或者写成 

extern “C” 

{ 

#include “myheader.h” 

. // 其它C 头文件 

} 

这就告诉C++编译译器，函数foo 是个C 连接，应该到库中找名字_foo 而不是找 

_foo_int_int。C++编译器开发商已经对C 标准库的头文件作了extern“C”处理， 

***************************************************** 

*VC++下怎样设置栈的大小

方法一：STACKSIZE 定义.def文件 

语法：STACKSIZE reserve[,commit] 

reserve：栈的大小；commit：可选项，与操作系统有关，在NT上只一次分配物理内存 

的大小 

方法二：设定/STACK 

打开工程，依次操作菜单如下：Project->Setting->Link，在Category 中选中Outpu 

t，然后 

在Reserve中设定堆栈的最大值和commit。 

注意：reserve默认值为1MB,最小值为4Byte；commit是保留在虚拟内存的页文件里面，它 

设置的较 

大会使栈开辟较大的值，可能增加内存的开销和启动时间 

//详细讲解：

具体可以看这个

一般接触到的是1M

*reserve* 值指定虚拟内存中的总的堆栈分配。对于 x86 和 x64 计算机，默认堆栈大小为 1 MB。在 Itanium 芯片组上，默认大小为 4 MB。

*commit* 取决于操作系统所作的解释。在 Windows NT 和 Windows 2000 中，它指定一次分配的物理内存量。提交的虚拟内存导致空间被保留在页面文件中。更高的*commit* 值在应用程序需要更多堆栈空间时可节省时间，但会增加内存需求并有可能延长启动时间。对于 x86 和 x64 计算机，默认提交值为 4 KB。在 Itanium
 芯片组上，默认值为 16 KB。

以十进制或 C 语言表示法指定 *reserve* 值和 *commit* 值。

设置堆栈大小的另一种方法是使用模块定义 (.def) 文件中的 [STACKSIZE](http://msdn.microsoft.com/zh-cn/library/333azx74.aspx) 语句。如果两者都指定，则 **STACKSIZE** 重写堆栈分配 (/STACK) 选项。可以使用[EDITBIN](http://msdn.microsoft.com/zh-cn/library/xd3shwhf.aspx)
 工具在生成 .exe 文件之后更改堆栈大小。

修改方法

### 在 Visual Studio 开发环境中设置此链接器选项

- 
打开此项目的“属性页”对话框  单击“链接器”文件夹。

- 
单击“系统”属性页。

- 
修改下列任一属性： 
- 
**堆栈提交大小  指定物理内存中堆栈分配的合计大小**

- 
**堆栈保留大小  指定虚拟内存中对照分配的合计大小**

********************************************************** 
*鼠标移动到适当的位置时改变光标

void CM_curosrView::OnMouseMove(UINT nFlags, CPoint point) 

{ 

// TODO: Add your message handler code here and/or call default 

SetCursor(Cross); 

// 

CView::OnMouseMove(nFlags, point); 

} 

其中HCURSOR Cross在构造函数中被初始化成 

Cross = AfxGetApp()->LoadStandardCursor(IDC_CROSS); 

只要移动，即为十字标 

******************************************************** 

*如何发送一个关于某个按钮单击的消息

试试这两种： 

1、GetDlgItem(IDOK)->PostMessage(BM_CLICK,0,0); 

这种是某位高人说的，没试过。这种是直接发送给按钮窗口的，是窗口消息。 

2、PostMessage(WM_COMMAND,IDOK|BN_CLICKED<<16,GetDlgItem(IDOK)->m_hWnd); 

这个是我自己找的一种方法。这个是发送给按钮父窗口的,是通知消息。 

****************************************************** 

*什么函数能获取本地IP

虽然感觉很滥，直接调用ipconfig。 

这个程序肯定是每台机器有得， 

windows自己保证，所以你调用就可以方便获得了。 

********************************** 

*得到windows和system目录路径的函数

getwindowsdirectory 

getsystemdirectory 

*********************************** 

1:使用vc开发项目时， 

常会遇到这种情况： 

即明明只改动了一个文件，却要把整个项目全部重新编译连接一次。 

刚刚连接好，一运行，又提示重新编译连接一次。非常讨厌。 

这是因为出现了　未来文件 的缘故。（vc5的bug?) 

解决方法：找出对应文件夹下的debug目录，将未来文件全部delete, 

再rebuild all一次。(未来 文件即其创建和修改时间都比系统时间靠后） 

2: 
有时，workspace中的classview显示混乱。其表现如下： 

1:添加的成员变量或函数不能显示； 

2:即使显示出来了变量或函数，双击后不能跳至正确的位置。 

解决方法： 

删除.ncb文件，再rebuld all. 

3: 
如何干净的删除一个类？ 

1:先从workspace中的FileView中删除对应的.h和.cpp文件。 

2.再关闭项目，从实际的文件夹中删除对应的.h和.cpp文件。 

3.灾删除.clw文件。 

打开项目，rebuld all。 

4: 
初学者常有这样的疑惑： 

直接望工程文件里加入一个CPP原文件再编译连接的话 

老是提示没有找到预编译头 

解决方法： 

#include "stdafx.h" 

5: 
如何向项目中加入自己定义的类? 

方法很多，介绍一个简便的： 

选择Insert/New Class菜单 

弹出对话框； 

选择Class Type为generic; 

输入类名。 

当然，也可以自己决定该类的基类 

6． 通过DLL共享变量

a. 用#pragma data_seg(“segname”) 创建一个节 

＃pragma data_seg(“share”) 

LONG g_lShareData=0; 

#pragma data_seg() 

b. 在连接器的命令行中使用/SECTION:name attributes开关 

//SECTION:share,RWS 

或在源代码中嵌入连接开关： 

#pragma comment(linker,”/SECTION:share,RWS”) 

7． 动态连接库的隐式连接

a. DLL输出函数的头文件：输出函数前加_declspec(dllexport)；EXE输入函数的头文件： 

输入函数前加_delcspec(dllimport)。 

b. 在EXE工程项目设置的Link页面的Object/Libaray Modules 中添加DLL对应的LIB文件（ 

该文件是在生成DLL的同时生成的）。 

8． 用FindFirstChangeNotification(…)来检测文件系统的变化。 

先用FindFirstChangeNotification登记要监测的文件夹，然后用WaitForSingleObject等 

函数来等待文件变化事件的到来，并处理该时间。在处理后如果需要继续监测，那么还要 

调用FindNextChangeNotification(…)。 

*********************************************** 

*怎样在c++里得到文件长度

HANDLE hFile; 

hFile = CreateFile(lpcTheFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, 

CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

if (hFile != INVALID_HANDLE_VALUE) { 

DWORD dwFileSize = GetFileSize(hFile, NULL); 

CloseHandle(hFile); 

} 

********************************************* 

*MessageBox()与AfxMessageBox()的区别

C 中可调用的涵数大致可分三类： 

1.类自己的涵数，只对类自己的数据成员有作用； 

2.AFX小组在设计 Application Framworks 时设计的全局函数，多冠在Afx前缀，在包含 

了MFC库/框架的工程中可用； 

3.Windows API的全局函数。对所有Windows平台下的程序设计都可以调用，如Vb,Vc,De 

phi等等。 

你说的 MessageBox是属于 CWnd 类的成员函数，只能在 CWnd 和CWnd的派生类的对象中 

调用；AfxMessageBox则可在任何地方调用。 

另外对应的还有： ::MessageBox()这个windows API的全局函数。 

上述中1和3一般有一个区别，就是1要比3少一个参数，即窗口句柄。大家知道，这个句 

柄是通过 this 指针曲折转换得到的，不用程序员操心了。 

************************************************* 

*VC开发小技巧20个 

一、打开CD-ROM

mciSendString("Set cdAudio door open wait",NULL,0,NULL); 

二、关闭CD_ROM

mciSendString("Set cdAudio door closed wait",NULL,0,NULL); 

三、关闭计算机

OSVERSIONINFO OsVersionInfo; //包含操作系统版本信息的数据结构 

OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 

GetVersionEx(&OsVersionInfo); //获取操作系统版本信息 

if(OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) 

{ 

//Windows98,调用ExitWindowsEx()函数重新启动计算机 

DWORD dwReserved; 

ExitWindowsEx(EWX_REBOOT,dwReserved); //可以改变第一个参数，实现注销用户、 

//关机、关闭电源等操作 

// 退出前的一些处理程序 

} 

四、重启计算机

typedef int (CALLBACK *SHUTDOWNDLG)(int); //显示关机对话框函数的指针 

HINSTANCE hInst = LoadLibrary("shell32.dll"); //装入shell32.dll 

SHUTDOWNDLG ShutDownDialog; //指向shell32.dll库中显示关机对话框函数的指针 

if(hInst != NULL) 

{ 

//获得函数的地址并调用之 

ShutDownDialog = (SHUTDOWNDLG)GetProcAddress(hInst,(LPSTR)60); 

(*ShutDownDialog)(0); 

} 

五、枚举所有字体

LOGFONT lf; 

lf.lfCharSet = DEFAULT_CHARSET; // Initialize the LOGFONT structure 

strcpy(lf.lfFaceName,""); 

CClientDC dc (this); 

// Enumerate the font families 

::EnumFontFamiliesEx((HDC) dc,&lf, 

(FONTENUMPROC) EnumFontFamProc,(LPARAM) this,0); 

//枚举函数

int CALLBACK EnumFontFamProc(LPENUMLOGFONT lpelf, 

LPNEWTEXTMETRIC lpntm,DWORD nFontType,long lparam) 

{ 

// Create a pointer to the dialog window 

CDay7Dlg* pWnd = (CDay7Dlg*) lparam; 

// add the font name to the list box 

pWnd ->m_ctlFontList.AddString(lpelf ->elfLogFont.lfFaceName); 

// Return 1 to continue font enumeration 

return 1; 

} 

其中m_ctlFontList是一个列表控件变量 

六、一次只运行一个程序实例，如果已运行则退出 

if( FindWindow(NULL,"程序标题")) exit(0); 

七、得到当前鼠标所在位置

CPoint pt; 

GetCursorPos(&pt); //得到位置 

八、上下文菜单事件触发事件：OnContextMenu事件 

九、显示和隐藏程序菜单

CWnd *pWnd=AfxGetMainWnd(); 

if(b_m) //隐藏菜单 

{ 

pWnd->SetMenu(NULL); 

pWnd->DrawMenuBar(); 

b_m=false; 

} 

else 

{ 

CMenu menu; 

menu.LoadMenu(IDR_MAINFRAME); ////显示菜单 也可改变菜单项 

pWnd->SetMenu(&menu); 

pWnd->DrawMenuBar(); 

b_m=true; 

menu.Detach(); 

} 

十、获取可执行文件的图标

HICON hIcon=::ExtractIcon(AfxGetInstanceHandle(),_T("NotePad.exe"),0); 

if (hIcon &&hIcon!=(HICON)-1) 

{ 

pDC->DrawIcon(10,10,hIcon); 

} 

DestroyIcon(hIcon); 

十一、窗口自动靠边程序演示

BOOL AdjustPos(CRect* lpRect) 

{//自动靠边 

int iSX=GetSystemMetrics(SM_CXFULLSCREEN); 

int iSY=GetSystemMetrics(SM_CYFULLSCREEN); 

RECT rWorkArea; 

BOOL bResult = SystemParametersInfo(SPI_GETWORKAREA, sizeof(RECT), &rWorkAre 

a, 0); 

CRect rcWA; 

if(!bResult) 

{//如果调用不成功就利用GetSystemMetrics获取屏幕面积 

rcWA=CRect(0,0,iSX,iSY); 

} 

else 

rcWA=rWorkArea; 

int iX=lpRect->left; 

int iY=lpRect->top; 

if(iX < rcWA.left + DETASTEP && iX!=rcWA.left) 

{//调整左 

//pWnd->SetWindowPos(NULL,rcWA.left,iY,0,0,SWP_NOSIZE); 

lpRect->OffsetRect(rcWA.left-iX,0); 

AdjustPos(lpRect); 

return TRUE; 

} 

if(iY < rcWA.top + DETASTEP && iY!=rcWA.top) 

{//调整上 

//pWnd->SetWindowPos(NULL ,iX,rcWA.top,0,0,SWP_NOSIZE); 

lpRect->OffsetRect(0,rcWA.top-iY); 

AdjustPos(lpRect); 

return TRUE; 

} 

if(iX + lpRect->Width() > rcWA.right - DETASTEP && iX !=rcWA.right-lpRect->W 

idth()) 

{//调整右 

//pWnd->SetWindowPos(NULL ,rcWA.right-rcW.Width(),iY,0,0,SWP_NOSIZE); 

lpRect->OffsetRect(rcWA.right-lpRect->right,0); 

AdjustPos(lpRect); 

return TRUE; 

} 

if(iY + lpRect->Height() > rcWA.bottom - DETASTEP && iY !=rcWA.bottom-lpRect 

->Height()) 

{//调整下 

//pWnd->SetWindowPos(NULL ,iX,rcWA.bottom-rcW.Height(),0,0,SWP_NOSIZE); 

lpRect->OffsetRect(0,rcWA.bottom-lpRect->bottom); 

return TRUE; 

} 

return FALSE; 

} 

//然后在ONMOVEING事件中使用所下过程调用 

CRect r=*pRect; 

AdjustPos(&r); 

*pRect=(RECT)r; 

十二、给系统菜单添加一个菜单项

给系统菜单添加一个菜单项需要进行下述三个步骤： 

首先，使用Resource Symbols对话（在View菜单中选择Resource Symbols．．．可以显 

示该对话）定义菜单项ID，该ID应大于0x0F而小于0xF000； 

其次，调用CWnd::GetSystemMenu获取系统菜单的指针并调用CWnd:: Appendmenu将菜单 

项添加到菜单中。下例给系统菜单添加两个新的 

int CMainFrame:: OnCreate (LPCREATESTRUCT lpCreateStruct) 

{ 

… 

//Make sure system menu item is in the right range. 

ASSERT(IDM_MYSYSITEM<0xF000); 

//Get pointer to system menu. 

CMenu* pSysMenu=GetSystemMenu(FALSE); 

ASSERT_VALID(pSysMenu); 

//Add a separator and our menu item to system menu. 

CString StrMenuItem(_T ("New menu item")); 

pSysMenu->AppendMenu(MF_SEPARATOR); 

pSysMenu->AppendMenu(MF_STRING, IDM_MYSYSITEM, StrMenuItem); 

… 

} 

十三、运行其它程序

//1、运行EMAIL或网址

char szMailAddress[80]; 

strcpy(szMailAddress,"mailto:netvc@21cn.com"); 

ShellExecute(NULL, "open", szMailAddress, NULL, NULL, SW_SHOWNORMAL); 

//2、运行可执行程序

WinExec("notepad.exe",SW_SHOW); //运行计事本 

十四、动态增加或删除菜单

1、 增加菜单

//添加 

CMenu *mainmenu; 

mainmenu=AfxGetMainWnd()->GetMenu(); //得到主菜单 

(mainmenu->GetSubMenu (0))->AppendMenu (MF_SEPARATOR);//添加分隔符 

(mainmenu->GetSubMenu (0))->AppendMenu(MF_STRING,ID_APP_ABOUT,_T("Always on 

&Top")); //添加新的菜单项 

DrawMenuBar(); //重画菜单 

2、 删除菜单

//删除 

CMenu *mainmenu; 

mainmenu=AfxGetMainWnd()->GetMenu(); //得到主菜单 

CString str ; 

for(int i=(mainmenu->GetSubMenu (0))->GetMenuItemCount()-1;i>=0;i--) //取得菜 

单的项数。 

{ 

(mainmenu->GetSubMenu (0))->GetMenuString(i,str,MF_BYPOSITION); 

//将指定菜单项的标签拷贝到指定的缓冲区。MF_BYPOSITION的解释见上。 

if(str=="Always on &Top") //如果是刚才我们增加的菜单项，则删除。 

{ 

(mainmenu->GetSubMenu (0))->DeleteMenu(i,MF_BYPOSITION); 

break; 

} 

十五、改变应用程序的图标

静态更改： 修改图标资源IDR_MAINFRAME。它有两个图标，一个是16*16的，另一个是3 

2*32的，注意要一起修改。 

动态更改： 向主窗口发送WM_SETICON消息.代码如下： 

HICON hIcon=AfxGetApp()->LoadIcon(IDI_ICON); 

ASSERT(hIcon); 

AfxGetMainWnd()->SendMessage(WM_SETICON,TRUE,(LPARAM)hIcon); 

十六、另一种改变窗口标题的方法

使用语句 CWnd* m_pCWnd = AfxGetMainWnd( )，然后，再以如下形式调用SetWindowTe 

xt()函数： 

SetWindowText( *m_pCWnd,(LPCTSTR)m_WindowText)；// m_WindowText可以是一个CSt 

ring类的变量。 

十七、剪切板上通过增强元文件拷贝图像数据

下面代码拷贝通过元文件拷贝图像数据到任何应用程序，其可以放置在CView派生类的函 

数中。 

CMetaFileDC * m_pMetaDC = new CMetaFileDC(); 

m_pMetaDC->CreateEnhanced(GetDC(),NULL,NULL,"whatever"); 

//draw meta file 

//do what ever you want to do: bitmaps, lines, text... 

//close meta file dc and prepare for clipboard; 

HENHMETAFILE hMF = m_pMetaDC->CloseEnhanced(); 

//copy to clipboard 

OpenClipboard(); 

EmptyClipboard(); 

::SetClipboardData(CF_ENHMETAFILE,hMF); 

CloseClipboard(); 

//DeleteMetaFile(hMF); 

delete m_pMetaDC; 

十八、剪切板上文本数据的传送

把文本放置到剪接板上： 

CString source; 

//put your text in source 

if(OpenClipboard()) 

{ 

HGLOBAL clipbuffer; 

char * buffer; 

EmptyClipboard(); 

clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1); 

buffer = (char*)GlobalLock(clipbuffer); 

strcpy(buffer, LPCSTR(source)); 

GlobalUnlock(clipbuffer); 

SetClipboardData(CF_TEXT,clipbuffer); 

CloseClipboard(); 

} 

从剪接板上获取文本： 

char * buffer; 

if(OpenClipboard()) 

{ 

buffer = (char*)GetClipboardData(CF_TEXT); 

//do something with buffer here 

//before it goes out of scope 

} 

CloseClipboard(); 

十九、将捕捉屏幕图像到剪切版中

void CShowBmpInDlgDlg::OnCutScreen() 

{ 

ShowWindow(SW_HIDE); 

RECT r_bmp={0,0,::GetSystemMetrics(SM_CXSCREEN), 

::GetSystemMetrics(SM_CYSCREEN)}; 

HBITMAP hBitmap; 

hBitmap=CopyScreenToBitmap(&r_bmp); 

//hWnd为程序窗口句柄 

if (OpenClipboard()) 

{ 

EmptyClipboard(); 

SetClipboardData(CF_BITMAP, hBitmap); 

CloseClipboard(); 

} 

ShowWindow(SW_SHOW); 

} 

HBITMAP CShowBmpInDlgDlg::CopyScreenToBitmap(LPRECT lpRect) 

{ 

//lpRect 代表选定区域 

{ 

HDC hScrDC, hMemDC; 

// 屏幕和内存设备描述表 

HBITMAP hBitmap, hOldBitmap; 

// 位图句柄 

int nX, nY, nX2, nY2; 

// 选定区域坐标 

int nWidth, nHeight; 

// 位图宽度和高度 

int xScrn, yScrn; 

// 聊环直媛? 

// 确保选定区域不为空矩形 

if (IsRectEmpty(lpRect)) 

return NULL; 

//为屏幕创建设备描述表 

hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 

//为屏幕设备描述表创建兼容的内存设备描述表 

hMemDC = CreateCompatibleDC(hScrDC); 

// 获得选定区域坐标 

nX = lpRect->left; 

nY = lpRect->top; 

nX2 = lpRect->right; 

nY2 = lpRect->bottom; 

// 获得 聊环直媛? 

xScrn = GetDeviceCaps(hScrDC, HORZRES); 

yScrn = GetDeviceCaps(hScrDC, VERTRES); 

//确保选定区域是可见的 

if (nX<0) 

nX = 0; 

if (nY<0) 

nY = 0; 

if (nX2>xScrn) 

nX2 = xScrn; 

if (nY2>yScrn) 

nY2 = yScrn; 

nWidth = nX2 - nX; 

nHeight = nY2 - nY; 

// 创建一个与屏幕设备描述表兼容的位图 

hBitmap = CreateCompatibleBitmap 

(hScrDC, nWidth, nHeight); 

// 把新位图选到内存设备描述表中 

hOldBitmap =(HBITMAP)SelectObject(hMemDC, hBitmap); 

// 把屏幕设备描述表拷贝到内存设备描述表中 

BitBlt(hMemDC, 0, 0, nWidth, nHeight, 

hScrDC, nX, nY, SRCCOPY); 

//得到屏幕位图的句柄 

hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap); 

//清除 

DeleteDC(hScrDC); 

DeleteDC(hMemDC); 

// 返回位图句柄 

return hBitmap; 

} 

} 

二十、如何将位图缩放显示在Static控件中

//在Staic控件内显示位图 

void CShowBmpInDlgDlg::ShowBmpInStaic() 

{ 

CBitmap hbmp; 

HBITMAP hbitmap; 

//将pStatic指向要显示的地方 

CStatic *pStaic; 

pStaic=(CStatic*)GetDlgItem(IDC_IMAGE); 

//装载资源 MM.bmp是我的一个文件名，用你的替换 

hbitmap=(HBITMAP)::LoadImage (::AfxGetInstanceHandle(),"MM.bmp", 

IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION); 

hbmp.Attach(hbitmap); 

//获取图片格式 

BITMAP bm; 

hbmp.GetBitmap(&bm); 

CDC dcMem; 

dcMem.CreateCompatibleDC(GetDC()); 

CBitmap *poldBitmap=(CBitmap*)dcMem.SelectObject(hbmp); 

CRect lRect; 

pStaic->GetClientRect(&lRect); 

lRect.NormalizeRect(); 

//显示位图 

pStaic->GetDC()->StretchBlt(lRect.left ,lRect.top ,lRect.Width(),lRect.Heigh 

t(), 

&dcMem,0 ,0,bm.bmWidth,bm.bmHeight,SRCCOPY); 

dcMem.SelectObject(&poldBitmap); 

} 

*************************************** 

*如何在程序里知道硬盘的剩余空间

CString s; 

ULONGLONG 

ullFreeBytesAvailableToCaller, 

ullTotalNumberOfBytes, 

ullTotalNumberOfFreeBytes; 

if(GetDiskFreeSpaceEx( 

_T("C:\\"), 

(PULARGE_INTEGER)&ullFreeBytesAvailableToCaller, 

(PULARGE_INTEGER)&ullTotalNumberOfBytes, 

(PULARGE_INTEGER)&ullTotalNumberOfFreeBytes)) 

{ 

s.Format(_T("磁盘剩余空间:%lKB"), ullTotalNumberOfFreeBytes/1024); 

MessageBox(s); 

} 

*********************************************** 

*如何unable对话框上的x

标 题: Re: 如何unable对话框上的x？ 

CMenu *pMenu=this->GetSystemMenu(FALSE); 

pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_GRAYED); 

【 在 eleven (eleven) 的大作中提到: 】 

: 但保留最大化和最小化按钮？ 

: 谢了先！ 

********************************** 

*怎么把CString型的数字转变成int

先得到CString的字串char*， 

再用atoi 

****************************************** 

*已知HDC,怎么得到CDC 
static CDC* PASCAL FromHandle( HDC hDC ); 

CDC* GetDC( ); 

*********************************** 

*把某个盘设为共享，有这样的函

NetShareAdd 

************************************** 

*如何映射网络驱动器

WNetAddConnection 

********************** 

*注册表中有没有启动加载DLL的设置

HKEY_LOCAL_MACHINE\Software\Microsoft 

\Windows\CurrentVersion\SharedDLLs.好像是的吧 

********************************* 

*怎样把进程注册为服务

HKEY_CLASSES_ROOT\APPID 

****************************** 

*在应用程序类中，有个InitInstance()函数， 

: 在这个函数中假如这样的两行： 

: RegisterShellFileType(); 

: EnableShellOpen(); 

RegisterShellFileType()作用就是允许文件关联。如notepad可以打开text文本文件， 

如果你创建了一个新的文件类型，操作系统默认以你的程序 

打开你创建的文件类型。 

EnableShellOpen():当你拖动文件到你程序时，可以打开相应的文档。入IE,当你拖动文 

档到IE 

时可以打开该文档。 

******************************* 

*怎么得到DLL的输出函数

使用dumpbin.exe 

不过没有参************ 

*************************************** 

*谁给我解答一下SPY++用法的几点疑惑

: （1）用 find window 看到 子窗口和主窗口properties中 

: 的Instance handle怎么会不一样？ 

: 我想都应该是程序的实例句柄（唯一）。 

: 

: 

: （2）WINDOW View中有很多"Default IME" IME

: 表示什么窗口？ 

首先必须明白的是: 对于按钮,列表框等等东西,他们都是一个窗口,只是窗口 

的风格不一样,因此它们也有自己句柄. 

Default IME是指缺省的输入法,每个窗口都有一个缺省的输入法. 

********************************************** 

*如何调用DLL中的资源(位图...) 
使用方法如下： 

HMODULE hMoudle = (HMODULE)LoadLibrary("XXXXX.dll"); 

HRSRC hRes = FindResource(hModule,MAKEINTRESOURCE(IDB_MYBITMAP),"RT_BITMAP") 

; 

HBITMAP hBmp = (HBITMAP)LoadResource(hModule,hRes); 

Remarks 

Both Windows 95 and Windows NT automatically free resources. You do not need 

to call the FreeResource function to free a resource loaded by using the Lo 

adResource function. 

但是这一句是需要的: 

FreeLibrary(hMoudle); 

***************************************** 

*如何在VC程序中执行*.lnk文件

Try ShellExecuteEx(). 

************************************ 

*把指定目录下的所有文件拷到另一个指定目

SHFILEOPSTRUCT fileop; 

CString Title = "Copy Files"; 

fileop.hwnd = this->m_hWnd; 

fileop.pFrom = "C:/Winnt/*.*"; 

fileop.pTo = "d:/temp" 

fileop.wFunc = FO_COPY; 

fileop.fFlags = FOF_NOCONFIRMMKDIR; 

fileop.lpszProgressTitle = (LPCSTR)Title; 

SHFileOperation(&fileop); 

******************************************** 

* 怎么得到主窗口的句柄

AfxGetMainWnd()->GetSafeHwnd() 

****************************************** 

*动态的声明一个二维数组(两个维数都未知)

double ** twoPtrAlloc(LONG h,LONG w) 

{ 

double ** pd; 

pd=new double* [h]; 

if(!pd) 

{ 

return NULL; 

} 

for (int i=0;i<h;i++) 

{ 

pd[i]=new double[w]; 

if(!pd[i]) 

{ 

return NULL; 

} 

} 

return pd; 

} 

************************************** 

*MDI程序中,在不同的子窗口中切换时系统会产生何种消息? 

WM_MDIACTIVATE 

**************************************************** 

*有效的跟踪WM_PAINT消息

可以在 

OnDraw中打印一个自增的数。看看运行时是否进入了 

OnDraw中，也可以用MessageBeep听听声音 

**************************************** 

*在非视图非文档类中得到视图指针 MDI

CMainFrame * pFrame=(CMainFrame *)(AfxGetApp()->m_pMainWnd()); 

ASSERT(pFrame!=NULL); 

(CYourView *)pView=(CYourView *)(pFrame->MDIGetActive()->GetActiveView()); 

ASSERT(pView!=NULL); 

********************************** 

*VC5中MFC DLL的分类及特点

　　在VC5中有三种形式的MFC DLL（在该DLL中可以使用和继承已有的MFC类)可供选择，即Regular statically linked to MFC DLL（标准静态链接MFC DLL）和Regular using the shared MFC DLL（标准动态链接MFC DLL）以及Extension MFC DLL（扩展MFC DLL）。第一种DLL的特点是，在编译时把使用的MFC代码加入到DLL中，因此，在使用该程序时不需要其他MFC动态链接类库的存在，但占用磁盘空间比较大；第二种DLL的特点是，在运行时，动态链接到MFC类库，因此减少了空间的占用，但是在运行时却依赖于MFC动态链接类库；这两种DLL既可以被MFC程序使用也可以被Win32程序使用。第三种DLL的特点类似于第二种，做为MFC类库的扩展，只能被MFC程序使用。

******************************************* 

*Win32全局钩子的运行机制

　　钩子实际上是一个处理消息的程序段，通过系统调用，把它挂入系统。每当特定的消息发出，在没有到达目的窗口前，钩子程序就先捕获该消息，亦即钩子函数先得到控制权。这时钩子函数即可以加工处理（改变）该消息，也可以不作处理而继续传递该消息，还可以强制结束消息的传递。对每种类型的钩子由系统来维护一个钩子链，最近安装的钩子放在链的开始，而最先安装的钩子放在最后，也就是后加入的先获得控制权。要实现Win32的系统钩子，必须调用SDK中的API函数SetWindowsHookEx来安装这个钩子函数，这个函数的原型是HHOOK
 SetWindowsHookEx(int idHook,HOOKPROC lpfn,HINSTANCE hMod,DWORD dwThreadId);，其中，第一个参数是钩子的类型；第二个参数是钩子函数的地址；第三个参数是包含钩子函数的模块句柄；第四个参数指定监视的线程。如果指定确定的线程，即为线程专用钩子；如果指定为空，即为全局钩子。其中，全局钩子函数必须包含在DLL（动态链接库）中，而线程专用钩子还可以包含在可执行文件中。得到控制权的钩子函数在完成对消息的处理后，如果想要该消息继续传递，那么它必须调用另外一个SDK中的API函数CallNextHookEx来传递它。钩子函数也可以通过直接返回TRUE来丢弃该消息，并阻止该消息的传递。

************************************************* 

*在VC5中全局共享数据的实现

　　在主文件中，用#pragma data_seg建立一个新的数据段并定义共享数据，其具体格式为： 

　　#pragma data_seg （"shareddata") 

　　HWND sharedwnd=NULL;//共享数据 

　　#pragma data_seg() 

　　仅定义一个数据段还不能达到共享数据的目的，还要告诉编译器该段的属性，有两种方法可以实现该目的（其效果是相同的），一种方法是在.DEF文件中加入如下语句： 

　　SETCTIONS 

　　shareddata READ WRITE SHARED 

　　另一种方法是在项目设置链接选项中加入如下语句： 

　　/SECTION:shareddata,rws 

*********************************************************** 

*怎么由句柄得到指针

CWnd有一个静态函数叫做FromHandlePermanent(HWND hWnd) 

如果是通过窗口句柄得到自己进程内的窗口指针，这个函数将会正常工作 

如果是得到其它进程的窗口指针就不对了 

创建的窗口在系统中有一张表维护指针和句柄之间的关系 

CWnd的FromHandlePermanent(HWND hWnd)就是通过这张表查找到窗口指针的 

CHandleMap* pMap = afxMapHWND();//首先取得这张表 

CWnd* pWnd = NULL; 

if (pMap != NULL) 

{ 

//查找窗口句柄对应的CWnd对象 

pWnd = (CWnd*)pMap->LookupPermanent(hWnd); 

ASSERT(pWnd == NULL || pWnd->m_hWnd == hWnd); 

} 

return pWnd; 

********************************************************* 

*怎么调试一个Automation服务器

Project->Setting->Debug->Program arguments 

加入/Embedding 

********************************************* 

*如何使窗口在一开始就不可见

看看有没有WS_HIDE，如果没有，则加入WS_MINIMIZED,然后在 

OnEraseBkGnd的消息里面加入： 

static BOOL bFirst=TRUE; 

if(bFirst) 

{ 

bFirst=FALSE; 

ShowWindow(SW_HIDE); 

return TRUE; 

} 

************************************************ 

*.dll如何在VC中应用

如果有这个动态连接库的调用格式资料的话,问题很简单. 

1)动态连接 

LoadLibery() 

GetProcessAddress() 

2)静态连接 

首先生成这个动态连接库的引入库; 

再用到这个库的文件开始加上 

#progma comment(lib,"libname"); 

************************************************ 

*新式的用CSocket获得本机地址方法。 

CSocket socket; 

socket.Create(); 

socket.Connect("pop3.sina.com.cn", 110);//或连接pop.tom.com等等 

CString strAddress; 

UINT uPort; 

socket.GetSockName( strAddress, uPort); 

CClientDC dc(this); 

dc.TextOut( 30, 30, "你的IP：" + strAddress); 

socket.Close();　　 

************************************************ 

*如何用VC创建ODBC数据源，而不是人工在控制面板的ODBC数据源创建？ 

如建一个为MyTax的Access 2000数据源，Tax.mdb在D:\ 

if( !SQLConfigDataSource( 

NULL, 

ODBC_ADD_DSN, //加在USER DSN，若要加在SYS DSN则为ODBC_ADD_SYSDSN 

"Microsoft Access Driver (*.mdb)", 

"DSN = MyTaxDBQ = D:\\Tax.mdb") 

//注意，XXXX.mdb必须为英文，若为中文在VC6会报错的，VC.NET 2003 没测试过 

) 

AfxMessageBox("创建失败！"); 

　要加上头文件#include <odbcinst.h>，在Link处加上odbccp32.lib。 

************************************************ 

*怎么样用VC调用 regsvr32.exe 命令

WinExec( "regsvr32.exe Test.dll", SW_SHOW); 

shellexecute 

************************************************ 

* 隐藏WINDOWS系统任务栏

::ShowWindow (::FindWindow("Shell_TrayWnd",NULL),SW_HIDE); 

************************************************ 

*全屏的实现关键代码

int CX = GetSystemMetrics(SM_CXSCREEN); 

int CY = GetSystemMetrics(SM_CYSCREEN); 

::MoveWindow(hwnd,0,0,CX,CY,TRUE); 

ShowWindow(hwnd, SW_SHOWMAXIMIZED); 

******************************** 

* 怎样使应用程序窗口在最前

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 

{ 

if( !CFrameWnd::PreCreateWindow(cs) ) 

return FALSE; 

// TODO: Modify the Window class or styles here by modifying 

// the CREATESTRUCT cs 

cs.dwExStyle|=WS_EX_TOPMOST; 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~only add this sentence 

return TRUE; 

} 

******************************** 

* 去除框架窗口的标题

LONG style=::GetWindowLong(m_wndToolBar,GWL_STYLE); 

style&=~(WS_CAPTION); 

::SetWindowLong(m_wndToolBar,GWL_STYLE,style); 

******************************** 

* 怎样除掉工具栏的标题（浮动时出现的）

LONG style=::GetWindowLong(m_wndToolBar.GetParentFrame()->m_hWnd,GWL_STYLE); 

style&=~(WS_CAPTION); 

::SetWindowLong(m_wndToolBar.GetParentFrame()->m_hWnd,GWL_STYLE,style); 

m_wndToolBar.GetParentFrame()->ModifyStyle(0,MFS_MOVEFRAME|MFS_4THICKFRAME|MFS_THICKFRAME);

注意工具栏的父窗口并不是你的CMainFrame!! 

******************************** 

*把cstring对象转换成普通字符串char

CString str="***********"; 

char *p=str.GetBuffer(str.GetLength()+1); 

******************************** 

*如何调用DLL中的资源(位图...)

]使用方法如下： 

HMODULE hMoudle = (HMODULE)LoadLibrary("XXXXX.dll"); 

HRSRC hRes = FindResource(hModule,MAKEINTRESOURCE(IDB_MYBITMAP),"RT_BITMAP") 

; 

HBITMAP hBmp = (HBITMAP)LoadResource(hModule,hRes); 

Remarks 

Both Windows 95 and Windows NT automatically free resources. You do not need 

to call the FreeResource function to free a resource loaded by using the Lo 

adResource function. 

但是这一句是需要的: 

FreeLibrary(hMoudle); 

***************************************************************** 

*dll中定义的全局变量和静态变量,共享变量 

dll中定义的全局变量和静态变量在每一个使用它的exe中都有拷贝，不能 

共享。 

如果要共享用#pragma data_seg建立一个新的数据 

段并定义共享数据，其具体格式为： 

　　#pragma data_seg （"shareddata") 

　　HWND sharedwnd=NULL;//共享数据 

　　#pragma data_seg() 

仅定义一个数据段还不能达到共享数据的目的，还要告诉编译器该段的属性，有两种方 

法可以实现该目的（其效果是相同的）， 

一种方法是在.DEF文件中加入如下语句： 

　　SETCTIONS 

　　shareddata READ WRITE SHARED 

*********************************************************************** 

*fopen()打开一个文件后想读其中第90行的第19个字符

先用fseek(fp,offset,SEEK_SET)把指针定位到要读取的位置，然后fread(void *buffe 

r，int size，int count，FILE *fp)；读取/ 

到网上搜索一下函数的用法。 

*********************************************************************** 

*如何锁定键盘和鼠标

typedef BOOL ( __stdcall *BlockInput)(BOOL Flag); 

BlockInput p = (BlockInput)GetProcAddress(::LoadLibrary("user32.dll"), "BlockInput");

if (p) 

{ 

p(FALSE); //这样就可以了 

} 

*********************************************************************** 

VC6中

工程 -- 属性 -- C/C++ -- 警告级别 中可以设置编译器的警告级别，分为5类：“无”，“level 1*”，“level 2”，“level 3”，“level 4”

严格程度依次递增，其中 “level 3” 是默认级别。

一般来说 你可以用默认级别 一定要重视警告 不建议调低 你可以用#pragma warning(diable:xxxx)来禁用特定警告（如果很确定这个警告可以忽略）

如果你有洁癖 建议调到最高4（警告作为错误 有警告就不能编译通过 很爽的）。

1.#pragma warning(disable: n) 将某个警报置为失效 

2.#pragma warning(default: n) 将报警置为默认

有种说法将1中的n设为4996是屏蔽所有警告

***************************************************************************

* 请问如何做一个安装程序

简单介绍一下Installshield for VC＋＋6.0的用法如下：? 

1。首先按照File->new->project wizard操作生成新project。 

2。在File Groups中，对各个文件夹的link右键点击，加入对应的files或者new folder。 

3。对Components的included files groups双击，弹出对话框，从而导入需要 

的File groups中的对应文件夹。你高兴也可以修改status text等。 

4。加入开始菜单中的快捷操作方式： 

在resources中， 

shell objects->explorer shell->desktop->start menu->programs，然后对programs 

右键点击操作，加入快捷方式，然后对第一行双击，会谈出对话框，在target中 

加入<TARGETDIR>\***.exe，icon选择0，即可。 

5。对media的操作，菜单build->media build wizard...->finish。 

6。菜单build->send media to操作导出安装。 

7。如有其他细节如是否压缩等，自己看一看就明白了。


