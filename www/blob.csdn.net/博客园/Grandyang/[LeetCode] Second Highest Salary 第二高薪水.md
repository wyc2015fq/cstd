# [LeetCode] Second Highest Salary 第二高薪水 - Grandyang - 博客园







# [[LeetCode] Second Highest Salary 第二高薪水](https://www.cnblogs.com/grandyang/p/5348961.html)







Write a SQL query to get the second highest salary from the `Employee` table.
+----+--------+
| Id | Salary |
+----+--------+
| 1  | 100    |
| 2  | 200    |
| 3  | 300    |
+----+--------+

For example, given the above Employee table, the second highest salary is `200`. If there is no second highest salary, then the query should return `null`.



这道题让我们找表中某列第二大的数，这道题有很多种解法，先来看一种使用Limit和Offset两个关键字的解法，MySQL中Limit后面的数字限制了我们返回数据的个数，Offset是偏移量，那么如果我们想找第二高薪水，我们首先可以先对薪水进行降序排列，然后我们将Offset设为1，那么就是从第二个开始，也就是第二高薪水，然后我们将Limit设为1，就是只取出第二高薪水，如果将Limit设为2，那么就将第二高和第三高薪水都取出来：



解法一：

```
SELECT Salary FROM Employee GROUP BY Salary
UNION ALL (SELECT NULL AS Salary)
ORDER BY Salary DESC LIMIT 1 OFFSET 1;
```



我们也可以使用Max函数来做，这个返回最大值，逻辑是我们取出的不包含最大值的数字中的最大值，即为第二大值：



解法二：

```
SELECT MAX(Salary) FROM Employee 
WHERE Salary NOT IN
(SELECT MAX(Salary) FROM Employee);
```



下面这种方法和上面基本一样，就是用小于号<代替了Not in关键字，效果相同：



解法三：

```
SELECT MAX(Salary) FROM Employee
Where Salary <
(SELECT MAX(Salary) FROM Employee);
```



最后来看一种可以扩展到找到第N高的薪水的方法，只要将下面语句中的1改为N-1即可，第二高的薪水带入N-1就是1，下面语句的逻辑是，假如我们要找第二高的薪水，那么我们允许其中一个最大值存在，然后在其余的数字中找出最大的，即为整个的第二大的值；



解法四：

```
SELECT MAX(Salary) FROM Employee E1
WHERE 1 =
(SELECT COUNT(DISTINCT(E2.Salary)) FROM Employee E2
WHERE E2.Salary > E1.Salary);
```



参考资料：

[https://leetcode.com/discuss/47041/very-very-simple-solution](https://leetcode.com/discuss/47041/very-very-simple-solution)

[https://leetcode.com/discuss/42849/general-solution-not-using-max](https://leetcode.com/discuss/42849/general-solution-not-using-max)

[https://leetcode.com/discuss/21751/simple-query-which-handles-the-null-situation](https://leetcode.com/discuss/21751/simple-query-which-handles-the-null-situation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












