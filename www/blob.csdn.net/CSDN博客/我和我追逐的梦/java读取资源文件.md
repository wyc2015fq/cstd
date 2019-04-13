
# java读取资源文件 - 我和我追逐的梦~~~ - CSDN博客


2014年04月23日 14:15:04[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：924


转自: http://www.blogjava.net/pdw2009/archive/2007/10/11/152182.html
如何读取资源文件：
（一）
Properties props = new Properties();
props.load(new FileInputStream("db.properties"));
（二）
blog.properties文件如下
dbdriver=oracle.jdbc.driver.OracleDriver
dburl=jdbc:oracle:thin:@127.0.0.1:1521:ora92
dbuser=blog
dbpwd=blog
--------------
public class Config {
public Config() {
this.initDBConfig();
}
public String DBdriver;
public String DBuser;
public String DBpwd;
public String DBurl;
private void initDBConfig() {
try {
ResourceBundle bundle = ResourceBundle.getBundle("blog");
DBdriver = bundle.getString("dbdriver");
DBurl = bundle.getString("dburl");
DBuser = bundle.getString("dbuser");
DBpwd = bundle.getString("dbpwd");
}
catch (Exception ex) {
ex.printStackTrace();
}
}
}
----------------
public class DAO {
public DAO() {
}
public Connection getConnection() {
Connection conn = null;
Config config = new Config();
String DBdriver = config.DBdriver;
String DBuser = config.DBuser;
String DBpwd = config.DBpwd;
String DBurl = config.DBurl;
try {
Class.forName(DBdriver);
conn = DriverManager.getConnection(DBurl, DBuser, DBpwd);
}
catch (Exception ex) {
System.out.println("********************");
System.out.println("不能得到数据库连接");
System.out.println("DBdriver: " + DBdriver);
System.out.println("DBuser: " + DBuser);
System.out.println("DBpwd: " + DBpwd);
System.out.println("DBurl: " + DBurl);
ex.printStackTrace();
}
return conn;
}
}
（三）
Properties props=new Properties();
props.load(BugFactory.class.getResourceAsStream("xx.properties"));
String name = props.getPropery("xxxx");
此时xx.properties应该与该类放在同一个目录.
（四）
ResourceBundle res = ResourceBundle.getBundle("yy.properties");
String name = res.getString("yyyy");
yy.properties应放在/WEB-INF/classes目录
（五）
如果你这个Bean打包的话，就把这个文件放在包内。
我一般是这样写的
Properties prop = new Properties();
try
{
InputStream is = getClass().getResourceAsStream("db.properties");
prop.load(is);
if(is!=null)
is.close();
}
另：
props.load(new FileInputStream("db.properties")); 是读取当前目录的db.properties文件
getClass.getResourceAsStream("db.properties"); 是读取当前类所在位置一起的db.properties文件
getClass.getResourceAsStream("/db.properties"); 是读取ClassPath的根的db.properties文件,注意ClassPath如果是多个路径或者jar文件的,只要在任意一个路径目录下或者jar文件里的根下都可以,如果存在于多个路径下的话,按照ClassPath中的先后顺序,使用先找到的,其余忽略.

