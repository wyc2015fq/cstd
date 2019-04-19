# WinCE Service.exe and Device.exe 服务与设备 . - xqhrs232的专栏 - CSDN博客
2013年01月10日 14:22:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：716
原文地址::[http://blog.csdn.net/xiaozhujulia/article/details/6442681](http://blog.csdn.net/xiaozhujulia/article/details/6442681)
服务与设备的比较
本节将讨论有关Windows CE服务模块、服务模块和普通驱动程序的区别以及如何注册、激活/控制/停止一个服务。
Services.exe是一个和Device.exe有同等地位的进程，它的用途是为了弥补Device.exe的不稳定。如果一个设备驱动由于某种原因处于异常状态或者不受设备管理器控制时，若让其继续存留在设备管理器中，则会影响其他正常运行的驱动程序，甚至使系统崩溃，而使用服务这种机制能尽可能地避免这种情况发生，当系统中有类似服务操作失败的信号产生时，系统不会因此而受到影响。和驱动程序一样，服务也提供了能控制其自身运行状态的函数接口集,如启动服务、暂停服务、其他I/O控制。
在Windows CE. NET中操作服务和操作设备驱动的方式十分类似。如果一个设备驱动实际上并不需要和硬件发生交互，也不需要进程间通信的时候，就应当将其实现为一个服务，以提高系统安全性。服务出错，不会引起系统的严重错误。
Services.exe同样可以被配置用来在特定的连接上等待信息。Socket端口接收到数据包时，Services.exe就会将数据分发到相应的服务中去。
有一点需要注意的是，Services.exe只是防止驱动程序因为服务失败而崩溃，但并不保证它不会引起其他服务的错误，因此服务应当认真调试。
在%WINCEROOT%/Public/Servers/Sdk/Samples/Telnetd目录下，Windows CE.NET提供了一个样例程序，里面包含了如何启动、加载和卸载一个服务，同时还包含了如何进行流接口服务的实现方式。
1．如何激活并且控制一个服务
所谓激活服务就是将服务加载到Service.exe的进程空间里来。可以使用以下方法来加载服务并且在加载后进行控制。
使用注册表中built-in键的功能，在系统启动时，通过注册表自举将服务加载；
使用ActivateService函数，通过程序来加载服务；
可以使用CreateFile函数来打开一个服务；
可以使用I/O control、ReadFile、WriteFile、SetFilePointer等函数来操作一个服务；
还可以使用GetServiceHandle来获得服务的句柄。
当用程序来加载一个服务时，代码加载的格式为：
ActivateService(L"TELNETD", 0);
和设备管理器能控制一个正在运行的设备驱动一样，Service.exe也能通过多种途径来控制一个正在运行的服务。
以下是实现控制服务的具体步骤。
（1）使用CreateFile函数，在参数中设置适当的前缀和索引值。这些值应当在注册服务的时候被写入注册表；
（2）向服务发送I/O控制字。利用DeviceIoControl函数，设备管理器重定向到该服务的XXX_IOControl入口。下面的代码段显示了如何获取一个服务的状态：
HANDLE hService = CreateFile(L"TEL0:",0,0,NULL,OPEN_EXISTING,0,NULL);
if(hService != INVALID_HANDLE_VALUE) {
DWORD dwState;
DeviceIoControl(hService, IOCTL_SERVICE_STATUS, NULL, 0,
&dwState, sizeof(DWORD), NULL, NULL);
CloseHandle(hService);
};
（3）如果服务还支持流式接口的话，那么可以通过额外的操作来控制一个服务。如ReadFile、WriteFile、SetFilePointer函数。
还可以使用GetServiceHandle 函数来控制一个服务的状态。需要注意的是，在调用流式接口之前，必须获取服务的有效句柄，否则这些操作不会成功。可以通过GetServiceHandle函数获取服务的有效句柄。
下面是使用GetServiceHandle的一个例子：
HANDLE hService = GetServiceHandle("TEL0:", NULL, NULL);
if(hService != INVALID_HANDLE_VALUE) {
DWORD dwState;
ServiceIoControl(hService, IOCTL_SERVICE_STATUS, NULL, 0,
&dwState, sizeof(DWORD), NULL, NULL);
}
2．Windows CE服务的目的
作为设备管理器的补充
处理不需要直接和系统有交互的请求
增强驱动的可靠性，降低系统崩溃的可能
提供超级服务
Services.exe是作为对Device.exe模块的补充，着重加强了其加载系统服务的可靠性。它将那些和系统内核交互不是很多或者不用直接和内核交互的请求与系统服务分割开来独立处理，这样就降低了内核的处理负担。
当有一个服务发生故障时，及时地将这个服务和系统服务分割开来是保障系统安全运行的重要手段之一。这个特性使得Windows CE中的驱动程序在系统服务发生故障的时候，仍然有较高的安全运行能力，降低系统崩溃风险。例如，很多嵌入式设备在运行之后可能无人监管，因此系统安全性要求比较高，不能象桌面Windows系统那样经常因为设备驱动的错误而崩溃。
所谓的超级服务是指一个所有端口都允许被Services.exe监控的标准服务，即一个服务能监视所有其他的服务。使用超级服务有一个好处就是，即便许多的服务在使用，只要它们是被超级服务监视的，那么这些服务的处理线程都只由超级服务的处理线程承担，即所有服务最终只有一个处理线程。如果不使用超级服务，那么每个服务都要创建一个接受线程。
Services.exe模块是通过注册表加载的。代码如下所示：
[HKEY_LOCAL_MACHINE/init]
"Launch60"="services.exe"
"Depend60"=hex:14,00
3．服务与设备的区别
Device.exe加载驱动用来管理设备
Sevice.exe加载驱动用来管理软件服务
Services.exe象Device.exe一样可以加载多个服务
如果要同时使用Device.exe和Services.exe, Windows CE中32个进程槽就要被占用两个
Services.exe和Device.exe模块被设计用来互相补充。Device.exe即设备管理器用来加载驱动程序，以此来管理外围设备，Services.exe将加载那些不和设备打交道的服务。
Services.exe可以管理多个服务，正如设备管理器可以管理多个设备驱动那样。 Services.exe负责将和系统无关的服务的处理从设备管理器中分割出来。当然，设备管理器可以在Services.exe没有被加载的环境下运行，这样可以节省一个进程槽。但是，不加载Service.exe的后果就是驱动程序运行的安全性下降。
4．如何注册一个服务
使用RegisterService函数注册
RegisterService函数功能和RegisterDevice函数功能类似，后者目前逐渐被ActivateDeviceEx所替代。
Services.exe 也可以通过编程来启动。代码如下所示：
HANDLE hService = RegisterService("TEL",0,"telnetd.dll",0);
这行代码启动了telnet server的服务例程。如果telnet server当前正在TEL0设备上运行，那么这个函数将返回false。如果调用GetlastError函数，将得到ERROR_DEVICE_IN_USE的错误信息。
RegisterService调用后，设备管理器将调用服务的xxx_Init入口，在调用完xxx_Init接口后，服务处于什么样的状态是依据服务各自不同dwContext设置而定的。在Services.h中，为这个参数预设置了以下值：
#define SERVICE_INIT_STARTED 0x00000000
#define SERVICE_INIT_STOPPED 0x00000001
如果dwContext参数中没有设置SERVICE_INIT_STOPPED值，那么服务在被注册后将建立一个线程，并且启动它作为服务的请求接收处理线程。
如果设置了SERVICE_INIT_STOPPED标志，那么代表本服务将接收超级服务的监督管理，自己将不拥有接收线程，而统一有超级服务进行处理。
下面是RegisterService函数的原型：
HANDLE RegisterService(
LPCWSTR lpszType,
DWORD dwIndex,
LPCWSTR lpszLib,
DWORD dwInfo
);
参数解释：
lpszType
指向一个字符串，这个字符串包含了服务的前缀名（由三个字母组成，和流驱动类似）
dwIndex
定义了0～9之间的服务的索引号。
lpszLib
指向一个字符串，该字符串包含了代表服务的DLL文件名。
dwInfo
这是代表在调用服务的xxx_Init 入口时，传入的参数。
返回值
如果返回值是被注册的服务的句柄，则代表操作成功。如果返回空值则代表操作失败。
RegisterService可以将一个未加载的服务加载到系统中，同样也可以为一个系统中已经存在的服务加载第2个实例。
RegisterService将先加载在参数lpszLib中定义的DLL链接库，然后检查该DLL链接库是否导出了xxx_Init、xxx_Deinit、xxx_IOControl接口。如果DLL链接库名出错，将导致系统寻找不到相应的文件，那么函数将返回ERROR_FILE_NOT_FOUND错误；如果DLL链接库中导出的接口集不正确的话，函数将返回ERROR_FILE_NOT_FOUND错误。
如果系统中已经有同类的服务在运行，即前缀名一致，那么就需要用索引值来区分实例。如果索引值也冲突的话，函数将返回ERROR_DEVICE_IN_USE错误。
在成功加载DLL文件后，Services.exe将调用xxx_Init入口，并且将dwInfo中的参数传递给它。如果init是成功的话，函数将xxx_Init的返回值保留，这将是后面xxx_IOControl、xxx_Deinit入口调用时的重要参数。
5．如何停止一个正在运行的服务
使用DeregisterService函数来停止并且卸载一个服务。DeregisterService将在系统中定位一个服务，并且将它标记为无效；然后DeregisterService将阻止任何企图打开该服务的函数。
下面的代码是实现关闭一个服务的例子：
HANDLE hServide = GetServiceHandle(L"TEL0: ",NULL,NULL);
if(hSerice != NULL)
DeregisterService(hService);
DeregisterService 需要使用从RegisterService返回的服务句柄，而不是使用CreateFile 返回的句柄。因此应用程序无法通过文件系统的流式API来获取服务的句柄，只能使用GetServiceHandle。
6．服务中所使用的API
Services.exe实现了如下的接口。
XXX_Close
服务实现这个接口，并且由Services.exe 调用。当应用程序调用CloseHandle或者CloseService来关闭一个服务时，Services.exe会将操作重映射到本接口；
XXX_Deinit
服务实现这个接口，并且由Services.exe 调用。当应用程序调用DeregisterService来取消注册一个服务时，Services.exe会将操作重映射到本接口；
XXX_Init
初始化服务的接口；
XXX_IOControl
向服务传递I/O控制字；
XXX_Read
服务实现这个接口，并且由Services.exe调用。这个接口提供了类似读文件的功能，只能被流式的服务所实现；
XXX_Seek
服务实现这个接口，并且由Services.exe调用。这个接口提供了类似文件定位的功能，只能被流式的服务所实现。
XXX_Write
服务实现这个接口，并且由Services.exe调用。这个接口提供了类似写文件的功能，只能被流式的服务所实现。
