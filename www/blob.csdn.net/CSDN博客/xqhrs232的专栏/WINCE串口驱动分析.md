# WINCE串口驱动分析 - xqhrs232的专栏 - CSDN博客
2013年01月27日 13:05:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：749
原文地址::[http://blog.chinaunix.net/uid-22597935-id-1772435.html](http://blog.chinaunix.net/uid-22597935-id-1772435.html)
虽然串口通讯已经是普遍的标准而且广为大家熟知，但驱动中涉及的部分内容也可能在平时的应用中并不是很常用到，在这里做一个简单的介绍待后面说明到具体代码的时候可以连贯一些。
串行通讯接口是目前十分流行的通讯接口之一。由于其电气界面的简单性使其在计算机领域的应用相当的广泛。在这里提到的串行通讯接口主要是指UART(通用串行)和IRDA两种。通常的串行连接电气连接上有3wire和9wire两种。3wire的接线方式下定义了发送、接收和地三根连接。其用途就如名称一样分别用于发送、接收。下面是通常3wire连接的结构框图
![](http://blogimg.chinaunix.net/blog/upfile2/100321185219.jpg)
通常在串行接口控制器上会有两个FIFO用作接收和发送的缓冲，当接收到数据后会直接将接收到的数据置入该缓冲器，并同时由控制电路向本地总线发出通知，以便让本地总线将缓冲器内的数据读走，这样在响应(等待和读取)的过程中仍然能通过缓冲器来接收数据。而发送的过程则刚好相反，本地总线可一直向发送缓冲写入数据直到器填满为止，而无需对每个数据的发送进行等待。这就是基本的收发流程(这部分逻辑流程相信大家是最熟悉的)。这一点在3wire和9wire中都是相同的。但是我们考虑下面的情况，如果接收一方的响应由于某种原因的干扰(如处理器被其他中断服务占用)的时候可能就来不及相应之前ReceiveFIFO就可能被填满了，这样后续发送过来的数据就会丢失，这样在需要数据可靠传输的情况下串行通讯的弊端也就显示出来了。如需要数据的可靠传输就需要对数据流的收发进行控制。在9wire中将串行连接定义为如下形式
|针号|1|2|3|4|5|6|7|8|9|
|----|----|----|----|----|----|----|----|----|----|
|缩写|DCD|RXD|TXD|DTR|GND|DSR|RTS|CTS|DELL|
|功能说明|数据载波检测|接收数据|发送数据|数据终端就绪|信号地|数据设备就绪|请求发送|清除发送|振铃指示|
也就是说在原3wire的基础上增加了DCD,DTR,DSR,RTS,CTS,DELL六个控制线。其中RTS/CTS用于流控制，另外的DCD和DELL则留作连接modem使用。有了专门的硬件流控制引脚也就使得流控制成为可能，以完成收发两端的匹配使得数据可以可靠的传输。用RTS/CTS（请求发送/清除发送）流控制时，应将通讯两端的RTS、CTS线对应相连).在发送端准备发送数据之前设置RTS(Request to send)也就使发送请求线，若接收端以作好接收准备，就启动响应的CTS（Clear
 to send）引线。这样，收发双发就进入数据传输状态，在此过程中如若接收端处理数据的速度低于发送端的发送速度，接收一端还可以设置CTS引线恢复原来阻塞得状态以暂时中断数据传输，之后若需要恢复数据传输恢复CTS状态即可。这样UART的传输即实现了流控制，保障了数据传输的完备性。
在这里还要说一下软件流控制，虽然硬件已经可以完成流控制的任务但很多少时候受到连线数的限制不能使用硬件流控制也就设计了专门的软件流控制的方法。现在回到3线传输的情景，若接收端接收数据过程中缓冲器的负载到达某一限制(也就是留出一定的缓冲空间)时接收端向发送端发送一个特殊的标示位(接收停止位)，当发送端收到该标示的时候就停止发送，直到接收端缓冲器低于另一限制后发送标示(接收许可位)给发送端，这样就可以控制数据流的传输起停。这种软件流控制是在给缓冲器留余量来完成的，在收发双端处理器速度差很大的时候就不太适用了，就必须要用硬件流控制。
其他几个引脚都是与modem相关的，DSR数据装置准备好（Data set ready)用于表明MODEM处于可以使用的状态。DTR数据终端准备好(Data terminal ready)表明数据终端可以使用。这两个信号用于检查Modem是否连接。DELL脚当有电话拨入时Modem将会设置这个引脚。DCD信号是当Modem接收到数字载波信号的时候被设置，用于了解Modem接收信号的情况。
至于剩下的奇偶效验和停止位设置就只是需要针对寄存器设置无需软件干涉就可以完成了。下面我们来看具体的驱动程序。
**架构**
在wince中串口的驱动实现是有固定模型的，wince中的串口模型遵循ISO/OSI网络通讯模型(7层)，就是说串口属于wince网络模块的一个部分。其中RS232界面(或其它的物理介质)实现网络的物理层，而驱动和SerialAPI共同组成数据链路层，其它部分都没有做定义。在典型的应用中,SerialAPI与间接通过TAPI或直接与ActiveSync交互，组成CE网络的一部分。而红外本身的协议就相对复杂的多，它有专门的一套模型来描述其使用规则，对红外设备本身了解不多也就不能深入下去。在串口的这一侧，整个驱动模型也是相当的复杂，但所幸的是驱动仅仅使用到SerialAPI这一层，在这个层次上串口的行为还是相对简单的。
![](http://blogimg.chinaunix.net/blog/upfile2/100321185629.jpg)
我们这里仅仅涉及上面所提到的Serial/irda Driver这部分(绿色部分)。在wince提供的驱动例程中串口/红外驱动采用分层结构设计，MDD提供框架性的实现，负责提供OS所需的基本实现，并将代码设计与具体的硬件设计无关。而PDD提供了对硬件操作相应的代码。这些代码通过结构HWOBJ来相互联系。对于MDD+PDD的整体驱动来看，串口驱动模型是作为Stream来实现的。
两者合一以达到实现驱动的目的。DDSI就是指这两个部分之间的接口，这个接口并非受到强制的物理/逻辑关系来约束，而是人为的规定的。在涉及到一种特定硬件我们进行针对实现的时候往往需要的是了解硬件的物理特性和控制逻辑，然后根据DDSI的约束就来进行实现。对于这里描述的驱动模型而言结合关键在于结构指针HWOBJ的使用和具体实现。在实际的驱动应用中仅仅需要实现HWOBJ相关的一系列函数，而无需从驱动顶层完全开发。串口驱动模型作为一种常用驱动模型在windowsCE中常常用于串口/红外/USB
 Client的具体实现。该驱动模型中对全功能的串口进行了定义，除了常用的TX和RX引线定义以外，针对DTR、RTS等功能引脚都进行了支持，使得用该模型设计的串口驱动支持流控制、具备驱动Modem等设备的能力。
事实上，如果需要的话完全可以将该驱动一体化设计(抛开PDD-MDD的划分，也就无须DDSI)。也就是不使用现有的驱动架构来进行实现。考虑到串口驱动的使用频率和执行效率要求都不是很苛刻的情况下抛弃驱动架构另外实现的就没有多大必要了。
对于驱动本身而言，串行驱动从功能和实现上相当的简单，却具被相当全面的成分，对该驱动的分析和了解无疑是学习流式驱动程序很好的典范。
**代码分析**
在开始具体代码之前我们先来看看，相关的一些结构。 HWOBJ是相应的硬件设备操作的抽象集合。结构的定义后的注释与实际的用途有点点出入，BandFlags指定IST的启动时间，可选为在初始化过程启动或是在打开设备的时候起动ISR.而第二个参数则是指定拦截的具体的系统中断号。最后一个参数是一个结构，该结构定义了硬件操作的各式行为函数的指针，MDD正是通过这些函数来访问具体的PDD操作。
typedef struct __HWOBJ {
    ULONG   BindFlags; // Flags controlling MDD behaviour. Se above.
    DWORD   dwIntID;   // Interrupt Identifier used if THREAD_AT_INIT or THREAD_AT_OPEN
    PHW_VTBL pFuncTbl;
} HWOBJ, *PHWOBJ;
而HW_VTBL则是代表具体硬件操作函数指针的集合，该结构所指向的函数包括了初始化、打开、关闭、接收、发送、设置Baudrate等一系列操作。结构存在就像纽带一样联系着PDD中的具体实现和MDD中的抽象操作。PDD的实现必须遵循HW_VTBL中所描述的函数形式，并构造出相应的HW_VTBL实例。驱动的编写就是针对这些函数来一一进行实现。
typedef struct __HW_VTBL    {
    PVOID  (*HWInit)(ULONG Identifier, PVOID pMDDContext, PHWOBJ pHWObj);
    BOOL   (*HWPostInit)(PVOID pHead);
    ULONG  (*HWDeinit)(PVOID pHead);
    BOOL   (*HWOpen)(PVOID pHead);
    ULONG  (*HWClose)(PVOID pHead);
    INTERRUPT_TYPE (*HWGetIntrType)(PVOID pHead);
    ULONG  (*HWRxIntrHandler)(PVOID pHead, PUCHAR pTarget, PULONG pBytes);
    VOID   (*HWTxIntrHandler)(PVOID pHead, PUCHAR pSrc, PULONG pBytes);
    VOID   (*HWModemIntrHandler)(PVOID pHead);
    VOID   (*HWLineIntrHandler)(PVOID pHead);
    ULONG  (*HWGetRxBufferSize)(PVOID pHead);
    BOOL  (*HWPowerOff)(PVOID pHead);
    BOOL  (*HWPowerOn)(PVOID pHead);
    VOID  (*HWClearDTR)(PVOID pHead);
    VOID  (*HWSetDTR)(PVOID pHead);
    VOID  (*HWClearRTS)(PVOID pHead);
    VOID  (*HWSetRTS)(PVOID pHead);
    BOOL  (*HWEnableIR)(PVOID pHead, ULONG BaudRate);
    BOOL  (*HWDisableIR)(PVOID pHead);
    VOID  (*HWClearBreak)(PVOID pHead);
    VOID  (*HWSetBreak)(PVOID pHead);
    BOOL  (*HWXmitComChar)(PVOID pHead, UCHAR ComChar);
    ULONG (*HWGetStatus)(PVOID pHead, LPCOMSTAT lpStat);
    VOID  (*HWReset)(PVOID pHead);
    VOID  (*HWGetModemStatus)(PVOID pHead, PULONG pModemStatus);
    VOID (*HWGetCommProperties)(PVOID pHead, LPCOMMPROP pCommProp);
    VOID  (*HWPurgeComm)(PVOID pHead, DWORD fdwAction);
    BOOL  (*HWSetDCB)(PVOID pHead, LPDCB pDCB);
    BOOL  (*HWSetCommTimeouts)(PVOID pHead, LPCOMMTIMEOUTS lpCommTO);
    BOOL  (*HWIoctl)(PVOID pHead, DWORD dwCode,PBYTE pBufIn,DWORD dwLenIn, PBYTE pBufOut,DWORD dwLenOut,PDWORD pdwActualOut);
    } HW_VTBL, *PHW_VTBL;交待了上述两个结构以后我们来看看具体的代码，为保障对系统架构的清晰认识，我们将MDD的代码和PDD的代码分开进行分析。
**MDD部分**
由于串口驱动由Device.exe直接调用，所以MDD部分是以完整的Stream接口给出的. 也就具备基于Stream接口的驱动程序所需的函数实现，包括COM_Init，COM_Deinit，COM_Open,COM_Close ,COM_Read ,COM_Write, COM_Seek,, COM_PowerUp, COM_PowerDown, COM_IOControl几个基本实现。由于串口发送/接收的信息并不能定位，而仅仅是简单的传送，所以COM_Seek仅仅是形式上实现了一下。
**COM_Init**
COM_Init是该驱动的初始化函数,在设备管理器加载该驱动后首先调用，用于初始化所需的变量，硬件设备等资源。该过程分配代表设备硬件实例的数据结构，并通过硬件抽象接口HWInit初始化硬件。同时该函数会调用InterruptInitialize为接收内核中的逻辑中断创建相应事件并初始化临界区。该函数还需要得到硬件缓冲器的物理地址和获知该缓冲器的大小(该冲器最小为2K)。最后它将建立相应的缓冲作为接收的中介。下面我们来看这个函数的实现过程。
在函数中定义了两个重要的变量。pSerialHead和pHWHead.前者用于描述相应的串口的状态，后者则是对应硬件的数据抽象。首先为pSerialHead分配空间和初始化链表和临界区等数据并同时为接收和发送中断创建事件。然后再从注册表中获得当前的注册项值(由于device.exe是根据注册表键值来调用驱动的，当前键注册表项指的就是与上述键值在同一根下的注册项)。得到DeviceArrayIndex、Priority256键下的具体值后就可以调用GetSerialObject
 (在PDD中实现)来获得具体的HWObj对象，并通过该对象来调用硬件初始化函数了。(由于在这里已经对硬件进行了初始化，之后的返回都需要调用COM_Deinit来完成。)由于硬件初始化(实际的驱动初始化代码)已经得到执行这个时候就只有分配和初始化缓冲的工作需要做了。所以调用HWGetRxBufferSize(PDD代码)来获取PDD中设定的缓冲大小，并根据返回的具体值分配缓冲。最后如果BindFlags被设定为THREAD_AT_INIT就再调用StartDispatchThread启动分发线程(实际的IST)。这样就完成了系统初始化的操作。
**COM_Deinit**
当驱动被称被卸下的时候该事件启动，用作与COM_Init相反的操作。这个过程大致会释放驱动中所使用的资源，停止期间创建的线程等操作。具体说来，大致为停止在MDD中的所有IST，和释放内存资源和临界区等系统资源。同时还需调用HWDeinit来释放PDD中所使用到的系统资源。
**COM_Open**
COM_Oepn在CreateFile后被调用，用于以读/写模式打开设备，并初始化所需要的空间/资源等，创建相应的实例，为后面的操作做好准备。这里的代码相对比较容易，下面就简单讲一下。既然是初始化，肯定就免不了对参数的检查。首先检查通过COM_Init返回的pHead结构是否有效，这里虽然没有显式的在这两个函数之间传递参数，而是在设备管理器的内部传递这个参数的。
![](http://blogimg.chinaunix.net/blog/upfile2/100321185718.jpg)
之后是检查文件系统传递过来的Open句柄中的Open模式是否有效，这个参数由应用程序产生，通过文件系统直接传递到驱动。之后就开始初始化的操作，在这里将会建立相应的HW_OPEN_INFO实体。下面为该结构的定义。
typedef struct __HW_OPEN_INFO {
    PHW_INDEP_INFO pSerialHead; // @field Pointer back to our HW_INDEP_INFO
    DWORD   AccessCode; // @field What permissions was this opened with
    DWORD   ShareMode; // @field What Share Mode was this opened with
    DWORD   StructUsers; // @field Count of threads currently using struct.
    COMM_EVENTS   CommEvents; // @field Contains all in…. handling
    LIST_ENTRY    llist; // @field Linked list of OPEN_INFOs
         } HW_OPEN_INFO, *PH
结构中的第一个参数指向我们前面提到的HW_INDEP_INFO结构，第二个参数为操作权限码，也就是READ/WRITE这类的权限。第三个参数为共享模式，以确定是否支持独占。这两个参数都是与文件系统的内容对应的。而CommEvent则对应于本实例的事件。由于驱动架构支持多个OPEN操作实例的存在，所以这里维护了一个链表来联系这些结构。在这里由于IST的启动可以在COM_Init和COM_Open中进行，还有处理器启动IST的内容。准备好HW_OPEN_INFO结构后就可以调用HWOpen(PDD)来进行PDD所需的Open操作了。Open操作完成后调用HWPurgeComm(PDD)来处理（取消或等待）当前仍在通讯状态的任务。然后重置软件FIFO就基本完成了COM_Open的动作了。
事实上这里主要是对所需的数据结构进行处理，对于硬件的具体操作都留给PDD去做了，MDD所维护的仅仅是一个架构性的代码。Open操作完成后，驱动这个时候就进入了工作状态。
**COM_Close**
COM_Close为与COM_Open相对应的操作。这期间的目的是释放COM_Open所使用的系统资源，除此以外如果在COM_Open期间创建了相应的IST还需要停止该线程，在最后将该HW_OPEN_INFO脱链。这样一来驱动状态就得以恢复。当然这期间还做了一写避免线程竞争的处理，使得代码看起来不是那么简单。
**StartDispatchThread/StopDispatchThread**
这两个函数都不是Stream所需要的标准接口，但却是中断服务程序所需的IST启动和关闭的手段，所以在这里顺便说一下。
StartDispatchThread函数用于启动IST，主要的工作为调用InterruptInitialize将系统中断与相应的事件联系起来。并启动SerialDispatchThread作为IST.其中调用了叫做 InterruptDone的函数，该函数会调用OAL中的OEMInterruptDone来完成中断的响应。
StopDispatchThread用于与StartDispatchThread相反的操作。停止的过程相对要复杂一些，该函数首先设定当前线程的优先级与分发线程相同，以便于在停止该线程的动作不会比释放内存的动作快以避免出错。停止的动作是让线程主动完成的，具体的方法是提交表示位KillRxThread然后通过Sleep请求调度,待到IST自己停止。这个时候由于IST已经停止所以在程序的最后调用InterruptDisable来屏蔽中断。
**SerialDispatchThread/ SerialEventHandler**
SerialDispatchThread/ SerialEventHandler就是串口驱动的中断分发程序(也就是IST的所在)。整个IST被分开写成两个部分---循环主体和事件处理程序。循环主体SerialDispatchThread内容相对比较简单，反复等待串口事件并调用SerialEventHandler对具体的中断进行处理，直到pSerialHead->KillRxThread被设置后退出。SerialEventHandler为中断处理的具体实现，程序在获得串口事件后运行，目的在于对中断进行进一步的判断并执行相应的处理。
下面参考两个结构体来完成接受和发送中断服务的分析。我们先来看RX_BUFFER_INFO结构。
typedef struct __RX_BUFFER_INFO {
         ULONG     Read; /* @field Current Read index. */
         ULONG     Write; /* @field Current Write index. */
         ULONG     Length; /* @field Length of buffer */
         BOOL      DataAvail; /* @field BOOL reflecting existence of data. */
         PUCHAR    RxCharBuffer; /* @field Start of buffer */
         CRITICAL_SECTION    CS; /* @field Critical section */
         } RX_BUFFER_INFO, *PRX_BUFFER_INFO;
用该结构的原因是在驱动内部维护了一个缓冲器用作驱动和应用程序之间的缓冲见下图.
![](http://blogimg.chinaunix.net/blog/upfile2/100321185805.jpg)
可以看到在硬件内部已经有一个FIFO缓冲器，这保障了驱动的数据接收，但由于应用不一定能保障在驱动获得数据后及时将数据取走，因此在驱动内部维护了另外一个缓冲器。在RX_BUFFER_FIFO结构中的read成员为MDD取数据时在FIFO的位置标志，而PDD向软件写入数据的位标则对应被称作Write，DataAvail用作表示缓冲器内的数据是否有效。而RxCharBuffer则是指向软件FIFO的指针。当收到数据的时候就将write标示往上递增，而程序向驱动取数的时候Read递增，这样就可以根据Read和Write成员来维护这个FIFO。有了这个基本思路垫底我们接着看RX的中断服务具体如何实现。这间还会涉及到流控制的成分。
接收分支：在接收分支的开始计算软件缓冲器的剩余空间，如果有剩余的空间的话直接调用HWRxIntrHandler(PDDa实现)来从硬件缓冲区获取剩余空间大小的数据，若已无剩余空间则建立一个16byte的临时缓冲区，将数据读入该区域，实际上这个缓冲区在后面根本就不会被读取所以这些数据全部丢失掉了这也就自然需要统计硬件/软件缓冲导致的数据丢失(接收不及时导致)。接下来就是所谓XFlow的流程了，所谓XFlow就是我们上面提到的软件流控制。也就是用软件的方法来协调发送和接收端的收发，保障数据的完整接收。当接收到XOFF/XON标记的时候由于这个标记本身不是数据而是控制标志，所以要讲后面的数据全部前置一位，覆盖掉XON/XOFF的位置。同时还需要根据标示的具体状态来设置DCB结构中的控制标示来控制数据收发流程。如果是XON标志，还需要在处理完接收流程后恢复发送流程。接收的动作会改变write标记的位置，这里需要重新计算该标示。至于硬件流控制的流程中该驱动模型是以缓冲器的75%为分位点来起停收发的，可用的硬件连线可以是DTR,也可以是RTS（模式相同仅仅是连线不同），这里的操作很简单，仅仅是通过计算缓冲器的存储状态来清除该标志就完成了硬件流控制的操作。由于在此过程中IST与其他部分是同步执行的,所以这个时候如果使用XFlow可能还会需要做一次安全检查。这样接收的流程就结束了。
发送分支： 我们同样来看看TX_BUFFER_INFO结构，看样子似乎该结构维护了一个和TX缓冲类似的缓冲区，但事实上这个缓冲区域是不独立存在的，发送的流程因为可以直接使用所需发送的数据的存储区域来作为发送缓冲，所以这个缓冲没有独立存在的必要。由于使用其它进程的数据区域，所以这里增加了权限控制项的成分，用于突破进程间的访问限制。
typedef struct __TX_BUFFER_INFO {
         DWORD     Permissions; /* @field Current permissions */
         ULONG     Read; /* @field Current Read index. */
         ULONG     Length; /* @field Length of buffer */
         PUCHAR    TxCharBuffer; /* @field Start of buffer */
         CRITICAL_SECTION    CS; /* @field Critical section */
} TX_BUFFER_INFO, *PTX_BUFFER_INFO;
下面来看看代码的具体内容。首先是对OpenCnt的检查，也就是设备是否被打开。若当会话已经关闭也就没有必要继续将数据送出了，并同时重置缓冲器的各个标志位。整个流程比较简单，在需要流控制时设置RTS或检查Xflow的情况后将数据送入硬件缓冲器.如果在没有数据需要发送的情况下简单的清除中断标示并发出发送结束事件就可以了。至于SetProcPermissions设置的目的在于获得访问其它线程数据空间的手段。
至于所谓的Modem和Line的情况则全部交给PDD来处理，我们后面再讨论。在这些全部都处理完了以后如果前面处理了接收，则发出接收（有可用的数据用于接收）的消息，让程序开始接收。后面还跟进了一个EvaluateEventFlag 函数，这个函数用于产生标准的Communication Events EV_RXFLAG，而且由于该驱动程序本身支持mult-open模式，所以需要将该事件送发到所有的实例中去。在ist期间有一些互锁、临界区的操作，因为不影响流程，同步化的内容这里我没有提。中断服务的分析大致就是如此，没有涉及到逻辑环节在后面的PDD部分再进行讨论。
**COM_Read**
COM_Read是获取串口所接收到数据的操作，在前面的IST中没有看到对RX buffer进行修改Read标记的操作，也就是这儿来完成的。该函数有三个参数，第一个参数是从上面的COM_OPEN通过设备管理器交换来的，后两个参数与文件系统的使用方法完全一样，一个是接受缓冲指针，另一个是长度。代码的开始照样是例行公事的参数检查，包括对存取权限，OpenCnt等。之后计算超时时间，如果设定了超时读取动作会在超时后返回，不管是否读到了足够长度的数据。随后就是简单对软件缓冲进行读取的操作了，读取的操作是在RX_CS中完成的。下面要处理器的主要就是几种异常的情形，读取过程中设备被关闭/取消读取和超时。最后在读取的过程中需要处理的就只是流控制的成本了。首先是软件流的情形，如果缓冲的状态由高于分位点至分位点以下就发出XON标记，启动发送端的发送。而硬件流的情形无论是RTS还是DTR与软件流的相类似，同样由一个分为点(50%)来决定发出启动发送端的信号，仅仅是这里使用的具体措施的不同。这些硬件信号的发出都是由PDD来完成的，其中包括HWSetRTS和HWSetDTR(2选一)。至此Read的流程就结束了。
**COM_Write**
COM_Write是与COM_Read相对应的操作。所传递的参数的形式也是很相似的，仅仅是数据流向的不同。在程序的开始，同样也是参数检查，内容与COM_Read一致。在数据检查完成之后进入临界区(保障多线程下的独占)将送入的目标地址和长度设置为TX buffer，待到数据发送完成事件后调用DoTxData来启动发送。这里启动发送的目的在于获得硬件中断维持发送流程。在这里DoTxData是作为两种状态来执行的，在通过COM_Write的执行的过程中是在device.exe所创建的线程空间内执行的，但由系统中断事件主动启动的过程中属于IST本身的的进程空间，这样在COM_Write中调用DoTxData之前设置的权限代码(由GetCurrentPermissions获得)就可以由TxBufferInfo传递到IST中去使得中断过程也具备了访问缓冲的权限(结合前面说明IST的流程)。当提交中断处理发送后待到pSerialHead->hTransmitEvent被设置或是异常或超时后就结束了发送流程，在这部分的最后。与COM_Read类似需要处理一些异常情况，当然如果使用了硬件流控制还需要在这里清除掉发送请求信号，当这些状态处理完成以后发送EV_TXEMPTY事件通告所有open的句柄发送结束就完成了该部分的流程。
**COM_PowerUp/ COM_PowerDown**
这两个函数的调用都由CE的电源事件来引发，MDD并没有对这两个函数进行处理，仅仅是将其传递给PDD。
**COM_IOControl**
对于串口驱动来说，COM_IOControl函数非常有用，应用程序通过调用COM_IOControl函数并传入不同的操作码，实现了控制串口的功能。该函数实现向设备发送命令的功能。由于代码本身没有什么流程或逻辑性可言，全都是单独的实现，下面就用列表的方式大致的说一下这些命令字和其实现。
|Command|Note|
|----|----|
|IOCTL_PSL_NOTIFY|在调用驱动的进程退出时产生，并不是串行驱动专有的IO命令。这里会调用ProcessExiting函数进行处理。这个函数的内容放到后面来看。|
|IOCTL_SERIAL_SET_BREAK_ON|中断(暂停)serial当前的发送或是接收，具体实现在PDD中|
|IOCTL_SERIAL_SET_BREAK_OFF|从中断（暂停）状态恢复，具体实现在PDD中|
|IOCTL_SERIAL_SET_DTR|将DTR引线拉高。(直接调用PDD实现)|
|IOCTL_SERIAL_CLR_DTR|将DTR引线拉低。(直接调用PDD实现)|
|IOCTL_SERIAL_SET_RTS|将RTS引线拉高。(直接调用PDD实现)|
|IOCTL_SERIAL_CLR_RTS|将RTS引线拉低。(直接调用PDD实现)|
|IOCTL_SERIAL_SET_XOFF|软件流模式下中止数据发送(Xflow控制)|
|IOCTL_SERIAL_SET_XON|软件流模式下启动数据发送(XFlow控制)|
|IOCTL_SERIAL_GET_WAIT_MASK|获取当前的事件对象|
|IOCTL_SERIAL_SET_WAIT_MASK|设置事件对象，这个过程相对比较麻烦，要将当前获得的事件对象mask设置到所有的Open实例中，这和前面的EvaluateEventFlag过程相似。|
|IOCTL_SERIAL_WAIT_ON_MASK|等待与提供的事件相同的事件发生，实现实体是 WaitCommEvent后面再讨论。|
|IOCTL_SERIAL_GET_COMMSTATUS|清除异常并返回当前状态(由PDD实现)|
|IOCTL_SERIAL_GET_MODEMSTATUS|获取modem状态(由PDD实现)|
|IOCTL_SERIAL_GET_PROPERTIES|获取通讯************************(由PDD实现)|
|IOCTL_SERIAL_SET_TIMEOUTS|设置超时时间(包含PDD实现)|
|IOCTL_SERIAL_GET_TIMEOUTS|获取超时时间|
|IOCTL_SERIAL_PURGE|清除制定的发送或接收缓冲内的数据(含PDD实现)|
|IOCTL_SERIAL_SET_QUEUE_SIZE|不明，若知道请告知|
|IOCTL_SERIAL_IMMEDIATE_CHAR|为扩展功能，在发送数据前设置一个标志数|
|IOCTL_SERIAL_GET_DCB|获取DCB数据结构|
|IOCTL_SERIAL_SET_DCB|设置DCB数据结构|
|IOCTL_SERIAL_ENABLE_IR|启动红外模式(由PDD实现)|
|IOCTL_SERIAL_DISABLE_IR|禁用红外模式(由PDD实现)|
到这里MDD的主要函数都已经介绍过了，下面几个函数是在DeviceIOControl中用到的。这里顺便也来看一下:
**ProcessExiting**
该函数在IOCTL_PSL_NOTIFY命令的执行过程中被调用,之前的情景是使用驱动的进程在被取消的过程中，在这里主要是清除所有正在会话中的线程。以便直接kill掉该进程。
**WaitCommEvent**
事实上该函数为SerialAPI WaitCommEvent在驱动内的实现，其作用为阻塞线程直道某一固定的串口通告(事件消息)发生。在具体的实现中，是用WaitForSingleObject来实现阻塞。在进入阻塞之前，函数适用一个循环主体首先查询是否存在已有的通告与等待通告相符，若没有就等待下一次事件发生，待事件发生再次进行检查。如此循环达到阻塞的目的。
**ApplyDCB**
DCB数据结构是描述串行口波特率，流控制，奇偶效验等资料的载体。该函数是MDD设置DCB数据结构至驱动内部和硬件的手段，这里使用了大量的PDD操作来完成硬件设置。
**总结：**
**在驱动实现方面，除去所谓Multi-Open的处理外，串口的MDD并没有什么特别的之处，在掌握了硬件行为和应用软件行为后很容易能读懂其间的代码。**
