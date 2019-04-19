# Android 无线接口层（Radio Layer Interface） - xqhrs232的专栏 - CSDN博客
2011年12月11日 22:54:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：832标签：[android																[interface																[layer																[通讯																[token																[电话](https://so.csdn.net/so/search/s.do?q=电话&t=blog)](https://so.csdn.net/so/search/s.do?q=token&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=layer&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://liangxh2008.blog.163.com/blog/static/1124116792010029490222/](http://liangxh2008.blog.163.com/blog/static/1124116792010029490222/)
INDEX
介绍
RIL 初始化
RIL 交互
主动请求
被动请求
实现RIL
RIL初始化
RIL函数
RIL 主动命令请求
RIL 被动请求的命令
**介绍**　　Android的无线接口层（RIL）提供了Android电话服务（android.telephony）与无线电硬件之间的抽象层。RIL是通讯无关的，提供基于GSM的网络支持。
　　下图显示了RIL位于Android电话系统架构中的位置。图1
![Android 无线接口层（Radio Layer Interface） - liangxh2008 - liangxh2008的博客](http://img.blog.163.com/photo/1UU0lZzWBD3sr0Yvt4t_SQ==/4869235622118566849.jpg)
　　实线框表示Android部分，虚线框表示合作伙伴所专用的部分。
　　RIL包含两个基本部件：
　　RIL守护进程（RIL Daemon）：RIL守护进程初始化Vendor RIL，管理所有来自Android通讯服务的通讯，将其作为被请求的命令（solicited commands）调度给Vendor RIL。
　　Vendor RIL：ril.h文件中的无线电专用Vendor RIL掌管着所有和无线电硬件的通讯，并且通过未被请求的命令（unsolicited commands）分发给RIL 守护进程。
**RIL初始化**　　Android在启动时初始化通讯栈和Vendor RIL，描述如下：
　　1. RIL守护进程读取rild.lib路径和rild.libargs系统参数，决定应该使用的Vendor RIL库和向Vendor RIL提供的初始化参数
　　2. RIL守护进程加载Vendor RIL库，执行RIL_Init初始化RIL并为RIL函数获取参数
　　3. RIL守护进程调用Android通讯栈中RIL_register，为Vendor RIL函数提供参考。
　　RIL守护进程源码请参考//device/commands/rild/rild.c
**RIL 交互**
　　RIL句柄提供了两种交互方式：
　　主动请求命令（Solicited commands）：主动请求命令来自RIL lib，比如DIAL和HANGUP。
被动请求命令（Unsolicited responses）：被动请求命令来自基带，比如CALL_STATE_CHANGED 和 NEW_SMS。
**主动请求**　　以下代码片段属于主动请求命令
　　void OnRequest (int request_id, void *data, size_t datalen, RIL_Token t);
　　void OnRequestComplete (RIL_Token t, RIL_Error e, void *response, size_t responselen);
　　有超过60个主动请求命令：
* SIM PIN，IO和IMSI/IMEI（11）
* 电话状态和动作（拨号，应答，静音…）（16）
* 网络状态查询（4）
* 网络设置（禁止，转发，选择…）（12）
* 短信（3）
* PDP连接（4）
* 电源和复位（2）
* 辅助服务（5）
* 供应商定义及其支持（4）
　　下图表明了Android系统一个主动请求的电话过程，图2
![Android 无线接口层（Radio Layer Interface） - liangxh2008 - liangxh2008的博客](http://img.blog.163.com/photo/3FTw0UsTp1vVWqVMz5LULw==/4869235622118566850.jpg)
**被动请求**
　　以下代码片段属于被动请求命令
　　void OnUnsolicitedResponse (int unsolResponse, void *data, size_t datalen);
　　有超过10条被动请求命令：
* 网络状态改变（4）
* 新短信通知（3）
* 新USSD通知（2）
* 信号强度和时间改变（2）
　　下图表明Android系统中一个被动请求的电话过程：图3
![Android 无线接口层（Radio Layer Interface） - liangxh2008 - liangxh2008的博客](http://img.blog.163.com/photo/2Js6iox4T6DHFYjDaqsBag==/4869235622118566851.jpg)
**实现RIL**
　　为了实现一个通讯专用RIL，需要执行一系列函数以创建一个共享库，保证Android能够相应无线通信请求。所需要的函数被定义在RIL头部（/include/telephony/ril.h）
　　Android通讯接口是通讯无关的，Vendor RIL可以使用任意协议进行无线通讯。Android提供了一个参考Vendor RIL，使用的是贺式（Hayes）AT命令设备，可作为一个商用的快速入门指导以及通讯测试使用。RIL参考源码在/commands/reference-ril/。
　　通常将你自己的Vendor RIL编译为以下形式：
　　libril-<companyname>-<RIL version>.so
　　比如：
　　libril-acme-124.so
　　其中：
　　 libril：所有vendor RIL的开头；
　　 <companyname>：专用公司缩写
　　 <RIL version>：RIL版本number
　　 so：文件扩展
**RIL初始化**　　特定的Vendor RIL必须定义一个初始化函数，提供一系列句柄函数以处理每一个通讯请求。Android RIL守护进程会在启动时调用RIL_Init以初始化RIL。
　　RIL_RadioFunctions *RIL_Init (RIL_Env* env, int argc, char **argv);
　　 RIL_Init 返回一个RIL_RadioFunctions结构体包含无线电函数指针。
　　type structure {
　　 int RIL_version;
　　 RIL_RequestFunc onRequest;
　　 RIL_RadioStateRequest onStateRequest;      
　　 RIL_Supports supports;
　　 RIL_Cancel onCancel;
　　 RIL_GetVersion getVersion;
　　} 
　　RIL_RadioFunctions;
**RIL函数**
　　ril.h定义了RIL状态和变量，比如RIL_UNSOL_STK_CALL_SETUP, RIL_SIM_READY, RIL_SIM_NOT_READY，具体函数描述见下表。忽略头文件细节。
　　RIL主动命令请求
　　Vendor RIL必须提供下表中的函数用以发送主动命令。RIL主动命令请求类型定义在ril.h的RIL_REQUEST_prefix中。
名称
描述
void (*RIL_RequestFunc) (int request, void *data, size_t datalen, RIL_Token t);
RIL主动命令入口指针，必须能够处理各种RIL主动请求（定义于ril.h的RIL_REQUEST_ prefix）
* request 是一种 RIL_REQUEST_* 
* data 是一个指向RIL_REQUEST_*数据的指针 
* t 应当被用于RIL_onResponse的后续调用
* datalen 由调用者所有，应当由被调者修改或释放 
必须调用RIL_onRequestComplete()函数完成通讯。?RIL_onRequestComplete() 在这个函数返回前或之后可能被任意线程调用。这个函数总会调用同一个线程，因此返回到这里意味着无线通讯准备去处理其他命令（无论前面命令是否完成传输）。
RIL_RadioState (*RIL_RadioStateRequest)();
这个函数应该返回当前通讯同步状态
int (*RIL_Supports)(int requestCode);
如果提供指定RIL_REQUEST代码，返回1，否则返回0.
void (*RIL_Cancel)(RIL_Token t);
本函数用来指示取消一个待处理请求。函数将被一个独立线程所调用，而不是RIL_RequestFunc函数。
一旦取消，被调用者应当尽量放弃请求并在这之后调用RIL_onRequestComplete 函数的RIL_Errno CANCELLED 。
响应请求后调用 RIL_onRequestComplete 并产生其他结果是可以被接受的，但会被忽略(理应忽略被取消的请求)。
RIL_Cancel 调用应该被立刻返回，不需要等待取消。
const char * (*RIL_GetVersion) (void);
向你的Vendor RIL返回版本字符串 
　　Vendor RIL使用以下回调函数与Android RIL守护进程通讯。
名称
描述
void RIL_onRequestComplete(RIL_Token t, RIL_Errno e, void *response, size_t responselen);
* t 是之前通讯传递至RIL_Notification的参数
* 如果e != SUCCESS，则可以没有相应，并且被忽略
* response 由调用者所有，应当由被调用者修改或者释放。 
* RIL_onRequestComplete 将尽快返回 
void RIL_requestTimedCallback (RIL_TimedCallback callback, void *param, const struct timeval *relativeTime);
用户指定的回调函数的线程中，RIL_RequestFunc 函数被调用。
如果指定了relativeTime，那么回调前将等待一个一个特定的时间值。
如果 relativeTime 为空，或者指针指向了一个空的结构体，回调函数会尽快被执行。
　　RIL被动命令
　　下表函数是Vedor RIL使用的回调函数，用来唤醒被动命令在Android平台的相应机制。
名称
描述
void RIL_onUnsolicitedResponse(int unsolResponse, const void *data, size_t datalen);
* unsolResponse 是 RIL_UNSOL_RESPONSE_* 其中之一
* data 是指向RIL_UNSOL_RESPONSE_*数据的指针 
* data被调用者所有，应当由被调用者修改或者释放
阿虚（Rockie Cheng）翻译，如果有些地方看不懂，请参考英文原文，我翻译的太烂
[http://www.netmite.com/android/mydroid/development/pdk/docs/telephony.html](http://www.netmite.com/android/mydroid/development/pdk/docs/telephony.html)
