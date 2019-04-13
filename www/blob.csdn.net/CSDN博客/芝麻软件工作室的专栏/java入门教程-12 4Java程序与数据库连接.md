
# java入门教程-12.4Java程序与数据库连接 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:48:16[seven-soft](https://me.csdn.net/softn)阅读数：245个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



一个网络关系数据库应用系统是一个三层次结构。客户机与服务器采用网络连接，客户机端应用程序按通信协议与服务器端的数据库程序通信；数据库服务程序通过SQL命令与数据库管理系统通信。
Java程序与数据库连接方法有两种。一种是使用JDBC-ODBC桥接器与数据库连接，一种是用纯Java的JDBC驱动程序实现与数据库连接。
## 使用JDBC-ODBC 桥接器与数据库连接
Java程序使用JDBC-ODBC 桥接器与数据库连接，Java程序与数据库通信的过程是：
先由数据库应用程序向ODBC驱动管理器发出API调用，ODBC驱动管理器将这个调用转换成向数据库管理系统的ODBC驱动程序调用，数据库管理系统又将这个调用转换成对操作系统的数据输入/输出调用。最后，操作系统从数据库中得到实际数据逐级返回。
数据库编程首先要设置数据源，在ODBC中设置数据源的步骤如下：打开Windows控制面板中的管理工具。对于windows XP：选择“性能维护”>>“管理工具”>>“数据源(ODBC)”； 对于windows 2000：选择“管理工具”>>“数据源”。
打开“数据源”。出现ODBC数据源管理器对话框，显示现有的数据源名称。
选择“用户DSN”，单击“添加”按钮，出现安装数据源驱动程序对话框。Access(*.mdb)数据源，单击“完成”按钮，出现“创建数据源对话框，键入需要创建的数据源名，并为创建的数据源选择一个数据库表。
单击数据库区域的“选择”按钮，选择需要的数据库表。当需要为数据源授权访问级别时，单击“高级”按钮。设置登录名和密码后，单击“确定”按钮，完成Access数据库在ODBC管理器中的配置。
如果还没有数据库表，则需创建一个数据库表。
数据源就是数据库，在设定了数据源的基础上，Java程序要访问数据库表，还要建立JDBC-ODBC桥接器，让程序与数据库连接。以后，程序就可向数据库发送SQL语句，处理数据库返回的结果。Java数据库连接JDBC(Java DataBase
 Connectivity)由一组用Java语言编写的类和接口组成，JDBC是Java程序与数据库连接API。它能做以下三件事情：与某个数据库建立连接、向数据库发送SQL语句和处理数据库返回的结果。
调用类方法Class.forName(String s)能建立JDBC-ODBC桥接器。例如，代码：
try{
Class.forName(“sun.jdbc.odbc.JdbcOdbcDriver”);
}catch(Exception e){}
为Java程序加载了驱动程序。
【例 13-7】说明与数据库连接的方法connectByJdbcOdbc()，该方法按给定的数据库URL、用户名和密码连接数据库，如果连接成功，方法返回连接对象，连接不成功，则返回空（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/13-7.txt)）。
## 用纯Java的JDBC驱动程序实现与数据库连接
Java程序也可以用纯Java的JDBC驱动程序实现与数据库连接。这种方法应用较广泛，但是需要下载相应的驱动程序包，因为不同的数据库的连接代码可能不同，连接不同的数据库，加载的驱动程序也可能不同。例如，连接SQLServer的驱动程序在www.msdn.com网站下载，有3个包：msbase.jar,mssqlserver.jar和msutil.jar，并要求将这3个包放在jdk\jre\lib\ext\目录下，或在CLASSPATH中设置其放置位置。
使用纯Java的JDBC驱动程序实现与数据库连接的过程如下：加载驱动程序。有两种加载驱动程序的方式：一各是将驱动程序添加到java.lang.System的属性jdbc.drivers中。这是一个DriverManager类加载驱动程序类名的列表，表元用冒号分隔。
另一种方式是从相关的网站下载驱动程序后，在程序中利用Class.forName()方法加载指定的驱动程序。例如：
Class.forName(“com.microsoft.jdbc.sqlserver.SQLServerDriver”);
创建指定数据库的URL。数据库的URL对象类似网络的统一资源定位符，其格式是：
jdbc:subProtocol:subName://hostname:port:Databasename=XXX
其中，subprotocol是某种驱动程序支持的数据库连接机制； subName是当前连接机制下的具体名称；hostName是主机名；port是相应的连接端口；DatabaseName是要连接的数据库名称。例如，以下代码可以是一个数据库的URL：
jdbc:Microsoft:sqlserver://localhost:1433;Databasename=ksinfo
该数据库的URL说明利用miscrosoft提供的机制，用sqlserve驱动，通过1433端口访问本机上的ksInfo数据库。
建立连接。驱动程序管理器(DriverManager)的方法getConnection()建立连接。
【例 13-8】说明与数据库连接的静态方法connectByJdbc()，该方法按给定的数据库URL、用户名和密码连接数据库，如果连接成功，方法返回true，连接不成功，则返回false（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/13-8.txt)）。

