# 浅谈新一代Android操作系统Android L - SHINING的博客 - CSDN博客
2014年06月26日 16:52:37[snsn1984](https://me.csdn.net/snsn1984)阅读数：3072
![](https://img-blog.csdn.net/20140626164740203)
今天早一些的时候，Google发布了新一代Android操作系统Android L。让我非常意外的是命名规则的变化，之前外界纷纷猜测会叫Android5.0，同时会给一个甜品的名字命名。因为Android操作系统发布了这么多的正式版本，一直是用甜品来命名的，现在直接用了一个L，和以往风格大大不同，让人不太理解。不过好在L非常好记，也非常容易上口。
看了一下官方宣称的Android L的新特性，主要有如下几点：
A New UI Design
Create a consistent experience across mobile and the web with**material design**, the new Google-wide standard.
采用了一个新的UI设计方案，让用户可以在手机和网页之间获得一致性的体验，至于这里说的material design不熟悉，毕竟一直也没搞过UI。既然介绍是新的Google-wide标准，估计在相当一段时间内，会在UI设计方面一直沿用这个标准。
从目前放出来的UI图片来看，是比之前要更漂亮一些。
A New Runtime
Test your apps and get them ready for**ART**(**A**ndroid**R**un**t**ime), the default runtime in the next release.
采用了一个新的运行时ART。其实不能说是新的，在Android4.4中就已经正式出现了ART，只不过不是默认选项，默认选项依然是dalvik，想来那个时候是打算过度一下的。在Android L中，ART将作为默认的运行时登场。可以大大的加快APK的运行速度，缺点是占用的空间会更大。因为将之前dalvik的即时编译改成了ART的提前编译。关于这个部分，在之前我的一篇博文：
[Android中编译工具链的改动----LLVM份量的增加](http://blog.csdn.net/snsn1984/article/details/20547601)
中有详细的介绍。Google这个举动也是为了彻底解决Android运行速度的问题。毕竟dalvik已经太老了，用的太久了，无法承担现在的需求。[](http://blog.csdn.net/snsn1984/article/details/20547601)
Enhanced Notifications
Get control over where notifications appear, how they look, and how they sync to non-handheld devices.
加强了消息提示功能。这里主要是调整了一下消息提示的显示功能，视图效果，和如何同非手持设备通信的问题。这个功能解决很多让大家头疼的问题，比如游戏中有来电消息等，新有的消息通知，不会干扰现有的操作。
Increased Efficiency
**Project Volta**is our effort to make the platform energy efficient and to give you more control over resource usage.
![](https://img-blog.csdn.net/20140626164244750)
主要是Volta，一个类似电源管理的工程。它可以开启省电模式，降低APP的耗电量，同时还有一个详细的历史用电分析展示。其实，省电模式这个在很多厂商自己搞的Android系统之中已经有了，只是Android官方版本中一直没有。
![](https://img-blog.csdn.net/20140626164320406)
主要介绍的新增功能有这么几个，还有一些其他的，比如可以设置在一定的场合不用密码就可以打开手机，方便在家里或者某种信赖的环境下面使用等。相信等真正的版本放出来之后，会发现更多的功能。
注：图片来自网络，英文内容来自Android网站
