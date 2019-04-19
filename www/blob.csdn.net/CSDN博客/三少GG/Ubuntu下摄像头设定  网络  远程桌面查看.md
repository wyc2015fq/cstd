# Ubuntu下摄像头设定 || 网络 || 远程桌面查看 - 三少GG - CSDN博客
2011年10月21日 11:02:10[三少GG](https://me.csdn.net/scut1135)阅读数：3474

**1. **查看摄像头芯片类型
 a.插入usb 摄像头
 b. lsusb 命令查看usb总线上的设备
    笔者的：
**2.**安装驱动(没有针对当前摄像头的linux下驱动，故需要装一个第三方的万能驱动UVC，可查看它所支持的摄像头具体型号Logitech等)
 支持的设备型号：[http://www.ideasonboard.org.sixxs.org/uvc/](http://www.ideasonboard.org.sixxs.org/uvc/)
“
The goal of this project is to provide all necessary software components to fully support**UVC**compliant devices in Linux. This
 include a V4L2 kernel device driver and patches for user-space tools.
The *USB Device Class Definition for Video Devices*, or USB Video Class, defines video streaming functionality on the Universal Serial Bus.
“
 1) git clone  http://linuxtv.org/git/media_build.git    
 2) cd media_build    
    ./build
    sudo make install      (可参见官方网站 [http://git.linuxtv.org/media_build.git](http://git.linuxtv.org/media_build.git)）
**重启系统。**
**3. **新立德安装工具  Cheese(推荐) 或﻿﻿Camorama(貌似有点小问题)4 I( U3 M0 c: |3 D  q
或者 sudo apt-get install  cheese  或 camorama
测试您的摄像头： cheese
两篇好的参考文章：
1. [http://blog.csdn.net/linucos/article/details/5368667](http://blog.csdn.net/linucos/article/details/5368667)
2. [http://tech.techweb.com.cn/thread-433380-1-1.html](http://tech.techweb.com.cn/thread-433380-1-1.html)
其中第二篇文章所使用的gspca驱动程序（支持多种设备）
“
[使gspca支持自己的USB摄像头](http://www.eefocus.com/chenzhufly/blog/09-03/167364_3b2fe.html)
##### 在Linux下驱动USB摄像头一直不是容易的事情，尽管其内核中就集成了对0V511系列摄像头的支持，开源驱动spca5xx/gspca也提供了对大部分主流摄像头的支持，但还是无法涵盖所有品牌的各种型号的摄像头，尤其是对于国内的中低端用户，很难找到直接可用的Linux下的驱动。
Gspca/Spca5xx, Spca5xx-LE WebCams Driver 所支持的摄像头型号列表[http://mxhaard.free.fr.sixxs.org/spca5xx.html](http://mxhaard.free.fr.sixxs.org/spca5xx.html)
“
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
　ifconfig 命令用于查看和更改网络接口的地址和参数
## [linux创建用户命令](http://blog.sina.com.cn/s/blog_49a665e10100agby.html)
创建用户、设置密码、修改用户、删除用户：
**useradd testuser**  创建用户testuser
**passwd testuser**  给已创建的用户testuser设置密码
说明：新创建的用户会在/home下创建一个用户目录testuser
**usermod --help**  修改用户这个命令的相关参数
**userdel testuser**  删除用户testuser
**rm -rf testuser**  删除用户testuser所在目录
上面的几个命令只有root账号才可以使用，如果你不知道自己的系统上面的命令在什么位置可以使用如下命令查找其路径：
locate useradd
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
远程桌面：
[http://www.cnblogs.com/xdzone/archive/2011/03/10/1979644.html](http://www.cnblogs.com/xdzone/archive/2011/03/10/1979644.html)
