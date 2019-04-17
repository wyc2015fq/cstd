# JavaWeb —— session 解决表单的重复提交 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:23:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：33








# 【表单重复提交的情况】

1.在表单提交一个 Servlet，而 Servlet 又通过请求转发的方式响应一个 JSP/HTML 页面，此时地址栏仍保留着Servlet 的路径，在响应的页面点击刷新，会造成重复提交

2.在响应页面没有到达时，重复点击提交按钮

3.在响应页面点击返回，在返回页面再次提交

# 【解决方法】

对于第一种情况，可以使用重定向来代替转发，从而可以避免，而对于其他的情况，一般是在提交表单中做一个标记，提交到 Servlet 时，检查标记是否存在，并判断是否与预定义的标记一致，若一致则受理请求并销毁标记，若不一致或无标记，则直接响应重复提交提示信息

具体方法：
- 在表单提交页面生成一个随机值 token 并将其放入 session 属性中
- 在表单提交页面将 token 值放入隐藏域中
- 在目标的 Servlet 中，获取 session 和隐藏域中的 token 值，并比较两个值是否一致

	  1）若一致，受理请求，并把 session 域中的 token 属性清除

	  2）若不一致，则直接响应重复提交页面

 表单提交页面

```
<%
    String tokenValue=new Date().getTime()+"";//设置标记值
    session.setAttribute("token",tokenValue);//存入session
%>
<form action="<%= request.getContextPath() %>/tokenServlet" method="post">
    <input type="hidden" name="token" vlaue="<%= tokenValue %>"/>//设置隐藏域
    name:<input type="text" name="name"/>
    password:<input type="password" name="password"/>
    <input type="submit" value="Submit"/>
</form>
```

表单验证界面

```java
HttpSession session=request.getSession();
Object token=session.getAttribute("token");//获取session中的标记
String tokenVale=request.getParemter("token"); //获取标记的标记值
 
if(token!=null&&token.equals(tokenValue)){//标记第一次出现且值与session中的相等
    session.removeAttribute("token");//从session中移除标记
} else {
    response.sendRedirect(request.getContextPath()+"/false.jsp");//重定向到失败页面
    return;
}
 
String name=request.getParameter("name");
String password=request.getParameter("password");
//访问数据库等
...
 
response.sendRedirect(request.getContextPath()+"/success.jsp");//重定向到成功页面
```



