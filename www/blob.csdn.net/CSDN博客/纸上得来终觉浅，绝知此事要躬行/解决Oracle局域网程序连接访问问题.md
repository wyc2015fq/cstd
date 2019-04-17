# 解决Oracle局域网程序连接访问问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年09月02日 16:17:07[boonya](https://me.csdn.net/boonya)阅读数：934








主要问题是在创建实例的时候采用的是默认的Listener设置和实例设置，IP都采用的是localhost，本地访问正常，而局域网内其他机器访问则不能顺利访问。错误代码如下：



```
org.apache.commons.dbcp.SQLNestedException: Cannot create PoolableConnectionFactory (The Network Adapter could not establish the connection)
	at org.apache.commons.dbcp.BasicDataSource.createPoolableConnectionFactory(BasicDataSource.java:1549)
	at org.apache.commons.dbcp.BasicDataSource.createDataSource(BasicDataSource.java:1388)
	at org.apache.commons.dbcp.BasicDataSource.getConnection(BasicDataSource.java:1044)
	at org.springframework.orm.hibernate3.LocalDataSourceConnectionProvider.getConnection(LocalDataSourceConnectionProvider.java:81)
	at org.hibernate.cfg.SettingsFactory.buildSettings(SettingsFactory.java:114)
	at org.hibernate.cfg.Configuration.buildSettingsInternal(Configuration.java:2163)
	at org.hibernate.cfg.Configuration.buildSettings(Configuration.java:2159)
```



解决此问题需操作Net Manger修改Host为局域网IP，操作图例如下： 

#### 第一步：修改监听HOST 
![](http://dl2.iteye.com/upload/attachment/0100/7783/e8c1812b-338e-3963-af87-7a3d3bb8fe2b.png)

#### 第二步：修改数据库实例HOST 
![](http://dl2.iteye.com/upload/attachment/0100/7785/4a2be429-08d7-393d-ab5c-01a3b00ee042.png)

#### 第三步：修改程序数据库配置HOST 
![](http://dl2.iteye.com/upload/attachment/0100/7781/fc8e8b58-a988-3c4d-a436-3547274976cf.png)
注意：不要忘记添加端口1521.否则报错：Cannot create PoolableConnectionFactory (Invalid number format for port number)

#### 第四步：关闭系统防火墙 
     我的系统是win7 64bit,怎么关闭看下面截图： 

![](http://dl2.iteye.com/upload/attachment/0100/7800/430dc38b-46e4-3216-a8f6-0943a72bed39.png)

关闭后界面如下： 

![](http://dl2.iteye.com/upload/attachment/0100/7795/a7bae261-d57a-3891-85bf-2f623fa88b65.png)

经过以上设置程序正常启动。



