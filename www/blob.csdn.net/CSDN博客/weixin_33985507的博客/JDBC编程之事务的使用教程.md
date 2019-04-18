# JDBC编程之事务的使用教程 - weixin_33985507的博客 - CSDN博客
2016年09月13日 15:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
**转载请注明原文地址：[http://www.cnblogs.com/ygj0930/p/5868750.html](http://www.cnblogs.com/ygj0930/p/5868750.html)**
关于事务的理论知识、ACID特性等等，网上太多了，在此不一一重复。本文主要着重  事务  这个工具在实际编程中是怎么实现的。
在MySQL命令行的默认设置下，事务都是自动提交的，即执行SQL语句后就会马上执行COMMIT操作，每一个SQL语句都被认为是一个完整的事务处理。
而我们想要实现事务，即：执行多句SQL语句，再最终一起提交或在出错时撤销（SQL语句要么提交，要么撤销。提交则对数据库造成永久性的影响，撤销则事务内的sql语句相当于没有执行）。
那么我们要做的就是：取消掉每一个SQL语句执行后自动提交这个属性，并设立一个开关（commit()）执行一些列语句的同一提交操作。
取消SQL语句自动提交的方法有：
con.setAutoCommit（false）：取消自动提交。则从此处开始，下面的一系列SQL语句除非遇到commit()命令，都不提交。
具体的JDBC编程中事务的应用步骤如下：
    1) JDBC对事务的管理交由Connection，都是由Connection的对象方法实现的；
    2) 首先关闭自动提交，开启事务：void Connection.setAutoCommit(false);  // false表示关闭自动提交
    3) 然后就是事务中包含的一系列SQL语句
    4) 提交事务：con.commit();  // 显式提交
    5) 回滚：如果在事务执行过程中出错（用try-catch语句捕捉），则在错误处理语句中显式回滚：con.rollback();
    6) 中间点：一般的事务回滚是回滚到事务开始之前，但是也可以只回滚到事务中的某个中间点。
    设置中间点
         i. Savepoint Connection.setSavepoint();  // 在事务的某个位置设置一个中间点，该中间点没有命名，使用系统默认的命名
         ii. Savepoint setSavepoint(String name);  // 给中间点命名
         iii. 回滚到指定的中间点：connection.rollback(Savepoint savepoint);  // 回滚到指定的中间点
代码样例：
```
try {
       conn.setAutoCommit(false);
       String sql1="。。。";
       stmt.executeUpdate(sql);
       String sql2="。。。";
       stmt.executeUpdate(sq2);
       String sql3="。。。";
       stmt.executeUpdate(sq3);
       conn.commit();
       } catch (Exception e) {
            e.printStackTrace();
             try {
                 conn.rollback();
             } catch (SQLException e1) {
                 
                 e1.printStackTrace();
             }
```
