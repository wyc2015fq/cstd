# 关于DBCP及使用。纯JDBC架构。 - z69183787的专栏 - CSDN博客
2016年12月13日 16:55:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1269
DBCP(DataBase connection pool),[数据库连接池](http://baike.baidu.com/view/84055.htm)。
是 apache 上的一个 [Java](http://lib.csdn.net/base/javase) 连接池项目，也是 tomcat 使用的连接池组件。单独使用dbcp需要3个包：common-dbcp.jar,common-pool.jar,common-collections.jar由于建立[数据库](http://lib.csdn.net/base/mysql)连接是一个非常耗时耗资源的行为，所以通过连接池预先同数据库建立一些连接，放在内存中，应用程序需要建立数据库连接时直接到连接池中申请一个就行，用完后再放回去。
目前[hibernate](http://lib.csdn.net/base/javaee)默认的连接方式就是DBCP，可见其重要性。不过话说hibernate只是一个框架，效率低多了，基础的才是笑到最后的。使用DBCP的连接池可以快速取存连接。进行一系列操作。
不说了，土豆我就直接上代码了。纯JDBC效率肯定比hibernate高哦。说不定哪天hibernate没落了。它只是工具，底层才是王道。
```java
package com.sinoglobal.db;  
  
import java.sql.Connection;  
import java.sql.DriverManager;  
import java.sql.ResultSet;  
import java.sql.SQLException;  
import java.sql.Statement;  
import java.util.Locale;  
import java.util.ResourceBundle;  
  
import org.apache.commons.dbcp.ConnectionFactory;  
import org.apache.commons.dbcp.DriverManagerConnectionFactory;  
import org.apache.commons.dbcp.PoolableConnectionFactory;  
import org.apache.commons.dbcp.PoolingDriver;  
import org.apache.commons.pool.ObjectPool;  
import org.apache.commons.pool.impl.GenericObjectPool;  
  
@SuppressWarnings("unchecked")  
public class PoolManager {  
    private static String driver = "net.sourceforge.jtds.jdbc.Driver"; // 驱动  
    private static String url = ""; // URL  
    private static String name = "sa"; // 用户名  
    private static String password = ""; // 密码  
    private static Class driverClass = null;  
    private static ObjectPool connectionPool = null;  
    private static String poolname = "";  
    private static ResourceBundle rb;  
  
    /** 
     * 初始化数据源 
     */  
    private static synchronized void initDataSource() {  
        if (driverClass == null) {  
            try {  
                driverClass = Class.forName(driver);  
            } catch (ClassNotFoundException e) {  
                e.printStackTrace();  
            }  
        }  
    }  
  
    /** 
     * 装配配置文件 initProperties 
     */  
    private static void loadProperties() {  
        rb = ResourceBundle.getBundle("config", Locale.getDefault());  
        driver = rb.getString("jdbc.sql.driverClassName");  
        url = rb.getString("jdbc.sql.url");  
        name = rb.getString("jdbc.sql.username");  
        password = rb.getString("jdbc.sql.password");  
        poolname = rb.getString("jdbc.sql.poolname");  
    }  
  
    /** 
     * 连接池启动 
     *  
     * @throws Exception 
     */  
    public static void StartPool() {  
        loadProperties();  
        initDataSource();  
        if (connectionPool != null) {  
            ShutdownPool();  
        }  
        try {  
            connectionPool = new GenericObjectPool(null);  
            ConnectionFactory connectionFactory = new DriverManagerConnectionFactory(  
                    url, name, password);  
            new PoolableConnectionFactory(connectionFactory, connectionPool,  
                    null, null, false, true);  
            Class.forName("org.apache.commons.dbcp.PoolingDriver");  
            PoolingDriver driver = (PoolingDriver) DriverManager  
                    .getDriver("jdbc:apache:commons:dbcp:");  
            driver.registerPool(poolname, connectionPool);  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }  
  
    /** 
     * 释放连接池 
     */  
    public static void ShutdownPool() {  
        try {  
            PoolingDriver driver = (PoolingDriver) DriverManager  
                    .getDriver("jdbc:apache:commons:dbcp:");  
            driver.closePool(poolname);  
        } catch (SQLException e) {  
            e.printStackTrace();  
        }  
    }  
  
    /** 
     * 取得连接池中的连接 
     *  
     * @return 
     */  
    public static Connection getConnection() {  
        Connection conn = null;  
        if (connectionPool == null)  
            StartPool();  
        try {  
            conn = DriverManager.getConnection("jdbc:apache:commons:dbcp:"  
                    + poolname);  
        } catch (SQLException e) {  
            e.printStackTrace();  
        }  
        return conn;  
    }  
  
    /** 
     * 获取连接 getConnection 
     *  
     * @param name 
     * @return 
     */  
    public static Connection getConnection(String name) {  
        return getConnection();  
    }  
  
    /** 
     * 释放连接 freeConnection 
     *  
     * @param conn 
     */  
    public static void freeConnection(Connection conn) {  
        if (conn != null) {  
            try {  
                conn.close();  
            } catch (SQLException e) {  
                e.printStackTrace();  
            }  
        }  
    }  
  
    /** 
     * 释放连接 freeConnection 
     *  
     * @param name 
     * @param con 
     */  
    public static void freeConnection(String name, Connection con) {  
        freeConnection(con);  
    }  
  
    /** 
     * 例子 main 
     *  
     * @param args 
     */  
    public static void main(String[] args) {  
        try {  
            Connection conn = PoolManager.getConnection();  
            System.out.println(conn.isClosed());  
            if (conn != null) {  
                Statement statement = conn.createStatement();  
                ResultSet rs = statement  
                        .executeQuery("select * from test2..log");  
                int c = rs.getMetaData().getColumnCount();  
                while (rs.next()) {  
                    System.out.println();  
                    for (int i = 1; i <= c; i++) {  
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
  
}
```
