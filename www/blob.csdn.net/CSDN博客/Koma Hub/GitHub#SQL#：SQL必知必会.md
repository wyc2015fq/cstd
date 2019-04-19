# GitHub#SQL#：SQL必知必会 - Koma Hub - CSDN博客
2018年05月09日 13:44:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：218
个人分类：[SQL																[GitHub](https://blog.csdn.net/Rong_Toa/article/category/7563730)](https://blog.csdn.net/Rong_Toa/article/category/7460911)
# [https://github.com/CyC2018/Interview-Notebook](https://github.com/CyC2018/Interview-Notebook)
一、基础
二、创建表
三、修改表
四、插入
五、更新
六、删除
七、查询
八、排序
九、过滤
十、通配符
十一、计算字段
十二、函数
十三、分组
十四、子查询
十五、连接
十六、组合查询
十七、视图
十八、存储过程
十九、游标
二十、触发器
二十一、事务处理
二十二、字符集
二十三、权限管理
参考资料
# 一、基础
模式定义了数据如何存储、存储什么样的数据以及数据如何分解等信息，数据库和表都有模式。
主键的值不允许修改，也不允许复用（不能使用已经删除的主键值赋给新数据行的主键）。
SQL（Structured Query Language)，标准 SQL 由 ANSI 标准委员会管理，从而称为 ANSI SQL。各个 DBMS 都有自己的实现，如 PL/SQL、Transact-SQL 等。
SQL 语句不区分大小写，但是数据库表名、列名和值是否区分依赖于具体的 DBMS 以及配置。
SQL 支持以下三种注释：
# 注释SELECT*FROM mytable; -- 注释/* 注释1   注释2 */
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%BA%8C%E5%88%9B%E5%BB%BA%E8%A1%A8)二、创建表
CREATETABLEmytable (
  id INTNOT NULL AUTO_INCREMENT,
  col1 INTNOT NULL DEFAULT 1,
  col2 VARCHAR(45) NULL,
  col3 DATENULL,
  PRIMARY KEY (`id`));
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%B8%89%E4%BF%AE%E6%94%B9%E8%A1%A8)三、修改表
添加列
ALTERTABLE mytable
ADD col CHAR(20);
删除列
ALTERTABLE mytable
DROP COLUMN col;
删除表
DROPTABLE mytable;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%9B%9B%E6%8F%92%E5%85%A5)四、插入
普通插入
INSERT INTO mytable(col1, col2)
VALUES(val1, val2);
插入检索出来的数据
INSERT INTO mytable1(col1, col2)
SELECT col1, col2
FROM mytable2;
将一个表的内容插入到一个新表
CREATETABLEnewtableASSELECT*FROM mytable;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%BA%94%E6%9B%B4%E6%96%B0)五、更新
UPDATE mytable
SET col = val
WHERE id =1;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%85%AD%E5%88%A0%E9%99%A4)六、删除
DELETEFROM mytable
WHERE id =1;
TRUNCATE TABLE 可以清空表，也就是删除所有行。
使用更新和删除操作时一定要用 WHERE 子句，不然会把整张表的数据都破坏。可以先用 SELECT 语句进行测试，防止错误删除。
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%B8%83%E6%9F%A5%E8%AF%A2)七、查询
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#distinct)DISTINCT
相同值只会出现一次。它作用于所有列，也就是说所有列的值都相同才算相同。
SELECT DISTINCT col1, col2
FROM mytable;
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#limit)LIMIT
限制返回的行数。可以有两个参数，第一个参数为起始行，从 0 开始；第二个参数为返回的总行数。
返回前 5 行：
SELECT*FROM mytable
LIMIT5;
SELECT*FROM mytable
LIMIT0, 5;
返回第 3 ~ 5 行：
SELECT*FROM mytable
LIMIT2, 3;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%85%AB%E6%8E%92%E5%BA%8F)八、排序
- ASC ：升序（默认）
- DESC ：降序
可以按多个列进行排序，并且为每个列指定不同的排序方式：
SELECT*FROM mytable
ORDER BY col1 DESC, col2 ASC;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%B9%9D%E8%BF%87%E6%BB%A4)九、过滤
不进行过滤的数据非常大，导致通过网络传输了多余的数据，从而浪费了网络带宽。因此尽量使用 SQL 语句来过滤不必要的数据，而不是传输所有的数据到客户端中然后由客户端进行过滤。
SELECT*FROM mytable
WHERE col IS NULL;
下表显示了 WHERE 子句可用的操作符
|操作符|说明|
|----|----|
|= < >|等于 小于 大于|
|<> !=|不等于|
|<= !>|小于等于|
|>= !<|大于等于|
|BETWEEN|在两个值之间|
|IS NULL|为NULL值|
应该注意到，NULL 与 0 、空字符串都不同。
AND OR 用于连接多个过滤条件，优先处理 AND，当一个过滤表达式涉及到多个 AND 和 OR 时，可以使用 () 来决定优先级，使得优先级关系更清晰。
IN 操作符用于匹配一组值，其后也可以接一个 SELECT 子句，从而匹配子查询得到的一组值。
NOT 操作符用于否定一个条件。
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E9%80%9A%E9%85%8D%E7%AC%A6)十、通配符
通配符也是用在过滤语句中，但它只能用于文本字段。
- 
% 匹配 >=0 个任意字符；
- 
_ 匹配 ==1 个任意字符；
- 
[ ] 可以匹配集合内的字符，例如 [ab] 将匹配字符 a 或者 b。用脱字符 ^ 可以对其进行否定，也就是不匹配集合内的字符。
使用 Like 来进行通配符匹配。
SELECT*FROM mytable
WHERE col LIKE'[^AB]%'-- 不以 A 和 B 开头的任意文本
不要滥用通配符，通配符位于开头处匹配会非常慢。
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E4%B8%80%E8%AE%A1%E7%AE%97%E5%AD%97%E6%AE%B5)十一、计算字段
在数据库服务器上完成数据的转换和格式化的工作往往比客户端上快得多，并且转换和格式化后的数据量更少的话可以减少网络通信量。
计算字段通常需要使用 AS 来取别名，否则输出的时候字段名为计算表达式。
SELECT col1*col2 AS alias
FROM mytable
CONCAT() 用于连接两个字段。许多数据库会使用空格把一个值填充为列宽，因此连接的结果会出现一些不必要的空格，使用 TRIM() 可以去除首尾空格。
SELECT CONCAT(TRIM(col1), ' (', TRIM(col2), ')')
FROM mytable
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E4%BA%8C%E5%87%BD%E6%95%B0)十二、函数
各个 DBMS 的函数都是不相同的，因此不可移植。
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E6%96%87%E6%9C%AC%E5%A4%84%E7%90%86)文本处理
|函数|说明|
|----|----|
|LEFT() RIGHT()|左边或者右边的字符|
|LOWER() UPPER()|转换为小写或者大写|
|LTRIM() RTIM()|去除左边或者右边的空格|
|LENGTH()|长度|
|SOUNDEX()|转换为语音值|
其中， SOUNDEX() 可以将一个字符串转换为描述其语音表示的字母数字模式。
SELECT*FROM mytable
WHERE SOUNDEX(col1) = SOUNDEX('apple')
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E6%97%A5%E6%9C%9F%E5%92%8C%E6%97%B6%E9%97%B4%E5%A4%84%E7%90%86)日期和时间处理
- 日期格式：YYYY-MM-DD
- 时间格式：HH:MM:SS
|函 数|说 明|
|----|----|
|AddDate()|增加一个日期（天、周等）|
|AddTime()|增加一个时间（时、分等）|
|CurDate()|返回当前日期|
|CurTime()|返回当前时间|
|Date()|返回日期时间的日期部分|
|DateDiff()|计算两个日期之差|
|Date_Add()|高度灵活的日期运算函数|
|Date_Format()|返回一个格式化的日期或时间串|
|Day()|返回一个日期的天数部分|
|DayOfWeek()|对于一个日期，返回对应的星期几|
|Hour()|返回一个时间的小时部分|
|Minute()|返回一个时间的分钟部分|
|Month()|返回一个日期的月份部分|
|Now()|返回当前日期和时间|
|Second()|返回一个时间的秒部分|
|Time()|返回一个日期时间的时间部分|
|Year()|返回一个日期的年份部分|
mysql>SELECT NOW();
```
2018-4-14 20:25:11
```
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E6%95%B0%E5%80%BC%E5%A4%84%E7%90%86)数值处理
|函数|说明|
|----|----|
|SIN()|正弦|
|COS()|余弦|
|TAN()|正切|
|ABS()|绝对值|
|SQRT()|平方根|
|MOD()|余数|
|EXP()|指数|
|PI()|圆周率|
|RAND()|随机数|
[](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E6%B1%87%E6%80%BB)汇总|函 数|说 明|
|----|----|
|AVG()|返回某列的平均值|
|COUNT()|返回某列的行数|
|MAX()|返回某列的最大值|
|MIN()|返回某列的最小值|
|SUM()|返回某列值之和|
AVG() 会忽略 NULL 行。
使用 DISTINCT 可以让汇总函数值汇总不同的值。
SELECTAVG(DISTINCT col1) AS avg_col
FROM mytable
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E4%B8%89%E5%88%86%E7%BB%84)十三、分组
分组就是把具有相同的数据值的行放在同一组中。
可以对同一分组数据使用汇总函数进行处理，例如求分组数据的平均值等。
指定的分组字段除了能按该字段进行分组，也会自动按按该字段进行排序。
SELECT col, COUNT(*) AS num
FROM mytable
GROUP BY col;
GROUP BY 按分组字段进行排序，ORDER BY 也可以以汇总字段来进行排序。
SELECT col, COUNT(*) AS num
FROM mytable
GROUP BY col
ORDER BY num;
WHERE 过滤行，HAVING 过滤分组，行过滤应当先于分组过滤。
SELECT col, COUNT(*) AS num
FROM mytable
WHERE col >2GROUP BY col
HAVING num >=2;
分组规定：
- GROUP BY 子句出现在 WHERE 子句之后，ORDER BY 子句之前；
- 除了汇总字段外，SELECT 语句中的每一字段都必须在 GROUP BY 子句中给出；
- NULL 的行会单独分为一组；
- 大多数 SQL 实现不支持 GROUP BY 列具有可变长度的数据类型。
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E5%9B%9B%E5%AD%90%E6%9F%A5%E8%AF%A2)十四、子查询
子查询中只能返回一个字段的数据。
可以将子查询的结果作为 WHRER 语句的过滤条件：
SELECT*FROM mytable1
WHERE col1 IN (SELECT col2
               FROM mytable2);
