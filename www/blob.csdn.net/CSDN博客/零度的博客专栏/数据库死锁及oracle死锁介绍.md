# 数据库死锁及oracle死锁介绍 - 零度的博客专栏 - CSDN博客
2016年05月04日 14:37:54[零度anngle](https://me.csdn.net/zmx729618)阅读数：565
个人分类：[事务																[数据库](https://blog.csdn.net/zmx729618/article/category/3108441)](https://blog.csdn.net/zmx729618/article/category/6213269)
所谓死锁： 是指两个或两个以上的进程在执行过程中，因争夺资源而造成的一种互相等待的现象，若无外力作用，它们都将无法推进下去。此时称系统处于死锁状态或系统产生了死锁，这些永远在互相等待的进程称为死锁进程。 由于资源占用是互斥的，当某个进程提出申请资源后，使得有关进程在无外力协助下，永远分配不到必需的资源而无法继续运行，这就产生了一种特殊现象死锁。
关于数据库死锁的检查方法
一、数据库死锁的现象
程序在执行的过程中，点击确定或保存按钮，程序没有响应，也没有出现报错。
二、死锁的原理
当对于数据库某个表的某一列做更新或删除等操作，执行完毕后该条语句不提
交，另一条对于这一列数据做更新操作的语句在执行的时候就会处于等待状态，
此时的现象是这条语句一直在执行，但一直没有执行成功，也没有报错。
三、死锁的定位方法
通过检查数据库表，能够检查出是哪一条语句被死锁，产生死锁的机器是哪一台。
1）用dba用户执行以下语句
select username,lockwait,status,machine,program from v$session where sid in
(select session_id from v$locked_object)
如果有输出的结果，则说明有死锁，且能看到死锁的机器是哪一台。字段说明：
Username：死锁语句所用的数据库用户；
Lockwait：死锁的状态，如果有内容表示被死锁。
Status： 状态，active表示被死锁
Machine： 死锁语句所在的机器。
Program： 产生死锁的语句主要来自哪个应用程序。
2）用dba用户执行以下语句，可以查看到被死锁的语句。
select sql_text from v$sql where hash_value in (select sql_hash_value from v$session where sid in
(select session_id from v$locked_object))
四、死锁的解决方法
一般情况下，只要将产生死锁的语句提交就可以了，但是在实际的执行过程中。用户可能不知道产生死锁的语句是哪一句。可以将程序关闭并重新启动就可以了。经常在Oracle的使用过程中碰到这个问题，所以也总结了一点解决方法。
1）查找死锁的进程：
sqlplus "/as sysdba" (sys/change_on_install)
SELECT s.username,l.OBJECT_ID,l.SESSION_ID,s.SERIAL#,l.ORACLE_USERNAME,l.OS_USER_NAME,l.PROCESS 
FROM V$LOCKED_OBJECT l,V$SESSION S WHERE l.SESSION_ID=S.SID;
2）kill掉这个死锁的进程：
alter system kill session ‘sid,serial#’; （其中sid=l.session_id）
3）如果还不能解决：
select pro.spid from v$session ses,v$process pro where ses.sid=XX and ses.paddr=pro.addr;
其中sid用死锁的sid替换: exit
ps -ef|grep spid
其中spid是这个进程的进程号，kill掉这个Oracle进程
select A.SQL_TEXT, B.USERNAME, C.OBJECT_ID, C.SESSION_ID, B.SERIAL#, C.ORACLE_USERNAME,C.OS_USER_NAME,C.Process,''''||C.Session_ID||','||B.SERIAL#||''' from
 v$sql A, v$session B, v$locked_object C where A.HASH_VALUE = B.SQL_HASH_VALUE and B.SID = C.Session_ID
ORACLE锁具体分為以下几类：
1.按用户与系统划分，可以分為自动锁与显示锁
自动锁：当进行一项数据库操作时，缺省情况下，系统自动為此数据库操作获得所有有必要的锁。
显示锁：某些情况下，需要用户显示的锁定数据库操作要用到的数据，才能使数据库操作执行得更好，显示锁是用户為数据库对像设定的。
2.按锁级别划分，可分為共享锁与排它锁
共享锁：共享锁使一个事务对特定数据库资源进行共享访问——另一事务也可对此资源进行访问或获得相同共享锁。共享锁為事务提供高并发性，但如拙劣的事务设计+共享锁容易造成死锁或数据更新丢失。
排它锁：事务设置排它锁后，该事务单独获得此资源，另一事务不能在此事务提交之前获得相同对象的共享锁或排它锁。
3.按操作划分，可分為DML锁、DDL锁
+DML锁又可以分為，行锁、表锁、死锁
-行锁：当事务执行数据库插入、更新、删除操作时，该事务自动获得操作 表中操作行的排它锁。
- 表级锁：当事务获得行锁后，此事务也将自动获得该行的表锁(共享锁),以 防止其它事务进行DDL语句影响记录行的更新。事务也可以在进行 过程中获得共享锁或排它锁，只有当事务显示使用LOCK TABLE语 句显示的定义一个排它锁时，事务才会获得表上的排它锁,也可使用
LOCK TABLE显示的定义一个表级的共享锁(LOCK TABLE具体用法请参 考相关文档)。
-死锁：当两个事务需要一组有衝突的锁，而不能将事务继续下去的话，就 出现死锁。
如事务1在表A行记录#3中有一排它锁，并等待事务2在表A中记录#4 中排它锁的释放，而事务2在表A记录行#4中有一排它锁，并等待事务 1在表A中记录#3中排它锁的释放，事务1与事务2彼此等待，因此就造 成了死锁。死锁一般是因拙劣的事务设计而產生。
对於查询哪些人或进程被锁使用如下SQL:
SELECT OBJECT_ID,SESSION_ID,SERIAL# ,a.oracle_username,a.os_user_name,a.process
 FROM V$LOCKED_OBJECT a , 
V$SESSION  WHERE a.SESSION_ID=SID;
解锁能使用SQL下:alter system kill session 'sid,serial#'；
或者使用相关操作系统kill进程的命令，如UNIX下kill -9 sid,或者 使用其它工具杀掉死锁进程。
+DDL锁又可以分為：排它DDL锁、共享DDL锁、分析锁
-排它DDL锁：创建、修改、删除一个数据库对象的DDL语句获得操作对象的 排它锁。
如使用alter table语句时，為了维护数据的完成性、一致性、
合法性，该事务获得一排它DDL锁。
-共享DDL锁：需在数据库对像之间建立相互依赖关系的DDL语句通常需共享
获得DDL锁。
如创建一个包，该包中的过程与函数引用了不同的数据库表，
当编译此包时，该事务就获得了引用表的共享DDL锁。
-分析锁：ORACLE使用共享池存储分析与优化过的SQL语句及PL/SQL程序，使
运行相同语句的应用速度更快。一个在共享池中缓存的对象获得
它所引用数据库对象的分析锁。分析锁是一种独特的DDL锁类型，
ORACLE使用它追踪共享池对象及它所引用数据库对像之间的依赖 关系。当一个事务修改或删除了共享池持有分析锁的数据库对像
时，ORACLE使共享池中的对象作废，下次在引用这条SQL/PLSQL语 句时，ORACLE重新分析编译此语句。
