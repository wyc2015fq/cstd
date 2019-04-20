# Flink迭代操作末文-迭代流 - Spark高级玩法 - CSDN博客
2019年01月22日 22:30:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：131
Flink作为优秀的流处理框架，自有其独到之处，比如我们前面讲了：
[flink的神奇分流器-sideoutput](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486266&idx=1&sn=d9e5b585ea70a1715514f621b4acda11&chksm=9f38e812a84f6104609912a96a41c80259ad2c4dab24303c667e6de85e4133ea7ed218e617e7&scene=21#wechat_redirect)
这个可以用来分流，很方便的一次就可以对数据进行筛选返回。
还有针对算法处理的迭代操作，我们已经讲过两篇文章了：
[Flink特异的迭代操作-bulkIteration](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486277&idx=1&sn=f6079800fea73c5f4eb5cd68d77e30a7&chksm=9f38e86da84f617b19911dc44cd175ca2400f1cb9c44d9cc7e8a5e38f8d6cb5c5eae2de10197&scene=21#wechat_redirect)
[不得不会的Flink Dataset的DeltaI迭代操作](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486296&idx=1&sn=e946bb9a42559613711f6a381ee3215c&chksm=9f38e870a84f6166d319668f0fb0ec50ce0af1b87e920c26836d4e74079fec3ad27c8e5589ed&scene=21#wechat_redirect)
一个是全量迭代，一个是增量迭代。
还有优秀又鸡肋的watermark机制
[不懂watermark？来吧～](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486324&idx=1&sn=b1132367e26e6da1c1982de4af436e91&chksm=9f38e85ca84f614a201eb20c5b365cfd9e93b2c2b93f1906a12181e8720afc7d0541309c1dc4&scene=21#wechat_redirect)
对于迭代操作，其实还有一讲，那就是流处理的迭代操作。那么本文就针对这个进行分析～
Flink的迭代流程序实际就是实现了一个步进函数，然后将其嵌入到IterativeStream内部。要知道Flink的Datastream正常情况下是不会结束的，所以也没有所谓的最大迭代次数。这种情况下，你需要自己指定哪个类型的数据需要回流去继续迭代，哪个类型的数据继续向下传输，这个分流的方式有两种：split和filter，官方网站在介绍迭代流的时候使用的是filter。我们这里就先按照官网的介绍走，然后案例展示的时候使用split给大家做个demo。
首先，要创建一个IterativeStream
IterativeStream<Integer> iteration =input.iterate();
接着就可以定义对该留要进行的逻辑操作，官网这里就很简单的举了一个map的例子。
DataStream<Integer> iterationBody =iteration.map(/* this is executed many times */);
调用IterativeStream的closeWith(feedbackStream)方法可以对迭代流进行闭环操作。传递给closeWith函数的DataStream会返回值迭代的头部。常用的做法是用filter来分离流的向后迭代的部分和向前传递的部分。。
iteration.closeWith(iterationBody.filter(/*one part of the stream */));
DataStream<Integer> output =iterationBody.filter(/* some other part of the stream */);
官方给了一个连续不断减1直到数据为零的例子：
DataStream<Long> someIntegers =env.generateSequence(0, 1000);
// 创建迭代流
IterativeStream<Long> iteration =someIntegers.iterate();
// 增加处理逻辑，对元素执行减一操作。
DataStream<Long> minusOne =iteration.map(new MapFunction<Long, Long>() {
@Override
public Long map(Long value)throws Exception {
return value - 1 ;
  }
});
// 获取要进行迭代的流，
DataStream<Long> stillGreaterThanZero= minusOne.filter(new FilterFunction<Long>() {
@Override
publicbooleanfilter(Long value)throws Exception {
return (value > 0);
  }
});
// 对需要迭代的流形成一个闭环
iteration.closeWith(stillGreaterThanZero);
// 小于等于0的数据继续向前传输
DataStream<Long> lessThanZero =minusOne.filter(new FilterFunction<Long>() {
@Override
publicbooleanfilter(Long value)throws Exception {
return (value <= 0);
  }
});
刚刚分享了一套kylin课程，欢迎与490+好友一起学习
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640?wx_fmt=jpeg)
