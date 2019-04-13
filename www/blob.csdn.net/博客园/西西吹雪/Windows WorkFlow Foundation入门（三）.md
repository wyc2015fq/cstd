
# Windows WorkFlow Foundation入门（三） - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[Windows WorkFlow Foundation入门（三）](https://www.cnblogs.com/watsonyin/archive/2007/01/21/626028.html)
WF支持的CodeDOM类型
WF System.CodeDOM。你可使用这些类型来创建你的表达式，其中一些用于条件(conditions)，一些用于规则(actions)，还有一些能用于两者。
注意：CodeDOM不支持一元操作符，也不支持不等操作符，如a!=b，你可以换成这样表达：(a==b)==false
下表列出了WF所支持的CodeDOM类型。
类
用于
|
CodeAssignStatement
Actions
|
CodeBinaryOperatorExpression
Conditions
|
CodeDirectionExpression
Conditions
|
CodeExpressionStatement
Actions
|
CodeFieldReferenceExpression
Conditions
|
CodeMethodInvokeExpression
Conditions
|
CodeMethodReferenceExpression
Conditions
|
CodePrimitiveExpression
Conditions
|
CodePropertyReferenceExpression
Conditions
|
CodeThisReferenceExpression
Conditions
|
CodeTypeReference
As part of expressions
|
CodeTypeReferenceExpression
Conditions
|

CodeBinaryOperatorType
Supported in
|
Add
Conditions, Actions
|
BitwiseAnd
Conditions, Actions
|
BitwiseOr
Conditions, Actions
|
BooleanAnd
Conditions, Actions
|
BooleanOr
Conditions, Actions
|
Divide
Conditions, Actions
|
GreaterThan
Conditions, Actions
|
GreaterThanOrEqual
Conditions, Actions
|
IdentityEquality
Conditions, Actions
|
IdentityInequality
Conditions, Actions
|
LessThan
Conditions, Actions
|
LessThanOrEqual
Conditions, Actions
|
Modulus
Conditions, Actions
|
Multiply
Conditions, Actions
|
Subtract
Conditions, Actions
|
ValueEquality
Conditions, Actions
|

在工作流中使用事务
WF提供的TransactionScopeActivity封装了.NET System.Transactions的事务功能，可以在操作发生错误时自动回滚，从而支持从事务失败中恢复。请参考MSDN中获取System.Transactions.Transaction和System.Transactions.TransactionScope类的更详细信息。

工作流中的批处理状态信息
工作流会周期性在各个检查点保存它有状态，如果发生错误，工作流引擎有必要检索保存过的信息来返回到一个稳定的状态。如果两个以上的组件正在通信，那么组件间的coordinate persistence有助于保证组件的一致性。
这一点在发送或接收消息时尤其有用。例如，一个工作流可能发送多条消息，每个发送的操作都会请求消息服务。消息服务必须要维护工作流的一致性和持续的状态，只有工作流状态被成功保存，消息才能被发送。这意味着在单个的事务中，工作流状态以及消息发送状态都会被保存起来，以保存所有组件间状态的一致性。
WF提供了System.Workflow.Runtime.IWorkBatch 和System.Workflow.Runtime.IpendingWork两个接口来解决这个问题。在对服务所有调用中，运行时引擎在自己的线程上下文中提供了System.Workflow.Runtime.IworkBatch，你可以添加一个挂起的工作项到批处理中，这样运行时引擎可以一次性将所有相关的工作项提交到事务处理。
在ExternalDataEventArgs的构造函数中，可以传递一个IpendingWork类型的参数。
当组件被请求时的执行顺序

1、在第一次请求前，工作流创建工作批处理对象。
2、工作流将工作批处理传对象递到到组件的请求方法中。
3、组件创建一个工作项，并将其添加到工作批处理对象中。
其它组件请求时，重复第二步和第三步。
在事务提交点的执行顺序
1、工作流创建一个事务
2、工作流遍历工作批处理对象中的工作项，并收集其中属于某一组件的工作项，维护其顺序，然后创建一个新工作批处理对象。工作流调用组件的Commit方法，并把事务对象和这个批处理对象传递给它。
3、组件把工作批处理对象中的工作添加到事务中。
对所有的组件重复第二步和第二步。

当组件的Commnit方法成功调有后，工作注提交相应的事务。
当事务成功提交后，工作流遍历工作批处理中工作项，如第二步一样。工作流调用每个组件的Complete方法，交把事务对象和批处理对象传递给它。

工作流发生错误时的执行顺序
1、工作流识别错误范围内的所有工作项并构造一个工作批处理对象，并将这些工作项添加这个工作批处理对象中。对于2、工作批处理中所有工作，工作流通过IPendingWork接口来调用其Complete方法，并传递完成状态为false
3、工作流取消工作批处理中的所有工作。
从错误中恢复后，运行时维护剩余的工作批处理项引用。




posted on2007-01-21 12:24[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=626028)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
