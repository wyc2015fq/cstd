# Android核心分析（19）----电话系统之GSMCallTacker - xqhrs232的专栏 - CSDN博客
2011年03月31日 10:09:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：722标签：[电话																[android																[table																[null																[2010																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=电话&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/maxleng/archive/2010/05/14/5593780.aspx](http://blog.csdn.net/maxleng/archive/2010/05/14/5593780.aspx)
Android电话系统之GSMCallTracker
通话连接管理
GSMCallTracker在本质上是一个Handler。
GSMCallTracker是Android的通话管理层。GSMCallTracker建立了ConnectionList来管理现行的通话连接，并向上层提供电话调用接口。
在GSMCallTracker中维护着通话列表：connections。顺序记录了正连接上的通话，这些通话包括：ACTIVE，DIALING，ALERTING，HOLDING，INCOMING，WAITING等状态的连接。GSMCallTracker将这些连接分为了三类别进行管理：
RingingCall: INCOMING ,WAITING
ForegourndCall: ACTIVE, DIALING ,ALERTING
BackgroundCall: HOLDING
上层函数通过getRingCall()，getForegrouandCall()等来获得电话系统中特定通话连接。
为了管理电话状态，GSMCallTracker在构造时就将自己登记到了电话状态变化通知表中。RIL-Java一收到电话状态变化的通知，就会使用EVENT_CALL_STATE_CHANGE通知到GSMCallTacker
    在一般的实现中，我们的通话Call Table是通过AT+CLCC查询到的，CPI可以通知到电话的改变，但是CPI在各个Modem的实现中差别比较大，所以参考设计都没有用到CPI这样的电话连接改变通知，而是使用最为传统的CLCC查询CALL TABLE。在GSMTracker中使用connections来管理Android电话系统中的通话连接。每次电话状态发生变化是GSMTracker就会使用CLCC查询来更新connections内容，如果内容有发生变化，则向上层发起电话状态改变的通知。
1 RIL-JAVA中发起电话连接列表操作
在RIL-JAVA中涉及到CurrentCallList查询的有以下几个操作：
（1）hangup
（2）dial
（3）acceptCall
（4）rejectCall
在GSMcallTracker在发起这些调用的时候都有一个共同的ResultMessage构造函数：obtainCompleteMessage()。obtainCompleteMessage()实际上是调用：
obtainCompleteMessage(EVENT_OPERATION_COMPLETE)
这就意味着在这些电话操作后，GSMCallTracker会收到EVENT_OPERATION_COMPLETE消息，于是我们将目光转移到handleMessage()@GSMCallTracker的EVENT_OPERATION_COMPLETE事件处理：[operationComplete@GSMCallTracker](mailto:operationComplete@GSMCallTracker)。
operationComplete（）操作会使用cm.getCurrentCalls(lastRelevantPoll)调用，向RILD发起RIL_REQUEST_GET_CURRENT_CALLS调用，这个最终就是向Modem发起AT+CLCC，获取到真正的电话列表。
2 在RILD中，引起getCurrentCalls调用
（1）在RILD中，收到URC消息：
+CRING
RING
NO CARRIER
+CCWA
将会使用RIL_onUnsolicitedResponse( RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED)，主动向ril-java上报RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED消息。
（2） 在处理requestCurrentCalls时，使用CLCC查询通话连接（CALL TABLE）后，如何发现有call Table不为空则开启一个定时器，主动上报RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED消息，直到没有电话连接为止。
在RIL-Java层收到RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED这个URC，并利用mCallStateRegistrants.notifyRegistrants(new AsyncResult(null, null, null))来通知电话状态的变化，此时GSMTracker会接收到EVENT_CALL_STATE_CHANGE消息，并使用
                pollCallsWhenSafe()->  cm.getCurrentCalls(lastRelevantPoll);
来发起查询，并更新JAVA层的电话列表。
3 handlePollCalls电话列表刷新
      首先我们来看看是什么引起了handlePollCalls的调用。
     上面的1,2分析了，Android电话系统中所有引起电话连接列表更新的条件及其处理。他们共同的调用了cm.getCurrentCalls(lastRelevantPoll) 来完成电话列表的获取。
lastRelevantPoll = obtainMessage(EVENT_POLL_CALLS_RESULT)
我们这里就从可以看到获取到的电话列表Result使用handlePollCalls进行了处理。Result实际上是一个DriverCall列表，handlePollCalls的工作就是将当前电话列表与RIL-Java的电话列表对比，使用DriverCall列表更新CallTracker的电话列表connections,并向上传递电话状态改变的通知。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/maxleng/archive/2010/05/14/5593780.aspx](http://blog.csdn.net/maxleng/archive/2010/05/14/5593780.aspx)
