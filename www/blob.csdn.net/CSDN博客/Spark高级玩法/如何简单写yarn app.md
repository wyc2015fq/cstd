# 如何简单写yarn app - Spark高级玩法 - CSDN博客
2018年08月31日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：91
尽管YARN自带的编程API已经得到了极大的简化，但从头开发一个YARN应用程序仍是一件非常困难的事情。在YARN上编写一个应用程序，你需要开发Client和ApplicationMaster两个模块，并了解涉及到的几个协议的若干API和参数列表，其中ApplicationMaster还要负责资源申请，任务调度、容错等，总之，整个过程非常复杂。
Apache Twill(http://twill.apache.org/index.html)这个项目则是为简化YARN上应用程序开发而成立的项目，该项目把与YARN相关的重复性的工作封装成库，使得用户可以专注于自己的应用程序逻辑。
下面代码示例是使用Apache Twill开发一个运行在YARN上的helloworld程序：
publicclassHelloWorld{
static Logger LOG = LoggerFactory.getLogger(HelloWorld.class);
staticclassHelloWorldRunnableextendsAbstractTwillRunnable{
@Override
publicvoidrun(){
 LOG.info("Hello World");
 }
}
publicstaticvoidmain(String[] args)throws Exception {
 YarnConfiguration conf = new YarnConfiguration();
 TwillRunnerService runner = new YarnTwillRunnerService(conf, "localhost:2181");
 runner.startAndWait();
 TwillController controller = runner.prepare(new HelloWorldRunnable()) .start();
 Services.getCompletionFuture(controller).get();
}
Twill所做的事情是开发了一个通用的Client和ApplicationMaster实现，并提供了一套编程API，使得用户可直接实现这套API便可以编写运行在YARN上的分布式程序。Twill应用程序运行在YARN上的流程跟普通程序一样，流程图下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWQv9BOTqtEUlhDnn4WOGMDB2qAichvtXPWCVD4g9aJu1Lq4ibBFEJ1V6AoCGpnzShty41FnQpKRULg/640?wx_fmt=png)
为极大地方便用户编写运行在YARN上的分布式应用程序，Twill做了大量通用模块和特性，包括：
（1）实时日志收集
基于kafka实现了日志收集功能，可将应用程序产生的日志实时收集起来，便于集中化分析和展示。
（2）资源汇报
可实时收集任务占用的各类资源，包括cpu利用率，物理内存等。
（3）资源弹性扩展
可动态改变应用程序占用的资源，比如将container数目在线由5个增长为10个。
（4）状态恢复
当客户端失去与应用程序链接后，可通过API重构一个新的客户端，重新获取状态。
（5）服务发现
应用程序可向zookeeper注册一些信息，便于其他服务或者客户端发现该应用程序的位置，以便与之通信。
随着YARN越来越成熟，它必将成为一个基础核心系统，届时与Apache Twill类似的项目显得尤为重要。
转载自董的博客:http://dongxicheng.org/mapreduce-nextgen/apache-twill-for-yarn/
twill官方demo，可以参看：https://github.com/apache/twill/tree/master/twill-examples
推荐阅读：
[hadoop系列之深入优化](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483973&idx=1&sn=fd4a53a88c1998b2db93e0537e35583f&chksm=9f38e16da84f687bbfde2211ce7c3654c34f9abc730fdde198c9328ef8fc355dd302a35e525f&scene=21#wechat_redirect)
[经验|如何设置Spark资源](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485250&idx=1&sn=f9a8a27d6b966fb53725505072f50f8f&chksm=9f38e46aa84f6d7ca50bec7c593f73ddc064af6e598cde4ea9ed85b91aef89e0f55065c4bad2&scene=21#wechat_redirect)
[Hive鲜为人知的宝石-Hooks](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485243&idx=1&sn=c7f92678f69f282a4047dd2291d95d28&chksm=9f38e413a84f6d05f107ac4db6e9dd712dbfd1f6302c3ee8171c35f605b8fcfc099111ce0a65&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
