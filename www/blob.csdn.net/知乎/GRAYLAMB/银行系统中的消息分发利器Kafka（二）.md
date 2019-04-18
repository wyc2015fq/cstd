# 银行系统中的消息分发利器Kafka（二） - 知乎
# 

建议从头阅读：[GRAYLAMB：银行系统中的消息分发利器Kafka（一）](https://zhuanlan.zhihu.com/p/54160439)

Kafka里有一些重要的概念：
![](https://pic3.zhimg.com/v2-900feb7c86f1d728f4c4318dac4d2706_b.jpg)
下面我们来一一解释一下：

1、Producer

前面已经介绍过，Producer是向Kafka传递数据的Application。

这里需要说明的是，Producer向Kafka传递的数据是一个一个小的消息数据（可能就数个字节），而不是一个很大的数据文件。

比如我们要把一个文件传递给Kafka，我们首先创建一个Producer，然后通过Producer将文件中的每一行数据作为一个消息发给Kafka，这样文件里的信息就都传递给Kafka了，但每个消息可能只有数个字节。
![](https://pic3.zhimg.com/v2-769d129e53c239d84b5599aa1ea8a602_b.jpg)
再比如我们想把一个数据表的查询结果给Kafka，那我就先做一个表查询，然后把查询到的每一行查询结果，作为一个消息送给Kafka。
![](https://pic1.zhimg.com/v2-321d8083743f47c74d443292b5113480_b.jpg)
2、Consumer

Consumer是从Kafka获取数据的Application。

Producer提供数据，Consumer获取数据，这就是一个闭环的流程。但需要注意的是，Producer并不是把数据直接送到Consumer的地址，而是先送到Kafka存下来，而Consumer作为获取方也是从Kafka拿数据。而且Consumer可以从Kafka获取到任何Producer产生的数据。（注：当然，Kafka内是有授权管理的，Consumer只能获取到它有权访问的数据）
![](https://pic2.zhimg.com/v2-2678d74ed6847fa88b1e1ac7a53b5c59_b.jpg)
好，综合起来看一下：

如果我们有一个文件要分发，那第一步是通过Producer将文件中的数据发送给Kafka存起来，第二步是Consumer向Kafka发送获取数据的请求（request），然后Kafka会根据Consumer的请求向Consumer传递它所要的数据（注：上面讲过，这些数据都是每一个数个字节大小的消息数据）。第三步Consumer会针对拿到的数据进行接下来的处理，同时，如有需要，会进一步request更多消息数据。
![](https://pic2.zhimg.com/v2-6e1cfb3f59b252cb3e863dc8d75588f1_b.jpg)
3、Broker

Broker就是一个Kafka Server的学名～

这个名字也是很贴切的，因为Kafka所扮演的角色就是一个Producer和Consumer之间的Broker（消息中介）。Producer和Consumer并不直接通讯，而是靠这个“中介”交换信息。
![](https://pic3.zhimg.com/v2-62fc57872165de40543780d2c721d662_b.jpg)
4、Cluster

Cluster就是集群，在分布式架构中，集群是很多负责同样事务的服务器的统称。Kafka采用的就是分布式架构，所以里面会有很多的Broker，他们组成了Cluster。
![](https://pic2.zhimg.com/v2-48667cc9075b93a01b84640a5ae12525_b.jpg)
5、Topic

这个比较有意思，咱们来模拟一下当Consumer向Kafka Broker获取数据时的情景：

Consumer：你能给我一些消息数据么？

Broker：我从n多的Producer那里收集数据，你要哪个数据？

Consumer：你给我3号Producer的数据吧……

Broker：3号Producer给了我3种不同的数据哦，你要哪一种？？

Consumer：给我销售数据吧……

Broker：呃，除了3号Producer，还有两个Producer也给了我销售数据，你要么？

Consumer：晕～

于是Topic就出现了。在Kafka中，Topic就是用来定位数据的，是一类数据的名字。

比如我们建立一个叫做GlobalOrders的Topic，这时我们有很多销售网点要把Order数据送给Kafka，不同的销售网点就是一个Producer。那我们就把这些Producer的数据都标记为GlobalOrders数据，当Consumer来调用时，只要告诉Broker我要GlobalOrders数据，就可以把这个类型的数据都拿到了。
![](https://pic1.zhimg.com/v2-83ec6411367df2e8f19ffa5c2dcd48ec_b.jpg)
总结一下，这次我们介绍了Kafka的Producer、Consumer、Broker、Cluster、Topic。下次再进一步介绍剩下的概念。
![](https://pic4.zhimg.com/v2-01cdb54b14088b5977e10b275943d877_b.jpg)

