# Ubuntu 16中如何安装TeamViewer 12 - 数据之美的博客 - CSDN博客
2018年01月09日 23:13:57[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：480
安装过程非常简单，大家按如下步骤操作即可：
1、使用终端下载安装程序(32位和64位Ubuntu通用)
wget http://download.teamviewer.com/download/teamviewer_i386.deb
2、在终端中执行如下命令进行安装
sudo dpkg -i teamviewer_i386.deb
3、使用如下命令处理依赖问题
sudo apt-get install –f 
