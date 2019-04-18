# 如何在SpringMVC中获取request对象 - z69183787的专栏 - CSDN博客
2016年08月02日 17:47:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1723
**如何在SpringMVC中获取request对象**
1.注解法
Java代码  ![收藏代码](http://yeelor.iteye.com/images/icon_star.png)
- @Autowired
- private  HttpServletRequest request;  
2. 在web.xml中配置一个监听
Xml代码  ![收藏代码](http://yeelor.iteye.com/images/icon_star.png)
- <listener>
- <listener-class>
-             org.springframework.web.context.request.RequestContextListener    
- </listener-class>
- </listener>
之后在程序里可以用
Java代码  ![收藏代码](http://yeelor.iteye.com/images/icon_star.png)
- HttpServletRequest request = ((ServletRequestAttributes)RequestContextHolder.getRequestAttributes()).getRequest();    
 3.直接在参数中引入
Java代码  ![收藏代码](http://yeelor.iteye.com/images/icon_star.png)
- public String hello(HttpServletRequest request,HttpServletResponse response)  

**如何在Struts2中获取request对象**
Java代码  ![收藏代码](http://yeelor.iteye.com/images/icon_star.png)
- HttpServletRequest request = ServletActionContext.getRequest();  
