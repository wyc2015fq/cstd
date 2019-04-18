# ubuntu14.04系统假死及强制关机后产生的问题解决 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:26[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：4635

待机时间比较长时，unbuntu可以设置锁定屏幕并log out，再次使用时需要输入密码登录。昨天在重新登录时遇到了系统假死，一直停留在登录界面并且没有输入密码的输入框。长按电源键强制关机后重新开机，产生了一系列的问题，具体是：
spyder程序图标、chrome程序图标无效，无法打开对应的应用程序，无法用快捷键启动终端。我估计是强制关机导致快捷键和程序图标的快捷方式失效，尝试了用以下方法，成功解决了这个问题。
首先，用ctrl+alt+F1启动tty1，
输入： 
sudo kill x
sudo stop lightdm    #lightdm是ubuntu的[桌面显示](https://www.baidu.com/s?wd=%E6%A1%8C%E9%9D%A2%E6%98%BE%E7%A4%BA&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YkrA79mWDkPjfdrHTvuA7-0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EP1fvnHfkPjRvnj04PWR1rjRz)环境管理器
sudo start lightdm
之后再用ctrl+alt+F7退出tty1
在桌面启动图标中输入terminal，启动终端。再次尝试用快捷键ctrl+alt+t , 成功启动终端。
之后，在home文件夹下发现了一个java的崩溃后的log文件，其中有如下文字：
Failed to write core dump. Core dumps have been disabled. To enable core dumping, try "ulimit -c unlimited" before starting Java again
在终端输入
ulimit -c unlimited
并升级spyder：conda update spyder
再次在终端中输入：spyder，成功启动spyder。
=====关于终端====
ctrl+alt+Fn（n=1~6）打开的是“系统级”终端，而ctrl+alt+T打开的是应用程序级的终端。
