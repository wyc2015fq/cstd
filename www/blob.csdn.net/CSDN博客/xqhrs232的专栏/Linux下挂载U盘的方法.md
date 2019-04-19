# Linux下挂载U盘的方法 - xqhrs232的专栏 - CSDN博客
2016年04月28日 10:54:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：570
原文地址::[http://www.2cto.com/os/201411/352016.html](http://www.2cto.com/os/201411/352016.html)
相关文章
1、linux如何挂载U盘----[http://jingyan.baidu.com/article/f0062228dc2aa6fbd3f0c8ec.html](http://jingyan.baidu.com/article/f0062228dc2aa6fbd3f0c8ec.html)
# Linux下挂载U盘的方法
毫无疑问这是一个非常简单的问题，但是同样也是初学者肯定会碰到的问题，那么下面我们来了解一下linux下挂载U盘的方法（以Ubuntu14.04为例）
**本文的目录结构为：**
1、主流的Linux系统基本都已经支持自动挂载了，我们在哪里查看
2、我们对U盘挂载是意味着什么呢，一切设备接文件
3、我们如何挂载U盘，跟U盘的格式有关
## 1、主流的Linux系统基本都已经支持自动挂载
我们插入U盘之后，系统基本上就开始自动执行挂载任务了。要说怎么实现的，在网上找了几个说法，各异，暂就不讨论了。最后的情况就是U盘基本上是挂载到了./media/*wangzx(userName)*/U盘名/。
![\](http://www.2cto.com/uploadfile/Collfiles/20141114/201411140919196.jpg)
## 2、我们对U盘实施挂载意味着什么
（个人理解，不敢武断）Linux系统将所有设备都当做是文件，也就是“一切设备皆文件”。那么我们插入一个U盘的时候，便可以在/dev/这个目录中找到这个U盘设备，但是这个设备在没有挂载的之前，我们还不能使用，所以，挂载是为了使用（也就是打开）这个U盘设备，挂载在哪儿意味着我们在哪儿可以像打开一个文件一样的打开它，当然它依旧与一般的文件不一样，“属性”为“设备”！这，就是对U盘实施挂载的作用
![\](http://www.2cto.com/uploadfile/Collfiles/20141114/201411140919207.jpg)
我们通过fdisk这个Linux的命令可以查看到有关硬盘设备的相关信息，这里自身的硬盘表示一般为sda，而U盘的标示一般是sdb。我不清楚他这个是怎么区别开来的；可能是根据连接的接口不同——硬盘一般情况是stata接口，而U盘或者移动硬盘一般都是USB接口，所以外接设备我们通常可以考虑sdb
命令：sudo fdisk -l /dev喎�"http://www.2cto.com/kf/ware/vc/" target="_blank" class="keylink">vc2RiPC9wPgo8cD48aW1nIHNyYz0="http://www.2cto.com/uploadfile/Collfiles/20141114/201411140919208.jpg" alt="\">
这个命令可以较为清楚的显示sdb sda代表的存储设备的相关信息。前面的sudo是以root权限执行，这里我曾考虑到用root权限，但是貌似Ubuntu的机制是一般不允许用root身份登录（以安全之名，当然可以通过设置绕开）
【注】**关于创建Ubuntu的root身份**
创建root身份在Unbutn中的实现：sudo passwd root 创建后输入密码，然后通过su root就可以切换到root身份了
<img src="http://www.2cto.com/uploadfile/Collfiles/20141114/201411140919209.jpg" alt="n块噁膻≯嶷熼λkk?-畨鄌�-畨帼歓糙铻於�'凒Х�喎�" http:="" www.2cto.com="" os="" "="" target="_blank" class="keylink" style="border-width: 0px; padding: 0px; margin: 0px auto; list-style:
 none; display: block; width: 340px; height: 146px;">系统的入口（或者我认为是树形文件目录的根节点的位置）。我们可以在自己想挂载的目录下面新建一个文件夹，例如：mkdir /mnt/usb，然后我们就考虑将U盘这个设备挂载在这个目录下面。
（b）如何挂载跟U盘的文件系统格式有关系（但是目测现在大部分的U盘格式一般是fat32的）
**fat16格式**
**mount -t ms[dos](http://www.2cto.com/os/dos/) /dev/sdb4 /mnt/usb/**
**fat32格式**
**mount -t vfat /dev/sdb4/mnt/usb/**
**ext2格式**
**mount -t ext2 /dev/sdb4 mnt/usb**
现在我们就可以通过打开usb这个文件夹来访问U盘了，cd /mnt/usb……里面就是我们想要的东西了。其实我们同时还可以选择挂载在其他位置都是没有问题的：mount -t vfat /dev/sdb4 /home/wangzx/usb,这样我们也同样可以开发wangzx下面的这个usb文件夹打开U盘 cd /home/wangzx/usb。
