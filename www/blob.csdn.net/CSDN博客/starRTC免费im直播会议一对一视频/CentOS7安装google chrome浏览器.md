# CentOS7安装google chrome浏览器 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月13日 18:28:17[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：40
1，下载离线包
rpm
2,rpm -ivh **
报错：
lsb_release被chrome依赖
libXss.so被chrome依赖
libappindicator3被chrome依赖
yum -y install  redhat-lsb
yum install libappindicator*
  yum -y install libXss*
再次安装即可。
