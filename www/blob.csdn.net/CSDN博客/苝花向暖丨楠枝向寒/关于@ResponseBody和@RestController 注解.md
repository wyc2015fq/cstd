# 关于@ResponseBody和@RestController 注解 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月05日 20:24:28[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：130


这两个注解主要用于和ajax的交互，由于在Spring MVC框架中、 return 代表转发，如果在该方法上面加上@ResponseBody 注解、那么 return 就和response.getWriter().print()功能一样 ，直接把 字符串 传给了 ajax了，而不进行转发了。 

如果 这个类的 所有方法 均有这样的需要，那就在 类上面 加上@ResponseBody ，但如果用的SSM 框架，这时类上面会有两个注解，一个是@controller、一个是@ResponseBody，这是可以用 @RestController 来代替 这两个注解。


注意：如果用@ResponseBody注解，ajax拿到数据以后，显示到前端页面、即使配置了过滤器也会乱码。

需要在 springMVC.xml 中  配置一下内容

<bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">    
       <property name="messageConverters">         
       <list>            
        <bean class="org.springframework.http.converter.StringHttpMessageConverter">                
       <property name="supportedMediaTypes">                     
       <list>                         
       <bean class="org.springframework.http.MediaType">                             
       <constructor-arg index="0" value="text" />                             
       <constructor-arg index="1" value="json" />                              
       <constructor-arg index="2" value="UTF-8" />                         
       </bean>                    
        </list>                 
        </property>            
         </bean>        
          </list>     
          </property> 
          </bean>

