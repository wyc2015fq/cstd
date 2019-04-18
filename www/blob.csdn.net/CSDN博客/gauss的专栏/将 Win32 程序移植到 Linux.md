# 将 Win32 程序移植到 Linux - gauss的专栏 - CSDN博客
2013年01月04日 22:05:31[gauss](https://me.csdn.net/mathlmx)阅读数：322
个人分类：[多线程](https://blog.csdn.net/mathlmx/article/category/954554)
首先声明开发环境。Win32程序的开发环境是VS.Net 2008，编程语言是C；Linux使用的RHEL 5.4，GCC的版本是4.1.2，编程语言也是C。
然后依次说明移植的对象：数据类型/字符串处理/系统调用/套接字/进程/进程锁/线程/线程锁/信号量/事件锁/条件锁/系统服务。
**1. 数据类型：**在开发过程中，无论哪种平台，只使用最通用的数据类型char，unsigned char，int，unsigned int，void *以及它们组合的结构体类型。对于数据长度敏感的代码，则只使用下面的数据类型：
|Common|Win32|Linux|
|----|----|----|
|int8|INT8|int8_t|
|uint8|UINT8|uint8_t|
|int16|INT16|int16_t|
|uint16|UINT16|uint16_t|
|int32|INT32|int32_t|
|uint32|UINT32|uint32_t|
|int64|INT64|int64_t|
|uint64|UINT64|uint64_t|
在VS.Net 2008的项目中，使用多字节字符集，对于需要宽字符的系统调用，使用字节转换函数来处理参数。
**2. 字符串处理：**虽然每个函数在两个平台下都有对应的实现，但是最好自己重新实现，因为它们都不足够令人满意：
|Common|Win32|Linux|
|----|----|----|
|stricmp_x|_stricmp|strcasecmp|
|strtok_x|strtok_s|strtok_r|
|sprintf_x|sprintf_s|snprintf|
|vsprintf_x|vsprintf_s|vsnprintf|
|strcpy_x|strcpy_s|strncpy|
|strcat_x|strcat_s|strncat|
**3. 系统调用：**真正的系统调用虽然不多，只有仅有的几十个，但有些系统调用差别很大，这里就不便做一一赘述了，而有些系统调用基本没有差别，比如文件相关的操作。
**4. 套接字：**套接字的几个主要函数都一样，socket/bind/listen/connect/accept/select/send/recv，几个细微的差别在于Win32使用套接字运行TCP/IP协议需要初始化上下文环境，另外，对于套接字定义，Win32使用SOCKET，Linux使用int，对于关闭套接字，Win32使用closesocket，Linux使用close。
**5. 进程：**在Windows平台中使用CreateProcess来创建进程，子进程返回句柄和ID给父进程，在Linux平台中使用fork和execv来创建进程，子进程返回ID给父进程。两者最大的差别在于，在Windows平台中子进程跟父进程没有任何关系，而在Linux平台中，子进程继承了父进程的进程上下文环境。其它相关函数的差别如下：
|Win32|Linux|
|----|----|
|CreateProcess|fork/execv|
|TerminateProcess|kill|
|ExitProcess|exit|
|GetCommandLine|argv|
|GetCurrentProcessId|getpid|
|KillTimer|alarm|
|SetEnvironmentVariable|putenv|
|GetEnvironmentVariable|getenv|
|GetExitCodeProcess|waitpid|
**6. 进程锁：**是指多个进程同步的机制。多进程同步的方法有很多，比如共享内存，命名信号量等。这里只说明一下命名信号量的机制，共享内存的方法可以查阅相关手册。Win32比较简单，在CreateMutex的参数中输入相应名称即可，Linux中，则可使用System V IPC的semget/semctl/semop操作，具体步骤直接man之。
**7. 线程：**线程同步、等待函数、线程本地存储以及初始化和终止抽象是线程模型的重要部分。主要对应函数列表如下：
|Win32|Linux|
|----|----|
|_beginthreadex|pthread_create|
|_endthreadex|pthread_exit|
|TerminateThread|pthread_cancel|
|GetCurrentThreadId|pthread_self|
**8. 线程锁：**对应函数列表如下。另外，特别注意的是，Win32的mutex在缺省情况下是可以递归加锁和解锁的，但是pthread的mutex在缺省情况下则不能，需要在pthread_mutex_init时设置pthread_mutexattr_t参数。
|Win32|Linux|
|----|----|
|CreateMutex|pthread_mutex_init|
|CloseHandle|pthread_mutex_destroy|
|WaitForSingleObject|pthread_mutex_lock|
|ReleaseMutex|pthread_mutex_unlock|
**9. 信号量/事件锁/条件锁：**这三个同步机制功能比较相似，甚至可以相互实现。除了这三种同步机制外，各个平台上还有一些专有的同步机制。Windows平台上有信号量和事件锁，但是没有条件锁，直到Windows 2008 Server上才会有。Linux平台上有信号量和条件锁，但是没有事件锁。在一些特定场合下，没有的同步机制只能通过已有的同步机制去实现：
|Common|Win32|Linux|
|----|----|----|
|Semaphore|CreateSemaphore|pthread_mutex_init pthread_cond_init (implement) or sem_init|
|Event|CreateEvent|pthread_mutex_init (implement)|
|Condition|CreateSemaphore (implement)|pthread_cond_init|
**10. 系统服务：**在Windows中，系统服务叫service，可以通过管理工具，或者运行services.msc进入管理界面，可以安装，卸载，启动，停止和重启。在Linux中，系统服务叫daemon，一般通过命令service ??? start/stop/restart来启动，停止和重启，同样也可以安装和卸载。Windows服务可以通过SCM（Service Control Management）架构来实现，Linux服务则需要编写chkconfig相关的脚本来实现。注意，不同的Linux发行版，比如RHEL和SLES，实现脚本是不一样的。
**转载声明：** 本文转自 [http://csynine.blog.51cto.com/1388509/310927](http://csynine.blog.51cto.com/1388509/310927)
========================================================================
[Linux下定义windows常用数据类型](http://blog.csdn.net/vah101/archive/2010/12/23/6094682.aspx)
