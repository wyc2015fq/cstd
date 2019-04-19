# mysql开户、权限设置、建库流程及常用操作 - fanyun的博客 - CSDN博客
2016年03月18日 13:59:33[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：565标签：[mysql](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Mysql](https://blog.csdn.net/fanyun_01/article/category/6140890)
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
       (1) 查看当前库所有mysql用户：
       SELECT HOST,USER FROM mysql.user;
       (2)创建一个新用户，密码自己定：
       CREATE USER 'sjdb'@'localhost' IDENTIFIED BY 'password';
       (3)给新用户增加增删改查权限：
       GRANT SELECT,INSERT,UPDATE,DELETE ON *.* TO sjdb@"localhost" IDENTIFIED BY "password"
       (4)如果需要更多权限，作如下设置：
       GRANT ALL PRIVILEGES ON *.* TO 'sjdb'@'%' IDENTIFIED BY 'password' WITH GRANT OPTION;
       (5)提交设置使及时生效：
       FLUSH   PRIVILEGES;
       (6)查看新用户状态：
       SELECT USER FROM mysql.user WHERE USER='sjdb';
       (7)创建新数据库：
       CREATE DATABASE bd_jrdb;
       (8)给新建用户对赋予对新创建数据库的操作权限：
       GRANT ALL PRIVILEGES ON bd_jrdb.* TO sjdb@localhost IDENTIFIED BY 'password';
       (9)在新库中创建新的库表SMTCN_EXCH_SECU：
       CREATE TABLE `SMTCN_EXCH_SECU` (
  `ID` INT NOT NULL,
  `INFO_SOUR` VARCHAR(200) CHARACTER SET utf8 ,
  `PUB_DT` DATETIME,
  `SECU_ID` INT NOT NULL,
  `BGN_DT` DATETIME NOT NULL,
  `END_DT` DATETIME NOT NULL,
  `IS_VALID` VARCHAR(1) CHARACTER SET utf8  NOT NULL,
  `TYP_CODEII` INT NOT NULL,
  `ENT_TIME` DATETIME NOT NULL,
  `UPD_TIME` DATETIME NOT NULL,
  `GRD_TIME` DATETIME NOT NULL,
  `RS_ID` VARCHAR(20) CHARACTER SET utf8  NOT NULL,
  `REC_ID` VARCHAR(50) CHARACTER SET utf8 ,
  PRIMARY KEY (`ID`),
  INDEX `IDX_SMTCN_EXCH_SECU` (`SECU_ID`, `BGN_DT`),
  INDEX `IDX_SMTCN_EXCH_SECU_RID` (`REC_ID`),
  INDEX `IDX_SMTCN_EXCH_SECU_UPDTIME` (`UPD_TIME`)
) ENGINE=InnoDB;
       (10)给新表SMTCN_EXCH_SECU插入数据：
     INSERT INTO `SMTCN_EXCH_SECU` VALUES(637223500, NULL, NULL, 14725, '2010-03-31', '4000-12-31', '1', 1001, '2012-11-18 08:30:03', '2015-10-16 12:50:03', '2015-03-26 11:14:53', 'JY', '325373671907');
       (11)删除表SMTCN_EXCH_SECU中secu_id在12175到52178区间的数据：
     DELETE FROM  WHERE  secu_id  BETWEEN 12175 AND 52178;
       (12)在表SMTCN_EXCH_SECU选出secu_id, end_dt并按secu_id降序排序：
    SELECT secu_id, end_dt FROM SMTCN_EXCH_SECU ORDER BY secu_id DESC;
       (13)在表SMTCN_EXCH_SECU选出secu_id, end_dt并按secu_id升序排序：
     SELECT secu_id, end_dt FROM SMTCN_EXCH_SECU ORDER BY secu_id ASC;
       (14)统计bd_jrdb数据库中所有表的数量：
     SELECT count(*) TABLES, table_schema FROM information_schema.TABLES where table_schema = 'bd_jrdb' GROUP BY table_schema;
      (15)列出bd_jrdb数据库中所有表的名称：
      SELECT table_name FROM information_schema.tables WHERE table_schema='bd_jrdb';
      (16)取消jrdb用户对数据库的操作权限
       REVOKE ALL PRIVILEGES ON *.* FROM sjdb@localhost;
      (17)删除sjdb用户
        DELETE FROM mysql.user WHERE USER='sjdb' AND HOST='localhost';
      (18)查看bd_jrdb数据库字符集：
        SHOW VARIABLES LIKE 'character_set_%';
      (19) 查找表SMTCN_EXCH_SECU中开始时间和结束时间大于2015年，更新时间大于2016年的数据
       SELECT CONCAT(id, " ", secu_id) FROM  SMTCN_EXCH_SECU WHERE BEG_DT/END_DT > 2015 AND  UPD_TIME>2016;
      (20)锁定数据表，避免在备份过程中，表被更新
       LOCK TABLES READ SMTCN_EXCH_SECU;
      (21)导出备份数据：
      SELECT * INTO OUTFILE SMTCN_EXCH_SECU.bak’ FROM SMTCN_EXCH_SECU;
      (22)解锁表：
      UNLOCK TABLES;
      (23)统计一张表有多少条数据：
      SELECT COUNT(*) FROM  SMTCN_EXCH_SECU;
