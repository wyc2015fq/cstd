# 【Win7开启DMA】 - 三少GG - CSDN博客
2011年10月03日 19:00:48[三少GG](https://me.csdn.net/scut1135)阅读数：4191
**   BIOS**是英文"Basic Input Output System"的缩略语，直译过来后中文名称就是"基本输入输出系统"。它的全称应该是ROM－BIOS，意思是只读存储器基本输入输出系统。其实，它是**一组固化到计算机内主板上一个ROM芯片上的程序，它保存着计算机最重要的基本输入输出的程序、系统设置信息、开机上电自检程序和系统启动自举程序**。有人认为既然BIOS是"程序"，那它就应该是属于软件，感觉就像自己常用的Word或Excel。但也很多人不这么认为，因为它与一般的软件还是有一些区别，而且它与硬件的联系也是相当地紧密。**形象地说，BIOS应该是连接软件程序与硬件设备的一座"桥梁"，负责解决硬件的即时要求。一块主板性能优越与否，很大程度上就取决于BIOS程序的管理功能是否合理、先进。****主板上的BIOS芯片或许是主板上唯一贴有标签的芯片**，一般它是一块32针的双列直插式的集成电路，上面印有"BIOS"字样。586以前的BIOS多为可重写EPROM芯片，上面的标签起着保护BIOS内容的作用(紫外线照射会使EPROM内容丢失)，不能随便撕下。586以后的ROM BIOS多采用EEPROM(电可擦写只读ROM)，通过跳线开关和系统配带的驱动程序盘，可以对EEPROM进行重写，方便地实现BIOS升级。**常见的BIOS芯片有Award、AMI、Phoenix、MR等**，在芯片上都能见到厂商的标记。 
还有就是开机的时候 按DEL键 可以调出。
++++++++++++++++++++++++++++++++++++++++++++++++++++++
**DMA代表直接存储器存取（Direct Memory Access）。这是允许外部设备打开某个通道，在不涉及CPU的情况下直接对存储器进行读/写操作的一种办法。这种方案减轻了CPU的负担**，使CPU可以进行执行更重要的任务，提高了设备的传输率及系统的整体性能。理论上DMA 33 的设备传输率可达 33M/Sec; DMA 66 为 66M/Sec; DMA 100为 100M/Sec.；DMA133为
 133MB/Sec。**现在几乎所有的主板都提供了支持DMA传输方式的IDE接口**而且几乎所有的IDE硬盘也都支持DMA传输方式。但在实际应用中绝大多数的人都没有用到DMA模式，或者没有发挥它的最大性能。那么怎样才能让它工作在DMA模式下呢？**首先主板，硬盘要支持，数据线要支持（其中40针排线支持DMA 33模式，80针排线支持DMA 66/100/133模式），其次要操作系统支持才可以。**除此之外还要正确安装硕泰克主板的芯片驱动程序才能真正发挥出DMA 的强大性能。 具体的设置步骤如下：点击“我的电脑”右键—“属性”—“设备管理器，双击“磁盘驱动器”—选择硬盘选项—“属性”—然后将“DMA”打勾，设置完后重新启动电脑就可以了。
   DMA 模式就是“Direct Memory Access”直接内存访问的缩写。就是允许设备从 CPU 手中接管系统总线，并直接把数据传送到主内存。通常 CPU 分两步来传送数据：1，从设备的I/O存储空间读数据，把数据放在CPU内部；2，CPU把数据从其内部送到主内存，DMA 方式通常用一步就可把数据从设备直接送到主内存。但并不是所有的设备都可以使用DMA的，设备硬件必须内置有这种能力，才能支持。从DMA传输占用系统总线开始的传输过程中CPU就不做什么了。这样采用DMA模式的设备占用系统总线资源就非常小，从而提高计算机子系统的性能。
    目前市场的IDE设备大部分都支持DMA模式了，包括硬盘、CDROM、DVDROM甚至CD-RW、DVD-RW和DVD+RW。而硬盘也已经发展到了ATA 133了，虽然ATA 133的硬盘相对于ATA 100并没有本质上的区别，可**是ATA 133的硬盘已经大量的占领市场了**。
**由于以Windows图形界面的操作系统的普及，加载DMA模式一般是在安装好操作系统以后进行的**，所以在DOS状态下的DMA模式很容易被人忽略。但实际上，DMA模式在DOS状态下还有很多的利用价值。
DMA is a technology found in devices such as CD/DVD drives and hard drives. DMA capable devices can directly transfer data
 between Computer
 Memory (RAM) and themselves, avoiding the need of Processor. Which means you can perform tasks like CD/DVD burning smoothly alongside because CPU will be used for this task.
But sometimes if there is problem with the hardware or drivers, DMA can cause problem and hence failure while burning CD/DVDs. In such troubleshooting cases, you can disable DMA. By default,Windows
 7 keeps DMA turned “ON” always. To turn it “OFF” follow these steps:
1. Login to Windows with Administrator Privileges
2. Click “Start” and then “Control Panel”—> “Hardware and Sound”.
3. Click on “Device Manager”. (You can also launch by simply entering “Device Manager” in the Programs search box
 and hit enter.
![Hardware_and_Sound](http://www.thewindowsplanet.com/wp-content/uploads/2010/03/Hardware_and_Sound.jpg)
4. Double-click IDE ATA/ATAPI controllers.
5. In IDE ATA/ATAPI controllers, for each item that has the word Channel in its label, right-click on item, and then click Properties.
![ATA_Channel_Device_Manager](http://www.thewindowsplanet.com/wp-content/uploads/2010/03/ATA_Channel_Device_Manager.png)
6. Click the Advanced Settings tab. Clear “Enable DMA” checkbox, and then click “OK”.
![ATA_Channel_DMA_Disable](http://www.thewindowsplanet.com/wp-content/uploads/2010/03/ATA_Channel_DMA_Disable.png)
++++++++++++++++++++++++++++++++++++++++++++++++++++++
请问SATA与ATA硬盘有什么区别？我的电脑现在用的是ATA硬盘，可不可以换个SATA硬盘使用。我的主板为升技SG-72芯片组为ISIS661
　　SATA硬盘与ATA硬盘区别 
　　PATA的全称是ParallelATA，就是并行ATA硬盘接口规范，也就是我们现在最常见的硬盘接口规范了。ATA硬盘接口规模已经具有相当的辉煌的历史了，而且从ATA33/66一直发展到ATA100/133一直到目前最高的ATA150。而SATA硬盘全称则是SerialATA，即串行ATA硬盘接口规范。目前PATA100硬盘的一般写入速度为65MB/s，而第一代SATA硬盘的写入速度为150MB/s，第二代SATA硬盘的写入速度则高达300MB/s，整整比第一代的速度提高了一倍。SATA硬盘接口规范的出现其实就要取代PATA，就和DDR取代SDRAM一样。
　　从技术角度去分析：SATA比PATA快多少？ 
　　既然SATA的出现是取代PATA的，那么SATA和PATA相比，主要的优势又在那里呢？首先就是速度，上文我们已经提到第二代SATA的传输速度为300MB/s，不过第三代的SATA产品的传输速度已经提高至600MB/s。从速度这一点上，SATA已经远远把PATA硬盘甩到了后面。另外，在传输方式上SATA也比PATA高人一等。SATA采用的是单通道传输，PATA是多通道传输。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AWARD公司是世界最大的BIOS生产厂商之一，其产品也被广泛使用。但由于AWARD BIOS里面的信息都是基于英文且需要用户对相关专业知识的理解相对深入，使得普通用户设置起来感到困难很大。而如果这些设置不当的话，将会影响整台电脑的性能设置是不能正常使用，所以一份详细的设置说明是必要的。下面就介绍一下AWARD BIOS中的有关设置选项的含义和设置方法。 
【Standard CMOS Setup】(标准设定) 
在本菜单中，用户可以修改日期、时间、第一主IDE设备(硬盘)和IDE设备(硬盘或CD－ROM)、第二个主IDE设备(硬盘或CD－ROM)和从IDE设备(硬盘或CD－ROM)、软驱A与B、显示系统的类型、什么出错状态要导致系统启动暂停等。主要说明如下： 
(1) 用户可以在Type(类型)和Mode(模式)项设置为Auto，使BIOS自动检测硬盘。也可以在主菜单中的【IDE HDD Auto Detection】操作来设置。用户还可以使用User选项，手动设定硬盘的参数。您必须输入柱面数(Cyls)，磁头数(Heads)，写预补偿(Precomp)，磁头着陆区(Landz)，每柱面扇区数(Sectorxs)，工作模式(Mode，内容见下)等几种参数。硬盘大小在上述参数设定后自动产生。 
硬盘工作模式（MODE）的三种选项： 
NORMAL模式：传统的标准模式，支持硬盘容量最高至528MB。 
LARGE模式：当硬盘容量超过528MB，而硬盘或操作系统不支持LBA模式时，可采用此选项。 
LBA 模式(Logical Block Addressing Mode)：适用于硬盘容量超过528M且支持"逻辑区块地址"(LBA)功能（一般都使用此项） 
......
+++++++++++++++++++++++++++++++++++++++++++
**[win7系统加快开机、关机和运行速度的技巧](http://jingyan.baidu.com/article/0eb457e55cb4ff03f0a90574.html)!!!!!!!!!!!!!!!!!!!!**
有很多朋友新买的笔记本运行很慢，有些朋友是使用很久了，机器慢慢的出现开机、关机都要2、3分钟使用中还经常假死甚至死机、蓝屏等。笔者下面就结合自己和广大网友的经验，总结一下解决以上问题的方法，供大家参考。
## 步骤/方法
- 
开始---控制面板---管理工具---系统配置----引导----高级选项----处理器数----2（单核的朋友不要改这里），再勾选最大内存----确定----应用----确定----重新启动。![](http://hiphotos.baidu.com/exp/pic/item/bbe0d3116912d146b8127b54.jpg)![](http://hiphotos.baidu.com/exp/pic/item/a28d62d9428b97bf38012f57.jpg)
- 
开始---控制面板---管理工具---系统配置----启动----在这里只勾选杀毒软件---确定----重启。![](http://hiphotos.baidu.com/exp/pic/item/f35ea009f0db60726a60fb34.jpg)
- 
计算机----属性----设备管理器----查看----显示隐藏的设备----双击“非即插即用驱动程序”---- 在展开的所有项目里检查有没有带 惊叹号和问号 的项目，有的话双击----启动，不能启动的---右键属性---卸载----确定，重启。![](http://hiphotos.baidu.com/exp/pic/item/9864a231ac94a2fe5fdf0ee8.jpg)![](http://hiphotos.baidu.com/exp/pic/item/72ccb77764ea254db151b9f4.jpg)
- 
计算机----属性----设备管理器----高级系统设置----高级----设置----设置----高级----虚拟内存---更改---去掉“自动管理所有驱动器分页文件大小”的勾选----C，D，E，F----分别选择----无分页文件---确定，重启（内存2G以下的朋友不要改动）。![](http://hiphotos.baidu.com/exp/pic/item/0b3a1c080cc89d8762d986b0.jpg)
- 
计算机----属性----设备管理器----高级系统设置----启动和故障恢复----设置----去掉 显示操作系统列表的时间 在需要时显示恢复选项的时间 将事件写入系统日志 的勾选（电脑新手可以把时间设为3秒钟），写入调试信息---无----确定，重启。![](http://hiphotos.baidu.com/exp/pic/item/8474fbdd3a7907b977c63864.jpg)
- 
计算机----属性----设备管理器----双击 IDE ATA/ATAPI控制器---- 分别双击展开的所有项目----高级设置----勾选 启用DMA ----确定，重启。![](http://hiphotos.baidu.com/exp/pic/item/027a45b56262829837d3ca44.jpg)
- 
对所有磁盘分区按倒序（F，E，D，C）进行磁盘查错：例如F---右键属性----工具----开始检查----勾选 自动修复系统文件错误 扫描并尝试修复坏扇区 ----开始。如果遇到要求卸载卷的，不要卸载，选择按计划检查，系统会在重启时自动运行。要耐心等运行结束。![](http://hiphotos.baidu.com/exp/pic/item/8bc3a70112396155728da525.jpg)
- 
对所有磁盘分区按倒序（F，E，D，C）进行磁盘碎片整理，要耐心等运行结束。![](http://hiphotos.baidu.com/exp/pic/item/4bac3073ee0f244a8601b017.jpg)
- 
经常手动检查系统更新（更新补丁都是为安全和稳定而推出）：开始---控制面板----windows update----检查更新----按提示安装重要更新和带win7字样的推荐更新，根据提示重启或不重启。![](http://hiphotos.baidu.com/exp/pic/item/b25d9901c9701080267fb5f7.jpg)
- 
同时按下 windows徽标键+R键，调出“运行”对话框，输入“regedit"-----确定，弹出“注册表编辑器”，找到HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control ，鼠标点击Control ，右边框中显示Wait To Kill Service Time out ，右键将Wait To Kill Service Time out后面的数值改到很小就可以了，一般默认值是12000（12秒）。改好后重启。![](http://hiphotos.baidu.com/exp/pic/item/1e71f724d72531574d088d30.jpg)![](http://hiphotos.baidu.com/exp/pic/item/263e802f6cb7b06e1e308933.jpg)![](http://hiphotos.baidu.com/exp/pic/item/38403f3f69c1e3ba55e72303.jpg)
- 
下载“驱动精灵”或“驱动人生”，按提示更新硬件驱动程序。![](http://hiphotos.baidu.com/exp/pic/item/263e802f6ce6b06e1e3089e2.jpg)
- 
对于配置不高的用户，可以关闭win7系统的Aero效果，以获得性能上的提升。计算机----右键属性-----高级系统设置----高级----设置-----视觉效果，只留下几项（可以在下图基础上再多选3项：启用透明玻璃；启用桌面组合；显示缩略图，而不是显示图标。），既美观又提升性能。![](http://hiphotos.baidu.com/exp/pic/item/a8362712e8332907f819b8a4.jpg)
- 
用系统维护软件关闭一些个人用户用不着的系统服务。![](http://hiphotos.baidu.com/exp/pic/item/8474fbdd3c5b09b976c63886.jpg)
- 
经常清理系统垃圾和注册表，保持系统轻快运行。![](http://hiphotos.baidu.com/exp/pic/item/b258f5c4f838daff8326ac71.jpg)
![](http://hiphotos.baidu.com/exp/pic/item/e6508eefaba63eb9ce1b3e9c.jpg)
## 注意事项
- 
这是在win7系统下的操作实例，XP系统大同小异，大家可以自己实践总结。
- 
不建议关闭系统还原（至少要打开C盘的还原，其他盘可以关闭）。因为一旦系统异常，我们可以进行系统还原，使系统恢复到以前正常的某一天。
- 
如果你的电脑性能不错，不建议你修改；如果你没有基本的电脑维护常识，不建议你修改！
- 
看到有网友反馈说害他重装系统，我感到很抱歉！仅在这里声明，此文仅供电脑爱好者参考！菜鸟们看看就可以了。
- 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++
- 
## [合理优化操作系统实现Windows 7快速开机(2)](http://www.win7soft.com/a/learning/system/2011/0516/5199_2.html)
[Win7教程](http://www.win7soft.com/a/learning/)（[www.win7soft.com](http://www.win7soft.com/)）:合理优化操作系统实现Windows
 7快速开机
本文跳过了Windows
 VISTA ，而谈Windows
 7，原因相信大家都明白，VISTA 只是一个过渡性的系统（有D似“水泡”），Windows
 7 继承了 VISTA 的优点，并有更多的新特性，将会成为未来的主流操作系统。
在同等硬件配置，都未进行过任何优化的前提下，Windows
 7 的开机速度是优于 Windows
 XP 的。
但这并不是说，Windows
 7 不需要优化！
首次采用“微博”形式发文，力求言简意赅、提纲挈领，让读者用尽可能少的时间，理解文章内容。希望大家喜欢！（某人唔好话“晕”啦，我希望你能看完）
———————————————————————————————————————
一、硬件：满足系统需求
二、软件：
1.BIOS：应用BIOS优化值等；
2.视窗系统：
**（1）安装合适驱动，最大化硬件性能；**
**（2）开启DMA传输模式；**
**（3）减少不必要的启动项；**
（4）关闭不必要的服务；
（5）清理注册表、磁盘垃圾。
（6）整理磁盘冗余文件
二、软件：
1.BIOS系统：开启物理内存扩展、设置First
 Boot Drive（首次启动设备）为系统所在的硬盘；
2.视窗系统：
（1）使用驱动精灵，搜索下载安装适合当前硬件的驱动程序，充分发挥硬件性能；
（2）计算机→管理→设备管理器→IDE
 ATA/ATAPI控制器→带“ATA
 Channel ”字样的菜单
高级设置→**查看设备是否工作在** Ultra
 DMA 模式5 或以上。
![姊妹篇-实现Windows 7 快速开机 - Hugo - Hugo Blog](http://www.win7soft.com/uploads/allimg/110516/1403445036-0.png)
提示：
A.近年的硬盘的传输模式，一般在DMA
 MODE 5或以上，如果没有达到，请检查硬件的连接。
可以使用[HD_TUNE](http://www.hanzify.org/software/11976.html)，检查硬件支持的传输模式及当前的传输模式。
![姊妹篇-实现Windows 7 快速开机 - Hugo - Hugo Blog](http://www.win7soft.com/uploads/allimg/110516/1403445316-1.png)
 （
3）运行→msconfig→尽可能少地保留你需要的即可；
提示：
1品牌机用户，必须保留某些特定的启动项，否则，键盘上的快捷键以及其他特色功能可能无法使用；
2.如果你需要了解某个启动项的内容，根据命令栏显示的文件位置运行指定文件即可。

