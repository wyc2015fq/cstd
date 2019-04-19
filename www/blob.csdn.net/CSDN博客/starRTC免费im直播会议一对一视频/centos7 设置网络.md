# centos7 设置网络 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月04日 13:49:36[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：404
[https://lintut.com/how-to-setup-network-after-rhelcentos-7-minimal-installation/](https://lintut.com/how-to-setup-network-after-rhelcentos-7-minimal-installation/)
First, type “nmcli d”
 command in your terminal for quick list ethernet card installed on your machine:
Type “nmtui”
 command in your terminal to open Network manager. After opening Network manager chose “Edit
 connection” and press Enter
choose you network interfaces and click “Edit”
DHCP configuration
Choose “Automatic”
 in IPv4 CONFIGURATION and check Automatically
 connect check box and press OK and quit from Network manager.
Reset network services:
service network restart
桥接：
