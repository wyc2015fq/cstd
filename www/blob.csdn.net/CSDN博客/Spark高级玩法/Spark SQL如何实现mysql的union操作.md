# Spark SQL如何实现mysql的union操作 - Spark高级玩法 - CSDN博客
2018年12月05日 00:18:34[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：444
**简介**
今天聊了聊一个小小的基础题，union和union all的区别：
union all是直接连接，取到得是所有值，记录可能有重复  
union 是取唯一值，记录没有重复  
1、UNION 的语法如下：
```
[SQL 语句 1]
      UNION
[SQL 语句 2]
```
2、UNION ALL 的语法如下：
```
[SQL 语句 1]
      UNION ALL
[SQL 语句 2]
```
**对比总结：**
UNION和UNION ALL关键字都是将两个结果集合并为一个，但这两者从使用和效率上来说都有所不同。
1、对重复结果的处理：UNION在进行表链接后会筛选掉重复的记录，Union All不会去除重复记录。
2、对排序的处理：Union将会按照字段的顺序进行排序；UNION ALL只是简单的将两个结果合并后就返回。
从效率上说，UNION ALL 要比UNION快很多，所以，如果可以确认合并的两个结果集中不包含重复数据且不需要排序时的话，那么就使用UNION ALL。
**Spark SQL**
实际上Spark SQL的DataSet的API是没有union all操作的，只有union操作，而且其union操作就是union all操作。
此时要实现union操作，需要在union之后加上distinct操作。
sales.union(sales).show()
输出结果是有重复数据的
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVicqArYFMDW4eqoCnAwLEKjXnTTE6oOwsZiaZrETb9Iicm3mI7xJNjAbECsTVVcbiaX3ibCjMHyib4FXIQ/640?wx_fmt=png)
需要将操作更改为：
sales.union(sales).distinct().show()
**推荐阅读：**
[Spark SQL的几个里程碑！](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485824&idx=1&sn=c92e846196fc11c01e9edfa8db3e7ff8&chksm=9f38eaa8a84f63bea211fc1d3950954833285c7b77cb9b48cda240676776e79f14788a944d6d&scene=21#wechat_redirect)
[Table API&SQL的基本概念及使用介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483931&idx=1&sn=324aba23f190d18fdb4a6f829d28aeb7&chksm=9f38e133a84f6825a8a30eb36bca4f8e7d0ddf969b131aa138fc4271953609fdf51caaed5e80&scene=21#wechat_redirect)
[Spark SQL用UDF实现按列特征重分区](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485842&idx=1&sn=cf7331c3a082a141366e5771640c5e52&chksm=9f38eabaa84f63acec983e9e15691b01239349f568bd8cb9725fbc42a01aa721e97145f8622c&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
