# Linux学习笔记(一)：Win10上用VMware12虚拟机安装Linux-CentOS7 - weixin_33985507的博客 - CSDN博客
2018年09月23日 15:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
  我教材参考：鸟哥的Linux私房菜.
  参考博客：[https://blog.csdn.net/qq_39521554/article/details/79428386](https://blog.csdn.net/qq_39521554/article/details/79428386)
> 
##### VMware简介：
  VMware是一个虚拟PC的软件，可以在现有的操纵系统上虚拟出一个新的硬件环境，相当于模拟出一台新的PC，以此来实现在一台机器上真正同时运行两个独立的操作系统。
  VMware的主要特点：
   - 不需要区分或重新开机就能在同一台PC上使用两种以上的操作系统；
   - 本机系统可以与虚拟机系统网络通信；
   - 可以设定并且随时修改虚拟机操作系统的硬件环境。
> 
##### CentOS简介：
   CentOS（Community ENTerprise Operating System）是Linux发行版之一，它是来自于Red Hat Enterprise Linux依照开放源代码规定释出的源代码所编译而成。由于出自同样的源代码，因此有些要求高度稳定性的服务器以CentOS替代商业版的Red Hat Enterprise Linux使用。两者的不同，在于CentOS并不包含封闭源代码软件。
> 
##### CentOS安装：
虚拟机版本：VMware Workstation_12
链接：[https://pan.baidu.com/s/1BaqcGw_PmZhDQ6OyM-vF1A](https://pan.baidu.com/s/1BaqcGw_PmZhDQ6OyM-vF1A) 密码：8pf9
Linux系统版本：CentOS_7（64位）
链接：[https://pan.baidu.com/s/1Lygr17HLArvq4xXrGEdslw](https://pan.baidu.com/s/1Lygr17HLArvq4xXrGEdslw) 密码：otyn
物理机版本：Win 10
- - 安装虚拟机：双击setup文件，按照步骤安装即可
- - 
打开VMware,点击创建新的虚拟机或者点击文件——新建虚拟机；
![5631876-d1f4fd0a25d85146.png](https://upload-images.jianshu.io/upload_images/5631876-d1f4fd0a25d85146.png)
- - 
默认选择典型，点击下一步；
![5631876-761f1dbdb5973c72.png](https://upload-images.jianshu.io/upload_images/5631876-761f1dbdb5973c72.png)
- - 
点击安装程序光盘映像文件-选择下载好的CentOS的iso文件路径，再点击稍后安装操作系统，点击下一步；
![5631876-eeaf0376765a6bd9.png](https://upload-images.jianshu.io/upload_images/5631876-eeaf0376765a6bd9.png)
- - 
客户操作系统选择Linux，版本选择对应的版本，这里选择的是CentOS 64位，点击下一步；
![5631876-1ac8682a82680d08.png](https://upload-images.jianshu.io/upload_images/5631876-1ac8682a82680d08.png)
- - 
虚拟机名称可以用默认名称，选择虚拟机存放的路径，最好不要存放在系统盘，点击下一步；
![5631876-0f0c3e0e52bc5682.png](https://upload-images.jianshu.io/upload_images/5631876-0f0c3e0e52bc5682.png)
- - 
磁盘大小和拆分文件选择默认即可，也可以根据自己需要修改，点击下一步；
![5631876-18376dc3929a6b9d.png](https://upload-images.jianshu.io/upload_images/5631876-18376dc3929a6b9d.png)
- - 
点击自定义硬件进行设置，也可以后面再进行设置。
![5631876-62b9409f7f97c9d1.png](https://upload-images.jianshu.io/upload_images/5631876-62b9409f7f97c9d1.png)
- - 
将处理器设置为4核，也可以不修改，将新CD/DVD(IDE)的连接选择ISO镜像文件，并选择下载好的CentOS的iso镜像文件路径，点击关闭，再点击完成；
![5631876-7efd569c7e1e8bce.png](https://upload-images.jianshu.io/upload_images/5631876-7efd569c7e1e8bce.png)
- - 
右击虚拟机CentOS 64位，选择电源——点击打开电源时进入固件；
![5631876-bc58587eaa0b52ee.png](https://upload-images.jianshu.io/upload_images/5631876-bc58587eaa0b52ee.png)
- - 
选择安装CentOS 7,接着回车；
![5631876-fff93ab603b21579.png](https://upload-images.jianshu.io/upload_images/5631876-fff93ab603b21579.png)
系统安装中
![5631876-d9f612ea91f7c0ee.png](https://upload-images.jianshu.io/upload_images/5631876-d9f612ea91f7c0ee.png)
- - 
选择简体中文，点击继续；
![5631876-354c487cc5bf24a6.png](https://upload-images.jianshu.io/upload_images/5631876-354c487cc5bf24a6.png)
- - 
等待界面变成如图所示，点击开始安装
![5631876-88d088f6c01e2d80.png](https://upload-images.jianshu.io/upload_images/5631876-88d088f6c01e2d80.png)
- - 
进入下面界面，设置ROOT密码
![5631876-a08c64cd3fef4c62.png](https://upload-images.jianshu.io/upload_images/5631876-a08c64cd3fef4c62.png)
- - 
等待安装完成
![5631876-83ae1738acc3aa96.png](https://upload-images.jianshu.io/upload_images/5631876-83ae1738acc3aa96.png)