下面的语句可以检索出客户的订单数量，子查询语句会对第一个查询检索出的每个客户执行一次：
SELECT cust_name, (SELECTCOUNT(*)
                   FROM Orders
                   WHEREOrders.cust_id=Customers.cust_id)
                   AS orders_num
FROM Customers
ORDER BY cust_name;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E4%BA%94%E8%BF%9E%E6%8E%A5)十五、连接
连接用于连接多个表，使用 JOIN 关键字，并且条件语句使用 ON 而不是 WHERE。
连接可以替换子查询，并且比子查询的效率一般会更快。
可以用 AS 给列名、计算字段和表名取别名，给表名取别名是为了简化 SQL 语句以及连接相同表。
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%86%85%E8%BF%9E%E6%8E%A5)内连接
内连接又称等值连接，使用 INNER JOIN 关键字。
select a, b, c
from A inner join B
onA.key=B.key
可以不明确使用 INNER JOIN，而使用普通查询并在 WHERE 中将两个表中要连接的列用等值方法连接起来。
select a, b, c
from A, B
whereA.key=B.key
在没有条件语句的情况下返回笛卡尔积。
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E8%87%AA%E8%BF%9E%E6%8E%A5)自连接
自连接可以看成内连接的一种，只是连接的表是自身而已。
一张员工表，包含员工姓名和员工所属部门，要找出与 Jim 处在同一部门的所有员工姓名。
子查询版本
select name
from employee
where department = (
      select department
      from employee
      where name ="Jim");
