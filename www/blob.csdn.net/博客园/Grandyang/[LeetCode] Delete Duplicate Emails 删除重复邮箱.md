# [LeetCode] Delete Duplicate Emails 删除重复邮箱 - Grandyang - 博客园







# [[LeetCode] Delete Duplicate Emails 删除重复邮箱](https://www.cnblogs.com/grandyang/p/5371227.html)







Write a SQL query to delete all duplicate email entries in a table named `Person`, keeping only unique emails based on its *smallest* Id.
+----+------------------+
| Id | Email            |
+----+------------------+
| 1  | john@example.com |
| 2  | bob@example.com  |
| 3  | john@example.com |
+----+------------------+
Id is the primary key column for this table.

For example, after running your query, the above `Person` table should have the following rows:
+----+------------------+
| Id | Email            |
+----+------------------+
| 1  | john@example.com |
| 2  | bob@example.com  |
+----+------------------+


这道题让我们删除重复邮箱，那我们可以首先找出所有不重复的邮箱，然后取个反就是重复的邮箱，都删掉即可，那么我们如何找出所有不重复的邮箱呢，我们可以按照邮箱群组起来，然后用Min关键字挑出较小的，然后取补集删除即可：



解法一：

```
DELETE FROM Person WHERE Id NOT IN
(SELECT Id FROM (SELECT MIN(Id) Id FROM Person GROUP BY Email) p);
```



我们也可以使用内交让两个表以邮箱关联起来，然后把相同邮箱且Id大的删除掉，参见代码如下：



解法二：

```
DELETE p2 FROM Person p1 JOIN Person p2 
ON p2.Email = p1.Email WHERE p2.Id > p1.Id;
```



我们也可以不用Join，而直接用where将两表关联起来也行：



解法三：

```
DELETE p2 FROM Person p1, Person p2
WHERE p1.Email = p2.Email AND p2.Id > p1.Id;
```



类似题目：

[Duplicate Emails](http://www.cnblogs.com/grandyang/p/5361967.html)



参考资料：

[https://leetcode.com/discuss/61176/simple-solution-using-a-self-join](https://leetcode.com/discuss/61176/simple-solution-using-a-self-join)

[https://leetcode.com/discuss/48403/my-answer-delete-duplicate-emails-with-double-nested-query](https://leetcode.com/discuss/48403/my-answer-delete-duplicate-emails-with-double-nested-query)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












