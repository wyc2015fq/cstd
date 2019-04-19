# WINCE压力测试 - xqhrs232的专栏 - CSDN博客
2014年04月11日 22:47:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：673
原文地址 ::[http://blog.csdn.net/ok138ok/article/details/4355071](http://blog.csdn.net/ok138ok/article/details/4355071)
**WINCE压力测试**
--作者：赖玉平(Auly)aulyp@163.com
开发一个新产品，有一个必要的阶段是压力测试，看看自已开发的东西是否稳定，所谓的压力测试，就是长时间高强度的操作机器，测试机器的稳定性与可靠性。看是否有内存泄露和崩溃等潜在BUG，
微软提供了一个很好的压力测试工具，该工具作为CETK的一个补充，
**（1）运行压力测试工具**
第一步：用CETK与目标设备建立连接，如以参照我写的《使用CETK测试驱动程序》里面有详细的连接方法
第二步：CETK与目标设备连接成功之后，鼠标右击目标设备名，如“WindowsCE (ARMV4I)”
选择Tools –> Windows Embedded CE Stress
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/001633833829002500000.jpg)
图1
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/002633833829003281250.jpg)
图2 压力测试软件界面
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/003633833829004218750.jpg)
图3 运行测试时，WINCE设备上显示的界面
**（2）建立自已的测试模块，**
要为自已的平台打造有针对性的测试，可以建立自已的测试模块，方法如下
㈠在PB环境下（CE6.0在VS2005环境下），“Solution Explorer”窗口，右键点击工程名，
**点Properties**进入图5， 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/004633833829004531250.jpg)
图4
㈡ 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/005633833829005156250.jpg)
图5
在下拉列表中选Environment，然后点new,建新的Variable,确定后回到VS2005界面，
㈢ Rebuild 工程
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/006633833829005781250.jpg)
图6
㈣ 当Build完成，右击工程名，选**Open Release Directory in Build Window**此时会打开一个CMD窗口，
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/007633833829007656250.jpg)
图7 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ok138ok/EntryImages/20090716/008.jpg)
 图8
㈤编译
输入
**cd %_WINCEROOT%/private/test/stress/stress/samples/simpledll**
进入simpledll目录，输入
**build –c**
如果编译成功，就可以用作为测试模组了。
