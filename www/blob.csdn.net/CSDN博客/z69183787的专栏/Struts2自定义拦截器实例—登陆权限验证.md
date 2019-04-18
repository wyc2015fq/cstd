# Struts2自定义拦截器实例—登陆权限验证 - z69183787的专栏 - CSDN博客
2014年07月15日 14:30:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1003
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)

版本：struts2.1.6
此实例实现功能：用户需要指定用户名登陆，登陆成功进入相应页面执行操作，否则返回到登陆页面进行登陆，当直接访问操作页面（登陆后才能访问的页面）时则不允许，须返回登陆页面。
代码如下：
一、页面
login.jsp
**[xhtml]**[view
 plain](http://blog.csdn.net/java_cxrs/article/details/5514340#)[copy](http://blog.csdn.net/java_cxrs/article/details/5514340#)
- <%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%>
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
- <html>
- <head>
- <title>yuewei'Login</title>
- <!--
-     <link rel="stylesheet" type="text/css" href="styles.css" mce_href="styles.css">
-     -->
- </head>
- 
- <body>
- <formaction="login.action"method="post">
-   User:<inputtype="text"name="username"><br>
-   Passoword:<inputtype="password"name="password"><br>
- <inputtype="submit"value="submit">
- </form>
- 
- </body>
- </html>
welcome.jsp
**[xhtml]**[view
 plain](http://blog.csdn.net/java_cxrs/article/details/5514340#)[copy](http://blog.csdn.net/java_cxrs/article/details/5514340#)
- <%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%>
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
- <html>
- <head>
- 
- <title> yuewei's Welcome</title>
- 
- <!--
-  <link rel="stylesheet" type="text/css" href="styles.css" mce_href="styles.css">
-  -->
- 
- </head>
- 
- <body>
- <h1>登录成功后显示此页面</h1>
- <ahref="show.action"mce_href="show.action">show</a>
- </body>
- </html>
show.jsp
**[xhtml]**[view
 plain](http://blog.csdn.net/java_cxrs/article/details/5514340#)[copy](http://blog.csdn.net/java_cxrs/article/details/5514340#)
- <%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%>
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
- <html>
- <head>
- 
- <title>yuewei's Show</title>
- 
- <!--
-     <link rel="stylesheet" type="text/css" href="styles.css" mce_href="styles.css">
-     -->
- 
- </head>
- 
- <body>
-     Show This Page   
-         登录后执行此页面<br>
- </body>
- </html>
二、Action
LoginFormAction
**[java]**[view
 plain](http://blog.csdn.net/java_cxrs/article/details/5514340#)[copy](http://blog.csdn.net/java_cxrs/article/details/5514340#)
- package com.ywjava.action;  
- 
- import com.opensymphony.xwork2.ActionSupport;  
- 
- publicclass LoginFormAction extends ActionSupport {  
- public String exexcute() {  
- return"success";  
-     }  
- }  
LoginAction
**[java]**[view
 plain](http://blog.csdn.net/java_cxrs/article/details/5514340#)[copy](http://blog.csdn.net/java_cxrs/article/details/5514340#)
- package com.ywjava.action;  
- 
- import com.opensymphony.xwork2.ActionContext;  
- import com.opensymphony.xwork2.ActionSupport;  
- import com.ywjava.utils.Constants;  
- 
- publicclass LoginAction extends  ActionSupport{  
- private String username;  
- private String password;  
- public String getPassword() {  
- return password;  
- }  
- publicvoid setPassword(String password) {  
- this.password = password;  
- }  
- public String getUsername() {  
- return username;  
- }  
- publicvoid setUsername(String username) {  
- this.username = username;  
- }  
- 
- privateboolean isInvalid(String value) {     
- return (value == null || value.length() == 0);     
- }     
- public String execute(){  
-         System.out.println(username);  
-         System.out.println(password);  
- 
- if (isInvalid(getUsername()))     
- return INPUT;     
- 
- if (isInvalid(getPassword()))     
- return INPUT;     
- 
- 
- if(this.getUsername().equals("yuewei")&& this.getPassword().equals("yuewei")){  
-         ActionContext.getContext().getSession().put(Constants.USER_SESSION,getUsername());  
-         ActionContext.getContext().getSession().put(Constants.PASS,getPassword());  
- return"success";  
-     }  
- return"error";  
- }  
- }  
ShowAction
**[java]**[view
 plain](http://blog.csdn.net/java_cxrs/article/details/5514340#)[copy](http://blog.csdn.net/java_cxrs/article/details/5514340#)
- package com.ywjava.action;  
- 
- import com.opensymphony.xwork2.ActionSupport;  
- 
- publicclass ShowAction extends ActionSupport {  
- public String execute() {  
- return"success";  
-  }  
- }  
三、拦截器
**[java]**[view
 plain](http://blog.csdn.net/java_cxrs/article/details/5514340#)[copy](http://blog.csdn.net/java_cxrs/article/details/5514340#)
- package com.ywjava.interceptot;  
- 
- import java.util.Map;  
- 
- import com.opensymphony.xwork2.Action;  
- import com.opensymphony.xwork2.ActionContext;  
- import com.opensymphony.xwork2.ActionInvocation;  
- import com.opensymphony.xwork2.interceptor.AbstractInterceptor;  
- import com.ywjava.utils.Constants;  
- 
- publicclass LoginInterceptor extends AbstractInterceptor {  
- 
- @Override
- public String intercept(ActionInvocation invocation) throws Exception {  
- 
- // 取得请求相关的ActionContext实例
-         ActionContext ctx = invocation.getInvocationContext();  
-         Map session = ctx.getSession();  
-         String user = (String) session.get(Constants.USER_SESSION);  
- 
- // 如果没有登陆，或者登陆所有的用户名不是yuewei，都返回重新登陆
- 
- if (user != null && user.equals("yuewei")) {  
-             System.out.println("test");  
- return invocation.invoke();  
-         }  
- 
-         ctx.put("tip", "你还没有登录");  
- return Action.LOGIN;  
- 
-     }  
- 
- }  
四 struts.xml
**[c-sharp]**[view
 plain](http://blog.csdn.net/java_cxrs/article/details/5514340#)[copy](http://blog.csdn.net/java_cxrs/article/details/5514340#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <!DOCTYPE struts PUBLIC  
- "-//Apache Software Foundation//DTD Struts Configuration 2.1//EN"
- "http://struts.apache.org/dtds/struts-2.1.dtd">  
- <struts>  
-     <package name="authority" extends="struts-default">  
- 
-         <!-- 定义一个拦截器 -->  
-         <interceptors>  
-             <interceptor name="authority"
- class="com.ywjava.interceptot.LoginInterceptor">  
-             </interceptor>  
-             <!-- 拦截器栈 -->  
-             <interceptor-stack name="mydefault">  
-                 <interceptor-ref name="defaultStack" />  
-                 <interceptor-ref name="authority" />  
-             </interceptor-stack>  
-         </interceptors>  
- 
-         <!-- 定义全局Result -->  
-         <global-results>  
-             <!-- 当返回login视图名时，转入/login.jsp页面 -->  
-             <result name="login">/login.jsp</result>  
-         </global-results>  
- 
-         <action name="loginform"
- class="com.ywjava.action.LoginFormAction">  
-             <result name="success">/login.jsp</result>  
-         </action>  
- 
-         <action name="login"class="com.ywjava.action.LoginAction">  
-             <result name="success">/welcome.jsp</result>  
-             <result name="error">/login.jsp</result>  
-             <result name="input">/login.jsp</result>  
-         </action>  
- 
-         <action name="show"class="com.ywjava.action.ShowAction">  
-             <result name="success">/show.jsp</result>  
-             <!-- 使用此拦截器 -->  
-             <interceptor-ref name="mydefault" />  
-         </action>  
- 
-     </package>  
- </struts>  
