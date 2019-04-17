# Ubuntu Mate + ROS Kinetic + raspberryPi 3 ARM64 - 站在巨人的肩膀上coding - CSDN博客





2018年04月07日 19:19:27[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：947








# 首先感谢 [UMRInside](http://www.cnblogs.com/UMRNInside/) 提供的原始镜像，64位的ubuntu ，https://github.com/chainsx/ubuntu64-rpi ，而后我在上面安装了Ubuntu Meta core 以及ROS kinetic，并且打包成img镜像，可以直接刷入使用。



注意，此镜像只适用于与64位的树莓派，目前是树莓派3B，3B+刚出不久，还没测试。文末有网盘地址。

1、首先解压.xz.镜像，使用win32diskimager 将镜像写入到microSD卡中。（镜像约7GB，所以尽量使用8G以上的SD卡，另外，boot分区下的cmdline.txt里面配置了屏幕分辨率，，默认是1080P）

2、首次使用准备好显示器键盘鼠标，以及5V2A电源等必备配件，上电开机。然后出现的是登录界面，默认用户名（user）：ubuntu，密码（password）：ubuntu。

3、登录到ubuntu mate桌面以后，首先要做的是扩展SD卡的分区容量，打开终端（ctrl+alt+t）：

 1、$ cat /sys/block/mmcblk0/mmcblk0p2/start #查看第二分区的起始地址
 回显：94208 #也许此数值会不同，请根据实际回显的值进行下面的操作。
 2、$ sudo fdisk /dev/mmcblk0
 Command (m for help): d    #d，删除分区 
 Partition number (1-4): 2    # 2，删除第二分区

 Command (m for help): n     #创建一个新分区 
 Partition type: 
   p primary (1 primary, 0 extended, 3 free) 
   e extended

 Select (default p): p      #创建主分区 
 Partition number (1-4, default 2): 2     #分区2 
 First sector (2048-7744511, default 2048): 94208     #输入第一次得到的第二分区起始扇区 
 Last sector, +sectors or +size{K,M,G} (94208-7744511, default 7744511):    #最后一个sector，默认即可Enter 
 Using default value 7744511

 Command (m for help): w    #将上面的操作写入分区表 
 The partition table has been altered! 
 $ sudo reboot
 启动完成以后，修复分区：
 $ sudo resize2fs /dev/mmcblk0p2 等待约2-3分钟

至此，SD卡已经扩容成功。



4、增加SWAP分区的大小：

$  sudo vi /etc/dphys-swapfile

找到CONF_SWAPFILE=64

改成1024，也就是1G左右，或者根据自己的需求修改，然后重启后生效。（修改后，重启时间变长）



5、安装turtlebot

我将三个源码包已放入网盘，源码git于2018年3月16日。

按照 创客智造里面的教程 https://www.ncnynl.com/archives/201612/1148.html，进行编译安装。

由于已经git完成，所以步骤可以减少很多，我只列出详细的步骤，不作解释了。

使用U盘或者sftp工具将三个源码包放入到 raspberryPi的/home/ubuntu 目录下，分别解压：

$  tar -zxvf rocon.tar.gz  & tar -zxvf kobuki.tar.gz & tar -zxvf turtlebot.tar.gz

进入rocon目录

$  cd ~/rocon

$ rosdep install --from-paths src -i -y

由于缺少pyqt5的工具，所以下面的一步会出错，先安装pyqt5的工具

$ sudo apt-get install pyqt5-dev-tools

$ catkin_make

编译会消耗大量内存，此时尽量只开一个窗口或者用远程工具连接进行编译。



进入kobuki目录

$  cd ~/kobuki

$  source ~/rocon/devel/setup.bash

$  rosdep install --from-paths src -i -y

注意，编译操作会消耗大量内存，以及长时间的等待，或者提前将swap扩展到2G，编译完成后再调回1G。

$  catkin_make



进入turtlebot目录

$ cd ~/turtlebot

$ source ~/kobuki/devel/setup.bash

$ rosdep install --from-paths src -i -y

$ catkin_make



编译完成，生成kobuki别名

$ sudo apt-get install ros-kinetic-kobuki-ftdi

$ rosrun kobuki_ftdi create_udev_rules

如果想要将编译好的包安装，可以在相应的目录下输入:

$ catkin_make install

----------------------我是分割线--------------------------

ubuntu mate ros kinetic img Baidu 盘地址：

链接：https://pan.baidu.com/s/1IzC7Pyb8IONeXbn3vH1a6w 密码：ulyp

turtlebot源码包地址

链接：https://pan.baidu.com/s/16fSLoRXiDg5Hcs-5CdT7_g 密码：fa2w



如无特殊说明，本人的文章文本均采用CC-BY 3.0许可，转载请注明原作者。





