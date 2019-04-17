# Windows 7硬盘安装CentOS 6.4 双系统 （WIN7下硬盘安装Linux（Fedora 16，CentOS 6.2，Ubuntu 12.04）) - 在思索中前行！ - CSDN博客





2014年11月20日 15:07:44[_Tham](https://me.csdn.net/txl16211)阅读数：2633标签：[win7下安装Centos																[win7+Centos双系统](https://so.csdn.net/so/search/s.do?q=win7+Centos双系统&t=blog)](https://so.csdn.net/so/search/s.do?q=win7下安装Centos&t=blog)
个人分类：[鸟哥的私房菜学习																[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)](https://blog.csdn.net/txl16211/article/category/2590995)







**[WIN7下硬盘安装Linux（Fedora 16，CentOS 6.2，Ubuntu 12.04）](http://www.linuxidc.com/Linux/2012-05/60828.htm)**

       最近在看《鸟哥私房菜：基础学习篇》，觉得很不错，想要用U盘装个windows 7 和 CentOS 6.4的 双系统，在网上找了很多教程，觉得乱七八糟的，弄得很复杂，而且很多都不是很完整，对于新手实在是伤不起。最终自己用U盘装了两次，有一次还把引导程序装到U盘MBR去了。o(╯□╰)o 决定写篇博客，防记忆衰老，也为所有想学 linux 的小白贡献一点资料吧。O(∩_∩)O~
 （高手出门右转... = = ）




       那为什么选择U盘安装呢？首先，是因为我没有CD....然后，笔记本里面的硬盘分区全部是NTFS 的，而 linux 是无法识别 NTFS格式的硬盘的，如果选择硬盘安装，首先还要把放镜像文件的分区转为FAT32格式，然后继续进行一系列复杂的安装步骤。但很多U盘本来就是FAT32的，这不是刚好符合我们的需求吗？最后，是我已经习惯用U盘来装系统了.....




       因为没有自己截图，图片都是网上找的。有些图片不符合我会指出来。部分没有图片的请不要pass了，因为那正是不同之处。（PS：后来自己拍了几张图上来，没错。。。那些很丑的就是我拍的。。。）




      如果各位发现什么问题或无法按此篇博客安装成功，请在评论区交流吧！O(∩_∩)O~




      默认背景：已安装windows 7   磁盘分区格式：传统 MBR  （用 GPT 分区的同学出门右转...楼主帮不了你... = = ）




     鉴于 linux 的安装不像 windows 那样的傻瓜式，我们安装之前要有一定基础，让我们先进修一下：

     鸟哥的：[http://vbird.dic.ksu.edu.tw/linux_basic/0130designlinux_2.php](http://vbird.dic.ksu.edu.tw/linux_basic/0130designlinux_2.php)

     关于linux分区的：[http://www.cnblogs.com/chenlulouis/archive/2009/08/27/1554983.html](http://www.cnblogs.com/chenlulouis/archive/2009/08/27/1554983.html)

     了解了点硬件，规划好分区，我们开始迫不及待的安装啦。

不急不急，我们先分析一下我们的目标：用U盘安装 Windows 7 和 CentOS 6.4双系统。重点是“U盘安装”和“windows + linux 双系统”（废话）。那么，这有什么主要的不同和要点呢？

        首先，我们是U盘安装的，所以我们要把U盘制作成启动盘。网上很多教程弄得挺复杂，我们简简单单就可以了。

        然后，我们的硬盘已经被Windows占领，全部分区都是NTFS的。这可不行。所以，我们要从硬盘空间里切一块下来，作为空闲空间来安装我们的 CentOS，至于要多大，当然是越大越好咯，不过一般几十G就够了。

        接着，在选择哪种类型的安装时，我们要选择“**创建自定义布局**”，要是你点太快选择了默认的“使用全部空间”的话，你就哭吧！然后建立 linux 分区的时候，我们要注意在空闲空间上建立，可不要把其他分区给弄没了。然后建立分区时**“允许的驱动器”** 要只勾选自己电脑硬盘，别把U盘也算进去。

        还有很重要的一点是，安装grub程序时，你如果没有选择**更换设备**，那么系统会吧引导程序默认安装在U盘的MBR中，结果U盘拔了之后你会发现自己安装的CentOS“不见了”，连U盘都无法格式化了。所以，我们要谨记选择更换设备。而开机启动的选项名称和默认开机进入系统也在这一步进行设置的。

好了，就这么多！别紧张。我们下面还会继续强调这些要点的。




 安装大于4G的Centos 镜像难点介绍及解决方法:

 http://blog.csdn.net/markho365/article/details/8969591

1、难点介绍：

（1）linux系统能识别windows下FAT32文件系统，不能识别NTFS文件系统，所以在linux安装时，选择任何sda都不行，FAT32可以

（2）FAT32文件系统单个文件不能超过4G

（3）CentOS 6.4文件大于4G

结论：在win7下使用FAT32和NTFS系统存放安装ISO都不可行，则要想办法用工具分出一块linux文件系统，如：ext3，不受4G的限制




常规镜像安装所需工具:

一、UltraISO

不大，才2M多，用来制作 U 盘启动  [http://www.crsky.com/soft/1134.html](http://www.crsky.com/soft/1134.html)   用户名：王涛 注册码：7C81-1689-4046-626F

二、CentOS 6.4 32位系统镜像 

（大于4G的Centos镜像由于文件系统的原因不能安装，复杂的安装方法见[链接](http://blog.csdn.net/markho365/article/details/8969591)）

下载地址的详见 [http://www.metsky.com/archives/548.html](http://www.metsky.com/archives/548.html)
三、U 盘8G


安装步骤:

**一、制作 U 盘启动引导盘**

0、请确认你的U盘是 **FAT32 **格式的，因为 linux 无法识别 NTFS 格式的文件系统。如果不是FAT32，真不幸，你还是把格式转过来吧。




1、电脑插上 U 盘，打开 UltraISO 软件，文件---**打开**，选择你下载好的**CentOS-6.4-i386-bin-DVD1.iso** 文件，点**启动** --**写入硬盘镜像** ，在硬盘驱动器里面选择你的 U 盘，写入方式默认为:：** USB-HDD+**，不用改，,然后** 格式化**，完成后，关闭对话框，然后再点**写入**，完成后，关闭
 UltraISO。




2、删除 U 盘根目录下面的 Packages 文件夹（不删除也可以，如果你U盘空间足够大）



3、把下载好的系统镜像 CentOS-6.4-i386-bin-DVD1.iso 和 CentOS-6.4-i386-bin-DVD2.iso 复制到 U 盘根目录下面。

至此，U 盘启动盘制作完成。




**二、在硬盘留出空闲空间**




我们要给硬盘划分空白空间安装CentOS6.4，我们不需要什么乱七八糟的工具，**右键**点击**我的电脑**——**管理**——**存储**——**磁盘管理（本地） **

我自己的F盘有90G的空间，60GB 空闲，我用35GB来装CentOS，那么单击F盘，**右键选择“压缩卷”** ，然后在弹出的窗口里的“**输入压缩空间量**”输入35000，然后点**压缩**就可以了。这样我们就预留出了35G 的空闲空间，在我们待会进行安装的时候 CentOS就可以选择安装在这一部分空间。接下来就可以安装了。




**二、安装 CentOS 6.4**




1、开机设置 U 盘第一启动，重启之后会出来安装界面，有如下选项：



Install or upgrade an existing system 安装或升级现有的系统 

install system with basic video driver 安装过程中采用基本的显卡驱动 

Rescue installed system 进入系统修复模式

Boot from local drive   退出安装从硬盘启动

Memory test  内存检测




网上的教程都是默认选择第一项，但我除了第二项，其他选择后都出现了屏幕扭曲无法安装，最后还是选择第二项，然后安装成功了。所以，你可以**先选择第一项，如果不行的话，再改为第二项**。但安装过程都是一样的。

![](http://files.jb51.net/file_images/article/201303/1904.jpg)





2.出现是否对CD媒体进行测试的提问，这里选择**“Skip**”跳过测试。


![](http://files.jb51.net/file_images/article/201303/1905.jpg)





3、出现安装过程中使用的语言界面：

选择 ** Chinese(Simplified)**，简体中文（网上教程大都选择英文，防止出现部分乱码，但我安装并没有乱码，所以还是推荐用中文吧）

![](http://files.jb51.net/file_images/article/201303/1907.jpg)


4、出现语言确认界面直接回车




5、下面出现键盘模式选择界面，不要修改，进入下一步

![](http://files.jb51.net/file_images/article/201303/1908.jpg)





6、“Installation Method” 选择 “**Hard drive**”，然后**选择u盘所在的分区**，我的U盘在这里显示的是 /dev/sdb4 。这一步是系统寻找镜像地址，你选择**只有 一个分区的那个就是你的U盘了。**(如下只有一个 sdb）

![](https://img-blog.csdn.net/20140214104338812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamNqYzkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20140214104442656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamNqYzkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


7、直接点 OK，会出现安装界面:，点击下一步




8、选择基本存储设备

![](http://files.jb51.net/file_images/article/201303/1909.jpg)





9、设置计算机名，随你便吧
![](http://files.jb51.net/file_images/article/201303/1911.jpg)




10、设置时区，不用改

![](http://files.jb51.net/file_images/article/201303/1912.jpg)





11、设置root密码，这是最重要的密码，而且必须填，牢记你的密码！

![](http://files.jb51.net/file_images/article/201303/1913.jpg)





12、**注意**：选择最后一项，**创建自定义布局**

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111R63962-12.jpg)



13、**注意**：这里可以看到磁盘信息，切忌要确定在自己所要分区的磁盘上分区，否则其他磁盘会被格式化。这张图和本教程是不搭配的，但我自己没有截图，理论上来说安装时会在 ntfs 的中间出现一行空闲，谨记我们**建立分区要点在那个空闲上面，进行创建**，否则你就哭吧！我们是采用U盘安装所以还会出现一个sdb，但不用管它。

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111RB335-13.jpg)



　　注意：分区之前，自己先要规划好，怎么分区

        但是你也可以简单的划分一个 /boot（启动分区） 100-200 M    Swap（交换空间） 1 - 2G   / （根分区） 剩余空间大小

　　我的分区如下：

　　硬盘总共35G

　　/  8G

        /boot 100M

　　Swap2G(内存小于2G时，设置为内存的2倍;内存大于或等于2G时，设置为2G)

         /usr  6G

         /home  选择使用全部可用空间

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111RC3M-14.jpg)



**选中空闲分区**，点**创建**，选择**标准分区**，再点**创建**，**允许的驱动器:只勾选自己电脑磁盘，把u盘取消掉**（这一点和图片不一样）

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111R62c6-15.jpg)



　　挂载点：/

　　文件系统类型：ext4

　　大小：8000

　　确定

　　继续**选中空闲分区**，点**创建**

　　选择**“标准分区**”，点**创建**

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111RC9B-18.jpg)



　　文件系统类型：**swap**

　　大小：2000

**允许的驱动器:同样只勾选自己电脑磁盘，把u盘取消掉**

　　确定

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111R642I-19.jpg)



　　创建好分区之后，如上图所示，然后点“下一步”

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111R64K4-21.jpg)



　　选择“**将修改写入磁盘**”



14.这个**非常非常重要**！这里有两个要点：

（1）看到图片上面的“在XXXX中安装引导程序了”了么，**默认情况下安装引导程序是装在U盘的**，我一开始没注意没有点 更换设备 进行设置，结果装好之后U盘插在电脑的时候就有双系统的引导，U 盘拔了之后就直接进入 win7，CentOS没有了！当然，这种情况下也可以解决，我们后面谈。

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111RA2K-22.jpg)


你要点击 **更换设备**  ->  **BIOS 驱动器顺序**
**第一BIOS 驱动器:  选择 本地磁盘驱动器(sda 还是 sdb 就看你自己的了）第二BIOS 驱动器: 选择 U 盘驱动器**

**![](https://img-blog.csdn.net/20140214105220656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamNqYzkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

然后在下面第三张图还有一个**主引导记录（MBR）**和一个**引导分区的第一个扇区**的选择，这是什么呢？这其实是决定了你要使用 windows 还是 linux 的引导程序。如果选择　ＭＢＲ　那么你就是安装 Linux 的引导程序，如果选择第二个，那么就是保持你原来的 windows 引导程序不变，但是你会发现安装后开机时只有 windows，没有出现linux
 选项，这就需要你进入 win 7 ,用类似 EasyBCD 的东西添加 Linux 启动项，重启之后就可以了。具体有需要的同学请评论区留言或私信，**新手看不懂这段不用管，直接选 MBR 就可以了**，不过以后如果想删 linux 的话会麻烦点，到时有需要的同学同样评论区留言或私信就可以了。O(∩_∩)O~

（2）这张图是安装单系统的（后来加了两张自己拍的），所以 引导装载程序操作系统列表 只有一项，但我们要安装双系统，怎么办？别担心，你在这一步会出现 两项 ，也就是说，CentOS会自动检测到Windows的引导程序，并自动帮你引导。那我们可以不用设置吗？也可以。如果你没有进行设置的话，你开机是就会出现两个选项，一个是**CentOS**，一个叫**Others**，**并默认进入CentOS**，那个
 Others 就是 windows 7 啦。所以我们还是至少做一点改动吧。**点击 Others**（你安装时会看到它的），然后 **编辑**，把Others 改为 windows 7，如果你**想把 windows 改为默认启动系统的话，就要在这里勾选默认开机进入啦。**

5.5启动引导设置
这里需要更改设置，“更改设置”它默认把启动文件放到Windows所在分区，这样会损坏Win7的启动文件，此时应当点击“更换设备”，选择“BIOS
 DRIVE Order”设置。

这一步至关重要,如果按照默认方式安装,会把系统启动引导安装到 U 盘里面,这也就是很多网友安装完系统之后,只要把 U 盘拔了,系统就启动不了,插上 U 盘系统又能正常启动的原因了

点击“更改设置”

点 BIOS 驱动器顺序

第一
 BIOS 驱动器:选择 本地磁盘驱动器
第二 BIOS 驱动器:选择 U 盘驱动器. 





![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111RA2K-22.jpg)




![](https://img-blog.csdn.net/20140214105142031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamNqYzkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20140214105220656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamNqYzkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


 15、这里我们选择**Desktop（桌面）**，然后看下面，有个**以后自定义**和**现在自定义**，我第一次安装选的是以后自定义 ，结果连个gcc都没有，要自己安装很麻烦，第二次的时候我就选了现在自定义，然后下一步，勾选自己想要的环境配置。

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111R61U2-23.jpg)


16、接下来就是检测软件包，然后出现安装界面，进行安装，安装完成后，点重新引导

18、重新启动，把U盘拔出，电脑自动引导到CentOS6.4系统（如果你做出修改的话当然就自动引导到windows），然后进行初次配置，按照引导进行就可以了，但Kdump建议不要开启，其他的这里我们就不说了

![点击查看原始图片](http://www.startos.com/uploads/allimg/120716/111RC220-35.jpg)





我们上面说过如果没有更改引导程序安装位置的话，引导程序是会安装在U盘的MBR上的，所以结果是你把U盘拔出来后再也进不了CentOS，就像你没有安装过一样，而你的U盘连格式化都不行了。

首先，解决U盘因错误写入MBR而无法格式化的问题：

   我们上面不是安装了UltraISO软件吗？我们现在运行软件，打开我们的CentOS-6.4-i386-bin-DVD1.iso，点击菜单栏**“启动**——**写入硬盘映像**”，在硬盘驱动器上选择U盘，然后写入方式选择**USB-HDD +**，点击**便捷启动**——**写入新的引导扇区**——**Windows NT/2K/XP**，OK！

如果还是不行，那么试试这个：**右键**点击**我的电脑**——**管理**——**存储**——**磁盘管理（本地）**  然后右键你的U盘，选择格式化，然后按提示来就可以了

这样我们的U盘又可以自由使用了。

那怎么找回我们的CentOS 系统呢？

其实也是很简单。我本来是要进入救援模式的，但无奈选择后总是出现画面扭曲无法进行，所以用了这个办法，觉得比救援模式还更简单些：

用我们制作好的U盘启动盘，然后就跟我们进行安装一样，开机进入U盘，选择 Install or upgrade an existing system 或者 install system with basic video driver ，接下来的和前面的步骤一样，但在选择完语言和键盘后，会出现类似的选项（这个是5.5的安装）：（再下图是后来上传的自己拍的....)

![曾经安装过 CentOS 出现的全新安装或升级](http://vbird.dic.ksu.edu.tw/linux_basic/0157installcentos5_files/centos5_02_15.jpg)


![](https://img-blog.csdn.net/20140214111016609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamNqYzkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后我们点击**升级现有系统，**进入下一步，这时有三个选项，我们可以选择最后的**安装新的grub**，接着进行 grub 的设置了，这次可要记得点击更换设备啊，然后下一步程序会安装新的grub到我们硬盘的MBR中，重新开机，你会发现我们的grub程序回来啦！

全部解决！





