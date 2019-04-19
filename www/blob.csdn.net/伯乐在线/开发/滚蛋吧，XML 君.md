# 滚蛋吧，XML 君 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [yemengying](http://www.jobbole.com/members/giraffe0813) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
Long long time ago, 在《Spring In Action》一书中看到这么一句话
> 
**JavaConfig is the preferred option for explicit configuration because it’s more powerful, type-safe, and refactor-friendly **。
不过当时并不知道 `JavaConfig` 是神马东东就选择性忽略了。最近新的项目采用了 Spring Boot + JavaConfig ，接触了一个星期的 JavaConfig，感觉还不错，简单比较下它和 XML。由于接连看了 4 部韩国灾难片心情比较蓝瘦，就暂时不要指正错误了，错就错吧(づ｡◕‿‿◕｡)づ
![lalala](http://oduh99f02.bkt.clouddn.com/alibaba.jpg)
### XML
虽然早在 Spring 3 中就开始支持以 `JavaConfig` 的方式配置项目，但感觉目前主流的还是以 `XML + Annotation` 的方式，很少见到使用 JavaConfig 的。可能有点以偏概全了，但在前司和前前司接触到的项目都是以 XML + Annotation 混合的方式配置的，即在 XML 配置文件中开启注解扫描, 业务 bean 的配置注入采用注解( @Serivce, @Autowire 等等)，全局的一些配置(如 MyBatis 的 DataSource,SqlSessionFactory ，web.xml 等等)使用 XML。
虽然 XML + Annotation 的方式比纯用 XML 配置的方式少写了很多 XML，但本质上还是基于 XML 的。
XML 的配置文件比较冗长，不易书写，而且可读性也不高。不知道大家感觉怎么样，反正我看着是挺头疼的╥﹏╥…
### JavaConfig
先简单介绍一下 JavaConfig，JavaConfig即Java Configuration, 即用纯 Java 的方式来配置 Spring
IoC 容器，允许开发者用代码来表示配置逻辑，不再需要 XML。粗略翻了两本关于 Spring Boot的书， JavaConfig 都是最推荐的配置方式。
使用 JavaConfig 的好处，Spring 官方文档中说的很清楚：
- JavaConfig 为依赖注入提供了一个真正面向对象的机制，这意味着开发者可以在配置代码中充分利用 Java 重用、继承和多态性等特性。
- 开发者可以完全控制实例化和依赖注入，即使是最复杂的对象也可以很优雅地处理。
- 因为使用 JavaConfig 只需要 Java，可以很容易的 refactor 代码，而无需再 IDE 之外使用特殊的工具或插件。
JavaConfig 其实很简单，主要是通过 @Configuration 和 @Bean 来进行配置。@Configuration 注解的作用是声明当前类是一个配置类， 就相当于一个 XML 文件。 @Bean 注解声明当前方法返回的是一个 bean。
可能这样说对于用惯了 XML 的人还是比较抽象的，下面看看这些注解与 XML 配置的对应关系。
### JavaConfig 与 XML
1.
**JavaConfig**: @Configuration
**XML**:


```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
      xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
      xmlns:tx="http://www.springframework.org/schema/tx"
      xmlns:util="http://www.springframework.org/schema/util"
      xmlns:p="http://www.springframework.org/schema/p"
      xsi:schemaLocation="http://www.springframework.org/schema/beans
  	   http://www.springframework.org/schema/beans/spring-beans.xsd
      http://www.springframework.org/schema/tx
  	   http://www.springframework.org/schema/tx/spring-tx.xsd
      http://www.springframework.org/schema/util
      http://www.springframework.org/schema/util/spring-util.xsd">	
   
</beans>
```
2.
**JavaConfig**: @Bean
**XML**:


```
<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource">
    <property name="url" value="jdbc:mysql://127.0.0.1:3307/giraffe"/>
    <property name="username" value="ymy"/>
    <property name="password" value="666666"/>
</bean>
```
3.
**JavaConfig**：@ComponentScan
**XML**：


```
<context:component-scan>
```
4.
**JavaConfig**：@Import
**XML**：


```
<import resource="XXX.xml"/>
```
比如要配置一个 dataSource， 在 XML 中通常的做法是这样的：


```
<bean id="drMainDataSource" class="com.alibaba.druid.pool.DruidDataSource">
    <property name="url" value="${mysql.datasource.url}"/>
    <property name="username" value="${mysql.datasource.username}"/>
    <property name="password" value="${mysql.datasource.password}"/>
    <property name="maxActive" value="{mysql.datasource.max-active}"/>
</bean>
```
对应的 JavaConfig 是酱紫的：


```
@Configuration
public class DataAccessConfig extends ConfigurationSupport {
  @Bean(name = "dataSource")
  public DataSource mysqlDataSource(@Value("${mysql.datasource.url}") String url,
                                    @Value("${mysql.datasource.username}") String username,
                                    @Value("${mysql.datasource.password}") String password,
                                    @Value("${mysql.datasource.driverClass}") String driver,
                                    @Value("${mysql.datasource.max-active}") int maxActive) {
    DruidAbstractDataSource dataSource = new DruidDataSource();
    dataSource.setDriverClassName(driver);
    dataSource.setUrl(url);
    dataSource.setUsername(username);
    dataSource.setPassword(password);
    dataSource.setMaxActive(maxActive);
    return dataSource;
  }
}
```
总结
虽然才接触了一周的 JavaConfig，但是相见恨晚啊，个人还是更喜欢 JavaConfig 的配置方式的。JavaConfig 的配置文件可读性更高也更容易学习，记住简单的几个注解即可；借助 IDE 的力量，更不容易出错；而且脑袋再也不用在 Java 和 XML 间来回切换了！！！
![开森](http://oduh99f02.bkt.clouddn.com/WechatIMG1.png)
### 相关文档
- [Spring JavaConfig Document](http://docs.spring.io/spring-javaconfig/docs/1.0.0.M4/reference/html/pr01.html)
- 《Spring Boot 揭秘》
欢迎一起讨论~~~
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/07/0d3d92b9284e4a3394f15a33a96eb89d.jpg)![](http://jbcdn2.b0.upaiyun.com/2016/07/f22e22ed7e5b4a227b39a6ce6da7471f.jpg)
