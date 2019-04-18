# mysql表数据以本地文件方式导入Hive - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年05月25日 15:39:05[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3666
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
1、mysql生成数据



1）创建数据库和数据表，脚本如下：



SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;

SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;

SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';



CREATE SCHEMA IF NOT EXISTS `auditdb` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;

USE `auditdb` ;



-- -----------------------------------------------------

-- Table `auditdb`.`t_evninfo1`

-- -----------------------------------------------------

DROP TABLE IF EXISTS `auditdb`.`t_evninfo1` ;



CREATE  TABLE IF NOT EXISTS `auditdb`.`t_evninfo1` (

  `server_ip` VARCHAR(20) NOT NULL ,

  `client_ip` VARCHAR(20) NOT NULL ,

  `ip_protocol` TINYINT NOT NULL ,

  `db_protocol` VARCHAR(20) NULL ,

  `statement` VARCHAR(1000) NOT NULL 

   )

ENGINE = InnoDB;



2）存储过程插入数据



mysql->

DELIMITER // 

create procedure analog() 

begin 

declare i int; 

set i=0;

while i<2000000 do 

INSERT INTO t_evninfo1(server_ip,client_ip,ip_protocol,db_protocol,statement) VALUES ('10.18.18.1','10.18.18.101',1,'tns','select * from tables'); 

set i = i+1;

end while;

end;

// 

mysql->call analog;



生成200万条记录。



3）导出表数据



mysql->mysqldump -u root -p auditdb t_evninfo1 > info.sql



info.db文件大小120M。



二、hive本地文件导入



1）hive建表：

hive>CREATE TABLE t_evninfo2(server_ip STRING,client_ip STRING,ip_protocol INT,db_protocol STRING,statement STRING) ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t' STORED AS TEXTFILE;  

注意info.sql的分隔符，最好将导出的info.sql本地简单加工下。



2）本地文件导入

hive>LOAD DATA LOCAL INPATH '/home/hadoop/info.sql' INTO TABLE t_evninfo2;



3）查看导入记录

hive>select * from t_evninfo2 limit 10;



当然也可以通过sqoop迁移来实现mysql数据移植到hadoop平台。
            


