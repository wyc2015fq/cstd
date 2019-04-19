# Andriod Phone模块相关 - xqhrs232的专栏 - CSDN博客
2011年12月11日 22:31:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：903标签：[null																[command																[socket																[buffer																[手机																[header](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://lisaguo.iteye.com/blog/920065](http://lisaguo.iteye.com/blog/920065)
**Andriod Phone模块相关（总览） **2010-01-30 13:50 
1、从java端发送at命令的处理流程。 
2、unsolicited 消息从modem上报到java的流程。 
3、猫相关的各种状态的监听和通知机制。 
4、通话相关的图标变换的工作原理。 
5、gprs拨号上网的通路原理。 
6、通话相关的语音通路切换原理、震动接口。 
7、通话相关的notification服务。 
8、通话相关的各种server。 
**Andriod Phone模块相关（一） **2010-01-30 14:52 
第一部分：从java端发送at命令的处理流程。 
拨出电话流程： 
1、Contacts的AndroidManifest.xml 中android:process="android.process.acore"说明此应用程序运行在acore进程中。
DialtactsActivity 的intent-filter的action属性设置为main，catelog属性设置为launcher，所以此activity能出现在主菜单中，并且是点击此应用程序的第一个界面。dialtactsactivity包含四个tab，分别由TwelveKeyDialer、 RecentCallsListActivity，两个activity-alias DialtactsContactsEntryActivity和DialtactsFavoritesEntryActivity分别表示联系人和收藏
 tab，但是正真的联系人列表和收藏是由ContactsListActivity负责。 
2、进入TwelveKeyDialer 中OnClick方法，按住的按钮id为：R.id.dialButton，执行placecall()方法： 
Intent intent = new Intent(Intent.ACTION_CALL_PRIVILEGED,Uri.fromParts("tel", number, null));
intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK); 
startActivity(intent); 
3、 intert.ACTION_CALL_PRIVILEGED实际字符串为 android.intent.action.CALL_PRIVILEGED，通过查找知道了packegs/phone下面的 AndroidManifest.xml中PrivilegedOutgoingCallBroadcaster activity-alias设置了intent-filter，所以需要找到其targetactivity为 OutgoingCallBroadcaster。所以进入OutgoingCallBroadcaster的onCreate（）中：
String action = intent.getAction(); 
String number = PhoneNumberUtils.getNumberFromIntent(intent, this); 
if (number != null) { 
number = PhoneNumberUtils.convertKeypadLettersToDigits(number); 
number = PhoneNumberUtils.stripSeparators(number); 
} 
final boolean emergencyNumber = 
(number != null) && PhoneNumberUtils.isEmergencyNumber(number); 
获取过来的Action以及Number，并对Action以及Number类型进行判断。 
//如果为callNow = true；则启动InCall界面： 
intent.setClass(this, InCallScreen.class); 
startActivity(intent); 
并发送广播给OutgoingCallReceiver： 
Intent broadcastIntent = new Intent(Intent.ACTION_NEW_OUTGOING_CALL); 
if (number != null) broadcastIntent.putExtra(Intent.EXTRA_PHONE_NUMBER, number); 
broadcastIntent.putExtra(EXTRA_ALREADY_CALLED, callNow); 
broadcastIntent.putExtra(EXTRA_ORIGINAL_URI, intent.getData().toString()); 
sendOrderedBroadcast(broadcastIntent, PERMISSION, 
new OutgoingCallReceiver(), null, Activity.RESULT_OK, number, null); 
4、 Intent.ACTION_NEW_OUTGOING_CALL实际字符串 android.intent.action.NEW_OUTGOING_CALL，通过查找知道了packegs/phone下面的 androidmanifest.xml中OutgoingCallReceiver Receiver接收此intent消息。找到OutgoingCallBroadcaster类中的内部类OutgoingCallReceiver，执行onReceive()函数：
执行doReceive(context, intent);方法： 
获取传给来的号码，根据PhoneApp的实例获取PhoneType等。最后启动InCall界面： 
Intent newIntent = new Intent(Intent.ACTION_CALL, uri); 
newIntent.putExtra(Intent.EXTRA_PHONE_NUMBER, number); 
newIntent.setClass(context, InCallScreen.class); 
newIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK); 
5、请求拨号的java部分流程 
6、请求拨号的c/c++部分流程 
6.1、初始化事件循环，启动串口监听，注册socket监听。 
rild.c->main() 
(1)、RIL_startEventLoop 
//建立事件循环线程 
ret = pthread_create(&s_tid_dispatch, &attr, eventLoop, NULL); 
//注册进程唤醒事件回调 
ril_event_set (&s_wakeupfd_event, s_fdWakeupRead, true, 
processWakeupCallback, NULL); 
rilEventAddWakeup (&s_wakeupfd_event); 
//建立事件循环 
ril_event_loop 
for (;;) { 
... 
n = select(nfds, &rfds, NULL, NULL, ptv); 
// Check for timeouts 
processTimeouts(); 
// Check for read-ready 
processReadReadies(&rfds, n); 
// Fire away 
firePending(); 
} 
(2)、funcs = rilInit(&s_rilEnv, argc, rilArgv);//实际是通过动态加载动态库的方式执行reference-ril.c中的RIL_Init
//单独启动一个线程读取串口数据 
ret = pthread_create(&s_tid_mainloop, &attr, mainLoop, NULL); 
fd = open (s_device_path, O_RDWR); 
ret = at_open(fd, onUnsolicited); 
ret = pthread_create(&s_tid_reader, &attr, readerLoop, &attr); 
RIL_requestTimedCallback(initializeCallback, NULL, &TIMEVAL_0); 
在initializeCallback中执行的程序： 
setRadioState (RADIO_STATE_OFF); 
at_handshake(); 
/* note: we don't check errors here. Everything important will 
be handled in onATTimeout and onATReaderClosed */ 
/* atchannel is tolerant of echo but it must */ 
/* have verbose result codes */ 
at_send_command("ATE0Q0V1", NULL); 
/* No auto-answer */ 
at_send_command("ATS0=0", NULL); 
... 
//注册rild socket端口事件监听到事件循环中 
(3)、RIL_register(funcs); 
s_fdListen = android_get_control_socket(SOCKET_NAME_RIL); 
ret = listen(s_fdListen, 4); 
ril_event_set (&s_listen_event, s_fdListen, false, 
listenCallback, NULL);//将此端口加入事件select队列 
rilEventAddWakeup (&s_listen_event); 
如果rild socket端口有数据来了将执行listencallback函数 
listencallback 
//为此客户端连接创建新的监听句柄，s_fdListen继续监听其他客户端的连接。 
s_fdCommand = accept(s_fdListen, (sockaddr *) &peeraddr, &socklen); 
ril_event_set (&s_commands_event, s_fdCommand, 1, 
processCommandsCallback, p_rs);//将此端口加入事件select队列 
rilEventAddWakeup (&s_commands_event); 
6.2、socket监听，收到dial的socket请求 
processCommandsCallback 
//读数据到p_record中 
ret = record_stream_get_next(p_rs, &p_record, &recordlen); 
processCommandBuffer(p_record, recordlen); 
p.setData((uint8_t *) buffer, buflen); 
// status checked at end 
status = p.readInt32(&request); 
status = p.readInt32 (&token);//请求队列中的序号 
pRI = (RequestInfo *)calloc(1, sizeof(RequestInfo)); 
pRI->token = token; 
/* 
包含#include "ril_commands.h"语句，结构体如下： 
typedef struct { 
int requestNumber; 
void (*dispatchFunction) (Parcel &p, struct RequestInfo *pRI); 
int(*responseFunction) (Parcel &p, void *response, size_t responselen); 
} CommandInfo; 
*/ 
pRI->pCI = &(s_commands[request]); 
pRI->p_next = s_pendingRequests; 
s_pendingRequests = pRI; 
pRI->pCI->dispatchFunction(p, pRI); 
//假设是接收了dial指令,pRI->PCI->dispatchFunction(p,pRI)，调用dispatchDial (p,pRI) 
dispatchDial (p,pRI) 
s_callbacks.onRequest(pRI->pCI->requestNumber, &dial, sizeof(dial), pRI); 
in reference-ril.c onRequest() 
... 
switch (request) { 
case RIL_REQUEST_DIAL: 
requestDial(data, datalen, t); 
asprintf(&cmd, "ATD%s%s;", p_dial->address, clir); 
ret = at_send_command(cmd, NULL); 
err = at_send_command_full (command, NO_RESULT, NULL, NULL, 0, pp_outResponse); 
err = at_send_command_full_nolock(command, type, responsePrefix, smspdu,timeoutMsec, sponse);
err = writeline (command); 
//此处等待，直到收到成功应答或失败的应答,如：ok,connect,error cme等 
err = pthread_cond_wait(&s_commandcond, &s_commandmutex); 
waiting.... 
waiting.... 
/* success or failure is ignored by the upper layer here. 
it will call GET_CURRENT_CALLS and determine success that way */ 
RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0); 
p.writeInt32 (RESPONSE_SOLICITED); 
p.writeInt32 (pRI->token); 
errorOffset = p.dataPosition(); 
p.writeInt32 (e); 
if (e == RIL_E_SUCCESS) { 
/* process response on success */ 
ret = pRI->pCI->responseFunction(p, response, responselen); 
if (ret != 0) { 
p.setDataPosition(errorOffset); 
p.writeInt32 (ret); 
} 
} 
sendResponse(p); 
sendResponseRaw(p.data(), p.dataSize()); 
blockingWrite(fd, (void *)&header, sizeof(header)); 
blockingWrite(fd, data, dataSize); 
6.4、串口监听收到atd命令的应答"OK"或"no carrier"等 
readerLoop() 
line = readline(); 
processLine(line); 
handleFinalResponse(line); 
pthread_cond_signal(&s_commandcond);//至此，前面的等待结束，接着执行RIL_onRequestComplete函数 
6.5、java层收到应答后的处理,以dial为例子. 
ril.java->RILReceiver.run() 
for(;;) 
{ 
... 
length = readRilMessage(is, buffer); 
p = Parcel.obtain(); 
p.unmarshall(buffer, 0, length); 
p.setDataPosition(0); 
processResponse(p); 
type = p.readInt(); 
if (type == RESPONSE_SOLICITED) { 
processSolicited (p); 
serial = p.readInt(); 
rr = findAndRemoveRequestFromList(serial); 
rr.mResult.sendToTarget(); 
...... 
} 
CallTracker.java->handleMessage (Message msg) 
switch (msg.what) { 
case EVENT_OPERATION_COMPLETE: 
ar = (AsyncResult)msg.obj; 
operationComplete(); 
cm.getCurrentCalls(lastRelevantPoll); 
**Andriod Phone模块相关（二） **2010-01-30 15:09 
第二部分：unsolicited 消息从modem上报到java的流程。 
C++部分： 
readerLoop() 
line = readline(); 
processLine(line); 
handleUnsolicited(line); 
if (s_unsolHandler != NULL) { 
s_unsolHandler (line1, line2);//实际执行的是void onUnsolicited (const char *s, const char *sms_pdu)
if (strStartsWith(s,"+CRING:")|| strStartsWith(s,"RING") 
|| strStartsWith(s,"NO CARRIER") || strStartsWith(s,"+CCWA") ) 
RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED, NULL, 0); 
p.writeInt32 (RESPONSE_UNSOLICITED); 
p.writeInt32 (unsolResponse); 
ret = s_unsolResponses[unsolResponseIndex].responseFunction(p, data, datalen); 
ret = sendResponse(p); 
sendResponseRaw(p.data(), p.dataSize()); 
ret = blockingWrite(fd, (void *)&header, sizeof(header)); 
blockingWrite(fd, data, dataSize); 
Java部分： 
ril.java->RILReceiver.run() 
for(;;) 
{ 
... 
length = readRilMessage(is, buffer); 
p = Parcel.obtain(); 
p.unmarshall(buffer, 0, length); 
p.setDataPosition(0); 
processResponse(p); 
processUnsolicited (p); 
response = p.readInt(); 
switch(response) { 
... 
case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED: ret = responseVoid(p); break; 
... 
} 
switch(response) { 
case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED: 
if (RILJ_LOGD) unsljLog(response)； 
mCallStateRegistrants 
.notifyRegistrants(new AsyncResult(null, null, null))； 
... 
} 
**Andriod Phone模块相关（三、四） **2010-02-01 10:51 
第三部分：猫相关的各种状态的监听和通知机制 
第四部分：通话相关的图标变换的工作原理。 
A. 注册监听部分 
B.事件通知部分 
注：所有的状态改变通知都在TelephonyRegistry中处理，详见该类源码。 
手机SIM卡功能解析 
2010-02-01 17:25 
SIM卡是GSM手机特有的用户身份的象征。 
那么，SIM卡到底具有哪些功能，其原理如何呢？下面作一简要描述。 
SIM卡作为用户身份的象征，主要含有以下两种信息：IMSI号和鉴权、加密算法。 
IMSI号全称为国际移动台用户识别号，与IMEI国际移动设备识别号是完全不同的两个概念。IMSI号是固化在SIM卡内部存储芯片上的号码。当客户申请入网时，电信营业人员随意拿来一张崭新的SIM卡，将卡上标注的15位IMSI号，对应记录在用户挑选的号码资料中，输入电脑建立档案。这就是 GSM系统方便快捷的入网方式。
IMEI号则是一部手机机身内部固有的一个号码，反应这部手机的出厂地、所属厂商等一系列信息。 
这两个号码的不同体现了GSM系统机、号分开的原则。 
GSM系统具有良好的保密性还体现在SIM卡上。在用户上网通话时，需要在空中传送IMSI号码以便鉴权。IMSI号码在空中传送是经过SIM卡中的鉴权、加密运算后发送的。经过这些复杂的运算，破译基本上是不可能的。这也是GSM系统优于ETACS系统的一大体现。
从外观上看，SIM卡有大、小卡之分，这是为满足不同手机的不同尺寸需求而设计的。但随着手机市场日益小巧、轻便的发展趋势，越来越多的厂商淘汰了大卡机型，小卡越来越受到青睐。
观察SIM卡可以看到每张卡上，都有8个金属触脚，它们分别有如下功能，见图1。 
图1 SIM卡引脚 
SIM卡的供电有两种：5V和3V。早期的SIM卡一般是5V供电。随着人们对电池使用时间的要求日趋加长，厂家采取了各种手法来降低手机的用电量，包括将CPU由原来的5V左右供电降至3V左右，随之手机整体机身的供电也基本上降到3V左右，这样SIM卡供电电压的下降也就势在必行了。目前，许多SIM卡可以兼容两种电压供电，这是为了适应过渡时期的需要。
另外，SIM卡的容量也不相同，这取决于SIM卡内部存储芯片的内存容量大小。卡的容量体现在用户使用电话簿功能时能往SIM卡上存多少条记录。 
在日常使用时，有时会出现"SIM卡不被接受"、"请插入SIM卡"等不正常的现象。这时，我们可以将SIM卡从机内取出，用橡皮轻轻地擦卡面。切不可用尖锐的东西刮卡面，以免造成卡触脚不平而接触不良，甚至彻底损坏SIM卡。如果擦拭后仍无法正常使用，则应将手机连卡送到专业维修点，让维修人员检查。
