# 解惑:这个SPARK任务是数据倾斜了吗？ - Spark高级玩法 - CSDN博客
2018年11月08日 05:49:27[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：72
**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUia1OswCGbUiba1S63osFLJGw7gxweQ5aibjKxxGUBbUgupYclPCaRkjkH8pyPFhlgKRkuFMYK0aH0Q/640)**
**健身前后对比**
**健身**回来的路上，看到微信群里聊技术，一群有问了一个神奇的问题，具体可以看如下截图：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUia1OswCGbUiba1S63osFLJGK3cCvdgUPfJnV170kA3DjxJ7CrONibFSoAXFCFafyOTAqOhJWVbPNhg/640)
哥们给出的结论是repartition导致的数据倾斜，我给他详细的回复了说明了不是数据倾斜。那么接下来，我们就仔细分析一下原因。
**为了大家更彻底的了解这块内容，文章底部浪尖也录制了一个小视频。**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUia1OswCGbUiba1S63osFLJGrTgAicVncUOmCZwicj0N91Mw9GcpVhB6MsK0mJtzwwjAOnr3m9nBGbcg/640)
那哥们数是repartition导致的数据倾斜原因，是由于前三行数据输入和输出都是好几百兆，而后面的都是只有几个MB的输入，0B输出，所以下结论是数据倾斜。
浪尖纠正他是错的原因是数据倾斜往往指的是同一个stage内部：**有的task数据量大，有的task数据量小，task间数据量大小差距比较大，而这个明显不是**。这个是executor的页面，可以看complete task列，会发现前三行占据了几乎所有task执行，完成的task数是其余的十几二十倍。这个就是导致前三行输入输出数据量比较大的原因。
数据本地性是导致这个问题的根本原因。由于数据本地性task调度会优先调度到数据所在的executor机器，假如机器executor存在执行中的task会等待一个时间，在这个时间内task执行完，新task会直接调度到该executor上。如此往复，导致executor处理的task差距比较大。
官网给出了关于spark调度task的时候数据本地性降级的等待时间配置。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUia1OswCGbUiba1S63osFLJGXTpiaSJXiaB2kbk3DtMjOys1gpeZlbkrzGogf79QwBnlpj75SQctHqSg/640)
很简单，将3s设置为0s，然后结果就是task不会等待数据本性降级，就立即调度执行。
**其实，根源还是kafka 创建topic的时候 partition数目没有够。**单个parition的吞吐量是可以达到数万qps，但是结合业务逻辑，不同的数据输出位置，吞吐量会急剧下降，所以topic分区数，应该根据处理逻辑和落地位置，磁盘数，综合考虑设置。
还有一点就是分析问题不准确，关于spark streaming性能瓶颈分析和解决方法，浪尖在知识星球里做了详细的分析，建议大家加入知识星球。
加入知识星球二维码
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640)
关于知识星球能学到啥可以阅读
[深入系统掌握大数据](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485594&idx=1&sn=59cafe4f953f17c79299cf8c1797d4b8&chksm=9f38ebb2a84f62a4c854e26f814dd00c6f91dbaa4a52813aa8c3f05140d751eb128a733ae418&scene=21#wechat_redirect)
微信群，可以加浪尖微信
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWutRWiaJP6pJLicbAdU9zDmXR7Udmo7TGaR7V7XkSO57VHaKkxyJdqay8UkwyMoibw7J1KDJHgKkM5Q/640)
点个赞吧
