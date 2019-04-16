# MySQL数据库入门练习200句 - wayblink的博客 - CSDN博客





2016年12月22日 23:02:46[WayBling](https://me.csdn.net/picway)阅读数：12196标签：[mysql																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[SQL(MySQL HiveSQL SparkSQL)](https://blog.csdn.net/picway/article/category/6585910)








## 目的：

学习总是需要记忆遗忘再记忆反反复复才能印入脑海。IT技术尤其如此。最好解决遗忘的方式，就是多多反复，然而捧起一本大部头从新看起显然效率低下。对于数据库入门级别的知识，看过一两遍，也就能理解用法了，欠缺在于代码练习。因此最近在重刷MySQL入门的同时，总结起来练习所使用的代码。用于以后的温习。某一天发现自己生疏了，花一点时间敲上一通，记忆就回来了。

## 说明：

1 大部分的语句都非常简单，进行了少量的注释； 

2 代码稍有一些冗余，是为了熟练操作； 

3 有意设置了一些错误，注意数据库的返回的信息； 

4 所有测试在MariaDB上进行，MariaDB和MySQL几乎就是一样，但如果有错误，系手误或理解错误，望担待； 

5 本文共100句代码，覆盖从数据库创建删除，表创建删除修改，键创建删除，索引创建删除的内容，后续会继续增加； 

6 附《MySQL入门很简单》一书电子版，可用于配合练习，[网盘地址](http://pan.baidu.com/s/1sl70yPB)，密码：77p4
——2016/12/14

## 关于200句

延续我之前的[MySQL入门练习100句](http://blog.csdn.net/picway/article/details/53646994)，增加到200句，这部分主要涉及SQL语言的增删改查。比较烦琐的是查询，MySQL支持非常灵活多样的查询。这里也涉足皮毛，但对于熟悉操作仍然有益。部分内容稍欠周详，在后续我自己练习的时候会再补上。

## 代码

```
//重建数据库
101, create database testdatabase;use database testdatabase;
102, create table tt1(id int, name varchar(20),age int,sex boolean);
103, show tables;desc tt1;

//插入
104, insert into tt1 values(1,"zhang",25,0);
105, insert into tt1 values(2,"wang",25,1);
106, select * from tt1;
107, insert into tt1(id,name,age,sex) values(3,"li",28,1);
108, insert into tt1(id,name,sex,age) values(4,"sun",0,22);
109, insert into tt1(id,name,sex) values(5,"zhao",30,1);
110, insert into tt1(id,age,name) values(6,"he",47,0);
111, insert into tt1(id,age,name) values(7,"chen",22,1),(7,"zhang",22,1),(7,"xie",32,1);
112, select * from tt1;

//修改
113, update tt1 set id=10,name="new",age=100,sex=0 where id=1; select * from tt1;
114, update tt1 set id=11,name="new" where id=2,age=25; select *from tt1;
115, update tt1 set id=12,sex=1 where id=7; select * from tt1;
116, update tt1 set sex=1 where id>3;
117, update tt1 set sex=0 where id<4;


//删除
118, delete from tt1 where id=1;select * from tt1;
119, delete from tt1 where id=12;select * from tt1;

//查询
120, alter table tt1 add address varchar(30);
121, update tt1 set address="Beijing" where sex=1;
122, update tt1 set address="Shanghai" where sex=0;
//简单查询
123, select id from tt1;
124, select id,name from tt1;
125, select id,name,address from tt1;
//条件查询
126, select id,name,address from tt1 where address="Beijing";
127, select * from tt1 where id in(2,3,4);
128, select * from tt1 where id not in(2,3,4);
129, select * from tt1 where id between 2 and 5;
130, select * from tt1 where id not between 2 and 5;
131, select * from tt1 where address like "beijing";
132, select * from tt1 where address like "bei";
133, select * from tt1 where address like "bei%";
134, select * from tt1 where address not like "bei%";
135, select * from tt1 where address is null;
136, select * from tt1 where address is not null;
137, select * from tt1 where age<20 and sex=1;
138, select * from tt1 where sex=0 or age>30;
//查询结果不重复
139, select distinct address from tt1;  
//查询结果排序
140, select * from tt1 order by age;
141, select * from tt1 order by age asc;
142, select * from tt1 order by age desc;
//分组查询
143, select * from tt1 group by sex;//单独使用group by 只会选择每个分组的一条记录
//group by 与 group_concat结合使用
144, select group_concat(name),sex from tt1 group by sex;
145, select group_concat(name),group_concat(age),sex from tt1 group by sex;
146, select group_concat(name,age),sex from tt1 group by sex;
//group by与集合函数结合使用
147, select sex,count(sex) from tt1 group by sex;
148, select sex,count(sex) from tt1 group by sex having sex>2;//having用于对分组后的结果加限制条件
149, select sex,count(sex) from tt1 group by sex having count(sex)>2;
//with rollup
150, select sex,count(sex) from tt1 group by sex with rollup;//在显示记录的最后一行加一条，记录是上面所有记录的和，通常用于合计数量
//limit显示限制
151, select * from tt1;
152, select * from tt1 limit 2;
153, select * from tt1 limit 3;
154, select * from tt1 limit 0,2;
155, select * from tt1 limit 1,2;
//使用集合函数查询
//为了更好的理解本问题，新建一个表
156, create table grade(id int,name varchar(10),subject varchar(10),score int,sex boolean);
157, insert into grade values(1,"wang","math",100,1),(1,"wang","english",96,1),(1,"wang","physics",90,1);
     insert into grade values(2,"li","math",96,1),(2,"li","english",85,1),(2,"li","physics",99,1);
     insert into grade values(3,"sun","math",85,0),(3,"sun","english",98,0),(3,"sun","physics",80,0);
158, select * from grade;
159, select count(*) from grade;
160, select id,name,sum(score) from grade where id=1;
161, select id,name,sun(score) from grade group by id;
162, select id,name,sum(score) from grade group by id order by sum(score) desc;
163, select id,name,avg(score) from grade where id=2;
164, select id,name,avg(score),sum(score) from grade where id =3;
165, select id,name,avg(score) from grade group by id;
166, select subject,avg(score) from grade group by subject;* from 
167, select subject,avg(score) from grade group by subject order by avg(score);
168, select name,max(score) from grade where subject="math";
169, select name,max(score) from grade where subject="english";
//连接查询
    //内连接
170, create table stu(id int,name varchar(10),age int,sex boolean);
171, insert into stu values(1,"wang",25,1),(2,"li",23,1),(3,"sun",23,0),(4,"zhou",27,1),(5,"zhang",22,0);
172, select id,name,age,sex,score from stu,grade where stu.id=grade.id;
173, select stu.id,stu.name,stu.age,stu.sex,score from stu,grade where stu.id=grade.id;
174, select stu.id,stu.name,stu.age,stu.sex,score from stu,grade where stu.id=grade.id and score>90;
    //外连接
175, select stu.id,stu.name,stu.age,stu.sex,score from stu left join grade on stu.id=grade.id;
176, select stu.id,stu.name,stu.age,stu,sex,score from stu right join grade on stu.id=grade.id;
177, insert into grade values(4,"hu","math",99,1),(5,"liu","english",97,0);
178, select stu.id,stu.name,stu.age,stu.sex,subject,score from stu left join grade on stu.id=grade.id;
179, select stu.id,stu.name,stu.age,stu.sex,subject,score from stu right join grade on stu.id=grade.id;
    //子查询
180, select * from stu where id in (select *from grade);
181, select * from grade where id in (select * from stu);
182, create table scholarship(level int,money int,score int);
183, insert into scholarship values(1,10000,90),(2,5000,85),(3,3000,80);
184, select id,name,score from grade where score>=(select score from scholarship where level=1);
185, select id,name,score from grade where score>=(select min(score) from scholarship);
    //exists子查询
186, select * from stu where exists (select name from grade where id=1);
187, select * from grade where score>any(select score from scholarship);
188, select * from grade where score>all(select score from scholarship);
//合并查询结果
189, select name from stu union select name from grade;
190, select name from stu union all select name from grade;
//别名
191, select * from scholarship s where s.level=1;
192, select * from scholarship s where s.money=5000;
193, select s.name from stu s,grade g where s.name=g.name;
194, select subject as su,score as sc from grade;
195, select * from stu where name regexp "^w";
196, select * from stu where name regexp "g$";
197, select * from stu where name regexp "^w.g$";
198, select * from stu where name regexp "^w..g$";
199, select * from stu where name regexp "[aeo]";
200, select * from stu where name regexp "[a-g]";
```





