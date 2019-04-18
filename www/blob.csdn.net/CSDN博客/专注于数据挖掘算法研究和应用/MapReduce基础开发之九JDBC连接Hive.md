# MapReduce基础开发之九JDBC连接Hive - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月19日 09:05:48[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2478
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
1、eclipse建工程，引入$HIVE_HOME/lib的包和$Hadoop_HOME/lib的包(一般核心包就可以，具体根据开发需求)，导出jar包（不带第三方jar)；

    这里是Hive2，代码如下：



```java
package com.hive;

import java.sql.SQLException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.sql.DriverManager;

public class HiveJdbc {
	private static String driverName = "org.apache.hive.jdbc.HiveDriver";

	public static void main(String[] args)  throws SQLException {
		
		try {
			Class.forName(driverName);
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.exit(1);
		}
		
		Connection con = DriverManager.getConnection("jdbc:hive2://IP:10000/default", "", "");
		Statement stmt = con.createStatement();
		String tableName = "JDBCTest";
		stmt.execute("drop table if exists " + tableName);
		stmt.execute("create table " + tableName +   " (key int, value string)");
		System.out.println("Create table success!");
		// show tables
		String sql = "show tables '" + tableName + "'";
		System.out.println("Running: " + sql);
		ResultSet res = stmt.executeQuery(sql);
		if (res.next()) {
			System.out.println(res.getString(1));
		}
		
		// describe table
		sql = "describe " + tableName;
		System.out.println("Running: " + sql);
		res = stmt.executeQuery(sql);
		while (res.next()) {
			System.out.println(res.getString(1) + "\t" + res.getString(2));
		}
		//select table
		sql = "select * from " + tableName;
		res = stmt.executeQuery(sql);
		while (res.next()) {
			System.out.println(String.valueOf(res.getInt(1)) + "\t"+ res.getString(2));
		}
	}
}
```
2、根据hadoop和hive集群部署情况，编写脚本并执行，加入hive和hadoop的jar包。

   注意：shell脚本最好在centos下编写，如果是Windows下编写因为字符问题执行起来会出现语法错误

   #vi /tmp/hivejdbc.sh



```
#!/bin/bash

HADOOP_HOME=/usr/lib/hadoop
HIVE_HOME=/usr/lib/hive
JAVA_HOME=/usr/lib/java

CLASSPATH=$CLASSPATH: 

for i in /usr/lib/hive/lib/*.jar; do 
CLASSPATH=$CLASSPATH:$i 
done

for i in /usr/lib/hadoop/client/*.jar; do 
CLASSPATH=$CLASSPATH:$i 
done

echo $CLASSPATH 
java -cp $CLASSPATH:/tmp/hivejdbc.jar com.hive.HiveJdbc
```


  执行脚本sh /tmp/hivejdbc.sh看结果。

  出现提示没有hdfs中hive用户的文件权限/user/hive/warehouse；

  执行命令hadoop fs -chmod 777 /user/hive 



3、实际上，在性能上，不建议通过jdbc连接hive来处理。



