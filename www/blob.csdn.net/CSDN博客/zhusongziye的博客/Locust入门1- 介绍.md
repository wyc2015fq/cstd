# Locust入门1- 介绍 - zhusongziye的博客 - CSDN博客





2018年10月28日 19:36:04[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：39








转自：http://www.testclass.net/

Locust 官方网站：https://www.locust.io/

Locust 介绍

An open source load testing tool.

一个开源性能测试工具。

define user behaviour with python code, and swarm your system with millions of simultaneous users.

使用 Python 代码来定义用户行为。用它可以模拟百万计的并发用户访问你的系统。

![](https://img-blog.csdnimg.cn/2018102819353540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_27,color_FFFFFF,t_70)



性能工具对比

LoadRunner 是非常有名的商业性能测试工具，功能非常强大。使用也比较复杂，目前大多介绍性能测试的书籍都以该工具为基础，甚至有些书整本都在介绍 LoadRunner 的使用。

Jmeter 同样是非常有名的开源性能测试工具，功能也很完善，在本书中介绍了它作为接口测试工具的使用。但实际上，它是一个标准的性能测试工具。关于Jmeter相关的资料也非常丰富，它的官方文档也很完善。

Locust 同样是性能测试工具，虽然官方这样来描述它 “An open source load testing tool.” 。但其它和前面两个工具有着较大的不同。相比前面两个工具，功能上要差上不少，但它也并非优点全无。
- 
Locust 完全基本 Python 编程语言，采用 Pure Python 描述测试脚本，并且 HTTP 请求完全基于 Requests 库。除了 HTTP/HTTPS 协议，Locust 也可以测试其它协议的系统，只需要采用Python调用对应的库进行请求描述即可。

- 
LoadRunner 和 Jmeter 这类采用进程和线程的测试工具，都很难在单机上模拟出较高的并发压力。Locust 的并发机制摒弃了进程和线程，采用协程（gevent）的机制。协程避免了系统级资源调度，由此可以大幅提高单机的并发能力。


正是基于这样的特点，使我选择使用Locust工具来做性能测试，另外一个原因是它可以让我们换一种方式认识性能测试，可能更容易看清性能测试的本质。

我想已经成功的引起了你的兴趣，那么接下来就跟着来学习Locust的使用吧。



