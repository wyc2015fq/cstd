# Developing COM Components using VC-ATL - ljx0305的专栏 - CSDN博客
2008年05月05日 19:50:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1522标签：[components																[服务器																[interface																[microsoft																[mfc																[basic](https://so.csdn.net/so/search/s.do?q=basic&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=components&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
第一节 用ATL创建COM组件
在 Project Name 编辑框中输入项目名如 MyProj ，并选择合适的 Location 后，按确认按钮进入下一个对话框：ATL  COM  Appwizard  -  step 1 of  1，在 Server Type 中选择 Dynamic  Link  Library [ DLL ]，即进程内服务器，这是最快的组件。
选中 Support  MFC 选择项。
在按下 Finish 和 Ok 按钮后，一个组件的框架已经建立。
二、给模型增加组件MyCom
在VC++ 菜单 Insert 中选中 New ATL  Object…菜单项，出现 ATL Object  Wizard 对话框。
在左边的 Category 中选择 Objects，右边的 Objects 中选中 Simple  Object 项。按 Next 按钮。
在出现的 ATL Object  Wizard 属性对话框中 Names 卡片中的八个编辑框中左上方的 Short Name 编辑框中输入短名如 MyCom ，其他七个编辑框的内容会自动生成。然后按确认按钮退出。
三、给组件增加方法(函数) MyF1、MyF2、MyF3、MyF4
在 VC++工作平台的左边的 Workspace 的 ClassView 卡片中找到接口 IMyCom 项，按右键，在出现的快捷菜单中选择 Add  Method …，出现 Add  Method  to Interface 对话框，在对话框中输入要增加的函数的函数名、参数和返回值类型。然后，按确认按钮退出。
先增加函数 MyF1：
函数名为： MyF1
参数：无
在MyCom.cpp文件中插入代码：
STDMETHODIMP CMyCom::MyF1()
{
 AFX_MANAGE_STATE(AfxGetStaticModuleState())
 // TODO: Add your implementation code here
 AfxMessageBox("欢迎使用我的组件");
 return S_OK;
}
用同样的方法给组件增加函数MyF2：
函数名为： MyF2
参数为：
  [in] BSTR str, 
  [out, retval] int* val
插入代码：
STDMETHODIMP CMyCom::MyF2(BSTR str, int *val)
{
 AFX_MANAGE_STATE(AfxGetStaticModuleState())
 // TODO: Add your implementation code here
 CString  sStr(str);
 int  n = sStr.GetLength();
 *val = n;
 return S_OK;
}
用同样的方法给组件增加函数 MyF3：
函数名为： MyF3
参数为：
  [in] BSTR str, 
  [out, retval] BSTR* retstr
插入代码:
STDMETHODIMP CMyCom::MyF3(BSTR str, BSTR *retstr)
{
 AFX_MANAGE_STATE(AfxGetStaticModuleState())
 // TODO: Add your implementation code here
 CString sStr(str);
 CString sRetstr = "组件收到你的信息：<" + sStr + ">/n特此告之。";
 CComBSTR  temp(sRetstr);
 *retstr = temp;
 return S_OK;
}
用同样的方法给组件增加函数MyF4：
函数名为： MyF4
参数为：
  [in] int x, 
  [out, retval] int* val
插入代码：
STDMETHODIMP CMyCom::MyF4(int x, int *val)
{
 AFX_MANAGE_STATE(AfxGetStaticModuleState())
 // TODO: Add your implementation code here
 *val = x+1;
 return S_OK;
}
编绎工程，生成组件DLL。
如果是在 Win2k 或者 WinXp 的计算机上完成了上面的三步操作，就会在 debug 子目录下生成组件的 DLL 文件，并且完成自动注册。但是在 Win98 的计算机上，注册要运行如下命令才能完成：
regsrv32  C:/MyProj/Debug/MyProj.dll
1
1
2，把服务器类型库导入客户
2-1在 StdAfx.h 文件中加入代码:
#import  "../MyProj/MyProj.tlb"
最后的StdAfx.h文件如下：
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#if !defined(AFX_STDAFX_H__2B646017_28AD_4CDE_9792_CB8F9A5C6B39__INCLUDED_)
#define AFX_STDAFX_H__2B646017_28AD_4CDE_9792_CB8F9A5C6B39__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define VC_EXTRALEAN  // Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>  // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>   // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#import  "../MyProj/MyProj.tlb"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_STDAFX_H__2B646017_28AD_4CDE_9792_CB8F9A5C6B39__INCLUDED_)
编译 StdAfx.cpp，
这样在客户端的debug目录下生成组件的类型库头文件(.tlh)和类型库实现文件(.tli)。
2-2在使用组件的源文件上方使用名字空间，最后的源文件上面部分代码如下
// MyExeDlg.cpp : implementation file
//
#include "stdafx.h"
#include "MyExe.h"
#include "MyExeDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace MYPROJLib;
以下代码略
3，初始化 COM 库
HRESULT hr=CoInitialize(NULL);
4，获得服务器组件的 CLSID
CLSID  clsid; 
hr=CLSIDFromProgID(OLESTR("MyProj.MyCom"),&clsid);
if(FAILED(hr))
{
         AfxMessageBox("COM Failed");
         return;
}
5，创建 COM 服务器组件实例，获得组件接口指针
IMyCom  *ptr = NULL;
hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(IMyCom),(LPVOID*)&ptr); 
6-1，在OnMyF1()中使用 COM 对象 MyF1()
ptr->MyF1();
ptr->Release(); // 释放实例
6-2，在OnMyF2()中使用 COM 对象 MyF2()
char str[32];
sprintf(str,"Len=%d",ptr->MyF2("abcdefg"));
AfxMessageBox(str);
ptr->Release();// 释放实例
6-3，在OnMyF3()中使用 COM 对象 MyF3()
AfxMessageBox(ptr->MyF3("abcdefg"));
ptr->Release(); // 释放实例
6-4，在OnMyF4()中使用 COM 对象 MyF4()
int x=8; char str[32];
sprintf(str,"x=%d,x+1=%d",x,ptr->MyF4(x));
AfxMessageBox(str);
ptr->Release();// 释放实例
7，终止 COM 库
CoUninitialize();
下面为客户测试端四个函数的完整代码。
// MyExeDlg.cpp : implementation file
//
#include "stdafx.h"
#include "MyExe.h"
#include "MyExeDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace MYPROJLib;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
         CAboutDlg();
// Dialog Data
         //{{AFX_DATA(CAboutDlg)
         enum { IDD = IDD_ABOUTBOX };
         //}}AFX_DATA
         // ClassWizard generated virtual function overrides
         //{{AFX_VIRTUAL(CAboutDlg)
         protected:
         virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
         //}}AFX_VIRTUAL
// Implementation
protected:
         //{{AFX_MSG(CAboutDlg)
         //}}AFX_MSG
         DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
         //{{AFX_DATA_INIT(CAboutDlg)
         //}}AFX_DATA_INIT
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
         CDialog::DoDataExchange(pDX);
         //{{AFX_DATA_MAP(CAboutDlg)
         //}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
         //{{AFX_MSG_MAP(CAboutDlg)
                   // No message handlers
         //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMyExeDlg dialog
CMyExeDlg::CMyExeDlg(CWnd* pParent /*=NULL*/)
         : CDialog(CMyExeDlg::IDD, pParent)
{
         //{{AFX_DATA_INIT(CMyExeDlg)
                   // NOTE: the ClassWizard will add member initialization here
         //}}AFX_DATA_INIT
         // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
         m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CMyExeDlg::DoDataExchange(CDataExchange* pDX)
{
         CDialog::DoDataExchange(pDX);
         //{{AFX_DATA_MAP(CMyExeDlg)
                   // NOTE: the ClassWizard will add DDX and DDV calls here
         //}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CMyExeDlg, CDialog)
         //{{AFX_MSG_MAP(CMyExeDlg)
         ON_WM_SYSCOMMAND()
         ON_WM_PAINT()
         ON_WM_QUERYDRAGICON()
         ON_BN_CLICKED(IDC_BUTTON1, OnMyF1)
         ON_BN_CLICKED(IDC_BUTTON2, OnMyF2)
         ON_BN_CLICKED(IDC_BUTTON3, OnMyF3)
         ON_BN_CLICKED(IDC_BUTTON4, OnMyF4)
         //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMyExeDlg message handlers
BOOL CMyExeDlg::OnInitDialog()
{
         CDialog::OnInitDialog();
         // Add "About..." menu item to system menu.
         // IDM_ABOUTBOX must be in the system command range.
         ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
         ASSERT(IDM_ABOUTBOX < 0xF000);
         CMenu* pSysMenu = GetSystemMenu(FALSE);
         if (pSysMenu != NULL)
         {
                   CString strAboutMenu;
                   strAboutMenu.LoadString(IDS_ABOUTBOX);
                   if (!strAboutMenu.IsEmpty())
                   {
                            pSysMenu->AppendMenu(MF_SEPARATOR);
                            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
                   }
         }
         // Set the icon for this dialog. The framework does this automatically
         // when the application's main window is not a dialog
         SetIcon(m_hIcon, TRUE);                            // Set big icon
         SetIcon(m_hIcon, FALSE);                  // Set small icon
         // TODO: Add extra initialization here
         return TRUE; // return TRUE unless you set the focus to a control
}
void CMyExeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
         if ((nID & 0xFFF0) == IDM_ABOUTBOX)
         {
                   CAboutDlg dlgAbout;
                   dlgAbout.DoModal();
         }
         else
         {
                   CDialog::OnSysCommand(nID, lParam);
         }
}
// If you add a minimize button to your dialog, you will need the code below
// to draw the icon. For MFC applications using the document/view model,
// this is automatically done for you by the framework.
void CMyExeDlg::OnPaint() 
{
         if (IsIconic())
         {
                   CPaintDC dc(this); // device context for painting
                   SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
                   // Center icon in client rectangle
                   int cxIcon = GetSystemMetrics(SM_CXICON);
                   int cyIcon = GetSystemMetrics(SM_CYICON);
                   CRect rect;
                   GetClientRect(&rect);
                   int x = (rect.Width() - cxIcon + 1) / 2;
                   int y = (rect.Height() - cyIcon + 1) / 2;
                   // Draw the icon
                   dc.DrawIcon(x, y, m_hIcon);
         }
         else
         {
                   CDialog::OnPaint();
         }
}
// The system calls this to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CMyExeDlg::OnQueryDragIcon()
{
         return (HCURSOR) m_hIcon;
}
void CMyExeDlg::OnMyF1() 
{
         // TODO: Add your control notification handler code here
         HRESULT hr=CoInitialize(NULL);
         CLSID clsid; 
         hr=CLSIDFromProgID(OLESTR("MyProj.MyCom"),&clsid);
         if(FAILED(hr))
         {
                   AfxMessageBox("COM Failed");
                   return;
         }
         IMyCom *ptr = NULL;
         hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(IMyCom),(LPVOID*)&ptr); 
         ptr->MyF1();
         ptr->Release(); // 释放实例
         CoUninitialize();
}
void CMyExeDlg::OnMyF2() 
{
         // TODO: Add your control notification handler code here
         HRESULT hr=CoInitialize(NULL);
         CLSID clsid; 
         hr=CLSIDFromProgID(OLESTR("MyProj.MyCom"),&clsid);
         if(FAILED(hr))
         {
                   AfxMessageBox("COM Failed");
                   return;
         }
         IMyCom *ptr = NULL;
         hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(IMyCom),(LPVOID*)&ptr); 
         char str[32];
         sprintf(str,"Len=%d",ptr->MyF2("abcdefg"));
         AfxMessageBox(str);
         ptr->Release();// 释放实例
         CoUninitialize();
}
void CMyExeDlg::OnMyF3() 
{
         // TODO: Add your control notification handler code here
         HRESULT hr=CoInitialize(NULL);
         CLSID clsid; 
         hr=CLSIDFromProgID(OLESTR("MyProj.MyCom"),&clsid);
         if(FAILED(hr))
         {
                   AfxMessageBox("COM Failed");
                   return;
         }
         IMyCom *ptr = NULL;
         hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(IMyCom),(LPVOID*)&ptr); 
         AfxMessageBox(ptr->MyF3("abcdefg"));
         ptr->Release(); // 释放实例
         CoUninitialize();
}
void CMyExeDlg::OnMyF4() 
{
         // TODO: Add your control notification handler code here
         HRESULT hr=CoInitialize(NULL);
         CLSID clsid; 
         hr=CLSIDFromProgID(OLESTR("MyProj.MyCom"),&clsid);
         if(FAILED(hr))
         {
                   AfxMessageBox("COM Failed");
                   return;
         }
         IMyCom *ptr = NULL;
         hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(IMyCom),(LPVOID*)&ptr); 
         int x=8; char str[32];
         sprintf(str,"x=%d,x+1=%d",x,ptr->MyF4(x));
         AfxMessageBox(str);
         ptr->Release();// 释放实例
         CoUninitialize();
}
通过这个简单的例子，我们做了最简单的COM组件，及测试组件的客户端，在这里只是教大家如何用VC ATL做COM组件，在接下来这一篇里Developing COM Components using VC-ATL(2)将对COM的基本原理进行介绍，以及对这一例子的源代码进行剖析。
Developing COM Components using VC-ATL(1)完
 第二节 COM基础知识
COM的产生
应用程序被开发来支持公司业务运转，应用程序获取作为输入的数据，处理基于业务规则的数据，和提供作为输出的数据和信息。基于这一事实，所有的应用程序都有三个要素：
n用户界面或呈现要素，通过它们可以输入数据。
n应用程序逻辑或业务规则要素，它们有助于实现对输入数据执行有效性、可用性操作。
n数据存储或数据管理要素，它们管理数据的存储和恢复。
这三个要素养组成了用于应用程序开发模型或体系结构的基础。
根据具体的用途和配置，开发都在他们的应用程序中决定采用何种体系结构时有三种选择：
n单层模型
n双层模型
n三层或多层模型
作为一个开发者，在决定应用程序的体系结构之前，你要考虑以下因素：
n伸缩性：这指的是应用程序适应新的硬件环境的性能。例如，当一个应用程序从一个单处理器计算机移到一个多处理计算机时，它是否能使用新增加的处理器，这些处理器在新硬件上是否发挥了作用。
n可用性：这指的是应用程序满足用户增加要求的能力。可能有一百个用户正在请求同样的数据，但是只有少数用户获得数据，其他的用户则显示一则消息“请稍后再试”。
n扩展性：这指的是应用程序有时递增地增加特色来提高性能和提供更好的功能给用户的能力。
n维护性：这指的是根据人力和时间使用最小化的资源和最大化的有效性来响应修修复改错误的能力。
n安全性：这指的是应用程序维持数据完整性、可靠性的能力。任何未经认可的用户都不允许篡改数据。
n性能：这指的是应用程序在最短的时间内响应用户请求的能力。用户不要等待很长时间就能获得请求数据。
为了设计更健壮的应用程序体系结构，三(多)层体系结构就运用而生。三层体系结构是一种逻辑应用程序观点，在这种体系结构中应用程序分成以下三层逻辑组成部分：
n用户服务层。用户服务层管理用户界面。
n业务服务层。业务服务层管理由应用程序实现的业务逻辑。业务逻辑由支配应用程序处理的规则组成。这一层对用户封装了业务逻辑，因而用户不必了解业务逻辑或业务规则是如何使用的。业务规则的任何变化将导致仅对业务服务层的更改，而应用程序的其余部分保持不变。业务服务层实现用于应用程序的多条规则。这一层扮演了用户服务层和数据服务层间连接的角色。
n数据服务层。数据服务层负责维护数据，通常为RDBMS。这一层维护数据的有效性和完整性，并且负责从数据存储中增加、修改和删除数据。
现在，我们如何来做业务服务层呢?答案就是COM。
什么是COM?
COM即为组件对象模型，它为开发组件定义了标准。它定义了组件交互标准、协议标准和组件的物理结构来确保组件间的互用性。一个组件就是一个执行特定操作的程序或二进制对象。下面我们就从组件的一些特点和功能来理解一下COM：
nCOM是一种规范。COM规范描述了为创建能共同操作的组件而必须遵守的标准。这个标准描述了COM是什么和它们是如何运转的，以及说明了如何可动态交替更新组件。
nCOM是一套服务。COM规范是由一套服务或APIs来支持的。COM库提供这些服务，它们是WIN32平台操作系统的一部分并且可用于当作其他操作系统的一个单独的包。
nCOM是面向对象的。COM组件对象有标识、状态和行为。
nCOM能够容易的用户化和升级你的应用程序。COM组件动态地相互联结，并且COM定义了定位其他组件和鉴定其功能的标准。因此，组件可以交换而不必重新编绎整个应用程序。
nCOM能够分布式应用。位置透明性是COM的特性之一。这使得你可以编写COM组件而不必考虑它们的位置。组件可以移动而不要求对使用它们的应用程序作任何改变。
nCOM组件可以用任何语言编写。任何能够处理二进制语言都能用于创建COM对象。能创建组件的语言和工具包括C/C++、JAVA、VISUAL BASIC和VISUAL C++等。
到目前为止，微软提倡的这套COM软件开发模式是最为符合现在计算机工业软件生产的方法。Microsoft的许多技术，如ActiveX, DirectX以及OLE等都是基于COM而建立起来的。并且Microsoft的开发人员也大量使用COM组件来定制他们的应用程序及操作系统。在COM构架下，程序员可以开发出各种各样的功能专一的组件，然后将它们按照需要组建起来，构成复杂的应用系统。由此带来的好处是多方面的：
n使用组件的最大好处首先是可以将它们动态的插入或卸出应用系统，是真正意义上的软件模块即插即用。组件可以在运行时刻，在不被重新链接或编译应用程序的情况下，可以将系统中的组件用新的组件替换掉，以便即时方便地进行系统的升级和定制。这样做的最大好处是程序员可以利用最新的技术写出的最新组件取代以有的旧组件而整个系统的其他部分不需要一点改动。应用程序可以随新组件不断取代旧的组件而渐趋完善且健壮。而且利用已有的组件，用户还可以快速的建立全新的应用。(注意：替换的组件均为自定义的组件，不是COM本身的运行时间库中的组件)
n二进制重用。按照COM规范，组件可以在多个应用系统中重复利用同一组件。
n分布式应用。利用COM/DCOM组件技术，可以方便的将应用系统扩展到网络环境下。
nCOM与语言无关。可以使用多种语言编写COM组件，如DELPHI、VISUAL BASIC…这使得程序员可以利用自己擅长的语言来编写组件，而无须学习其他专门的语言来编写组件。
nCOM与平台无关。利用已编写的组件可以方便地在不同平台间移植。
在三层体系结构中，创建COM组件来实现业务服务层。因为COM组件能够相互作用，每个组件必须严格地贯彻由COM拟定的完整的标准。
COM具有显著的优点在于它提供了三个独有的特性：
n调用组件间功能的二进制标准。COM确保用VISUAL BASIC开发的客户端和用VISUAL C++开发并且运行于Solaris或Mac服务端的组件间完全的二进制的兼容性。因此，COM允许你创建确实是独立于语言和操作系统的代码模块。
n代码重用。因为接口是文档化的，所以创建对象的代码能被不同的客户使用。
n版本控制。基于COM的组件是自我版本化的。暗示新的功能可以增加到组件中，而不影响已使用该组件的用户(/客户)
由此可见，COM组件是真正地面向对象，提供简易的用户化，提供地址透明并且能用任何语言编写。正由于COM具有这些优点，COM得到了广泛应用。
COM组件是真正地面向对象
n传统的面向对象编程
　　面向对象编程是一个被广大编程人员和工业界认同已久的概念。面向对象程序设计语言让开发者按照现实世界里人们思考问题的模式来编写程序，它让开发者更好地用代码直接表达现实中存在的对象，这样开发代码简单并且易于维护。面向对象语言有以下三个最重要的概念：
n封装（Encapsulation）强调隐藏对象的实现细节，对象的使用者仅仅通过定义好的接口使用对象。
n继承（Inheritance）新的对象可以利用旧的对象的功能。
n多态（Polymorphism）根据所使用的对象展现多种不同行为的能力。
而从程序的整体来看，面向对象编程提供给用户的最重要的概念则是源代码重用，它对于提高编写程序的效率极为重要。但是代码的共享与重用一旦应用于实践中仍然存在种种问题，如版本的升级、接口[1]的变化、在不同程序设计语言之间共享代码等等。对于这些困难原有的面向对象程序设计方法并没有相应的答案，这就是组件对象模型提出的背景。
n基于组件编程
它与传统面向对象模型有着根本的区别，传统的面向对象编程模型的重点是源程序以及系统分析和设计过程，面向对象归根到底还是基于源代码的重用，不是真正意义上的面向对象。相反，COM组件的概念则强调大的软件系统如何由不同的小组件构成整体应用程序体系结构。
将工程分解为逻辑组件是面向组件分析和设计的基础，这也是组件软件的基础。组件软件系统由可重用的二进制形式的软件组件模块组成，只需要相当小的改动就可以将这些来自不同开发商的组件模块组合在一起。特别重要的是这样的组合并不需要源代码，也不需要重新编译，组件之间是通过基于二进制的规范进行通讯的，这被称为二进制重用。组件模块是独立于编程语言的，使用组件的客户程序和组件之间除了通过标准的方法进行通讯以外，彼此不做任何限定。
组件可以划分为不同的类型，包括可视化组件如按钮或者列表框；功能组件如打印或者拼写检查。例如一个基于组件的架构可以提供将多个开发商的拼写检查组件插入到另一个开发商的字处理应用程序中的机制，这样用户可以根据自己的喜好方便地选择和替换字处理软件。
COM允许你创建相互联合起来形成应用程序的独立的、可重用的组件。在COM中，组件间的相互作用是基于Client/Server模型的(即客户机和服务器模型)。基于这个模型，COM组件可以分类为：
nClient 使用由其他组件提供的功能。
nServer 有其他组件可能使用的预先确定的功能的组件。
客户程序和组件程序是相对的，进行功能请求调用的是客户程序，而响应该请求的是组件程序。组件程序也可以作为客户程序去调用其它的组件程序，即提供服务给客户端的COM服务器可能是客户本身。正是这种角色的转换和相互调用关系使组件程序最终构成一个软件系统。
服务器组件的类型
    COM设计为允许组件相互通信而不管它们的地址。例如，用户接口组件将最好定位于接近客户端。另一方面，提供远程数据统计计算的组件将随同将被操作的数据可能定位在一台分离的计算机上。COM服务端组件可分为三类：
n进程内组件进程内服务器[2]是装入客户端的进程空间内的组件，即客户程序和组件程序在同一个进程地址空间内。在客户端和服务端组件间有大量数据转移操作的情况下是理想的。进程内服务器会更快地装载。由于它占用和客户端应用程序同样的地址空间，它可以与客户端更快的通信。进程内服务器是通过将组件作为动态连接库(DLL)的形式来实现的。DLL允许特定的一套功能以分离于可执行的、以DLL为扩展名的文件进行存储。只有当程序需要它们时，DLL才将其装入内存中，客户程序将组件程序加载到自己的进程地址空间后再调用组件程序的函数。
n本地(即进程外)组件进程外组件指客户程序和组件程序分别处在不同的进程空间地址中。进程外服务器对需要运行于独立的处理空间或作为独立客户端应用程序的线程的组件是理想的。这种服务器的类型自身是EXE，因此具有独立的进程的资格。由于数据必须从一个地址空间移到另一个地址空间，因此这些服务器就会慢得多。由于进程外服务器是可执行的，它们运行在自己的线程内。当客户端代码正在执行时，客户端不锁住服务器。进程外服务器对需要表现为独立的应用程序的组件也是理想的。例如，Microsoft Internet Explorer的应用程序是本地服务器的例子。客户端和服务端的通信是通过进程内的通信协议进行的，这个通信协议是IPC。
n远程组件远程服务器与本地服务器类似，除了远程服务器是运行在通过网络连接的分离的计算机上。这种功能是使用DCOM实现的。DCOM的优点在于它并不要求任何特别的编程来使具有功能。另外服务端和客户端通信是通过RPC通信协议进行的。
对于这三种不同的服务器组件，客户程序和组件程序交互的内在方式是完全不同的。但是对于功能相同的进程内和进程外组件，从程序编写的角度看，客户程序是以同样的方法来使用组件程序的，客户程序不需要做任何的修改。
COM的真相
COM是一种软件体系结构，这种体系结构允许用不同的软件商提供的组件来构造应用程序和系统。它是一套二进制的和网络标准，允许任何软件相互通信而不管硬件、操作系统OS和用于开发的编程语言。COM不是一种编程语言而是一套规范，它定义了组件怎么样可以相互通信。
每个COM组件被编写成满足由COM规定的二进制标准。这一些标准是：
n组件要求注意它们自己(即组件对象[3])的创建和销毁
n组件要求以标准方式提供它们的功能(即通过接口[4])
n组件的位置要求对用户透明
使用组件的优点在于具有将它们动态装载或卸出应用程序系统的能力。为了达到这一目的，组件必须满足两个需求：
n组件必须动态链接[5]。这允许它们在运行时改变。如果应用程序是由每次组件改变时静态链接的组件组成，那么应用程序就等同于一个单一的应用程序。
n在实现时组件必须隐藏。每个组件具有唯一识别自己的ID作为标识。这些ID出现在系统注册表HKEY_CLASSES_ROOT目录里。
动态链接对于组件而言是一个至关重要的要求，而实现细节隐藏则是动态链接的一个必要条件。
    COM的工作方式
到目前为止，显然COM并不仅仅是书面形式的规范。它也包含系统级的代码，即它自身的实现。COM规则出现在COM运行库里。
组件对象库，即COM运行时间库是一个系统组件，这个系统组件提供能够在进程内、进程外或通过网络进行调用的COM。
    COM核心，简单地讲，是组件对象和客户端使用二进制标准如何交互的规范。COM在操作系统内的实现为COM运行时间库。COM运行时间库包括：
nAPI函数
n服务
n允许应用程序在进程内控制内存分配的标准机制
n…
组件对象库是通过Windows3.1中的COMPOBJ.DLL以及Windows NT和Windows 9X中的OLE32.DLL实现的。
接口
n概述
n组件对象高度地封装。组件对象的内部实现对用户完全隐藏，根本无法知道组件对象在使用何种数据结构和这些数据是如何被函数处理的。每个组件有一个接口，这个接口是一种且唯一的访问组件对象功能的方式。接口是由一组虚函数的声明组成。它使得预期的行为和响应清淅化。接中并不具有这些函数的实现。函数是通过组件类(CoClass)实现的。组件类实例化时生成组件对象。
n接口是用称为虚函数表(VTable)的内存结构实现的。无论何时创建组件对象，组件对象也在内存中创建一张对应的虚函数表。虚函数表是由一系列指针组成，这些指针存储了由组件对象实现的成员函数的地址。
n客户端创建一个接口指针，接口指针指向一个虚指针，虚指针指向虚函数表。使用接口指针和虚指针，客户端就可以访问组件对象实现的成员函数。
VTable是一个函数指针数组的内存结构。每一个数组元素包含的是一个由组件所实现的函数地址。对于COM而言，接口也就是此内存结构，其他东西，均是COM不关心的实现细节。
n特征
n接口是集合在同一个名称(是一个系统唯一的ID值，称IID)下的相关函数(/方法)的集合。这说明接口必须是全球唯一的。
n如前所述，当组件类实例化时产生组件对象。接口是由组件对象实现的函数的集合。接口不可以被实例化，因为它没有实现。
n组件之间的通讯是基于接口的。接口是组件和其客户之间严格类型化的契约。实现相同接口的两个对象就被认为是多态的，这里的多态不包含诸如基类指针指向派生类对象的意义，这里是指同一个接口可以由多个组件对象以不同方法实现。
n组件对象可以实现多个接口。例如，银行交易组件(BankTransaction)支持两个接口，IDebit和ICredit，即借和贷；又例如，Microsoft SQL SERVER数据库服务器组件(SqlImplementation)支持两个接口，一个是维护数据处理，另一个是维护数据定义。数据处理接口提供增加、更新和删除数据方法，然而，数据定义接口提供创建数据库、表和视图的方法。
n接口没有版本化并且是不变的，如果功能必须改变以适应一个接口，则将创建一个有唯一标识的完全新接口。组件对象实现这两个接口，因而解决了版本问题。为较早版本的接口制作的客户端仍旧可以访问原来的接口。
n注意
n原来的旧接口是可以改变的，但是接口中的函数说明不能改变，即接口中的函数的函数名、参数形式不能改变，因为这样改变就需要使得客户端调用服务的代码进行修改和进行重新编绎。但是接口中的函数的函数体是可以修改的。这就是接口不变性的本质，也是COM规范。
COM标识符
因为全球都在开发和使用组件，因此需要标识唯一地标识每个组件。
开发软件基金会(OSF)研究出一种能产生唯一标识符的算法，称之为全球唯一标识符(UUID)。在COM中，UUID被称之为全局唯一标识符(GUID)。GUID是能分配给接口、组件类和类型库的128位或16字节数。GUID唯一地标识组件。
生成GUID的算法根据以下几个方面：
n当前日期和时间。
n网络适配器卡地址。
n时针序。
n自动递增计数器。
        COM使用的128位的接口标识符使得我们可能创建大约340282366920900000000000000000000000000个独立的接口，足够为将来10782897524560000000年每秒创建一万亿个接口。
网卡地址是相互不同的，对没有网卡的机器，地址对使用中的机器保持唯一性。
        GUID可能通过执行UUIDGEN.EXE或GUIDGEN.EXE产生。GUIDGEN是装载Microsoft Visual Studio中的一个工具，它通常在C:/Program Files/Microsoft Visual Studio/Common/Tools/GUIDGEN.EXE。
        GUID分为三类，具体见下：
nCLSID 是唯一地标识类或组件的GUID，传统地，CLSID的一般形式为CLSID_<unique identifier>，在本例中：
在MyProj_i.c中表示为
const CLSID CLSID_MyCom = {0xFEB7BDEF,0xFB6F,0x446B,{0xBE,0x31,0xDF,0x0A,0x3A,0xD3,0x91,0xBA}};
在MyProj.idl中表示为
         [
                   uuid(FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA),
                   helpstring("MyCom Class")
         ]
         coclass MyCom
         {
                   [default] interface IMyCom;
         };
在MyCom.rgs中表示为
HKCR
{
         MyProj.MyCom.1 = s 'MyCom Class'
         {
                   CLSID = s '{FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA}'
         }
         MyProj.MyCom = s 'MyCom Class'
         {
                   CLSID = s '{FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA}'
                   CurVer = s 'MyProj.MyCom.1'
         }
         NoRemove CLSID
         {
                   ForceRemove {FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA} = s 'MyCom Class'
                   {
                            ProgID = s 'MyProj.MyCom.1'
                            VersionIndependentProgID = s 'MyProj.MyCom'
                            ForceRemove 'Programmable'
                            InprocServer32 = s '%MODULE%'
                            {
                                     val ThreadingModel = s 'Apartment'
                            }
                            'TypeLib' = s '{FE651184-11DE-4D01-BD69-B07DDFA12D0C}'
                   }
         }
}
nProgID 显然，上面的CLSID难以记忆且难以使用。ProgID是分配给对象的用户友好名。ProgID不可能是单一的。每个ProgID映射到CLSID。命名习惯可以是<program>.<component>.<verson>。在本例中：
在MyCom.rgs中可以看到，有这么两句
…
                            ProgID = s 'MyProj.MyCom.1'
                            VersionIndependentProgID = s 'MyProj.MyCom'
…
通过分别地调用函数ProgIDFromCLSID和CLSIDFromProgID可以将ProgID转换为CLSID和将CLSID转换为ProgID。
如在本示例中的客户端程序中
HRESULT hr=CoInitialize(NULL);
     CLSID clsid; 
     hr=CLSIDFromProgID(OLESTR("MyProj.MyCom"),&clsid);
     if(FAILED(hr))
     {
              AfxMessageBox("COM Failed");
              return;
     }
调用函数CLSIDFromProgID把作为第一个参数的ProgID的CLSID存放到第二个参数中。
nIID 是唯一标识接口的GUID。按照惯例，IID的一般形式为IID_<unique identifier>，在本例中：
在MyProj_i.c中表示为
const IID IID_IMyCom = {0x65460F9C,0x3BAB,0x4055,{0x88,0x5A,0x8E,0xD5,0x9F,0x5F,0xA9,0xB0}};
在MyProj.idl中表示为
   [
            object,
            uuid(65460F9C-3BAB-4055-885A-8ED59F5FA9B0),
            dual,
            helpstring("IMyCom Interface"),
            pointer_default(unique)
   ]
   interface IMyCom : IDispatch
   {
            [id(1), helpstring("method MyF1")] HRESULT MyF1();
            [id(2), helpstring("method MyF2")] HRESULT MyF2([in] BSTR str,[out, retval] int* val);
            [id(3), helpstring("method MyF3")] HRESULT MyF3([in] BSTR str,[out, retval] BSTR* retstr);
            [id(4), helpstring("method MyF4")] HRESULT MyF4([in] int x,[out, retval] int* val);
   };
nTypeLibID 是标识系统上的类型库。按照惯例，TypeLibID的一般形式为LIBID_<组件工程名>Lib，在本例中：
在MyProj_i.c中表示为
const IID LIBID_MYPROJLib = {0xFE651184,0x11DE,0x4D01,{0xBD,0x69,0xB0,0x7D,0xDF,0xA1,0x2D,0x0C}};
在MyProj.idl中表示为
[
   uuid(FE651184-11DE-4D01-BD69-B07DDFA12D0C),
   version(1.0),
   helpstring("MyProj 1.0 Type Library")
]
library MYPROJLib
{
   importlib("stdole32.tlb");
   importlib("stdole2.tlb");
};
在MyCom.rgs中也可以看到这么一句
'TypeLib' = s '{FE651184-11DE-4D01-BD69-B07DDFA12D0C}'
COM对象的注册角色
如果COM组件用文件名装载并且组件位置或名字变化了，客户应用程序将不能访问组件。文件名无法提供在一个系统上的不同的组件版本，因而造成了不同版本商之间的产品冲突。
COM通过借助标识符ID注册系统注册表中的COM组件的位置来处理这个问题。COM组件的ID必须保证是系统唯一的，以避免与不同产品的冲突，并且客户可不必知道组件的物理位置而可以访问组件，这也就是所谓的组件对客户的透明化。
COM类注册在HKEY_CLASSES_ROOT/CLSID主键下。对每个CLSID主键，有一个子键InprocServer32，这个子键的默认值为DLL的文件名。
组件可以使用regsver32程序注册。这个程序提供进入注册表的组件。
     DLL文件可以使用以下命令注册：
     Regsver32 <DLL filename>
为了注册EXE文件，命令语法为：
<EXE filename>/regserver
注册COM组件读取的注册文件是MyCom.rgs
在本例中，注册表的结构为(红色为CLSID，绿色为IID，蓝色为LIBID)
HKEY_CLASSES_ROOT/CLSID/{FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA}
默认MyCom Class
/InprocServer32
默认C:/MyProj/Debug/MyProj.dll
Apartment ThreadingModel
/ProgID
默认MyProj.MyCom.1
/Programmable
/TypeLib
默认{FE651184-11DE-4D01-BD69-B07DDFA12D0C}
/VersionIndependentProgID
默认MyProj.MyCom
HKEY_CLASSES_ROOT/MyProj.MyCom
/CLSID
默认{FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA}
/CurVer
默认MyProj.MyCom.1
     HKEY_CLASSES_ROOT/MyProj.MyCom.1
/CLSID
默认{FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA}
     HKEY_CLASSES_ROOT/Interface/{65460F9C-3BAB-4055-885A-8ED59F5FA9B0}
默认IMyCom
/ProxyStubClsid
/ProxyStubClsid32
/TypeLib
默认{FE651184-11DE-4D01-BD69-B07DDFA12D0C}
Version 1.0
     HKEY_CLASSES_ROOT/TypeLib/{FE651184-11DE-4D01-BD69-B07DDFA12D0C}
/1.0
默认MyProj 1.0 Type Library
/1.0/0
    /1.0/0/win32
默认C:/MyProj/Debug/MyProj.dll
/1.0/FLAGS
/1.0/HELPDIR
默认C:/MyProj/Debug/
接口类型
完整的COM编程系统是基于接口的。接口可以分为以下几类：
n标准接口由COM系统提供的接口
n自定义接口由开发者创建的接口
大多数由标准接口实现的方法被系统调用。这样的标准接口有IUnknown、IDispatch、IClassFactory、IOle、IDateObject、IStream、IStorage…
自定义接口的目的是提供更多的功能给用户。
标准接口
由COM系统提供的不同的标准接口有：
n**IUnknown** IUnknown 是所有接口的基接口。甚至自定义接口也执行IUnknown。所有的COM组件都必须实现这个接口。IUnknown 用于组件对象的生命周期管理。它也可以查询被组件对象使用的其他接口。
下面的等式阐明了C++对象和COM对象之间的关系：
C++对象+ IUnknown=COM对象
IUnknown接口用于组件对象的生命周期管理，它执行以下方法实现这个功能
nAddRef
nRelease
组件对象可以被多个应用程序使用，每次执行应用程序，需要将组件装载进内存，在32位Windows如Windows NT，每次应用程序请求组件时，组件的拷贝则映射到应用程序的地址空间并增加引用计数；类似的，每次应用程序和组件断开连接，则减小引用计数。操作系统维护着组件对象存在的连接计数。当计数下降为0，则暗示没有应用程序访问组件并销毁组件。
组件的引用计数使用函数AddRef和Release维护。AddRef增加计数；而Release负责减小计数。
IUnknown还有一个称为QueryInterface的函数，它提供了所支持接口的列表。QueryInterface方法带有两个参数，第一个是一个标识所需要接口的接口标识符，第二个是QueryInterface存放接口地址的指针。因此，QueryInterface用来提供接口的指针来访问组件的功能。
n**IDispatch**当客户端正常时，脚本语言或Visual Basic应用程序调用组件对象的函数，IDispatch接口查询函数的地址并执行它。当组件相互调用时就使用IDispatch。
IDispatch接口是为那些需要和客户特别是Visual Basic和脚本语言等不支持指针的客户相互作用的组件实现的。考虑一个实例，Visual Basic应用程序在Microsoft Access数据库中存储数据并使用Microsoft Excel创建图表。这里，Visual Basic和Microsoft Access数据库组件及Microsoft Excel图表组件相互作用。这只有所有组件都实现了IDispatch接口才可能。
IDispatch接口是用来暴露组件功能的标准接口。但是它由于需依次调用系统函数而增加了系统的开销。
n**IClassFactory** IClassFactory接口用于创建新的COM对象的实例。
到现在，你已经熟悉组件的不同类型，即进程内、本地和远程的。假设你的组件是一个进程内组件，你如何创建类的一个实例呢?答案是C++的new操作符。但是如果组件是本地的或是远程的呢?那么，显然new操作符不是答案。
COM对这个问题的答案是IClassFactory接口。
IClassFactory接口是特定设计用来当实现时创建另一个本地类或远程类的实例的接口。
IClassFactory接口有CreateInstance和LockServer两个方法。
CreateInstance方法实际上创建COM对象的实例。
LockServer方法用来保持组件在内存中以致允许更快地创建实例。
n**IOle** IOleObject是实现OLE对象嵌套的主要接口。 IOleLink是使用OLE对象链接的主要接口。
n**IDataObject **用于实现拖放式功能的接口。
n**IStream****和****IStorage** IStream和IStorage接口用于创建和管理存储对象。这些接口典型用于数据来自多个源的OLE应用程序。
IDataObject、IStream和IStorage三个接个均使用统一数据传输协议(UDT)，它描述了通过不同类型的应用程序的交换标准。
小结
**为什么需要组件对象模型****COM?**
COM组件是执行特定操作的程序或二进制对象。COM为开发组件定义了标准、组件交互、协议标准以及组件的物理结构来确保组件的互操作性。COM规范在COM运行时间库中体现，它提供了在进程中、进程外及跨网络的调用组件的能力。
**COM****的工作原理是什么****?**
COM允许你创建独立的、可重用的、共同联合形成应用程序的组件。在COM中，组件之间的互操作是基于客户/服务器模型的。组件对象是高度封装的。组件对象的内部实现对用户是完全隐藏的。每个组件都有接口，接口是唯一的可以访问组件对象功能的方法。
**接口的特征是什么****?**
接口没有任何实现，所以所有接口不能被实例化。接口是非版本化的并且不可改变。
**什么是****UUID/GUID?**
当开发了组件并全局使用它时，它使用所谓的通用唯一标识行UUID来唯一标识组件。在COM中，UUID是指全局唯一标识符GUID。GUID分为CLSID、IID和LIBID三类。
对组件的十一个文件的剖析
一、stdafx.h
代码选取
…
#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED
#include <afxwin.h>
#include <afxdisp.h>
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
**extern CComModule _Module;**
#include <atlcom.h>
…
代码剖析
n包含了ATL头文件
#include <atlbase.h>
#include <atlcom.h>
n两个声明
#define _ATL_APARTMENT_THREADED
定义了将成为apartment模型的DLL默认线程模型。
_Module是CComModule类型的对象，CComModule实现了COM服务器的基本功能，实现并提供了诸如注册、实例化所支持对象以及通过对象映射管理生命周期等服务。注意因为它通过ATL头文件所引用，所以CComModule对象必须是全局的且必须命名为_Module。
另外，这个类也是ATL的中心，正如主函数对任何程序的入口点(C/C++)，这个类的模块对象处理几个任务，如：服务器锁计数、注册本地服务器的类工厂、根据注册表注册和反注册。
二、stdafx.cpp
…
#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif
#include <atlimpl.cpp>
三、MyProj.def输出定义文件
代码选取
; MyProj.def : Declares the module parameters.
**LIBRARY      "MyProj.DLL"**
EXPORTS
         DllCanUnloadNow     @1 PRIVATE
         DllGetClassObject   @2 PRIVATE
         DllRegisterServer   @3 PRIVATE
         DllUnregisterServer @4 PRIVATE
代码剖析
也称模块定义文件，只有进程内服务器DLL才产生，它提供了被链接程序的信息(DLL文件的名字)
四、MyProj.tlb类型库
编绎后生成，客户端使用时用#import方式导入之。
五、MyProj.idl接口定义语言文件
代码选取
…
import "oaidl.idl"; //此文件包括IDispatch接口的定义，import相当于#include
import "ocidl.idl";
         [
                   object,
                   uuid(65460F9C-3BAB-4055-885A-8ED59F5FA9B0),
                   dual,
                   helpstring("IMyCom Interface"),
                   pointer_default(unique)
         ]
         interface IMyCom : IDispatch
         {
                   [id(1), helpstring("method MyF1")] HRESULT MyF1();
                   [id(2), helpstring("method MyF2")] HRESULT MyF2([in] BSTR str,[out, retval] int* val);
                   [id(3), helpstring("method MyF3")] HRESULT MyF3([in] BSTR str,[out, retval] BSTR* retstr);
                   [id(4), helpstring("method MyF4")] HRESULT MyF4([in] int x,[out, retval] int* val);
         };
[
         uuid(FE651184-11DE-4D01-BD69-B07DDFA12D0C),
         version(1.0),
         helpstring("MyProj 1.0 Type Library")
]
library MYPROJLib
{
         importlib("stdole32.tlb");
         importlib("stdole2.tlb");
         [
                   uuid(FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA),
                   helpstring("MyCom Class")
         ]
         coclass MyCom
         {
                   [default] interface IMyCom;
         };
};
代码剖析
IDL文件由三部分组成，每部分又都由一对中括号和一对大括号组成。每部分也都包含一个UUID来唯一的标识自己
n接口部分：COM接口定义
[
                   object,
                   uuid(65460F9C-3BAB-4055-885A-8ED59F5FA9B0), //接口唯一标识符(GUID/IID)
                   dual, //表示客户机可用两种方法访问这个接口，一种是支持指针的非脚本语言，如VC；一种是非脚本语言，如VB、ASP
                   helpstring("IMyCom Interface"), //将提示字符串与此接口建立联系
                   pointer_default(unique) //指定除参数表中所列属性之外所有指针的缺省特征,unique表示指针可以是NULL，但不支持别名
         ]
         interface IMyCom : IDispatch
         {
                   [id(1), helpstring("method MyF1")] HRESULT MyF1();
                   [id(2), helpstring("method MyF2")] HRESULT MyF2([in] BSTR str,[out, retval] int* val);
                   [id(3), helpstring("method MyF3")] HRESULT MyF3([in] BSTR str,[out, retval] BSTR* retstr);
                   [id(4), helpstring("method MyF4")] HRESULT MyF4([in] int x,[out, retval] int* val);
         };//接口中的方法
n类型库部分
[
         uuid(FE651184-11DE-4D01-BD69-B07DDFA12D0C),
         version(1.0),
         helpstring("MyProj 1.0 Type Library")
]
library MYPROJLib
{
         importlib("stdole32.tlb");
         importlib("stdole2.tlb");
                   <组件类部分>
};
n组件类部分(组件类嵌套在类型库里面)
     [
              uuid(FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA),
              helpstring("MyCom Class")
     ]
     coclass MyCom
     {
              [default] interface IMyCom;
     };
手工为接口添加一个方法，该修改哪些地方?
假设组件类名叫CMyCom,接口叫IMyCom，要加入的方法是MyF5。
首先在IDL文件中找到接口IMyCom的定义，在其中加入如下方法定义：
[id(5), helpstring("method MyF5")] HRESULT MyF1([out,retval] VARIANT_BOOL *ret);
注意：id中的数字不要和已经存在的id重复。
其次，在CMyCom的类定义头文件中加入如下成员函数声明：
public:
STDMETHOD(MyF5)(/*[out, retval]*/ VARIANT_BOOL *ret);
最后，在CMyCom类的实现Cpp文件中加入函数的实现：
STDMETHODIMP CMyCom::MyF5(VARIANT_BOOL *ret)
{
         AFX_MANAGE_STATE(AfxGetStaticModuleState())
         // TODO: Add your implementation code here
*ret = VARAINT_TRUE;
         return S_OK;
}
六、MyCom.h
代码选取
// MyCom.h : Declaration of the CMyCom
#ifndef __MYCOM_H_
#define __MYCOM_H_
#include "resource.h"       // main symbols
// CMyCom
class ATL_NO_VTABLE CMyCom : 
         public CComObjectRootEx<CComSingleThreadModel>,
         public CComCoClass<CMyCom, &CLSID_MyCom>,
         public IDispatchImpl<IMyCom, &IID_IMyCom, &LIBID_MYPROJLib>
{
public:
         CMyCom(){}
DECLARE_REGISTRY_RESOURCEID(IDR_MYCOM)
DECLARE_PROTECT_FINAL_CONSTRUCT()
BEGIN_COM_MAP(CMyCom)
**         COM_INTERFACE_ENTRY(IMyCom)**
**         COM_INTERFACE_ENTRY(IDispatch)**
END_COM_MAP()
// IMyCom
public:
         STDMETHOD(MyF4)(/*[in]*/ int x,/*[out, retval]*/ int* val);
         STDMETHOD(MyF3)(/*[in]*/ BSTR str,/*[out, retval]*/ BSTR* retstr);
         STDMETHOD(MyF2)(/*[in]*/ BSTR str,/*[out, retval]*/ int* val);
         STDMETHOD(MyF1)();
};
#endif //__MYCOM_H_
代码剖析
nCComObjectRootEx
是每个COM组件必须继承的类之一，这个类管理组件的引用计数。由于引用计数是很重要的，所有的ATL COM组件必须从CComObjectRootEx继承。
nCComCoClass
如果一个类从CComCoClass继承，ATL确保了这个类一定是默认的类工厂对象。ATL支持一个默认的工厂实现，并且有使得可以恢复对象CLSID和设置错误信息的函数。
nBEGIN_COM_MAP(CMyCom)
         COM_INTERFACE_ENTRY(IMyCom)
         COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()
BEGIN_COM_MAP和END_COM_MAP这两个宏定义了COM的接口映射。列在映射中的接口是QueryInterface可以返回的接口指针的接口。
n另外CComObject类实现了属于IUnKnown接口的方法。这个类一直是最大的派生类，它提供了聚合和锁模型的选择。但是，对QueryInterface、AddRef、Release的调用会委派给CComObjectRootEx
n还有，我们在这里看到了和接口定义语言文件模样差不多的四个函数的声明
         STDMETHOD(MyF4)(/*[in]*/ int x,/*[out, retval]*/ int* val);
         STDMETHOD(MyF3)(/*[in]*/ BSTR str,/*[out, retval]*/ BSTR* retstr);
         STDMETHOD(MyF2)(/*[in]*/ BSTR str,/*[out, retval]*/ int* val);
         STDMETHOD(MyF1)();
那么它们两者间有什么意义区别呢?
在接口定义语言文件里有如下接口函数(方法)
         interface IMyCom : IDispatch
         {
                   [id(1), helpstring("method MyF1")] HRESULT MyF1();
                   [id(2), helpstring("method MyF2")] HRESULT MyF2([in] BSTR str,[out, retval] int* val);
                   [id(3), helpstring("method MyF3")] HRESULT MyF3([in] BSTR str,[out, retval] BSTR* retstr);
                   [id(4), helpstring("method MyF4")] HRESULT MyF4([in] int x,[out, retval] int* val);
         };
这都是对接口而言的，接口只是包含了这些函数的集合，它并没有实现这些函数。函数是通过组件类(CMyCom class)实现的。组件类实例化时生成组件对象。
七、MyCom.cpp
代码选取
// MyCom.cpp : Implementation of CMyCom
#include "stdafx.h"
#include "MyProj.h"
#include "MyCom.h"
// CMyCom
STDMETHODIMP CMyCom::MyF1()
{
         AFX_MANAGE_STATE(AfxGetStaticModuleState())
         // TODO: Add your implementation code here
         AfxMessageBox("欢迎使用我的组件");
         return S_OK;
}
…
代码剖析
组件类对接口函数的具体实现。
八、MyProj.h
代码选取
略
代码剖析
虚函数表等内部定义
九、MyProj.cpp
代码选取
// MyProj.cpp : Implementation of DLL Exports.
// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f MyProjps.mk in the project directory.
#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "MyProj.h"
#include "MyProj_i.c"
#include "MyCom.h"
**CComModule _Module;**//
BEGIN_OBJECT_MAP(ObjectMap)
**OBJECT_ENTRY(CLSID_MyCom, CMyCom)**//对象MAP，对每个服务器实现有组件都有一个入口
END_OBJECT_MAP()
class CMyProjApp : public CWinApp
{
public:
// Overrides
         // ClassWizard generated virtual function overrides
         //{{AFX_VIRTUAL(CMyProjApp)
         public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
         //}}AFX_VIRTUAL
         //{{AFX_MSG(CMyProjApp)
                   // NOTE - the ClassWizard will add and remove member functions here.
                   //    DO NOT EDIT what you see in these blocks of generated code !
         //}}AFX_MSG
         DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(CMyProjApp, CWinApp)
         //{{AFX_MSG_MAP(CMyProjApp)
                   // NOTE - the ClassWizard will add and remove mapping macros here.
                   //    DO NOT EDIT what you see in these blocks of generated code!
         //}}AFX_MSG_MAP
END_MESSAGE_MAP()
**CMyProjApp theApp;**
BOOL CMyProjApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_MYPROJLib);
    return CWinApp::InitInstance();
}
int CMyProjApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}
// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}
// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}
// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}
// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(TRUE);
}

