# oracle--left join and 和left join where的区别 - z69183787的专栏 - CSDN博客
2014年04月03日 18:33:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：800
    开发程序时，经常会遇到left join，inner join的语句，Join是关系型数据库系统的重要操作之一，相对来说速度要快一些，所以大家一般都会优先选择join语句。
     但是在做程序时，对于join的一些用法却不一定很清晰。今天给大家讲的是left join and 和left join where。
 ﻿数据库在通过连接两张或多张表来返回记录时，都会生成一张中间的临时表，然后再将这张临时表返回给用户。
     在使用left jion on时，and和where条件的区别如下：
     1、 on条件是在生成临时表时使用的条件，它不管on中的条件是否为真，都会返回左边表中的记录，and只会过滤掉B表中的记录。B表中不符合条件的部分全部被设置为null。
     2、where条件是在临时表生成好后，再对临时表进行过滤的条件。这时已经没有left join的含义（必须返回左边表的记录）了，条件不为真的就全部过滤掉。﻿
﻿
    示例：
建表语句：
      create table tmp_lxq_1
(
  id varchar2(10),
  name varchar2(20)
);
insert   into tmp_lxq_1
select '1','张三' from dual;
insert   into tmp_lxq_1
select '2','李四' from dual;
insert   into tmp_lxq_1
select '3','王五' from dual;
commit;
tmp_lxq_1表结果：
ID NAME
1 张三
2 李四
3 王五
drop table tmp_lxq_2;
create table tmp_lxq_2
(
  id varchar2(10),
  subject varchar2(30),
  score varchar2(30)
);
insert   into tmp_lxq_2
select '1','语文','80' from dual;
insert   into tmp_lxq_2
select '2','数学','90' from dual;
insert   into tmp_lxq_2
select '4','英语','60' from dual;
commit;
tmp_lxq_2结果：
ID SUBJECT SCORE
1 语文 80
2 数学 90
4 英语 60
然后运行下面几个语句：
1.inner join
inner jion 取A表和B表的交集，不管A表在左还是B表在左。
select a.id,a.name,b.id,b.subject,b.score from tmp_lxq_1 a
inner join tmp_lxq_2 b
on a.id=b.id;
结果：
ID NAME ID SUBJECT SCORE
1 张三 1 语文 80
2 李四 2 数学 90
2.left join 
left join是以A表的记录为基础的,A可以看成左表,B可以看成右表,left join是以左表为准的。换句话说,左表(A)的记录将会全部表示出来,而右表(B)只会显示符合搜索条件的记录(例子中为: A.ID = B.ID)。B表记录不足的地方均为NULL。
select a.id,a.name,b.id,b.subject,b.score from tmp_lxq_1 a
left join tmp_lxq_2 b
on a.id=b.id;
1 张三 1 语文 80
2 李四 2 数学 90
3 王五   
3.left join and
left join and也是以A表的记录为基础的,A可以看成左表,B可以看成右表,left join and是以左表为准的。换句话说,左表(A)的记录将会全部表示出来,而右表(B)只会显示符合搜索条件的记录(例子中为: A.ID = B.ID)。B表记录不足的地方均为NULL，加上and条件后，A表记录也将全部被表示出来，而B表只会将符合条件的记录显示出来，B表记录中不符合条件的地方均显示为null。
select a.id,a.name,b.id,b.subject,b.score from tmp_lxq_1 a
left join tmp_lxq_2 b
on a.id=b.id
and b.score>=80;
ID NAME ID SUBJECT SCORE
1 张三 1 语文 80
2 李四 2 数学 90
3 王五   
4.left join where 
left join where条件是在临时表生成好后，再对临时表进行过滤的条件。这时已经没有left join的含义（必须返回左边表的记录）了，条件不为真的就全部过滤掉。﻿
select a.id,a.name,b.id,b.subject,b.score from tmp_lxq_1 a
left join tmp_lxq_2 b
on a.id=b.id
where b.score>=80;
ID NAME ID SUBJECT SCORE
1 张三 1 语文 80
2 李四 2 数学 90
