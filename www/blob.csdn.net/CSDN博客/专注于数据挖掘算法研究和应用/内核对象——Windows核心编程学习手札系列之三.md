# 内核对象——Windows核心编程学习手札系列之三 - 专注于数据挖掘算法研究和应用 - CSDN博客





2008年12月15日 17:23:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2307








# 内核对象

### ——Windows核心编程学习手札系列之三

内核对象可供系统和应用程序使用来管理各种各样的资源，如进程、线程、文件等，是内核分配的一个内存块，只能又内核访问，该内存块是一种数据结构，它的成员负责维护该对象的各种信息。内核对象的数据结构，也就是内存块是不允许应用程序直接修改其内容，而由Windows提供一组函数进行访问，创建内核对象的函数会返回对象的句柄，该句柄对于进程中的任何线程都可见，这个句柄传递给Windows各个函数，操作系统就知道要操作的内核对象。内核句柄与进程相关，如果跨越进程调用句柄可能会失败，如何实现多进程共享单个内核对象需要一定机制，后文将提到。

内核对象为内核所拥有，而非进程。如果一个进程创建了内核对象，在进程终止运行后内信对象不一定被撤消。内核通过内核对象类型中的常用数据成员使用计数知道有多少个进程正在使用该内核对象。当进程终止时，内核会对使用计数进行减一操作，如果到零则撤消该对象，确保在没有任何进程引用该对象情况下系统中不保留该内核对象。

创建内核对象的函数都有一个SECURITY_ATTRIBUTES结构的指针作为参数，其结构成员lpSecurityDescriptor与内核对象安全性有关。安全描述符描述了谁创建了内核对象，谁可以访问或使用该对象。这里可以通过设置SECURITY_ATTRIBUTES结构内lpSecurityDescriptor成值来满足。对于用户对象或图形设备接口（菜单、窗口、光标、字体等）的创建函数是不需要设定安全属性的信息，而内核对象则需要。

进程初始化时，操作系统会为其分配一个句柄表，该表用于设置内核对象数据结构的内存地址、访问权限以及继承等标志位。进程的线程在创建内核对象时，内核为其分配内存块并初始化并到进程的句柄表中扫描空项并设置相关信息。当进程调用BOOL CloseHandle(HANDLE hobj)函数结束内核对象时，函数也会先检查进程句柄表，以标识进程无权访问该对象，同时获得内核对象数据结构地址中使用计数的数据成员，如为零则从内存中撤消该内核对象。当然，如果忘记调用CloseHandle函数将导致内存泄露的可能，不过仅存在进程运行时才会发生。因为一旦进程终止运行，操作系统将对进程内的任何资源进行释放，体现在内核对象的释放上就是通过访问进程句柄表；当进程终止运行，系统会扫描进程句柄表，如果该表仍有无效项目（终止进程前没有关闭的对象），系统将自动关闭这些对象，如果这些对象的使用计数为零则由内核撤消该对象。

不同进程中运行的线程在如下情况中需要共享内核对象：文件映射对象使你能够在同一台机器上运行的两个进程之间共享数据块；邮箱或指定的管道使得应用程序能够在联网的不同机器上运行的进程之间发送数据块；互斥对象、信标和事件使得不同进程中线程能够同步它们的连续运行，这与一个应用程序在完成某项任务时需要通知另一个应用程序的情况相同。总的来说是数据共享、数据发送、同步控制三方面的需要。

内核对象句柄的继承性，内核对象句柄是和进程相关的。当父进程创建内核对象句柄时，向操作系统指明对象句柄是个可继承的句柄，通过对SECURITY_ATTRIBUTES结构中bInheritHandle设置来实现，这将对进程句柄表中继承标志位设为1。内核对象本身不具备继承性，但内核对象句柄具有继承性。父进程中设置了该对象句柄可继承，那么在创建子进程的函数CreateProcess中bInheritHandle参数也需要设为TRUE，这时操作系统同样为子进程分配新的和空的句柄表之外还会遍历父进程句柄表，拷贝父进程有效的可继承的句柄项目到子进程句柄表中，同时递增内核对象的使用计数。对于已经运行的子进程，父进程正创建的带有可继承句柄的新内核对象是无法继承的，也就是说对象句柄的继承性只有在生成子进程时候才能使用。若要改变句柄标志，可通过SetHandleInformation函数：

