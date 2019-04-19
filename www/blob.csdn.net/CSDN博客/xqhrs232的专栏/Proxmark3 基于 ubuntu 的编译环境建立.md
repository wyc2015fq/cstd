# Proxmark3 基于 ubuntu 的编译环境建立 - xqhrs232的专栏 - CSDN博客
2019年04月15日 17:37:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：16
原文地址::[https://blog.csdn.net/etual/article/details/72819828](https://blog.csdn.net/etual/article/details/72819828)
相关文章
1、[https://github.com/Proxmark/proxmark3/wiki/Ubuntu-Linux](https://github.com/Proxmark/proxmark3/wiki/Ubuntu-Linux)
vmware12 安装 ubuntu 16.04 LTS AMD64
ubuntu 更新和安装需要的包
sudo apt-get update
sudo apt-get install p7zip git build-essential libreadline5 libreadline-dev libusb-0.1-4 libusb-dev libqt4-dev ncurses-dev perl pkg-config wget libtolua-dev
安装编译链
https://sourceforge.net/projects/devkitpro/files/devkitARM/
下载编译链，例如我的 64bit系统，目前最新版是 r46
devkitARM_r46-x86_64-linux.tar.bz2
创建编译链用的文件夹
sudo mkdir –p /opt/devkitpro/
cd /opt/devkitpro/
解压缩到当前目录，例如我的源文件在 ~/proxmark/ 目录
sudo tar xjf ~/proxmark/devkitARM_r46-x86_64-linux.tar.bz2
加入环境变量
echo 'export PATH=${PATH}:/opt/devkitpro/devkitARM/bin/' >> ~/.bashrc
source ~/.bashr
下载源代码
git clone https://github.com/Proxmark/proxmark3.git
编译
make help 查看可以用的编译命令
make all 编译
make clean 清除
注意，必须先编译 client，再编译其他，否则会有错误，或者直接 make all 也是可以的。
完结
参考
https://github.com/Proxmark/proxmark3/wiki/Ubuntu-Linux
