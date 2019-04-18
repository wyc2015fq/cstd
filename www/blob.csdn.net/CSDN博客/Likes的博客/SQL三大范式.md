# SQL三大范式 - Likes的博客 - CSDN博客
2019年03月29日 15:32:22[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：24标签：[SQL																[范式](https://so.csdn.net/so/search/s.do?q=范式&t=blog)](https://so.csdn.net/so/search/s.do?q=SQL&t=blog)
个人分类：[postgreSQL](https://blog.csdn.net/songchuwang1868/article/category/8532933)
[http://www.cnblogs.com/haofaner/archive/2011/12/20/2294461.html](http://www.cnblogs.com/haofaner/archive/2011/12/20/2294461.html)
****第一范式(1NF)****
****(必须有主键，列不可分)****
数据库表中的任何字段都是单一属性的，不可再分
create table aa(id int,NameAge varchar(100)) 
insert aa values(1,''无限-女 '') 
没有达到第一范式 
create table aa(id int,name varcahr(10),age char(2)) 
insert aa values(1,''无限'',''女 '') 
达到第一范式
****第二范式(2NF)****
数据库表中非关键字段对任一候选关键字段的 都 不存在部分函数依赖
**(当一个表是复合主键时，非主键的字段不依赖于部分主键(即必须依赖于全部的主键字段))**
create table sci(
   sno int(32),cno int(32),grade int(32),credit int(32),
primary key sno,cno
)
课程(cno)1---1学分(credit)
学生(sno)n---n课程(cno)
学生+课程--->分数(grade)
sci
sno cno grade credit
1    1   60     80
2    1    90     80
3   1    70     80
.   .   .      .
.   .   .     .
.    .   .     .
如此以来，学分被大量重复存储，数据冗余
如要某课程学分，则要大量重复操作
如要加新课程，由于sno和cno共同做为主键，则在加入新课程时，必须有人选该课
如某学生某课程结业，则该学生其它课程信息也同时被删除了
总之
这种设计不太好吧，非关键字属性credit仅函数依赖于cno，也就是credit部分依赖组合关键字（sno,cno）而不是完全依赖
解决
分成两个关系模式 sc1（sno,cno,grade），c2（cno,credit）。新关系包括两个关系模式，它们之间通过sc1中的外关键字cno相联系，需要时再进行自然联接，恢复了原来的关系
**第三范式(3NF)**
关系模式R（U，F）中的所有非主属性对任何候选关键字都不存在传递依赖
Likes：即所有的非主键属性都依赖于主键属性，不能依赖于非主键属性，如果依赖则应该拆分表，是被依赖的非主键属性作为此表的外键，新表的主键。
例----S1（SNO，SNAME，DNO，   DNAME， LOCATION)
          学号 姓名   所在系 系名称 系地址 
关键字SNO决定各个属性。由于是单个关键字，没有部分依赖的问题，肯定是2NF。但这关系肯定有大量的冗余，有关学生所在的几个属性DNO，DNAME，LOCATION将重复存储，插入，删除和修改时也将产生类似以上例的情况。 
原因:关系中存在传递依赖造成的。即SNO 1->1 DNO。 而DNO 1->n SNO却不存在， 而DNO -> LOCATION存在, 因此关键辽 SNO 对 LOCATION 函数决定是通过传递依赖 SNO -> LOCATION 实现的。也就是说，SNO不直接决定非主属性LOCATION。 
解决目地：每个关系模式中不能留有传递依赖。 
解决方法：分为两个关系 S（SNO，SNAME，DNO）（Likes：此时DNO为外键），D（DNO，DNAME，LOCATION） 
注意：关系S中不能没有外关键字DNO。否则两个关系之间失去联系
**鲍依斯-科得范式（BCNF）**
在3NF的基础上，库表中任何字段对任一候选关键字段的传递函数依赖都不存在
仓库管理关系表为StorehouseManage(仓库ID, 存储物品ID, 管理员ID, 数量),且管理员1---1仓库；仓库1---n物品。这个数据库表中存在如下决定关系： 
　　(仓库ID, 存储物品ID) →(管理员ID, 数量)
(管理员ID, 存储物品ID) → (仓库ID, 数量)
所以，(仓库ID, 存储物品ID)和(管理员ID, 存储物品ID)都是StorehouseManage的候选关键字，表中的唯一非关键字段为数量，它是符合第三范式的。但是，由于存在如下决定关系：
(仓库ID) → (管理员ID)
(管理员ID) → (仓库ID)
即存在关键字段决定关键字段的情况，所以其不符合BCNF范式
解决:
把仓库管理关系表分解为二个关系表： 
　　仓库管理：StorehouseManage(仓库ID, 管理员ID)
仓库：Storehouse(仓库ID, 存储物品ID, 数量)
