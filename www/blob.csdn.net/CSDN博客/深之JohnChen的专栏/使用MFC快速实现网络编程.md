# 使用MFC快速实现网络编程 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:17:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2833


使用MFC快速实现网络编程

随着计算机网络化的深入，计算机网络编程在程序设计的过程中变得日益
重要。由于C++语言对底层操作的优越性，许多文章都曾经介绍过用VC++
进行Socket编程的方法。但由于都是直接利用动态连接库wsock32.dll进
行操作，实现比较繁琐。其实，VC++的MFC类库中提供了CAsyncSocket这
样一个套接字类，用他来实现Socket编程，是非常方便的。 

本文将用一个Echo例程来介绍CAsyncSocket类的用法。 

一． 客户端 

1． 创建一个Dialog Based项目：CSockClient。 

2． 设计对话框 

去掉Ok和Cancle两个按钮，增加ID_Connect（连接）、ID_Send（发送）、
ID_Exit（关闭）按钮，增加ListBox控件IDC_LISTMSG和Edit控件IDC_EDITMSG，
并按下表在ClassWizard中为CCSockClientDlg类添加变量。 

Control ID Type Member

IDC_EDITMSG CEdit m_MSG
IDC_LISTMSG ClistBox m_MSGS

3． CAsyncSocket类用DoCallBack函数处理MFC消息，当一个网络事件发
生时，DoCallBack函数按网络事件类型：FD_READ、FD_WRITE、FD_ACCEPT、
FD_CONNECT分别调用OnReceive、OnSend、OnAccept、OnConnect函数。
由于MFC把这些事件处理函数定义为虚函数，所以要生成一个新的C++类，
以重载这些函数，做法如下： 

以Public方式继承CAsyncSocket类，生成新类MySock； 

为MySock类添加虚函数OnReceive、OnConnect、OnSend 

4． 在MySock.ccp中添加以下代码 

#include "CSockClient.h"
#include "CSockClientDlg.h"

5． 在MySock.h中添加以下代码 

public:
BOOL m_bConnected; //是否连接
UINT m_nLength; //消息长度
char m_szBuffer[4096]; //消息缓冲区

6． 在MySock.ccp中重载各函数 

MySock::MySock()
{
m_nLength=0;
memset(m_szBuffer,0,sizeof(m_szBuffer));
m_bConnected=FALSE;
}

MySock::~MySock()
{
//关闭套接字
if(m_hSocket!=INVALID_SOCKET)
Close();
}

void MySock::OnReceive(int nErrorCode) 
{
m_nLength=Receive(m_szBuffer,sizeof(m_szBuffer),0);
//下面两行代码用来获取对话框指针
CCSockClientApp* pApp=(CCSockClientApp*)AfxGetApp();
CCSockClientDlg* pDlg=(CCSockClientDlg*)pApp- >m_pMainWnd;
pDlg- >m_MSGS.InsertString(0,m_szBuffer);
memset(m_szBuffer,0,sizeof(m_szBuffer));
CAsyncSocket::OnReceive(nErrorCode);
}

void MySock::OnSend(int nErrorCode) 
{
Send(m_szBuffer,m_nLength,0);
m_nLength=0;
memset(m_szBuffer,0,sizeof(m_szBuffer));
//继续提请一个“读”的网络事件，接收Server消息
AsyncSelect(FD_READ);
CAsyncSocket::OnSend(nErrorCode);
}

void MySock::OnConnect(int nErrorCode) 
{
if (nErrorCode==0)
{
m_bConnected=TRUE;
CCSockClientApp* pApp=(CCSockClientApp*)AfxGetApp();
CCSockClientDlg* pDlg=(CCSockClientDlg*)pApp- >m_pMainWnd;
memcpy(m_szBuffer,"Connected to ",13);
strncat(m_szBuffer,pDlg- >m_szServerAdr,
sizeof(pDlg- >m_szServerAdr));
pDlg- >m_MSGS.InsertString(0,m_szBuffer);
AsyncSelect(FD_READ); ////提请一个“读”的网络事件，准备接收
}
CAsyncSocket::OnConnect(nErrorCode);
}

7． 新建对话框IDD_Addr，用来输入IP地址和Port；生成新类CAddrDlg。
增加两个Edit控件：IDC_Addr、IDC_Port按下表在ClassWizard中为CAddrDlg
类添加变量。 

Control ID Type Member

IDC_Addr CString m_Addr
IDC_Port Int m_Port

8． 在CSockClientDlg.ccp中添加代码 

#include "AddrDlg.h"
protected:
int TryCount;
MySock m_clientSocket;
UINT m_szPort;
public:
char m_szServerAdr[256]; 

9． 双击IDD_CSOCKCLIENT_DIALOG对话框中的“连接”按钮，添加以下代码 

