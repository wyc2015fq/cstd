
# CI框架浅析（一） - 小小情意 - 博客园






# [CI框架浅析（一）](https://www.cnblogs.com/xiaoxiaoqingyi/p/6654190.html)
CodeIgniter 是一个小巧但功能强大的 PHP 框架，作为一个简单而“优雅”的工具包，它可以为开发者们建立功能完善的 Web 应用程序。本人使用CI框架有一段时间了，现在决定把该框架源码剖析一遍，理解其构架的用意与精妙之处。分析完所有的源码后，我才来总结CI框架的优缺点，以及适用于哪些场景开发。
目前CI最新的版本是 3.1.4， 4.0版本也即将发布。我们先分析3.1.4版本，然后再看看4.0有哪些重大突破。
首先是查看根目录下index.php文件了，主要定义了几个常量：
1、ENVIRONMENT （开发环境常量），该常理三个值选择：
a、development（默认值）：开发模式，在该模式下，那肯定是应该尽量输出捕获的日志错误，让开发者尽快找出错误，定位代码行，解决问题。
b、production：生产模式，该模式下，不输出错误的信息给用户看，会跳转到一个友好的用户错误提示页面。
c、testting: 该值与 2 是一样的含义；
2、BASEPATH（基本路径）指向 项目的system目录，框架的核心库
3、APPPATH（应用路径）指向application目录，然而你也可以修改该目录名
最后一行 require_once BASEPATH.'core/CodeIgniter.php'; 引入框架的核心库。
CodeIgniter.php 主要做了以下操作：
1、加载框架中的常量，即 application/config/constants.php
2、加载全局方法 system/core/common.php
3、全局变量安全筛选处理
4、设置程序错误、异常与奔溃的异常处理方法
5、加载子类的前缀
6、判断是否使用自定义的自动加载
7、启动计时器，记录程序运行的时间
8、初始化钩子（Hook），且在各个环节预定义好钩子
9、加载配置文件 core/config，同时也加载用户配置的config
10、加载字符集相关的类库
11、加载兼容性特性
12、初始化系统核心库 core下的 Utf8、URI、Router、Output、
Security、Input、Lang类，同时引入Controller.php文件
最后解析相应的controller、方法与参数，执行相应的业务处理。
下面的篇幅，我们一 一的讲解每一步的具体功能，以及为何要这么做！
欢迎关注我，一起探讨框架的设计原理。





