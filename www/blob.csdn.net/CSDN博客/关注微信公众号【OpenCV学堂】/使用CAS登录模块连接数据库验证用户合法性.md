# 使用CAS登录模块连接数据库验证用户合法性 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年11月06日 17:16:18[gloomyfish](https://me.csdn.net/jia20003)阅读数：4197








**使用CAS登录模块连接应用数据库验证用户合法性**

关于如何配置CAS登录模块集成到开发的Web Application请看这里：

[http://blog.csdn.net/jia20003/article/details/49683827](http://blog.csdn.net/jia20003/article/details/49683827)

 基于cas server 3.5.0版本配置实现。

这里我们主要讲述如何利用已经存在的用户数据表user_table中的username与password两个字段，实现用户从CAS模块登录时候查询我们应用数据库basicweb中的user_table表。验证用户名与密码，真正实现用户合法性检查。



大致步骤需要实现如下配置，首先打开cas server的war包然后在WEB-INF目录下找到deployerConfigContext.xml这个文件，打开找到：


`<bean class="org.jasig.cas.authentication.handler.support.SimpleTestUsernamePasswordAuthenticationHandler" />`


`替换为：`

``
``
``

```
<bean id="SearchModeSearchDatabaseAuthenticationHandler"
	  class="org.jasig.cas.adaptors.jdbc.SearchModeSearchDatabaseAuthenticationHandler"
	  abstract="false" lazy-init="default" autowire="default" >
  <property  name="tableUsers">
   <value>user_table</value>
  </property>
  <property name="fieldUser">
   <value>username</value>
  </property>
  <property name="fieldPassword">
   <value>password</value>
  </property>
  <property name="dataSource" ref="dataSource" />
</bean>
 
<bean class="org.jasig.cas.adaptors.jdbc.QueryDatabaseAuthenticationHandler">
  <property name="dataSource" ref="dataSource" />
  <property name="sql" value="select password from user_table where lower(username) = lower(?)" />
</bean>
```



其中user_table为basicweb数据库中的表

username与password是它的两个字段名



然后在最外层的beans标签内添加DataSource相关配置如下：



```
<bean id="dataSource" class="org.springframework.jdbc.datasource.DriverManagerDataSource">  
	<property name="driverClassName"><value>com.mysql.jdbc.Driver</value></property>  
	<property name="url"><value>jdbc:mysql://localhost:3306/basicweb</value></property>  
	<property name="username"><value>root</value></property>  
	<property name="password"><value>password123</value></property>  
</bean>
```



保存以后覆盖war中原来的配置文件即可。最后还需要打开war中WEB-INF目录下lib目录把mysql JDBC连接支持jar与下载好的cas-server-3.5.0-RC1\modules\cas-server-support-jdbc-3.5.0-RC1.jar都添加到lib目录中去，然后保存war，重新部署到tomcat\webapps目录下即可。

测试一下发现，Post提交form的时候取不到数据，解决办法是添加如下配置在web.xml中：



```
<filter>
		<filter-name>CAS Validation Filter</filter-name>
		<filter-class>org.jasig.cas.client.validation.Cas20ProxyReceivingTicketValidationFilter</filter-class>
		<init-param>
			<param-name>casServerUrlPrefix</param-name>
			<param-value>http://localhost:8080/cas</param-value>
		</init-param>
		<init-param>
			<param-name>serverName</param-name>
			<param-value>http://localhost:8080</param-value>
		</init-param>


		<!-- 代理 -->
		<init-param>
			<param-name>acceptAnyProxy</param-name>
			<param-value>true</param-value>
		</init-param>

		<!-- 配置为false，否则使用代理传来的参数接收不到 -->
		<init-param>
			<param-name>redirectAfterValidation</param-name>
			<param-value>false</param-value>
		</init-param>

	</filter>
```
原因在于如果不添加，直接提交它会去验证ticket，发现我们提交的URL后面没有ticket就认为不合法，就重新创建了新ticket，导致form数据丢失








