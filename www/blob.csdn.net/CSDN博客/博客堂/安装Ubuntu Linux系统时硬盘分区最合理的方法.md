# 安装Ubuntu Linux系统时硬盘分区最合理的方法 - 博客堂 - CSDN博客





2017年08月13日 15:36:20[最小森林](https://me.csdn.net/u012052268)阅读数：153719










无论是安装Windows还是Linux操作系统，硬盘分区都是整个系统安装过程中最为棘手的环节，网上的一些Ubuntu Linux安装教程一般都是自动分区，给初学者带来很大的不便，下面我就根据多年来在装系统的经验谈谈安装Ubuntu Linux系统时硬盘分区最合理的方法。

在讲硬盘分区之前，我先来普及一下硬盘的相关分类，硬盘一般分为IDE硬盘、SCSI硬盘和SATA硬盘三种，在Linux系统中，IDE接口的硬盘被称为hd，SCSI和SATA接口的硬盘则被称为sd，其中IDE硬盘基本上已经淘汰，现在市面上最常见的就是SATA接口的硬盘，第1块硬盘称为sda，第2块硬盘称为sdb……，依此类推。

一块硬盘最多有4个主分区，主分区以外的分区称为扩展分区，硬盘可以没有扩展分区，但是一定要有主分区，在主分区中要有一个激活分区用来启动Windows系统，在扩展分区中可以建立若干个逻辑分区，因此，最合理的分区方式应该最多分三个主分区，一个扩展分区，这样可以有效地利用有限的主分区，然后在扩展分区中建立逻辑分区。

在Linux系统中每一个硬盘总共最多有 16个分区，硬盘上的4个主分区，分别标识为sdal、sda2、sda3和sda4，逻辑分区则从sda5开始标识一直到sda16。

Ubuntu Linux可以把分区作为挂载点，载入目录，其中最常用的硬盘大小（500G-1000G）分配目录推荐如下表所示：
|目录|建议大小|格式|描述|
|----|----|----|----|
|/|150G-200G|ext4|根目录|
|swap|物理内存两倍|swap|交换空间：交换分区相当于Windows中的“虚拟内存”，如果内存低的话（1-4G），物理内存的两倍，高点的话（8-16G）要么等于物理内存，要么物理内存+2g左右，|
|/boot|1G左右|ext4|**空间起始位置 **分区格式为ext4** /boot****建议：应该大于400MB或1GB **Linux的内核及引导系统程序所需要的文件，比如 vmlinuz initrd.img文件都位于这个目录中。在一般情况下，GRUB或LILO系统引导管理器也位于这个目录；启动撞在文件存放位置，如kernels，initrd，grub。|
|/tmp|5G左右|ext4|系统的临时文件，一般系统重启不会被保存。（建立服务器需要？）|
|/home|尽量大些|ext4|用户工作目录；个人配置文件，如个人环境变量等；所有账号分配一个工作目录。|

**另外一种情况是没有单独的机器配置，只能在自己电脑上安装 虚拟机，这种情况下 分配硬盘一般不超过100G  内存也只有2-4G左右，可以参考这种方式配置：**
|目录|建议大小|格式|描述|
|----|----|----|----|
|/|10G-20G|ext4|根目录|
|swap|<2048M|swap|交换空间|
|/boot|200M左右|ext4|Linux的内核及引导系统程序所需要的文件，比如 vmlinuz initrd.img文件都位于这个目录中。在一般情况下，GRUB或LILO系统引导管理器也位于这个目录；启动撞在文件存放位置，如kernels，initrd，grub。|
|/tmp|5G左右|ext4|系统的临时文件，一般系统重启不会被保存。（建立服务器需要？）|
|/home|尽量大些|ext4|用户工作目录；个人配置文件，如个人环境变量等；所有账号分配一个工作目录。|

接下来就以Ubuntu Linux 12.04.3为例来为大家图文讲解手动对硬盘进行分区的方法。

在如图1所示的分区界面中有两个选项可供选择。最简单的莫过于“清除整个硬盘并安装Ubuntu”选项，单击“继续”按钮，安装向导将根据原有的分区信息自动选择合理的分区方式。

![](https://img-blog.csdn.net/20170813153803918?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjA1MjI2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



图1：Ubuntu Linux分区向导

如果希望对分区过程进行完全控制，可以使用“其它”选项。单击“继续”按钮，安装向导将把用户带至手动分区界面，如图2所示。

![](https://img-blog.csdn.net/20170813153809854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjA1MjI2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



图2：手动分区模式

在这里可以看到当前硬盘的分区信息，由于我使用的是虚拟机为大家进行演示，因此，只能看到一块没有使用过的硬盘，硬盘容量为20GB。

第一步：选中空闲的磁盘空间，点击“添加”按钮，弹出“创建新分区”对话框，新分区的类型为主分区，挂载点为/boot，新建分区容量为128MB，最后点击“确定”按钮，如下图所示。



![](https://img-blog.csdn.net/20170813153814564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjA1MjI2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图3：建立启动分区

这个挂载点包含了操作系统的内核和在启动系统过程中所要用到的文件，接下来要新建一个交换分区。

第二步：选中空闲的磁盘空间，点击“添加”按钮，弹出“创建新分区”对话框，新分区的类型为逻辑分区，“用于”选择“交换空间”，新建分区容量为1024M，最后点击“确定”按钮，如下图所示。



![](https://img-blog.csdn.net/20170813153824758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjA1MjI2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图4：建立交换分区

交换分区相当于Windows中的“虚拟内存”，如果物理内存小于或等于512MB，建议分配交换分区的大小为物理内存容量的2倍；如果物理内存大于512MB，建议分配交换分区的大小等于物理内存容量；如果您的内存够大也可以不建立交换分区。

按同样的方法依次建立/var、/tmp、/home等挂载点，最终分区结果如下图所示。

![](https://img-blog.csdn.net/20170813153829402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjA1MjI2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

                                              图5：最终硬盘分区结果



> 
本文转载自： [http://www.ah168.com/jishuwenzhang/xitong/Ubuntu-916.html](http://www.ah168.com/jishuwenzhang/xitong/Ubuntu-916.html)

1. 近些年人民的物质生活极大提高，电脑内存和硬盘大小也出现了长足的发展，而文章是转载自原作者大神15年之前的文章，所以文中推荐的**数值**在现在来看已经显得有些捉襟见肘；但是作为学习linux的硬盘盘分区方法和理论还是有借鉴意义的，在这里向原作者致敬。

2. 另外我本人还是学习linux的小菜鸡(┬＿┬)，这篇文章对我来说主要是学习之用，有不懂的可以去评论区找大神探讨




