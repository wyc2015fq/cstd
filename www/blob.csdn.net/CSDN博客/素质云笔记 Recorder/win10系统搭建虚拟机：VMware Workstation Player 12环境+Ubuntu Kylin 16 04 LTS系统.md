
# win10系统搭建虚拟机：VMware Workstation Player 12环境+Ubuntu Kylin 16.04 LTS系统 - 素质云笔记-Recorder... - CSDN博客

2016年11月19日 15:52:20[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4853




笔者小白一枚，其实连虚拟机是个啥都不知道...实属惭愧，介于此所以今天倒腾了一下花了一上午就已经搭建好一个VMware Workstation Player 12免费版的，很哈皮，于是赶紧分享一下。

**问题1：win10自带的虚拟机去哪儿了？**

笔者自用win10系统，就在苦恼win10好像听说过有自带的虚拟机hyper-v，我的笔记本里面怎么没有呢？
那么win10系统的虚拟机跑哪儿了呢？

于是查阅了一下发现，自己电脑win10是家庭中文版，这个版本是不带虚拟机，只有专业版、企业版才有，而且hyper-v跟其他虚拟机（vm、vbox）冲突。不过vm有免费版（vmware player），vbox完全免费所以就试了试界面比较和谐的VM。

如果你是以上两个版本的话，你可以参考以下链接去激发win10自带的虚拟机：[http://jingyan.baidu.com/article/c146541397aaba0bfcfc4cb9.html](http://jingyan.baidu.com/article/c146541397aaba0bfcfc4cb9.html)

——————————————————————————————————


# 绝对简单︱整个安装与下载导航：

两个安装文件+参考两个百度经验
1、VMware Workstation Player 12安装文件——74MB（[链接](http://www.ithome.com/html/soft/171846.htm)）
2、Ubuntu Kylin 16.04 LTS 安装文件——1.04GB（[链接](http://cn.ubuntu.com/download/)）
3、VMware Workstation Player 12安装教程（[百度经验](http://jingyan.baidu.com/article/358570f64500dace4624fc7d.html)）
4、Ubuntu Kylin 16.04 LTS 安装教程（[百度经验](http://jingyan.baidu.com/article/49711c61696a1dfa441b7cbc.html)）

**应用一：报错****vmware提示：此虚拟机似乎正在使用中，取得该虚拟机的所有权失败错误怎么办？**

——————————————————————————————————

# 一、认识
# 免费虚拟机VMware Player 12

说到 Windows 系统上的虚拟机，[VMWare Workstation Pro](http://www.iplaysoft.com/vmware-workstation.html)无论在性能还是兼容性上都是目前最好的，可惜它收费高昂。而另一款官方免费精简版的虚拟机软件——[VMWare Player](http://www.iplaysoft.com/vmware-player.html)在性能上则和 Workstation 几乎完全一样，只是少了些附加的高级功能，但换取回来的是体积小巧＋完全免费使用，因此更加适合个人用户。2015年8月已经直接更新到VMware Player 12，下面贴一些他的特性：

1、VMware Player 12特性
对于大部分个人用户来说，其实**VMware Workstation Player 免费版**已经够用了，加上它体积十分小巧 (仅70多MB)，[性能](http://www.iplaysoft.com/tag/%E6%80%A7%E8%83%BD)不错，支持3D加速，而且运行流畅稳定，可以说是值得推荐作为首选的虚拟机软件的。下面是一些 VMware Workstation Player 提供的特性：
免费的 VMWare Workstation Player 体积仅 70 多MB
完美支持[Windows 10 系统](http://www.iplaysoft.com/windows10.html)以及 DirectX 10、OpenGL 3.3
宿主系统支持 Windows 与[Linux](http://www.iplaysoft.com/os/linux-platform)
软件主界面简洁，几乎所以的功能都不需要手工设置，适合给家长或小白们使用
支持创建虚拟机，实用性比以前的 VMware Player 旧版本有极大提升
支持 32 位与 64 位操作系统
支持4K显示器和高分辨率界面、支持多显示器不同DPI设置
增加虚拟机[USB](http://www.iplaysoft.com/tag/usb)3.0 支持
而缺点方面，唯一算是遗憾的比较实用的"虚拟机快照"功能在免费版上并不提供。对此功能有需求且追求免费的朋友只能选择[VirtualBox](http://www.iplaysoft.com/virtualbox.html)作为替代了。

2、更新内容——可以支持Ubuntu 15.04

VMware Workstation Player 12.0.0更新内容：
• 全面支持[Windows10](http://win10.ithome.com/)，可将Win10作为虚拟机和主机操作系统
• 增加以下操作系统支持：
- Ubuntu 15.04
- Fedora 22
- CentOS 7.1
- RHEL 7.1
- Oracle Linux 7.1
- VMware Project Photon
————————————————————————————

# 二、VMware Workstation Player 12下载与安装

VMware Workstation Player 12安装包下载链接可戳：[http://www.ithome.com/html/soft/171846.htm](http://www.ithome.com/html/soft/171846.htm)
![](https://img-blog.csdn.net/20161119152530174)

VMware Player 12 的安装过程相当简单，如果不需要自定义安装路径，一切按照软件的默认设置即可，安装过程非常人性化。
如果不会可以参考百度经验，链接：http://jingyan.baidu.com/article/358570f64500dace4624fc7d.html

————————————————————————————


# 三、搭建并下载操作系统：Ubuntu Kylin 16.04 LTS

更新上说只能支持到Ubuntu 15.04，但是笔者直接尝试16.04也是可以的，所以觉得应该兼容，而且可以运行。
![](https://img-blog.csdn.net/20161119153250883)

笔者已经搭建过，所以如果第一次的话你可以点击“创建虚拟机”，点击之后进入了以下界面：
![](https://img-blog.csdn.net/20161119153403630)
没有默认直接帮你装，所以你得去下载iso镜像文件，于是可以去ubuntu中文官网系统下载一个镜像，一共1.04GB，下载链接：http://cn.ubuntu.com/download/
笔者选择的是：优麒麟 16.04 LTS(长期支持)版本
![](https://img-blog.csdn.net/20161119153532918?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下载完之后选填iso镜像文件即可。
![](https://img-blog.csdn.net/20161119153828579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来都是随便取名字的环节啦，我就不赘述了，详情可以看百度经验：http://jingyan.baidu.com/article/49711c61696a1dfa441b7cbc.html

最后笔者按照百度经验的教程完成了，贴图一张：
![](https://img-blog.csdn.net/20161119154158411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**其他**
如果你是VMware Workstation12 pro的话，你可以参考以下链接进行安装与下载：http://www.jb51.net/article/97387.htm

————————————————————————————————
# 应用一：报错
# vmware提示：此虚拟机似乎正在使用中，取得该虚拟机的所有权失败错误
![](https://img-blog.csdn.net/20161119155025059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在虚拟机运行时，一次非正常关机。导致虚拟机出现以下错误：
此虚拟机似乎正在使用中。
如果此虚拟机已在使用中，请按“取消”按钮，以免损坏它。如果此虚拟机未使用，请按“取得所有权(&T)”按钮以获取它的所有权。
配置文件: D:\Virtual Machines\CentOS-6.3\CentOS-6.3.vmx
但是点击取得所有权，又报错：
取得该虚拟机的所有权失败。
虚拟机正在被你主机上的某个程序使用。
配置文件: D:\Virtual Machines\CentOS-6.3\CentOS-6.3.vmx
后来一阵乱折腾，突然想到是不是*.lck文件夹的问题，因为以前也遇到过类似问题。
（见[Vmware:Failed to lock main memory file “Windows Server 2003.vmem”](http://www.gemingcao.com/archives/vmware-failed-to-lock-main-memory-file.html)）
**于是一不做二不休，直接删除了D:\Virtual Machines\CentOS-6.3\下的*.lck文件夹，然后再虚拟机中重新打开D:\Virtual Machines\CentOS-6.3\CentOS-6.3.vmx。**
**呵，正常了。**

提示：如果怕出问题，可以先把D:\Virtual Machines\CentOS-6.3\下的*.lck文件夹移动到其他目录，不要直接删除。
参考博客：http://www.gemingcao.com/archives/vmware-failed-suoyouquanshibai.html

——————————————————————————————————————————————————

# 报错：开机时出现错误 VMware Player cannot connect to the virtual machine

![](https://img-blog.csdn.net/20161206120454928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用管理员身份运行即可.
针对这一问题，官网给出的解答是两个原因：
1、VMware授权服务没有运行，解决办法参考见blog：http://www.2cto.com/os/201111/112418.html
2、服务不具有管理员权限，用管理员身份运行



