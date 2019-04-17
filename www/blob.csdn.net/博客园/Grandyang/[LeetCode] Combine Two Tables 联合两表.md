# [LeetCode] Combine Two Tables 联合两表 - Grandyang - 博客园







# [[LeetCode] Combine Two Tables 联合两表](https://www.cnblogs.com/grandyang/p/5348900.html)







Table: `Person`
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| PersonId    | int     |
| FirstName   | varchar |
| LastName    | varchar |
+-------------+---------+
PersonId is the primary key column for this table.

Table: `Address`
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| AddressId   | int     |
| PersonId    | int     |
| City        | varchar |
| State       | varchar |
+-------------+---------+
AddressId is the primary key column for this table.



Write a SQL query for a report that provides the following information for each person in the Person table, regardless if there is an address for each of those people:
FirstName, LastName, City, State


LeetCode还出了是来到数据库的题，来那么也来做做吧，这道题是第一道，相对来说比较简单，是一道两表联合查找的问题，我们需要用到Join操作，关于一些Join操作可以看我之前的博客[SQL Left Join, Right Join, Inner Join, and Natural Join 各种Join小结](http://www.cnblogs.com/grandyang/p/5346963.html)，最直接的方法就是用Left Join来做，根据PersonId这项来把两个表联合起来：



解法一：

```
SELECT Person.FirstName, Person.LastName, Address.City, Address.State FROM Person LEFT JOIN Address ON Person.PersonId = Address.PersonId;
```



在使用Left Join时，我们也可以使用关键Using来声明我们相用哪个列名来进行联合：



解法二：

```
SELECT Person.FirstName, Person.LastName, Address.City, Address.State FROM Person LEFT JOIN Address USING(PersonId);
```



或者我们可以加上Natural关键字，这样我们就不用声明具体的列，MySQL可以自行搜索相同的列：



解法三：

```
SELECT Person.FirstName, Person.LastName, Address.City, Address.State FROM Person NATURAL LEFT JOIN Address;
```



参考资料：

[https://leetcode.com/discuss/21216/its-a-simple-question-of-left-join-my-solution-attached](https://leetcode.com/discuss/21216/its-a-simple-question-of-left-join-my-solution-attached)

[https://leetcode.com/discuss/53001/comparative-solution-between-left-using-natural-left-join](https://leetcode.com/discuss/53001/comparative-solution-between-left-using-natural-left-join)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














