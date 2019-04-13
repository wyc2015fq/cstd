
# tcp-ip协议栈--socket API 之发送函数（read-recv） - 3-Number - CSDN博客


2017年09月29日 16:14:29[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：199


# 0x01 缘由
前面博文把服务端、客户端三次握手连接过程学习完了，现在开始学习下相关接收数据API。
# 0x02 API介绍
![](https://img-blog.csdn.net/20170929161451909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170929161510234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


# 0x03 相关调用一览
![](https://img-blog.csdn.net/20170929161532014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 0x04 tcp层
对应博文[http://blog.csdn.net/pangyemeng/article/details/78052244](http://blog.csdn.net/pangyemeng/article/details/78052244)
# 0x05 总结
发送过程仅仅是将相关数据接收，其他细节后期在继续学习。

