# VC笔记（四） - L_Andy的专栏 - CSDN博客

2013年01月06日 16:22:07[卡哥](https://me.csdn.net/L_Andy)阅读数：1160


关于ＶＣ的一些常见的小问题总结(1)

# 张明奇(卡哥)

改变对话框中某个控件的字体颜色

///////////////////////////////////////////////////////////

if(nCtlColor== CTLCOLOR_EDIT )   

{   

pWnd->GetDlgCtrlID();      

pDC->SetTextColor(RGB(255,0,0));  

}

2>改变对话框的背景颜色和前景颜色

///////////////////////////////////////////////////////////

方法一：调用CWinApp类的成员函数SetDialogBkColor来实现。

其中函数的第一个参数指定了背景颜色，第二个参数指定了文本颜色。

下面的例子是将应用程序对话框设置为蓝色背景和红色文本，步骤如下： 

① 新建一个基于Dialog的MFC AppWizard应用程序ExampleDlg。

② 在CExampleDlgApp ::InitInstance()中添加如下代码：

BOOL CExampleDlgApp: : InitInstance ( )

{

…

CExampleDlgDlg dlg;

m_pMainWnd = &dlg;

//先于DoModal()调用，将对话框设置为蓝色背景、红色文本

SetDialogBkColor(RGB(0,0,255),RGB(255,0,0));

int nResponse = dlg.DoModal();

…

}

方法二：

CRect rect;

CPaintDC dc(this);

GetClientRect(rect);

dc.FillSolidRect(rect,RGB(0,255,0));

方法三：

在CBckDialogDlg的头文件中建立一个画刷

CBrush m_nbrush;

在CBckDialogDlg的构造函数中初始化一个画刷

m_nbrush.CreateSoidBrush(RGB(0,0,255))

HBRUSH CExampleDlgDlg::OnCtlColor

(CDC* pDC, CWnd* pWnd, UINT nCtlColor)

{

HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

//在这加一条是否为对话框的判断语句

if(nCtlColor ==CTLCOLOR_DLG)

return m_brush; //返加绿色刷子

return hbr;

}

3>改变对话框上某个控件的字体

/////////////////////////////////////////////////////////

首先在构造函数中建立一个字体的定义

CFont m_nFont;

在OnInitDialog()中建立一个字体结构体对象 

LOGFONT logfong;

并且对它进行初始化

logfont.ifHeight=20//设置字体的大小

logfont.ifFaceName="字体名"//设置字体的名称(不过这种方法有时会出错)

strcpy(logfont.lfFaceName,"字体名");

下面是选中字体       

m_nFont.CreateFontIndirect(&logfont);

把字体格式写入到指定的控件中去

GetDlgItem(IDC_STATIC_TEXT)->SetFont(&m_nFont);

/////////////////////////////////////////////////////////

4>改变对话框中的的最大化，最小化，和恢复

///////////////////////////////////////////////////////////

<1>使窗口最大化显示

WINDOWPLACEMENT wndpl; 

   WINDOWPLACEMENT *pwndpl; 

    pwndpl = &wndpl; 

   GetWindowPlacement(pwndpl); 

   pwndpl->showCmd = SW_SHOWMAXIMIZED; //实现窗口最大化

  SetWindowPlacement(pwndpl);

这段代码是写在OnInitDialog()当中的

另一种方法直接::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE);

5>把窗口设为当前窗

int   nWidth,   nHeight;   

nWidth   =   GetSystemMetrics(SM_CXSCREEN);  

nHeight   =   GetSystemMetrics(SM_CYSCREEN);  

SetWindowPos(&wndTopMost,   0,   0,   nWidth,   nHeight,   0);

6>却掉对话框中的最大和最小按钮

/////////////////////////////////////////////////////////

在OnCreate()的消息响应中，添加如入代码：

SetWindowLong(this->m_nWnd,GWL_STYLE,GetWindowLong(this

->m_nWnd,GWL_STYLE)&~WS_MINIMIZEBOX&~WS_MAXIMIZEBOX);

6>一个对于位图非常有用的类：

//////////////////////////////////////

//你用一下这个简单的类,你就知道了

class CmyBitmap 

{

public:

    CBitmap bitmap;

    CDC memDC;

    BITMAP BM;

public:

         CmyBitmap(){}

