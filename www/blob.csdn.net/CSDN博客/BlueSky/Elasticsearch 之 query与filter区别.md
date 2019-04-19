# Elasticsearch 之 query与filter区别 - BlueSky - CSDN博客
2016年01月22日 16:48:04[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：6709
参考官方文档说明如下：https://www.elastic.co/guide/en/elasticsearch/guide/current/_queries_and_filters.html#_performance_differences
## 主要意思是：
fiter是精确查询，对待的文档检索的结果是 是/否 ；query对应文档检索是对文档相关性评分。
表现（性能Performance）区别：filter返回是和条件匹配的一个简单的列表这是很快可以计算得到的并且也很容易在内存中做缓存；query不仅要找到匹配的文档，而且还要计算每个文档的相关性（评分），这就很明显比filter花费更多的计算。
想看下官方文档的翻译：
Query查询器 与 Filter 过滤器
尽管我们之前已经涉及了查询DSL，然而实际上存在两种DSL：查询DSL（query DSL）和过滤DSL（filter DSL）。
过滤器（filter）通常用于过滤文档的范围，比如某个字段是否属于某个类型，或者是属于哪个时间区间
* 创建日期是否在2014-2015年间？
* status字段是否为success？
* lat_lon字段是否在某个坐标的10公里范围内？
查询器（query）的使用方法像极了filter，但query更倾向于更准确的查找。
* 与full text search的匹配度最高
* 正则匹配
* 包含run单词，如果包含这些单词：runs、running、jog、sprint，也被视为包含run单词
* 包含quick、brown、fox。这些词越接近，这份文档的相关性就越高
查询器会计算出每份文档对于某次查询有多相关（relevant），然后分配文档一个相关性分数：_score。而这个分数会被用来对匹配了的文档进行相关性排序。相关性概念十分适合全文搜索（full-text search），这个很难能给出完整、“正确”答案的领域。
query filter在性能上对比：filter是不计算相关性的，同时可以cache。因此，filter速度要快于query。
## query与filter
1、区别如下：
query是要相关性评分的，filter不要；
query结果无法缓存，filter可以。
所以，选择参考：
1、全文搜索、评分排序，使用query；
2、是非过滤，精确匹配，使用filter。
过滤器（filter）通常用于过滤文档的范围，比如某个字段是否属于某个类型，或者是属于哪个时间区间。
## 参考博客一：
http://xiaorui.cc/2015/11/09/elasticsearch%E7%9A%84%E6%9F%A5%E8%AF%A2%E5%99%A8query%E4%B8%8E%E8%BF%87%E6%BB%A4%E5%99%A8filter%E7%9A%84%E5%8C%BA%E5%88%AB/
下面是使用query语句查询的结果,第一次查询用了300ms,第二次用了280ms.

Python
```
```
下面是使用filter查询出来的结果,第一次查询时间是280ms,第二次130ms…. 速度确实快了不少，也证明filter走了cache缓存。 但是如果我们对比下命中的数目，query要比filter要多一点，换句话说，更加的精准。 

Python
``
如果你想同时使用query和filter查询的话，需要使用 {query:{filtered：{}}} 来包含这两个查询语法。他们的好处是，借助于filter的速度可以快速过滤出文档，然后再由query根据条件来匹配。
我们这业务上关于elasticsearch的查询语法基本都是用query filtered方式进行的，我也推荐大家直接用这样的方法。should ，must_not, must 都是列表，列表里面可以写多个条件。 这里再啰嗦一句，如果你的查询是范围和类型比较粗大的，用filter ！ 如果是那种精准的，就用query来查询。 
{
   ”bool”:{
     ”should”:[],   #相当于OR条件
     ”must_not”:[],  #必须匹配的条件，这里的条件都会被反义
     ”must”:[]        #必须要有的
  }
}
