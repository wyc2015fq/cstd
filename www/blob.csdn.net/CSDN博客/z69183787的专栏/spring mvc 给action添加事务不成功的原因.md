# spring mvc 给action添加事务不成功的原因 - z69183787的专栏 - CSDN博客
2014年07月15日 16:52:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3590

Java代码  ![收藏代码](http://uule.iteye.com/images/icon_star.png)
- <context-param>  
-         <param-name>contextConfigLocation</param-name>  
-         <param-value>classpath*:META-INF/spring/applicationContext*.xml</param-value>  
-     </context-param>  
- 
- <!-- Creates the Spring Container shared by all Servlets and Filters -->  
-     <listener>  
-         <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>  
-     </listener>  
- 
- 
- <!-- Handles Spring requests -->  
-     <servlet>  
-         <servlet-name>servlet</servlet-name>  
-         <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
-         <init-param>  
-             <param-name>contextConfigLocation</param-name>  
-             <param-value>/WEB-INF/spring/webmvc-config.xml</param-value>  
-         </init-param>  
-         <load-on-startup>1</load-on-startup>  
-     </servlet>  
- 
-     <servlet-mapping>  
-         <servlet-name>servlet</servlet-name>  
-         <url-pattern>/</url-pattern>  
-     </servlet-mapping>  
- 
- 
1、在主容器中（applicationContext.xml），将Controller的注解排除掉 
<context:component-scan base-package="com"> 
  <context:exclude-filter type="annotation" expression="org.springframework.stereotype.Controller" /> 
</context:component-scan> 
而在springMVC配置文件中将Service注解给去掉 
<context:component-scan base-package="com"> 
  <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller" /> 
  <context:exclude-filter type="annotation" expression="org.springframework.stereotype.Service" /> 
  </context:component-scan> 
因为spring的context是父子容器，所以会产生冲突，由ServletContextListener产生的是父容 器，springMVC产生的是子容器，子容器Controller进行扫描装配时装配了@Service注解的实例，而该实例理应由父容器进行初始化以 保证事务的增强处理，所以此时得到的将是原样的Service（没有经过事务加强处理，故而没有事务处理能力。 
还有一种方式是将service层改用xml配置,其实这样做也是变相的让springmvc无法扫描service，而只能依赖父窗口也就是ServletContextListener来进行初始化，这样同样被赋予了事务性。
2、自己单独做了个小网站 但是发现action事务不起作用了 但是如果用service层就没问题 找了很多办法没解决 最后自己解决了
其实就是一个加载顺序的问题
首先使用了spring MVC的项目是不需要配置action bean  而是通过spring mvc的配置文件进行扫描注解加载的
spring事务配置文件还有上下文都是通过org.springframework.web.context.ContextLoaderListener加载的，
而spring MVC的action是通过org.springframework.web.servlet.DispatcherServlet加载的 
这样就有个优先级的问题了  web是先启动ContextLoaderListener后启动DispatcherServlet
在ContextLoaderListener加载的时候action并没在容器中，所以现在使用AOP添加事务或者扫描注解都是无用的。
那么解决办法就是在DispatcherServlet 加载spring-MVC配置文件后再进行一次AOP事务扫描和注解事务扫描就OK了
<tx:annotation-driven transaction-manager="transactionManager"/>
<aop:config>  
        <aop:advisor advice-ref="transactionAdvice" pointcut="execution(* com.yang.web.*.action.*.*(..))"/>  
</aop:config>
至于为什么要在Action中加事务
 spring in action 一书中也说过 service dao action 是很经典的组合但不是必须的，对于一个简单的增删改查系统，没必要分那么多层，比如一个简单保存功能 无非就new 一个实体 映射参数 使用了spring jdbcTemplate 保存就一行代码 就一个这么简单的功能有必要 一个service接口 一个service实现类 一行代码调用一个dao接口一个dao实现类  要多建四个类 还要在spring上下文中配置  不累吗？  对于一个简单的系统而言这就是为自己找不自在  明明盖的是民房
 硬要打摩天大楼的地基
