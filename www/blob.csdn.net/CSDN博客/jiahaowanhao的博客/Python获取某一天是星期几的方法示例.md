
# Python获取某一天是星期几的方法示例 - jiahaowanhao的博客 - CSDN博客


2018年06月15日 21:31:45[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1479


[Python获取某一天是星期几的方法示例](http://cda.pinggu.org/view/25830.html)
本文实例讲述了Python获取某一天是星期几的方法。分享给大家供大家参考，具体如下：
这里以2017年的春节（1月28号）为例：
import re;
import time;
import datetime;
if(__name__=="__main__"):
\#today=int(time.strftime("%w"));
anyday=datetime.datetime(2017,1,28).strftime("%w");
print anyday
运行效果图如下：
![](http://cda.pinggu.org/uploadfile/image/20180615/20180615065836_36287.png)

