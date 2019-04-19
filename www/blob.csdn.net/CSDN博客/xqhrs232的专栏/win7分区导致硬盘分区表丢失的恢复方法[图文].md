# win7分区导致硬盘分区表丢失的恢复方法[图文] - xqhrs232的专栏 - CSDN博客
2016年10月11日 16:11:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1829
原文地 址::[http://www.jb51.net/os/windows/73780.html](http://www.jb51.net/os/windows/73780.html)
相关文章 
1、硬盘分区丢失怎么找回.----[http://ask.zol.com.cn/q/6201.html#title](http://ask.zol.com.cn/q/6201.html#title)
2、搜索已丢失分区(重建分区表)----[http://www.diskgenius.cn/help/rebuild.php](http://www.diskgenius.cn/help/rebuild.php)
3、求助win7硬盘分区消失问题----[http://zhidao.baidu.com/link?url=42HCVsN9W3tbkgeIcVOWOcvSuaBmFMJxLnoBzxRBRbRe0Pcapl8eKMTXWr9xkGIb9eLQw7IC-62kXIfigOIS0q](http://zhidao.baidu.com/link?url=42HCVsN9W3tbkgeIcVOWOcvSuaBmFMJxLnoBzxRBRbRe0Pcapl8eKMTXWr9xkGIb9eLQw7IC-62kXIfigOIS0q)
下面我就给大家来介绍一个恢复的方法，虽然很简单，但是如果你经历过硬盘数据丢失的痛就知道，其实心情并不简单
前一阵子，由于2G的内存已经无法满足我的工作，我对电脑硬件进行了一系列的升级，由于是8G的内存条，所以只能从windows XP升级成windows 7，把原来的500G硬盘C盘给格式化后，便开始用U盘安装win7，安装成功后，再熟悉了几天后，决定把重新再安装一遍win7，保持系统的纯净度，没想到这次重装，导致了硬盘分区表的错误，一晚上没睡觉才恢复正常。
      到了安装程序下，发现无法格式化C盘，便随手删除了C盘和100MB的系统分区，再创建分区的时候，发现硬盘原本的四个区已经只剩下两个分区了。这个时候，我想到了硬盘分区表，因为以前有过一次这样的情况，刚学习电脑时，误动了硬盘分区表，导致硬盘上的数据全部丢失。只不过上次是40G的硬盘，没有什么重要数据，这次500G的硬盘，可是存了300G的软件和数据，找了个光驱，进入PE系统，E盘已经消失，D盘只有少量数据在。这个时候，我感觉心象被掏空了一般，因为许多数据，都是三年收集的资料。下面我就给大家来介绍一个恢复的方法，虽然很简单，但是如果你经历过硬盘数据丢失的痛就知道，其实心情并不简单。
      一、 首先我决定尝试Win7自带的Bootrec.exe修复工具来修复一下分区表，首先简单的介绍一下Bootrec.exe，这里我们会用到两个参数/fixmbr和/fixboot，/FixMbr选项的系统分区写入Windows 7 或 Windows Vista 兼容 MBR，此选项不会覆盖现有的分区表。/FixBoot选项通过使用与 Windows Vista 或 Windows 7 的引导扇区向系统分区写入新的引导扇区。详细说明我们可以查看[微软官方的介绍](http://support.microsoft.com/kb/927392/zh-cn)。
       用U盘或者光驱进入win7的安装界面，按shift+f10进入命令提示符界面：
![win7分区导致硬盘分区表丢失的恢复方法](http://files.jb51.net/file_images/article/201302/201302231536157.jpg)

       输入bootrce.exe /fixmbr或者bootrce.exe  /fixboot来尝试修复，经常上述的尝试，我的硬盘分区表并没有恢复正常。如果大家的也没有恢复正常，请接着看下面。
        二、使用DiskGenius来恢复分区表
        在使用了Bootrec.exe修复分区表无果后，我又下载了DiskGenius来进行尝试，首先在另一台电脑上将下载好的DiskGenius复制进U盘，然后随便找一个XP的安装盘，进行PE系统，把DiskGenius复制进PE系统里，然后点击工具—搜索已丢失分区（重建分区表）
![win7分区导致硬盘分区表丢失的恢复方法](http://files.jb51.net/file_images/article/201302/201302231536158.jpg)

        搜索范围定为整个硬盘
![win7分区导致硬盘分区表丢失的恢复方法](http://files.jb51.net/file_images/article/201302/201302231536159.jpg)

       开始搜索后，DiskGenius会将以后的硬盘分区一个个显示，这个时候，你需要自己来判断搜索到的分区是不是你丢失的分区，如果是，点击保留即可。
![win7分区导致硬盘分区表丢失的恢复方法](http://files.jb51.net/file_images/article/201302/2013022315361510.jpg)

       如果搜索完成后，如果已经完整的找回原有分区，点击硬盘—保存分区表，重新启动电脑进入PE，就会发现原来的硬盘已经恢复原貌。如果大家觉得我写的不够详细，也可以看一下[DiskGenius官网的帮助](http://www.diskgenius.cn/help/rebuild.asp)。
       经过了一晚上的折腾，虽然已经找回了丢失的硬盘分区，可还是让人出了一身冷汗。经过这件事情，我总结出了两条经验：一、硬盘分区表打死都不要动。二、重要资料一定要刻成光盘来保存。如果大家也出现了这样的问题，也欢迎大家留言，我会尽我所能来回答。

