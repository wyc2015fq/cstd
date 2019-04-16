# 在Web Application中集成CAS登录模块 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年11月06日 15:54:11[gloomyfish](https://me.csdn.net/jia20003)阅读数：6620








一：安装下载与CAS工作原理



关于版本问题：



Java客户端 ：3.1.11



CASServer版本：3.5.0版本



使用的Tomcat版本是7.0的



下载地址[http://developer.jasig.org/cas/](http://developer.jasig.org/cas/)



关于CAS认证授权的工作流程如下：



![](https://img-blog.csdn.net/20151106154032759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


第一步：从浏览器端请求访问web应用主页，web应用是baiscweb



第二步：被CAS的Filter拦截，重定向到CAS服务器，要求登录



第三步：登录以后CAS重定向浏览器访问跟一个ticket参数



第四步：使用带ticket的参数访问basicweb应用的相关servlet



第五步：向CAS服务器检查张票(ticket)是否合法，合法则可以访问



其中第四步，是图中4，5两步合并。



二：CAS server war包修改与部署

下载cas相关版本的war包以后，使用zip编辑器打开找到下面这两个文件：



WEB-INF\deployerConfigContext.xml





WEB-INF\spring-configuration\ticketGrantingTicketCookieGenerator.xml



在deployerConfigContext.xml中的找到这个



```
<bean class="org.jasig.cas.authentication.handler.support.HttpBasedServiceCredentialsAuthenticationHandler"
					p:httpClient-ref="httpClient"/>
```

然后替换为如下：




```
<bean class="org.jasig.cas.authentication.handler.support.HttpBasedServiceCredentialsAuthenticationHandler"
					p:httpClient-ref="httpClient" p:requireSecure="false"/>
```



在ticketGrantingTicketCookieGenerator.xml文件中找到p:cookieSecure="true"这个内容，然后把true改为false



把上述修改好的两个xml文件重新保存到war包中去。Casserver的修改配置就结束了。把修改好的war放到tomcat的webapps目录下即可。

三：Web应用中的配置（客户端配置）



下面就要在我们自己的Web应用中集成CAS功能，我的web应用是baiscweb，是基于maven的项目，所以首先需要在mavne中添加cas client的包支持。添加如下



```
<dependency>
	<groupId>org.jasig.cas.client</groupId>
	<artifactId>cas-client-core</artifactId>
	<version>3.1.11</version>
</dependency>
```



然后在web.xml中添加如下配置实现登录功能：





```
<filter>
	<filter-name>CAS Authentication Filter</filter-name>
	<filter-class>org.jasig.cas.client.authentication.AuthenticationFilter</filter-class>
	<init-param>
		<param-name>casServerLoginUrl</param-name>
		<param-value>http://localhost:8080/cas/login</param-value>
	</init-param>
	<init-param>
		<param-name>serverName</param-name>
		<param-value>http://localhost:8080</param-value>
	</init-param>
</filter>
```

```
<filter-mapping>
	<filter-name>CAS Authentication Filter</filter-name>
	<url-pattern>/*</url-pattern>
</filter-mapping>
```



添加如下配置实现Logout功能：





```
<listener>
	<listener-class>org.jasig.cas.client.session.SingleSignOutHttpSessionListener</listener-class>
</listener>
```

```
<filter>
	<filter-name>CAS Single Sign Out Filter</filter-name>
	<filter-class>org.jasig.cas.client.session.SingleSignOutFilter</filter-class>
	<init-param>
		<param-name>casServerUrlPrefix</param-name>
		<param-value>https://localhost:8080/cas</param-value>
	</init-param>
</filter>
<filter-mapping>
	<filter-name>CAS Single Sign Out Filter</filter-name>
	<url-pattern>/*</url-pattern>
</filter-mapping>
```



完成以后保存web.xml文件，重新部署basicweb应用到tomcat启动以后输入访问地址[http://localhost:8080/basicweb/personInfo](http://localhost:8080/basicweb/personInfo)



就会自动被重定向到CAS登录页面要求登录。






这里使用的登录是它的默认登录方式，只要用户名与密码相同，即可登录访问，我用的是admin/amdin



关于退出(LogOut):



添加如下代码到你的web应用主页相关位置，即可实现退出功能：




`<a href="http://localhost:8080/cas/logout">注销</a>`





**需要注意**的是你的CAS登录Filter必须在所有web应用程序Filter之前

**运行Login界面：**

**![](https://img-blog.csdn.net/20151106155822634)**

**后台打印出来的信息：**

****

```
2015-11-06 15:58:44,353 INFO [org.jasig.cas.web.flow.InitialFlowSetupAction] - <Setting path for cookies to: /cas/>
2015-11-06 15:58:44,397 INFO [com.github.inspektr.audit.support.Slf4jLoggingAuditTrailManager] - <Audit trail record BEGIN
=============================================================
WHO: audit:unknown
WHAT: http://localhost:8080/basicweb/personInfo
ACTION: SERVICE_TICKET_NOT_CREATED
APPLICATION: CAS
WHEN: Fri Nov 06 15:58:44 CST 2015
CLIENT IP ADDRESS: 0:0:0:0:0:0:0:1
SERVER IP ADDRESS: 0:0:0:0:0:0:0:1
=============================================================
```




