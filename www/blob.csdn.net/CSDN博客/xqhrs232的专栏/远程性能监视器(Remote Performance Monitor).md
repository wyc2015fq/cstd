# 远程性能监视器(Remote Performance Monitor) - xqhrs232的专栏 - CSDN博客
2013年03月19日 17:52:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1929
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://hi.baidu.com/timematter/item/619a4f488152bce1bcf4513a](http://hi.baidu.com/timematter/item/619a4f488152bce1bcf4513a)
相关网帖
1.使用Wince 自带的远程工具----[http://code086.com/article-bluesea725-6612968.html](http://code086.com/article-bluesea725-6612968.html)
Remote Performance Monitor工具是测量性能的一个图形化工具。Remote Performance Monitor应用程序可以追踪目标设备上的当前活动，并可查看日志文件数据。Performance Monitor工具可用于监视CE目标设备的系统资源消耗状况和其他的性能相关问题。
为了使用Remote Performance Monitor工具，需要一个运行在CE目标设备上的具有KITL的OS运行时映像。
为使用运行在仿真器上的带有映像的Remote Performance Monitor工具，在VS2005 IDE中选择Target | Remote Tools | Performance Monitor命令，然后使用与Remote File Viewer中相同的步骤来建立和仿真器之间的连接。
在建立与仿真器的连接之后，Remote Performance Monitor应用程序会启动，正如图6-18所示。
![](http://b.hiphotos.baidu.com/album/pic/item/8326cffc1e178a8260ad6375f603738da877e856.jpg) （点击查看大图）图  6-18
在Windows CE Remote Performance Monitor对话框中，完成下列操作：
(1) 选择Edit | Add to Chart命令进入Add to Chart 对话框，如图6-19所示。
![](http://d.hiphotos.baidu.com/album/pic/item/7c1ed21b0ef41bd506d8f72151da81cb38db3d5f.jpg) （点击查看大图）图  6-19
(2) 在Add to Chart对话框中，从Object列表中选择CE Process Statistics。在Counter列表中，选择%Processor Time。在Instance列表中，选择_Total，然后单击Add按钮继续。
(3) 在Add to Chart对话框中，从Object列表选择CE Memory Statistics。在Counter列表中，选择Memory Load，然后单击Add按钮继续。
(4) 在Add to Chart 对话框中，从Object列表中选择CE UDP Statistics。在Counter列表中，选择Datagrams Received/Sec，然后单击Add按钮继续。
(5) 在仿真器桌面上，快速移动鼠标，在屏幕的空白区域单击鼠标左键以避免激活其他程序，同时查看Remote Performance Monitor对话框上变化的图形。
(6) 在仿真器桌面上，启动Internet Explorer，查看Remote Performance Monitor对话框上变化的图形，如图6-20所示。
![](http://f.hiphotos.baidu.com/album/pic/item/5366d0160924ab18229dfee235fae6cd7a890b5f.jpg) （点击查看大图）图  6-20
(7) 在Remote Performance Monitor对话框的下方，单击以突出显示最后一行，以1.000 Datagrams Received/Sec显示为Scale Counter值。
(8) 在Remote Performance Monitor对话框中，选择Edit | Edit Chart Line命令进入Edit Chart Line对话框，如图6-21所示。
![](http://h.hiphotos.baidu.com/album/pic/item/0eb30f2442a7d9337723fba6ad4bd11372f0015f.jpg) （点击查看大图）图  6-21
(9) 在Edit Chart Line对话框的Scale列表上，选择10.0，然后单击OK按钮继续。
(10) 在仿真器上利用Internet Explorer导航，并在Remote Performance Monitor上观察其变化，如图6-22所示。
![](http://c.hiphotos.baidu.com/album/pic/item/0823dd54564e9258ab91993b9c82d158cdbf4eb9.jpg) （点击查看大图）图  6-22改变Datagrams Received/Sec显示比例之后，使用Internet Explorer导航Internet的活动变得更加栩栩如生。
转载自：http://book.51cto.com/art/201001/178704.htm

