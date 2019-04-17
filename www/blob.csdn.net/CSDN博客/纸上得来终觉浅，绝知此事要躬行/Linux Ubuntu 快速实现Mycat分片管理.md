# Linux Ubuntu 快速实现Mycat分片管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年06月02日 18:44:50[boonya](https://me.csdn.net/boonya)阅读数：521








首先我们先准备好一个主-从（Master-Slave）Mysql配置节点（数据库用户和密码保持一致，请参考：[http://blog.csdn.net/boonya/article/details/72842251](http://blog.csdn.net/boonya/article/details/72842251)），然后就可以准备Mycat的事情了。

## 环境说明

Linux：Ubuntu-server-15.04.iso


Master：192.168.178.128

Slave：192.168.178.129


## 下载Mycat

使用的是最新发布的Mycat1.6：http://dl.mycat.io/1.6-RELEASE/

![](https://img-blog.csdn.net/20170602180025017)

这里的地址在Linux下 可以使用wget进行下载。

## 解压Mycat

我们将Mycat-server-1.6-RELEASE-20161028204710-linux.tar.gz下载到/usr/local下面，解压：


`tar -zxvf Mycat-server-1.6-RELEASE-20161028204710-linux.tar.gz`
解压后得到/usr/local/mycat。




## 安装Mycat



找到mycat的conf目录进行修改。


### 修改mycat server.xml

重点是修改下面的内容：



```
<user name="root">
		<property name="password">123456</property>
		<property name="schemas">TESTDB</property>
		
		<!-- 表级 DML 权限设置 -->
		<!-- 		
		<privileges check="false">
			<schema name="TESTDB" dml="0110" >
				<table name="tb01" dml="0000"></table>
				<table name="tb02" dml="1111"></table>
			</schema>
		</privileges>		
		 -->
	</user>
```




### 修改schema.xml

主要修改如下：



```
<dataHost name="localhost1" maxCon="1000" minCon="10" balance="0"
			  writeType="0" dbType="mysql" dbDriver="native" switchType="1"  slaveThreshold="100">
		<heartbeat>select user()</heartbeat>
		<!-- can have multi write hosts -->
		<writeHost host="hostM1" url="192.168.178.128:3306" user="root"
				   password="123456">
			<!-- can have multi read hosts -->
			<readHost host="hostS2" url="192.168.178.129:3306" user="root" password="123456" />
		</writeHost>
		<!-- <writeHost host="hostM2" url="localhost:3316" user="root" password="123456"/> -->
	</dataHost>
```

rule.xml文件暂不做修改。


### Mycat操作命令

进入到mycat的bin目录下：




```
./mycat start 启动
./mycat stop 停止
./mycat console 前台运行
./mycat restart 重启服务
./mycat pause 暂停
./mycat status 查看启动状态
```




### Mycat窗口模拟mysql


`mysql -uroot  -p123456  -h192.168.178.128  -P8066 -DTESTD`



## Mysql节点脚本

### db1分片脚本



```
/*
Navicat MySQL Data Transfer
Source Server         : mysql
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : db1
Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001
Date: 2016-01-27 15:48:40
*/
SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `company`
-- ----------------------------
DROP TABLE IF EXISTS `company`;
CREATE TABLE `company` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of company
-- ----------------------------
-- ----------------------------
-- Table structure for `customer`
-- ----------------------------
DROP TABLE IF EXISTS `customer`;
CREATE TABLE `customer` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  `sharding_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of customer
-- ----------------------------
INSERT INTO `customer` VALUES ('1', null);
INSERT INTO `customer` VALUES ('4', '10000');
-- ----------------------------
-- Table structure for `employee`
-- ----------------------------
DROP TABLE IF EXISTS `employee`;
CREATE TABLE `employee` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of employee
-- ----------------------------
-- ----------------------------
-- Table structure for `goods`
-- ----------------------------
DROP TABLE IF EXISTS `goods`;
CREATE TABLE `goods` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of goods
-- ----------------------------
INSERT INTO `goods` VALUES ('11');
-- ----------------------------
-- Table structure for `hotnews`
-- ----------------------------
DROP TABLE IF EXISTS `hotnews`;
CREATE TABLE `hotnews` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of hotnews
-- ----------------------------
-- ----------------------------
-- Table structure for `mycat_sequence`
-- ----------------------------
DROP TABLE IF EXISTS `mycat_sequence`;
CREATE TABLE `mycat_sequence` (
  `name` varchar(50) NOT NULL,
  `current_value` int(11) NOT NULL,
  `increment` int(11) NOT NULL DEFAULT '100',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of mycat_sequence
-- ----------------------------
INSERT INTO `mycat_sequence` VALUES ('GLOBAL', '100400', '100');
-- ----------------------------
-- Table structure for `orders`
-- ----------------------------
DROP TABLE IF EXISTS `orders`;
CREATE TABLE `orders` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  `customer_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of orders
-- ----------------------------
INSERT INTO `orders` VALUES ('1', '1');
INSERT INTO `orders` VALUES ('4', '4');
-- ----------------------------
-- Table structure for `travelrecord`
-- ----------------------------
DROP TABLE IF EXISTS `travelrecord`;
CREATE TABLE `travelrecord` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of travelrecord
-- ----------------------------
INSERT INTO `travelrecord` VALUES ('1');
INSERT INTO `travelrecord` VALUES ('4');
INSERT INTO `travelrecord` VALUES ('10001');
INSERT INTO `travelrecord` VALUES ('100400');
-- ----------------------------
-- Function structure for `mycat_seq_currval`
-- ----------------------------
DROP FUNCTION IF EXISTS `mycat_seq_currval`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` FUNCTION `mycat_seq_currval`(seq_name VARCHAR(50)) RETURNS varchar(64) CHARSET utf8
    DETERMINISTIC
BEGIN DECLARE retval VARCHAR(64); 
SET retval="-999999999,null"; 
SELECT concat(CAST(current_value AS CHAR),",",CAST(increment AS CHAR)) INTO retval FROM MYCAT_SEQUENCE WHERE name = seq_name; 
RETURN retval; 
END
;;
DELIMITER ;
-- ----------------------------
-- Function structure for `mycat_seq_nextval`
-- ----------------------------
DROP FUNCTION IF EXISTS `mycat_seq_nextval`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` FUNCTION `mycat_seq_nextval`(seq_name VARCHAR(50)) RETURNS varchar(64) CHARSET utf8
    DETERMINISTIC
BEGIN UPDATE MYCAT_SEQUENCE SET current_value = current_value + increment WHERE name = seq_name; 
RETURN mycat_seq_currval(seq_name); 
END
;;
DELIMITER ;
-- ----------------------------
-- Function structure for `mycat_seq_setval`
-- ----------------------------
DROP FUNCTION IF EXISTS `mycat_seq_setval`;
DELIMITER ;;
CREATE DEFINER=`root`@`%` FUNCTION `mycat_seq_setval`(seq_name VARCHAR(50),value INTEGER) RETURNS varchar(64) CHARSET utf8
    DETERMINISTIC
BEGIN UPDATE MYCAT_SEQUENCE SET current_value = value WHERE name = seq_name;
RETURN mycat_seq_currval(seq_name);
END
;;
DELIMITER ;
```




### db2分片脚本



```
/*
Navicat MySQL Data Transfer
Source Server         : mysql
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : db2
Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001
Date: 2016-01-27 15:48:50
*/
SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `company`
-- ----------------------------
DROP TABLE IF EXISTS `company`;
CREATE TABLE `company` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of company
-- ----------------------------
-- ----------------------------
-- Table structure for `customer`
-- ----------------------------
DROP TABLE IF EXISTS `customer`;
CREATE TABLE `customer` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  `sharding_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of customer
-- ----------------------------
INSERT INTO `customer` VALUES ('2', null);
INSERT INTO `customer` VALUES ('5', '10010');
-- ----------------------------
-- Table structure for `employee`
-- ----------------------------
DROP TABLE IF EXISTS `employee`;
CREATE TABLE `employee` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of employee
-- ----------------------------
-- ----------------------------
-- Table structure for `goods`
-- ----------------------------
DROP TABLE IF EXISTS `goods`;
CREATE TABLE `goods` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of goods
-- ----------------------------
INSERT INTO `goods` VALUES ('11');
-- ----------------------------
-- Table structure for `hotnews`
-- ----------------------------
DROP TABLE IF EXISTS `hotnews`;
CREATE TABLE `hotnews` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of hotnews
-- ----------------------------
INSERT INTO `hotnews` VALUES ('1');
-- ----------------------------
-- Table structure for `orders`
-- ----------------------------
DROP TABLE IF EXISTS `orders`;
CREATE TABLE `orders` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  `customer_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of orders
-- ----------------------------
INSERT INTO `orders` VALUES ('2', '2');
INSERT INTO `orders` VALUES ('5', '5');
-- ----------------------------
-- Table structure for `travelrecord`
-- ----------------------------
DROP TABLE IF EXISTS `travelrecord`;
CREATE TABLE `travelrecord` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of travelrecord
-- ----------------------------
INSERT INTO `travelrecord` VALUES ('2');
```





### db3分片脚本



```
/*
Navicat MySQL Data Transfer
Source Server         : mysql
Source Server Version : 50527
Source Host           : localhost:3306
Source Database       : db3
Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001
Date: 2016-01-27 15:48:58
*/
SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `company`
-- ----------------------------
DROP TABLE IF EXISTS `company`;
CREATE TABLE `company` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of company
-- ----------------------------
-- ----------------------------
-- Table structure for `hotnews`
-- ----------------------------
DROP TABLE IF EXISTS `hotnews`;
CREATE TABLE `hotnews` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of hotnews
-- ----------------------------
INSERT INTO `hotnews` VALUES ('2');
-- ----------------------------
-- Table structure for `travelrecord`
-- ----------------------------
DROP TABLE IF EXISTS `travelrecord`;
CREATE TABLE `travelrecord` (
  `ID` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of travelrecord
-- ----------------------------
INSERT INTO `travelrecord` VALUES ('3');
```




## Mycat测试

### 连接Mycat



```
boonya@ubuntu:/usr/local/mycat/bin$ mysql -uroot  -p123456  -h127.0.0.1 -P8066 -DTESTDB
Warning: Using a password on the command line interface can be insecure.
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 1
Server version: 5.6.29-mycat-1.6-RELEASE-20161028204710 MyCat Server (OpenCloundDB)

Copyright (c) 2000, 2015, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql>
```




### 数据库操作

Mycat做了分片的join使得我们的数据看上去是一张表。



```
##========================分片Join=====================##


mysql> select * from travelrecord;
+---------+
| ID      |
+---------+
|       2 |
| 5000001 |
|       3 |
|       1 |
|       4 |
|   10001 |
|  100400 |
+---------+
7 rows in set (0.16 sec)

mysql> insert into travelrecord(id)  values(2000001);
Query OK, 1 row affected (0.02 sec)

mysql> select * from travelrecord;
+---------+
| ID      |
+---------+
|       3 |
|       1 |
|       4 |
|   10001 |
|  100400 |
| 2000001 |
|       2 |
| 5000001 |
+---------+
8 rows in set (0.02 sec)

mysql> insert into travelrecord(id)  values(3000001);
Query OK, 1 row affected (0.01 sec)

mysql> select * from travelrecord;
+---------+
| ID      |
+---------+
|       2 |
| 5000001 |
|       1 |
|       4 |
|   10001 |
|  100400 |
| 2000001 |
| 3000001 |
|       3 |
+---------+
9 rows in set (0.01 sec)

mysql> insert into travelrecord(id)  values(4000001);
Query OK, 1 row affected (0.01 sec)

mysql> select * from travelrecord;
+---------+
| ID      |
+---------+
|       1 |
|       4 |
|   10001 |
|  100400 |
| 2000001 |
| 3000001 |
| 4000001 |
|       3 |
|       2 |
| 5000001 |
+---------+
10 rows in set (0.03 sec)

mysql> delete from travelrecord where id=1;
Query OK, 1 row affected (0.04 sec)

mysql> select * from travelrecord;
+---------+
| ID      |
+---------+
|       2 |
| 5000001 |
|       3 |
|       4 |
|   10001 |
|  100400 |
| 2000001 |
| 3000001 |
| 4000001 |
+---------+
9 rows in set (0.03 sec)

mysql> delete from travelrecord where id<5;
Query OK, 3 rows affected (0.05 sec)

mysql> select * from travelrecord;
+---------+
| ID      |
+---------+
| 5000001 |
|   10001 |
|  100400 |
| 2000001 |
| 3000001 |
| 4000001 |
+---------+
6 rows in set (0.09 sec)

mysql> delete from travelrecord where id<1000001;
Query OK, 2 rows affected (0.01 sec)

mysql> select * from travelrecord;
+---------+
| ID      |
+---------+
| 2000001 |
| 3000001 |
| 4000001 |
| 5000001 |
+---------+
4 rows in set (0.00 sec)

mysql>  show engines;
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
| Engine             | Support | Comment                                                        | Transactions | XA   | Savepoints |
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
| MyISAM             | YES     | MyISAM storage engine                                          | NO           | NO   | NO         |
| CSV                | YES     | CSV storage engine                                             | NO           | NO   | NO         |
| MRG_MYISAM         | YES     | Collection of identical MyISAM tables                          | NO           | NO   | NO         |
| BLACKHOLE          | YES     | /dev/null storage engine (anything you write to it disappears) | NO           | NO   | NO         |
| MEMORY             | YES     | Hash based, stored in memory, useful for temporary tables      | NO           | NO   | NO         |
| PERFORMANCE_SCHEMA | YES     | Performance Schema                                             | NO           | NO   | NO         |
| ARCHIVE            | YES     | Archive storage engine                                         | NO           | NO   | NO         |
| InnoDB             | DEFAULT | Supports transactions, row-level locking, and foreign keys     | YES          | YES  | YES        |
| FEDERATED          | NO      | Federated MySQL storage engine                                 | NULL         | NULL | NULL       |
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
9 rows in set (0.00 sec)

mysql>
```


### 基于注解的查询

从 1.6 开始支持三种注解方式：
/*#mycat:db_type=master*/ select * from travelrecord

/*!mycat:db_type=slave*/ select * from travelrecord

/**mycat:db_type=master*/ select * from travelrecord


示例：




```
mysql> /*!mycat:db_type=master*/ select * from travelrecord;
+---------+
| ID      |
+---------+
| 2000001 |
| 3000001 |
| 4000001 |
| 5000001 |
+---------+
4 rows in set (0.06 sec)

mysql> /*!mycat:db_type=slave*/ select * from travelrecord;
+---------+
| ID      |
+---------+
| 2000001 |
| 3000001 |
| 4000001 |
| 5000001 |
+---------+
4 rows in set (0.01 sec)

mysql>
```





















