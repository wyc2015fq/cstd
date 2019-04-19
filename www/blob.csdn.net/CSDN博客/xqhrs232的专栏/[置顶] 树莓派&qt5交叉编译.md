# [置顶] 树莓派&qt5交叉编译 - xqhrs232的专栏 - CSDN博客
2017年03月19日 13:23:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1475
原文地址::[http://blog.csdn.net/scylhy/article/details/52818779](http://blog.csdn.net/scylhy/article/details/52818779)
相关文章
1、玩转树莓派－ RaspBerry，Qt5交叉编译移植----[https://my.oschina.net/u/2306127/blog/389263](https://my.oschina.net/u/2306127/blog/389263)
2、[QT树莓派交叉编译环开发环境搭建](http://www.jerryzone.cn/qt-embeded-raspberrypi/)----[http://www.jerryzone.cn/qt-embeded-raspberrypi/](http://www.jerryzone.cn/qt-embeded-raspberrypi/)
## Rasberrypi&Qt cross compilation
```
本以为树莓派这么受欢迎，大家应该把开发树莓派中遇到的问题解决的差不多。但遇到一个有一个问题，解决一个有一个，心累啊，最终还是把环境搭建好了。ps:加入到某些树莓派qq群，本以为有大神能够帮忙，结果，进去后刚问问题，所谓的大神就说 新手！！ 怎么启动的程序……，等我把问题说清楚，全都哑巴了！
```
树莓派安装qt5&qtcreator–>pc交叉编译对应版本的qt源码–>配置pc端的qtcreator–>[测试](http://lib.csdn.net/base/softwaretest)
- 
准备
- 树莓派2B+
- 系统是 2016-09-23-raspbian-jessie.img,该版本支持apt-get方式安装qt,目前是5.3.2版本。
- [低版本升级](https://www.linuxdashen.com/debian-7-wheezy%E5%8D%87%E7%BA%A7%E5%88%B0debian-8-jessie%E7%9A%84%E5%85%B7%E4%BD%93%E6%AD%A5%E9%AA%A4)或者重新烧制系统（格式化后重新分区，彻底清理干净sd卡，否则会发生意想不到的结果）
- 
概览
- 树莓派 
1、系统烧制、驱动安装 
2、树莓派安装qt-default（qt5.3.2） 和 qtcreator
- PC(ubuntu16.04) 
1、[QT官方教程：交叉编译qt5.3.2要和树莓派版本对应](https://wiki.qt.io/RaspberryPi_Beginners_Guide)
2、qtcreator arm工具及配置，及远程部署 
3、 忽略这一步—————最终，我们将挂载的镜像烧到sd卡，启动系统后，就可以直接使用qt5了。所以编译的qt5的库就在挂载的镜像中，我们只需把它拷到树莓派上，再设置环境变量，就应该能用了。qt5库位置：/mnt/rasp-pi-rootfs/usr/local/qt5pi
- 问题汇总 
1、[make阶段的符号链接问题 undefined reference to `__dlopen’等问题的解决方案](http://stackoverflow.com/questions/13626726/an-error-building-qt-libraries-for-the-raspberry-pi)
2、文件不匹配的问题，重新获取源码github或者qt官网 
3、如果哪天qtcreator编译树莓派程序时，发生丢失文件的错误。请先确认是否未挂载树莓派
```
sudo mkdir /mnt/rasp-pi-rootfs 
sudo mount -o loop,offset=62914560 2015-05-05-raspbian-wheezy.img /mnt/rasp-pi-rootfs
```
4、启动树莓派后，不能正常驱动键盘、3.5LCD显示器。格式化SD卡重新分区，烧制系统。
- 树莓派上安装qt5 
- 要求安装的系统是jessie
- 安装qt&qtcreator 
```
sudo apt-get install qt-default//目前是qt5.3.2 
sudo apt-get install qtcreator//安装完这一步，qt程序的界面才启动起来
```
- 
交叉编译教程 
[QT官方教程：交叉编译](https://wiki.qt.io/RaspberryPi_Beginners_Guide)
[官方教程的翻译](http://blog.diveinedu.com/%E6%A0%91%E8%8E%93%E6%B4%BE%E4%B8%8Aqt5%E4%BA%A4%E5%8F%89%E7%BC%96%E8%AF%91%E7%A7%BB%E6%A4%8D%E6%96%B0%E6%89%8B%E6%8C%87%E5%8D%97/)
简单说一下流程
- 准备 
- [rasberrypi jessie镜像](https://www.raspberrypi.org/downloads/)
- qt交叉编译工具链（按照官方教程做就可以）
- [qt5.3.2源码 on qt official site](https://download.qt.io/archive/qt/5.3/5.3.2/single)
[qt5.3.2源码on github](https://github.com/GiterLab/qt-everywhere-opensource-src-5.3.2)
- 32位运行库（如果你的系统是64位的）
- 
编译
- 
挂载树莓派（百度jessie挂载即可//交叉编译时会使用树莓派的库和工具,以后qtcreator程序时，也会用到系统中的文件）
```
sudo mkdir /mnt/rasp-pi-rootfs
sudo mount -o loop,offset=62914560 2015-05-05-raspbian-wheezy.img /mnt/rasp-pi-rootfs
```
- 1
- 2
- 1
- 2
- 
矫正系统链接和库路径（是用的是挂载的树莓派文件；文件位置~/opt，如果更改，请对应）
```
cd ~/opt/cross-compile-tools
sudo ./fixQualifiedLibraryPaths /mnt/rasp-pi-rootfs/ ~/opt/gcc-4.7-linaro-rpi-gnueabihf/bin/arm-linux-gnueabihf-gcc
```
- 1
- 2
- 1
- 2
- 
配置make
```
cd ~/opt/cross-compile-tools
sudo ./fixQualifiedLibraryPaths /mnt/rasp-pi-rootfs/ ~/opt/gcc-4.7-linaro-rpi-gnueabihf/bin/arm-linux-gnueabihf-gcc
```
- 1
- 2
- 1
- 2
- 
make&install
```
make -j 4  //这步会发生符号链接问题，见下
sudo make install  //安装到挂载的树莓派系统中。
```
- 1
- 2
- 1
- 2
- 烧制镜像到SD卡（现在不使用该步骤，直接在树莓派上以apt-get方式安装qt-default和qtcreator） 
最终，我们将挂载的镜像烧到sd卡，启动系统后，就可以直接使用qt5了。所以编译的qt5的库就在挂载的镜像中，我们只需把它拷到树莓派上，再设置环境变量，就应该能用了。qt5库位置：/mnt/rasp-pi-rootfs/usr/local/qt5pi
- 
注意事项
- 交叉编译前最好不要使用过QT，或者重启机器后交叉编译，不要启动qtcreator等
- 如果不按照官方教程的文件结构，请将文件结构对应准确
- 关于安装32库 
```
sudo apt-get install ia32-libs
 //如果无法定位ia32-libs尝试安装下列内容，或者安装时提示的软件 lib32z1 lib32ncurses5 lib32bz2-1.0
```
- 1
- 2
- 1
- 2
- 
PC上的其他配置
- [qtcreator 配置,所需要工具均在编译源码的目录下](http://wiki.lemaker.org/Compiling_Qt5.5_for_LeMaker_Guitar/zh-hans#.E5.87.86.E5.A4.87)
- [远程部署](http://blog.csdn.net/u011911086/article/details/10812111)
- 
遇到的问题及解决方案
- 
[make阶段的符号链接问题](http://stackoverflow.com/questions/13626726/an-error-building-qt-libraries-for-the-raspberry-pi)
按照官方教程正确操作后，出现undefined reference to `__dlopen’等问题，显示内容如下
```
qlibrary_unix.cpp:(.text+0x1330): warning: Using 'dlopen' in statically linked applications requires at runtime the shared libraries from the glibc version used for linking 
/mnt/rasp-pi-rootfs/usr/lib/arm-[Linux](http://lib.csdn.net/base/linux)-gnueabihf/libdl.a(dlopen.o): In function
 `dlopen': 
(.text+0xc): undefined reference to `__dlopen' 
/mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf/libdl.a(dlclose.o): In function `dlclose': 
(.text+0x0): undefined reference to `__dlclose' 
/mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf/libdl.a(dlsym.o): In function `dlsym': 
(.text+0xc): undefined reference to `__dlsym' 
/mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf/libdl.a(dlerror.o): In function `dlerror': 
(.text+0x0): undefined reference to `__dlerror' 
/mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf/libm.a(feholdexcpt.o): In function `feholdexcept': 
(.text+0x48): undefined reference to `_dl_hwcap' 
/mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf/libm.a(fesetenv.o): In function `fesetenv': 
(.text+0x64): undefined reference to `_dl_hwcap' 
collect2: error: ld returned 1 exit status 
Makefile:1215: recipe for target '../../lib/libQt5Core.so.5.5.1' failed 
make[2]: *** [../../lib/libQt5Core.so.5.5.1] Error 1 
make[2]: Leaving directory '/home/liu/bin/opt/qt5/qtbase/src/corelib' 
Makefile:170: recipe for target 'sub-corelib-make_first' failed 
make[1]: *** [sub-corelib-make_first] Error 2 
make[1]: Leaving directory '/home/liu/bin/opt/qt5/qtbase/src' 
Makefile:45: recipe for target 'sub-src-make_first' failed 
make: *** [sub-src-make_first] Error 2
```
解决方案
软连接不对，重建两个软连接，均在挂载的树莓派镜像上。如果已经存在该软链接，先删除再创建。
```
//最终，我们将挂载的镜像烧到sd卡，启动系统后，就可以直接使用qt5了，所以编译的qt5的库就在挂载的镜像中，我们只需把它拷到树莓派上，再设置环境变量，就应该能用了。
sudo ln -s /mnt/rasp-pi-rootfs/lib/arm-linux-gnueabihf/libdl-2.13.so /mnt/rasp-pi-rootfs/lib/arm-linux-gnueabihf/libdl.so
sudo ln -s  /mnt/rasp-pi-rootfs/lib/arm-linux-gnueabihf/libm-2.13.so  /mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf/libm.so
```
- 1
- 2
- 3
- 1
- 2
- 3
```
出现上述问题的原因
```
in that case the fixQualifiedLibraryPaths won’t help you as it can’t find the symlinks. Copying libdl.so and libm.so might also fail, for example, if you use a flash drive to copy data from your existing Raspberry Pi, it won’t
 copy them as symlinks, but will copy the libraries themselves. However, for the build to succeed, it seems to require symlinks.
- 
没有找到文件问题
在make 时发生如下问题
```
/vc/lib -L/home/liu/bin/opt/qt5/qtlocation/lib -lQt5Location -L/home/liu/bin/opt/qt5/qtbase/lib -L/home/liu/bin/opt/qt5/qtdeclarative/lib -lQt5Quick -lQt5Gui -lQt5Qml -lQt5Network -lQt5Positioning -lQt5Core -lGLESv2 -lpthread -lpoly2tri 
/home/liu/opt/gcc-4.7-linaro-rpi-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/4.7.2/../../../../arm-linux-gnueabihf/bin/ld: cannot find -lpoly2tri
collect2: error: ld returned 1 exit status
Makefile:281: recipe for target '../../../qml/QtLocation/libdeclarative_location.so' failed
make[4]: *** [../../../qml/QtLocation/libdeclarative_location.so] Error 1
make[4]: Leaving directory '/home/liu/bin/opt/qt5/qtlocation/src/imports/location'
Makefile:80: recipe for target 'sub-location-install_subtargets' failed
make[3]: *** [sub-location-install_subtargets] Error 2
make[3]: Leaving directory '/home/liu/bin/opt/qt5/qtlocation/src/imports'
Makefile:135: recipe for target 'sub-imports-install_subtargets' failed
make[2]: *** [sub-imports-install_subtargets] Error 2
make[2]: Leaving directory '/home/liu/bin/opt/qt5/qtlocation/src'
Makefile:56: recipe for target 'sub-src-install_subtargets' failed
make[1]: *** [sub-src-install_subtargets] Error 2
make[1]: Leaving directory '/home/liu/bin/opt/qt5/qtlocation'
Makefile:366: recipe for target 'module-qtlocation-install_subtargets' failed
make: *** [module-qtlocation-install_subtargets] Error 2
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
解决办法其原因可能是，make配置文件和源文件不匹配造成的，请重新获取源码，重新编译。
- 
启动不出来qt界面问题
问题描述：在终端启动程序后，测试字符都已经显示，但图形渲染没有出现。因为这个为题让我，真正工作了三天！！
解决方案
树莓派上安装qt-default其版本是5.3.2，交叉编译时请编译对应版本。安装qtcreator,再启动测试demo时图形界面就渲染出来了。
- 
树莓派上启动qt程序发现不了设备等，百度即可
#### 远程部署测试结果
## ![测试demo](https://img-blog.csdn.net/20161014202116842)
引用链接
- [debian7 wheezy升级到debian8 jessie](https://www.linuxdashen.com/debian-7-wheezy%E5%8D%87%E7%BA%A7%E5%88%B0debian-8-jessie%E7%9A%84%E5%85%B7%E4%BD%93%E6%AD%A5%E9%AA%A4)
- [rasberrypi jessie镜像](https://www.raspberrypi.org/downloads/)
- [安装qt5](http://m.blog.csdn.net/article/details?id=50850702)
- [树莓派安装QT5](http://blog.csdn.net/lbsljn/article/details/51789892)
- [qt5.3.2源码 on qt official site](https://download.qt.io/archive/qt/5.3/5.3.2/single)
- [qt5.3.2源码on github](https://github.com/GiterLab/qt-everywhere-opensource-src-5.3.2)
- [QT官方教程：交叉编译](https://wiki.qt.io/RaspberryPi_Beginners_Guide)
- [官方教程的翻译](http://blog.diveinedu.com/%E6%A0%91%E8%8E%93%E6%B4%BE%E4%B8%8Aqt5%E4%BA%A4%E5%8F%89%E7%BC%96%E8%AF%91%E7%A7%BB%E6%A4%8D%E6%96%B0%E6%89%8B%E6%8C%87%E5%8D%97/)
- [undefined reference to `__dlopen’等问题的解决方案](http://stackoverflow.com/questions/13626726/an-error-building-qt-libraries-for-the-raspberry-pi)
- [qtcreator配置](http://wiki.lemaker.org/Compiling_Qt5.5_for_LeMaker_Guitar/zh-hans#.E5.87.86.E5.A4.87)
- [Issues with Qt5 apps](https://www.raspberrypi.org/forums/viewtopic.php?t=86467&p=610078)
- [移植qt之更改分辨率](http://www.voidcn.com/blog/duinodu/article/p-6221089.html)
- [配置无线驱动](http://blog.csdn.net/c80486/article/details/8545307)
- [fdisk分区](http://blog.csdn.net/openn/article/details/9856451)
- [树莓派2安装使用小米WIfi（360 小度 腾讯wifi）](http://www.cnblogs.com/sjqlwy/p/4415935.html)
- [微雪3.5inch LCD安装教程含驱动](http://www.waveshare.net/wiki/3.5inch_RPi_LCD_%28A%29)
- [如何使用手机作为树莓派的“屏幕”教程](http://tieba.baidu.com/p/4064959646)
- [scp传输文件](http://www.cnblogs.com/jiangyao/archive/2011/01/26/1945570.html)
- [软件定义硬件，使用js开发硬件程序](https://ruff.io/zh-cn/)
