
# python获得两个数组交集、并集、差集的方法 - jiahaowanhao的博客 - CSDN博客


2018年01月14日 20:51:36[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：109标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)


[python获得两个数组交集、并集、差集的方法](http://cda.pinggu.org/view/24451.html)
本文实例讲述了python获得两个数组交集、并集、差集的房部分。分享给大家供大家参考。具体如下：
1. 获取两个list 的交集
![](http://www.cda.cn/uploadfile/image/20170928/20170928111530_58557.png)
\#方法二
print list(set(a).intersection(set(b)))
2. 获取两个list 的并集
print list(set(a).union(set(b)))
3. 获取两个 list 的差集
print list(set(b).difference(set(a))) \# b中有而a中没有的
通过以上方法，就能处理python list 的交集，并集，差集了。

