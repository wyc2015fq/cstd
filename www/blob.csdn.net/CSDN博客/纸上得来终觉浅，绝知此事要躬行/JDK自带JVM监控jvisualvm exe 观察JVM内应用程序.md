# JDK自带JVM监控jvisualvm.exe 观察JVM内应用程序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月05日 10:56:38[boonya](https://me.csdn.net/boonya)阅读数：3353








无论在测试环境还是在生产环境，我们都想知道程序在JVM中是否正常运行，除了使用第三方的一些工具，最直接的就是使用JDK自带的jvisualvm.exe。


## 系统主界面操作

JVM提供了本地的JVM监控，远程的JMX监控和快照服务。


![](https://img-blog.csdn.net/20170405103556114)

## VM示例：VisualVM

### 概述

![](https://img-blog.csdn.net/20170405104105304)




### 监视

![](https://img-blog.csdn.net/20170405104148106)




### 线程

![](https://img-blog.csdn.net/20170405104236685)




### 抽样器

![](https://img-blog.csdn.net/20170405104312843)





## 远程

此功能用于远程服务器的JVM监控，需要配置JMX。

![](https://img-blog.csdn.net/20170405104851742)

本地可以在$JAVA_HOME/bin/jmc.exe去查看。

## 快照

快照主要是截取运行在一定间隔时间点应用程序监控数据的截图。

### 概述

![](https://img-blog.csdn.net/20170405105136544)




### 监视

![](https://img-blog.csdn.net/20170405105212778)

### 线程

![](https://img-blog.csdn.net/20170405105224903)



















