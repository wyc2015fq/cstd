# Mycat catlet跨库JOIN与全局JOIN - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年06月14日 10:38:00[boonya](https://me.csdn.net/boonya)阅读数：7159








基于catlet的分库JOIN为数据库表按类型分库提供了很好的支持，而全局表在每个节点都有DDL实现也可以实现直接JOIN操作。当表设置为全局表后可以与任意一个表进行JOIN操作。


## Mysql分库

这里我们将基础和业务分别放在不同的数据库分片上，创建m和n数据库实例名。

![](https://img-blog.csdn.net/20170614102744504)



m基础配置数据库：包含t_user

n业务数据数据库：包含t_service

m&n数据库建表语句：

t_user



```
/*
Navicat MySQL Data Transfer

Source Server         : 10.10.13.251-dev
Source Server Version : 50719
Source Host           : 10.10.13.251:3306
Source Database       : m

Target Server Type    : MYSQL
Target Server Version : 50719
File Encoding         : 65001

Date: 2017-06-14 13:08:49
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `t_user`
-- ----------------------------
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
  `u_id` int(11) NOT NULL AUTO_INCREMENT,
  `u_name` varchar(50) DEFAULT NULL,
  `u_email` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`u_id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
```
t_service:


```
/*
Navicat MySQL Data Transfer

Source Server         : 10.10.13.251-dev
Source Server Version : 50719
Source Host           : 10.10.13.251:3306
Source Database       : n

Target Server Type    : MYSQL
Target Server Version : 50719
File Encoding         : 65001

Date: 2017-06-14 13:08:59
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `t_service`
-- ----------------------------
DROP TABLE IF EXISTS `t_service`;
CREATE TABLE `t_service` (
  `s_id` int(11) NOT NULL AUTO_INCREMENT,
  `s_name` varchar(50) DEFAULT NULL,
  `s_uid` int(11) DEFAULT NULL,
  PRIMARY KEY (`s_id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
```

rule.xml配置：



```
<tableRule name="mod-long">
		<rule>
			<columns>id</columns>
			<algorithm>mod-long</algorithm>
		</rule>
	</tableRule>
```

```
<function name="mod-long" class="io.mycat.route.function.PartitionByMod">
   <!-- how many data nodes -->
   <property name="count">1</property>
</function>
```

## Mycat 跨库JOIN
只需要修改${MYCAT_HOME}/conf/schema.xml


```
<?xml version="1.0"?>
<!DOCTYPE mycat:schema SYSTEM "schema.dtd">
<mycat:schema xmlns:mycat="http://io.mycat/">

        <schema name="TESTDB" checkSQLschema="false" sqlMaxLimit="100">
                <!-- auto sharding by id (long) -->
                <table name="t_user"   primaryKey="u_id" autoIncrement="true"   dataNode="dn1" rule="mod-long" >
                   <!--  <childTable name="t_service" primaryKey="s_id" joinKey="s_uid" parentKey="u_id"/> -->
                </table>

                <table name="t_service" primaryKey="s_id" autoIncrement="true"   dataNode="dn2"  rule="mod-long" />
                <!-- random sharding using mod sharind rule -->

                <!-- <table name="dual" primaryKey="ID" dataNode="dnx,dnoracle2" type="global"
                        needAddLimit="false"/> <table name="worker" primaryKey="ID" dataNode="jdbc_dn1,jdbc_dn2,jdbc_dn3"
                        rule="mod-long" /> -->
               
        </schema> 
        <!-- <dataNode name="dn1$0-743" dataHost="localhost1" database="db$0-743"
                /> -->
        <dataNode name="dn1" dataHost="localhost1" database="m" />
        <dataNode name="dn2" dataHost="localhost1" database="n" />
        
        <!--<dataNode name="dn4" dataHost="sequoiadb1" database="SAMPLE" />
         <dataNode name="jdbc_dn1" dataHost="jdbchost" database="db1" />
        <dataNode       name="jdbc_dn2" dataHost="jdbchost" database="db2" />
        <dataNode name="jdbc_dn3"       dataHost="jdbchost" database="db3" /> -->
        <dataHost name="localhost1" maxCon="1000" minCon="10" balance="0"
                          writeType="0" dbType="mysql" dbDriver="native" switchType="1"  slaveThreshold="100">
                <heartbeat>select user()</heartbeat>
                <!-- can have multi write hosts -->
                <writeHost host="hostM1" url="192.168.178.128:3306" user="root"   password="123456">
                        <!-- can have multi read hosts -->
                        <readHost host="hostS2" url="192.168.178.128:3306" user="root" password="123456" />
                </writeHost>
                <!-- <writeHost host="hostM2" url="localhost:3316" user="root" password="123456"/> -->
        </dataHost>
       
</mycat:schema>
```





利用catlets人工智能解析工具JOIN：

`/*!mycat:catlet=io.mycat.catlets.ShareJoin */SELECT * from t_service s,t_user u ON u.u_id=s.s_uid;`
![](https://img-blog.csdn.net/20170614103016539)

## Mycat 全局JOIN

当表设置为全局表后可以与任意一个表进行JOIN操作。


### 分类全局表



一个真实的业务系统中，往往存在大量的类似字典表的表格，它们与业务表之间可能有关系，这种关系，可以理解为“标签”，而不应理解为通常的“主从关系”，这些表基本上很少变动，可以根据主键ID进行缓存，下面这张图说明了一个典型的“标签关系”图：



         在分片的情况下，当业务表因为规模而进行分片以后，业务表与这些附属的字典表之间的关联，就成了比较棘手的问题，考虑到字典表具有以下几个特性：
- 变动不频繁
- 数据量总体变化不大
- 数据规模不大，很少有超过数十万条记录。

鉴于此，**MyCAT**定义了一种特殊的表，称之为“全局表”，全局表具有以下特性：
- 全局表的插入、更新操作会实时在所有节点上执行，保持各个分片的数据一致性
- 全局表的查询操作，只从一个节点获取
- 全局表可以跟任何一个表进行JOIN操作

将字典表或者符合字典表特性的一些表定义为全局表，则从另外一个方面，很好的解决了数据JOIN的难题。通过全局表+基于E-R关系的分片策略，**MyCAT**可以满足80%以上的企业应用开发。

### 全局表配置

全局表配置比较简单，不用写 Rule 规则，如下配置即可：


`<table name="company" primaryKey="ID" type="global" dataNode="dn1,dn2,dn3" />`
需要注意的是，全局表每个分片节点上都要有运行创建表的 DDL 语句。




## 人工智能JOIN测试数据

### 非limit查询



```
boonya@ubuntu:~$ mysql -h192.168.178.128  -uroot -p123456  -P8666
Warning: Using a password on the command line interface can be insecure.
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 5
Server version: 5.6.29-mycat-1.6-RELEASE-20161028204710 MyCat Server (OpenCloundDB)

Copyright (c) 2000, 2015, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql> /*!mycat:catlet=demo.catlets.ShareJoin */SELECT * from t_service s,t_user u ON u.u_id=s.s_uid;
ERROR 1064 (HY000): java.lang.ClassNotFoundException: demo.catlets.ShareJoin
mysql> /*!mycat:catlet=io.mycat.catlets.ShareJoin */SELECT * from t_service s,t_user u ON u.u_id=s.s_uid;
Empty set (0.15 sec)

mysql> show databases;
+----------+
| DATABASE |
+----------+
| TESTDB   |
+----------+
1 row in set (0.00 sec)

mysql> /*!mycat:catlet=io.mycat.catlets.ShareJoin */SELECT * from t_service s,t_user u ON u.u_id=s.s_uid;
+------+--------------+-------+-------+--------+----------------+
| s_id | s_name       | s_uid | s_uid | u_name | u_email        |
+------+--------------+-------+-------+--------+----------------+
|    1 | MYCATSERVICE |     1 |     1 | boonya | boonya@163.com |
+------+--------------+-------+-------+--------+----------------+
1 row in set (0.02 sec)

mysql> /*!mycat:catlet=io.mycat.catlets.ShareJoin */SELECT * from t_service s,t_user u ON u.u_id=s.s_uid;
ERROR 2006 (HY000): MySQL server has gone away
No connection. Trying to reconnect...
Connection id:    15
Current database: *** NONE ***

+------+--------------+-------+-------+--------+-----------------+
| s_id | s_name       | s_uid | s_uid | u_name | u_email         |
+------+--------------+-------+-------+--------+-----------------+
|    1 | MYCATSERVICE |     1 |     1 | boonya | boonya@163.com  |
|    2 | SHOPPING     |     2 |     2 | niuniu | niuniu@sina.com |
+------+--------------+-------+-------+--------+-----------------+
2 rows in set (0.02 sec)

mysql>
```

JOIN查询有效。

### limit查询



```
+------+--------------+-------+-------+--------+-----------------+
| s_id | s_name       | s_uid | s_uid | u_name | u_email         |
+------+--------------+-------+-------+--------+-----------------+
|    1 | MYCATSERVICE |     1 |     1 | boonya | boonya@163.com  |
|    2 | SHOPPING     |     2 |     2 | niuniu | niuniu@sina.com |
+------+--------------+-------+-------+--------+-----------------+
2 rows in set (0.37 sec)

mysql> /*!mycat:catlet=io.mycat.catlets.ShareJoin */SELECT * from t_service s,t_user u ON u.u_id=s.s_uid limit 2;
+------+--------------+-------+-------+--------+-----------------+
| s_id | s_name       | s_uid | s_uid | u_name | u_email         |
+------+--------------+-------+-------+--------+-----------------+
|    1 | MYCATSERVICE |     1 |     1 | boonya | boonya@163.com  |
|    2 | SHOPPING     |     2 |     2 | niuniu | niuniu@sina.com |
+------+--------------+-------+-------+--------+-----------------+
2 rows in set (0.06 sec)

mysql> /*!mycat:catlet=io.mycat.catlets.ShareJoin */SELECT * from t_service s,t_user u ON u.u_id=s.s_uid limit 1;
+------+--------------+-------+-------+--------+----------------+
| s_id | s_name       | s_uid | s_uid | u_name | u_email        |
+------+--------------+-------+-------+--------+----------------+
|    1 | MYCATSERVICE |     1 |     1 | boonya | boonya@163.com |
+------+--------------+-------+-------+--------+----------------+
1 row in set (0.02 sec)
```


最终看得到数据表对应关系如下：

![](https://img-blog.csdn.net/20170614133953875)








