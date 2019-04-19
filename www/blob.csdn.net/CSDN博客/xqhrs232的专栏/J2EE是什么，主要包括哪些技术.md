# J2EE是什么，主要包括哪些技术 - xqhrs232的专栏 - CSDN博客
2018年10月19日 10:54:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：42
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/Ashes18/article/details/73614571](https://blog.csdn.net/Ashes18/article/details/73614571)
相关文章
1、J2EE、Java SE、Java EE、Java ME 区别----[https://blog.csdn.net/yanming19891101/article/details/4786784?utm_source=blogxgwz1](https://blog.csdn.net/yanming19891101/article/details/4786784?utm_source=blogxgwz1)
2、Java EE 包含哪些技术----[https://blog.csdn.net/wujingjing_crystal/article/details/52839667?utm_source=blogxgwz0](https://blog.csdn.net/wujingjing_crystal/article/details/52839667?utm_source=blogxgwz0)
3、J2EE平台的13种核心技术----[https://blog.csdn.net/u011225629/article/details/48347507?utm_source=blogxgwz0](https://blog.csdn.net/u011225629/article/details/48347507?utm_source=blogxgwz0)
4、J2EE核心技术简介----[https://blog.csdn.net/delong_liu/article/details/79464165?utm_source=blogxgwz4](https://blog.csdn.net/delong_liu/article/details/79464165?utm_source=blogxgwz4)
最近最为深刻的认识：被面试老师问到了的知识，哪些是在自己心中模棱两可，似是而非的概念都一目了然。而后，只有在顿悟中不断总结才能进步。今天，我总结的部分是J2EE到底是什么东西，它包括了哪些技术。
一、J2EE是什么？
从整体上讲，J2EE 是使用 Java 技术开发企业级应用的工业标准，它是 Java 技术不断适应和促进企业级应用过程中的产物。 
适用于企业级应用的 J2EE，提供一个平台独立的、可移植的、多用户的、安全的和基于标准的企业级平台，从而简化企业应用的 
开发、管理和部署。J2EE 是一个标准，而不是一个现成的产品。（来自百度）
二、J2EE主要包括哪些技术
servlet 
Servlet是java平台上的CGI技术。Servlet在服务器端运行，动态地生成Web页面。与传统的CGI和许多其他 
类似CGI的 技术相比，Java Servlet具有更高的效率并更容易使用。 
对于Servlet，重复的请求不会导致同一程序的多次转载，它是依靠线程的方式来支持并发访问的。
JSP（Java server Page） 
JSP是一种实现普通静态HTML和动态页面输出混合编码的技术。从这一点来看，非常类似Microsoft ASP、 
PHP等技术。借助形式上的内容和外观表现的分离，Web页面制作的任务可以比较方便地划分给页面设计人员，并 
方便地通过JSP来合成。在运行时态，JSP将会被首先转换成Servlet，并以Servlet的形态编译运行，因此它的效 
率和功能与Servlet相比没有差别，一样具有很高的效率。
JDBC 
JDBC（java Database Connectivity，java数据库连接）API是一个标准SQL（Structured Query 
Language， 结构化查询语言）数据库访问接口，它使数据库开发人员能够用标准java API编写数据库应用程序。 
JDBC API主要用来连接数据库和直接调用SQL命令执行各种SQL语句。利用JDBC API可以执行一般的SQL语句、 
动态SQL语句以及带IN和OUT参数的存储过程。Java中的JDBC相当于Microsoft平台中的ODBC（open Database Connectivity）
EJB 
EJB定义了一组可重用的组件：Enterprise Beans。开发人员可以利用这些组件，像搭积木一样建立分布式应用。 
（参考极客学院面试宝典）
补充总结：
1. javaEE应用的分层模型大致分为以下几层：
（1）Domain Object（领域对象）层： 
此层由一系列的POJO（Plain Old Java Object，普通的、传统的java 
对象）组成，这些对象是该系统的Domain Object，往往包含了各自所需实现的业务逻辑方法。
（2）DAO（Data Access Object，数据访问对象）层：此层由一系列的DAO组件组成，这些DAO实现了对数据库的创建、查询、更新和删除（CRUD）等原子操作。
（3）业务逻辑层：此层由一系列的业务逻辑对象组成，这些业务逻辑对象实现了系统所需要的业务逻辑方法。这些 
业务逻辑方法可能仅仅用于暴露Domain Object对象所实现的业务逻辑方法，也可能是依赖DAO组件实现的业务 
逻辑方法。
（4）控制器层：此层由一系列控制器组成，这些控制器用于拦截用户请求，并调用业务逻辑组件的业务逻辑方法， 
处理用户请求，并根据处理结果转发到不同的表现层组件。
（5）表现层：此层由一系列的JSP页面，Velocity页面，PDF文档视图组件组成，负责收集用户请求，并显示处理结果。
各层的javaEE组件之间以松耦合的方式耦合在一起，各组件并不以硬编码方式耦合，这种方式是为了应用以后的扩展性。 从上向下，上面组件的实现依赖于下面组件的功能；从下向上，下面组件支持上面组件的实现。
2. javaEE应用组件：
JavaEE构架提供了良好的分离，隔离了各组件之间的代码依赖，javaEE应用大致包括以下几类组件：
（1）表现层组件：主要负责收集用户输入数据，或者向客户显示系统状态。最常用的表现层技术是JSP，还可以是Velocity等技术。
（2）控制器组件：对于JavaEE的MVC框架而言，框架提供一个前端核心控制器，而核心控制器负责拦截用户请求，并将请求转发 
给用户实现的控制器组件。而这些用户实现的控制器则负责处理调用业务逻辑方法，处理用户请求。
（3）业务逻辑组件：是系统的核心组件，实现系统的业务逻辑。通常一个业务逻辑方法对应一次用户操作。一个业务逻辑方法应该 
是一个整体，因此要求对业务逻辑方法增加事务性。业务逻辑方法仅仅负责实现业务逻辑，不应该进行数据库访问。因此，业务逻辑 
组件中不应该出现原始的Hibernate，JDBC等API。
（4）DAO组件：Data Access Object，也被称为数据访问对象。这个类型的对象比较缺乏变化，每个DAO组件都提供Domain 
Object 对象基本的创建、查询、更新和删除等操作，这些操作对应于数据表的CURD等原子操作。当然，如果采用
--------------------- 
作者：待长的小蘑菇 
来源：CSDN 
原文：https://blog.csdn.net/Ashes18/article/details/73614571 
版权声明：本文为博主原创文章，转载请附上博文链接！
