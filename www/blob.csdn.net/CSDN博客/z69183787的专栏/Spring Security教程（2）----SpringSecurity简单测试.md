# Spring Security教程（2）----SpringSecurity简单测试 - z69183787的专栏 - CSDN博客
2014年03月13日 21:20:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11181
前面讲到了SpringSecurity的简单配置，今天做一个简单的测试，先看配置文件
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18350183#)[copy](http://blog.csdn.net/jaune161/article/details/18350183#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/158599/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:sec="http://www.springframework.org/schema/security"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-         http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-         http://www.springframework.org/schema/security  
-         http://www.springframework.org/schema/security/spring-security-3.1.xsd">
- 
- <!-- 配置不过滤的资源（静态资源及登录相关） -->
- <sec:httppattern="/**/*.css"security="none"></sec:http>
- <sec:httppattern="/**/*.jpg"security="none"></sec:http>
- <sec:httppattern="/**/*.jpeg"security="none"></sec:http>
- <sec:httppattern="/**/*.gif"security="none"></sec:http>
- <sec:httppattern="/**/*.png"security="none"></sec:http>
- <sec:httppattern="/js/*.js"security="none"></sec:http>
- 
- <sec:httppattern="/login.jsp"security="none"></sec:http>
- <sec:httppattern="/getCode"security="none"/><!-- 不过滤验证码 -->
- <sec:httppattern="/test/**"security="none"></sec:http><!-- 不过滤测试内容 -->
- 
- <sec:httpauto-config="true">
- 
- <sec:intercept-urlpattern="/app.jsp"access="ROLE_SERVICE"/>
- <sec:intercept-urlpattern="/**"access="ROLE_ADMIN"/>
- 
- </sec:http>
- 
- <sec:authentication-manager>
- <sec:authentication-provider>
- <sec:user-service>
- <sec:username="admin"password="admin"authorities="ROLE_ADMIN"/>
- </sec:user-service>
- </sec:authentication-provider>
- </sec:authentication-manager>
- 
- </beans>
其中
<sec:http pattern="" security="none"></sec:http>
是忽略拦截某些资源的意思，主要是针对静态资源
<sec:intercept-url pattern="/app.jsp" access="ROLE_SERVICE"/>
表示访问app.jsp时，需要ROLE_SERVICE权限
<sec:intercept-url pattern="/**" access="ROLE_ADMIN"/>
表示访问任何资源都需要ROLE_ADMIN权限。
注：/**的配置要放到最后，因为如果放到最前面的话就失去了拦截意义，任何只要有ROLE_ADMIN权限的用户都可以访问任何资源，并不会对app.jsp拦截。因为在访问app.jsp的时候先经过<sec:intercept-url pattern="/**" access="ROLE_ADMIN"/>，、/**又表示任何资源，所以只要具有ROLE_ADMIN权限就会放行。如果放到最后，先经过<sec:intercept-url
 pattern="/app.jsp" access="ROLE_SERVICE"/>，这时候访问app.jsp是就会先判断用户是否有ROLE_SERVICE权限，如果有则放行，反之拦截。
权限必须已ROLE_开头，在后面的文章中我们会讲到如何修改权限头和去掉权限头
authentication-manager用来配置用户管理，在这里我们定义了一个admin用户并且具有ROLE_ADMIN权限，也就是说我们访问任何资源都可以但是访问app.jsp时将被拦截
在没有自定义登录页面之前，SpringSecurity会自动生成登录页面，如下图
![](https://img-blog.csdn.net/20140116094844781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF1bmUxNjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后输入admin/admin登录
![](https://img-blog.csdn.net/20140116095630578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF1bmUxNjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后访问app.jsp发现已被spring拦截，说明我们的配置成功了
![](https://img-blog.csdn.net/20140116095644296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF1bmUxNjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
符：在JSP页面获取当前登录的用户名的方法
首先引入taglib
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18350183#)[copy](http://blog.csdn.net/jaune161/article/details/18350183#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/158599/fork)
- <%@ taglib prefix="sec"uri="http://www.springframework.org/security/tags" %>
然后在jsp页面中使用下面的方法就可以获取到用户名了
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18350183#)[copy](http://blog.csdn.net/jaune161/article/details/18350183#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/158599/fork)
- <sec:authenticationproperty="name"/>
在此仅仅是为了方便测试所以只说下获取用户名的方法，如何获取用户的其他信息将在后续的章节中讲到
