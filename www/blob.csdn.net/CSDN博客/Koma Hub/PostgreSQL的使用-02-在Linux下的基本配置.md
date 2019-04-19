# PostgreSQL的使用-02-在Linux下的基本配置 - Koma Hub - CSDN博客
2018年06月10日 20:18:34[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：53
## 初始化数据库
$ su 
# su - postgres 
postgres$
postgres$ /usr/local/pgsql/bin/initdb -D /usr/local/pgsql/data
如果一切正常，你将在`initdb`命令的`-D`参数指向的位置拥有一个全新的空白数据库。
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E9%85%8D%E7%BD%AE%E8%BF%9E%E6%8E%A5%E6%9D%83%E9%99%90)配置连接权限
默认情况下，`PostgreSQL`不允许全面的远程访问。为了赋权给远程连接，你必须编辑配置文件`pg_hba.conf`（位于`/usr/local/pgsql/`）. 例如，希望允许用户neil通过IP地址为192.168.0.3的主机连接到bpsimple数据库，添加以下行到`pg_hba.conf`文件：
    host bpsimple neil 192.168.0.3/32 md5
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E5%88%9B%E5%BB%BA%E7%94%A8%E6%88%B7)创建用户
$ createuser neil
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E5%88%9B%E5%BB%BA%E6%95%B0%E6%8D%AE%E5%BA%93)创建数据库
$ createdb bpsimple
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E9%93%BE%E6%8E%A5%E6%95%B0%E6%8D%AE%E5%BA%93)链接数据库
$ psql -U neil -d bpsimple
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E5%BB%BA%E8%A1%A8)建表
bpsimple=# \i create_tables-bpsimple.sql
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E6%9F%A5%E7%9C%8B%E6%95%B0%E6%8D%AE%E5%BA%93)查看数据库
```
\l  --相当于MySQL的： show datasets
```
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E6%9F%A5%E7%9C%8B%E5%A4%9A%E8%A1%A8)查看多表
```
\dt --相当于mysql的，mysql> show tables
```
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E5%88%9B%E5%BB%BA%E4%B8%80%E4%B8%AA%E8%A1%A8)创建一个表
CREATETABLEcustomer ( 
    customer_id serial, 
    title char(4), 
    fname varchar(32), 
    lname varchar(32) not null, 
    addressline varchar(64), 
    town varchar(32), 
    zipcode char(10) not null, 
    phone varchar(16), 
);
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E5%90%91%E8%A1%A8%E4%B8%AD%E6%8F%92%E5%85%A5%E4%B8%80%E8%A1%8C%E6%95%B0%E6%8D%AE)向表中插入一行数据
INSERT INTO customer(title, fname, lname, addressline, town, zipcode, phone) 
VALUES('Mr','Neil','Matthew','5 Pasture Lane','Nicetown','NT3 7RT','267 1232'); 
INSERT INTO customer(title, fname, lname, addressline, town, zipcode, phone) 
VALUES('Mr','Richard','Stones','34 Holly Way','Bingham','BG4 2WE','342 5982');
## [](https://github.com/rtoax/SQL/blob/master/PostgreSQL-01.md#%E5%85%B3%E8%81%94%E4%B8%80%E4%B8%AA%E8%A1%A8)关联一个表
SELECT*FROM customer, orderinfo WHEREcustomer.customer_id=orderinfo.customer_id;
