# 为AM335x+Linux移植SGX+OpenGL+Qt5之完全开发笔记 - xqhrs232的专栏 - CSDN博客
2017年12月21日 10:51:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：453
原文地址::[http://www.eefocus.com/marianna/blog/15-04/311437_e4f0a.html](http://www.eefocus.com/marianna/blog/15-04/311437_e4f0a.html)
相关文章
1、在Beaglebone Black板上使用Yocto移植Qt5----[http://blog.csdn.net/ab454619135/article/details/50961253](http://blog.csdn.net/ab454619135/article/details/50961253)
2、Compiling Qt5 in Linux AM335x----[http://e2e.ti.com/support/embedded/linux/f/354/p/259862/977368](http://e2e.ti.com/support/embedded/linux/f/354/p/259862/977368)
3、[Buildroot] Qt 5.3.0 fails to build with eglfs (ti-glx) enabled, ARM platform (beaglebone black)----[http://lists.busybox.net/pipermail/buildroot/2014-June/100044.html](http://lists.busybox.net/pipermail/buildroot/2014-June/100044.html)
这里的主机是64位的ACER
 5745DG，安装了桌面发行版Fedora20，桌面环境是GNOME。
这里的目标板是CortexA8的AM335x，安装了之前编译好的U-Boot、Kernel和Filesystem，安装的过程见《U-Boot
 for AM335x》、《为AM335x移植Linux内核主线代码》系列。
Step 1: Install
 Qt for Master
给主机安装Qt的方法可以非常简单粗暴：
yum install qt5*
yum install qt-creator
当然也可以不使用这样简单粗暴的“全部的通通的安装”的方式，而是选择需要的组件来安装，比如:
yum install qt5-qtbase
yum install qt5-qttools
安装完成之后，文件系统会增加以下的内容：
/usr/bin/qmake-qt5
/usr/bin/qtcreator
/usr/share/qt5/
/usr/share/qtcreator/
/usr/lib64/qt5/
/usr/lib64/qtcreator/
执行一个示例程序试试：
[maria@localhost ~]$ cd /usr/lib64/qt5/examples/quick/demos/clocks/
[maria@localhost lighting]$ ./clocks
NOTE:也可以采用编译源代码的方式来安装。在解压后的源代码目录下执行：
[maria@localhost qt-x86]$ ./configure -opensource -confirm-license -debug -prefix ../../build -platform linux-g++ -v
[maria@localhost qt-x86]# gmake -j8
[maria@localhost qt-x86]# gmake -j8 install
编译过程大概会花两个小时，完成之后运行示例程序，和使用yum来安装的效果是一样的：
[maria@localhost qt]$ cd ../build/examples/quick/demos/clocks/
[maria@localhost lighting]$ ./clocks
NOTE：configure命令中-prefix的“当前目录”为源代码目录下的qtbase。
NOTE：readelf命令可以用来观察可执行文件需要的动态库和装载器，从而知道动态链接的库为什么找不到。
NOTE：ldd命令可以用来观察可执行文件的库依赖。
NOTE：rpm
 -qf命令可以用来观察库文件所属的软件包。
NOTE： sync将内存缓存的文件强制写入磁盘，使用tftp获取文件之后，需要先执行sync再断电。
Step 2: About
 SGX
在编译目标板Qt之前，先要理解什么是SGX：
The SGX subsystem is a Texas Instruments instantiation of the POWERVR SGX530 core form Imagination Technologies Ltd.
The 2D/3D graphics accelerator (SGX) subsystem accelerates 2-dimensional (2D) and 3-dimensional (3D) graphics applications. The SGX subsystem is based on the POWERVRSGX
 core from Imagination Technologies. SGX is a new generation of programmable POWERVR graphic cores. The POWERVR SGX530 v1.2.5 architecture is scalable and can target all market segments from mainstream mobile devices to high-end desktop graphics. Targeted applications
 include feature phone, PDA, and hand-held games.
[http://processors.wiki.ti.com/index.php/AM35x-OMAP35x_Graphics_SDK_Getting_Started_Guide](http://processors.wiki.ti.com/index.php/AM35x-OMAP35x_Graphics_SDK_Getting_Started_Guide)
[http://processors.wiki.ti.com/index.php/SGXDbg](http://processors.wiki.ti.com/index.php/SGXDbg)
[http://processors.wiki.ti.com/index.php/Building_Qt](http://processors.wiki.ti.com/index.php/Building_Qt)
[http://www.thing-printer.com/3d-rendering-pvr-sgx-beaglebone/](http://www.thing-printer.com/3d-rendering-pvr-sgx-beaglebone/)
TI SOCs例如AM335x，具备3D核心，它能够使用dedicated
 hardware进行3D加速；而这里的delicated
 hardware就基于SGX技术；
Graphics cores是用来加速图形的，它并没有video
 decode功能，视频加速神马的和它没有关系；
视频加速和OpenGL ES1.1 OpenGL ES2.0有关（不推荐使用OpenVG
 1.1），它们需要调用SGX驱动；
OpenGL可以做成单独的application，也可以做成Android、Qt、Xrog等的back-end。
SGX core没有包含在ARM核里，但是它的Graphics
 drivers需要跑在ARM核上（所有的东西都需要跑在ARM核上），怎么办呢？其实Graphics
 drivers包含有OS
 specific driver ，它能够将SGX
 core做内存映射，因此可在ARM
 core上对图形引擎SGX编程。
到这里为止，是不是弄清楚了SGX、OpenGL、Qt之间的关系了呢？(^
 - ^)
首先，移植Qt需要指定它的platform，也就是eglfs、directfb、linuxfb等等；
其次，安装eglfs需要OpenGL图形库；
最后，OpenGL会调用SGX，驱动delicated
 hardware；
Linux的mainline里面并没有SGX
 Driver，因为TI公司并没有将其开源。TI公司不开源的代码极少，SGX是其中之一（也许是唯一一个）。
因为SGX的驱动不开源，所以必须要使用TI公司提供的SDK包，将二进制可执行文件打包进内核。
另外，在编译SGX之前，还要先制作编译好的Kernel和目标文件系统，以及为主机安装ssh、ftp、tftp服务。
Step 3: Install SSH, FTP, TFTP Services
为了调试的方便，这里为主机安装SSH、FTP、TFTP服务：
[root@localhost maria]# yum install vsftpd
[root@localhost maria]# yum install ftp
[root@localhost maria]# yum install tftp-server
[root@localhost maria]# yum install tftp
[root@localhost maria]# service sshd start
[root@localhost maria]# service vsftpd start
[root@localhost maria]# service tftp start
网络功能对于嵌入式Linux的调试是非常重要的，正常工作的网络能够大大节省调试的时间。
当然，串口也是非常重要的。
Step 4: Make
 the Kernel
内核需要修改，是因为Graphics
 SDK会调用内核的公共函数地址入口，有一些函数是TI提供的内核里面存在，而mainline的内核里面没有的。看起来貌似要修改的地方很多，其实关键只有两个内容：reset_controller和register_vsync_cb。
A. 修改memuconfig中有关reset的内容：
使用make
 menuconfig命令，使能RESET_CONTROLLER：
CONFIG_RESET_CONTROLLER=y
这一步是必须的，因为PVR服务的pvrsrvkm模块用到了很多reset_control_*函数。
B. 为drivers/reset/core.c文件添加如下内容：
int reset_control_is_reset(struct reset_control *rstc)
{
if (rstc->rcdev->ops->is_reset)
return rstc->rcdev->ops->is_reset(rstc->rcdev, rstc->id);
return -ENOSYS;
}
EXPORT_SYMBOL_GPL(reset_control_is_reset);
int reset_control_clear_reset(struct reset_control *rstc)
{
if (rstc->rcdev->ops->clear_reset)
return rstc->rcdev->ops->clear_reset(rstc->rcdev, rstc->id);
return -ENOSYS;
}
EXPORT_SYMBOL_GPL(reset_control_clear_reset);
这两个函数会给Graphics SDK调用，因此需要定义它们。重新编译之后，函数名会内核源代码根目录下的System.map文件中出现。
C. 为include/linux/reset-controller.h文件添加如下内容：
struct reset_control_ops {
int (*reset)(struct reset_controller_dev *rcdev, unsigned long id);
int (*assert)(struct reset_controller_dev *rcdev, unsigned long id);
int (*deassert)(struct reset_controller_dev *rcdev, unsigned long id);
int (*is_reset)(struct reset_controller_dev *rcdev, unsigned long id);
int (*clear_reset)(struct reset_controller_dev *rcdev, unsigned long id);
};
相应的为 reset_control_ops结构体增加成员变量。
D. 为include/linux/reset.h文件添加如下内容：
int reset_control_reset(struct reset_control *rstc);
int reset_control_assert(struct reset_control *rstc);
int reset_control_deassert(struct reset_control *rstc);
int reset_control_is_reset(struct reset_control *rstc);
int reset_control_clear_reset(struct reset_control *rstc);
相应的为头文件做声明。
E. 为arch/arm/boot/dts/am33xx.dtsi文件添加如下内容：
prcm: prcm@44e00000 {
compatible = "ti,am3-prcm";
reg = <0x44e00000 0x4000>;
#reset-cells = <1>;
prcm_clocks: clocks {
#address-cells = <1>;
#size-cells = <0>;
};
prcm_clockdomains: clockdomains {
};
};
sgx@0x56000000 {
compatible = "ti,sgx";
ti,hwmods = "gfx";
reg = <0x56000000 0x1000000>;
interrupts = <37>;
resets = <&prcm 0>;
};
这是devicetree的内容。
如果看不懂dts文件的格式，可以阅读内核文档Documentation/devicetree/booting-without-of.txt 。
F. 添加ti_reset.c文件：
drivers/reset/core.c文件声明了一个结构体struct
 reset_control，它的定义则由函数of_reset_control_get返回，并且在返回之前给其成员变量struct
 reset_controller_dev *rcdev赋值。
of_reset_control_get函数被reset_control_get函数调用；reset_control_get函数被SGX的PVRSRVDriverProbe函数调用。
在继续调试之前，先来理解list_for_each：
/**
* list_for_each - iterate over a list
* @pos: the &struct list_head to use as a loop cursor.
* @head: the head for your list.
*/
#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)                      \
pos = list_next_entry(pos, member))
pos是循环变量，相当于for循环里面的i；head是链表list的表头；list_next_entry展开得到：
container_of((pos)->member.next, typeof(*(pos)), member);
container_of的原型是container_of(ptr,
 type, member) ，它是一个非常常见的宏定义，含义是，返回type类型的地址，type类型包含了member类型，而ptr是实际的member类型指针。所以list_next_entry的含义是，返回一个typeof(*(pos))的地址，它包含有member这个类型，且(pos)->member.next指向这个member。
再来理解 list_for_each_entry：
/**
* list_for_each_entry - iterate over list of given type
* @pos: the type * to use as a loop cursor.
* @head: the head for your list.
* @member: the name of the list_struct within the struct.
*/
#define list_for_each_entry(pos, head, member) \
for (pos = list_first_entry(head, typeof(*pos), member); \
&pos->member != (head); \
pos = list_next_entry(pos, member))
pos是循环变量，相当于for循环里面的i；head是链表list的表头；member是被包含的类型定义；
首先，pos初始化为typeof(*pos)类型的指针，它包含有member类型，且member的地址为head->next；
然后，如果没有循环返回到head，则pos会指向包含有(pos)->member.next的pos类型地址。
所以，of_reset_control_get函数中的list_for_each_entry(r,
 &reset_controller_list, list)含义是：
r相当与for循环里面的i，指向下一个被操作的struct
 reset_controller_dev；reset_controller_list是链表的表头；
首先，r初始化为struct
 reset_controller_dev类型的指针，它包含有list类型，且list的地址为(&reset_controller_list)->next；
但是已知 reset_controller_list在此文件头被初始化了，它是一个空链表：
static LIST_HEAD(reset_controller_list);
因此为了让list_for_each_entry得到有效的执行，即得到可用的rcdev，需要执行reset_controller_register函数。
So，将TI公司提供的内核中的ti_reset.c拷贝到driver/reset目录下，它包含有关键的注册操作，即reset_controller_register函数。
另外，不要忘记修改Makefile和Kconfig。
另外，不要忘记把ti_reset.c中的.compatible值改为和am33xx.dtsi文件中相同的"ti,am3-prcm"。
G. 为drivers/video/fbdev/da8xx-fb.c文件添加如下内容：
static vsync_callback_t vsync_cb_handler;
static void *vsync_cb_arg;
int register_vsync_cb(vsync_callback_t handler, void *arg, int idx)
{
if ((vsync_cb_handler == NULL) && (vsync_cb_arg == NULL)) {
vsync_cb_arg = arg;
vsync_cb_handler = handler;
} else {
return -EEXIST;
}
return 0;
}
EXPORT_SYMBOL(register_vsync_cb);
int unregister_vsync_cb(vsync_callback_t handler, void *arg, int idx)
{
if ((vsync_cb_handler == handler) && (vsync_cb_arg == arg)) {
vsync_cb_handler = NULL;
vsync_cb_arg = NULL;
} else {
return -ENXIO;
}
return 0;
}
EXPORT_SYMBOL(unregister_vsync_cb);
H. 为include/video/da8xx-fb.h文件添加如下内容：
typedef void (*vsync_callback_t)(void *arg);
int register_vsync_cb(vsync_callback_t handler, void *arg, int idx);
int unregister_vsync_cb(vsync_callback_t handler, void *arg, int idx);
是不是觉得内核的修改很复杂呢？其实它的主要功能，就是添加了一个reset_controller的platform设备，并且提供reset和vsync_cb函数供Graphics
 SDK调用。编译完内核，就可以进行文件系统的制作啦（^_^）
Step 5: Prepare
 the Filesystem
A. 新建目标文件系统：
[maria@localhost qt]$ mkdir /home/maria/qt/rootfs -p
它相当于目标文件系统的根目录。
B. 编译busybox，将输出拷贝到目标文件系统：
make ARCH=arm CROSS_COMPILE=/opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf- menuconfig
make ARCH=arm CROSS_COMPILE=/opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf- all
make ARCH=arm CROSS_COMPILE=/opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf- install
使用busybox默认配置即可，不用修改它。
之前在《为AM335x移植Linux内核主线代码》系列里，制作busybox的时候使用了静态编译，动态编译无法运行，这是因为动态编译的_install/bin/busybox找不到装载器。
[maria@localhost bin]$ /opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf-ldd --root ../../../rootfs.save busybox
libm.so.6 => /lib/libm.so.6 (0xdeadbeef)
ld-linux-armhf.so.3 => /lib/ld-linux-armhf.so.3 (0xdeadbeef)
libc.so.6 => /lib/libc.so.6 (0xdeadbeef)
解决的方法很简单，就是拷贝正确的装载器和库文件，放置在目标文件系统的正确位置即可，见接下来的步骤。
编译完成之后，将_install目录下的所有内容，拷贝到A步骤创建的rootfs下：
bin linuxrc sbin usr
C. 创建lib目录，拷贝库文件：
在目标文件系统的根目录下，创建lib目录；
一般这些库文件在交叉编译器安装目录下的libc里面，除了busybox需要的三个库文件之外，还有很多其他的库文件；
将这些库文件拷贝到lib目录中；
D. 创建etc目录，编辑需要的内容：
创建rc0.d、rc1.d、rc2.d、rc3.d、rc4.d、rc5.d、rc6.d、rcS.d八个目录；
将SDK开发包提供的文件系统中的etc/group、etc/passwd、etc/shadow三个文件拷贝过来；
将SDK开发包提供的文件系统中的etc/default/rcS文件拷贝过来；
将SDC开发包提供的文件系统中的etc/inittab文件拷贝过来；
将SDC开发包提供的文件系统中的etc/fstab文件拷贝过来；
将SDC开发包提供的文件系统中的etc/init.d/rc、/etc/init.d/rcS文件拷贝过来；
E. 创建dev、home、home/root、media、mnt、opt、proc、sys、var目录：
无需拷贝dev文件，因为内核会生成它们。
将目标文件系统拷贝到SD卡的rootfs分区，然后将SD插入目标板，上电运行，串口终端会出现启动信息，最终出现登陆提示符。这说明，Linux的runlevel
 3已经可以正确运行了。
NOTE：将飞凌提供init.sysvinit拷贝到sbin目录下。
NOTE：将飞凌提供的ethtool拷贝到sbin目录下。
NOTE：将Graphics
 SKD编译出来的devmem2拷贝到sbin目录下。
NOTE：记得在lib下创建modules/3.18.4目录；
NOTE：这里要在rcS文件的倒数第二个非空行添加：
echo "mount -o remount,rw /dev/root"
mount -o remount,rw /dev/root
这样启动的时候就不用手动执行mount命令了。
Step 6: Make
 the SDK
从TI官网下载最新版的Graphics
 SDK：
[http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/gfxsdk/latest/index_FDS.html](http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/gfxsdk/latest/index_FDS.html)
编译Graphics
 SDK之前，需要编译一次内核，因此Graphics
 SDK需要调用内核符号表，最好是确保它一直是最新的。
Graphics SDK不会对内核做任何修改，它只会更改目标文件系统。
如果Graphics
 SDK编译成功，应该能够生成omaplfb.ko、pvrsrvkm.ko、bufferclass_ti.ko三个模块文件，并且它们能够被内核正确加载。omaplfs负责和framebuffer的接口，pvrsrvkm负责和用户层服务的接口， bufferclass_ti负责使用proprietary
 extension，它允许streaming
 playback through SGX。除了模块文件外，它还会生成OpenGL
 ES11/20/VG的二进制可执行文件，供用户的程序调用。
[maria@localhost graphics]$ ./Graphics_SDK_setuplinux_hardfp_5_01_01_02.bin –help
[maria@localhost graphics]$ ./Graphics_SDK_setuplinux_hardfp_5_01_01_02.bin –prefix \
/home/maria/qt/graphics/Graphics_SDK_5_01_01_02
（好像Fedora21只能使用--mode
 console，--mode
 standard没有效果，而且使用—es8.x参数会出现error。）
为源代码根目录下的vim
 Rules.make文件，添加这四个变量（换成自己的实际路径）：
SDK_INSTALL_DIR=/home/maria/qt
LINUX_DEVKIT_PATH=/opt/i686-arago-linux/usr
LINUXKERNEL_INSTALL_DIR=$(HOME)/linux-3.18.4-v3
DESTDIR=$(HOME)/rootfs
另外，如果你使用的是Graphics_SDK_5_01_01_01，还需要将GFX_Linux_KM/services4/3rdparty/dc_ti335x_linux/Kbuild文件的EXTRA_CFLAGS修改过来，因为3.18.4的内核将omap2目录放置在了driver/video/fbdev目录下，而不是driver/video目录：
EXTRA_CFLAGS = -DLINUX \
-DCONFIG_OMAP2_DSS \
-I$(PVR_BUILD_DIR)/include4 \
-I$(PVR_BUILD_DIR)/services4/include \
-I$(PVR_BUILD_DIR)/services4/system/$(PVR_SYSTEM) \
-I$(KERNELDIR)/drivers/video/omap2 \
-I$(KERNELDIR)/drivers/video/fbdev/omap2 \
-I$(PVR_BUILD_DIR)/services4/system/include \
$(SYS_CFLAGS.1) \
然后在源代码根目录下执行make命令（注意ARCH必须以环境变量的形式出现）：
[maria@localhost Graphics_SDK_5_01_01_02]$ export ARCH=arm
[maria@localhost Graphics_SDK_5_01_01_02]$ make help
[maria@localhost Graphics_SDK_5_01_01_02]$ make CROSS_COMPILE=/opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf-BUILD=debug
 OMAPES=8.x FBDEV=yes SUPPORT_XORG=0 all
[maria@localhost Graphics_SDK_5_01_01_02]$ make CROSS_COMPILE=/opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf- BUILD=debug OMAPES=8.x FBDEV=yes
 SUPPORT_XORG=0 install
执行完毕之后，会发现目标文件系统的opt下增加了两个目录（etc目录下也有增加的内容，暂时可不考虑它们）。
将gfxlibraries、gfxsdkdemos这两个目录拷贝到目标文件系统下，将文件系统放置进SD卡，上电启动：
~ # cd /opt/gfxsdkdemos/
/opt/gfxsdkdemos # ./335x-demo
/* 很多的打印信息 */
PVR: High Water Mark = 0 bytes
Loaded PowerVR consumer services.
/opt/gfxsdkdemos # lsmod
omaplfb 12320 0 - Live 0xbf08a000 (O)
pvrsrvkm 470425 1 omaplfb, Live 0xbf000000 (O)
如果lsmod命令执行的结果如上所示，说明模块被正确的加载，Graphics
 SDK的驱动部分就制作成功啦。
由上面的调试过程也可以知道，遇到问题的时候不要害怕找不到解决方法，因为阅读README和源代码总是能有非常大的帮助，而且由于Linux本身代码的健壮性，摸索它的脉络是相对容易的，它不会像糟糕的代码那样给人一团乱麻的感觉。
Step 7: Use the
 SDK
首先要修改bootargs：
修改bootargs有很多种方法，比如在编译内核的menuconfig时修改其Boot
 options，或者修改U-Boot里面的config文件，或者在U-Boot运行时指定，等等。由于U-Boot在autoboot的时候会读取boot分区下的uEnv.txt文件，因此将bootargs添加在这个文件中：
bootargs=console=ttyO0,115200n8 root=/dev/mmcblk0p2 rootwait init=/sbin/init.sysvinit mem=1024M vram=50M
bootcmd=mmc rescan; fatload mmc 0 0x82000000 uImage; \
fatload mmc 0 0x83000000 maria-am335x.dtb; bootm 0x82000000 - 0x83000000
uenvcmd=boot
这个uEnv.txt的内容是，设置bootargs，并且设置U-Boot自启动时从SD卡读取dts和uImage。
U-Boot是非常轻量且灵活的，它给人很多意想不到的惊喜。
运行gfxsdkdemos目录下的示例：
~ # cd /opt/gfxsdkdemos/ogles2/
/opt/gfxsdkdemos/ogles2 # ./OGLES2ChameleonMan
/opt/gfxsdkdemos/ogles2 # ./OGLES2MagicLantern
/opt/gfxsdkdemos/ogles2 # cd /opt/gfxsdkdemos/ogles/
/opt/gfxsdkdemos/ogles # ./OGLESEvilSkull
/opt/gfxsdkdemos/ogles # ./OGLESFilmTV
运行OGLES2ChameleonMan这个程序的时候，有没有觉得画面上的这个人跑得很快，图形也没有命令行界面下的拖影呢（好吧，可能只是俺的心理作用～）。
Step 8: Build Qt
[http://processors.wiki.ti.com/index.php/Building_Qt](http://processors.wiki.ti.com/index.php/Building_Qt)
[http://processors.wiki.ti.com/index.php/Building_Qt_with_OpenGL_ES_accelerated_by_SGX](http://processors.wiki.ti.com/index.php/Building_Qt_with_OpenGL_ES_accelerated_by_SGX)
（NOTE：触摸屏tslib的调试暂时不考虑。）
到目前为止的目标文件系统，具备了那些内容呢：
有etc目录和其中的启动文件，有存放在bin、sbin目录下的busybox可执行程序，lib库，其他目录如home、media、mnt、proc等，以及存放在opt目录下的Graphics
 SDK，包括它的库文件、驱动文件和demo文件。只有demo文件无错误的运行在目标板上了，才可以往下进行，否则，要继续修改Kernel代码或者Graphics
 SDK编译方式。
准备好文件系统，以及解压Qt源代码：
[maria@localhost qt-am335x]$ mkdir roofts.withSDK
[maria@localhost qt-am335x]$ cp ../ro otfs/* rootfs.withSDK/ -rv
[maria@localhost qt-am335x]$ tar xvf ../qt-everywhere-opensource-src-5.4.1.tar.gz -C .
从下面这个地址下载qmake.conf文件，并将它复制进Qt源代码目录，并根据主机的实际情况修改内容：
[http://processors.wiki.ti.com/images/7/76/Linux-TIarmv7-sgx-g%2B%2B.tar.gz](http://processors.wiki.ti.com/images/7/76/Linux-TIarmv7-sgx-g%2B%2B.tar.gz)
[maria@localhost qt-am335x]$ tar xvf Linux-TIarmv7-sgx-g++.tar.gz
[maria@localhost qt-am335x]$ ln -s qt-everywhere-opensource-src-5.4.1 qt-everywhere
[maria@localhost qt-am335x]$ cp linux-TIarmv7-sgx-g++ qt-everywhere/qtbase/mkspecs/device/ -r
[maria@localhost qt-am335x]$ vim qt-everywhere/qtbase/mkspecs/device/linux-TIarmv7-sgx-g++/qmake.conf
MAKEFILE_GENERATOR = UNIX
CONFIG += incremental
QMAKE_INCREMENTAL_STYLE = sublib
include(../../common/linux.conf)
include(../../common/gcc-base-unix.conf)
include(../../common/g++-unix.conf)
load(device_config)
QT_QPA_DEFAULT_PLATFORM = eglfs
QT_INSTALL_DIR = /home/maria/qt/qt-am335x/qt-everywhere/qtbase
SGX_SDK_ROOT = /home/maria/qt/graphics/Graphics_SDK_5_01_01_02
COMPILER_FLAGS = -march=armv7-a -mtune=cortex-a8 -mfpu=vfpv3 -mfloat-abi=hard
QMAKE_CFLAGS_RELEASE = -O3 -march=armv7-a -mtune=cortex-a8 -mfpu=vfpv3 -mfloat-abi=hard
QMAKE_CXXFLAGS_RELEASE = -O3 -march=armv7-a -mtune=cortex-a8 -mfpu=vfpv3 -mfloat-abi=hard
QMAKE_CC = arm-linux-gnueabihf-gcc
QMAKE_CXX = arm-linux-gnueabihf-g++
QMAKE_LINK = arm-linux-gnueabihf-g++
QMAKE_LINK_SHLIB = arm-linux-gnueabihf-g++
QMAKE_AR = arm-linux-gnueabihf-ar cqs
QMAKE_OBJCOPY = arm-linux-gnueabihf-objcopy
QMAKE_STRIP = arm-linux-gnueabihf-strip
QMAKE_NM = arm-linux-gnueabihf-nm -P
QMAKE_INCDIR_OPENGL_ES2 = $$SGX_SDK_ROOT/GFX_Linux_SDK/OGLES2/SDKPackage/Builds/OGLES2/Include/
QMAKE_INCDIR_OPENGL_ES2 += $$SGX_SDK_ROOT/include
QMAKE_INCDIR_OPENGL_ES2 += $$SGX_SDK_ROOT/GFX_Linux_SDK/OGLES/SDKPackage/Builds/OGLES/Include/
QMAKE_LIBDIR_OPENGL_ES2 = $$SGX_SDK_ROOT/gfx_dbg_es8.x/
QMAKE_LIBS_OPENGL_ES2 = -lEGL -lGLESv2 -lGLES_CM -lIMGegl -lsrv_um -lusc
QMAKE_INCDIR_OPENGL += $$SGX_SDK_ROOT/GFX_Linux_SDK/OGLES/SDKPackage/Builds/OGLES/Include/
QMAKE_LIBDIR_OPENGL = $$SGX_SDK_ROOT/gfx_dbg_es8.x
QMAKE_LIBDIR_OPENGL_QT = $$SGX_SDK_ROOT/gfx_dbg_es8.x
QMAKE_LIBS_OPENGL_ES1 = -lEGL -lGLES_CM -lIMGegl -lsrv_um -lusc
QMAKE_INCDIR_OPENVG = $$SGX_SDK_ROOT/GFX_Linux_SDK/OVG/SDKPackage/Builds/OVG/Include/
QMAKE_LIBDIR_OPENVG = $$SGX_SDK_ROOT/gfx_dbg_es8.x/
QMAKE_LIBS_OPENVG = -lEGL -lGLESv2 -lGLES_CM -lIMGegl -lsrv_um -lOpenVG -lOpenVGU
QMAKE_INCDIR_EGL = $$QMAKE_INCDIR_OPENGL_ES2
QMAKE_INCDIR_EGL += $$QT_INSTALL_DIR/src/3rdparty/powervr/wsegl2
QMAKE_INCDIR_POWERVR = $$QT_INSTALL_DIR/src/3rdparty/powervr/wsegl2
QMAKE_LIBDIR_EGL = $$QMAKE_LIBDIR_OPENGL_ES2
QMAKE_LIBS_EGL = -lEGL -lIMGegl -lsrv_um -lGLESv2 -lGLES_CM -lusc
QMAKE_INCDIR += $$QMAKE_INCDIR_OPENGL_ES2
QMAKE_LIBDIR += $$QMAKE_LIBDIR_OPENGL_ES2
QMAKE_LIBS = $$QMAKE_LIBS_OPENGL_ES2 -lts -lrt -lpthread
deviceSanityCheckCompiler()
load(qt_config)
NOTE：修改qmake.conf文件，要特别注意它的inluce目录、交叉编译器的设置，以及INCDIR和LIBDIR是否正确；
NOTE：对比Qt的mainline里面的qmake.conf，观察它新增的内容；
NOTE：$$QT_INSTALL_DIR/src/3rdparty/powervr/wsegl2这个目录并不存在，但并不影响编译结果；
另外还要将交叉编译器的bin目录加入环境变量PATH，拷贝头文件和lib（为了省事可以把lib的内容全都拷贝进目标文件系统，但是这里还是暂时采取需要什么就拷贝什么的方法，你甚至先什么都不做，等到编译报错的时候再去寻找对应的lib或者头文件，这样能够更加清晰的理解Qt的编译过程），再执行configure和make步骤：
[maria@localhost qt-everywhere]$ export PATH=$PATH:/opt/i686-arago-linux/usr/bin
[maria@localhost qt-everywhere]$ mkdir /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/include /home/maria/qt/qt-am335x/roofts.withSDK/usr/
 -r
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/crt* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libts* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/librt.*
 /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libpthread* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libm.* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libc.* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libc_nonshared.a* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libz.so* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libjpeg.so* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libpng* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libnsl.* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libgthread-2.0.so* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libglib-2.0.so* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libasound.so* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libdl.* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libfreetype.so* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/glib-2.0 /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
 -r
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libfontconfig.so* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libexpat.so* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libdbus* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libu* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
[maria@localhost qt-everywhere]$ cp /opt/ti-sdk-am335x-evm-07.00.00.00/linux-devkit/sysroots/cortexa8hf-vfp-neon-3.8-oe-linux-gnueabi/usr/lib/libglslcompiler* /home/maria/qt/qt-am335x/roofts.withSDK/usr/lib/
另外，还要将目标文件系统的include/freetypes2/freetype软链接成include/freetype：
[maria@localhost qt-everywhere]$ cd /home/maria/qt/qt-am335x/roofts.withSDK/usr/include
[maria@localhost include]$ ln -s freetype2/freetype freetype
(如果freetypes报错了就做此修改，没有报错的话就不用管了～)
[maria@localhost qt-everywhere]$ ./configure –help
[maria@localhost qt-everywhere]$ ./configure \
-debug -opensource -confirm-license -shared \
-prefix /home/maria/qt/qt-am335x/qt-everywhere/build \
-sysroot /home/maria/qt/qt-am335x/roofts.withSDK \
-platform linux-g++-64 \
-device linux-TIarmv7-sgx-g++ \
-device-option CROSS_COMPILE=/opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf- \
-D QT_NO_QWS_CURSOR -D QT_QWS_CLIENTBLIT \
-eglfs -opengl es2 -qreal float -v \
-nomake examples -nomake tests
NOTE：configure的编译选项，需要根据主机和目标板的实际情况慢慢摸索，有的编译选项随着新版本的发布不再支持，有的编译选项被添加在新版本中，有的编译错误即使存在也没有关系，有的编译错误则会影响结果。一般来说，提示“Just
 run 'gmake'”就算是成功。
NOTE：configure会生成很多.o文件，在重新configure之前可以编写脚本，来删除旧有的.o文件。
-prefix: 主机上Qt
 SDK安装的目录；
-sysroot: 目标文件系统的根目录；
-platform: 主机使用的mkspecs；
-device: 目标机使用的mkspecs，替代xplatform；
-nomake examples -nomake tests: 不编译示例和测试程序，否则得花很长的时间；
[maria@localhost qt-everywhere]$ gmake -j8
[maria@localhost qt-everywhere]$ gmake -j8 install
这里的gmake，是指GNU
 Make，在Fedora主机上就是make；
编译完成之后，会发现目标文件系统里面增加了build 目录：
[maria@localhost roofts.withSDK]$ find . -mmin 1
./home/maria/qt/qt-am335x/qt-everywhere/build/*
接下来编译示例程序（qmake为上步编译时生成，在/home/maria/qt/qt-am335x/qt-everywhere/build目录下）：
[maria@localhost qt-everywhere]$ cd qtbase/examples/
[maria@localhost examples]$ ../../build/bin/qmake examples.pro
[maria@localhost examples]$ gmake -j8
[maria@localhost examples]$ gmake -j8 install
编译完成之后，会发现目标文件系统里面增加了build/examples目录：
[maria@localhost roofts.withSDK]$ find . -mmin 1
./home/maria/qt/qt-am335x/qt-everywhere/build/examples/*
将./home/maria目录拷贝到SD的目标文件系统里；
将./usr/lib和./usr/include也拷贝到SD卡的目标文件系统里；
运行Graphics SDK的335x-demo；
此外，还需要执行下面的步骤（Qt程序会试图获取/etc/machine-id的值，如果运行示例程序时报错，可以主机上的machine-id文件拷贝过来，另外还要设置屏幕的分辨率值）：
~ # tftp 192.168.1.118 -g -l machine-id
~# cp machine-id /etc/
~ # export QT_QPA_EGLFS_PHYSICAL_HEIGHT=272
~ # export QT_QPA_EGLFS_PHYSICAL_WIDTH=480
然后执行Qt的示例程序（如果报找不到platform的错误信息，则执行第二条）：
~ # /home/maria/qt/qt-am335x/qt-everywhere/build/examples/widgets/widgets/digitalclock/digitalclock
~ # /home/maria/qt/qt-am335x/qt-everywhere/build/examples/widgets/widgets/digitalclock/digitalclock
 -platform eglfs
到这里，就可以看到液晶上显示的数字时钟啦～
运行Qt程序的时候，可能会发现终端会打印出很多错误，但是不要害怕这些错误，它们提示的信息往往非常关键，比如需要设置哪些环境变量，或者缺乏哪些库文件或者头文件。另外，arm-linux-gnueabihf-ldd和readelf是非常有用的工具，通过它们来观察.so或者可执行程序，往往能够发现很多问题的答案。
Step 9: Build
 Qt Programs
编写Qt程序，首先要具备C++基础知识；
A. 打开主机上的Qt
 Creator：
B. 创建一个新项目：
File -> New File or Project -> Application -> Qt Widgets Application
将名字设置为serial_test，目录设置为/home/maria/qt/qt_workspace/serial_test；
然后在manage里面，添加新的Kit：
为Qt Versions添加：/home/maria/qt/qt-am335x/qt-everywhere/build/bin/qmake
为Compilers添加：/opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf-gcc
为Debuggers添加：/opt/i686-arago-linux/usr/bin/arm-linux-gnueabihf-gdb
为Kits添加新的am335x-kit，并将其sysroot、Compiler、Debugger等选成实际的交叉编译类型。
选择Kits为刚刚创建的am335x-kit，然后一路点击next。
C. 将以前的项目mainwindow.ui替代新项目的mainwindow.ui：
D. 将以前的项目mainwindow.h替代新项目的mainwindow.h：
E. 将以前的项目mainwindow.cpp替代新项目的mainwindow.cpp：
（其实我也不想这样的偷懒，但是以前在主机上编写了一个简单的串口应用，正好可以拿来搞这个^_^）
F. 编译项目：
Build -> Build All
G. 将可执行文件拷贝到目标板运行：
生成的可执行文件，存放在serial_test/build-serial_test-am335x-Debug目录下，将它拷贝到目标板上：
~ # tftp 192.168.1.118 -g -l serial_test
~ # chmod +x serial_test
~ # ./serial_test
然后可以看见serial_test的程序界面出现在液晶上，编写基础的Qt应用真的是很简单，因为它的开发环境和跨平台特性都非常的完善^_^。
到这里，SGX+OpenGL+Qt5移植在AM335x+Linux上的基本步骤就算是完成了，虽然还有触摸屏tslib和字体的问题待解决，但还是不放在本文中啦，maybe以后调试的时候再补充进来。
Step 10: About
 Linux
后记：为什么要选择Linux做硬件开发？
如果Linux真的像很多人所认为的装x专用，毫无用户体验可言，它不会受到那么多人的喜爱。它所能提供的通透、自由和参与的感觉，是做技术的人不可抗拒的诱惑。它的结构极其健壮简洁，并没有很多内容来帮助用户，因此你需要花时间（对我来说是很长的时间）去学习它，但是随着学习的深入，你会发现一个自由的世界打开，你能用极其合理的开销实现非常强大的功能。
对于计算机来说，你不是用户，而是上帝。
使用Linux环境做硬件开发，也是同样的感受，随着开发的进行，你会发现你不仅知道怎么做可以实现硬件的功能，你还能知道为什么要这样做。Linux对它的使用者完全真诚，它只忠实于事物本身的逻辑，而不会为自身的利益为使用者做任何决定，所有的决定都是为了一个最优的最合理的最强壮的未来，它每一个透明的自由的部件，最终提供给了使用者unlimited
 possibility。
Free不是免费，free是自由，你，值得拥有。

