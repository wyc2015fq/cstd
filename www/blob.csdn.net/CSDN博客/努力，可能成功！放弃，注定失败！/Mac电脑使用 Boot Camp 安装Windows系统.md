# Mac电脑使用 Boot Camp 安装Windows系统 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月24日 09:42:03[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：6249
昨天公司一个做文秘的同事由于公司给配的电脑很不给力，用个office，再打开一两个其他的软件就卡的要死，最后实在是受不了，就差砸电脑了。所以只好把她12年末买的一款2012年中的MacBook Pro拿过来准备安装win7（mac电脑虽好，但是对于经常用office做编辑的人来说，mac电脑显然不能够满足工作的需要），看了她macPro的配置(处理器：2.9 GHz Intel Core i7，8 GB内存，硬盘容量750G)之后第一反应是真土豪啊，居然把这么好的电脑藏在家中不用，太浪费了。虽然之前也没安装双系统，但是作为一个程序猿技术男，此时正是一展身手的好机会啊，好让妹子们也崇拜下咋们，给咋们程序猿长长脸。
# 前言
本篇文章将教你一步一步在MAC电脑上面安装windows系统，尽管作为一个iOS开发者用mac笔记本确实体验很好，但是有时确实有特殊的需求需要用windows系统。(本文将以Mac机型为：MacBook Pro 13 英寸，2012 年中 的电脑安装win7 64位为例演示，切记：**安装windows系统之前必须先确定你的mac电脑能否匹配windows系统版本**，[点击这里查看](https://support.apple.com/zh-cn/HT205016))
# 准备工作
- 一个64位的win7的ISO镜像文件，[点击这里](http://pan.baidu.com/s/1boks4tp)下载我在百度云盘共享一个win7镜像；(反正就是得下载一个靠谱的iso镜像，我第一次安装失败就是因为在网上随便下载的镜像是坏的，导致最后提示window未能启动)
- 两个U盘或者移动硬盘，一个16G已上 MS-DOS (FAT) 格式的作为系统安装盘，和一个8G已上 MS-DOS (FAT) 格式的的装驱动程序的驱动盘。有移动硬盘尽量用移动硬盘，因为速度快，为你节省时间；(第一次操作过程很慢的一部分原因的就是因为U盘不给力，导致整个安装过程非常慢，后面果断换成的)
- 互联网连接，网速越快，安装过程速度越快！(白天第一次安装失败，过程慢，等得我是非常痛苦，下班后果断回家弄，100M的网，这速度溜溜的，整个过程不到两小时就安装好了) 。
- 首次安装 Windows 时，建议留出 30 GB 可用磁盘空间，如果您是从之前版本的 Windows 升级，则建议留出 40 GB 的可用磁盘空间
# win7系统安装
## 一、系统安装盘的准备：(此步骤在需要安装双系统的mac电脑上操作)
- 插入系统盘，将之前下载好的win7的ISO镜像文件拷贝到桌面，然后删除系统盘的ISO镜像文件，是系统盘成为空白盘；
- 用Spotlight搜索Boot Camp助理，或者打开launchpad--》其他--》Boot Camp助理 ,打开的界面如下：
![](http://upload-images.jianshu.io/upload_images/1603505-e396e46cfe150a0f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
bootcamp.png
- 点击继续，按下图进行勾选（**切记只勾选前两个，最后一个不要勾选**）
![](http://upload-images.jianshu.io/upload_images/1603505-2c1f5fd62b7db452.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step2.jpg
- 点击继续，出现以下界面
![](http://upload-images.jianshu.io/upload_images/1603505-e5678c91596aa47d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step3.jpg
点击选取，选择之前保存在mac电脑桌面的ISO镜像文件，目的磁盘就是那个系统安装盘，并且保持与你的Mac的连接，直到windows安装完成
- 点击继续，会出现以下界面（整个过程将会持续至少半个小时，这取决于你的U盘和网速，我在这步都要疯掉了，等了一个多小时都没成功，果断换了新的U盘和100M网络）
![](http://upload-images.jianshu.io/upload_images/1603505-d69a885ee6cd58d2.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step4.jpg
- step4.jpg完成之后，会弹出以下界面：
![](http://upload-images.jianshu.io/upload_images/1603505-3879938d7cc1c869.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step5.jpg
- 至此为止，系统安装盘已经准备好了，点击退出关闭“Boot Camp助理”窗口以退出此应用。
## 二、驱动盘的准备：(因为上一步骤需要部分时间，所以在进行上一步骤的同时，你可以另一台电脑上面进行此步骤的操作，节省时间)
- 从苹果官网上[按Mac机型列出的BootCamp要求]()来查找您所用的 Windows 版本和 Mac 所需的 Windows 支持软件（驱动程序）如下图：
![](http://upload-images.jianshu.io/upload_images/1603505-1566fd38a037cb52.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step6.png
- 点击step6.png红色箭头指向的‘5’将跳转到下载页面，如下图：
![](http://upload-images.jianshu.io/upload_images/1603505-83087977b14b9536.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step7.png
- 点击下载，得到的是zip包，解压之后的BootCamp5文件夹（这就是需要下载的驱动程序）如下：
![](http://upload-images.jianshu.io/upload_images/1603505-815f18c341ee825a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step8.png
- 将上一步得到的BootCamp5文件保存到事先准备好的驱动盘中。
## 三、正式安装：（系统安装盘一直连接在电脑上，直到安装win7成功）
- 将驱动盘的BootCamp5文件拖动到系统安装盘中，在系统提示您是否要替换系统安装盘上的现有项目时，请点按“是”。
- 再次打开“Boot Camp 助理”，然后点按“继续”。
- 切记仅勾选第三个，即仅选择“安装 Windows7或更高版本”选项。
![](http://upload-images.jianshu.io/upload_images/1603505-435a653c1c26bfcc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step9.png
- 点击继续，然后按照屏幕提示重新对您的驱动器进行分区，然后安装 Windows。
- 完成该助理的操作后，Mac 会重新启动至 Windows 安装程序。
![](http://upload-images.jianshu.io/upload_images/1603505-b1be47f39b49069a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
安装.jpg
![](http://upload-images.jianshu.io/upload_images/1603505-31c9ab7519990a69.JPG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
安装2.JPG
- 在系统询问您要在何处安装 Windows 时，请选择 BOOTCAMP 分区，然后点按“驱动器选项”，并对您的 Boot Camp 分区进行格式化。如下图：
![](http://upload-images.jianshu.io/upload_images/1603505-7e6f8900108bc268.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step10.jpg
![](http://upload-images.jianshu.io/upload_images/1603505-4b5736344b9aaea0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step11.png
- 点击下一步将出现以下界面：(此步骤也需要等待一段时间)
![](http://upload-images.jianshu.io/upload_images/1603505-16bf523d0189b77d.JPG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step12.JPG
- 等待一段时间后，也就大功告成了。
![](http://upload-images.jianshu.io/upload_images/1603505-b62505845a78c904.JPG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step13.JPG
## 四、安装驱动（因为到这步为止，电脑还不能联网，有线网，无线网络都连不上，所以我怀疑没有网卡驱动）
- 苹果官方给的操作步骤：
window安装成功后但是还没有任何驱动，是不能上网的，这时需要安装系统盘中的驱动，按照step7.png给的步骤操作，
![](http://upload-images.jianshu.io/upload_images/1603505-43be283b303b9ad2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step13.png
- 我实际的操作
从我的驱动盘中找到并安装无线网卡驱动然后就可以上网了，如下图：
![](http://upload-images.jianshu.io/upload_images/1603505-73feb46e9a86167a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
step14.png
最后交由驱动精灵来给我win7系统安装其他驱动和打系统补丁。
## 到此你的mac电脑双系统也就安装成功了，可以愉快的玩耍了，
# 安装失败的解决方法
- 我第一次安装是失败了，在安装过程中出现了下图的问题，最后只好重新安装，并且换了一个win7系统镜像文件。
![](http://upload-images.jianshu.io/upload_images/1603505-cf92b034f049a90d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
fail.jpg
第一步，此时需要先把电脑强制关机，就是按住电源键；
第二步，再开机，听到开机声或者白屏之后按住option按键或者一直按住option键，直到看到启动磁盘选项，选择Mac的系统盘就可以了；
第三步，进入 Boot Camp助理选项，前面的选项不要勾选，只勾选最后的删除win选项，
第四步，重新按照我说的步骤操作进行win7系统安装。
> 
我们终此一生，就是要摆脱他人的期待，找到真正的自己;
记录学习的点滴，以此勉励不断奋斗的自己✌️✌️✌️
