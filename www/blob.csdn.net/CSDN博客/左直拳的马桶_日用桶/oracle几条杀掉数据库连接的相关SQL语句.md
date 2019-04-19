# oracle几条杀掉数据库连接的相关SQL语句 - 左直拳的马桶_日用桶 - CSDN博客
2019年02月15日 15:07:42[左直拳](https://me.csdn.net/leftfist)阅读数：84
个人分类：[oracle](https://blog.csdn.net/leftfist/article/category/5734215)
几条杀掉数据库连接的SQL语句。应该用有管理员权限的账号登录数据库，加以运行。
我也不大理解，基本抄来的，备忘。
```
--不知道有啥用
select sysdate as 当前时间,
sys.login_user as 数据库用户,
machine as 登录机器名,
SYS_CONTEXT('USERENV', 'IP_ADDRESS') as 登录IP,
program as 应用程序
from v$session
where AUDSID = USERENV('SESSIONID');
--查找有INSERT、TG_RADAR_LOST字样的SQL语句
select * from v$process WHERE addr in (select ADDRESS from v$sql where sql_text like '%TG_RADAR_LOST%' and sql_text like '%INSERT%');
--查找使用了SDE表空间的连接
select saddr, sid, serial#, paddr, username, status, machine
  from v$session  
 where username is not null
 and username='SDE';
--查找锁定信息 
select * from v$locked_object;   
--杀！
alter system kill session '2317,64848' immediate;  --SID,SERIAL
```
