# [LeetCode] Employees Earning More Than Their Managers 员工挣得比经理多 - Grandyang - 博客园







# [[LeetCode] Employees Earning More Than Their Managers 员工挣得比经理多](https://www.cnblogs.com/grandyang/p/5354306.html)







The `Employee` table holds all employees including their managers. Every employee has an Id, and there is also a column for the manager Id.
+----+-------+--------+-----------+
| Id | Name  | Salary | ManagerId |
+----+-------+--------+-----------+
| 1  | Joe   | 70000  | 3         |
| 2  | Henry | 80000  | 4         |
| 3  | Sam   | 60000  | NULL      |
| 4  | Max   | 90000  | NULL      |
+----+-------+--------+-----------+

Given the `Employee` table, write a SQL query that finds out employees who earn more than their managers. For the above table, Joe is the only employee who earns more than his manager.
+----------+
| Employee |
+----------+
| Joe      |
+----------+


这道题给我们了一个Employee表，里面有员工的薪水信息和其经理的信息，经理也属于员工，其经理Id为空，让我们找出薪水比其经理高的员工，那么就是一个很简单的比较问题了，我们可以生成两个实例对象进行内交通过ManagerId和Id，然后限制条件是一个Salary大于另一个即可：



解法一：

```
SELECT e1.Name FROM Employee e1
JOIN Employee e2 ON e1.ManagerId = e2.Id
WHERE e1.Salary > e2.Salary;
```



我们也可以不用Join，直接把条件都写到where里也行：



解法二：

```
SELECT e1.Name FROM Employee e1, Employee e2
WHERE e1.ManagerId = e2.Id AND e1.Salary > e2.Salary;
```



参考资料：

[https://leetcode.com/discuss/88189/two-straightforward-way-using-where-and-join](https://leetcode.com/discuss/88189/two-straightforward-way-using-where-and-join%20)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