         void LoadBMap(UINT nID_resource)

    {

        bitmap.LoadBitmap(nID_resource);

        bitmap.GetObject(sizeof(BITMAP),&BM);

    }

    void ShowBitmap(int x,int y,int weight,int height,CClientDC  *pDC)

    {

        memDC.CreateCompatibleDC(pDC);

       memDC.SelectObject(&bitmap);

        memDC.SetBkMode(TRANSPARENT);

        pDC->BitBlt(x,y,weight,height,&memDC,0,0,SRCCOPY);

             memDC.DeleteDC();//要注意删除

    }

};

//创建一个对象,就可载入一个位图，并可显示位图。

//也就是说，创建几个对象就可载入几个

关于ＶＣ的一些常见的小问题总结(2)

关于ClistCtrl控件的用法：

设置控件的类型：

DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |

                     LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE |

LVS_EX_UNDERLINEHOT;

m_nList.SetExtendedStyle(dwExStyle);//显示数据框的属性

m_nList.SetTextColor(RGB(255,0,0));//字体颜色

插入标题

m_nList.InsertColumn(第几个标题，标题名，标题类型（一般情况下是：

LVCFMT_CENTER），标题的宽度）；

插入列值：

每一行的第一列：m_nList.InsertItem(行号，每一列的内容）

其它列：m_nList.SetItemText(行号，列号（开始列号是从0开始),列中的内容）

删除行：

m_nList.DeleteAllItems();

8>把双精度数转换成字符：

方法一：

CString str;

str.Format("%.xf",double)

方法二：

其中x表示要保留的小数位数.

如果是要在对话框中输出:"我是第5介到这里来的";

char buf[10];

CString str;

int a=5;

memset(&buf,0,10);

itoa(a,buf,10);(这个10表示的是进制)

str.Format("这是第%s个到这里来的",buf);

当在使用atof或atod时，一定注意它们的接收字符串是CString型的

9>在VC中写一段代码来注册某个控件：

//////////////////////////////////////////////

//定义一个函数

BOOL CCpp_EquipmentApp::RegisterOcx(LPCTSTR OcxFileName)

{     

              LPCTSTR pszDllName = OcxFileName ;        //ActiveX控件的

路径及文件名         

              HINSTANCE hLib = LoadLibrary(pszDllName); //装载ActiveX控

件

              if (hLib < (HINSTANCE)HINSTANCE_ERROR)

              {

                     return FALSE ;

              }

              FARPROC lpDllEntryPoint;

              lpDllEntryPoint = GetProcAddress(hLib,_T("DllRegisterServer"));   //获取注册函数DllRegisterServer地址

              if(lpDllEntryPoint!=NULL)   //调用注册函数

DllRegisterServer

              {

                     if(FAILED((*lpDllEntryPoint)()))

                     {

                            FreeLibrary(hLib);

                            return FALSE ;

                     }

                     return TRUE ;

              }

              else

                     return FALSE ;

}

/*****说白了就使倒入一个DLL*****/

10>写一个文件读取对话框(本文件是一个比较大的数据文件）：

CFile m_nFile;

m_nFile.Open("文件名",CFile::modeRead(打开文件的类型));

CArchive arStore(&m_nFile,CArchive::load(打开文件的类型));

CString SoueceData,TempStr;

整行的读取数据，再根据每行数据中的标志截取存放到数组中去：

while(arStore.ReadString(SourceData)!=NULL)//当读取的是空行的时候便停止读取

{

   if(SourceData=="标题内容"||SourceData=="")//数据中如果出现的情况

       continue;

   count+=1;

   //以文件中存在空格为例来说明截取的方法

   TempStr=SourceData.Left(SourceData.Find(' '));//取第一行数据

   numdata[count][1]=atof(TempStr);

   //取第二行

   SourceData=SourceData.Right(SourceData.GetLength()-SourceData.Find(' '))

   SourceData.TrimLeft();

   TempStr=SourceData.Left(SourceData.Find(' '));

   numdata[count][2]=atof(TempStr);

   .......

}

11>//读取系统当前时间并把它显示出来

COleDateTime oleDate=COleDateTime::GetCurrentTime();

CString strDate=oleDate.Format("%Y-%M-%D")+' '+oleDate.Format("%h:%m:%s");

关于ＶＣ的一些常见的小问题总结(3)

这一篇算是对一些常见错误的总结吧

12>在VC的当中如果出现

error   LNK2001:   unresolved   external   symbol   _main  

可以采用下面的方法来进行改正：project->setting->Link中将：  

  /SUBSYSTEM：Console改为：/SUBSYSTEM:WINDOWS  

当我在用WIN32写程序的时候，在WndProc()中用到了这样一个句子：

PlaySound(TEXT("Hello .wav"),NULL,SND_FILENAME|SND_ASYNC);

即用到PlaySound()函数这样在程序的编译的时候就会出现这样一个错误：

unresolved external symbol __imp__PlaySoundA@12

这时解决方法是在project->setting->link加入winmm.lib 库就可以了

如果在写程序的时候，不小心把数据组的下标写成了不是int型的时候

程序就会出再subscript is not of integral type这样的错误提示

错误分析：

13>在VC6.0中编好的程序，每次在编译后总会出现这样一个错误：Compiling...

Errore spawning cl.exe

Cpp1.obj-1error(s),0 waring(s)

错误原因分析：

对于这个问题，很多多人选择重装，实际上这个问题很情况下是由于路径设置的问题

引起的，“cl.exe“是VC真正的编译器（编译程序），基路径在“VC根目录\VC98\Bin"

下面，你可以到相应的路径下找到这个应用程序，

因为问题可以按照以下方法来解决：点击VC”TOOL->Option->Directories"重新设置

“Excutable,Files,Include File,Library Files,Source Files"

的路径，很多情况下就是一个盘符的不同，改过来就OK了.

14>在.net中写程序的时候，有时总爱出现这样的错误提示：

LINK：fatal error LNK1201:写入数据库:"所写文件的路径\Debug\MyProgram.pcb"

时出错，请检查是否是磁盘空间不足，路径无效或权限不够

一般原因：

在编译的时候如果开着像金山毒霸，诺顿之类的烂杀毒软件的话，经常会出现这种

错误，特别是在程序较大的时候，

看看你的磁盘还有多少可用空间，最好留出200M来可用空间。

再看看你的虚拟内存的设置，是不是把你的磁盘空间给分配完了。

解决：

个人认识删除你的Debug关掉.net然后重新打开文件，再重新编译一下，就可以。本人经常这样解决。

15>用PS来做立体字的一个N简单的方法：建立好文件字后，按下ALT+方位键就完全OK了。

16>在使用ShellExecute()的时候，如果只是简单的1对1的文件调用形式，则可根据一

般的调用格式来设定，

当用到例如：A->B->这种连环式调用的时候，我们在设置文件路径的时候一定要十

分小心，特别是在设置

C程序的路径，当在B中调用C的时候，所设置的查找路径是相对于B来说的，但是当

如果B再次被A调用的时候，那么在B中设置的查找C路径

的路径就会失无效。出现不能调用的情况。

在这种情况下解决的办法是：

根据在B中对C的但找路径的设置，再把C程序复制到相对于A正确的位置。当然这种

办法是程序于C是一个小程序，但当C是一个比较大的应用程序的时候，

这种办法显然是否定定。但是其它办法（如动态设置路径）本人还没有想好。所以也就只能先这么用着吧。

17>在编译程序的时候出现：ASSERT(pMap->LookingPermanent(hWndOring)==NULL)的错

误提示

注意：ASSERT（）是一个调度程序时经常使用到的宏，在程序运行时它计算括号内的表达式，如果表达式为FALSE(0)，程序将报告错误信息，并终止执行，如果不表达式不为0，则继续执行后面的语句，这个宏通常原来判断程序中是否出现了明显的非法数据，如果出现了终止程序以免导致严重后果，同时也便于查找错误。

解决：

当出现了这种错误的时，可以看一下ASSERT括号里的表达式，看看是当什么条件的时候出现错误的。

一般这个时候，不大会是MFC的内部错误，多数是由于我们自己的写代码的时候出现的错误.

如果是利用GetWindowRect或是GetClientRect()来得到窗口的句柄的话，那一定要

注为时注意它们所用到的参与系统是不相同的，所以返回来的结果也是不相同的。

再者关于：GetClientRect(&rect)与GetClientRect(rect)用法说明：

有时候这两种用法是可以相等的。为什么呢？MSDN：A  Crect object can be

passed as a function parameter wherever a RECT structure,

LPCRECT ,or LPRECT can be passed.CRect类的说明：

Operators

opertaor LPCRECT Converts a CRect to an LPCRECT

opertaor LPRECT Converts a CRect to an LPRECT

首先我们知道GetClientRect() 的原形：

void GetClientRect(LPRECT lpRect)const;

说明这个函数的参数是LPRECT，而不是CRect 或CRect*，那为什么上面两种情况都行呢？

(1)对GetClientRect(rc),这里的参数是一个CRect,因为CRect类里面有个LPRECT运算符重载(operator LPRECT()),所以当传一个CRect变量给这个函数的时候，这个变量会自动变成LPRECT，

(2)对于一个GetClientRect(&rc)那就更好了，因为还有一点CRect是从RECT中继承

来的，所以你传一个CRect*给它的时候，它会自动转换成基类形式（RECT）

的地址，所以也是可以的。

关于ＶＣ的一些常见的小问题总结(4)2007-06-14 00:4218>(1)打开一个应用程序

ShellExecute(this->hWnd,"open","*.exe","","",SW_SHOWNORMAL);

或

ShellExecute(this->hWnd,"open","*.exe","c:\MyLog.log","",SW_SHOWNORMAL);

(2)打开一个同系统程序相关的文档

ShellExecute(this->hWnd,"open","c:\abc.txt","","",SW_SHOWNORMAL);

(3)打开一个网布

ShellExecute(this->hWnd,"open","网址","","",SW_SHOWNORMAL);

(3)打开相关程序来发关EMAIL

ShellExecute(this->hWmd,"open","vcvbvf@126.com","","",SW_SHOWNORMAL);

(4)用系统打印机打文档

ShellExecute(this->hWnd,"print","c:\abc.txt","","",SW_SHOWNORMAL);

(5)用系统查找功能来查找指定文件

ShellExecute(m_hWnd,"find","d:\mish",NULL,NULL,SW_SHOWNORMAL);

(6)启动一个程序，直到它运行结束

SHELLEXECUTEINFO ShExecInfo = {0};

ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

ShExecInfo.hwnd = NULL;

ShExecInfo.lpVerb = NULL;

ShExecInfo.lpFile = "c:\MyProgram.exe";

ShExecInfo.lpParameters = ""; 

ShExecInfo.lpDirectory = NULL;

ShExecInfo.nShow = SW_SHOW;

ShExecInfo.hInstApp = NULL; 

ShellExecuteEx(&ShExecInfo);

WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

或：

PROCESS_INFORMATION ProcessInfo; 

STARTUPINFO StartupInfo; //入口参数

ZeroMemory(&StartupInfo, sizeof(StartupInfo));

StartupInfo.cb = sizeof StartupInfo ; //分配大小

if(CreateProcess("c:\winnt\notepad.exe", NULL,

NULL,NULL,FALSE,0,NULL,

NULL,&StartupInfo,&ProcessInfo))

{ 

WaitForSingleObject(ProcessInfo.hProcess,INFINITE);

CloseHandle(ProcessInfo.hThread);

CloseHandle(ProcessInfo.hProcess);

} 

else

{

MessageBox("The process could not be started...");

}

19>(7)显示文件或文件夹的属性

SHELLEXECUTEINFO ShExecInfo ={0};

ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

ShExecInfo.fMask = SEE_MASK_INVOKEIDLIST ;

ShExecInfo.hwnd = NULL;

ShExecInfo.lpVerb = "properties";

ShExecInfo.lpFile = "c:\"; //也可以是文件

ShExecInfo.lpParameters = ""; 

ShExecInfo.lpDirectory = NULL;

ShExecInfo.nShow = SW_SHOW;

ShExecInfo.hInstApp = NULL; 

ShellExecuteEx(&ShExecInfo);

20>错误提示：

有error c2447:missing function header (old-style format list?)

造成这种错误的原因是由于你在你的函数头上用了一个分号所造成的。

解决的办法：

按F4找到那个错误的分号的地方，把分号去掉就要以了。

21>#define和typedef的区别:

(1)#definej 预处理命令，在编译预处理时进行了简单的替换，不作正确性检查，

不关含义是否

正确照样带入，只在编译已被展进的源程序时才会发现可能的错误并报错。

例如：#define pi=3.14159

程序中的：area=pi*r*r会替换成字母3.14159

如果你不小心把9与成g的话，在程序预编译的时候不会发现这个错误的。

(2)typedef是在编译时处理的，它在自己的作用域内给一个已经存在的类型一个别名。

但是: you cannot use the typedef specifier inside a function definition+

(3)typedef int * int_ptr;与#define int_ptr int*

作用都是用int_ptr代表int*，但是二者是不同的，正如前面所说，#define在预处

理时进行简单的替换，而typedef则不是

它是采用如同定义变量的方法那样来声明一种类型。

22>在VC中来获得本机所按装的协议：

/////////例程///////////////

#include <Winsock2.h>

#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

void main(void)

{

    WSADATA wsaData;

    WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

    LPWSAPROTOCOL_INFO info = NULL;

    DWORD dwLen;

    //因为info还未分配内存，第一次调用必定会失败，但能够得到dwLen的值

    if (SOCKET_ERROR == WSAEnumProtocols(NULL, info, &dwLen))

    {

        int iErrorCode = GetLastError();

        if (WSAENOBUFS != iErrorCode)

        {

            WSACleanup();

            return;

        }                

    }

    if (dwLen > 0)

    {

        info = (LPWSAPROTOCOL_INFO)GlobalAlloc(GPTR, dwLen);

        if (info == NULL)

        {

            printf("Allocate memory failed! ErrorCode is %d",

GetLastError());

            WSACleanup();

            return;            

        }

        //第二次调用

        int iProtocolCounts = WSAEnumProtocols(NULL, info, &dwLen);   

        if (SOCKET_ERROR == iProtocolCounts)

        {

            printf("WSCEnumProtocols Error: %d", GetLastError());

            GlobalFree(info);

            WSACleanup();

            return;    

        }

        //打印本机安装的协议信息

        for (int i=0; i<iProtocolCounts; i++)

        {

            printf("\ni=%d\nProtocolDescription:%s\n", i, info[i].szProtocol);           

        }

        GlobalFree(info);    

    }

    WSACleanup();

}

关于ＶＣ的一些常见的小问题总结(5)//创建异型窗口

1：在多文档窗口下：

在CMainFrame下，添加一个CRgn m_rgn;

添加WM_SIZE 消息响应在WM_SIZE里面添代码如下：

{

m_rgn.DeleteObject();//删除原来的区域对象

CRect rect;

GetWindowRect(&rect);

rect-=rect.TopLeft();

m_rgn.CreateEllipticRgnIndirect(&rect);

SetWindowRgn(m_rgn,TRUE);

2:在对话框下：

CRect rectDlg;

GetClientRect(&rectDlg);

CRgn rgn1,rgn2;

rgn1.CreateEllipticRgn(0,0,rctDlg.Width(),rectDlg.Height());

::SetWindowRgn(GetSafeHwnd(),(GRGN)m_rgn,TRUE);

}

24>/////////一个屏代码的实现//////////////

       CDC memDC;

       memDC.CreateDCW(_T("Display"),NULL,NULL,NULL);

       CDC dcCompatible;

       dcCompatible.CreateCompatibleDC(&memDC);

       CBitmap snBitmap;

       snBitmap.CreateCompatibleBitmap(&memDC,memDC.GetDeviceCaps

(HORZRES),memDC.GetDeviceCaps(VERTRES));

       CBitmap *oldBitmap;

       oldBitmap=dcCompatible.SelectObject(&snBitmap);

       //显示

       BITMAP bitInfo;

       snBitmap.GetBitmap(&bitInfo);

       dcCompatible.BitBlt

(50,50,bitInfo.bmWidth,bitInfo.bmHeight,&memDC,50,50,SRCCOPY);

       AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);

       CDC *pDC=GetDC();

       CRect rect;

       GetClientRect(&rect);

       CSize size=rect.Size();

       pDC->StretchBlt

(50,50,size.cx,size.cy,&dcCompatible,50,50,bitInfo.bmWidth,bitInfo.bmHeig

ht,SRCCOPY);

       dcCompatible.SelectObject(oldBitmap);

25>//////////////////枚举当前的所有进程/////////////////////////////

1>利用EnumWindows(...)回调函数来实现

在程序里面添加一个WM_TIMER事件

在程序的适当位置添加调用Timer事件的程序代码SetTimer(...)

在Timer的响应OnTimer(...)里面添加相应的代码：

EnumWindows(EnumWindowProc,NULL);

******************************************************

*.h

static BOOL CALLBACK EnumWindowProc(HWND,LPARAM);

*****************************************************

BOOL CALLBACK C*Dlg::EnumWindowProc(HWND hWnd,LPARAM lParam)

{

       char className[250];

       memset(&className,0,250);

       char winTitle[250];

       memset(&winTitle,0,250);

       LONG exStyle=0;

       if(IsWindowVisible(hWnd)&&(GetParent(hWnd)==NULL))

       {

              exStyle=GetWindowLong(hWnd,GWL_EXSTYLE);

              if(!(exStyle&WS_EX_TOOLWINDOW))

              {

                     GetWindowText(hWnd,winTitle,sizeof(winTitle));

                     GetClassName(hWnd,className,sizeof(className));

                     printf("Windows class name %s\n",className);

                     printf("Windows title %s\n",winTitle);

              }

              return TRUE;

       }

}

26>2>利用FindWindowEx(...)方法来进行查找

WINDOWINFO *wininfo=NULL;

int i=0;

int index=0;

DWORD dwStyle=0;

HWND hTempWnd=NULL;

hTempWnd=::FindWindowEx(NULL,NULL,NULL,NULL);

int count=0;

while(hTempWnd!=NULL)

{

       if(IsWindow(hTempWnd)&&::IsWindowVisible(hTempWnd))

       {

              dwStyle=GetWindowLong(hTempWnd,GWL_EXSTYLE);

              if(!(dwStyle&WS_EX_TOOLWINDOW))

              {

                     ::GetWindowText(hTempWnd,wininfo[count].title,255);

                     if(strlen(wininfo[count].title)>0&&strcmp

("AutoHidw 1.01",wininfo[count].title)!=0)

                     {

                            wininfo[count].hWndProc=hTempWnd;

                            count++;

                            if(count>=TOTALWINDOW)

                            {

                                   break;

                            }

                     }

              }

       }

hTempWnd=::FindWindowEx(NULL,hTempWnd,NULL,NULL);

}

27>用代码隐藏、最小化、最大化窗口 

CWnd::ShowWindow(0); //Hide

CWnd::ShowWindow(2); //Mix

CWnd::ShowWindow(3); //Max

更详细请参考:CWnd::ShowWindow

BOOL ShowWindow( int nCmdShow );

返回值

　　如果窗口原来可见，则返回非零值；

　　如果CWnd原来是隐藏的，则返回0。

参数

　　nCmdShow指定了CWnd应如何被显示。它必须是下列值之一：

　　SW_HIDE 隐藏窗口并将活动状态传递给其它窗口。

　　SW_MINIMIZE 最小化窗口并激活系统列表中的顶层窗口。

　　SW_RESTORE 激活并显示窗口。如果窗口是最小化或最大化的，Windows恢复其原来的大小和位置。

　　SW_SHOW 激活窗口并以其当前的大小和位置显示。

　　SW_SHOWMAXIMIZED 激活窗口并显示为最大化窗口。

　　SW_SHOWMINIMIZED 激活窗口并显示为图标。

　　SW_SHOWMINNOACTIVE 将窗口显示为图标。当前活动的窗口将保持活动状态。

　　SW_SHOWNA 按照当前状态显示窗口。当前活动的窗口将保持活动状态。

　　SW_SHOWNOACTIVATE 按窗口最近的大小和位置显示。当前活动的窗口将保持活动状态。

　　SW_SHOWNORMAL 激活并显示窗口。如果窗口是最小化或最大化的，则Windows恢复它原来的大小和位置。

*/*/******************************/

m_p0.SetState(1);//这个是让按钮暗下不弹起。  

  m_p0.SetState(0);//这个是让按钮暗下弹起

/*****************得到当前的系统日期得用COleDateTime类

*******************/

       COleDateTime DateTime;

       int date;

       CString str;

       DateTime=COleDateTime::GetCurrentTime();

       date=DateTime.GetDay();

       str.Format("%d",date);

       SetDlgItemText(IDC_EDIT3,str);

       date=DateTime.GetYear();

       str.Format("%d",date);

       SetDlgItemText(IDC_EDIT1,str);

       date=DateTime.GetMonth();

       str.Format("%d",date);

       SetDlgItemText(IDC_EDIT2,str);

关于ＶＣ的一些常见的小问题总结(6)*遍历数据库中的所有表名*/

try

{

pRcordset=pConnection->OpenSchema(adSchemalTable,vtMissing,vtMissing);

while(!pRecordset->adoEof)

{

TheValue=pRecordset->GetCollect("TABLE_NAME");

if(TheValue.vt!=VT_NULL)

       kooky=(char*)_bstr_t(TheValue);

if(kooky.Left(4)!="MSys")

{

OpenDlg.SelectOpenFile[count]=kooky;

count++;

}

pRecordset->MoveNext();

}

}

catch(_com_error e)

{

MessageBox("遍历数据库失败");

}

31>如何改变对话框的形状？

可用下面一些函数： 

CreatePolygonRgn

CreateRectRgn

CreateRoundRectRgn 等.   CRgn m_rgn;  // Put this in your dialog''s

header file. i.e. a member variable

  // This Gets the size of the Dialog: This piece of code is to be placed

in the

  // OnInitDialog Function of your dialog.

  CRect rcDialog

  GetClientRect(rcDialog);

  // The following code Creates the area and assigns it to your Dialog

  m_rgn.CreateEllipticRgn(0, 0, rcDialog.Width(), rcDialogHeight());

  SetWindowRgn(GetSafeHwnd(), (HRGN) m_rgn, TRUE);

VC常见问题总结(8)如何在对话框中加入工具条;

BOOL CYourDlg::OnInitDialog()

{

       CDialog::OnInitDialog();  

// Create the toolbar. To understand the meaning of the styles

used, you 

       // can take a look at the MSDN for the Create function of the

CToolBar class.

       ToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP |

CBRS_TOOLTIPS |CBRS_FLYBY | CBRS_BORDER_BOTTOM);

      // I have assumed that you have named your toolbar''s resource as

IDR_TOOLBAR1.

      // If you have given it a different name, change the line below to

accomodate 

      // that by changing the parameter for the LoadToolBar function.

      ToolBar.LoadToolBar(IDR_TOOLBAR1);

      CRect rcClientStart;

      CRect rcClientNow;

      GetClientRect(rcClientStart);

      // To reposition and resize the control bar

     RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,0,

reposQuery, rcClientNow);

     CPoint ptOffset(rcClientNow.left -

rcClientStart.left,rcClientNow.top-rcClientStart.top);

     CRect rcChild;

     CWnd* pwndChild = GetWindow(GW_CHILD);

     while (pwndChild)

     {

       pwndChild->GetWindowRect(rcChild);

       ScreenToClient(rcChild);

       rcChild.OffsetRect(ptOffset);

       pwndChild->MoveWindow(rcChild, FALSE);

       pwndChild = pwndChild->GetNextWindow();

     } 

     CRect rcWindow;

     GetWindowRect(rcWindow);

     rcWindow.right += rcClientStart.Width() - rcClientNow.Width();

     rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();

     MoveWindow(rcWindow, FALSE);    

     // And position the control bars

     RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

     return TRUE;  // return TRUE  unless you set the focus to a control

  }

33>如何在对话框中加入状态条？ 

定义 CStatusBar 变量：CStatusBar m_StatusBar;

定义状态条指定状态：static UINT BASED_CODE indicators[] =

  {

     ID_INDICATOR_CAPS,

     ID_INDICATOR_NUM

  };

在 OnInitDialog 中加入下面代码：   m_StatusBar.CreateEx

(this,SBT_TOOLTIPS,WS_CHILD|WS_VISIBLE|CBRS_BOTTOM,AFX_IDW_STATUS_BAR);

  // Set the indicators namely caps and nums lock status

  m_StatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));

  CRect rect;

  GetClientRect(&rect);

  m_StatusBar.SetPaneInfo(0,ID_INDICATOR_CAPS,SBPS_NORMAL,rect.Width()/2);

  m_StatusBar.SetPaneInfo(1,ID_INDICATOR_NUM,SBPS_STRETCH ,rect.Width()/2);

  RepositionBars

(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,ID_INDICATOR_NUM);

  m_StatusBar.GetStatusBarCtrl().SetBkColor(RGB(180,180,180))

