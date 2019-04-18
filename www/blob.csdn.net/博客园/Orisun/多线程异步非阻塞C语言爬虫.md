# 多线程异步非阻塞C语言爬虫 - Orisun - 博客园







# [多线程异步非阻塞C语言爬虫](https://www.cnblogs.com/zhangchaoyang/articles/2351758.html)





最近用C写了个爬虫，纯属练习，离实用还相差甚远。

下载源码：[ISeeSpider](http://files.cnblogs.com/zhangchaoyang/ISeeSpider3.zip)

bloom.h实现布隆过滤器算法。对一条url拆分为domain和path两部分，bloomDomain函数判断domain是否出现过，bloomPath函数判断path是否出现过。如果domain未出现过，则要先进行DNS解析（解析之后把domain和ip对存入map），再下载网页；如果domain出现过，则不需要再进行DNS解析，此时如果path也出现过，则该url直接忽略；如果是新的url，需要放入queue。

建立好socket connection后向连接写入http request，然后把sockfd放入epoll中，同时sockfd设为非阻塞式的。当sockfd准备就绪后，就说明可以从sockfd中读取http response数据（即下载网页）了。对于每一个下载网页的任务创建一个分离的子线程去完成。

下载网页时，一边下载，一边抽取超链接放入待爬取的url queue。此时对于每一个下载需要单独创建一个buffer，比如我把buffer的大小设为1K，则每次从sockfd中read时，最多读取1K的数据。从buffer中提取出所有超链接，然后整个buffer左移，把包含所有超链接的最短子串移出去。实际上还可以继续左移，直到一个空格移到buffer的首位置为止，记下此时buffer中还有多少数据（记为left_size）。则下次从sockfd中read时，需要读取的数据量为1k - left_size。

每次从buffer中读取了一些url，去除已下载过的url，再去除已存在于map<domain,ip>中的，剩下的需要进行一次DNS异步解析（使用libevent）。libevent是非线程安全的，即event_base不被多个线程share，所以我每次调用libevent时在一个线程中完成event_base和创建和释放，即一次完整DNS异步解析是在一个线程中就完成的。

每个线程完成下载网页的工作后，从url_queue中读出1个或2个url（epoll中的sockfd比较少时就2个，否则就1个），建立sockte connect，发出http request，然后把sockfd加入到主线程的epoll中去，最后子线程退出。

下面给个程序运行时的截图：

![](https://pic002.cnblogs.com/images/2012/103496/2012021419304969.png)












