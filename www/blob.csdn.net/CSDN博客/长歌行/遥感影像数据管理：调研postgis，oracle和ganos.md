# 遥感影像数据管理：调研postgis，oracle和ganos - 长歌行 - CSDN博客





置顶2019年03月06日 11:02:51[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：834标签：[gis																[遥感																[云																[数据库																[时空](https://so.csdn.net/so/search/s.do?q=时空&t=blog)
个人分类：[时空																[GIS/RS/GPS](https://blog.csdn.net/iuhsihsow/article/category/1078364)](https://blog.csdn.net/iuhsihsow/article/category/8582762)

所属专栏：[时空云计算](https://blog.csdn.net/column/details/32384.html)







# 背景

最近有一批分幅的影像数据，每幅大概100-200MB，之前都是以散文件的形式直接放在磁盘里面。然后用QGIS加载，保存一个工程文件。同事说需要那个区域的影像的时候，我就打开QGIS，拉框选择，然后查看文件名，再根据文件名找到数据，然后发给同事。
时间久了，觉得这种方法比较low，也比较低效，人工操作的内容比较多，不好写到脚本里面去。正好这段时间手头没什么事情，就想想看有什么好办法，将影像管理起来，操作更加自动化。



# 方案筛选



## 自己编写脚本

最开始想到的是，自己编写一个脚本，遍历文件夹中所有影像，使用GDAL读影像的范围，然后把影像的名字，路径，范围等信息写在一张数据表里面。以后需要哪个范围的数据，直接一个SQL语句，返回区域内影像的路径。然后按照路径打包数据，发给同事。
但是后来发现一些问题：1. GDAL不太熟，而且不同类型的影像数据投影还不一样，空间范围要重投影，2. 既然都用到数据库来存储代表了影像空间范围的几何体，为什么不直接用数据库来存影像啊。3. 将数据发给同事的过程不好自动化，如果同事要的不是整张影像而是一部分的话，还需要进行裁剪。想到这些，就开始在网上查找可以存储影像的数据库。



## GDB/ArcSDE

最开始想到的是ArcGIS系列的[geodatabase](http://desktop.arcgis.com/zh-cn/arcmap/10.3/manage-data/raster-and-images/how-raster-data-is-stored-and-managed.htm)，使用栅格数据集或者镶嵌数据集来管理。数据支持类型也挺多。

> 
ArcGIS 支持超过 70 种不同的栅格数据集文件格式，其中包括 TIFF、JPEG 2000、Esri Grid 和 MrSid。


但是它对于影像支持有大小限制，对于FileGDB每个栅格数据集1TB，对于个人地理数据库，每个表的限制为2GB。从数据量方面来说，不是很符合要求啊。企业级数据库和ArcSDE倒是没限制，但我也用不起啊。另外，单位目前也没ArcGIS系列软件。还是先看看有没有其他方案。还是直接看看其他数据库对影像的支持。



## Oracle

Oracle数据库可是鼎鼎有名，想来对于影像的支持也不差。可以试试。（其实是正好机房里面有一台电脑安装了oracle‘以供学习’）。
Oracle 数据库中的地理数据库以 LONG RAW、BLOB、GeoRaster 或 ST_Raster 存储类型来存储栅格数据。使用的比较多的还是GeoRaster数据。相关概念见[链接](https://docs.oracle.com/cd/B19306_01/appdev.102/b14254/geor_intro.htm)。GeoRaster体系结构是栅格单元数据和元数据的混合体。



## PostGIS

作为一个GISer，对于PostgreSQL（与PostGIS）还是不陌生的。就看看PG的raster模块应该可以一试。PostGIS中支持使用raster类型来表示栅格数据。可以使用raster2pgsql来进行导入，支持的数据格式为GDAL支持的数据格式。



## Ganos

之前从3s news上听说阿里云最近推出了时空引擎：[Ganos](http://www.3snews.net/domestic/244000054166.html)。里面有关于栅格的介绍：“Ganos遥感影像数据库引擎支持各类空间栅格型数据的导入导出、拼接、剪切、统计分析等操作，并融合了云上GPU的并行计算加速能力和OSS稳定、高效、低成本的对象存储扩展能力。然后大概看了下官方文档，虽然没有对数据表结构和关系的进一步分析，单从说明来看，应该是可以一试。正好以后工作和云也许有交集，弄个云数据库试试。


大致看来，准备试试Oracle，Postgis和Ganos。



# 初步调研



## Oracle

oracle 导入数据暂时还没找着现成的工具，就使用sql plus和sql进行导入了。网上搜了下，代码如下所示。

```
--创建一个自增的ID序列
CREATE SEQUENCE land_image_id_seq
    increment by 1
    start with 1
    nomaxvalue
    minvalue 1
    NOCYCLE;

--创建GeoRaster表（GeoRaster表就是带有SDO_GEORASTER类型列的表）
CREATE TABLE land(
    id NUMBER PRIMARY KEY,
    name VARCHAR(30),
    image SDO_GEORASTER
);

--创建一个自增的触发器
CREATE OR REPLACE TRIGGER land_image_id_trig
    BEFORE INSERT ON land
    FOR EACH ROW
    BEGIN
        SELECT land_image_id_seq.nextval INTO :new.id FROM dual;
    END;
/

--创建GeoRaster Data Table（RDT）表用来存储GeoRaste影像数据
CREATE TABLE rdt_land OF SDO_RASTER
(
    PRIMARY KEY(RASTERID, PYRAMIDLEVEL, BANDBLOCKNUMBER, ROWBLOCKNUMBER, COLUMNBLOCKNUMBER)
)
LOB(RASTERBLOCK) STORE AS (NOCACHE NOLOGGING);

--插入GeoRaster表一条记录（初始化GeoRaster对象）
INSERT INTO land (name, image) VALUES ('0', SDO_GEOR.INIT('rdt_land'));
call dbms_java.grant_permission('MDSYS','SYS:java.io.FilePermission', '/opt/test_data/land0.tif', 'read');
-- 导入Raster Data Table表中
DECLARE
    img SDO_GEORASTER;
BEGIN
    --Select the georater column
    SELECT image INTO img FROM land WHERE name='0' FOR UPDATE;
    --Import into the georater object
    SDO_GEOR.IMPORTFROM(img, 'blocksize=(256, 256)', 'TIFF', 'file', '/opt/test_data/land0.tif');
    --update the column
    UPDATE land SET image=img WHERE name='0';

    COMMIT;
END;
/
```

虽然网上资料比较多，但是自己操作起来，过程还是比较复杂的，得先创建栅格表和块表，然后获取数据的读权限，再进行导入。调试了很久才能成功导入数据，对于接触oracle影像模块的我来说，几乎每一步操作都会卡壳，还好从网上都能找到解决方案，总体来说，使用起来感觉还是不直观，需要书写大段的SQL语句，查询较多的参数配置。而且对于数据量大于2G的影像，总是导入失败。搜了下，有可能使用sql语句不能直接导入2G以上的影像。。鉴于如此多不便之处，暂时不采用这个方案了吧。



## PostGIS

翻阅网上的资料可以使用 raster2pgsql来进行导入，大概用法如下所示：

> 
The `raster2pgsql` is a raster loader executable that loads GDAL supported raster formats into sql suitable for loading into a PostGIS raster table. It is capable of loading folders of raster files as well as creating overviews of rasters.


后面还有10多个参数说明，太长了，就不列在这一篇博客里了。感觉功能很强大，但是有点复杂，不利于上手。

```
EXAMPLE USAGE:
raster2pgsql raster_options_go_here raster_file someschema.sometable > out.sql

Example：
raster2pgsql -s 4326 -I -C -M *.tif -F -t 100x100 public.demelevation > elev.sql
psql -d gisdb -f elev.sql

导入多个图像
raster2pgsql -s 4326 -C -l 2,4 -I -F -t 2700x2700 gray_50m_partial*.tif data_import.gray_50m_partial | psql -h localhost -p 5434 -U postgres -d mastering_postgis
```

运行raster2pgsql，可以生成sql语句，SQL语句大概如下所示：

```
1 BEGIN;
2 CREATE TABLE "public"."demelevation" ("rid" serial PRIMARY KEY,"rast" raster,"filename" text);
3 INSERT INTO "public"."demelevation" ("rast","filename") VALUES ('0100000300F..省略若干..223E4C32224056'::raster,'land.tif');
...中间省略若干行...
10587 CREATE INDEX ON "public"."demelevation" USING gist (st_convexhull("rast"));
10588 ANALYZE "public"."demelevation";
10589 SELECT AddRasterConstraints('public','demelevation','rast',TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE);
10590 END;
10591 VACUUM ANALYZE "public"."demelevation";
```

一个300MB的数据，生成的导入sql语句大概600MB，10000多行。看来要合理设置分片大小。



## Ganos

Ganos是阿里云推出的时空数据库，需要在云上操作。对于GISer来说，估计大家一般都习惯于在本地开发，或者使用公司服务器上的各种数据库服务。对于云数据库，估计觉得还是很新鲜吧。我想着，趁现在有时间，学习学习，云技术是未来的趋势，万一以后单位业务要上云呢？加上之前使用阿里ECS搭代理翻墙（详见上一篇文章[阿里云ECS使用小记](https://blog.csdn.net/uninterrupted/article/details/86617247)），感觉还不错。

先购买RDS，既然是测试，就买个低配，选择按量付费，用完就停掉。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190306221620543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)
后面大概是这几步把，和申请ECS差不太多。


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190306221701714.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)

官网里面也提供了和影像相关的操作方法。[链接](https://help.aliyun.com/document_detail/95571.html?spm=a2c4g.11186623.6.1038.7bac24d8oNewnx)

```
创建扩展 
Create Extension Ganos_Raster cascade;

创建栅格表
Create Table raster_table(id integer, raster_obj raster);

导入数据：
Insert into raster_table Values(1, ST_ImportFrom('chunk_table','OSS://ABCDEFG:1234567890@oss-cn.aliyuncs.com/mybucket/data/4.tif'))
```

数据好像只能放到OSS或者ECS上，不过我这边数据本来就是要发给同事的，发个链接还是发个文件，差别不大。看了下OSS收费，价格还是挺便宜的。问题不大。
只用一条SQL语句就能导入影像数据了。这个比较好。相对于Oracle几十行代码来说，真是轻松了不少。使用SQL语句的话，可以在在线网页界面执行了。对于像我这样，终端玩不太转的人来说，是福音啊。
另外，看到官网里面有说明“当用户购买包含GPU的RDS实例时，自动开启GPU并行加速计算”，GPU并行加速在影像领域还是不错的，大概瞅了一眼，没注意到GPU版本的选型，不过目前我就是简单测试下，暂时还不需要GPU。以后再看看吧。
然后看了下，使用ST_Extent得到影像范围，再使用st_intersects进行求交计算就可以实现框选操作。

```
BOX ST_Extent(raster raster_obj,CoorSpatialOption csOption = 'WorldFirst')
```



# 小结

目前看来oracle各种操作有点复杂，需要书写大段的SQL语句。使用postgis需要使用raster2sql语句来生成sql，感觉不是很直观，如果不进行切分的话，存储不了过大的数据。Ganos整体来说，使用起来比较简便，一条SQL语句就可以导入影像，文档是中文的，也比较通俗易懂，所有操作都在云上进行，维护应该比本地简单，也符合目前云计算的发展趋势。
后续准备对Ganos for PG进行进一步的评估，从功能和性价比方面看是否合适目前的需求。



# 后记

数据是导入进去了，但是能不能直接从数据库查看这些导入的数据呢？从论坛里面看到好像可以使用WMS等影像服务来查看，具体不知道怎么做，以后等有这个需求再去深入问问。



# 参考

[http://desktop.arcgis.com/zh-cn/arcmap/10.3/manage-data/raster-and-images/how-raster-data-is-stored-and-managed.htm](http://desktop.arcgis.com/zh-cn/arcmap/10.3/manage-data/raster-and-images/how-raster-data-is-stored-and-managed.htm)
[https://postgis.net/docs/using_raster_dataman.html](https://postgis.net/docs/using_raster_dataman.html)
[http://www.3snews.net/domestic/244000054166.html](http://www.3snews.net/domestic/244000054166.html)
[https://help.aliyun.com/document_detail/95571.html?spm=a2c4g.11186623.6.1038.7bac24d8oNewnx](https://help.aliyun.com/document_detail/95571.html?spm=a2c4g.11186623.6.1038.7bac24d8oNewnx)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=云&t=blog)](https://so.csdn.net/so/search/s.do?q=遥感&t=blog)](https://so.csdn.net/so/search/s.do?q=gis&t=blog)




