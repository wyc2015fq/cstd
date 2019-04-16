# 改变Oracle数据库连接端口 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年11月17日 17:38:13[gloomyfish](https://me.csdn.net/jia20003)阅读数：7531








Oracle数据库默认数据库监听与连接端口是1521, 但是有时候项目中需要更改默认端口

这样做很多时候客户要求,基于安全考虑. 以Oracle 11g 为例, 更改Listener的端口大致

需要以下几步:

1. 通过命令行查看当前Listener的状态

 c:\>lsnrctl status

执行以后,显示内容一般会告诉你,当前Listener与那些数据库实例相关联

2. 执行命令停止Oracle Listener服务

c:\>lsnrctl stop

执行该命令,停止Oracle监听服务,

3. 打开listener.ora与tnsnames.ora文件修改端口号,同时要在listener.ora文件

中加入SID注册内容如下:

SID_LIST_listener_name=
  (SID_LIST= 
   (SID_DESC=
    (GLOBAL_DBNAME=global_database_name)
    (SID_NAME=sid)
    (ORACLE_HOME=oracle_home))
   (SID_DESC=...))
两个文件文件夹地址 - $ORACLE_HOME/network/admin/

保存修改以后的文件

4. 在windows命令行中执行

lsnrctl start

你会发现Oracle 监听服务重新启动, 并且关联到你指定的SID服务启动

5. 验证connection

c:\>sqlplus /nolog

SQL:\> connect system/manager@SID

如果出现成功连接字样,说明端口更改已经成功

注意点:

默认情况下listener.ora文件中没有包含静态注册服务Section内容

一定要手工加上,而且一定格式正确. 否则就会得到如下错误:

TNS-12560:
 TNS:protocol adapter error
TNS-00530: Protocol adapter error


这个时候一定记得检查修改的文件!!



