
# Windows10+Ubuntu双系统安装[多图] - 郭云飞的专栏 - CSDN博客


2016年07月05日 18:01:18[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：694个人分类：[计算机应用																](https://blog.csdn.net/guoyunfei20/article/category/1848145)


最近因为毕设重新回归Ubuntu，手头有一台装了Win10的`ThinkPad X240s`，最终成功完成了`Windows 10 教育版`和`Ubuntu Kylin 15.10`的双系统配置，下文(多图慎入)是我完成整个过程的手记。
### 安装方式
Ubuntu是很多Linux初学者最理想的选择，如果你恰好对Windows系列审美疲劳或者累觉不爱，那就要听好，有三种方法助你走进Ubuntu新世界。
虚拟机安装
原料：Ubuntu Kylin的[ISO](http://www.ubuntukylin.com/downloads/)、[VMware](https://my.vmware.com/cn/web/vmware/downloads)或[VirtualBox](https://www.virtualbox.org/wiki/Downloads)
优点：一条龙服务，安全简单
缺点：用户体验差，隔靴搔痒
Wubi安装
原料：Ubuntu Kylin的[ISO](http://www.ubuntukylin.com/downloads/)、wubi.exe
优点：即点即用，默认Windows开机引导
缺点：易死机，待机和网络易出问题
U盘安装
原料：Ubuntu Kylin的[ISO](http://www.ubuntukylin.com/downloads/)、[UltraISO](http://cn.ultraiso.net/xiazai.html)、[EasyBCD](http://neosmart.net/EasyBCD/)、U盘(>=2G)
优点：简单安全，正牌双系统
缺点：默认Ubuntu开机引导

### 安装环境
本人的原则是，既然玩就玩最正宗的，so我选择U盘镜像双系统。
![](http://upload-images.jianshu.io/upload_images/671333-c8e08a5e8b5553a2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
系统属性

### 游戏开始
下面正式开始安装，step by step。
数据备份
先别着急，你备份了吗？如果你看到这里，说明你选择了风险最大的一条路，在游戏开始之前，一定要做好数据备份，数据备份，数据备份。
创建磁盘分区
按住Win + X，选择“磁盘管理”:
![](http://upload-images.jianshu.io/upload_images/671333-dc6d1df4a0cdeeb9.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
磁盘管理概览

选择剩余空间较大的可分配磁盘，右键并选择“压缩卷”，这里选择压缩E盘50G左右的空间：
![](http://upload-images.jianshu.io/upload_images/671333-62656bf485dfa53b.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
压缩卷

点击“压缩”之后，E盘后部出现黑色的50G“未分配空间”：
![](http://upload-images.jianshu.io/upload_images/671333-774209de6270a208.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
50G未分配空间

至此，磁盘分区过程完成。
禁用快速启动(可选)
按住Win + X(请记住这个万能的组合)，选择“电源选项”，依次执行：
![](http://upload-images.jianshu.io/upload_images/671333-837bb5e8df95cfa7.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
"选择电源按钮的功能"

![](http://upload-images.jianshu.io/upload_images/671333-a2f098c304adf7a3.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
"更改当前不可用的设置"

![](http://upload-images.jianshu.io/upload_images/671333-d8c1eddc38c86f02.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
取消选择"启用快速启动(推荐)"

注： “快速启动”是Windows 8时代引进的新特性，建议关闭该特性的原因是，“快速启动”会影响Grub开机引导过程，可能出现无法载入Ubuntu的状况，最后选择“保存修改”。
禁用安全启动(Secure Boot)
进入系统“设置”，依次执行：
![](http://upload-images.jianshu.io/upload_images/671333-9ad9e1bbe89a46b9.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
更新和安全

![](http://upload-images.jianshu.io/upload_images/671333-f6d30476f9890b87.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
恢复>高级启动>立即重启

![](http://upload-images.jianshu.io/upload_images/671333-a4c2b221482662ce.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
疑难解答

![](http://upload-images.jianshu.io/upload_images/671333-79c14596b8e2ff01.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
高级选项

![](http://upload-images.jianshu.io/upload_images/671333-8744a1856b523ffb.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
启动设置

执行重启(并不是什么都不做)。
![](http://upload-images.jianshu.io/upload_images/671333-1e32fb618b44052d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
启动设置

大部分机器默认是关闭Secure Boot的，如果不放心，直接重启进Boot，简单粗暴：
![](http://upload-images.jianshu.io/upload_images/671333-dc6247170f77acb5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Boot>Security>Secure Boot

![](http://upload-images.jianshu.io/upload_images/671333-c55a0714e274bc38.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
确保Secure Boot是Disabled

注：同样的，“安全启动”也是Windows 8时代为了防范RootKit病毒所采取的安全措施，但也阻止了Windows和其他操作系统的双启动，因此在载入Ubuntu镜像之前，务必确保“安全启动”已禁用。
制作Ubuntu的启动U盘
（用U盘安装过操作系统的童鞋可以跳过这一步）
备份待写入的U盘
进入UltraISO，打开文件：
![](http://upload-images.jianshu.io/upload_images/671333-e92ae2799befcd4d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
打开镜像文件

启动>写入硬盘映像：
![](http://upload-images.jianshu.io/upload_images/671333-b48b97bc234b3d9a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
写入硬盘映像

按默认值写入：
![](http://upload-images.jianshu.io/upload_images/671333-eb7ee3645028df4f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
写入硬盘映像

完成写入：
![](http://upload-images.jianshu.io/upload_images/671333-0632d96cf8a98934.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
完成硬盘映像写入

U盘安装Ubuntu
万事俱备，跟往常重装系统一样，插上U盘，根据机器找到进入Boot的快捷键(我的是F1)，下面step by step详解。
找到镜像U盘，调整Priority Order，Save and Exit：
![](http://upload-images.jianshu.io/upload_images/671333-52245da80b6a24fe.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Boot Priority Order

注：下次restart记得重置，否则无限循环。
![](http://upload-images.jianshu.io/upload_images/671333-b2c03e24a877eb5b.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
炫酷的起始画面
选择“安装Ubuntu Kylin”：
![](http://upload-images.jianshu.io/upload_images/671333-930c13649fd95892.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
欢迎页

完成默认设置：
![](http://upload-images.jianshu.io/upload_images/671333-316c38f12e231f9c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
准备事项

注：如果网络和空间匀速，建议选择“安装中下载更新”和“安装这个第三方软件”。
非常重要的一步，选择“其他选项”：
![](http://upload-images.jianshu.io/upload_images/671333-7f565024748264af.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
安装类型选择

为空闲磁盘分区：
在这一步会看到我们之前分配的未使用磁盘空间，我们即将为这块空闲磁盘分区，为了更方便理解接下来的操作，这里简单介绍一下安装过程所涉及到的几个主要的Linux分区：
/：存储系统文件，建议10GB ~ 15GB；
swap：交换分区，即Linux系统的虚拟内存，建议是物理内存的2倍；
/home：home目录，存放音乐、图片及下载等文件的空间，建议最后分配所有剩下的空间；
/boot：包含系统内核和系统启动所需的文件，实现双系统的关键所在，建议200M。
![](http://upload-images.jianshu.io/upload_images/671333-efa9c15dce9a2366.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
选择空闲磁盘

选定空闲磁盘，点击+，首先分配16G空间给/分区，选择“主分区”、“空间起始位置”、Ext4和“挂载点/”：
![](http://upload-images.jianshu.io/upload_images/671333-b66a114eca0c8aee.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建/分区
注：实际上，一块硬盘最多容纳4个主分区，或3个主分区外加1个扩展分区，在选择分区类型时，可能会出现“安装系统时空闲分区不可用”状况，为了解决问题，下面一律选择“逻辑分区”。
重复创建步骤，分配16G空间给swap分区，选择“逻辑分区”(主分区已满)、“空间起始位置”、用于“交换空间”：
![](http://upload-images.jianshu.io/upload_images/671333-8e62830d2d99a979.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建交换分区
接着分配200M空间给/boot分区，选择“逻辑分区”(主分区已满)、“空间起始位置”、“Ext4”和“挂载点/boot”：
![](http://upload-images.jianshu.io/upload_images/671333-3245cb65c8097b19.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建/boot分区
最后将所有剩余空间分配给/home分区，选择“逻辑分区”(主分区已满)、“空间起始位置”、“Ext4”和“挂载点/home”：
![](http://upload-images.jianshu.io/upload_images/671333-c70f4de86b3d6db5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建/home分区
选择/boot对应的盘符作为“安装启动引导器的设备”，务必保证一致：
![](http://upload-images.jianshu.io/upload_images/671333-f4cecdad1256398f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
确保引导设备编号和/boot一致
将改动写入磁盘，去喝杯咖啡：
![](http://upload-images.jianshu.io/upload_images/671333-e60e325a26327ab6.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

咖啡喝完，刚好进入最后的设置：
![](http://upload-images.jianshu.io/upload_images/671333-611c071e24380048.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
地理位置

![](http://upload-images.jianshu.io/upload_images/671333-cc8b9e0e50fefef9.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
键盘布局

![](http://upload-images.jianshu.io/upload_images/671333-159aa5ed54bb2977.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
用户信息

![](http://upload-images.jianshu.io/upload_images/671333-abe8611d11530843.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
欢迎界面

![](http://upload-images.jianshu.io/upload_images/671333-0829a0d732bb9991.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
大功告成

重启系统，进入Windows完成最后的引导设置。
EasyBCD引导Ubuntu
进入EasyBCD，选择“添加新条目”，选择Linux/BSD操作系统，在“驱动器”栏目选择接近200M的Linux分区：
![](http://upload-images.jianshu.io/upload_images/671333-ddffde46eed192e9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
EasyBCD设置

完成条目添加后，重启电脑，会发现Windows10和Ubuntu的双系统已经完成安装，祝玩得开心！
![](http://upload-images.jianshu.io/upload_images/671333-f3df520a5db6ab7b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
添加之后的条目
![](http://upload-images.jianshu.io/upload_images/671333-b778f990a9319307.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Windows引导的开机画面
![](http://upload-images.jianshu.io/upload_images/671333-c221037a9a4731f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Ubuntu Kylin桌面

### 最后
用Windows引导Ubuntu最大的好处就是，当不再需要Ubuntu的时候，直接在Windows磁盘管理中将其所在所有分区删除，然后将EasyBCD中对应条目删除即可。关于Ubuntu引导Windows的方法，如果大家感兴趣，欢迎尝试和分享。

文／Volcanoo（简书作者）
原文链接：http://www.jianshu.com/p/2eebd6ad284d\#
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。

