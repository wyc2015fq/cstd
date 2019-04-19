# Android GSM驱动模块详细分析 - xqhrs232的专栏 - CSDN博客
2011年03月15日 17:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：654
原文地址::[http://os.51cto.com/art/200903/116131.htm](http://os.51cto.com/art/200903/116131.htm)
Android的RIL驱动模块， 在hardware/ril目录下，一共分rild，libril.so以及librefrence_ril.so三个部分，另有一radiooptions可供自动或手动调试使用。都依赖于include目录中ril.h头文件。目前cupcake分支上带的是gsm的支持，另有一cdma分支，这里分析的是gsm驱动。
GSM模块，由于Modem的历史原因，AP一直是通过基于串口的AT命令与BB交互。包括到了目前的一些edge或3g模块，或像omap这类ap,bp集成的芯片，已经使用了USB或其他等高速总线通信，但大多仍然使用模拟串口机制来使用AT命令。这里的RIL(Radio Interface Layer)层，主要也就是基于AT命令的操作，如发命令，response解析等。（gprs等传输会用到的MUX协议等在这里并没有包含，也暂不作介绍。）
以下是详细分析，本文主要涉及基本架构和初始化的内容：
首先介绍一下rild与libril.so以及librefrence_ril.so的关系：
1. rild：
仅实现一main函数作为整个ril层的入口点，负责完成初始化。
2. libril.so：
与rild结合相当紧密，是其共享库，编译时就已经建立了这一关系。组成部分为ril.cpp，ril_event.cpp。libril.so驻留在rild这一守护进程中，主要完成同上层通信的工作，接受ril请求并传递给librefrence_ril.so， 同时把来自librefrence_ril.so的反馈回传给调用进程。
3. librefrence_ril.so：
rild通过手动的dlopen方式加载，结合稍微松散，这也是因为librefrence.so主要负责跟Modem硬件通信的缘故。这样做更方便替换或修改以适配更多的Modem种类。它转换来自libril.so的请求为AT命令，同时监控Modem的反馈信息，并传递回libril.so。在初始化时， rild通过符号RIL_Init获取一组函数指针并以此与之建立联系。
4. radiooptions：
radiooptiongs通过获取启动参数， 利用socket与rild通信，可供调试时配置Modem参数。
接下来分析初始化流程，主入口是rild.c中的main函数，主要完成三个任务：
1. 开启libril.so中的event机制， 在RIL_startEventLoop中，是最核心的由多路I/O驱动的消息循环。
2. 初始化librefrence_ril.so，也就是跟硬件或模拟硬件modem通信的部分（后面统一称硬件）， 通过RIL_Init函数完成。
3. 通过RIL_Init获取一组函数指针RIL_RadioFunctions， 并通过RIL_register完成注册，并打开接受上层命令的socket通道。
首先看第一个任务，也就是RIL_startEventLoop函数。RIL_startEventLoop在ril.cpp中实现， 它的主要目的是通过pthread_create(&s_tid_dispatch, &attr, eventLoop, NULL)建立一个dispatch线程，入口点在eventLoop. 而eventLoop中，会调ril_event.cpp中的ril_event_loop（）函数，建立起消息(event)队列机制。
我们来仔细看看这一消息队列的机制，这些代码都在ril_event.cpp中。
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->void ril_event_init();
void ril_event_set(struct ril_event
* ev, int fd, bool persist, ril_event_cb func,
void* param);
void ril_event_add(struct ril_event
* ev);
void ril_timer_add(struct ril_event
* ev, struct timeval
* tv);
void ril_event_del(struct ril_event
* ev);
void ril_event_loop();
struct ril_event {
struct ril_event *next;
struct ril_event *prev;
int fd;
int index;
bool persist;
struct timeval timeout;
ril_event_cb func;
void*param;
};
每个ril_event结构，与一个fd句柄绑定（可以是文件，socket，管道等），并且带一个func指针去执行指定的操作。
具体流程是： ril_event_init完成后，通过ril_event_set来配置一新ril_event，并通过ril_event_add加入队列之中（实际通常用rilEventAddWakeup来添加），add会把队列里所有ril_event的fd，放入一个fd集合readFds中。这样ril_event_loop能通过一个多路复用I/O的机制（select）来等待这些fd， 如果任何一个fd有数据写入，则进入分析流程processTimeouts()，processReadReadies(&rfds,
 n)，firePending()。 后文会详细分析这些流程。
另外我们可以看到， 在进入ril_event_loop之前， 已经挂入了一s_wakeupfd_event， 通过pipe的机制实现的， 这个event的目的是可以在一些情况下，能内部唤醒ril_event_loop的多路复用阻塞，比如一些带timeout的命令timeout到期的时候。
至此第一个任务分析完毕，这样便建立起了基于event队列的消息循环，稍后便可以接受上层发来的的请求了（上层请求的event对象建立，在第三个任务中）。
接下来看第二个任务，这个任务的入口是RIL_Init, RIL_Init首先通过参数获取硬件接口的设备文件或模拟硬件接口的socket. 接下来便新开一个线程继续初始化， 即mainLoop。
mainLoop的主要任务是建立起与硬件的通信，然后通过read方法阻塞等待硬件的主动上报或响应。在注册一些基础回调（timeout,readerclose）后，mainLoop首先打开硬件设备文件，建立起与硬件的通信，s_device_path和s_port是前面获取的设备路径参数，将其打开（两者可以同时打开并拥有各自的reader，这里也很容易添加双卡双待等支持）。
接下来通过at_open函数建立起这一设备文件上的reader等待循环，这也是通过新建一个线程完成， ret = pthread_create(&s_tid_reader, &attr, readerLoop, &attr)，入口点readerLoop。
AT命令都是以/r/n或/n/r的换行符来作为分隔符的，所以readerLoop是line驱动的，除非出错，超时等，否则会读到一行完整的响应或主动上报，才会返回。这个循环跑起来以后，我们基本的AT响应机制已经建立了起来。它的具体分析，包括at_open中挂接的ATUnsolHandler, 我们都放到后面分析response的连载文章里去。
有了响应的机制（当然，能与硬件通信也已经可以发请求了），通过RIL_requestTimedCallback(initializeCallback, NULL, &TIMEVAL_0)，跑到initializeCallback中，执行一些Modem的初始化命令，主要都是AT命令的方式。发AT命令的流程，我们放到后面分析request的连载文章里。这里可以看到，主要是一些参数配置，以及网络状态的检查等。至此第二个任务分析完毕，硬件已经可以访问了。
最后是第三个任务。第三个任务是由RIL_Init的返回值开始的，这是一个RIL_RadioFunctions结构的指针。
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->typedef struct {
int version;        /* set to RIL_VERSION
*/
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
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->ril_event_set (&s_listen_event, s_fdListen,
false,
listenCallback, NULL);
rilEventAddWakeup (&s_listen_event);
这样将两个socket加到任务一中建立起来多路复用I/O的检查句柄集合中，一旦有上层来的（调试）请求，event机制便能响应处理了。到这里启动流程已经分析完毕。