十、MyProj_i.c
代码选取
/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */
/* link this file in with the server and any clients */
/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Nov 30 23:39:23 2003
 */
/* Compiler settings for C:/MyProj/MyProj.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING( )
#ifdef __cplusplus
extern "C"{
#endif 
#ifndef __IID_DEFINED__
#define __IID_DEFINED__
typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char c[8];
} IID;
#endif // __IID_DEFINED__
#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED
const IID IID_IMyCom = {0x65460F9C,0x3BAB,0x4055,{0x88,0x5A,0x8E,0xD5,0x9F,0x5F,0xA9,0xB0}};
const IID LIBID_MYPROJLib = {0xFE651184,0x11DE,0x4D01,{0xBD,0x69,0xB0,0x7D,0xDF,0xA1,0x2D,0x0C}};
const CLSID CLSID_MyCom = {0xFEB7BDEF,0xFB6F,0x446B,{0xBE,0x31,0xDF,0x0A,0x3A,0xD3,0x91,0xBA}};
#ifdef __cplusplus
}
#endif
十一、MyCom.rgs
代码选取
HKCR
{
         MyProj.MyCom.1 = s 'MyCom Class'
         {
                   CLSID = s '{FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA}'
         }
         MyProj.MyCom = s 'MyCom Class'
         {
                   CLSID = s '{FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA}'
                   CurVer = s 'MyProj.MyCom.1'
         }
         NoRemove CLSID
         {
                   ForceRemove {FEB7BDEF-FB6F-446B-BE31-DF0A3AD391BA} = s 'MyCom Class'
                   {
                            ProgID = s 'MyProj.MyCom.1'
                            VersionIndependentProgID = s 'MyProj.MyCom'
                            ForceRemove 'Programmable'
                            InprocServer32 = s '%MODULE%'
                            {
                                     val ThreadingModel = s 'Apartment'
                            }
                            'TypeLib' = s '{FE651184-11DE-4D01-BD69-B07DDFA12D0C}'
                   }
         }
}
代码剖析
组件注册时用，在这里你可以更改组件的在注册时存放在注册表中的名称及组件的其他相关注册信息。
Developing COM Components using VC-ATL(2)完
Visual Basic、Visual C++组件调用相互通
nVisual Basic调用Visual Basic组件
nVisual C++调用Visual Basic组件
nVisual Basic调用Visual C++组件
nVisual C++调用Visual C++组件
n编写Visual Basic组件
n组件功能：完成三种类型的信用卡号码的合法性检查
n编写步骤：
1.新建一工程，工程类型选择ActiveX DLL，把工程名由默认的Project1改为ValidateCardServer，并把缺省的类模块Class1改为IValidateCard，最后在此类里敲入如下代码。
Option Explicit
**Public** Function **fnValidateCreditCard**(ByVal strCCNumber As String, ByVal strCCType As String) As Boolean
    Dim bValid As Boolean
    strCCType = Trim(strCCType)
    Select Case UCase(strCCType)
        Case "VISA"
            strCCType = "V"
        Case "MASTER"
            strCCType = "M"
        Case "American"
            strCCType = "A"
    End Select
    If fnPrefixCheck(strCCNumber, strCCType) And fnLengthCheck(strCCNumber, strCCType) And fnLuhnCheck(strCCNumber, strCCType) Then
        bValid = True
    Else
        bValid = False
    End If
    fnValidateCreditCard = bValid
End Function
**Private** Function **fnPrefixCheck**(strCCNumber As String, strCCType As String) As Boolean
    Dim bValidPrefix As Boolean
    Select Case UCase(strCCType)
        Case "V"
            If InStr(1, strCCNumber, "4") = 1 Then
                bValidPrefix = True
            End If
        Case "M"
            If InStr(1, strCCNumber, "51") = 1 Or _
               InStr(1, strCCNumber, "52") = 1 Or _
               InStr(1, strCCNumber, "53") = 1 Or _
               InStr(1, strCCNumber, "54") = 1 Or _
               InStr(1, strCCNumber, "55") = 1 Then
               bValidPrefix = True
            End If
        Case "A"
            If InStr(1, strCCNumber, "34") = 1 Or _
               InStr(1, strCCNumber, "37") Then
               bValidPrefix = True
            End If
    End Select
    fnPrefixCheck = bValidPrefix
End Function
**Private** Function **fnLengthCheck**(strCCNumber As String, strCCType As String) As Boolean
    Dim bValidLength As Boolean
    Select Case UCase(strCCType)
        Case "V"    'A visa card has a 13 digit or a 16 digit number
            If Len(strCCNumber) = 13 Or Len(strCCNumber) = 16 Then
                bValidLength = True
            End If
        Case "M"    'A mastercard has a 16 digit number
            If Len(strCCNumber) = 16 Then
                bValidLength = True
            End If
        Case "A"    'American Express has a 15 digit number
            If Len(strCCNumber) = 15 Then
                bValidLength = True
            End If
    End Select
    fnLengthCheck = bValidLength
End Function
**Private** Function **fnLuhnCheck**(strCCNumber As String, strCCType As String) As Boolean
    Dim bValidLuhn As Boolean
    Dim strRev As String
    Dim strCh As String
    Dim intNumber As Integer
    Dim strNumberFinal As String
    Dim intSum As Integer
    Dim intTemp As Integer
    strRev = StrReverse(strCCNumber)
    For intTemp = 1 To Len(strRev)
        strCh = Mid(strRev, intTemp, 1)
        intNumber = CInt(strCh)
        If intTemp Mod 2 = 0 Then
            intNumber = intNumber * 2
            If intNumber > 9 Then
                intNumber = intNumber - 9
            End If
        End If
        strNumberFinal = strNumberFinal & intNumber
    Next intTemp
    For intTemp = 1 To Len(strNumberFinal)
        intSum = intSum + Mid(strNumberFinal, intTemp, 1)
    Next intTemp
    If intSum Mod 10 = 0 Then
        bValidLuhn = True
    Else
        bValidLuhn = False
    End If
    fnLuhnCheck = bValidLuhn
End Function
2.编绎组件。从File菜单中选择 Make ValidateCardServer. dll…
nVisual Basic调用Visual Basic组件
n编写步骤：
1.新建一个标准工程，工程名设置为VBTestVBComProj；引入组件，选择菜单Project->References…，点击浏览，选择C:/VBCom/ ValidateCardServer.dll；敲入如下窗体代码。
Private Sub Command1_Click()
Dim comobj As New **ValidateCardServer**.**IValidateCard**
If comobj.fnValidateCreditCard(Text1.Text, Text2.Text) = True Then
    MsgBox "valid card"
Else
    MsgBox "invalid card"
End If
End Sub
2.编绎运行测试程序。
n代码测试：在信用卡号码中输入4567890123456783，在信用卡类型中输入visa，点击确定，弹出"valid card"的提示框；在信用卡号码中输入4567890123456789，在信用卡类型中输入visa，点击确定按钮，弹出"invalid card"的提示框。
Visual C++调用Visual Basic组件
编写步骤：
n使用Visual C++创建一个基于对话框的应用程序，工程名为VCTestVBComProj，同上面VBTestVBComProj工程一样，在对话框上面放置两个文本框和一个确定按钮。
n在客户端中导入服务器的类型库。导入服务器的类型库将允许你在客户中使用服务器方法和属性。
现在，为了访问服务器的功能，它的类型库应被导入客户的工作空间。这相当于在Visual Basic环境中从访问COM组件的工程中设置组件引用。在MFC或C++中，这使用#import语句完成。
#import关键字在带有扩展名tlh(类型库头文件)的文件中创建了包装类的头，以及在另一个带有扩展名tli(类型库实现文件)的文件中实现了它。这些文件在客户工作空间输出目录中显示，通常是在当前工作空间的Debug目录下。
包装类是#import语句创建的类，它封装了服务器信息。它用来访问客户定义在服务器中的属性和方法。它显示在tlh文件中在前面用关键字namespace的一块区域。包装类的名字是服务器空间名字插入关键字Lib。如果你的服务器组件名字是**Server**，那它的名字是**SERVERLib**。注意：这里讲的是针对Visual C++创建的组件，而不是Visual Basic创建的组件。
2-1在 StdAfx.h 文件中加入代码:
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
//      are changed infrequently
//
#if !defined(AFX_STDAFX_H__A07C8507_082B_4B60_9880_AEB5ADB6D68A__INCLUDED_)
#define AFX_STDAFX_H__A07C8507_082B_4B60_9880_AEB5ADB6D68A__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define VC_EXTRALEAN           // Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>                   // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                            // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
**#import "../VBCom/ValidateCardServer.dll"//**如果你的服务器组件是用Visual C++创建，那么你应该导入有.tlb扩展名的类型库文件来代替.dll文件。具体示例见**Developing COM Components using VC-ATL(1)**
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_STDAFX_H__A07C8507_082B_4B60_9880_AEB5ADB6D68A__INCLUDED_)
2-2编译 StdAfx.cpp，当编绎**#import **语句结构时编绎器产生包装类，这个包装类将封装服务器组件的功能以致它能被客户访问。这个信息将在客户工程空间中输出目录中创建的两个文件：类型库头文件(tlh)和类型库实现文件(tli)。包装类的名称将是默认的服务器名。所以，在本例中，它的名字将是**ValidateCardServer****。**
如果服务器组件是用Visual C++创建，那么包装类的名字将是大写的服务器名后面跟上Lib。如果Visual C++创建的服务器组件有一个名字**ValidateCardServer**，那么包装类的名字将是**VALIDATECARDSERVERLib****。**
2-3现在你必须从对话框访问服务器组件的功能。为此，在VCTestVBComProjDlg.cpp文件中增加如下的语句：
// VCTestVBComProjDlg.cpp : implementation file
//
#include "stdafx.h"
#include "VCTestVBComProj.h"
#include "VCTestVBComProjDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
**using namespace ValidateCardServer;**
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
         CAboutDlg();
…
n初始化COM库。应用程序必须在调用COM函数前初始化COM库。
因为客户和组件间的交互是通过COM库，所以必须初始化COM库。为了初始化COM库，可调用CoInitialize函数。CoInitialize函数的返回值是HRESULT数据类型，它暗示了该操作是成功还是失败。返回值可以是S_OK或S_FALSE。
语法：
HRESULT CoInitialize(NULL);
n获得服务器的CLSID。客户应用程序只有在获得COM服务器组件的CLSID后才能实例化服务器。
为了创建服务器的实例，必须获得服务器的CLSID。因为COM的CLSID很难记住，所以你必须使用可读的ProgID获得CLSID。这通过调用CLSIDFromProgID函数完成。这个函数带有两个参数：输入参数ProgID和输出参数CLSID。
语法：
HRESULT  CLSIDFromProgID(LPCOLESTR lpszProgID,LPCLSID pclsid);
n创建COM服务器组件的实例。创建COM服务器组件的实例并返回所需接口的指针。
一旦获得组件的CLSID，它可以用来创建COM服务器的实例。使用CoCreateInstance函数来创建服务器组件的实例。**COM****将不提供实例化对象的指针，****COM****只提供接口的指针。**
语法：
HRESULT  CoCreateInstance(
REFCLSID rclsid,       //要创建组件的CLSID
LPUNKNOWN punkouter,//当对象支持聚合时使用，否则为NULL
DWORD dwclsContext, //对象初化化环境(进程内、进程外或远程)
REFIID riid,            //要获得类的接口的ID
LPVOID *ppv           //**[out]**所需求接口的指针
);
1.CoCreateInstance函数的第一个参数是实例化组件的CLSID，它通过CLSIDFromProgID函数获得。为了寻找所需要的组件，CoCreateInstance函数使用它在注册表中的HKEY_CLASSES_ROOT/CLSID部分寻找匹配。如果未发现匹配，则CoCreateInstance失败。
2.CoCreateInstance函数的第二个参数是用来指明你是否创建对象作为聚合对象的一部分。既然该程序不使用聚合，故使用NULL。
3.CoCreateInstance函数的第三个参数是用来指明组件运行的环境。在COM中，你可以创建三种类型的组件：进程内、进程外和远程。下面的表显示了不同的类环境变量。
|Class环境|说明|
|----|----|
|CLSCTX_INPROC_SERVER|指明进程内服务器。|
|CLSCTX_INPROC_HANDLER|指明进程内服务器的特定类型，在客户进程中运行但类的实例被远程访问。|
|CLSCTX_LOCAL_SERVER|指明了本地(进程外)服务器。|
|CLSCTX_REMOTE_SERVER|指明了远程服务器，使用CoCreateInstanceEx函数。|
|CLSCTX_SERVER|指明如果不是特定的组件类型。这个值找回进程内、进程外、远程中的第一个变量。|
4.CoCreateInstance函数的第四个参数指明所需要接口的接口标识符IID。接口ID由__uuidof函数返回。接口指针在第五个参数中返回。
5.输出参数到指定接口指针。
n使用COM对象。在获得接口指针之后，客户应用程序能调用COM服务器对象的方法和属性。
         CoCreateInstance函数返回一个组件的接口指针。使用这个指针，可以访问服务器的属性和方法。
n终止COM库。在释放COM对象后，客户应用程序应指示不再需要已初始化的COM库。
在释放COM对象的实例后，初始化的COM库不再需要。为了释放初始化的COM库，可调用函数CoUninitialize。
语法：
void CoUninitialize();
n附完整代码
         void CVCTestVBComProjDlg::OnButton1() 
{
         // TODO: Add your control notification handler code here
         HRESULT hr=CoInitialize(NULL);//step 3
         CLSID clsid;
         hr=CLSIDFromProgID(OLESTR("ValidateCardServer.IValidateCard"),&clsid);//step 4
         _IValidateCard * cardptr;
         hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(_IValidateCard),(LPVOID *)&cardptr);
         if(FAILED(hr))
         {
                   AfxMessageBox("Server Creation Failed");
                   return;
         }//step 5
         char cCardNum[20];
         GetDlgItemText(IDC_EDIT1,cCardNum,20);
         char cCardType[10];
         GetDlgItemText(IDC_EDIT2,cCardType,10);
         _bstr_t bstCardNum(cCardNum),bstCardType(cCardType);
         long lStatus;
         lStatus=cardptr->fnValidateCreditCard(bstCardNum,bstCardType);
         if(lStatus==-1)
                   AfxMessageBox("valid card");
         else
                   AfxMessageBox("invalid card");//step 6          
         CoUninitialize();//step 7
}

编写Visual C++组件
4.编写实现函数功能的代码
     ValidateCard.cpp清单
// ValidateCard.cpp : Implementation of CValidateCard
#include "stdafx.h"
#include "ValidateCreditCardServer.h"
#include "ValidateCard.h"
/////////////////////////////////////////////////////////////////////////////
// CValidateCard
STDMETHODIMP CValidateCard::ValidateCreditCard(BSTR bstrCCNo, BSTR bstrCCType, long *val)
{
         AFX_MANAGE_STATE(AfxGetStaticModuleState())
         // TODO: Add your implementation code here
    CString strType(bstrCCType);
         strType.MakeUpper();
         if(strType.Compare("VISA")==0)
                   bstrCCType=SysAllocString(OLESTR("V"));
         else if(strType.Compare("MASTER")==0)
                   bstrCCType=SysAllocString(OLESTR("M"));
         else if(strType.Compare("AMERICAN")==0)
                   bstrCCType=SysAllocString(OLESTR("A"));
         else
         {
                   *val=0;
                   return S_FALSE;
         }
         if((PrefixCheck(bstrCCNo,bstrCCType)==S_OK) && (LengthCheck(bstrCCNo,bstrCCType)==S_OK) && (LuhnCheck(bstrCCNo,bstrCCType)==S_OK))
         {
                   *val=1;
                   return S_OK;
         }
         else
         {
                   *val=0;
                   return S_FALSE;
         }
         return S_FALSE;
}
STDMETHODIMP CValidateCard::LuhnCheck(BSTR bstrCCNo, BSTR bstrCCType)
{
         AFX_MANAGE_STATE(AfxGetStaticModuleState())
         // TODO: Add your implementation code here
         CString strobj(bstrCCNo);
         CString strReverse,strFinal;
         int iNum,iSum;
         char cTemp;
         strReverse=strobj;
         strReverse.MakeReverse();
         for(int iTemp=0;iTemp<strReverse.GetLength();iTemp++)
         {
                   cTemp=strReverse.GetAt(iTemp);
                   iNum=atoi(&cTemp);
                   if(iTemp % 2 !=0)
                   {
                            iNum=iNum*2;
                            if(iNum>9)
                                     iNum-=9;
                   }
                   cTemp=iNum+'0';
                   strFinal+=cTemp;
         }
         for(iTemp=0;iTemp<strFinal.GetLength();iTemp++)
         {
                   cTemp=strFinal.GetAt(iTemp);
                   iSum+=atoi(&cTemp);
         }
         if(iSum % 10 == 0)
                   return S_OK;
         else
                   return S_FALSE;
}
STDMETHODIMP CValidateCard::LengthCheck(BSTR bstrCCNo, BSTR bstrCCType)
{
         AFX_MANAGE_STATE(AfxGetStaticModuleState())
         // TODO: Add your implementation code here
         CString strType(bstrCCType),strLength(bstrCCNo);
         int ilen=strLength.GetLength();
         if(strType=="V")
         {
                   if(ilen==16)return S_OK;
         }
         else if(strType=="M")
         {
                   if(ilen==16)return S_OK;
         }
         else if(strType=="A")
         {
                   if(ilen==15)return S_OK;
         }
         return S_FALSE;
}
STDMETHODIMP CValidateCard::PrefixCheck(BSTR bstrCCNo, BSTR bstrCCType)
{
         AFX_MANAGE_STATE(AfxGetStaticModuleState())
         // TODO: Add your implementation code here
         CString strNumber(bstrCCNo),strType(bstrCCType);
         if(strType=="V")
         {
                   if(strNumber.Find("4",0)>=0)
                            return S_OK;
         }
         else if(strType=="M")
         {
                   if(strNumber.Find("51",0)>=0 || strNumber.Find("52",0)>=0 || strNumber.Find("53",0)>=0 || strNumber.Find("54",0)>=0)
                            return S_OK;
         }
         else if(strType=="A")
         {
                   if(strNumber.Find("34",0)>=0 || strNumber.Find("37",0)>=0)
                            return S_OK;
         }
         return S_FALSE;
}
5.编绎组件
编绎会读取ValidateCard.rgs文件进行组件的自动注册，若在Windows 9X环境下，请运行regsvr32程序进行注册，具体参见
Developing COM Components using VC-ATL(2-3)  COM对象的注册角色
同时编绎完成后，会
在Debug目录下生成ValidateCreditCardServer.dll
在Debug上一层目录下，即工程名工作空间目录下生成ValidateCreditCardServer.tlb
这两个文件都可以导入到客户端，测试时分别会介绍这两种方法。
nVisual Basic调用Visual C++组件
n编写步骤：
1.新建一个标准工程，工程名设置为VBTestVCComProj；引入组件，选择菜单Project->References…，点击浏览，选择C:/ValidateCreditCardServer/Debug/ValidateCreditCardServer.dll；敲入如下窗体代码(别忘了在窗体上放两个文本框和一个按钮哦)。
Private Sub Command1_Click()
Dim comobj As New **VALIDATECREDITCARDSERVERLib**.**ValidateCard**
If comobj.**ValidateCreditCard**(Text1.Text, Text2.Text) = **1** Then
    MsgBox "valid card"
Else
    MsgBox "invalid card"
End If
End Sub
2.编绎运行测试程序。
n代码测试：同上(略)。
nVisual C++调用Visual C++组件
在这里我不再详细介绍步骤了，相信通过前面两个例子介绍，你应该掌握了其具体的操作要点。在这里我只扼要地提一下纲领。
1.使用Visual C++创建一个基于对话框的应用程序，工程名为VCTestVCComProj，同上面几个测试工程一样，在对话框上面放置两个文本框和一个确定按钮。
2.在客户端中导入服务器的类型库
2-1在 StdAfx.h 文件中加入代码
**#import "../ValidateCreditCardServer/ValidateCreditCardServer.tlb"**
或
**#import "../ValidateCreditCardServer/Debug/ValidateCreditCardServer.dll"**
这两种方法都可以!
2-2编译 StdAfx.cpp，产生包装类，即在客户工程空间中输出目录(C:/VCTestVCComProj/Debug)中创建的两个文件：类型库头文件(**ValidateCreditCardServer.tlh**)和类型库实现文件(**ValidateCreditCardServer.tli**)。
2-3对话框访问服务器组件的功能需要使用服务器包装类名字空间。为此，在VCTestVCComProjDlg.cpp文件头部中增加如下的语句：
**using namespace VALIDATECREDITCARDSERVERLib;**
3.初始化COM库
4.获得服务器的CLSID
5.创建COM服务器组件的实例
6.使用COM对象
7.终止COM库
VCTestVCComProjDlg.cpp清单
// VCTestVCComProjDlg.cpp : implementation file
//
#include "stdafx.h"
#include "VCTestVCComProj.h"
#include "VCTestVCComProjDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
**using namespace VALIDATECREDITCARDSERVERLib;**
…
BEGIN_MESSAGE_MAP(CVCTestVCComProjDlg, CDialog)
         //{{AFX_MSG_MAP(CVCTestVCComProjDlg)
         ON_WM_SYSCOMMAND()
         ON_WM_PAINT()
         ON_WM_QUERYDRAGICON()
**         ON_BN_CLICKED(IDC_BUTTON1, OnButton1)**
         //}}AFX_MSG_MAP
END_MESSAGE_MAP()
…
void CVCTestVCComProjDlg::OnButton1() 
{
         // TODO: Add your control notification handler code here
         HRESULT hr=CoInitialize(NULL);// 初始化COM库
         CLSID clsid;
         hr=CLSIDFromProgID(OLESTR("**ValidateCreditCardServer**.**ValidateCard**"),&clsid);// 获得服务器的CLSID
         IValidateCard * cardptr;
         hr=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(**IValidateCard**),(LPVOID *)&cardptr);
         if(FAILED(hr))
         {
                   AfxMessageBox("Server Creation Failed");
                   return;
         }//创建COM服务器组件的实例
         char cCardNum[20],cCardType[10];
         GetDlgItemText(IDC_EDIT1,cCardNum,20);
         GetDlgItemText(IDC_EDIT2,cCardType,10);
         _bstr_t bstCardNum(cCardNum),bstCardType(cCardType);
         long lStatus;
         lStatus=cardptr->**ValidateCreditCard**(bstCardNum,bstCardType);
         if(lStatus==**1**)
                   AfxMessageBox("valid card");
         else 
                   AfxMessageBox("invalid card");          //使用COM对象
         CoUninitialize();//终止COM库
}
8.编绎运行测试程序。
n代码测试：同上(略)。

最后让我们分析一下C:/VCTestVCComProj/Debug/ValidateCreditCardServer.tlh作为Developing COM Components using VC-ATL(3)的结尾吧。
ValidateCreditCardServer.tlh清单
// Created by Microsoft (R) C/C++ Compiler Version 12.00.8168.0 (ad98f509).
//
// c:/vctestvccomproj/debug/ValidateCreditCardServer.tlh
//
// C++ source equivalent of Win32 type library ../ValidateCreditCardServer/Debug/ValidateCreditCardServer.dll
// compiler-generated file created 12/19/03 at 03:29:03 - DO NOT EDIT!
#pragma once
#pragma pack(push, 8)
#include <comdef.h>
namespace **VALIDATECREDITCARDSERVERLib {…………………………………..………**名字空间
//
// Forward references and typedefs
//
struct /* coclass */ ValidateCard;………………………………………………………………..….. 组件类
struct __declspec(uuid("b1d1c73c-e340-4fdc-8734-92ecec64dc4e"))
/* dual interface */ IValidateCard;………………………………………………….………………用户接口
//
// Smart pointer typedef declarations
//
**_COM_SMARTPTR_TYPEDEF(IValidateCard, __uuidof(IValidateCard));………..………**智能指针
//
// Type library items
//
struct __declspec(uuid("c79635d2-7b86-4b20-a83c-0565dca54c3c"))…………………组件类CLSID
ValidateCard;
    // [ default ] interface IValidateCard
