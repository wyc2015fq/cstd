# 一些非常有用的Oracle SQL - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年02月23日 09:50:32[gloomyfish](https://me.csdn.net/jia20003)阅读数：2042








**前提，要打开windows command line窗口。**

1。打开SQLPLUS

C> sqlplus /nolog

2。连接到指定数据库

SQL>connect  user/password@SID 或者

SQL>connect  user/password@localhost(IP Address):1521/SID

3。查询该schema下的所有用户表

SQL>select table_name from user_tables;

4。使用DBA身份登陆数据库

SQL>connect system/manager@SID as sysdba;

5。系统表中检索用户

SQL>select name, password from sys.user$ where name = ''zhangsan';

6。修改用户密码

SQL>alter user user_name identified by new_password;

7。查询DBA Profile表

SQL>select * from dba_profiles where profile='DEFAULT_PROFILE';

8。使用完整模式导出指定数据库

c\:>exp 'system/manager@SID as sysdba'  FULL=y FILE=c:\dbfulldata.dmp log=c:\dbfulldata.log

9。导入到新的数据库中

c\:>imp system/manager@SID FULL=Y IGNORE=Y file==imp_dbfull.dmp log=imp_dbfull.dmp.log

10。输出SQL执行结果到指定的文件

SQL>spool c:\result.txt;

SQL>select * from yourtables;

SQL>spool off; --强制清孔缓冲区数据到指定txt文件

11。修改system用户的密码

SQL>connect  sys/install_on_change@SID as sysdba;

SQL>alter user system identified by new_password;

12。查看Oracle当前版本信息

SQL>select * from v$version;

13。删除数据库SID

c\:>oradim -delete -sid SID

14。查看用户信息

SQL>select * from dba_users;



