# 【mysql】replace into 插入数据用法 - zkq_1986的博客 - CSDN博客





2018年11月20日 11:29:08[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：68








使用 replace into插入数据时：

REPLACE INTO test(title,uid) VALUES ('1234657','1003');

执行结果：
[SQL]REPLACE INTO test(title,uid) VALUES ('1234657','1003');
受影响的行: 1
时间: 0.035s
当前数据库test表所有数据如下：

![](https://images2015.cnblogs.com/blog/553900/201612/553900-20161219160539932-1025929232.png)

当uid存在时，使用replace into 语句

REPLACE INTO test(title,uid) VALUES ('1234657','1001');

[SQL]REPLACE INTO test(title,uid) VALUES ('1234657','1001');
受影响的行: 2
时间: 0.140s
![](https://images2015.cnblogs.com/blog/553900/201612/553900-20161219160702807-1188018521.png)

replace into t(id, update_time) values(1, now());

或

replace into t(id, update_time) select 1, now();

replace into 跟 insert 功能类似，不同点在于：replace into 首先尝试插入数据到表中，** 1. 如果发现表中已经有此行数据（根据主键或者唯一索引判断）则先删除此行数据，然后插入新的数据。 2. 否则，直接插入新数据。**



