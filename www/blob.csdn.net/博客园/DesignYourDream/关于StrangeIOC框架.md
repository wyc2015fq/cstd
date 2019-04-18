# 关于StrangeIOC框架 - DesignYourDream - 博客园
# [关于StrangeIOC框架](https://www.cnblogs.com/designyourdream/p/4477101.html)
在Unity上进行开发，请先看对其开发模式应用的讨论：
http://www.reddit.com/r/Unity3D/comments/1nb06h/unity_design_patterns_and_concepts_looking_for/
第一个问题，整个程序的入口在哪里。原来框架里的入口是个Scriptobject，用来启动所有的管理器。在uFrame的架构里，设计一个GameManager继承自Monobehaviour，然后利用DontdestroyOnLoad方法在不同的场景之间进行切换。
看看网上的各种讨论，感觉uFrame不如SOC靠谱，后者用的人多，讨论也多，抱着这颗大树吧。
strangeioc提出了一个mvcs的框架，下面这个算是对框架各个部分一个比较清楚的说明
- **Model** – “I hold ALL of the data, ha ha ha.”
- **View** – “Hey, look at this” (graphics, gui, sound) and “Oh, you clicked the mouse” (input), “Either way I’ll tell the controller about it”.
- **Controller – **“Hey Model (or Service), someone clicked a button so do X or Y”, or “Service, you loaded? Ok, I’ll tell the model”, or “Thanks Model for the updated data, I’ll tell the view to show that”.
- **Services** – “Welcome everyone, I’m going to just load stuff from offline files and online servers and tell the controller”.
描述一个最基本的业务流程：
1.玩家点击了一下UI
2.mediator得到通知（回调），因为它绑定了UI事件
3.mediator触发了一次command执行指令，因为mediator和command进行了绑定
4.command执行自己的execute方法
5.请求Service从一个文件中读取数据
6.Service读取数据完成后，触发事件通知Command执行execute函数。
7.command会把加载后的数据送给Model
8.Model检测到自己更新数据后，通知mediator说有新数据来了
9.Mediator就会把数据送给UI，显示到界面上。
原则：
View应该只负责显示和输入，当输入事件发生时，view应当通知mediator，绝不能让View直接通知给Model和Service，View只会分发事件，例如某个按钮按下了，至于对这个按钮的响应，应该交给Mediator来做，例如view告知说帮助按钮按下了，mediator知道后，调用具体的command再去执行具体的任务。Model和Service是被Command来使用的，而不会去监听view的事件。
StrangeIOC的例子
程序的入口是一个ContextView脚本，此脚本是个mb必须要挂在gameobject上，这应该也是命名带上view的意思。这个ContextView里一般就是创建Context。
Context的功能把各个部分组合到了一起 。
**分析一个StrangeRocks的例子**
入口。此例子实现了类似飞行射击类的游戏。分成了三个部分，入口，游戏模块，UI，对应设计了三个场景，分别是main,game,ui，存放在Art的Scene目录下。每个场景里都有一个节点，挂的是重写的contextview脚本，分别是MainBootstrap,GameBootstrap,UIBootstrap.继承关系如图：
![](https://images0.cnblogs.com/blog2015/698097/201505/141123342678892.png)
 这其中每一个contextview脚本里都创建了一个context对象，分别是MainContext,GameContext,UIContext。这几个类的集成关系如图：
![](https://images0.cnblogs.com/blog2015/698097/201505/141128088454373.png)
其中，crosstext可作为全局的定义，其他context可以访问到。CrossContext里将会给ICrossContextInjectionBinder类型的变量 injectionBinder赋值。MVCSContext具备了搭建一个IOC应用的所有元素，包括（ an Injector）注入器，（a message bus）消息总线，（Command
patterns）命令模式，（model and service support）模型支持，（View mediation）视图中转器等。
三个继承的context ，都会重写mapBindings函数。
1.MainContext里，会绑定全局的事件，绑定数据模型gamemodel为单件。
2.GameContext里，会绑定键盘输入为单件，绑定精灵管理器，绑定技能管理器，定义了游戏过程中用到的各种池对象，包括特效，武器，精灵等。
随后commandBinder，这里用的是SignalCommandBinder，来源于SignalContext的addCoreComponents方法里，把injectionBinder的ICommandBinder绑定为了SignalCommandBinder，这种binder就会把各种事件和对应的command执行器进行绑定。注意，这个command bind将会自动创建注入映射。最后是mediationBinder，用的是MediationBinder， 这个具体对象的绑定来源于MVCSContext的addCoreComponents方法，mediator的绑定将view和mediator结合到了一起。这就是说，context知道所有的细节，mediator不知道的，view不知道的，它都知道。
如何实现bind map to注入，有如下函数段：
![](https://images0.cnblogs.com/blog2015/698097/201505/141510395793717.png)
其中command的注入过程是动态的，不是在bind的时候就绑定的，这样也好理解，因为有可能我这个事件永远不会发生，那就不需要绑定了：
![](https://images0.cnblogs.com/blog2015/698097/201505/141541045176287.png)
 尝试实现精灵：
可以参考实现精灵的数据对象，例如ISpriteModel包含一个精灵需要的各种数据，包括血量，等级各种东西。然后是精灵的显示对象，ISpriteView，这个view的作用是管理显示相关的逻辑，即所有看得见的东西，例如移动，播放动画，换装，武器挂载，特效挂载，所有跟monobehaviour相关的，而Mecanim相关的事件必然是和gameobject相关的，所以必然是View相关的，gameobject上接收的所有事件都由View来接受，显然这和他相关，是显示。而比如当到达某个关键帧，需要触发特效，让view通知mediator再通知到controller，执行一次command。controller是个中转环节，可以知道model,view,service等各种东西。这样一来有可能就会设计的很复杂。command会有很多个，因为每个行为都是一个command，而不是一个controller。
strangeioc的Attribute竟然没有给自己加命名空间，导致InjectAttribute.cs和nodecanvas里的Attribute命名冲突，nodecanvas是有自己的命名空间，strangeioc竟然没有，略坑。
** 这里做个总结：**
1.StrangeIOC的成熟度还不够，用来做大型游戏项目显然有些力不从心。至少目前没有3A级的大作出来。
2.概念很多，在团队中推广难度很大，理解起来麻烦，各种事件调用机制 ，调试难度也很大。
3.MVC思想是务必会使用的，在各个模块的设计过程中，都要有这个思想，数据，显示，控制逻辑分开。但至于说是不是真要分的那么清楚，倒不用锱铢必究。
4.关于绑定框架。目前看在运行时，也许有些模块可以做些小应用，但整个application范围的应用需要慎重。
5.做这个框架的两名工程师，可能没做过大型的网络游戏项目，这个框架套在网游上会很别扭，至少现在看来是的。
6.这个框架其实可以持续跟进，也许有一天，牛逼人就搞出来个牛逼的大作来。

