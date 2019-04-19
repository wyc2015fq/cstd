# PostgreSQL与MySQL的日期类型DATE/date的格式区别 - Koma Hub - CSDN博客
2019年03月11日 22:27:13[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：38
**PostgreSQL**：MM-DD-YYYY
**MySQL**：YYYY-MM-DD
例如：
在PostgreSQl中这是正确的：
```
CREATE TABLE orderinfo ( 
    orderinfo_id serial , 
    customer_id integer NOT NULL, 
    date_placed date NOT NULL, 
    date_shipped date , 
    shipping numeric(7,2) , 
    CONSTRAINT orderinfo_pk PRIMARY KEY(orderinfo_id) 
); 
INSERT INTO orderinfo(customer_id, date_placed, date_shipped, shipping) 
VALUES(8,'07-21-2019','07-24-2019', 0.00);
```
但是这在MySQL中报错：
```
mysql> INSERT INTO orderinfo(customer_id, date_placed, date_shipped, shipping) 
    -> VALUES(8,'06-23-2000','06242000', 0.00); 
ERROR 1292 (22007): Incorrect date value: '06-23-2000' for column 'date_placed' at row 1
```
MySQL的格式为：
```
INSERT INTO orderinfo(customer_id, date_placed, date_shipped, shipping) 
VALUES(3,'2019-3-13','2019-3-17', 2.99); 
INSERT INTO orderinfo(customer_id, date_placed, date_shipped, shipping) 
VALUES(8,'2019-6-23','2019-6-24', 0.00); 
INSERT INTO orderinfo(customer_id, date_placed, date_shipped, shipping) 
VALUES(15,'2019-9-02','2019-9-12', 3.99); 
INSERT INTO orderinfo(customer_id, date_placed, date_shipped, shipping) 
VALUES(13,'2019-9-03','2019-9-10', 2.99); 
INSERT INTO orderinfo(customer_id, date_placed, date_shipped, shipping) 
VALUES(8,'2019-7-21','2019-7-24', 0.00);
```
见消费者数据库：[https://blog.csdn.net/Rong_Toa/article/details/80644443](https://blog.csdn.net/Rong_Toa/article/details/80644443)
> 
SQL Date 数据类型
MySQL 使用下列数据类型在数据库中存储日期或日期/时间值：
DATE - 格式 YYYY-MM-DD
DATETIME - 格式: YYYY-MM-DD HH:MM:SS
TIMESTAMP - 格式: YYYY-MM-DD HH:MM:SS
YEAR - 格式 YYYY 或 YY
SQL Server 使用下列数据类型在数据库中存储日期或日期/时间值：
DATE - 格式 YYYY-MM-DD
DATETIME - 格式: YYYY-MM-DD HH:MM:SS
SMALLDATETIME - 格式: YYYY-MM-DD HH:MM:SS
TIMESTAMP - 格式: 唯一的数字
--------------------- 
作者：上海-阿拉Chen 
来源：CSDN 
原文：https://blog.csdn.net/weixin_39142498/article/details/78890210 
版权声明：本文为博主原创文章，转载请附上博文链接！
