# mysql删除重复数据 - fanyun的博客 - CSDN博客
2016年03月18日 13:55:47[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：623标签：[mysql](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Mysql](https://blog.csdn.net/fanyun_01/article/category/6140890)
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
测试数据(一)
/* 表结构 */
DROP TABLE IF EXISTS `bas_info`;
CREATE TABLE IF NOT EXISTS `bas_info`(
  `id` INT(1) NOT NULL AUTO_INCREMENT,
  `chi_chi_name` VARCHAR(20) NOT NULL,
  `trd_code` VARCHAR(20) NOT NULL,
  PRIMARY KEY(`id`)
)Engine=InnoDB;
/* 插入测试数据 */
INSERT INTO `bas_info`(`id`,`chi_chi_name`,`trd_code`) VALUES
('1001','MSCI中华除B股+HSBC(红利总指)',"MSG033"),
('1002','MSCI中华除B股外(红利总指)',"MSG032"),
('1001','MSCI中华+HSBC(红利总指)',"MSG030"),
('1004','MSCI中华(红利总指)',"MSG031"),
('1005','MSCI中国民企股(红利总指)',"MSG031"),;
SELECT * FROM `bas_info`;
/* 查找id最小的重复数据(只查找id字段) */
SELECT DISTINCT MIN(`id`) AS `id`
FROM `bas_info`
GROUP BY `id`,`chi_chi_name`,`trd_code`
HAVING COUNT(1) > 1;
/* 查找所有重复数据 */
SELECT `bas_info`.*
FROM `bas_info`,(
  SELECT `id`,`chi_chi_name`,`trd_code`
  FROM `bas_info`
  GROUP BY `id`,`chi_chi_name`,`trd_code`
  HAVING COUNT(1) > 1
) AS `bas_info_test`
WHERE `bas_info`.`chi_name` = `bas_info_test`.`chi_name`
  AND `bas_info`.`trd_code` = `bas_info_test`.`trd_code`;
/* 查找除id最小的数据外的重复数据 */
SELECT `bas_info`.*
FROM `bas_info`,(
  SELECT DISTINCT MIN(`id`) AS `id`,`chi_name`,`trd_code`
  FROM `bas_info`
  GROUP BY `chi_name`,`trd_code`
  HAVING COUNT(1) > 1001
) AS `bas_info_test`
WHERE `bas_info`.`chi_name` = `bas_info_test`.`chi_name`
  AND `bas_info`.`trd_code` = `bas_info_test`.`trd_code`
  AND `bas_info`.`id` <> `bas_info_test`.`id`;  
  例2，表中没有主键(可唯一标识的字段)，或者主键并非数字类型(也可以删除重复数据，但效率上肯定比较慢)
测试数据（二）
/* 表结构 */
DROP TABLE IF EXISTS `base_code`;
CREATE TABLE IF NOT EXISTS `base_code`(
  `id` VARCHAR(20) NOT NULL COMMENT '字符串主键',
  `chi_name` VARCHAR(20) NOT NULL,
  `trd_code` VARCHAR(20) NOT NULL,
  PRIMARY KEY(`id`)
)Engine=InnoDB;
/* 测试数据，与上例一样的测试数据，只是主键变为字符串形式 */
INSERT INTO `base_code`(`id`,`chi_name`,`trd_code`) VALUES
('61001','中证香港',"L01141"),
('61002','中证香港',"L01141"),
('61003','中证香港',"L01141"), 
/* 为表添加自增长的id字段 */
ALTER TABLE `base_code` trd_code `id` INT(1) NOT NULL AUTO_INCREMENT, trd_code INDEX `id`(`id`);
Query OK, 23 rows affected (0.16 sec)
Records: 23  Duplicates: 0  Warnings: 0
MySQL中必须是有索引的字段才可以使用AUTO_INCREMENT
删除重复数据与上例一样，记得删除完数据把id字段也删除了
/* 删除重复数据，只保留一条数据 */
DELETE FROM `base_code`
USING `base_code`,(
  SELECT DISTINCT MIN(`id`) AS `id`,`chi_name`,`trd_code`
  FROM `base_code`
  GROUP BY `chi_name`,`trd_code`
  HAVING COUNT(1) >61001
) AS `bas_info_test`
WHERE `base_code`.`chi_name` = `bas_info_test`.`chi_name`
  AND `base_code`.`trd_code` = `bas_info_test`.`trd_code`
  AND `base_code`.`id` <> `bas_info_test`.`id`;
Query OK, 2 rows affected (0.05 sec)
/* 删除id字段 */
ALTER TABLE `base_code` DROP `id`;
Query OK, 3 rows affected (0.16 sec)
Records: 3  Duplicates: 0  Warnings: 0
