# 对Netty的简单理解 - walkabc - 博客园
# [对Netty的简单理解](https://www.cnblogs.com/hitfire/articles/5352016.html)
　　2010/10/5~2014/3/31在这个时间点，自己是在做网页游戏开发，那段时间页游是挺火爆的，后来随着手游的兴起，页游走向了没落，我也考虑了转向了其他方向，PHP研发。
　　后来项目需要做一个实时聊天服务器，我便正式开始了Java的开发，以前也是有些了解的，但是还只是停留在基础的认识上面，现在可以说有些自己的心得了。
　　实时聊天服务器采用Java开发，使用的网络框架是Netty,目前的聊天服务器是分布式的，其实单纯的聊天服务器是简单的，但是如果要支持一个巨大的聊天室是非常困难的(30W人)，单机可能承载了不了那么多人，这个就需要把这种大聊天室的用户均衡负载到多台服务器中，并保证多台服务器间的消息的同步。
　　Netty开发网络程序实在是太简单了，其本身的Reactor架构，使得开发者只需要关注自己的逻辑即可。
　　Netty中显著的特性就是Pipeline，管线操作，我们可以在Pipeline中添加自己的Handler，Pipeline的操作可以动态add，remove掉Handler。
　　即使是Netty开发方便也要注意几个关键点。
　　1.Handler里面不要做费时的IO/计算操作，最好是自己添加线程池处理比较费时的IO/计算
　　2.ChannelOption一定要注意设置ChannelOption.WRITE_BUFFER_LOW_WATER_MARK和ChannelOption.WRITE_BUFFER_HIGH_WATER_MARK
　　注意以上两点服务器的性能和内存都会得到比较好的控制。
　　给大家推荐一个Netty编程实践，非常好的介绍了Netty的使用，大家仔细阅读，不要错过，通过百度云分享给大家，通过最佳编程实践，就能很好的控制Netty了。
　　百度云链接：http://pan.baidu.com/s/1o8b5h8I

