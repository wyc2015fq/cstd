
# Windows WorkFlow Foundation入门（二） - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[Windows WorkFlow Foundation入门（二）](https://www.cnblogs.com/watsonyin/archive/2007/01/19/624531.html)
二、使用WF开发介绍
WF允许我们用.net framework来开发自己的工作流程序，它并不是一个可执行程序，但它使得我们能开发自己的工作流应用程序。
传统的通用编程语言致力于开发短生命期的程序，它们缺乏对程序运行的持续性和稳固性的内在的支持。WF是一个框架，不是一门语言。它对工作流编程有着深入的理解，从而能很好的处理诸于persistence and compensation这样的工作。
WF是灵活的，可扩展的。你可以直接通过代码或markup（？）来编写工作流，可使用两者组合。你可以通过定义可重用的活动来，来实现自定义的工作流模式。
WF支持模型驱动的工作流开发，它支持自然的可视化设计，同时隐藏了一些系统级的概念，如事务、状态管理以及并发控制。
开发工作流程序有两件基本的事情，一是定义工作流和它的活动，再就是在程序使用这些工作流。（一）工作流程模式
WF支持多种工作流程式，如顺序工作流，以及数据驱动。顺序模式是一直向前执行，它在重复和、可预见的操作（这些操作通常相同）中非常有用。状态机模式由一系列事件驱动的状态组成，数据驱动模式依赖于数据来决定某个活动是否执行。
顺序工作流
顺序模式是一直向前执行，它在重复和、可预见的操作（这些操作通常相同）中非常有用，例如，一系列的活动以规定的、通常相同的顺序来运行。参见下图。
![](https://images.cnblogs.com/cnblogs_com/watsonyin/wf002.JPG)
顺序工作流以按顺序执行的方式来执行活动，直到最后一个活动执行完成。顺序工作流不一定是完全确定的，即使在非正常操作中。例如，你可以使用一个“监听”活动或一个“并发”活动，事件的具体顺序在不同的情况下可以不同。
状态机工作流
在状态机模型中，工作流就是一个状态机，它由一系列的状态组成。其中一个状态指示了开始状态，每个状态都可以接收一系列的事件。基于一件，工作流从一个状态迁移到另一个状态。状态机工作流可以有一个终结状态，当迁移到终结状态时，工作流就完成了。
下面这幅图是一个状态机工作流的例子。
![](https://images.cnblogs.com/cnblogs_com/watsonyin/wf003.JPG)
下面这张表列出了在WF框架的活动集合中与状态机相关的活动
活动说明|
EventDrivenActivity
用于依赖于外部事件才能开始执行的状态。EventDrivenActivity必须有一个Activity实现在了IEventActivity 接口，作为其第一个子活动。|
SetStateActivity用于迁移到一个新状态。|
StateActivity
代表状态机中的一个状态，可以包含附加的“状态”活动|
StateInitializationActivity
当进一个状态时执行，可以包含其它活动。|
StateFinalizationActivity当离开一个StateActivity时会执行它所包含的活动。|
使用活动来控制流程
WF框架包含不同功能的活动，我们可以这些活动来控制工作流的逻辑流程。
下面这张表列出了在WF框架的活动集合中用来控制工作流逻辑流程的活动。
活动说明|
ListenActivity可以根据一些事件或超时操作来产生分支。|
IfElseActivity在每个分支上测试同一个条件，并且执行第一个返回True的分支上的活动。|
ParallelActivity可以使工作流同时执行两个以上互不相关的操作。|
SuspendActivity挂起一个操作，可以产生错误用来干预流程。|
TerminateActivity当错误发生时，可以用来立即终止工作流中的一个操作。|
WhileActivity循环执行一个操作，执行指定的条件得到满足。|
ConditionedActivityGroup也被称为CAG。当设定到CAG的条件满足时，会执行CAG包含的子活动。子活的执行也必须满足该子活动设定的条件。|
EventDrivenActivity它是一个包含另一个活动的活动，在指定的事件发生时执行。|

在工作流中使用条件
我们可以通过条件来控制工作流的状态。运行时引擎会自动判别一个条件，并根据判别的结果来执行某一些操作。
有两种方式来表达工作流中的条件：
一是代码方式，编写一个Handler返回Bool值；（注：以下译作代码条件）
二是通过工作流定义中的规则(rule)。（注：以下译作规则条件）
我们可以在运行的时候动态的更新规则条件来改变工作流的行为。
我们还可以使用PolicyActivity，通过规则来完成有条件的行为。
条件活动
WF提供了多个使用条件的活动：IfElseActivity分支：基于对条件结果的判别。WhileActivity：只要条件保持为True，就会持续的执行它包含的所有活动。条件的值会在每次循环执行时重新进行判别。ConditionedActivityGroup：持续的执行它包含的所有活动直到其条件为真，它包含的每个活动都有一个When条件，只有当When条件为真时，活动才会执行。
你可以在你创建的工作流中自定义条件。注意：规则条件可以引用public成员（即在你的工作流类中定义的变量或方法），而代码条件可以引用public或private成员。
使用规则条件
规则条件在条件表达式编辑器中定义，根据工作流状态和数据来确定条件的值。运行时引擎会判别这个条件，从而决定要不要让与之相联的活动执行或使之可用。
规则条件非常有用，因为你在运行的时候可以修改它们，这意味着你可以不用停止、修改或编译你的工作流就可以改变它的行为。
可以使用下面的要素来构建规则条件：
逻辑操作符，如AND,，OR，NOT算术操作符，如+，-，*，/工作流中的public成员，间接地使用其它程序集中的成员
对包含复杂类型的变量的嵌套调用，这种变量如：this.Address.State == "WA"
注意：if关键字会被自动添加到条件语句的前面，但它不会显示在这个语句中。
动态更新规则条件
你可以通过动态更新规则条件，在工作流运行的时候修改它的行为，而不需要重新编译。
条件会作为活动的一个属性出现，这些活动诸如：WhileActivity, IfElseActivity, ReplicatorActivity, 和ConditionedActivityGroup。宿主程序会显式的更新这些条件定义，更新后的条件会在下次条件被判别时使用到。
因为条件是可序列化的，如果工作注被暂停或终止时，更新后的条件也会被保存。重要：任何动态更新只能当前正在运行的实例有效，而不会影响到工新创建的作流实例。
使用代码条件
要以用C\#或VB.net来创建条件。代码条件简单看，是分隔代码的处理器，它为ConditionalEventArgs.Result设置一个布尔值。代码条件被编译后，成为程序集一的部份。运行时引擎执行这个方法，并返回一个一个布尔值作为代码条件的判别结果。
下面的例子是一个用于判别某个值是否大于10的一个代在条件：
privatevoidEvaluateCondition(objectsender, ConditionalEventArgs e)
{
e.Result=(this.CustomPropertyValue>10);
}






posted on2007-01-19 11:10[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=624531)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
