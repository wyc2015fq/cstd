# Zeroc Ice原理介绍 - 建建的博客 - CSDN博客
2017年05月02日 17:51:50[纪建](https://me.csdn.net/u013898698)阅读数：246
# **Ice介绍**
        Ice（Internet Communications Engine）是ZeroC公司的杰作，继承了CORBA的血统，是新一代的面向对象的分布式系统中间件。Ice是RPC通讯领域里最稳定、强大、高性能、跨平台、多语言支持的老牌开源中间件，特别适合于当前互联网领域中一个平台存在多种开发语言编程，以及网站和app应用并存的复杂大型项目。
        RPC（Remote Procedure Call Protocol 远程过程调用协议），是一种通过网络从远程计算机程序上请求服务，而不需要了解底层网络技术的协议。RPC假定某些传输协议的存在，如TCP或UDP，为通讯程序之间携带信息数据；在OSI网络通讯模型中，RPC跨越了传输层和应用层。
        Ice通过与编程语言无关的中立语言Slice（Specification Language fro Ice）来描述服务的接口，从而达到对象接口与其实现想分离的目的。
        目前Ice平台支持客户端API的语言有C++、.NET、Java、Python、Object-C、Ruby、PHP、JavaScript等。在服务器可以使用C、.NET、Java、Python等来开发。
# 关键特性
    1. 支持多语言之间的RPC互通。
    2. 高性能的RPC调用。
    3. 支持传统的RPC调用、异步调用、One-Way调用、批量发起请求，支持TCP通信、UDP通信等。
    4. 多平台支持。
    5. 不断更新，与时俱进。
# 通讯原理
![](https://img-blog.csdn.net/20161001083150933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        1. 开发者通过slice定义接口规范Printer.ice。
        2. 客户端和服务端分别通过对应的转换工具生成各自语言的接口。
        3. 客户端和服务器分别用各自的语言根据统一的接口实现具体逻辑代码。
        至于调用等底层实现由ICE帮我们完成，大大简化、节省了开发的时间。
# Ice RPC架构
![](https://img-blog.csdn.net/20161001083238403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        如图画红圈圈的为用户实现的代码。
        客户端和服务端具体代码实现
![](https://img-blog.csdn.net/20161001083458237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
参考
        1. 《ZeroC Ice 权威指南》
        2. 网络、龙果学院视频
下载
        Ice官方文档：Ice-3.6.1.pdf【[http://pan.baidu.com/s/1qYzqWzi](http://pan.baidu.com/s/1qYzqWzi)】
[Ice-3.5.1.pdf](http://pan.baidu.com/s/1jHTatZg)
