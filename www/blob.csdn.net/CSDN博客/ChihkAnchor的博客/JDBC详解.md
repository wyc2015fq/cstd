# JDBC详解 - ChihkAnchor的博客 - CSDN博客





2019年02月09日 18:16:12[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：45标签：[java																[jdbc](https://so.csdn.net/so/search/s.do?q=jdbc&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/weixin_40871455/article/category/8560856)








JDBC:(java database connectivity)  SUN公司提供的一套操作数据库的标准规范，是一种用于执行SQL语句的Java API

JDBC与数据库驱动的关系：接口与实现的关系。

![](https://img-blog.csdnimg.cn/20190209165540946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

### JDBC中常用类和接口

连接到数据库（Connection）、建立操作指令（Statement）、执行查询指令（executeQuery）、获得查询结果（ResultSet）

### 1、驱动程序管理类（DriverManager）

DriverManager类是JDBC的管理类，作用于用户和驱动程序之间。它在数据库和相应驱动程序之间建立连接，DriverManager类处理诸如驱动程序登陆时间限制及登录和跟踪消息的显示事务。JDBC允许用户调用DriverManager的方法getDriver()、getDrivers()和registerDriver()及Driver的方法connect().

### 2、数据库连接类 （Connection）

Connection对象代表与数据库的链接。连接过程包括所执行的SQL语句和在该连接上所返回的结果。一个应用程序可与单个数据库有一个或多个连接，或者可与很多数据库有连接。打开连接与数据库建立连接的标准方法是调用DriverManager.getConnection()方法。

String url="jdbc:mysql://127.0.0.1:3306/imooc";

String user="root";

String password="1234";

Connection conn = DriverManager.getConnection(url,user,password);

### 3、声明类（Statement）

Statement对象用于将SQL语句发送到数据库中。实际上有三种Statement对象，它们都作为在给定链接上执行SQL语句的包容器：Statement、PreparedStatement（它从Statement继承而来）和CallableStatement（它从PreparedStatement继承而来）。它们都专用于发送特定类型的SQL语句：

（1）Statement对象用于执行不带参数的简单的SQL语句；Statement接口提供了执行语句和获取结果的基本方法。

（2）PerparedStatement对象用于发送含有一个或多个参数的SQL语句；PreparedStatement对象比Statement对象的效率更高，并且可以防止SQL注入，所以我们一般都使用PreparedStatement。

（3）CallableStatement继承自PreparedStatement接口，由方法prepareCall创建，用于调用存储过程。

Statement提供了许多方法，最常用的方法如下：

（1）execute()方法：运行语句，返回是否有结果集。

（2）executeQuery()方法：运行查询语句，返回ReaultSet对象。

（3）executeUpdata()方法：运行更新操作，返回更新的行数。

（4）addBatch()方法：增加批处理语句。

（5）executeBatch()方法：执行批处理语句。

（6）clearBatch()方法：清除批处理语句。

### 4、结果集合类 （ResultSet）

ResultSet提供检索不同类型字段的方法，常用的有：
- getString(int index)、getString(String columnName)：获得在数据库里是varchar、char等类型的数据对象。
- getFloat(int index)、getFloat(String columnName)：获得在数据库里是Float类型的数据对象。
- getDate(int index)、getDate(String columnName)：获得在数据库里是Date类型的数据。
- getBoolean(int index)、getBoolean(String columnName)：获得在数据库里是Boolean类型的数据。
- getObject(int index)、getObject(String columnName)：获取在数据库里任意类型的数据。

ResultSet还提供了对结果集进行滚动的方法：
- next()：移动到下一行
- Previous()：移动到前一行
- absolute(int row)：移动到指定行
- beforeFirst()：移动resultSet的最前面。
- afterLast() ：移动到resultSet的最后面。

使用后依次关闭对象及连接：ResultSet → Statement → Connection

代码示例：

```java
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import com.entity.User;


public class TestCRUD {
	
	@Test
	public void testSelect() throws Exception{
		//加载驱动
		Class.forName("com.mysql.jdbc.Driver");
		
		//获取连接Connection
		Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/jdbcTest", "root", "1234");
		//得到执行sql语句的对象Statement
		Statement stmt = conn.createStatement();
		//执行sql语句，并返回结果
		ResultSet rs = stmt.executeQuery("select password,email,birthday,id,name from users");
		List<User> list = new ArrayList<User>();
		//处理结果 
		while(rs.next()){ 
			User u = new User();
			u.setId(rs.getInt("id"));
			u.setName(rs.getString("name"));
			u.setPassword(rs.getString("password"));
			u.setEmail(rs.getString("email"));
			u.setBirthday(rs.getDate("birthday"));
			list.add(u);
		}
		//关闭资源
		rs.close();
		stmt.close();
		conn.close();
		
		for (User user : list) {
			System.out.println(user);
		}
	}
	
	
	@Test
	public void testInsert() throws Exception{
		//加载驱动
		Class.forName("com.mysql.jdbc.Driver");
		//获取连接Connection
		Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/jdbcTest?user=root&password=abc");
		//得到执行sql语句的对象Statement
		Statement stmt = conn.createStatement();
		//执行sql语句，并返回结果
		int i = stmt.executeUpdate("INSERT INTO users VALUES(4,'tom','123','tom@163.com','2018-12-28')");
		if(i>0){
			System.out.println("success");
		}
		
		//关闭资源
		stmt.close();
		conn.close();
	}
	
	@Test
	public void testUpdate() throws Exception{
		//加载驱动
		Class.forName("com.mysql.jdbc.Driver");
		//获取连接Connection
		Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/jabcTest?user=root&password=1234");
		//得到执行sql语句的对象Statement
		Statement stmt = conn.createStatement();
		//执行sql语句，并返回结果
		int i = stmt.executeUpdate("UPDATE users SET NAME='jerry',PASSWORD='333',email='jerry@163.com' WHERE id=3");
		if(i>0){
			System.out.println("success"+" 修改了"+i+"行");
		}else{
			System.out.println("修改了"+i+"行");
		}
		
		//关闭资源
		stmt.close();
		conn.close();
		
	}
```

User： 

```java
import java.util.Date;

public class User {
	private int id;
	private String name;
	private String password;
	private String email;
	private Date birthday;
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}
	public Date getBirthday() {
		return birthday;
	}
	public void setBirthday(Date birthday) {
		this.birthday = birthday;
	}
	@Override
	public String toString() {
		return "User [id=" + id + ", name=" + name + ", password=" + password
				+ ", email=" + email + ", birthday=" + birthday + "]";
	}
}
```






