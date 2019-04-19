# SQL Server 聚合函数算法优化技巧 - 文章 - 伯乐在线
原文出处： [头头哥](http://www.cnblogs.com/toutou/p/5002996.html)
博客前言
> 
Sql server聚合函数在实际工作中应对各种需求使用的还是很广泛的，对于聚合函数的优化自然也就成为了一个重点，一个程序优化的好不好直接决定了这个程序的声明周期。Sql server聚合函数对一组值执行计算并返回单一的值。聚合函数对一组值执行计算，并返回单个值。除了 COUNT 以外，聚合函数都会忽略空值。 聚合函数经常与 SELECT 语句的 GROUP BY 子句一起使用。
1.写在前面
如果有对Sql server聚合函数不熟或者忘记了的可以看我之前的一片博客。[sql server 基础教程](http://www.cnblogs.com/toutou/p/4733670.html#_nav_5)。
本文中所有数据演示都是用Microsoft官方示例数据库：Northwind，至于Northwind大家也可以在网上下载。至于下载方法[MSDN](https://msdn.microsoft.com/zh-cn/library/ms143221.aspx)已经有了详细的说明了，这里就不多说了。
2.Sql server标量聚合
### **2.1.概念： **
在只包含聚合函数的 SELECT 语句列列表中指定的一种聚合函数（如 MIN()、MAX()、COUNT()、SUM() 或 AVG()）。当列列表只包含聚合函数时，则结果集只具有一个行给出聚合值，该值由与 WHERE 子句谓词相匹配的源行计算得到。
### **2.2.探索标量聚合： **
我们先用Sql server的”包括实际的执行计划”来看看一个简单的流聚合COUNT()来看看表里数据所有的行数。
![](http://ww3.sinaimg.cn/mw690/6941baebgw1ezvfbkt89fj20r30dkdk0.jpg)
再通过[SET SHOWPLAN_ALL ON](https://msdn.microsoft.com/en-us/library/ms187735.aspx)(关于输出中包含的列更多信息可以在链接中查看)来看看有关语句执行情况的详细信息，并估计语句对资源的需求。
通过SET SHOWPLAN_ALL ON我们来看看COUNT()具体做了那些事情:
- 索引扫描：扫描当前表的行数
- 流计算：计算行数的数量
- 计算标量：将流计算出来的结果转化为适当的类型。(因为索引扫描出来的结果是根据表中数据的大小决定的，如果表中数据很多的话，COUNT是int类型就会有问题，所以在最终返回的时候需要将默认类型(数值一般默认类型是Big)转成int类型。)
- 小结：通过SET SHOWPLAN_ALL ON我们可以查看Sql server聚合函数在给我们呈现最终效果的时候，为这个效果做了些什么事情。
![](http://ww3.sinaimg.cn/mw690/6941baebgw1ezvfbkfuafj20uk08ftd4.jpg)
### **2.3.标量聚合优化技巧： **
我们通过两个比较简单的sql查询来看看他们的区别

MySQL
```
SELECT COUNT(DISTINCT ShipCity) FROM Orders
SELECT COUNT(DISTINCT OrderID) FROM Orders
```
![](http://ww2.sinaimg.cn/mw690/6941baebgw1ezvfbjydt0j20ku0a5wi1.jpg)
从上图中可以看到，其实这两个查询从语句上来说没什么太大的区别，但是为什么开销会不一样，一个是查询城市一个是查询订单号。这是因为其实DISTINCT对于OrderID查询来说，是没有什么意义的，因为OrderID是主键，是不会有重复的。而ShipCity是会有重复的，Sql server的去重机制在去重的时候，会有一个排序的过程。这个排序还是比较消耗资源的。
对于数据量比较大的表其实不是很建议对大表排序或者对大表的某个重复次数多的字段去重运算。所以我们这里可以对ShipCity进行优化一下。可以对ShipCity创建一个[非聚集索引](http://baike.baidu.com/link?url=xD7yYSlVnLniNzgN_wJRselPVgRZSEtpKBXTre2tVp4oH6Qr_q8eNgYtQAgSJ0n2avwths8lJTiKNZesdHsq1q)。

MySQL
```
CREATE INDEX Index_ShipCity On Orders(ShipCity desc)
go
```
![](http://ww1.sinaimg.cn/mw690/6941baebgw1ezvfbj6zh7j20l40d477x.jpg)
从上图中可以看到，加了索引以后COUNT(DISTINCT ShipCity)的查询变成了两个流聚合，而没有了排序，节省了开销。
总结：对于标量聚合从上面的例子大家可以看到，标量聚合优缺点很明显:
- Sql server标量聚合优点：算法比较简单直观，适合非重复值的聚合操作。
- Sql server标量聚合缺点：性能较差(需要排序)，不适合重复值的聚合操作。
优化技巧:
- 尽量避免排序产生
- 将分组字(GROUP BY)段锁定在索引覆盖范围内
3.Sql server哈希聚合
### **3.1.概念： **
哈希(Hash，一般翻译做“散列”，也有直接音译为“哈希”的，就是把任意长度的输入（又叫做预映射， pre-image），通过散列算法，变换成固定长度的输出，该输出就是散列值。这种转换是一种压缩映射，也就是，散列值的空间通常远小于输入的空间，不同的输入可能会散列成相同的输出，所以不可能从散列值来唯一的确定输入值。简单的说就是一种将任意长度的消息压缩到某一固定长度的消息摘要的函数。)
哈希聚合的内部实现方法和哈希连接的实现机制一样，需要哈希函数的内部运算，形成不同的哈希值，依次并行扫描数据形成聚合值。
### **3.2.背景： **
为了解决流聚合的不足，应对大数据的操作，所以哈希聚合就诞生了。
### **3.3.分析： **
来看看两个简单的查询。
![](http://ww4.sinaimg.cn/mw690/6941baebgw1ezvfbizn0cj20uf0grqbh.jpg)
ShipCountry和CustomerID的分组查询看上去很类似，但是为什么执行计划会不同呢？这是因为ShipCountry包含了大量的重复值，CustomerID重复值非常少，所以Sql server系统给ShipCountry推送的哈希聚合，而CustomerID推送的是流聚合。也就是说Sql server系统会动态的根据查询的情况选择合适的聚合方式。所以我们在做SQL优化的时候不能仅根据SQL语句来优化，还得结合具体数据分布的环境。
4.运算过程监控指标
### **4.1.监控元素： **
- 可视化查看运行时间
- T-sql语句查询时间
- 占用内存
- T-sql语句查询IO
### **4.2.可视化查看运行时间： **
![](http://ww4.sinaimg.cn/mw690/6941baebgw1ezvfbicr58j20s40dctdk.jpg)
### **4.3.T-sql语句查询时间： **
![](http://ww2.sinaimg.cn/mw690/6941baebgw1ezvfbhpua9j20hu07xjsy.jpg)
### **4.4.占用内存： **
![](http://ww2.sinaimg.cn/mw690/6941baebgw1ezvfbgv1n9j20ue0h4grc.jpg)
### **4.5.T-sql语句查询IO： **
![](http://ww4.sinaimg.cn/mw690/6941baebgw1ezvfbgo8yxj20io07l40x.jpg)
关于监控元素还有很多，这里就列举几个。
博客总结
> 
SQL Server 聚合函数算法优化技巧差不多就介绍到这里，如果有对sql语句优化感兴趣的可以看这篇博客。[sql server之数据库语句优化](http://blog.jobbole.com/97102/)
