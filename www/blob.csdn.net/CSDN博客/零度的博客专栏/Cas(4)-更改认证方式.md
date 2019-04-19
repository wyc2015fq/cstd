# Cas(4)-更改认证方式 - 零度的博客专栏 - CSDN博客
2016年05月25日 13:49:32[零度anngle](https://me.csdn.net/zmx729618)阅读数：1666
       在Cas Server的WEB-INF目录下有一个deployerConfigContext.xml文件，该文件是基于[spring](http://lib.csdn.net/base/17)的配置文件，里面存放的内容常常是部署人员需要修改的内容。其中认证方式也是定义在这个文件中的，id为authenticationManager的bean的authenticationHandlers即定义了需要使用的AuthenticationHandler列表。默认使用了两个AuthenticationHandler，第一个是用来确保当前使用的是https协议的HttpBasedServiceCredentialsAuthenticationHandler，第二个是我们需要改的，其简单认证用户名与密码相等的SimpleTestUsernamePasswordAuthenticationHandler。我们只需要更改这里的SimpleTestUsernamePasswordAuthenticationHandler就行了。Cas中已经为我们提供了不少AuthenticationHandler的实现，包括基于[数据库](http://lib.csdn.net/base/14)认证的实现，当然用户也可以实现自己的AuthenticationHandler。下面将以使用数据库进行认证为例讲解如何更改认证方式。
       Cas的各个模块都是基于Maven开发的，Cas Server也不例外。所以官方推荐我们使用Maven的War覆盖机制来修改Cas Server的配置文件。Maven的War覆盖机制是指当一个package类型为war的Maven项目A中引入了一个package类型为war的项目B作为依赖时，最终项目A打包的war包中不仅会包含项目A的内容，还会包含项目B的内容，且相同位置的文件项目A中的会覆盖项目B中的，即当项目A和项目B在WEB-INF下都拥有一个web.xml文件时，最终生成的war包中将使用项目A在WEB-INF下的web.xml文件；而当项目A在WEB-INF下没有web.xml文件，而项目B在WEB-INF下拥有web.xml文件时最终生成的war包中将使用项目B在WEB-INF下的web.xml文件。所以如果我们要修改Cas
 Server的配置文件，我们可以建立一个自己的基于Maven的Web项目，然后引入Cas Server作为依赖，并在自己的项目中建立对应的deployerConfigContext.xml文件。下面来看详细步骤。
       1、建立基于Maven的Web项目，取名为myCasServer。
       2、打开pom.xml文件，删除默认的依赖项，添加如下内容。
<build>
<plugins>
<plugin>
<artifactId>maven-war-plugin</artifactId>
<configuration>
<warName>cas</warName>
</configuration>
</plugin>
</plugins>
</build>
<dependencies>
<dependency>
<groupId>org.jasig.cas</groupId>
<artifactId>cas-server-webapp</artifactId>
<version>${cas.version}</version>
<type>war</type>
<scope>runtime</scope>
</dependency>
</dependencies>
<properties>
<cas.version>3.5.2</cas.version>
</properties>
<repositories>
<repository>
<id>ja-sig</id>
<url>http://oss.sonatype.org/content/repositories/releases/
</url>
</repository>
</repositories>
       3、删除myCasServer项目中src/main/webapp下的index.jsp文件和src/main/webapp/WEB-INF下的web.xml文件，因为在cas-server-webapp中都存在对应的文件，不删除的话打包后的结果将是myCasServer中的覆盖cas-server-webapp中的。如果这个时候使用Maven进行打包的话你将得到一个和cas-server-webapp一模一样的war包。
       4、使用数据库进行认证的话还需要添加对应的依赖，打开myCasServer的pom.xml文件，添加如下依赖。
<dependency>
<groupId>org.jasig.cas</groupId>
<artifactId>cas-server-support-jdbc</artifactId>
<version>${cas.version}</version>
<scope>runtime</scope>
</dependency>
       5、将cas-server-webapp/WEB-INF下的deployerConfigContext.xml文件copy到myCasServer的src/main/webapp/WEB-INF目录下。
       6、基于数据库的AuthenticationHandler有多种，这里以QueryDatabaseAuthenticationHandler为例。QueryDatabaseAuthenticationHandler需要配置两个参数，dataSource和sql。dataSource就是数据源，表示从哪个数据源进行查询。sql是对应的查询语句，其会接收username作为参数，然后查询出对应的密码，之后QueryDatabaseAuthenticationHandler会将查询出来的密码与用户提交的密码进行匹配。所以这里我们打开复制到myCasServer中的deployerConfigContext.xml文件，找到id为authenticationManager的bean的authenticationHandlers属性定义，将最后一个AuthenticationHandler替换成我们想要的QueryDatabaseAuthenticationHandler。
**       替换前**：
<propertyname=*"authenticationHandlers"*>
<list>
<beanclass=*"org.jasig.cas.authentication.handler.support.HttpBasedServiceCredentialsAuthenticationHandler"*
p:httpClient-ref=*"httpClient"*/>
<bean
class=*"org.jasig.cas.authentication.handler.support.SimpleTestUsernamePasswordAuthenticationHandler"*/>
</list>
</property>
**       替换后：**
<propertyname=*"authenticationHandlers"*>
<list>
<beanclass=*"org.jasig.cas.authentication.handler.support.HttpBasedServiceCredentialsAuthenticationHandler"*
p:httpClient-ref=*"httpClient"*/>
<beanclass=*"org.jasig.cas.adaptors.jdbc.QueryDatabaseAuthenticationHandler"*>
<propertyname=*"dataSource"*ref=*"dataSource"*/>
<propertyname=*"sql"*value=*"select password from t_user where username = ?"*/>
</bean>
</list>
</property>
       像dataSource的定义及其需要使用到的依赖包我就不贴了，比较常用。
       打包以后生成的war包中使用的AuthenticationHandler就会是我们在myCasServer的src/main/webapp/WEB-INF目录下的deployerConfigContext.xml文件中定义的QueryDatabaseAuthenticationHandler了。以后需要修改Cas Server中的其它内容也可以依照此种方式进行修改。
（注：本文是基于cas 3.5.2所写，原文地址：[http://haohaoxuexi.iteye.com/blog/2128869](http://haohaoxuexi.iteye.com/blog/2128869)）
