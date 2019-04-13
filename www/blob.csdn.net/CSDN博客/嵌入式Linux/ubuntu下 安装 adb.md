
# ubuntu下 安装 adb - 嵌入式Linux - CSDN博客

2015年08月21日 10:33:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：746




1、把adb tool工具考到你要安装的目录夏目
解压后为：/home/qyy/install/platform-tools
2、配置环境变量
vi ~/.bashrc
export PATH=${PATH}:/home/qyy/install/platform-tools
3、得创建两个文件
1）创建51-android.rules文件
cd /etc/udev/rules.d
sudo vim 51-android.rules
写入
SUBSYSTEM=="usb", ATTR{idVendor}=="2207", MODE="0666", GROUP="plugdev"
qyy@qyy-hena:~$ lsusb
![](https://img-blog.csdn.net/20150821103434741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意：ATTR{idVendor}=="2207",其中2207为usb的id号
2）创建adb_usb.ini文件
cd /home/qyy/.android（得创建：sudo mkdir .android目录，本来是没有的）
sudo vim /home/qyy/.android/adb_usb.ini输入
0x2207
注意：其中0x2207也是usb的id号
4、cd /home/qyy/install/platform-tools
第一次执行：
sudo ./adb kill-server
sudo ./adb shell


weiqifa@weiqifa-Inspiron-3847:/usr/bin$ sudo ln -s ~/software/platform-tools/adb ./adb

进入到我的usr/bin目录下，把platform-tools/adb下面的命令　链接到usr/bin/目录下的adb这样就可以不用每次进入adb下面运行adb

usdo是在usr/bin/下面去找命令的，所以要运行上面的命令。


