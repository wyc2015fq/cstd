# Hive学习之Lateral View - Spark高级玩法 - CSDN博客
2018年10月16日 00:03:38[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：127
Lateral view与UDTF函数如explode()一起使用，UDTF对每个输入行产生0或者多个输出行。Lateral view首先在基表的每个输入行应用UDTF，然后连接结果输出行与输入行组成拥有指定表别名的虚拟表。Lateralview的语法如下：
```
LateralView: LATERAL VIEW udtf(expression) tableAlias AS columnAlias (',' columnAlias)*
fromClause: FROM baseTable (lateralView)*
```
在Hive-0.6.0之前，Lateral view不支持谓词下推优化，在Hive-0.5.0或者更早的版本中，如果查询中使用了WHERE子句，可能不会被编译，一个措施是在查询前将参数hive.optimize.ppd设置为false。在Hive-0.6.0中修复了该问题，UDTF不支持谓词下推。从Hive-0.12.0开始，可以省略列别名，在这种情况下，别名从UDTF返回的StructObjectInspector的域名继承而来。
下面通过例子来更好地理解Lateral view。假设存在下面的基表pageAds，该表有两列pageid（page的名称）和adid_list(page上出现的广告的数组)：
|**列名称**|**列类型**|
|----|----|
|pageid|STRING|
|adid_list|Array<int>|
拥有两列的样例表如下，用户想统计出现在所有页面上的某个广告出现的总次数。|**pageid**|**adid_list**|
|----|----|
|front_page|[1, 2, 3]|
|contact_page|[3, 4, 5]|
Lateral view结合explode()可以将adid_list转化为单独的行（explode函数将数组中的每个元素做为一行返回）：
```
SELECT pageid, adid
FROM pageAds LATERAL VIEW explode(adid_list) adTable AS adid;
```
输出的结果为：
|**pageid (string)**|**adid (int)**|
|----|----|
|"front_page"|1|
|"front_page"|2|
|"front_page"|3|
|"contact_page"|3|
|"contact_page"|4|
|"contact_page"|5|
要统计特定广告的出现次数，可以使用count和group by：
```
SELECT adid, count(1)
FROM pageAds LATERAL VIEW explode(adid_list) adTable AS adid
GROUP BY adid;
```
输出结果为：
|int adid|count(1)|
|----|----|
|1|1|
|2|1|
|3|2|
|4|1|
|5|1|
FROM子句可以有多个LATERAL VIEW子句，后面的LATERALVIEW可以引用出现在LATERAL VIEW左侧的任何表中的列，例如：
```
SELECT * FROM exampleTable
LATERAL VIEW explode(col1) myTable1 AS myCol1
LATERAL VIEW explode(myCol1) myTable2 AS myCol2;
```
LATERAL VIEW子句按照出现的顺序依次使用，例如存在下面的基表：
|Array<int> col1|Array<string> col2|
|----|----|
|[1, 2]|[a", "b", "c"]|
|[3, 4]|[d", "e", "f"]|
下面的查询：
```
SELECT myCol1, col2 FROM baseTable
LATERAL VIEW explode(col1) myTable1 AS myCol1;
```
输出的结果为：
|int mycol1|Array<string> col2|
|----|----|
|1|[a", "b", "c"]|
|2|[a", "b", "c"]|
|3|[d", "e", "f"]|
|4|[d", "e", "f"]|
再增加一个LATERAL VIEW后的查询语句为：
```
SELECT myCol1, myCol2 FROM baseTable
LATERAL VIEW explode(col1) myTable1 AS myCol1
LATERAL VIEW explode(col2) myTable2 AS myCol2;
```
输出的结果为：
|int myCol1|string myCol2|
|----|----|
|1|"a"|
|1|"b"|
|1|"c"|
|2|"a"|
|2|"b"|
|2|"c"|
|3|"d"|
|3|"e"|
|3|"f"|
|4|"d"|
|4|"e"|
|4|"f"|
从Hive-0.12.0开始，支持OUTER LATERALVIEW。当使用OUTRE关键字时可以再LATERAL VIEW不产生行时生成输出行。当UDTF不产生任何行时，比如explode()函数的输入列为空，LATERALVIEW就不会生成任何输出行。在这种情况下原有行永远不会出现在结果中。OUTRE可被用于阻止这种情况，输出行中来自UDTF的列将被设置为NULL。比如下面的语句将生成空的结果集：`SELEC * FROM src LATERAL VIEW explode(array()) C AS a limit 10;`
但使用OUTER关键字后：
`SELEC * FROM src LATERAL VIEW OUTRE explode(array()) C AS a limit 10;`
输出的结果为：
|238|val_238|NULL|
|----|----|----|
|86|val_86|NULL|
|311|val_311|NULL|
|27|val_27|NULL|
原文地址：https://blog.csdn.net/skywalker_only/article/details/39289709
推荐阅读：
[Hive性能优化（全面）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485140&idx=1&sn=dd8d05309b8e2e86b3bde6728c6932ec&chksm=9f38e5fca84f6ceae8eb4791337ccfe81fc6764890100bb2cb7f7aec2ad23b1a78e1e25f56c4&scene=21#wechat_redirect)
[戳破 | hive on spark 调优点](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485198&idx=1&sn=2ab4a6297443d0a6822d193d6ada0823&chksm=9f38e426a84f6d300a283df4a1d360bc8fc0b9b0e7965af46e039f34f46f6b9753d30d95a4d2&scene=21#wechat_redirect)
[重要 | mr使用hcatalog读写hive表](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485234&idx=1&sn=a4a2795b320acdbe6f8767e4d145549f&chksm=9f38e41aa84f6d0c0b3fffad5212ae0ea3501b974fc6ce81856f2de06bb6162056ad644c2a6e&scene=21#wechat_redirect)
[重磅：关于hive的join使用必须了解的事情](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484426&idx=1&sn=50613f6f4f6fda4a139b01d2f04656d6&chksm=9f38e722a84f6e34c4c8df1d9849fda384a0e7659ac383b378ea9948c522b98c8a5462250a60&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
