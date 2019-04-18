# jetty配置jndi数据源 - z69183787的专栏 - CSDN博客
2014年03月16日 12:51:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1690
1.下载jetty服务器（8.1.0.RC2），解压到任意目录下
  http://dist.codehaus.org/jetty/jetty-hightide-8.1.0/jetty-hightide-8.1.0.RC2.zip
2.新建jetty-dataSource.xml文件，放在${JETTY_HOME}\contexts目录下
这个例子新建了两个mysql数据源，
**[html]**[view
 plain](http://blog.csdn.net/crazycoder2010/article/details/7099397#)[copy](http://blog.csdn.net/crazycoder2010/article/details/7099397#)[print](http://blog.csdn.net/crazycoder2010/article/details/7099397#)[?](http://blog.csdn.net/crazycoder2010/article/details/7099397#)
- <?xmlversion="1.0"encoding="ISO-8859-1"?>
- <!DOCTYPE Configure PUBLIC "-//Jetty//Configure//EN" "http://www.eclipse.org/jetty/configure.dtd">
- 
- <!-- ==================================================================  
- Configure and deploy the test web application in $(jetty.home)/webapps/test  
- 
- Note. If this file did not exist or used a context path other that /test  
- then the default configuration of jetty.xml would discover the test  
- webapplication with a WebAppDeployer.  By specifying a context in this  
- directory, additional configuration may be specified and hot deployments   
- detected.
- ===================================================================== -->
- 
- <Configureclass="org.eclipse.jetty.webapp.WebAppContext">
- <Setname="contextPath">/datasource</Set>
- <Setname="resourceBase">./</Set>
- <Newid="showcase"class="org.eclipse.jetty.plus.jndi.Resource">
- <Arg></Arg>
- <Arg>jdbc/showcase</Arg>
- <Arg>
- <Newclass="com.mchange.v2.c3p0.ComboPooledDataSource">
- <Setname="driverClass">com.mysql.jdbc.Driver</Set>
- <Setname="jdbcUrl">jdbc:mysql://localhost:3306/showcase?useUnicode=true&characterEncoding=UTF8</Set>
- <Setname="user">root</Set>
- <Setname="password">111111</Set>
- </New>
- </Arg>
- </New>
- 
- <Newid="quartz"class="org.eclipse.jetty.plus.jndi.Resource">
- <Arg></Arg>
- <Arg>jdbc/quartz</Arg>
- <Arg>
- <Newclass="com.mchange.v2.c3p0.ComboPooledDataSource">
- <Setname="driverClass">com.mysql.jdbc.Driver</Set>
- <Setname="jdbcUrl">jdbc:mysql://localhost:3306/quartz?useUnicode=true&characterEncoding=UTF8</Set>
- <Setname="user">root</Set>
- <Setname="password">111111</Set>
- </New>
- </Arg>
- </New>
- </Configure>
3.将数据源用到的jar包放到${JETTY_HOME}\lib\ext目录下
因为以上用到了c3p0数据库连接池和mysql数据库，因此需要将mysql-connector-java-5.1.17.jar和c3p0-0.9.1.2.jar放到jetty容器的lib中
4.重启jetty容器
  命令行到${JETTY_HOME}目录下，运行 java -jar start.jar启动容器
5.附：常见的jndi数据库链接池配置
参考：[http://wiki.eclipse.org/Jetty/Howto/Configure_JNDI_Datasource](http://wiki.eclipse.org/Jetty/Howto/Configure_JNDI_Datasource)
