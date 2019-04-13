
# Windows WorkFlow Foundation入门（一） - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[Windows WorkFlow Foundation入门（一）](https://www.cnblogs.com/watsonyin/archive/2007/01/19/624474.html)
这几天在学习Windows WorkFlow Foundation(以下简称WF)，但并没有很好的学习资料，所以干脆翻译了。本人英文不怎么样，译得不好，请见谅啊![](https://www.cnblogs.com/CuteSoft_Client/CuteEditor/images/emteeth.gif)
一、工作流概述
工作流是由活动单元组成的集合，活动是真实过程的的一个模型。工作流提供了一种描述一系列相互关联的工作之间有执行顺序，这种工作从头到尾贯穿了整个活动，这些活动可能是由人工或系统来执行。
每一个运行的工作流实例由工作流运行时引擎来创建和维护的。虽然对于每一个应用程序域只能有一个工作流运行时引擎，但工作流运行时引擎内可以行多个工作流实例并发工作。
一旦一个工作流模型被编译，它就可以在任何一Windows进程内工作，包括控制台程序，窗口程序，Windows服务程序，Asp.net网站及Web Service等。因为工作流驻留在进程中，所以它可以很容易与它的宿主进程进行通信。
下面这幅图表明了工作流、活动以及工作流运行时引擎都存在于一个宿主程序中。
![](https://images.cnblogs.com/cnblogs_com/watsonyin/wf001.JPG)
活动
如上所述，活动是工作流的基本单元，它们通过程序被加入到一个工作流中，其方式就好比将一个XML DOM子节点加入到根节点中。一旦工作流中的所有节点都运行完成，工作流实例就会结束。
WF由一系列标准活动类库组成，同时也提供了一个机制帮助开发人员开发自己的类库。这使得工作流之间的可扩展性和可重用性更加优异。
服务
当一个工作流运行的时候，工作流运行时引擎要使用到多个服务。这些服务组件是可插拔的，这使得应用程序可以在它们的运行环境中，提供具有唯一性的服务。Windows Workflow Foundation提供了这些运行时服务的默认实现，可以满许多种应用程序需求。例如，它提供的这些组件实现了工作流实例的调度模型，工作流及其宿主程序之间的通信，怎样监视和跟踪你的工作流等等。
开发人员同样可以从平台提供的服务基类继承，来自定义工作流，从而扩展WF平台。
有状态的、稳固的、长时间运行的应用程序
WF简化了编写有状态的、稳固的、长时间运行的程序的过程。工作流运行时引擎管理着工作注的执行，并允许工作流长期保持活动状态，哪怕是计算机重新启动。运行时服务通过完美的错误管理机制，来实现这一点。
WF包含一个标准的服务，它与Microsoft SQL Server 2005 Express, Microsoft SQL Server 2000 or later or Microsoft SQL Server 2000 Desktop Engine (MSDE) 很好的集成，从而能列容易和高效的保证工作流的稳固性。
关于Visual Studio 2005 Extensions
使用WF组件来开如工作流程序，不一定要用Visual Studio 2005，但是，Visual Studio 2005 Extensions for Windows Workflow Foundation包含了许多工作流和自定义活动的设计器，将会使得开发工作更加方便。






posted on2007-01-19 10:42[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=624474)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
