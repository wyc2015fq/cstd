
# JDBC事务的概述及应用 - 李昆鹏的博客 - CSDN博客


2018年06月19日 13:03:43[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：79


--------------------------------------------------JDBC事务的概述及应用---------------------------------------------
# 事务
## 1 事务概述
为了方便演示事务，我们需要创建一个account表：
|CREATE TABLE account(
|ID  NUMBER(10),
|NAME  VARCHAR(30),
|balance  NUMBER(10,2)
|);
|INSERT INTO account(NAME,balance) VALUES('zs', 100000);
|INSERT INTO account(NAME,balance) VALUES('ls', 100000);
|INSERT INTO account(NAME,balance) VALUES('ww', 100000);
|SELECT * FROM account;
|

### 什么是事务
银行转账！张三转1000块到李四的账户，这其实需要两条SQL语句：
l  给张三的账户减去1000元；
l  给李四的账户加上1000元。
如果在第一条SQL语句执行成功后，在执行第二条SQL语句之前，程序被中断了（可能是抛出了某个异常，也可能是其他什么原因），那么李四的账户没有加上10000元，而张三却减去了10000元。这肯定是不行的！
你现在可能已经知道什么是事务了吧！事务中的多个操作，要么完全成功，要么完全失败！不可能存在成功一半的情况！也就是说给张三的账户减去10000元如果成功了，那么给李四的账户加上10000元的操作也必须是成功的；否则给张三减去10000元，以及给李四加上10000元都是失败的！

### 事务的四大特性（ACID）
**面试！**
事务的四大特性是：
l  原子性（Atomicity）：事务中所有操作是不可再分割的原子单位。事务中所有操作要么全部执行成功，要么全部执行失败。
l  一致性（Consistency）：事务执行后，数据库状态与其它业务规则保持一致。如转账业务，无论事务执行成功与否，参与转账的两个账号余额之和应该是不变的。
l  隔离性（Isolation）：隔离性是指在并发操作中，不同事务之间应该隔离开来，使每个并发中的事务不会相互干扰。
l  持久性（Durability）：一旦事务提交成功，事务中所有的数据操作都必须被持久化到数据库中，即使提交事务后，数据库马上崩溃，在数据库重启时，也必须能保证通过某种机制恢复数据。

### 3　Oracle中的事务
在默认情况下，每执行一条增、删、改SQL语句，都是一个单独的事务。如果需要在一个事务中包含多条SQL语句，那么需要开启事务和结束事务。
l  结束事务：**commit**或**rollback**。
在执行增、删、改一条SQL就开启了一个事务（事务的起点），然后可以去执行多条SQL语句，最后要结束事务，commit表示提交，即事务中的多条SQL语句所做出的影响会持久化到数据库中。或者rollback，表示回滚，即回滚到事务的起点，之前做的所有操作都被撤消了！
下面演示zs给li转账10000元的示例：
|UPDATE account SET balance=balance-10000  WHERE id=1;
|UPDATE account SET balance=balance+10000  WHERE id=2;
|ROLLBACK;
|
|UPDATE account SET balance=balance-10000  WHERE id=1;
|UPDATE account SET balance=balance+10000  WHERE id=2;
|COMMIT;
|
## 2 JDBC事务
Connection的三个方法与事务相关：
l  setAutoCommit(boolean)：设置是否为自动提交事务，如果true（默认值就是true）表示自动提交，也就是每条执行的SQL语句都是一个单独的事务，如果设置false，那么就相当于开启了事务了；
l  commit()：提交结束事务；
l  rollback()：回滚结束事务。
|public|void|transfer(|boolean|b) {
|Connection con =|null|;
|PreparedStatement pstmt =|null|;
|try|{
|con = JdbcUtils.|getConnection|();
|//|手动提交
|con.setAutoCommit(|false|);
|String sql =|"update account set balance=balance+? where  id=?"|;
|pstmt = con.prepareStatement(sql);
|//|操作
|pstmt.setDouble(1, -10000);
|pstmt.setInt(2, 1);
|pstmt.executeUpdate();
|//|在两个操作中抛出异常
|if|(b) {
|throw|new|Exception();
|}
|pstmt.setDouble(1, 10000);
|pstmt.setInt(2, 2);
|pstmt.executeUpdate();
|//|提交事务
|con.commit();
|}|catch|(Exception e) {
|//|回滚事务
|if|(con !=|null|) {
|try|{
|con.rollback();
|}|catch|(SQLException ex) {}
|}
|throw|new|RuntimeException(e);
|}|finally|{
|//|关闭
|JdbcUtils.|close|(con, pstmt);
|}
|}
|
代码示例：
|package|com.rl.jdbc.tran;
|import|java.math.BigDecimal;
|import|java.sql.Connection;
|import|java.sql.PreparedStatement;
|import|java.sql.SQLException;
|import|com.rl.jdbc.utils.DBUtils;
|public|class|JDBCTran {
|public|static|void|main(String[]|args|){
|//|定义事务执行语句
|String|sql|=|"update myaccount t set t.balance =  t.balance - ? where t.id = ?"|;
|String|sql2|=|"update myaccount t set t.balance =  t.balance + ? where t.id = ?"|;
|Connection|conn|= DBUtils.|getConn|();
|PreparedStatement|ps|=|null|;
|PreparedStatement|ps1|=|null|;
|try|{
|//|获得预编译对象
|ps|=|conn|.prepareStatement(|sql|);
|ps1|=|conn|.prepareStatement(|sql2|);
|//|设置手动提交事务|,|让当前的连接|connection|的所有的数据库变更
|//|的|sql|都在一个事务之内，要么全部成功要么全部失败。
|conn|.setAutoCommit(|false|);
|//|给执行|sql|语句赋参数值
|ps|.|setBigDecimal|(1,|new|BigDecimal(1000));
|ps|.setInt(2, 1);
|ps|.executeUpdate();
|if|(|true|){
|throw|new|Exception();
|}
|ps1|.|setBigDecimal|(1,|new|BigDecimal(1000))|;
|ps1|.setInt(2, 2);
|ps|.executeUpdate();
|//|手动提交事务
|conn|.commit();
|}|catch|(Exception|e|) {
|//|如果发生异常就事务回滚
|try|{
|conn|.rollback();
|}|catch|(SQLException|e1|) {
|e1|.printStackTrace();
|}
|e|.printStackTrace();
|}|finally|{
|//|关闭资源
|DBUtils.|closeUpdate|(|ps1|);
|DBUtils.|closeUpdate|(|ps|);
|}
|}
|}
|


