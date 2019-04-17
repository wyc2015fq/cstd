# [CareerCup] 15.2 Renting Apartment II 租房之二 - Grandyang - 博客园







# [[CareerCup] 15.2 Renting Apartment II 租房之二](https://www.cnblogs.com/grandyang/p/5351362.html)







Write a SQL query to get a list of all buildings and the number of open requests (Requests in which status equals 'Open').



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



这道题让我们返回所有的building，并标记出来每个building有多少个Open的requests，那么我们首先要计算每个building的Open的request的个数，然后再和Buildings表联合返回对应的BuildingName，因为Requests表里对应的是Apartment和request，而一个Building里可能有很多个Apartment，所以我们先要联合Apartments表和Requests表来计算每个building的Open请求的个数，我们用内交Inner Join来做，通过AptID列来内交Apartments表和Requests表，然后通过BuildingID来群组，并生成一个名为Count的列，然后再用Buildings表和Count列左交，这里需要注意下，如果某个building没有Open请求，那么我们需要返回0，即需要把NULL变为0，在MySQL里面我们用IFNULL函数来做，而SQL Server则用ISNULL，Oracle则用NVL，详细对比可参见[这里](http://www.w3school.com.cn/sql/sql_isnull.asp)。参见代码如下：



```
SELECT BuildingName, IFNULL(Count, 0) AS 'Count' FROM Buildings
LEFT JOIN
(SELECT Apartments.BuildingID, COUNT(*) AS 'Count' FROM Requests
INNER JOIN 
Apartments ON Requests.AptID = Apartments.AptID
WHERE Requests.Status = 'Open' GROUP BY Apartments.BuildingID) ReqCounts
ON ReqCounts.BuildingID = Buildings.BuildingID;
```



运行结果：

```
+---------------+-------+
| BuildingName  | Count |
+---------------+-------+
| Eastern Hills |     1 |
| East End      |     0 |
| North Park    |     0 |
| South Lake    |     1 |
| West Forest   |     1 |
+---------------+-------+
```



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












