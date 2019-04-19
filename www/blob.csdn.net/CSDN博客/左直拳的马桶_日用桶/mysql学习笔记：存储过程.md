# mysql学习笔记：存储过程 - 左直拳的马桶_日用桶 - CSDN博客
2014年07月14日 22:19:35[左直拳](https://me.csdn.net/leftfist)阅读数：1784
```
use test;
drop table if exists t8;
CREATE TABLE t8(s1 INT,PRIMARY KEY(s1));
drop procedure if exists handlerdemo;
DELIMITER $$
CREATE PROCEDURE handlerdemo()
BEGIN
declare xx int default 4;
DECLARE oh_no condition for sqlstate '23000';
#DECLARE CONTINUE HANDLER FOR SQLSTATE '23000' SET @X2=1;
declare exit handler for oh_no set @info='违反主键约束';
SET @X=1;
INSERT INTO t8 VALUES(1);
SET @X=2;
INSERT INTO t8 VALUES(1);
SET @X=3;
END
```
调用存储过程
```
/* 调用存储过程*/
CALL handlerdemo();
/* 查看调用存储过程结果*/
SELECT @X,@info;
```
心得体会：
1、语句结束符
也许mysql是将存储过程、自定义函数看作一条语句的，因此，存储过程里多条语句用“；”隔开，为了避免冲突，就要用delimiter 重新定义结束符。
一般可以在存储过程开始前，定义新的结束符，如 
delimiter //
存储过程书写完毕以后，再恢复定义：delimiter ;
2、变量
mysql的变量，跟SQL SERVER一样，形如 @X ，但无需声明，直接使用。
而在存储过程里面，变量无须用 @，但要声明。并且声明要放在存储过程的头部（？），如本例，否则报错。真奇怪啊，一方面，有时候变量无须声明就可以使用，另一方面，有时候又要限定声明位置，令人无所适从，似乎随便了点。
存储过程内部的变量，作用范围仅限于存储过程。但那些带@的变量，却仿佛可以跨越会话和连接，看上去是全局变量？如上面的例子。
3、条件和处理
定义条件，是为了给处理调用。如上面的例子：
```
DECLARE oh_no condition for sqlstate '23000';
#DECLARE CONTINUE HANDLER FOR SQLSTATE '23000' SET @X2=1;
declare exit handler for oh_no set @info='违反主键约束';
```

