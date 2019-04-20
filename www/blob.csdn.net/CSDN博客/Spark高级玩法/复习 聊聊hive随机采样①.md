# 复习:聊聊hive随机采样① - Spark高级玩法 - CSDN博客
2018年08月28日 00:01:02[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：5317
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
数据量大的时候，对数据进行采样，然后再做模型分析。作为数据仓库的必备品hive，我们如何对其进行采样呢？
当然，浪尖写本文还有另一个目的就是复习hive的四by。不止是否有印象呢？
[Hive : SORT BY vs ORDER BY vs DISTRIBUTE BY vs CLUSTER BY](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484236&idx=1&sn=ee9622d02aa4d6888650014d139e0793&chksm=9f38e064a84f697293ecc7bcd2773c990e26f014dda4d095ff17ec48b35849f76fe7107ba9b2&scene=21#wechat_redirect)
欢迎点击**阅读原文**，加入浪尖知识星球。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
## 假设有一张包含100亿行的Hive表，希望有效地随机抽样一个固定行数的数据 - 比如10000。最明显（而且显然是错误的）的方法是：
```
select * from my_table
limit 10000;
```
如果不对表进行排序，Hive不保证数据的顺序，但在实践中，它们按照它们在文件中的顺序返回，所以这远非真正随机。那么接着可以尝试：
```
select * from my_table
order by rand()
limit 10000;
```
这确实提供了真正的随机数据，但性能并不是那么好。为了实现总排序，Hive必须将所有数据强制传输到单个reducer。该reducer将对整个数据集进行排序。这很不好。幸运的是，Hive有一个非标准SQL“sort by”子句，它只在单个reducer中排序，并且不保证数据跨多个reducers中排序：
```
select * from my_table
sort by rand()
limit 10000;
```
这要好得多，但我不相信它真的是随机的。问题是Hive的将数据拆分为多个reducer的方法是未定义的。它可能是真正随机的，它可能基于文件顺序，它可能基于数据中的某些值。Hive如何在reducers中实现limit子句也是未定义的。也许它按顺序从reducer中获取数据 - 即，reducer 0中的所有数据，然后全部来reducer1，等等。也许它通过它们循环并将所有内容混合在一起。
在最坏的情况下，假设reduce 的key是基于数据列，而limit子句是reducers的顺序。然后样品会非常倾斜。
解决方案是另一个非标准的Hive功能：“distribute by”。对于reduce key不是由查询结构确定的查询（没有“group by”，没有join），可以准确指定reduce key的内容。如果我们随机分布，并在每个reducer中随机排序，那么“limit”功能如何无关紧要。
```
select * from my_table
distribute by rand()
sort by rand()
limit 10000;
```
最后，作为最后一次优化，可以在map-side做一些过滤。如果表的总大小是已知的，轻松设置一个随机阈值条件来进行数据过滤，如下所示：
```
select * from my_table
where rand() <= 0.0001
distribute by rand()
sort by rand()
limit 10000;
```
在这种情况下，由于总大小是100亿，样本大小是一万，我可以很容易地计算出样本占总数据的0.000001。但是，如果where子句是“rand（）<0.000001”，则最终输出的行数可能少于10000行。“rand（）<0.000002”可能会起作用，但这确实依赖于rand（）有非常好的实现。最后它并不重要，因为瓶颈是全表扫描，而不是传输给reducer的这点数据。
**推荐阅读：**
[Hive鲜为人知的宝石-Hooks](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485243&idx=1&sn=c7f92678f69f282a4047dd2291d95d28&chksm=9f38e413a84f6d05f107ac4db6e9dd712dbfd1f6302c3ee8171c35f605b8fcfc099111ce0a65&scene=21#wechat_redirect)
[Hive性能优化（全面）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485140&idx=1&sn=dd8d05309b8e2e86b3bde6728c6932ec&chksm=9f38e5fca84f6ceae8eb4791337ccfe81fc6764890100bb2cb7f7aec2ad23b1a78e1e25f56c4&scene=21#wechat_redirect)
[不可不知的spark shuffle](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485276&idx=1&sn=c96a6731a48d5b714a0a76c83a9bc4ab&chksm=9f38e474a84f6d620defe9962fe8f325a9c1bd04bc398db43f6104280ae6ca1a9fe693a0ffd8&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
