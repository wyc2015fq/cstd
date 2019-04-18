# Spring获取bean方式(BeanFactory和ApplicationContext) - z69183787的专栏 - CSDN博客
2012年11月25日 13:04:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9415
Bean工厂（com.springframework.beans.factory.BeanFactory）是Spring框架最核心的接口，它提供了高级IoC的配置机制。BeanFactory使管理不同类型的Java对象成为可能，应用上下文（com.springframework.context.ApplicationContext）建立在BeanFactory基础之上，提供了更多面向应用的功能，它提供了国际化支持和框架事件体系，更易于创建实际应用。我们一般称BeanFactory为IoC容器，而称ApplicationContext为应用上下文。但有时为了行文方便，我们也将ApplicationContext称为Spring容器。
对于两者的用途，我们可以进行简单划分：BeanFactory是Spring框架的基础设施，面向Spring本身；ApplicationContext面向使用Spring框架的开发a者，几乎所有的应用场合我们都直接使用ApplicationContext而非底层的BeanFactory。
##  BeanFactory介绍
Spring为BeanFactory提供了多种实现，最常用的是XmlBeanFactory。 BeanFactory接口位于类结构树的顶端，它最主要的方法就是getBean(String beanName)，该方法从容器中返回特定名称的Bean，BeanFactory的功能通过其他的接口得到不断扩展。
![](http://www.coder.xxx/wp-content/uploads/2012/10/a.jpg)
代码示例：
beans.xml：Car的配置文件
|`01`|`<?``xml``version``=``"1.0"``encoding``=``"UTF-8"``?>`|
|`02`|`<``beans``xmlns``=``"[http://www.springframework.org/schema/beans](http://www.springframework.org/schema/beans)"`|
|`03`|`    ``xmlns:xsi``=``"[http://www.w3.org/2001/XMLSchema-instance](http://www.w3.org/2001/XMLSchema-instance)"`|
|`04`|`    ``xmlns:p``=``"[http://www.springframework.org/schema/p](http://www.springframework.org/schema/p)"`|
|`05`|`    ``xsi:schemaLocation="[http://www.springframework.org/schema/beans](http://www.springframework.org/schema/beans)`|
|`06`|`        ``[http://www.springframework.org/schema/beans/spring-beans-3.0.xsd](http://www.springframework.org/schema/beans/spring-beans-3.0.xsd)">`|
|`07`|`            ``<``bean``id``=``"car1"``class``=``"com.baobaotao.Car"`|
|`08`|`                ``p:brand``=``"红旗CA72"`|
|`09`|`                ``p:color``=``"黑色"`|
|`10`|`                ``p:maxSpeed``=``"200"``/>`|
|`11`|`</``beans``>`|
java调用代码：
|`1`|```ResourcePatternResolver resolver =````new``PathMatchingResourcePatternResolver();`|
|`2`|```Resource res = resolver.getResource(````"classpath:com/coder/beanfactory/beans.xml"``);`|
|`3`|```BeanFactory bf =````new``XmlBeanFactory(res);`|
|`4`|```Car car = bf.getBean(````"car"``,Car.``class``);`|
|`5`|`System.out.println(````"car bean is ready for use!"````);`|
XmlBeanFactory通过Resource装载Spring配置信息并启动IoC容器，然后就可以通过BeanFactory#getBean(beanName)方法从IoC容器中获取Bean了。通过BeanFactory启动IoC容器时，并不会初始化配置文件中定义的Bean，初始化动作发生在第一个调用时。对于单实例（singleton）的Bean来说，BeanFactory会缓存Bean实例，所以第二次使用getBean()获取Bean时将直接从IoC容器的缓存中获取Bean实例。
Spring在DefaultSingletonBeanRegistry类中提供了一个用于缓存单实例Bean的缓存器，它是一个用HashMap实现的缓存器，单实例的Bean以beanName为键保存在这个HashMap中。
## ApplicationContext介绍
ApplicationContext的主要实现类是ClassPathXmlApplicationContext和FileSystemXmlApplicationContext，前者默认从类路径加载配置文件，后者默认从文件系统中装载配置文件，我们来了解一下ApplicationContext的类继承体系:
![](http://www.coder.xxx/wp-content/uploads/2012/10/b.jpg)
和BeanFactory初始化相似，ApplicationContext的初始化也很简单:
◆如果配置文件放置在类路径下，用户可以优先使用ClassPathXmlApplicationContext实现类：
|`1`|```ApplicationContext ctx =````new``ClassPathXmlApplicationContext(``"com/coder/context/beans.xml"``);`|
对于ClassPathXmlApplicationContext来说，“com/coder/context/beans.xml”等同于“classpath:com/coder/context/beans.xml”。
◆如果配置文件放置在文件系统的路径下，则可以优先考虑使用FilySystemXmlApplicationContext实现类：
|`1`|```ApplicationContext ctx =````new``FileSystemXmlApplicationContext(``"com/coder/context/beans.xml"``);`|
对于FileSystemXmlApplicationContext来说，“com/coder/context/beans.xml”等同于“file:com/coder/context/beans.xml”。
◆还可以指定一组配置文件，Spring会自动将多个配置文件在内存中"整合"成一个配置文件，如下所示：
|`1`|```ApplicationContext ctx =````new``ClassPathXmlApplicationContext(``new``String[]{``"conf/beans1.xml"``,``"conf/beans2.xml"``});`|
在获取ApplicationContext实例后，就可以像BeanFactory一样调用getBean(beanName)返回Bean了。ApplicationContext的初始化和BeanFactory有一个重大的区别：BeanFactory在初始化容器时，并未实例化Bean，直到第一次访问某个Bean时才实例目标Bean；而ApplicationContext则在初始化应用上下文时就实例化所有单实例的Bean。因此ApplicationContext的初始化时间会比BeanFactory稍长一些，不过稍后的调用则没有"第一次惩罚"的问题。
Spring 3.0支持基于类注解的配置方式，和基于XML文件配置方式的优势在于，类注解的配置方式可以很容易地让开发者控制Bean的初始化过程，比基于XML的配置更加灵活。 Spring为基于注解类的配置提供了专门的ApplicationContext实现类：AnnotationConfigApplicationContext。来看一个如何使用AnnotationConfigApplicationContext启动Spring容器的示例：
带注解的Java类提供的配置信息:
|`01`|`//①表示是一个配置信息提供类`|
|`02`|`@Configuration``public``class````Beans {```|
|`03`|`    ``//②定义一个Bean`|
|`04`|`    ``@Bean````(name =````"car"``)`|
|`05`|`    ``public````Car buildCar() {```|
|`06`|`        ````Car car =````new``Car();`|
|`07`|`        ``car.setBrand(``"红旗CA72"``);`|
|`08`|`        ``car.setMaxSpeed(``200``);`|
|`09`|`        ``return``car;`|
|`10`|`    ``}`|
|`11`|`}`|
通过带@Configuration的配置类启动容器:
|`1`|`//①通过一个带@Configuration的POJO装载Bean配置`|
|`2`|```ApplicationContext ctx =````new``AnnotationConfigApplicationContext(Beans.``class``);`|
|`3`|```Car car = ctx.getBean(````"car"``,Car.``class``);`|
##  WebApplicationContext介绍
WebApplicationContext是专门为Web应用准备的，它允许从相对于Web根目录的路径中装载配置文件完成初始化工作。
![](http://www.coder.xxx/wp-content/uploads/2012/10/c.jpg)
从WebApplicationContext中可以获得ServletContext的引用，整个Web应用上下文对象将作为属性放置到ServletContext中，以便Web应用环境可以访问Spring应用上下文。 可以通过Spring提供的工具类WebApplicationContextUtils，获取WebApplicationContext实例:
|`1`|```WebApplicationContext ctx = WebApplicationContextUtils.getRequiredWebApplicationContext(ServletContext sc);```|
|`2`|```WebApplicationContext ctx = WebApplicationContextUtils.getWebApplicationContext(ServletContext sc);```|
上面两个工具方式的区别是，前者在获取失败时抛出异常，后者返回null。
由于Web应用比一般的应用拥有更多的特性，因此WebApplicationContext扩展了ApplicationContext。WebApplicationContext定义了一个常量ROOT_WEB_APPLICATION_ CONTEXT_ATTRIBUTE，在上下文启动时，WebApplicationContext实例即以此为键放置在ServletContext的属性列表中，因此我们可以直接通过以下语句从Web容器中获取WebApplicationContext：
|`1`|```WebApplicationContext wac = (WebApplicationContext)servletContext.getAttribute(WebApplicationContext.ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE);```|
** WebApplicationContext初始化**
WebApplicationContext的初始化方式和BeanFactory、ApplicationContext有所区别，因为WebApplicationContext需要ServletContext实例，也就是说它必须在拥有Web容器的前提下才能完成启动的工作。
Spring分别提供了用于启动WebApplicationContext的Servlet和Web容器监听器：
◆org.springframework.web.context.ContextLoaderServlet；
◆org.springframework.web.context.ContextLoaderListener。
两者的内部都实现了启动WebApplicationContext实例的逻辑，我们只要根据Web容器的具体情况选择两者之一，并在web.xml中完成配置就可以了:
通过Web容器监听器引导:
|`01`|`…`|
|`02`|`<!--①指定配置文件-->`|
|`03`|`<``context-param``>`|
|`04`|`    ``<``param-name``>contextConfigLocation</``param-name``>`|
|`05`|`    ``<``param-value``>/WEB-INF/coder-dao.xml,/WEB-INF/coder-service.xml</``param-value``>`|
|`06`|`</``context-param``>`|
|`07`|`…`|
|`08`|`<!--②声明Web容器监听器-->`|
|`09`|`<``listener``>`|
|`10`|`    ``<``listener-class````>org.springframework.web.context.ContextLoaderListener </````listener-class``>`|
|`11`|`</``listener``>`|
通过自启动的Servlet引导:
|`01`|`…`|
|`02`|`<``context-param``>`|
|`03`|`    ``<``param-name``>contextConfigLocation</``param-name``>`|
|`04`|`    ``<``param-value``>/WEB-INF/coder-dao.xml,/WEB-INF/coder-service.xml</``param-value``>`|
|`05`|`</``context-param``>`|
|`06`|`…`|
|`07`|```<!--①声明自动启动的Servlet -->```|
|`08`|`<``servlet``>`|
|`09`|`    ``<``servlet-name``>springContextLoaderServlet</``servlet-name``>`|
|`10`|`    ``<``servlet-class``>org.springframework.web.context.ContextLoaderServlet</``servlet-class``>`|
|`11`|`    ``<!--②启动顺序-->`|
|`12`|`    ``<``load-on-startup``>1</``load-on-startup``>`|
|`13`|`</``servlet``>`|
如果使用标注@Configuration的Java类提供配置信息，则web.xml的配置需要按以下方式配置：
使用@Configuration的Java类提供配置信息的配置:
[view
 source](http://www.coder.xxx/archives/82.html#viewSource)
|`01`|`<``web-app``>`|
|`02`|`    ``<!--通过指定context参数，让Spring使用AnnotationConfigWebApplicationContext而非XmlWebApplicationContext启动容器-->`|
|`03`|`    ``<``context-param``>`|
|`04`|`        ``<``param-name``>contextClass</``param-name``>`|
|`05`|`        ``<``param-value``>org.springframework.web.context.support.AnnotationConfigWebApplicationContext</``param-value``>`|
|`06`|`    ``</``context-param``>`|
|`07`|`    ````<!-- 指定标注了@Configuration的配置类，多个可以使用逗号或空格分隔-->```|
|`08`|`    ``<``context-param``>`|
|`09`|`        ``<``param-name``>contextConfigLocation</``param-name``>`|
|`10`|`        ``<``param-value``>com.coder.AppConfig1,com.coder.AppConfig1</``param-value``>`|
|`11`|`    ``</``context-param``>`|
|`12`|`    ````<!-- ContextLoaderListener监听器将根据上面配置使用 AnnotationConfigWebApplicationContext根据contextConfigLocation指定的配置类启动Spring容器-->```|
|`13`|`    ``<``listener``>`|
|`14`|`        ``<``listener-class``>org.springframework.web.context.ContextLoaderListener</``listener-class``>`|
|`15`|`    ``</``listener``>`|
|`16`|`</``web-app``>`|
