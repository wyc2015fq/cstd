# 基于Spark Streaming的僵尸主机检测算法 - BlueSky - CSDN博客
2015年11月18日 21:15:37[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：938
个人分类：[Spark Learning](https://blog.csdn.net/ASIA_kobe/article/category/5912555)
转自：http://www.tuicool.com/articles/7bUJBbV
#### 基于Spark Streaming的僵尸主机检测算法
张蕾 李井泉 曲武 白涛
僵尸网络通过多类传播和感染程序，构建一个可一对多控制的网络，操控大量僵尸主机发起DDoS攻击、发送垃圾邮件、偷窃敏感数据和钓鱼等恶意行为。基于一种分布式实时处理框架，提出一种分布式的僵尸主机检测算法，该算法能够充分利用网络流量的统计数据IPFIX，在无需深度包解析的情况下，能够识别僵尸主机行为。同时，使用该算法实现了IPFIXScanner原型系统，系统的鲁棒性和可扩展性是设计该系统的核心原则。实验表明，IPFIXScanner原型在使用指定僵尸家族样本训练的情况下，对于特定类的僵尸主机能够获得较高的检出率和较低的误报率。在核心交换机上的测试结果表明，IPFIXScanner能够进行分布式的实时检测，加速比接近线性，验证了Spark
 Streaming引擎在分布式流处理方面的优势，以及用于僵尸主机检测方面的可行性。
[基于Spark Streaming的僵尸主机检测算法](http://www.chinacloud.cn/upload/2015-11/15110509113241.pdf)
