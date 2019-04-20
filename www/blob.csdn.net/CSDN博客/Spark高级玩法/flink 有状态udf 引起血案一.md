# flink 有状态udf 引起血案一 - Spark高级玩法 - CSDN博客
2018年10月27日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：7444
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU4MTjGPjfQKmDpSXFBaOdjeMicjE6WibZiapS8WhJm68VJwyZFdejLmNHMVZkiaYa7QZu1ibXka8pByVQ/640)
**场景**
最近在做一个画像的任务，sql实现的，其中有一个udf，会做很多事情，包括将从redis读出历史值加权，并将中间结果和加权后的结果更新到redis。
大家都知道，flink 是可以支持事件处理的，也就是可以没有时间的概念，那么在聚合，join等操作的时候，flink内部会维护一个状态，假如此时你也用redis维护了历史状态，也即是类似 result = currentState（flink）+lastState（redis），且此时要针对计算的结果用where进行筛选.
**SQL如下**
CREATEVIEW view_count AS
select
`time`,
  gid,
  cid,
count(feed_id) * 1as strength
FROM
  view_cid
GROUPBY
  gid,
  cid,`time`;
CREATEVIEW view_strength ASselect
`time`,
  gid,
  cid ,
  Get_Strength_Weaken(gid, cid, cast(strength asdouble), `time`, 0.95)  as`result`
FROM
  view_count
;
insertinto
  hx_app_server_sink_common
SELECT
  gid,
  cid,
`result`
FROM
  view_strength
where`result` <> '0.0'
GROUPBY
  gid,
  cid,
`result`;
**业务分析**
第一个sql视图完成的是首先分组，然后统计某一个字段并乘以权重；
第二个sql视图，udf ：Get_Strength_Weaken完成当前值和历史值叠加工作，历史值存储在redis，同时将结果返回并更新redis，返回值作为result字段。
第三个sql在输出的时候，result字段作为了where的条件和group by里的字段。
这时候生成的flink概图如下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU4MTjGPjfQKmDpSXFBaOdjUXoic5kamVstqYF2Ndn55cjBP92OWGF03Eppb46TxWgVJFeU4bKaaKQ/640)
观察中间的结构图可以发现，Get_Strength_Weaken被调用两次：
1. where条件，这个的生成是由于第三条sql
where `result` <> '0.0'
产生的执行计划，是不是看起来很懵逼。。。
2. select里面还有一次调用Get_Strength_Weaken，这个很明显。
当然，可以打印一下flink udf里eval函数的调用细节日志，很容易发现重复调用的问题，浪院长这个也是通过分析日志，对比输出结果来得出的论。
综合上面分析和udf调用日志，结论就是udf被调用了两次。
对于这个flink的udf被多次调用引起的结果偏大，整整调试了一下午。
由于上面分析可以得出结论，flink将where条件下推了，where 条件判断会先执行，而select里后执行，那么可以调整SQL，如下：
CREATEVIEW view_count AS
select
`time`,
 gid,
 cid,
count(feed_id) * 1as strength
FROM
 view_cid
GROUPBY
 gid,
 cid,`time`;
CREATEVIEW view_strength ASselect
`time`,
 gid,
 cid ,
getResult(gid,cid) as`result`
FROM
 view_count
where Get_Strength_Weaken(gid, cid, cast(strength asdouble), `time`, 0.95)  as`result` <> '0.0'
;
insertinto
 hx_app_server_sink_common
SELECT
 gid,
 cid,
`result`
FROM
 view_strength
GROUPBY
 gid,
 cid,
`result`;
那么实际上，select里的udf主要目的是取出来计算结果，那么这个时候可以写个简单的udf--getResult，只让他从redis获取 where条件里更新到redis里的结果，由于该udf是无状态的即使多次调用，也无所谓。
所以，总结一下，对于flink 来说，由于基于事件的处理，聚合、join等操作会有状态缓存，那么此时再用到含有外部存储状态的udf，一定要慎重，结合执行计划，来合理放置udf的位置，避免出错。
当然，调试阶段最好是有详细的日志，便于分析和定位问题。
**flink 状态删除**
其实，flink聚合等内部状态有配置可以使其自动删除的，具体配置使用如下：
```
StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();
StreamTableEnvironment tableEnv = TableEnvironment.getTableEnvironment(env);
// obtain query configuration from TableEnvironment
StreamQueryConfig qConfig = tableEnv.queryConfig();
// set query parameters
qConfig.withIdleStateRetentionTime(Time.hours(12));
// define query
Table result = ...
// create TableSink
TableSink<Row> sink = ...
// emit result Table via a TableSink
result.writeToSink(sink, qConfig);
// convert result Table into a DataStream<Row>
DataStream<Row> stream = tableEnv.toAppendStream(result, Row.class, qConfig);
```
**[完]**
**推荐阅读：**
[推荐两个不错的flink项目](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485652&idx=1&sn=170ece64334979fc5ea5fc5628a33759&chksm=9f38ebfca84f62ea0c1aaaeb8410ec15aec31b1122558662175aa70a2d953f9a3f0e6e904c8f&scene=21#wechat_redirect)
[Spark SQL从入门到精通](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485504&idx=1&sn=e19460f43aa2fe388140a2241e9ea219&chksm=9f38eb68a84f627ee0b6dac115f0684dddba573be04ae7e4e54d17c084cf1946282f81b1fd43&scene=21#wechat_redirect)
[重要 : 优化flink的四种方式](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485344&idx=1&sn=cde8915c6983dcfe541043cd6db8dbe6&chksm=9f38e488a84f6d9eb7b961a2c4fa05f2bd85fa82df81e4d8e56d0c22d86ff413a0baaacea01e&scene=21#wechat_redirect)
[flink超越Spark的Checkpoint机制](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485181&idx=1&sn=05623930a6bcb09596d1a252d440fd4e&chksm=9f38e5d5a84f6cc3c56ae158a005dbe9122f16a6d27782a682bf1c9c5f1ac9f022d75c89a95b&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
