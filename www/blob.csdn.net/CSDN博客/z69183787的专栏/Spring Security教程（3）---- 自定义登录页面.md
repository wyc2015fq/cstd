# Spring Security教程（3）---- 自定义登录页面 - z69183787的专栏 - CSDN博客
2014年03月13日 21:20:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6280
在项目中我们肯定不能使用Spring自己生成的登录页面，而要用我们自己的登录页面，下面讲一下如何自定义登录页面，先看下配置
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18351247#)[copy](http://blog.csdn.net/jaune161/article/details/18351247#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/158615/fork)
- <sec:httpauto-config="true">
- 
- <sec:intercept-urlpattern="/app.jsp"access="ROLE_SERVICE"/>
- <sec:intercept-urlpattern="/**"access="ROLE_ADMIN"/>
- 
- <sec:form-loginlogin-page="/login.jsp"authentication-failure-url="/login.jsp"
- default-target-url="/index.jsp"/>
- 
- </sec:http>
使用form-login配置来指定我们自己的配置文件，其中
login-page:登录页面
authentication-failure-url：登录失败后跳转的页面
default-target-url：登录成功后跳转的页面
重启Tomcat后，再次打开项目发现登录页面已经变成了我们自己的登录页面，如下图
![](https://img-blog.csdn.net/20140116101225718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF1bmUxNjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
输入用户名密码后跳转到了我们指定的页面
![](https://img-blog.csdn.net/20140116101356796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF1bmUxNjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注：重启Tomcat有时候并不会使Session失效，在测试的时候可能会出现，明明重启了Tomcat可以访问资源时却没有跳到登录页面。所以需要重启浏览器再重试就可以了。
