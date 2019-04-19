# Android核心分析（18）-----Android电话系统之RIL-Java - xqhrs232的专栏 - CSDN博客
2011年03月31日 10:07:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：727标签：[android																[电话																[token																[command																[interface																[框架](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=token&t=blog)](https://so.csdn.net/so/search/s.do?q=电话&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx](http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx)
Android RIL-Java
    RIL-Java在本质上就是一个RIL代理，起到一个转发的作用，是Android Java概念空间中的电话系统的起点。在RIL-D的分析中，我们知道RILD建立了一个侦听套接口，等待RIL-Java的连接。一旦连接成功，RIL-JAVA就可发起一个请求，并等待应答，并将结构发送到目标处理对象。在RIL-Java中，这个请求称为RILRequest。为了直观起见，我还是不厌其烦的给出RIL-Java的框架图。
RIL-Java的大框架包含了四个方面：
Receiver，Sender，CommandInterface，异步通知机制
（1） Command Interface
   在ril.java源代码中，我们可以看到RIL-JAVA对象提供了如下的Command Interface：
…
getlccCardStatus
getCurrrentCalls
dial
acceptCall
rejectCall
sendDTMF
sendSMS
setupDataCall
setRadioPower
…
为什么要定义这些接口呢？这函数接口不是凭空捏造出来的，这些都是电话的基本功能的描述，是对Modem AT指令的提炼抽象。大多数Modem都是根据通讯协议提供接口，我们如果不熟悉通讯协议，请参阅3GPP的相关文档，以及自己使用的Modem的SPEC说明。
V.25ter AT Commands
  3GPP 07.07 AT Comamnds-General commands
3GPP 07.07 AT Comamnds-Call Control commans
3GPP 07.07 AT Comamnds-Network Service related commands
3GPP 07.07 AT Comamnds-MT control and status command
3GPP 07.07 AT Comamnds-GPRS Commands
3GPP 07.07 Mobile Termination Errors
3GPP 07.05 SMS AT Commands
（2）Receiver
Receiver连接到RILD的服务套接口，接收读取RILD传递过来的Response Parcel。Response分为两种类型，一种是URC，一种是命令应答。对于URC将会直接分发到通知注册表中的Handler。而命令应答则通过Receiver的异步通知机制传递到命令的发送者进行相应处理。
（3）Sender
Sender应该分为两部分架构，
上层函数调用Command Interface将请求消息发送到Sender的架构。
Sender接收到EVENT_SEND消息后，将请求发送到RILD的架构。
（4）异步应答框架
    对于异步应答来讲，命令的发起者发送后，并不等待应答就返回，应答的回应是异步的，处理结果通过消息的方式返回。站在设计者的角度思考如何设计合适的框架来完成异步通讯的功能呢？对于异步系统我们首先应该考虑的是如何标识命令和结果，让命令和结果有一个对应关系，还有命令没有响应了，如何管理命令超时？让我们来看看Android设计者如何完成这些工作。
Android设计者利用了Result Message 和RILRequest对象来完成Request和Result的对应对于关系。在上层做调用的时候生成Result Message对象传递到ril_java，并在Modem有应答后，通过Result Message对象带回结果。如何保证该应答是该RILRequest的呢？Android设计者还提供了一个Token（令牌）的概念。在源代码中RILRequest的mSerail就用作了Token。Token用来唯一标识每次发送的请求，并且Token将被传递到RILD，RILD在组装应答是将Token写入，并传回到ril-java，ril-java根据该Token找到相应的Request对象。
（4.1）RIL命令的发送模式
协议的真正实现是在rild中，RIL-JAVA更多的是一个抽象和代理，我们在研究源代码的过程中就会体会到到RIL-JAVA中的命令函数都有一个共同的框架。
SendXxxCmd(传入参数Data，传出参数result){
组合RILRequest(请求号，result，mSerail)
Data->RR
send(RILRequest)： Message
}
1)RILRequest
请求号：
request将传递到RILD用以标识命令，request代表某个功能。例如拨叫的request号为：RIL_REQUEST_DIAL。在libs/telephony/ril_commands.h有定义。[RILRequest.obtain@RILRequest](mailto:RILRequest.obtain@RILRequest)根据命令请求号,传入参数Result Message，mSerail构造了一个RILRequest。Result Message将带回应答信息回到命令的发起者。
mSerail：
Android使用了一个RILRequest对象池来管理Andoird RILRequest。mSerail是一个递增的变量，用来唯一标识一个RILRequest。在发送时正是用了该变量为Token,在rild层看到的token就是该mSerail。
EVENT_END:
[EVENT_END@handleMessage@RILSender@RIL.java](mailto:EVENT_END@handleMessage@RILSender@RIL.java)
2)发送步骤：
第一步：
生成RILRequest，此时将生成m_Serial（请求的Token)并将请求号，数据，及其Result Message 对象填入到RILRequest中
第二步：
使用send将RILRequest打包到EVENT_SEND消息中发送到到RIL Sender Handler，
第三步：
RilSender 接收到EVENT_SEND消息，将RILRequest通过套接口发送到RILD，同时将RILRequest保存在mRequest中以便应答消息的返回。
（4.2） 接收模式
第一步：分析接收到的Parcel，根据类型不同进行处理。
第二步：根据数据中的Token（mSerail),反查mRequest,找到对应的请求信息。
第三步：将是数据转换成结果数据。
第四步：将结果放在RequestMessage中发回到请求的发起者。
4.3)详细的GSMCallTracker,RIL-Java函数对照
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx](http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx](http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx](http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx](http://blog.csdn.net/maxleng/archive/2010/05/14/5593759.aspx)
