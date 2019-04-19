# Bus Hound 的使用方法 - xqhrs232的专栏 - CSDN博客
2017年09月14日 17:36:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1837
原文地址::[http://www.cnblogs.com/ChYQ/p/5371806.html](http://www.cnblogs.com/ChYQ/p/5371806.html)
相关文章
1、[如何查看BUSHOUND内容](http://blog.csdn.net/yu704645129/article/details/38819437)----[http://blog.csdn.net/yu704645129/article/details/38819437](http://blog.csdn.net/yu704645129/article/details/38819437)
2、bushound使用说明----[https://wenku.baidu.com/view/ded529eea6c30c2258019ebd.html](https://wenku.baidu.com/view/ded529eea6c30c2258019ebd.html)
3、
# [Bus Hound的简单介绍](http://blog.csdn.net/parasoft/article/details/38639877)----[http://blog.csdn.net/parasoft/article/details/38639877](http://blog.csdn.net/parasoft/article/details/38639877)
# 4、**【外设篇】干货！！！bushound对usb协议进行抓包的使用方法----[http://support.huawei.com/huaweiconnect/enterprise/thread-329153.html](http://support.huawei.com/huaweiconnect/enterprise/thread-329153.html)**

背景：
　　最近在研究USB相关协议，需要对USB数据进行抓取分析，Bus Hound是个非常赞的工具，在此将其使用方法记录下来，以备下次快速上手使用。
正文：　　
主界面如下：
![](http://images2015.cnblogs.com/blog/758869/201604/758869-20160409151730515-786073844.png)
首先关注菜单栏三个选项：
Capture: 数据抓取菜单选项；　　Setting：设备设置菜单选项；　　Devices：设备选取菜单选项。
第一步：点击“Devices”菜单选项选取设备，（选取之后，就可以监控该设备了。）
![](http://images2015.cnblogs.com/blog/758869/201604/758869-20160409152202015-1507342921.png)
　　根据VID/PID来单击选取要操作的设备对象，最下方 “Properties” 即所要操作的USB设备的详细信息：人机接口（HID）设备，共配置有3个端点，端点0为控制端点，大小为8bytes；端点1分别为中断OUT/IN端点，大小为64bytes。
　　点击SendCommands即向USB设备发送命令，具体如何发送参见第二步。
第二步，点击“Devices”菜单页面下的“SendCommands”来向USB设备发送命令，
![](http://images2015.cnblogs.com/blog/758869/201604/758869-20160409153925437-672457077.png)
选取USB界面，要发送数据即选择OUT端点，选择发送长度（注意！不能超过端点设定大小！），填写发送数据，（此处当初也让我非常郁闷，根本没有方框填写数据好吧！！！？无意中将鼠标点击到最下面的红框内，才发现最下面框中的部分既是数据输入部分，直接输入数值就可以。）最后点击 “RUN” 完成发送。
　　为了DEBUG，在下位机做了个Loopback，即将上位机发送的数据发回给上位机，如何抓取USB数据参见第四步。
第三步，点击“Setting”菜单做一些必要的设置，
![](http://images2015.cnblogs.com/blog/758869/201604/758869-20160409154736203-764061095.png)
按照自己的需求设置相应的设置，在此我也只是设置了抓取的最大包记录显示大小为64bytes。
第四步，点击“Capture”菜单选项，观察USB上传的数据。
![](http://images2015.cnblogs.com/blog/758869/201604/758869-20160409155446906-829475322.png)
即可观察到，OUT下发USB设备数据与USB设备上发的IN数据与预期相符，完全一致。 
更新一下最新的发现：关于如何用BUS Hound查看USB设备的端点号。
看上图的红框内，29.1这个“.1”我原来一直忽视它，直到最近才发现，这个“.1”代表USB设备的端点号。“.1”代表USB设备端点“1”发给主机或者主机发给端点“1”的信息，其他的端点号同理。
2016-12-21
记录地点： 深圳WZ
记录时间： 2016-04-09

