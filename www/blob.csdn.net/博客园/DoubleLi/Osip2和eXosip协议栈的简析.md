# Osip2和eXosip协议栈的简析 - DoubleLi - 博客园






Osip2是一个开放源代码的sip协议栈，是开源代码中不多使用C语言写的协议栈之一，它具有短小简洁的特点，专注于sip底层解析使得它的效率比较高。

eXosip是Osip2的一个扩展协议集，它部分封装了Osip2协议栈，使得它更容易被使用。



一、介绍



Osip2是一个开放源代码的sip协议栈，是开源代码中不多使用C语言写的协议栈之一，它具有短小简洁的特点，专注于sip底层解析使得它的效率比较高。但缺点也很明显，首先就是可用性差，没有很好的api封装，使得上层应用在调用协议栈时很破碎；其次，只做到了transaction层次的协议过程解析，缺少call、session、dialog等过程的解析，这也增加了使用的难度；再次，缺少线程并发处理的机制，使得它的处理能力有限。



eXosip是Osip2的一个扩展协议集，它部分封装了Osip2协议栈，使得它更容易被使用。eXosip增加了call、dialog、registration、subscription等过程的解析，使得实用性更强。但是eXosip局限于UA的实现，使得它用于registrar、sip server等应用时极其不容易。另外，它并没有增加线程并发处理的机制。而且只实现了音频支持，缺少对视频和其它数据格式的支持。



综合来说，Osip2加上eXosip协议栈仍然是个实现Sip协议不错的选择。当然需要根据不同的需求来增加更多的内容。







二、Osip2协议栈的组成



Osip2协议栈大致可以分为三部分：sip协议的语法分析、sip协议的过程分析和协议栈框架。







1、Sip协议的语法分析：



主要是osipparser2部分，目前支持RFC3261和RFC3265定义的sip协议消息，包括INVITE、ACK、OPTIONS、CANCEL、BYE、SUBSCRIBE、NOTIFY、MESSAGE、REFER和INFO。不支持RFC3262定义的PRACK。



遵循RFC3264关于SDP的offer/answer模式。带有SDP的语法分析。



支持MD5加解密算法。支持Authorization、www_authenticate和proxy_authenticate。







2、Sip协议的过程分析：



主要是osip2部分，基于RFC3261、RFC3264和RFC3265的sip协议描述过程，围绕transaction这一层来实现sip的解析。



Transaction是指一个发送方和接收方的交互过程，由请求和应答组成。请求分为Invite类型和Non-Invite类型。应答分为响应型的应答和确认型的应答。响应型的应答是指这个应答仅代表对方收到请求。请求经过处理后都必须返回确认型的应答。响应型的应答有1xx，确认型的应答包括2xx、3xx、4xx、5xx和6xx。一个transaction由一个请求和一个或多个响应型应答、一个确认型应答组成。



Transaction根据请求的不同和发送/接收的不同可以分为四类：ict、nict、ist和nist。



Ict是指Invite client transaction，就是会话邀请的发起方。



Nict是指Non-Invite client transaction，是指非邀请会话的发起方。



Ist是指Invite server tranaction，是指会话邀请的接收方。



Nist是指Non-Invite server transaction，是指非邀请会话的接收方。



每种类型的transaction都有自己相应的状态机，Osip2协议栈根据状态机来处理所有的sip事件，所以这部分就是整个协议栈的核心。但是因为Osip2只做到transaction这一层，所以它可以忽略掉call、registration等应用的复杂性，显得相当简单，这就使得需要使用它的应用必须要自己处理应用的逻辑。必须注意的一点是，transaction的资源在Osip里是由协议栈负责释放的，但是在Osip2里改成由使用的应用负责释放。



下面简单的用时序图来描述四种transaction的状态机，只着重于描述状态间的转换，忽略了调用的处理函数，也简化了很多没有状态变换的事件。也就是说，每个状态下定义的事件并没有完整的表现在图中，不要以为这些事件没定义或在该状态下没有处理。



图中方框里的是状态名，箭头线上的是触发状态变换的事件名称。同一个状态下的事件并没有时序关系。



Ict的状态机如下：



（图略）





Nict的状态机如下：













（图略）



Ist的状态机如下：













（图略）



Nist的状态机如下：













（图略）





3、协议栈框架：





框架并不是指代码的某一部分，而是指它的构成形式。主要有三部分：底层套接字接收/发送，模块间通信管道，上层调用api接口。





Osip2并不实现底层套接字的接收/发送，由eXosip实现，现在只支持UDP的链路连接。





模块间的通信管道包括：transaction的消息管道、jevent的消息管道。Transaction的消息管道是驱动其状态机的部件，通过不断的接收来自底层套接字的远端信令，或者来自上层调用的指令，根据上述的状态机制来驱动这个transaction的运转。Jevent的消息管道是eXosip实现的，用于汇报底层事件，使得调用程序能处理感兴趣的事件。





上层调用的api接口大致有两类：sip协议的调用接口和sdp协议的调用接口。EXosip封装了大部分的sip协议调用接口，一般来说都不需要直接调用osip2的接口函数。接口函数很多，在这里就不详述了，函数定义请参照源代码部分的注释。









三、eXosip协议栈的分析





eXosip是Osip2协议栈的封装和调用。它实现了作为单个sip终端的大部分功能，如register、call、subscription等。





EXosip使用UDP socket套接字实现底层sip协议的接收/发送。并且封装了sip消息的解释器。





EXosip使用定时轮循的方式调用Osip2的transaction处理函数，这部分是协议栈运转的核心。透过添加/读取transaction消息管道的方式，驱动transaction的状态机，使得来自远端的sip信令能汇报给调用程序，来自调用程序的反馈能通过sip信令回传给远端。





EXosip增加了对各个类型transaction的超时处理，确保所有资源都能循环使用，不会被耗用殆尽。





EXosip使用jevent消息管道来向上通知调用程序底层发生的事件，调用程序只要读取该消息管道，就能获得感兴趣的事件，进行相关的处理。





EXosip里比较重要的应用有j_calls、j_subscribes、j_notifies、j_reg、j_pub、osip_negotiation和authinfos。J_calls对应呼叫链表，记录所有当前活动的呼叫。J_reg对应注册链表，记录所有当前活动的注册信息。Osip_negotiation记录本地的能力集，用于能力交换。Authinfos记录需要的认证信息。













四、总结





本文是从使用的角度去阐述分析Osip2和eXosip协议栈，并不涉及很多的细节，需要进一步了解协议栈的结构和实现，可参考《osip》一文。









