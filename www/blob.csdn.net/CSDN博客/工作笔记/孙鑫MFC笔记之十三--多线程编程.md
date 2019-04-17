# 孙鑫MFC笔记之十三--多线程编程 - 工作笔记 - CSDN博客





2012年05月31日 17:26:41[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6020标签：[多线程																[编程																[mfc																[数据结构																[winapi																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[基础编程](https://blog.csdn.net/App_12062011/article/category/952065)





程序，进程，线程的区别

程序 是计算机指令的集合，它以文件的形式存储在磁盘上。

进程 通常被定义一个正在执行的程序实例，是一个程序在其自身的地址空间中的一次执行活动。

          我们通常的exe程序是以文件的形式存放在磁盘上的，当我们运行exe时，就生成了一个进程。一个程序可以对应多个进程。

          进程是资源申请，调度和独立运行的单位，因此它使用系统的运行资源。而程序不能申请资源，调度资源，也不能独立运行。因此它不占用系统运行资源。



进程的组成

         进程由两部分组成


    1、操作系统用来管理进程的内核对象。内核对象也是系统用来存放关于进程的统计信息的地方。 

        2、地址空间。它包含所有可执行模块或DLL模块的代码和数据。它还包含动态内存分配的空间。如线程堆栈和堆分配空间。 





进程是不活泼的。进程从来不执行任何东西，它只是线程的容器。若要使进程完成某项操作，它必须拥有一个在它的环境中运行的线程，此线程负责执行包含在进程的地址空间中的代码。

单个进程可能包含若干个线程，这些线程都“同时” 执行进程地址空间中的代码。 

每个进程至少拥有一个线程，来执行进程的地址空间中的代码。当创建一个进程时，操作系统会自动创建这个进程的第一个线程，称为主线程。此后，该线程可以创建其他的线程。

       地址空间：

      系统赋予每个进程独立的虚拟地址空间。对于32位进程来说，这个地址空间是4GB。 

      每个进程有它自己的私有地址空间。进程A可能有一个存放在它的地址空间中的数据结构，地址是0x12345678，而进程B则有一个完全不同的数据结构存放在它的地址空间中，地址是0x12345678。当进程A中运行的线程访问地址为0x12345678的内存时，这些线程访问的是进程A的数据结构。当进程B中运行的线程访问地址为0x12345678的内存时，这些线程访问的是进程B的数据结构。进程A中运行的线程不能访问进程B的地址空间中的数据结构，反之亦然。


4GB是虚拟的地址空间，只是内存地址的一个范围。在你能成功地访问数据而不会出现非法访问之前，必须赋予物理存储器，或者将物理存储器映射到各个部分的地址空间。

4GB虚拟地址空间中，2GB是内核方式分区，供内核代码、设备驱动程序、设备I/O高速缓冲、非页面内存池的分配和进程页面表等使用，而用户方式分区使用的地址空间约为2GB，这个分区是进程的私有地址空间所在的地方。一个进程不能读取、写入、或者以任何方式访问驻留在该分区中的另一个进程的数据。对于所有应用程序来说，该分区是维护进程的大部分数据的地方。



线程：

        线程由两个部分组成： 



*       1、线程的内核对象，操作系统用它来对线程实施管理。内核对象也是系统用来存放线程统计信息的地方。 *

*       2、线程堆栈，它用于维护线程在执行代码时需要的所有参数和局部变量。*

当创建线程时，系统创建一个线程内核对象。该线程内核对象不是线程本身，而是操作系统用来管理线程的较小的数据结构。可以将线程内核对象视为由关于线程的统计信息组成的一个小型数据结构。

线程总是在某个进程环境中创建。系统从进程的地址空间中分配内存，供线程的堆栈使用。新线程运行的进程环境与创建线程的环境相同。因此，新线程可以访问进程的内核对象的所有句柄、进程中的所有内存和在这个相同的进程中的所有其他线程的堆栈。这使得单个进程中的多个线程确实能够非常容易地互相通信。

线程只有一个内核对象和一个堆栈，保留的记录很少，因此所需要的内存也很少。

因为线程需要的开销比进程少，因此在编程中经常采用多线程来解决编程问题，而尽量避免创建新的进程。



操作系统为每一个运行线程安排一定的CPU时间 —— 时间片。系统通过一种循环的方式为线程提供时间片，线程在自己的时间内运行，因时间片相当短，因此，给用户的感觉，就好像线程是同时运行的一样。

如果计算机拥有多个CPU，线程就能真正意义上同时运行了。



   线程创建函数：

   1.1CreateThread

         WINBASEAPI

HANDLE

WINAPI

CreateThread(

    LPSECURITY_ATTRIBUTES lpThreadAttributes, //NULL 缺省安全性

    DWORD dwStackSize, //初始栈大小，系统四舍五入为最近的页面。X86页面大小4KB，该值为0，或者小于默认的提交大小，则默认使用与调用该函数的线程相同的栈空间大小

    LPTHREAD_START_ROUTINE lpStartAddress,//新线程的起始地址。因此必须定义一个新线程的入口函数。入口函数名字任意，但申明形式固定。

    LPVOID lpParameter,//传递参数，类似main的命令行参数

    DWORD dwCreationFlags,//标记，0,则创建后立即运行。非0，创建后暂停。

    LPDWORD lpThreadId //线程ID，window95或98下，不能为NULL

    );

返回互斥对象句柄后，根据返回值有效，调用getlasterr函数，返回error_already_exits.表示命名的互斥对象已经被创建，已经有该程序的实例。否则，创建新的。这样。利用实现只能创建一个应用程序的实例功能。



DWORD WINAPI Fun1Proc(

  LPVOID lpParameter   // thread data，新线程入口函数，名称任意，形式固定

);



多线程同时访问一个全局变量时，由于多次频繁的切换，导致出错。为了避免这个错误，利用线程同步处理。保证线程访问一个共享资源时，其他线程不能访问该资源。



互斥对象：

      为了避免上面的错误，利用互斥对象。

      互斥对象(mutex)属于内核对象，它能够确保线程拥有对单个资源的互斥访问权。 

      互斥对象包含一个使用数量，一个线程ID和一个计数器。 

      ID用于标识系统中的哪个线程当前拥有互斥对象，计数器用于指明该线程拥有互斥对象的次数。 

  CreateMutex

CreateMutex(

    LPSECURITY_ATTRIBUTES lpMutexAttributes,//NULL，缺省安全性

    BOOL bInitialOwner,//true，线程获得互斥对象的所有权。false，调用线程不获得互斥对象的所有权。

    LPCSTR lpName //NULL。匿名互斥对象。互斥对象的名字。

    );



对互斥对象来说，谁拥有，谁释放。释放时，先判断线程ID是否相等，相等才释放、

线程终止，即使不释放，系统自动释放。






#include <windows.h>//包含API

#include <iostream.h>//C++输入输出流

DWORD WINAPI Fun1Proc(

  LPVOID lpParameter   // thread data，新线程入口函数，名称任意，形式固定

);

DWORD WINAPI Fun2Proc(

  LPVOID lpParameter   // thread data 新线程入口函数

);

int index=0;

int tickets=100;

HANDLE hMutex;//定义互斥对象句柄

void main()

{

 HANDLE hThread1;

 HANDLE hThread2;

 hThread1=CreateThread(NULL,0,Fun1Proc,NULL,0,NULL);

 hThread2=CreateThread(NULL,0,Fun2Proc,NULL,0,NULL);

 CloseHandle(hThread1);//关闭句柄。表示在主线程中，对新线程的引用不感兴趣。如果不关闭，线程内核对象的引用始终保留，不会为0，

 CloseHandle(hThread2);

 /*while(index++<1000)

  cout<<"main thread is running"<<endl;*/

// hMutex=CreateMutex(NULL,FALSE,NULL);//当前无线程拥有该互斥对象，系统将互斥对象设为有信号状态，即已通知状态

// hMutex=CreateMutex(NULL,TRUE,NULL);//当前主线程拥有该互斥对象，系统将互斥对象设为无信号状态，即未通知状态，计数器变为1

 hMutex=CreateMutex(NULL,TRUE,"tickets");

 if(hMutex)

 {

  if(ERROR_ALREADY_EXISTS==GetLastError())

  {

   cout<<"only instance can run!"<<endl;

   return;

  }

 }

 WaitForSingleObject(hMutex,INFINITE);//线程ID相同，请求到互斥对象，计数器为2.

 ReleaseMutex(hMutex);//因此要释放2次

 ReleaseMutex(hMutex);

 Sleep(4000);

// Sleep(10);

}

DWORD WINAPI Fun1Proc(

  LPVOID lpParameter   // thread data

)

{

 /*while(index++<1000)

  cout<<"thread1 is running"<<endl;*/


//  while(TRUE)

//  {

//   //ReleaseMutex(hMutex);

// 

//   //该线程获得互斥对象，系统将互斥对象的线程ID，设为该线程ID

//   WaitForSingleObject(hMutex,INFINITE);//线程必须主动请求共享对象的使用权，才有可能获得该对象的所有权。

//   //该函数的参数：1, 所请求的对象的句柄。2 指定等待的时间间隔

//   //一旦互斥对象处于有信号状态，则函数返回，否则，一直等待，则线程暂停。

//   //一旦等待的时间超过。函数也会返回。INFINITE，永远等待、

//   //函数调用成功，则返回值将表明引起该函数返回的原因。

//   //此时，互斥对象设为未通知状态。

//   if(tickets>0)

//   {

//    Sleep(1);//人为的同步处理。为了看到多线程访问共享资源产生的错误。

//    cout<<"thread1 sell ticket : "<<tickets--<<endl;

//   }

//   else

//    break;

//   ReleaseMutex(hMutex);//线程使用共享资源结束时，应该释放对象的所有权。也就是让对象处于已通知状态。

//  }

 WaitForSingleObject(hMutex,INFINITE);

 cout<<"thread1 is running"<<endl;

 return 0;

}

DWORD WINAPI Fun2Proc(

  LPVOID lpParameter   // thread data

)

{

//  

//  while(TRUE)

//  {

//   //ReleaseMutex(hMutex);

//   WaitForSingleObject(hMutex,INFINITE);

//   if(tickets>0)

//   {

//    Sleep(1);

//    cout<<"thread2 sell ticket : "<<tickets--<<endl;

//   }

//   else

//    break;

//   ReleaseMutex(hMutex);

//  }

 WaitForSingleObject(hMutex,INFINITE);

 cout<<"thread2 is running"<<endl;

 return 0;

}




在多线程编程中，线程不能访问类成员函数，因为线程中，没有创建类的对象，因此，将所要访问的函数写为全局函数，或者，类静态成员函数。




3.多线程聊天程序

  1.加载套接字库在InitInstance()中，调用AfxSocketInit(),此时可以不加载库文件，但要加入Afxsock.h"头文件

  2.在CChatDlg中创建成员变量m_socket,然后增加一个成员函数，IniSocket(),在其中完成m_socket的初始化和绑定。在OnInitDialog中调用InitSocket完成初始化工作。

  3.定义一个结构体，包含两个参数，sock和hwnd，在OnInitDialog()中初始化这个结构体的对象。

  4.创建一个线程，CreateThread(),须将线程函数RecvProc定义为静态的或者全局函数。

    ::PostMessage()完成将收到的数据发送给对话框。用自定义的消息，自定义的消息如何写？以前说过，参考下面的代码。注意要将EDitBox的MultiLine属性选上。 


    在ChatDlg.h中#define WM_RECVDATA  WM_USER+1

afx_msg void OnRecvData(WPARAM wParam,LPARAM lParam);

    在ChatDlg.cpp中

ON_MESSAGE(WM_RECVDATA,OnRecvData)

然后实现这个函数

void CChatDlg::OnRecvData(WPARAM wParam,LPARAM lParam)

{

CString str=(char*)lParam;

CString strTemp;

GetDlgItemText(IDC_EDIT_RECV,strTemp);

str+="\r\n";

str+=strTemp;

SetDlgItemText(IDC_EDIT_RECV,str);

}

     最后在DWORD WINAPI CChatDlg::RecvProc(LPVOID lpParameter)

中调用 ::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)tempBuf);

//不能用SendMessage()

    4.对发送按纽的响应代码：

void CChatDlg::OnBtnSend() 

{

// TOD Add your control notification handler code here

DWORD dwIP;

((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIP);



SOCKADDR_IN addrTo;

addrTo.sin_family=AF_INET;

addrTo.sin_port=htons(6000);

addrTo.sin_addr.S_un.S_addr=htonl(dwIP);



CString strSend;

GetDlgItemText(IDC_EDIT_SEND,strSend);

sendto(m_socket,strSend,strSend.GetLength()+1,0,

  (SOCKADDR*)&addrTo,sizeof(SOCKADDR));

SetDlgItemText(IDC_EDIT_SEND,"");

}](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)




