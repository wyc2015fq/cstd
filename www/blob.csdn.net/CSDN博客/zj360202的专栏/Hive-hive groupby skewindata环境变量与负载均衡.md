# Hive - hive.groupby.skewindata环境变量与负载均衡 - zj360202的专栏 - CSDN博客





2014年08月07日 16:11:51[zj360202](https://me.csdn.net/zj360202)阅读数：4917









**HiveQL 去重操作**

和SQL一样，HiveQL中同样支持DISTINCT操作，如下示例：

(1) SELECT 

count(DISTINCT uid) FROM log

(2) SELECT ip, count(DISTINCT uid) FROM log GROUP BY ip

(3) SELECT ip, count(DISTINCT uid, uname) FROMlog GROUP BY ip

(4) SELECT ip, count(DISTINCTuid), count(DISTINCT uname) FROMlog GROUP BY ip




当我们使用Hive QL中的去重关键字DISTINCT时，需要注意的一点是：

在多个列上进行的去重操作与hive环境变量hive.groupby.skewindata存在关系。

当hive.groupby.skewindata=true时，hive不支持多列上的去重操作，并报错：

Error in semantic analysis: DISTINCT on different columns notsupported with skew in data. 

注意：上面示例中的(3)不属于多列上的DISTINCT操作。


```java
select mac,count(1) from log group by mac;
```
当某一个mac的值非常多，如果几个mac的hash是一样的，会分到一个reduce，且数量已经超过一个reduce的容量或是处理能力的时候，上面的语句改成

```java
hive.groupby.skewindata=true;
select mac,count(1) from log group by mac;
```




#### Group By 语句
- 
Map 端部分聚合：
- - 
并不是所有的聚合操作都需要在 Reduce 端完成，很多聚合操作都可以先在 Map 端进行部分聚合，最后在 Reduce端得出最终结果。
- 
基于 Hash
- 
参数包括：
- - **hive.map.aggr = true** 是否在 Map 端进行聚合，默认为True
- **hive.groupby.mapaggr.checkinterval =100000** 在 Map 端进行聚合操作的条目数目


- 
有数据倾斜的时候进行负载均衡
- - **hive.groupby.skewindata = false**
- 
当选项设定为 true，生成的查询计划会有两个 MR Job。第一个 MR Job 中，Map 的输出结果集合会随机分布到Reduce 中，每个 Reduce 做部分聚合操作，并输出结果，这样处理的结果是相同的 Group By Key有可能被分发到不同的 Reduce 中，从而达到负载均衡的目的；第二个 MR Job 再根据预处理的数据结果按照 Group ByKey 分布到 Reduce 中（这个过程可以保证相同的 Group By Key 被分布到同一个 Reduce中），最后完成最终的聚合操作。


**hive.groupby.skewindata变量**
从上面groupby语句可以看出，这个变量是用于控制负载均衡的。当数据出现倾斜时，如果该变量设置为true，那么Hive会自动进行负载均衡。

**HIVE-2416**



Currently when multiple distinct function is used,hive.groupby.skewindata optimization parameter shall be set false,or else an exception is raised:


Error in semantic analysis: DISTINCT on different columns not supported with skew in data




Skew groupby should support multiple distinct function.






