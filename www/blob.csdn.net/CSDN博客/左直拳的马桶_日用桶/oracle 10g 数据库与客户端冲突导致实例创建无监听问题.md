# oracle 10g 数据库与客户端冲突导致实例创建无监听问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年03月22日 18:51:29[左直拳](https://me.csdn.net/leftfist)阅读数：1984
同事在oracle 10g上创建一个实例，快结束时弹出一个错误，提示监听失败之类。查看服务，并无生成监听服务。于是删除重来，一连试了好几次，都是如此。
这真是令人心烦意乱。提示里面有说到端口1521，难道此端口被占用了？用netstat -aon命令看了下，并没有。而且创建实例过程中，并没有什么地方可以设置端口的。
后来试了下Net manager，手动创建了一个监听。看系统服务，发现已有listener；然后修改 tnsnames.ora 文件，创建一个连接串，连接，成功。
其实呢，我修改的那个tnsnames.ora 文件，是oracle 10g client下面的。同事先装了客户端，后来才装数据库。估计是二者有点冲突。那应对办法就是
1、创建实例 
2、手动创建监听 
3、修改客户端处的 tnsnames.ora 
那是否一定要修改客户端的 tnsnames.ora 呢？是否客户端的优先级比数据库要高？估计不是，这个可以在操作系统的环境变量里设置的：
设置PATH。
```
PATH=C:\instantclient-basic-windows.x64-12.1.0.2.0\instantclient_12_1;E:\oracle\product\10.2.0\db_1\bin;
1
```
排在前面的应该优先级高。
至于什么oracle_home之类，好像没啥卵用。也不清楚它们究竟有啥卵用。在网上抄了些，先记下来：
```bash
ORACLE_BASE、ORACLE_HOME有什么区别
ORACLE_BASE下是admin和product
ORACLE_HOME下则是ORACLE的命令、连接库、安装助手、listener等等一系列的东东。
这只是ORACLE自己的定义习惯。ORACLE_HOME比ORACLE_BASE目录要更深一些。也就是说：ORACLE_HOME＝$ORACLE_BASE/product/version
ORACLE_BASE是oracle的根目录，ORACLE_HOME是oracle产品的目录。
简单说，你如果装了2个版本的oracle，那么ORACLE_BASE可以是一个，但ORACLE_HOME是2个
```
