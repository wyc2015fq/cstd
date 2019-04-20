# Spring Boot 到底是怎么运行的，你知道吗？ - 纯洁的微笑 - CSDN博客
2019年03月20日 09:30:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：294
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnrIddvuKIU7ibrFdzl7DRZAexyibYqlJiaia2owpCVJTbibc0xxrI0n6QZDEg9hC1KjlmnBB6Zc8VaNAvw/640?wx_fmt=jpeg)
导读
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/b2YlTLuGbKDsbJzupnILVFhPtMaRjmvPKYRqTMjibE9pnd8oiawLVrQbOHQe4wBXkBQkzpKCWPKBqWgOLgwccBug/640?wx_fmt=png)
Spring Boot方式的项目开发已经逐步成为Java应用开发领域的主流框架，它不仅可以**方便地创建生产级的Spring应用程序**，还能轻松地通过一些注解配置与目前比较流行的微服务框架SpringCloud快速地集成。
在我们实际使用Spring Boot进行项目开发的过程中，往往只需要几个很简单的注解配置就能够将应用启动运行了，相比于传统的Spring项目而已，这种提升大大地提高了我们的研发效率。然而，这种便捷性则是通过高度地上层封装来实现的，如：**“大量的注解封装、约定大于配置的原则“**等手段。所以，也许你已经使用Spring Boot开发很多个项目了，但对Spring Boot的运行原理真的搞清楚了吗？如果，面试中有人问你Spring Boot的实现原理是什么？你能正确地回答出来吗？
与大部分其他框架及技术的使用场景一样，我们**往往过多地专注于使用层面**，以便快速地完成业务开发，却往往**忽略了对框架底层运行原理的关注**，所以面试中被怼也就不足为奇了。不过没关系，在今天的文章中，小码哥将为大家全方位地梳理下Spring Boot的底层运行原理，并通过图文结合的方式给大家进行展示，希望对您的工作或者面试能够有所帮助！
Spring Boot运行原理
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/b2YlTLuGbKDsbJzupnILVFhPtMaRjmvPKYRqTMjibE9pnd8oiawLVrQbOHQe4wBXkBQkzpKCWPKBqWgOLgwccBug/640?wx_fmt=png)
实际上**Spring Boot并不是要替代Spring框架**，我们知道在**JDK1.5推出注解功能**以后，Spring框架实现了大量的注解来替代原有的基于XML的配置，主要用于配置管理、Bean的注入以及AOP等相关功能的实现。然而，随着Spring注解的数量越来越多，并且被大量的使用，尤其是**相同的多个注解会被大量重复地用到各个类或者方法中**。这样就**导致了繁琐的配置及大量冗余的代码**。
到这里你也许就会想到既然这么多Spring注解很繁琐，那么可不可以将其**组合**一下呢？通过**定义一些新的注解，将功能进行分类，不同的Spring注解通过新的注解定义进行一定的组合**，这样对于大部分通用场景下，只需要引入一个新的注解，就自动包含了与之相关的其他Spring注解？**没错！Spring Boot说到底就是这么个玩意**！
但是，要**实现注解的组合并不是简单的把多个注解牵强的叠加在一起**，这里涉及到一些编程语言上的实现，例如要组合一个注解，那么该注解是否支持注解到别的注解上呢（略微有点拗口）？还有如果组合注解后，因为注解的背后还涉及到**Spring容器上下文的初始化**以及Bean注入相关的逻辑，如果一个A注解涉及的Bean，涉及到另外一个B注解涉及到的Bean的初始化；也就意味着A注解的Bean初始化，需要在B注解的Bean初始化完成后才能进行注入，否则就会导致Bean依赖注入的失败。
**Spring Boot框架本质上就是通过组合注解的方式实现了诸多Spring注解的组合**，从而极大地简化了Spring框架本身的繁琐配置，实现快速的集成和开发。只是要这样实现，也需要一定的**基础条件**！
**元注解**
说到底**Spring Boot框架是在Spring框架的基础上做了一层二次封装**，最重要的特点就是Spring Boot框架定义了一些新的注解来实行一些Spring注解的组合，而Spring注解则是基于JDK1.5+后的注解功能的支持来完成的。
关于JDK的注解如果想要注解到别的注解上，就需要将其定义为**元注解**，所谓的元注解，就是可以**注解到其他注解上的注解**，被注解的注解就是我们上面说到的**组合注解**。而Spring框架的很多注解都是可以作为元注解的，并且Spring框架本身也实现了很多组合注解，例如我们常用的**@Configuration**就是一个这样的组合注解。因此，有了这样一个条件Spring Boot的实现才有了基础条件！
**条件注解****@Conditional**
Spring 4提供了一个通用的基于条件的注解**@Conditional**。该注解可以**根据满足某一个特定条件与否来决定是否创建某个特定的Bean**，例如，某个依赖包jar在一个类路径的时候，自动配置一个或多个Bean时，可以通过**@Conditional**注解来实现只有某个Bean被创建时才会创建另外一个Bean，这样就可以依据特定的条件来控制Bean的创建行为，这样的话我们就可以利用这样一个特性来实现一些自动的配置。
而**这一点对于Spring Boot实现自动配置来说是一个核心的基础能力**，从本质上来说Spring Boot之所以可以实现自动注解配置很大程度上也是基于这一能力。在Spring Boot中以@**Conditional**为元注解又重新定义了一组针对不同场景的组合条件注解，它们分别是：
- 
**@ConditionalOnBean**：当容器中有指定Bean的条件下进行实例化。
- 
**@ConditionalOnMissingBean**：当容器里没有指定Bean的条件下进行实例化。
- 
**@ConditionalOnClass**：当classpath类路径下有指定类的条件下进行实例化。
- 
**@ConditionalOnMissingClass**：当类路径下没有指定类的条件下进行实例化。
- 
**@ConditionalOnWebApplication**：当项目是一个Web项目时进行实例化。
- 
**@ConditionalOnNotWebApplication**：当项目不是一个Web项目时进行实例化。
- 
**@ConditionalOnProperty**：当指定的属性有指定的值时进行实例化。
- 
**@ConditionalOnExpression**：基于SpEL表达式的条件判断。
- 
## **@ConditionalOnJava**：当JVM版本为指定的版本范围时触发实例化。
- 
## **@ConditionalOnResource**：当类路径下有指定的资源时触发实例化。
- 
## **@ConditionalOnJndi**：在JNDI存在的条件下触发实例化。
- 
## **@ConditionalOnSingleCandidate**：当指定的Bean在容器中只有一个，或者有多个但是指定了首选的Bean时触发实例化。
纵观Spring Boot的一些核心注解，基于**@Conditional元注解的组合注解就占了很大部分**，所以Spring Boot的核心功能基于就是这些注解实现的。在Spring Boot源码项目“**spring-boot-autoconfigure**”中，随意打开一个AutoConfiguration文件，我们都会看到有上述条件注解的使用。如：
```java
@Configuration
@ConditionalOnClass(DSLContext.class)
@ConditionalOnBean(DataSource.class)
@AutoConfigureAfter({ DataSourceAutoConfiguration.class,
        TransactionAutoConfiguration.class })
public class JooqAutoConfiguration {
    @Bean
    @ConditionalOnMissingBean
    public DataSourceConnectionProvider dataSourceConnectionProvider(
            DataSource dataSource) {
        return new DataSourceConnectionProvider(
                new TransactionAwareDataSourceProxy(dataSource));
    }
    @Bean
    @ConditionalOnBean(PlatformTransactionManager.class)
    public SpringTransactionProvider transactionProvider(
            PlatformTransactionManager txManager) {
        return new SpringTransactionProvider(txManager);
    }
    ....
}
```
## **Spring Boot运行原理**
在前面的篇幅中我们重点阐述了为什么Spring Boot可以实现高度地自动化配置。那么，接下来我们就结合Spring Boot最核心的组合注解**@SpringBootApplication**来分析下Spring Boot的项目到底是怎么启动运行的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/l89kosVutomRw35icEVq1E3whq2N8wRG0HzZd4qf5Cgkl7avohLfBL6z333yBeHIxRbBVuMNkfY9nD61FbVjuuA/640?wx_fmt=png)
@SpringBootApplication注解实际上是一个组合注解，除了对应用开放的@ComponentScan注解（实现对开发者自定义的应用包扫描）外，其最核心的注解就是**@EnableAutoConfiguration**，该注解表示开启自动配置功能，而在具体的实现上则是通过导入**@Import(EnableAutoConfigurationImportSelector.class)**类的实例，在逻辑上实现了对所依赖的核心jar下***META-INF/spring.factories***文件的扫描，该文件则声明了有哪些自动配置需要被Spring容器加载，从而Spring Boot应用程序就能自动加载Spring核心容器配置，以及其他依赖的项目组件配置，从而最终完成应用的自动初始化，通过这种方法就向开发者屏蔽了启动加载的过程。
如“spring-boot-autoconfigure”核心包中的***META-INF/spring.factories***文件就是定义了需要加载的Spring Boot项目所依赖的基础配置类，如Spring的容器初始化配置类等。如：
```java
# Initializers
org.springframework.context.ApplicationContextInitializer=\
org.springframework.boot.autoconfigure.SharedMetadataReaderFactoryContextInitializer,\
org.springframework.boot.autoconfigure.logging.ConditionEvaluationReportLoggingListener
# Application Listeners
org.springframework.context.ApplicationListener=\
org.springframework.boot.autoconfigure.BackgroundPreinitializer
.....
```
而对于大部分第三方需要与Spring Boot集成的框架，或者我们日常开发中需要进行抽象的公共组件而言，得益于这种机制，也可以很容易地定制成开箱即用的各种Starter组件。而使用这些组件的用户，往往只需要将依赖引入就好，不再需要进行任何额外的配置了！
Spring Boot后记
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/b2YlTLuGbKDsbJzupnILVFhPtMaRjmvPKYRqTMjibE9pnd8oiawLVrQbOHQe4wBXkBQkzpKCWPKBqWgOLgwccBug/640?wx_fmt=png)
以上就是Spring Boot运行的基本原理了，希望这篇文章能够对你有所帮助！实际上学习Spring Boot进行项目开发关键就是要掌握各种**Spring及Spring Boot的各种注解，特别是一些关键核心注解**。同样在进行基于Spring Cloud微服务的开发中，也是需要理解Spring Cloud相关组件所提供的各种核心注解，只有这样才能更好的理解框架的原理及使用，而不只是云里雾里地进行各种似懂非懂的Copy开发。
本周六晚上8点，知识星球分享内容预告。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnrIddvuKIU7ibrFdzl7DRZAeNHEhA4iaeRa1prSMUW7Lv2Nuj7lpZ1NKwkpdicBDuetZhRDg2F333nibQ/640?wx_fmt=png)
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
