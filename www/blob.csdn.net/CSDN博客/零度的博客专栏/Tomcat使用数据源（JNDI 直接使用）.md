# Tomcat使用数据源（JNDI/直接使用） - 零度的博客专栏 - CSDN博客
2016年04月21日 15:33:54[零度anngle](https://me.csdn.net/zmx729618)阅读数：5341
Tomcat 在 7.0 以前的版本都是使用 [commons-dbcp](http://www.oschina.net/p/dbcp) 做为连接池的实现，但是
 dbcp 饱受诟病，原因有：- dbcp 是单线程的，为了保证线程安全会锁整个连接池
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
1、直接使用
```
package zmx.jndi.test;
import java.sql.Connection; 
import java.sql.ResultSet; 
import java.sql.Statement; 
import org.apache.tomcat.jdbc.pool.DataSource; 
import org.apache.tomcat.jdbc.pool.PoolProperties; 
public class SimplePOJOExample { 
     public static void main(String[] args) throws Exception { 
	     PoolProperties p = new PoolProperties(); 
	     p.setUrl("jdbc:mysql://localhost:3306/testweb"); 
	     p.setDriverClassName("com.mysql.jdbc.Driver"); 
	     p.setUsername("root"); 
	     p.setPassword("0729"); 
	     p.setJmxEnabled(true); 
	     p.setTestWhileIdle(false); 
	     p.setTestOnBorrow(true); 
	     p.setValidationQuery("SELECT 1"); 
	     p.setTestOnReturn(false); 
	     p.setValidationInterval(30000); 
	     p.setTimeBetweenEvictionRunsMillis(30000); 
	     p.setMaxActive(100); 
	     p.setInitialSize(10); 
	     p.setMaxWait(10000); 
	     p.setRemoveAbandonedTimeout(60); 
	     p.setMinEvictableIdleTimeMillis(30000); 
	     p.setMinIdle(10); 
	     p.setLogAbandoned(true); 
	     p.setRemoveAbandoned(true); 
	     p.setJdbcInterceptors("org.apache.tomcat.jdbc.pool.interceptor.ConnectionState;"+ 
	       "org.apache.tomcat.jdbc.pool.interceptor.StatementFinalizer"); 
	     DataSource datasource = new DataSource(); 
	     datasource.setPoolProperties(p); 
	     Connection con = null; 
	     try { 
	       con = datasource.getConnection(); 
	       Statement st = con.createStatement(); 
	       ResultSet rs = st.executeQuery("select * from test"); 
	       int cnt = 1; 
	       while (rs.next()) { 
	           System.out.println((cnt++)+". Host:" +rs.getString("name")); 
	       } 
	       rs.close(); 
	       st.close(); 
	
	     } finally { 
	
	       if (con!=null){
	    	   try {
	     	      con.close();
	     	   }catch (Exception ignore) {
	     		   
	     	   }
	    	   
	       } 
	            
	     }
     } 
     
     
     
}
```
2、使用JNDI配置数据源
2.1、JNDI配置
现在来讲一下如何到服务器上对 Data Source 进行配置？服务器： Tomcat 7 数据库：MySQL，在Tomcat服务器上conf/context.xml中的<Context></Context>标签中配置Resource
```
Resource name="jdbc/testweb" 
       auth="Container" 
       type="javax.sql.DataSource" 
       factory="org.apache.tomcat.jdbc.pool.DataSourceFactory" 
       testWhileIdle="true" 
       testOnBorrow="true" 
       testOnReturn="false" 
       validationQuery="SELECT 1" 
       validationInterval="30000" 
       timeBetweenEvictionRunsMillis="30000" 
       maxActive="100" 
       minIdle="10" 
       maxWait="10000" 
       initialSize="10" 
       removeAbandonedTimeout="60" 
       removeAbandoned="true" 
       logAbandoned="true" 
       minEvictableIdleTimeMillis="30000" 
       jmxEnabled="true" 
       jdbcInterceptors="org.apache.tomcat.jdbc.pool.interceptor.ConnectionState;
	                     org.apache.tomcat.jdbc.pool.interceptor.StatementFinalizer" 
       username="root" 
       password="0729" 
       driverClassName="com.mysql.jdbc.Driver" 
       url="jdbc:mysql://localhost:3306/testweb"/>
```
         2.2 
修改web.xml，添加如下配置
```
<resource-ref>
      <description>DB Connection</description>
      <res-ref-name>jdbc/testweb</res-ref-name>
      <res-type>javax.sql.DataSource</res-type>
      <res-auth>Container</res-auth>
  </resource-ref>
```
2.3 添加jar包 
JDBC驱动程序mysql-connector-java-5-bin.jar放置在%TOMCAT_HOME%\lib和应用的WEB-INF\lib下
       2.4 代码中使用
```
package org.nercita.test.servlet;
import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.concurrent.Future;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.tomcat.jdbc.pool.DataSource;
public class JNDIServlet extends HttpServlet {
	
	private static final long serialVersionUID = -4063005588610103364L;
	
	final String JNDINAME = "java:comp/env/jdbc/testweb" ;
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		
		
        try {
			// 初始化查找命名空间
			Context ctx = new InitialContext() ;
			// 找到DataSource
			DataSource datasource = (DataSource)ctx.lookup(JNDINAME) ;
            Connection conn = null;  
            //异步获取数据库连接
            Future<Connection> future = datasource.getConnectionAsync();  
		    while (!future.isDone()) { 
		       System.out.println("Connection is not yet available. Do some background work"); 
		       try { 
		          Thread.sleep(100); //simulate work 
		       }catch (InterruptedException x) { 
		           Thread.currentThread().interrupted(); 
		
		      } 
		
		   } 
		   conn = future.get(); //should return instantly 
		   Statement st = conn.createStatement(); 
		   ResultSet rs = st.executeQuery("select * from test"); 
		   int cnt = 1; 
	       while (rs.next()) { 
	           System.out.println((cnt++)+". Host:" +rs.getString("name")); 
	       } 
	       rs.close(); 
	       st.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	
	}
}
```

