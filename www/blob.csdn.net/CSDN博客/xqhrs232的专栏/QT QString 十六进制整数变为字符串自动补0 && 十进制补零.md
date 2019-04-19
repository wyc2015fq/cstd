# QT QString 十六进制整数变为字符串自动补0 && 十进制补零 - xqhrs232的专栏 - CSDN博客
2018年05月25日 13:45:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：371
原文地址::[https://blog.csdn.net/z278930050/article/details/51496856](https://blog.csdn.net/z278930050/article/details/51496856)
QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
int a=0001;
QString str=QString("%1").arg(a,4,10,QLatin1Char('0'));
原文地址：[http://www.cnblogs.com/luxiaolai/p/3723946.html](http://www.cnblogs.com/luxiaolai/p/3723946.html)