自连接版本
selecte1.namefrom employee as e1, employee as e2
wheree1.department=e2.departmentande2.name="Jim";
连接一般比子查询的效率高。
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E8%87%AA%E7%84%B6%E8%BF%9E%E6%8E%A5)自然连接
自然连接是把同名列通过等值测试连接起来的，同名列可以有多个。
内连接和自然连接的区别：内连接提供连接的列，而自然连接自动连接所有同名列。
select*from employee natural join department;
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%A4%96%E8%BF%9E%E6%8E%A5)外连接
外连接保留了没有关联的那些行。分为左外连接，右外连接以及全外连接，左外连接就是保留左表没有关联的行。
检索所有顾客的订单信息，包括还没有订单信息的顾客。
selectCustomers.cust_id, Orders.order_numfrom Customers left outer join Orders
onCustomers.cust_id=Orders.cust_id;
如果需要统计顾客的订单数，使用聚集函数。
selectCustomers.cust_id,
       COUNT(Orders.order_num) as num_ord
from Customers left outer join Orders
onCustomers.cust_id=Orders.cust_idgroup byCustomers.cust_id;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E5%85%AD%E7%BB%84%E5%90%88%E6%9F%A5%E8%AF%A2)十六、组合查询
使用 UNION 来组合两个查询，如果第一个查询返回 M 行，第二个查询返回 N 行，那么组合查询的结果一般为 M+N 行。
每个查询必须包含相同的列、表达式和聚集函数。
默认会去除相同行，如果需要保留相同行，使用 UNION ALL。
只能包含一个 ORDER BY 子句，并且必须位于语句的最后。
SELECT col
FROM mytable
WHERE col =1UNIONSELECT col
FROM mytable
WHERE col =2;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E4%B8%83%E8%A7%86%E5%9B%BE)十七、视图
视图是虚拟的表，本身不包含数据，也就不能对其进行索引操作。对视图的操作和对普通表的操作一样。
视图具有如下好处：
- 简化复杂的 SQL 操作，比如复杂的联结；
- 只使用实际表的一部分数据；
- 通过只给用户访问视图的权限，保证数据的安全性；
- 更改数据格式和表示。
CREATEVIEWmyviewASSELECT Concat(col1, col2) AS concat_col, col3*col4 AS count_col
FROM mytable
WHERE col5 = val;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E5%85%AB%E5%AD%98%E5%82%A8%E8%BF%87%E7%A8%8B)十八、存储过程
存储过程可以看成是对一系列 SQL 操作的批处理；
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%BD%BF%E7%94%A8%E5%AD%98%E5%82%A8%E8%BF%87%E7%A8%8B%E7%9A%84%E5%A5%BD%E5%A4%84)使用存储过程的好处
- 代码封装，保证了一定的安全性；
- 代码复用；
- 由于是预先编译，因此具有很高的性能。
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%88%9B%E5%BB%BA%E5%AD%98%E5%82%A8%E8%BF%87%E7%A8%8B)创建存储过程
命令行中创建存储过程需要自定义分隔符，因为命令行是以 ; 为结束符，而存储过程中也包含了分号，因此会错误把这部分分号当成是结束符，造成语法错误。
包含 in、out 和 inout 三种参数。
给变量赋值都需要用 select into 语句。
每次只能给一个变量赋值，不支持集合的操作。
delimiter //
create procedure myprocedure( out ret int )
    begin
        declare y int;
        selectsum(col1)
        from mytable
        into y;
        select y*y into ret;
    end //
