# 使用 Tomcat 7 新的连接池 —— Tomcat jdbc pool - z69183787的专栏 - CSDN博客
2014年05月08日 16:30:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1786
Tomcat 在 7.0 以前的版本都是使用 [commons-dbcp](http://www.oschina.net/p/dbcp) 做为连接池的实现，但是 dbcp 饱受诟病，原因有：
- dbcp 是单线程的，为了保证线程安全会锁整个连接池
- dbcp 性能不佳
- dbcp 太复杂，超过 60 个类
- dbcp 使用静态接口，在 JDK 1.6 编译有问题
- dbcp 发展滞后
因此很多人会选择一些第三方的连接池组件，例如 [c3p0](http://www.oschina.net/p/c3p0) , [bonecp](http://www.oschina.net/p/bonecp), [druid](http://www.oschina.net/p/druid) ([@wenshao](http://my.oschina.net/u/127152) )
 等。
为此，Tomcat 从 7.0 开始引入一个新的模块：Tomcat jdbc pool
- tomcat jdbc pool 近乎兼容 dbcp ，性能更高
- 异步方式获取连接
- tomcat jdbc pool 是 tomcat 的一个模块，基于 tomcat JULI，使用 Tomcat 的日志框架
- 使用 javax.sql.PooledConnection 接口获取连接
- 支持高并发应用环境
- 超简单，核心文件只有8个，比 c3p0 还
- 更好的空闲连接处理机制
- 支持 JMX
- 支持 XA Connection
tomcat jdbc pool 的优点远不止这些，详情请看[这里](https://tomcat.apache.org/tomcat-7.0-doc/jdbc-pool.html)。
tomcat jdbc pool 可在 Tomcat 中直接使用，也可以在独立的应用中使用。
Tomcat 中直接使用的方法：
数据源配置：
|`01`|`<``Resource``name``=``"jdbc/TestDB"`|
|`02`|`      ``auth``=``"Container"`|
|`03`|`      ``type``=``"javax.sql.DataSource"`|
|`04`|`      ``factory``=``"org.apache.tomcat.jdbc.pool.DataSourceFactory"`|
|`05`|`      ``testWhileIdle``=``"true"`|
|`06`|`      ``testOnBorrow``=``"true"`|
|`07`|`      ``testOnReturn``=``"false"`|
|`08`|`      ``validationQuery``=````"SELECT 1"```|
|`09`|`      ``validationInterval``=``"30000"`|
|`10`|`      ``timeBetweenEvictionRunsMillis``=``"30000"`|
|`11`|`      ``maxActive``=``"100"`|
|`12`|`      ``minIdle``=``"10"`|
|`13`|`      ``maxWait``=``"10000"`|
|`14`|`      ``initialSize``=``"10"`|
|`15`|`      ``removeAbandonedTimeout``=``"60"`|
|`16`|`      ``removeAbandoned``=``"true"`|
|`17`|`      ``logAbandoned``=``"true"`|
|`18`|`      ``minEvictableIdleTimeMillis``=``"30000"`|
|`19`|`      ``jmxEnabled``=``"true"`|
|`20`|`      ``jdbcInterceptors``=`|
|`21`|`"org.apache.tomcat.jdbc.pool.interceptor.ConnectionState;org.apache.tomcat.jdbc.pool.interceptor.StatementFinalizer"`|
|`22`|`      ``username``=``"root"`|
|`23`|`      ``password``=``"password"`|
|`24`|`      ``driverClassName``=``"com.mysql.jdbc.Driver"`|
|`25`|`      ``url``=``"jdbc:mysql://localhost:3306/mysql"``/>`|
异步获取连接的方法：
|`01`|```Connection con =````null``;`|
|`02`|`try``{`|
|`03`|`  ````Future<Connection> future = datasource.getConnectionAsync();```|
|`04`|`  ``while````(!future.isDone()) {```|
|`05`|`      ``System.out.println(````"Connection is not yet available. Do some background work"````);`|
|`06`|`      ``try``{`|
|`07`|`      ``Thread.sleep(``100``); ````//simulate work```|
|`08`|`      ``}``catch````(InterruptedException x) {```|
|`09`|`      ``Thread.currentThread().interrupted();`|
|`10`|`      ``}`|
|`11`|`  ``}`|
|`12`|`  ````con = future.get();``````//should return instantly```|
|`13`|`  ````Statement st = con.createStatement();```|
|`14`|`  ````ResultSet rs = st.executeQuery(``````"select * from user"````);`|
在独立的应用中使用：
|`01`|`import``java.sql.Connection;`|
|`02`|`import``java.sql.ResultSet;`|
|`03`|`import``java.sql.Statement;`|
|`04`||
|`05`|`import``org.apache.tomcat.jdbc.pool.DataSource;`|
|`06`|`import``org.apache.tomcat.jdbc.pool.PoolProperties;`|
|`07`||
|`08`|`public``class````SimplePOJOExample {```|
|`09`||
|`10`|`    ``public``static``void````main(String[] args)````throws````Exception {```|
|`11`|`    ````PoolProperties p =````new``PoolProperties();`|
|`12`|`    ``p.setUrl(``"jdbc:mysql://localhost:3306/mysql"``);`|
|`13`|`    ``p.setDriverClassName(``"com.mysql.jdbc.Driver"``);`|
|`14`|`    ``p.setUsername(``"root"``);`|
|`15`|`    ``p.setPassword(``"password"``);`|
|`16`|`    ``p.setJmxEnabled(``true``);`|
|`17`|`    ``p.setTestWhileIdle(``false``);`|
|`18`|`    ``p.setTestOnBorrow(``true``);`|
|`19`|`    ``p.setValidationQuery(````"SELECT 1"````);`|
|`20`|`    ``p.setTestOnReturn(``false``);`|
|`21`|`    ``p.setValidationInterval(``30000``);`|
|`22`|`    ``p.setTimeBetweenEvictionRunsMillis(``30000``);`|
|`23`|`    ``p.setMaxActive(``100``);`|
|`24`|`    ``p.setInitialSize(``10``);`|
|`25`|`    ``p.setMaxWait(``10000``);`|
|`26`|`    ``p.setRemoveAbandonedTimeout(``60``);`|
|`27`|`    ``p.setMinEvictableIdleTimeMillis(``30000``);`|
|`28`|`    ``p.setMinIdle(``10``);`|
|`29`|`    ``p.setLogAbandoned(``true``);`|
|`30`|`    ``p.setRemoveAbandoned(``true``);`|
|`31`|`    ``p.setJdbcInterceptors(``"org.apache.tomcat.jdbc.pool.interceptor.ConnectionState;"``+`|
|`32`|`      ``"org.apache.tomcat.jdbc.pool.interceptor.StatementFinalizer"``);`|
|`33`|`    ````DataSource datasource =````new``DataSource();`|
|`34`|`    ``datasource.setPoolProperties(p);`|
|`35`||
|`36`|`    ````Connection con =````null``;`|
|`37`|`    ``try``{`|
|`38`|`      ````con = datasource.getConnection();```|
|`39`|`      ````Statement st = con.createStatement();```|
|`40`|`      ````ResultSet rs = st.executeQuery(``````"select * from user"````);`|
|`41`|`      ``int````cnt =````1``;`|
|`42`|`      ``while````(rs.next()) {```|
|`43`|`          ``System.out.println((cnt++)+````". Host:"````+rs.getString(``"Host"``)+`|
|`44`|`        ````" User:"````+rs.getString(``"User"``)+````" Password:"````+rs.getString(``"Password"``));`|
|`45`|`      ``}`|
|`46`|`      ``rs.close();`|
|`47`|`      ``st.close();`|
|`48`|`    ``} ``finally``{`|
|`49`|`      ``if``(con!=``null``) ``try``{con.close();}``catch````(Exception ignore) {}```|
|`50`|`    ``}`|
|`51`|`    ``}`|
|`52`||
|`53`|`}`|
以上只是简单的介绍，具体的使用方法还需参考：
[https://tomcat.apache.org/tomcat-7.0-doc/jdbc-pool.html](https://tomcat.apache.org/tomcat-7.0-doc/jdbc-pool.html)
