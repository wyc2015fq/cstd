# Ubuntu安装MediaInfo - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月11日 16:15:29[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：258

## Ubuntu版
打包下载：[艺搜下载](http://dl.vmall.com/c0b9kx4fte)
适用于Ubuntu 12.10(i386)
安装libzen0_v0.4.29
**sudo ****dpkg **-i libzen0_0.4.29-1_i386.xUbuntu_12.10.deb
安装libmediainfo0_v0.7.64(需要先安装libzen0)
**sudo ****dpkg **-i libmediainfo0_0.7.64-1_i386.xUbuntu_12.10.deb
安装libtiff4
**sudo ****dpkg **-i libtiff4_3.9.5-2ubuntu1.5_i386.deb
安装libwxbase2.8-0
**sudo ****dpkg **-i libwxbase2.8-0_2.8.12.1-6ubuntu2_i386.deb
安装libwxgtk2.8-0(需要先安装libtiff4和libwxbase2.8-0)
**sudo ****dpkg **-i libwxgtk2.8-0_2.8.12.1-6ubuntu2_i386.deb
安装GUI_v0.7.64(需要先安装libmediainfo0和libwxgtk2.8-0)
**sudo ****dpkg **-i mediainfo-gui_0.7.64-1_i386.Debian_6.0.deb
**sudo** ldconfig
安装完成后，可以看到在/usr/bin目录下生成了mediainfo-gui程序
输入sudo mediainfo-gui即可执行程序
安装CLI命令行版_v0.7.64(需要先安装libmediainfo0)
**sudo ****dpkg **-i mediainfo_CLI_0.7.64-1_i386.Debian_6.0.deb
**sudo** ldconfig
用法：mediainfo tj.mp4
GUI版截图：
CLI版截图：
## 艺搜参考
[http://mediaarea.net/zh/MediaInfo/Download](http://mediaarea.net/zh/MediaInfo/Download)
[http://packages.ubuntu.com/zh-cn/precise/libwxgtk2.8-0](http://packages.ubuntu.com/zh-cn/precise/libwxgtk2.8-0)
## Ubuntu版
打包下载：[艺搜下载](http://dl.vmall.com/c0b9kx4fte)
适用于Ubuntu 12.10(i386)
安装libzen0_v0.4.29
sudo dpkg -i libzen0_0.4.29-1_i386.xUbuntu_12.10.deb
安装libmediainfo0_v0.7.64(需要先安装libzen0)
sudo dpkg -i libmediainfo0_0.7.64-1_i386.xUbuntu_12.10.deb
安装libtiff4
sudo dpkg -i libtiff4_3.9.5-2ubuntu1.5_i386.deb
安装libwxbase2.8-0
sudo dpkg -i libwxbase2.8-0_2.8.12.1-6ubuntu2_i386.deb
安装libwxgtk2.8-0(需要先安装libtiff4和libwxbase2.8-0)
sudo dpkg -i libwxgtk2.8-0_2.8.12.1-6ubuntu2_i386.deb
安装GUI_v0.7.64(需要先安装libmediainfo0和libwxgtk2.8-0)
sudo dpkg -i mediainfo-gui_0.7.64-1_i386.Debian_6.0.deb
sudo ldconfig
安装完成后，可以看到在/usr/bin目录下生成了mediainfo-gui程序
输入sudo mediainfo-gui即可执行程序
安装CLI命令行版_v0.7.64(需要先安装libmediainfo0)
sudo dpkg -i mediainfo_CLI_0.7.64-1_i386.Debian_6.0.deb
sudo ldconfig
用法：mediainfo tj.mp4
GUI版截图：
CLI版截图：
## 艺搜参考
[http://mediaarea.net/zh/MediaInfo/Download](http://mediaarea.net/zh/MediaInfo/Download)
[http://packages.ubuntu.com/zh-cn/precise/libwxgtk2.8-0](http://packages.ubuntu.com/zh-cn/precise/libwxgtk2.8-0)
