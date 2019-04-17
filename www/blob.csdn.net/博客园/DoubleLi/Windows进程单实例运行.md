# Windows进程单实例运行 - DoubleLi - 博客园






场景
         Windows进程单实例运行，如果有进程没有退出，继续等待，直到进程完全退出，才会进入下一个实例
 HANDLE pHandle = NULL;
 do 
 {
  pHandle = ::CreateMutex(NULL, FALSE, "Global\\fengyuzaitu.blog.51cto.com");
  if (ERROR_ALREADY_EXISTS == GetLastError() || ERROR_ACCESS_DENIED == GetLastError())
  {
   ::Sleep(1000);
   if (NULL != pHandle) ::CloseHandle(pHandle);
  }
  else
  {
   break;
  }
 } while (1);

      //开始进入进程服务代码

     //进程结束，关闭句柄
 if (NULL != pHandle) ::CloseHandle(pHandle);

分析1）“Global\\”前缀代表全局的内核对象空间，对任何的登陆会话都有效
翻译：https://docs.microsoft.com/zh-cn/windows/desktop/TermServ/kernel-object-namespaces
远程服务器为事件，信号量，互斥量，超时定时器，文件映射对象和作业对象这些命名内核对象，创建不同的命名空间。在客户端/服务器应用程序中存在一个全局的命名空间。此外，在Windows Vista系统中，每一个客户端会话都会有一个隔离的命名空间来存放这些对象。隔离的客户端会话空间允许多个客户运行相同的进程实例而互不干扰。对于在客户端会话启动的进程，系统会在
默认的会话空间中创建进程。但是，所有的进程都共享以"Global\"作为前缀定义的命名空间。例如，下面的代码通过调用CreateEvent，在全局命名空间中创建一个叫CSAPP的的事件对象。
代码
 CreateEvent( NULL, FALSE, FALSE, "Global\\CSAPP" );
在远程桌面服务环境中，服务程序默认采用全局命名空间。会话零只应用于宿主服务，没有控制台会话。全局命名空间允许进程同一个服务应用在不同的用户会话进行通信。例如，一个客户端/服务器应用可能会为了同步，使用一个互斥对象。服务断模块创建一个全局命名空间的互斥对象。然后客户端会话可以使用“Global\"前缀打开一个互斥对象。


全局命名空间也可以用在检测一个使用了命名对象的应用是否已经启动，检测的范围包括所有的会话。命名对象必须在全局命名空间中创建或者打开，不能是单个会话空间。The more common case of running the application once per session is supported by default because the named object is created in a per session namespace.除了"Global\"前缀，客户端进程可以用"Local\”前缀在会话空间中几餐对象。关键字大小写敏感。



分析2）
ERROR_ALREADY_EXISTS == GetLastError()
这个错误是在同一个会话中会出现这个错误，同一个会话是指同样的方式启动，在同一个登陆用户以控制台或者服务启动
ERROR_ACCESS_DENIED == GetLastError()
这个错误是在一个进程实例以服务的形式启动，一个进程实例以控制台的形式启动，就会返回这个错误

https://docs.microsoft.com/zh-cn/windows/desktop/api/synchapi/nf-synchapi-createmutexa
CreateMutexA说明









