# 如何使用jrtplib进行RTP发送和接收 - 爱写代码的自由人 - CSDN博客





2019年01月15日 18:41:32[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：79








RTP 协议是目前流媒体实时传输用得最广的一种协议，大家在开发用到RTP协议的程序时，可能会考虑使用一些开放源代码的RTP库，比如大名鼎鼎的JRTPLIB。 JRTPLIB是一个面向对象的RTP库，它完全遵循RFC 1889设计，在很多场合下是一个非常不错的选择，JRTPLIB 是一个用C++语言实现的RTP库，目前已经可以运行在Windows、Linux、FreeBSD、Solaris、Unix和 VxWorks等多种操作系统上。

关于jrtplib的使用方法，下面这篇文章说得很详细。

[http://www.cnblogs.com/yuweifeng/p/7550737.html](http://www.cnblogs.com/yuweifeng/p/7550737.html)

jrtplib的下载链接：[http://research.edm.uhasselt.be/jori/page/CS/Jrtplib.html](http://research.edm.uhasselt.be/jori/page/CS/Jrtplib.html)

注意下载到的源码包里是没有VC编译好的静态库的，需要自己编译，方法是用CMake-GUI工具转成你想要的指定版本的VS工程文件（VC2013，VC2015，VC2017...），然后打开工程文件编译成库。

如果你需要带VC2015工程文件的jrtplib源码包，可以从我的资源里下载：

[https://download.csdn.net/download/zhoubotong2012/10918948](https://download.csdn.net/download/zhoubotong2012/10918948)

另外，提供一个我写的使用jrtplib收发视频的例子：[https://download.csdn.net/download/zhoubotong2012/10918971](https://download.csdn.net/download/zhoubotong2012/10918971)



