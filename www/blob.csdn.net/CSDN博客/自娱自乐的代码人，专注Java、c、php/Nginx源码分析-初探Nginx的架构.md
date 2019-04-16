# Nginx源码分析 - 初探Nginx的架构 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年01月25日 19:20:44[initphp](https://me.csdn.net/initphp)阅读数：4655
个人分类：[Nginx源码分析](https://blog.csdn.net/initphp/article/category/6081681)

所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









### 

Nginx是我们日常使用非常多的一款服务器。

Nginx的源码写的非常漂亮，是c语言学习和进阶的最好的学习资料。

### 源码目录

下面我们先看下Nginx的目录结构：

![](https://img-blog.csdn.net/20160125191433988?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Nginx的源码主要分布在src/目录下，而src/目录下主要包含三部分比较重要的模块。

1. core：包含了Nginx的最基础的库和框架。包括了内存池、链表、hashmap、String等常用的数据结构。

2. event：事件模块。Nginx自己实现了事件模型。而我们所熟悉的Memcached是使用了Libevent的事件库。自己实现event会性能和效率方便更加高效。

3. http：实现HTTP的模块。实现了HTTP的具体协议的各种模块，该部分内容量比较大。




### Nginx架构

Nginx是一款多进程的软件。Nginx启动后，会产生一个master进程和N个工作进程。其中nginx.conf中可以配置工作进程的个数：


`worker_processes  1;`
多进程模块有一个非常大的好处，就是不需要太多考虑并发锁的问题。


我们常见的软件Memcached就和Nginx相反，就是典型的多线程模型的c语言软件。

![](https://img-blog.csdn.net/20160127165343223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





