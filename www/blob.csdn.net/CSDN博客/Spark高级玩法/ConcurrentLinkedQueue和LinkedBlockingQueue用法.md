# ConcurrentLinkedQueue和LinkedBlockingQueue用法 - Spark高级玩法 - CSDN博客
2018年04月06日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：70
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTLVFC6HNYqFs0xGxCkIV2kFuDXhud9ZrO7aqaKaQDhxLib9JjANs61sg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
## 作者： Ruthless 关注 - 28 粉丝 - 2969
在Java多线程应用中，队列的使用率很高，多数生产消费模型的首选数据结构就是队列(先进先出)。Java提供的线程安全的Queue可以分为阻塞队列和非阻塞队列，其中阻塞队列的典型例子是BlockingQueue，非阻塞队列的典型例子是ConcurrentLinkedQueue，在实际应用中要根据实际需要选用阻塞队列或者非阻塞队列。
注：什么叫线程安全？这个首先要明确。线程安全就是说多线程访问同一代码，不会产生不确定的结果。
**并行和并发区别**
1、并行是指两者同时执行一件事，比如赛跑，两个人都在不停的往前跑；
2、并发是指资源有限的情况下，两者交替轮流使用资源，比如一段路(单核CPU资源)同时只能过一个人，A走一段后，让给B，B用完继续给A ，交替使用，目的是提高效率
**LinkedBlockingQueue**
**由于LinkedBlockingQueue实现是线程安全的，实现了先进先出等特性，是作为生产者消费者的首选**，LinkedBlockingQueue 可以指定容量，也可以不指定，不指定的话，默认最大是Integer.MAX_VALUE，其中主要用到put和take方法，put方法在队列满的时候会阻塞直到有队列成员被消费，take方法在队列空的时候会阻塞，直到有队列成员被放进来。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTnd6oeXzE21fdLHhDOhKER9r1pMescEXdopKQkUcOnCQN0B9MIcCcjg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CT1XN1mkJzkYaObGpwGZGXnWVwbGG8OCBMGaY98CK7o5ks4ibicq7poRJw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTlliczk1n6k3hvGULkV9uvNs33lUbZLJ2B0VDrbqVFIR8qHuCBzKJsbA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTwfvUzSg6NsZvGTUMwEoQtoNU5kTP5MVyGeDiap6SU6n18Icj3kEvibLQ/640?wx_fmt=png)
**ConcurrentLinkedQueue**
ConcurrentLinkedQueue是Queue的一个安全实现．Queue中元素按FIFO原则进行排序．采用CAS操作，来保证元素的一致性。
LinkedBlockingQueue是一个线程安全的阻塞队列，它实现了BlockingQueue接口，BlockingQueue接口继承自java.util.Queue接口，并在这个接口的基础上增加了take和put方法，这两个方法正是队列操作的阻塞版本。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTRTEvmdtRNtRlKAJYOGEUr2ZoCSmGXVSicQJzSwiauUtsCCaVrjvqcg5A/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTIqH8mBBzwfRX1k0D57DGaOJZI8WzjzNUeIpPcPhmflayjE1Ultw9GQ/640?wx_fmt=png)
**运行结果：**
**costtime 2360ms**
**改用while (queue.size()>0)后**
**运行结果：**
**cost time 46422ms**
**结果居然相差那么大，看了下ConcurrentLinkedQueue的API原来.size()是要遍历一遍集合的，难怪那么慢，所以尽量要避免用size而改用isEmpty().**
总结了下， 在单位缺乏性能测试下，对自己的编程要求更加要严格，特别是在生产环境下更是要小心谨慎。
## **使用场景：**
适用阻塞队列的好处：多线程操作共同的队列时不需要额外的同步，另外就是队列会自动平衡负载，即那边（生产与消费两边）处理快了就会被阻塞掉，从而减少两边的处理速度差距，自动平衡负载这个特性就造成它能被用于多生产者队列，因为你生成多了（队列满了）你就要阻塞等着，直到消费者消费使队列不满你才可以继续生产。 当许多线程共享访问一个公共 collection 时，ConcurrentLinkedQueue 是一个恰当的选择。 
LinkedBlockingQueue 多用于任务队列（单线程发布任务，任务满了就停止等待阻塞，当任务被完成消费少了又开始负载 发布任务） 
ConcurrentLinkedQueue 多用于消息队列（多个线程发送消息，先随便发来，不计并发的-cas特点）
多个生产者，对于LBQ性能还算可以接受；但是多个消费者就不行了mainLoop需要一个timeout的机制，否则空转，cpu会飙升的。LBQ正好提供了timeout的接口，更方便使用 如果CLQ，那么我需要收到处理sleep
## 总结
单生产者，单消费者 用 LinkedBlockingqueue 
多生产者，单消费者 用 LinkedBlockingqueue 
单生产者 ，多消费者 用 ConcurrentLinkedQueue 
多生产者 ，多消费者 用 ConcurrentLinkedQueue 
文章参考自：
https://www.cnblogs.com/linjiqin/archive/2013/05/30/3108188.html
http://blog.csdn.net/jameshadoop/article/details/52729796
**推荐阅读：**
1，[Java动态代理原理及解析](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484556&idx=1&sn=7ef7d0de2216698eb5b5d8dbf08fdcb0&chksm=9f38e7a4a84f6eb23ffc5b6d6247b8b1e79df57ebbf02402f8a198243764677b0fbee680a70c&scene=21#wechat_redirect)
2，[必读：再讲Spark与kafka 0.8.2.1+整合](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484551&idx=1&sn=ee51a406c1fa975489b7f9758a9e8d2c&chksm=9f38e7afa84f6eb934bc8bd09b106db456b4146a663d4bff759c0e2f9c0a03f30c7479ece62b&scene=21#wechat_redirect)
3，[编程语言排行榜：你选对了吗？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484553&idx=1&sn=ff570c42f91c81ca7573283071dc2b43&chksm=9f38e7a1a84f6eb72cf45462de2274101045a6ad438b0a3388349d4c7313b12b43f183f14570&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
