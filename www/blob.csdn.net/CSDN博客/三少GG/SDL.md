# SDL - 三少GG - CSDN博客
2011年05月26日 15:58:00[三少GG](https://me.csdn.net/scut1135)阅读数：838标签：[android																[shell																[手机																[远程登录																[linux																[eclipse](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=远程登录&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
一 .百度百科 http://baike.baidu.com/view/8348.htm
- 扩展阅读：
- 1
[http://www.libsdl.org/](http://www.libsdl.org/)
 SDL官方网站
- 2
[http://lazyfoo.net/SDL_tutorials/index.php](http://lazyfoo.net/SDL_tutorials/index.php)
 非常好的一个SDL教程（英文）
- 3
[http://www.cppblog.com/lf426/category/6107.html](http://www.cppblog.com/lf426/category/6107.html)
 SDL入门教程（中文）
二. SDL官方下载 及帮助文档
[http://www.codesourcery.com/sgpp/lite/arm/portal/release1803](http://www.codesourcery.com/sgpp/lite/arm/portal/release1803)
三. Android 移植
[http://blog.sina.com.cn/s/blog_4a0a39c30100awgs.html](http://blog.sina.com.cn/s/blog_4a0a39c30100awgs.html)
[](http://blog.sina.com.cn/s/blog_4a0a39c30100b1n1.html)
[http://blog.sina.com.cn/s/blog_4a0a39c30100b1n1.html](http://blog.sina.com.cn/s/blog_4a0a39c30100b1n1.html)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ADB
ADB: Android debug bridge. 
Android手机实际是基于Linux系统的。
在你的android手机的设置中，Settings->Applications->Development->USB debugging勾选上，就可以允许PC通过adb shell命令远程登录你手机的linux系统。
这时通过USB线将android手机与电脑连起来，在电脑上dos命令行中敲adb shell命令，可以远程登录到android手机的linux系统中。
成功登陆到android手机的系统中后，就可以运行一些简单的linux命令，比如'ls'查看目录中的文件，‘cd xxx’进入到xxx目录， 'exit'退出登录等等。
adb shell 命令将打开一个Linux "壳" shell。从中可以输入要在设备上运行的其他命令 
**       疑问一：adb是什么****　    疑问二：adb有什么用?****    　疑问三：作为最关键的问题，adb工具如何用?**
**答疑一**
：adb的全称为Android Debug 
Bridge，就是起到调试桥的作用。通过adb我们可以在Eclipse中方面通过DDMS来调试Android程序，说白了就是debug工具。
adb的工作方式比较特殊，采用监听Socket TCP 
5554等端口的方式让IDE和Qemu通讯，默认情况下adb会daemon相关的网络端口，所以当我们运行Eclipse时adb进程就会自动运行。
**答疑二**
：借助adb工具，我们可以管理设备或手机模拟器的状态。还可以进行很多手机操作，如安装软件、系统升级、运行shell命令等等。其实简而言说，adb就是连接Android手机与PC端的桥梁，可以让用户在电脑上对手机进行全面的操作。
**答疑三**
：这也是今天这篇教程的关键所在，下面我会为大家介绍一下adb工具如何操作，并介绍几个常用命令以备大家参考!
**　【软件安装】**
**　　步骤1：安装USB驱动**
这个相关步骤其实笔者已经在Android前系列教程中介绍，故今天简要叙述一下过程。进入设置-应用
程序-开发-USB调试，将第一个选项打钩选中。然后通过USB线连接电脑，提示安装驱动。如果是32位的系统选择x86文件夹安装驱动，如果是64位系
统选择amd64文件夹。
**　　步骤2：软件准备**
将提供的软件工具包中的TMG文件夹，放到C盘根目录下。之所以放于此不是指定位置，只是为了后续在MS-DOS中进入文件夹带来方便。文件夹路径：C:/TMG/
![Android系列教程(三) 如何获取Root权限](http://cimg2.163.com/catchimg/20091105/9033955_1.jpg)
图为：软件素材准备
**      【操作命令】**
**　　1. 查看设备**
**　　adb devices**
这个命令是查看当前连接的设备, 连接到计算机的android设备或者模拟器将会列出显示
![Android系列教程(六) adb操作命令详解](http://cimg2.163.com/catchimg/20091105/9033955_2.png)
**　　2.安装软件**
**　　adb install**
adb install <apk文件路径> :这个命令将指定的apk文件安装到设备上
![Android系列教程(六) adb操作命令详解](http://cimg2.163.com/catchimg/20091105/9033955_3.png)
**　　3. 卸载软件**
**　　adb uninstall <软件名>**
**　　adb uninstall -k <软件名>**
如果加 -k 参数,为卸载软件但是保留配置和缓存文件.
**　　4. 进入设备或模拟器的shell：**
**　　adb shell**
通过上面的命令，就可以进入设备或模拟器的shell环境中，在这个Linux Shell中，你可以执行各种Linux的命令，另外如果只想执行一条shell命令，可以采用以下的方式：
**　　adb shell [command]**
如：adb shell dmesg会打印出内核的调试信息。
**　　5. 发布端口**
可以设置任意的端口号，做为主机向模拟器或设备的请求端口。如：
adb forward tcp:5555 tcp:8000
**　　6. 从电脑上发送文件到设备**
**　　adb push <本地路径> <远程路径>**
用push命令可以把本机电脑上的文件或者文件夹复制到设备(手机)
**　　7. 从设备上下载文件到电脑**
**　　adb pull <远程路径> <本地路径>**
用pull命令可以把设备(手机)上的文件或者文件夹复制到本机电脑
**　　8、查看bug报告**
adb bugreport
**　　9、记录无线通讯日志**
一般来说，无线通讯的日志非常多，在运行时没必要去记录，但我们还是可以通过命令，设置记录：
adb shell
logcat -b radio
**　　10、获取设备的ID和序列号**
adb get-product
adb get-serialno
adb shell
sqlite3
