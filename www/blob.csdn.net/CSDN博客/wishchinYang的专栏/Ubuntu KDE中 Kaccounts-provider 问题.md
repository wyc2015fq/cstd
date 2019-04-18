# Ubuntu KDE中 Kaccounts-provider 问题 - wishchinYang的专栏 - CSDN博客
2019年02月08日 14:49:37[wishchin](https://me.csdn.net/wishchin)阅读数：89
      安装KDE之后Kaccounts-provider 总是不能覆盖16.04版本，多次修正仍然不能命令通过；
      参考：[ubuntuKylin17.04重装KDE]()..,使用强制覆盖命令 
**代码:**
apt-get -o Dpkg::Options::="--force-overwrite" -f install
       依然不能使用KDM，不知为何。但至少能在Ubuntu18.04上安装ROS-Melodic（旋律）系统。
