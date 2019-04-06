## [javaweb学习总结(三十六)——使用JDBC进行批处理](https://www.cnblogs.com/xdp-gacl/p/3983253.html)

　　在实际的项目开发中，有时候需要向数据库发送一批SQL语句执行，这时应避免向数据库一条条的发送执行，而应采用JDBC的批处理机制，以提升执行效率。
　　JDBC实现批处理有两种方式：statement和preparedstatement

## 一、使用Statement完成批处理

　　1、使用Statement对象添加要批量执行SQL语句，如下：

```
1 Statement.addBatch(sql1);
2 Statement.addBatch(sql2);
3 Statement.addBatch(sql3);
```

　　2、执行批处理SQL语句：Statement.executeBatch();
　　3、清除批处理命令：Statement.clearBatch();

### 1.1、**使用Statement完成批处理范例**

　　1、编写测试的SQL脚本创建表

```
1  create table testbatch
2 (
3      id int primary key,
4      name varchar(20)
5 );
```

　　2、编写测试代码，如下所示：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.demo;
 2 
 3 import java.sql.Connection;
 4 import java.sql.ResultSet;
 5 import java.sql.Statement;
 6 import me.gacl.utils.JdbcUtils;
 7 import org.junit.Test;
 8 
 9 /**
10 * @ClassName: JdbcBatchHandleByStatement
11 * @Description: 使用Statement实现JDBC批处理操作
12 * @author: 孤傲苍狼
13 * @date: 2014-9-20 下午10:05:45
14 *
15 */ 
16 public class JdbcBatchHandleByStatement {
17 
18     @Test
19     public void testJdbcBatchHandleByStatement(){
20         Connection conn = null;
21         Statement st = null;
22         ResultSet rs = null;
23         try{
24             conn = JdbcUtils.getConnection();
25             String sql1 = "insert into testbatch(id,name) values(1,'aaa')";
26             String sql2 = "insert into testbatch(id,name) values(2,'bbb')";
27             String sql3 = "insert into testbatch(id,name) values(3,'CCC')";
28             String sql4 = "insert into testbatch(id,name) values(4,'DDD')";
29             String sql5 = "update testbatch set name='gacl' where id=1";
30             String sql6 = "insert into testbatch(id,name) values(5,'FFF')";
31             String sql7 = "delete from testbatch where id=2";
32             st = conn.createStatement();
33             //添加要批量执行的SQL
34             st.addBatch(sql1);
35             st.addBatch(sql2);
36             st.addBatch(sql3);
37             st.addBatch(sql4);
38             st.addBatch(sql5);
39             st.addBatch(sql6);
40             st.addBatch(sql7);
41             //执行批处理SQL语句
42             st.executeBatch();
43             //清除批处理命令
44             st.clearBatch();
45         }catch (Exception e) {
46             e.printStackTrace();
47         }finally{
48             JdbcUtils.release(conn, st, rs);
49         }
50     }
51 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 1.2、采用Statement.addBatch(sql)方式实现批处理的优缺点

　　采用Statement.addBatch(sql)方式实现批处理：
　　　　优点：可以向数据库发送多条不同的ＳＱＬ语句。
　　　　缺点：SQL语句没有预编译。
　　　　当向数据库发送多条语句相同，但仅参数不同的SQL语句时，需重复写上很多条SQL语句。例如：

```
1 Insert into user(name,password) values('aa','111');
2 Insert into user(name,password) values('bb','222');
3 Insert into user(name,password) values('cc','333');
4 Insert into user(name,password) values('dd','444');
```

## 二、使用PreparedStatement完成批处理

### 2.1、**使用PreparedStatement完成批处理范例**

　　测试代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.demo;
 2 
 3 import java.sql.Connection;
 4 import java.sql.PreparedStatement;
 5 import java.sql.ResultSet;
 6 import me.gacl.utils.JdbcUtils;
 7 import org.junit.Test;
 8 
 9 /**
10 * @ClassName: JdbcBatchHandleByStatement
11 * @Description: 使用prepareStatement实现JDBC批处理操作
12 * @author: 孤傲苍狼
13 * @date: 2014-9-20 下午10:05:45
14 *
15 */ 
16 public class JdbcBatchHandleByPrepareStatement {
17 
18     @Test
19     public void testJdbcBatchHandleByPrepareStatement(){
20         long starttime = System.currentTimeMillis();
21         Connection conn = null;
22         PreparedStatement st = null;
23         ResultSet rs = null;
24         
25         try{
26             conn = JdbcUtils.getConnection();
27             String sql = "insert into testbatch(id,name) values(?,?)";
28             st = conn.prepareStatement(sql);
29             for(int i=1;i<1000008;i++){  //i=1000  2000
30                 st.setInt(1, i);
31                 st.setString(2, "aa" + i);
32                 st.addBatch();
33                 if(i%1000==0){
34                     st.executeBatch();
35                     st.clearBatch();
36                 }
37             }
38             st.executeBatch();
39         }catch (Exception e) {
40             e.printStackTrace();
41         }finally{
42             JdbcUtils.release(conn, st, rs);
43         }
44         long endtime = System.currentTimeMillis();
45         System.out.println("程序花费时间：" + (endtime-starttime)/1000 + "秒！！");
46     }
47 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 2.2、采用PreparedStatement.addBatch()方式实现批处理的优缺点

　　采用PreparedStatement.addBatch()实现批处理
　　　　优点：发送的是预编译后的SQL语句，执行效率高。
　　　　缺点：只能应用在SQL语句相同，但参数不同的批处理中。因此此种形式的批处理经常用于在同一个表中批量插入数据，或批量更新表的数据。

　　关于JDBC批处理的内容就总结这么多。



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)