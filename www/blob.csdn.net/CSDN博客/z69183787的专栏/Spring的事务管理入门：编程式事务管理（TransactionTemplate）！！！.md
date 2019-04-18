# Spring的事务管理入门：编程式事务管理（TransactionTemplate）！！！ - z69183787的专栏 - CSDN博客
2012年11月19日 14:33:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1512
个人分类：[Spring-事务处理](https://blog.csdn.net/z69183787/article/category/7068794)

Spring可以支持编程式事务和声明式事务。
Spring使用事务管理器，每个不同平台的事务管理器都实现了接口：PlatformTransactionManager
此接口是事务管理的核心，提供了三个需要实现的函数：
**[java]**[view
 plain](http://blog.csdn.net/zq9017197/article/details/6321391#)[copy](http://blog.csdn.net/zq9017197/article/details/6321391#)[print](http://blog.csdn.net/zq9017197/article/details/6321391#)[?](http://blog.csdn.net/zq9017197/article/details/6321391#)
- commit(TransactionStatus status) ;     
- getTransaction(TransactionDefinition definition) ;     
- rollback(TransactionStatus status) ;  
如果我们使用的是JDBC来处理事务，那么这个事务管理器就是DataSourceTransactionManager。
通过Spring文档查找到这个类，发现其需要DataSource这个类。也就是只要实现了javax.sql.DataSource这个接口的类，都可以作为参数传入到DataSourceTransactionManager。
 然后，找到 包org.springframework.transaction.support中的 TransactionTemplate。
发现TransactionTemplate中有一个重要的方法：
**[java]**[view
 plain](http://blog.csdn.net/zq9017197/article/details/6321391#)[copy](http://blog.csdn.net/zq9017197/article/details/6321391#)[print](http://blog.csdn.net/zq9017197/article/details/6321391#)[?](http://blog.csdn.net/zq9017197/article/details/6321391#)
- execute(TransactionCallback action) ;  
就是利用这个方法，我们可以在这个方法中添加事务。
这个方法需要传入参数 TransactionCallback。
TransactionCallback，顾名思义，就是事务回调然后查到TransactionCallback。
发现这是一个接口（这也必须是接口，因为任务都是自己具体定义的）
里面只有一个方法：
**[java]**[view
 plain](http://blog.csdn.net/zq9017197/article/details/6321391#)[copy](http://blog.csdn.net/zq9017197/article/details/6321391#)[print](http://blog.csdn.net/zq9017197/article/details/6321391#)[?](http://blog.csdn.net/zq9017197/article/details/6321391#)
- doInTransaction(TransactionStatus status) ;   
很明显，就是在一个事务中需要做的事情都包括这这个方法中了。
而这个doInTransaction 又传入了一个参数，这次是 TransactionStatus，继续顾名思义，也就是事务状态。
查询下去，这个 TransactionStatus 还是一个接口。 看看这个接口定义了哪些服务（方法）：
**[java]**[view
 plain](http://blog.csdn.net/zq9017197/article/details/6321391#)[copy](http://blog.csdn.net/zq9017197/article/details/6321391#)[print](http://blog.csdn.net/zq9017197/article/details/6321391#)[?](http://blog.csdn.net/zq9017197/article/details/6321391#)
- hasSavepoint() ;  
- isCompleted() ;    
- isNewTransaction() ;  
- setRollbackOnly() ;  
当需要回滚的时候，需要在调用 setRoolbackOnly(); 就OK了。
**好了，走了这么久，现在就来简单总结一下编程式事务管理。**
**首先：** 因为我们使用的是特定的平台，所以，我们需要创建一个合适我们的平台事务管理PlateformTransactionManager。如果使用的是JDBC的话，就用DataSourceTransactionManager。注意需要传入一个DataSource，这样，平台才知道如何和数据库打交道。
**第二：** 为了使得平台事务管理器对我们来说是透明的，就需要使用 TransactionTemplate。使用TransactionTemplat需要传入一个 PlateformTransactionManager 进入，这样，我们就得到了一个 TransactionTemplate，而不用关心到底使用的是什么平台了。
**第三：** TransactionTemplate 的重要方法就是 execute 方法，此方法就是调用 TransactionCallback 进行处理。
也就是说，实际上我们需要处理的事情全部都是在 TransactionCallback 中编码的。
**第四：** 也就是 TransactionCallback 接口，我们可以定义一个类并实现此接口，然后作为 TransactionTemplate.execute 的参数。把需要完成的事情放到 doInTransaction中，并且传入一个 TransactionStatus 参数。此参数是来调用回滚的。
**也就是说** ，PlateformTransactionManager 和 TransactionTemplate 只需在程序中定义一次，而TransactionCallback 和 TransactionStatus 就要针对不同的任务多次定义了。
这就是Spring的编程式事务管理。下面贴出例子代码：
TemplateUtils
**[java]**[view
 plain](http://blog.csdn.net/zq9017197/article/details/6321391#)[copy](http://blog.csdn.net/zq9017197/article/details/6321391#)[print](http://blog.csdn.net/zq9017197/article/details/6321391#)[?](http://blog.csdn.net/zq9017197/article/details/6321391#)
- import javax.naming.Context;  
- import javax.naming.InitialContext;  
- import javax.naming.NamingException;  
- import javax.sql.DataSource;  
- 
- import org.apache.log4j.Logger;  
- import org.springframework.jdbc.core.JdbcTemplate;  
- import org.springframework.jdbc.core.namedparam.NamedParameterJdbcTemplate;  
- import org.springframework.jdbc.core.simple.SimpleJdbcTemplate;  
- import org.springframework.jdbc.datasource.DataSourceTransactionManager;  
- import org.springframework.transaction.PlatformTransactionManager;  
- import org.springframework.transaction.support.TransactionTemplate;  
- 
- publicclass TemplateUtils {  
- privatestatic Logger logger = Logger.getLogger(TemplateUtils.class);  
- privatestatic String oracleDS = "java:OracleDS";  
- privatestatic DataSource dataSource = null;  
- 
- static {  
- try {  
-             Context context = new InitialContext();  
-             dataSource = (DataSource) context.lookup(oracleDS);  
-         } catch (NamingException e) {  
-             logger.info("查找数据源失败···", e);  
-         }  
-     }  
- 
- publicstatic TransactionTemplate getTransactionTemplate() {  
-         PlatformTransactionManager txManager = new DataSourceTransactionManager(  
-                 dataSource);  
- returnnew TransactionTemplate(txManager);  
-     }  
- 
- publicstatic JdbcTemplate getJdbcTemplate() {  
- returnnew JdbcTemplate(dataSource);  
-     }  
- 
- publicstatic NamedParameterJdbcTemplate getNamedParameterJdbcTemplate() {  
- returnnew NamedParameterJdbcTemplate(dataSource);  
-     }  
- 
- publicstatic SimpleJdbcTemplate getSimpleJdbcTemplate() {  
- returnnew SimpleJdbcTemplate(dataSource);  
-     }  
- 
- }  
Test
**[java]**[view
 plain](http://blog.csdn.net/zq9017197/article/details/6321391#)[copy](http://blog.csdn.net/zq9017197/article/details/6321391#)[print](http://blog.csdn.net/zq9017197/article/details/6321391#)[?](http://blog.csdn.net/zq9017197/article/details/6321391#)
- import javax.naming.Context;  
- import javax.naming.InitialContext;  
- import javax.sql.DataSource;  
- 
- import org.springframework.jdbc.core.JdbcTemplate;  
- import org.springframework.jdbc.datasource.DataSourceTransactionManager;  
- import org.springframework.transaction.PlatformTransactionManager;  
- import org.springframework.transaction.TransactionStatus;  
- import org.springframework.transaction.support.DefaultTransactionDefinition;  
- import org.springframework.transaction.support.TransactionCallback;  
- import org.springframework.transaction.support.TransactionCallbackWithoutResult;  
- import org.springframework.transaction.support.TransactionTemplate;  
- 
- @SuppressWarnings("all")  
- publicclass Test {  
- 
- publicvoid m1() throws Exception {  
-         TransactionTemplate transactionTemplate = TemplateUtils  
-                 .getTransactionTemplate();  
-         Object object = transactionTemplate.execute(new TransactionCallback() {  
- public Object doInTransaction(TransactionStatus status) {  
- try {  
- // 数据库操作1
- // 数据库操作2
-                 } catch (Exception e) {  
-                     status.setRollbackOnly();  
-                     e.printStackTrace();  
-                 }  
- returnnull;  
-             }  
-         });  
-     }  
- 
- publicvoid m2() throws Exception {  
-         TransactionTemplate transactionTemplate = TemplateUtils  
-                 .getTransactionTemplate();  
-         transactionTemplate.execute(new TransactionCallbackWithoutResult() {  
- protectedvoid doInTransactionWithoutResult(TransactionStatus s) {  
- try {  
- // 数据库操作1
- // 数据库操作2
-                 } catch (Exception e) {  
-                     s.setRollbackOnly();  
-                     e.printStackTrace();  
-                 }  
-             }  
-         });  
-     }  
- 
- publicvoid m3() throws Exception {  
-         Context ctx = new InitialContext();  
-         DataSource ds = (DataSource) ctx.lookup("java:OracleDS");  
-         JdbcTemplate jt = new JdbcTemplate(ds);  
-         DefaultTransactionDefinition tf = new DefaultTransactionDefinition();  
-         PlatformTransactionManager tm = new DataSourceTransactionManager(ds);  
-         TransactionStatus ts = tm.getTransaction(tf);  
- try {  
- // 数据库操作1
- // 数据库操作2
-             tm.commit(ts);  
-         } catch (Exception e) {  
-             tm.rollback(ts);  
-             e.printStackTrace();  
-         }  
-     }  
- 
- }  
JdbcUtils
**[java]**[view
 plain](http://blog.csdn.net/zq9017197/article/details/6321391#)[copy](http://blog.csdn.net/zq9017197/article/details/6321391#)[print](http://blog.csdn.net/zq9017197/article/details/6321391#)[?](http://blog.csdn.net/zq9017197/article/details/6321391#)
- import java.sql.CallableStatement;  
- import java.sql.Connection;  
- import java.sql.PreparedStatement;  
- import java.sql.ResultSet;  
- import java.sql.SQLException;  
- import java.sql.Savepoint;  
- 
- import javax.naming.Context;  
- import javax.naming.InitialContext;  
- import javax.naming.NamingException;  
- import javax.sql.DataSource;  
- 
- import org.apache.log4j.Logger;  
- 
- publicclass JdbcUtils {  
- privatestatic Logger logger = Logger.getLogger(JdbcUtils.class);  
- privatestatic String oracleDS = "java:OracleDS";  
- privatestatic DataSource dataSource = null;  
- 
- static {  
- try {  
-             Context context = new InitialContext();  
-             dataSource = (DataSource) context.lookup(oracleDS);  
-         } catch (NamingException e) {  
-             logger.info("查找数据源失败···", e);  
-         }  
-     }  
- 
- publicstatic Connection getConnection() {  
-         Connection conn = null;  
- try {  
-             conn = dataSource.getConnection();  
-         } catch (SQLException e) {  
-             logger.info("获取数据库连接失败···", e);  
-         }  
- return conn;  
-     }  
- 
- publicstaticvoid close(Connection conn) {  
- if (conn != null) {  
- try {  
-                 conn.close();  
-             } catch (SQLException e) {  
-                 logger.info("释放数据库连接失败···", e);  
-             }  
-         }  
-     }  
- 
- publicstaticvoid close(CallableStatement cs) {  
- if (cs != null) {  
- try {  
-                 cs.close();  
-             } catch (SQLException e) {  
-                 logger.info("关闭CallableStatement失败···", e);  
-             }  
-         }  
-     }  
- 
- publicstaticvoid close(PreparedStatement ps) {  
- if (ps != null) {  
- try {  
-                 ps.close();  
-             } catch (SQLException e) {  
-                 logger.info("关闭PreparedStatement失败···", e);  
-             }  
-         }  
-     }  
- 
- publicstaticvoid close(ResultSet rs) {  
- if (rs != null) {  
- try {  
-                 rs.close();  
-             } catch (SQLException e) {  
-                 logger.info("关闭ResultSet失败···", e);  
-             }  
-         }  
-     }  
- 
- publicstaticvoid setAutoCommit(Connection conn, boolean autoCommit) {  
- if (conn != null) {  
- try {  
-                 conn.setAutoCommit(autoCommit);  
-             } catch (SQLException e) {  
-                 logger.info("设置事务提交方式失败···", e);  
-             }  
-         }  
-     }  
- 
- publicstaticvoid commit(Connection conn) {  
- if (conn != null) {  
- try {  
-                 conn.commit();  
-             } catch (SQLException e) {  
-                 logger.info("提交事务失败···", e);  
-             }  
-         }  
-     }  
- 
- publicstaticvoid rollback(Connection conn) {  
- if (conn != null) {  
- try {  
-                 conn.rollback();  
-             } catch (SQLException e) {  
-                 logger.info("回滚事务失败···", e);  
-             }  
-         }  
-     }  
- 
- publicstaticvoid rollback(Connection conn, Savepoint sp) {  
- if (conn != null) {  
- try {  
-                 conn.rollback(sp);  
-             } catch (SQLException e) {  
-                 logger.info("回滚事务失败···", e);  
-             }  
-         }  
-     }  
- 
- }  

