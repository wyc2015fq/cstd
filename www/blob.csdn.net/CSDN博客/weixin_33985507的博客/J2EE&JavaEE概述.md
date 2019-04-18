# J2EE&JavaEE概述 - weixin_33985507的博客 - CSDN博客
2016年06月17日 17:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**来源**
Sun公司在1998年发表JDK1.2版本的时候， 使用了新名称Java 2 Platform，即“Java2平台”，修改后的JDK称为Java 2 Platform Software Develping Kit，即J2SDK。并分为标准版(Standard Edition，J2SE), 企业版(Enterprise Edition，J2EE)，微型版(MicroEdition，J2ME)。J2EE便由此诞生。
2005年6月，JavaOne大会召开，SUN公司公开Java SE 6。此时，Java的各种版本已经更名以取消其中的数字“2”：J2EE更名为Java EE, J2SE更名为Java SE，J2ME更名为Java ME。Java EE 是在 Java SE 的基础上构建的，它提供Web 服务、组件模型、管理和通信 API，可以用来实现企业级的面向服务体系结构（service-oriented architecture，SOA）和 Web 2.0应用程序。
J2EE技术架构，包含许多组件及标准规范，用来简化且规范应用系统的开发与部署，提高其安全、复用性、维护性及可移植性。J2EE核心是一组技术规范与指南，其中所包含的各类组件、服务架构及技术层次，均有共同的标准及规格，让各种依循J2EE架构的不同平台之间，存在良好的兼容性，解决过去企业后端使用的信息产品彼此之间无法兼容，企业内部或外部难以互通的窘境。
**J2EE说明书中定义了以下的J2EE组件：**
应用客户端程序和applets是客户层组件.
Java Servlet和JavaServer Pages(JSP）是web层组件.
Enterprise JavaBeans(EJB）是业务层组件.
**J2EE 典型的四层结构：**
运行在客户端机器上的客户层组件
运行在J2EE服务器上的Web层组件
运行在J2EE服务器上的业务逻辑层组件
运行在EIS服务器上的企业信息系统（Enterprise information system）层软件
**J2EE中的13种技术规范**
JDBC(Java Database Connectivity)
JNDI(Java Name and Directory Interface)
EJB(Enterprise JavaBean)
RMI(Remote Method Invoke)
Java IDL/CORBA(通用对象请求代理架构是软件构建的一个标准 )
JSP
Java Servlet
XML
JMS
JTA
JTS
JavaMail
JAF(JavaBeans Activation Framework)
**发展近况**
2013年6月[Java EE 7](http://www.iteye.com/news/27980)发布，下面的这个图表包含了Java EE 7中的各种组件。橙色部分为Java EE中新添加的组件。 
![30f0b05b-e3a5-3861-a759-720cceed0f4e.jpg](http://dl2.iteye.com/upload/attachment/0087/0515/30f0b05b-e3a5-3861-a759-720cceed0f4e.jpg)
[Java EE 7](http://www.iteye.com/news/27980)发布之后，Java开发团队在这段时间内一直在规划和搜集下一个大版本Java EE 8带来的新功能。 近日，Java EE 8中的JSR366部分已经通过JCP投票通过，正如大家所期待的Java EE 8包括HTTP2和HTML5支持、CDI/Bean托管的支持、云服务等，值得关注的特性如下：
## **Web****标准方面**
- HTTP2
- HTML5中Server-Sent Events(SSE) 允许服务端推送数据到客户端的功能
- 基于事件的web框架组件JSF
- 对超媒体的更多支持
- JSON-P、WebSocket和JAX-RS性能的提高
## **Bean****托管和上下文和依赖注入（CDI）方面**
- 上下文和依赖注入2（CDI 2）
- 提供了如EJB风格声明安全控制和调度容器服务
- 提供基于声明式的JMS消息监听器的MDB式CDI
- 废除了EJB2／CORBA一些老的接口
- JAX-RS 支持上下文和依赖注入（CDI）
## **云服务方面**
- 云服务相关配置
- 多租户的支持
- 简化的安全控制
- 管理和监控的REST风格API
[http://www.oracle.com/technetwork/java/javaee/overview/index.html](http://www.oracle.com/technetwork/java/javaee/overview/index.html)
[来自为知笔记(Wiz)](http://www.wiz.cn/i/d0bf0767)
