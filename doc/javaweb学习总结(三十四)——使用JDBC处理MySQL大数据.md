## [javaweb学习总结(三十四)——使用JDBC处理MySQL大数据](https://www.cnblogs.com/xdp-gacl/p/3982581.html)

## 一、基本概念

　　**大数据也称之为LOB(Large Objects)，LOB又分为：clob和blob，clob用于存储大文本，blob用于存储二进制数据**，例如图像、声音、二进制文等。

　　在实际开发中，有时是需要用程序把大文本或二进制数据直接保存到数据库中进行储存的。

　　对MySQL而言只有blob，而没有clob，mysql存储大文本采用的是Text，Text和blob分别又分为：
　　TINYTEXT、TEXT、MEDIUMTEXT和LONGTEXT
　　TINYBLOB、BLOB、MEDIUMBLOB和LONGBLOB

## 二、搭建测试环境

### 2.1、搭建的测试项目架构

　　如下：

　　![img](data:image/png;base64,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)

### 2.2、编写db.properties配置文件

```
1 driver=com.mysql.jdbc.Driver
2 url=jdbc:mysql://localhost:3306/jdbcStudy
3 username=root
4 password=XDP
```

### 2.3、编写JdbcUtils工具类

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

## 三、使用JDBC处理MySQL的大文本

　　对于MySQL中的Text类型，可调用如下方法设置

```
1 PreparedStatement.setCharacterStream(index, reader, length);//注意length长度须设置，并且设置为int型
```

　　对MySQL中的Text类型，可调用如下方法获取

```
1 reader = resultSet. getCharacterStream(String columnLabel);2 string s = resultSet.getString(String columnLabel);
```

### 3.1、 测试范例

