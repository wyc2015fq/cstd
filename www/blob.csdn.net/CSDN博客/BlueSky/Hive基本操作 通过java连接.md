# Hive基本操作 通过java连接 - BlueSky - CSDN博客
2016年03月14日 09:46:03[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：3471
## 1、启动服务
`#hive --service hiveserver2 >/dev/null  2>/dev/null &`以上命令启动hiveserver2服务。
Hive提供了jdbc驱动，使得我们可以用java代码来连接Hive并进行一些类关系型数据库的sql语句查询等操作。首先，我们必须将Hive的服务，也就是HiveServe打开。在Hive 0.11.0版本前，只有HiveServer服务可用，但是HiveServer本身存在很多问题（比如：安全性、并发性等）；针对这些问题，Hive-0.11.0版本后提供了一个全新的服务：HiveServer2,这个很好的解决HiveServer存在的安全性、并发性等问题。
如果启动hiveserver就把上面命令改为
`#hive --service hiveserver >/dev/null  2>/dev/null &`
## 2、java连接程序
```
public class HiveApp {
	public static void main(String[] args) throws Exception{
		Class.forName("org.apache.hive.jdbc.HiveDriver");
		//default为数据库名
		Connection con = DriverManager.getConnection("jdbc:hive2://master:10000/", "root", "root");
		Statement stmt = con.createStatement();
		String querySQL="SELECT * FROM default.t1";
	
		ResultSet res = stmt.executeQuery(querySQL);  
	
		while (res.next()) {
		System.out.println(res.getInt(1));
		}
	}
}
```
上面代码是针对hiveserver的。如果是hiveserver2。那有两处需要修改，具体修改如下：
org.apache.hive.jdbc.HiveDriver
改为：
org.apache.hadoop.hive.jdbc.HiveDriver
jdbc:hive2://localhost:10000/default
改为：
jdbc:hive://localhost:10000/default
其中’localhost’是主机地址，10000是端口后，default是默认的db。

