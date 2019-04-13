
# ORACLE锁表暴力解决方法 - lpty的博客 - CSDN博客

2018年10月19日 10:45:46[lpty](https://me.csdn.net/sinat_33741547)阅读数：72



## 一、概述
关于oracle锁详见：[https://www.cnblogs.com/zhoading/p/8547320.html](https://www.cnblogs.com/zhoading/p/8547320.html)
下述方法为处理ORA-00054:资源正忙,要求指定NOWAIT，这种问题。
## 二、方法
### 1、找出被锁对象
`select l.session_id,o.owner,o.object_name from v$locked_object l,dba_objects o where l.object_id=o.object_id`
### 2、找出造成锁的会话
`select t2.username,t2.sid,t2.serial#,t2.logon_time from v$locked_object t1,v$session t2 where t1.session_id=t2.sid order by t2.logon_time;`
### 3、杀掉对应会话
`alter system kill session 'sid,serial#';`上述sid，serial为2步骤中查询出来希望杀掉的会话id。
### 4、ORA-00031:session marked for kill
执行上述操作后，会话长时间未被杀掉，强制kill
#### （1）查找对应会话进程
`select spid, osuser, s.program  from v$session s,v$process p where s.paddr=p.addr and s.sid=sid`上述sid与3步骤中sid一致；
#### （2）杀掉进程
`su root
kill -9 spid`spid为（1）中查找出来的进程id。

