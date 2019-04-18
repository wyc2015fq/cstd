# Oracle rac 配置Weblogic数据源时 实例名及URL的选择 - z69183787的专栏 - CSDN博客
2013年03月26日 15:12:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9265
## 这几天，应用程序后台一直报无法取得连数据库接池。但之前从来没有这个问题，迁移到Weblogic后才发生。之后据了解，我们服务器上的Oracle 10G 是 RAC 的，即有两个节点。两个节点 IP及实例名分别为：10.1.43.11 stnic110.1.43.21 stnic2配置数据源时 一直使用的是第一个 URL 及 实例名，配置完后，连接URL为：jdbc:oracle:thin:@10.1.43.11:1521:stnic1Weblogic 中显示的是但之后根据和DBA 的交流 发现当ORACLE是RAC配置的时候，连接池的 URL及实例名 应该用另一种写法：jdbc:oracle:thin:@(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=10.1.43.11)(PORT=1521))(ADDRESS=(PROTOCOL=TCP)(HOST=10.1.43.21)(PORT=1521))(FAILOVER=on)(LOAD_BALANCE=on))(CONNECT_DATA=(SERVER=DEDICATED)(SERVICE_NAME=stnic)))修改完成后，即将连接数平均分布在2台RAC上，而不是和之前一样将负载 全部加到1台机器上。
查阅文档后发现，配置多数据源还有另一种方法，即先添加 11 及 21 2个数据源
然后进入weblogic后台 ，添加数据源中 选择 多数据源，之后选择已经添加的 2个数据源即可。（一般都与RAC一起使用）
[http://edocs.weblogicfans.net/wls/docs92/ConsoleHelp/taskhelp/jdbc/jdbc_multidatasources/CreateMultiDataSources.html](http://edocs.weblogicfans.net/wls/docs92/ConsoleHelp/taskhelp/jdbc/jdbc_multidatasources/CreateMultiDataSources.html)
查询当前数据名
方法一:select name from v$database;
方法二：show parameter db
方法三：查看参数文件。
查询当前数据库实例名
方法一：select instance_name from v$instance;
方法二：show parameter instance
方法三：在参数文件中查询。
在一般情况下，数据库名和实例名是一对一的关系，但如果在oracle并行服务器架构(即oracle实时应用集群)中，数据库名和实例名是一对多的关系。
