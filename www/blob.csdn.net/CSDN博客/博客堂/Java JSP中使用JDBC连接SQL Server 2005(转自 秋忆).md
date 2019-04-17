# Java/JSP中使用JDBC连接SQL Server 2005(转自 秋忆) - 博客堂 - CSDN博客





2015年04月21日 00:13:55[最小森林](https://me.csdn.net/u012052268)阅读数：482







接触Java或者JSP，难免会使用到数据库SQL Server 2000/2005（我使用2005标准版[9.0.3054]测试），经过自己的搜索和研究，使用JDBC连接SQL Server成功，特此整理方法（使用Eclipse 3.2）如下。

准备工作

首先，操作系统中安装好SQL Server 2000/2005，如果系统中都装有2000和2005版，记得停用一个，只开一个行了。

然后，到微软网站下载[Microsoft
 SQL Server 2005 JDBC Driver 1.1](http://www.microsoft.com/downloads/details.aspx?FamilyID=6d483869-816a-44cb-9787-a866235efc7c&DisplayLang=zh-cn)，也可以使用这个地址[直接下载](http://download.microsoft.com/download/2/8/9/289dd6a3-eeeb-46dc-9045-d0c6b59bfbc1/sqljdbc_1.1.1501.101_chs.exe)。

解压sqljdbc_1.1.1501.101_chs.exe，把sqljdbc_1.1复制到%ProgramFiles%（如果系统在C盘则为C:\Program Files）。

设置 Classpath

JDBC 驱动程序并未包含在 Java SDK 中。因此，如果要使用该驱动程序，必须将 classpath 设置为包含 sqljdbc.jar 文件。如果 classpath 缺少 sqljdbc.jar 项，应用程序将引发“找不到类”的常见异常。

sqljdbc.jar 文件的安装位置如下：


<安装目录>\sqljdbc_<版本>\<语言>\sqljdbc.jar


下面是用于 Windows 应用程序的 CLASSPATH 语句实例：

`CLASSPATH =.;`%ProgramFiles%`\sqljdbc_1.1\chs\sqljdbc.jar`


下面是用于 Unix/Linux 应用程序的 CLASSPATH 语句实例：

`CLASSPATH =.:/home/usr1/mssqlserver2005jdbc/Driver/sqljdbc_1.1/chs/sqljdbc.jar`

注意： 在 Window 系统中，如果目录名长于 8.3 或文件夹名中包含空格，将导致 classpath 出现问题。如果怀疑存在这类问题，应暂时将 sqljdbc.jar 文件移动到名称简单的目录中，例如 `C:\Temp`，更改 classpath，然后测试这样是否解决了问题。

# 直接在命令提示符运行的应用程序



在操作系统中配置 classpath。将 sqljdbc.jar 追加到系统的 classpath 中。或者，使用 `java -classpath `选项，可以在运行此应用程序的 Java 命令行上指定 classpath。


设置SQL Server服务器

我使用的是SQL Server 2005标准版SP2，这些都默认即可，一般不用配置。如果需要配置端口请看下面。

1、“开始”→“程序”→“Microsoft SQL Server 2005”→“配置工具”→“SQL Server 配置管理器”→“SQL Server 2005 网络配置”→“MSSQLSERVER 的协议”
2、如果“TCP/IP”没有启用，右键单击选择“启动”。
3、双击“TCP/IP”进入属性设置，在“IP 地址”里，可以配置“IPAll”中的“TCP 端口”，默认为1433。
4、重新启动SQL Server或者重启计算机。


创建数据库

打开“SQL Server Management Studio”，登录连接SQL Server服务器，新建数据库，命名为test


在Eclipse中测试

1、打开Eclipse，“文件”→“新建”→“项目”→“Java 项目”，项目名为Test
2、在Eclipse中，选择“窗口”→“首选项...”→“Java”→“已安装的 JRE”，选择已安装的 JRE，点击“编辑”→“添加外部”，选择%ProgramFiles%
```
\sqljdbc_1.1\chs\sqljdbc.jar

3、在Test项目的“JRE 系统库”中可以看见
```
`sqljdbc.jar，如果没有可以`右键单击项目Test→“构建路径”→“配置构建路径...”→“Java
 构建路径”→“库”→“添加外部 JAR...”，选择%ProgramFiles%
```
\sqljdbc_1.1\chs\sqljdbc.jar

4、
```
编写Java代码，如下：

import java.sql.*;

public class Test {

public static void main(String[] srg) {

  String driverName = "com.microsoft.sqlserver.jdbc.SQLServerDriver";  //加载JDBC驱动

  String dbURL = "jdbc:sqlserver://localhost:1433; DatabaseName=test";  //连接服务器和数据库test

  String userName = "sa";  //默认用户名

  String userPwd = "123456";  //密码

  Connection dbConn;
  try {
   Class.forName(driverName);
   dbConn = DriverManager.getConnection(dbURL, userName, userPwd);
   System.out.println("Connection Successful!");  //如果连接成功 控制台输出Connection Successful!
  } catch (Exception e) {
   e.printStackTrace();
  }
}
}


```

```
注：

1、因为SQL Express这个版本的服务器默认是禁用的并且端口号没有配置，所以要进行重新设置


2、如果你以前用Java连接SQL Server 2000的话就要注意了：


在SQL Server 2000 中加载驱动和URL路径的语句是


String driverName = "com.microsoft.jdbc.sqlserver.SQLServerDriver";

String dbURL = "jdbc:microsoft:sqlserver://localhost:1433;
 DatabaseName=sample";


而SQL Server 2005 中加载驱动和URL的语句则为


String driverName = "com.microsoft.sqlserver.jdbc.SQLServerDriver";

String dbURL = "jdbc:sqlserver://localhost:1433; DatabaseName=sample";
如果写法错误将会找不到驱动。

秋忆：对于Microsoft SQL Server 2005 JDBC Driver 1.1，据我看官方文档就没有提及2000与2005的差别，相信都是按照2005的语句写法，我没有测试2000，如果朋友你测试到不一样请告诉我，谢谢！


其它参考资料：
[http://dev.csdn.net/author/yeno/3f06bf19b1c147198e00afa0af6bd0dc.html](http://dev.csdn.net/author/yeno/3f06bf19b1c147198e00afa0af6bd0dc.html)
[http://www.baidu.com/s?wd=JDBC+for+SQL+Server+2005&cl=3](http://www.baidu.com/s?wd=JDBC+for+SQL+Server+2005&cl=3)


