# SpringMVC学习--入门程序 - 建建的博客 - CSDN博客
2017年12月18日 09:54:48[纪建](https://me.csdn.net/u013898698)阅读数：58
　前面基本介绍了下SpringMVC的运行原理，现在按照前面的原理一步步实现一个简单的程序。先搭建一个简单的web工程，将spring的jar包导入项目中。
- 前端控制器配置
　在web.xml中配置如下：
```
1 <servlet>
 2 <servlet-name>springmvc</servlet-name>
 3 <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
 4 <init-param>
 5 <param-name>contextConfigLocation</param-name>
 6 <param-value>classpath:springmvc.xml</param-value>
 7 </init-param>
 8 <load-on-startup>1</load-on-startup>
 9 </servlet>
10 <servlet-mapping>
11 <servlet-name>springmvc</servlet-name>
12 <url-pattern>*.action</url-pattern>
13 </servlet-mapping>
```
　　load-on-startup：表示servlet随服务启动；
　　url-pattern：*.action的请交给DispatcherServlet处理。
　　contextConfigLocation：指定springmvc配置的加载位置，如果不指定则默认加载WEB-INF/[DispatcherServlet 的Servlet 名字]-servlet.xml。
　　servlet拦截方法有如下注意点：
　　1、拦截固定后缀的url，比如设置为 *.do、*.action， 例如：/user/add.action 此方法最简单，不会导致静态资源（jpg,js,css）被拦截。
 　  2、拦截所有，设置为/，例如：/user/add  /user/add.action,此方法可以实现REST风格的url，很多互联网类型的应用使用这种方式。但是此方法会导致静态文件（jpg,js,css）被拦截后不能正常显示。需要特殊处理。
 　　3、拦截所有，设置为/*，此设置方法错误，因为请求到Action，当action转到jsp时再次被拦截，提示不能根据jsp路径mapping成功。
- 配置处理器映射器
　　在classpath下的springmvc.xml中配置处理器映射器
```
1 <!-- 处理器映射器 -->
2 <bean class="org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping" />
```
- 配置处理器适配器
```
1 <!-- 处理器适配器 -->
2 <bean
3 class="org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter" />
```
- 开发hander
```
public class ItemList1 implements Controller {
    @Override
    public ModelAndView handleRequest(HttpServletRequest arg0,
            HttpServletResponse arg1) throws Exception {
        // 商品列表
        List<Items> itemsList = new ArrayList<Items>();
        Items items_1 = new Items();
        items_1.setName("技嘉笔记本");
        items_1.setPrice(6000f);
        items_1.setDetail("P35K V3笔记本电脑！");
        Items items_2 = new Items();
        items_2.setName("苹果手机");
        items_2.setPrice(5000f);
        items_2.setDetail("iphone6plus苹果手机！");
        itemsList.add(items_1);
        itemsList.add(items_2);
        // 创建modelAndView准备填充数据、设置视图
        ModelAndView modelAndView = new ModelAndView();
        // 填充数据
        modelAndView.addObject("itemsList", itemsList);
        // 视图
        modelAndView.setViewName("order/itemsList");
        return modelAndView;
    }
}
```
配置处理器
```
1 <bean name="/items1.action" id="itemList1" class="com.luchao.controller.ItemList1"/>
```
- 配置视图解析器
```
1 <!-- 视图解析器 -->
2     <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
3         <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
4         <property name="prefix" value="/WEB-INF/jsp/"/>
5         <property name="suffix" value=".jsp"/>
6     </bean>
```
如果不使用视图解析器，那么上面控制器代码中需要指定具体的视图名称，如：modelAndView.setViewName("/WEB-INF/jsp/order/itemsList.jsp");可见使用视图解析器可以增加其灵活性。
- 视图代码
```
1 <%@ page language="java" contentType="text/html; charset=UTF-8"
 2     pageEncoding="UTF-8"%>
 3 <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
 4 <%@ taglib uri="http://java.sun.com/jsp/jstl/fmt"  prefix="fmt"%>
 5 <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
 6 <html>
 7 <head>
 8 <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
 9 <title>查询商品列表</title>
10 </head>
11 <body>
12 商品列表：
13 <table width="100%" border=1>
14 <tr>
15     <td>商品名称</td>
16     <td>商品价格</td>
17     <td>商品描述</td>
18 </tr>
19 <c:forEach items="${itemsList }" var="item">
20 <tr>
21     <td>${item.name }</td>
22     <td>${item.price }</td>
23     <td>${item.detail }</td>
24 </tr>
25 </c:forEach>
26 
27 </table>
28 </body>
29 </html>
```
上面控制器实现了controller接口，才能由org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter适配器执行。并且上面的处理器映射器为org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping，这是一种根据Url的名字找到处理器。如上：items1.action对应了com.luchao.controller.ItemList1这个处理类。
　　另一种简单的处理器映射器：org.springframework.web.servlet.handler.SimpleUrlHandlerMapping其配置如下：
- 处理器映射器配置
```
1 <bean class="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping">
2         <property name="mappings">
3             <props>
4                 <prop key="/items1.action">itemList1</prop>
5                 <prop key="/items2.action">itemList2</prop>
6             </props>
7         </property>
8 </bean>
```
　　如果处理器实现了HttpRequestHandler，那么则需要使用org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter适配器。
- 处理器适配器配置
```
1 <bean class="org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter"/>
```
- 处理器代码
```
1 public class ItemList2 implements HttpRequestHandler {
 2     @Override
 3     public void handleRequest(HttpServletRequest request,
 4             HttpServletResponse response) throws ServletException, IOException {
 5         // 商品列表
 6         List<Items> itemsList = new ArrayList<Items>();
 7 
 8         Items items_1 = new Items();
 9         items_1.setName("技嘉笔记本");
10         items_1.setPrice(6000f);
11         items_1.setDetail("P35K V3笔记本电脑！");
12 
13         Items items_2 = new Items();
14         items_2.setName("苹果手机");
15         items_2.setPrice(5000f);
16         items_2.setDetail("iphone6plus苹果手机！");
17 
18         itemsList.add(items_1);
19         itemsList.add(items_2);
20         // 填充数据
21         request.setAttribute("itemsList", itemsList);
22         //视图
23         request.getRequestDispatcher("/WEB-INF/jsp/order/itemsList.jsp").forward(request, response);
24     }
25 }
```
　　//使用此方法可以通过修改response，设置响应的数据格式，比如响应json数据
```
1 response.setCharacterEncoding("utf-8");
2 
3 response.setContentType("application/json;charset=utf-8");
4 
5 response.getWriter().write("json串");
```
　　因为处理器有不同的实现，所以需要通过适配器来满足其处理的需求，这是一种适配器模式的经典实现。
- 使用注解来实现
　　注解处理器映射器和注解处理器适配器配置：
```
1 <!--注解映射器 -->
2 <bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping"/>
3 <!--注解适配器 -->
4 <bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter"/>
```
　　一般中开发中使用
```
1 <mvc:annotation-driven/>
```
　　使用 mvc:annotation-driven代替上边注解映射器和注解适配器配置，mvc:annotation-driven默认加载很多的参数绑定方法，比如json转换解析器就默认加载了，如果使用mvc:annotation-driven不用配置上边的RequestMappingHandlerMapping和RequestMappingHandlerAdapter，实际开发时使用mvc:annotation-driven。
　　另外还需要扫描包来加载处理器，配置如下：
```
1 <!-- 扫描controller注解,多个包中间使用半角逗号分隔 -->
2 <context:component-scan base-package="com.luchao.controller"/>
```
- 处理器代码
```
1 @Controller
 2 public class ItemList3 {
 3     // 商品查询列表
 4     // @RequestMapping实现 对queryItems方法和url进行映射，一个方法对应一个url
 5     // 一般建议将url和方法写成一样
 6     @RequestMapping("/items3")
 7     public ModelAndView queryItems() throws Exception {
 8 
 9         // 调用service查找 数据库，查询商品列表，这里使用静态数据模拟
10         List<Items> itemsList = new ArrayList<Items>();
11         // 向list中填充静态数据
12         Items items_1 = new Items();
13         items_1.setName("技嘉笔记本");
14         items_1.setPrice(6000f);
15         items_1.setDetail("P35K V3笔记本电脑！");
16 
17         Items items_2 = new Items();
18         items_2.setName("苹果手机");
19         items_2.setPrice(5000f);
20         items_2.setDetail("iphone6plus苹果手机！");
21 
22         itemsList.add(items_1);
23         itemsList.add(items_2);
24 
25         // 返回ModelAndView
26         ModelAndView modelAndView = new ModelAndView();
27         // 相当 于request的setAttribut，在jsp页面中通过itemsList取数据
28         modelAndView.addObject("itemsList", itemsList);
29 
30         // 指定视图
31         modelAndView.setViewName("order/itemsList");
32 
33         return modelAndView;
34 
35     }
36 }
```
　以上就是注解和非注解的几种实现，其实还可以根据实际需要来实现其他的处理器映射器和处理器适配器。
