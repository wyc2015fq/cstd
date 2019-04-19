# Unix5种IO模型 - youfangyuan - CSDN博客
2012年05月27日 15:53:38[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：255标签：[io																[unix](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)
个人分类：[linux编程](https://blog.csdn.net/youfangyuan/article/category/1144942)
Unix下的五种I/O模型是：
阻塞I/O
非阻塞I/O
I/O复用(select 和poll)
信号驱动I/O
异步I/O
阻塞I/O模型图：
![](http://my.oschina.net/uploads/space/2010/1224/170610_FFFA_102078.jpg)
非阻塞IO模型图解：
![](http://my.oschina.net/uploads/space/2010/1224/170637_3LNT_102078.jpg)
IO复用模型图解：
![](http://my.oschina.net/uploads/space/2010/1224/170701_c9lo_102078.jpg)
信号驱动IO图解：
![](http://my.oschina.net/uploads/space/2010/1224/170722_T6el_102078.jpg)
异步IO模型图解：
![](http://my.oschina.net/uploads/space/2010/1224/170747_JgRF_102078.jpg)
同步IO引起进程阻塞，直至IO操作完成。
异步IO不会引起进程阻塞。
IO复用是先通过select调用阻塞。
好了，上图已经解释的很清楚了，再来一张书中的综合图解：
![](http://my.oschina.net/uploads/space/2010/1224/171225_GBB8_102078.jpg)
