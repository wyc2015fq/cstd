
# MySQL常用性能分析方法-profile，explain，索引 - 无界 - CSDN博客

置顶2016年10月26日 21:17:30[21aspnet](https://me.csdn.net/21aspnet)阅读数：4554个人分类：[MySQL																](https://blog.csdn.net/21aspnet/article/category/838844)



**1.查版本号**
无论做什么都要确认版本号，不同的版本号下会有各种差异。
>Select  version();
![](https://img-blog.csdn.net/20161026133925528)

**2.执行状态分析**
显示哪些线程正在运行
>show processlist;
![](https://img-blog.csdn.net/20161026134141779)
下面是完整的信息
![](https://img-blog.csdn.net/20161026134827844)

**3.show profile**
show profile默认的是关闭的，但是会话级别可以开启这个功能，开启它可以让MySQL收集在执行语句的时候所使用的资源。
显示数据库列表
>show databases;
切换数据库
>use test;
>SELECT @@profiling;
![](https://img-blog.csdn.net/20161026135459861)
输出0说明profiles功能是关闭的
开启profiles功能
> set profiling=1;
![](https://img-blog.csdn.net/20161026140539634)

需要注意为了明确看到查询性能，我们启用profiling并关闭query cache：
>SET profiling = 1;
>SET query_cache_type = 0;
>SET GLOBAL query_cache_size = 0;

执行2条SQL语句
> show profiles;
![](https://img-blog.csdn.net/20161026141714954)

根据query_id 查看某个查询的详细时间耗费
> show profile for query 3;
![](https://img-blog.csdn.net/20161026141857409)

ALL
显示所有性能信息
>show profile all for query 3;
![](https://img-blog.csdn.net/20161026142037048)

ALL
显示所有性能信息
BLOCK IO
显示块IO（块的输入输出）的次数
CONTEXT SWITCHES
显示自动和被动的上下文切换数量
IPC
显示发送和接收的消息数量。
MEMORY
MySQL5.6中还未实现，只是计划实现。
SWAPS
显示swap的次数。

show profile for cpu只显示cpu信息
![](https://img-blog.csdn.net/20161026142232142)
show profile for cpu,block io 显示cpu和io信息
![](https://img-blog.csdn.net/20161026142239960)

参考：[http://dev.mysql.com/doc/refman/5.7/en/show-profile.html](http://dev.mysql.com/doc/refman/5.7/en/show-profile.html)
[http://wing324.github.io/2016/02/02/MySQL的SHOW-PROFILE详解/?utm_source=tuicool&utm_medium=referral](http://wing324.github.io/2016/02/02/MySQL%E7%9A%84SHOW-PROFILE%E8%AF%A6%E8%A7%A3/?utm_source=tuicool&utm_medium=referral)

**4.分析执行计划和最左前缀原理**
>explain + sql
![](https://img-blog.csdn.net/20161026151107386)

关于分析结果需要注意索引有没有用到。如果显示type=ALL就是全表扫描了。
当用Explain查看SQL的执行计划时，里面有列显示了 key_len 的值，根据这个值可以判断索引的长度，在组合索引里面可以更清楚的了解到了哪部分字段使用到了索引。
如果只用到一个int那么就是4，比如上面的联合索引只用到一个就是4，用到2个就是8以此类推。
联合索引需要注意**最左前缀原理**，就是说匹配最左边的字段，如果你的索引使用到abc三个字段，那么查a,ab,abc都可以用到索引，查ac和bc和b和c是不行的，必须从左到右逐渐增多。
最左前缀参考：http://blog.codinglabs.org/articles/theory-of-mysql-index.html

**联合索引**优化策略：
如何选择索引列的顺序
1.经常会被使用到的列优先
2.选择性高的列优先
3.宽度小的列优先

如果不是int型数据，对于varchar，假如设定长度是255，由于你使用的是UTF-8字符集占3个字节，255*3+2=767，如上图所示。
具体公式如下：
key_len的长度计算公式：
varchr(10)变长字段且允许NULL    =  10 * ( character set：utf8=3,gbk=2,latin1=1)+1(NULL)+2(变长字段)
varchr(10)变长字段且不允许NULL =  10 *( character set：utf8=3,gbk=2,latin1=1)+2(变长字段)
char(10)固定字段且允许NULL        =  10 * ( character set：utf8=3,gbk=2,latin1=1)+1(NULL)
char(10)固定字段且不允许NULL        =  10 * ( character set：utf8=3,gbk=2,latin1=1)

关于key_len的计算参考：http://www.cnblogs.com/gomysql/p/4004244.html

explain小技巧，可以加 \G以按行来显示分析结果，避免因为按列显示不下的情况：
![](https://img-blog.csdn.net/20161027144312720)

**5.索引选择性与前缀索引**
因为索引虽然加快了查询速度，但索引也是有代价的，另外，MySQL在运行时也要消耗资源维护索引，因此索引并不是越多越好。
一般两种情况下不建议建索引。
1.表记录比较少，超过2000条可以酌情考虑索引。
2.索引的选择性较低。所谓索引的选择性（Selectivity），是指不重复的索引值（也叫基数，Cardinality）与表记录数（\#T）的比值：
Index Selectivity = Cardinality / \#T
显然选择性的取值范围为(0, 1]，选择性越高的索引价值越大，这是由B+Tree的性质决定的。
参考：http://blog.codinglabs.org/articles/theory-of-mysql-index.html

其实就是算索引的**不重复度**
SELECT count(DISTINCT(ID))/count(*) AS Selectivity FROM xxxx;
SELECT count(DISTINCT(xx))/count(*) AS Selectivity FROM xxxx;
SELECT count(DISTINCT(concat(xx, xxx)))/count(*) AS Selectivity FROM xxxx;
SELECT count(DISTINCT(concat(xxx, left(xx, 2))))/count(*) AS Selectivity FROM xxxx;
![](https://img-blog.csdn.net/20161026162739176)
很明显因为主键id是不会重复的所以不重复度是100%也就是1.

6.**覆盖索引**
覆盖索引是指索引的叶子节点已包含所有要查询的列，因此不需要访问表数据所谓“回表”。
覆盖索引的优点
1.优化缓存，减少磁盘IO
2.减少随机IO，变随机IO为顺序IO
3.避免对Innodb主键索引的二次查询
4.避免MyISAM表进行系统调用

下面是《高性能MySQL(第3版)》中关于explain的Extra列的信息：
![](https://img-blog.csdn.net/20161027135643953)
下面举例
第一条sql和第二条sql都是where带相同的查询条件，这个已经建了索引，但是select里第一条是都覆盖索引的，而第二条是*，自然有不覆盖的，所以需要**回表**。
这样性能就差很多了。
![](https://img-blog.csdn.net/20161027135430442)
再开启profile，很明显回表的要慢0.012秒，也就是12毫秒。
![](https://img-blog.csdn.net/20161027140142492)

7.**文件排序和索引排序**
使用索引扫描来优化排序条件
1.索引的列顺序和Order by子句的顺序完全一致
2.索引中所有列的方向（升序，降序）和Order by子句完全一致
3.Order by中的字段全部在关联表中的第一张表中
![](https://img-blog.csdn.net/20161027174202526)
上面是一些不同的场景，其实就是如果排序可以利用索引就可以避免文件排序。
要想知道真实的时间还是需要执行SQL，然后比较。
![](https://img-blog.csdn.net/20161027174741151)
上图中使用主键索引id排序的查询是最快的，而使用二级索引，或者无索引的排序是非常耗时的需要10秒和17秒。使用了二级索引稍微好点，但是效果也不好。