void CCSockClientDlg::OnConnect() 
{
m_clientSocket.ShutDown(2);
m_clientSocket.m_hSocket=INVALID_SOCKET;
m_clientSocket.m_bConnected=FALSE;
CAddrDlg m_Dlg;
//默认端口1088
m_Dlg.m_Port=1088;
if (m_Dlg.DoModal()==IDOK && !m_Dlg.m_Addr.IsEmpty())
{
memcpy(m_szServerAdr,m_Dlg.m_Addr,sizeof(m_szServerAdr));
m_szPort=m_Dlg.m_Port;
//建立计时器，每1秒尝试连接一次，直到连上或TryCount>10
SetTimer(1,1000,NULL);
TryCount=0;
}
}

10． 添加Windows消息WM_TIMER响应函数OnTimer 

void CCSockClientDlg::OnTimer(UINT nIDEvent) 
{
if (m_clientSocket.m_hSocket==INVALID_SOCKET)
{
BOOL bFlag=m_clientSocket.Create(0,SOCK_STREAM,FD_CONNECT);
if(!bFlag)
{
AfxMessageBox("Socket Error!");
m_clientSocket.Close();
PostQuitMessage(0);
return;
}
}
m_clientSocket.Connect(m_szServerAdr,m_szPort);
TryCount++;
if (TryCount >=10 || m_clientSocket.m_bConnected)
{ 
KillTimer(1);
if (TryCount >=10)
AfxMessageBox("Connect Failed!");
return;
}
CDialog::OnTimer(nIDEvent);
}

11． 双击IDD_CSOCKCLIENT_DIALOG对话框中的“发送”按钮，添加以下代码 

void CCSockClientDlg::OnSend() 
{
if (m_clientSocket.m_bConnected)
{
m_clientSocket.m_nLength=m_MSG.GetWindowText
(m_clientSocket.m_szBuffer, sizeof(m_clientSocket.m_szBuffer));
m_clientSocket.AsyncSelect(FD_WRITE);
m_MSG.SetWindowText("");
}
}

12． 双击IDD_CSOCKCLIENT_DIALOG对话框中的“关闭”按钮，添加以下代码 

void CCSockClientDlg::OnExit() 
{
//关闭Socket
m_clientSocket.ShutDown(2);
//关闭对话框
EndDialog(0); 
}

12．运行此项目，连接时输入主机名或IP均可，CAsyncSocket类会自动处理。

二． 服务端

Server端的编程与Client端的类似，下面主要介绍他的Listen及Accept函数

1． 建立一个CNewSocket类，重载CAsyncSocket类的OnReceive、OnSend函数，
如何进行信息的显示和发送可以参考Client程序。本例中采用将收到信息原
封不动发回的方法来实现Echo功能，代码如下
CNewSocket：：OnReceive（int nErrorCOde）
{
m_nLength=Receive（m_szBuffer，sizeof（m_szBuffer），0）；
// 直接转发消息
AsyncSelect（FD_WRITE）；
}

CNewSocket：：OnSend（int nErrorCode）
{
Send（m_szBuffer，m_nLength，0）；
}

2． 建立一个CMyServerSocket类，重载CAsyncSocket类的OnAccept函数代码如下

在MyServerSocket.h中声明变量
public:：
CNewSocket* m_pSocket；

void CMyServerSocket：：OnAccept（int nErrorCode）
{
//侦听到连接请求，调用Accept函数
CNewSocket* pSocket = new CNewSocket（）；
if （Accept（*pSocket））
{
pSocket- >AsyncSelect（FD_READ）；
m_pSocket=pSocket；
}
else
delete pSocket；
}

3． 为对话框添加一个“侦听”按钮，添加如下代码

在CsockServerDlg.ccp中声明变量
public：
CMyServerSocket m_srvrSocket;
void CCSockServerDlg：：OnListen（）
{
if （m_srvrSocket.m_hSocket==INVALID_SOCKET）
{
BOOL bFlag=m_srvrSocket.Create
(UserPort，SOCK_STREAM，FD_ACCEPT)；
if （！bFlag）
{
AfxMessageBox（“Socket Error！”）；
M_srvrSocket.Close（）；
PostQuitMessage（0）；
Return；
}
}
//“侦听”成功，等待连接请求
if （！m_srvrSocket。Listen（1））
{
int nErrorCode = m_srvrSocket.GetLastError（）；
if （nError！=WSAEWOULDBLOCK）
{
AfxMessageBox（“Socket Error！”）；
M_srvrSocket.Close（）；
PostQuitMessage（0）；
Return；
}
}
}

4． 目前程序只能实现Echo功能，将信息原封不动的转发，若能将Accept中
由CNewSocket* pSocket = new CNewSocket（）；得到的Socket指针存入一
个CList或一个数组中，便像Client端那样，对所有的连接进行读写控制。

三． 总结

CAsyncSocket类为我们使用Socket提供了极大方便。建立Socket的WSAStartup
过程和bind过程被简化成为Create过程，IP地址类型转换、主机名和IP地址
转换的过程中许多复杂的变量类型都被简化成字符串和整数操作，特别是
CAsyncSocket类的异步特点，完全可以替代繁琐的线程操作。MFC提供了大
量的类库，我们若能灵活的使用他们，便会大大提高编程的效率。 

