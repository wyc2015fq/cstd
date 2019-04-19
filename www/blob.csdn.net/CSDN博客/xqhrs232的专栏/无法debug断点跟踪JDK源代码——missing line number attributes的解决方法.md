# 无法debug断点跟踪JDK源代码——missing line number attributes的解决方法 - xqhrs232的专栏 - CSDN博客
2018年11月08日 11:55:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：59
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/wavky/p/3802537.html](https://www.cnblogs.com/wavky/p/3802537.html)
在项目工程->Properties->Java Build Path->Libraries中导入的JRE System Library库里，给jar包添加JDK源代码包后，能够直接打开JDK的各种类文件源代码，如Object、String、HashMap等
![](https://images0.cnblogs.com/i/539308/201406/221346510985785.jpg)
![](https://images0.cnblogs.com/i/539308/201406/221350033791629.jpg)
![](https://images0.cnblogs.com/i/539308/201406/221350219266828.jpg)
但是，在给这些源代码打断点开启debug跟踪的时候，却发现eclipse报了一个 Unable to install breakpoint in *** due to missing line number attributes.Modify compiler options to generate line number attributes. 的错误提示框
![](https://images0.cnblogs.com/i/539308/201406/221355101922728.jpg)
打开Window->Preferences与项目的Properties窗口，在Java Compiler页面中重复确认Add line number attributes to generated class files项均被正确勾选（默认已勾选）并Apply后，依然没有解决问题
![](https://images0.cnblogs.com/i/539308/201406/221400088013622.jpg)
这种情况下发生的问题应该就是，eclipse（或项目工程本身）引入的JRE System Library（JDK库）搞错了，多数情况下是引入了JDK附送的JRE包，如下图这样
![](https://images0.cnblogs.com/i/539308/201406/221405592547954.jpg)
正确的情况应该是引入JDK本身而不是JRE，点击Edit，更改Location指向正确的JDK目录，点击Restore Default更新左侧所有jar包的引用目录（指向jdk文件夹下的jre目录），确认必需的jar包已配置源代码包路径（主要是rt.jar），Finish确认。
![](https://images0.cnblogs.com/i/539308/201406/221410003321004.jpg)
检查项目Properties的Build Path是否单独配置了其他的JRE System Library，类似的确认按上一步所述重新指向正确的JDK目录后，刷新工程，或再重复确认Add line number attributes to generated class files项的勾选状态，Apply一下，重新进入debug流程，一般情况下应该已经能够正常的暂停在JDK类源代码断点行上了。
![](https://images0.cnblogs.com/i/539308/201406/221420529572617.jpg)
