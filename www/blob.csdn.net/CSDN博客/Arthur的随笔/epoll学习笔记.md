# epoll学习笔记 - Arthur的随笔 - CSDN博客
2012年04月17日 23:41:33[largetalk](https://me.csdn.net/largetalk)阅读数：3867
一篇讲怎么使用在python中使用epoll得好文章 [http://scotdoyle.com/python-epoll-howto.html](http://scotdoyle.com/python-epoll-howto.html) ，文章里不仅有epoll得使用和代码展示，还讲了些epoll工作模式和TCP得一些内容， 稍微做点笔记。
设置socket为non-block ： 
serversocket.setblocking()
ET和LT：
epoll工作模式分为edge-trigger 和 level-trigger, 默认是 LT，设置方式分别是：
epoll.register(serversocket.fileno(), select.EPOLLIN | select.EPOLLET)  #ET
epoll.register(serversocket.fileno(), select.EPOLLIN )                                  #LT
两者的区别是，ET事件触发只会在读/写从unready到ready跃迁时发生， 而LT是只要ready就会触发。想象有个readylist， ET是事件触发一次之后就将fd从list里面去除掉，而LT不会，LT会等到fd为unready时才会从这个list去除掉。理论上ET更节省内存，但ET需要更复杂得逻辑和更好得控制，所以一般使用LT。
TCP_CORK和TCP_NODELAY:
设置方式:
connections[fileno].setsockopt(socket.IPPROTO_TCP, socket.TCP_CORK, 1)
serversocket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
理解这个两个参数要先理解Nagle算法。 Nagle算法被设计用来解决拥塞网络中大量小封包问题，方法就是在缓存数据包，在缓存区未满且时间未超过阈值时，不直接发送数据包，将多个数据包组合起来一起发送，提供网络使用率。TCP中默认是开启该算法，TCP_CORK和TCP_NODELAY被用来禁止Nagle算法，但他们是互斥得，使用环境和目的不一样。
TCP_NODELAY适用于要求实时数据发送环境，不希望有延时。比如ssh
TCP_CORK是自己控制数据包发送时机，原理是先“塞”TCP,然后往缓冲区写数据，然后打开塞子，将数据一起发出，适合发送大量数据，且小的封包也会被发送，不会被缓存住。比如http， ftp
另：从Unix network programming 6.2节讲得I/O模型来看， epoll其实也是属于同步I/O，我们平时讲的epoll异步应该是指异步处理，而非epoll是异步I/O
