## [javaweb学习总结(三十三)——使用JDBC对数据库进行CRUD](https://www.cnblogs.com/xdp-gacl/p/3973886.html)

## 一、statement对象介绍　　

　　Jdbc中的statement对象用于向数据库发送SQL语句，想完成对数据库的增删改查，只需要通过这个对象向数据库发送增删改查语句即可。
　　Statement对象的executeUpdate方法，用于向数据库发送增、删、改的sql语句，executeUpdate执行完后，将会返回一个整数(即增删改语句导致了数据库几行数据发生了变化)。
　　Statement.executeQuery方法用于向数据库发送查询语句，executeQuery方法返回代表查询结果的ResultSet对象。

### 1.1、CRUD操作-create

　　使用executeUpdate(String sql)方法完成数据添加操作，示例操作：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 Statement st = conn.createStatement();
2 String sql = "insert into user(….) values(…..) "; 
3 int num = st.executeUpdate(sql);
4 if(num>0){
5     System.out.println("插入成功！！！");
6 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 1.2、CRUD操作-update

　　使用executeUpdate(String sql)方法完成数据修改操作，示例操作：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 Statement st = conn.createStatement();
2 String sql = “update user set name=‘’ where name=‘’"; 
3 int num = st.executeUpdate(sql);
4 if(num>0){
5     System.out.println(“修改成功！！！");
6 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 1.3、CRUD操作-delete

　　使用executeUpdate(String sql)方法完成数据删除操作，示例操作：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 Statement st = conn.createStatement();
2 String sql = “delete from user where id=1; 
3 int num = st.executeUpdate(sql);
4 if(num>0){
5     System.out.println(“删除成功！！！");
6 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 1.4、CRUD操作-read

　　使用executeQuery(String sql)方法完成数据查询操作，示例操作：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 Statement st = conn.createStatement();
2 String sql = “select * from user where id=1; 
3 ResultSet rs = st.executeUpdate(sql);
4 while(rs.next()){
5     //根据获取列的数据类型，分别调用rs的相应方法映射到java对象中
6 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 二、使用jdbc对数据库增删改查

### 2.1、搭建实验环境    

　　1、在mysql中创建一个库，并创建user表和插入表的数据。

　　SQL脚本如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 create database jdbcStudy;
 2 
 3 use jdbcStudy;
 4 
 5 create table users(
 6     id int primary key,
 7     name varchar(40),
 8     password varchar(40),
 9     email varchar(60),
10     birthday date
11 );
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　2、新建一个JavaWeb工程，并导入MySQL数据库驱动。

　　![img](https://images0.cnblogs.com/blog/289233/201409/152243488155686.png)

　　3、在src目录下创建一个db.properties文件，如下图所示：

　　![img](data:image/png;base64,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)

　　在db.properties中编写MySQL数据库的连接信息，代码如下所示：

```
1 driver=com.mysql.jdbc.Driver
2 url=jdbc:mysql://localhost:3306/jdbcStudy
3 username=root
4 password=XDP
```

　　4、编写一个JdbcUtils工具类，用于连接数据库，获取数据库连接和释放数据库连接，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.utils;
 2 
 3 import java.io.InputStream;
 4 import java.sql.Connection;
 5 import java.sql.DriverManager;
 6 import java.sql.ResultSet;
 7 import java.sql.SQLException;
 8 import java.sql.Statement;
 9 import java.util.Properties;
10 
11 public class JdbcUtils {
12 
13     private static String driver = null;
14     private static String url = null;
15     private static String username = null;
16     private static String password = null;
17     
18     static{
19         try{
20             //读取db.properties文件中的数据库连接信息
21             InputStream in = JdbcUtils.class.getClassLoader().getResourceAsStream("db.properties");
22             Properties prop = new Properties();
23             prop.load(in);
24             
25             //获取数据库连接驱动
26             driver = prop.getProperty("driver");
27             //获取数据库连接URL地址
28             url = prop.getProperty("url");
29             //获取数据库连接用户名
30             username = prop.getProperty("username");
31             //获取数据库连接密码
32             password = prop.getProperty("password");
33             
34             //加载数据库驱动
35             Class.forName(driver);
36             
37         }catch (Exception e) {
38             throw new ExceptionInInitializerError(e);
39         }
40     }
41     
42     /**
43     * @Method: getConnection
44     * @Description: 获取数据库连接对象
45     * @Anthor:孤傲苍狼
46     *
47     * @return Connection数据库连接对象
48     * @throws SQLException
49     */ 
50     public static Connection getConnection() throws SQLException{
51         return DriverManager.getConnection(url, username,password);
52     }
53     
54     /**
55     * @Method: release
56     * @Description: 释放资源，
57     *     要释放的资源包括Connection数据库连接对象，负责执行SQL命令的Statement对象，存储查询结果的ResultSet对象
58     * @Anthor:孤傲苍狼
59     *
60     * @param conn
61     * @param st
62     * @param rs
63     */ 
64     public static void release(Connection conn,Statement st,ResultSet rs){
65         if(rs!=null){
66             try{
67                 //关闭存储查询结果的ResultSet对象
68                 rs.close();
69             }catch (Exception e) {
70                 e.printStackTrace();
71             }
72             rs = null;
73         }
74         if(st!=null){
75             try{
76                 //关闭负责执行SQL命令的Statement对象
77                 st.close();
78             }catch (Exception e) {
79                 e.printStackTrace();
80             }
81         }
82         
83         if(conn!=null){
84             try{
85                 //关闭Connection数据库连接对象
86                 conn.close();
87             }catch (Exception e) {
88                 e.printStackTrace();
89             }
90         }
91     }
92 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 2.2、使用statement对象完成对数据库的CRUD操作

　　测试代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.demo;
  2 
  3 import java.sql.Connection;
  4 import java.sql.ResultSet;
  5 import java.sql.Statement;
  6 import me.gacl.utils.JdbcUtils;
  7 
  8 import org.junit.Test;
  9 
 10 /**
 11 * @ClassName: JdbcCRUDByStatement
 12 * @Description: 通过Statement对象完成对数据库的CRUD操作
 13 * @author: 孤傲苍狼
 14 * @date: 2014-9-15 下午11:22:12
 15 *
 16 */ 
 17 public class JdbcCRUDByStatement {
 18 
 19     @Test
 20     public void insert(){
 21         Connection conn = null;
 22         Statement st = null;
 23         ResultSet rs = null;
 24         try{
 25             //获取一个数据库连接
 26             conn = JdbcUtils.getConnection();
 27             //通过conn对象获取负责执行SQL命令的Statement对象
 28             st = conn.createStatement();
 29             //要执行的SQL命令
 30             String sql = "insert into users(id,name,password,email,birthday) values(3,'白虎神皇','123','bhsh@sina.com','1980-09-09')";
 31             //执行插入操作，executeUpdate方法返回成功的条数
 32             int num = st.executeUpdate(sql);
 33             if(num>0){
 34                 System.out.println("插入成功！！");
 35             }
 36             
 37         }catch (Exception e) {
 38             e.printStackTrace();
 39         }finally{
 40             //SQL执行完成之后释放相关资源
 41             JdbcUtils.release(conn, st, rs);
 42         }
 43     }
 44     
 45     @Test
 46     public void delete(){
 47         Connection conn = null;
 48         Statement st = null;
 49         ResultSet rs = null;
 50         try{
 51             conn = JdbcUtils.getConnection();
 52             String sql = "delete from users where id=3";
 53             st = conn.createStatement();
 54             int num = st.executeUpdate(sql);
 55             if(num>0){
 56                 System.out.println("删除成功！！");
 57             }
 58         }catch (Exception e) {
 59             e.printStackTrace();
 60             
 61         }finally{
 62             JdbcUtils.release(conn, st, rs);
 63         }
 64     }
 65     
 66     @Test
 67     public void update(){
 68         Connection conn = null;
 69         Statement st = null;
 70         ResultSet rs = null;
 71         try{
 72             conn = JdbcUtils.getConnection();
 73             String sql = "update users set name='孤傲苍狼',email='gacl@sina.com' where id=3";
 74             st = conn.createStatement();
 75             int num = st.executeUpdate(sql);
 76             if(num>0){
 77                 System.out.println("更新成功！！");
 78             }
 79         }catch (Exception e) {
 80             e.printStackTrace();
 81             
 82         }finally{
 83             JdbcUtils.release(conn, st, rs);
 84         }
 85     }
 86     
 87     @Test
 88     public void find(){
 89         Connection conn = null;
 90         Statement st = null;
 91         ResultSet rs = null;
 92         try{
 93             conn = JdbcUtils.getConnection();
 94             String sql = "select * from users where id=3";
 95             st = conn.createStatement();
 96             rs = st.executeQuery(sql);
 97             if(rs.next()){
 98                 System.out.println(rs.getString("name"));
 99             }
100         }catch (Exception e) {
101             e.printStackTrace();
102         }finally{
103             JdbcUtils.release(conn, st, rs);
104         }
105     }
106 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 三、PreparedStatement对象介绍

　　PreperedStatement是Statement的子类，它的实例对象可以通过调用Connection.preparedStatement()方法获得，相对于Statement对象而言：PreperedStatement可以避免SQL注入的问题。
　　Statement会使数据库频繁编译SQL，可能造成数据库缓冲区溢出。PreparedStatement可对SQL进行预编译，从而提高数据库的执行效率。并且PreperedStatement对于sql中的参数，允许使用占位符的形式进行替换，简化sql语句的编写。

### 3.1、使用PreparedStatement对象完成对数据库的CRUD操作

　　测试代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.demo;
  2 
  3 import java.sql.Connection;
  4 import java.sql.PreparedStatement;
  5 import java.sql.ResultSet;
  6 import java.util.Date;
  7 import me.gacl.utils.JdbcUtils;
  8 import org.junit.Test;
  9 
 10 /**
 11 * @ClassName: JdbcCRUDByPreparedStatement
 12 * @Description: 通过PreparedStatement对象完成对数据库的CRUD操作
 13 * @author: 孤傲苍狼
 14 * @date: 2014-9-15 下午11:21:42
 15 *
 16 */ 
 17 public class JdbcCRUDByPreparedStatement {
 18 
 19     @Test
 20     public void insert(){
 21         Connection conn = null;
 22         PreparedStatement st = null;
 23         ResultSet rs = null;
 24         try{
 25             //获取一个数据库连接
 26             conn = JdbcUtils.getConnection();
 27             //要执行的SQL命令，SQL中的参数使用?作为占位符
 28             String sql = "insert into users(id,name,password,email,birthday) values(?,?,?,?,?)";
 29             //通过conn对象获取负责执行SQL命令的prepareStatement对象
 30             st = conn.prepareStatement(sql);
 31             //为SQL语句中的参数赋值，注意，索引是从1开始的
 32             /**
 33              * SQL语句中各个字段的类型如下：
 34              *  +----------+-------------+
 35                 | Field    | Type        |
 36                 +----------+-------------+
 37                 | id       | int(11)     |
 38                 | name     | varchar(40) |
 39                 | password | varchar(40) |
 40                 | email    | varchar(60) |
 41                 | birthday | date        |
 42                 +----------+-------------+
 43              */
 44             st.setInt(1, 1);//id是int类型的
 45             st.setString(2, "白虎神皇");//name是varchar(字符串类型)
 46             st.setString(3, "123");//password是varchar(字符串类型)
 47             st.setString(4, "bhsh@sina.com");//email是varchar(字符串类型)
 48             st.setDate(5, new java.sql.Date(new Date().getTime()));//birthday是date类型
 49             //执行插入操作，executeUpdate方法返回成功的条数
 50             int num = st.executeUpdate();
 51             if(num>0){
 52                 System.out.println("插入成功！！");
 53             }
 54             
 55         }catch (Exception e) {
 56             e.printStackTrace();
 57         }finally{
 58             //SQL执行完成之后释放相关资源
 59             JdbcUtils.release(conn, st, rs);
 60         }
 61     }
 62     
 63     @Test
 64     public void delete(){
 65         Connection conn = null;
 66         PreparedStatement st = null;
 67         ResultSet rs = null;
 68         try{
 69             conn = JdbcUtils.getConnection();
 70             String sql = "delete from users where id=?";
 71             st = conn.prepareStatement(sql);
 72             st.setInt(1, 1);
 73             int num = st.executeUpdate();
 74             if(num>0){
 75                 System.out.println("删除成功！！");
 76             }
 77         }catch (Exception e) {
 78             e.printStackTrace();
 79         }finally{
 80             JdbcUtils.release(conn, st, rs);
 81         }
 82     }
 83     
 84     @Test
 85     public void update(){
 86         Connection conn = null;
 87         PreparedStatement st = null;
 88         ResultSet rs = null;
 89         try{
 90             conn = JdbcUtils.getConnection();
 91             String sql = "update users set name=?,email=? where id=?";
 92             st = conn.prepareStatement(sql);
 93             st.setString(1, "gacl");
 94             st.setString(2, "gacl@sina.com");
 95             st.setInt(3, 2);
 96             int num = st.executeUpdate();
 97             if(num>0){
 98                 System.out.println("更新成功！！");
 99             }
100         }catch (Exception e) {
101             e.printStackTrace();
102             
103         }finally{
104             JdbcUtils.release(conn, st, rs);
105         }
106     }
107     
108     @Test
109     public void find(){
110         Connection conn = null;
111         PreparedStatement st = null;
112         ResultSet rs = null;
113         try{
114             conn = JdbcUtils.getConnection();
115             String sql = "select * from users where id=?";
116             st = conn.prepareStatement(sql);
117             st.setInt(1, 1);
118             rs = st.executeQuery();
119             if(rs.next()){
120                 System.out.println(rs.getString("name"));
121             }
122         }catch (Exception e) {
123             
124         }finally{
125             JdbcUtils.release(conn, st, rs);
126         }
127     }
128 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 　　以上就是使用JDBC对数据库进行CRUD的简单总结。



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)