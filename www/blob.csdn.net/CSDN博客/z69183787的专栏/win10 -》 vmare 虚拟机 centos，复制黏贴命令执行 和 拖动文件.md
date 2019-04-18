# win10 -》 vmare 虚拟机 centos，复制黏贴命令执行 和  拖动文件 - z69183787的专栏 - CSDN博客
2018年02月07日 18:51:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6452
**1、安装 vmare tools**
**2、下载putty，虚拟机安装 openssh，远程登录操作 centos**
//拖动
[http://blog.csdn.net/u010899985/article/details/54999487](http://blog.csdn.net/u010899985/article/details/54999487)
## **1.首先要确保自己的虚拟机安装了vmware tools**
 ①点击虚拟机的 虚拟机(M)**->重现安装VMware Tools（T）**
**![](https://img-blog.csdn.net/20170212100103278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg5OTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**②此时系统会弹出装载虚拟CD驱动器 点击打开文件**
**![](https://img-blog.csdn.net/20170212100740177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg5OTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**
**③**打开文件后可将 文件夹里的文件全部复制到自己的某个文件夹中，例如放在桌面****
****![](https://img-blog.csdn.net/20170212101227649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg5OTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)****
****④**Ctrl+ALT+t打开终端，**
 输入命令：****cd Desktop****
      tar -xvf VMwareTools-10.0.0-2977863.tar.gz(注意：这里的文件名是你自己桌面上那个.gz文件 的名称，根据自己对应的版本来哦)
**这时候你的桌面上应该多了一个文件夹，下面执行命令：**
 cd vmware-tools-distrib
**  sudo ./vmware-install.pl**
**然后输入密码，一路 enter/YES，就OK了！**
## **到这里VMware tools安装完成，试着将文件拖动到liunx系统当中！**
## **2.安装完VMware tools如果还不能解决文件从从宿主机拖动复制文件到虚拟机VM，可尝试下面几个方法：**
**①在windows下复制的东东西在linux下粘贴，有时可行，有时不行，**可以尝试到服务中看一下，****
点击：任务管理器****——>****服务，是否有vmware开头的服务项没有启动。
**![](https://img-blog.csdn.net/20170212102611953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg5OTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**
**一般电脑安装的金山卫士或360杀毒软件有时会把Vmware虚拟机的的一些开机启动项关闭了，确保VMware开头服务的均启用，重启计算机就可以了。**
②虚拟机****——>****客户机隔离，选中均启用拖放和复制粘贴
******![](https://img-blog.csdn.net/20170212102931396?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg5OTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)******
**③****选择View菜单中的Fit window now（自动适应客户机），然后再拖动文件。******
![](https://img-blog.csdn.net/20170212103642732?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg5OTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**原因是：如果你虚拟机的桌面尺寸小于VMware窗口的尺寸，那就不能拖动文件（也是在某论坛看到的，不知道原因，但确实在View->自动适应客户机 后就可以复制了）。**
**④ubuntu高版本与低版本在拖动文件有一点小区别：**
** 比如我使用的是Ubuntu 15.10版本，将文件拖动到界面是不能**直接**复制到系统中，可以先打开 Ubuntu中的某一个文件夹，然后将你所要复制的文件拖入到该文件夹，就可以随意复制了！**
**![](https://img-blog.csdn.net/20170212105225192?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg5OTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**
**到此，关于从宿主机拖动复制文件到虚拟机VM中的解决方法就是这些了，如果还有其他方法，大家可以补充哦![吐舌头](http://static.blog.csdn.net/xheditor/xheditor_emot/default/tongue.gif)**