delimiter ;
call myprocedure(@ret);
select @ret;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8D%81%E4%B9%9D%E6%B8%B8%E6%A0%87)十九、游标
在存储过程中使用游标可以对一个结果集进行移动遍历。
游标主要用于交互式应用，其中用户需要对数据集中的任意行进行浏览和修改。
使用游标的四个步骤：
- 声明游标，这个过程没有实际检索出数据；
- 打开游标；
- 取出数据；
- 关闭游标；
delimiter //
create procedure myprocedure(out ret int)
    begin
        declare done boolean default 0;
        declare mycursor cursor for
        select col1 from mytable;
        # 定义了一个continue handler，当 sqlstate '02000' 这个条件出现时，会执行 set done = 1
        declare continue handler for sqlstate '02000'set done =1;
        open mycursor;
        repeat
            fetch mycursor into ret;
            select ret;
        until done end repeat;
        close mycursor;
    end //
 delimiter ;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%BA%8C%E5%8D%81%E8%A7%A6%E5%8F%91%E5%99%A8)二十、触发器
触发器会在某个表执行以下语句时而自动执行：DELETE、INSERT、UPDATE
触发器必须指定在语句执行之前还是之后自动执行，之前执行使用 BEFORE 关键字，之后执行使用 AFTER 关键字。BEFORE 用于数据验证和净化。
INSERT 触发器包含一个名为 NEW 的虚拟表。
CREATETRIGGERmytrigger AFTER INSERT ON mytable
FOR EACH ROW SELECTNEW.col into @result;
-- 获取结果SELECT @result;
DELETE 触发器包含一个名为 OLD 的虚拟表，并且是只读的。
UPDATE 触发器包含一个名为 NEW 和一个名为 OLD 的虚拟表，其中 NEW 是可以被修改地，而 OLD 是只读的。
可以使用触发器来进行审计跟踪，把修改记录到另外一张表中。
MySQL 不允许在触发器中使用 CALL 语句 ，也就是不能调用存储过程。
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%BA%8C%E5%8D%81%E4%B8%80%E4%BA%8B%E5%8A%A1%E5%A4%84%E7%90%86)二十一、事务处理
基本术语：
- 事务（transaction）指一组 SQL 语句；
- 回退（rollback）指撤销指定 SQL 语句的过程；
- 提交（commit）指将未存储的 SQL 语句结果写入数据库表；
- 保留点（savepoint）指事务处理中设置的临时占位符（placeholder），你可以对它发布回退（与回退整个事务处理不同）。
不能回退 SELECT 语句，回退 SELECT 语句也没意义；也不能回退 CREATE 和 DROP 语句。
MySQL 的事务提交默认是隐式提交，每执行一条语句就把这条语句当成一个事务然后进行提交。当出现 START TRANSACTION 语句时，会关闭隐式提交；当 COMMIT 或 ROLLBACK 语句执行后，事务会自动关闭，重新恢复隐式提交。
通过设置 autocommit 为 0 可以取消自动提交，直到 autocommit 被设置为 1 才会提交；autocommit 标记是针对每个连接而不是针对服务器的。
如果没有设置保留点，ROLLBACK 会回退到 START TRANSACTION 语句处；如果设置了保留点，并且在 ROLLBACK 中指定该保留点，则会回退到该保留点。
START TRANSACTION// ...
SAVEPOINT delete1
// ...
ROLLBACK TO delete1
// ...
COMMIT
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%BA%8C%E5%8D%81%E4%BA%8C%E5%AD%97%E7%AC%A6%E9%9B%86)二十二、字符集
基本术语：
- 字符集为字母和符号的集合；
- 编码为某个字符集成员的内部表示；
- 校对字符指定如何比较，主要用于排序和分组。
除了给表指定字符集和校对外，也可以给列指定：
CREATETABLEmytable
(col VARCHAR(10) CHARACTER SET latin COLLATE latin1_general_ci )
DEFAULT CHARACTER SET hebrew COLLATE hebrew_general_ci;
可以在排序、分组时指定校对：
SELECT*FROM mytable
ORDER BY col COLLATE latin1_general_ci;
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%BA%8C%E5%8D%81%E4%B8%89%E6%9D%83%E9%99%90%E7%AE%A1%E7%90%86)二十三、权限管理
MySQL 的账户信息保存在 mysql 这个数据库中。
USE mysql;
SELECT user FROM user;
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%88%9B%E5%BB%BA%E8%B4%A6%E6%88%B7)创建账户
CREATEUSERmyuser IDENTIFIED BY 'mypassword';
新创建的账户没有任何权限。
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E4%BF%AE%E6%94%B9%E8%B4%A6%E6%88%B7%E5%90%8D)修改账户名
RENAME myuser TO newuser;
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%88%A0%E9%99%A4%E8%B4%A6%E6%88%B7)删除账户
DROPUSER myuser;
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E6%9F%A5%E7%9C%8B%E6%9D%83%E9%99%90)查看权限
SHOW GRANTS FOR myuser;
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E6%8E%88%E4%BA%88%E6%9D%83%E9%99%90)授予权限
GRANTSELECT, INSERT ON mydatabase.* TO myuser;
账户用 username@host 的形式定义，username@% 使用的是默认主机名。
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%88%A0%E9%99%A4%E6%9D%83%E9%99%90)删除权限
REVOKESELECT, INSERT ON mydatabase.*FROM myuser;
GRANT 和 REVOKE 可在几个层次上控制访问权限：
- 整个服务器，使用 GRANT ALL 和 REVOKE ALL；
- 整个数据库，使用 ON database.*；
- 特定的表，使用 ON database.table；
- 特定的列；
- 特定的存储过程。
## [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E6%9B%B4%E6%94%B9%E5%AF%86%E7%A0%81)更改密码
必须使用 Password() 函数
SET PASSWROD FOR myuser = Password('newpassword');
# [](https://github.com/CyC2018/Interview-Notebook/blob/master/notes/SQL.md#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99)参考资料
- BenForta. SQL 必知必会 [M]. 人民邮电出版社, 2013.
