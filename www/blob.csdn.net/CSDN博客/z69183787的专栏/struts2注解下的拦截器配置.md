# struts2注解下的拦截器配置 - z69183787的专栏 - CSDN博客
2014年08月08日 15:50:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3658
   貌似这个这问题已经纠结了半年了，网上找资料也不是很多。上周六调试通了。借CSDN mark一下吧。
# 一  、实现
1、 首先上拦截器部分的代码
- publicclass CheckLoginInterceptor extends AbstractInterceptor {  
- 
- privatestaticfinallong serialVersionUID = 1L;  
- publicstaticfinal String USER_SESSION_KEY = "user";  
- private Logger LOG = Logger.getLogger(CheckLoginInterceptor.class);  
- 
- @Override
- public String intercept(ActionInvocation actionInvocation) throws Exception {  
-         LOG.info("begin check login interceptor");  
- // 对LoginAction不做该项拦截
-         Object action = actionInvocation.getAction();  
- if (action instanceof LoginAction) {  
-             LOG.info("exit check login, because this is login action.");  
- return actionInvocation.invoke();  
-         }  
- // 验证 session
-         Map<String, Object> session = ActionContext.getContext().getSession();  
-         String username = (String) session.get(USER_SESSION_KEY);  
-         LOG.info("username-----------" + username);  
- if (username != null) {  
- // 存在的情况下进行后续操作。
-             LOG.info(username + " already login!");  
- return actionInvocation.invoke();  
-         } else {  
- // 否则终止后续操作，返回LOGIN
-             LOG.info("no login, forward login page!");  
- return Action.LOGIN;  
-         }  
-     }  
- }  
2、然后就是struts.xml的内容
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE struts PUBLIC  
-     "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"  
-     "http://struts.apache.org/dtds/struts-2.0.dtd">
- <struts>
- <packagename="custom-default"extends="struts-default">
- <!-- 继承默认的 struts2 配置文件 -->
- <interceptors>
- <interceptorname="checkLogin"
- class="com.cmcc.intt.util.CheckLoginInterceptor"/>
- <interceptor-stackname="checkLoginStack">
- <interceptor-refname="checkLogin"></interceptor-ref>
- <interceptor-refname="defaultStack"></interceptor-ref>
- </interceptor-stack>
- </interceptors>
- <!-- 设置全局 全局默认的拦截器栈-->
- <default-interceptor-refname="checkLoginStack"></default-interceptor-ref>
- <!-- 定义全局Result -->
- <global-results>
- <!-- 当返回login视图名时，转入/login.jsp页面 -->
- <resultname="login">/login.jsp</result>
- </global-results>
- </package>
- </struts>
3、LoginAction的代码(只上关键部分，下面也一样)
- @ParentPackage("custom-default")  
- @InterceptorRefs(@InterceptorRef("checkLoginStack"))  
- @Results( { @Result(name = SUCCESS, location = "/redirect.jsp"),  
- @Result(name = "input", location = "/login.jsp") })  
- publicclass LoginAction extends ActionSupport {  
- public String param = null;  
- privatestaticfinallong serialVersionUID = -7146549074248299166L;  
- private Logger LOG = Logger.getLogger(LoginAction.class);  
- 
- public String execute() {  
-         LOG.info(param);  
-         Map<String, Object> session = ActionContext.getContext().getSession();  
- if (param.equals("1")) {  
-             session.put("user", "123");  
- return SUCCESS;  
-         } else {  
-             session.remove("user");  
- return"input";  
-         }  
-     }  
-  （public getter  and setter  ………………）  
- }  
附 通过http://localhost:8080/intt/config-browser/index.action   看此action的Action information如下：
[Results](http://localhost:8080/intt/config-browser/showConfig.action?namespace=&actionName=sign&detailView=results)|Name|Type|Parameters|
|----|----|----|
|input|org.apache.struts2.dispatcher.ServletDispatcherResult|location = /login.jsp|
|success|org.apache.struts2.dispatcher.ServletDispatcherResult|location = /redirect.jsp|
|login|org.apache.struts2.dispatcher.ServletDispatcherResult|location = /login.jsp|
[Interceptors](http://localhost:8080/intt/config-browser/showConfig.action?namespace=&actionName=sign&detailView=interceptors)|Name|Type|
|----|----|
|CheckLoginInterceptor|com.cmcc.intt.util.CheckLoginInterceptor|
|……(18个defaultstack的拦截器的第一个)|……………………|
|……(18个defaultstack的拦截器的第二个)|…………|
|……(18个defaultstack的拦截器的第三个)|…………|
可以看到  此action继承了custom-default中定义的login以及CheckLoginInterceptor这个拦截器
4、最后是仿业务逻辑的 代码
- @ParentPackage("custom-default")  
- @InterceptorRefs(@InterceptorRef("checkLoginStack"))  
- @Results( { @Result(name = SUCCESS, location = "/redirect.jsp"),  
- @Result(name = "input", location = "/login.jsp") })  
- publicclass SignAction extends ActionSupport {  
- privatestaticfinallong serialVersionUID = -1326137892057921638L;  
- 
- public String execute() {  
-         System.out.println("SignAction.execute");  
- return SUCCESS;  
-     }  
- }  
  此action的Action information的跟LoginAction一样，就不说了
# 二  执行过程  
1 模仿已登录 
http://localhost:8080/intt/login?param=1  模仿登录(1),完成后浏览http://localhost:8080/intt/sign 则显示 “already login!” (2)
(1)console 显示 “begin check login interceptor    exit check login, because this is login action.”
(2)console 显示"begin check login interceptor   username-----------123   123 already login!"
2 模仿未登录
http://localhost:8080/intt/login?param=2  模仿登录(1)。 完成后浏览http://localhost:8080/intt/sign 则显示 “please login” (2)
(1)console 显示 “begin check login interceptor    exit check login, because this is login action.”
(2)console 显示"begin check login interceptor   username-----------null    no login, forward login page!     Forwarding to location /login.jsp"
# 三    总结
                在util包中写入类似CheckLoginInterceptor 这样的拦截器代码。然后在struts.xml中新增一个package 建立拦截器栈。建好后在action的parentpackage中继承此package并且拦截器要填入刚才新建的拦截器栈。
                     如果你在新增的package定义LoginAction 的跳转映射。对不起哦，这样做是不行滴。但是如果你把注解的两个包( struts2-config-browser-plugin-2.2.1.jar, struts2-convention-plugin-2.2.1.jar)拿掉就行了。但这样一做，就不是注解了撒。
# 四  运行环境
                   下面我把我的运行环境的 jar包贴出来吧
1)        commons-fileupload-1.2.2.jar
2)        commons-io-1.3.2.jar
3)        commons-lang-2.3.jar
4)        commons-logging-1.1.1.jar
5)        freemarker-2.3.16.jar
6)        javassist-3.11.0.GA.jar
7)        log4j-1.2.16.jar
8)        ognl-3.0.1.jar
9)        struts2-config-browser-plugin-2.2.1.jar
10)    struts2-convention-plugin-2.2.1.jar
11)    struts2-core-2.2.1.jar
12)    xwork-core-2.2.1.jar
