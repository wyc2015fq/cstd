# 不懂watermark？来吧～ - Spark高级玩法 - CSDN博客
2019年01月18日 22:37:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：215
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXLaTT80RMqf88anZVQEsuImEwRCOOM1Qp1dJcJ7CyebT2hOYbcBnWk0ZUDYcicZWXzHbdib5RbhOpg/640?wx_fmt=png)*经常有球友或者微信群友反馈自己搞不明白watermark怎么回事儿，所以今天浪尖准备发文详细说明一下。*
*首先，拿基于窗口的计算来说吧，窗口的大小 size和滑动间隔 slide，都是基于时间维度处理的，像Spark Streaming就是基于处理时间，也即是处理任务所在机器的本地时间，用这个时间处理数据我们自然无法关注事件是否在时间维度上乱序，是否是滞后数据，那么为了保证数据有序和处理滞后数据就不能用处理时间进行处理。*
*幸运的事情是往往我们采集数据的时候往往会给数据打上一个采集时间，那么我们在后面流处理的时候假如窗口大小 size和滑动间隔 slide 是基于这个时间的话，那么我们就可以感知到事件是否在时间维度上有序，是否滞后。在这种情况下就引入了watermark，其作用一是驱动流计算向前执行，二是可以作为依据处理延迟数据，也即是可以允许数据有多大的延迟。*
*1. 时间概念*
从时间概念上来讲，对于流程序总共有三种时间概念：
- 
处理时间
- 
注入时间
- 
事件时间
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWJQ4nZCsvicMOsiay3aq74509zhMSGwxAGrolbI2PkCiazaMbWSYhX4OM6T6RTxnejythB3WMRQTjTA/640?wx_fmt=png)
其中，注入时间可以视为事件时间的特殊形式，但是要注意的事情是注入时间就不能处理无序事件和滞后事件，自然不能用watermark机制了。
**2. 事件时间和watermark**
支持事件时间的流处理器需要一种方法来测量事件时间的进展。例如，一个小时窗口windows的操作，当事件时间已经超过一个小时的时候需要通知该操作算子，以便操作算子可以关闭正在进行中的窗口。
例如在一个程序中，操作算子的当前事件时间可能稍微落后于处理时间(事件传输延迟导致的)。另外流程序可能只需要几秒钟的处理时间就可以处理事件跨度为几周的数据，比如通过快速处理一些已经缓存在kafka topic(或者另外的消息队列)中的历史数据。
Flink中使用watermark去测量事件时间的进度。Watermark 作为数据流的一部分，携带一个时间戳 t。一个Watermark(t) 声明事件时间已经到达时间t，意味着已经没有事件时间t1<t的元素在流中存在(也即时间的时间戳应该早于或者等于watermark)。如下图：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXLaTT80RMqf88anZVQEsuIfEenWPOfFClMubicKclglJK8pYgMHtVEDVmr1xZWBmXc8cGBiaSZ6VMA/640?wx_fmt=png)
Watermark对于无序流是至关重要的，如下图所示，事件不是根据时间戳排序。一般来说，watermark类似于deadline吧，到了这个点你还没到，那就pass掉了。也即是一旦watermark到达操作算子，操作算子就可以将内部时间设置为watermark的值，再来数据就会弃掉了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXLaTT80RMqf88anZVQEsuIHoycJFIKKVqwPVOKNh3XGzeC2UX2ZB0fHIzYaianhBV6culPxoQ9L0w/640?wx_fmt=png)
**3. 在并行流中的watermark**
对于flink来说，一般Watermark是在Source函数生成，当然也可以再后期的算子中生成，但是一定要在时间函数(主要是窗口函数)之前生成。一个source函数的每个并行子任务通常独立的产生watermark。这些watermark定义了特定并行source的事件时间。
当watermark流经流程序时，会调整操作算子中的事件时间至watermark到达的时间。每当操作算子更新它自己的事件时间时，它就会为后继的操作算子生成一个新的下行watermark。
一些操作算子会有多个输入流。例如，union操作或者keyBy(...)或partition(...)之后的操作.这些操作算子的当前事件时间是取决于所有输入流最小的事件时间。当输入流更新它们的事件时间时，操作算子也会更新。
下图显示了，流经并行流的事件和watermark，以及跟踪事件时间的运算符。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXLaTT80RMqf88anZVQEsuIlRIGSS3ib4zcJOdPWRZxFDmDVnic5HicYsOTbnFicAYImOdn2sCpibHogEw/640?wx_fmt=png)
**4. 迟滞元素**
也可能存在违反watermark条件的事件，也即在Watermark(t) 已经发生以后，会有时间戳t1<t的事件出现。事实上，在现实设置中，某些事件可能会有任意的延迟，使得所有元素在watermark之前准时到达变得不是很可能。**即使延迟是有界的，大尺度的延迟watermark也是不可取的，因为会导致事件时间窗口计算延迟很大，窗口会等到watermark时间戳到达才会刷写到sink的**。
比如，窗口一个小时，watermark设置的是一天，那结果就会在一天零一个小时候刷出去，这个明显不是我们要的，尤其是对实时性要求比较高的。那么这时候咋处理呢？星球里见～
**5. 空闲source**
目前，对于纯事件时间watermark生成器，如果没有要处理的事件，则watermark是不会生成并且下发的。 这意味着在输入数据存在间隙的情况下，事件时间将不会继续前进，例如窗口操作算子将不会被触发，因此现有窗口将不能产生任何输出数据。
为了避免这种情况，可以使用周期watermark分配器，它们不仅基于事件时间戳进行分配，还会在没有事件的时候产生空的watermark。 比如可以在长时间没有观测到事件流入的话，可以采用系统当前时间来生成watermark。
当然，可以使用SourceFunction.SourceContext #markAsTemporarilyIdle将源标记为空闲。 
**6. watermark处理机制**
前面说了，watarmark的作用和产生，那么watermark是如何被算子处理的呢？
通用的规则是操作算子需要在向下游转发watermark之前完全处理给定的watermark。 例如，WindowOperator将首先评估应该触发哪些窗口，并且只有在产生由watermark触发的所有输出之后，watermark本身才会被发送到下游。 换句话说，由于出现水印而产生的所有元素将在水印之前发出。
同样的规则适用于TwoInputStreamOperator。 但是，在这种情况下，操作算子的当前watermark被定义为其两个输入的最小值。
***7. 总结***
要注意watermark指定延迟的话进行不要太大，太大会导致结果刷新滞后太久。
数据上报有周期性间隔的话，要注意以心跳的形式发watermark，保证流程序继续前进。
**【懂了么？】**
**推荐阅读**
[Spark Streaming VS Flink](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486214&idx=1&sn=8bd44797a23c9350c8884213e82c3830&chksm=9f38e82ea84f6138774af6bdbdd6e5332e8c98e97a5b0dea83de295c779026426ab458e05f2b&scene=21#wechat_redirect)
[Structured Streaming  VS Flink](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486237&idx=1&sn=427e452f78f725c535b450eb759173ea&chksm=9f38e835a84f6123c697e76bf4a3a2e77ddb09af433e958f1206a2aba897608675eccb973f93&scene=21#wechat_redirect)
[结合Spark讲一下Flink的runtime](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485964&idx=1&sn=224f6cc89d9e824055673f5ac313d28b&chksm=9f38e924a84f603203311d7c561bfcb3d5fca7241adf3540c4b50e0efca63040add04b2b64a1&scene=21#wechat_redirect)
[不得不会的Flink Dataset的Delta 迭代操作](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486296&idx=1&sn=e946bb9a42559613711f6a381ee3215c&chksm=9f38e870a84f6166d319668f0fb0ec50ce0af1b87e920c26836d4e74079fec3ad27c8e5589ed&scene=21#wechat_redirect)
480+好友一起努力于2019～
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640?wx_fmt=jpeg)
