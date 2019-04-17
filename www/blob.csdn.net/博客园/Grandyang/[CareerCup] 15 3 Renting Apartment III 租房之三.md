# [CareerCup] 15.3 Renting Apartment III 租房之三 - Grandyang - 博客园







# [[CareerCup] 15.3 Renting Apartment III 租房之三](https://www.cnblogs.com/grandyang/p/5351390.html)







Building #11 is undergoing a major renovation. Implement a query to close all requests from apartments in this building. 



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



这道题让我们更新11号Building，将其所有的Apartment的状态改为Closed，那么我们需要在Requests表中改，该表是状态和Apartment的关系，所以还需要在Apartments表里找和Building的关系，我们用Update和Set关键字来更新Requests表，参见代码如下：



```
UPDATE Requests SET Status = 'Closed'
WHERE AptID IN 
(SELECT AptID FROM Apartments
WHERE BuildingID = 11);
```



运行结果：

```
+-----------+--------+-------+-------------+
| RequestID | Status | AptID | Description |
+-----------+--------+-------+-------------+
|        50 | Closed |   101 |             |
|        60 | Closed |   103 |             |
|        70 | Closed |   102 |             |
|        80 | Open   |   201 |             |
|        90 | Open   |   202 |             |
+-----------+--------+-------+-------------+
```



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)














