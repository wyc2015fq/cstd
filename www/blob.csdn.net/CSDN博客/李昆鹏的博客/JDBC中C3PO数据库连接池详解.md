
# JDBC中C3PO数据库连接池详解 - 李昆鹏的博客 - CSDN博客


2018年06月24日 23:53:17[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：390


-----------------------------------------------------JDBC中C3PO数据库连接池详解------------------------------------------------
### C3P0简介
C3P0也是开源免费的连接池！C3P0被很多人看好！

### C3P0的使用
C3P0中池类是：ComboPooledDataSource。
|public|static|void|main(String[] args) {
|try|{
|Class.|forName|(|"oracle.jdbc.OracleDriver"|);
|DataSource|unpoolds|= DataSources.|unpooledDataSource|(|"jdbc:oracle:thin:@127.0.0.1:1521:orcl"|,
|"scott"|,|"tiger"|);
|DataSource poolds = DataSources.|pooledDataSource|(|unpoolds|);
|Connection conn =  poolds.getConnection();
|System.|out|.println(conn);
|}|catch|(ClassNotFoundException e) {
|//|TODO|Auto-generated catch block
|e.printStackTrace();
|}|catch|(SQLException e) {
|//|TODO|Auto-generated catch block
|e.printStackTrace();
|}
|}
|
c3p0也可以指定配置文件，而且配置文件可以是properties，也可骒xml的。当然xml的高级一些了。但是c3p0的配置文件名必须为c3p0-config.xml，并且必须放在类路径下。

|1.|<!--|连接池中保留的最大连接数。默认值|: 15 -->
|2.|<property|name|=|"maxPoolSize"|value|=|"20"|/>
|3.|<!--|连接池中保留的最小连接数，默认为：|3-->
|4.|<property|name|=|"minPoolSize"|value|=|"2"|/>
|5.|<!--|初始化连接池中的连接数，取值应在|minPoolSize|与|maxPoolSize|之间，默认为|3-->
|6.|<property|name|=|"initialPoolSize"|value|=|"2"|/>
|7.
|8.|<!--|最大空闲时间，|60|秒内未使用则连接被丢弃。若为|0|则永不丢弃。默认值|: 0 -->
|9.|<property|name|=|"maxIdleTime"|>|60|</property>
|10.
|11.|<!--|当连接池连接耗尽时，客户端调用|getConnection()|后等待获取新连接的时间，超时后将抛出|SQLException|，如设为|0|则无限期等待。单位毫秒。默认|: 0 -->
|12.|<property|name|=|"checkoutTimeout"|value|=|"3000"|/>
|13.
|14.|<!--|当连接池中的连接耗尽的时候|c3p0|一次同时获取的连接数。默认值|: 3 -->
|15.|<property|name|=|"acquireIncrement"|value|=|"2"|/>
|16.
|17.|<!--|定义在从数据库获取新连接失败后重复尝试的次数。默认值|: 30|；小于等于|0|表示无限次|-->
|18.|<property|name|=|"acquireRetryAttempts"|value|=|"0"|/>
|19.
|20.|<!--|重新尝试的时间间隔，默认为：|1000|毫秒|-->
|21.|<property|name|=|"acquireRetryDelay"|value|=|"1000"|/>
|22.
|23.|<!--|关闭连接时，是否提交未提交的事务，默认为|false|，即关闭连接，回滚未提交的事务|-->
|24.|<property|name|=|"autoCommitOnClose"|>|false|</property>
|25.
|26.|<!--c3p0|将建一张名为|Test|的空表，并使用其自带的查询语句进行测试。如果定义了这个参数那么属性|preferredTestQuery|将被忽略。你不能在这张|Test|表上进行任何操作，它将只供|c3p0|测试使用。默认值|: null -->
|27.|<property|name|=|"automaticTestTable"|>|Test|</property>
|28.
|29.|<!--|如果为|false|，则获取连接失败将会引起所有等待连接池来获取连接的线程抛出异常，但是数据源仍有效保留，并在下次调用|getConnection()|的时候继续尝试获取连接。如果设为|true|，那么在尝试获取连接失败后该数据源将申明已断开并永久关闭。默认|: false-->
|30.|<property|name|=|"breakAfterAcquireFailure"|>|false|</property>
|31.
|32.|<!--|每|60|秒检查所有连接池中的空闲连接。默认值|: 0|，不检查|-->
|33.|<property|name|=|"idleConnectionTestPeriod"|>|60|</property>
|34.|<!--c3p0|全局的|PreparedStatements|缓存的大小。如果|maxStatements|与|maxStatementsPerConnection|均为|0|，则缓存不生效，只要有一个不为|0|，则语句的缓存就能生效。如果默认值|: 0-->
|35.|<property|name|=|"maxStatements"|>|100|</property>
|36.|<!--maxStatementsPerConnection|定义了连接池内单个连接所拥有的最大缓存|statements|数。默认值|: 0 -->
|37.|<property|name|=|"maxStatementsPerConnection"|></property>
|
代码示例：
**第一步引入驱动包**
![](https://img-blog.csdn.net/20180624235232684?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**创建c3p0配置文件，文件名为****c3p0-config.xml**
|<c3p0-config>
|<default-config>
|<property  name="automaticTestTable">con_test</property>
|<property  name="checkoutTimeout">30000</property>
|<property  name="idleConnectionTestPeriod">30</property>
|<property name="initialPoolSize">10</property>
|<property name="maxIdleTime">30</property>
|<property name="maxPoolSize">100</property>
|<property name="minPoolSize">10</property>
|<property name="maxStatements">200</property>
|<user-overrides user="test-user">
|<property name="maxPoolSize">10</property>
|<property name="minPoolSize">1</property>
|<property name="maxStatements">0</property>
|</user-overrides>
|</default-config>
|<!-- This app is massive! -->
|<named-config name="intergalactoApp">
|<property name="acquireIncrement">50</property>
|<property name="initialPoolSize">100</property>
|<property name="minPoolSize">50</property>
|<property name="maxPoolSize">1000</property>
|<!-- intergalactoApp adopts a different approach to configuring  statement caching -->
|<property  name="maxStatements">0</property>
|<property  name="maxStatementsPerConnection">5</property>
|<!-- he's important, but there's only one of him -->
|<user-overrides user="master-of-the-universe">
|<property name="acquireIncrement">1</property>
|<property name="initialPoolSize">1</property>
|<property name="minPoolSize">1</property>
|<property name="maxPoolSize">5</property>
|<property  name="maxStatementsPerConnection">50</property>
|</user-overrides>
|</named-config>
|</c3p0-config>
|
创建测试类
|package com.rl.c3p0;
|import java.sql.Connection;
|import java.sql.PreparedStatement;
|import java.sql.ResultSet;
|import java.sql.SQLException;
|import java.util.Properties;
|import javax.sql.DataSource;
|import com.mchange.v2.c3p0.DataSources;
|public class JDBCPool {
|public  static void main(String[] args) {
|PreparedStatement  ps = null;
|ResultSet  rs = null;
|try  {
|//注册驱动
|Class.forName("oracle.jdbc.OracleDriver");
|//获得非池化的数据源
|DataSource  unpoolds =  DataSources.unpooledDataSource("jdbc:oracle:thin:@127.0.0.1:1521:orcl",  "scott", "tiger");
|//将非池化的数据源转换成池化的数据源
|DataSource  poolds = DataSources.pooledDataSource(unpoolds);
|//获得连接
|Connection  conn = poolds.getConnection();
|System.out.println(conn);
|String  sql = "select * from person";
|ps = conn.prepareStatement(sql);
|rs = ps.executeQuery();
|while(rs.next()){
|System.out.println("ID"+rs.getInt(1)+"  name" +rs.getString(2)+"  address" +rs.getString(3)+"  birthday"+rs.getDate(4));
|}
|}  catch (ClassNotFoundException e) {
|e.printStackTrace();
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally{
|try  {
|if(rs  != null){
|rs.close();
|}
|if(ps  != null){
|ps.close();
|}
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|}
|}
|


