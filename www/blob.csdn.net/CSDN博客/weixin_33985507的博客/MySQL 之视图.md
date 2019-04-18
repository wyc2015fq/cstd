# MySQL 之视图 - weixin_33985507的博客 - CSDN博客
2018年05月03日 05:45:16[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 视图
视图为虚拟的表，作为一种用来查看存储在别处的数据的机制，视图本身不包含数据，它们返回的数据是从其他表中检索出来的。可以这么说，视图提供了一种 MySQL 的 `SELECT` 语句层次的封装，可以用来简化数据处理以及重新格式化基础数据或保护基础数据。
使用数据库视图，有以下优点：
- 重用 SQL 语句。
- 简化复杂的 SQL 操作。创建视图之后可以方便地重用它而不必知道它的基本查询细节。
- 使用表的组成部分而不是整个表。
- 保护数据。可以给用户授权表的特定部分的访问权而不是整个表的访问权限。
- 更改数据格式和表示。视图可以返回与底层表的表示和格式不同的数据。
- 视图可以帮助我们兼容旧的系统。假设我们拥有一个数据中心，这个数据中心被很多的程序在使用。如果有一天我们决定重新设计这个数据中心以适应一些新的业务需求，可能需要删除一些旧的表，并且创建一些新的表，但是我们并不希望这些变动影响到那些老的程序。那么我们可以创建一些视图用来适配那些老的程序。
## 视图语法
#### 1. 创建视图
```
CREATE 
     [ALGORITHM = {MERGE  | TEMPTABLE | UNDEFINED}]
VIEW [database_name].[view_name] 
AS
SELECT  statement
```
MySQL 有三种视图执行策略（`ALGORITHM`），分别是：
- `MERGE` 策略：MySQL 会先将输入的查询语句和视图的声明语句进行合并，然后执行合并后的语句并返回。
- `TEMPTABLE` 策略：MySQL 先基于视图的声明创建一张 `emporary table`，当输入查询语句时会直接查询这张 `temporary table`。由于需要创建 `temporary table` 来存储视图的结果集, `TEMPTABLE` 的效率要比 `MERGE` 策略低，另外使用 `temporary table` 策略的视图是无法更新的。
- `UNDEFINED` 策略：如果创建视图的时候不指定策略，MySQL 默认使用此策略。`UNDEFINED` 策略会自动选择使用上述两种策略中的一个，优先选择 `MERGE` 策略，无法使用则转为 `TEMPTABLE` 策略。
#### 2.更新视图
```
CREATE
     [OR REPLACE] [ALGORITHM = {MERGE  | TEMPTABLE | UNDEFINED}] 
VIEW view_name[(column_list)] 
AS
SELECT statement
```
#### 3.查看视图
```
// 查看视图基本信息，如字段定义、字段数据类型、是否为空、是否为主外键、默认值和额外信息
DESCRIBE view_name;
// 查看视图的详细定义
SHOW CREATE VIEW view_name;
```
#### 4.删除视图
`DROP VIEW view_name;`
## 视图应用
#### 1. 利用视图简化复杂的联结
下面 SQL 语句创建了一个名为 `product_customers` 的视图，它联结三个表，以返回已订购了任意产品的所有客户的列表：
```
CREATE VIEW product_customers AS
SELECT cust_name, cust_contact, prod_id
FROM customers, orders, orderitems
WHERE customers.cust_id = orders.cust_id
    AND orderitems.order_num = orders.order_num;
```
现在，为了检索订购了产品 `TNT2` 的客户，可如下进行：
```
SELECT cust_name, cust_contact
FROM product_customers
WHERE prod_id = 'TNT2';
```
#### 2. 利用视图格式化检索数据
假如，我们需要得到供应商名和位置作为一个列返回数据，可以创建这样一个视图：
```
CREATE VIEW vendor_locations AS 
SELECT Concat(RTrim(vend_name), '(', RTrim(vend_country), ')') AS vend_title
FROM vendord
ORDER BY vend_name;
```
现在，为了检索出包含供应商名和位置的邮件标签的数据，可如下进行：
`SELECT * FROM vendor_locations ;`
#### 3. 利用视图过滤数据
假如现在我们需要得到一个有电子邮件地址的用户列表，可以创建这样一个视图：
```
CREATE VIEW customer_email AS
SELECT cust_is, cust_name, cust_email
FROM customers
WHERE cust_email IS NOT NULL;
```
现在，在发送电子邮件到邮件列表是，我们可以如下进行：
`SELECT * FROM customer_email;`