struct __declspec(uuid("b1d1c73c-e340-4fdc-8734-92ecec64dc4e"))……………………用户接口IID
IValidateCard : IDispatch
{
    //
    // Wrapper methods for error-handling
    //
**long** ValidateCreditCard (
**_bstr_t** bstrCCNo,
**_bstr_t** bstrCCType );
    HRESULT LuhnCheck (
**_bstr_t** bstrCCNo,
**_bstr_t** bstrCCType );
    HRESULT LengthCheck (
**_bstr_t** bstrCCNo,
**_bstr_t** bstrCCType );
    HRESULT PrefixCheck (
**_bstr_t** bstrCCNo,
**_bstr_t **bstrCCType );……………………………………………….………….. 接口方法(对外)
    //
    // Raw methods provided by interface
    //
    virtual HRESULT __stdcall raw_ValidateCreditCard (
**BSTR** bstrCCNo,
**BSTR** bstrCCType,
**long *** val ) = 0;
    virtual HRESULT __stdcall raw_LuhnCheck (
**BSTR** bstrCCNo,
**BSTR** bstrCCType ) = 0;
    virtual HRESULT __stdcall raw_LengthCheck (
**BSTR** bstrCCNo,
**BSTR** bstrCCType ) = 0;
    virtual HRESULT __stdcall raw_PrefixCheck (
**BSTR** bstrCCNo,
**BSTR** bstrCCType ) = 0;…………………………………………………..……. 接口方法(对内)
};
//
// Wrapper method implementations
//
**#include "c:/vctestvccomproj/debug/ValidateCreditCardServer.tli"…………..**类型库实现文件
**} // namespace VALIDATECREDITCARDSERVERLib……………………………..………..**名字空间
#pragma pack(pop)
参照COM组件端C:/ValidateCreditCardServer/ValidateCreditCardServer.idl
ValidateCreditCardServer.idl清单
// ValidateCreditCardServer.idl : IDL source for ValidateCreditCardServer.dll
//
// This file will be processed by the MIDL tool to
// produce the type library (ValidateCreditCardServer.tlb) and marshalling code.
import "oaidl.idl";
import "ocidl.idl";
         [
                   object,
                   uuid(B1D1C73C-E340-4FDC-8734-92ECEC64DC4E),……………………………. IID部分
                   dual,
                   helpstring("IValidateCard Interface"),
                   pointer_default(unique)
         ]
         interface IValidateCard : IDispatch
         {
                   [id(1), helpstring("method ValidateCreditCard")] HRESULT ValidateCreditCard([in] BSTR bstrCCNo,[in]BSTR bstrCCType,[out,retval]long *val );
                   [id(2), helpstring("method LuhnCheck")] HRESULT LuhnCheck([in] BSTR bstrCCNo,[in]BSTR bstrCCType);
                   [id(3), helpstring("method LengthCheck")] HRESULT LengthCheck([in] BSTR bstrCCNo,[in]BSTR bstrCCType);
                   [id(4), helpstring("method PrefixCheck")] HRESULT PrefixCheck([in] BSTR bstrCCNo,[in]BSTR bstrCCType);
         };
