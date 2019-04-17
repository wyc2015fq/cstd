# [LeetCode] Department Top Three Salaries 系里前三高薪水 - Grandyang - 博客园







# [[LeetCode] Department Top Three Salaries 系里前三高薪水](https://www.cnblogs.com/grandyang/p/5367670.html)







The `Employee` table holds all employees. Every employee has an Id, and there is also a column for the department Id.
+----+-------+--------+--------------+
| Id | Name  | Salary | DepartmentId |
+----+-------+--------+--------------+
| 1  | Joe   | 70000  | 1            |
| 2  | Henry | 80000  | 2            |
| 3  | Sam   | 60000  | 2            |
| 4  | Max   | 90000  | 1            |
| 5  | Janet | 69000  | 1            |
| 6  | Randy | 85000  | 1            |
+----+-------+--------+--------------+

The `Department` table holds all departments of the company.
+----+----------+
| Id | Name     |
+----+----------+
| 1  | IT       |
| 2  | Sales    |
+----+----------+

Write a SQL query to find employees who earn the top three salaries in each of the department. For the above tables, your SQL query should return the following rows.
+------------+----------+--------+
| Department | Employee | Salary |
+------------+----------+--------+
| IT         | Max      | 90000  |
| IT         | Randy    | 85000  |
| IT         | Joe      | 70000  |
| Sales      | Henry    | 80000  |
| Sales      | Sam      | 60000  |
+------------+----------+--------+


这道题是之前那道[Department Highest Salary](http://www.cnblogs.com/grandyang/p/5366412.html)的拓展，难度标记为Hard，还是蛮有难度的一道题，综合了前面很多题的知识点，首先看使用Select Count(Distinct)的方法，我们内交Employee和Department两张表，然后我们找出比当前薪水高的最多只能有两个，那么前三高的都能被取出来了，参见代码如下：



解法一：

```
SELECT d.Name AS Department, e.Name AS Employee, e.Salary FROM Employee e
JOIN Department d on e.DepartmentId = d.Id
WHERE (SELECT COUNT(DISTINCT Salary) FROM Employee WHERE Salary > e.Salary
AND DepartmentId = d.Id) < 3 ORDER BY d.Name, e.Salary DESC;
```



下面这种方法将上面方法中的<3换成了IN (0, 1, 2)，是一样的效果：



解法二：

```
SELECT d.Name AS Department, e.Name AS Employee, e.Salary FROM Employee e, Department d
WHERE (SELECT COUNT(DISTINCT Salary) FROM Employee WHERE Salary > e.Salary
AND DepartmentId = d.Id) IN (0, 1, 2) AND e.DepartmentId = d.Id ORDER BY d.Name, e.Salary DESC;
```



或者我们也可以使用Group by Having Count(Distinct ..) 关键字来做：



解法三：

```
SELECT d.Name AS Department, e.Name AS Employee, e.Salary FROM 
(SELECT e1.Name, e1.Salary, e1.DepartmentId FROM Employee e1 JOIN Employee e2 
ON e1.DepartmentId = e2.DepartmentId AND e1.Salary <= e2.Salary GROUP BY e1.Id 
HAVING COUNT(DISTINCT e2.Salary) <= 3) e JOIN Department d ON e.DepartmentId = d.Id 
ORDER BY d.Name, e.Salary DESC;
```



下面这种方法略微复杂一些，用到了变量，跟[Consecutive Numbers](http://www.cnblogs.com/grandyang/p/5354173.html)中的解法三使用的方法一样，目的是为了给每个人都按照薪水的高低增加一个rank，最后返回rank值小于等于3的项即可，参见代码如下：



解法四：

```
SELECT d.Name AS Department, e.Name AS Employee, e.Salary FROM 
(SELECT Name, Salary, DepartmentId,
@rank := IF(@pre_d = DepartmentId, @rank + (@pre_s <> Salary), 1) AS rank,
@pre_d := DepartmentId, @pre_s := Salary 
FROM Employee, (SELECT @pre_d := -1, @pre_s := -1, @rank := 1) AS init
ORDER BY DepartmentId, Salary DESC) e JOIN Department d ON e.DepartmentId = d.Id
WHERE e.rank <= 3 ORDER BY d.Name, e.Salary DESC;
```



类似题目：

[Department Highest Salary](http://www.cnblogs.com/grandyang/p/5366412.html)

[Second Highest Salary](http://www.cnblogs.com/grandyang/p/5348961.html)

[Combine Two Tables](http://www.cnblogs.com/grandyang/p/5348900.html)



参考资料：

[https://leetcode.com/discuss/23002/my-tidy-solution](https://leetcode.com/discuss/23002/my-tidy-solution)

[https://leetcode.com/discuss/91087/yet-another-solution-using-having-count-distinct](https://leetcode.com/discuss/91087/yet-another-solution-using-having-count-distinct)

[https://leetcode.com/discuss/69880/two-solutions-1-count-join-2-three-variables-join](https://leetcode.com/discuss/69880/two-solutions-1-count-join-2-three-variables-join)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












