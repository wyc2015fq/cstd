# 利用MFC的Csocket类实现网络通信  - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 14:46:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5721


**利用MFC的Csocket类实现网络通信 **　　近年来，利用Internet进行网际间通讯,在WWW浏 览、FTP、Gopher这些常规服务，以及在网络电话、多媒体会议等这些对实时性要求严格 的应用中成为研究的热点，而且已经是必需的了。Windows环境下进行通讯程序设计的最基本方法是应用Windows Sockets实现进程间的通讯，为此微软提供了大量基于Windows Sockets的通讯API，如WinSockAPI、WinInetAPI和ISAPI，并一直致力于开发更快、 更容易的通讯API，将其和MFC集成在一起以使通讯编程越来越容易。本实例重点介绍使用MFC的CSocket类编写网络通讯程序的方法，并通过使用CSocket类实现了网络聊天程序。程序编译运行后的界面效果如图一所示：

![](http://dev.yesky.com/imagelist/05/11/wlpmww1b91uo.jpg)
图一、网络聊天程序界面[效果图](http://www.yesky.com/key/1059/6059.html)

**一、实现方法**

　　微软的MFC把复杂的[WinSock](http://www.yesky.com/key/1632/1632.html) API函数封装到类里，这使得编写网络应用程序更容易。CAsyncSocket类逐个封装了WinSock API，为高级网络程序员 提供了更加有力而灵活的方法。这个类基于程序员了解网络通讯的假设，目的是为了在MFC中使用WinSock，程序员有责任处理诸如阻塞、字节顺序和在Unicode与MBCS 间转换字符的任务。为了给程序员提供更方便的接口以自动处理这些任务，MFC给出 了CSocket类，这个类是由CAsyncSocket类继承下来的，它提供了比CAsyncSocket更高层的WinSock API接口。Csocket类和CsocketFile类可以与Carchive类一起合作来管理发送和接收的数据，这使管理数据收发更加便利。CSocket对象提供阻塞模式，这对于Carchive的同步操作是至关重要的。阻塞函数（如Receive()、Send()、ReceiveFrom()、SendTo() 和Accept()）直到操作完成后才返回控制权，因此如果需要低层控制和高效率，就使用CasyncSock类；如果需要方便，则可使用Csocket类。 

　　一些网络应用程序(如网络电话、多媒体会议工具)对实时性要求非常强，要求能够直接应用WinSock发送和接收数据。为了充分利用MFC 的优势，首选方案应当是MFC中的CAsyncSocket类或CSocket类，这两个类完全封装了WinSock API，并提供更多的便利。本实例介绍应用这两个类的编程模型，并引出相关的成员函数与一些概念的解释。 

　　CSocket类是由CAsyncSocket继承而来的，事实上，在MFC中CAsyncSocket 逐个封装了WinSock API，每个CAsyncSocket对象代表一个Windows Socket对象，使用CAsyncSocket 类要求程序员对网络编程较为熟悉。相比起来，CSocket类是CAsyncSocket的派生类， 继承了它封装的WinSock API。

　　一个CSocket对象代表了一个比CAsyncSocket对象更高层次的Windows Socket的抽象，CSocket类与CSocketFile类和CArchive类一起工作来发送和接收数据，因此使用它更加容易使用。CSocket对象提供阻塞模式，因为阻塞功 能对于CArchive的同步操作是至关重要的。在这里有必要对阻塞的概念作一解释： 一个socket可以处于"阻塞模式"或"非阻塞模式"，当一个套接字处于阻塞模式（即同步操作）时，它的阻塞函数直到操作完成才会返回控制权，之所以称为阻塞是因为此套接字的阻塞函数在完成操作返回之前什么也不能做。如果一个socket处于非阻塞模式（即异步操作），则会被调用函数立即返回。在CAsyncSocket类中可以用GetLastError 成员函数查询最后的错误，如果错误是WSAEWOULDBLOCK则说明有阻塞，而CSocket绝不会返回WSAEWOULDBLOCK，因为它自己管理阻塞。微软建议尽量使用非阻塞模式，通过网络事件的发生而通知应用程序进行相应的处理。但在CSocket类中，为了利用CArchive 处理通讯中的许多问题和简化编程，它的一些成员函数总是具有阻塞性质的，这是因为CArchive类需要同步的操作。

　　在[Win32](http://www.yesky.com/key/671/5671.html)环境下，如果要使用具有阻塞性质的套接字，应该放在独立的工作线程中处理，利用多线程的方法使阻塞不至于干扰其他线程，也不会把CPU时间浪费在阻塞上。多线程的方法既可以使程序员享受CSocket带 来的简化编程的便利，也不会影响用户界面对用户的反应。 

　　CAsyncSocket类编程模型

　　在一个MFC应用程序中，要想轻松处理多个网 络协议，而又不牺牲灵活性时，可以考虑使用CAsyncSocket类，它的效率比CSocket 类要高。CAsyncSocket类针对字节流型套接字的编程模型简述如下：

　　1、构造一个CAsyncSocket对象，并用这个 对象的Create成员函数产生一个Socket句柄。可以按如下两种方法构造： 

CAsyncSocket sock; //使用默认参数产生一个字节流套接字
Sock.Create(); 
　　或在指定端口号产生一个数据报套接字

CAsyncSocket*pSocket=newCAsyncSocket;
intnPort=27;
pSocket->Create(nPort,SOCK-DGRAM);
　　第一种方法在栈上产生一个CAsyncSocket对象， 而第二种方法在堆上产生CAsyncSocket对象；第一种方法中Create（）成员函数用缺省参数产生一个字节流套接字，第二种方法中用Create（）成员函数在指定的端口产生一个数字报套接字。Create（）函数的原型为：

BOOL Create( UINT nSocketPort = 0, int nSocketType = SOCK_STREAM,
LPCTSTR lpszSocketAddress = NULL );
　　该函数的参数有：

　　1）端口，UINT类型。注意：如果是服务方，则使 用一个众所周知的端口供服务方连接；如果是客户方，典型做法是接受默认参数，使 套接字可以自主选择一个可用端口；

　　2）socket 类型，可以是SOCK-STREAM（默认值，字节流）或SOCK-DGRAM（数据报）； 

　　3）socket的地址，例如"ftp.gliet.edu.cn"或"202.193.64.33"。 

　　2、如是客户方程序，用CAsyncSocket∷Connect（）成员函数连接到服务方；如是服务方程序，用CAsyncSocket∷[Listen](http://www.yesky.com/key/2472/2472.html)（）成员函数开始 监听，一旦收到连接请求，则调用CAsyncSocket∷Accept（）成员函数开始接收。注意：CAsyncSocket ∷Accept（）成员函数要用一个新的并且是空的CAsyncSocket对象作为它的参数，这里所说 的"空的"指的是这个新对象还没有调用Create（）成员函数。 

　　3、调用其他的CAsyncSocket类的Receive（）、ReceiveFrom（）、Send（）和SendTo（）等成员函数进行数据通信。 

　　4、通讯结束后，销毁CAsyncSocket对象。如果是在栈上产生的CAsyncSocket对象，则对象超出定义的范围时自动被析构；如果是在堆上产生，也就是用了new这个操作符，则必须使用delete操作符销毁CAsyncSocket 对象。 

**CSocket类编程模型 **

　　使用CSocket对象涉及CArchive和CSocketFile 类对象。以下介绍的针对字节流型套接字的操作步骤中，只有第3步对于客户方和服务方操作是不同的，其他步骤都相同。 

　　1、构造一个CSocket对象。 

　　2、使用这个对象的Create（）成员函数产生一个socket对象。在客户方程序中，除非需要数据报套接字，Create（）函数一般情况下应该使用默认参数。而对于服务方程序，必须在调用Create时指定一个端口。需要注意的是，Carchive类对象不能与数据报（UDP）套接字一起工作，因此对于数据报套接字，CAsyncSocket和CSocket 的使用方法是一样的。 

　　3、如果是客户方套接字，则调用CAsyncSocket ∷Connect（）函数与服务方套接字连接；如果是服务方套接字，则调用CAsyncSocket∷Listen（）开始监听来自客户方的连接请求，收到连接请求后，调用CAsyncSocket∷Accept（）函数接受请求，建立连接。请注意Accept（）成员函数需要一个新的并且为空的CSocket对象作为它的参数，解释同上。 

　　4、产生一个CSocketFile对象，并把它与CSocket 对象关联起来。

　　5、为接收和发送数据各产生一个CArchive 对象，把它们与CSocketFile对象关联起来。切记CArchive是不能和数据报套接字一起工作的。 

　　6、使用CArchive对象的Read（）、Write（）等函数在客户与服务方传送数据。

　　7、通讯完毕后，销毁CArchive、CSocketFile和CSocket对象。 

**二、编程步骤**

　　1、 启动[Visual](http://www.yesky.com/key/577/577.html) C++6.0，生成一个基于对话框架的应用程序，将该程序命名为"Test"；

　　2、 按照图一所示的效果图设置对话框的界面；

　　3、 使用Class Wizard为应用程序的按钮添加鼠标单击消息响应函数；

　　4、 使用Class Wizard在应用程序中定义新类CNewSocket，其基类选择为CSocket；

　　5、 添加代码，编译运行程序。
**三、程序代码**////////////////////////////////////////////////// NewSocket.h : header file
#if !defined(AFX_NEWSOCKET_H__8CE2ED73_1D56_11D3_9928_00A0C98F3E85__INCLUDED_)
#define AFX_NEWSOCKET_H__8CE2ED73_1D56_11D3_9928_00A0C98F3E85__INCLUDED_
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
class CTestDlg;
#include <afxsock.h>

class CNewSocket : public CSocket
{
// Attributes
public:

// Operations
public:
CNewSocket();
virtual ~CNewSocket();

// Overrides
public:
int m_Status;
void GetDlg(CTestDlg *dlg);
CTestDlg *m_dlg;
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CNewSocket)
public:
virtual void OnAccept(int nErrorCode);
virtual void OnReceive(int nErrorCode);
virtual void OnClose(int nErrorCode);
//}}AFX_VIRTUAL
// Generated message map functions
//{{AFX_MSG(CNewSocket)
// NOTE - the ClassWizard will add and remove member functions here.
//}}AFX_MSG
// Implementation
protected:
};
#endif 

//////////////////////////////////////////////////////// NewSocket.cpp : implementation file
#include "stdafx.h"
#include "Test.h"
#include "NewSocket.h"
#include "TestDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNewSocket::CNewSocket()
{}

CNewSocket::~CNewSocket()
{}

#if 0
BEGIN_MESSAGE_MAP(CNewSocket, CSocket)
//{{AFX_MSG_MAP(CNewSocket)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif // 0

void CNewSocket::OnAccept(int nErrorCode) 
{
　if (m_dlg->m_ClientSocket==NULL) m_dlg->OnAccept();
　CSocket::OnAccept(nErrorCode);
}

void CNewSocket::OnReceive(int nErrorCode) 
{
　m_dlg->OnReceive();
　CSocket::OnReceive(nErrorCode);
}

void CNewSocket::GetDlg(CTestDlg *dlg)
{
　m_dlg=dlg;
}

void CNewSocket::OnClose(int nErrorCode) 
{
　m_dlg->OnClose();
　CSocket::OnClose(nErrorCode);
}

///////////////////////////////////////////////////////////////// TestDlg.h : header file
#if !defined(AFX_TESTDLG_H__EDDDE196_1BF1_11D3_BE77_0000B454AEE4__INCLUDED_)
#define AFX_TESTDLG_H__EDDDE196_1BF1_11D3_BE77_0000B454AEE4__INCLUDED_
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "NewSocket.h"

class CTestDlg : public CDialog
{
　// Construction
　public:
　　void SocketReset();
　　void OnClose();
　　void OnReceive();
　　void OnAccept();
　　CSocketFile *m_file;
　　CArchive *m_arOut;
　　CArchive *m_arIn;
　　CNewSocket* m_ServerSocket;
　　CNewSocket* m_ClientSocket;
　　CTestDlg(CWnd* pParent = NULL); // standard constructor
　　// Dialog Data
　　//{{AFX_DATA(CTestDlg)
　　enum { IDD = IDD_TEST_DIALOG };
　　CString m_Info;
　　CString m_Output;
　　CString m_Input;
　　CString m_Connect;
　　CString m_IPAddress;
　　UINT m_Port;
　　int m_Status;
　　//}}AFX_DATA
　　// ClassWizard generated virtual function overrides
　　//{{AFX_VIRTUAL(CTestDlg)
　protected:
　　virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
　　//}}AFX_VIRTUAL
　　// Implementation
　protected:
　　HICON m_hIcon;
　　// Generated message map functions
　　//{{AFX_MSG(CTestDlg)
　　virtual BOOL OnInitDialog();
　　afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
　　afx_msg void OnPaint();
　　afx_msg HCURSOR OnQueryDragIcon();
　　afx_msg void OnConnect();
　　afx_msg void OnDisconnect();
　　afx_msg void OnSend();
　　afx_msg void OnServerradio();
　　afx_msg void OnClientradio();
　　afx_msg void OnSendclear();
　　afx_msg void OnReceiveclear();
　　//}}AFX_MSG
　　DECLARE_MESSAGE_MAP()
　};
#endif 

//////////////////////////////////////////////////////////////// TestDlg.cpp : implementation file
#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include <afxsock.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
　　virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
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

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestDlg::IDD, pParent)
{
　//{{AFX_DATA_INIT(CTestDlg)
　　m_Info = _T("");
　　m_Output = _T("");
　　m_Input = _T("");
　　m_Connect = _T("");
　　m_IPAddress = _T("");
　　m_Port = 0;
　　m_Status = -1;
　//}}AFX_DATA_INIT
　// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
　m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
　CDialog::DoDataExchange(pDX);
　//{{AFX_DATA_MAP(CTestDlg)
　　DDX_Text(pDX, IDC_OUTPUTEDIT, m_Output);
　　DDX_Text(pDX, IDC_INPUTEDIT, m_Input);
　　DDX_Text(pDX, IDC_CONNECTEDIT, m_Connect);
　　DDX_Text(pDX, IDC_IPADDRESS, m_IPAddress);
　　DDV_MaxChars(pDX, m_IPAddress, 15);
　　DDX_Text(pDX, IDC_PORT, m_Port);
　　DDX_Radio(pDX, IDC_SERVERRADIO, m_Status);
　//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
//{{AFX_MSG_MAP(CTestDlg)
　ON_WM_SYSCOMMAND()
　ON_WM_PAINT()
　ON_WM_QUERYDRAGICON()
　ON_BN_CLICKED(IDC_CONNECTBUTTON, OnConnect)
　ON_BN_CLICKED(IDC_DISCONNECTBUTTON, OnDisconnect)
　ON_BN_CLICKED(IDC_SENDBUTTON, OnSend)
　ON_BN_CLICKED(IDC_SERVERRADIO, OnServerradio)
　ON_BN_CLICKED(IDC_CLIENTRADIO, OnClientradio)
　ON_BN_CLICKED(IDC_SENDCLEARBUTTON, OnSendclear)
　ON_BN_CLICKED(IDC_RECEIVECLEARBUTTON, OnReceiveclear)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTestDlg::OnInitDialog()
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
　// Set the icon for this dialog. The framework does this automatically
　// when the application's main window is not a dialog
　SetIcon(m_hIcon, TRUE); // Set big icon
　SetIcon(m_hIcon, FALSE); // Set small icon
　m_Status=-1;
　m_ServerSocket=NULL;
　m_ClientSocket=NULL;
　m_arIn=NULL;
　m_arOut=NULL;
　m_file=NULL;
　m_Connect="";
　m_IPAddress="202.207.243.29";
　m_Port=5000;
　GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
　GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
　UpdateData(FALSE);
　return TRUE; // return TRUE unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
// to draw the icon. For MFC applications using the document/view model,
// this is automatically done for you by the framework.
void CTestDlg::OnPaint() 
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
// the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
　return (HCURSOR) m_hIcon;
}

void CTestDlg::OnConnect() 
{
　CString msg;
　UpdateData(TRUE); 
　if (m_Status==0 ) //server
　{
　　if ( m_ServerSocket!=NULL) 
　　{
　　　m_Connect="Please disconnect!";
　　　UpdateData(FALSE);
　　}
　　else
　　{
　　　m_Connect="Waiting for Client...";
　　　UpdateData(FALSE);
　　　if(!AfxSocketInit())
　　　{ 
　　　　MessageBox("WindowsSocket initial failed!","Send",MB_ICONSTOP); 
　　　　return; 
　　　} 
　　　m_ServerSocket=new CNewSocket;
　　　m_ServerSocket->m_Status=m_Status;
　　　m_ServerSocket->GetDlg(this); 
　　　if(!m_ServerSocket->Create(m_Port)) 
　　　　　MessageBox("SendSocket create failed!", "Send",MB_ICONSTOP); 
　　　else
　　　{ 
　　　　m_ServerSocket->Listen();
　　　} 
　　}
　}
　else 
　{
　　if (m_Status==1) 
　　{
　　　if (m_ClientSocket!=NULL)
　　　{ 
　　　　m_Connect="Please disconnect!";
　　　　UpdateData(FALSE);
　　　}
　　　else
　　　{
　　　　m_Connect="Connect to the Server...";
　　　　UpdateData(FALSE);
　　　　if(!AfxSocketInit())
　　　　{
　　　　　MessageBox("WindowsSocket initial failed!","Receive",MB_ICONSTOP); 
　　　　　return; 
　　　　} 　
　　　　m_ClientSocket=new CNewSocket; 
　　　　m_ClientSocket->GetDlg(this); 
　　　　m_ClientSocket->m_Status=m_Status;
　　　　if(!m_ClientSocket->Create())
　　　　{ 
　　　　　MessageBox("ReceiveSocket create failed!","Receive",MB_ICONSTOP); 
　　　　　return; 
　　　　}
　　　　else
　　　　{ 
　　　　　if (!m_ClientSocket->Connect(m_IPAddress,m_Port))
　　　　　{
　　　　　　CString str=m_Connect;
　　　　　　SocketReset();
　　　　　　m_Connect=str;
　　　　　　m_Connect+="Error!";
　　　　　　UpdateData(FALSE);
　　　　　} 
　　　　　else 
　　　　　{ 
　　　　　　m_Connect+="OK!";
　　　　　　m_file=new CSocketFile(m_ClientSocket); 
　　　　　　m_arIn=new CArchive(m_file, CArchive::load); 
　　　　　　m_arOut=new CArchive(m_file, CArchive::store); 
　　　　　}
　　　　　UpdateData(FALSE);
　　　　} 
　　　}
　　}
　}
　if (m_Status==-1)
　{ 
　　msg="Please choose the status!";
　　AfxMessageBox(msg); 
　}
}

void CTestDlg::OnSend() 
{
　if (m_arOut)
　{
　　if (m_Status==0)
　　{
　　　UpdateData(TRUE);
　　　*m_arOut<<m_Output;
　　　m_arOut->Flush(); 
　　}
　　else 
　　{
　　　UpdateData(TRUE);
　　　*m_arOut<<m_Output;
　　　m_arOut->Flush(); 
　　}
　}
　else AfxMessageBox("Not connected!");
}

void CTestDlg::OnAccept()
{
　m_Connect+="OK!";
　UpdateData(FALSE);
　m_ClientSocket=new CNewSocket;
　m_ClientSocket->GetDlg(this);
　m_ServerSocket->Accept(*m_ClientSocket);
　m_ClientSocket->m_Status=m_ServerSocket->m_Status;
　m_file=new CSocketFile(m_ClientSocket); 
　m_arIn=new CArchive(m_file, CArchive::load); 
　m_arOut=new CArchive(m_file, CArchive::store); 
}

void CTestDlg::OnReceive()
{
　*m_arIn>>m_Input; 
　UpdateData(FALSE); 
}

void CTestDlg::OnDisconnect() 
{
　if (m_arOut!=NULL)
　{
　　SocketReset();
　　m_Connect="Disconnected!";
　　UpdateData(FALSE);
　}
}

void CTestDlg::OnClose()
{
　if (m_ClientSocket->m_Status==0) m_Connect="Client ";
　else m_Connect="Server ";
　m_Connect+="has disconnected!"; 
　UpdateData(FALSE);
}

void CTestDlg::SocketReset()
{
　if (m_arIn!=NULL) 
　{ 
　　delete m_arIn;
　　m_arIn=NULL;
　}
　if (m_arOut!=NULL) 
　{
　　delete m_arOut;
　　m_arOut=NULL;
　} 
　if (m_file!=NULL) 
　{
　　delete m_file;
　　m_file=NULL;
　}
　if (m_ClientSocket!=NULL) 
　{
　　delete m_ClientSocket;
　　m_ClientSocket=NULL;
　}
　if (m_ServerSocket!=NULL)
　{ 
　　delete m_ServerSocket;
　　m_ServerSocket=NULL;
　}
　m_Connect="";
　UpdateData(FALSE);
}

void CTestDlg::OnServerradio() 
{
　UpdateData(TRUE);
　GetDlgItem(IDC_IPADDRESS)->EnableWindow(FALSE);
　GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
　UpdateData(FALSE);
}

void CTestDlg::OnClientradio() 
{ 
　UpdateData(TRUE);
　GetDlgItem(IDC_IPADDRESS)->EnableWindow(TRUE);
　GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
　UpdateData(FALSE);
}

void CTestDlg::OnSendclear() 
{
　m_Output="";
　UpdateData(FALSE);
}

void CTestDlg::OnReceiveclear() 
{
　m_Input="";
　UpdateData(FALSE);
}
**四、小结**

　　本实例介绍了CAsyncSocket、CSocket类，并通过使用CSocket类实现了网络聊天程序。读者朋友还可以通过MFC CArchive 对象进行信息的接发操作，使得网络传输如同使用MFC的文档连载协议(Serialization protocol)，简捷易用。

