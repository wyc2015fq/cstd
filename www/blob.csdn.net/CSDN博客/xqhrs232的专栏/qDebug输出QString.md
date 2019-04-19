# qDebug输出QString - xqhrs232的专栏 - CSDN博客
2018年06月07日 15:24:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：193
原文地址::[https://blog.csdn.net/lian_gong2009/article/details/50468887](https://blog.csdn.net/lian_gong2009/article/details/50468887)
相关文章
1、Qt中int转换成QString----[https://blog.csdn.net/liangzhuangdongtou/article/details/52242091](https://blog.csdn.net/liangzhuangdongtou/article/details/52242091)
   在使用qDebug输出QString,直接输出会有问题，正确的方法如下：
[cpp][view plain](https://blog.csdn.net/lian_gong2009/article/details/50468887#)[copy](https://blog.csdn.net/lian_gong2009/article/details/50468887#)
- QString readString = “Hello World!”;   
- qDebug("readString = %s",qPrintable(readString));  
应该使用如下函数转换一下：
const char * qPrintable(const QString & str)
Returns str as a const char *. This is equivalent to str.toLocal8Bit().constData().
