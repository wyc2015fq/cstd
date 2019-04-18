# Spring RMI，Hessian/Burlap，HttpInvoker的区别 - z69183787的专栏 - CSDN博客
2015年05月19日 23:29:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：852
1 Hessian/Burlap 方式
    客户端通过Hessian/BurlapProxyFactoryBean生成的代理对象负责通过HTTP协议与远程服务通信。
![](http://hi.csdn.net/attachment/201103/16/0_1300248197961l.gif)
在服务端，HessianServiceExporter是一个Spring MVC的控制器，可以通过接收请求并将这些请求翻译成对POJO对象的调用将POJO输出为一个服务。
需要添加两处配置
（1）Spring配置文件处理URL。用来分发Hessian的URL请求给适当的Hessian服务Bean。
（2）在web.xml文件配置Spring的DispatcherServlet，并把应用部署为web应用。
![](http://hi.csdn.net/attachment/201103/16/0_1300248423wkWa.gif)
2 HttpInvoker 的远程调用方式
可以看出HttpInvoker和Hessian方式的区别仅仅在于客户端的ProxyFactoryBean和服务端的ServiceExporter的区别。
![](http://hi.csdn.net/attachment/201103/16/0_1300255241102r.gif)
3 三者的区别
相同点：客户端都是通过**ProxyFactoryBean创建代理的方式调用远程服务。
区别：   
通信方式：RMI通过JRMP，Hessian/Burlap 和HttpInvoker通过Http协议。
RMI ： java序列化方式，不能穿越防火墙，可用任意端口。
Hessian/Burlap ：基于HTTP协议；Hessian使用二进制格式，Burlap使用XML格式传输文件；使用自己的序列化方式。
HttpInvoker ：基于HTTP协议，使用java的序列化机制；只能用于Spring框架。
