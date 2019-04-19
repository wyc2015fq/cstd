# Elasticsearch之query与filter - BlueSky - CSDN博客
2016年01月22日 14:00:34[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1922
## Aggregation聚合
aggregation分为bucket和metric两种，分别作为词元划分和数值计算。其中的bucket aggregation还支持在自身结果的基础上叠加新的aggregation。这就是aggregation领先于facet的地方。比如实现一个时序百分比统计在facet接口无法直接完成，在aggregation接口就很简单。
## query与filter
1、区别如下：
query是要相关性评分的，filter不要；
query结果无法缓存，filter可以。
所以，选择参考：
1、全文搜索、评分排序，使用query；
2、是非过滤，精确匹配，使用filter。
## filter缓存
默认情况下，并不是所有的filter都能用缓存。常用的比如term、terms、prefix、range、bool等filter，其过滤结果明确，也容易设置缓存，ES就对这几个默认开启了filter cache工嗯呢该。而复杂一些比如：geo、script等filter，从filddata数据到过滤结果还需要进行一些列计算的，ES默认是不开启filter cache的。而想and、not、or这几个关系型filter，也是不开启的。如果想要强制开启这些默认没有的filter cache，需要在请求的JSON中带上“cache”：true参数。
## query_then_fetch
ESd对请求的处理过程，是有不同类型的，默认的叫query_then_fetch。在这种情况下，各数据节点处理检索请求后，返回的是只包含文档id和相关性分之的结果，这一段处理，叫做query阶段；汇聚到这份结果后，按照分值排序，得到一个全集群最终需要的文档id，再向对应节点点发送一次文档获取请求，拿到文档内容，这一段处理叫做fetch阶段。两阶段都结束后才返回响应。
DFS_query_then_fetch类型，提高小数据量时的精确度；query_then_fetch类型在明确routing时可以省略一个数据来回；count类型，在不关心文档内容只需要计数时省略query极端；sacn类型批量获取数据省略query阶段（在reindex时就是使用这种类型）。
