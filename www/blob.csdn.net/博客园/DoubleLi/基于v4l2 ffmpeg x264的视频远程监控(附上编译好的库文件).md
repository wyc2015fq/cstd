# 基于v4l2 ffmpeg x264的视频远程监控(附上编译好的库文件) - DoubleLi - 博客园






说明：主要是基于ghostyu网友整理的《 [arm mini2440 基于v4l2 ffmpeg x264的视频远程监控](http://blog.csdn.net/ghostyu/article/details/7371310)》。自己做了一遍，遇到不少问题，就整理记录下来。







**1、平台**


> 

硬件：arm mini2440，usb摄像头（UVC免驱）



软件：ubuntu12.10，arm-[Linux](http://lib.csdn.net/base/linux)-gcc v4.4.3






**2、源文件**


> 

功能体验包：[http://download.csdn.net/detail/kangear/5202160](http://download.csdn.net/detail/kangear/5202160)

－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

源码编译包：

[ffmpeg-1.0.tar.bz2](http://download.chinaunix.net/download/0003000/2990.shtml) （ghostyu用的是-0.10.1版本的不过我没有编译通过，改为1.0的可以编译通过了）



last_x264.tar.bz2 (`[Git](http://lib.csdn.net/base/git) clone [git](http://lib.csdn.net/base/git):``//git``.videolan.org``/x264``.git 获得` )



[zlib-1.2.5.tar.gz](http://ishare.iask.sina.com.cn/f/13924714.html) (mini2440中没有libz.so，但是程序运行时需要这个库支持，所以下载源文件交叉编译）



网友sunkwei写的webcam应用程序，下载地址：



[http://download.csdn.net/detail/sunkwei/3425209](http://download.csdn.net/detail/sunkwei/3425209)



> 下面开始交叉编译，编译过程中会遇到几个小问题，一一解决
**3、功能体验**
         ARM版本的库和应用程序都已经打包，可以直接运行，也可以自己编译。对应包中的kangear_webcam/Server/arm目录下的webcam.tar.gz，解压后对应的目录如下：




> 

webcam-|



               |_Server    //arm端的可执行程序加源码



               |_ffmeg      //制作好的库文件（包括ffmpeg，x264，zlib，用于编译或者应用程序的运行）



               |_webcam  //在开发板上运行此shell脚本会设置一些环境变量并运行Server目录下的webcam_server。




        这个包解压后放到开发板上，把webcam这个shell脚本中的ip改成linux pc的ip就可以运行了。

        关于PC端的显示程序可以参考《[基于v4l2的webcam应用, 本地预监(编译过程)](http://blog.csdn.net/kangear/article/details/8721068)》，安装PC端的环境。然后在kangear_webcam/Client执行make就可以运行webcam_shower。（这个Client程序我加入了LINUX PC的IP自动获取，不需要再改IP）。

         运行效果：

![](http://img.my.csdn.net/uploads/201303/27/1364354782_5616.png)



4、库文件和编译

          以下转自《 [arm mini2440 基于v4l2 ffmpeg x264的视频远程监控](http://blog.csdn.net/ghostyu/article/details/7371310)》只是修改了原作者的几处笔误。


。。。最后想了想还是好好整理一下，单独写一篇文章吧。整理好就可以安心研究源码了。



**(20130809补充)关于USB摄像头型号：**

　　　很多人问我关于摄像头型号的问题，主要是说zc301的不支持，我今天没有事，就特意再次试了一下，确实是不支持的：

![](http://img.blog.csdn.net/20130809221133953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2FuZ2Vhcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









