# 宿主机windows Xp部署virtualBox虚拟机并在其上安装linux ubuntu操作系统 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年09月10日 11:26:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2779标签：[虚拟机																[ubuntu																[linux																[windows																[xp																[oracle](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Linux																[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)](https://blog.csdn.net/fjssharpsword/article/category/6480442)





 在Windows操作系统上安装虚拟机VirtualBox，在虚拟机上部署Linux ubuntu操作系统：

一 虚拟机VirtualBox安装

1．下载Virtualbox：[http://www.virtualbox.org/wiki/Downloads](http://www.virtualbox.org/wiki/Downloads)

基于windows操作系统为主机的虚拟机最新版本：VirtualBox-4.1.2-73507-Win.exe；

2．Virtualbox安装：

1）打开exe文件，进入安装页面；

2）默认路径和安装内容；

3）继续next直到Install安装页面；

4）选择“Install”开始安装，进入安装进度条；

5）安装完成；

3．创建虚拟机：

1）打开Oracle VM VirtualBox，选择“新建”进入向导；

 2）设置虚拟机名称及运行的操作系统类型；

3）设置虚拟机内存大小；

4）创建新的虚拟硬盘；

选择“创建新的虚拟硬盘”，下一步进入向导：

       默认选择“VDI(VirtualBox Disk Image)”下一步：

       选择虚拟硬盘类型为“Dynamically allocated”动态扩展方式，下一步：

虚拟硬盘路径和大小设置，下一步：

确认虚拟硬盘设置信息，选择“Create”完成虚拟硬盘新建。

5）虚拟机摘要确认并完成创建。



 二 linux操作系统ubuntu安装

1．下载Ubuntu：http://www.ubuntu.com/download/ubuntu/download

最新版本ubuntu-11.04-desktop-i386.iso；

2．Virtualbox完成虚拟机的创建；

3．打开Oracle VM VirtualBox，选择“fjs_linux”并“开始”，进入安装操作系统向导：

选择操作系统存放路径，下一步：

启动安装操作系统：

提示操作系统正在安装。

4．进入虚拟机环境，看到Ubuntu操作系统进入安装页面，按照其提示完成安装即可，过程出现下载的部分可以跳过，可以加快安装速度。](https://so.csdn.net/so/search/s.do?q=xp&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)




