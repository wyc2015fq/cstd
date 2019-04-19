# OutOfMemoryError Direct buffer memory - Ms.Fox的专栏 - CSDN博客
2019年01月17日 17:02:58[lulu-lu](https://me.csdn.net/smbluesky)阅读数：18
![](https://img-blog.csdnimg.cn/20190117155706672.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NtYmx1ZXNreQ==,size_16,color_FFFFFF,t_70)
查了好多，
[https://blog.csdn.net/jiangguilong2000/article/details/42297873](https://blog.csdn.net/jiangguilong2000/article/details/42297873)
把内存数据dump下来，查看使用的比例，发现是netty底层占用异常。这里说的是
Netty.4.0.24.final版本有个bug,原因是因为缓冲区的数据如果超过了max_capacity的话，if(size==maxCapacity){....}永远不会成立，所以内存占用会无限扩增导致。
那就更新版本把~~
又查到一个地方，是这样解释的：
![](https://img-blog.csdnimg.cn/20190117170037499.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NtYmx1ZXNreQ==,size_16,color_FFFFFF,t_70)
[https://blog.csdn.net/yanchangyufan/article/details/81877483](https://blog.csdn.net/yanchangyufan/article/details/81877483)
按着这个改了一版，跑着看看结果~~