　　1、编写SQL测试脚本

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 create database jdbcstudy;
2 use jdbcstudy;
3 create table testclob
4 (
5          id int primary key auto_increment,
6          resume text
7 );
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　2、编写测试代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.demo;
  2 
  3 import java.io.File;
  4 import java.io.FileReader;
  5 import java.io.FileWriter;
  6 import java.io.Reader;
  7 import java.sql.Connection;
  8 import java.sql.PreparedStatement;
  9 import java.sql.ResultSet;
 10 import me.gacl.utils.JdbcUtils;
 11 import org.junit.Test;
 12 
 13 /**
 14 * @ClassName: JdbcOperaClob
 15 * @Description: 使用JDBC操作MySQL的大文本
 16 * @author: 孤傲苍狼
 17 * @date: 2014-9-19 下午10:10:04
 18 *
 19 */ 
 20 public class JdbcOperaClob {
 21 
 22     /**
 23     * @Method: add
 24     * @Description:向数据库中插入大文本数据
 25     * @Anthor:孤傲苍狼
 26     *
 27     */ 
 28     @Test
 29     public void add(){
 30         Connection conn = null;
 31         PreparedStatement st = null;
 32         ResultSet rs = null;
 33         Reader reader = null;
 34         try{
 35             conn = JdbcUtils.getConnection();
 36             String sql = "insert into testclob(resume) values(?)";
 37             st = conn.prepareStatement(sql);
 38             //这种方式获取的路径，其中的空格会被使用“%20”代替
 39             String path = JdbcOperaClob.class.getClassLoader().getResource("data.txt").getPath();
 40             //将“%20”替换回空格
 41             path = path.replaceAll("%20", " ");
 42             File file = new File(path);
 43             reader = new FileReader(file);
 44             st.setCharacterStream(1, reader,(int) file.length());
 45             int num = st.executeUpdate();
 46             if(num>0){
 47                 System.out.println("插入成功！！");
 48             }
 49             //关闭流
 50             reader.close();
 51         }catch (Exception e) {
 52             e.printStackTrace();
 53         }finally{
 54             JdbcUtils.release(conn, st, rs);
 55         }
 56     }
 57     
 58     /**
 59     * @Method: read
 60     * @Description: 读取数据库中的大文本数据
 61     * @Anthor:孤傲苍狼
 62     *
 63     */ 
 64     @Test
 65     public void read(){
 66         Connection conn = null;
 67         PreparedStatement st = null;
 68         ResultSet rs = null;
 69         try{
 70             conn = JdbcUtils.getConnection();
 71             String sql = "select resume from testclob where id=2";
 72             st = conn.prepareStatement(sql);
 73             rs = st.executeQuery();
 74             
 75             String contentStr ="";
 76             String content = "";
 77             if(rs.next()){
 78                 //使用resultSet.getString("字段名")获取大文本数据的内容
 79                 content = rs.getString("resume");
 80                 //使用resultSet.getCharacterStream("字段名")获取大文本数据的内容
 81                 Reader reader = rs.getCharacterStream("resume");
 82                 char buffer[] = new char[1024];
 83                 int len = 0;
 84                 FileWriter out = new FileWriter("D:\\1.txt");
 85                 while((len=reader.read(buffer))>0){
 86                     contentStr += new String(buffer);
 87                     out.write(buffer, 0, len);
 88                 }
 89                 out.close();
 90                 reader.close();
 91             }
 92             System.out.println(content);
 93             System.out.println("-----------------------------------------------");
 94             System.out.println(contentStr);
 95         }catch (Exception e) {
 96             e.printStackTrace();
 97         }finally{
 98             JdbcUtils.release(conn, st, rs);
 99         }
100     }
101 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 四、使用JDBC处理MySQL的二进制数据

　　对于MySQL中的BLOB类型，可调用如下方法设置：

```
1 PreparedStatement. setBinaryStream(i, inputStream, length);
```

　　对MySQL中的BLOB类型，可调用如下方法获取：

```
1 InputStream in  = resultSet.getBinaryStream(String columnLabel);
2 InputStream in  = resultSet.getBlob(String columnLabel).getBinaryStream(); 
```

###  4.1、 测试范例

　　1、编写SQL测试脚本

```
1 create table testblob
2 (
3      id int primary key auto_increment,
4      image longblob
5 );
```

　　2、编写测试代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.demo;
 2 
 3 import java.io.File;
 4 import java.io.FileInputStream;
 5 import java.io.FileOutputStream;
 6 import java.io.InputStream;
 7 import java.sql.Connection;
 8 import java.sql.PreparedStatement;
 9 import java.sql.ResultSet;
10 import me.gacl.utils.JdbcUtils;
11 import org.junit.Test;
12 
13 /**
14 * @ClassName: JdbcOperaClob
15 * @Description: 使用JDBC操作MySQL的二进制数据(例如图像、声音、二进制文)
16 * @author: 孤傲苍狼
17 * @date: 2014-9-19 下午10:10:04
18 *
19 */ 
20 public class JdbcOperaBlob {
21 
22     /**
23     * @Method: add
24     * @Description:向数据库中插入二进制数据
25     * @Anthor:孤傲苍狼
26     *
27     */ 
28     @Test
29     public void add(){
30         Connection conn = null;
31         PreparedStatement st = null;
32         ResultSet rs = null;
33         try{
34             conn = JdbcUtils.getConnection();
35             String sql = "insert into testblob(image) values(?)";
36             st = conn.prepareStatement(sql);
37             //这种方式获取的路径，其中的空格会被使用“%20”代替
38             String path = JdbcOperaBlob.class.getClassLoader().getResource("01.jpg").getPath();
39             //将“%20”替换会空格
40             path = path.replaceAll("%20", " ");
41             File file = new File(path);
42             FileInputStream fis = new FileInputStream(file);//生成的流
43             st.setBinaryStream(1, fis,(int) file.length());
44             int num = st.executeUpdate();
45             if(num>0){
46                 System.out.println("插入成功！！");
47             }
48             fis.close();
49         }catch (Exception e) {
50             e.printStackTrace();
51         }finally{
52             JdbcUtils.release(conn, st, rs);
53         }
54     }
55     
56     /**
57     * @Method: read
58     * @Description: 读取数据库中的二进制数据
59     * @Anthor:孤傲苍狼
60     *
61     */ 
62     @Test
63     public void read() {
64         Connection conn = null;
65         PreparedStatement st = null;
66         ResultSet rs = null;
67         try {
68             conn = JdbcUtils.getConnection();
69             String sql = "select image from testblob where id=?";
70             st = conn.prepareStatement(sql);
71             st.setInt(1, 1);
72             rs = st.executeQuery();
73             if (rs.next()) {
74                 //InputStream in = rs.getBlob("image").getBinaryStream();//这种方法也可以
75                 InputStream in = rs.getBinaryStream("image");
76                 int len = 0;
77                 byte buffer[] = new byte[1024];
78                 
79                 FileOutputStream out = new FileOutputStream("D:\\1.jpg");
80                 while ((len = in.read(buffer)) > 0) {
81                     out.write(buffer, 0, len);
82                 }
83                 in.close();
84                 out.close();
85             }
86         } catch (Exception e) {
87             e.printStackTrace();
88         } finally {
89             JdbcUtils.release(conn, st, rs);
90         }
91     }
92 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　关于使用JDBC处理MySQL大数据的内容就总结这么多！



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)