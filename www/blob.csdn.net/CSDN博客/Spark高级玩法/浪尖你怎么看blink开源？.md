# 浪尖你怎么看blink开源？ - Spark高级玩法 - CSDN博客
2019年01月28日 17:33:12[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：51
有微信群友和星球球友问浪尖怎么看待blink开源。
网上看到有祝兄跟浪尖一样是spark支持者，不同的事情是浪尖对flink也比较熟悉，也挺喜欢用flink的。
浪尖前面有分享过flink 与 Spark对比
[Spark Streaming VS Flink](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486214&idx=1&sn=8bd44797a23c9350c8884213e82c3830&chksm=9f38e82ea84f6138774af6bdbdd6e5332e8c98e97a5b0dea83de295c779026426ab458e05f2b&scene=21#wechat_redirect)
[Structured Streaming VS Flink](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486237&idx=1&sn=427e452f78f725c535b450eb759173ea&chksm=9f38e835a84f6123c697e76bf4a3a2e77ddb09af433e958f1206a2aba897608675eccb973f93&scene=21#wechat_redirect)
这都是功能性对比，浪尖没有总结，浪尖个人觉得单纯的从流处理来说，spark 确是比flink稍微弱了一点，尤其是，任务大，数据小的时候，这是由spark微批处理的特性决定的，每个批次都会产生task进行调度（序列化反序列化、jar传输等）引起性能开销，而flink task是常驻的，数据在task之间流动，数据量小数据移动成本明显会低于task调度成本。这不是说flink不能处理大数据，要知道实时处理数据不会落地，数据本地性可以用的地方少，数据一直在传输。
但是flink和spark 两者我从功能上来说可以相互替代吧，尤其是窗口处理，大家都变成了批处理，这个点区别不大。但是flink状态维护机制确实强于spark，比如统计一天从凌晨到当前时间的pv，uv这个用flink实时统计很简单 ，spark需要借助第三方存储实现灵活的状态管理。还有就是join操作等flink也是要优秀点。
但是一旦用第三方存储维护状态，故障恢复就会是个大难题。spark故障恢复机制相对若于flink，前面浪尖也分享过了spark的checkpoint和flink checkpoint机制的区别，flink checkpoint确实是分布式快照机制，而spark主要是driver端的故障恢复。
[flink超越Spark的Checkpoint机制](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485181&idx=1&sn=05623930a6bcb09596d1a252d440fd4e&chksm=9f38e5d5a84f6cc3c56ae158a005dbe9122f16a6d27782a682bf1c9c5f1ac9f022d75c89a95b&scene=21#wechat_redirect)
[必会:关于SparkStreamingcheckpoint那些事儿](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485157&idx=1&sn=6714f0579d230000336fac78fe9a730a&chksm=9f38e5cda84f6cdb2e924bcae2127f025cdef9757a611407eb2e028927d54a8ab02b752e5963&scene=21#wechat_redirect)
假如flink一直这么坚持优化的话，流处理确是会抢spark的一杯羹，而批处理我觉得spark还是很赞的，尤其是假如你能充分利用数据本地性的情况下。
接着再来看看contributors数，spark碾压flink，所以你用spark网上资料会是你强力的支持，用flink那么你要学习和解决一些flink自身已有的bug。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVlrgH0OCNtnVqFhYTS4sfjj4ZkCia8kV5dKUWzg6BiaGHO56j6eluMv5sIviamct3mnY2kNGrUdgXPQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVlrgH0OCNtnVqFhYTS4sfjkCBFccicSRtLp0ibQMAYDzdeQcP49BPJW6Rq3b7t2PbOEqp0bPdiahONw/640?wx_fmt=png)
blink开源我们应该持借鉴学习的态度，浪尖这边也是内部维护了flink分支，可以借鉴学习blink特点融合进来。
至于要不要用blink。建议持观望态度或者还是使用社区的然后边融合blink特性，因为要想想jstorm的前车之鉴。
假如后期合并结束，然后验证稳定，那么再使用也不晚。
当然，星球里急躁的小伙伴比较多，天天催着我上线flink教程，我这边虽然再加紧整理。但是工作，学习，健身，写文章都要时间，所以要点时间。
还有尤其是新手我觉得没必要急于去学习flink，别因为最近blink在吹风就忘了自己有几斤几两，尤其是年后跳槽的，别指望过年期间熟悉一下flink就会加分很多，其他技术底子薄弱，会点flink也不会给自己增光添彩。过年期间还是要刷java基础，各种官网和权威指南，扫盲点，这样自己完善了现有的体系，年后再扩展一下小小flink，就是如虎添翼，否则就是自己给自己埋坑了。
星球里会陆续分享flink的技术文章及学习教程，欢迎加入～
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
500位好友了～～
