# 基站Cell-ID转经纬度工具（开源） - xqhrs232的专栏 - CSDN博客
2014年11月28日 16:40:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2963
原文地址::[http://blog.csdn.net/lion888/article/details/6196032](http://blog.csdn.net/lion888/article/details/6196032)
相关文章
1、基站定位，已知基站LAC和CELL-ID，如何获取对应的经纬度？----[http://bbs.csdn.net/topics/370010755?page=1](http://bbs.csdn.net/topics/370010755?page=1)
下载地址：[http://download.csdn.net/source/3030380](http://download.csdn.net/source/3030380)
可以把Cell-ID信息转为经纬度数据，采用两种Google接口。
![](http://hi.csdn.net/attachment/201102/20/0_12981739947Gh9.gif)
1）一种是Google Gears，RESTful接口，简洁，Google已经停止Gears开发，但是此接口依然可用。（[http://code.google.com/intl/zh-TW/apis/gears/geolocation_network_protocol.html](http://code.google.com/intl/zh-TW/apis/gears/geolocation_network_protocol.html)）
2）另外一种是Google Mobile Map，二进制接口，是Google内部接口，没有对外API文档，通过Android手机接口破解
备注：这两个接口对中国的基站数据收集得不是很全
编程语言：C#, VS2008
