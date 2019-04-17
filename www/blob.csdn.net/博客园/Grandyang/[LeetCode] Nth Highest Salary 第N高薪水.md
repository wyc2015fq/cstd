# [LeetCode] Nth Highest Salary 第N高薪水 - Grandyang - 博客园







# [[LeetCode] Nth Highest Salary 第N高薪水](https://www.cnblogs.com/grandyang/p/5348976.html)







Write a SQL query to get the *n*th highest salary from the `Employee` table.
+----+--------+
| Id | Salary |
+----+--------+
| 1  | 100    |
| 2  | 200    |
| 3  | 300    |
+----+--------+

For example, given the above Employee table, the *n*th highest salary where *n* = 2 is `200`. If there is no *n*th highest salary, then the query should return `null`.



这道题是之前那道[Second Highest Salary](http://www.cnblogs.com/grandyang/p/5348961.html)的拓展，根据之前那道题的做法，我们可以很容易的将其推展为N，根据对[Second Highest Salary](http://www.cnblogs.com/grandyang/p/5348961.html)中解法一的分析，我们只需要将OFFSET后面的1改为N-1就行了，但是这样MySQL会报错，估计不支持运算，那么我们可以在前面加一个SET N = N - 1，将N先变成N-1再做也是一样的：



解法一：

```
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  SET N = N - 1;
  RETURN (
      SELECT DISTINCT Salary FROM Employee GROUP BY Salary
      ORDER BY Salary DESC LIMIT 1 OFFSET N
  );
END
```



根据对[Second Highest Salary](http://www.cnblogs.com/grandyang/p/5348961.html)中解法四的分析，我们只需要将其1改为N-1即可，这里却支持N-1的计算，参见代码如下：



解法二：

```
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      SELECT MAX(Salary) FROM Employee E1
      WHERE N - 1 =
      (SELECT COUNT(DISTINCT(E2.Salary)) FROM Employee E2
      WHERE E2.Salary > E1.Salary)
  );
END
```



当然我们也可以通过将最后的>改为>=，这样我们就可以将N-1换成N了：



解法三：

```
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      SELECT MAX(Salary) FROM Employee E1
      WHERE N =
      (SELECT COUNT(DISTINCT(E2.Salary)) FROM Employee E2
      WHERE E2.Salary >= E1.Salary)
  );
END
```



类似题目：

[Second Highest Salary](http://www.cnblogs.com/grandyang/p/5348961.html)



参考资料：

[https://leetcode.com/discuss/88875/simple-answer-with-limit-and-offset](https://leetcode.com/discuss/88875/simple-answer-with-limit-and-offset)

[https://leetcode.com/discuss/63183/fastest-solution-without-using-order-declaring-variables](https://leetcode.com/discuss/63183/fastest-solution-without-using-order-declaring-variables)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












