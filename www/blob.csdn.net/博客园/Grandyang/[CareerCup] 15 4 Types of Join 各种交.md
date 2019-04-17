# [CareerCup] 15.4 Types of Join 各种交 - Grandyang - 博客园







# [[CareerCup] 15.4 Types of Join 各种交](https://www.cnblogs.com/grandyang/p/5354535.html)







15.4 What are the different types of joins? Please explain how they differ and why certain types are better in certain situations. 



Join是用来联合两个表的，每个表至少需要有一列是相同的，不同的Join类型会返回不同的结果。我们来看一个例子，有两个表，普通饮料和无卡饮料如下：

-- TABLE RegularBeverages

```
+-----------+-----------+
| Name      | Code      |
+-----------+-----------+
| Budweiser | BUDWEISER |
| Coca-Cola | COCACOLA  |
| Pepsi     | PEPSI     |
+-----------+-----------+
```



-- TABLE CalorieFreeBeverages

```
+----------------+----------+
| Name           | Code     |
+----------------+----------+
| Diet Coca-Cola | COCACOLA |
| Fresca         | FRESCA   |
| Diet Pepsi     | PEPSI    |
| Pepsi Light    | PEPSI    |
| Purfied water  | Water    |
+----------------+----------+
```



如果我们想联合这两个表，有很多种Join可以使用：

**内交Inner Join**：只会显示相同列匹配的项：

```
SELECT * FROM RegularBeverages 
INNER JOIN CalorieFreeBeverages
ON RegularBeverages.Code = CalorieFreeBeverages.Code;

+-----------+----------+----------------+----------+
| Name      | Code     | Name           | Code     |
+-----------+----------+----------------+----------+
| Coca-Cola | COCACOLA | Diet Coca-Cola | COCACOLA |
| Pepsi     | PEPSI    | Diet Pepsi     | PEPSI    |
| Pepsi     | PEPSI    | Pepsi Light    | PEPSI    |
+-----------+----------+----------------+----------+
```



**外交Outer Join**：外交会包含内交的结果，同时也会包含一些没有匹配到的结果，外交有如下几种：

- 左交Left Outer Join (Left Join)，会包含左表的所有结果，如果没有匹配，右边就是NULL：

```
SELECT * FROM RegularBeverages 
LEFT JOIN CalorieFreeBeverages
ON RegularBeverages.Code = CalorieFreeBeverages.Code;

+-----------+-----------+----------------+----------+
| Name      | Code      | Name           | Code     |
+-----------+-----------+----------------+----------+
| Coca-Cola | COCACOLA  | Diet Coca-Cola | COCACOLA |
| Pepsi     | PEPSI     | Diet Pepsi     | PEPSI    |
| Pepsi     | PEPSI     | Pepsi Light    | PEPSI    |
| Budweiser | BUDWEISER | NULL           | NULL     |
+-----------+-----------+----------------+----------+
```



- 右交Right Outer Join (Right Join)，返回右表的所有项，如果没有匹配，左边就是NULL：

```
SELECT * FROM RegularBeverages 
RIGHT JOIN CalorieFreeBeverages
ON RegularBeverages.Code = CalorieFreeBeverages.Code;

+-----------+----------+----------------+----------+
| Name      | Code     | Name           | Code     |
+-----------+----------+----------------+----------+
| Coca-Cola | COCACOLA | Diet Coca-Cola | COCACOLA |
| Pepsi     | PEPSI    | Diet Pepsi     | PEPSI    |
| Pepsi     | PEPSI    | Pepsi Light    | PEPSI    |
| NULL      | NULL     | Fresca         | FRESCA   |
| NULL      | NULL     | Purfied water  | Water    |
+-----------+----------+----------------+----------+
```



- 全交Full Outer Join，联合左交和右交的结果，不论有没有匹配都把结果显示出来，由于MySQL中没有这个命令，所以我们用Union把左右交的结果并起来：

```
SELECT * FROM RegularBeverages 
LEFT JOIN CalorieFreeBeverages
ON RegularBeverages.Code = CalorieFreeBeverages.Code
UNION
SELECT * FROM RegularBeverages 
RIGHT JOIN CalorieFreeBeverages
ON RegularBeverages.Code = CalorieFreeBeverages.Code;

+-----------+-----------+----------------+----------+
| Name      | Code      | Name           | Code     |
+-----------+-----------+----------------+----------+
| Coca-Cola | COCACOLA  | Diet Coca-Cola | COCACOLA |
| Pepsi     | PEPSI     | Diet Pepsi     | PEPSI    |
| Pepsi     | PEPSI     | Pepsi Light    | PEPSI    |
| Budweiser | BUDWEISER | NULL           | NULL     |
| NULL      | NULL      | Fresca         | FRESCA   |
| NULL      | NULL      | Purfied water  | Water    |
+-----------+-----------+----------------+----------+
```



关于此内容还可以参见我之前的一篇博客[SQL Left Join, Right Join, Inner Join, and Natural Join 各种Join小结](http://www.cnblogs.com/grandyang/p/5346963.html)。



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












