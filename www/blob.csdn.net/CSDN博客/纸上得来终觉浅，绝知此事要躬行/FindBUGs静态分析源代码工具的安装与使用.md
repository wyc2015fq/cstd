# FindBUGs静态分析源代码工具的安装与使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月29日 18:07:54[boonya](https://me.csdn.net/boonya)阅读数：661










findbugs插件的快捷方式足以为我们探索项目中需要优化的代码以数字标识。这个工具的使用很简单，就不过多做教学了。静态分析工具承诺无需开发人员费劲就能找出代码中已有的缺陷。当然，如果有多年的编写经验，就会知道这些承诺并不是一定能兑现。所以工作中编码规范和常识很重要。

## 什么是FindBUGs

FindBugs是一个能静态分析源代码中可能会出现Bug的Eclipse插件工具。

![](http://static.oschina.net/uploads/img/201207/10232344_jHWp.png)

## Eclipse安装findbugs

安装步骤：

1.点击“Help->InstallNew Software”，如下图：



![](https://img-blog.csdn.net/20140401085052093)

2.点击“Add”，然后在弹出框“Name”输入“findBugs”，“Location”输入“**http://findbugs.cs.umd.edu/eclipse**”，点击“OK”，如下图：

![](https://img-blog.csdn.net/20140401085101421)

3.选择对应插件，然后点击“next->next->finish”。

![](https://img-blog.csdn.net/20140401085106109)



4.完成安装之后重启eclipse，右击项目文件或目录，会发现多了Findbugs的菜单，如下图：

![](https://img-blog.csdn.net/20140401085120031)



上面这种方式来自：[http://blog.csdn.net/gaofuqi/article/details/22679609](http://blog.csdn.net/gaofuqi/article/details/22679609)懒得去写步骤了，有需要的将就着看吧。



## 其他安装方式



1、通过Eclipse的help>Eclipse Makertplace进行探索安装：输入findbugs即可查询到目标插件。

2、下载插件包：[https://sourceforge.net/projects/findbugs/files/](https://sourceforge.net/projects/findbugs/files/)

注：安装的方式太多了，选择合适自己的就好，最终的目的是检查出存在缺陷的代码。



更多更全参考资料：

[常用 Java 静态代码分析工具的分析与比较](https://www.ibm.com/developerworks/cn/java/j-lo-statictest-tools/)





