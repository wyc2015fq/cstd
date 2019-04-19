# Android Phone分析 - xqhrs232的专栏 - CSDN博客
2011年12月11日 22:33:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：903
原文地址::[http://lisaguo.iteye.com/blog/920065](http://lisaguo.iteye.com/blog/920065)
**Android Phone分析（一） **2010-01-27 17:45 
Android的Radio Interface Layer (RIL)提供了电话服务和的radio硬件之间的抽象层。 
Radio Interface Layer RIL(Radio Interface Layer)负责数据的可靠传输、AT命令的发送以及response的解析。应用处理器通过AT命令集与带GPRS功能的无线通讯模块通信。
AT command由Hayes公司发明，是一个调制解调器制造商采用的一个调制解调器命令语言，每条命令以字母"AT"开头。 
JAVA Framework 
代码的路径为： 
frameworks/base/telephony/java/android/telephony 
android.telephony以及android.telephony.gsm 
Core native: 
在hardware/ril目录中，提供了对RIL支持的本地代码，包括4个文件夹： 
hardware/ril/include 
hardware/ril/libril 
hardware/ril/reference-ril 
hardware/ril/rild 
kernel Driver 
在Linux内核的驱动中，提供了相关的驱动程序的支持，可以建立在UART或者SDIO，USB等高速的串行总线上。 
hardware/ril/include/telephony/目录中的ril.h文件是ril部分的基础头文件。 
其中定义的结构体RIL_RadioFunctions如下所示： 
typedef struct { 
int version; 
RIL_RequestFunc onRequest; 
RIL_RadioStateRequest onStateRequest; 
RIL_Supports supports; 
RIL_Cancel onCancel; 
RIL_GetVersion getVersion; 
} RIL_RadioFunctions; 
RIL_RadioFunctions中包含了几个函数指针的结构体，这实际上是一个移植层的接口，下层的库实现后，由rild守护进程得到这些函数指针，执行对应的函数。
几个函数指针的原型为： 
typedef void (*RIL_RequestFunc) (int request, void *data, 
size_t datalen, RIL_Token t); 
typedef RIL_RadioState (*RIL_RadioStateRequest)(); 
typedef int (*RIL_Supports)(int requestCode); 
typedef void (*RIL_Cancel)(RIL_Token t); 
typedef const char * (*RIL_GetVersion) (void); 
其中最为重要的函数是onRequest()，它是一个请求执行的函数。 
**Android Phone分析（二） **2010-01-27 17:47 
Android的RIL驱动模块， 在hardware/ril目录下，一共分rild，libril.so以及librefrence_ril.so三个部分，另有一 radiooptions可供自动或手动调试使用。都依赖于include目录中ril.h头文件。目前cupcake分支上带的是gsm的支持，另有一 cdma分支，这里分析的是gsm驱动。
GSM模块，由于Modem的历史原因，AP一直是通过基于串口的AT命令与BB交互。包括到了目前的一些edge或3g模块，或像omap这类ap,bp集成的芯片， 已经使用了USB或其他等高速总线通信，但大多仍然使用模拟串口机制来使用AT命令。这里的RIL(Radio Interface Layer)层，主要也就是基于AT命令的操作，如发命令，response解析等。（gprs等传输会用到的MUX协议等在这里并没有包含，也暂不作介 绍。）
　　以下是详细分析，本文主要涉及基本架构和初始化的内容： 
　　首先介绍一下rild与libril.so以及librefrence_ril.so的关系： 
　　1. rild： 
　　仅实现一main函数作为整个ril层的入口点，负责完成初始化。 
　　2. libril.so： 
　 　与rild结合相当紧密，是其共享库，编译时就已经建立了这一关系。组成部分为ril.cpp，ril_event.cpp。libril.so驻留在 rild这一守护进程中，主要完成同上层通信的工作，接受ril请求并传递给librefrence_ril.so， 同时把来自librefrence_ril.so的反馈回传给调用进程。
　　3. librefrence_ril.so： 
　 　rild通过手动的dlopen方式加载，结合稍微松散，这也是因为librefrence.so主要负责跟Modem硬件通信的缘故。这样做更方便替 换或修改以适配更多的Modem种类。它转换来自libril.so的请求为AT命令，同时监控Modem的反馈信息，并传递回libril.so。在初 始化时， rild通过符号RIL_Init获取一组函数指针并以此与之建立联系。
4. radiooptions： 
　　radiooptiongs通过获取启动参数， 利用socket与rild通信，可供调试时配置Modem参数。 
**Android Phone分析（三） **2010-01-27 17:51 
分析初始化流程，主入口是rild.c中的main函数，主要完成三个任务： 
　　1. 开启libril.so中的event机制， 在RIL_startEventLoop中，是最核心的由多路I/O驱动的消息循环。 
　　2. 初始化librefrence_ril.so，也就是跟硬件或模拟硬件modem通信的部分（后面统一称硬件）， 通过RIL_Init函数完成。 
　　3. 通过RIL_Init获取一组函数指针RIL_RadioFunctions， 并通过RIL_register完成注册，并打开接受上层命令的socket通道。
　 　首先看第一个任务，也就是RIL_startEventLoop函数。RIL_startEventLoop在ril.cpp中实现， 它的主要目的是通过pthread_create(&s_tid_dispatch, &attr, eventLoop, NULL)建立一个dispatch线程，入口点在eventLoop. 而eventLoop中，会调ril_event.cpp中的ril_event_loop（）函数，建立起消息(event)队列机制。
　　我们来仔细看看这一消息队列的机制，这些代码都在ril_event.cpp中。 
void ril_event_init(); 
void ril_event_set(struct ril_event * ev, int fd, bool persist, ril_event_cb func, void * param);
void ril_event_add(struct ril_event * ev); 
void ril_timer_add(struct ril_event * ev, struct timeval * tv); 
void ril_event_del(struct ril_event * ev); 
void ril_event_loop(); 
struct ril_event { 
struct ril_event *next; 
struct ril_event *prev; 
int fd; 
int index; 
bool persist; 
struct timeval timeout; 
ril_event_cb func; 
void *param; 
}; 
　　每个ril_event结构，与一个fd句柄绑定（可以是文件，socket，管道等），并且带一个func指针去执行指定的操作。 
　 　具体流程是： ril_event_init完成后，通过ril_event_set来配置一新ril_event，并通过ril_event_add加入队列之中（实 际通常用rilEventAddWakeup来添加），add会把队列里所有ril_event的fd，放入一个fd集合readFds中。这样 ril_event_loop能通过一个多路复用I/O的机制（select）来等待这些fd， 如果任何一个fd有数据写入，则进入分析流程processTimeouts()，processReadReadies(&rfds,
 n)，firePending()。 后文会详细分析这些流程。 
　　另外我们可以看到， 在进入ril_event_loop之前， 已经挂入了一s_wakeupfd_event， 通过pipe的机制实现的， 这个event的目的是可以在一些情况下，能内部唤醒ril_event_loop的多路复用阻塞，比如一些带timeout的命令timeout到期的 时候。
　　至此第一个任务分析完毕，这样便建立起了基于event队列的消息循环，稍后便可以接受上层发来的的请求了（上层请求的event对象建立，在第三个任务中）。 
　　接下来看第二个任务，这个任务的入口是RIL_Init, RIL_Init首先通过参数获取硬件接口的设备文件或模拟硬件接口的socket. 接下来便新开一个线程继续初始化， 即mainLoop。
　 　mainLoop的主要任务是建立起与硬件的通信，然后通过read方法阻塞等待硬件的主动上报或响应。在注册一些基础回调 （timeout,readerclose）后，mainLoop首先打开硬件设备文件，建立起与硬件的通信，s_device_path和s_port 是前面获取的设备路径参数，将其打开（两者可以同时打开并拥有各自的reader，这里也很容易添加双卡双待等支持）。
　　接下来通过 at_open函数建立起这一设备文件上的reader等待循环，这也是通过新建一个线程完成， ret = pthread_create(&s_tid_reader, &attr, readerLoop, &attr)，入口点readerLoop。
　 　AT命令都是以rn或nr的换行符来作为分隔符的，所以readerLoop是line驱动的，除非出错，超时等，否则会读到一行完整的响应或主动上 报，才会返回。这个循环跑起来以后，我们基本的AT响应机制已经建立了起来。它的具体分析，包括at_open中挂接的ATUnsolHandler, 我们都放到后面分析response的连载文章里去。
　　有了响应的机制（当然，能与硬件通信也已经可以发请求了），通过 RIL_requestTimedCallback(initializeCallback, NULL, &TIMEVAL_0)，跑到initializeCallback中，执行一些Modem的初始化命令，主要都是AT命令的方式。发AT命令的 流程，我们放到后面分析request的连载文章里。这里可以看到，主要是一些参数配置，以及网络状态的检查等。至此第二个任务分析完毕，硬件已经可以访 问了。
　　最后是第三个任务。第三个任务是由RIL_Init的返回值开始的，这是一个RIL_RadioFunctions结构的指针。 
typedef struct { 
int version;　　　　　　　　/* set to RIL_VERSION */ 
RIL_RequestFunc onRequest; 
RIL_RadioStateRequest onStateRequest; 
RIL_Supports supports; 
RIL_Cancel onCancel; 
RIL_GetVersion getVersion; 
} RIL_RadioFunctions; 
　　其中最重要的是onRequest域，上层来的请求都由这个函数进行映射后转换成对应的AT命令发给硬件。 
　　rild通过RIL_register注册这一指针。 
　　RIL_register中要完成的另外一个任务，就是打开前面提到的跟上层通信的socket接口（s_fdListen是主接口，s_fdDebug供调试时使用）。
　　然后将这两个socket接口使用任务一中实现的机制进行注册（仅列出s_fdListen） 
ril_event_set (&s_listen_event, s_fdListen, false, 
listenCallback, NULL); 
rilEventAddWakeup (&s_listen_event); 
　　这样将两个socket加到任务一中建立起来多路复用I/O的检查句柄集合中，一旦有上层来的（调试）请求，event机制便能响应处理了。到这里启动流程已经分析完毕。
**Android Phone分析（四） **2010-01-27 17:52 
request流程 
1. 多路复用I/O机制的运转 
上文说到request是接收,是通过ril_event_loop中的多路复用I/O,也对初始化做了分析.现在我们来仔细看看这个机制如何运转. 
ril_event_set负责配置一个event,主要有两种event： 
ril_event_add添加使用多路I/O的event,它负责将其挂到队列,同时将event的通道句柄fd加入到watch_table,然后通过select等待.
ril_timer_add添加timer event,它将其挂在队列,同时重新计算最短超时时间. 
无论哪种add,最后都会调用triggerEvLoop来刷新队列,更新超时值或等待对象. 
刷新之后, ril_event_loop从阻塞的位置,select返回,只有两种可能,一是超时,二是等待到了某I/O操作. 
超时的处理在processTimeouts中,摘下超时的event,加入pending_list. 
检查有I/O操作的通道的处理在processReadReadies中,将超时的event加入pending_list. 
最后在firePending中,检索pending_list的event并依次执行event->func. 
这些操作完之后,计算新超时时间,并重新select阻塞于多路I/O. 
前面的初始化流程已分析得知,初始化完成以后,队列上挂了3个event对象,分别是： 
s_listen_event: 名为rild的socket,主要requeset & response通道 
s_debug_event: 名为rild-debug的socket,调试用requeset & response通道（流程与s_listen_event基本相同,后面仅分析s_listen_event）
s_wakeupfd_event: 无名管道,用于队列主动唤醒（前面提到的队列刷新,就用它来实现,请参考使用它的相关地方） 
2. request的传入和dispatch 
明白了event队列的基本运行流程,我们可以来看看request是怎么传入和dispatch的了. 
上 层的部分,核心代码在frameworks/base/telephony/java/com/android/internal/telephony /gsm/RIL.java,这是android java框架处理radio(gsm)的核心组件.本文因为主要关注rild,也就是驱动部分,所以这里只作简单介绍.
我们看一个具体的例子,RIL.java中的dial函数： 
public void 
dial (String address, int clirMode, Message result) 
{ 
RILRequest rr = RILRequest.obtain(RIL_REQUEST_DIAL, result); 
rr.mp.writeString(address); 
rr.mp.writeInt(clirMode); 
if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest)); 
send(rr); 
} 
rr是以RIL_REQUEST_DIAL为request号而申请的一个RILRequest对象.这个request号在java框架和rild库中共享（参考RILConstants.java中这些值的由来:)）
RILRequest初始化的时候,会连接名为rild的socket（也就是rild中s_listen_event绑定的socket）,初始化数据传输的通道. 
rr.mp 是Parcel对象,Parcel是一套简单的序列化协议,用于将对象（或对象的成员）序列化成字节流,以供传递参数之用.这里可以看到String address和int clirMode都是将依次序列化的成员.在这之前,rr初始化的时候,request号跟request的序列号（自动生成的递增数）,已经成为头两个 将被序列化的成员.这为后面的request解析打下了基础.
接下来是send到handleMessage的流程,send将rr直接传递给另 一个线程的handleMessage,handleMessage执行data = rr.mp.marshall()执行序列化操作, 并将data字节流写入到rild socket.
接下来回到我们的rild,select发现rild socket有了请求链接的信号,导致s_listen_event被挂入pending_list,执行event->func,即
static void listenCallback (int fd, short flags, void *param); 
接下来,s_fdCommand = accept(s_fdListen, (sockaddr *) &peeraddr, &socklen),获取传入的socket描述符,也就是上层的java RIL传入的连接.
然 后,通过record_stream_new建立起一个record_stream, 将其与s_fdCommand绑定, 这里我们不关注record_stream 的具体流程, 我们来关注command event的回调, processCommandsCallback函数, 从前面的event机制分析, 一旦s_fdCommand上有数据, 此回调函数就会被调用. （略过onNewCommandConnect的分析）
processCommandsCallback通过 record_stream_get_next阻塞读取s_fdCommand上发来的 数据, 直到收到一完整的request(request包的完整性由record_stream的机制保证), 然后将其送达processCommandBuffer.
进入processCommandBuffer以后,我们就正式进入了命令的解析部分. 每个命令将以RequestInfo的形式存在. 
typedef struct RequestInfo { 
int32_t token; //this is not RIL_Token 
CommandInfo *pCI; 
struct RequestInfo *p_next; 
char cancelled; 
char local; // responses to local commands do not go back to command process 
} RequestInfo; 
这 里的pRI就是一个RequestInfo结构指针, 从socket过来的数据流, 前面提到是Parcel处理过的序列化字节流, 这里会通过反序列化的方法提取出来. 最前面的是request号, 以及token域(request的递增序列号). 我们更关注这个request号, 前面提到, 上层和rild之间, 这个号是统一的. 它的定义是一个包含ril_commands.h的枚举, 在ril.cpp中
static CommandInfo s_commands[] = { 
#include "ril_commands.h" 
}; 
pRI直接访问这个数组, 来获取自己的pCI. 
这是一个CommandInfo结构: 
typedef struct { 
int requestNumber; 
void (*dispatchFunction) (Parcel &p, struct RequestInfo *pRI); 
int(*responseFunction) (Parcel &p, void *response, size_t responselen); 
} CommandInfo; 
基本解析到这里就完成了, 接下来, pRI被挂入pending的request队列, 执行具体的pCI->dispatchFunction, 进行详细解析. 
3. request的详细解析 
对dial而言, CommandInfo结构是这样初始化的: 
{RIL_REQUEST_DIAL, dispatchDial, responseVoid}, 
这 里执行dispatchFunction, 也就是dispatchDial这一函数.我们可以看到其实有很多种类的dispatch function, 比如dispatchVoid, dispatchStrings, dispatchSIM_IO等等, 这些函数的区别, 在于Parcel传入的参数形式,Void就是不带参数的,Strings是以string[]做参数,又如Dial等,有自己的参数解析方式,以此类 推.
request号和参数现在都有了,那么可以进行具体的request函数调用了. 
s_callbacks.onRequest(pRI->pCI->requestNumber, xxx, len, pRI)完成这一操作. 
s_callbacks 是上篇文章中提到的获取自libreference-ril的RIL_RadioFunctions结构指针,request请求在这里转入底层的 libreference-ril处理,handler是reference-ril.c中的onRequest.
onRequest进行一个简单的switch分发,我们依然来看RIL_REQUEST_DIAL 
流程是 onRequest-->requestDial-->at_send_command-->at_send_command_full-->at_send_command_full_nolock-->writeline
requestDial中将命令和参数转换成对应的AT命令,调用公共send command接口at_send_command. 
除 了这个接口之外,还有 at_send_command_singleline,at_send_command_sms,at_send_command_multiline 等,这是根据at返回值,以及发命令流程的类型来区别的.比如at+csq这类,需要at_send_command_singleline,而发送短 信,因为有prompt提示符">",传裸数据,结束符等一系列操作,需要专门用at_send_command_sms来实现.
然后执行at_send_command_full,前面几个接口都会最终到这里,再通过一个互斥的at_send_command_full_nolock调用,然后完成最终的写出操作,在writeline中,写出到初始化时打开的设备中.
writeline返回之后,还有一些操作,如保存type等信息,供response回来时候使用,以及一些超时处理. 不再详述. 
到这里,request的详细流程,就分析完毕了. 
**Android Phone分析（五） **2010-01-27 17:53 
response流程 
前文对request的分析， 终止在了at_send_command_full_nolock里的writeline操作，因为这里完成命令写出到硬件设备的操作，接下来就是等待硬件响应，也就是response的过程了。我们的分析也是从这里开始。
response信息的获取，是在第一篇初始化分析中，提到的readerLoop中。由readline函数以‘行'为单位接收上来。 
AT的response有两种，一是主动上报的，比如网络状态，短信，来电等都不需要经过请求，有一unsolicited词语专门描述。另一种才是真正意义上的response，也就是命令的响应。
这 里我们可以看到，所有的行，首先经过sms的自动上报筛选，因为短信的AT处理通常比较麻烦，无论收发都单独列出。这里是因为要即时处理这条短信消息（两 行，标志＋pdu），而不能拆开处理。处理函数为onUnsolicited（由s_unsolHandler指向），我们等下介绍。
除开sms的特例，所有的line都要经过processLine，我们来看看这个流程： 
processLine 
|----no cmd--->handleUnsolicited //主动上报 
|----isFinalResponseSuccess--->handleFinalResponse //成功,标准响应 
|----isFinalResponseError--->handleFinalResponse //失败，标准响应 
|----get '>'--->send sms pdu //收到>符号，发送sms数据再继续等待响应 
|----switch s_type--->具体响应 //命令有具体的响应信息需要对应分析 
我 们这里主要关注handleUnsolicited自动上报（会调用到前面smsUnsolicite也调用的onUnsolicite），以及 switch s_type具体响应信息，另外具体响应需要handleFinalResponse这样的标准响应来最终完成。
1. onUnsolicite（主动上报响应） 
static void onUnsolicited (const char *s, const char *sms_pdu)； 
短信的AT设计真是麻烦的主，以致这个函数的第二个参数完全就是为它准备的。 
response 的主要的解析过程，由at_tok.c中的函数完成，其实就是字符串按块解析，具体的解析方式由每条命令或上报信息自行决定。这里不再详 述，onUnsolicited只解析出头部(一般是+XXXX的形式)，然后按类型决定下一步操作，操作为 RIL_onUnsolicitedResponse和RIL_requestTimedCallback两种。
a)RIL_onUnsolicitedResponse： 
将 unsolicited的信息直接返回给上层。通过Parcel传递，将 RESPONSE_UNSOLICITED，unsolResponse（request号）写入Parcel先，然后通过 s_unsolResponses数组，查找到对应的responseFunction完成进一步的的解析，存入Parcel中。最终通过 sendResponse将其传递回原进程。流程：
sendResponse-->sendResponseRaw-->blockingWrite-->write to s_fdCommand(前面建立起来的和上层框架的socket连接)
这些步骤之后有一些唤醒系统等其他操作。不再详述。 
b)RIL_requestTimedCallback： 
通 过event机制（参考文章二）实现的timer机制，回调对应的内部处理函数。通过internalRequestTimedCallback将回调添 加到event循环，最终完成callback上挂的函数的回调。比如pollSIMState，onPDPContextListChanged等回 调， 不用返回上层， 内部处理就可以。
2. switch s_type（命令的具体响应）及handleFinalResponse（标准响应） 
命 令的类型（s_type）在send command的时候设置（参考文章二），有NO_RESULT，NUMERIC，SINGLELINE，MULTILINE几种，供不同的AT使用。比 如AT+CSQ是singleline, 返回at+csq=xx,xx，再加一行OK，比如一些设置命令，就是no_result, 只有一行OK或ERROR。
这几个类型的解析都很相仿，通过一定的判断（比较AT头标记等），如果是对应的响应，就通过 addIntermediate挂到一个临时结果sp_response->p_intermediates队列里。如果不是对应响应，那它其实应 该是穿插其中的自动上报，用onUnsolicite来处理。
具体响应，只起一个获取响应信息到临时结果，等待具体分析的作用。无论有无具体响应，最终都得以标准响应handleFinalResponse来完成，也就是接受到OK,ERROR等标准response来结束，这是大多数AT命令的规范。
handleFinalResponse 会设置s_commandcond这一object，也就是at_send_command_full_nolock等待的对象。到这里，响应的完整信息 已经完全获得，send command可以进一步处理返回的信息了（临时结果，以及标准返回的成功或失败，都在sp_response中）。
pp_outResponse参数将sp_response返回给调用at_send_command_full_nolock的函数。 
继续我们在文章二的分析的话，这个函数其实是requestDial，不过requestDial忽略了响应，所以我们另外看个例子，如requestSignalStrength，命令其实就是前面提到的at+csq：
可以看到确实是通过at_send_command_singleline来进行的操作，response在p_response中。 
p_response如果返回失败（也就是标准响应的ERROR等造成），则通过RIL_onRequestComplete发送返回数据给上层，结束命令。 
如果成功，则进一步分析p_response->p_intermediates， 同样是通过at_tok.c里的函数进行分析。并同样将结果通过RIL_onRequestComplete返回。
RIL_onRequestComplete： 
RIL_onRequestComplete和RIL_onUnsolicitedResponse很相仿，功能也一致。 
通 过Parcel来传递回上层，同样是先写入RESPONSE_SOLICITED（区别于 RESPONSE_UNSOLICITED），pRI->token(上层传下的request号），错误码（send command的错误，不是AT响应）。如果有AT响应，通过访问pRI->pCI->responseFunction来完成具体 response的解析，并写入Parcel。
然后通过同样的途径： 
sendResponse-->sendResponseRaw-->blockingWrite-->write to s_fdCommand 
完成最终的响应传递。 
到这里，我们分析了自动上报与命令响应，其实response部分，也就告一段落了。 
