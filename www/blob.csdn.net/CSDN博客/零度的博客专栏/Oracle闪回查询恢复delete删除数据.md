# Oracle闪回查询恢复delete删除数据 - 零度的博客专栏 - CSDN博客
2016年04月28日 14:06:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：466
1、执行　select * from A as of timestamp sysdate-10/1440;
　　　　该SQL语会查找出距离现在10分钟之前A表的所有数据。
　　　　sysdate-10/1440表示距离现在10分钟之前，1440这个数字表示一天有1440分钟。
　　　　如果不写距离时间，SQL语句可写成：select * from A as of timestamp sysdate;表示查找出到现在为止A表中的所有数据。
　　2、从以上查找出的数据中找出被误删的数据，再插入到原来的表中就可以了。
Flashback query(闪回查询)原理
**Oracle**根据undo信息，利用undo数据，类似一致性读取方法，可以把表置于一个删除前的时间点(或SCN)，从而将数据找回。
Flashback query(闪回查询)前提：
SQL> show parameter undo;
NAME                                TYPE       VALUE
------------------------------------ ----------- ------------------------------
undo_management                     string     AUTO
undo_retention                        integer    900
undo_tablespace                       string     UNDOTBS1
其中undo_management = auto，设置自动undo**管理**（AUM），该参数默认设置为：auto；
Undo_retention = n(秒),设置决定undo最多的保存时间，其值越大，就需要越多的undo表空间的支持。修改undo_retention的命令如下：
SQL> alter system set undo_retention = 3600;
System altered
闪回实现方式
1.获取数据删除前的一个时间点或scn，如下：
SQL>select to_char(sysdate, 'yyyy-mm-dd hh24:mi:ss') time, to_char(dbms_flashback.get_system_change_number) scn from dual;
TIME               SCN
------------------- ----------------------------------------
2010-06-29 23:03:14 1060499
2.查询该时间点（或scn）的数据，如下：
**SQL> select * from t as of timestamp to_timestamp('2010-06-29 22:57:47', 'yyyy-mm-dd hh24:mi:ss');**
SQL> select * from t as of scn 1060174;
3.将查询到的数据，新增到表中。也可用更直接的方法，如：
SQL>create table tab_test as select * from t of timestamp to_timestamp('2010-06-29
 22:57:47', 'yyyy-mm-dd hh24:mi:ss');
SQL>insert into tab_test select * from1060174;
示例：
Create table t(id number);
insertinto t values(1);
insert into t values(2);
insert into t values(3);
insert into t values(4);
insert into t values(5);
1.查看t表中的原始数据
SQL> select * from t;
       ID
---------
        1
        2
        3
        4
        5
2．获取数据删除前的一个时间点或scn
SQL> select to_char(sysdate, 'yyyy-mm-dd hh24:mi:ss') time, to_char(dbms_flashback.get_system_change_number) scn from dual;
TIME               SCN
------------------- ----------------------------------------
2010-06-29 23:23:33  1061279
3．删除t表中的数据，并提交事物
SQL> delete from t;
5 rows deleted
SQL> commit;
Commit complete
4．在查看t表，此时t表中数据以删除
SQL> select * from t;
       ID
----------
5．查看t表中scn为1061279(或时间点为2010-06-29
 23:23:33)时的数据
SQL> select * from t as of scn 1061279;
       ID
----------
        1
        2
        3
        4
        5
6．确认要恢复后，将t表中的数据还原到scn为1061279(或时间点为2010-06-29
 23:23:33)时的数据，并提交事物
SQL> insert into t select * from t as of scn 1061279;
5 rows inserted
SQL> commit;
Commit complete
7．确认t表数据的还原情况
SQL> select * from t;
       ID
----------
        1
        2
        3
        4
        5
注：推荐使用scn，由于oracle9i中，因为scn与时间点的同步需要5分钟，如果最近5分钟之内的数据需要Falshback
 query查询，可能会查询丢失，而scn则不存在这个问题。Oracle10g中这个问题已修正（scn与时间点的大致关系，可以通过logmnr分析归档日志获得）。
Falshback query查询的局限：
1． 不能Falshback到5天以前的数据。
2． 闪回查询无法恢复到表结构改变之前，因为闪回查询使用的是当前的数据字典。
3． 受到undo_retention参数的影响，对于undo_retention之前的数据，Flashback不保证能Flashback成功。
4． 对drop,truncate等不记录回滚的操作，不能恢复。
5． 普通用户使用dbms_flashback包，必须通过管理员授权。命令如下:
SQL>grant execute on dbms_flashback to scott;
