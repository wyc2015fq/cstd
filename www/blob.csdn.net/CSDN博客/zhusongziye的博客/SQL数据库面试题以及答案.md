# SQL数据库面试题以及答案 - zhusongziye的博客 - CSDN博客





2018年03月21日 22:13:49[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：163








找了一道面试题的题目


题目如下：




Student(S#,Sname,Sage,Ssex) 学生表

S#：学号；Sname：学生姓名；Sage：学生年龄；Ssex：学生性别
Course(C#,Cname,T#) 课程表

C#,课程编号；Cname：课程名字；T#：教师编号
SC(S#,C#,score) 成绩表

S#：学号；C#,课程编号；score：成绩
Teacher(T#,Tname) 教师表

T#：教师编号； Tname：教师名字

问题：
1、查询“001”课程比“002”课程成绩高的所有学生的学号；
select a.S# from (select s#,score from SC where C#='001') a,(select s#,score from SC where C#='002') b where a.score>b.score and a.s#=b.s#;

2、查询平均成绩大于60分的同学的学号和平均成绩；
select S#,avg(score)
from sc
group by S# having avg(score) >60;

3、查询没有学全所有课的同学的学号、姓名；
select Student.S#,Student.Sname
from Student,SC
where Student.S#=SC.S# group byStudent.S#,Student.Sname having count(C#) <(select count(C#) from Course);

4、查询至少学过学号为“001”同学所有一门课的其他同学学号和姓名；
select distinct SC.S#,Sname
from Student,SC
where Student.S#=SC.S# and C# in (select C# from SC where S#='001');

5、删除学习“叶平”老师课的SC表记录；
Delect SC
from course ,Teacher
where Course.C#=SC.C# and Course.T#= Teacher.T# and Tname='叶平';

6、查询没学过“叶平”老师课的同学的学号、姓名；
select Student.S#,Student.Sname
from Student
where S# not in (select distinct( SC.S#) from SC,Course,Teacher whereSC.C#=Course.C# and Teacher.T#=Course.T# and Teacher.Tname='叶平');



