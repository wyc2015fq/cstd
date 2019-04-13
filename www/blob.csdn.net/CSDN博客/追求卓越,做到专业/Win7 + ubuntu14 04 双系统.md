
# Win7 + ubuntu14.04 双系统 - 追求卓越,做到专业 - CSDN博客


2014年08月07日 13:43:17[Waldenz](https://me.csdn.net/enter89)阅读数：67843


安装主要分为以下几步：
一． 下载Ubuntu 14.04镜像软件；
二． 制作U盘启动盘；
三． 安装Ubuntu系统；
四． 用EasyBCD 创建启动系统。
**1. 下载 U****buntu 14.04**
直接到官网免费下载Ubuntu 14.04.1 LTS[下载地址](http://www.ubuntu.com/download/desktop)
PS: 如果内存为4G以上，而且为双核CPU的计算机，建议安装64位的系统。
**2. 制作U盘启动器**
制作U盘启动盘的Universal-USB-Installer软件。[下载地址](http://www.pendrivelinux.com/universal-usb-installer-easy-as-1-2-3/)
![](https://img-blog.csdn.net/20140807125649359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZW50ZXI4OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
PS: 因为我的U盘在计算机中显示是在硬盘区域，而不是正常的可移动存储设备区域，所以我需要勾选“Now Showing All Drives(BE CAREFUL)”这一项，才能在step3中的下拉框中找到U盘盘符，其他的忽略。点击Create即可将Ubuntu系统写入U盘。
**3.安装Ubuntu系统**
（1）要在Windows下新划出一个大于10G的硬盘空间。[具体方法参考](http://blog.sciencenet.cn/blog-685489-759452.html)。
（2）在电脑上插入U盘，重启电脑，F12或者F8进入boot界面，选择通过USB启动。
（3）进入ubuntu安装菜单，选择 “Install Ubuntu”。
（4）在安装类型界面中，会出现“这台计算机已经安装了多个操作系统，你准备怎么做？”，然后选择最后一项“其他选项”，以为这样可以自己手动分区，点击继续。
（5）现在我们看到的是硬盘的分区情况，找到前边有“空闲”二字，我们要做到就是，把空闲的空间给ubuntu划分分区。
（6）点击“空闲”的分区，选择下边的“添加”，弹出窗口如下，上边填写分区空间大小，下边填写要挂载的分区，然后确定。
逻辑分区，200M，起始，Ext4日志文件系统，/boot；（引导分区200M足够）
逻辑分区，5000M，起始，交换空间，无挂载点；（交换分区swap，一般不大于物理内存）
逻辑分区，160000M，起始，Ext4日志文件系统，/；（系统分区"/"或称作"/root"装系统和软件，15G以上足够）
逻辑分区，剩余空间数，起始，Ext4日志文件系统，/home；（home分区存放个人文档）
（7）分区设置好后，查看/boot分区的编号，然后在下边的“安装启动引导区的设备”下拉框中选择/boot分区的编号，点击安装。

### 4. 用
### EasyBCD
### 创建启动系统。
1. 下载EasyBCD，此软件用于在启动电脑的时候选择要进入的系统。[下载地址](http://dl.pconline.com.cn/download/90611.html)
2. 打开easyBCD，选择add new entry,选择linux/BSD, name这一行随便填写，只是系统名词，写ubuntu吧，Device这一行选择刚刚我们创建的200MB的那个”/boot“分区,前边有linux标记的。如下图PS:EasyBCD很强大，不懂的别乱搞，搞坏MBR别怪我。
![](https://img-blog.csdn.net/20131126220108328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVlYmxvZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3. 做完这些重启系统后，系统会将win7系统和ubuntu系统都列出来，你可以选择自己要进入的系统That's all.
PS: 当你不要ubuntu的时候，直接在window里磁盘管理删了它所在的分区，然后在Easybcd里删了它的引导就行，不影响你的windows系统，这就是为啥我不用ubuntu来引导windows的原因。

在贴吧和论坛中找的“安装完Ubuntu14.04后要做的14件事”：[贴吧翻译地址](http://tieba.baidu.com/p/3000822153?fr=ala0&pstaala=2)[英语原文地址](http://itsfoss.com/things-to-do-after-installing-ubuntu-14-04/)
文章引用地址：[引用地址1](http://blog.csdn.net/jueblog/article/details/16972635#t0)[引用地址2](http://blog.csdn.net/liangcaiyun2013/article/details/10410797)

