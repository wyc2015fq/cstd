
# Duilib + libuv实践小程序 - paohui0134的博客 - CSDN博客


2016年07月20日 11:14:44[paohui0134](https://me.csdn.net/paohui0134)阅读数：728


# Duilib + libuv实践小程序
最近因为有些空闲的时间，所以使用duilib + libuv写了一个小程序，一方面是为了实践之前所学的libuv，复习duilib库的使用，另一方面也是为了督促自己持续学习。
## 简介
程序界面部分通过duilib实现，底层通过libuv实现，主要是一个简单的通过socket实现的端口转发功能，目前基本走通了TCP协议的流程，UDP还在实现中。
截图如下：
![这里写图片描述](https://img-blog.csdn.net/20160720111018742)
由于本人水平有限，对网络开发也不熟悉，所以程序肯定存在许多不足与bug，可能界面实现上的参考意义更大一些。如果您在使用或参考中发现了问题，或者有什么建议，请提出来，鄙人感激不尽。联系方式：
564954180@qq.con
工程地址：
[https://github.com/paohui0117/PortMapping](https://github.com/paohui0117/PortMapping)

