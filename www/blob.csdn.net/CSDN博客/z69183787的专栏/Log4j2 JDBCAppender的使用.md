# Log4j2 JDBCAppender的使用 - z69183787的专栏 - CSDN博客
2016年10月25日 17:59:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3402
## 一.缘由最近打算重新整理一下项目的日志系统,将系统日志和业务日志区分开,然后采用文件或是数据库来存储日志,于是上网查了一下log4j的相关配置,并将系统日志写入了数据库,但是发生了一个bug:当写入的信息里存在单引号时,数据库就会报错ORA-00917: 缺失逗号查了一些资料,发现log4j已经很古老了,它对SQL基本没有做任何处理就执行存储操作,这样SQL里可能存在的一些特殊字符并没有经过处理导致上面的错误.找了好久都没有找到解决办法,所以痛定思痛告别log4j而使用log4j2二.使用基本的使用就不累述了,这里有Apache Log4j2官方文档,给firebug装个有道翻译插件,或者直接阅读也没什么难度,这篇博客主要讲述如何配置JDBCAppender(其实文档里也有)1.web.xml配置注意,log4j2.xml这个名字是默认的,放在resource目录下的,如果你是采用默认的,那么web.xml里就不需要配置. 如果需要自定义路径记得在web.xml里如下配置
```
<span style="font-size:14px;"><context-param>
    <param-name>log4j2ConfigLocation</param-name>
    <param-value>classpath:config/log4j2.xml</param-value>
</context-param></span>
```
2.log4j2.xml的配置
这里仅仅配置了两个输出方式,Console和JDBC也就是控制台和数据库,其中com.amayadream.webchat.listener.PoolManager是一个连接池管理类,getConnection方法是获取Connection对象,代码会在下面贴出.
数据库名为syslog,一共六个字段分别为id,class,function,message,leavl,time,类型都是varchar2,id为主键,默认值为sys_guid()
配置中:tableName是数据库中日志表的表名,Column是数据库的字段,pattern是字段的值,因为我用的是oracle数据库,id用的是sys_guid(),所以在这里省略以实现不重复的主键效果.
```
<span style="font-size:14px;"><?xml version="1.0" encoding="UTF-8"?>
<Configuration status="INFO" monitorInterval="1800">
    <appenders>
        <Console name="consolePrint" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
        <JDBC name="databaseAppender" tableName="SYSLOG">
            <ConnectionFactory class="com.amayadream.webchat.listener.PoolManager" method="getConnection" />
            <!--<Column name="ID" pattern=""/>-->
            <Column name="CLASS" pattern="%C" />
            <Column name="FUNCTION" pattern="%M" />
            <Column name="MESSAGE" pattern="%m" />
            <Column name="LEAVL" pattern="%level" />
            <Column name="TIME" pattern="%d{yyyy-MM-dd HH:mm:ss.SSS}" />
        </JDBC>
    </appenders>
    <loggers>
        <root level="info">
            <appender-ref ref="consolePrint" />
            <AppenderRef ref="databaseAppender" level="INFO" />
        </root>
    </loggers>
</Configuration></span>
```
3.Connection获取
PoolManager.java
```java
<span style="font-size:14px;">package com.amayadream.webchat.listener;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.sql.*;
import java.util.Properties;
import org.apache.commons.dbcp.ConnectionFactory;
import org.apache.commons.dbcp.DriverManagerConnectionFactory;
import org.apache.commons.dbcp.PoolableConnectionFactory;
import org.apache.commons.dbcp.PoolingDriver;
import org.apache.commons.pool.ObjectPool;
import org.apache.commons.pool.impl.GenericObjectPool;
import org.junit.Test;
/**
 * @author :  Amayadream
 * @date :  2016.04.06 20:17
 */
public class PoolManager {
    private static String driver = "oracle.jdbc.driver.OracleDriver";
    private static String url = "jdbc:oracle:thin:@localhost:1521:XE";
    private static String Name = "amayadream";
    private static String Password = "123456";
    private static Class driverClass = null;
    private static ObjectPool connectionPool = null;
    public PoolManager() {
    }
    /**
     * 装配配置文件
     */
    private static void loadProperties(){
        try {
            InputStream stream = PoolManager.class.getClassLoader().getResourceAsStream("jdbc.properties");
            Properties props = new Properties();
            props.load(stream);
            driver = props.getProperty("driver");
            url = props.getProperty("url");
            Name = props.getProperty("username");
            Password = props.getProperty("password");
        } catch (FileNotFoundException e) {
            System.out.println("读取配置文件异常");
        } catch(IOException ie){
            System.out.println("读取配置文件时IO异常");
        }
    }
    /**
     * 初始化数据源
     */
    private static synchronized void initDataSource(){
        if (driverClass == null) {
            try{
                driverClass = Class.forName(driver);
            }catch (ClassNotFoundException e){
                e.printStackTrace();
            }
        }
    }
    /**
     * 连接池启动
     */
    public static void startPool(){
        loadProperties();
        initDataSource();
        if (connectionPool != null) {
            destroyPool();
        }
        try {
            connectionPool = new GenericObjectPool(null);
            ConnectionFactory connectionFactory = new DriverManagerConnectionFactory(url, Name, Password);
            PoolableConnectionFactory poolableConnectionFactory = new PoolableConnectionFactory(connectionFactory, connectionPool, null, null, false, true);
            Class.forName("org.apache.commons.dbcp.PoolingDriver");
            PoolingDriver driver = (PoolingDriver) DriverManager.getDriver("jdbc:apache:commons:dbcp:");
            driver.registerPool("dbpool", connectionPool);
            System.out.println("装配连接池OK");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    public static void destroyPool(){
        try {
            PoolingDriver driver = (PoolingDriver) DriverManager.getDriver("jdbc:apache:commons:dbcp:");
            driver.closePool("dbpool");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    /**
     * 取得连接池中的连接
     * @return
     */
    public static Connection getConnection() {
        Connection conn = null;
        if(connectionPool == null)
            startPool();
        try {
            conn = DriverManager.getConnection("jdbc:apache:commons:dbcp:dbpool");
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return conn;
    }
    /**
     * 获取连接
     * getConnection
     * @param name
     * @return
     */
    public static Connection getConnection(String name){
        return getConnection();
    }
    /**
     * 释放连接
     * freeConnection
     * @param conn
     */
    public static void freeConnection(Connection conn){
        if(conn != null){
            try {
                conn.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }
    /**
     * 释放连接
     * freeConnection
     * @param name
     * @param con
     */
    public static void freeConnection (String name,Connection con){
        freeConnection(con);
    }
    @Test
    public void test(){
        try {
            Connection conn = PoolManager.getConnection();
            if(conn != null){
                Statement statement = conn.createStatement();
                ResultSet rs = statement.executeQuery("select * from syslog");
                int c = rs.getMetaData().getColumnCount();
                while(rs.next()){
                    System.out.println();
                    for(int i=1;i<=c;i++){
                        System.out.print(rs.getObject(i));
                    }
                }
                rs.close();
            }
            PoolManager.freeConnection(conn);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}</span>
```
4.jdbc.properties
这里没什么要说的,主要是PoolManager中getProperties()中的key要和这里对应
```
<span style="font-size:14px;">driver=oracle.jdbc.driver.OracleDriver
url=jdbc:oracle:thin:@localhost:1521:XE
username=amayadream
password=123456
#定义初始连接数
initialSize=0
#定义最大连接数
maxActive=20
#定义最大空闲
maxIdle=20
#定义最小空闲
minIdle=1
#定义最长等待时间
maxWait=60000</span>
```
