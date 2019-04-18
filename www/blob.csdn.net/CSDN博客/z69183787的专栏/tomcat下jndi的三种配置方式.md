# tomcat下jndi的三种配置方式 - z69183787的专栏 - CSDN博客
2014年03月16日 16:58:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1696
         jndi(Java Naming and Directory Interface，Java命名和目录接口)是一组在Java应用中访问命名和目录服务的API。命名服务将名称和对象联系起来，使得我们可以用名称
访问对象。目录服务是一种命名服务，在这种服务里，对象不但有名称，还有属性。
         tomcat配置jndi有全局配置和局部配置。大致的有以下三种配置方式：
**第一种：全局配置。**
1）在tomcat的conf文件夹下的context.xml配置文件中加入：
**[html]**[view
 plain](http://blog.csdn.net/lgm277531070/article/details/6711177#)[copy](http://blog.csdn.net/lgm277531070/article/details/6711177#)
- <Resourcename="jndi/mybatis"
- auth="Container"
- type="javax.sql.DataSource"
- driverClassName="com.mysql.jdbc.Driver"
- url="jdbc:mysql://localhost:3306/appdb"
- username="root"
- password="123456"
- maxActive="20"
- maxIdle="10"
- maxWait="10000"/>
2)在项目的web.xml中加入资源引用：
**[html]**[view
 plain](http://blog.csdn.net/lgm277531070/article/details/6711177#)[copy](http://blog.csdn.net/lgm277531070/article/details/6711177#)
- <resource-ref>
- <description>JNDI DataSource</description>
- <res-ref-name>jndi/mybatis</res-ref-name>
- <res-ref-type>javax.sql.DataSource</res-ref-type>
- <res-auth>Container</res-auth>
- </resource-ref>
其中res-ref-name值要和context.xml的name值一致。
3）jndi测试方法：
**[java]**[view
 plain](http://blog.csdn.net/lgm277531070/article/details/6711177#)[copy](http://blog.csdn.net/lgm277531070/article/details/6711177#)
- publicvoid testJNDI() throws NamingException, SQLException{  
-     Context ctx = new InitialContext();  
-     DataSource ds = (DataSource) ctx.lookup("java:comp/env/jndi/mybatis");  
-     Connection conn = ds.getConnection();  
-     System.out.println(conn.isClosed());  
- 
- }  
4）在jsp中调用加载jndi方式，不可以直接用main方法测试，必须通过启动容器从jsp中调用：
**[java]**[view
 plain](http://blog.csdn.net/lgm277531070/article/details/6711177#)[copy](http://blog.csdn.net/lgm277531070/article/details/6711177#)
- TestPageAccessURL test = new TestPageAccessURL();  
- test.testJNDI();  
**第二种：局部配置（不推荐）。**
1）在tomcat的server.xml的<host>标签内，添加：
**[html]**[view
 plain](http://blog.csdn.net/lgm277531070/article/details/6711177#)[copy](http://blog.csdn.net/lgm277531070/article/details/6711177#)
- <Contextpath="/demo_jndi"docBase="/demo_jndi">
- <Resource
- name="jndi/mybatis"
- type="javax.sql.DataSource"
- driverClassName="com.mysql.jdbc.Driver"
- maxIdle="2"
- maxWait="5000"
- username="root"
- password="123456"
- url="jdbc:mysql://localhost:3306/appdb"
- maxActive="4"/>
- </Context>
其他配置同第一种方式。
**第三种：局部配置。**
1）在项目的META-INFO下面新建context.xml。加入：
**[html]**[view
 plain](http://blog.csdn.net/lgm277531070/article/details/6711177#)[copy](http://blog.csdn.net/lgm277531070/article/details/6711177#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <Context>
- <Resourcename="jndi/mybatis"
- auth="Container"
- type="javax.sql.DataSource"
- driverClassName="com.mysql.jdbc.Driver"
- url="jdbc:mysql://localhost:3306/appdb"
- username="root"
- password="123456"
- maxActive="20"
- maxIdle="10"
- maxWait="10000"/>
- </Context>
其他配置同第一种方式。
**总结：如果要配置局部的话，推荐使用第三种方式，这样不依赖tomcat了。但是还是推荐使用第一种方式好，虽然依赖tomat，但是是全局的，而且可以配置多个。对于以后切换使用方便。在项目的web.xml中添加的资源引用可有可无。**
