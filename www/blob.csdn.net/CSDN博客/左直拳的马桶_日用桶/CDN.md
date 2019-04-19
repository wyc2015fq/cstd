# CDN - 左直拳的马桶_日用桶 - CSDN博客
2015年03月05日 19:42:27[左直拳](https://me.csdn.net/leftfist)阅读数：1563
个人分类：[系统架构](https://blog.csdn.net/leftfist/article/category/2567173)
cdn，内容分发网络。
主要作用在于加快网站内容在互联网上的传输、响应速度：1)将用户请求引导到就近服务器；2)缓存原始网站内容，快速响应。
CDN网络架构有两大部分组成：
1）CDN中心
包括CDN网管中心和DNS重定向解释中心，负责全局均衡负载。
2）CDN节点
缓存原始站点内容，CDN中心负责将用户请求引导到最近的CDN节点。
每个CDN节点由高速缓存服务器 + 负载均衡器组成。但其实最简单的CDN节点只有一个Cache服务器，只有多台Cache服务器同时工作，该节点才需要引入负载均衡。
高速缓存服务器也可以换成镜像服务器。镜像服务器的好处是可以处理静态、动态内容的请求，但成本比较高昂。高速缓存服务器存储静态内容，根据二八定律，这部分其实占了大头。
工作原理：
CDN的DNS服务，将用户请求引导至最近的CDN节点，CDN节点检查自己的内容，如果为新，则直接返回给用户；否则到原始服务器获取，更新自己，同时返回给用户。
适用情况：
北联通、南电信的中国特色网络；静态内容需要缓存，提高响应速度的；直接想提升相应速度的，。。。。
参考文章：
[http://baike.baidu.com/link?url=B84YVe0GEWYovdh9aU80G1x93MqAQTUOeGfgCRCCqZYSEDkgi35lxrciv7R9qhihJPt8KdHUthbB_zQlvPubna](http://baike.baidu.com/link?url=B84YVe0GEWYovdh9aU80G1x93MqAQTUOeGfgCRCCqZYSEDkgi35lxrciv7R9qhihJPt8KdHUthbB_zQlvPubna)
[http://wenku.baidu.com/link?url=0QmMCve_UaP131TDAnSNYok7ZKsX1mMiqt8AUY2yRFTMRM0_UpAcBBnQfzNLfAikV38XrYsPU_8NfBKJxzIAOCGPUhMRxsHujjebl9TrmxK](http://wenku.baidu.com/link?url=0QmMCve_UaP131TDAnSNYok7ZKsX1mMiqt8AUY2yRFTMRM0_UpAcBBnQfzNLfAikV38XrYsPU_8NfBKJxzIAOCGPUhMRxsHujjebl9TrmxK)
一个网站的优化，有许多部分，CDN只是其中之一。整理了一下，可分为：
1、前端
1）浏览器缓存
2）页面压缩
3）改进页面布局
4）减少cookie传递
2、网络
1）CDN
2）反向代理
3、服务器
1）缓存
2）多线程
3）异步操作
4）消息队列
5）集群
4、数据库
1）优化
2）读写分离
3）分布式
