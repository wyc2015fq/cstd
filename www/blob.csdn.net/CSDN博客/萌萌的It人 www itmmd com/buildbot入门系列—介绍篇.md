
# buildbot入门系列—介绍篇 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年08月07日 18:20:00[Jlins](https://me.csdn.net/dyllove98)阅读数：6739


## 一、介绍
1. buildbot是一个开源的基于python的持续集成系统，它能够以下三种方式触发相应的自动构建和测试运行，从而迅速的发现问题所在，同时指出造成这个错误的开发人员，当然我们还可以通过页面直观的了解到当前所有和master绑定的任务以及各种测试状态。
1)**监控代码管理库的变化**从而触发构建测试任务
2) 通过配置从而**定时**触发构建测试任务
3) 通过配置从而允许**强制**触发构建测试任务
2. 因为它有很多比较好的特点：
1)**跨平台**：可以运行在各种平台上，实现不同平台上的测试
2)**可以处理各种语言编写的程序**,例如C，Java，Python
3)**环境要求低并且配置简单**：仅仅需要Python，和网络库Twisted
4)**结果的交付方式多**，例如Email，webpage，IRC或者其他协议工具
5)**通过子类继承并重写父类从而灵活的配置**
6)**很好的实现了分布式部署和集成工作**
所以目前有很多大公司都在使用这个系统，比如
1) chrome :[
https://chromium-build.appspot.com/p/chromium/console](https://chromium-build.appspot.com/p/chromium/console)
2) webkit  :[http://build.webkit.org/waterfall](http://build.webkit.org/waterfall)
## 二、系统基本原理
### 1.系统整体架构
buildbot主要由一个buildbot-master和一个或者多个buildbot-slave两部分通过网络拓扑结构中的星型结构连接而成，如图：
注意：由于历史原因，很多情况下，我们都直接称buildmaster为buildbot，而buildslave称呼不变。
下面我们通过上面这幅图来详细了解下各部分的作用吧。
1)**Repository ：**代码管理库，用于团队开发的代码管理和版本控制，目前流行的有svn，cvs，git……
2)**Buildmaster：**主要负责分派并且告诉slave什么时候进行测试，怎样进行测试，进行什么样的测试，可以说是一个决策中心，而这个决策中心的核心在于master.cfg这个配置文件，它其实是一个用python语法来写的配置文件（配置文件后期会进行讲解）。
3)**Buildslave :**负责根据buildmaster下发的Command命令执行测试，同时将执行状态和结果返回给buildmaster
4)**Notifiers ：**当BuildMaster接收到BuildSlave的执行结果后触发Notifiers，根据配置的方式将结果交付。
### 2.BuildSlave、BuildMaster、Repository间的通讯
1) BuildSlave通常运行在一台或者多台不同的独立的机器中，这些机器可以有不一样的系统，具体根据你的兴趣和需求而定。然后这些BuildSlave机器都是通过TCP连接到BuildMaster机器的公共端口进行通讯的。当然这些机器和buildmaster或者Repository之间不一定必须直接相连，中间可以有一些简单的防火墙机器，只要不妨碍builslave和buildmaster、Repository之间的通讯即可。
2) 他们三者间的TCP连接，都是由BuildSlave主动发起的
3) 所有的命令都只能由BuildMaster下发到BuildSlave，而且不管是BuildMaster下发的命令，还是BuildSlave上传的结果都是通过TCP连接进行传输的
4) BuildMaster并不提供源代码，因此BuildSlave需要源代码的时候必须能够连接到Repository 进行相应操作。

### 2.BuildMaster的架构
Buildmaster由以下四大部分组成，如图，假设我们的Repository是VC Repository:
下面我们来简单介绍下上面的每个部分的用途，这些在master.cfg中都能够进行配置的：
1)**ChangeSources：**Scheduler的触发源，每当VC Repository发生了变动，就会创建一个Change对象提交给各个Scheduler
2)**Schedulers :**决定什么时候进行构建和测试任务，该Scheduler将会收集ChangeSources提交过来的Changes到BuildRequests中，当BuilderSlave可用时，就将这些changes排队成Queue交付给Builders
3)**Builder ：**决定如何执行构建和测试任务,每一个Build只运行在一部机器中
4)**SlaveBuilder :**多个SlaveBuilder组成一个BuildSlave。
5)**Status plugins：**用于交付构建结果的插件
从图中我们还可以看出(注意BuildSlave和SlaveBuild是不一样的哦)，
1) 每一个Builder是由一些列与他的构建任务相关的BuilderSlave组成的。这些BuilderSlave的地位是平等的，这么做的原因只是为了负载均衡
2) Builder将会在与它相关联的BuilderSlave中创建一到多个SlaveBilder，这些SlaveBuilder是独立运行在不一样的文件夹中。
3) 多个Builder可以共享一个BuildSlave。
### 3.Buildbot的控制流程
从**2.BuildMaster的架构**章节中的那幅图中，我们还可以看出BuildBot的整个控制流程。
1) 有这么一个开发人员，提交了一些改动了的代码到Repository 中，然后builbot中的相关脚本监控到了就通过Email或者网络连接将这些改变信息发送给了buildmaster，信息中包括了谁导致了这些变动，哪些文件被修改了，包含这些改变的版本以及一些其他的注释。
2) buildmaster分配这些改变到不同的Scheduler，一些相应有配置的改变将会触发tree-stable-timer开始计算，同时该改变被添加到一个新的Build中，当tree-stable-timer到了有效期，该Build将会被添加到一个Builder中并且运行在一个BuildSlave中，当然这个Builder中的所有Build构建运行测试任务用的都是同一版本的源代码。
3) 这些Build又是由一系列的步骤Step构成的。每个Step都会包含一条甚至多条将会在BuildSlave机器中运行的命令。一般情况下，第一个Step都是从Repository中导出源代码。接下来的一般都是进行编译或者单元测试的。在执行过程中，所有的Setp都会将它们自身执行命令的结果以及状态返回给buildmaster。
4) 当这些Build运行的时候，它们会将相应的如"Build Started", "Step Started", "Build Finished"这些命令打印到所有的状态终端中。比如我们常见的waterfall。
5) 当一个 MailNotifier 状态终端被激活的时候，一个build完成之后都会发送一封email通知相应的开发人员。当然我们也可以配置当且仅当运行失败的时候才通知相应的开发人员。
### 4.状态交付架构
BuildMaster维持着一个核心的Status对象，这个核心的Status对象连接着很多Status plugin，通过这个核心对象，我们可以获取到每一层的所有构建状态对象，如图：

因此，每一个Status plugin都有一个引用指向图中最顶层的那个核心Status对象，从而可以获取到每一个Builder、build，Step甚至日志文件的信息。除此之外，当有新的Build创建时，Status还可以通过核心Status对象去监听新的Build。



