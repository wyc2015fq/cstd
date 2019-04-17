# Windows下VMware准备IOS开发环境 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月04日 15:34:16[boonya](https://me.csdn.net/boonya)阅读数：3542








       苹果的开发只支持Mac系统开发，如果你的机器是Windows的系统，那么你就需要在Windows上安装一个虚拟机，然后在虚拟机中安装Mac系统即可。安装虚拟机这里就不说了。下面主要说说虚拟机安装成功后的事情。

### 下载Mac系统的镜像

Mac镜像下载链接：http://pan.baidu.com/s/1dDnHvrj 密码：ct3o

使用方法如下：

1.首先解压文件，两个同时选中，右键进行解压。

![](http://bbs.itheima.com/data/attachment/forum/201409/05/235752mvxl4c2lj79240l5.png.thumb.jpg)



2.打开虚拟机VMware workstation，点击文件--打开选择解压的文件。
![](http://bbs.itheima.com/data/attachment/forum/201409/06/000146sv2vlovwxl5ewq2w.png.thumb.jpg)



3.打开后，点击运行此虚拟机，开机后通过虚拟光驱挂载tools.iso，安装VMware tools工具。

4.安装完成后，可自行安装xcode。

**经验**：虚拟机对内存要求较高，如果自己电脑配置内存低于4G，不必考虑此方法。

### VMware安装Xcode

可以参考下面两个文档：

VM下的Mac安装Xcode：[http://www.doc88.com/p-1532298455228.html](http://www.doc88.com/p-1532298455228.html)



windows下安装xcode：[http://www.doc88.com/p-333768466786.html](http://www.doc88.com/p-333768466786.html)







