# http流量复制工具goreplay - zhusongziye的博客 - CSDN博客





2018年12月26日 20:38:38[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：93








场景

一个待测服务，用来处理线上千万量级用户的各式请求；

问题

如果数据交换使用比较简单的xml、json等，可以设计各类case，去覆盖正常、异常的情况，但是如果数据交换格式比较复杂，且服务逻辑也比较复杂，这样的话就需要对代码逻辑非常熟悉才能设计全面的case；但是如果没有足够的时间去熟悉代码逻辑，那怎么能保证各类case都能覆盖到呢？

解决方案

今天介绍一款能快速解决上述问题的工具——goreplay

工具原理

官方介绍：

> 
GoReplay is the simplest and safest way to test your app using real traffic before you put it into production.

As your application grows, the effort required to test it also grows exponentially. GoReplay offers you the simple idea of reusing your existing traffic for testing, which makes it incredibly powerful. Our state of art technique allows to analyze and record your application traffic without affecting it. This eliminates the risks that come with putting a third party component in the critical path.

GoReplay increases your confidence in code deployments, configuration changes and infrastructure changes. Did we mention that no coding is required?

Here is basic workflow: The listener server catches http traffic and sends it to the replay server or saves to file. The replay server forwards traffic to a given address.


大概意思就是：goreplay是一款从生产环境copy流量到测试环境的工具，且不会影响生产环境的业务相应，又能很简单的达到复用http请求来进行稳定性测试 的目的；

![](https://img-blog.csdnimg.cn/20181226203616530.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

使用方法

这个工具可以直接从github上下载成品，如果你的服务器是centos7以上，应该可以直接解压后使用；

https://github.com/buger/goreplay

解压后只有一个文件：goreplay

他会依赖两个系统库，一般系统已经预置，如果没有，可以手动安装，详见这里：

https://github.com/buger/goreplay/wiki/Compilation

简单的场景下，这个工具使用方法非常简单

sudo ./goreplay执行

example：将请求到本机80端口的http请求，毫无变化的转发到192.168.100.100的1234端口上

./goreplay --input-raw :80 --output-http="http://192.168.100.100:1234"

优缺点

这个工具相比之前接触的一些工具的优点在于，在centos7以上的机器上不需编译或安装有内存或cpu消耗的程序，拷贝过来就可以直接使用；

缺点是这类工具的通病，需要操作生产环境服务器；

但是有需要 快速验证服务稳定性 需求的话，还是非常方便的；



Python软件开发测试交友群QQ:952490269（加群备注software），欢迎加入！



