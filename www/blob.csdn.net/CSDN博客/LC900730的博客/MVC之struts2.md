# MVC之struts2 - LC900730的博客 - CSDN博客
2017年08月04日 10:59:48[lc900730](https://me.csdn.net/LC900730)阅读数：102标签：[struts](https://so.csdn.net/so/search/s.do?q=struts&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## Struts2框架
```
Servlet启动控制器作用；
    1.获取请求数据封装；
    2.调用service处理业务逻辑
    3.跳转(转发/重定向)
```
如： 
    登录成功–>首页 
    注册成功–>登录/首页? 
跳转代码写死。 
每次都需要配置servlet
Servlet+JSP+JavaBean 
Struts2框架预先实现 一些功能：
```
1.请求数据的 自动封装；
2.文件上传功能；
3.对国际化功能的简化
4.数据校验功能
```
### struts
控制器：FilterDispatcher：
```
用户请求到达前端控制器FilterDispatcher。FilterDispatcher负责根据用户提交的URL和struts.xml中的配置，来选择合适的动作(Action),让这个Action来处理用户的请求。FilterDispatcher其实是一个过滤器(Filter,servlet规范中的一种web组件)。
```
动作–Action
```
在请求经过FilterDispatcher之后，被分发到了合适的Action对象。Action负责把用户请求中的参数组装成合适的数据模型，并调用相应的业务逻辑进行真正的功能处理，然后获取下一个视图 需要展示的数据。
```
Struts2中的Action，相比于别的Web框架，实现了与Servlet API的解耦，使得在Action里面不需要直接去引用HttpServletRequest和HttpServletResponse等接口。因而使得Action的单元测试更加简单，而且还有强大的类型转换;
Struts开发步骤： 
    1.web项目，引入struts-jar包 
    2.web.xml中，引入struts的核心功能 
    3.开发action 
    4.配置action 
        src/struts.xml
### struts开发之helloworld
```
helloworld
    WebContent:
        WEB-INF
            lib/
                导入：
```
![这里写图片描述](https://img-blog.csdn.net/20170801174305406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
servletimpl
            login.jsp
            welcome.jsp
        src
            com.it.a
            HelloWorldAction.java
        struts.xml
        <?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE struts PUBLIC  
"-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"  
"http://struts.apache.org/dtds/struts-2.0.dtd">  
<struts>  
    <constant name="struts.devMode" value="true" />  
<constant name="struts.locale" value="zh_CN"/>  
<constant name="struts.i18n.encoding" value="UTF-8"/>  
<package name="helloworld"  extends="struts-default">  
    <action name="helloworldAction" class="com.zhougu.a.HelloWorldAction" method="execute">  
        <result name="toWelcome">/servletimpl/welcome.jsp</result>  
    </action>  
</package>  
</struts>
```
login.jsp
```xml
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
    <form action="/helloworld/helloworldAction.action" method="post">
    <input type="hidden" name="submitFlag" value="login"/>
    账号：<input type="text" name="account"><br/>
    密码：<input type="password" name="password"><br/>
    <input type="submit" value="提交">
    </form> 
</body>
</html>
```
## struts2中的jar包
```
commons-fileupload-1.2.2.jar【文件上传相关】
struts2-core-2.3.4.1.jar 【struts2核心功能包】
xwork-core-2.3.4.1.jar xwork【核心功能包】
ognl-3.0.5.jar    【ognl表达式支持包，页面取值使用】
commons-lang3-3.1.jar  【 struts对java.lang包的扩展】
freemarker-2.3.19.jar  【struts标签模板库文件】
javassist-3.11.0.GA.jar 【struts对字节码的处理相关jar】
```
### 配置web.xml
```
tomcat启动的时候-->加载自身的web.xml-->加载所有项目的web.xml
通过在项目的web.xml中引入了过滤器
        -->struts的核心功能初始化，通过过滤器完成
        -->filter【init/doFilter/destroy】
```
StrutsPrepareAndExecuteFilter:即为核心过滤器 
注意：  
    使用的struts版本不同，核心过滤器类是不一样的! 
开发Action类 
        注意 
        1. 
        action类，也叫动作类，一般继承ActionSupport类 
        即处理请求的类(struts中的action类取代之前的servlet) 
        2. 
        action中的业务方法，处理具体的请求 
            必须返回string 
            方法不能有参数 ()
### 配置struts.xml
Action的话，每次访问都会创建action实例
### Struts2执行流程
服务器启动： 
    1.加载项目web.xml 
    2.创建Struts核心过滤器对象，执行filter–>init 
        struts-default.xml    核心功能初始化 
        struts-plugin.xml      struts相关插件 
        struts.xml            用户编写的 配置文件
```
访问：
3.用户每次访问Action，服务器根据访问路径名称，找对应的action，创建action对象,后面每次访问，都会创建action实例
4.执行默认拦截器栈中定义的18个拦截器
5.执行action中的业务处理方法
```
struts2-core-2.3.4.1.jar/struts-default.xml 
内容： 
1.bean结点指定struts在运行的时候创建的对象类型 
2.指定srtuts-default包【用户写的package(struts.xml)一样要继承此包】
```
package    struts-default中定义了
    **a.跳转的结果类型**
<result name="success" type="dispatcher">  
dispatcher  转发，不指定默认为转发
redirect：重定向
redirectionAction:重定向到action资源  
<stream>(文件下载的时候使用)
    **b拦截器**
定义了所有的拦截器
    定义了32个拦截器
为了拦截器引用方便，可以通过定义栈的方式引用拦截器
如果引用了栈，则栈中的拦截器都会被顺序执行
    <interceptor-stack name="XXX">
        <interceptor-ref name="exception"/>
        <interceptor-ref name="i18n"/>
    **c.默认的拦截器**
<default-interceptor-ref name="defaultStack"/>
<default-class-ref class="com.opensymphony.xwork2.ActionSupport">
```
如：
```
<action name="hello"    class="cn.itcast.action.HelloAction" method="add">
<result name="success" type="redirectAction">
    list
</result>
</action>
    <action name="list"     class="cn.itcast.action.HelloAction2" method="add">
<result name="success" type="dispatcher">
    /success.jsp
</result>
</action>
```
### 拦截器
```
拦截器与过滤器类似
区别：
    共同点：都拦截资源！
    区别：
        过滤器，拦截器所有资源都可以(jsp/servlet/img/css/js)
        拦截器：只拦截action请求
拦截是struts的概念，只能在struts中用；
过滤器是servlet概念，可以在struts、servlet项目中使用
```
### 共性问题
```
struts.xml配置文件没有提示；
解决：找到struts-2.0.dtd文件，拷贝到某个目录：d:/dtd(不要用中文)
```
### struts.xml中的配置
#### package
```
package：定义一个包，管理action(通常一个模块用一个包)
name:包的名字，包名不能重复
extends：当前包继承自哪个包，在struts中，包一定要继承自struts-default(struts-default.xml中定义的包)
abstract：true(如果定义了此属性，那么为抽象包，不能有action的定义，否则运行报错)
只有当当前包被其他包继承的时候才用。
    namespace：命名空间，默认为/，作为路径的一部分
    访问路径：localhost/项目名称/命名空间/action
如果namespace='/user',那么访问路径就是${pageContext.request.contextPath}/user/login
这个/user就是命名空间
    login就是action的name
```
如form提交到${pageContext.request.contextPath}/user/login，那么 就是提交到package的namespace=/user下面的对应的login的action
#### action
```
配置请求路径与Action类的映射关系
name：请求路径名称
class：请求处理类的action类的全名称
method:请求处理方法
result:
    name:action方法的返回值
    type:跳转的结果类型
```
tomcat运行struts2只会默认加载src/struts.xml文件 
    我们包中的如cn.a/hello.xml,,cn.b/config.xml，那么我们 
在struts.xml中
```
<struts>
        <include file="cn/a/hello.xml"></include>
        <include file="cn/b/config.xml"></include>
    </struts>
```
