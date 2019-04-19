# mysql数据包导入或接受失败异常处理 - fanyun的博客 - CSDN博客
2016年03月18日 13:58:34[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1989
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
        在我们数据库的迁移和传输中，尤其是java开发的数据传输软件，经常会遇到传输失败的问题，一般错误如下提示信息：
       discription: 插入失败,error msg[插入失败,异常信息如下[id=460025409051,error msg[PreparedStatementCallback; SQL [INSERT INTO LC_InterimBulletin(InvolvedStock,InfoTitle,Category,Detail,Media,BulletinDate,JSID,RecordDate,Market,BulletinType,ID,XGRQ) VALUES(?,?,?,?,?,?,?,?,?,?,?,?)];
 Packet for query is too large (1365565 > 1048576). You can change this value on the server by setting the max_allowed_packet’ variable.; nested exception is com.mysql.jdbc.PacketTooBigException: Packet for query is too large (1365565 > 10485 ；
      Got a packet bigger than 'max_allowed_packet' bytes”或者“MySQL server has gone away”等错误，一般是由于当前导入的数据大于系统的限制的最大包大小。
        此时，我们就要想到通过修改'max_allowed_packet' 参数的大小就可以解决问题：
        max_allowed_packet这个值理论上最大可以设置1G，但是实际上mysql客户端最大只支持16M。
1、修改配置文件
可以编辑my.cnf来修改（windows下my.ini）,在[mysqld]段或者mysql的server配置段进行修改。
(1)查看当前系统默认值：
      SHOW VARIABLES LIKE '%max_allowed_packet%';
![](https://img-blog.csdn.net/20160307101536406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(2)按所需定值设置
max_allowed_packet = 10M设置完以后重启mysql：查看设置后的参数是否生效：   SHOW VARIABLES LIKE '%max_allowed_packet%';max_allowed_packet=*M  （自己视情况设置）即可。
如果找不到my.cnf可以通过
mysql --help | grep my.cnf或者find / -name  my.cnf
去寻找my.cnf文件。
linux下该文件在/etc/下。
2、在mysql命令行中修改
在mysql 命令行中运行
set global max_allowed_packet = 2*1024*1024*10
然后退出命令行，重启mysql服务，再进入。
show VARIABLES like '%max_allowed_packet%';
查看下max_allowed_packet是否生效！
