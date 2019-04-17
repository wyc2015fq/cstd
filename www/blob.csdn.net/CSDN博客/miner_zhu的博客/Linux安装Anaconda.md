# Linux安装Anaconda - miner_zhu的博客 - CSDN博客





2018年08月10日 10:54:06[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：50








# Linux安装Anaconda

发布时间:2016-10-17 11:58:03来源:topspeedsnail.com作者:斗大的熊猫

Anaconda是用于科学计算的平台，它集成了很多关于Python科学计算的第三方库，可运行在Linux、Windows 和 macOS下。

它的功能包括：包管理、环境管理、700+软件包。Anaconda主要由Miniconda和Conda两个组件组成。

我在Ubuntu 16.04上安装Anaconda，安装步骤应该适用于任何Linux发行版。



Linux安装Anaconda

下载安装脚本：https://www.continuum.io/downloads，选择要安装的Python版本和系统架构：

# Python 3.5

$ wget https://repo.continuum.io/archive/Anaconda3-4.2.0-Linux-x86_64.sh  # 64位系统

$ wget https://repo.continuum.io/archive/Anaconda3-4.2.0-Linux-x86.sh     # 32位系统

# Python 2.7

$ wget https://repo.continuum.io/archive/Anaconda2-4.2.0-Linux-x86_64.sh  # 64位系统

$ wget https://repo.continuum.io/archive/Anaconda2-4.2.0-Linux-x86.sh     # 32位系统

运行安装向导：

$ bash Anaconda3-4.2.0-Linux-x86_64.sh

接受协议：

![Linux安装Anaconda](http://www.linuxdiyf.com/linux/uploads/allimg/161017/2-16101G20015b0.jpg)

yes

设置安装路径，默认安装在用户家目录：

![Linux安装Anaconda](http://www.linuxdiyf.com/linux/uploads/allimg/161017/2-16101G20026428.jpg)

最后，把安装路径添加到环境变量：

![Linux安装Anaconda](http://www.linuxdiyf.com/linux/uploads/allimg/161017/2-16101G2003J04.jpg)



测试安装

启动新终端；查看conda版本：

$ conda --version

conda 4.2.9

列出安装的包：

$ conda list

更新包：

$ conda update conda



Anaconda文档：https://docs.continuum.io/



