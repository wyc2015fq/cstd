# Mysql学习笔记（一）：子查询与选择 - Machine Learning with Peppa - CSDN博客





2017年12月28日 12:07:33[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：230








下列代码是我在学习MOOC的视频时做的一些练习，内容比较简单，主要涉及子查询与连接，大家可以作为练习参考直接使用(更多相关代码可以访问 [我的GITHUB](https://github.com/hatwanghacker/DataBase-Mysql)

代码如下：

```
Create database MOOC;

Use MOOC;
SET NAMES utf8;

  CREATE TABLE IF NOT EXISTS tdb_goods(
    goods_id    SMALLINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    goods_name  VARCHAR(150) NOT NULL,
    goods_cate  VARCHAR(40)  NOT NULL,
    brand_name  VARCHAR(40)  NOT NULL,
    goods_price DECIMAL(15,3) UNSIGNED NOT NULL DEFAULT 0,
    is_show     BOOLEAN NOT NULL DEFAULT 1,
    is_saleoff  BOOLEAN NOT NULL DEFAULT 0
  );
```

![表创建以后如下图](https://img-blog.csdn.net/20171228192709253?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
ALTER TABLES tdb_goods CHARACTER SET utf8;

Delete from tdb_goods; //测试安全模式

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('R510VC 15.6英寸笔记本','笔记本','华硕','3399',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('Y400N 14.0英寸笔记本电脑','笔记本','联想','4899',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('G150TH 15.6英寸游戏本','游戏本','雷神','8499',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('X550CC 15.6英寸笔记本','笔记本','华硕','2799',DEFAULT,DEFAULT);
 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('X240(20ALA0EYCD) 12.5英寸超极本','超级本','联想','4999',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('U330P 13.3英寸超极本','超级本','联想','4299',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('SVP13226SCB 13.3英寸触控超极本','超级本','索尼','7999',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('iPad mini MD531CH/A 7.9英寸平板电脑','平板电脑','苹果','1998',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('iPad Air MD788CH/A 9.7英寸平板电脑 （16G WiFi版）','平板电脑','苹果','3388',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES(' iPad mini ME279CH/A 配备 Retina 显示屏 7.9英寸平板电脑 （16G WiFi版）','平板电脑','苹果','2788',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('IdeaCentre C340 20英寸一体电脑 ','台式机','联想','3499',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('Vostro 3800-R1206 台式电脑','台式机','戴尔','2899',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('iMac ME086CH/A 21.5英寸一体电脑','台式机','苹果','9188',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('AT7-7414LP 台式电脑 （i5-3450四核 4G 500G 2G独显 DVD 键鼠 Linux ）','台式机','宏碁','3699',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('Z220SFF F4F06PA工作站','服务器/工作站','惠普','4288',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('PowerEdge T110 II服务器','服务器/工作站','戴尔','5388',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('Mac Pro MD878CH/A 专业级台式电脑','服务器/工作站','苹果','28888',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES(' HMZ-T3W 头戴显示设备','笔记本配件','索尼','6999',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('商务双肩背包','笔记本配件','索尼','99',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('X3250 M4机架式服务器 2583i14','服务器/工作站','IBM','6888',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('玄龙精英版 笔记本散热器','笔记本配件','九州风神',default,DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES(' HMZ-T3W 头戴显示设备','笔记本配件','索尼','6999',DEFAULT,DEFAULT);

 INSERT tdb_goods (goods_name,goods_cate,brand_name,goods_price,is_show,is_saleoff) VALUES('商务双肩背包','笔记本配件','索尼','99',DEFAULT,DEFAULT);

 Select count(goods_id) as How_many from tdb_goods;
 Select * from tdb_goods\G

 /**************
 **子查询练习**
 **************/

//查询价格大于平均价格的产品
select ROUND(AVG(goods_price),1) as Avgerage_price from tdb_goods;
写法一
select goods_id, goods_name, goods_price from tdb_goods 
    group by goods_price, goods_id /*GROUP BY可以放置任意字段作为函数依据*/
    having
    goods_price >= 
    ROUND(AVG(goods_price),1);
```

![查询结果](https://img-blog.csdn.net/20171228193551031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
写法二
select goods_id, goods_name, goods_price from tdb_goods 
    where goods_price >=  (select
    ROUND(AVG(goods_price),1) from tdb_goods); 


//查询价格大于超极本的笔记本
select goods_id, goods_name, goods_price from tdb_goods 
    where goods_price >=  any (select
    goods_price from tdb_goods where goods_cate = '超级本'); 
select * from mysql.user;
show variables like 'autocommit';

use mooc;
select * from tdb_goods;

**STEP 1**
//创建新的数据表作为外键INSERT 子查询插入
//用于第二步的模式分解（多表更新）

Create table if not exists tb_cates(
cate_id smallint unsigned not null auto_increment,
cate_name varchar(20) not null,
primary key(cate_id)
);

Select goods_cate From tdb_goods 
    group by goods_cate;
DESC tdb_goods; 
/*根据每一列的数据类型插入，即目标列和查询列必须一致*/



**STEP 2**
Insert INTO tb_cates(cate_name)   /*和select嵌套查询不同，insert不需要=(select)*/
    Select goods_cate From tdb_goods 
        group by goods_cate; 

Select * from tb_cates group by cate_id ASC;

//通过TB-cates中对应的序号将原表类别使用ID替代，即连接

Update tdb_goods 
    INNER join tb_cates 
    ON tdb_goods.goods_cate = tb_cates.cate_name
    SET tdb_goods.goods_cate = tb_cates.cate_id
    where tdb_goods.goods_id >= 1; /*安全模式下必须要WHERE+KEY进行UPDATE*/

**STEP 3**
//将STEP 1 & 2合并为一句

SELECT brand_name, count(brand_name)
    from tdb_goods GROUP BY goods_cate ASC;

Create Table goods_brands(
brand_id smallint unsigned auto_increment,
brand_name VARCHAR(40) default null,
primary key(brand_id)
)
Select brand_id from tdb_goods group by 
    brand_id;

SELECT * FROM goods_brands;

UPDATE tdb_goods as A INNER JOIN goods_brands AS b
    ON      /*使用join形成的外键称为“事实外键”，不需要FK声明*/
    A.brand_name = B.brand_name
    SET A.brand_name = B.brand_id;

//测试：修改原表数据类型

Alter table tdb_goods
Change goods_cate cate_id smallint unsigned not null,
Change brand_name brand_id smallint unsigned not null
;


ALTER TABLE tb_cates rename to goods_cate;
```



