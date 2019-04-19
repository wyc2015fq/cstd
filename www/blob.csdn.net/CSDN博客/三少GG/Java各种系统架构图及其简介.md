# Java各种系统架构图及其简介 - 三少GG - CSDN博客
2013年05月07日 23:39:32[三少GG](https://me.csdn.net/scut1135)阅读数：5029

**各种系统架构图及其简介**
转载请保留出处,不胜[**人生**]()一场醉汇总。
以下文字和架构图均在本人相关系统[**设计**]()和架构方案中有所应用。
原文出处：http://space.itpub.net/6517/viewspace-609654
1.Spring架构图
 Spring是一个[**开源**]()框架，是为了解决[**企业**]()应用程序开发复杂性而创建的。框架的主要优势之一就是其分层架构，分层架构允许您选择使用哪一个组件，同时为J2EE应用程序开发提供集成的框架。Spring框架的功能可以用在任何J2EE服务器中，大多数功能也适用于不受[**管理**]()的环境。Spring的核心要点是：支持不绑定到特定J2EE服务的可重用业务和数据访问对象。这样的对象可以在不同J2EE环境
 （[**Web**]()或EJB）、独立应用程序、[**测试**]()环境之间重用。
![](http://space.itpub.net/attachments/2009/07/6517_200907200031341.gif)
组成Spring框架的每个模块（或组件）都可以单独存在，或者与其他一个或多个模块联合实现。每个模块的功能如下：
- 核心容器：核心容器提供Spring框架的基本功能。核心容器的主要组件是BeanFactory，它是工厂模式的实现。BeanFactory使用控制反转 （IOC） 模式将应用程序的配置和依赖性规范与实际的应用程序[**代码**]()分开。
- Spring上下文：Spring上下文是一个配置文件，向Spring框架提供上下文信息。Spring上下文包括企业服务，例如JNDI、EJB、电子邮件、国际化、校验和调度功能。
- Spring AOP： 通过配置管理特性，Spring AOP模块直接将面向方面的[**编程**]()功能集成到了Spring框架中。所以，可以很容易地使Spring框架管理的任何对象支持AOP。Spring AOP模块为基于Spring的应用程序中的对象提供了事务管理服务。通过使用Spring AOP，不用依赖EJB组件，就可以将声明性事务管理集成到应用程序中。
- Spring DAO：JDBC DAO抽象层提供了有意义的异常层次结构，可用该结构来管理异常处理和不同[**数据库**]()供应商抛出的错误消息。异常层次结构简化了错误处理，并且极大地降低了需要编写     的异常代码数量（例如打开和关闭连接）。Spring DAO的面向JDBC的异常遵从通用的DAO异常层次结构。
- Spring ORM：Spring框架插入了若干个ORM框架，从而提供了ORM的对象关系工具，其中包括JDO、Hibernate和iBatis [**SQL**]() Map。所有这些都遵从Spring的通用事务和DAO异常层次结构。
2.ibatis架构图
ibatis是一个基于[**Java**]()的持久层框架。iBATIS提供的持久层框架包括SQL Maps和[**Data**]() Access Objects（DAO），同时还提供一个利用这个框架开发的JPetStore实例。
![](http://space.itpub.net/attachments/2009/07/6517_200907200031342.jpg)
OGX%E@mx0
IBATIS：最大的优点是可以有效的控制sql发送的数目，提高数据层的执行效率！它需要程序员自己去写sql语句，不象hibernate那样是完全面向对象的，自动化的，ibatis是半自动化的，通过表和对象的映射以及手工书写的sql语句，能够实现比hibernate等更高的查询效率。
Ibatis只是封装了数据访问层，替我们做了部分的对象关系映射。但代价是必须要写[**xml**]()配置文件，相对于Hibernate还要写很多sql。Hibernate通过工具直接从数据库模式生成实体类和基本的配置文件，而且大部分情况下不需要我们写sql，会较大的提升开发效率。但这些也有很多的局限性，尤其是对环境的要求较高（数据库设计，对象设计，团队的协作等）。 个人感觉Ibatis对项目比较有意义的地方在于它小巧灵活，可扩展，封装了数据访问层（事务，缓存，异常，日志），并提供了DAO框架支持。
利用Ibatis我们可以做到代码和sql的分离，只要sql能够解决的问题，Ibatis就能帮我们较容易的解决，同时也使我们的项目对某一框架的依赖性变小（因为Ibatis是非侵入性的）。这将极大的降低项目风险，减少解决复杂问题的时间，使项目的维护变得简单。
Ibatis对于应用的修改，调试，扩充和维护将会变得容易自然。修改时，我们主要修改的是代表模型的实体对象，xml配置文件中的sql，和/或配置文件的ResultMap（很多时候是不需要的）。同时，sql和代码分离，我们不用在代码的StringBuffer的append方法之间寻找需要修改的sql。配置文件中的sql便利了我们的调试和对sql的评审及以后的sql重用。
3.structs1架构图
Struts是Apache基金会Jakarta项目组的一个Open Source项目，它采用MVC模式，能够很好地帮助java开发者利用J2EE开发Web应用。和其他的java架构一样，Struts也是面向对象设计，将MVC模式"分离显示逻辑和业务逻辑"的能力发挥得淋漓尽致。Structs框架的核心是一个弹性的控制层，基于如Java Servlets，JavaBeans，ResourceBundles与XML等标准[**技术**]()，以及Jakarta
 Commons的一些类库。Struts有一组相互协作的类（组件）、Serlvet以及[**jsp**]() tag lib组成。基于struts构架的web应用程序基本上符合JSP Model2的设计标准，可以说是一个传统MVC设计模式的一种变化类型。　　
Struts有其自己的控制器（Controller），同时整合了其他的一些技术去实现模型层（Model）和视图层（View）。在模型层，Struts可以很容易的与数据访问技术相结合，如JDBC / EJB，以及其它第三方类库，如Hibernate / iBATIS，或者Object Relational Bridge(对象关系桥)。在视图层，Struts能够与JSP，包括JSTL与JSF，以及Velocity模板，XSLT与其它表示层技术。
Struts为每个专业的Web应用程序做背后的支撑，帮助为你的应用创建一个扩展的开发环境。
![](http://space.itpub.net/attachments/2009/07/6517_200907200031343.jpg)
 Client browser（客户浏览器）
来自客户浏览器的每个HTTP请求创建一个事件。Web容器将用一个HTTP响应作出响应。
 Controller（控制器）
控制器接收来自浏览器的请求，并决定将这个请求发往何处。就Struts而言，控制器是以servlet实现的一个命令设计模式。struts-config.xml文件配置控制器。
 业务逻辑
业务逻辑更新模型的状态，并帮助控制应用程序的流程。就Struts而言，这是通过作为实际业务逻辑“瘦”包装的Action类完成的。
 Model（模型）的状态
模型表示应用程序的状态。业务对象更新应用程序的状态。ActionForm. bean在会话级或请求级表示模型的状态，而不是在持久级。JSP文件使用JSP标记读取来自ActionForm. bean的信息。
 View（视图）
视图就是一个JSP文件。其中没有流程逻辑，没有业务逻辑，也没有模型信息--只有标记。标记是使Struts有别于其他框架（如Velocity）的因素之一
4.structs2架构图
Struts 2相对于Struts 1.X，将实现用户业务逻辑（Action）同Servlet API分离开，这种分离机制，是采用了拦截器或者拦截器栈（拦截器链）。拦截器是Struts 2的核心内容之一。
Struts 2内建了多个拦截器和拦截器栈（由多个拦截器形成的拦截器链），将用户的Web请求进行拦截处理，从而提供了更加丰富的功能，例如数据类型转换、国际化、文件上传等。![](http://space.itpub.net/attachments/2009/07/6517_200907200031344.jpg)
5.Hibernate架构图
Hibernate是一个开放[**源代码**]()的对象关系映射框架，它对JDBC进行了非常轻量级的对象封装，使得Java程序员可以随心所欲的使用对象编程思维来 操纵数据库。Hibernate可以应用在任何使用JDBC的场合，既可以在Java的客户端程序使用，也可以在Servlet/JSP的Web应用中使用，最具革命 意义的是，Hibernate可以在应用EJB的J2EE架构中取代CMP，完成数据持久化的重任。
![](http://space.itpub.net/attachments/2009/07/6517_200907200031345.jpg)
　　Hibernate的核心接口一共有5个，分别为:Session、[SessionFactory](http://baike.baidu.com/view/1097304.htm)、Transaction、[Query](http://baike.baidu.com/view/209416.htm)和Configuration。这5个核心接口在任何开发中都会用到。通过这些接口，不仅可以对持久化对象进行存取，还能够进行事务控制。下面对这五个核心接口分别加以介绍。
·Session接口:Session接口负责执行被持久化对象的CRUD操作(CRUD的任务是完成与数据库的交流，包含了很多常见的SQL语句。)。但需要注意的是Session对象是非线程安全的。同时，Hibernate的session不同于JSP应用中的HttpSession。这里当使用session这个术语时，其实指的是Hibernate中的session，而 以后会将HttpSesion对象称为用户session。
·SessionFactory接口:SessionFactory接口负责初 始化Hibernate。它充当数据存储源的代理，并负责创建Session对象。这里用到了工厂模式。需要注意的是SessionFactory并不是 轻量级的，因为一般情况下，一个项目通常只需要一个SessionFactory就够，当需要操作多个数据库时，可以为每个数据库指定一个SessionFactory。
·Configuration接口:Configuration接口负责配置并启动Hibernate，创建SessionFactory对象。在Hibernate的启动的过程中，Configuration类的实例首先定位映射文档位置、读取配置，然后创建SessionFactory对象。
·Transaction接口:Transaction接口负责事务相关的操作。它是可选的，开发人员也可以设计编写自己的底层事务处理代码。
·Query和Criteria接口:Query和Criteria接口负责执行各种数据库查询。它可以使用HQL语言或SQL语句两种表达方式。
6.J2EE架构图
J2EE是一套全然不同于传统应用开发的技术架构，包含许多组件，主要可简化且规范应用系统的开发与部署，进而提高可移植性、安全与再用价值。
J2EE核心是一组技术规范与指南，其中所包含的各类组件、服务架构及技术层次，均有共通的标准及规格，让各种依循J2EE架构的不同平台之间，存在良好的兼容性，解决过去企业后端使用的信息产品彼此之间无法兼容，导致企业内部或外部难以互通的窘境。
![](http://space.itpub.net/attachments/2009/07/6517_200907200031346.jpg)
7.Oracle架构图
![](http://space.itpub.net/attachments/2009/07/6517_200907200031347.jpg)

