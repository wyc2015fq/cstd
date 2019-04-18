# Tomcat架构总结 - 草稿 - weixin_33985507的博客 - CSDN博客
2018年09月22日 21:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
# 简介
一个Tomcat中只有一个Server，一个Server可以包含多个Service,
一个Service只有一个Container，但是可以有多个Connectors.
# Service
Service主要包含两个部分：Connector和Container.
- 
Connector用于处理连接相关的事情，并提供Socket与Request和Response相关的转化.
```
Connector最底层使用的是Socket来进行连接的，
 Request和Response是按照HTTP协议来封装的，
 所以Connector同时需要实现TCP/IP协议和HTTP协议.
```
- 
Container用于封装和管理Servlet，以及具体处理Request请求.
# 请求处理流程
- 一个请求发送到Tomcat之后，首先经过Service然后会交给我们的Connector，
- Connector用于接收请求，并将接收的请求封装为Request和Response来具体处理.
- Request和Response封装完之后再交由Container进行处理，
- Container处理完请求之后再返回给Connector，
- 最后在由Connector通过Socket将处理的结果返回给客户端，这样整个请求的就处理完了.
# Connector
Connector就是使用ProtocolHandler来处理请求的，不同的ProtocolHandler代表不同的连接类型，
- 比如：Http11Protocol使用的是普通Socket来连接的，Http11NioProtocol使用的是NioSocket来连接的.
- ProtocolHandler由包含了三个部件：
- Endpoint 用来处理底层Socket的网络连接，实现TCP/IP协议.
Endpoint的抽象实现AbstractEndpoint里面定义的Acceptor和AsyncTimeout两个内部类和一个Handler接口。
Acceptor用于监听请求，
AsyncTimeout用于检查异步Request的超时，
Handler用于处理接收到的Socket，在内部调用Processor进行处理。
- Processor 用于将Endpoint接收到的Socket封装成Request.
- Adapter 用于将Request交给Container进行具体的处理.
# container
在Connector内部包含了4个子容器：
- 
Engine：引擎，用来管理多个站点，一个Service最多只能有一个Engine.
- 
Host：代表一个站点，也可以叫虚拟主机，通过配置Host就可以添加站点.
- 
Context：代表一个应用程序，对应着平时开发的一套程序，或者一个WEB-INF目录以及下面的web.xml文件.
- 
Wrapper：每一Wrapper封装着一个Servlet.
```
Context和Host的区别是Context表示一个应用，
我们的Tomcat中默认的配置下webapps下的每一个文件夹目录都是一个Context，
其中ROOT目录中存放着主应用，其他目录存放着子应用，
而整个webapps就是一个Host站点.
```
Container处理请求是使用Pipeline-Valve管道来处理的！（Valve是阀门之意），Pipeline-Valve是责任链模式.
Pipeline-Valve使用的责任链模式和普通的责任链模式有些不同，区别主要有以下两点：
- 每个Pipeline都有特定的Valve，而且是在管道的最后一个执行，这个Valve叫做BaseValve，BaseValve是不可删除的.
四个子容器对应的BaseValve：
StandardEngineValve
StandardHostValve
StandardContextValve
StandardWrapperValve
- 在上层容器的管道的BaseValve中会调用下层容器的管道.
Container容器处理请求的流程：
- Connector在接收到请求后会首先调用最顶层容器的Pipeline（EnginePipeline）来处理.
```
- 在Engine的管道中依次会执行EngineValve1、EngineValve2等等，最后会执行StandardEngineValve，
  在StandardEngineValve中会调用Host管道，然后再依次执行Host的HostValve1、HostValve2等，
  最后会执行StandardHostValve，然后再依次调用Context的管道和Wrapper的管道，
  最后执行到StandardWrapperValve.
- 在StandardWrapperValve中会创建FilterChain，并调用其doFilter方法来处理请求，
  FilterChain包含着我们配置的与请求相匹配的Filter和Servlet，
  其doFilter方法会依次调用所有的Filter的doFilter方法和Servlet的service方法，
  这样请求就得到了处理.
- 当所有的Pipeline-Valve都执行完之后，并且处理完了具体的请求，
  这个时候就可以将返回的结果交给Connector了，Connector在通过Socket的方式将结果返回给客户端.
```
- 责任链模式是指在一个请求处理的过程中有很多处理者依次对请求进行处理，
每个处理者负责做自己相应的处理，处理完之后将处理后的请求返回，
再让下一个处理着继续处理.
# 原文衔接
[四张图带你了解Tomcat系统架构](https://mp.weixin.qq.com/s?__biz=MzI4NTA1MDEwNg==&mid=2650765542&idx=1&sn=de9ad03456739f4f0ea17eb0cc721e5a&chksm=f3f9cd73c48e44651d51412f39b5cd6dde7705ed3adcc80ac31b221182d76af4fd6f4b05f8b7&mpshare=1&scene=1&srcid=02086k4eegtSsJORCcby3fLo&pass_ticket=zlEVer0M7NUvLluBOQVFq5xJmDv5QwkrunXXAXoCDPKoQOMNJGYSs5KvBdnT0URT#rd)
