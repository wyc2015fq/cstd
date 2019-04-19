# 解决oracle 表被锁住问题 - 左直拳的马桶_日用桶 - CSDN博客
2016年01月19日 19:03:42[左直拳](https://me.csdn.net/leftfist)阅读数：2181
想修改Oracle下的某一张表，提示 
“资源正忙， 但指定以 NOWAIT 方式获取资源， 或者超时失效”
看上去是锁住了。
用系统管理员登录进数据库，然后
```
SELECT sid, serial#, username, osuser FROM v$session where sid 
in(select session_id from v$locked_object);
--kill掉相关的会话
ALTER SYSTEM KILL SESSION '597,1171';--sid,serial#
```
有文章说这样说有时可能资源释放得不够快，还有一些别的办法： 
[http://www.eygle.com/archives/2005/10/oracle_howto_kill_session.html](http://www.eygle.com/archives/2005/10/oracle_howto_kill_session.html)
