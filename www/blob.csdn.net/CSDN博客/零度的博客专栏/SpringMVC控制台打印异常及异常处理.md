# SpringMVC控制台打印异常及异常处理 - 零度的博客专栏 - CSDN博客
2017年01月10日 14:29:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：2660
                
在程序开发中，如果程序发生异常，而控制台没有打印，那将是很麻烦的事情。
使用SpringMvc的时候一般会在springmvc.xml中进行如下配置来处理异常
**[html]**[view plain](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[copy](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[print](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[?](http://blog.csdn.net/ssyang_0102/article/details/38091199#)
- <beanclass="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
- <propertyname="exceptionMappings">
- <props>
- <propkey="java.io.IOException">ioErr.jsp</prop>
- </props>
- </property>
- </bean>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<bean class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">  
        <property name="exceptionMappings">  
            <props>  
                <prop key="java.io.IOException">ioErr.jsp</prop>
            </props>  
        </property>  
    </bean>
```
在上面的XML中，如果没有配置异常处理页面，像这样：
**[html]**[view plain](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[copy](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[print](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[?](http://blog.csdn.net/ssyang_0102/article/details/38091199#)
- <beanclass="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
- 
- </bean>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<bean class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">  
        
</bean>
```
那么程序发生的异常将打印在控制台。
如果对exceptionMappings配置了prop，则异常信息将不再控制台打印输出，可以在定义的ioErr.jsp中进行异常输出：
**[html]**[view plain](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[copy](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[print](http://blog.csdn.net/ssyang_0102/article/details/38091199#)[?](http://blog.csdn.net/ssyang_0102/article/details/38091199#)
- <%  
- Exception exc = (Exception)request.getAttribute("exception");//这样就在JSP页面获取到了程序发生的异常  
- exc.printStackTrace();//这样依然可以在控制台打印异常  
- exc.printStackTrace(new PrintWriter(out))//还可以将异常打印在页面，总之你已经通过request.getAttribute("exception")获取到了程序发生的异常，怎样处理，随便  
- 
-  %>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<%
Exception exc = (Exception)request.getAttribute("exception");//这样就在JSP页面获取到了程序发生的异常
exc.printStackTrace();//这样依然可以在控制台打印异常
exc.printStackTrace(new PrintWriter(out))//还可以将异常打印在页面，总之你已经通过request.getAttribute("exception")获取到了程序发生的异常，怎样处理，随便
 %>
```
这样一来就能快速的定位程序中哪里报错。
