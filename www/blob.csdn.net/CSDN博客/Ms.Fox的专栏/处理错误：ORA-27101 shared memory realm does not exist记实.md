# 处理错误：ORA-27101: shared memory realm does not exist记实 - Ms.Fox的专栏 - CSDN博客
2017年01月03日 13:39:50[lulu-lu](https://me.csdn.net/smbluesky)阅读数：256
个人分类：[JAVA](https://blog.csdn.net/smbluesky/article/category/6556788)
## [出了ORA-27101: shared memory realm does not exist 错误的处理](http://www.blogjava.net/imcb/archive/2007/04/02/108080.html)
我试图连接数据库时的ORA-01034 和 ORA-27101错误信息，它总是显示这样的信息，“ORA-01034 - Oracle not available”和“ORA-27101 - shared memory realm does not exist”。
不能连接到我的Oracle 数据库上。
因为碰到防火墙的问题次数比较多，这次也先看看，发现根本就没什么拦截的。查看了一下监听，数据库实例也是运行着的。
奇怪的事情，刚才还好好的，重启了机器就不行了。上网找了找，问题就解决了。
解决的方法倒是不麻烦...
　　首先看一下 ORACLE_BASE\ORACLE_HOME\DATABASE 下面的 ORADIM.LOG 文件
　　如果里边有这个错误
　　ORA-12640: 验证适配器初始化失败
　　那么问题就是他了.
　　到ORACLE_BASE\ORACLE_HOME\NETWORK\ADMIN下面找sqlnet.ora文件
　　把sqlnet.authentication_services=(NTS) 改成
　　sqlnet.authentication_services=(NONE) 
　　然后手动重起一下你的哪个数据库服务..在开始->控制面板->管理工具->服务里然后把这个服务的启动帐号改成你的帐号吧...其实不改大概也没有毛病.我就没改。。
看了这个方法，出错的原因大概有两种情况，修改了帐户，实例有些变动等。
说明：以上内容为引用．
附言：
就在今天我的８Ｉ数据库也出现了错误：“ORA-01034 - Oracle not available”和“ORA-27101 - shared memory realm does not exist”。然后在网上搜到了如上的内容，并按其中所说的把问题解决了。很高兴！![](http://blog.itpub.net/images/edit/face/001.gif)
要补充的一点是我的ORADIM.LOG 文件在目录 orant\database 下，而文件sqlnet.ora在目录 orant\network\ADMIN
下。我的数据库发生这个问题的可能原因是：非正常关闭数据库（在没有关闭数据库的情况下直接关机）。
关于这个问题ITPUB上有如下说法：（但我没测试过）
---------------------
1、实例没有启动
sqlplus /nolog
connect / as sysdba
startup
估计是我的电脑非正常关机，导致数据库非正常关闭，在cmd模式下，，执行以上指令，即可以打开数据库了。
--------------------
２、最近遇到一次，杀毒软件把ORACLE杀DOWN了，
ORACLE没有报错.
startup 就可以了 .
３、执行如下语句启动数据库
sqlplus /nolog
connect / as sysdba
startup force
cmd中因为不能执行sqlplus，则采用窗口执行方式sqlplusw
基本上用LS的就可以！
sqlplusw /nolog
connect / as sysdba
startup force
另还搜到一高手有关这个问题的记录（虽然我没测试但我相信他说的是可行的，并且他里面所谈到的“异常关机”与我上面所说的原因“非正常关闭数据库（在没有关闭数据库的情况下直接关机）”是相通的）：
昨天夜里写程式，忙到12点多。一朋友说他数据库有问题，就用joinnet连过去检查。发现是ORA-27101.先将操作步骤整理出来供大家参考。
1。首先检查oracle\admin\sid\bdump\下的alert.log发现没有最后一次数据库的启动相关信息。也就是说电脑可能有异常关机过。
2。进入cmd模式，sqlplus /nolog,connect / as sysdba,执行startup,还是27101错误。初步判定为init.ora文件有异常，copy之前的init.ora文件覆盖。
3。进入cmd模式，sqlplus /nolog,connect / as sysdba,执行startup pfile=c:\oracle\admin\sid\pfile\init.ora，启动成功。
