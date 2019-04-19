# 树莓派teamviewer远程 windows远程桌面 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年05月31日 18:22:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：610

[https://mirror.tuna.tsinghua.edu.cn/help/raspbian/](https://mirror.tuna.tsinghua.edu.cn/help/raspbian/)
用这个源后，再安装
apt-get update
[https://download.teamviewer.com/download/linux/teamviewer-host_armhf.deb](https://download.teamviewer.com/download/linux/teamviewer-host_armhf.deb)
apt install ./teamviewer-host_13.x.yyy_[arch].deb
支持手机端，web端
如果想要用windwos的远程桌面连接
可以
（1）在树莓派上安装xrdp服务
      sudo apt-get install xrdp
      （2）在树莓派上安装tightvncserver服务
      sudo apt-get install tightvncserver
      安装好以上两个服务后，即可使用WIndows自带工具远程连接到树莓派。
