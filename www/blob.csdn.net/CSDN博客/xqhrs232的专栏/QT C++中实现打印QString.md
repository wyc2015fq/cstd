# QT C++中实现打印QString - xqhrs232的专栏 - CSDN博客
2017年09月26日 18:06:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：244
原文地址::[http://blog.csdn.net/joe199106/article/details/53010370](http://blog.csdn.net/joe199106/article/details/53010370)
相关文章
1、
# [QT控制台输出QString](http://blog.csdn.net/u013007900/article/details/44566567)----[http://blog.csdn.net/u013007900/article/details/44566567](http://blog.csdn.net/u013007900/article/details/44566567)

在调试QT C++代码过程中，有时候不免需要打印QString值以判断程序运行到哪一步，网上常有的用qDebug然后std：：out 输出。但是嵌入式系统中很少有加入STD库。在这里我推荐一种直接调用printf的方法，简单又快捷
QString output
printf("%s\n",output.toStdString().data());

