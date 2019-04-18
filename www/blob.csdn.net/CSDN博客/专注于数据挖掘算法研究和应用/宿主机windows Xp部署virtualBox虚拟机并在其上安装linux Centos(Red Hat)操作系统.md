# 宿主机windows Xp部署virtualBox虚拟机并在其上安装linux Centos(Red Hat)操作系统 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年12月29日 09:26:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3665标签：[centos																[虚拟机																[windows																[linux																[xp																[图形](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
个人分类：[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)





 在Windows操作系统上安装虚拟机VirtualBox，在虚拟机上部署Linux Centos(Red Hat)操作系统：

一 虚拟机VirtualBox安装

1．下载Virtualbox：http://www.virtualbox.org/wiki/Downloads

基于windows操作系统为主机的虚拟机最新版本：VirtualBox-4.1.2-73507-Win.exe；

2．Virtualbox安装：

1）打开exe文件，进入安装页面；

2）默认路径和安装内容；

3）继续next直到Install安装页面；

4）选择“Install”开始安装，进入安装进度条；

5）安装完成；



二  VirtualBox上安装linux Centos(Red Hat)操作系统

1．新建虚拟电脑，设置虚拟名称和类型，为CentOS和RedHat（输入Centos自动识别为Red Hat类型）；

2．下面步骤可默认，最后设置虚拟文件存放目录（空间）；

3．下载centos6.2：

       ——镜像地址：[http://isoredirect.centos.org/centos/6/isos/i386/](http://isoredirect.centos.org/centos/6/isos/i386/)

      ——下载地址：[http://mirrors.163.com/centos/6.2/isos/i386/](http://mirrors.163.com/centos/6.2/isos/i386/)

      ——下载版本：

     1）终端版本CentOS-6.2-i386-minimal.iso，最小化软件包；

     2）完整版本[CentOS-6.2-i386-bin-DVD1.iso](http://mirrors.163.com/centos/6.2/isos/i386/CentOS-6.2-i386-bin-DVD1.iso) /[CentOS-6.2-i386-bin-DVD2.iso](http://mirrors.163.com/centos/6.2/isos/i386/CentOS-6.2-i386-bin-DVD2.iso) ，完整软件包，应该可以选择装终端或图形，不过没找到安装为图形方式的选项；

    3）图形版本CentOS-6.2-i386-LiveCD.iso，直接介质就启动进入图形界面，只能运行在光盘，加载到内存，不能安装到硬盘，似乎选择桌面的“Install to Hard Drive”可从光盘安装到硬盘，但我选择后一直没有响应，所以放弃了；

    4）图形版本CentOS-6.2-i386-LiveDVD.iso，和livecd一样是运行在光盘，不能安装到硬盘，后来发现将虚拟机virtualbox配置的内存和空间提高，进入桌面后“Install to Hard Drive”就可以启动并进入Centos的安装，空间我设置为20G（可能不需要那么高），内存设置为1024M（设置为768M被证明是可以的，设置为512M就不可以）；

5.安装过程按步骤即可，终端版本设置root的密码；](https://so.csdn.net/so/search/s.do?q=xp&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=centos&t=blog)




