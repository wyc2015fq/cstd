# 浪尖，请问如何确定hive分桶数？ - Spark高级玩法 - CSDN博客
2018年08月01日 00:11:36[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：470
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
今日，有人在星球问了一个比较好的问题：浪尖，请问如何确定hive的分桶数呢？
关于这个问题，浪尖想写个文章，谈谈我自己的看法，当然也欢迎有经验的同学么留言。
顺便打个广告，更多优质文章和问题答疑及视频教程请点击原文链接，加入浪尖**知识星球-****Spark技术学院**获取。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
需要了解hive的分区分桶及二者的区别
[hive的分区和分桶](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484404&idx=1&sn=88c0418a866572c1dd85ced93d1168f0&chksm=9f38e0dca84f69ca45cd77722839004db02019297c8608835114e382d1b85c6bb6c1fe920934&scene=21#wechat_redirect)
相关hive文章
[Hive性能优化（全面）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485140&idx=1&sn=dd8d05309b8e2e86b3bde6728c6932ec&chksm=9f38e5fca84f6ceae8eb4791337ccfe81fc6764890100bb2cb7f7aec2ad23b1a78e1e25f56c4&scene=21#wechat_redirect)
为啥要分桶？
首先要知道，Hive 分区提供了一种将hive表数据分成多个文件/目录的方法。 但是，它只在少数情况下提供有效的效果，比如：
- 当分区数量有限时。
-且 分区的大小相对相等。
然而，这基本是不可能。 例如，根据国家/地区等地理位置对表格进行分区。 那么会发现一些较大国家产生的分区会很大（例如：4-5个国家本身占总数据的70-80％）。 小国家会产生小分区（世界上所有国家仍然可能只占总数据的20-30％）。 因此，这时分区将不是理想的。为了解决过度分区的问题，Hive提供了分桶的概念。 这是将表数据集分解为更易于管理的部分的另一种有效技术。
hive分桶的特点
分桶的基本原理是分桶列的hash_function%mod = bucketId。
可以看到前提是，我们要指定mod，也即是分桶的个数，其实该值也是运行的最大reduce个数。
分桶的特征如下：
- 
 hash_function取决于bucketing列的类型。
- 
具有相同分段列的记录将始终存储在同一个桶中。
- 
使用CLUSTERED BY将表分成桶。
- 
 通常，在表目录中，每个存储桶只是一个文件，并且存储桶编号是从1开始的。
- 
可以先分区再分桶，也可以直接分桶。
- 
此外，Bucketed表将创建几乎相等的分布式数据文件块（取决于分桶列是否离散）。
hive分桶的优势
- 
与非分桶表相比，分桶表提供了高效采样。通过采样，我们可以尝试对一小部分数据进行查询，以便在原始数据集非常庞大时进行测试和调试。
- 
由于数据文件是相同大小的部分，map-side join在分桶表上执行的速度比分区表块很多。在map-side join时，处理左侧表的map知道要匹配的右表中的行在相关的桶中，因此只需要检索该桶。
- 
分桶表查询速度快于非分桶表。
- 
Bucketing概念还提供了灵活性，可以使每个存储桶中的记录按一列或多列进行排序。 这使得map-side join更加高效，因为每个存储桶的join变为高效的合并排序（merge-sort）。
hive分桶的缺点
指定bucketing并不能确保正确填充表。 数据加载到存储桶需要由我们自己处理。
举个例子
**创建分区分桶表**
比如创建一个表，按照国家分区，州分桶，然后对城市进行升序排序
CREATETABLE bucketed_user(
          firstname VARCHAR(64),
          lastname  VARCHAR(64),
          address   STRING,
          city     VARCHAR(64),
        state     VARCHAR(64),
          post      STRING,
          phone1    VARCHAR(64),
          phone2    STRING,
          email     STRING,
          web       STRING
          )
COMMENT'A bucketed sorted user table'
          PARTITIONED BY (country VARCHAR(64))
        CLUSTERED BY (state) SORTED BY (city) INTO32 BUCKETS
STOREDAS SEQUENCEFILE;
**插入数据**
与分区表类似，我们不能直接使用LOAD DATA（LOCAL）INPATH命令加载数据到分桶表，而是需要使用INSERT OVERWRITE TABLE ... SELECT ... FROM子句来填充分桶表。 为此，我们将在hive中创建一个临时表，其中包含该表中输入文件中的所有列，我们将复制到目标bucketed表中。
假设我们已经创建了temp_user临时表，下面是用于使用temp_user表填充分桶表的HiveQL。
要填充分桶表，我们需要设置属性hive.enforce.bucketing = true，以便Hive知道创建表定义中声明的桶数。
set hive.enforce.bucketing = true;
INSERT OVERWRITE TABLE bucketed_user PARTITION (country)
SELECT  firstname ,
            lastname  ,
            address   ,
          city      ,
        state     ,
            post      ,
            phone1    ,
            phone2    ,
            email     ,
            web       ,
            country   
FROM temp_user;
**注意：**
属性hive.enforce.bucketing = true类似于分区中的hive.exec.dynamic.partition = true属性。 通过设置此属性，我们将在将数据加载到hive表时启用动态分桶。
它会自动将reduce任务的数量设置为等于表定义中提到的桶数（例如，在我们的例子中为32），并自动从表定义中选择clustered by列。
如果我们不在Hive Session中设置此属性，我们必须手动将相同的信息在上面的INSERT ... SELECT语句结尾处传递给Hive，也即要运行的reduce任务的数量（例如在我们的例子中，通过set mapred.reduce.tasks = 32）和CLUSTER BY （state）还有SORT BY（city）子句。
完整的sql语句如下
set hive.exec.dynamic.partition=true;
set hive.exec.dynamic.partition.mode=nonstrict;
set hive.exec.max.dynamic.partitions.pernode=1000;
set hive.enforce.bucketing = true;
DROPTABLEIFEXISTS bucketed_user;
CREATETEMPORARYTABLE temp_user(
         firstname VARCHAR(64),
         lastname  VARCHAR(64),
         address   STRING,
         country   VARCHAR(64),
         city      VARCHAR(64),
         state     VARCHAR(64),
         post      STRING,
         phone1    VARCHAR(64),
         phone2    STRING,
         email     STRING,
         web       STRING
         )
ROWFORMATDELIMITED
FIELDSTERMINATEDBY','
LINESTERMINATEDBY'\n'
STOREDAS TEXTFILE;
LOADDATALOCAL INPATH '/home/user/user_table.txt'INTOTABLE temp_user;
CREATETABLE bucketed_user(
         firstname VARCHAR(64),
         lastname  VARCHAR(64),
         address   STRING,
         city        VARCHAR(64),
       state     VARCHAR(64),
         post      STRING,
         phone1    VARCHAR(64),
         phone2    STRING,
         email     STRING,
         web       STRING
         )
COMMENT'A bucketed sorted user table'
         PARTITIONED BY (country VARCHAR(64))
       CLUSTERED BY (state) SORTED BY (city) INTO32 BUCKETS
STOREDAS SEQUENCEFILE;
set hive.enforce.bucketing = true;
INSERT OVERWRITE TABLE bucketed_user PARTITION (country)
SELECT  firstname ,
                   lastname  ,
                   address   ,
               city      ,
               state     ,
                   post      ,
                   phone1    ,
                   phone2    ,
                   email     ,
                   web       ,
                   country   
FROM temp_user;
如何确定分桶数
分桶数的确定要结合和两点：
1，分桶的列基数要大，也即是该列去重后的值要大。
3，每个桶数据文件不能太小也不能太大。比如，如果block大小是256MB，那么使每个桶512 MB，是个不错的选择。
**强调一下，为了正确的加载数据，需要将reduce数目和分桶数一样。设置方法如上。**
