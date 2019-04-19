# wince 驱动编写的八部曲 - xqhrs232的专栏 - CSDN博客
2014年12月02日 14:46:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：610
原文地址::[http://blog.csdn.net/baiduluckyboy/article/details/5978786](http://blog.csdn.net/baiduluckyboy/article/details/5978786)
相关文章
1、将驱动移植到Windows Embedded CE 6.0----[http://www.docin.com/p-73369092.html](http://www.docin.com/p-73369092.html)
2、[_WIN32_WCE有什么用](http://blog.csdn.net/thanklife/article/details/21477561)----[http://blog.csdn.net/thanklife/article/details/21477561](http://blog.csdn.net/thanklife/article/details/21477561)
3、[外国wince学习论坛收集](http://blog.csdn.net/baiduluckyboy/article/details/5978779)----[http://blog.csdn.net/baiduluckyboy/article/details/5978779](http://blog.csdn.net/baiduluckyboy/article/details/5978779)
4、[推荐国外的一些ARM开发网站](http://blog.csdn.net/baiduluckyboy/article/details/5978876)----[http://blog.csdn.net/baiduluckyboy/article/details/5978876](http://blog.csdn.net/baiduluckyboy/article/details/5978876)
本文将为读者详细介绍驱动程序的实现方法。我们首先介绍驱动程序的任务，并继续探讨缓冲区的处理、内存地址重建及调试域等。
**1.驱动程序的任务**
实际上，驱动程序的实现方法有多种，具体采用哪种，在很大程度上要取决于设备的类型以及有关要求。举例来说，开发工具附带的网卡调试驱动程序大多数都工作在轮询模式下，但是对于常规网络驱动程序来说，通常这是不能接受的。
下面我们考察一个利用中断的驱动程序的实现。在Windows Embedded CE系统中，中断是由中断服务例程(ISR)和中断服务线程(IST)进行处理的。ISR例程属于OAL层的组成部分。此外，如果支持被归入OAL层，它们就可以在执行期间(安装型ISR例程—IISR)安装。ISR例程的主任务包括确定中断源、屏蔽中断以及返回逻辑系统中断(SYSINTR)标识符。IST则是一个跑腿的，大部分的中断处理工作都是它完成的。它生成一个事件，在内核中将该事件注册为某种逻辑中断，然后就开始等待该事件的出现。当事件发生时，它会对其进行所有必要的处理工作。如果一个驱动程序使用了安装型ISR，那么IST就会加载这个安装型例程;如果一个驱动程序使用多个线程实现，那么当一个线程在等待和处理一个事件时，同时还可以在另一个线程中完成ISR的创建和安装过程。驱动程序的任务包括以下内容：
确定中断的系统标识符：
　　·可以直接在驱动程序中规定。
　　·可以利用DDKReg_GetIsrInfo()函数从注册表项中获得。
　　·可以利用IRQ–IOCTL_HAL_REQUEST_SYSINTR 向OAL层发送请求。
建立一个事件(CreateEvent())。
在内核中为该事件注册一个指定的中断的系统标识符(InterruptInitialize())。
利用WaitForSingleObject()等待一个事件。
事件发生后，对其进行相应的处理。
处理结束后，调用InterruptDone()。
如果驱动程序使用了安装型ISR例程，那么它还要完成下列任务：
确定ISR例程的设置(名称、入口点及其他参数)：
　　·可以直接在驱动程序内部指定。
　　·可以利用DDKReg_GetIsrInfo()函数从注册表项中获得。
为特定IRQ请求装入一个已安装的IISR过程(LoadIntChainHandler())。
配置IISR过程(KernelLibIoControl())。
结束后，它会调用FreeintChainHandler()函数，该函数的作用是，当处理某个中断请求(IRQ)时，将在OAL层中调用的、已安装过程链中除去已安装的IISR过程。它会把这些库代码装载到内存中。
**2.安装型ISR例程**
安装型ISR例程是作为一个动态加载库来实现的，这个库必须满足以下要求：
　　·实现代码必须全部位于该库内部;不能存在显式的依赖性。
　  · 不存在隐式的依赖性(NOMUPS16CODE = 1)。
　　·不能使用C运行时库(NOLIBC = 1)。
**3.通用安装型服务程序(GIISR)**
通用安装型服务程序(GIISR)是一个已安装好的过程，用来处理各种常见的中断，随它一起提供的还有许多开发工具。GIISR为我们提供了源代码，位于/ Public/Common/Oak/Drivers/GIISR/，它适用于大多数的情况，并且会读取寄存器/端口以确定中断的状态。我们可以使用KernelLibIoControl配置GIISR过程，如下所示：
　　·寄存器地址/端口地址。
　　·寄存器长度/端口长度。
　　·功能部件、内存或者输入/输出寄存器或者端口。
　　·屏蔽。
**4.与缓冲区有关概念**
驱动程序开发的一个重要部分是从调用代码处理需要传递的缓冲区，下面是一些有关的概念。
　　·访问检验：对发起调用的进程进行检查，以确保它具备访问指定缓冲区的足够权限。
　　·指针参数：一个指针，将作为参数传给一个API函数。
　　·嵌入指针：指包含在数据结构或者缓冲区内部作为参数传递给API函数的指针。
　　·安全副本：已传送缓冲区数据的一个本地副本。
　　·内存地址重建或映射：通常也应用于指针，为将指针用于另一进程而做好准备。
　　·同步访问：当函数调用时, 在被调用线程中访问内存。
**5.内存地址重建**
当应用程序需要调用驱动程序实现的某些功能时，通常需要向驱动程序传递某些必要的信息。这时，可以通过共享内存向驱动程序传递参数，例如利用共享堆或者内存映像文件等。 在大多数情况下，驱动程序的功能都可以通过API调用来加以访问。
这可能带来两个问题：首先，API参数位于用户内存进程空间中，而驱动程序则位于内核内存空间(对于内核模式驱动程序)或者另一个用户进程(对于用户模式驱动程序而言)中。其次，调用方必须具备足够的权限才能访问被传递的缓冲区。因此，在驱动程序开发期间，您必须检查对要传递的缓冲区的访问权，并为驱动程序提供访问调用方的缓冲区数据所需权限。下图给出了内存地址重建示例。
![](http://writeblog.csdn.net/uploads/allimg/101014/2141113009-0.jpg)
图1 Abc.exe内存地址重建示例
在这个例子中，Abc.exe将通过两个参数来调用一个Driver.dll函数。第一个参数是指针参数，第二个是带有嵌入指针的结构体。如果Driver.dll函数是从调用方的线程中调用的，那么可以在调用期间直接从Driver.dll函数中访问Abc.exe的内存空间。同步调用时，直接访问需要检查访问权限，不过无需进行内存地址重建。
如果需要异步访问缓冲区的话，有两种选择：第一种是将缓冲区拷贝至驱动程序内存，第二种方法是把要传递的缓冲区设为其物理内存的别名。
内存地址重建的类型有多种，如下：
　　·直接访问。
调用进程的缓冲区在调用期间可以直接访问。
这仅适用于内核模式驱动程序的同步访问。
　　·复制。
将需要传递的缓冲区复制到驱动程序的工作缓冲区。
一个驱动程序使用一份副本。 如果需要，还可以反向复制。
　　·使用别名。
在驱动程序内创建一个新缓冲区，而该驱动程序与需要传递的缓冲区恰好位于同一物理内存区。
缓冲区的所有改变在调用进程中都是可访问的。
内核程序能确定最合适的内存地址重建方法，它会根据对缓冲区的同步或者异步访问，来确定出内存地址重建所需的API。
对于同步访问，内核程序会自动转换指针参数，所以开发人员必须通过调用CeOpenCallerBufer()来手动映射嵌入指针，验证访问权限，并执行内存地址重建，最后调用CeCloseCallerBuffer()。
对于异步访问，转换过程较之于同步访问还需为调用CeAllocAsynchronousBuffer()函数进行异步访问准备好所有的指针，并且最后要调用CeFreeAsynchronousBuffer()函数。
用户模式驱动程序的内存地址重建有以下限制：对于异步访问来说，指针参数必须以只读方式访问，而不可以写方式访问。尽管可以手工方式完成内部指针的内存地址重建，但是从内核程序调用一个驱动程序时，有可能收到无法从用户模式驱动程序访问的指针。
因此，更有效的方式是使用一个平滑的缓冲区存放用户模式驱动程序的所有数据，并且不异步访问。
下面介绍检查调用方缓冲区的访问权限和内存地址重建所需的系统API。对于同步访问，指针参数不需要额外的API调用;嵌入指针可以使用CeOpenCallerBufer()和CeCloseCallerBuffer () 。对于异步访问，当指针参数使用CeAllocAsynchronousBuffer ()时，嵌入指针可以使用CeOpenCallerBufer()和CeAllocAsynchronousBuffer();当指针参数使用CeFreeAsynchronousBuffer()时，嵌入指针可以使用CeFreeAsynchronousBuffer()
 和CeCloseCallerBuffer() 。
**6.安全复制**
向驱动程序传递数据会导致额外的风险——可能改变已经过驱动程序验证的、API执行期间的指针及或它们指向的数据。为了防止这些类型的问题，可以使用安全副本方法，它会给驱动程序的数据创建一个单独的副本。创建安全副本时，被传递的缓冲区数据会拷贝至驱动程序的内部缓冲区中。这种方式适合于下列情形：
　　·用于所有嵌入指针。
　　·用于所有使用之前需验证的参数。
注意，使用安全副本在一定程度上会影响效率。创建安全副本时，可以使用下列方法：
　　·手动方式。
　　·利用CeOpenCallerBuffer()函数，并将嵌入指针的参数ForceDuplicate设为TRUE。
　　·为指针参数调用CeAllocDuplicateBuffer()。
**7.内存地址重建API函数和类**
下面我们介绍内存地址重建API函数。
CeOpenCallerBuffer ：验证访问权限并完成指针的内存地址重建。返回一个内存地址重建后的指针。分配资源。为了在指针处理结束后释放资源，必须调用CeCloseCallerBuffer函数。
CeCloseCallerBuffer：释放CeOpenCallerBuffer函数分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
CeAllocAsynchronousBuffer：使用CeOpenCallerBuffer函数或者由系统整理出一个缓冲区，以供异步访问之用。在返回发起调用的线程之前，必须同步调用这个函数。它会分配有关资源，为了在指针处理结束后释放资源，必须调用CeFreeAsynchronousBuffer函数。
CeFreeAsynchronousBuffer：会释放由函数CeAllocAsynchronousBuffer分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
CeFlushAsynchronousBuffer：会根据函数CeAllocAsynchronousBuffer对该缓冲区所做的修改而改变源缓冲区。
CeAllocDuplicateBuffer：为参数指针创建安全副本。
CeFreeDuplicateBuffer：释放由函数CeAllocDuplicateBuffer分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
除此之外，Windows Embedded CE 6.0还另外提供了一组C++类，用于内存地址重建，它们位于/PUBLIC/COMMON/OAK/INC/MARSHAL.HPP，下面对它们做相应说明。
AsynchronousBuffer_t：CeAllocAsynchronousBuffer函数和CeFreeAsynchronousBuffer函数的包装类，用于需要异步访问的内存地址重建后的指针。
DuplicatedBuffer_t：CeAllocDuplicateBuffer函数和CeFreeDuplicateBuffer函数的包装类，用于指针参数。
MarshalledBuffer_t：CeOpenCallerBuffer、CeCloseCallerBuffer、CeAllocAsynchronousBuffer和CeFreeAsynchronousBuffer函数的包装类，用于未重建内存地址的嵌入指针。
**8.调试域**
要想开发出可靠和稳定的驱动程序，必须在可能引起异常错误的可执行码中插入__try/__except/__finally块，特别是那些从外部接收数据的代码。
调试是驱动程序开发工作的一大项，幸运的是Windows Embedded CE开发工具提供了调试驱动程序所需的全部功能。Windows Embedded CE提供了两种调试驱动程序的方法，第一种标准的单步调试方式，它能够进入内核提供的代码;第二种是无间断的调试方式。注意，要使用标准内核调试工具，应该为特定的硬件平台实现KITL，并选择传输方法。
如果希望调试系统的所有部分，您应当构建一个Debug OS映像。如果希望调试整个驱动程序，那么只需构建一个包括内核程序调试工具KITL和调试版的带有辅助调试文件的驱动程序的Retail OS映像就足够了。如果希望调试整个驱动程序和部分系统的话，您应当在映像中包含前面所说的各种组件，以及带有辅助调试文件的系统部分。
调试域是一种改进型的printf调试技术，并且能在运行时配置输出，还能与Platfrom Builder集成使用。从最基础的角度而言，调试域向调试输出发送条件输出。按照这种方式，调试域可以在不需要中断执行的情况下提供驱动程序执行信息。
所有的系统码都可用于调试域，所以您不仅能够看到来自驱动程序调试域的输出，而且还能得到有关系统活动信息。这一功能有助于在驱动程序开发期间暴露并解决问题。
为了使用调试域，需要进行如下所示的工作：
·在驱动程序的头文件中包含dbgapi.h：#include
·为调试域定义掩码，例如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->//zone 0
#define ZONEMASK_INIT (0x00000001<<0)
//zone 1
#define ZONEMASK_ACTIONS (0x00000001<<1)
//zone 2
#define ZONEMASK_EXCEPTIONS (0x00000001<<2)
//zone 14
#define ZONEMASK_WARNING (0x00000001<<14)
//zone 15
#define ZONEMASK_ERRORS (0x00000001<<16)
定义用于条件式调试域输出的标志，例如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->//如果zone 0启用，则为true
#define ZONE_INIT DEBUGZONE (0)
//如果zone 1启用，则为true
#define ZONE_ACTIONS DEBUGZONE (1)
//如果zone 2启用，则为true
#define ZONE_EXCEPTIONS DEBUGZONE (2)
//如果zone 14启用，则为true
#define ZONE_WARNING DEBUGZONE (14)
//如果zone 15启用，则为true
#define ZONE_ERRORSDEBUGZONE (15)
定义参数dpCurSettings，如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->　　DBGPARAM dpCurSettings = {
//通常为模块名称
　　TEXT("MyDriver"),
　　{//16个域的名称
　　TEXT("Init"),TEXT("Actions"),TEXT("Exceptions"),TEXT(""),
　　TEXT(""),TEXT(""),TEXT(""),TEXT(""),
　　TEXT(""),TEXT(""),TEXT(""),TEXT(""),
　　TEXT(""),TEXT(""),TEXT("Warnings"), TEXT("Errors")
　　},
//由系统默认为启用状态的域
　　ZONEMASK_ERRORS| ZONEMASK_EXCEPTIONS|ZONEMASK_INIT
　　};
使用相应的宏注册调试域，如下所示：
DEBUGREGISTER()：用于Debug构建，如果用于构建.exe程序的话，则以NULL作为参数。如果要构建为.dll程序的话，以handle作为其参数。
RETAILREGISTERZONES()：用于Retail构建和Debug构建，如果用于构建.exe程序的话，则以NULL作为参数。如果要构建为.dll程序的话，以handle作为其参数。
您可以在驱动程序代码中包含所需的宏，具体参见下文。
建立合适的OS构建(Debug或者Retail)。
将映像装入设备。
使用Platform Builder配置活动的调试域。
下面我们介绍用于调试域的宏，如下所示：
　　RETAILMSG ( , )：有条件地输出一个printf风格的格式化消息。
　　RETAILLED (, )：有条件地将WORD的值输出至LED。
　　ERRORMSG( , )：有条件地输出一个printf风格的格式化ERROR消息，并带有错误所在文件名和行号。
　　DEBUGMSG ( , )：有条件地输出一个printf风格的格式化消息。
　　DEBUGLED (, )：有条件地将WORD的值输出至LED。
　　DEBUGCHK( )：为表达式建立一个断言，并且在表达式为FALSE的时候生成一个DebugBreak。
　　DEBUGZONE()：测试当前调试域设置中的掩码位。
　　DEBUGREGISTER()：仅在Debug构建时注册调试域。
　　RETAILREGISTERZONES()：在Debug构建和Retail构建时都注册调试域。
**9.小结**
本文为读者详细介绍驱动程序的实现方法，内容包括驱动程序的任务、缓冲区的处理、内存地址重建及调试域等。希望本文对您有所帮助。
本文将为读者详细介绍驱动程序的实现方法。我们首先介绍驱动程序的任务，并继续探讨缓冲区的处理、内存地址重建及调试域等。
**1.驱动程序的任务**
实际上，驱动程序的实现方法有多种，具体采用哪种，在很大程度上要取决于设备的类型以及有关要求。举例来说，开发工具附带的网卡调试驱动程序大多数都工作在轮询模式下，但是对于常规网络驱动程序来说，通常这是不能接受的。
下面我们考察一个利用中断的驱动程序的实现。在Windows Embedded CE系统中，中断是由中断服务例程(ISR)和中断服务线程(IST)进行处理的。ISR例程属于OAL层的组成部分。此外，如果支持被归入OAL层，它们就可以在执行期间(安装型ISR例程—IISR)安装。ISR例程的主任务包括确定中断源、屏蔽中断以及返回逻辑系统中断(SYSINTR)标识符。IST则是一个跑腿的，大部分的中断处理工作都是它完成的。它生成一个事件，在内核中将该事件注册为某种逻辑中断，然后就开始等待该事件的出现。当事件发生时，它会对其进行所有必要的处理工作。如果一个驱动程序使用了安装型ISR，那么IST就会加载这个安装型例程;如果一个驱动程序使用多个线程实现，那么当一个线程在等待和处理一个事件时，同时还可以在另一个线程中完成ISR的创建和安装过程。驱动程序的任务包括以下内容：
确定中断的系统标识符：
　　·可以直接在驱动程序中规定。
　　·可以利用DDKReg_GetIsrInfo()函数从注册表项中获得。
　　·可以利用IRQ–IOCTL_HAL_REQUEST_SYSINTR 向OAL层发送请求。
建立一个事件(CreateEvent())。
在内核中为该事件注册一个指定的中断的系统标识符(InterruptInitialize())。
利用WaitForSingleObject()等待一个事件。
事件发生后，对其进行相应的处理。
处理结束后，调用InterruptDone()。
如果驱动程序使用了安装型ISR例程，那么它还要完成下列任务：
确定ISR例程的设置(名称、入口点及其他参数)：
　　·可以直接在驱动程序内部指定。
　　·可以利用DDKReg_GetIsrInfo()函数从注册表项中获得。
为特定IRQ请求装入一个已安装的IISR过程(LoadIntChainHandler())。
配置IISR过程(KernelLibIoControl())。
结束后，它会调用FreeintChainHandler()函数，该函数的作用是，当处理某个中断请求(IRQ)时，将在OAL层中调用的、已安装过程链中除去已安装的IISR过程。它会把这些库代码装载到内存中。
**2.安装型ISR例程**
安装型ISR例程是作为一个动态加载库来实现的，这个库必须满足以下要求：
　　·实现代码必须全部位于该库内部;不能存在显式的依赖性。
　  · 不存在隐式的依赖性(NOMUPS16CODE = 1)。
　　·不能使用C运行时库(NOLIBC = 1)。
**3.通用安装型服务程序(GIISR)**
通用安装型服务程序(GIISR)是一个已安装好的过程，用来处理各种常见的中断，随它一起提供的还有许多开发工具。GIISR为我们提供了源代码，位于/ Public/Common/Oak/Drivers/GIISR/，它适用于大多数的情况，并且会读取寄存器/端口以确定中断的状态。我们可以使用KernelLibIoControl配置GIISR过程，如下所示：
　　·寄存器地址/端口地址。
　　·寄存器长度/端口长度。
　　·功能部件、内存或者输入/输出寄存器或者端口。
　　·屏蔽。
**4.与缓冲区有关概念**
驱动程序开发的一个重要部分是从调用代码处理需要传递的缓冲区，下面是一些有关的概念。
　　·访问检验：对发起调用的进程进行检查，以确保它具备访问指定缓冲区的足够权限。
　　·指针参数：一个指针，将作为参数传给一个API函数。
　　·嵌入指针：指包含在数据结构或者缓冲区内部作为参数传递给API函数的指针。
　　·安全副本：已传送缓冲区数据的一个本地副本。
　　·内存地址重建或映射：通常也应用于指针，为将指针用于另一进程而做好准备。
　　·同步访问：当函数调用时, 在被调用线程中访问内存。
**5.内存地址重建**
当应用程序需要调用驱动程序实现的某些功能时，通常需要向驱动程序传递某些必要的信息。这时，可以通过共享内存向驱动程序传递参数，例如利用共享堆或者内存映像文件等。 在大多数情况下，驱动程序的功能都可以通过API调用来加以访问。
这可能带来两个问题：首先，API参数位于用户内存进程空间中，而驱动程序则位于内核内存空间(对于内核模式驱动程序)或者另一个用户进程(对于用户模式驱动程序而言)中。其次，调用方必须具备足够的权限才能访问被传递的缓冲区。因此，在驱动程序开发期间，您必须检查对要传递的缓冲区的访问权，并为驱动程序提供访问调用方的缓冲区数据所需权限。下图给出了内存地址重建示例。
![](http://writeblog.csdn.net/uploads/allimg/101014/2141113009-0.jpg)
图1 Abc.exe内存地址重建示例
在这个例子中，Abc.exe将通过两个参数来调用一个Driver.dll函数。第一个参数是指针参数，第二个是带有嵌入指针的结构体。如果Driver.dll函数是从调用方的线程中调用的，那么可以在调用期间直接从Driver.dll函数中访问Abc.exe的内存空间。同步调用时，直接访问需要检查访问权限，不过无需进行内存地址重建。
如果需要异步访问缓冲区的话，有两种选择：第一种是将缓冲区拷贝至驱动程序内存，第二种方法是把要传递的缓冲区设为其物理内存的别名。
内存地址重建的类型有多种，如下：
　　·直接访问。
调用进程的缓冲区在调用期间可以直接访问。
这仅适用于内核模式驱动程序的同步访问。
　　·复制。
将需要传递的缓冲区复制到驱动程序的工作缓冲区。
一个驱动程序使用一份副本。 如果需要，还可以反向复制。
　　·使用别名。
在驱动程序内创建一个新缓冲区，而该驱动程序与需要传递的缓冲区恰好位于同一物理内存区。
缓冲区的所有改变在调用进程中都是可访问的。
内核程序能确定最合适的内存地址重建方法，它会根据对缓冲区的同步或者异步访问，来确定出内存地址重建所需的API。
对于同步访问，内核程序会自动转换指针参数，所以开发人员必须通过调用CeOpenCallerBufer()来手动映射嵌入指针，验证访问权限，并执行内存地址重建，最后调用CeCloseCallerBuffer()。
对于异步访问，转换过程较之于同步访问还需为调用CeAllocAsynchronousBuffer()函数进行异步访问准备好所有的指针，并且最后要调用CeFreeAsynchronousBuffer()函数。
用户模式驱动程序的内存地址重建有以下限制：对于异步访问来说，指针参数必须以只读方式访问，而不可以写方式访问。尽管可以手工方式完成内部指针的内存地址重建，但是从内核程序调用一个驱动程序时，有可能收到无法从用户模式驱动程序访问的指针。
因此，更有效的方式是使用一个平滑的缓冲区存放用户模式驱动程序的所有数据，并且不异步访问。
下面介绍检查调用方缓冲区的访问权限和内存地址重建所需的系统API。对于同步访问，指针参数不需要额外的API调用;嵌入指针可以使用CeOpenCallerBufer()和CeCloseCallerBuffer () 。对于异步访问，当指针参数使用CeAllocAsynchronousBuffer ()时，嵌入指针可以使用CeOpenCallerBufer()和CeAllocAsynchronousBuffer();当指针参数使用CeFreeAsynchronousBuffer()时，嵌入指针可以使用CeFreeAsynchronousBuffer()
 和CeCloseCallerBuffer() 。
**6.安全复制**
向驱动程序传递数据会导致额外的风险——可能改变已经过驱动程序验证的、API执行期间的指针及或它们指向的数据。为了防止这些类型的问题，可以使用安全副本方法，它会给驱动程序的数据创建一个单独的副本。创建安全副本时，被传递的缓冲区数据会拷贝至驱动程序的内部缓冲区中。这种方式适合于下列情形：
　　·用于所有嵌入指针。
　　·用于所有使用之前需验证的参数。
注意，使用安全副本在一定程度上会影响效率。创建安全副本时，可以使用下列方法：
　　·手动方式。
　　·利用CeOpenCallerBuffer()函数，并将嵌入指针的参数ForceDuplicate设为TRUE。
　　·为指针参数调用CeAllocDuplicateBuffer()。
**7.内存地址重建API函数和类**
下面我们介绍内存地址重建API函数。
CeOpenCallerBuffer ：验证访问权限并完成指针的内存地址重建。返回一个内存地址重建后的指针。分配资源。为了在指针处理结束后释放资源，必须调用CeCloseCallerBuffer函数。
CeCloseCallerBuffer：释放CeOpenCallerBuffer函数分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
CeAllocAsynchronousBuffer：使用CeOpenCallerBuffer函数或者由系统整理出一个缓冲区，以供异步访问之用。在返回发起调用的线程之前，必须同步调用这个函数。它会分配有关资源，为了在指针处理结束后释放资源，必须调用CeFreeAsynchronousBuffer函数。
CeFreeAsynchronousBuffer：会释放由函数CeAllocAsynchronousBuffer分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
CeFlushAsynchronousBuffer：会根据函数CeAllocAsynchronousBuffer对该缓冲区所做的修改而改变源缓冲区。
CeAllocDuplicateBuffer：为参数指针创建安全副本。
CeFreeDuplicateBuffer：释放由函数CeAllocDuplicateBuffer分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
除此之外，Windows Embedded CE 6.0还另外提供了一组C++类，用于内存地址重建，它们位于/PUBLIC/COMMON/OAK/INC/MARSHAL.HPP，下面对它们做相应说明。
AsynchronousBuffer_t：CeAllocAsynchronousBuffer函数和CeFreeAsynchronousBuffer函数的包装类，用于需要异步访问的内存地址重建后的指针。
DuplicatedBuffer_t：CeAllocDuplicateBuffer函数和CeFreeDuplicateBuffer函数的包装类，用于指针参数。
MarshalledBuffer_t：CeOpenCallerBuffer、CeCloseCallerBuffer、CeAllocAsynchronousBuffer和CeFreeAsynchronousBuffer函数的包装类，用于未重建内存地址的嵌入指针。
**8.调试域**
要想开发出可靠和稳定的驱动程序，必须在可能引起异常错误的可执行码中插入__try/__except/__finally块，特别是那些从外部接收数据的代码。
调试是驱动程序开发工作的一大项，幸运的是Windows Embedded CE开发工具提供了调试驱动程序所需的全部功能。Windows Embedded CE提供了两种调试驱动程序的方法，第一种标准的单步调试方式，它能够进入内核提供的代码;第二种是无间断的调试方式。注意，要使用标准内核调试工具，应该为特定的硬件平台实现KITL，并选择传输方法。
如果希望调试系统的所有部分，您应当构建一个Debug OS映像。如果希望调试整个驱动程序，那么只需构建一个包括内核程序调试工具KITL和调试版的带有辅助调试文件的驱动程序的Retail OS映像就足够了。如果希望调试整个驱动程序和部分系统的话，您应当在映像中包含前面所说的各种组件，以及带有辅助调试文件的系统部分。
调试域是一种改进型的printf调试技术，并且能在运行时配置输出，还能与Platfrom Builder集成使用。从最基础的角度而言，调试域向调试输出发送条件输出。按照这种方式，调试域可以在不需要中断执行的情况下提供驱动程序执行信息。
所有的系统码都可用于调试域，所以您不仅能够看到来自驱动程序调试域的输出，而且还能得到有关系统活动信息。这一功能有助于在驱动程序开发期间暴露并解决问题。
为了使用调试域，需要进行如下所示的工作：
·在驱动程序的头文件中包含dbgapi.h：#include
·为调试域定义掩码，例如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->//zone 0
#define ZONEMASK_INIT (0x00000001<<0)
//zone 1
#define ZONEMASK_ACTIONS (0x00000001<<1)
//zone 2
#define ZONEMASK_EXCEPTIONS (0x00000001<<2)
//zone 14
#define ZONEMASK_WARNING (0x00000001<<14)
//zone 15
#define ZONEMASK_ERRORS (0x00000001<<16)
定义用于条件式调试域输出的标志，例如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->//如果zone 0启用，则为true
#define ZONE_INIT DEBUGZONE (0)
//如果zone 1启用，则为true
#define ZONE_ACTIONS DEBUGZONE (1)
//如果zone 2启用，则为true
#define ZONE_EXCEPTIONS DEBUGZONE (2)
//如果zone 14启用，则为true
#define ZONE_WARNING DEBUGZONE (14)
//如果zone 15启用，则为true
#define ZONE_ERRORSDEBUGZONE (15)
定义参数dpCurSettings，如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->　　DBGPARAM dpCurSettings = {
//通常为模块名称
　　TEXT("MyDriver"),
　　{//16个域的名称
　　TEXT("Init"),TEXT("Actions"),TEXT("Exceptions"),TEXT(""),
　　TEXT(""),TEXT(""),TEXT(""),TEXT(""),
　　TEXT(""),TEXT(""),TEXT(""),TEXT(""),
　　TEXT(""),TEXT(""),TEXT("Warnings"), TEXT("Errors")
　　},
//由系统默认为启用状态的域
　　ZONEMASK_ERRORS| ZONEMASK_EXCEPTIONS|ZONEMASK_INIT
　　};
使用相应的宏注册调试域，如下所示：
DEBUGREGISTER()：用于Debug构建，如果用于构建.exe程序的话，则以NULL作为参数。如果要构建为.dll程序的话，以handle作为其参数。
RETAILREGISTERZONES()：用于Retail构建和Debug构建，如果用于构建.exe程序的话，则以NULL作为参数。如果要构建为.dll程序的话，以handle作为其参数。
您可以在驱动程序代码中包含所需的宏，具体参见下文。
建立合适的OS构建(Debug或者Retail)。
将映像装入设备。
使用Platform Builder配置活动的调试域。
下面我们介绍用于调试域的宏，如下所示：
　　RETAILMSG ( , )：有条件地输出一个printf风格的格式化消息。
　　RETAILLED (, )：有条件地将WORD的值输出至LED。
　　ERRORMSG( , )：有条件地输出一个printf风格的格式化ERROR消息，并带有错误所在文件名和行号。
　　DEBUGMSG ( , )：有条件地输出一个printf风格的格式化消息。
　　DEBUGLED (, )：有条件地将WORD的值输出至LED。
　　DEBUGCHK( )：为表达式建立一个断言，并且在表达式为FALSE的时候生成一个DebugBreak。
　　DEBUGZONE()：测试当前调试域设置中的掩码位。
　　DEBUGREGISTER()：仅在Debug构建时注册调试域。
　　RETAILREGISTERZONES()：在Debug构建和Retail构建时都注册调试域。
**9.小结**
本文为读者详细介绍驱动程序的实现方法，内容包括驱动程序的任务、缓冲区的处理、内存地址重建及调试域等。希望本文对您有所帮助。
本文将为读者详细介绍驱动程序的实现方法。我们首先介绍驱动程序的任务，并继续探讨缓冲区的处理、内存地址重建及调试域等。
**1.驱动程序的任务**
实际上，驱动程序的实现方法有多种，具体采用哪种，在很大程度上要取决于设备的类型以及有关要求。举例来说，开发工具附带的网卡调试驱动程序大多数都工作在轮询模式下，但是对于常规网络驱动程序来说，通常这是不能接受的。
下面我们考察一个利用中断的驱动程序的实现。在Windows Embedded CE系统中，中断是由中断服务例程(ISR)和中断服务线程(IST)进行处理的。ISR例程属于OAL层的组成部分。此外，如果支持被归入OAL层，它们就可以在执行期间(安装型ISR例程—IISR)安装。ISR例程的主任务包括确定中断源、屏蔽中断以及返回逻辑系统中断(SYSINTR)标识符。IST则是一个跑腿的，大部分的中断处理工作都是它完成的。它生成一个事件，在内核中将该事件注册为某种逻辑中断，然后就开始等待该事件的出现。当事件发生时，它会对其进行所有必要的处理工作。如果一个驱动程序使用了安装型ISR，那么IST就会加载这个安装型例程;如果一个驱动程序使用多个线程实现，那么当一个线程在等待和处理一个事件时，同时还可以在另一个线程中完成ISR的创建和安装过程。驱动程序的任务包括以下内容：
确定中断的系统标识符：
　　·可以直接在驱动程序中规定。
　　·可以利用DDKReg_GetIsrInfo()函数从注册表项中获得。
　　·可以利用IRQ–IOCTL_HAL_REQUEST_SYSINTR 向OAL层发送请求。
建立一个事件(CreateEvent())。
在内核中为该事件注册一个指定的中断的系统标识符(InterruptInitialize())。
利用WaitForSingleObject()等待一个事件。
事件发生后，对其进行相应的处理。
处理结束后，调用InterruptDone()。
如果驱动程序使用了安装型ISR例程，那么它还要完成下列任务：
确定ISR例程的设置(名称、入口点及其他参数)：
　　·可以直接在驱动程序内部指定。
　　·可以利用DDKReg_GetIsrInfo()函数从注册表项中获得。
为特定IRQ请求装入一个已安装的IISR过程(LoadIntChainHandler())。
配置IISR过程(KernelLibIoControl())。
结束后，它会调用FreeintChainHandler()函数，该函数的作用是，当处理某个中断请求(IRQ)时，将在OAL层中调用的、已安装过程链中除去已安装的IISR过程。它会把这些库代码装载到内存中。
**2.安装型ISR例程**
安装型ISR例程是作为一个动态加载库来实现的，这个库必须满足以下要求：
　　·实现代码必须全部位于该库内部;不能存在显式的依赖性。
　  · 不存在隐式的依赖性(NOMUPS16CODE = 1)。
　　·不能使用C运行时库(NOLIBC = 1)。
**3.通用安装型服务程序(GIISR)**
通用安装型服务程序(GIISR)是一个已安装好的过程，用来处理各种常见的中断，随它一起提供的还有许多开发工具。GIISR为我们提供了源代码，位于/ Public/Common/Oak/Drivers/GIISR/，它适用于大多数的情况，并且会读取寄存器/端口以确定中断的状态。我们可以使用KernelLibIoControl配置GIISR过程，如下所示：
　　·寄存器地址/端口地址。
　　·寄存器长度/端口长度。
　　·功能部件、内存或者输入/输出寄存器或者端口。
　　·屏蔽。
**4.与缓冲区有关概念**
驱动程序开发的一个重要部分是从调用代码处理需要传递的缓冲区，下面是一些有关的概念。
　　·访问检验：对发起调用的进程进行检查，以确保它具备访问指定缓冲区的足够权限。
　　·指针参数：一个指针，将作为参数传给一个API函数。
　　·嵌入指针：指包含在数据结构或者缓冲区内部作为参数传递给API函数的指针。
　　·安全副本：已传送缓冲区数据的一个本地副本。
　　·内存地址重建或映射：通常也应用于指针，为将指针用于另一进程而做好准备。
　　·同步访问：当函数调用时, 在被调用线程中访问内存。
**5.内存地址重建**
当应用程序需要调用驱动程序实现的某些功能时，通常需要向驱动程序传递某些必要的信息。这时，可以通过共享内存向驱动程序传递参数，例如利用共享堆或者内存映像文件等。 在大多数情况下，驱动程序的功能都可以通过API调用来加以访问。
这可能带来两个问题：首先，API参数位于用户内存进程空间中，而驱动程序则位于内核内存空间(对于内核模式驱动程序)或者另一个用户进程(对于用户模式驱动程序而言)中。其次，调用方必须具备足够的权限才能访问被传递的缓冲区。因此，在驱动程序开发期间，您必须检查对要传递的缓冲区的访问权，并为驱动程序提供访问调用方的缓冲区数据所需权限。下图给出了内存地址重建示例。
![](http://writeblog.csdn.net/uploads/allimg/101014/2141113009-0.jpg)
图1 Abc.exe内存地址重建示例
在这个例子中，Abc.exe将通过两个参数来调用一个Driver.dll函数。第一个参数是指针参数，第二个是带有嵌入指针的结构体。如果Driver.dll函数是从调用方的线程中调用的，那么可以在调用期间直接从Driver.dll函数中访问Abc.exe的内存空间。同步调用时，直接访问需要检查访问权限，不过无需进行内存地址重建。
如果需要异步访问缓冲区的话，有两种选择：第一种是将缓冲区拷贝至驱动程序内存，第二种方法是把要传递的缓冲区设为其物理内存的别名。
内存地址重建的类型有多种，如下：
　　·直接访问。
调用进程的缓冲区在调用期间可以直接访问。
这仅适用于内核模式驱动程序的同步访问。
　　·复制。
将需要传递的缓冲区复制到驱动程序的工作缓冲区。
一个驱动程序使用一份副本。 如果需要，还可以反向复制。
　　·使用别名。
在驱动程序内创建一个新缓冲区，而该驱动程序与需要传递的缓冲区恰好位于同一物理内存区。
缓冲区的所有改变在调用进程中都是可访问的。
内核程序能确定最合适的内存地址重建方法，它会根据对缓冲区的同步或者异步访问，来确定出内存地址重建所需的API。
对于同步访问，内核程序会自动转换指针参数，所以开发人员必须通过调用CeOpenCallerBufer()来手动映射嵌入指针，验证访问权限，并执行内存地址重建，最后调用CeCloseCallerBuffer()。
对于异步访问，转换过程较之于同步访问还需为调用CeAllocAsynchronousBuffer()函数进行异步访问准备好所有的指针，并且最后要调用CeFreeAsynchronousBuffer()函数。
用户模式驱动程序的内存地址重建有以下限制：对于异步访问来说，指针参数必须以只读方式访问，而不可以写方式访问。尽管可以手工方式完成内部指针的内存地址重建，但是从内核程序调用一个驱动程序时，有可能收到无法从用户模式驱动程序访问的指针。
因此，更有效的方式是使用一个平滑的缓冲区存放用户模式驱动程序的所有数据，并且不异步访问。
下面介绍检查调用方缓冲区的访问权限和内存地址重建所需的系统API。对于同步访问，指针参数不需要额外的API调用;嵌入指针可以使用CeOpenCallerBufer()和CeCloseCallerBuffer () 。对于异步访问，当指针参数使用CeAllocAsynchronousBuffer ()时，嵌入指针可以使用CeOpenCallerBufer()和CeAllocAsynchronousBuffer();当指针参数使用CeFreeAsynchronousBuffer()时，嵌入指针可以使用CeFreeAsynchronousBuffer()
 和CeCloseCallerBuffer() 。
**6.安全复制**
向驱动程序传递数据会导致额外的风险——可能改变已经过驱动程序验证的、API执行期间的指针及或它们指向的数据。为了防止这些类型的问题，可以使用安全副本方法，它会给驱动程序的数据创建一个单独的副本。创建安全副本时，被传递的缓冲区数据会拷贝至驱动程序的内部缓冲区中。这种方式适合于下列情形：
　　·用于所有嵌入指针。
　　·用于所有使用之前需验证的参数。
注意，使用安全副本在一定程度上会影响效率。创建安全副本时，可以使用下列方法：
　　·手动方式。
　　·利用CeOpenCallerBuffer()函数，并将嵌入指针的参数ForceDuplicate设为TRUE。
　　·为指针参数调用CeAllocDuplicateBuffer()。
**7.内存地址重建API函数和类**
下面我们介绍内存地址重建API函数。
CeOpenCallerBuffer ：验证访问权限并完成指针的内存地址重建。返回一个内存地址重建后的指针。分配资源。为了在指针处理结束后释放资源，必须调用CeCloseCallerBuffer函数。
CeCloseCallerBuffer：释放CeOpenCallerBuffer函数分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
CeAllocAsynchronousBuffer：使用CeOpenCallerBuffer函数或者由系统整理出一个缓冲区，以供异步访问之用。在返回发起调用的线程之前，必须同步调用这个函数。它会分配有关资源，为了在指针处理结束后释放资源，必须调用CeFreeAsynchronousBuffer函数。
CeFreeAsynchronousBuffer：会释放由函数CeAllocAsynchronousBuffer分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
CeFlushAsynchronousBuffer：会根据函数CeAllocAsynchronousBuffer对该缓冲区所做的修改而改变源缓冲区。
CeAllocDuplicateBuffer：为参数指针创建安全副本。
CeFreeDuplicateBuffer：释放由函数CeAllocDuplicateBuffer分配的所有资源。如有必要，可以向被传递的缓冲区进行回写。
除此之外，Windows Embedded CE 6.0还另外提供了一组C++类，用于内存地址重建，它们位于/PUBLIC/COMMON/OAK/INC/MARSHAL.HPP，下面对它们做相应说明。
AsynchronousBuffer_t：CeAllocAsynchronousBuffer函数和CeFreeAsynchronousBuffer函数的包装类，用于需要异步访问的内存地址重建后的指针。
DuplicatedBuffer_t：CeAllocDuplicateBuffer函数和CeFreeDuplicateBuffer函数的包装类，用于指针参数。
MarshalledBuffer_t：CeOpenCallerBuffer、CeCloseCallerBuffer、CeAllocAsynchronousBuffer和CeFreeAsynchronousBuffer函数的包装类，用于未重建内存地址的嵌入指针。
**8.调试域**
要想开发出可靠和稳定的驱动程序，必须在可能引起异常错误的可执行码中插入__try/__except/__finally块，特别是那些从外部接收数据的代码。
调试是驱动程序开发工作的一大项，幸运的是Windows Embedded CE开发工具提供了调试驱动程序所需的全部功能。Windows Embedded CE提供了两种调试驱动程序的方法，第一种标准的单步调试方式，它能够进入内核提供的代码;第二种是无间断的调试方式。注意，要使用标准内核调试工具，应该为特定的硬件平台实现KITL，并选择传输方法。
如果希望调试系统的所有部分，您应当构建一个Debug OS映像。如果希望调试整个驱动程序，那么只需构建一个包括内核程序调试工具KITL和调试版的带有辅助调试文件的驱动程序的Retail OS映像就足够了。如果希望调试整个驱动程序和部分系统的话，您应当在映像中包含前面所说的各种组件，以及带有辅助调试文件的系统部分。
调试域是一种改进型的printf调试技术，并且能在运行时配置输出，还能与Platfrom Builder集成使用。从最基础的角度而言，调试域向调试输出发送条件输出。按照这种方式，调试域可以在不需要中断执行的情况下提供驱动程序执行信息。
所有的系统码都可用于调试域，所以您不仅能够看到来自驱动程序调试域的输出，而且还能得到有关系统活动信息。这一功能有助于在驱动程序开发期间暴露并解决问题。
为了使用调试域，需要进行如下所示的工作：
·在驱动程序的头文件中包含dbgapi.h：#include
·为调试域定义掩码，例如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->//zone 0
#define ZONEMASK_INIT (0x00000001<<0)
//zone 1
#define ZONEMASK_ACTIONS (0x00000001<<1)
//zone 2
#define ZONEMASK_EXCEPTIONS (0x00000001<<2)
//zone 14
#define ZONEMASK_WARNING (0x00000001<<14)
//zone 15
#define ZONEMASK_ERRORS (0x00000001<<16)
定义用于条件式调试域输出的标志，例如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->//如果zone 0启用，则为true
#define ZONE_INIT DEBUGZONE (0)
//如果zone 1启用，则为true
#define ZONE_ACTIONS DEBUGZONE (1)
//如果zone 2启用，则为true
#define ZONE_EXCEPTIONS DEBUGZONE (2)
//如果zone 14启用，则为true
#define ZONE_WARNING DEBUGZONE (14)
//如果zone 15启用，则为true
#define ZONE_ERRORSDEBUGZONE (15)
定义参数dpCurSettings，如：
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->　　DBGPARAM dpCurSettings = {
//通常为模块名称
　　TEXT("MyDriver"),
　　{//16个域的名称
　　TEXT("Init"),TEXT("Actions"),TEXT("Exceptions"),TEXT(""),
　　TEXT(""),TEXT(""),TEXT(""),TEXT(""),
　　TEXT(""),TEXT(""),TEXT(""),TEXT(""),
　　TEXT(""),TEXT(""),TEXT("Warnings"), TEXT("Errors")
　　},
//由系统默认为启用状态的域
　　ZONEMASK_ERRORS| ZONEMASK_EXCEPTIONS|ZONEMASK_INIT
　　};
使用相应的宏注册调试域，如下所示：
DEBUGREGISTER()：用于Debug构建，如果用于构建.exe程序的话，则以NULL作为参数。如果要构建为.dll程序的话，以handle作为其参数。
RETAILREGISTERZONES()：用于Retail构建和Debug构建，如果用于构建.exe程序的话，则以NULL作为参数。如果要构建为.dll程序的话，以handle作为其参数。
您可以在驱动程序代码中包含所需的宏，具体参见下文。
建立合适的OS构建(Debug或者Retail)。
将映像装入设备。
使用Platform Builder配置活动的调试域。
下面我们介绍用于调试域的宏，如下所示：
　　RETAILMSG ( , )：有条件地输出一个printf风格的格式化消息。
　　RETAILLED (, )：有条件地将WORD的值输出至LED。
　　ERRORMSG( , )：有条件地输出一个printf风格的格式化ERROR消息，并带有错误所在文件名和行号。
　　DEBUGMSG ( , )：有条件地输出一个printf风格的格式化消息。
　　DEBUGLED (, )：有条件地将WORD的值输出至LED。
　　DEBUGCHK( )：为表达式建立一个断言，并且在表达式为FALSE的时候生成一个DebugBreak。
　　DEBUGZONE()：测试当前调试域设置中的掩码位。
　　DEBUGREGISTER()：仅在Debug构建时注册调试域。
　　RETAILREGISTERZONES()：在Debug构建和Retail构建时都注册调试域。
**9.小结**
本文为读者详细介绍驱动程序的实现方法，内容包括驱动程序的任务、缓冲区的处理、内存地址重建及调试域等。希望本文对您有所帮助。
