# [CareerCup] 15.7 Student Grade 学生成绩 - Grandyang - 博客园







# [[CareerCup] 15.7 Student Grade 学生成绩](https://www.cnblogs.com/grandyang/p/5380947.html)







15.7 Imagine a simple database storing information for students' grades. Design what this database might look like and provide a SQL query to return a list of the honor roll students (top 10%), sorted by their grade point average.



在一个简化的数据库中我们有三个表，Students表，Courses表和CourseEnrollment表如下：

TABLE Students

```
+-------------+-------------+
| Students                  |
+-------------+-------------+
| StudentID   | int(11)     |
| StudentName | varchar(30) |
| Address     | varchar(50) |
+-------------+-------------+
```



TABLE Courses

```
+-------------+-------------+
| Courses                   |
+-------------+-------------+
| CourseID    | int(11)     |
| CourseName  | varchar(30) |
| ProfessorID | int(11)     |
+-------------+-------------+
```



TABLE CourseEnrollment

```
+-----------+---------+
| CourseEnrollment    |
+-----------+---------+
| CourseID  | int(11) |
| StudentID | int(11) |
| Grade     | float   |
| Term      | int(11) |
+-----------+---------+
```



使用SQL Server的Top .. Percent 函数可以如下代码：

```
-- SQL Server (Incorrect Code)
SELECT TOP 10 PERCENT AVG(CourseEnrollment.Grade) AS GPA, CourseEnrollment.StudentID
FROM CourseEnrollment GROUP BY CourseEnrollment.StudentID ORDER BY AVG(CourseEnrollment.Grade);
```

由于我使用的是MySQL，本来可以用Limit关键字来做，但是MySQL的Limit关键字后面只能为常量，不能为变量，所以只能换一种写法，所以代码如下：

```
SET @limit = (SELECT 0.1 * COUNT(*) FROM CourseEnrollment);
SELECT Grade FROM (
SELECT *, @rownum := @rownum + 1 AS rank 
FROM CourseEnrollment, (SELECT @rownum := 0) init ORDER BY Grade DESC
) d WHERE rank <= @limit ;
```



但是上面的写法确实返回了top 10%的行，但是假如我们有100个学生，前15个学生都是4.0的GPA，而上面的方法只能返回10个学生，而我们需要返回那15个都是4.0的学生，所以为了实现这个，我们可以这么做：

```
-- SQL Server
DECLARE @GPACutOff FLOAT;
SET @GPACutOff = (SELECT MIN(GPA) AS 'GPAMin' FROM (
SELECT TOP 10 PERCENT AVG(CourseEnrollment.Grade) AS GPA, FROM CourseEnrollment
GROUP BY CourseEnrollment.StudentID ORDER BY GPA DESC) Grades);

SELECT StudentName, GPA FROM (
SELECT AVG(CourseEnrollment.Grade) AS GPA, CourseEnrollment.StudentID
FROM CourseEnrollment GROUP BY CourseEnrollment.StudentID 
HAVING AVG(CourseEnrollment.Grade) >= @GPACutOff) Honors
INNER JOIN Students ON Honors.StudentID = Students.StudentID;
```

上面的方法先定义了一个GPACutOff变量，算出了前10%的GPA，然后在后面的代码中遍历所有的GPA，返回所有大于等于GPACutOff的行。而是用MySQL可以写出实现同样功能的代码，参考了我之前的博客[Department Top Three Salaries](http://www.cnblogs.com/grandyang/p/5366412.html)，[Department Highest Salary](http://www.cnblogs.com/grandyang/p/5366412.html)和[Second Highest Salary](http://www.cnblogs.com/grandyang/p/5348961.html)，用更简洁的方式如下：

```
-- MySQL
SELECT c.Grade FROM CourseEnrollment c WHERE (SELECT COUNT(DISTINCT Grade) FROM CourseEnrollment 
WHERE Grade > c.Grade) < (SELECT 0.1 * COUNT(*) FROM CourseEnrollment) ORDER BY c.Grade DESC;
```



要注意那些潜在的假设，比如看上面的设计，一个潜在的假设就是每门课只能由一个教授交，而在一些学校，课程可能被多个教授交。但是，我们需要做一些假设，而潜在的一些不正确的假设需要多加注意。

我们需要在灵活性和复杂性之间做出平衡，比如设计一个课程可以被多个教授交的系统是增加了灵活性，但是也增加了复杂度，如果我们的系统对于任何情况都适用，那么系统就会无比的复杂。所以我们需要设计出一个相对灵活的系统，但仍然有假设和限定，这不仅需要数据库设计的知识，还包括了面向对象的设计。



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












