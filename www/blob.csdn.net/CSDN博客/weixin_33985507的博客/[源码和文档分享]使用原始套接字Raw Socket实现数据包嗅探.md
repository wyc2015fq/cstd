# [源码和文档分享]使用原始套接字Raw Socket实现数据包嗅探 - weixin_33985507的博客 - CSDN博客
2018年12月28日 09:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
背景
网络上随时都流通了大量的数据包，我们要想实现抓包并分析，实现思路思路大概是：在合适的时候捕获数据包，保存到缓冲区，作为备用；然后，按照一定的结构和格式去读取缓冲区的内容。由于各种公开的网络协议是已知的，所以对于数据包的分析就比较简单。
通常我们都是使用类似WireShark的抓包软件嗅探数据包，这些抓包工具大部分都是基于WinPcap库实现的数据包嗅探功能。而现在，本文讲解下使用原始套接字的RawSocket方式实现数据包的嗅探。当然，和WinPcap相比，Raw Socket有很大的局限性。，它只能抓到IP层及以上的数据包，抓不到MAC层的数据包。
但是，Raw Socket开发的程序使用起来比较方便，不需要额外安装什么。本文就把实现的过程原理写成文档，分享给大家。
![6607861-d596a5a9bc21504a.png](https://upload-images.jianshu.io/upload_images/6607861-d596a5a9bc21504a.png)
**参考文档和完整的文档和源码下载地址：**
**[https://www.write-bug.com/article/1737.html](https://www.write-bug.com/article/1737.html)**
