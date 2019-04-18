# VC服务编程 - 深之JohnChen的专栏 - CSDN博客

2013年02月02日 16:51:47[byxdaz](https://me.csdn.net/byxdaz)阅读数：12301



一、服务编写原理

1．1什么是windows的服务程序？

可以使用下面的几种方法看到它。打开控制面板，然后是管理工具，里面有一个“服务”，双击后打开；或者是通过输入命令的方式，打开开始菜单，点击运行，输入mmc services.msc(mmc可省略)，也可打开。我们会在打开的页面中看到一个大的列表，标题栏上包含有名称、描述、状态、启动类型、登录身份等项。其中在状态一栏中显示为“已启动”的是系统中已经启动了的服务。我们先看一下服务的属性。举个例子，找到Print
 Spooler这一名称，然后用右键在上面点击，选择“属性”，可以看到它所执行的命令行是C:\WINDOWS\system32\spoolsv.exe，按下停止后，任务管理器中spoolsv.exe进程退出。我们所见到的这个列表就是服务程序的集中地，每一项就是一个服务程序。

上面这些标为自启动的服务程序随系统一起启动。它与一些修改注册表：

HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Windows/CurrentVersionRun项，及类似注册表项的程序不同的是，即使用户没有登录到系统中，它们也是会运行的，或者说它们在系统登录前运行。

1．2服务组成

一个服务由三部分组成，第一部分是Service Control Manager(SCM)。每个Windows NT/2000系统都有一个SCM，SCM存在于Service.exe中，在Windows启动的时候会自动运行，伴随着操作系统的启动和关闭而产生和终止。这个进程以系统特权运行，并且提供一个统一的、安全的手段去控制服务。它其实是一个RPC
 Server，因此我们可以远程安装和管理服务，不过这不在本文讨论的范围之内。SCM包含一个储存着已安装的服务和驱动程序的信息的数据库，通过SCM可以统一的、安全的管理这些信息，因此一个服务程序的安装过程就是将自身的信息写入这个数据库。

第二部分就是服务本身。一个服务拥有能从SCM收到信号和命令所必需的的特殊代码，并且能够在处理后将它的状态回传给SCM。

第三部分也就是最后一部分，是一个Service Control Dispatcher(SCP)。它是一个拥有用户界面，允许用户开始、停止、暂停、继续，并且控制一个或多个安装在计算机上服务的Win32应用程序。SCP的作用是与SCM通讯，Windows
 2000管理工具中的“服务”就是一个典型的SCP。在这三个组成部分中，用户最可能去写服务本身，同时也可能不得不写一个与其伴随的客户端程序作为一个SCP去和SCM通讯。

       1.3服务设计

一个服务拥有很重要的三个函数，第一个就是入口点函数，其实用WinMain()作为入口点函数也不是不可以，虽然说服务不应该有用户界面，但是其实存在很少的几个例外。由于要和用户桌面进行信息交互，服务程序有时会以WinMain()作为入口点函数。入口函数负责初始化整个进程，由这个进程中的主线程来执行。这意味着它应用于这个可执行文件中的所有服务。要知道，一个可执行文件中能够包含多个服务以使得执行更加有效。主进程通知SCM在可执行文件中含有几个服务，并且给出每一个服务的ServiceMain回调(Call
 Back)函数的地址。一旦在可执行文件内的所有服务都已经停止运行，主线程就在进程终止前对整个进程进行清除。第二个很重要的函数就是ServiceMain，我看过一些例子程序里面对自己的服务的进入点函数都固定命名为ServiceMain，其实并没有规定过一定要那样命名，任何的函数只要符合下列的形式都可以作为服务的进入点函数。
VOID WINAPI ServiceMain(

  DWORD dwArgc, // 参数个数
  LPTSTR *lpszArgv // 参数串
);

这个函数由操作系统调用，并执行能完成服务的代码。一个专用的线程执行每一个服务的ServiceMain函数，注意是服务而不是服务程序，这是因为每个服务也都拥有与自己唯一对应的ServiceMain函数，关于这一点可以用“管理工具”里的“服务”去察看Win2000里面自带的服务，就会发现其实很多服务都是由service.exe单独提供的。当主线程调用Win32函数StartServiceCtrlDispatcher的时候，SCM为这个进程中的每一个服务产生一个线程。这些线程中的每一个都和它的相应的服务的ServiceMain函数一起执行，这就是服务总是多线程的原因——一个仅有一个服务的可执行文件将有一个主线程，其它的线程执行服务本身。

第三个也就是最后的一个重要函数是CtrlHandler，它必须拥有下面的原型：
VOID WINAPI CtrlHandler(

DWORD fdwControl //控制命令
)
像ServiceMain一样，CtrlHandler也是一个回调函数，用户必须为它的服务程序中每一个服务写一个单独的CtrlHandler函数，因此如果有一个程序含有两个服务，那么它至少要拥有5个不同的函数：作为入口点的main()或WinMain()，用于第一个服务的ServiceMain函数和CtrlHandler函数，以及用于第二个服务的ServiceMain函数和CtrlHandler函数。

SCM调用一个服务的CtrlHandler函数去改变这个服务的状态。例如，当某个管理员用管理工具里的“服务”尝试停止你的服务的时候，你的服务的CtrlHandler函数将收到一个SERVICE_CONTROL_STOP通知。CtrlHandler函数负责执行停止服务所需的一切代码。由于是进程的主线程执行所有的CtrlHandler函数，因而必须尽量优化你的CtrlHandler函数的代码，使它运行起来足够快，以便相同进程中的其它服务的CtrlHandler函数能在适当的时间内收到属于它们的通知。而且基于上述原因，你的CtrlHandler函数必须要能够将想要传达的状态送到服务线程，这个传递过程没有固定的方法，完全取决于你的服务的用途。

SERVICE_TABLE_ENTRY service_table_entry[] =

{

  { /"MyFTPd/" , FtpdMain },

  { /"MyHttpd/", Httpserv},

  { NULL, NULL },

};

第一个成员代表服务的名字，第二个成员是ServiceMain回调函数的地址，上面的服务程序因为拥有两个服务，所以有三个SERVICE_TABLE_ENTRY元素，前两个用于服务，最后的NULL指明数组的结束。

接下来这个数组的地址被传递到StartServiceCtrlDispatcher函数：

BOOL StartServiceCtrlDispatcher(

LPSERVICE_TABLE_ENTRY lpServiceStartTable

)

这个Win32函数表明可执行文件的进程怎样通知SCM包含在这个进程中的服务。就像上一章中讲的那样，StartServiceCtrlDispatcher为每一个传递到它的数组中的非空元素产生一个新的线程，每一个进程开始执行由数组元素中的lpServiceStartTable指明的ServiceMain函数。

SCM启动一个服务程序之后，它会等待该程序的主线程去调StartServiceCtrlDispatcher。如果那个函数在两分钟内没有被调用，SCM将会认为这个服务有问题，并调用TerminateProcess去杀死这个进程。这就要求你的主线程要尽可能快的调用StartServiceCtrlDispatcher。

StartServiceCtrlDispatcher函数则并不立即返回，相反它会驻留在一个循环内。当在该循环内时，StartServiceCtrlDispatcher悬挂起自己，等待下面两个事件中的一个发生。第一，如果SCM要去送一个控制通知给运行在这个进程内一个服务的时候，这个线程就会激活。当控制通知到达后，线程激活并调用相应服务的CtrlHandler函数。CtrlHandler函数处理这个服务控制通知，并返回到StartServiceCtrlDispatcher。StartServiceCtrlDispatcher循环回去后再一次悬挂自己。

第二，如果服务线程中的一个服务中止，这个线程也将激活。在这种情况下，该进程将运行在它里面的服务数减一。如果服务数为零，StartServiceCtrlDispatcher就会返回到入口点函数，以便能够执行任何与进程有关的清除工作并结束进程。如果还有服务在运行，哪怕只是一个服务，StartServiceCtrlDispatcher也会继续循环下去，继续等待其它的控制通知或者剩下的服务线程中止。

上面的内容是关于入口点函数的，下面的内容则是关于ServiceMain函数的。还记得以前讲过的ServiceMain函数的的原型吗？但实际上一个ServiceMain函数通常忽略传递给它的两个参数，因为服务一般不怎么传递参数。设置一个服务最好的方法就是设置注册表，一般服务在
HKEY_LOCAL_MACHINE//SYSTEM//CurrentControlSet//Service//ServiceName//Parameters
子键下存放自己的设置，这里的ServiceName是服务的名字。事实上，可能要写一个客户应用程序去进行服务的背景设置，这个客户应用程序将这些信息存在注册表中，以便服务读取。当一个外部应用程序已经改变了某个正在运行中的服务的设置数据的时候，这个服务能够用RegNotifyChangeKeyValue函数去接受一个通知，这样就允许服务快速的重新设置自己。

前面讲到StartServiceCtrlDispatcher为每一个传递到它的数组中的非空元素产生一个新的线程。接下来，一个ServiceMain要做些什么呢？MSDN里面的原文是这样说的：The
 ServiceMain function should immediately call the RegisterServiceCtrlHandler function to specify a Handler function to handle control requests. Next, it should call the SetServiceStatus function to send status information to the service control manager.为什么呢？因为发出启动服务请求之后，如果在一定时间之内无法完成服务的初始化，SCM会认为服务的启动已经失败了，这个时间的长度在Win
 NT 4.0中是80秒，Win2000中不详...

基于上面的理由，ServiceMain要迅速完成自身工作，首先是必不可少的两项工作，第一项是调用RegisterServiceCtrlHandler函数去通知SCM它的CtrlHandler回调函数的地址：

SERVICE_STATUS_HANDLE RegisterServiceCtrlHandler(

LPCTSTR lpServiceName, //服务的名字

LPHANDLER_FUNCTION lpHandlerProc //CtrlHandler函数地址

)

第一个参数指明你正在建立的CtrlHandler是为哪一个服务所用，第二个参数是CtrlHandler函数的地址。lpServiceName必须和在SERVICE_TABLE_ENTRY里面被初始化的服务的名字相匹配。RegisterServiceCtrlHandler返回一个SERVICE_STATUS_HANDLE，这是一个32位的句柄。SCM用它来唯一确定这个服务。当这个服务需要把它当时的状态报告给SCM的时候，就必须把这个句柄传给需要它的Win32函数。注意：这个句柄和其他大多数的句柄不同，你无需关闭它。

SCM要求ServiceMain函数的线程在一秒钟内调用RegisterServiceCtrlHandler函数，否则SCM会认为服务已经失败。但在这种情况下，SCM不会终止服务，不过在NT
 4中将无法启动这个服务，同时会返回一个不正确的错误信息，这一点在Windows 2000中得到了修正。

在RegisterServiceCtrlHandler函数返回后，ServiceMain线程要立即告诉SCM服务正在继续初始化。具体的方法是通过调用SetServiceStatus函数传递SERVICE_STATUS数据结构。

BOOL SetServiceStatus(

SERVICE_STATUS_HANDLE hService, //服务的句柄

SERVICE_STATUS lpServiceStatus //SERVICE_STATUS结构的地址

)

这个函数要求传递给它指明服务的句柄(刚刚通过调用RegisterServiceCtrlHandler得到)，和一个初始化的SERVICE_STATUS结构的地址：

typedef struct _SERVICE_STATUS

{

DWORD dwServiceType;

DWORD dwCurrentState;

DWORD dwControlsAccepted;

DWORD dwWin32ExitCode;

DWORD dwServiceSpecificExitCode;

DWORD dwCheckPoint; 

DWORD dwWaitHint; 

} SERVICE_STATUS, *LPSERVICE_STATUS;

SERVICE_STATUS结构含有七个成员，它们反映服务的现行状态。所有这些成员必须在这个结构被传递到SetServiceStatus之前正确的设置。

成员dwServiceType指明服务可执行文件的类型。如果你的可执行文件中只有一个单独的服务，就把这个成员设置成SERVICE_WIN32_OWN_PROCESS；如果拥有多个服务的话，就设置成SERVICE_WIN32_SHARE_PROCESS。除了这两个标志之外，如果你的服务需要和桌面发生交互(当然不推荐这样做)，就要用“OR”运算符附加上SERVICE_INTERACTIVE_PROCESS。这个成员的值在你的服务的生存期内绝对不应该改变。

成员dwCurrentState是这个结构中最重要的成员，它将告诉SCM你的服务的现行状态。为了报告服务仍在初始化，应该把这个成员设置成SERVICE_START_PENDING。在以后具体讲述CtrlHandler函数的时候具体解释其它可能的值。

成员dwControlsAccepted指明服务愿意接受什么样的控制通知。如果你允许一个SCP去暂停/继续服务，就把它设成SERVICE_ACCEPT_PAUSE_CONTINUE。很多服务不支持暂停或继续，就必须自己决定在服务中它是否可用。如果你允许一个SCP去停止服务，就要设置它为SERVICE_ACCEPT_STOP。如果服务要在操作系统关闭的时候得到通知，设置它为SERVICE_ACCEPT_SHUTDOWN可以收到预期的结果。这些标志可以用“OR”运算符组合。

成员dwWin32ExitCode和dwServiceSpecificExitCode是允许服务报告错误的关键，如果希望服务去报告一个Win32错误代码(预定义在WinError.h中)，它就设置dwWin32ExitCode为需要的代码。一个服务也可以报告它本身特有的、没有映射到一个预定义的Win32错误代码中的错误。为了这一点，要把dwWin32ExitCode设置为ERROR_SERVICE_SPECIFIC_ERROR，然后还要设置成员dwServiceSpecificExitCode为服务特有的错误代码。当服务运行正常，没有错误可以报告的时候，就设置成员dwWin32ExitCode为NO_ERROR。

最后的两个成员dwCheckPoint和dwWaitHint是一个服务用来报告它当前的事件进展情况的。当成员dwCurrentState被设置成SERVICE_START_PENDING的时候，应该把dwCheckPoint设成0，dwWaitHint设成一个经过多次尝试后确定比较合适的数，这样服务才能高效运行。一旦服务被完全初始化，就应该重新初始化SERVICE_STATUS结构的成员，更改dwCurrentState为SERVICE_RUNNING，然后把dwCheckPoint和dwWaitHint都改为0。

dwCheckPoint成员的存在对用户是有益的，它允许一个服务报告它处于进程的哪一步。每一次调用SetServiceStatus时，可以增加它到一个能指明服务已经执行到哪一步的数字，它可以帮助用户决定多长时间报告一次服务的进展情况。如果决定要报告服务的初始化进程的每一步，就应该设置dwWaitHint为你认为到达下一步所需的毫秒数，而不是服务完成它的进程所需的毫秒数。

在服务的所有初始化都完成之后，服务调用SetServiceStatus指明SERVICE_RUNNING，在那一刻服务已经开始运行。通常一个服务是把自己放在一个循环之中来运行的。在循环的内部这个服务进程悬挂自己，等待指明它下一步是应该暂停、继续或停止之类的网络请求或通知。当一个请求到达的时候，服务线程激活并处理这个请求，然后再循环回去等待下一个请求/通知。

如果一个服务由于一个通知而激活，它会先处理这个通知，除非这个服务得到的是停止或关闭的通知。如果真的是停止或关闭的通知，服务线程将退出循环，执行必要的清除操作，然后从这个线程返回。当ServiceMain线程返回并中止时，引起在StartServiceCtrlDispatcher内睡眠的线程激活，并像在前面解释过的那样，减少它运行的服务的计数。当调用RegisterServiceCtrlHandler函数时，SCM得到并保存这个回调函数的地址。一个SCP调一个告诉SCM如何去控制服务的Win32函数，现在已经有10个预定义的控制请求：

|Control code|Meaning|
|----|----|
|SERVICE_CONTROL_STOP|Requests the service to stop. The hService handle must have SERVICE_STOP access.|
|SERVICE_CONTROL_PAUSE|Requests the service to pause. The hService handle must have SERVICE_PAUSE_CONTINUE access.|
|SERVICE_CONTROL_CONTINUE|Requests the paused service to resume. The hService handle must have SERVICE_PAUSE_CONTINUE access.|
|SERVICE_CONTROL_INTERROGATE|Requests the service to update immediately its current status information to the service control manager. The hService handle must have SERVICE_INTERROGATE access.|
|SERVICE_CONTROL_SHUTDOWN|Requests the service to perform cleanup tasks, because the system is shutting down. For more information, see Remarks.|
|SERVICE_CONTROL_PARAMCHANGE|Windows 2000: Requests the service to reread its startup parameters. The hService handle must have SERVICE_PAUSE_CONTINUE access.|
|SERVICE_CONTROL_NETBINDCHANGE|Windows 2000: Requests the service to update its network binding. The hService handle must have SERVICE_PAUSE_CONTINUE access.|
|SERVICE_CONTROL_NETBINDREMOVE|Windows 2000: Notifies a network service that a component for binding has been removed. The service should reread its binding information and unbind from the removed component.|
|SERVICE_CONTROL_NETBINDENABLE|Windows 2000: Notifies a network service that a disabled binding has been enabled. The service should reread its binding information and add the new binding.|
|SERVICE_CONTROL_NETBINDDISABLE|Windows 2000: Notifies a network service that one of its bindings has been disabled. The service should reread its binding information and remove the binding.|

上表中标有Windows 2000字样的就是2000中新添加的控制代码。除了这些代码之外，服务也可以接受用户定义的，范围在128-255之间的代码。

当CtrlHandler函数收到一个SERVICE_CONTROL_STOP、SERVICE_CONTROL_PAUSE、
 SERVICE_CONTROL_CONTINUE控制代码的时候，SetServiceStatus必须被调用去确认这个代码，并指定你认为服务处理这个状态变化所需要的时间。

例如：你的服务收到了停止请求，首先要把SERVICE_STATUS结构的dwCurrentState成员设置成SERVICE_STOP_PENDING，这样可以使SCM确定你已经收到了控制代码。当一个服务的暂停或停止操作正在执行的时候，必须指定你认为这种操作所需要的时间：这是因为一个服务也许不能立即改变它的状态，它可能必须等待一个网络请求被完成或者数据被刷新到一个驱动器上。指定时间的方法就像我上一章说的那样，用成员dwCheckPoint和dwWaitHint来指明它完成状态改变所需要的时间。如果需要，可以用增加dwCheckPoint成员的值和设置dwWaitHint成员的值去指明你期待的服务到达下一步的时间的方式周期性的报告进展情况。

当整个启动的过程完成之后，要再一次调用SetServiceStatus。这时就要把SERVICE_STATUS结构的dwCurrentState成员设置成SERVICE_STOPPED，当报告状态代码的同时，一定要把成员dwCheckPoint和dwWaitHint设置为0，因为服务已经完成了它的状态变化。暂停或继续服务的时候方法也一样。

当CtrlHandler函数收到一个SERVICE_CONTROL_INTERROGATE控制代码的时候，服务将简单的将dwCurrentState成员设置成服务当前的状态，同时，把成员dwCheckPoint和dwWaitHint设置为0，然后再调用SetServiceStatus就可以了。

在操作系统关闭的时候，CtrlHandler函数收到一个SERVICE_CONTROL_SHUTDOWN控制代码。服务根本无须回应这个代码，因为系统即将关闭。它将执行保存数据所需要的最小行动集，这是为了确定机器能及时关闭。缺省时系统只给很少的时间去关闭所有的服务，MSDN里面说大概是20秒的时间，不过那可能是Windows
 NT 4的设置，在我的Windows 2000 Server里这个时间是10秒，你可以手动的修改这个数值，它被记录在HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control子键里面的WaitToKillServiceTimeout，单位是毫秒。

当CtrlHandler函数收到任何用户定义的代码时，它应该执行期望的用户自定义行动。除非用户自定义的行动要强制服务去暂停、继续或停止，否则不调SetServiceStatus函数。如果用户定义的行动强迫服务的状态发生变化，SetServiceStatus将被调用去设置dwCurrentState、dwCheckPoint和dwWaitHint，具体控制代码和前面说的一样。

如果你的CtrlHandler函数需要很长的时间执行操作的话，千万要注意：假如CtrlHandler函数在30秒内没有返回的话，SCM将返回一个错误，这不是我们所期望的。所以如果出现上述情况，最好的办法是再建立一个线程，让它去继续执行操作，以便使得CtrlHandler函数能够迅速的返回。例如，当收到一个SERVICE_CONTROL_STOP请求的时候，就像上面说的一样，服务可能正在等待一个网络请求被完成或者数据被刷新到一个驱动器上，而这些操作所需要的时间是你不能估计的，那么就要建立一个新的线程等待操作完成后执行停止命令，CtrlHandler函数在返回之前仍然要报告SERVICE_STOP_PENDING状态，当新的线程执行完操作之后，再由它将服务的状态设置成SERVICE_STOPPED。如果当前操作的时间可以估计的到就不要这样做，仍然使用前面交待的方法处理。

CtrlHandler函数我就先讲这些，下面说说服务怎么安装。一个服务程序可以使用CreateService函数将服务的信息添加到SCM的数据库。

SC_HANDLE CreateService( SC_HANDLE hSCManager, // handle to SCM database LPCTSTR lpServiceName, // name of service to start LPCTSTR lpDisplayName, // display name DWORD dwDesiredAccess, // type of
 access to service DWORD dwServiceType, // type of service DWORD dwStartType, // when to start service DWORD dwErrorControl, // severity of service failure LPCTSTR lpBinaryPathName, // name of binary file LPCTSTR lpLoadOrderGroup, // name of load ordering group
 LPDWORD lpdwTagId, // tag identifier LPCTSTR lpDependencies, // array of dependency names LPCTSTR lpServiceStartName, // account name LPCTSTR lpPassword // account password );

hSCManager是一个标示SCM数据库的句柄，可以简单的通过调用OpenSCManager得到。

SC_HANDLE OpenSCManager( LPCTSTR lpMachineName, // computer name LPCTSTR lpDatabaseName, // SCM database name DWORD dwDesiredAccess // access type );

lpMachineName是目标机器的名字，还记得我在第一章里说过可以在其它的机器上面安装服务吗？这就是实现的方法。对方机器名字必须以“\\”开始。如果传递NULL或者一个空的字符串的话就默认是本机。

lpDatabaseName是目标机器上面SCM数据库的名字，但MSDN里面说这个参数要默认的设置成SERVICES_ACTIVE_DATABASE，如果传递NULL，就默认的打开SERVICES_ACTIVE_DATABASE。所以我还没有真的搞明白这个参数的存在意义，总之使用的时候传递NULL就行了。

dwDesiredAccess是SCM数据库的访问权限，具体值见下表：

|Object access|Description|
|----|----|
|SC_MANAGER_ALL_ACCESS|Includes STANDARD_RIGHTS_REQUIRED, in addition to all of the access types listed in this table.|
|SC_MANAGER_CONNECT|Enables connecting to the service control manager.|
|SC_MANAGER_CREATE_SERVICE|Enables calling of the CreateService function to create a service object and add it to the database.|
|SC_MANAGER_ENUMERATE_SERVICE|Enables calling of the EnumServicesStatus function to list the services that are in the database.|
|SC_MANAGER_LOCK|Enables calling of the LockServiceDatabase function to acquire a lock on the database.|
|SC_MANAGER_QUERY_LOCK_STATUS|Enables calling of the QueryServiceLockStatus function to retrieve the lock status information for the database.|

想要获得访问权限的话，似乎没那么复杂。MSDN里面说所有进程都被允许获得对所有SCM数据库的SC_MANAGER_CONNECT, SC_MANAGER_ENUMERATE_SERVICE,
 and SC_MANAGER_QUERY_LOCK_STATUS权限，这些权限使得你可以连接SCM数据库，枚举目标机器上安装的服务和查询目标数据库是否已被锁住。但如果要创建服务，首先你需要拥有目标机器的管理员权限，一般的传递SC_MANAGER_ALL_ACCESS就可以了。这个函数返回的句柄可以被CloseServiceHandle函数关闭。

lpServiceName是服务的名字，lpDisplayName是服务在“服务”管理工具里显示的名字。

dwDesiredAccess也是访问的权限，有一个比上面的还长的多的一个表，各位自己查MSDN吧。我们要安装服务，仍然简单的传递SC_MANAGER_ALL_ACCESS。

dwServiceType是指你的服务是否和其它的进程相关联，一般是SERVICE_WIN32_OWN_PROCESS，表示不和任何进程相关联。如果你确认你的服务需要和某些进程相关联，就设置成SERVICE_WIN32_SHARE_PROCESS。当你的服务要和桌面相关联的时候，需要设置成SERVICE_INTERACTIVE_PROCESS。

dwStartType是服务的启动方式。服务有三种启动方式，分别是“自动(SERVICE_AUTO_START)”“手动(SERVICE_DEMAND_START)”和“禁用(SERVICE_DISABLED)”。在MSDN里还有另外的两种方式，不过是专为驱动程序设置的。

dwErrorControl决定服务如果在系统启动的时候启动失败的话要怎么办。

|值|意义|
|----|----|
|SERVICE_ERROR_IGNORE|启动程序记录错误发生，但继续启动。|
|SERVICE_ERROR_NORMAL|启动程序记录错误发生，并弹出一个消息框，但仍继续启动|
|SERVICE_ERROR_SEVERE|启动程序记录错误发生，如果是以last-known-good configuration启动的话，启动会继续。否则会以last-known-good configuration重新启动计算机。|
|SERVICE_ERROR_CRITICAL|启动程序记录错误发生，如果可能的话。如果是以last-known-good configuration启动的话，启动会失败。否则会以last-known-good configuration重新启动计算机。好严重的错误啊。|

lpBinaryPathName是服务程序的路径。MSDN里面特别提到如果服务路径里面有空格的话一定要将路径用引号引起来。例如d:\\my share\\myservice.exe就一定要指定为\d:\\my
 share\\myservice.exe\。

lpLoadOrderGroup的意义在于，如果有一组服务要按照一定的顺序启动的话，这个参数用于指定一个组名用于标志这个启动顺序组，不过我还没有用过这个参数。你的服务如果不属于任何启动顺序组，只要传递NULL或者一个空的字符串就行了。

lpdwTagId是应用了上面的参数之后要指定的值，专用于驱动程序，与本文内容无关。传递NULL。

lpDependencies标示一个字符串数组，用于指明一串服务的名字或者一个启动顺序组。当与一个启动顺序组建立关联的时候，这个参数的含义就是只有你指定的启动顺序组里有至少一个经过对整个组里所有的成员已经全部尝试过启动后，有至少一个成员成功启动，你的服务才能启动。不需要建立依存关系的话，仍是传递NULL或者一个空的字符串。但如果你要指定启动顺序组的话，必须为组名加上SC_GROUP_IDENTIFIER前缀，因为组名和服务名是共享一个命名空间的。

lpServiceStartName是服务的启动账号，如果你设置你的服务的关联类型是SERVICE_WIN32_OWN_PROCESS的话，你需要以DomainName\UserName的格式指定用户名，如果这个账户在你本机的话，用.\UserName就可以指定。如果传递NULL的话，会以本地的系统账户登陆。如果是Win
 NT 4.0或更早的版本的话，如果你指定了SERVICE_WIN32_SHARE_PROCESS，就必须传递.\System指定服务使用本地的系统账户。最后，如果你指定了SERVICE_INTERACTIVE_PROCESS，你必须使服务运行在本机系统账户。看名字就知道了，lpPassword是账户的密码。如果指定系统账户的话，传递NULL。如果账户没有密码的话，传递空字符串。

二、服务框架代码

2.1定义全局函数变量

//定义全局函数变量

void Init();

BOOL IsInstalled();

BOOL Install();

BOOL Uninstall();

void LogEvent(LPCTSTR pszFormat, ...);

void WINAPI ServiceMain();

void WINAPI ServiceStrl(DWORD dwOpcode);

TCHAR szServiceName[] = _T("ServiceTest");

BOOL bInstall;

SERVICE_STATUS_HANDLE hServiceStatus;

SERVICE_STATUS status;

DWORD dwThreadID;

2.2添加Init初始化函数
这里主要是设置服务句柄和状态。

hServiceStatus = NULL;

status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

status.dwCurrentState = SERVICE_STOPPED;

tatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

status.dwWin32ExitCode = 0;

status.dwServiceSpecificExitCode = 0;

status.dwCheckPoint = 0;

status.dwWaitHint = 0;

2.3添加安装和删除服务函数这里主要是用到了五个函数OpenSCManager、CreateService、OpenService、ControlService、DeleteService。OpenSCManager用于打开服务控制管理器；CreateService用于创建服务；OpenService用于打开已有的服务，返回该服务的句柄；ControlService则用于控制已打开的服务状态，这里是让服务停止后才删除；DeleteService用于删除指定服务。

BOOL Install();

{

//这里列出主要的两个函数，其它的可以在代码里找。
//打开服务控制管理器

OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

//创建服务

SC_HANDLE hService = ::CreateService(

hSCM, szServiceName, szServiceName,

SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,

SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,

 szFilePath, NULL, NULL, _T(""), NULL, NULL);

::CloseServiceHandle(hService);

::CloseServiceHandle(hSCM);

}

BOOL Uninstall();

{

//这里列出主要的两个函数，其它的可以在代码里找。
//打开服务控制管理器

OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

//打开服务

OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);

//停止服务

ControlService(hService, SERVICE_CONTROL_STOP, &status);

//删除服务

DeleteService(hService);

…

}

2.4 添加服务主线程函数和控制函数
这里调用RegisterServiceCtrlHandler来注册服务的控制函数，这里要设置status.dwControlsAccepted为SERVICE_ACCEPT_STOP，否则你不能控制这个服务的状态。

void WINAPI ServiceMain()

{

// Register the control request handler

status.dwCurrentState = SERVICE_START_PENDING;

status.dwControlsAccepted = SERVICE_ACCEPT_STOP;//这个要使用，否则你不能控制

//注册服务控制

hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl);

if (hServiceStatus == NULL)

{

LogEvent(_T("Handler not installed"));

return;

}

SetServiceStatus(hServiceStatus, &status);

status.dwWin32ExitCode = S_OK;

status.dwCheckPoint = 0;

status.dwWaitHint = 0;

status.dwCurrentState = SERVICE_RUNNING;

SetServiceStatus(hServiceStatus, &status);

//模拟服务的运行，10后自动退出。应用时将主要任务放于此即可

 int i = 0;

 while (i < 10)

 {

 Sleep(1000);

 i++;

 }

 //

 status.dwCurrentState = SERVICE_STOPPED;

 SetServiceStatus(hServiceStatus, &status);

 LogEvent(_T("Service stopped"));

}

2.5 在主线程函数里注册控制函数和程序执行主体

void WINAPI ServiceMain()

{

 …

 //如上，这里主要是说明这就是程序的执行体
 //模拟服务的运行，10后自动退出。应用时将主要任务放于此即可

 int i = 0;

 while (i < 10)

 {

 Sleep(1000);

 i++;

 }

 …

}

2.6 在main函数里注册添加安装、删除、注册主函数

int APIENTRY WinMain(HINSTANCE hInstance,

HINSTANCE hPrevInstance,

LPSTR lpCmdLine,

int nCmdShow)

{

 Init();

 dwThreadID = ::GetCurrentThreadId();

 SERVICE_TABLE_ENTRY st[] =

 {

 { szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },

 { NULL, NULL }

 };

 if (stricmp(lpCmdLine, "/install") == 0)

 {

 Install();

 }

 else if (stricmp(lpCmdLine, "/uninstall") == 0)

 {

 Uninstall();

 }

 else

 {

 if (!::StartServiceCtrlDispatcher(st))

 {

 LogEvent(_T("Register Service Main Function Error!"));

 }

 }

 return 0;

}

三、注意事项

1、服务调试方法

第一种方法:
在你的service's code中加入产生Debug中断的语句DebugBreak().当程序执行到此时弹出一个DLG框,你可以选择启动Msdev.exe从DebugBreak()语句后,开始跟踪.
从实际使用来说,我觉得这种方法适合于调试个别简单错误,比较初级,而且需要额外修改service程序,所以本人只是在开始学习调试service时使用过几次. 以后只使用第二种方法.
第二种方法:

attaching debugger(Msdev.exe)到一个正在运行的service上. 我觉得这才是专业的方法.
它充分体现的VC的强大调试功能. 再加上
Debug菜单下的Module..., Thread...功能.

Project菜单下Settings->Debug tab->Gategory(Additional Dlls)
不仅可以调试主程序,观察Module, Thread等信息,还可以调试service调用的DLL,而且是真正的源代码级调试. 太cool了. 我以前学习VC时,只会用F5,F10,..., 后来看了这篇文章以及其他相关的一些文章才知道原来VC还可以这么使. 后来不仅调试service程序, 象一些高级HOOK程序,ISAPI程序也可以这样调试.

2、StartServiceCtrlspatcher（）但是却执行失败，1063错误？

解决方法：服务程序根本就不是直接执行的，你应该是在 控制面板|管理工具中的“服务”中找到你写的服务，选启动来开启它。如果要调试，就使用attaching debugger(Msdev.exe)到一个正在运行的service上来调试。

四、相关实例代码

检测服务器运行服务代码：[http://download.csdn.net/detail/byxdaz/5048617](http://download.csdn.net/detail/byxdaz/5048617)

界面系统服务代码：[http://download.csdn.net/detail/hzligong_wangshuai/2103201#comment](http://download.csdn.net/detail/hzligong_wangshuai/2103201#comment)


