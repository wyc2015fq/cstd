# sql 练习(四) - weixin_33985507的博客 - CSDN博客
2017年10月27日 16:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
### 环境是mysql
[练习数据见SQL：练习的前期准备](https://www.jianshu.com/p/f5cfc965fcef)
[sql 练习(一)](https://www.jianshu.com/p/7ea88af1f147)
[sql 练习(二)](https://www.jianshu.com/p/5e2d9149a18a)
[sql 练习(三)](https://www.jianshu.com/p/06dbe9896319)
31、查询所有“女”教师和“女”同学的name、sex和birthday.
```
SELECT sname,ssex,sbirthday
  FROM student
 WHERE ssex = '女'
  UNION 
SELECT tname,tsex,tbirthday 
  FROM teacher
 WHERE tsex = '女'
```
32、查询成绩比该课程平均成绩低的同学的成绩表。
```
SELECT a.sno,a.cno,a.degree 
  FROM score a
  LEFT JOIN (
        SELECT AVG(degree) degree,cno
          FROM score
         GROUP BY cno)b
    ON a.cno=b.cno
 WHERE a.degree< b.degree
 
 
SELECT Sno,Cno,degree 
  FROM Score a 
 WHERE degree<(SELECT AVG(degree) FROM score b WHERE a.Cno=b.Cno)
```
33、查询所有任课教师的Tname和Depart.
```
SELECT tname,depart
  FROM teacher
 WHERE tno IN(SELECT tno FROM course )
 
 SELECT tname,depart 
   FROM teacher a 
  WHERE EXISTS (SELECT * FROM course b WHERE a.tno=b.tno)
```
34、查询所有未讲课的教师的Tname和Depart.
```
SELECT tname,depart 
  FROM Teacher 
 WHERE tno  NOT IN(SELECT tno FROM course)
```
35、查询至少有2名男生的班号。
```
SELECT class 
  FROM Student 
 WHERE Ssex='男' 
 GROUP BY class 
HAVING COUNT(SSex)>=2
```
36、查询不姓王的同学信息
```
SELECT *
  FROM student 
 WHERE sname NOT LIKE '王%'
```
37、查询Student表中每个学生的姓名和年龄。
```
SELECT Sname,(YEAR(NOW())-YEAR(Sbirthday)) age
  FROM student
```
38、查询Student表中最大和最小的Sbirthday日期值。
```
SELECT MAX(sbirthday),MIN(sbirthday)
  FROM student
```
39、以班号和年龄从大到小的顺序查询Student表中的全部记录。
```
SELECT *
  FROM student
 ORDER BY sbirthday ASC,class DESC
```
40、查询“男”教师及其所上的课程。
```
SELECT a.tname,b.cname 
  FROM teacher  a 
  JOIN course b
    ON a.tno = b.tno
 WHERE a.tsex ='男'
```
41、查询最高分同学的Sno、Cno和Degree列。
```
SELECT * 
  FROM score 
 WHERE degree=(SELECT MAX(degree)FROM score)
```
42、查询所有选修“计算机导论”课程的“男”同学的成绩表。
```
SELECT sno,degree
  FROM score
 WHERE sno IN (SELECT sno FROM student WHERE ssex='男')
   AND cno IN (SELECT cno FROM course WHERE cname='计算机导论')
```
43、查询和“李军”同性别的所有同学的Sname.
```
SELECT sname
  FROM student
 WHERE ssex=(SELECT ssex FROM student WHERE sname='李军')
```
44、查询日期
```
SELECT *
  FROM teacher 
 WHERE tbirthday=STR_TO_DATE('1972-05-05 00:00:00','%Y-%m-%d')
```
