# Socket心跳包机制 - xqhrs232的专栏 - CSDN博客
2016年04月20日 16:32:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：372
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址:[http://blog.csdn.net/xuyuefei1988/article/details/8279812](http://blog.csdn.net/xuyuefei1988/article/details/8279812)
相关文章
1、socket关于心跳检测的正确做法----[http://bbs.csdn.net/topics/390322340](http://bbs.csdn.net/topics/390322340)
2、Socket心跳包----[http://bbs.csdn.net/topics/350248427](http://bbs.csdn.net/topics/350248427)
心跳包的发送，通常有两种技术
方法1：应用层自己实现的心跳包 
由应用程序自己发送心跳包来检测连接是否正常，大致的方法是：服务器在一个 Timer事件中定时 向客户端发送一个短小精悍的数据包，然后启动一个低级别的线程，在该线程中不断检测客户端的回应， 如果在一定时间内没有收到客户端的回应，即认为客户端已经掉线；同样，如果客户端在一定时间内没 有收到服务器的心跳包，则认为连接不可用。
方法2：TCP的KeepAlive保活机制
因为要考虑到一个服务器通常会连接多个客户端，因此由用户在应用层自己实现心跳包，代码较多 且稍显复杂，而利用TCP／IP协议层为内置的KeepAlive功能来实现心跳功能则简单得多。 不论是服务端还是客户端，一方开启KeepAlive功能后，就会自动在规定时间内向对方发送心跳包， 而另一方在收到心跳包后就会自动回复，以告诉对方我仍然在线。 因为开启KeepAlive功能需要消耗额外的宽带和流量，所以TCP协议层默认并不开启KeepAlive功 能，尽管这微不足道，但在按流量计费的环境下增加了费用，另一方面，KeepAlive设置不合理时可能会
 因为短暂的网络波动而断开健康的TCP连接。并且，默认的KeepAlive超时需要7,200，000 MilliSeconds， 即2小时，探测次数为5次。对于很多服务端应用程序来说，2小时的空闲时间太长。因此，我们需要手工开启KeepAlive功能并设置合理的KeepAlive参数。
以上转自网络。
[心跳包机制](http://www.cppblog.com/tx7do/archive/2009/11/09/100513.html)
  跳包之所以叫心跳包是因为：它像心跳一样每隔固定时间发一次，以此来告诉服务器，这个客户端还活着。事实上这是为了保持长连接，至于这个包的内容，是没有什么特别规定的，不过一般都是很小的包，或者只包含包头的一个空包。
   在TCP的机制里面，本身是存在有心跳包的机制的，也就是TCP的选项：SO_KEEPALIVE。系统默认是设置的2小时的心跳频率。但是它检查不到机器断电、网线拔出、防火墙这些断线。而且逻辑层处理断线可能也不是那么好处理。一般，如果只是用于保活还是可以的。
   心跳包一般来说都是在逻辑层发送空的echo包来实现的。下一个定时器，在一定时间间隔下发送一个空包给客户端，然后客户端反馈一个同样的空包回来，服务器如果在一定时间内收不到客户端发送过来的反馈包，那就只有认定说掉线了。
   其实，要判定掉线，只需要send或者recv一下，如果结果为零，则为掉线。但是，在长连接下，有可能很长一段时间都没有数据往来。理论上说，这个连接是一直保持连接的，但是实际情况中，如果中间节点出现什么故障是难以知道的。更要命的是，有的节点（防火墙）会自动把一定时间之内没有数据交互的连接给断掉。在这个时候，就需要我们的心跳包了，用于维持长连接，保活。
   在获知了断线之后，服务器逻辑可能需要做一些事情，比如断线后的数据清理呀，重新连接呀……当然，这个自然是要由逻辑层根据需求去做了。
   总的来说，心跳包主要也就是用于长连接的保活和断线处理。一般的应用下，判定时间在30-40秒比较不错。如果实在要求高，那就在6-9秒。
心跳检测步骤：
1客户端每隔一个时间间隔发生一个探测包给服务器
2客户端发包时启动一个超时定时器
3服务器端接收到检测包，应该回应一个包
4如果客户机收到服务器的应答包，则说明服务器正常，删除超时定时器
5如果客户端的超时定时器超时，依然没有收到应答包，则说明服务器挂了
转自：http://blog.sina.com.cn/s/blog_a459dcf5010153m5.html
根据上面的介绍我们可以知道对端以一种非优雅的方式断开连接的时候，我们可以设置SO_KEEPALIVE属性使得我们在2小时以后发现对方的TCP连接是否依然存在。
具体操作：
    //设置KeepAlive     
   1、 BOOL   bKeepAlive   =   TRUE;     
    int nRet=::setsockopt(sockClient,SOL_SOCKET,SO_KEEPALIVE,(char*)&bKeepAlive,sizeof(bKeepAlive));     
    if(nRet!=0)    
    {     
        AfxMessageBox("出错"); 
        return   ;
    }     
   2、感觉两小时时间太长可以自行设定方法1
//设置KeepAlive检测时间和次数     
    tcp_keepalive    inKeepAlive   =   {0};   //输入参数     
    unsigned   long   ulInLen   =   sizeof(tcp_keepalive );         
    tcp_keepalive    outKeepAlive   =   {0};   //输出参数     
    unsigned   long   ulOutLen   =   sizeof(tcp_keepalive );         
    unsigned   long   ulBytesReturn   =   0;     
    //设置socket的keep   alive为10秒，并且发送次数为3次     
    inKeepAlive.onoff   =   1;       
    inKeepAlive.keepaliveinterval   =   4000;   //两次KeepAlive探测间的时间间隔     
    inKeepAlive.keepalivetime   =   1000;   //开始首次KeepAlive探测前的TCP空闭时间     
    nRet=WSAIoctl(sockClient,       
        SIO_KEEPALIVE_VALS,     
        (LPVOID)&inKeepAlive,     
        ulInLen,     
        (LPVOID)&outKeepAlive,     
        ulOutLen,     
        &ulBytesReturn,     
        NULL,     
        NULL);     
    if(SOCKET_ERROR   ==   nRet)     
    {     
        AfxMessageBox("出错");
        return;    
    }   
3、感觉两小时时间太长可以自行设定方法2
因此我们可以得到
    int                 keepIdle = 6;
    int                 keepInterval = 5;
    int                 keepCount = 3;
    Setsockopt(listenfd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));
    Setsockopt(listenfd, SOL_TCP,TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
    Setsockopt(listenfd,SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
详见：http://blog.csdn.net/gavin1203/article/details/5290609
对**setsockopt**的操作，详见：http://www.cnblogs.com/hateislove214/archive/2010/11/05/1869886.html
