# springMVC中使用dubbo注解配置的问题 - z69183787的专栏 - CSDN博客
2017年12月19日 18:31:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：540
问题，在controller中无法通过注解自动注入dubbo服务，
但是在service中可以自动注入。
package com.sl;
@Controller
public class P{
//期望注入dubbo服务
@Reference(version="1.0.0")
private I0 o;
//注入service
@Autowired
private S s;
@RequestMapping("p")
public  void p() throws IOException{
//o is null!没有自动注入
//s 自动注入且s.s()中dubbo服务也注入成功
}
}
package com.sl;
@Service
public class S{
//成功注入dubbo
@Reference(version="1.0.0")
private I0 o;
public  void s() throws IOException{
//o is not null!可以直接使用
}
}
经过分析，原来配置导致两个上下文，一个是根上下文，一个是springMVC的上下文，
dubbo的注解配置在根上下文中，因此无法解析springMVC bean的注入。
当前配置：
web.xml
....
<!-- 
tomcat启动后，此设置导致spring生成根上下文,且使用默认的applicationContext.xml来获得其他设置。
根上下文保存在ServletContext中，key是WebApplicationContext.ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE
可以用spring工具类获得：WebApplicationContextUtils.getWebApplicationContext(ServletContext);
-->
<listener>
 <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
</listener>
...
<!-- 
这里再生成springMVC的上下文，其父是根上下文。
使用默认的springMVC-servlet.xml配置文件
当一个Request对象产生时，会把这个WebApplicationContext上下文保存在Request对象中，
key是DispatcherServlet.class.getName() + ".CONTEXT"。
可以用spring工具类获得：RequestContextUtils.getWebApplicationContext(request);
-->
<servlet>
        <servlet-name>springMVC</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <load-on-startup>1</load-on-startup>
</servlet>
applicationContext.xml
...
<!-- dubbo使用注解 -->
<dubbo:annotation />
<!-- 需要扫描注解的包 -->
<context:component-scan base-package="com.sl" />  
<!-- dubbo会在根上下文注册postProcessBeforeInitialization，
所以以后这里生成的bean中的@Reference都会被正确解析注入
 -->
springMVC-servlet.xml
...
<!-- MVC注解驱动 -->
<mvc:annotation-driven />
<!-- sprint MVC 注解扫描包-->
<context:component-scan base-package="com.sl" />
<!-- springMVC中并没有dubbo配置所以这里生成的controll bean不会解析 @Reference-->
...
解决办法：
1.只将dubbo服务注入到service和Repository中而不是Controller中，其实大部分时候都可如此
2.去掉web.xml中listener，将全部配置都放到springMVC-servlet.xml，这样只生成一个上下文。
3.在springMVC-servlet.xml也加入duboo的配置，这样虽然有了冗余，但是可以保证两个上下文。
