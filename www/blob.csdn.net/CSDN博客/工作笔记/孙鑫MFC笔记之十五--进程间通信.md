# 孙鑫MFC笔记之十五--进程间通信 - 工作笔记 - CSDN博客





2012年06月29日 14:50:25[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6665








#### 方法：

#### n剪贴板 

#### n匿名管道 

#### n命名管道 

#### n邮槽 



1.剪贴板

  a.创建个ClipBoard的对话框应用程序，加两EditBox和两个Button发送接收。

  b.具体代码：

    发送端代码：

if(OpenClipboard())//打开剪贴板，0，已经被打开。非0，真确打开，但不拥有所有权

{

  CString str;

  HANDLE hClip;

  char *pBuf;

  EmptyClipboard();//清空剪贴板数据，让当前窗口拥有剪贴板所有权、

  GetDlgItemText(IDC_EDIT_SEND,str);//得到发送编辑框数据

  hClip=GlobalAlloc(GMEM_MOVEABLE,str.GetLength()+1);//分配堆内存

  pBuf=(char*)GlobalLock(hClip);将句柄转换为指针！对全局内存对象加锁，反悔该内存块第一个字节的指针。

  strcpy(pBuf,str);

  GlobalUnlock(hClip);

  SetClipboardData(CF_TEXT,hClip);//以指定剪贴板格式放置数据。参数2为NULL。延迟提交，必须处理WM_RENDERFORMAT消息。

  CloseClipboard();

}

     接收端代码：

if(OpenClipboard())

{

  if(IsClipboardFormatAvailable(CF_TEXT))//检测剪贴板是否包含了指定特定格式的数据。非0，可用。0 不可用

  {

   HANDLE hClip;

   char *pBuf;

   hClip=GetClipboardData(CF_TEXT);//获取指定格式的数据

   pBuf=(char*)GlobalLock(hClip);//获取指针

   GlobalUnlock(hClip);

   SetDlgItemText(IDC_EDIT_RECV,pBuf);

   CloseClipboard();

  }

}



延迟提交技术：

由于剪切板内数据量大，占用内存，浪费资源，因此，先让数据提供进程先提供一个空剪贴板数据块，当需要获取数据时，系统向数据提供进程发送WM_RENDERFORMAT消息，而数据提供进程响应该消息，调用函数将数据放置在剪贴板上。SetClipboardData该函数首次用NULL,之后用需要的句柄。



2.匿名管道：只能在父子进程之间进行通信

   匿名管道是一个未命名，单向管道，通常在一个父进程和子进程之间传递数据。只能实现本地机器上两个进程通信，不能跨网络是实现通信。

  a.先建一个Parent的单文档应用程序，增加“创建管道”“读取数据”“写入数据”三个菜单

  b.增加成员变量HANDLE类型的hRead,hWrite，初始化变量，并在析构函数中释放句柄

  c.响应菜单代码：

void CParentView::OnPipeCreate() 菜单“创建管道”代码

{

// TOD Add your command handler code here

SECURITY_ATTRIBUTES sa;

sa.bInheritHandle=TRUE;

sa.lpSecurityDescriptor=NULL;

sa.nLength=sizeof(SECURITY_ATTRIBUTES);

if(!CreatePipe(&hRead,&hWrite,&sa,0))

{

  MessageBox("创建匿名管道失败！");

  return;

}

STARTUPINFO sui;

PROCESS_INFORMATION pi;

ZeroMemory(&sui,sizeof(STARTUPINFO));将数据清0！

sui.cb=sizeof(STARTUPINFO);

sui.dwFlags=STARTF_USESTDHANDLES;

sui.hStdInput=hRead;

sui.hStdOutput=hWrite;

sui.hStdError=GetStdHandle(STD_ERROR_HANDLE);



if(!CreateProcess("..\\Child\\Debug\\Child.exe",NULL,NULL,NULL,

   TRUE,0,NULL,NULL,&sui,&pi))创建子进程

{

  CloseHandle(hRead);

  CloseHandle(hWrite);关闭句柄，将内核对象的使用计数减少1，这样当操作系统发现内核对象的使用计数为0时，将清除内核对象。

  hRead=NULL;

  hWrite=NULL;

  MessageBox("创建子进程失败！");

  return;

}

else

{

  CloseHandle(pi.hProcess);

  CloseHandle(pi.hThread);

}

}



void CParentView::OnPipeRead() 菜单“读取数据”代码

{

// TOD Add your command handler code here

char buf[100];

DWORD dwRead;

if(!ReadFile(hRead,buf,100,&dwRead,NULL))

{

  MessageBox("读取数据失败！");

  return;

}

MessageBox(buf);

}



void CParentView::OnPipeWrite() 菜单“写入数据”代码

{

// TOD Add your command handler code here

char buf[]="[http://www.sunxin.org](http://www.sunxin.org/)";

DWORD dwWrite;

if(!WriteFile(hWrite,buf,strlen(buf)+1,&dwWrite,NULL))

{

  MessageBox("写入数据失败！");

  return;

}

}

     d.再建一个Child的单文档，在View中增加两个成员hRead和hWrite.在OnInitialUpdate()中得到句柄的值。

void CChildView::OnInitialUpdate() 

{

CView::OnInitialUpdate();



// TOD Add your specialized code here and/or call the base class

hRead=GetStdHandle(STD_INPUT_HANDLE);注意这句代码！

hWrite=GetStdHandle(STD_OUTPUT_HANDLE);

}



     e.加菜单“读取数据”“写入数据”其代码如下：

void CChildView::OnPipeRead() 

{

// TOD Add your command handler code here

char buf[100];

DWORD dwRead;

if(!ReadFile(hRead,buf,100,&dwRead,NULL))

{

  MessageBox("读取数据失败！");

  return;

}

MessageBox(buf);

}



void CChildView::OnPipeWrite()

{

// TOD Add your command handler code here

char buf[]="匿名管道测试程序";

DWORD dwWrite;

if(!WriteFile(hWrite,buf,strlen(buf)+1,&dwWrite,NULL))

{

  MessageBox("写入数据失败！");

  return;

}

}



3.命名管道：

命名管道是通过网络来完成进程间的通信，它屏蔽了底层的网络协议细节。我们在不了解网络协议的情况下，也可以利用命名管道来实现进程间的通信。

命名管道充分利用了Windows NT和Windows 2000内建的安全机制。

将命名管道作为一种网络编程方案时，它实际上建立了一个客户机／服务器通信体系，并在其中可靠地传输数据。

命名管道是围绕Windows文件系统设计的一种机制，采用“命名管道文件系统(Named Pipe File System，NPFS)”接口，因此，客户机和服务器可利用标准的Win32文件系统函数(例如：ReadFile和WriteFile)来进行数据的收发。



名管道服务器和客户机的区别在于：服务器是唯一一个有权创建命名管道的进程，也只有它才能接受管道客户机的连接请求。而客户机只能同一个现成的命名管道服务器建立连接。

命名管道服务器只能在Windows NT或Windows 2000上创建，所以，我们无法在两台Windows 95或Windows 98计算机之间利用管道进行通信。不过，客户机可以是Windows 95或Windows 98计算机，与Windows NT或Windows 2000计算机进行连接通信。

命名管道提供了两种基本通信模式：字节模式和消息模式。在字节模式中，数据以一个连续的字节流的形式，在客户机和服务器之间流动。而在消息模式中，客户机和服务器则通过一系列不连续的数据单位，进行数据的收发，每次在管道上发出了一条消息后，它必须作为一条完整的消息读入。


还可以跨网络通信，服务器只能在win2000和NT下运行！而客户端可以在95下运行。关键函数CreateNamedPipe

  a.先建一个NamedPipeSRV单文档应用程序，加菜单“创建管道”“读取数据”“写入数据”

  b.在View中增加Handle变量hPipe，注意在析构函数中释放它！

  c.响应菜单，创建命名管道

void CNamedPipeSrvView::OnPipeCreate() 

{

// TOD Add your command handler code here

hPipe=CreateNamedPipe("\\\\.\\pipe\\MyPipe",

  PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,

  0,1,1024,1024,0,NULL);

if(INVALID_HANDLE_VALUE==hPipe)

{

  MessageBox("创建命名管道失败！");

  hPipe=NULL;

  return;

}

HANDLE hEvent;

hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

if(!hEvent)

{

  MessageBox("创建事件对象失败！");

  CloseHandle(hPipe);

  hPipe=NULL;

  return;

}

OVERLAPPED ovlap;

ZeroMemory(&ovlap,sizeof(OVERLAPPED));

ovlap.hEvent=hEvent;

if(!ConnectNamedPipe(hPipe,&ovlap))

{

  if(ERROR_IO_PENDING!=GetLastError())

  {

   MessageBox("等待客户端连接失败！");

   CloseHandle(hPipe);

   CloseHandle(hEvent);

   hPipe=NULL;

   return;

  }

}

if(WAIT_FAILED==WaitForSingleObject(hEvent,INFINITE))

{

  MessageBox("等待对象失败！");

  CloseHandle(hPipe);

  CloseHandle(hEvent);

  hPipe=NULL;

  return;

}

CloseHandle(hEvent);

}



void CNamedPipeSrvView::OnPipeRead()

{

// TOD Add your command handler code here

char buf[100];

DWORD dwRead;

if(!ReadFile(hPipe,buf,100,&dwRead,NULL))

{

  MessageBox("读取数据失败！");

  return;

}

MessageBox(buf);

}



void CNamedPipeSrvView::OnPipeWrite()

{

// TOD Add your command handler code here

char buf[]="[http://www.sunxin.org](http://www.sunxin.org/)";

DWORD dwWrite;

if(!WriteFile(hPipe,buf,strlen(buf)+1,&dwWrite,NULL))

{

  MessageBox("写入数据失败！");

  return;

}

}



      d.再建一个NamedPipeCLT单文档工程，加菜单“连接管道”“读取数据”“写入数据”，当然别忘记成员变量hPipe的定义和初始化

      e.响应菜单代码

void CNamedPipeCltView::OnPipeConnect() 连接管道

{

// TOD Add your command handler code here

if(!WaitNamedPipe("\\\\.\\pipe\\MyPipe",NMPWAIT_WAIT_FOREVER))

{

  MessageBox("当前没有可利用的命名管道实例！");

  return;

}

hPipe=CreateFile("\\\\.\\pipe\\MyPipe",GENERIC_READ | GENERIC_WRITE,

  0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

if(INVALID_HANDLE_VALUE==hPipe)

{

  MessageBox("打开命名管道失败！");

  hPipe=NULL;

  return;

}

}



void CNamedPipeCltView::OnPipeRead() 读取数据

{

// TOD Add your command handler code here

char buf[100];

DWORD dwRead;

if(!ReadFile(hPipe,buf,100,&dwRead,NULL))

{

  MessageBox("读取数据失败！");

  return;

}

MessageBox(buf);

}



void CNamedPipeCltView::OnPipeWrite() 写入数据

{

// TOD Add your command handler code here

char buf[]="命名管道测试程序";

DWORD dwWrite;

if(!WriteFile(hPipe,buf,strlen(buf)+1,&dwWrite,NULL))

{

  MessageBox("写入数据失败！");

  return;

}

}



4.邮槽，使用时应将消息长度限制在424字节以下，关键函数CreateMailSlot()

邮槽是基于广播通信体系设计出来的，它采用无连接的不可靠的数据传输。

邮槽是一种单向通信机制，创建邮槽的服务器进程读取数据，打开邮槽的客户机进程写入数据。

为保证邮槽在各种Windows平台下都能够正常工作，我们传输消息的时候，应将消息的长度限制在424字节以下。

  a.先建一个MailSlotSRV工程，加菜单“接收数据”

  b.消息响应代码：

void CMailslotSrvView::OnMailslotRecv() 菜单“接收数据”的代码

{

// TOD Add your command handler code here

HANDLE hMailslot;

hMailslot=CreateMailslot("\\\\.\\mailslot\\MyMailslot",0,

  MAILSLOT_WAIT_FOREVER,NULL);

if(INVALID_HANDLE_VALUE==hMailslot)

{

  MessageBox("创建油槽失败！");

  return;

}

char buf[100];

DWORD dwRead;

if(!ReadFile(hMailslot,buf,100,&dwRead,NULL))

{

  MessageBox("读取数据失败！");

  CloseHandle(hMailslot);

  return;

}

MessageBox(buf);

CloseHandle(hMailslot);

}

    c.加工程MailSlotCLT,加菜单“发送数据”

    d.加消息响应，添加代码，客户端也比较简单。

void CMailslotCltView::OnMailslotSend() 菜单“发送数据”的代码

{

// TOD Add your command handler code here

HANDLE hMailslot;

hMailslot=CreateFile("\\\\.\\mailslot\\MyMailslot",GENERIC_WRITE,

  FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

if(INVALID_HANDLE_VALUE==hMailslot)

{

  MessageBox("打开油槽失败！");

  return;

}

char buf[]="[http://www.sunxin.org](http://www.sunxin.org/)";

DWORD dwWrite;

if(!WriteFile(hMailslot,buf,strlen(buf)+1,&dwWrite,NULL))

{

  MessageBox("写入数据失败！");

  CloseHandle(hMailslot);

  return;

}

CloseHandle(hMailslot);

}



5.以上4种方法各有优缺点：剪贴板比较简单。邮槽是基于广播的，可以一对多发送。但只能一个发送，一个接收，要想同时发送接收，须写两次代码。

命名管道和邮槽可以进行网络通信。