BOOL SetHandleInformation( HANDLE hObject,DWORD dwMask,DWORD dwFlags);

第一个参数hObject是一个有效句柄；第二个参数dwMask告诉函数想要改变哪个标志，目前有两个标志与每个句柄关联，#define HANDLE_FLAG_INHERIT 0x00000001/#define HANDLE_FLAG_PROTECT_FROM_CLOSE 0x00000002，同时改变这两个标志可用OR连接起来；第三个参数是dwFlags指明想将标志设置成什么值，如要打开一个内核对象句柄的继承标志，可用下面代码：

SetHandleInformation(hObj,HANDLE_FLAG_INHERIT,HANDLE_FLAG_INHERIT);

若要关闭标志，则：

SetHandleInformation(hObj,HANDLE_FLAG_INHERIT,0);

HANDLE_FLAG_PROTECT_FROM_CLOSE标志用于告诉系统，该句柄不应被关闭：

SetHandleInfomation(hObj,HANDLE_FLAG_PROTECT_FROM_CLOSE,HANDLE_FLAG_PROTECT_FROM_CLOSE);

CloseHandle(hObj);

如果一个线程试图关闭一个受保护的句柄，CloseHandle会产生一个异常条件。当然可以通过GetHandleInformation来获取句柄的标志是否满足可继承性，如下面：

DWORD dwFlags;

GetHandleInformation(hObj,&dwFlags);

BOOL fHandleIsInheritable=(0!=(dwFlags & HANDLE_FLAG_INHERIT));

if(fHandleIsInheritable) //可继承

else //不可继承

给对象命名是实现共享跨越进程边界内核对象的第二种方法。对对象命名，而后要共享者打开该名字，即可共享该对象。服务器的名字命名空间对象是放在全局名字空间中，按照默认设置，在终端服务器中，应用程序的命名内核对象将放入会话的名字空间中。将命名对象置于全局名字空间的代码：

HANDLE h=CreateEvent(NULL,FALSE,FALSE,”Global//myname”);

将内核对象放入会话的名字空间代码：

HANDLE h=CreateEvent(NULL,FALSE,FALSE,”Local//myname”);

终端服务器拥有内核对象的多个名字空间，全局名字空间和会话名字空间。全局名字空间意味着所有客户程序都可以访问，该名字空间主要供服务程序使用；而会话名字空间则属于每个客户程序自己的名字空间，可防止运行相同应用程序的两个或多个会话之间出现互相干扰情况，一个会话无法访问另一个会话的对象，尽管对象之间可能拥有相同名字。Microsoft将Golbal和Local作为保留关键字。

复制对象句柄是共享跨越进程边界内核对象最后方法：

BOOL DuplicateHandle(

HANDLE hSourceProcessHandle,

HANDLE hSoruceHandle,

HANDLE hTargetProcessHandle,

PHANDLE phTargetHandle,

DWORD dwDesiredAccess,

BOOLbInheritHandle,

DWORD dwOptions);

这个函数将一个进程句柄表中的项目取出，拷贝到另一个进程的句柄表中。例子：Process S拥有对一个内核对象的访问权，想让Process T能够访问该对象：

//All of the following code is executed by Process S

//Create a mutex object accessible by Process S

HANDLE hObjProcessS=CreateMutex(NULL,FALSE,NULL);

//Open a handle to Process T’s kernel object.

HANDLE hProcessT=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessIDT);

HANDLE hObjProcessT;//An uninitialized handle relative to Process T

//Give Process T access to our mutex object.

DuplicateHandle(GetCurrentProcess(),hObjProcessS,hProcessT,&hObjProcessT,0,FALSE,

DUPLICATE_SAME_ACCESS);

//Use some IPC mechanism to get the handle

//value in hObjProcessS into Process T

……

//We no longer need to communicate with ProcessT.

CloseHandle(hProcessT);

……

//When Process S no longer needs to use the mutex,ti should close it

CloseHandle(hObjProcessS);

IPC(Internet Process Connection)是共享"命名管道"的资源，它是为了让进程间通信而开放的命名管道，通过提供可信任的用户名和口令，连接双方可以建立安全的通道并以此通道进行加密数据的交换，从而实现对远程计算机的访问。

如非 <?xml:namespace prefix = st1 ns = "urn:schemas-microsoft-com:office:smarttags" />2008-11-25



