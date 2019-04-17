# [LeetCode] Department Highest Salary 系里最高薪水 - Grandyang - 博客园







# [[LeetCode] Department Highest Salary 系里最高薪水](https://www.cnblogs.com/grandyang/p/5366412.html)







The `Employee` table holds all employees. Every employee has an Id, a salary, and there is also a column for the department Id.
+----+-------+--------+--------------+
| Id | Name  | Salary | DepartmentId |
+----+-------+--------+--------------+
| 1  | Joe   | 70000  | 1            |
| 2  | Henry | 80000  | 2            |
| 3  | Sam   | 60000  | 2            |
| 4  | Max   | 90000  | 1            |
+----+-------+--------+--------------+

The `Department` table holds all departments of the company.
+----+----------+
| Id | Name     |
+----+----------+
| 1  | IT       |
| 2  | Sales    |
+----+----------+

Write a SQL query to find employees who have the highest salary in each of the departments. For the above tables, Max has the highest salary in the IT department and Henry has the highest salary in the Sales department.
+------------+----------+--------+
| Department | Employee | Salary |
+------------+----------+--------+
| IT         | Max      | 90000  |
| Sales      | Henry    | 80000  |
+------------+----------+--------+



这道题让给了我们两张表，Employee表和Department表，让我们找系里面薪水最高的人的，实际上这题是[Second Highest Salary](http://www.cnblogs.com/grandyang/p/5348961.html)和[Combine Two Tables](http://www.cnblogs.com/grandyang/p/5348900.html)的结合题，我们既需要联合两表，又要找到最高薪水，那么我们首先让两个表内交起来，然后将结果表需要的列都标明，然后就是要找最高的薪水，我们用Max关键字来实现，参见代码如下：



解法一：

```
SELECT d.Name AS Department, e1.Name AS Employee, e1.Salary FROM Employee e1
JOIN Department d ON e1.DepartmentId = d.Id WHERE Salary IN 
(SELECT MAX(Salary) FROM Employee e2 WHERE e1.DepartmentId = e2.DepartmentId);
```



我们也可以不用Join关键字，直接用Where将两表连起来，然后找最高薪水的方法和上面相同：



解法二：

```
SELECT d.Name AS Department, e.Name AS Employee, e.Salary FROM Employee e, Department d
WHERE e.DepartmentId = d.Id AND e.Salary = (SELECT MAX(Salary) FROM Employee e2 WHERE e2.DepartmentId = d.Id);
```



下面这种方法没用用到Max关键字，而是用了>=符号，实现的效果跟Max关键字相同，参见代码如下：



解法三：

```
SELECT d.Name AS Department, e.Name AS Employee, e.Salary FROM Employee e, Department d
WHERE e.DepartmentId = d.Id AND e.Salary >= ALL (SELECT Salary FROM Employee e2 WHERE e2.DepartmentId = d.Id);
```



类似题目：

[Second Highest Salary](http://www.cnblogs.com/grandyang/p/5348961.html)

[Combine Two Tables](http://www.cnblogs.com/grandyang/p/5348900.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












