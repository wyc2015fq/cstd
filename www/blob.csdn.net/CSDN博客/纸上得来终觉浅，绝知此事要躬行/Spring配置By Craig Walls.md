# Spring配置By Craig Walls - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月14日 13:40:11[boonya](https://me.csdn.net/boonya)阅读数：621标签：[java																[spring](https://so.csdn.net/so/search/s.do?q=spring&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)








原文地址：[https://dzone.com/refcardz/spring-configuration](https://dzone.com/refcardz/spring-configuration)


![](https://img-blog.csdn.net/20170214133903853)


本文采用Google翻译，有些地方数据类型和配置被翻译成了中文，建议中英文对照一起看。

第1节

## 关于Spring配置


Spring Framework永远改变了企业Java开发的面貌，使得以松散耦合的方式配置和组合应用程序对象和服务变得更加容易。当你开发你的Spring启用的应用程序，你会发现这个参考卡是Spring上下文配置的一个方便的资源。它编目了Spring 2.5中可用的XML元素，突出了最常用的元素。除了Spring
 XML配置，还将有一个Spring的丰富的注释集，这对于最小化配置Spring所需的XML数量非常有用。





第2节

## 依赖注入在一个坚果



虽然Spring框架做了很多事情，但依赖注入是Spring容器提供的基本功能。

任何非平凡应用程序由协作执行某些业务逻辑的两个或多个对象组成。
传统上，这些对象中的每一个负责获得对其协作的那些对象的引用（其依赖性）。这导致紧耦合和难以测试的代码。

![依赖注入](https://dzone.com/storage/rc-covers/6774-thumb.png)

然而，使用依赖注入，对象被一些外部实体赋予它们的依赖性。
换句话说，依赖注入到需要它们的对象中。在启用Spring的应用程序的情况下，它是Spring容器，将对象注入到依赖它们的对象中。







第3节

## 使用XML配置Spring



从Spring 2.0开始，我们鼓励使用Spring的基于XML Schema的配置，这比基于DTD的传统的XML更灵活。典型的Spring 2.5配置将至少具有以下结构：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd"> <!-- place configuration details here --> </beans>`
在<beans>元素中，您将放置bean声明和配置应用程序上下文的其他元素。
“beans”命名空间是第一个，并且仍然是Spring XML配置中的主命名空间，但它不是单独的。
Spring还带有另外七个命名空间，将在此参考卡中描述。如果你想使用其他命名空间，你需要确保声明它们。例如，如果你想使用“上下文”命名空间，你应该在XML中声明它如下：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-2.5.xsd"> <!-- place configuration details here --> </beans>`






第4节

## Bean命名空间



模式URI
[www.springframework.org/schema/beans](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.springframework.org/schema/beans&usg=ALkJrhisARRw4ObGPt_pVLhj0Z3dbPvuDw)

模式XSD
[www.springframework.org/schema/beans/spring-beans-2.5.xsd](http://www.springframework.org/schema/beans/spring-beans-2.5.xsd)

bean命名空间是Spring的核心命名空间，也是在配置Spring时最常用的命名空间。
根元素是<beans>元素。它通常包含一个或多个<bean>元素，但它可能包括来自其他命名空间的元素，甚至可能根本不包括<bean>元素。

### Spring XML图表键

Spring XML图使用以下符号表示必需的元素，基数和包含：

![Spring XML图表键](https://dzone.com/storage/rc-covers/6775-thumb.png)

### Bean命名空间图

![Bean命名空间图](https://dzone.com/storage/rc-covers/6776-thumb.png)

### Bean命名空间元素
|元件|描述|
|----|----|
|<alias>|为bean定义创建别名。|
|<bean>|在Spring容器中定义一个bean。|
|<constructor-arg>|将值或bean引用注入到bean的构造函数的参数中。通常称为构造函数注入。|
|<description>|用于描述一个Spring上下文或一个单独的bean。尽管被容器忽略，但<description>可以被文档Spring上下文的工具使用。|
|<import>|导入另一个Spring上下文定义。|
|<lookup-method>|通过方法替换启用吸气剂注射。指定将被重写以返回特定bean的方法。通常称为吸气剂注入。|
|<meta>|允许bean的元配置。仅当配置了解释和操作元信息的bean时才有用。|
|<property>|将值或bean引用插入到bean的特定属性中。通常称为setter-injection。|
|<replacement-method>|用新实现替换bean的方法。|

### <bean>元素蒸馏

即使有几个XML元素可以用于配置Spring上下文，您最常使用的一个是<bean>元素。
因此，它只是似乎是正确的，你了解细节的<bean>的属性。
|属性|描述|
|----|----|
|抽象|如果为**true** ，那么bean是抽象的，不会被Spring容器实例化。|
|自动连线|声明如何和如果一个bean应该自动连线。有效值为**byType** ，**byName** ，**constructor** ，**autodetect**或**no** ，无自动装配。|
|自动布线候选|如果为**false** ，那么bean不会成为自动装入另一个bean的候选。|
|类|bean的完全限定类名。|
|依赖检查|确定Spring应该如何强制对bean设置属性。**simple**表示应设置所有基本类型属性;**objects**表示应设置所有复杂类型属性。其他值为**default** ，**none**或**all** 。|
|依赖于取决于|标识在实例化此bean之前应由容器实例化的bean。|
|破坏法|指定当从容器中卸载Bean时应调用的方法。|
|工厂豆|与**factory-method**一起使用时，指定一个提供了工厂方法来创建此bean的bean。|
|工厂方法|将被用来代替构造函数来实例化此bean的方法的名称。|
|ID|这个bean在Spring容器中的身份。|
|init方法|一旦bean被实例化和注入，应该调用的方法的名称。|
|延迟初始化|如果为**true，**那么bean将被延迟实例化。如果为**false** ，那么bean将被热切地实例化。|
|名称|bean的名称。这是**id的**一个较弱的替代品。|
|父母|指定此bean将从其继承其配置的bean。|
|范围|设置bean的范围。默认情况下，所有bean都是**单**例的。其他范围包括**原型** ，**请求**和**会话** 。|

### Bean命名空间示例

下面的Spring XML配置两个bean，一个注入另一个：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd"> <bean id="pirate" class="Pirate"> <constructor-arg value="Long John Silver" /> <property name="map" ref="treasureMap" /> </bean> <bean id="treasureMap" class="TreasureMap" /> </beans>`
第一个bean被赋予“盗版”作为其ID并且是类型“海盗”。
它是通过一个构造函数构造的，它接受一个String作为参数，在这种情况下，它将构造与“长约翰银”作为该值。
此外，它的“map”属性与对“treasureMap”bean的引用有关，该对象被定义为TreasureMap的一个实例。


![热提示](https://dzone.com/storage/rc-covers/6777-thumb.png)

**不要将所有的bean放在一个XML文件中。**

一旦你的应用程序超越了平凡的阶段，你的Spring配置中可能会有大量的XML。
没有理由将所有配置放在一个XML文件中。通过将它分割到多个XML文件，使您的Spring配置更易于管理。然后在创建应用程序上下文或使用<import>元素时将它们组合在一起：
`<import resource="service-layer-config.xml" /> <import resource="data-layer-config.xml" /> <import resource="transaction-config.xml" />`







第5节

## 上下文命名空间



模式URI
[www.springframework.org/schema/context](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.springframework.org/schema/context&usg=ALkJrhhNQ5d83IZSBs0THtSLfluVDpuqEQ)

模式XSD
[www.springframework.org/schema/context/spring-context-2.5.xsd](http://www.springframework.org/schema/context/spring-context-2.5.xsd)

在Spring 2.5中添加了上下文命名空间，以提供几种特定于应用程序上下文的配置。
它包括对基于注释的配置，JMX和域对象注入的支持。

![上下文命名空间图](https://dzone.com/storage/rc-covers/6778-thumb.png)

### 上下文命名空间元素
|<context：annotation-config>|在Spring bean中启用基于注释的配置。如果正在使用<context：component-scan>元素，则不需要此元素。|
|----|----|
|<context：component-scan>|扫描包的bean以自动注册到Spring容器中。使用这个元素意味着与<context：annotation-config>相同的功能。|
|<context：exclude-filter>|用于排除某些类被component-scan自动注册。|
|<context：include-filter>|用于在组件扫描自动注册bean时指定要包括哪些类。|
|<context：load-time-weaver>|注册AspectJ加载时编织程序。|
|<context：mbean-export>|将bean导出为JMX MBeans。|
|<context：mbean-server>|使用Spring上下文启动MBean服务器。|
|<context：property-placeholder>|通过属性文件启用外部配置。|
|<context：spring-configured>|启用对未由Spring实例化的对象的注入。|

### 上下文命名空间示例

以下Spring配置使用<context：component-scan>自动注册来自“com.springinaction.service”命名空间的某些bean：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-2.5.xsd"> <context:component-scan base-package="com.springinaction. service" /> </beans>`
如上配置，<context：component-scan>将扫描“com.springinaction.service”包，并将自动注册为bean，发现使用@Component，@Controller，@Repository，@Service注释的所有类，或@Aspect。


![热提示](https://dzone.com/storage/rc-covers/6779-thumb.png)

**为最终用户外部化配置**

并非所有配置都必须在Spring中完成。
您不会期望应用程序的管理员或最终用户在Spring XML中进行挖掘以调整数据库或其他部署特定的详细信息。
相反，使用<context：property-placeholder>外部化配置：
`<context:property-placeholder location="file:////etc/pirate.properties"`
然后可以使用/etc/pirate.properties中的名称/值对在Spring上下文中填充占位符值。例如：
`<bean id="pirate" class="Pirate"> <constructor-arg value="${pirate.name}" /> <bean>`







第6节

## AOP命名空间



模式URI
[www.springframework.org/schema/aop](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.springframework.org/schema/aop&usg=ALkJrhjFjzTvRuV4vDrdhd8S2AGLde3U5Q)

模式XSD
[www.springframework.org/schema/aop/spring-aop-2.5.xsd](http://www.springframework.org/schema/aop/spring-aop-2.5.xsd)

aop命名空间使得可以在Spring上下文中声明方面，切入点和通知。
它还使用@AspectJ注释提供对基于注释的方面的支持。使用方面，您可以定义跨应用程序的许多点应用（或“编织”）的功能。

### AOP命名空间图

![AOP命名空间图](https://dzone.com/storage/rc-covers/6780-thumb.png)

### AOP命名空间元素
|元件|描述|
|----|----|
|<aop：advisor>|声明一个Spring AOP顾问。|
|<aop：after>|在通知之后声明（例如，在切入点之后调用的方法）。|
|<aop：after-returning>|声明后返回通知（例如，在切入点成功返回后调用的方法）。|
|<aop：after-throwing>|声明抛出后的建议（例如，在从切入点抛出异常后调用的方法）。|
|<aop：around>|声明建议（例如，其功能包含切入点的方法）。|
|<aop：aspect>|定义一个方面，包括一个或多个切入点和一个或多个建议。|
|<aop：aspectj-autoproxy>|启用使用@AspectJ注释声明方面。|
|<aop：before>之前|在通知之前声明（例如，在切入点执行之前调用的方法）。|
|<aop：config>|AOP命名空间中大多数元素的父元素。|
|<aop：declare-parents>|定义AOP介绍（有效的混合）。|
|<aop：include>|可选地与aspectj-autoproxy一起使用以指定要为其创建代理的@ AspectJ注释的bean。|
|<aop：pointcut>|声明切入点（例如，应用建议的机会）。|
|<aop：scoped-proxy>|指定使用复杂范围（例如“request”和“session”）声明的bean的代理。|

### AOP命名空间示例

以下Spring配置使用来自aop命名空间的元素创建一个方面：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:aop="http://www.springframework.org/schema/aop" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd"> <bean id="pirateTalker" class="PirateTalker" /> <aop:config> <aop:pointcut id="plunderPointcut" expression="execution(* *.plunder(..))" /> <aop:aspect ref="pirateTalker"> <aop:before pointcut-ref="plunderPointcut" method="sayAvastMeHearties" /> <aop:after-returning pointcut-ref="plunderPointcut" method="sayYarr" /> </aop:aspect> </aop:config> </beans>`
该方面由一个切入点和两个通知定义组成。
切入点被定义为对任何对象执行plunder（）方法。<aop：before> advice配置为在执行plunder（）方法时调用“pirateTalker”bean上的sayAvastMeHearties（）方法。同样，在任何对象上执行plunder（）方法时，将调用sayYarr（）方法。


![热提示](https://dzone.com/storage/rc-covers/6781-thumb.png)

**通过使用@AspectJ注释减少AOP相关的XML**

“aop”命名空间中的元素使得将相对简单的旧Java对象转换为多个方面变得相当容易。
但是，<aop：aspectj-autoproxy>元素可以单独消除几乎所有其他“aop”命名空间XML的需要。
通过在Spring配置中放置<aop：aspectj-autoproxy>，您可以使用@AspectJ注释（如@Aspect，@Pointcut，@Before和@After）将切点和建议声明移动到Java代码中。有关详细信息，请参阅Spring in Action第二版的第4章第4.3.2节。








第7节

## JEE命名空间



模式URI
[www.springframework.org/schema/jee](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.springframework.org/schema/jee&usg=ALkJrhj2Uon3k8lds3V5qVUl7MVZLXQG3g)

模式XSD
[www.springframework.org/schema/jee/spring-jee-2.5.xsd](http://www.springframework.org/schema/jee/spring-jee-2.5.xsd)

JEE命名空间提供用于从JNDI查找对象以及将EJB的引用连接到Spring上下文的配置元素。

### JEE命名空间图

![JEE命名空间图](https://dzone.com/storage/rc-covers/6782-thumb.png)

### JEE命名空间元素
|元件|描述|
|----|----|
|<jee：jndi-environment>|定义JNDI查找的环境设置。|
|<jee：jndi-lookup>|声明对要从JNDI检索的对象的引用。|
|<jee：local-slsb>|声明对本地无状态会话EJB的引用。|
|<jee：remote-slsb>|声明对远程无状态会话EJB的引用。|

### JEE命名空间示例

以下Spring配置使用几个jee命名空间的元素来从Spring外部检索对象，并将它们配置为Spring bean：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:jee="http://www.springframework.org/schema/jee" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd http://www.springframework.org/schema/jee http://www.springframework.org/schema/jee/spring-jee-2.5.xsd"> <jee:remote-slsb id="hispaniola" jndi-name="ejb/PirateShip" business-interface="com.pirate.PirateShipEjb" resource-ref="true" /> <jee:jndi-lookup id="parrot" jndi-name="pirate/Parrot" resource-ref="false" /> </beans>`
第一个元素<jee：remote-slsb>配置一个名为“Hispaniola”的Bean，它实际上是对EJB 2远程无状态会话bean的引用。EJB的home接口在JNDI中以名称“java：comp / env / ejb / PirateShip”找到。resource-ref属性指示jndi-name中的值应以“java：comp / env / 。EJB实现在PirateShipEjb业务接口中定义的方法。

另一个元素<jee：jndi-lookup>从JNDI检索对象的引用（它可以是EJB 3会话bean或纯Java对象）。该对象在JNDI中找到，名称为“pirate / Parrot”。因为resource-ref是“false”，所以jndi-name不用“java：comp / env /”作为前缀。







第8节

## JMS命名空间



模式URI
[www.springframework.org/schema/jms](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.springframework.org/schema/jms&usg=ALkJrhgm1ica68d8gZaW7P_7wZssTpV5gw)

模式XSD
[www.springframework.org/schema/jms/spring-jms-2.5.xsd](http://www.springframework.org/schema/jms/spring-jms-2.5.xsd)

JMS命名空间提供用于配置消息驱动的POJO，响应到达JMS目标（主题或队列）的消息的bean的元素。

### JMS命名空间图

![JMS命名空间图](https://dzone.com/storage/rc-covers/6783-thumb.png)

### JMS命名空间元素
|元件|描述|
|----|----|
|<jms：jca-listener-container>|为基于JCA的JMS目标侦听器配置容器。|
|<jms：listener-container>|为标准JMS目标侦听器配置容器。|
|<jms：listener>|向JMS目标声明侦听器。用于创建消息驱动的POJO。|

### JMS命名空间示例

以下Spring配置设置了一个消息驱动的POJO，以响应到达队列的消息。
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:jms="http://www.springframework.org/schema/jms" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd http://www.springframework.org/schema/jms http://www.springframework.org/schema/jms/spring-jms-2.5.xsd"> ... <bean id="connectionFactory" class="org.apache.activemq.ActiveMQConnectionFactory"> <property name="brokerURL" value="tcp://localhost:61616" /> </bean> <bean id="messageHandlerService" class="com.pirate.MessageHandlerImpl" /> <jms:listener-container connection-factory="connectionFactory"> <jms:listener destination="queue.bottle" ref="messageHandlerService" method="readMessageFromBottle" /> </jms:listener-container> </beans>`
<jms：listener-container>配置一个容器，用于处理到达JMS连接工厂中的主题或队列的消息。在此元素中，您可以声明一个或多个<jms：listener>元素来响应特定主题。在这种情况下，单个<jms：listener>对到达“queue.bottle”主题的消息作出反应，在“messageHandlerService”bean到达时调用readMessageFromBottle（）方法。







第9节

## Lang命名空间



模式URI
[www.springframework.org/schema/lang](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.springframework.org/schema/lang&usg=ALkJrhie8zEVcn-Y_KVhKOZySGohLMge0A)

模式XSD
[www.springframework.org/schema/lang/spring-lang-2.5.xsd](http://www.springframework.org/schema/lang/spring-lang-2.5.xsd)

“lang”命名空间使您能够将脚本对象链接到Spring。
这些对象可以用Groovy，JRuby或BeanShell编写。

### Lang命名空间图

![Lang命名空间图](https://dzone.com/storage/rc-covers/6784-thumb.png)

### Lang命名空间元素
|元件|描述|
|----|----|
|<lang：bsh>|配置BeanShell定义的bean。|
|<lang：defaults>|配置默认值应用于所有脚本化的bean。|
|<lang：groovy>|声明一个实现为Groovy脚本的bean。|
|<lang：inline-script>|在Spring XML中直接嵌入脚本bean的代码。|
|<lang：jruby>|声明一个实现为JRuby脚本的bean。|
|<lang：property>|用于向脚本化的bean中插入值或引用。|

### Lang命名空间示例

在这个Spring上下文中，使用<lang：groovy>和<lang：jruby>定义的脚本bean注入一个Pirate bean：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:lang="http://www.springframework.org/schema/lang" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd http://www.springframework.org/schema/lang http://www.springframework.org/schema/lang/spring-lang-2.5.xsd"> <bean id="jackSparrow" class="Pirate"> <constructor-arg value="Jack Sparrow" /> <property name="compass" ref="compass" /> <property name="hat" ref="hat" /> </bean> <lang:groovy id="compass" script-source="classpath:Compass.groovy" refresh-check-delay="10000" /> <lang:jruby id="hat" script-source="classpath:PirateHat.rb" script-interface="PirateHat" refresh-check-delay="60000" /> </beans>`
<lang：groovy>元素创建一个bean，它实现为一个名为Compass.groovy的Groovy脚本，并且位于类路径的根目录中。refresh-check-delay属性指示应该每10秒检查一次脚本，并在脚本更改时重新装入。

<lang：jruby>元素创建一个实现为Ruby（JRuby，特别是）脚本PirateHat.rb的bean。它实现一个PirateHat接口，并每分钟检查更新一次。







第10节

## TX命名空间



模式URI
[www.springframework.org/schema/tx](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.springframework.org/schema/tx&usg=ALkJrhjslrXjIttXVEdmpWqWxJNgB_FlDA)

模式XSD
[www.springframework.org/schema/tx/spring-tx-2.5.xsd](http://www.springframework.org/schema/tx/spring-tx-2.5.xsd)

“tx”命名空间支持在Spring中声明的bean的声明性事务。

### TX命名空间图

![TX命名空间图](https://dzone.com/storage/rc-covers/6785-thumb.png)

### TX命名空间元素
|元件|描述|
|----|----|
|<tx：advice>|声明事务建议。|
|<tx：annotation-driven>|将Spring配置为对事务规则使用@Transactional注释。|
|<tx：attributes>|声明一个或多个方法的事务规则。|
|<tx：jta-transaction-manager>|配置JTA事务管理器，自动检测WebLogic，WebSphere或OC4J。|
|<tx：method>|描述给定方法签名的事务规则。|

### TX命名空间示例

以下Spring配置使用tx命名空间中的元素来配置事务规则和边界：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:tx="http://www.springframework.org/schema/tx" xmlns:aop="http://www.springframework.org/schema/aop" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd"> <tx:jta-transaction-manager /> <tx:advice id="txAdvice"> <tx:attributes> <tx:method name="plunder*" propagation="REQUIRED" /> <tx:method name="*" propagation="SUPPORTS" /> </tx:attributes> </tx:advice> <aop:config> <aop:advisor pointcut="execution(* ..Pirate.*(..))" advice-ref="txAdvice" /< </aop:config> </beans>`
在Spring 2.5中添加了<tx：jta-transaction-manager>，以自动检测WebLogic，WebSphere或OC4J提供的JTA事务管理器。它将事务管理器暴露为Spring上下文中的一个bean，名称为“transactionManager”。

接下来，<tx：advice>设置声明事务规则的AOP通知。
在这种情况下，任何以“plunder”开头的方法都需要事务。所有其他方法都支持事务，但不需要它们。最后，本示例从aop命名空间中借用以配置使用事务建议的AOP顾问程序。这里的切入点是对于Pirate类中的所有方法。


![热提示](https://dzone.com/storage/rc-covers/6786-thumb.png)

**在Java中配置事务规则**

如果你“寻找在Spring配置中减少XML的数量的方法，考虑使用<tx：annotation-driven>元素。一旦这个元素到位，你可以开始用@注释你的bean和他们的方法@事务性以定义事务边界和规则。请查看*Spring in Action第二版的*第6章第6.4.4节了解更多。








第11节

## Util命名空间



模式URI
[www.springframework.org/schema/util](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.springframework.org/schema/util&usg=ALkJrhiRVLO7JntAtDbnLUL4tDGDgYrKBg)

模式XSD
[www.springframework.org/schema/util/spring-util-2.5.xsd](http://www.springframework.org/schema/util/spring-util-2.5.xsd)

实用程序命名空间提供了可以在Spring中将集合和其他非bean对象连接起来的元素，就像它们是任何其他bean一样。

### Util命名空间图

![Util命名空间图](https://dzone.com/storage/rc-covers/6787-thumb.png)

### Util命名空间元素
|元件|描述|
|----|----|
|<util：constant>|引用类型上的静态字段，并将其值作为bean公开。|
|<util：list>|声明一个值或引用的列表作为bean。|
|<util：map>|将地图声明为bean。|
|<util：properties>|从属性文件加载java.util.Properties并将其作为bean公开。|
|<util：set>|将集合声明为bean。|
|<util：property-path>|引用bean属性（或嵌套属性），并将该属性作为bean本身公开。|

### Util命名空间示例

下面的Spring配置使用了“util”命名空间中的几个元素：
`<?xml version="1.0" encoding="UTF-8"?> <beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:util="http://www.springframework.org/schema/util" xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util-2.5.xsd"> <util:list id="piratePhrases"> <value>Yo ho ho</value> <value>Yarr</value> <value>Avast me hearties!</value> <value>Blow me down</value> </util:list> <util:constant id="pirateCode" static-field="Pirate.PIRATE_CODE" /> <util:property-path id="doubloonCount" path="pirate.treasure.doubloonCount" /> </beans>`
这里使用<util：list>元素来创建包含海盗说出的各种短语的字符串列表。
<util：constant>元素创建对Pirate类的名为PIRATE_CODE的常量（public static field）的引用。
最后，<util：property-path>深入到“pirate”bean中，检索名为“pirate”的bean的treasure属性的doubloonCount属性的值。在所有三种情况下，结果值都暴露为bean在Spring上下文中，适合注入其他bean。







第12节

## 春天注释



从历史上看，Spring配置主要涉及XML。
但是，随着Spring逐渐接受注释驱动配置，这种情况正在改变。从Spring 2.5开始，Spring提供了36个注释，更不用说由第三方库和各种Spring附加组件提供的注释。

### 上下文配置注释

Spring使用这些注释来指导bean的创建和注入。
|注解|使用|描述|
|----|----|----|
|@Autowired|构造函数，字段，方法|声明构造函数，字段，setter方法或按类型自动连接的配置方法。用@Autowired注释的项目不必是公开的。|
|@Configurable|类型|与<context：spring-configured>一起使用，以声明其属性应该被注入的类型，即使它们没有被Spring实例化。通常用于注入域对象的属性。|
|@订购|类型，方法，字段|定义排序，作为实现org.springframework.core.Ordered接口的替代方法。|
|@Qualifier|字段，参数，类型，注释类型|指南自动接线通过除类型以外的方法执行。|
|@需要|方法（setters）|指定必须注入特定属性，否则配置将失败。|
|@范围|类型|指定bean的范围，包括单例，原型，请求，会话或某些自定义作用域。|

### 事务注释

@Transactional注释与<tx：annotation-driven>元素一起使用，以在Java中将事务边界和规则声明为类和方法元数据。
|注解|使用|描述|
|----|----|----|
|@Transactional|方法，类型|在bean和/或其方法上声明事务边界和规则。|

### 定型注释

这些注释用于构造关于它们所属的应用程序层的类。
使用这些注释之一注释的类将在Spring应用程序上下文中自动注册，如果<context：component-scan>在Spring XML配置中。

此外，如果在Spring中配置了PersistenceExceptionTranslationPostProcessor，那么使用@Repository注释的任何bean都会将SQLExceptions从其方法中抛出，并将其转换为Spring未检查的DataAccessExceptions之一。
|注解|使用|描述|
|----|----|----|
|@零件|类型|任何Spring管理的组件的通用构造型注释。|
|@Controller|类型|将组件定型为Spring MVC控制器。|
|@Repository|类型|将组件定型为存储库。还指示从组件的方法抛出的SQLExcepts应该被转换为Spring DataAccessExceptions。|
|@服务|类型|将组件定型为服务。|

### Spring MVC注释

这些注释在Spring 2.5中引入，使得更容易使用最小的XML配置创建Spring MVC应用程序，并且不会扩展Controller接口的许多实现之一。
|注解|使用|描述|
|----|----|----|
|@Controller|类型|将组件定型为Spring MVC控制器。|
|@InitBinder|方法|注释自定义数据绑定的方法。|
|@ModelAttribute|参数，方法|应用于方法时，用于使用从方法返回的值预先装入模型。应用于参数时，将模型属性绑定到参数。|
|@RequestMapping|方法，类型|将URL模式和/或HTTP方法映射到方法或控制器类型。|
|@RequestParam|参数|将请求参数绑定到方法参数。|
|@SessionAttributes|类型|指定模型属性应存储在会话中。|

### JMX注释

这些注释与<context：mbean-export>元素一起使用，将Bean方法和属性声明为MBean操作和属性。
|注解|使用|描述|
|----|----|----|
|@ManagedAttribute|方法|用于setter或getter方法，以指示bean的属性应显示为MBean属性。|
|@ManagedNotification|类型|表示bean发出的JMX通知。|
|@ManagedNotifications|类型|表示bean发出的JMX通知。|
|@ManagedOperation||指定方法应显示为MBean操作。|
|@ManagedOperation参数||用于提供操作参数的描述。|
|@ManagedOperation参数||提供一个或多个操作参数的说明。|
|@ManagedResource|类型|指定类的所有实例应显示为MBeans。|

### 测试注释

这些注释对于创建依赖于Spring bean和/或需要事务上下文的JUnit 4样式的单元测试很有用。
|注解|使用|描述|
|----|----|----|
|@AfterTransaction|方法|用于标识在事务完成后要调用的方法。|
|@BeforeTransaction|方法|用于标识在事务开始之前调用的方法。|
|@ContextConfiguration|类型|为测试配置Spring应用程序上下文。|
|@DirtiesContext|方法|表示方法弄脏了Spring容器，因此必须在测试完成后重新构建。|
|@ExpectedException|方法|表示测试方法应该抛出特定的异常。如果未抛出异常，测试将失败。|
|@IfProfileValue|类型，方法|表示为特定配置文件配置启用了测试类或方法。|
|@NotTransactional|方法|指示测试方法不得在事务上下文中执行。|
|@ProfileValueSource配置|类型|标识配置文件值源的实现。缺少此注释将导致从系统属性加载配置文件值。|
|@重复|方法|表示测试方法必须重复特定次数。|
|@Rollback|方法|指定是否应回滚带注释的方法的事务。|
|@TestExecution侦听器|类型|标识测试类的零个或多个测试执行侦听器。|
|@定时|方法|指定测试方法的时间限制。如果测试未在时间到期之前完成，测试将失败。|
|@Transaction配置|类型|为事务配置测试类，为测试类中的所有测试方法指定事务管理器和/或默认回滚规则。|










