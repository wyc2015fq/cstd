# 深入浅出，了解 Spring 框架和 Spring Boot 的历史 - 纯洁的微笑 - CSDN博客
2018年12月28日 09:09:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：130
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnqicF9lKuoiczINC81zKPwjoD6Lc1iaYn9iavribgcR6jvuWq4KMgdIWLjDDqobfrD8g8qln5rWv6OibB1g/640?wx_fmt=png)
转载自公号：开源中国  (ID:oschina2013)
Spring 框架可以说是 Java 开发人员使用的最流行的应用程序开发框架之一。它目前由大量提供一系列服务的模块组成。包括模块容器，为构建横切关注点提供支持的面向切面编程（AOP），安全框架，数据存取框架，Web 应用框架和用于模块测试提供支持的类。
Spring 框架的所有组件都通过依赖注入粘在一起。依赖注入（也称为控制反转）使设计和测试松散耦合的软件模块变得更容易。
多年来 Spring 框架已变得足够成熟。几乎所有 Java 企业应用需要用到的基础组件都可以在 Spring 框架中找到。但在一个新应用中将所有需要的 Spring 组件整合并配置好并不容易。这包括在 gradle/maven 中设置依赖库，使用 xml、注解或 java 代码配置需要的 Spring Bean。Spring 开发者意识到这里的绝大多数工作是可以可以自动化的，Spring Boot 出现了！
**Spring 框架的历史**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dkwuWwLoRK9KuhKUFXNbZnAtXEmo9UqWTAcH7Tv0rzPaGCKjGBP69f67HfNcTcLpA3z2vP8bDM8YbiaE7vrdwkw/640?wx_fmt=png)
**起点**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dkwuWwLoRK9KuhKUFXNbZnAtXEmo9UqW3x1O57ZPqia8Zv8E1ZoL22RDElxU7vkVf8JbpHjFscJcuolQDKSEDjw/640?wx_fmt=png)
2002 年 10 月，Rod Johnson 撰写了一本名为 Expert One-on-One J2EE 设计和开发的书。本书由 Wrox出版，介绍了当时 Java 企业应用程序开发的情况，并指出了 Java EE 和 EJB 组件框架中的存在的一些主要缺陷。在这本书中，他提出了一个基于普通 Java 类和依赖注入的更简单的解决方案。
在书中，他展示了如何在不使用 EJB 的情况下构建高质量，可扩展的在线座位预留系统。为了构建应用程序，他编写了超过 30,000 行的基础结构代码。包含许多可重用的 Java 接口和类，如 ApplicationContext和BeanFactory。由于java接口是依赖注入的基本构建块，因此他将这些类的根包命名为com.interface21。
一对一的 J2EE 设计和开发一炮而红。本书免费提供的大部分基础架构代码都是高度可重用的。即使在 15 年后，本书及其原则仍然与构建高质量的 Java Web 应用程序相关。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dkwuWwLoRK9KuhKUFXNbZnAtXEmo9UqWTAcH7Tv0rzPaGCKjGBP69f67HfNcTcLpA3z2vP8bDM8YbiaE7vrdwkw/640?wx_fmt=png)
**Spring 诞生 **
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dkwuWwLoRK9KuhKUFXNbZnAtXEmo9UqW3x1O57ZPqia8Zv8E1ZoL22RDElxU7vkVf8JbpHjFscJcuolQDKSEDjw/640?wx_fmt=png)
在本书发布后不久，开发者 Juergen Hoeller 和 Yann Caroff 说服 Rod Johnson 创建一个基于基础结构代码的开源项目。Rod，Juergen 和 Yann 于 2003 年 2 月左右开始合作开发该项目 。Yann 为新框架创造了“Spring”的名字。据 Rod 介绍，Spring 是传统 J2EE 的新开始。
200 年 6 月，Spring 2.0 在 Apache 2.0 许可下发布。2004 年 3 月，1.0 版发布。有趣的是，在1.0发布之前，spring 就被开发人员广泛采用。2004 年 8 月，Rod Johnson，Juergen Hoeller，Keith Donald 和Colin Sampaleanu 共同创立了一家专注于 Spring 咨询，培训和支持的公司 interface21。
Yann Caroff 在早期离开了团队，Rod Johnson 在 2012 年离开，Juergen Hoeller 仍然是 Spring 开发团队的积极成员。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dkwuWwLoRK9KuhKUFXNbZnAtXEmo9UqWTAcH7Tv0rzPaGCKjGBP69f67HfNcTcLpA3z2vP8bDM8YbiaE7vrdwkw/640?wx_fmt=png)
**Spring 框架的快速增长**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dkwuWwLoRK9KuhKUFXNbZnAtXEmo9UqW3x1O57ZPqia8Zv8E1ZoL22RDElxU7vkVf8JbpHjFscJcuolQDKSEDjw/640?wx_fmt=png)
自 2004 年 1.0 版本发布以来，Spring 框架迅速发展。Spring 2.0 于 2006 年 10 月发布，到那时，Spring的下载量超过了 100 万。Spring 2.0 具有可扩展的 XML 配置功能，用于简化 XML 配置，支持 Java 5，额外的 IoC 容器扩展点，支持动态语言。
在 Rod 领导下管理 Interface21 项目于 2007 年 11 月更名为 SpringSource。同时发布了 Spring 2.5。Spring 2.5 中的主要新功能包括支持 Java 6 / Java EE 5，支持注释配置，classpath 中的组件自动检测和兼容 OSGi 的 bundle。
2007 年，SpringSource 从基准资本获得了 A 轮融资（1000万美元）。
SpringSource 在此期间收购了多家公司，如Hyperic，G2One 等。2009年8月，SpringSource 以 4.2 亿美元被 VMWare 收购。SpringSource 在几周内收购了云代工厂，这是一家云 PaaS 提供商。2015 年，云代工厂转型成了非营利云代工厂。
2009 年 12 月，Spring 3.0 发布。Spring 3.0 具有许多重要特性，如重组模块系统，支持 Spring 表达式语言，基于 Java 的 bean 配置（JavaConfig），支持嵌入式数据库（如 HSQL，H2 和 Derby），模型验证/ REST 支持和对 Java EE 的支持。
2011 年和 2012 年发布了许多 3.x 系列的小版本。2012 年 7 月，Rod Johnson 离开了团队。2013 年 4月，VMware 和 EMC 通过 GE 投资创建了一家名为 Pivotal 的合资企业。所有的 Spring 应用项目都转移到了 Pivotal。
2013 年 12 月，Pivotal 宣布发布 Spring 框架 4.0。Spring 4.0 是 Spring 框架的一大进步，它包含了对Java 8 的全面支持，更高的第三方库依赖性（groovy 1.8+，ehcache 2.1+，hibernate 3.6+等），Java EE 7 支持，groovy DSL for bean 定义，对 websockets 的支持以及对泛型类型的支持作为注入 bean 的限定符。
2014 年至 2017 年期间发布了许多 Spring 框架 4.xx 系列版本。Spring 4.3.7 于 2017 年 3 月发布。Spring 4.3.8 于 2017 年 4 月发布，并成为 4.x 系列中的最后一个。Spring 框架的下一个延续的主要版本是在 5.0 版本展开。
**Spring Boot 的历史**
### 2012 年 10 月，Mike Youngstrom 在 Spring jira 中创建了一个功能请求，要求在 Spring 框架中支持无容器 Web 应用程序体系结构。他谈到了在主容器引导 spring 容器内配置 Web 容器服务。这是 jira 请求的摘录：
> 
我认为 Spring 的 Web 应用体系结构可以大大简化，如果它提供了从上到下利用 Spring 组件和配置模型的工具和参考体系结构。在简单的 main（）方法引导的 Spring 容器内嵌入和统一这些常用Web 容器服务的配置。
这一要求促使了 2013 年初开始的 Spring Boot 项目的研发。2014 年 4 月，Spring Boot 1.0.0 发布。从那以后，一些 Spring Boot 版本出来了：
- 
**Spring boot 1.1（2014 年 6 月）** - 改进的模板支持，gemfire 支持，elasticsearch 和 apache solr 的自动配置。
- 
**Spring Boot 1.2（2015 年 3 月）** - 升级到 servlet 3.1 / tomcat 8 / jetty 9，spring 4.1 升级，支持 banner / jms / SpringBootApplication 注解。
- 
**Spring Boot 1.3（2016 年 12 月）** - Spring 4.2 升级，新的 spring-boot-devtools，用于缓存技术（ehcache，hazelcast，redis 和 infinispan）的自动配置以及完全可执行的 jar 支持。
- 
**Spring boot 1.4（2017年1月）** - spring 4.3 升级，支持 couchbase / neo4j，分析启动失败和RestTemplateBuilder。
- 
**Spring boot 1.5（2017年2月）** - 支持 kafka / ldap，第三方库升级，弃用 CRaSH 支持和执行器记录器端点以动态修改应用程序日志级别。
- 
**Spring boot 2.0（2018 年 03 月）**-基于 Java 8，支持 Java 9，支持 Quartz ，调度程序大大简化了安全自动配置，支持嵌入式 Netty
Spring Boot 简单性使 java 开发人员能够快速大规模地采用该项目。Spring Boot 可以说是在 Java 中开发基于 REST 的微服务 Web 应用程序的最快方法之一。
**Spring IO 和 Spring Boot**
### 2014 年 6 月，Spring io 1.0.0 发布。Spring io 代表应用程序库之间的一组预定义的依赖关系。这意味着如果使用特定的 Spring IO 版本创建项目，则不再需要定义库的版本。
即使是 Spring Boot 启动项目也是这个 Spring IO 的一部分。例如，如果你使用的是 spring io 1.0.0，则在添加启动器项目的依赖关系时，不需要指定 spring 启动版本。它会自动假定它是spring boot 1.1.1.RELEASE。
概念上，spring io 由模块的基础层和执行层域特定的运行时（DSR）组成。基础层代表核心弹簧模块和第三方依赖关系的策划清单。Spring Boot 是 Spring IO 提供的执行层 DSR 之一。因此，现在有两种主要的方法来构建应用程序：
- 
使用或不使用 spring io 直接使用 Spring Boot。
- 
使用所需 Spring 模块的 Spring IO。
请注意，通常每当发布新的 Spring 框架版本时，都会触发新的 Spring 启动版本。这将反过来引发新的Spring Boot 版本。
2015 年 11 月，Spring io 2.0.0 发布。这提供了一套更新的依赖项，包括 Spring Boot 1.3。2016 年 7月，Spring io 团队决定使用按字母顺序排版的方案。Spring IO 为此使用城市名称。在字母版本方案中，新名称表示依赖库的次要和主要升级。
2016 年 9 月，雅典发布了首个按字母顺序排列的城市命名的 Spring io 平台版本。它包含 Spring Boot 1.4 和其他第三方库升级。此后，雅典发布了多个服务版本（SR1，SR2，SR3和SR4）。
2017 年 3 月，最新的 Spring io平台（Brussels-SR1）发布。它使用最新的 Spring Boot 版本 1.5.2。下一个 Spring io 平台是开罗计划推出的 spring boot 2.0 和 spring framework 5.0。
**Spring 时间线图表**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/dkwuWwLoRK9KuhKUFXNbZnAtXEmo9UqWjUT2crX2pXywLuzkGBXmYIG1yw44kAZJEdkOyvzjrtuDNYhZHNy0qg/640?wx_fmt=jpeg)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/dkwuWwLoRKicLMSBQwMJh2CAFYq0Vic0Q8Z7oicWvA5lqA69VOhOjbu7pOMw2wl4j2HgzQ65l8PGXmaciatDUJT95w/640?wx_fmt=gif)
**福利来袭：**
最后给大家推荐一个我写的 Spring Boot 课程，帮忙大家快速学习 Spring Boot ,快速将 Spirng Boot 技术应用于工作中，此教程主要抽取了实际工作中的使用场景，用课程和示例项目的形式来展示，适合初中级工程师订阅学习。这两天课程做活动特价，感兴趣的朋友可以扫描下方二维码了解。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqicF9lKuoiczINC81zKPwjoD1biaNaFLjJBhPbennCp9ult0hhl4IRlb0P4BNNnqSntpvSa4Fty8RSg/640?wx_fmt=jpeg)
**限时半价，不容错过，欢迎扫码了解详情！**
