# Java 学习笔记01：使用JDBC连接MYSQL数据库 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月12日 15:36:26[initphp](https://me.csdn.net/initphp)阅读数：6105
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- JDBC是什么？

> 
JDBC是一种用于执行SQL语句的Java API，可以为多种关系数据库提供统一访问，它由一组用JAVA编写的类和接口组成。说白了就和PHP的PDO一样，通过JDBC，JAVA可以使用相同的API接口来链接例如：MYSQL,SQLSERVER等不同类型的数据库。

- 连接MYSQL数据库：

```java
package main;

import java.sql.Connection;
import java.sql.DriverManager;

public class Mysql {
	public static void main(String arg[]) {
		try {
			Connection con = null; //定义一个MYSQL链接对象
			Class.forName("com.mysql.jdbc.Driver").newInstance(); //MYSQL驱动
			con = DriverManager.getConnection("jdbc:mysql://127.0.0.1:3306/test", "root", "root"); //链接本地MYSQL
			System.out.print("yes");
		} catch (Exception e) {
			System.out.print("MYSQL ERROR:" + e.getMessage());
		}
		
	}
}
```
- Class.forName("com.mysql.jdbc.Driver").newInstance();  我们链接的是MYSQL数据库，所以需要一个MYSQL的数据库驱动，如果你的环境中木有安装，可以下载：mysql-connector-java-5.1.17-bin.jar JAR包，然后放进jdk1.6.0_37\jre\lib\ext 重启eclispe 就可以在JRE系统库中看到。
- con = DriverManager.getConnection("jdbc:mysql://127.0.0.1:3306/test", "root", "root");  是链接数据库的语句，返回Connection con对象。参数格式：("jdbc:mysql://ip:端口/数据库名称", 用户名,密码)
-  写入一条数据

```java
package main;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

public class Mysql {
	
	/**
	 * 入口函数
	 * @param arg
	 */
	public static void main(String arg[]) {
		try {
			Connection con = null; //定义一个MYSQL链接对象
			Class.forName("com.mysql.jdbc.Driver").newInstance(); //MYSQL驱动
			con = DriverManager.getConnection("jdbc:mysql://127.0.0.1:3306/test", "root", "root"); //链接本地MYSQL
			
			Statement stmt; //创建声明
			stmt = con.createStatement();
			
			//新增一条数据
			stmt.executeUpdate("INSERT INTO user (username, password) VALUES ('init', '123456')");
			ResultSet res = stmt.executeQuery("select LAST_INSERT_ID()");
			int ret_id;
			if (res.next()) {
				ret_id = res.getInt(1);
				System.out.print(ret_id);
			}
			
			
			
		} catch (Exception e) {
			System.out.print("MYSQL ERROR:" + e.getMessage());
		}
		
	}
}
```
- stmt.executeUpdate  INSERT DELETE UPDATE语句都用executeUpdate函数来操作
- stmt.executeQuery SELECT语句都用stmt.executeQuery函数来操作
- ResultSet res = stmt.executeQuery("select LAST_INSERT_ID()"); 查询最后插入数据的ID号，返回ResultSet res对象
- 删除和更新数据

```java
package main;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

public class Mysql {
	
	/**
	 * 入口函数
	 * @param arg
	 */
	public static void main(String arg[]) {
		try {
			Connection con = null; //定义一个MYSQL链接对象
			Class.forName("com.mysql.jdbc.Driver").newInstance(); //MYSQL驱动
			con = DriverManager.getConnection("jdbc:mysql://127.0.0.1:3306/test", "root", "root"); //链接本地MYSQL
			
			Statement stmt; //创建声明
			stmt = con.createStatement();
			
			//新增一条数据
			stmt.executeUpdate("INSERT INTO user (username, password) VALUES ('init', '123456')");
			ResultSet res = stmt.executeQuery("select LAST_INSERT_ID()");
			int ret_id;
			if (res.next()) {
				ret_id = res.getInt(1);
				System.out.print(ret_id);
			}
			
			//删除一条数据
			String sql = "DELETE FROM user WHERE id = 1";
			long deleteRes = stmt.executeUpdate(sql); //如果为0则没有进行删除操作，如果大于0，则记录删除的条数
			System.out.print("DELETE:" + deleteRes);
			
			//更新一条数据
			String updateSql = "UPDATE user SET username = 'xxxx' WHERE id = 2";
			long updateRes = stmt.executeUpdate(updateSql);
			System.out.print("UPDATE:" + updateRes);
			
			
			
		} catch (Exception e) {
			System.out.print("MYSQL ERROR:" + e.getMessage());
		}
		
	}
}
```
- 删除和更新数据都使用stmt.executeUpdate函数。
- 删除和更新数据都会返回一个Long的结果，如果为0，则删除或者更新失败，如果大于0则是操作的记录数

- 查询语句

```java
package main;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.Statement;

public class Mysql {
	
	/**
	 * 入口函数
	 * @param arg
	 */
	public static void main(String arg[]) {
		try {
			Connection con = null; //定义一个MYSQL链接对象
			Class.forName("com.mysql.jdbc.Driver").newInstance(); //MYSQL驱动
			con = DriverManager.getConnection("jdbc:mysql://127.0.0.1:3306/test", "root", "root"); //链接本地MYSQL
			
			Statement stmt; //创建声明
			stmt = con.createStatement();
			
			//新增一条数据
			stmt.executeUpdate("INSERT INTO user (username, password) VALUES ('init', '123456')");
			ResultSet res = stmt.executeQuery("select LAST_INSERT_ID()");
			int ret_id;
			if (res.next()) {
				ret_id = res.getInt(1);
				System.out.print(ret_id);
			}
			
			//删除一条数据
			String sql = "DELETE FROM user WHERE id = 1";
			long deleteRes = stmt.executeUpdate(sql); //如果为0则没有进行删除操作，如果大于0，则记录删除的条数
			System.out.print("DELETE:" + deleteRes);
			
			//更新一条数据
			String updateSql = "UPDATE user SET username = 'xxxx' WHERE id = 2";
			long updateRes = stmt.executeUpdate(updateSql);
			System.out.print("UPDATE:" + updateRes);
			
			//查询数据并输出
			String selectSql = "SELECT * FROM user";
			ResultSet selectRes = stmt.executeQuery(selectSql);
			while (selectRes.next()) { //循环输出结果集
				String username = selectRes.getString("username");
				String password = selectRes.getString("password");
				System.out.print("\r\n\r\n");
				System.out.print("username:" + username + "password:" + password);
			}
			
			
		} catch (Exception e) {
			System.out.print("MYSQL ERROR:" + e.getMessage());
		}
		
	}
}
```
- 查询语句使用stmt.executeQuery函数
- 
rs.absolute() //绝对位置，负数表示从后面数

rs.first()第一条

rs.last()最后一条

rs.previoust()前一条

rs.next()后一条

rs.beforeFirst()第一条之前

rs.afterLast()最后之后

rs.isFirst(),rs.isLast(),rs.isBeforeFirst(),rs.isAfterLast






