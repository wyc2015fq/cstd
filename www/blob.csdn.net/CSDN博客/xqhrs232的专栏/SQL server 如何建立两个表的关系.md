# SQL server 如何建立两个表的关系 - xqhrs232的专栏 - CSDN博客
2018年07月14日 21:07:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1389
原文地址::[https://jingyan.baidu.com/article/0f5fb0993342106d8334ea8e.html](https://jingyan.baidu.com/article/0f5fb0993342106d8334ea8e.html)
相关文章
1、两个数据库表数据实时同步（sql2008 触发器）----[https://blog.csdn.net/leamonjxl/article/details/6924576](https://blog.csdn.net/leamonjxl/article/details/6924576)
2、SQL server触发器实例讲解（如何实现两个表的关联）----[https://blog.csdn.net/xiaruilin/article/details/8120237](https://blog.csdn.net/xiaruilin/article/details/8120237)
3、**[sql server][基础] 多表关联创建----[https://blog.csdn.net/xys_777/article/details/5766742](https://blog.csdn.net/xys_777/article/details/5766742)**
4、**SQL Server 2008 ——关系----[https://blog.csdn.net/aiming66/article/details/51469739](https://blog.csdn.net/aiming66/article/details/51469739)**
5、sql server 两个表之间的关联（触发器）----[https://blog.csdn.net/fendou300/article/details/79037544](https://blog.csdn.net/fendou300/article/details/79037544)
**6、**
# SQL怎样把一个表的数据插入到另一个表里？----[https://zhidao.baidu.com/question/5920017.html](https://zhidao.baidu.com/question/5920017.html)
- 
复制表结构及数据到新表 select * into 目标表名 from 源表名
![](https://gss0.baidu.com/-fo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=d0f991673687e9504242fb6a20087f7b/1e30e924b899a9012af1c59211950a7b0308f54a.jpg)
- 
只复制表结构到新表 CREATE TABLE 新表 SELECT * FROM 旧表 WHERE 1=2 即:让WHERE条件不成立. 
![](https://gss0.baidu.com/9fo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=ac44085870d98d1076810437110f9436/b3fb43166d224f4aa23dedb705f790529922d1ea.jpg)
- 
复制旧表的数据到新表(假设两个表结构一样) INSERT INTO 新表 SELECT * FROM 旧表 
![](https://gss0.baidu.com/-vo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=0fb6188de7c4b74534c1bf10ffcc322f/d0c8a786c9177f3e62ea62a97ccf3bc79e3d56b3.jpg)
- 
复制旧表的数据到新表(假设两个表结构不一样) INSERT INTO 新表(字段1,字段2,) SELECT 字段1,字段2， FROM 旧表 
![](https://gss0.baidu.com/94o3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=527e42c4392ac65c67506e75cbc29e29/b999a9014c086e06756ca1ef0e087bf40bd1cb4a.jpg)
- 
oracle数据库也是类似的。
![](https://gss0.baidu.com/-Po3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=ac20d2a50655b3199cac8a737399ae1e/241f95cad1c8a786caf4d4aa6b09c93d71cf50b3.jpg)
将数据库A中某表的的某列字段，更新到数据库B中某表的某列字段：（use master 数据库）
update a
set a.name=b.name 
from temp1.dbo.tableA a,temp2.dbo.tableA b
where a.id=b.id
