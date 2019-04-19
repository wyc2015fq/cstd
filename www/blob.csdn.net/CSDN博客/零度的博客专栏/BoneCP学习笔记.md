# BoneCP学习笔记 - 零度的博客专栏 - CSDN博客
2016年08月09日 14:35:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：448
什么是BoneCP
BoneCP 是一个快速、免费而且开源的java数据库连接池(JDBC Pool)管理工具库。如果你曾经使用过C3P0或者DBCP，那你肯定知道上面这句话的意思；如果你没用过这些，那简单的解释一下，它是一个能为你的应用管理数据库连接的工具。
官方网站：[http://jolbox.com/](http://jolbox.com/)
### 为什么要学习BoneCP
       本着不重复发明轮子的原则，不应该在有了C3P0这种成熟的数据库连接池管理工具后再搞个新的出来，但是传说中BoneCP在快速这个特点上做到了极致，官方数据是C3P0等的25倍左右。不相信？其实我也不怎么信，正找时间自己写测试类测试呢，你也可以去试试看， 在[这里下载](http://jolbox.com/bonecp/downloads/maven/com/jolbox/bonecp/0.6.7.2/) （现在最新版本0.6.7.2，如果你有困难，下面会具体提到在项目中怎么配置）
        好吧，先看一下官方给出的华丽数据：
#### 1.    单线程（1,000,000获得及释放数据库连接请求，连接池大小20-50）
#### ![](http://hi.csdn.net/attachment/201008/2/0_12807419542TdE.gif)
####  2.    多线程（500线程分别获取释放100个链接，连接池大小50-200）
![](http://hi.csdn.net/attachment/201008/2/0_12807419471XuM.gif)
#### 3.    Prepared Statement (multi-threaded)（500个线程每个100次获得/释放，连接池大小20-500）
![](http://hi.csdn.net/attachment/201008/2/0_1280741951w3z7.gif)
         这里只应用了部分，还有其他比较多的测试数据，有兴趣就去[这里](http://jolbox.com/benchmarks.html) 瞧瞧吧。个人还是建议自己写点代码测试一下，那样感受更深啊。
### 在介绍怎么使用之前，先来看看BoneCP的特性（[Features](http://jolbox.com/features.html) ）：
- 高度可扩展,
 快速的连接池.    注：1）不用synchronized  关键字来处理多线程对资源的争用，而是使用 java.util.concurrent 包中的锁机制；2）首次使用分区机制来分开管理数据库连接；或许还有其他原因.
- Callback
 (hook interceptor) mechanisms on a change of connection state.
- 利用分区技术提高性能
- 允许直接访问一个连接或者语句
- 智能调整连接池大小
- SQL语句缓存支持
- 支持异步获取数据库连接
 (通过返回Future<Connection>的形式)
- 通过释放连接助理进程来释放数据库连接，提高性能.
- 通过initSQL参数在每次获取连接的时候执行SQL
- 支持数据库热切换
- 自动重试失败的数据库操作（当数据库或者网络挂掉的时候）
- JMX support
- 延迟初始化能力(Lazy
 initialization capable)
- 自动检测连接可用性
 (keep-alives 等)
- 允许直接通过数据源而不是通过驱动来获取一个新的数据库连接（Allow
 obtaining of new connections via a datasource rather than via a Driver）
- Datasource/Hibernate
 support capable
- Debug支持准确地高亮那些已经得到但是还没有关闭的链接(Debugging
 hooks to highlight the exact place where a connection was obtained but not closed)
- Debug支持展示那些被关闭两次的链接地址堆栈信息(Debugging
 support to show stack locations of connections that were closed twice. )
- 支持自定义连接池名称.
- 干净的代码结构，TestCase代码覆盖率达到100%
 (over 125 JUnit tests).
- 免费的，开源的而且都是用java干的，最重要的是有很完整的javadocs支持。(Free,
 open source and written in 100% pure Java with complete Javadocs).
       好了，废话说的挺多的，看看到底怎么用吧。
### 怎么使用BoneCP
我是Maven的忠实拥护者，如果你也是，那就简单了：
你可以在你的settings.xml文件里加上下面一段，声明一个Maven仓库，如果你不想影响其他项目，那也可以在你项目的pom.xml文件里面加。
- <repositories>
- <repository>
- <releases>
- <enabled>true</enabled>
- </releases>
- <id>bonecp-repo</id>
- <name>BoneCP Repository</name>
- <url>http://jolbox.com/bonecp/downloads/maven</url>
- </repository>
- </repositories>
![](http://static.blog.csdn.net/images/save_snippets.png)
然后你就可以像引用其他jar包一样引用bonecp了，下面应该很熟悉
- <dependency>
- <groupId>com.jolbox</groupId>
- <artifactId>bonecp</artifactId>
- <version>0.6.7.2</version>
- </dependency>
![](http://static.blog.csdn.net/images/save_snippets.png)
 如果你的项目直接用类似Hibernate的东东，没有配置dataSource的话，你很牛，那就加上下面这个吧
- <dependency>
- <groupId>com.jolbox</groupId>
- <artifactId>bonecp-provider</artifactId>
- <version>0.6.7.2</version>
- </dependency>
![](http://static.blog.csdn.net/images/save_snippets.png)
 如果这些都没有满足你,那参考[这里](http://jolbox.com/download.html) 吧。
如果你不用Spring，那应该是这样写吧：
- Class.forName("org.hsqldb.jdbcDriver");     // load the DB driver
- BoneCPConfig config = new BoneCPConfig();   // create a new configuration object
- config.setJdbcUrl("jdbc:hsqldb:mem:test");  // set the JDBC url
- config.setUsername("sa");           // set the username
- config.setPassword("");             // set the password
- config.setXXXX(...);                // (other config options here)
- BoneCP connectionPool = new BoneCP(config);     // setup the connection pool
- Connection connection;  
- connection = connectionPool.getConnection();    // fetch a connection
- ...  do something with the connection here ...  
- connection.close();             // close the connection
- connectionPool.shutdown();          // close the connection pool
![](http://static.blog.csdn.net/images/save_snippets.png)
### BoneCP-Spring详细配置
主要讲一下在Spring下面怎么配置，其实更简单，只要配置dataSrouce就可以搞定。如下，具体含义见注释。
- <beanid="dataSource"class="com.jolbox.bonecp.BoneCPDataSource"
- destroy-method="close">
- <!-- 数据库驱动 -->
- <propertyname="driverClass"value="${aliLibrary.db.driverClass}"/>
- <!-- 相应驱动的jdbcUrl,你懂的 -->
- <propertyname="jdbcUrl"value="${aliLibrary.db.jdbcUrl}"/>
- <!-- 数据库的用户名 -->
- <propertyname="username"value="${aliLibrary.db.username}"/>
- <!-- 数据库的密码 -->
- <propertyname="password"value="${aliLibrary.db.password}"/>
- <!-- 检查数据库连接池中空闲连接的间隔时间，单位是分，默认值：240，如果要取消则设置为0 -->
- <propertyname="idleConnectionTestPeriod"value="${aliLibrary.db.idleConnectionTestPeriod}"/>
- <!-- 连接池中未使用的链接最大存活时间，单位是分，默认值：60，如果要永远存活设置为0 -->
- <propertyname="idleMaxAge"value="${aliLibrary.db.idleMaxAge}"/>
- <!-- 每个分区最大的连接数 -->
- <propertyname="maxConnectionsPerPartition"value="${aliLibrary.db.maxConnectionsPerPartition}"/>
- <!-- 每个分区最小的连接数 -->
- <propertyname="minConnectionsPerPartition"value="${aliLibrary.db.minConnectionsPerPartition}"/>
- <!-- 分区数 ，默认值2，最小1，推荐3-4，视应用而定-->
- <propertyname="partitionCount"value="${aliLibrary.db.partitionCount}"/>
- <!-- 每次去拿数据库连接的时候一次性要拿几个,默认值：2 -->
- <propertyname="acquireIncrement"value="${aliLibrary.db.acquireIncrement}"/>
- <!-- 缓存prepared statements的大小，默认值：0 -->
- <propertyname="statementsCacheSize"value="${aliLibrary.db.statementsCacheSize}"/>
- <!-- 每个分区释放链接助理进程的数量，默认值：3，除非你的一个数据库连接的时间内做了很多工作，不然过多的助理进程会影响你的性能 -->
- <propertyname="releaseHelperThreads"value="${aliLibrary.db.releaseHelperThreads}"/>
- </bean>
![](http://static.blog.csdn.net/images/save_snippets.png)
 其他还有些不是很常用的包括：
- connectionTestStatement：在做keep-alive的时候的SQL语句。  
- statementsCachedPerConnection：No of statements that can be cached per connection，反正源码中不推荐使用，就别用了.  
- initSQL:在每次到数据库取连接的时候执行的SQL语句，只执行一次。  
- closeConnectionWatch：如果设置为true，则会增加一个线程监控关闭连接时的情况，如果关闭时出现异常，则打出错误日志，主要用于debug。上线后记得关掉。  
- logStatementsEnabled：如果设置为true，就会打印执行的SQL语句，如果你用了其他能打印SQL语句的框架，那就不必了。  
- acquireRetryDelay：在获取连接失败后，第二次参试前的延迟时间，默认为7000毫秒。  
- acquireRetryAttempts：在获取连接失败后的重试次数，默认为5次。  
- lazyInit：如果设置为true，那么连接池不会自动创建最小连接数的链接，而是保持为空，直到有需求要获取连接。  
- transactionRecoveryEnabled：如果设置为true，则会保存该链接上的所有活动，以备下次重试的时候使用，这里指的活动是数据库操作。  
- connectionHookClassName：Connection hook class name.没看懂…  
- poolName：上面特性中说到的自定义连接池名称。  
- disableJMX：控制JMX的支持开关。  
- connectionTimeout：获取连接的时候最大的等待时间，默认值为：Long.MAX_VALUE  
![](http://static.blog.csdn.net/images/save_snippets.png)
  Spring里面怎么用这个dataSource就不说了吧，今天重点在BoneCP。
### 最后跑一个TestCase，看看日志
初始化的时候打印出BoneCP的配置参数，很清楚，关闭的时候也有日志输出。
- 2010-08-02 15:10:22,457 [BoneCPDataSource.java:121] [com.jolbox.bonecp.BoneCPDataSource] DEBUG com.jolbox.bonecp.BoneCPDataSource :: JDBC URL = jdbc:mysql://10.249.128.109:3306/ali_library, Username = root, partitions = 3, max (per partition) = 3, min (per partition) = 3, helper threads = 3, idle max age = 240 min, idle test period = 30 min  
- 2010-08-02 15:10:22,457 [BoneCPDataSource.java:121] [com.jolbox.bonecp.BoneCPDataSource] DEBUG com.jolbox.bonecp.BoneCPDataSource :: JDBC URL = jdbc:mysql://10.249.128.109:3306/ali_library, Username = root, partitions = 3, max (per partition) = 3, min (per partition) = 3, helper threads = 3, idle max age = 240 min, idle test period = 30 min  
- 2010-08-02 15:10:45,958 [BoneCP.java:129] [com.jolbox.bonecp.BoneCP] INFO  com.jolbox.bonecp.BoneCP :: Shutting down connection pool...  
- 2010-08-02 15:10:45,958 [BoneCP.java:129] [com.jolbox.bonecp.BoneCP] INFO  com.jolbox.bonecp.BoneCP :: Shutting down connection pool...  
- 2010-08-02 15:10:45,973 [BoneCP.java:145] [com.jolbox.bonecp.BoneCP] INFO  com.jolbox.bonecp.BoneCP :: Connection pool has been shutdown.  
- 2010-08-02 15:10:45,973 [BoneCP.java:145] [com.jolbox.bonecp.BoneCP] INFO  com.jolbox.bonecp.BoneCP :: Connection pool has been shutdown.   
![](http://static.blog.csdn.net/images/save_snippets.png)
              接下去会写点代码横向对比一下C3p0与BoneCP的相关点。
