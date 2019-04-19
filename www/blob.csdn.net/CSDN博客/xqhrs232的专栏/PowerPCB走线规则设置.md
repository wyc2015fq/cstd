# PowerPCB走线规则设置 - xqhrs232的专栏 - CSDN博客
2013年07月02日 20:47:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：898
个人分类：[PCB/EDA/硬件设计与仿真](https://blog.csdn.net/xqhrs232/article/category/906926)
原文地址::[http://www.dzjs.net/html/PCBjishu/2008/1120/3498.html](http://www.dzjs.net/html/PCBjishu/2008/1120/3498.html)
有朋友说POWERPCB布好局不会设置走线规则，比如默认线宽、安全线距等等，哪么我就截几个图片给大家看看吧！
下图设置走线默认线宽！
![](http://www.dzjs.net/upimg/userup/0811/200Z30919E.jpg)
设置线与线及过孔、铺铜等相关规则参照下图设置就行了！
![](http://www.dzjs.net/upimg/userup/0811/200Z3362P0.jpg)
下面是如何设置等长线及相关网络的走线规则
![](http://www.dzjs.net/upimg/userup/0811/200Z40H1U.jpg)
首先按上图点NET 进入设置介面
![](http://www.dzjs.net/upimg/userup/0811/200Z43010J.jpg)
然后选中其中要走等长的网络点HISPEED 进入设置介面，如下图：
![](http://www.dzjs.net/upimg/userup/0811/200Z455V55.jpg)
填好相关设置后点OK 就可以了！
另外如果要设置其中的某条网络的走线宽度可以选中要设置的网络后点CLEARANCE 进入设置！
随手写写，如有不对的地方欢迎大家指正，探讨等等！
//===============================================================================
**pads layout中怎样设置线宽为30，方便画电源线，（不使用w30的情况下）**
设置design rules，选net，选择你要设置线宽的电源网络，clearance里设置线宽- 追问
如果我要所有的走线都为30mil，该怎样设置呢- 回答
那就不进NET，进DEFAULT-CLEARANCE- 提问者评价
是啊，多谢你提醒啊，再问个问题，怎样使走线上面显示网络名啊？

