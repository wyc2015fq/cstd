# JDBC连接各种数据库方法 - weixin_33985507的博客 - CSDN博客
2006年04月24日 10:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
1、Oracle8/8i/9i数据库（thin模式） 
Class.forName("oracle.jdbc.driver.OracleDriver").newInstance(); 
String url="jdbc:oracle:thin:@localhost:1521:orcl"; 
//orcl为数据库的SID 
String user="test"; 
String password="test"; 
Connection conn= DriverManager.getConnection(url,user,password); 
2、DB2数据库 
Class.forName("com.ibm.db2.jdbc.app.DB2Driver ").newInstance(); 
String url="jdbc:db2://localhost:5000/sample"; 
//sample为你的数据库名 
String user="admin"; 
String password=""; 
Connection conn= DriverManager.getConnection(url,user,password); 
3、Sql Server7.0/2000数据库 
Class.forName("com.microsoft.jdbc.sqlserver.SQLServerDriver").newInstance(); 
String url="jdbc:microsoft:sqlserver://localhost:1433;DatabaseName=mydb"; 
//mydb为数据库 
String user="sa"; 
String password=""; 
Connection conn= DriverManager.getConnection(url,user,password); 
4、Sybase数据库 
Class.forName("com.sybase.jdbc.SybDriver").newInstance(); 
String url =" jdbc:sybase:Tds:localhost:5007/myDB"; 
//myDB为你的数据库名 
Properties sysProps = System.getProperties(); 
SysProps.put("user","userid"); 
SysProps.put("password","user_password"); 
Connection conn= DriverManager.getConnection(url, SysProps); 
5、Informix数据库 
Class.forName("com.informix.jdbc.IfxDriver").newInstance(); 
String url = 
"jdbc:informix-sqli://123.45.67.89:1533/myDB:INFORMIXSERVER=myserver; 
user=testuser;password=testpassword"; 
//myDB为数据库名 
Connection conn= DriverManager.getConnection(url); 
6、MySQL数据库 
Class.forName("org.gjt.mm.mysql.Driver").newInstance(); 
String url ="jdbc:mysql://localhost/myDB?user=soft&password=soft1234&useUnicod 
e=true&characterEncoding=8859_1" 
//myDB为数据库名 
Connection conn= DriverManager.getConnection(url); 
7、PostgreSQL数据库 
Class.forName("org.postgresql.Driver").newInstance(); 
String url ="jdbc:postgresql://localhost/myDB" 
//myDB为数据库名 
String user="myuser"; 
String password="mypassword"; 
Connection conn= DriverManager.getConnection(url,user,password); 
