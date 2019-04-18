# Spring @Configuration用annotation装配spring - z69183787的专栏 - CSDN博客
2015年08月08日 21:38:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2535
下面是一个典型的Spring配置文件（application-config.xml）：
**Xml代码**
**[xml]**[view
 plain](http://blog.csdn.net/tanksyg/article/details/8556769#)[copy](http://blog.csdn.net/tanksyg/article/details/8556769#)
- <beans>
- <beanid="orderService"class="com.acme.OrderService"/>
- <constructor-argref="orderRepository"/>
- </bean>
- <beanid="orderRepository"class="com.acme.OrderRepository"/>
- <constructor-argref="dataSource"/>
- </bean>
- </beans>
然后你就可以像这样来使用是bean了：
**java代码**
**[java]**[view
 plain](http://blog.csdn.net/tanksyg/article/details/8556769#)[copy](http://blog.csdn.net/tanksyg/article/details/8556769#)
- ApplicationContext ctx = new ClassPathXmlApplicationContext("application-config.xml");  
- OrderService orderService = (OrderService) ctx.getBean("orderService");  
现在Spring Java Configuration这个项目提供了一种通过java代码来装配bean的方案：
**[java]**[view
 plain](http://blog.csdn.net/tanksyg/article/details/8556769#)[copy](http://blog.csdn.net/tanksyg/article/details/8556769#)
- @Configuration
- publicclass ApplicationConfig {  
- 
- public@Bean OrderService orderService() {  
- returnnew OrderService(orderRepository());  
-         }  
- 
- public@Bean OrderRepository orderRepository() {  
- returnnew OrderRepository(dataSource());  
-         }  
- 
- public@Bean DataSource dataSource() {  
- // instantiate and return an new DataSource …
-         }  
- }  
然后你就可以像这样来使用是bean了：
**[java]**[view
 plain](http://blog.csdn.net/tanksyg/article/details/8556769#)[copy](http://blog.csdn.net/tanksyg/article/details/8556769#)
- JavaConfigApplicationContext ctx = new JavaConfigApplicationContext(ApplicationConfig.class);  
- OrderService orderService = ctx.getBean(OrderService.class);  
 这么做有什么好处呢?
     1.使用纯java代码，不在需要xml
     2.在配置中也可享受OO带来的好处
     3.类型安全对重构也能提供良好的支持
     4.依旧能享受到所有springIoC容器提供的功能
对于以注解为中心的配置方式，只需使用 @ImportResource
 注解引入存在的 XML 即可，如下所示： 
@Configuration 
@ImportResource(“classpath:/bookstore/config/spring-beans.xml”) 
public class MyConfig{ 
…… 
} 
