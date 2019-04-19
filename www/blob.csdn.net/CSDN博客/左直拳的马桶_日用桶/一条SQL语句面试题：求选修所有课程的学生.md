# 一条SQL语句面试题：求选修所有课程的学生 - 左直拳的马桶_日用桶 - CSDN博客
2015年03月12日 00:13:40[左直拳](https://me.csdn.net/leftfist)阅读数：4486
前几天求职面试，有一道SQL题：给出三个表：学生、课程、成绩，求选修了所有课程的学生。
一道看似很简单的问题，把我难住了，我改了又改，涂涂画画，抓耳挠腮，因为试卷没有多少空白位置了，最后只好放弃。心情大受影响，尽管最后还是获得offer。
但是心中有愧呀！
于是在机器上试了试：
先建好表
```
use test;
go
create table student(sno varchar(50) not null,name varchar(50) not null);
insert into student(sno,name) values('001','张三');
insert into student(sno,name) values('002','李四');
insert into student(sno,name) values('003','王五');
create table class(cno varchar(50) not null,name varchar(50) not null) 
insert into class(cno,name) values('c01','数据结构');
insert into class(cno,name) values('c02','操作系统');
insert into class(cno,name) values('c03','计算机组成原理');
insert into class(cno,name) values('c04','网络基础');
create table score(sno varchar(50) not null,cno varchar(50) not null,score decimal(18,2) not null) 
insert into score(sno,cno,score) values('001','c01',80);
insert into score(sno,cno,score) values('001','c02',85);
insert into score(sno,cno,score) values('001','c03',89);
insert into score(sno,cno,score) values('001','c04',87);
insert into score(sno,cno,score) values('002','c01',80);
insert into score(sno,cno,score) values('003','c04',70);
```
我想到了三种写法：
1、
**[sql]**[view plain](http://blog.csdn.net/leftfist/article/details/44206673#)[copy](http://blog.csdn.net/leftfist/article/details/44206673#)
- select * from student s  
- wherenot exists(select 1 from class c  
- wherenot exists(select 1 from score   
- where sno=s.sno and cno=c.cno));  
两个not exists。我当时是只写了一个。
由内嵌到外部，
1）不存在一门为当前学生所选修的课程
select 1 from class c where not exists(select 1 from score where sno=s.sno and cno=c.cno)
2）不存在 情况1），也就是不存在当前学生有没选修的课程这种情况
换言之，当前学生选修了所有的课程
2、
**[sql]**[view plain](http://blog.csdn.net/leftfist/article/details/44206673#)[copy](http://blog.csdn.net/leftfist/article/details/44206673#)
- select * from student where sno notin(   
- select st.sno from student st,class c  
- wherenot exists(select 1 from score   
- where sno=st.sno and cno=c.cno)  
- )  
嵌套里面的语句是有未选修课程的学生
然后外部是不在此名单内的学生
3、
**[sql]**[view plain](http://blog.csdn.net/leftfist/article/details/44206673#)[copy](http://blog.csdn.net/leftfist/article/details/44206673#)
- select * from student where sno in(  
- select st.sno from student st  
- innerjoin score sc on st.sno=sc.sno  
- groupby st.sno  
- havingcount(*)=(selectcount(*) from class)  
-     )  
这个语句比较容易理解。但效率可能不高，我不确定(select count(*) from class)是否要执行很多次。
