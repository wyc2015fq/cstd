# MySQL 实现树形的遍历 - 文章 - 伯乐在线
原文出处： [黄杉](http://blog.csdn.net/mchdba/article/details/39277301)
**前言：**
关于多级别菜单栏或者权限系统中部门上下级的树形遍历，oracle中有connect by来实现，mysql没有这样的便捷途径，所以MySQL遍历数据表是我们经常会遇到的头痛问题，下面通过存储过程来实现。
**1，建立测试表和数据：**

MySQL
```
DROP TABLE IF EXISTS csdn.channel;   
CREATE TABLE csdn.channel (   
  id INT(11) NOT NULL AUTO_INCREMENT,     
  cname VARCHAR(200) DEFAULT NULL,   
  parent_id INT(11) DEFAULT NULL,   
  PRIMARY KEY (id)   
) ENGINE=INNODB DEFAULT CHARSET=utf8;   
INSERT  INTO channel(id,cname,parent_id)    
VALUES (13,'首页',-1),   
       (14,'TV580',-1),   
       (15,'生活580',-1),   
       (16,'左上幻灯片',13),   
       (17,'帮忙',14),   
       (18,'栏目简介',17);  
DROP TABLE IF EXISTS channel;
```
**2，利用临时表和递归过程实现树的遍历（mysql的UDF不能递归调用）：**
**2.1，从某节点向下遍历子节点，递归生成临时表数据**

MySQL
```
-- pro_cre_childlist
DELIMITER
DROP PROCEDURE IF EXISTS csdn.pro_cre_childlist
CREATE PROCEDURE csdn.pro_cre_childlist(IN rootId INT,IN nDepth INT)
BEGIN
      DECLARE done INT DEFAULT 0;
      DECLARE b INT;
      DECLARE cur1 CURSOR FOR SELECT id FROM channel WHERE parent_id=rootId;
      DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;
      SET max_sp_recursion_depth=12;
       
      INSERT INTO tmpLst VALUES (NULL,rootId,nDepth);   
       
      OPEN cur1;   
       
      FETCH cur1 INTO b;   
      WHILE done=0 DO   
              CALL pro_cre_childlist(b,nDepth+1);   
              FETCH cur1 INTO b;   
      END WHILE;   
       
      CLOSE cur1;   
END
```
**2.2，从某节点向上追溯根节点，递归生成临时表数据**

MySQL
```
-- pro_cre_parentlist
DELIMITER
DROP PROCEDURE IF EXISTS csdn.pro_cre_parentlist
CREATE PROCEDURE csdn.pro_cre_parentlist(IN rootId INT,IN nDepth INT)   
BEGIN   
      DECLARE done INT DEFAULT 0;   
      DECLARE b INT;   
      DECLARE cur1 CURSOR FOR SELECT parent_id FROM channel WHERE id=rootId;   
      DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;   
      SET max_sp_recursion_depth=12;   
       
      INSERT INTO tmpLst VALUES (NULL,rootId,nDepth);   
       
      OPEN cur1;   
       
      FETCH cur1 INTO b;   
      WHILE done=0 DO   
              CALL pro_cre_parentlist(b,nDepth+1);   
              FETCH cur1 INTO b;   
      END WHILE;   
       
      CLOSE cur1;   
     END
```
**2.3，实现类似Oracle SYS_CONNECT_BY_PATH的功能，递归过程输出某节点id路径**

MySQL
```
-- pro_cre_pathlist
DELIMITER
USE csdn
DROP PROCEDURE IF EXISTS pro_cre_pathlist
CREATE PROCEDURE pro_cre_pathlist(IN nid INT,IN delimit VARCHAR(10),INOUT pathstr VARCHAR(1000))
BEGIN                     
      DECLARE done INT DEFAULT 0;   
      DECLARE parentid INT DEFAULT 0;         
      DECLARE cur1 CURSOR FOR    
      SELECT t.parent_id,CONCAT(CAST(t.parent_id AS CHAR),delimit,pathstr)   
        FROM channel AS t WHERE t.id = nid;   
           
      DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;   
      SET max_sp_recursion_depth=12;                     
       
      OPEN cur1;   
       
      FETCH cur1 INTO parentid,pathstr;   
      WHILE done=0 DO              
              CALL pro_cre_pathlist(parentid,delimit,pathstr);   
              FETCH cur1 INTO parentid,pathstr;   
      END WHILE;   
            
      CLOSE cur1;    
END
DELIMITER ;
```
**2.4，递归过程输出某节点name路径**

MySQL
```
-- pro_cre_pnlist
DELIMITER
USE csdn
DROP PROCEDURE IF EXISTS pro_cre_pnlist
CREATE PROCEDURE pro_cre_pnlist(IN nid INT,IN delimit VARCHAR(10),INOUT pathstr VARCHAR(1000))
BEGIN                     
      DECLARE done INT DEFAULT 0;   
      DECLARE parentid INT DEFAULT 0;         
      DECLARE cur1 CURSOR FOR    
      SELECT t.parent_id,CONCAT(t.cname,delimit,pathstr)   
        FROM channel AS t WHERE t.id = nid;   
           
      DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;   
      SET max_sp_recursion_depth=12;                     
       
      OPEN cur1;   
       
      FETCH cur1 INTO parentid,pathstr;   
      WHILE done=0 DO              
              CALL pro_cre_pnlist(parentid,delimit,pathstr);   
              FETCH cur1 INTO parentid,pathstr;   
      END WHILE;   
            
      CLOSE cur1;    
     END
DELIMITER ;
```
**2.5，调用函数输出id路径**

MySQL
```
-- fn_tree_path
DELIMITER
DROP FUNCTION IF EXISTS csdn.fn_tree_path
CREATE FUNCTION csdn.fn_tree_path(nid INT,delimit VARCHAR(10)) RETURNS VARCHAR(2000) CHARSET utf8   
BEGIN     
  DECLARE pathid VARCHAR(1000);   
     
  SET pathid=CAST(nid AS CHAR);   
  CALL pro_cre_pathlist(nid,delimit,pathid);   
     
  RETURN pathid;   
END
```
**2.6，调用函数输出name路径**

MySQL
```
-- fn_tree_pathname
-- 调用函数输出name路径   
DELIMITER
DROP FUNCTION IF EXISTS csdn.fn_tree_pathname
CREATE FUNCTION csdn.fn_tree_pathname(nid INT,delimit VARCHAR(10)) RETURNS VARCHAR(2000) CHARSET utf8   
BEGIN     
  DECLARE pathid VARCHAR(1000);   
  SET pathid='';       
  CALL pro_cre_pnlist(nid,delimit,pathid);   
  RETURN pathid;   
END
DELIMITER ;
```
**2.7，调用过程输出子节点**

MySQL
```
-- pro_show_childLst  
DELIMITER
-- 调用过程输出子节点   
DROP PROCEDURE IF EXISTS pro_show_childLst
CREATE PROCEDURE pro_show_childLst(IN rootId INT)   
BEGIN   
      DROP TEMPORARY TABLE IF EXISTS tmpLst;   
      CREATE TEMPORARY TABLE IF NOT EXISTS tmpLst    
       (sno INT PRIMARY KEY AUTO_INCREMENT,id INT,depth INT);         
       
      CALL pro_cre_childlist(rootId,0);   
       
      SELECT channel.id,CONCAT(SPACE(tmpLst.depth*2),'--',channel.cname) NAME,channel.parent_id,tmpLst.depth,fn_tree_path(channel.id,'/') path,fn_tree_pathname(channel.id,'/') pathname   
      FROM tmpLst,channel WHERE tmpLst.id=channel.id ORDER BY tmpLst.sno;   
     END
```
**2.8，调用过程输出父节点**

MySQL
```
-- pro_show_parentLst
DELIMITER
-- 调用过程输出父节点   
DROP PROCEDURE IF EXISTS `pro_show_parentLst`
CREATE PROCEDURE `pro_show_parentLst`(IN rootId INT)   
BEGIN   
      DROP TEMPORARY TABLE IF EXISTS tmpLst;   
      CREATE TEMPORARY TABLE IF NOT EXISTS tmpLst    
       (sno INT PRIMARY KEY AUTO_INCREMENT,id INT,depth INT);         
       
      CALL pro_cre_parentlist(rootId,0);   
      SELECT channel.id,CONCAT(SPACE(tmpLst.depth*2),'--',channel.cname) NAME,channel.parent_id,tmpLst.depth,fn_tree_path(channel.id,'/') path,fn_tree_pathname(channel.id,'/') pathname   
      FROM tmpLst,channel WHERE tmpLst.id=channel.id ORDER BY tmpLst.sno;   
     END
```
**3，开始测试：**
**3.1，从根节点开始显示，显示子节点集合：**

MySQL
```
mysql> CALL pro_show_childLst(-1);
+----+-----------------------+-----------+-------+-------------+----------------------------+
| id | NAME                  | parent_id | depth | path        | pathname                   |
+----+-----------------------+-----------+-------+-------------+----------------------------+
| 13 |   --首页              |        -1 |     1 | -1/13       | 首页/                      |
| 16 |     --左上幻灯片      |        13 |     2 | -1/13/16    | 首页/左上幻灯片/           |
| 14 |   --TV580             |        -1 |     1 | -1/14       | TV580/                     |
| 17 |     --帮忙            |        14 |     2 | -1/14/17    | TV580/帮忙/                |
| 18 |       --栏目简介      |        17 |     3 | -1/14/17/18 | TV580/帮忙/栏目简介/       |
| 15 |   --生活580           |        -1 |     1 | -1/15       | 生活580/                   |
+----+-----------------------+-----------+-------+-------------+----------------------------+
6 rows in set (0.05 sec)
Query OK, 0 rows affected (0.05 sec)
```
**3.2，显示首页下面的子节点**

MySQL
```
CALL pro_show_childLst(13);
mysql> CALL pro_show_childLst(13);
+----+---------------------+-----------+-------+----------+-------------------------+
| id | NAME                | parent_id | depth | path     | pathname                |
+----+---------------------+-----------+-------+----------+-------------------------+
| 13 | --首页              |        -1 |     0 | -1/13    | 首页/                   |
| 16 |   --左上幻灯片      |        13 |     1 | -1/13/16 | 首页/左上幻灯片/        |
+----+---------------------+-----------+-------+----------+-------------------------+
2 rows in set (0.02 sec)
Query OK, 0 rows affected (0.02 sec)
mysql>
```
**3.3，显示TV580下面的所有子节点**

MySQL
```
CALL pro_show_childLst(14);
mysql> CALL pro_show_childLst(14);
+----+--------------------+-----------+-------+-------------+----------------------------+
| id | NAME               | parent_id | depth | path        | pathname                   |
+----+--------------------+-----------+-------+-------------+----------------------------+
| 14 | --TV580            |        -1 |     0 | -1/14       | TV580/                     |
| 17 |   --帮忙           |        14 |     1 | -1/14/17    | TV580/帮忙/                |
| 18 |     --栏目简介     |        17 |     2 | -1/14/17/18 | TV580/帮忙/栏目简介/       |
+----+--------------------+-----------+-------+-------------+----------------------------+
3 rows in set (0.02 sec)
Query OK, 0 rows affected (0.02 sec)
mysql>
```
**3.4，“帮忙”节点有一个子节点，显示出来：**

MySQL
```
CALL pro_show_childLst(17);
mysql> CALL pro_show_childLst(17);
+----+------------------+-----------+-------+-------------+----------------------------+
| id | NAME             | parent_id | depth | path        | pathname                   |
+----+------------------+-----------+-------+-------------+----------------------------+
| 17 | --帮忙           |        14 |     0 | -1/14/17    | TV580/帮忙/                |
| 18 |   --栏目简介     |        17 |     1 | -1/14/17/18 | TV580/帮忙/栏目简介/       |
+----+------------------+-----------+-------+-------------+----------------------------+
2 rows in set (0.03 sec)
Query OK, 0 rows affected (0.03 sec)
mysql>
```
**3.5，“栏目简介”没有子节点，所以只显示最终节点：**

MySQL
```
mysql> CALL pro_show_childLst(18);
+----+----------------+-----------+-------+-------------+----------------------------+
| id | NAME           | parent_id | depth | path        | pathname                   |
+----+----------------+-----------+-------+-------------+----------------------------+
| 18 | --栏目简介     |        17 |     0 | -1/14/17/18 | TV580/帮忙/栏目简介/       |
+----+----------------+-----------+-------+-------------+----------------------------+
1 row in set (0.36 sec)
Query OK, 0 rows affected (0.36 sec)
mysql>
```
**3.6，显示根节点的父节点**

MySQL
```
CALL pro_show_parentLst(-1);
mysql> CALL pro_show_parentLst(-1);
Empty set (0.01 sec)
Query OK, 0 rows affected (0.01 sec)
mysql>
```
**3.7，显示“首页”的父节点**

MySQL
```
CALL pro_show_parentLst(13);
mysql> CALL pro_show_parentLst(13);
+----+----------+-----------+-------+-------+----------+
| id | NAME     | parent_id | depth | path  | pathname |
+----+----------+-----------+-------+-------+----------+
| 13 | --首页   |        -1 |     0 | -1/13 | 首页/    |
+----+----------+-----------+-------+-------+----------+
1 row in set (0.02 sec)
Query OK, 0 rows affected (0.02 sec)
mysql>
```
**3.8，显示“TV580”的父节点，parent_id为-1**

MySQL
```
CALL pro_show_parentLst(14);
mysql> CALL pro_show_parentLst(14);
+----+---------+-----------+-------+-------+----------+
| id | NAME    | parent_id | depth | path  | pathname |
+----+---------+-----------+-------+-------+----------+
| 14 | --TV580 |        -1 |     0 | -1/14 | TV580/   |
+----+---------+-----------+-------+-------+----------+
1 row in set (0.02 sec)
Query OK, 0 rows affected (0.02 sec)
```
**3.9，显示“帮忙”节点的父节点**

MySQL
```
mysql>
CALL pro_show_parentLst(17);
mysql> CALL pro_show_parentLst(17);
+----+-----------+-----------+-------+----------+---------------+
| id | NAME      | parent_id | depth | path     | pathname      |
+----+-----------+-----------+-------+----------+---------------+
| 17 | --帮忙    |        14 |     0 | -1/14/17 | TV580/帮忙/   |
| 14 |   --TV580 |        -1 |     1 | -1/14    | TV580/        |
+----+-----------+-----------+-------+----------+---------------+
2 rows in set (0.02 sec)
Query OK, 0 rows affected (0.02 sec)
mysql>
```
**3.10，显示最低层节点“栏目简介”的父节点**

MySQL
```
CALL pro_show_parentLst(18);
mysql> CALL pro_show_parentLst(18);
+----+----------------+-----------+-------+-------------+----------------------------+
| id | NAME           | parent_id | depth | path        | pathname                   |
+----+----------------+-----------+-------+-------------+----------------------------+
| 18 | --栏目简介     |        17 |     0 | -1/14/17/18 | TV580/帮忙/栏目简介/       |
| 17 |   --帮忙       |        14 |     1 | -1/14/17    | TV580/帮忙/                |
| 14 |     --TV580    |        -1 |     2 | -1/14       | TV580/                     |
+----+----------------+-----------+-------+-------------+----------------------------+
3 rows in set (0.02 sec)
Query OK, 0 rows affected (0.02 sec)
mysql>
```
**参考网址：**
http://jan.kneschke.de/projects/mysql/sp/sp_tree.sql
http://blog.csdn.net/ylqmf/article/details/5172901
