
# Windows WorkFlow Foundation入门（四）-工作流中的异常处理 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[Windows WorkFlow Foundation入门（四）-工作流中的异常处理](https://www.cnblogs.com/watsonyin/archive/2007/01/22/626676.html)
WF中的错误处理
WF中的错误是以异步的方式来处理的。在活动中显式或隐式的抛出异常后，工作流运行时引擎会将捕捉到的异常先保存到队列中，在之后的时间再进行处理。这与一般的异常处理方式不同在于，如果一个异常在try{}语句块中被抛出，在相应的catch{}语句块中无法截获该错误，同时该错误也不会立即抛出给用户。
异常的起因
以下几种情况可能会产生异常
Ø 原子事务超时
Ø 其它类型的事务失败
Ø 宿主程序通过ThrowActivity抛出异常
Ø 用户代码错误。当工作流调用外部的用户代码时，CLR类可能会抛出异常，如果这种异常在用户代码中没有被处理，它们最终会以工作流异常的的表式出现。
Ø 其它类型的系统异常，例如保存失败，.net或系统异常，或数据转换错误等
错误捕获
在错误处理中，如果抛出异常的活动不能处理它，异常会被转交到它的上一级活动处理，直到它被处理为止，否则的话，工作流实例就会被工作流运行时引擎终止。
错误处理是由FaultHandlerActivity活动来完成的。每个FaultHandlerActivity活动都与一个.net异常类型相关联，并且它包含一系列的活动，如果错误类型与某个FaultHandlerActivity关联的错误类型匹配的话，就会执行为个FaultHandlerActivity活动。一个FaultHandlersActivity活动包含0-n个FaultHandlersActivity子活动，FaultHandlersActivity可以是任何复合活动的子活动。
WF中的错误处理通常被视为一个反向的工作，因为它的目标是当异常发生时，撤销活动中不成功的部分的工作。FaultHandlerActivity的执行完成不能被认为是事务活动的成功完成。事务活动会先被设置成错误状态，然后是关闭状态。任何兄弟活动同样会被取消。同时，compensation也会成为不可用状态。
在工作流中使用Compensation(下面译为补偿)
WF中对于已完成事务的补偿模式，是对工作流中出现的异常进行处理的过程，并且会在逻辑上撤销已完成的事务。
WF补偿有以下两种形式：
1、当异常没有被处理的时候，系统隐式的补偿；
2、 使用Compensate活动进行显式的补偿。





posted on2007-01-22 11:48[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=626676)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
