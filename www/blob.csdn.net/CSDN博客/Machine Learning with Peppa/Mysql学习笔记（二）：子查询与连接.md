# Mysql学习笔记（二）：子查询与连接 - Machine Learning with Peppa - CSDN博客





2017年12月28日 12:15:03[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：252








下列代码是我在学习MOOC的视频时做的一些练习，内容比较简单，本节主要涉及内外连接以及子分类（自连接），大家可以作为练习参考直接使用

代码如下：

```
//Join的使用

USe mooc;
    Select * from tdb_goods group by goods_id ASC;
```

![这里写图片描述](https://img-blog.csdn.net/20171229172830845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
ALTER TABLE tb_cates rename to goods_cate;
   INSERT goods_cate(cate_name) VALUES('路由器'),('交换机'),('网卡');

   INSERT goods_brands(brand_name) VALUES('海尔'),('清华同方'),('神舟');
-- 在tdb_goods数据表写入任意记录

//通过这三列数据可以发现，当ID不同时存在（插入）两张表时，内连接无法查询到记录
   INSERT tdb_goods(goods_name,cate_id,brand_id,goods_price) VALUES(' LaserJet Pro P1606dn 黑白激光打印机','12','4','1849');
   INSERT goods_brands(brand_name) VALUES('LaserJet Pro P1606dn 黑白激光打印机');
   INSERT goods_cate(cate_name) VALUES('黑白激光打印机');


Show columns from tdb_goods;
Show tables from mooc;
```

![TDB_GOODS](https://img-blog.csdn.net/20171231182221170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
describe goods_cate;
describe goods_brands;

//确保上面两张表的各字段数据类型是否一致

**内连接**

Select goods_id, goods_name, cate_name From
    tdb_goods INNER JOIN goods_cate On
    tdb_goods.cate_id = goods_cate.cate_id;
/*新增的三种产品无法显示在内连接中*/

Select * from goods_brands;

**外连接**

Select goods_id, goods_name, brand_name From
    tdb_goods LEFT OUTER JOIN goods_brands On
    tdb_goods.cate_id = goods_brands.brand_id;

    truncate goods_brands; /*清楚表内数据之后，外连接的参照列全部为NULL*/

**多表连接**

Select goods_id, goods_name, a.cate_id, a.brand_id, goods_price, is_show, is_saleoff
    from tdb_goods AS a INNER JOIN
        goods_brands as b 

-- 无限分类的数据表设计
   CREATE TABLE tdb_goods_types(
     type_id   SMALLINT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
     type_name VARCHAR(20) NOT NULL,
     parent_id SMALLINT UNSIGNED NOT NULL DEFAULT 0
  ); 

---------插入测试数据--------
  INSERT tdb_goods_types(type_name,parent_id) VALUES('家用电器',DEFAULT);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('电脑、办公',DEFAULT);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('大家电',1);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('生活电器',1);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('平板电视',3);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('空调',3);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('电风扇',4);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('饮水机',4);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('电脑整机',2);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('电脑配件',2);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('笔记本',9);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('超级本',9);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('游戏本',9);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('CPU',10);
  INSERT tdb_goods_types(type_name,parent_id) VALUES('主机',10);


  -- 查找所有分类及其父类

  SELECT s.type_id,s.type_name,p.type_name FROM tdb_goods_types AS s LEFT JOIN tdb_goods_types AS  p ON s.parent_id = p.type_id;

-- 查找所有分类及其子类

  SELECT p.type_id,p.type_name,s.type_name FROM tdb_goods_types AS p LEFT JOIN tdb_goods_types AS  s ON s.parent_id = p.type_id;

-- 查找所有分类及其子类的数目

  SELECT p.type_id,p.type_name,count(s.type_name) AS children_count FROM tdb_goods_types AS p LEFT JOIN tdb_goods_types AS s ON s.parent_id = p.type_id GROUP BY p.type_name ORDER BY p.type_id;



-- 为tdb_goods_types添加child_count字段

  ALTER TABLE tdb_goods_types ADD child_count MEDIUMINT UNSIGNED NOT NULL DEFAULT 0;



-- 将刚才查询到的子类数量更新到tdb_goods_types数据表

  UPDATE tdb_goods_types AS t1 INNER JOIN ( SELECT p.type_id,p.type_name,count(s.type_name) AS children_count FROM tdb_goods_types AS p 

  LEFT JOIN tdb_goods_types AS s ON s.parent_id = p.type_id 

            GROUP BY p.type_name 

           ORDER BY p.type_id ) AS t2 

        ON  t1.type_id = t2.type_id 

        SET t1.child_count = t2.children_count;


-- 复制编号为12,20的两条记录

  SELECT * FROM tdb_goods WHERE goods_id IN (19,20);


-- INSERT ... SELECT实现复制

  INSERT tdb_goods(goods_name,cate_id,brand_id) SELECT goods_name,cate_id,brand_id FROM tdb_goods WHERE goods_id IN (19,20);

-- 查找重复记录

  SELECT goods_id,goods_name FROM tdb_goods GROUP BY goods_name HAVING count(goods_name) >= 2;


-- 删除重复记录

  DELETE t1 FROM tdb_goods AS t1 LEFT JOIN (SELECT goods_id,goods_name FROM tdb_goods GROUP BY goods_name HAVING count(goods_name) >= 2 ) AS t2  ON t1.goods_name = t2.goods_name  WHERE t1.goods_id > t2.goods_id;
 a.brand_id = b.brand_id INNER JOIN
            goods_cate as c on a.cate_id = c.cate_id;

Drop table goods_brands;
```



