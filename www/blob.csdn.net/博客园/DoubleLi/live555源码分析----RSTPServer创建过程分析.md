# live555源码分析----RSTPServer创建过程分析 - DoubleLi - 博客园






最近五一回家，终于有机会能安静的看一下流媒体这方面相关的知识，准备分析live555的源码，接下来会把我读源码的过程记录成博客，以供其他的同路人参考。

因为再读源码的过程中，并不是一路顺着读下来，往往是读到后面，才能反过来明白之前读的内容，所以有的时候会写的有点穿越感。

在这个过程中，参考了其他大牛的博客，所以一些基本概念，就不再重复，也感谢前辈们的贡献。

五一在家条件不便，源码只好用截图方式给出。 



下面进入正题：



 我们观察live555MediaServer.cpp里的main函数，主要做了三件事：

1.       创建TaskScheduler和UsageEnvironment，一个用来做调度，一个代表运行的环境。

2.       创建一个DynamicRTSPServer，这个继承自RSTPServer对象。

3.       进入死循环，实际上是使用select来完成并发网络模型。



那这篇文章就是主要讲的创建一个DynamicRTSPServer到底发生了什么（即第二条），下面这张图是根据源码画出来的（能力有限，画的简陋），是创建一个DynamicRTSPServer过程中的重要函数调用关系：



下面挑重点讲一下：

 DynamicRTSPServer继承自RTSPServer，所以构造DynamicRTSPServer也会构造其父类RTSPServer，RTSPServer构造函数如下：



其实核心代码就是调用turnOnBackgroundReadHandling函数，这个函数的代码如下：



再追踪：



这下就明了了，首先用assignHandler（）把一些属性和server socket做一个绑定，其次就是使用FD_SET把这个server socket加入select的相应的集合，为接下来的select死循环做准备。



那么turnOnBackgroundReadHandling（）的参数incomingConnectionHandlerRTSP又是什么鬼呢？上边说道，这个函数指针会和当前的socket被做一个绑定，所以当这个socket接收到数据的时候就会调用这个函数（这个以后会细说）。incomingConnectionHandlerRTSP经过层层调用，来到下面这个函数：



可以看到，server socket 进行accept操作就是在这一步进行的，也就是说初始的时候集合中只有一个server socket，当有客户端发起链接的时候，select调用返回，发现server socket发生了读操作，接下来便会调用注册了的incomingConnectionHandlerRTSP（）函数，在这个函数里，依次调用accept（）函数创建新的socket来与客户端进行通信。

incomingConnectionHandlerRTSP（）里接下来会调用createNewClientSession（），一个RTSPClientSession代表着一条与客户端的链接。createNewClientSession（）经过层层调用，会来到如下函数：



这下就全明了了，又是调用turnOnBackgroundReadHandling（）函数，只不过参数从incomingConnectionHandlerRTSP函数指针变为了incomingRequestHandler，为什么呢，因为上一次我们调用这个函数是针对于server socket，server socket的主要功能是建立网络连接并分配新的socket和客户端进行会话。而这一次，我们是针对的socket是直接和客户端进行通话的socket，两种socket接收到数据时处理操作是不同的，所以绑定的函数也是不一样的。

这次绑定这个函数的内部结构是什么样的呢，经过调用来到如下函数：



简单的说就是使用readsock（）从socket中读数据，使用handleRequstBytes（）来对数据进行处理，比如客户端发过来了DESCRIBE或者SETUP等要分别怎么处理呢，就是在这个函数里进行的。











