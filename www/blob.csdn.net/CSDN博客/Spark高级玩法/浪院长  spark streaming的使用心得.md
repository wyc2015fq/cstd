# 浪院长 | spark streaming的使用心得 - Spark高级玩法 - CSDN博客
2018年09月08日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：9565
今天，主要想聊聊spark streaming的使用心得。
**1，基本使用**
主要是转换算子，action，和状态算子，这些其实，就按照api手册或者源码里接口介绍结合业务来编码。
其实，想用好spark streaming 掌握spark core，spark rpc，spark 任务调度，spark 并行度等原理还非常有必要。
**2，中间状态缓存**
说到中间算子大家肯定都会想到UpdateStateByKey等状态。里面很多注意事项，比如顺序性，key的超时机制维护。这个适合数据量不多，尤其是key的维度不多，value不大的情况。
当然数据量上来了，要想维护中间状态怎么办？其实这个时候肯定是第三方存储，比如redis，alluxio。redis更适合那种key带超时机制的，并且数据量肯定不能过大。而alluxio就很适合那种高吞吐量的，比如去重统计。
**3，结果输出**
direct streaming能保证仅一次处理，但是要求输出存储支持密等性，或者主动将结果更改为存在更新不存在插入。当然，如果外部存储系统支持事务那就更嗨，能实现恰一次处理。
实际上在offset维护这个层面上，spark streaming 不同版本于kafka不同版本结合实现有很大不同。
**4，监控告警及故障自动恢复**
我觉得对于监控告警及故障自动恢复，重要程度不亚于业务场景。因为再好的业务实现，架不住系统挂掉你不知道。因为你总不能二十四小时盯着系统。而且很多公司对故障自动恢复都有kpi，比如3min，人工去检测故障并恢复不太可能，需要自己实现一套监控系统。
**5，调优**
调优对于spark streaming非常重要，因为一个批次处理延迟就会导致job堆积，结果输出延迟，深圳任务挂掉数据丢失。调优其实最注重对spark 原理把控，数据量的了解及资源和数据的关系。
**6，源码**
源码阅读，为了帮助大家更透彻的理解原理。主要会分三块：
spark streaming 与kafka-0.8.2 direct stream。
spark streaming 与kafka-0.8.2 receiver based stream。
spark streaming 与kafka-0.10.2 direct api。
这些内容比较多，本周日**晚上八点到十点**浪尖准备搞个**qq直播**，对这些内容有兴趣的小伙伴可以扫码参团，费用不高，算是对浪尖创作的支持吧。
当然，准备是两到三次，每次两个小时，实际次数看效率。
参与直播的直接联系微信 **158570986**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFViaZrvyvwFlVxZHunGoicxSHFGQPXoaQ7aZYWXrEpO27oCJUQpgYdjoqt16lKObpx5h1iaCnQ6ObUJQ/640?wx_fmt=png)
当然，要是喜欢浪尖，更希望加入浪尖知识星球，球友可以免费参与直播。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
