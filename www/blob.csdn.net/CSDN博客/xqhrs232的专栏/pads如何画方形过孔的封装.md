# pads如何画方形过孔的封装 - xqhrs232的专栏 - CSDN博客
2015年03月26日 23:31:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2349
原文地址::[http://zhidao.baidu.com/link?url=57aywD0Q6WTnl7XKbIHuEwoww4Oq1lIfSPqjou1Gmcm7HNJA3FrqJPxGDiPRhTBBxpx9zdyTGKeZh2tc3PyGNYq26vZ6EicuLlp9imtNnm3](http://zhidao.baidu.com/link?url=57aywD0Q6WTnl7XKbIHuEwoww4Oq1lIfSPqjou1Gmcm7HNJA3FrqJPxGDiPRhTBBxpx9zdyTGKeZh2tc3PyGNYq26vZ6EicuLlp9imtNnm3)

PADS中不能直接放置方孔，能接受的话，可以用顺道儿椭圆形孔：设置焊盘时，过孔点Slot进行设置长宽
向左转|向右转
![](http://f.hiphotos.baidu.com/zhidao/wh%3D600%2C800/sign=7739668f79f40ad115b1cfe5671c3de7/962bd40735fae6cd06cbfad60db30f2442a70f0d.jpg)
如果一定要方孔，则可以画在Outline层，另外通知PCB厂作特别处理（即先冲这个孔再沉铜）
