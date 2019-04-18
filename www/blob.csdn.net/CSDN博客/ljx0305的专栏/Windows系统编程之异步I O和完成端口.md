# Windows系统编程之异步I/O和完成端口 - ljx0305的专栏 - CSDN博客
2009年04月06日 09:54:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1282标签：[windows																[编程																[null																[io																[file																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[网络编程																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403915)
标 题: Windows系统编程之异步I/O和完成端口
作 者: 北极星2003
时 间: 2006-07-02 18:46 
链 接: [http://bbs.pediy.com/showthread.php?threadid=28342](http://bbs.pediy.com/showthread.php?threadid=28342)
详细信息:
> 
Windows系统编程之异步I/O和完成端口
【作者】北极星2003
【来源】看雪技术论坛（bbs.pediy.com） 
【时间】2006年7月1日
一、  同步I/O和异步I/O
在介绍这部分内容之前先来认识下“异步I/O”。
  说起异步IO，很容易联想到同步I/O，对于同一个I/O对象句柄在同一时刻只允许一个I/O操作，其原理如下图所示：
![1_gif](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090406/1_gif_796633746084671728750.gif)
  显然，当内核真正处理I/O的时间段（T2~T4），用户线程是处于等待状态的，如果这个时间段比较段的话，没有什么影响；倘若这个时间段很长的话，线程就会长时间处于挂起状态。事实上，该线程完全可以利用这段时间用处理其他事务。
  异步I/O恰好可以解决同步I/O中的问题，而且支持对同一个I/O对象的并行处理，其原理如下图所示：
![2_gif](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090406/2_gif_801633746084672197500.gif)
  异步I/O在I/O请求完成时，可以使用让I/O对象或者事件对象受信来通知用户线程，而用户线程中可以使用GetOverlappedResult来查看I/O的执行情况。
由于异步I/O在进行I/O请求后会立即返回，这样就会产生一个问题：“程序是如何取得I/O处理的结果的？”。
  有多种方法可以实现异步I/O，其不同资料上的分类一般都不尽相同，但原理上都类似，这里我把实现异步I/O的方法分为3类，本文就针对这3类方法进行详细的讨论。
（1）重叠I/O
（2）异步过程调用（APC），扩展I/O
（3）使用完成端口（IOCP）
二、使用重叠I/O实现异步I/O
  同一个线程可以对多个I/O对象进行I/O操作，不同的线程也可以对同一个I/O对象进行操作，在我的理解中，重叠的命名就是这么来的。
  在使用重叠I/O时，线程需要创建OVERLAPPED结构以供I/O处理。该结构中最重要的成员是hEvent，它是作为一个同步对象而存在，如果hEvent为NULL，那么此时的同步对象即为文件句柄、管道句柄等I/O操作对象。当I/O完成后，会使这里的同步对象受信，从而通知用户线程。
  由于在进行I/O请求后会立即返回，但有时用户线程需要知道I/O当前的执行情况，此时就可以使用GetOverlappedResult。如果该函数的bWait参数为true,那么改函数就会阻塞线程直到目标I/O处理完成为止；如果bWait为false，那么就会立即返回，如果此时的I/O尚未完，调用GetLastError就会返回ERROR_IO_INCOMPLETE。
代码示例一：
> 代码:
DWORD   nReadByte ;
BYTE   bBuf[BUF_SIZE] ;
OVERLAPPED ov = { 0, 0, 0, 0, NULL } ;  // hEvent = NULL ;
HANDLE hFile = CreateFile ( ……, FILE_FLAG_OVERLAPPED, …… ) ;
ReadFile ( hFile, bBuf, sizeof(bBuf), &nReadByte, &ov ) ;
// 由于此时hEvent=NULL，所以同步对象为hFile,下面两句的效果一样
WaitForSingleObject ( hFile, INFINITE ) ;
//GetOverlappedResult ( hFile, &ov, &nRead, TRUE ) ;
这段代码在调用ReadFile后会立即返回，但在随后的WaitForSingleObject或者GetOverlappedResult中阻塞，利用同步对象hFile进行同步。
  这段代码在这里可以实现正常的异步I/O，但存在一个问题，倘若现在需要对hFile句柄进行多个I/O操作，就会出现问题。见下面这段代码。
代码示例二：
> 代码:
DWORD   nReadByte ;
BYTE   bBuf1[BUF_SIZE],bBuf2[BUF_SIZE],bBuf3[BUF_SIZE] ;
OVERLAPPED ov1 = { 0, 0, 0, 0, NULL } ;  
OVERLAPPED ov2 = { 0, 0, 0, 0, NULL } ;  
OVERLAPPED ov3 = { 0, 0, 0, 0, NULL } ;  
HANDLE hFile = CreateFile ( ……, FILE_FLAG_OVERLAPPED, …… ) ;
ReadFile ( hFile, bBuf1, sizeof(bBuf1), &nReadByte, &ov1 ) ;
ReadFile ( hFile, bBuf2, sizeof(bBuf2), &nReadByte, &ov2 ) ;
ReadFile ( hFile, bBuf3, sizeof(bBuf3), &nReadByte, &ov3 ) ;
//假设三个I/O处理的时间比较长，到这里还没有结束
GetOverlappedResult ( hFile, &ov1, &nRead, TRUE ) ;
  这里对于hFile有三个重叠的I/O操作，但他们的同步对象却都为hFile。使用GetOverlappedResult进行等待操作，这里看似在等待第一个I/O处理的完成，其实只要有任何一个I/O处理完成，该函数就会返回，相当于忽略了其他两个I/O操作的结果。
  其实，这里有一个很重要的原则：对于一个重叠句柄上有多于一个I/O操作的时候，应该使用事件对象而不是文件句柄来实现同步。正确的实现见示例三。
代码示例三：
> 代码:
DWORD   nReadByte ;
BYTE   bBuf1[BUF_SIZE],bBuf2[BUF_SIZE],bBuf3[BUF_SIZE] ;
HANDLE  hEvent1 = CreateEvent ( NULL, FALSE, FALSE, NULL ) ; 
HANDLE  hEvent2 = CreateEvent ( NULL, FALSE, FALSE, NULL ) ;
HANDLE  hEvent3 = CreateEvent ( NULL, FALSE, FALSE, NULL ) ;
OVERLAPPED ov1 = { 0, 0, 0, 0, hEvent1 } ;  
OVERLAPPED ov2 = { 0, 0, 0, 0, hEvent2 } ;  
OVERLAPPED ov3 = { 0, 0, 0, 0, hEvent3 } ;  
HANDLE hFile = CreateFile ( ……, FILE_FLAG_OVERLAPPED, …… ) ;
ReadFile ( hFile, bBuf1, sizeof(bBuf1), &nReadByte, &ov1 ) ;
ReadFile ( hFile, bBuf2, sizeof(bBuf2), &nReadByte, &ov2 ) ;
ReadFile ( hFile, bBuf3, sizeof(bBuf3), &nReadByte, &ov3 ) ;
//此时3个I/O操作的同步对象分别为hEvent1,hEvent2,hEvent3
GetOverlappedResult ( hFile, &ov1, &nRead, TRUE ) ;
  这样，这个GetOverlappedResult就可以实现对第一个I/O处理的等待
关于重叠I/O的就讨论到这里，关于重叠I/O的实际应用，可以参考《Windows系统编程之进程通信》其中的命名管道实例。
http://bbs.pediy.com/showthread.php?s=&threadid=26252
三、  使用异步过程调用实现异步I/O
异步过程调用（APC），即在特定的上下文中异步的执行一个调用。在异步I/O中可以使用APC，即让操作系统的IO系统在完成异步I/O后立即调用你的程序。（在有些资料中，把异步I/O中的APC称为“完成例程”，感觉这个名称比较贴切，下文就以“完成例程”来表述。另外通常APC是作为线程同步这一块的内容，这里尽量淡化这个概念以免混淆。关于APC的详细内容到线程同步时再介绍 ）
这里需要注意三点：
（1）  APC总是在调用线程中被调用；
（2）  当执行APC时，调用线程会进入可变等待状态；
（3）  线程需要使用扩展I/O系列函数，例如ReadFileEx,WriteFileEx, 另外可变等待函数也是必须的（至少下面其中之一）：
WaitForSingleObjectEx
WaitForMultipleObjectEx
SleepEx
SignalObjectAndWait
MsgWaitForMultipleObjectsEx
  在使用ReadFileEx,WriteFileEx时，重叠结构OVERLAPPED中的hEvent成员并非一定要指定，因为系统会忽略它。当多个IO操作共用同一个完成例程时，可以使用hEvent来携带序号等信息，用于区别不同的I/O操作，因为该重叠结构会传递给完成例程。如果多个IO操作使用的完成例程都不相同时，则直接把hEvent设置为NULL就可以了。
在系统调用完成例程有两个条件：
（1）  I/O操作必须完成
（2）  调用线程处于可变等待状态
对于第一个条件比较容易，显然完成例程只有在I/O操作完成时才调用；至于第二个条件就需要进行认为的控制，通过使用可变等待函数，让调用线程处于可变等待状态，这样就可以执行完成例程了。这里可以通过调节调用可变等待函数的时机来控制完成例程的执行，即可以确保完成例程不会被过早的执行。
当线程具有多个完成例程时，就会形成一个队列。使用可变等待函数使线程进入可变等待状态时有一个表示超时值的参数，如果使用INFINITE，那么只有所有排队的完成例程被执行或者句柄获得信号时该等待函数才返回。
上面已经对利用完成例程实现异步I/O的一些比较重要的细节进行的简洁的阐述，接下来就以一个实例来说明完成例程的具体实现过程。
实例一：使用完成例程的异步I/O示例
1、  设计目标
体会完成例程的异步I/O实现原理及过程。
2、  问题的分析与设计
设计流程图如下：
![3_gif](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090406/3_gif_812633746084672666250.gif)
示图说明：
  三个IO操作分别是IO_A, IO_B, IO_C, 他们的完成例程分别是APC_A, APC_B, APC_C。IO_A, IO_B是两个很短的IO操作，IO_C是一个比较费时的IO操作。
3、  详细设计（关键代码如下,具体参见附件中的源代码CompletionRoutine）
> 代码:
VOID WINAPI APC_A ( DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo )
{
  pTempInfo.push_back ( "执行IO_A的完成例程" ) ;
}
VOID WINAPI APC_B ( DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo )
{
  pTempInfo.push_back ( "执行IO_B的完成例程" ) ;
}
VOID WINAPI APC_C ( DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo )
{
  pTempInfo.push_back ( "执行IO_C的完成例程" ) ;
}
void CCompletionRoutineDlg::OnTest() 
{
  // TODO: Add your control notification handler code here
  HANDLE    hFile_A, hFile_B, hFile_C ;
  OVERLAPPED  ov_A = {0}, ov_B = {0}, ov_C = {0} ;
#define C_SIZE 1024 * 1024 * 32
  string  szText_A = "Sample A !" ;
  string  szText_B = "Sampel B !" ;
  string  szText_C ;
  szText_C.resize ( C_SIZE ) ;
  memset ( &(szText_C[0]), 0x40, C_SIZE ) ;
  
  pTempInfo.clear () ;
  hFile_A = CreateFile ( "A.txt", GENERIC_WRITE, 0, NULL, /
              CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL ) ;
  hFile_B = CreateFile ( "B.txt", GENERIC_WRITE, 0, NULL, /
              CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL ) ;
  hFile_C = CreateFile ( "C.txt", GENERIC_WRITE, 0, NULL, /
              CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL ) ;
  WriteFileEx ( hFile_A, &(szText_A[0]), szText_A.length(), &ov_A, APC_A ) ;
  pTempInfo.push_back ( "启动IO_A, 并立即返回" ) ;
  WriteFileEx ( hFile_B, &(szText_B[0]), szText_B.length(), &ov_B, APC_B ) ;
  pTempInfo.push_back ( "启动IO_B, 并立即返回" ) ;
  WriteFileEx ( hFile_C, &(szText_C[0]), szText_C.size(), &ov_C, APC_C ) ;
  pTempInfo.push_back ( "启动IO_C, 并立即返回" ) ;
  pTempInfo.push_back ( "进入可变等待状态" ) ;
  SleepEx ( 1, true ) ;
  pTempInfo.push_back ( "结束可变等待状态" ) ;
  pTempInfo.push_back ( "进入可变等待状态" ) ;
  SleepEx ( 10000, true ) ;
  pTempInfo.push_back ( "结束可变等待状态" ) ;
  CloseHandle ( hFile_A ) ;
  CloseHandle ( hFile_B ) ;
  CloseHandle ( hFile_C ) ;
  m_ListBox.ResetContent () ;
  
  list<string>::iterator p ;
  for ( p = pTempInfo.begin(); p != pTempInfo.end(); p++ )
  {
    m_ListBox.AddString ( p->data() ) ;
  }
  DeleteFile ( "A.txt" ) ;
  DeleteFile ( "B.txt" ) ;
  DeleteFile ( "C.txt" ) ;
}
执行后的效果如下（WinXP+SP2+VC6.0）：
![pic](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090406/pic633746084674385000.gif)
4、  心得体会
每当一个IO操作结束时会产生一个完成信息，如果该IO操作有完成例程的话就添加到完成例程队列。一旦调用线程进入可变等待状态，就会依次执行队列中的完成例程。
在这个示例中还有一个问题，如果把这个软件放在系统分区的文件目录下可以正常执行，而放在其他盘符下就会出现问题，执行结果就不同，真是奇怪了。
四、使用完成端口（IOCP）
实例二、使用IOCP的异步I/O示例
1、设计目标
体会完成端口的异步I/O实现原理及过程。
2、  问题的分析与设计
![4_gif](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090406/4_gif_818633746084673760000.gif)
说明：
  每个客户端与一个管道进行交互，而在交互过程中I/O操作结束后产生的完成包就会进入“I/O完成包队列”。完成端口的线程队列中的线程使用GetQueuedCompletionStatus来检测“I/O完成包队列”中是否有完成包信息。 
3、详细设计（关键代码如下，具体见附件中的源码）
> 代码:
UINT ServerThread ( LPVOID lpParameter )
{
  ……
  while ( true )
  {
    GetQueuedCompletionStatus ( pMyDlg->hCompletionPort, &cbTrans, &dwCompletionKey, &lpov, INFINITE ) ;
    if ( dwCompletionKey == -1 )
      break ;
    // 读取管道信息
    // 响应管道信息（写入）
  }
  return 0 ;
}
void CMyDlg::OnStart() 
{
  // 创建完成端口
  hCompletionPort = CreateIoCompletionPort ( INVALID_HANDLE_VALUE, NULL, 0, nMaxThread ) ;
  CString lpPipeName = "////.//Pipe//NamedPipe" ;
  for ( UINT i = 0; i < nMaxPipe; i++ )
  {
    // 创建命名管道
    PipeInst[i].hPipe =  CreateNamedPipe ( lpPipeName, PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED, /
          PIPE_TYPE_BYTE|PIPE_READMODE_BYTE|PIPE_WAIT, nMaxPipe, 0, 0, INFINITE, NULL ) ;
    ……
    // 把命名管道与完成端口关联起来
    HANDLE hRet = CreateIoCompletionPort ( PipeInst[i].hPipe, hCompletionPort, i, nMaxThread ) ;
    ……
    // 等待连接
    ConnectNamedPipe ( PipeInst[i].hPipe, &(PipeInst[i].ov) ) ;
  }
  // 创建线程
  for ( i = 0; i < nMaxThread; i++ )
  {
    hThread[i] = AfxBeginThread ( ServerThread, NULL, THREAD_PRIORITY_NORMAL ) ;
  }
  ……
}
void CMyDlg::OnStop() 
{
  for ( UINT i = 0; i < nMaxThread; i++ )
  {
    // 用来唤醒线程的虚假I/O完成包
    PostQueuedCompletionStatus ( hCompletionPort, 0, -1, NULL ) ;
    CloseHandle ( hThread[i] ) ;
  }
  for ( i = 0; i < nMaxPipe; i++ )
  {
    DisconnectNamedPipe ( PipeInst[i].hPipe ) ;
    CloseHandle ( PipeInst[i].hPipe ) ;
  }
  ……
}
4、心得体会
  上面这个例子是关于完成端口的简单应用。可以这样来理解完成端口，它与三种资源相关分别是管道、I/O完成包队列、线程队列，它的作用是协调这三种资源。
【参考文献】
[1]. Windows系统编程. Johnson M.Hart著
【版权声明】必须注明来源看雪技术论坛(bbs.pediy.com) 及作者，并保持文章的完整性。
引用:http://www.pediy.com/bbshtml/bbs8/pediy8-709.htm
