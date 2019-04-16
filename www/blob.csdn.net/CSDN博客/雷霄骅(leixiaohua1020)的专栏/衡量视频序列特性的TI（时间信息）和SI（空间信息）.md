# 衡量视频序列特性的TI（时间信息）和SI（空间信息） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年06月08日 15:49:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：120









做视频评价试验的时候，需要根据一定的条件选择有代表性的视频序列。ITU-R BT.1788建议使用时间信息（TI，Temporal perceptual Information，也可以称时间复杂度）和空间信息（SI，Spatial perceptual Information，也可以称空间复杂度）来衡量视频的特性。

SI表征一帧图像的空间细节量。空间上越复杂的场景，SI值越高。

TI表征视频序列的时间变化量。运动程度较高的序列通常会有更高的TI值。

SI计算方法：对第n帧视频进行Sobel滤波，然后对滤波后图像计算标准差。选这些帧中的最大值为SI。

TI计算方法：求n与n-1帧图像的帧差，然后对帧差图像计算标准差。选这些帧中的最大值为TI。

以下截图分别显示Sobel滤波和帧差结果：

![](https://img-blog.csdn.net/20130608152758593)

Sobel滤波后结果（反映空间信息，即空间上的复杂度）：

![](https://img-blog.csdn.net/20130608152918218)

帧差的结果（反映时间信息，即时间上的复杂度）：

![](https://img-blog.csdn.net/20130608152949140)

附带我们实验室自己编写的计算SI和TI的工具（程序+源代码，工程是vs2010的）：

[http://download.csdn.net/detail/leixiaohua1020/5546783](http://download.csdn.net/detail/leixiaohua1020/5546783)

更新记录（2014.7.28）=============

上述工具是图形界面的，后来发现图形界面也不是特别方便，直接用命令行进行批处理会更方便些，因此拜托师弟把代码抽取出来做了个控制台程序。师弟也很给力，迅速就搞了出来，在这里上传上来。

可以直接使用的exe：[http://download.csdn.net/detail/leixiaohua1020/7686483](http://download.csdn.net/detail/leixiaohua1020/7686483)

源代码（VS2010）：[http://download.csdn.net/detail/leixiaohua1020/7686489](http://download.csdn.net/detail/leixiaohua1020/7686489)

命令行工具命令示例如下，计算了一个分辨率为1920x1080，像素为格式YUV420P，路径为D:\test.yuv的YUV格式的文件。


`tisi -i D:\test.yuv -x 1920 -y 1080 -f 420`


界面截图如下：

![](https://img-blog.csdn.net/20140728120939161?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

帮助菜单

![](https://img-blog.csdn.net/20140728122555718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

运行时截图

运行完毕后会在测试序列相同的目录下生成*.csv的结果文件。

输出的数据可以使用Excel进行统计，如下示例统计了某个视频序列的TI，SI和视频帧序号之间的关系。SI较高的地方，视频帧画面的空间细节比较多（空间复杂度高）。TI较高的地方，视频帧内容运动比较剧烈（时间复杂度高）。

![](https://img-blog.csdn.net/20140728130530546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140728130814487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



TI，SI概念出处《ITU-R BT.1788 建议书》：[http://download.csdn.net/detail/leixiaohua1020/6423425](http://download.csdn.net/detail/leixiaohua1020/6423425)




