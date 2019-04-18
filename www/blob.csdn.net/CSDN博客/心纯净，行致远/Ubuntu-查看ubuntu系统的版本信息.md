# Ubuntu-查看ubuntu系统的版本信息 - 心纯净，行致远 - CSDN博客





2019年04月02日 18:01:32[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：42








安装软件或者搭建服务的时候，需要了解当前系统的版本信息，在这里分享怎么查看当前ubuntu系统的版本信息。

注：我这里使用的是带图形界面的Ubuntu

## 工具/原料
- 
Ubuntu系统的电脑一台或Ubuntu虚拟机

- 
Linux系统的电脑一台或Linux虚拟机


## 方法/步骤
- 
进入Ubuntu系统，点击左侧图标栏中的黑框，打开终端

![Ubuntu-查看ubuntu系统的版本信息](https://imgsa.baidu.com/exp/w=500/sign=916f9d7767600c33f079dec82a4c5134/5882b2b7d0a20cf4cba1a0bc7a094b36acaf99a4.jpg)

- 
左侧没有终端图标的情况，可以搜索按钮(图中左侧第一个)，输入termial，打开终端

![Ubuntu-查看ubuntu系统的版本信息](https://imgsa.baidu.com/exp/w=500/sign=0e61934a8a18367aad897fdd1e738b68/279759ee3d6d55fb0ccb284561224f4a20a4dd40.jpg)

- 
桌面终端也没有搜索图标的情况，可以使用快捷键打开终端

快捷键(Ctrl+Alt+t)

![Ubuntu-查看ubuntu系统的版本信息](https://imgsa.baidu.com/exp/w=500/sign=6162f3717e0e0cf3a0f74efb3a46f23d/9213b07eca806538fde7fa709bdda144ad348240.jpg)

- 
输入命令

cat /proc/version

显示如下

Linux version 4.10.0-28-generic (buildd@lgw01-12)         linux内核版本号

gcc version 5.4.0                                                                 gcc编译器版本号

Ubuntu 5.4.0-6ubuntu1                                                      Ubuntu版本号

![Ubuntu-查看ubuntu系统的版本信息](https://imgsa.baidu.com/exp/w=500/sign=9e4f055511950a7b75354ec43ad0625c/6a63f6246b600c33550380ab164c510fd9f9a15f.jpg)

- 
输入命令

uname -a

显示linux的内核版本和系统是多少位的：X86_64代表系统是64位的。

![Ubuntu-查看ubuntu系统的版本信息](https://imgsa.baidu.com/exp/w=500/sign=df80a04045540923aa69637ea258d1dc/738b4710b912c8fc91ffa8aef0039245d6882140.jpg)

- 
输入命令

lsb_release -a

显示如下

Distributor ID: Ubuntu                           //类别是ubuntu

Description:  Ubuntu 16.04.3 LTS          //16年3月发布的稳定版本，LTS是Long Term Support：长时间支持版本，支持周期长达三至五年

Release:    16.04                                    //发行日期或者是发行版本号

Codename:   xenial                               //ubuntu的代号名称

![Ubuntu-查看ubuntu系统的版本信息](https://imgsa.baidu.com/exp/w=500/sign=d428d762506034a829e2b881fb1349d9/9e3df8dcd100baa1801c19d84b10b912c8fc2ea4.jpg)

END




