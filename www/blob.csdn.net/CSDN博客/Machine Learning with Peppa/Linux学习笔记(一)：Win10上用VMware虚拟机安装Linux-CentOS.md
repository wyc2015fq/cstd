# Linux学习笔记(一)：Win10上用VMware虚拟机安装Linux-CentOS - Machine Learning with Peppa - CSDN博客





2018年03月03日 10:04:27[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：4087
所属专栏：[Linux私房菜](https://blog.csdn.net/column/details/20153.html)









这周开始Linux的学习，教材参考鸟哥的Linux私房菜，系统本来想用UBUNTU，不过为了方便还是使用教材推荐的CENTOS，下面附上自己的安装过程。

**一、 VMware简介：**

VMware是一个虚拟PC的软件，可以在现有的操纵系统上虚拟出一个新的硬件环境，相当于模拟出一台新的PC，以此来实现在一台机器上真正同时运行两个独立的操作系统。


VMware的主要特点：
- 不需要区分或重新开机就能在同一台PC上使用两种以上的操作系统；
- 本机系统可以与虚拟机系统网络通信；
- 可以设定并且随时修改虚拟机操作系统的硬件环境。

**二、CentOS简介：**


CentOS（Community ENTerprise Operating System）是Linux发行版之一，它是来自于Red Hat Enterprise Linux依照开放源代码规定释出的源代码所编译而成。由于出自同样的源代码，因此有些要求高度稳定性的服务器以CentOS替代商业版的Red Hat Enterprise Linux使用。两者的不同，在于CentOS并不包含封闭源代码软件。

**三、CentOS的安装：**

虚拟机版本：VMware Workstation_14.0.3


Linux系统版本：CentOS_7（64位）


物理机版本：Win 10


注：本人为初学者，在虚拟机上安装CentOS，方便同时在Win10上操作以及和物理机进行通信；等操作熟练之后可以装双系统。




首先，请在Windows7下安装VMware虚拟机，这个比较简单，直接从官网下载安装即可，这里不再叙述。

接着，从官网直接下载CentOS6.9的iso镜像文件，地址：https://www.centos.org/download/

下载完成之后，下面就是安装过程：

第一步：打开VMware,点击创建新的虚拟机或者点击文件——新建虚拟机；

![](https://img-blog.csdn.net/20171104213424727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第二步：默认选择典型，点击下一步；

![](https://img-blog.csdn.net/20171104213333970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第三步：点击安装程序光盘映像文件-选择下载好的CentOS的iso文件路径，再点击稍后安装操作系统，点击下一步；

![](http://blog.csdn.net/ProgrammingWay/article/details/78237856)![](https://img-blog.csdn.net/20171104213525257?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第四步：客户操作系统选择Linux，版本选择对应的版本，这里选择的是CentOS 64位，点击下一步；

![](http://blog.csdn.net/ProgrammingWay/article/details/78237856)![](https://img-blog.csdn.net/20171104213554415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第五步：虚拟机名称可以用默认名称，选择虚拟机存放的路径，最好不要存放在系统盘，点击下一步；

![](http://blog.csdn.net/ProgrammingWay/article/details/78237856)![](https://img-blog.csdn.net/20171104213613872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第六步：磁盘大小和拆分文件选择默认即可，也可以根据自己需要修改，点击下一步；

![](https://img-blog.csdn.net/20171104213625734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第七步：点击自定义硬件进行设置，也可以后面再进行设置。

![](http://blog.csdn.net/ProgrammingWay/article/details/78237856)![](https://img-blog.csdn.net/20171104213656353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第八步：将处理器设置为4核，也可以不修改，将新CD/DVD(IDE)的连接选择ISO镜像文件，并选择下载好的CentOS的iso镜像文件路径，点击关闭，再点击完成；

![](https://img-blog.csdn.net/20171104213708766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第九步：右击虚拟机CentOS 64位，选择电源——点击打开电源时进入固件；

![](http://blog.csdn.net/ProgrammingWay/article/details/78237856)![](https://img-blog.csdn.net/20171104213724835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十步：使用方向键切换至Boot下，再使用“+”号，将CD-ROM Driver移动到zuis；

![](http://blog.csdn.net/ProgrammingWay/article/details/78237856)![](https://img-blog.csdn.net/20171104213737870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十一步：使用方向键切换到Exit，选择Exit Discarding Changes，点击回车，选择Yes，再点击回车；

![](https://img-blog.csdn.net/20171104213748287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十二步：进入到下面页面，选择第一项，直接回车；

![](https://img-blog.csdn.net/20171104213800020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十三步：选择Skip，直接回车；

![](https://img-blog.csdn.net/20171104213819669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十四步：点击Next，进入下一步；

![](https://img-blog.csdn.net/20171104213831867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十五步：选择简体中文，点击Next；

![](https://img-blog.csdn.net/20171104213846862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十六步：默认选择美国英语式，点击下一步；

![](https://img-blog.csdn.net/20171104213905070?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十七步：默认选择基本存储设备，点击下一步；

![](https://img-blog.csdn.net/20171104213938728?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十八步：下一步，点击“是，忽略所有数据（Y）”；

![](http://blog.csdn.net/ProgrammingWay/article/details/78237856)![](https://img-blog.csdn.net/20171104214034507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十九步：主机名默认即可，不需要修改，点击下一步；

![](https://img-blog.csdn.net/20171104214226382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第二十步：默认时间是亚洲/上海，直接点击下一步；

![](https://img-blog.csdn.net/20171104214301440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第二十一步：输入6位数密码，点击下一步；

![](https://img-blog.csdn.net/20171104214319853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第二十二步：选择创建自定义布局，点击下一步；

![](https://img-blog.csdn.net/20171104214350989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第二十三步：点击创建，给系统分别分"/boot"、"/home"、"swap"、"/"分区；

![](https://img-blog.csdn.net/20171104214400864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171104214411051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171104214429246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171104214441216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171104214455292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第二十四步：分区完成后，点击下一步，

![](https://img-blog.csdn.net/20171104214544694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第二十五步：点击格式化分区，并点击“将修改写入磁盘”；

![](https://img-blog.csdn.net/20171104214557514?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171104214610656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第二十六步：这里选择Basic Server,也可以根据自己的需要选择其他类型，点击下一步；

![](https://img-blog.csdn.net/20171104214623853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171104214637218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

第二十七步：等待安装完成后，点击重新引导；

![](https://img-blog.csdn.net/20171104214646618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


第十九步：安装成功，启动进入下面的Linux开机界面，输入localhost login：root；密码是之前设置的：123456；到此就完成了。

![](https://img-blog.csdn.net/20171104214706643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUHJvZ3JhbW1pbmdXYXk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这样，就大功告成了。



