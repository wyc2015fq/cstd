# 如何在windows系统下用串口通信完爆raspberry pi（树莓派） - xqhrs232的专栏 - CSDN博客
2016年04月12日 15:04:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6432
原文地址::[http://www.alsrobot.cn/article-141.html](http://www.alsrobot.cn/article-141.html)
相关文章
1、Windows下用串行连接控制树莓派----[http://shumeipai.nxez.com/2014/05/04/under-windows-serial-connection-control-raspberry-pi.html](http://shumeipai.nxez.com/2014/05/04/under-windows-serial-connection-control-raspberry-pi.html)
2、通过串口连接控制树莓派----[http://www.cnblogs.com/ma6174/archive/2013/04/23/3038626.html](http://www.cnblogs.com/ma6174/archive/2013/04/23/3038626.html)
3、[树莓派相关-树莓派串口配置方法](http://blog.csdn.net/yangqicong11/article/details/26571787)----[http://blog.csdn.net/yangqicong11/article/details/26571787](http://blog.csdn.net/yangqicong11/article/details/26571787)
4、树莓派串口调试描述----[http://wenku.baidu.com/link?url=sygffsZQl-SnGUBvM7QMiGlX8wsFs82GjoaO3DjzJlgwuPd32J0vF0eIWa7S-q8i-i3bbZzCTja--mODxrBjt0eXtErBdf71b7r-lKpgL-u](http://wenku.baidu.com/link?url=sygffsZQl-SnGUBvM7QMiGlX8wsFs82GjoaO3DjzJlgwuPd32J0vF0eIWa7S-q8i-i3bbZzCTja--mODxrBjt0eXtErBdf71b7r-lKpgL-u)
 在没有网络，没用键盘，没有显示器的情况下，控制树莓派就成了一个问题。 通过串口通讯果断的试用了一次发现效果不错，下面就和大家一起分享一下。
# 所需设备：
1.**raspberry pi **板子一块
2.一张SD卡（至少2G我们采用8G）
3.一根USB电源线（5v/700ma以上）
4.一根usb B型口线
5. 一个**[USB to TTL Board](http://www.alsrobot.cn/goods-49.html)**（USB 转串口）
6.3P 杜邦线一个
![树莓派](http://www.alsrobot.cn/images/upload/Image/IMG_20130914_173620_%E6%97%8B%E8%BD%AC.jpg)
具体操作过程：
下载系统镜像文件，并写入sd卡中
    首先要把镜像文件下载到准备的sd卡上去，镜像文件可以到官网下载，在官网上可以看到多个不同的系统镜像，可以根据自己的需要下载所需的镜像。
    我下载了**Raspbian "wheezy"**这个镜像，接下来要把镜像文件写入到sd卡上去。把下载下来的zip文件解压以后，得到一个img文件。然后把sd卡通过读卡器插到电脑上，系统会自己挂载sd卡，千万不要直接直接把镜像文件拖进去，这样是没有用的。我们可以通过**[USB Image Tool](http://www.alsrobot.cn/goods-49.html)**将镜像文件写入到**[SD卡](http://www.alsrobot.cn/goods-392.html)**中。
![](http://www.alsrobot.cn/images/upload/Image/usb%20image%20tool%202(4).jpg)
     打开软件点击Favorites添加系统镜像文件，点击Restore开始将镜像文件写入到SD卡中。
![](http://www.alsrobot.cn/images/upload/Image/usb%20image%20tool%203(1).jpg)
![](http://www.alsrobot.cn/images/upload/Image/usb%20image%20tool(1).jpg)
    这样一个带有系统的树莓派**SD卡**就制作完成了。在这是会看到8G的内存只剩下50多MB说明系统已经成功刷入到**SD卡**中，不见存储空间为**Linux**系统下的分区在Windows下部显示。
![](http://www.alsrobot.cn/images/upload/Image/%E5%86%85%E5%AD%98%E5%8D%A1(1).jpg)
# **串口**的连接
    串口我使用的是**USB to TTL Board**，通常**串口**共有4根，分别为红，绿，白，黑。这里我们不需要从串口取电，所以红色的在这里不需要用到。
    黑线是接地线(图中的ground)，**TXD**(下图的GPIO 14)，**RXD**(下图的GPIO 15)
![Raspberry piRaspberry pi](http://www.alsrobot.cn/images/upload/Image/123(7).jpg)
![](http://www.alsrobot.cn/images/upload/Image/raspberry%20pi.png)
![](http://www.alsrobot.cn/images/upload/Image/DSC_0094(1).jpg)
    奥松机器人的**USB to TTL Board**是不需要驱动，在电脑可以直接在设备管理中找到COM口，连接好线路后，将usb口连接到电脑上，然后使用**PuTTY**软件和**串口**进行通讯。打开软件选择serial设置**串口**号，波特率设置为115200。
![](http://www.alsrobot.cn/images/upload/Image/putty(1).jpg)
    点击Open给树莓派上电会在窗口中看到类似**Linux**终端的界面。
![](http://www.alsrobot.cn/images/upload/Image/%E5%BC%80%E6%9C%BA(1).jpg)
    开机会加载系统文件当加载好内核文件后为我们输入账号和密码,账号是pi，密码为raspberry当我们输入无误后就可以访问系统中的目录文件。
![](http://www.alsrobot.cn/images/upload/Image/%E5%BC%80%E6%9C%BA%E8%84%9A%E6%9C%AC.jpg)
    下面我们就可以尽情的享受树莓派神奇了，不用东奔西走的在寻找显示了，你还在犹豫么快买个pi上手试试吧让自己也成为 有pi一族。
系统下载网址 [http://www.raspberrypi.org/downloads](http://www.raspberrypi.org/downloads)
USB Image Tool 下载地址[http://pan.baidu.com/share/link?shareid=3094444598&uk=2651806117](http://pan.baidu.com/share/link?shareid=3094444598&uk=2651806117)
欲了解更多<爱上Raspberry Pi>请下载电子书：[http://pan.baidu.com/share/link?shareid=2305046148&uk=2651806117](http://pan.baidu.com/share/link?shareid=2305046148&uk=2651806117)
关于树莓派的那些事：
新书《爱上 Raspberry Pi》“树莓风”来袭 ：[http://alsrobot.cn/article-113.html](http://alsrobot.cn/article-113.html)什么是树莓派Raspberry Pi？：[http://www.alsrobot.cn/article-82.html](http://www.alsrobot.cn/article-82.html)玩转树莓派Raspberry Pi之准备工作：[http://www.alsrobot.cn/article-83.html](http://www.alsrobot.cn/article-83.html)树莓派 Raspberry Pi 使用简介之linux和windows：[http://www.alsrobot.cn/article-107.html](http://www.alsrobot.cn/article-107.html)爱上Raspberry Pi初体验：[http://www.alsrobot.cn/article-112.html](http://www.alsrobot.cn/article-112.html)《爱上Arduino》中文教材：[http://www.alsrobot.cn/goods-193.html](http://www.alsrobot.cn/goods-193.html)Raspberry Pi 树莓派 pcDuino专用 散热片：[http://www.alsrobot.cn/goods-403.html](http://www.alsrobot.cn/goods-403.html)树莓派控制器外部供电电源：[http://www.alsrobot.cn/goods-399.html](http://www.alsrobot.cn/goods-399.html)树莓派控制器HDMI转VGA转换器：[http://www.alsrobot.cn/goods-398.html](http://www.alsrobot.cn/goods-398.html)金士顿8G存储SD卡 树莓派系统 ：[http://www.alsrobot.cn/goods-392.html](http://www.alsrobot.cn/goods-392.html)树莓派通用AV数据线 AV视频线 镀金头：[http://www.alsrobot.cn/goods-391.html](http://www.alsrobot.cn/goods-391.html)7寸液晶屏 树莓派显示器 车载显示器：[http://www.alsrobot.cn/goods-386.html](http://www.alsrobot.cn/goods-386.html)iPazzPort 迷你无线键盘鼠标 树莓派pcduino专用：[http://www.alsrobot.cn/goods-382.html](http://www.alsrobot.cn/goods-382.html)
爱上Raspberry Pi 树莓派 首本中文教材 正版全新：[http://www.alsrobot.cn/goods-410.html](http://www.alsrobot.cn/goods-410.html)正品树莓派 Raspberry pi 2代 512M：[http://www.alsrobot.cn/goods-333.html](http://www.alsrobot.cn/goods-333.html)[爱上Raspberry Pi 入门套件](http://alsrobot.cn/goods-433.html)：[http://alsrobot.cn/goods-433.html](http://alsrobot.cn/goods-433.html)[树莓派Raspberry PI 美国进口外壳](http://alsrobot.cn/goods-431.html)：[http://alsrobot.cn/goods-431.html](http://alsrobot.cn/goods-431.html)[RS原装正品 树莓派 二代 Raspberry Pi](http://alsrobot.cn/goods-427.html)：[http://alsrobot.cn/goods-427.html](http://alsrobot.cn/goods-427.html)