[
         uuid(B3404054-C33E-4931-B737-5783CEEFECDB),………………………………… LIBID部分
         version(1.0),
         helpstring("ValidateCreditCardServer 1.0 Type Library")
]
library **VALIDATECREDITCARDSERVERLib**
{
         importlib("stdole32.tlb");
         importlib("stdole2.tlb");
         [
                   uuid(C79635D2-7B86-4B20-A83C-0565DCA54C3C),…………………….……CLSID部分
                   helpstring("ValidateCard Class")
         ]
         coclass ValidateCard
         {
                   [default] interface IValidateCard;
         };
};
小结:
Developing COM Components using VC-ATL(3)主要介绍了Visual C++与Visual Basic编写的组件的相互间的调用。在接下来一篇里将介绍双接口。
Developing COM Components using VC-ATL(3)完

转自：[http://blog.csdn.net/callzjy/category/12933.aspx?PageNumber=2](http://blog.csdn.net/callzjy/category/12933.aspx?PageNumber=2)
确定创建服务器组件的最有效的工具
当你着手创建COM组件时，你可以选择与Visual Basic、Delphi、MFC和ATL不同的工具。让我们比较一下每个工具，来揭露它们各自的优点和缺点。
当着手写COM类时，你可以有几种选择，你可以选择用C++满足COM。这种选择提供了最大的灵活性但是很枯燥。另外，你可以选择Visual C++提供的两种主要的框架来写COM：MFC和ATL。
MFC使用文档视图体系结构设计良好的开发基于Windows的应用程序。使用MFC设计基于Windows应用程序要求运行时间库MFC42.DLL来执行。当用于开发确定类型的应用程序时，MFC遇到要求大的运行时间库开销。MFC有利于像基于Windows的单层应用程序、请求许多GUI客户端的应用程序、ActiveX控件和OLE文档的开发。对COM的支持是后来增加的并且依靠它的文档视图体系结构。虽然Visual C++向导使得用MFC创建组件容易很多，但于框架结构先天设计缺陷，所以它并不是一种灵活的方法。而且，组件以体积大和要求扩展DLL而告终。
相反，ATL是针对COM来设计的。此外，ATL提供像聚合和包容不同的类合成技术的完全支持。ATL使用C++模板类的多继承技术来实现COM类。ATL允许COM组件、自动化服务器和ActiveX控件的创建。使用ATL创建的COM更小更快。ATL提供许多COM接口的本地支持和由Windows NT服务器支持的新线程模型。由于ATL主要使用模板的概念，创建的组件大小很小，并且运行期DLL很小并可动态加载。
所以，你应用ATL创建COM。
组件功能：同上(编写Visual Basic组件)。
组件将包含以下函数：ValidateCreditCard、PrefixCheck、LengthCheck、LuhnCheck。仅ValidateCreditCard可以被应用程序调用来封装组件的工作方式。
为了创建验证信用卡的服务器组件，你需要执行以下步骤：
1.创建应用程序框架
启动VC++6.0，从File菜单选项选中 New 菜单项，在出现的 New 对话框中选中 Projects 卡片，从显示的工程类型中选中 ATL COM  AppWizard。指定工程工作空间名，即在 Project Name 编辑框中输入项目名ValidateCreditCardServer，以区别于Visual Basic组件工程ValidateCardServer。选择合适的 Location 后，按确认按钮进入下一个对话框：ATL  COM  Appwizard  -  step 1 of  1，在 Server Type 中选择 Dynamic  Link  Library [ DLL ]，即进程内服务器组件。选中 Support  MFC 选项。在按下 Finish 和 Ok 按钮后，一个组件的框架已经建立。现在让我们来看一下生成的文件的清单
nstdafx.h
nstdafx.cpp
nValidateCreditCardServer.idl
nValidateCreditCardServer.rc
nResource.h
nValidateCreditCardServer.def
nValidateCreditCardServerps.def
nValidateCreditCardServer.h
nValidateCreditCardServer.cpp
nValidateCreditCardServer.dsp
nValidateCreditCardServer.dsw
nValidateCreditCardServer.ncb
nValidateCreditCardServerps.mk
nValidateCreditCardServer.clw
ValidateCreditCardServer.cpp清单
// ValidateCreditCardServer.cpp : Implementation of DLL Exports.
// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f ValidateCreditCardServerps.mk in the project directory.
#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
**#include "ValidateCreditCardServer.h"**
**#include "ValidateCreditCardServer_i.c"**
**#include "ValidateCard.h"**
**CComModule _Module;**
**BEGIN_OBJECT_MAP(ObjectMap)**
**OBJECT_ENTRY(CLSID_ValidateCard, CValidateCard)**
**END_OBJECT_MAP()**
class CValidateCreditCardServerApp : public CWinApp
{
public:
// Overrides
         // ClassWizard generated virtual function overrides
         //{{AFX_VIRTUAL(CValidateCreditCardServerApp)
         public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
         //}}AFX_VIRTUAL
         //{{AFX_MSG(CValidateCreditCardServerApp)
                   // NOTE - the ClassWizard will add and remove member functions here.
                   //    DO NOT EDIT what you see in these blocks of generated code !
         //}}AFX_MSG
         DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(CValidateCreditCardServerApp, CWinApp)
         //{{AFX_MSG_MAP(CValidateCreditCardServerApp)
                   // NOTE - the ClassWizard will add and remove mapping macros here.
                   //    DO NOT EDIT what you see in these blocks of generated code!
         //}}AFX_MSG_MAP
END_MESSAGE_MAP()
CValidateCreditCardServerApp theApp;
BOOL CValidateCreditCardServerApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_VALIDATECREDITCARDSERVERLib);
    return CWinApp::InitInstance();
}
int CValidateCreditCardServerApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}
/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE
STDAPI **DllCanUnloadNow**(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type
STDAPI **DllGetClassObject**(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}
/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
STDAPI **DllRegisterServer**(void)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}
/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
STDAPI **DllUnregisterServer**(void)
{
    return _Module.UnregisterServer(TRUE);
}
2.创建框架中的组件
从Insert菜单中选中 New ATL  Object…菜单项，出现 ATL Object  Wizard 对话框。
在ATL Object  Wizard中，在左边的 Category 中选择 Objects，右边的 Objects 中选中 Simple  Object 项。按 Next 按钮。
在出现的 ATL Object  Wizard 属性对话框中 Names 卡片中的八个编辑框中左上方的 Short Name 编辑框中输入短名ValidateCard ，其他七个编辑框的内容会自动生成。在这里我们也对这一对话框的其他七个编辑框的内容作一个详细地剖析：
C++块
Short Name：ValidateCard//用户输入
Class：CValidateCard//产生组件类的实现类的类名
.H File：ValidateCard.h//产生组件类的实现类的头文件
.CPP File：ValidateCard.cpp//产生组件类的实现类的实现文件
COM块
CoClass：ValidateCard//产生的组件类，区别于组件类的实现类
Interface：IValidateCard//创建组件类时，同时创建一个此组件类的一个用户接口
Type：ValidateCard Class//组件类的类型
ProgID：ValidateCreditCardServer.ValidateCard// CLSIDFromProgID调用，用它得到CLSID
然后按确认按钮退出。这将生成如上的信息并且在需要的文件中同样地作一个登陆，特别是IDL文件。在这里我们比较一下创建组件前后的两个版本的IDL文件。
创建组件前的ValidateCreditCardServer.idl文件，即创建应用程序框架后的IDL文件。
// ValidateCreditCardServer.idl : IDL source for ValidateCreditCardServer.dll
//
// This file will be processed by the MIDL tool to
// produce the type library (ValidateCreditCardServer.tlb) and marshalling code.
import "oaidl.idl";
import "ocidl.idl";
[
         uuid(B3404054-C33E-4931-B737-5783CEEFECDB),
         version(1.0),
         helpstring("ValidateCreditCardServer 1.0 Type Library")
]
library VALIDATECREDITCARDSERVERLib
{
         importlib("stdole32.tlb");
         importlib("stdole2.tlb");
};
创建组件后的IDL文件
ValidateCreditCardServer.idl清单
import "oaidl.idl";
import "ocidl.idl";
         [
                   object,
                   uuid(B1D1C73C-E340-4FDC-8734-92ECEC64DC4E),
                   dual,
                   helpstring("IValidateCard Interface"),
                   pointer_default(unique)
         ]
         interface IValidateCard : IDispatch
         {
         };
