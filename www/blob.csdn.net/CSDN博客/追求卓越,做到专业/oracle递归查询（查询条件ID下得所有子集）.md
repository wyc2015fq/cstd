
# oracle递归查询（查询条件ID下得所有子集） - 追求卓越,做到专业 - CSDN博客


2013年06月16日 15:56:04[Waldenz](https://me.csdn.net/enter89)阅读数：5661


一、CREATE TABLE TBL_TEST
(
ID    NUMBER,
NAME  VARCHAR2(100 BYTE),
PID   NUMBER                                  DEFAULT 0
)
INSERT INTO TBL_TEST(ID,NAME,PID) VALUES('1','10','0');
INSERT INTO TBL_TEST(ID,NAME,PID) VALUES('2','11','1');
INSERT INTO TBL_TEST(ID,NAME,PID) VALUES('3','20','0');
INSERT INTO TBL_TEST(ID,NAME,PID) VALUES('4','12','1');
INSERT INTO TBL_TEST(ID,NAME,PID) VALUES('5','121','2');
二、格式
Select * from …. Where [结果过滤条件语句]
Start with  [and起始条件过滤语句]
Connect by prior [and中间记录过滤条件语句]
三、查找所有下级
select * from tbl_test start with id=1 connect by prior id=pid
注意：此sql能查找id=1的数据的所有下级，写sql语句时要注意，因为是从id开始查找下级，所以connect by prior 子句的条件是         id=pid
四、查找所有上级
select * from tbl_test start with id=5 connect by prior pid=id
因为是从id开始查找上级，所以connect by prior 子句的条件是pid=d

