# Mysql 练习:hrs数据库 - weixin_33985507的博客 - CSDN博客
2018年11月22日 20:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
hrs数据库里面有两个表
一个是Tbdept
![9277819-32b8f3a580eb4f0e.png](https://upload-images.jianshu.io/upload_images/9277819-32b8f3a580eb4f0e.png)
Tbdept.png
另一个是TbEmp
![9277819-12e07d589ce818a6.png](https://upload-images.jianshu.io/upload_images/9277819-12e07d589ce818a6.png)
TbEmp.png
## 创建库和表
-- 创建人力资源管理系统数据库
drop database if exists HRS;
create database HRS default charset utf8;
-- 切换数据库上下文环境
use HRS;
-- 创建部门表
create table TbDept
(
dno int comment '部门编号',
dname varchar(10) not null comment '部门名称',
dloc varchar(20) not null comment '所在地',
primary key (dno)
);
-- 添加部门记录
insert into TbDept values (10, '会计部', '北京');
insert into TbDept values (20, '研发部', '成都');
insert into TbDept values (30, '销售部', '重庆');
insert into TbDept values (40, '运维部', '深圳');
-- 创建员工表
create table TbEmp
(
eno int not null comment '员工编号',
ename varchar(20) not null comment '员工姓名',
job varchar(20) not null comment '员工职位',
mgr int comment '主管编号',
sal int not null comment '员工月薪',
comm int comment '每月补贴',
dno int comment '所在部门编号',
primary key (eno)
);
-- 添加外键约束
alter table TbEmp add constraint fk_dno foreign key (dno) references TbDept (dno);
-- 添加员工记录
insert into TbEmp values (7800, '张三丰', '总裁', null, 9000, 1200, 20);
insert into TbEmp values (2056, '乔峰', '分析师', 7800, 5000, 1500, 20);
insert into TbEmp values (3088, '李莫愁', '设计师', 2056, 3500, 800, 20);
insert into TbEmp values (3211, '张无忌', '程序员', 2056, 3200, null, 20);
insert into TbEmp values (3233, '丘处机', '程序员', 2056, 3400, null, 20);
insert into TbEmp values (3251, '张翠山', '程序员', 2056, 4000, null, 20);
insert into TbEmp values (5566, '宋远桥', '会计师', 7800, 4000, 1000, 10);
insert into TbEmp values (5234, '郭靖', '出纳', 5566, 2000, null, 10);
insert into TbEmp values (3344, '黄蓉', '销售主管', 7800, 3000, 800, 30);
insert into TbEmp values (1359, '胡一刀', '销售员', 3344, 1800, 200, 30);
insert into TbEmp values (4466, '苗人凤', '销售员', 3344, 2500, null, 30);
insert into TbEmp values (3244, '欧阳锋', '程序员', 3088, 3200, null, 20);
insert into TbEmp values (3577, '杨过', '会计', 5566, 2200, null, 10);
insert into TbEmp values (3588, '朱九真', '会计', 5566, 2500, null, 10);
## 练习
-- 查询薪资最高的员工姓名和工资
-- 知识点(子连接)
select ename,sal from TbEmp where sal=(select max(sal) from TbEmp)
-- 查询员工的姓名和年薪((月薪+补贴)*12)-- (知识点)把null变成0进行运算select ename, (sal + ifnull(comm,0))*12 as 年薪 from tbemp
select ename, (sal + if(comm is null ,0,comm))*12 as 年薪 from tbemp
-- 查询有员工的部门的编号和人数
-- 知识点(分组)
select dno, count(dno) from TbEmp group by dno
-- 查询所有部门的名称和人数
-- 知识点(左外连接)
select dname as 部门名称 ,ifnull(countdno,0) as 部门人数 from TbDept as t1  left join
(select dno,count(dno) as countdno from TbEmp group by dno) as t2
on t1.dno= t2.dno
-- 查询薪资最高的员工(Boss除外)的姓名和工资
-- 知识点(子连接,is null的使用)
select ename, sal from TbEmp where sal=(select max(sal) from TbEmp where mgr is not null);
-- 查询薪水超过平均薪水的员工的姓名和工资
-- 知识点(子连接)
select ename, sal from TbEmp where  sal>(select avg(sal) from TbEmp)
-- 查询薪水超过其所在部门平均薪水的员工的姓名、部门编号和工资
-- **知识点(连接查询,分组,筛选条件,必须说明dno来自哪个表)**
select ename,sal,t1.dno from TbEmp as t1 inner join
(select  dno,avg(sal) as avgsal from TbEmp group by dno) as t2
on t1.dno =t2.dno and t1.sal>t2.avgsal -- 也可以把and写成where
-- 查询部门中薪水最高的人姓名、工资和所在部门名称
-- 知识点(用两次连接查询 inner join  从里往外写)
select ename, sal, dname from TbDept as t1 inner join
(select ename, sal, t2.dno from TbEmp as t2 inner join
(select dno, max(sal) as maxSal from TbEmp group by dno) t3
on t2.dno=t3.dno and sal=maxSal) as t4 on t1.dno=t4.dno;
-- 查询主管的姓名和职位
-- **知识点( 子连接,where in, 去重) 集合运算和去重性能都很差,尽量少用**
select ename,job from TbEmp  where eno in (select  distinct mgr from TbEmp where mgr is not null)
select ename ,job from TbEmp as t1  where exists
(select 'x' from TbEmp as t2 where t1.eno =t2.mgr)
**扩展**
```
如果数据非常多
 列如:几万学生 有学号和姓名
 查询一般都是输姓名 可以给姓名加个索引 这样速度可以快很多
 **创建索引
 create index 索引名 on 表名(列表(下标))优化时间
 索引不不能乱用 ,索引相当于一本书的目录 可以加速查询但是索引会让增删改变得更慢
 因为增删改数据时 索引也需要更新，索引要建在经常被用作筛选条件的列上
 删除索引
 alter table 表名 drop index 索引名
```
-- 查询薪资排名4~6名的员工姓名和工资
select ename,sal from TbEmp  order by sal desc limit 3,3
select ename,sal from TbEmp  order by sal desc limit 3 OFFSET 3
## 视图
create  viem 视图名 as(
表)；
**创建视图**
create viem vw_emp as
select eno,ename,job from TbEmp
**删除视图**
drop viem 删除视图名
**创建视图的目的是给普通用户部分权限来操作表**
create user 'aaa'@'localhost' identified by '123123'
grant select ,update on 库名.vw_emp to 'aaa'@'localhost'
**更新视图会导致表的更新**
