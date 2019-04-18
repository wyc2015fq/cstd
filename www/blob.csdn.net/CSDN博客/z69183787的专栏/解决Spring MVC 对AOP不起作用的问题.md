# 解决Spring MVC 对AOP不起作用的问题 - z69183787的专栏 - CSDN博客
2014年07月15日 16:35:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4213
用的是 SSM3的框架 Spring MVC 3.1 + Spring 3.1 + Mybatis3.1
**第一种情况**：
Spring MVC 和 Spring 整合的时候，SpringMVC的springmvc.xml文件中 配置扫描包，不要包含 service的注解，Spring的applicationContext.xml文件中
 配置扫描包时，不要包含controller的注解，如下所示：
SpringMVC的xml配置：
<context:component-scan base-package="com.insigma">
<context:exclude-filter
 type="annotation" expression="org.springframework.stereotype.Service"/>
</context:component-scan>
Spring MVC启动时的配置文件，包含组件扫描、url映射以及设置freemarker参数，让spring不扫描带有@Service注解的类。为什么要这样设置？因为springmvc.xml与applicationContext.xml不是同时加载，如果不进行这样的设置，那么，spring就会将所有带@Service注解的类都扫描到容器中，等到加载applicationContext.xml的时候，会因为容器已经存在Service类，使得cglib将不对Service进行代理，直接导致的结果就是在applicationContext 中的事务配置不起作用，发生异常时，无法对数据进行回滚。以上就是原因所在。
同样的在Spring的xml配置如下：
<context:component-scan
 base-package="com.insigma">
 <context:exclude-filter
 type="annotation" expression="org.springframework.stereotype.Controller"/>
</context:component-scan>
扫描包路径，不扫描带有@Controller注解的类。因为这些类已经随容器启动时，在springmvc.xml中扫描过一遍了。
完成以上工作
注意以上几点就OK了。