[
         uuid(B3404054-C33E-4931-B737-5783CEEFECDB),
         version(1.0),
         helpstring("ValidateCreditCardServer 1.0 Type Library")
]
library VALIDATECREDITCARDSERVERLib
{
         importlib("stdole32.tlb");
         importlib("stdole2.tlb");
         [
                   uuid(C79635D2-7B86-4B20-A83C-0565DCA54C3C),
                   helpstring("ValidateCard Class")
         ]
         coclass ValidateCard
         {
                   [default] interface IValidateCard;
         };
};
文中黑体部分表示在框架中加入组件类后形成的代码，在框架中加入组件类之前，只有类型库部分，而在创建组件类后我们可以看到上面部分是一个组件类的用户接口，下面部分是一个组件类，其嵌套在类型库中。呵呵，三部分全到齐了。
另外，也说明一下VALIDATECREDITCARDSERVERLib这几个字母的用处，大家想到没有呀?呵呵，它就是在客户端中使用名字空间时使用的呀。详细请见**Visual C++****调用****Visual C++****组件**。
3.增加组件的功能。
要实现这一点，你必须增加检验信用卡有效性的函数。如何增加函数到你的组件，请按以下步骤操作：
在 VC++工作平台的左边的 Workspace 的 ClassView 卡片中找到接口 IValidateCard项，按右键，在出现的快捷菜单中选择 Add  Method …，出现 Add  Method  to Interface 对话框，在对话框中输入要增加的函数的函数名、参数和返回值类型。在这里，我们在Method Name中输入
ValidateCreditCard
在Parameters中输入
[in] BSTR bstrCCNo,[in]BSTR bstrCCType,[out,retval]long *val
然后，按确认按钮退出。
类似地输入以下三个函数
|函数功能|Method Name|Parameters|
|----|----|----|
|检验信用卡的号码是否合法|LuhnCheck|[in] BSTR bstrCCNo,[in]BSTR bstrCCType|
|检验信用卡的长度是否合法|LengthCheck|[in] BSTR bstrCCNo,[in]BSTR bstrCCType|
|检验信用卡的类型是否合法|PrefixCheck|[in] BSTR bstrCCNo,[in]BSTR bstrCCType|
现在你可以看见为你的组件创建的接口定义语言文件的变化，你将在IDL文件ValidateCreditCardServer.idl中看到上面增加的所有函数的入口。
ValidateCreditCardServer.idl清单
// ValidateCreditCardServer.idl : IDL source for ValidateCreditCardServer.dll
//
// This file will be processed by the MIDL tool to
// produce the type library (ValidateCreditCardServer.tlb) and marshalling code.
import "oaidl.idl";
import "ocidl.idl";
         [
                   object,
                   uuid(B1D1C73C-E340-4FDC-8734-92ECEC64DC4E),
                   dual,
                   helpstring("IValidateCard Interface"),
                   pointer_default(unique)
         ]
         interface IValidateCard : IDispatch
         {
                   [id(1), helpstring("method ValidateCreditCard")] HRESULT ValidateCreditCard([in] BSTR bstrCCNo,[in]BSTR bstrCCType,[out,retval]long *val );
                   [id(2), helpstring("method LuhnCheck")] HRESULT LuhnCheck([in] BSTR bstrCCNo,[in]BSTR bstrCCType);
                   [id(3), helpstring("method LengthCheck")] HRESULT LengthCheck([in] BSTR bstrCCNo,[in]BSTR bstrCCType);
                   [id(4), helpstring("method PrefixCheck")] HRESULT PrefixCheck([in] BSTR bstrCCNo,[in]BSTR bstrCCType);
         };
