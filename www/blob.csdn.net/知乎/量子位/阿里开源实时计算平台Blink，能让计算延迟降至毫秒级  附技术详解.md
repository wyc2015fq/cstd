# 阿里开源实时计算平台Blink，能让计算延迟降至毫秒级 | 附技术详解 - 知乎
# 



> 雷刚 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

阿里巴巴这份开源礼物，业内期待已久。


近期，中国科技互联网巨头正式宣布将实时计算平台Blink开源，该技术由开源的Flink改造而来，被广泛应用在阿里众多核心业务中。

比如淘宝天猫智能推荐系统、阿里妈妈广告、高德地图、城市大脑等，均有实时计算平台Blink的支持。

这一技术甚至被认为是引领“下一代计算”的 “计算王牌”。

之前阿里透露开源意向时，就已经在开源和开发者群体中引发期待。

## **实时计算**

所谓实时计算，通俗来讲，即能够基于实时变化数据更新大数据处理的状态和结果。

最典型的如天猫双11抢购中，即便在万千人同时涌入的第一分钟，也能保证每个人点开App瞬间实现千人千面的智能推荐。



![](https://pic4.zhimg.com/v2-f88bad0f9c22675def6f17cfd3d0496f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='359'></svg>)



这背后正是阿里云实时计算平台Blink保驾护航的结果。


此外，对于众多AI项目来说，实时计算也关系到实时机器学习和实时测试的效果。



![](https://pic4.zhimg.com/v2-79d12e63a6f8760bc20a287b65d21ae7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='359'></svg>)



在阿里此前的探索中，归纳总结出三方面挑战：

- 业务庞大，场景多，大量的机器学习需求，这些因素一起导致了计算逻辑十分复杂；
- 数据量大，作业多，因此整个实时计算的机器规模十分巨大；
- 要保障低延迟和数据精确性，同时要满足高吞吐量的需求。

## **Blink方案**

为了应对上述挑战，阿里内部在调研诸多计算框架后，最终选定了Flink。

首先，Flink很好地引入和设计了State，基于State复杂的逻辑计算如join能得到很好的描述。

其次，Flink引入了Chandy-Lamport 算法，在此算法的支撑下可以完美实现Exactly-Once，并能在低延迟下实现高吞吐量。

值得一提的是，为了解决Flink在State、Chandy-Lamport 算法等方面的缺陷，阿里进一步结合Improvement，打造了Blink项目。



![](https://pic4.zhimg.com/v2-9ddeb0d1c23a3b16fc01abea1e7e46bf_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='358'></svg>)



该项目可以分为2大板块：


**1、BlinkRuntime**

包括存储、调度和计算，不同公司在使用Flink时，存储、调度以及底层优化等方面会有诸多不同，阿里巴巴的blink内部也对Runtime做了诸多个性化的优化。

**2、Flink SQL**

原生的Flink只有比较底层的DataStream API，用户在使用时需要设计实现大量的代码，此外DataStream本身也有设计上的缺陷。

为了方便用户使用，阿里巴巴团队设计了流计算的Flink SQL并推回了社区，取名Flink SQL。

此外，作为开源版Blink，跟社区版本还有几点差异：

第一是稳定性方面，在某些场景会比社区版本更加稳定，特别是在大规模场景；

另外Blink拥有全新的 Flink SQL 技术栈，它在功能上，特别是在批处理的功能上比社区版本强大很多。

它支持现在标准 SQL 几乎所有的语法和语义，在性能上，无论是在流式 SQL 还是批 SQL，当前版本在性能上都有很大的优势。

特别是在批 SQL 的性能方面，当前 Blink 版本是社区版本性能的 10 倍以上，跟 Spark 相比，在 TPCDS 这样的场景 Blink 的性能也能达到 3 倍以上。

阿里方面强调，如果用户对批处理或者对 SQL 有着比较强的需求，这个版本会用户可以得到很多好处。

（更多详细技术解析详见传送门）

## **实战效果**

阿里方面还介绍说，现在，阿里集团的全部核心业务已经用上Blink。

除了技术大考双11之外，ET城市大脑实时计算着杭州1300个信号灯路口、4500路视频，保障着交通动脉的通畅；淘宝、天猫每天为数亿用户展现实时的“专属”页面。

未来，这项技术还会被应用到社会的各个场景：工厂设备的物联网设备能实时分析数据、提高生产良品率；物流派送平台能第一时间下单，把商品及时送到消费者手中；导航软件时刻紧随出行轨迹，不再错过转弯口……

而面向业界开源，更是能将更前沿的计算技术让更多开发者受益其中。

## **传送门**

Blink on GitHub：

[https://github.com/apache/flink/tree/blink](https://link.zhihu.com/?target=https%3A//github.com/apache/flink/tree/blink)

技术详解：

[一文揭秘阿里实时计算Blink核心技术](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzOTU0NTQ0MA%3D%3D%26mid%3D2247486824%26idx%3D1%26sn%3D3ac64f02c3bd35573e2889c42f053980%26chksm%3De9293067de5eb97161b24df25d2b9ac0efbe30706ea429781c5460e4c1a515b55d9a0766dc53%26scene%3D21%23wechat_redirect)

— **完** —
量子位 · QbitAI
վ'ᴗ' ի 追踪AI技术和产品新动态
戳右上角「+关注」获取最新资讯↗↗
如果喜欢，请分享or点赞吧~比心❤






