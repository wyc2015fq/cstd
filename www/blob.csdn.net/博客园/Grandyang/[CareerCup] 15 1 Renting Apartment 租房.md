# [CareerCup] 15.1 Renting Apartment 租房 - Grandyang - 博客园







# [[CareerCup] 15.1 Renting Apartment 租房](https://www.cnblogs.com/grandyang/p/5349036.html)







Write a SQL query to get a list of tenants who are renting more than one apartment.



-- TABLE Apartments

```
+-------+------------+------------+
| AptID | UnitNumber | BuildingID |
+-------+------------+------------+
|   101 | A1         |         11 |
|   102 | A2         |         12 |
|   103 | A3         |         13 |
|   201 | B1         |         14 |
|   202 | B2         |         15 |
+-------+------------+------------+
```



-- TABLE Buildings

```
+------------+-----------+---------------+---------------+
| BuildingID | ComplexID | BuildingName  | Address       |
+------------+-----------+---------------+---------------+
|         11 |         1 | Eastern Hills | San Diego, CA |
|         12 |         2 | East End      | Seattle, WA   |
|         13 |         3 | North Park    | New York      |
|         14 |         4 | South Lake    | Orlando, FL   |
|         15 |         5 | West Forest   | Atlanta, GA   |
+------------+-----------+---------------+---------------+
```



-- TABLE Tenants

```
+----------+------------+
| TenantID | TenantName |
+----------+------------+
|     1000 | Zhang San  |
|     1001 | Li Si      |
|     1002 | Wang Wu    |
|     1003 | Yang Liu   |
+----------+------------+
```



-- TABLE Complexes

```
+-----------+---------------+
| ComplexID | ComplexName   |
+-----------+---------------+
|         1 | Luxuary World |
|         2 | Paradise      |
|         3 | Woderland     |
|         4 | Dreamland     |
|         5 | LostParis     |
+-----------+---------------+
```



-- TABLE AptTenants

```
+----------+-------+
| TenantID | AptID |
+----------+-------+
|     1000 |   102 |
|     1001 |   102 |
|     1002 |   101 |
|     1002 |   103 |
|     1002 |   201 |
|     1003 |   202 |
+----------+-------+
```



-- TABLE Requests

```
+-----------+--------+-------+-------------+
| RequestID | Status | AptID | Description |
+-----------+--------+-------+-------------+
|        50 | Open   |   101 |             |
|        60 | Closed |   103 |             |
|        70 | Closed |   102 |             |
|        80 | Open   |   201 |             |
|        90 | Open   |   202 |             |
+-----------+--------+-------+-------------+
```



这道题让我们租了不止一间公寓的人，那么我们需要两个表Tenants和AptTenants，其他的表都不需要，那么我们可以用Inner Join来关联两个表，关于SQL的各种Join请参见我之前的博客[SQL Left Join, Right Join, Inner Join, and Natural Join 各种Join小结](http://www.cnblogs.com/grandyang/p/5346963.html)，然后我们还需要用Group by和Count关键字来表示在AptTenants表中出现的次数大于1的TenantID，然后在Tenants表中找到名字返回：



解法一：

```
SELECT TenantName FROM Tenants
INNER JOIN
(SELECT TenantID FROM AptTenants
GROUP BY TenantID HAVING COUNT(*) > 1) C
ON Tenants.TenantID = C.TenantID;
```



下面这种解法用了Using关键字指定了相同列TenantID：



解法二：

```
SELECT TenantName FROM Tenants
INNER JOIN
(SELECT TenantID FROM AptTenants
GROUP BY TenantID HAVING COUNT(*) > 1) C
USING (TenantID);
```



运行结果：

```
+------------+
| TenantName |
+------------+
| Wang Wu    |
+------------+
```



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












