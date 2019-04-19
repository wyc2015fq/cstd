# 15个你必须知道的Facebook开源项目 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月28日 09:02:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：659
Facebook为了争当开源第一大厂，已经把自家很多好东西都拿了出来贡献给开源世界，在开源之路上一骑绝尘，根本停不下来。
Facebook开源项目涉及的领域有移动工具多样化、大数据、客户端web库、后台运行时和基础设施，还有通过开放计算项目涉及到的服务器和存储硬件等等。Facebook的GitHub账户有274个代码库，收到了39000次fork、提交了79000个commit，拥有242000个关注者……
下面是Facebook最受欢迎的15个开源项目。
## **移动开发**
**Buck**
[Buck](https://github.com/facebook/buck)是一个高性能的安卓编译系统。此系统鼓励用户创建由代码和资源组成的可复用的小模块。因为安卓应用主要是用Java写的，Buck也是一个Java编译系统。
**Rebound**
[Rebound](http://github.com/facebook/rebound)是一个模仿弹簧动力学的Java库。回弹弹簧模型可以用来创建动画，通过将真实物理世界引入到你的应用可使动画更自然。回弹使用相同的弹簧常数，就像Origami使得折纸交互模型很容易被转换到安卓应用中。
**Origami**
[Origami](http://facebook.github.io/origami)是用来设计现代用户界面的工具。可以迅速的创建原型，运行在你的iPhone或iPad上并进行迭代，将可用的代码片段输出给工程师。
**Stetho**
[Stetho](http://facebook.github.io/stetho/)是一个全新的安卓平台调试工具。Stetho提供C/S协议，使强大的Chrome开发者工具能在应用程序中使用该协议。你的应用程序整合之后，只需访问chrome://inspect ，点击“检查”即可开始。
**Infer**
[Infer](https://github.com/facebook/infer)是一个静态分析工具，用来检测安卓和苹果系统应用发布前的缺陷。如果你给Infer一些Objective-C，Java或C代码，它会生成一个潜在的缺陷列表。Infer工具也有助于防止系统崩溃和性能下降。Infer的目标是空指针异常、资源漏洞、内存溢出之类的致命缺陷。
# **Web开发**
**React Js**
[React Js](https://facebook.github.io/react/)是个用于构建用户界面的JavaScript库，高效且灵活。很多人把React当作MVC中的V来用，因为React不依赖你技术栈里的其它技术，因此很容易把它用在一些已有项目的小特性上。
**HHVM**
[HHVM](https://github.com/facebook/hhvm)是个开源虚拟机，设计目的是用来执行Hack和PHP写的程序。HHVM用即时编译方式实现卓越的性能并保持了PHP的开发灵活性。和Zend PHP5.2相比，HHVM为Facebook实现了超过5倍的产能提升。HipHop通常作为一个独立的服务器运行，同时替代Apache和modphp，它也可以在命令行运行单独的脚本。
**Flux**
[Flux](https://github.com/facebook/flux)是Facebook用户创建客户端web应用的应用架构。利用一个单向的数据流，Flux补充了React的组合视图组件。它更像是一种模式，而非正式框架，不用写太多新代码就能直接使用Flux。
![](http://upload-images.jianshu.io/upload_images/1714245-363eb5c8989c6146?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**Flow**
[Flow](https://github.com/facebook/flow)给JavaScript添加了静态类型，提高开发者的效率和代码质量。Flow的目标是减少程序员花在查找JavaScript错误上的精力。Flow很大程度上依赖类型推断来查找类型错误，即使代码中并未标注——它像流经程序一样精确的跟踪变量的类型。
**fb-flo**
[fb-flo](https://github.com/facebook/fb-flo)是Chrome的延伸，可以不重新加载而修改运行的应用。它能轻易和你的系统、开发环境实现整合，可以和你喜欢的编辑器一起使用。
**Jest**
[Jest](https://github.com/facebook/jest)是一款JavaScript的单元测试框架。它建立在Jasmine测试框架之上，使用我们熟悉的expect（期望）和toBe（实际值）。它自动模拟require（）返回的CommonJS模块，使得大部分现有代码可测试。
**Nuclide**
[Nuclide](https://github.com/facebook/nuclide)是一套Atom包，为许多编程语言和技术提供编辑功能。设计目的是为了在整个公司为工程师提供一套标准的开发者经验——无论他们从事纯iOS应用，React和React Native代码，或者在Hack运行我们的HHVM网络服务。
## **后端开发**
**Presto**
[Presto](https://github.com/facebook/presto)是开源的分布式SQL查询引擎，适用于运行交互式解析查询，数据量支持从GB到PB。Facebook用Presto进行交互式查询，用于多个内部数据存储，包括300PB的数据仓库。每天有1000多名脸谱网员工用Presto执行超过30000次查询，扫描超过1PB数据量。
**Osquery**
[Osquery](https://github.com/facebook/osquery)提供一个SQL接口，用来尝试新的查询和监控操作系统。它拥有一套完整的SQL语言和许多有用的内置表，Osquery对于执行事件响应、诊断系统操作问题、排除性能故障是非常出色的工具。它也允许开发人员和系统管理员部署安全工具。
**RocksDB**
[RocksDB](https://github.com/facebook/rocksdb)基于LevelDB，可运行在多CPU内核的服务器上，高效使用快速存储，支持IO绑定，内存和一次写负载，并且非常灵活。
