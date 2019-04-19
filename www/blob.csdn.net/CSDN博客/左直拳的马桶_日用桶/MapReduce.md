# MapReduce - 左直拳的马桶_日用桶 - CSDN博客
2014年09月16日 22:52:33[左直拳](https://me.csdn.net/leftfist)阅读数：1939
个人分类：[mongoDB																[nosql](https://blog.csdn.net/leftfist/article/category/2557431)](https://blog.csdn.net/leftfist/article/category/2557427)
一、什么是MapReduce
不要看百度百科上那个解释，什么映射啦，归约啦，我靠，什么鬼东西？这个解释成功地将一个简单的道理说到无人能懂。
还有个印度傻逼，说要给他那个傻乎乎的老婆解释什么是MapReduce，晒命似的专门跑到披萨店里去举洋葱作为例子，冗长得要命，越说越糊涂。这说明这个傻逼阿三自己也没弄明白。
其实，Map就是分解执行，Reduce就是结果合并。
有老外用极其简单的例子解释得清清楚楚：
我们要数图书馆中的所有书。你数1号书架，我数2号书架。这就是“Map”。我们人越多，数书就越快。
然后我们到一起，把所有人的统计数加在一起。这就是“Reduce”。
二、MapReduce有什么用
看上去，MapReduce跟大数据处理有关。数据很大，分布在好多服务器里，于是分解执行，然后结果合并，得到最终结果。
三、MongoDB中的mapReduce
MongoDB里也有MapReduce，但大家举的例子，似乎MongoDB版的MapReduce与关系型数据的group by类似。大概是mongoDB中的分组操作，会自然而然地适用于分布式计算吧？
假设集合TalkRecord有文档结构类似：
{ "_id" : ObjectId("54179064b3d587157c805001"), "id" : 220936, "content" : "Hello World!" }
我们可以写这么一个MapReduce：
```java
> db.talkrecord.mapReduce( function(){emit(this.id,1)}, function(key,values){return Array.sum(values)}, {query:{id:220936}, out:"post_total" } )
```
map函数中，emit是必须的。
emit(key,value)中，那么这个key，value怎么赋值呢？我觉得这有点误导，以为key必定是字段名，value是该字段的值，那么
**[javascript]**[view plain](http://blog.csdn.net/leftfist/article/details/39321045#)[copy](http://blog.csdn.net/leftfist/article/details/39321045#)
- emit(this.id,1)  
作何解释，难道是this.id==1才行，这样岂不是成了过滤条件啦？
其实，emit(key,value)中，key相当于group by 的字段，value则是 聚合操作的字段。比如关系型数据库中，有SQL语句
**[sql]**[view plain](http://blog.csdn.net/leftfist/article/details/39321045#)[copy](http://blog.csdn.net/leftfist/article/details/39321045#)
- selectsum([money]) from t groupby userId  
那在mongodb这里，userId就相当于emit里的KEY，money就相当于value。
emit(this.Id,1)的意思是，按id分组，数个数。1，跟count(1)差不多意思，你写个100，其实结果也一样。
