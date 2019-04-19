# 用PostgreSQL运行文件中的SQL程序 - Koma Hub - CSDN博客
2018年06月10日 21:52:41[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1140

## 用PostgreSQL运行文件中的SQL程序
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-03-TABLES.md#%E9%A6%96%E5%85%88%E6%96%87%E4%BB%B6%E5%86%85%E5%AE%B9%E5%A6%82%E4%B8%8B)首先文件内容如下：
```
$ ls
barcode.sql                 drop_tables.sql  orderline.sql
create_tables-bpsimple.sql  item.sql         PostgreSQL.md
customer.sql                orderinfo.sql    stock.sql
```
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-03-TABLES.md#%E7%84%B6%E5%90%8E%E5%88%9B%E5%BB%BA%E6%95%B0%E6%8D%AE%E5%BA%93bpsimple)然后创建数据库`bpsimple`
```
$ su
密码：
# su - postgres
$ createdb bpsimple
Password:
```
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-03-TABLES.md#%E7%84%B6%E5%90%8E%E5%8F%AF%E4%BB%A5%E4%B8%A4%E6%AC%A1%E9%80%80%E5%87%BAexit%E8%BF%94%E5%9B%9E%E5%8E%9F%E6%9D%A5%E7%9A%84%E7%94%A8%E6%88%B7%E8%BF%9B%E8%A1%8C%E6%93%8D%E4%BD%9C%E6%AF%94%E8%BE%83%E5%AE%89%E5%85%A8)然后可以两次退出`exit`，返回原来的用户进行操作，比较安全。
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-03-TABLES.md#%E9%A6%96%E5%85%88%E5%9C%A8%E6%95%B0%E6%8D%AE%E5%BA%93bpsimple%E5%88%9B%E5%BB%BA%E4%B8%AD%E5%88%9B%E5%BB%BA%E5%90%84%E7%A7%8D%E8%A1%A8)首先在数据库`bpsimple`创建中创建各种表：
```
$ psql -U postgres -d bpsimple -f create_tables-bpsimple.sql 
Password for user postgres: 
CREATE TABLE
CREATE TABLE
CREATE TABLE
CREATE TABLE
CREATE TABLE
CREATE TABLE
```
- 其中`create_tables-bpsimple.sql`中的内容如下：
CREATETABLEcustomer ( 
    customer_id serial , 
    title char(4) ,
    fname varchar(32) , 
    lname varchar(32) NOT NULL, 
    addressline varchar(64) , 
    town varchar(32) , 
    zipcode char(10) NOT NULL, 
    phone varchar(16) , 
    CONSTRAINT customer_pk PRIMARY KEY(customer_id) 
);
CREATETABLEitem ( 
    item_id serial , 
    description varchar(64) NOT NULL, 
    cost_price numeric(7,2) , 
    sell_price numeric(7,2) , 
    CONSTRAINT item_pk PRIMARY KEY(item_id) 
);
CREATETABLEorderinfo ( 
    orderinfo_id serial , 
    customer_id integerNOT NULL, 
    date_placed dateNOT NULL, 
    date_shipped date , 
    shipping numeric(7,2) , 
    CONSTRAINT orderinfo_pk PRIMARY KEY(orderinfo_id) 
); 
CREATETABLEstock ( 
    item_id integerNOT NULL, 
    quantity integerNOT NULL, 
    CONSTRAINT stock_pk PRIMARY KEY(item_id) 
);
CREATETABLEorderline ( 
    orderinfo_id integerNOT NULL,
    item_id integerNOT NULL, 
    quantity integerNOT NULL, 
    CONSTRAINT orderline_pk PRIMARY KEY(orderinfo_id, item_id) 
); 
CREATETABLEbarcode ( 
    barcode_ean char(13) NOT NULL, 
    item_id integerNOT NULL, 
    CONSTRAINT barcode_pk PRIMARY KEY(barcode_ean) 
);
- 其他文件也按照此方法即可创建这个数据库。下面见结果（此处注意语句末尾的“;”）：
```
$ psql -U postgres -d bpsimple
Password for user postgres: 
psql.bin (10.4)
Type "help" for help.
bpsimple=# table item
bpsimple-# ;
 item_id |  description  | cost_price | sell_price 
---------+---------------+------------+------------
       1 | Wood Puzzle   |      15.23 |      21.95
       2 | Rubik Cube    |       7.45 |      11.49
       3 | Linux CD      |       1.99 |       2.49
       4 | Tissues       |       2.11 |       3.99
       5 | Picture Frame |       7.54 |       9.95
       6 | Fan Small     |       9.23 |      15.75
       7 | Fan Large     |      13.36 |      19.95
       8 | Toothbrush    |       0.75 |       1.45
       9 | Roman Coin    |       2.34 |       2.45
      10 | Carrier Bag   |       0.01 |       0.00
      11 | Speakers      |      19.73 |      25.32
(11 rows)
bpsimple=# \dt
           List of relations
 Schema |   Name    | Type  |  Owner   
--------+-----------+-------+----------
 public | barcode   | table | postgres
 public | customer  | table | postgres
 public | item      | table | postgres
 public | orderinfo | table | postgres
 public | orderline | table | postgres
 public | stock     | table | postgres
(6 rows)
bpsimple=# table customer;
 customer_id | title |   fname   |  lname  |   addressline    |   town    |  zip
code   |  phone   
-------------+-------+-----------+---------+------------------+-----------+-----
-------+----------
           1 | Miss  | Jenny     | Stones  | 27 Rowan Avenue  | Hightown  | NT2 
1AQ    | 023 9876
           2 | Mr    | Andrew    | Stones  | 52 The Willows   | Lowtown   | LT5 
7RA    | 876 3527
           3 | Miss  | Alex      | Matthew | 4 The Street     | Nicetown  | NT2 
2TX    | 010 4567
           4 | Mr    | Adrian    | Matthew | The Barn         | Yuleville | YV67
 2WR   | 487 3871
           5 | Mr    | Simon     | Cozens  | 7 Shady Lane     | Oakenham  | OA3 
6QW    | 514 5926
           6 | Mr    | Neil      | Matthew | 5 Pasture Lane   | Nicetown  | NT3 
7RT    | 267 1232
           7 | Mr    | Richard   | Stones  | 34 Holly Way     | Bingham   | BG4 
2WE    | 342 5982
           8 | Mrs   | Ann       | Stones  | 34 Holly Way     | Bingham   | BG4 
2WE    | 342 5982
           9 | Mrs   | Christine | Hickman | 36 Queen Street  | Histon    | HT3 
5EM    | 342 5432
          10 | Mr    | Mike      | Howard  | 86 Dysart Street | Tibsville | TB3 
7FG    | 505 5482
          11 | Mr    | Dave      | Jones   | 54 Vale Rise     | Bingham   | BG3 
8GD    | 342 8264
          12 | Mr    | Richard   | Neill   | 42 Thatched Way  | Winersby  | WB3 
6GQ    | 505 6482
          13 | Mrs   | Laura     | Hardy   | 73 Margarita Way | Oxbridge  | OX2 
3HX    | 821 2335
          14 | Mr    | Bill      | Neill   | 2 Beamer Street  | Welltown  | WT3 
8GM    | 435 1234
          15 | Mr    | David     | Hudson  | 4 The Square     | Milltown  | MT2 
6RT    | 961 4526
(15 rows)
```
完整的SQL代码：[https://blog.csdn.net/Rong_Toa/article/details/80644443](https://blog.csdn.net/Rong_Toa/article/details/80644443)
