# java web项目与oracle rac连接JNDI - 零度的博客专栏 - CSDN博客
2016年04月28日 12:32:09[零度anngle](https://me.csdn.net/zmx729618)阅读数：1999
某环境使用的oralce数据库采用了rac的策略做了负载均衡，现需要将开发的项目部署至该环境， 
假设，该oracle环境配置了2个IP。 
【发现】： 
当使用PL/SQL客户端连接其中一个IP的oracle时，是可以连接的。 
但部署web项目时，使用传统的spring配置与某一个IP相连时，web项目启动失败，报错：数据库SID无效， 
【解决方法】： 
修改spring的配置文件中关于数据源的配置中的数据库连接地址（url），如下： 
Java代码  ![收藏代码](http://zhbwww.iteye.com/images/icon_star.png)
- <bean id="dataSource"
- class="org.logicalcobwebs.proxool.ProxoolDataSource">    
-         <property name="driver" value="${db.driver}" />    
-         <property name="driverUrl" value="jdbc:oracle:thin:@(DESCRIPTION =    
-           (ADDRESS_LIST =    
-                     (ADDRESS = (PROTOCOL = TCP)(HOST =  IP1)(PORT = 1521))    
-                     (ADDRESS = (PROTOCOL = TCP)(HOST = IP2)(PORT = 1521))    
-                     (FAILOVER=yes)     
-                     (LOAD_BALANCE =yes)    
-          )    
-          (CONNECT_DATA =    
-                  (SERVER = DEDICATED)    
-                 (SERVICE_NAME = 集群的服务名)    
-           )    
-     )    
-     "/>    
-         <property name="user" value="${db.user}" />    
-         <property name="password" value="${db.password}" />    
- </bean>    
web服务器中JNDI的配置方式如下（以下为tomcat中JNDI的配置）： 
Java代码  ![收藏代码](http://zhbwww.iteye.com/images/icon_star.png)
- <Context path="/web访问名" docBase="路径" debug="5" reloadable="false" crossContext="true">    
-             <Resource    
-                 name="jndi名"
-                 type="javax.sql.DataSource"
-                 url="jdbc:oracle:thin:@(DESCRIPTION =    
-           (ADDRESS_LIST =    
-                     (ADDRESS = (PROTOCOL = TCP)(HOST = IP1)(PORT = 1521))    
-                     (ADDRESS = (PROTOCOL = TCP)(HOST = IP2)(PORT = 1521))    
-                     (FAILOVER=yes)     
-                     (LOAD_BALANCE =yes)    
-          )    
-          (CONNECT_DATA =    
-                  (SERVER = DEDICATED)    
-                 (SERVICE_NAME = 集群的服务名)    
-           )    
-     )"    
-                 driverClassName="oracle.jdbc.driver.OracleDriver"
-                 username="数据库用户名"
-                 password="数据库密码"
-                 maxIdle="5"
-                 maxWait="5000"
-                 maxActive="10"/>     
- </Context>    
