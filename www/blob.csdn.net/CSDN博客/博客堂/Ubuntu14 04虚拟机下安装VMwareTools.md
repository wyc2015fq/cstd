# Ubuntu14.04虚拟机下安装VMwareTools - 博客堂 - CSDN博客





2017年08月13日 16:33:18[最小森林](https://me.csdn.net/u012052268)阅读数：3221










- [Ubuntu1404虚拟机下安装VMwareTools](#ubuntu1404虚拟机下安装vmwaretools)- [1选择虚拟机菜单栏安装VMware tools](#1选择虚拟机菜单栏安装vmware-tools)
- [2VMware Tools 光盘](#2vmware-tools-光盘)
- [3安装vmware-tools](#3安装vmware-tools)
- [4点击自动适应](#4点击自动适应)
- [5重启](#5重启)





# Ubuntu14.04虚拟机下安装VMwareTools

众所周知在VMware虚拟机中安装好了VMware Tools，才能实现主机与虚拟机之间的文件共享，同时可支持自由拖拽的功能，鼠标也可在虚拟机与主机之前自由移动（而不再用按ctrl+alT释放），而且还可以令虚拟机屏幕也可实现**全屏化 !**

那么Ubuntu 系统如何安装VMware tools的呢?

## 1选择虚拟机菜单栏–安装VMware tools

VMware Workstation界面下，点击： 

虚拟机——>安装VMware Tools 

（英文为：VM——> Install VMware Tools）
## 2VMware Tools 光盘

打开ubuntu文件系统，会出现一个光盘图标，双击打开，如图所示。 
![这里写图片描述](https://img-blog.csdn.net/20141120021249113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmVud3VkYW8yNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

找到VMwaretools-9.6.0-1294478.tar.gz
选择【复制到】，放在/tmp文件夹下。

然后右击【提取到此处】（就是解压缩）

这时候/tmp文件夹就多了一个vmware-tools-distrib的文件夹 

## 3、安装vmware-tools

3.1 打开“终端”

3.2 进入/tmp文件夹

```bash
cd /tmp
```

3.3 再进入vmware-tools-distrib文件夹
`cd /vmware-tools-distrib  `
3.4、安装vmware

```bash
sudo ./vmware-install.pl
```

输入管理员密码，一路回车即可。

## 4点击自动适应

在vmware中点击“查看”–>”自动调整大小”–>”自动适应客户机”

## 5重启

重启后，等待你的是如此舒适的界面。 
![这里写图片描述](https://img-blog.csdn.net/20141120030116171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmVud3VkYW8yNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





