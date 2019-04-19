# JDBC学习笔记(4)——PreparedStatement的使用 - xqhrs232的专栏 - CSDN博客
2018年11月07日 16:39:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：89
原文地址::[https://www.cnblogs.com/ysw-go/p/5459330.html](https://www.cnblogs.com/ysw-go/p/5459330.html)
相关文章
1、select * from tb_shop id order by id desc limit 0,1----[https://blog.csdn.net/tracymm19891990/article/details/7329653](https://blog.csdn.net/tracymm19891990/article/details/7329653)
2、select * from user limit 0, 10;----[https://zhidao.baidu.com/question/353628055.html](https://zhidao.baidu.com/question/353628055.html)
3、在mysql中的sql语句:select * from 表明 limit 0,10; 换成Oracle应该怎么写----[https://zhidao.baidu.com/question/179749410.html](https://zhidao.baidu.com/question/179749410.html)
**PreparedStatement**
public interface PreparedStatement extends Statement;可以看到PreparedStatement是Statement的子接口，我们在执行查询或者更新数据表数据的时候，拼写SQL语句是一个很费力并且容易出错的事情，PreparedStatement可以简化这样的一个过程.
PreParedStatement
1).why?我们为什么要使用它
使用Statement需要进行拼写SQl语句，辛苦并且容易出错，之前使用Statement的SQL语句的形式是这样的
```
String sql = "insert into examstudent" + " values("
+ student.getFlowId() + "," + student.getType() + ",'"
+ student.getIdCard() + "','" + student.getExamCard() + "','"
+ student.getStudentName() + "','" + student.getLocation()
+ "'," + student.getGrade() + ")";
```
使用PreparedStatement:是Statement的子接口,可以传入**带占位符**的SQL语句，提供了补充占位符变量的方法
`PreparedStatement ps=conn.preparedStatement(sql);`
可以看到将sql作为参数传入了，就不需要我们在费力拼写了。
2)变成了这样的形式
`String sql="insert into examstudent values(?,?,?,?,?,?,?)";`
可以调用PreparedStatement的setXxx(int index,Object val)设置占位符的值，其中index的值从1开始
执行SQl语句：excuteQuery()或者excuteUpdate()就可以完成查询或者数据的更新.【注意】：**此时函数的参数位置不需要传入SQL语句，注意同使用Statement的update函数的差别**
具体代码实现：
```
1 @Test
 2     public void testPreparedStatement() {
 3         Connection connection = null;
 4         PreparedStatement preparedStatement = null;
 5         try {
 6             // 连接数据库
 7             connection = JDBCTools.getConnection();
 8             // 使用占位符的SQl语句
 9             String sql = "insert into customers(name,email,birth)"
10                     + "values(?,?,?)";
11             // 使用preparedStatement的setXxx方法设置每一个位置上的值
12             preparedStatement = connection.prepareStatement(sql);
13             // 设置name字段
14             preparedStatement.setString(1, "ATGUIGU");
15             // 设置email字段
16             preparedStatement.setString(2, "simale@163.com");
17             // 设置birth字段
18             preparedStatement.setDate(3,
19                     new Date(new java.util.Date().getTime()));
20             // 执行更新操作
21             preparedStatement.executeUpdate();
22         } catch (Exception e) {
23             e.printStackTrace();
24         } finally {
25             // 释放资源
26             JDBCTools.release(null, preparedStatement, connection);
27         }
28     }
```
使用PreparedStatement执行SQl(更新操作：插入、删除、更新，但不包括select查询操作)，JDBCTools中的通用函数update更改成下面的形式：这里使用了可变参数，而不是使用数组
```
1     public static void update(String sql,Object ...args){
 2         /**
 3          * 执行SQL语句，使用PreparedStatement
 4          */
 5         Connection connection=null;
 6         PreparedStatement preparedStatement=null;
 7         try {
 8             connection=JDBCTools.getConnection();
 9             preparedStatement=connection.prepareStatement(sql);
10             for(int i=0;i<args.length;i++){
11                 preparedStatement.setObject(i+1, args[i]);
12             }
13             preparedStatement.executeUpdate();
14         } catch (Exception e) {
15             e.printStackTrace();
16         }finally{
17             JDBCTools.release(null, preparedStatement, connection);
18         }
19     }
```
使用PreparedStatement的好处：
1).提高代码的可读性和可维护性;
2).最大程度的提高性能:JDBC驱动的最佳化是基于使用的是什么功能. 选择PreparedStatement还是Statement取决于你要怎么使用它们. 对于只执行一次的SQL语句选择Statement是最好的. 相反, 如果SQL语句被多次执行选用PreparedStatement是最好的.PreparedStatement的第一次执行消耗是很高的. 它的性能体现在后面的重复执行(缓存的作用). 例如, 假设我使用Employee ID, 使用prepared的方式来执行一个针对Employee表的查询. JDBC驱动会发送一个网络请求到数据解析和优化这个查询. 而执行时会产生另一个网络请求. 在JDBC驱动中，减少网络通讯是最终的目的. 如果我的程序在运行期间只需要一次请求, 那么就使用Statement. 对于Statement, 同一个查询只会产生一次网络到数据库的通讯.当使用PreparedStatement池时, 如果一个查询很特殊, 并且不太会再次执行到, 那么可以使用Statement. 如果一个查询很少会被执行,但连接池中的Statement池可能被再次执行, 那么请使用PreparedStatement. 在不是Statement池的同样情况下, 请使用Statement.
3).可以防止SQL注入
**SQL注入**指的是通过构建特殊的输入作为参数传入Web应用程序，而这些输入大都是SQL语法里的一些**组合**，通过执行SQL语句进而执行攻击者所要的操作，其主要原因是程序没有**细致地过滤用户输入的数据**，致使非法数据侵入系统。
比如我们新建一个数据表users,表中有两个字段username和password;
![](https://images2015.cnblogs.com/blog/653266/201605/653266-20160504165332982-1401816362.png)
我们在图形化界面SQLyog的sql语句的查询界面输入这样的查询语句：**select * from users where username='a' or password='and password=' or '1'='1'；**
执行该语句，会得到我们表中的数据：
![](https://images2015.cnblogs.com/blog/653266/201605/653266-20160504170234982-17961835.png)
我们可以分析一下这条语句：where的后面，通过多个字段的组合作为查询过滤的条件。
字段一：username='a'
字段二：password='and password='
字段三：'1'='1'
因为用逻辑连接符OR来连接的三个字段，只要有一个为真就可以将查询工作完成.
下面我们看下具体的代码实现：
```
1     @Test
 2     public void testSQLinjection() {
 3         String username = "a' or password =";
 4         String password = " or '1'='1";
 5         String sql = "select * from users where username='" + username
 6                 + "' AND " + "password='" + password + "'";
 7         System.out.println(sql);
 8         Connection connection = null;
 9         Statement statement = null;
10         ResultSet resultSet = null;
11         try {
12             connection = getConnection();
13             statement = connection.createStatement();
14             resultSet = statement.executeQuery(sql);
15             if (resultSet.next()) {
16                 System.out.println("登陆成功");
17             } else {
18                 System.out.println("不匹配");
19             }
20         } catch (Exception e) {
21             e.printStackTrace();
22         } finally {
23             JDBCTools.release(resultSet, statement, connection);
24         }
25     }
```
运行结果：
```
select * from users where username='a' or password =' AND password=' or '1'='1'
登陆成功
```
可以看到我们的SQl语句中都没有明确我们要查的字段的名，但是还是获取了查询的结果(SQL语句太能混了)
于是，我们用了PreparedStatement就可以解决SQL注入的问题。
```
1     @Test
 2     public void testSQLinjection2() {
 3         String username = "a' or password =";
 4         String password = " or '1'='1";
 5         String sql = "select * from users where username=?" + " and password=?";
 6         System.out.println(sql);
 7         Connection connection = null;
 8         PreparedStatement preparedStatement = null;
 9         ResultSet resultSet = null;
10         try {
11             connection = getConnection();
12             preparedStatement = connection.prepareStatement(sql);
13             preparedStatement.setString(1, username);
14             preparedStatement.setString(2, password);
15             resultSet = preparedStatement.executeQuery();
16             if (resultSet.next()) {
17                 System.out.println("登陆成功");
18             } else {
19                 System.out.println("不匹配");
20             }
21         } catch (Exception e) {
22             e.printStackTrace();
23         } finally {
24             JDBCTools.release(resultSet, preparedStatement, connection);
25         }
26     }
```
执行结果：
```
select * from users where username=? and password=?
不匹配
```
可以看到：再次使用伪装后的SQL语句已经不能获取我们数据表中的信息，我们这里在sql语句中使用了占位符。因此使用PreparedStatement可以结解决这里的SQL注入的问题。
