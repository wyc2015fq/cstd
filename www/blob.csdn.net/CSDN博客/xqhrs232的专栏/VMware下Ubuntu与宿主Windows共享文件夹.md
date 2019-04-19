# VMware下Ubuntu与宿主Windows共享文件夹 - xqhrs232的专栏 - CSDN博客
2016年03月18日 17:37:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：303
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址：[http://blog.csdn.net/zz962/article/details/7706755](http://blog.csdn.net/zz962/article/details/7706755)
相关文章
1、[windows
 和 Ubuntu实现文件共享](http://www.cnblogs.com/zhengyuxin/articles/1938414.html)----[http://www.cnblogs.com/zhengyuxin/articles/1938414.html](http://www.cnblogs.com/zhengyuxin/articles/1938414.html)
**概述**
1、安装VMware Tool
2、设置共享
**步骤**
开始安装VMware Tool
![](https://img-my.csdn.net/uploads/201207/01/1341132709_9233.jpg)
显示如下画面（如果宿主无法访问外网，可能会出现一个更新失败，可以无视之）
![](https://img-my.csdn.net/uploads/201207/01/1341132886_2285.jpg)
通过下列命令解压、执行，分别是下面的tar和sudo的两行（下面是已有vmware tool的情况，注意密码是登录口令，不会显示）
![](https://img-my.csdn.net/uploads/201207/01/1341133002_7745.jpg)
一路按Enter，最后得到
![](https://img-my.csdn.net/uploads/201207/01/1341138308_4994.jpg)
在VMware中给虚拟机设置共享目录
![](https://img-my.csdn.net/uploads/201207/01/1341138641_5292.jpg)
执行mount命令（下图中sudo mount那一行），之后mnt/hgfs里就可以看到前面设置的共享目录
![](https://img-my.csdn.net/uploads/201207/01/1341138470_7480.jpg)
如果mnt出错，可以执行 sudo apt-get install open-vm-dkms。该命令从网上下载东西安装，这个包是干什么的不清楚，如果不联网怎么办也不清楚。
杂项- 切换到虚拟机画面后，输入内容，终端不显示：需要点一下终端界面，让其获得焦点
- 装vmtool的过程有点长
- 重启动虚拟机后，hgfs里又看不到共享目录，需要把mount命令重新执行一遍
参考
1、[http://topic.csdn.net/u/20110101/13/99eb73ee-b002-4ee0-9995-6fede0e23995.html](http://topic.csdn.net/u/20110101/13/99eb73ee-b002-4ee0-9995-6fede0e23995.html) （解决了hgfs中看不到共享的问题）
2、[http://studiogang.blog.51cto.com/505887/385625](http://studiogang.blog.51cto.com/505887/385625) （如何设置共享，访问共享）
3、[http://www.linuxidc.com/Linux/2010-05/25965.htm](http://www.linuxidc.com/Linux/2010-05/25965.htm) （如何安装vmtools）
