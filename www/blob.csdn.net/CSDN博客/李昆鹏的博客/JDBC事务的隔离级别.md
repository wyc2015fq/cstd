
# JDBC事务的隔离级别 - 李昆鹏的博客 - CSDN博客


2018年06月24日 23:50:16[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：56标签：[JDBC																](https://so.csdn.net/so/search/s.do?q=JDBC&t=blog)个人分类：[JDBC																](https://blog.csdn.net/weixin_41547486/article/category/7739465)


----------------------------------------------JDBC事务的隔离级别----------------------------------------------------
## 事务隔离级别
l  隔离性（Isolation）：隔离性是指在并发操作中，不同事务之间应该隔离开来，使每个并发中的事务不会相互干扰。
（准确的说隔离级别是可以设置的，可以设置为事物之间相互干扰，也可以设置为事务之间不会相互干扰，主要体现在两个事务一个在写数据，一个在读数据，对读的一方是有影响的）
Ø**事务的并发读问题**
l  脏读：读取到另一个事务未提交数据；
![](https://img-blog.csdn.net/20180624234619114?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
l  不可重复读：两次读取不一致；
![](https://img-blog.csdn.net/20180624234624398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
l  幻读（虚读）：读到另一事务已提交数据。
![](https://img-blog.csdn.net/20180624234634639?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ø**2　五大并发事务问题**
因为并发事务导致的问题大致有5类，其中两类是更新问题，三类是读问题。
l  脏读（dirty read）：**读到未提交更新数据**
|时间
|转账事务A
|取款事务B
|
|T1
|开始事务
|
|T2
|开始事务
|
|T3
|查询账户余额为1000元
|
|T4
|取出500元把余额改为500元
|
|T5
|查看账户余额为500元（脏读）
|
|T6
|撤销事务，余额恢复为1000元
|
|T7
|汇入100元把余额改为600元
|
|T8
|提交事务
|
A事务查询到了B事务未提交的更新数据，A事务依据这个查询结果继续执行相关操作。但是接着B事务撤销了所做的更新，这会导致A事务操作的是脏数据。（这是绝对不允许出现的事情）
l  虚读（幻读）（phantom read）：**读到已提交插入数据**
|时间
|统计金额事务A
|转账事务B
|
|T1
|开始事务
|
|T2
|开始事务
|
|T3
|统计总存款数为10000元
|
|T4
|新增一个存款账户，存款为100元
|
|T5
|提交事务
|
|T6
|再次统计总存款数为10100元
|
A事务第一次查询时，没有问题，第二次查询时查到了B事务已提交的新插入数据，这导致两次查询结果不同。（在实际开发中，很少会对相同数据进行两次查询，所以可以考虑是否允许虚读）
l  不可重复读（unrepeatable read）：**读到已提交更新数据**
|时间
|取款事务A
|转账事务B
|
|T1
|开始事务
|
|T2
|开始事务
|
|T3
|查询账户余额为1000元
|
|T4
|查询账户余额为1000元
|
|T5
|取出100元，把余额改为900元
|
|T6
|提交事务
|
|T7
|查询账户余额为900元（与T4读取的一不一致）
|
不可重复读与虚读有些相似，都是两次查询的结果不同。后者是查询到了另一个事务已提交的新插入数据，而前者是查询到了另一个事务已提交的更新数据。
Ø**3　四大隔离级别**
|隔离级别
|脏读
|不可重复读
|虚读
|
|READ UNCOMMITTED
|允许
|允许
|允许
|
|READ COMMITTED
|不允许
|允许
|允许
|
|REPEATABLE READ
|不允许
|不允许
|允许
|
|SERIALIZABLE
|不允许
|不允许
|不允许
|
Ø**4　哪种隔离级别最好**
4个等级的事务隔离级别，在相同数据环境下，使用相同的输入，执行相同的工作，根据不同的隔离级别，可以导致不同的结果。不同事务隔离级别能够解决的数据并发问题的能力是不同的。
1　SERIALIZABLE（串行化）
当数据库系统使用SERIALIZABLE隔离级别时，一个事务在执行过程中完全看不到其他事务对数据库所做的更新。当两个事务同时操作数据库中相同数据时，如果第一个事务已经在访问该数据，第二个事务只能停下来等待，必须等到第一个事务结束后才能恢复运行。因此这两个事务实际上是串行化方式运行。
2　REPEATABLE READ（可重复读）
当数据库系统使用REPEATABLEREAD隔离级别时，一个事务在执行过程中可以看到其他事务已经提交的新插入的记录，但是不能看到其他事务对已有记录的更新。
3　READ COMMITTED（读已提交数据）
当数据库系统使用READ COMMITTED隔离级别时，一个事务在执行过程中可以看到其他事务已经提交的新插入的记录，而且还能看到其他事务已经提交的对已有记录的更新。
4　READ UNCOMMITTED（读未提交数据）
当数据库系统使用READ UNCOMMITTED隔离级别时，一个事务在执行过程中可以看到其他事务没有提交的新插入的记录，而且还能看到其他事务没有提交的对已有记录的更新。
你可能会说，选择SERIALIZABLE，因为它最安全！没错，它是最安全，但它也是最慢的！而且也最容易产生死锁。四种隔离级别的安全性与性能成反比！最安全的性能最差，最不安全的性能最好！
**MySQL的默认隔离级别为REPEATABLE READ**
**Oracle数据库支持READ COMMITTED 和 SERIALIZABLE这两种事务隔离级别。所以Oracle不支持脏读Oracle的默认隔离级别是READ COMMITTED**
Ø**6　JDBC设置隔离级别**
con.setTransactionIsolation(int level)
参数可选值如下：
l  Connection.TRANSACTION_READ_UNCOMMITTED；
l  Connection.TRANSACTION_READ_COMMITTED；
l  Connection.TRANSACTION_REPEATABLE_READ；
l  Connection.TRANSACTION_SERIALIZABLE。
事务总结：
l事务的特性：ACID；
l事务开始边界与结束边界：开始边界（con.setAutoCommit(false)），结束边界（con.commit()或con.rollback()）；
l事务的隔离级别： READ_UNCOMMITTED、READ_COMMITTED、REPEATABLE_READ、SERIALIZABLE。多个事务并发执行时才需要考虑并发事务。
**事务的隔离主要体现在读数据和写数据**
**Read committed（oracle默认的隔离级别）隔离级别代码示例**
**不可提交读代码示例：我们需要两个类，一个修改数据，一个查询数据**
![](https://img-blog.csdn.net/20180702201329343?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
修改数据类
|package com.rl.jdbc.tran;
|import java.math.BigDecimal;
|import java.sql.Connection;
|import java.sql.PreparedStatement;
|import java.sql.SQLException;
|import com.rl.jdbc.utils.DBUtils;
|public class JDBCTran1 {
|public  static void main(String[] args){
|//定义事务执行语句
|String  sql = "update myaccount t set t.balance = t.balance - ? where t.id =  ?";
|Connection  conn = DBUtils.getConn();
|PreparedStatement  ps = null;
|try  {
|//获得预编译对象
|ps  = conn.prepareStatement(sql);
|//设置手动提交事务,让当前的连接connection的所有的数据库变更
|//的sql都在一个事务之内，要么全部成功要么全部失败。
|conn.setAutoCommit(false);
|//给执行sql语句赋参数值
|ps.setBigDecimal(1,  new BigDecimal(1000));
|ps.setInt(2,  1);
|ps.executeUpdate();
|//手动提交事务
|conn.commit();
|}  catch (Exception e) {
|//如果发生异常就事务回滚
|try  {
|conn.rollback();
|}  catch (SQLException e1) {
|e1.printStackTrace();
|}
|e.printStackTrace();
|}  finally{
|//关闭资源
|DBUtils.closeUpdate(ps);
|}
|}
|}
|
查询数据类
|package com.rl.jdbc.tran;
|import java.sql.Connection;
|import java.sql.PreparedStatement;
|import java.sql.ResultSet;
|import java.sql.SQLException;
|import com.rl.jdbc.utils.DBUtils;
|public class JDBCRead {
|public  static void main(String[] args) {
|Connection  conn = DBUtils.getConn();
|PreparedStatement  ps = null;
|try  {
|//建立开启事务的查询语句
|String  sql = "select * from myaccount t where t.id = 2 for update";
|//查询事务的隔离级别
|System.out.println(conn.getTransactionIsolation());
|conn.setAutoCommit(false);
|ps  = conn.prepareStatement(sql);
|ps.executeUpdate();
|read(conn);
|read(conn);
|conn.commit();
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|DBUtils.closeUpdate(ps);
|}
|}
|/**
|* 创建一个查询数据的方法
|*/
|public  static void read(Connection conn) {
|PreparedStatement  ps = null;
|String  sql = "select * from myaccount t where t.id = ?";
|ResultSet  rs = null;
|try  {
|ps  = conn.prepareStatement(sql);
|ps.setInt(1,  1);
|rs  = ps.executeQuery();
|rs.next();
|System.out.println(rs.getString("balance"));
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(rs  != null)
|rs.close();
|if(ps  != null)
|ps.close();
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|}
|}
|
**幻读（虚读）代码示例：**
添加数据类
|package com.rl.jdbc.tran;
|import java.math.BigDecimal;
|import java.sql.Connection;
|import java.sql.PreparedStatement;
|import java.sql.SQLException;
|import com.rl.jdbc.utils.DBUtils;
|public class JDBCTran2 {
|public  static void main(String[] args){
|//定义事务执行语句
|String  sql = "insert into myaccount values(?,?,?)";
|Connection  conn = DBUtils.getConn();
|PreparedStatement  ps = null;
|try  {
|//获得预编译对象
|ps  = conn.prepareStatement(sql);
|//设置手动提交事务,让当前的连接connection的所有的数据库变更
|//的sql都在一个事务之内，要么全部成功要么全部失败。
|conn.setAutoCommit(false);
|//给执行sql语句赋参数值
|ps.setInt(1,  3);
|ps.setString(2,  "王五");
|ps.setBigDecimal(3,  new BigDecimal(10001));
|ps.executeUpdate();
|//手动提交事务
|conn.commit();
|}  catch (Exception e) {
|//如果发生异常就事务回滚
|try  {
|conn.rollback();
|}  catch (SQLException e1) {
|e1.printStackTrace();
|}
|e.printStackTrace();
|}  finally{
|//关闭资源
|DBUtils.closeUpdate(ps);
|}
|}
|}
|
查询数据类
|package com.rl.jdbc.tran;
|import java.sql.Connection;
|import java.sql.PreparedStatement;
|import java.sql.ResultSet;
|import java.sql.SQLException;
|import com.rl.jdbc.utils.DBUtils;
|public class JDBCRead2 {
|public  static void main(String[] args) {
|Connection  conn = DBUtils.getConn();
|PreparedStatement  ps = null;
|try  {
|//建立开启事务的查询语句
|String  sql = "select * from myaccount t where t.id = 2 for update";
|//查询事务的隔离级别
|System.out.println(conn.getTransactionIsolation());
|conn.setAutoCommit(false);
|ps  = conn.prepareStatement(sql);
|ps.executeUpdate();
|read(conn);
|System.out.println("-----------------------------");
|read(conn);
|conn.commit();
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|DBUtils.closeUpdate(ps);
|}
|}
|/**
|* 创建一个查询数据的方法
|*/
|public  static void read(Connection conn) {
|PreparedStatement  ps = null;
|String  sql = "select * from myaccount";
|ResultSet  rs = null;
|try  {
|ps  = conn.prepareStatement(sql);
|rs  = ps.executeQuery();
|while(rs.next())  {
|System.out.println("用户名" + rs.getString("username")+ "  余额" +  rs.getString("balance"));
|}
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally {
|try  {
|if(rs  != null)
|rs.close();
|if(ps  != null)
|ps.close();
|}  catch (SQLException e) {
|e.printStackTrace();
|}
|}
|}
|}
|
**Serializeble的隔离级别需要在修改和查询两个类设置隔离级别，要在事务提交前设置**
|//|事务隔离级别要在提交事务前提交
|conn|.setTransactionIsolation(|conn|.|TRANSACTION_SERIALIZABLE|);
|conn|.setAutoCommit(|false|);
|


