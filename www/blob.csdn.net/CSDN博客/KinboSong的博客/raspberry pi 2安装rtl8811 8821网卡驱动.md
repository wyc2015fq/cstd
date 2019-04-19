# raspberry pi 2安装rtl8811/ 8821网卡驱动 - KinboSong的博客 - CSDN博客
2017年07月18日 18:50:01[songjinbo3](https://me.csdn.net/KinboSong)阅读数：1617
							转载：
[Raspberry PI 2上的802.11ac网卡驱动编译](http://www.cnblogs.com/xiedidan/p/4994635.html%20)
最近在树莓派2上折腾视频，用来做FPV，但是发现2.4G的控会严重干扰2.4G WIFI，在开控的时候我的台式机+外置USB网卡都频频掉线。考虑了一下，5.8G的频段虽然相对2.4G穿透能力更低，但是现阶段5.8G更干净，并且5.8G与2.4G没有倍频关系，不容易互相干扰（1.2G的图传就会干扰2.4G的控）。
于是去买了个支持双频11AC的网卡，拿到之后直接插上Pi2，果然——不认，lsusb查了下这个网卡还比较新，用的是RTL8811芯片，官网上提供了Linux下的驱动，和GitHub上的比较了一下还算比较新。仔细看了下驱动包发现里面没有RTL8811的驱动，对比Github上的源码Makefile，认定应该是用RTL8821A的驱动就可以。
首先在PC上实验一下，解压驱动包，安装脚本加上执行权限chmod +x install.sh，然后./install.sh（需要配好内核代码和编译环境），就可以了，至少这个驱动是可以用的。
进一步在树莓派上编译内核驱动，首先是要获取树莓派Linux内核源码。树莓派的Linux内核是定制的，源码发布在https://github.com/raspberrypi/linux 。从GitHub上不一定可以找到原装镜像的源码版本，如果源码版本和运行的镜像内核版本不对应，编译出来的内核模块就无法插入。为了减少不必要的麻烦，最好直接编译一份最新的内核来使用，我是直接在树莓派上做的内核编译，至少需要4个小时。为了节约时间，经常编译的话可以在PC上搭建交叉编译环境，这里就不展开说了。
准备内核编译环境和源码，当前的镜像（官方镜像：2015-09-24-raspbian-jessie.zip）中已经包含了大部分编译环境：
```
sudo apt-get bc #内核编译环境需求：高精度计算语言
git clone https://github.com/raspberrypi/linux.git #假设源码在/home/pi/linux
cd /lib/modules
mkdir 4.1.y #先随便建立一个目录，有可能需要sudo，酌情添加，以下不再提示
cd 4.1.y
ln -s /home/pi/linux build
ln -s /home/pi/linux source
接下来要获取现在运行内核的内核配置，make的时候就可以根据这个现成配置自动编译（不需要手工去指定各项配置，解除了一件很繁重的工作）。由于最新树莓派内核已经把configs作为一个模块编译，所以需要先加载：
```
sudo modprobe configs
cd /home/pi/linux
zcat /proc/config.gz > .config #得到内核配置文件
make #开始编译内核
```
执行make之后，需要回答几个问题（因为老版本内核配置没有包含新版本的某些设置），这里我全部按m编译为模块，以便以后需要的时候可以modprobe加载。
编译开始之后就需要好几个小时等待内核编译完成，如果用SSH等网络方式登陆到树莓派上，很容易掉线导致编译停止，最好直接用串口或者screen等方式启动make。
在Pi2上如果想快一点可以用make -j4，使用4核编译。
内核编译完成后需要继续编译module，这一步比较快：
```
make modules
sudo make modules_install
sudo cp arch/arm/boot/Image /boot/kernel-4.1.y.img #将新编译的内核拷贝到/boot
```
接下来需要配置树莓派使用新内核启动，编辑/boot/config.txt，追加一行：
```
kernel=/boot/kernel-4.1.y.img
```
保存退出之后，用sudo reboot重启即可，启动完成之后/lib/modules下面会多一个以当前内核版本命名的目录，这个目录名字和uname -r获取的结果是一致的。cd进去看看，如果没有build和source就像前面一样，把Linux内核源码链接过来。
终于可以开始编译内核驱动了。由于驱动没有提供树莓派的编译选项，需要手工修改Makefile，就不再执行./install.sh脚本，而是手工调用make：
```
sudo apt-get install unrar
cd /home/pi #假设驱动包在/home/pi
unrar x 8811linux.rar
cd 8811linux/driver/rtl8821AU_linux_v4.3.14_13455.20150212_BTCOEX20150128-51 #具体路径根据版本调整
vim Makefile
```
在Makefile中确认CONFIG_RTL8821A = y存在，然后搜索CONFIG_PLATFORM_I386_PC将其设为n，并添加一行CONFIG_PLATFORM_ARM_RPI = y。
继续搜索CONFIG_PLATFORM_I386_PC的处理代码块，在其后面添加以下处理脚本：
```
ifeq ($(CONFIG_PLATFORM_ARM_RPI), y)
EXTRA_CFLAGS += -DCONFIG_LITTLE_ENDIAN
ARCH := arm
CROSS_COMPILE :=
KVER  := $(shell uname -r)
KSRC := /lib/modules/$(KVER)/build
MODDESTDIR := /lib/modules/$(KVER)/kernel/drivers/net/wireless/
endif
保存退出，到现在准备活动终于完成，可以开始编译驱动：
```
make
sudo make install
sudo insmod 8821au.ko
```
以上是正常编译Linux内核模块的步骤，同样make的时候可以加上-j4参数。
实际上在树莓派上编译的时候有两个坑。首先是编译的时候会报错：
```
-Werror=date-time/macro “__DATE__” might prevent reproducible builds
```
解决这个错误只需要在Makefile中查找EXTRA_CFLAGS，并添加一行EXTRA_CFLAGS += -Wno-error=date-time即可。
其次，我这个版本的驱动引用了strnicmp函数，但是目前的树莓派Linux内核源码（4.1.17）并没有定义这个函数，不过定义了strncasecmp，功能是相同的。grep驱动代码找到所有的strnicmp替换为strncasecmp即可。
顺利的话insmod之后网卡会亮起来，可以用dmesg和iwconfig进行确认。
最后修改/etc/network/interfaces添加WIFI配置就可以上网了：
```
allow-hotplug wlan0
iface wlan0 inet dhcp
wpa-ssid <YOUR-ACCESS-POINT-SSID>
wpa-psk <YOUR-ACCESS-POINT-PASSKEY>
```
测试了一下5.8G的网络确实干扰少很多，但穿墙效果不行。由于FPV是在空中，较少有阻隔，问题也不大就是了。
另外研究了一下这个网卡是50mW发射功率的，比起正常图传来说小了很多，用来前期做实验折腾是不错的，后期需要增程的话可以换大功率网卡或者加装双向WIFI功放。
```
```
