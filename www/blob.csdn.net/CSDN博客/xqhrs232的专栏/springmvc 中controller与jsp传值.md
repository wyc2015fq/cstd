# springmvc 中controller与jsp传值 - xqhrs232的专栏 - CSDN博客
2018年10月26日 15:42:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：55
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/friends-wf/p/3801120.html](https://www.cnblogs.com/friends-wf/p/3801120.html)
在springmvc中的controller所对应的函数中，如果需要从*.jsp页面中获取数据，可以自行在函数括号中写，springmvc会自动封装传过来的。
spring-mvc.xml
中加入
1   <!-- 自动扫描 -->
2 <context:component-scan base-package="cn.itcast.springmvc.service,cn.itcast.springmvc.web.controller"/>
3  <!-- 注解驱动 -->
4 <mvc:annotation-driven/>
Controller.java　　 两种形式都可以，但是第二种，jsp页面中的参数是personList1
 1 //列表
 2     @RequestMapping("/listAll")
 3     public String listAll(Map<String,Object> model){
 4         List<Person> personList = ps.listAll();
 5         model.put("personList", personList);
 6         
 7         System.out.println(" listall hello");
 8         
 9         return "person/jPersonList";
10     }
11     
12     //列表
13     @RequestMapping("/listAllOther")
14     public String listAllOther(Model model){
15         List<Person> personList1 = ps.listAll();
16         model.addAttribute(personList1);
17         
18         System.out.println(" listallother1 hello");
19         
20         return "person/jPersonList";
21     }
jsp页面中
 1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
 3 <html>
 4   <head>
 5     <title>My JSP 'index.jsp' starting page</title>
 6   </head>
 7   
 8   <body>
 9 <h2>用户列表</h2>
10     
11         <div style="padding:10px;"><a href="${pageContext.request.contextPath}/person/tocreate.action">新增</a></div>
12     
13 <table border="1">
14 <tr>
15     <td>photo</td>
16     <td>id</td>
17     <td>name</td>
18     <td>age</td>
19     <td>操作</td>
20 </tr>
21 
22 <c:forEach items="${personList}" var="p">
23 <tr>
24     <td><img src="${pageContext.request.contextPath}"/></td>
25     <td>${p.id}</td>
26     <td>${p.name}</td>
27     <td>${p.age}</td>
28     <td>
29         <a href="${pageContext.request.contextPath}/person/toupdate.action?id=${p.id}">修改</a>
30         <a href="${pageContext.request.contextPath}/person/delete.action?delId=${p.id}">删除</a>
31     </td>
32 </tr>
33 </c:forEach>
34 
35 </table>
36   </body>
37 </html>