...
另外它也在组件类的实现类的头文件ValidateCard.h和实现文件ValidateCard.cpp中分别加入函数的入口。
// ValidateCard.h : Declaration of the CValidateCard
#ifndef __VALIDATECARD_H_
#define __VALIDATECARD_H_
#include "resource.h"       // main symbols
/////////////////////////////////////////////////////////////////////////////
// CValidateCard
class ATL_NO_VTABLE CValidateCard : 
         public **CComObjectRootEx**<CComSingleThreadModel>,
         public **CComCoClass**<CValidateCard, &CLSID_ValidateCard>,
         public **IDispatchImpl**<IValidateCard, &IID_IValidateCard, &LIBID_VALIDATECREDITCARDSERVERLib>
{
public:
         CValidateCard()
         {
         }
DECLARE_REGISTRY_RESOURCEID(IDR_VALIDATECARD)
DECLARE_PROTECT_FINAL_CONSTRUCT()
****BEGIN_COM_MAP(CValidateCard)****         COM_INTERFACE_ENTRY(IValidateCard)****         COM_INTERFACE_ENTRY(IDispatch)****END_COM_MAP()**// IValidateCardpublic:         STDMETHOD(PrefixCheck)(/*[in]*/ BSTR bstrCCNo,/*[in]*/BSTR bstrCCType);         STDMETHOD(LengthCheck)(/*[in]*/ BSTR bstrCCNo,/*[in]*/BSTR bstrCCType);         STDMETHOD(LuhnCheck)(/*[in]*/ BSTR bstrCCNo,/*[in]*/BSTR bstrCCType);         STDMETHOD(ValidateCreditCard)(/*[in]*/ BSTR bstrCCNo,/*[in]*/BSTR bstrCCType,/*[out,retval]*/long *val);};#endif //__VALIDATECARD_H_**

，创建一个客户程序
用 MFC  AppWizard ( exe ) 创建一个基于对话框的应用程序 MyExe。
在对话框中放置四个按钮，分别为 MyF1、MyF2 、MyF3和 MyF4。
用 ClassWizard 生成单击按钮的四个响应函数 OnMyF1() 、OnMyF2() 、OnMyF3()和 OnMyF4()。，创建一个基于对话框的客户程序。
2，把服务器类型库导入客户工作平台。
3，初始化 COM 库。
4，获得服务器的 GLSID。
5，创建 COM 组件服务器组件的实例。
6，使用 COM 对象。
7，终止 COM 库。创建 COM 组件客户
一、创建模型(工程) MyProj
在VC++6.0工作平台中，点击菜单 File 下的 New 菜单项，在出现的 New 对话框中选中 Projects 卡片，在列表框中选中 ATL COM  AppWizard（活动模板库组件导航）。　　一、创建一个模型(工程) MyProj。
二、给模型(工程)增加一个组件 MyCom。
三、给组件增加方法(函数) MyF1、MyF2、MyF3、MyF4。
