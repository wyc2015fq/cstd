# Android在TQ2440开发板上的移植(转) - h13 - 博客园
一、目前进展
1．Android已经可以通过NFS在TQ2440开发板上运行
最近自己编译了android的文件系统，并在TQ上运行了，比熊猫版的bin程序多些，基本功能已经实现，大家可以从：
[http://d.namipan.com/d/b64ce43b2388e5e1f66af18149824b98cd13b926c4921f02](http://d.namipan.com/d/b64ce43b2388e5e1f66af18149824b98cd13b926c4921f02)
下载文件系统试试，改天整理一下，发个详细的移植过程和注意事项。
网络设置：
route add default gw 192.168.1.1 dev eth0
setprop net.dns1 202.120.223.5
1.使用ADB
Ubuntu：
export ADBHOST=192.168.1.101
adb kill-server
adb start-server
adb shell
2.启动时battery导致关闭
修改：frameworks/base/services/jni/com_android_server_BatteryService.cpp,补丁见附件。
在init.rc文件中添加
setprop battery.fake_ac 1 
二、目前缺陷
1．触摸屏没有校准
2．Android中时钟停止，结束zygote进程，系统重启，时钟更新，但再次停止不动
三、移植步骤
1、移植Android内核到TQ2440开发板上
1、从[http://code.google.com/p/android/downloads/list](http://code.google.com/p/android/downloads/list)上下载linux-2.6.25-android-1.0_r1.tar.gz
2、将内核解压到用户目录，如/root/kernel.git
3、进入kernel.git文件夹，如cd ~/kernel.git/
4、修改arch/arm/plat-s3c24xx/common-smdk.c文件中的nand flash分区设置，如gedit arch/arm/plat-s3c24xx/common-smdk.c
修改static struct mtd_partition smdk_default_nand_part[]结构体为：
static struct mtd_partition smdk_default_nand_part[] = {
        [0] = {
                .name        = "TQ2440_uboot",
                .size        = 0x00040000,
                .offset        = 0x00000000,
        },
        [1] = {
                .name        = "TQ2440_kernel",
                .offset        = 0x0004C000,
                .size        = 0x00200000,
        },
        [2] = {
                .name        = "TQ2440_yaffs2",
                .offset        = 0x0024C000,
                .size        = 0x03DB0000,
        }
};
5、在[http://www.codesourcery.com/gnu_toolchains/arm/portal/release644](http://www.codesourcery.com/gnu_toolchains/arm/portal/release644)网站上下载IA32 GNU/Linux TAR。
6、将文件解压到用户目录，如/usr/local/arm/arm-2008q3
7、进入arm-2008q3/bin文件夹，如cd ~/arm-2008q3/bin/
8、添加当前路径进入环境变量，如export PATH=$PWD:$PATH，或修改~/.bashrc文件并重新登录
9、进入内核文件夹，如cd ~/kernel.git/
10、修改Makefile文件，如gedit Makefile
更改ARCH和CROSS_COMPILE
#ARCH                ?= $(SUBARCH)
ARCH                ?= arm
#CROSS_COMPILE        ?= arm-eabi-
CROSS_COMPILE        ?= arm-none-gnueabi-
11、使用make s3c2410_defconfig，默认配置2410内核
12、运行make menuconfig
(1) 确保System Type ---> ARM system type ()中的内容为Samsung S3C2410, S3C2412, S3C2413, S3C2440, S3C2442, S3C2443
(2) 确保System Type ---> S3C2410 Machines ---> 
SMDK2410/A9M2410被勾选
(3) 取消选择与Goldfish相关的内容
Device Drivers ---> Character devices ---> < > Goldfish TTY Driver
Device Drivers ---> Power supply class support ---> < >   Goldfish battery driver (NEW)
Device Drivers ---> Real Time Clock ---> < >   GOLDFISH (NEW)
Device Drivers ---> Graphics support ---> Support for frame buffer devices ---> < >   Goldfish Framebuffer
(4) 选择S3C2410 LCD相关的内容
Device Drivers ---> Graphics support ---> Support for frame buffer devices ---> <*>   S3C2410 LCD framebuffer support
Device Drivers ---> Graphics support ---> 
Bootup logo ---> 
   Standard black and white Linux logo
Device Drivers ---> Graphics support ---> 
Bootup logo ---> 
   Standard 16-color Linux logo
Device Drivers ---> Graphics support ---> 
Bootup logo ---> 
   Standard 224-color Linux logo
(5) 选中Android内核必须选项
Kernel Features ---> 
Use the ARM EABI to compile the kernel
General setup ---> 
Use full shmem filesystem
General setup ---> 
Enable Android's Shared Memory Subsystem
System Type ---> 
Support Thumb user binaries
Device Drivers ---> Android ---> 
Android log driver
Device Drivers ---> Android ---> <*> Binder IPC Driver
(6) 尽量选中Android内核可选选项
Device Drivers ---> Android ---> 
RAM buffer console
Device Drivers ---> Android ---> 
Android timed gpio driver
Device Drivers ---> Android ---> 
Only allow certain groups to create sockets
(7) 其余CONFIG选项，如系统支持请一并选择
CONFIG_ANDROID_POWER =y
CONFIG_ANDROID_POWER_STAT =y
CONFIG_ANDROID_POWER_ALARM =y
（可以在Kconfig文件中查找ANDROID_POWER等字段进行选择，或者直接修改.config文件）
13、退出并保存.config
二、移植LCD驱动到Linux-2.6.25内核（骤二、三、四可以参照手册，也可以参照我的其他文章）
TQ2440的LCD移植可参照本人的另外的文章：[http://www.arm9.org.ru/viewthread.php?tid=3&extra=page%3D1](http://www.arm9.org.ru/viewthread.php?tid=3&extra=page%3D1)
三、移植触摸屏驱动到Linux-2.6.25内核
        TQ2440的触摸屏移植可参照本人的另外的文章：[http://www.arm9.org.ru/viewthread.php?tid=4&extra=page%3D1](http://www.arm9.org.ru/viewthread.php?tid=4&extra=page%3D1)
将256行
        ts.dev->evbit[0] = BIT(EV_SYN) | BIT(EV_KEY) | BIT(EV_ABS);
        ts.dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT(BTN_TOUCH);
改为：
ts.dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
ts.dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
四、移植网卡驱动到Linux-2.6.25内核
        TQ2440的网卡移植可参照本人的另外的文章：[http://www.arm9.org.ru/viewthread.php?tid=5&extra=page%3D1](http://www.arm9.org.ru/viewthread.php?tid=5&extra=page%3D1)
五、制作支持ARMv4的Android根文件系统
1、从[http://d.namipan.com/d/403e5eb4ec3af5d8029d994879808140f0c9222be3858a02](http://d.namipan.com/d/403e5eb4ec3af5d8029d994879808140f0c9222be3858a02)
或
[http://www.hiapk.com/bbs/download.php?keyid=f5d23685c772ca9eb4772a6a86274a1705ef58019c851c3972830c94b24c361d](http://www.hiapk.com/bbs/download.php?keyid=f5d23685c772ca9eb4772a6a86274a1705ef58019c851c3972830c94b24c361d)
下载Android的ARMv4版本根文件
2、在/dev下增加null和console两个设备节点
mknod -m 660 null c 1 3
mknod -m 660 console c 5 1
3、在U-boot中修改启动参数init为/init，完整的命令行如：
a,开机按住空格键
b,选[0] Set the boot parameters
c, [1] Set NFS boot parameter
d,输入192.168.1.3（这个是你的pc的ip，我的是这个），回车
e, 输入192.168.1.6（这个是板子ip，默认是这个），回车，输入255.255.255.0
f, 再选[3] Set parameter，这个很重要，输入bootargs,回车，输入console=ttySAC0 root=/dev/nfs init=/init nfsroot=192.168.1.3:/root_nfs ip=192.168.1.6:192.168.1.3:192.168.1.6:255.255.255.0:[www.arm9.org.ru:eth0:off](http://www.arm9.org.ru:eth0:off/)
g,保存重启
注：TQ2440自带的内核文件似乎在nfs启动上有问题，建议参照第五步驱动网卡；/root_nfs 这个是你的nfs目录，设置成你自己的
4、修改/init.rc文件，去除除mount tmpfs tmpfs /sqlite_stmt_journals size=4m之外的所有mount命令
系统制作完成，现在应该可以看到漂亮的大表了。首次启动需要创建文件，TQ2440启动还是比较快的，大约5分钟。然后每次启动只需要1分钟。
后记：至此，android能够驱动了，由于文件系统太大，做成文件系统有70多兆，2440只有64m空间，因此只能用nfs方式。哪天有时间将文件系统精简一下或许能到64m一下，以后再专门写吧！
注意：关于触摸屏，lcd，网卡的驱动上面的连接均是基于2.6.29的，而android内核目前是2.6.25，关于2.6.25的驱动可以参考TQ2440官方文档。
一、目前进展
1．Android已经可以通过NFS在TQ2440开发板上运行
2010.03.12 update:最近自己编译了android的文件系统，并在TQ上运行了，比熊猫版的bin程序多些，基本功能已经实现，大家可以从：
[http://d.namipan.com/d/b64ce43b2388e5e1f66af18149824b98cd13b926c4921f02](http://d.namipan.com/d/b64ce43b2388e5e1f66af18149824b98cd13b926c4921f02)
下载文件系统试试，改天整理一下，发个详细的移植过程和注意事项。
2010.04.09 update:
网络设置：
route add default gw 192.168.1.1 dev eth0
setprop net.dns1 202.120.223.5
2010.05.08 update:
1.使用ADB
Ubuntu：
export ADBHOST=192.168.1.101
adb kill-server
adb start-server
adb shell
2.启动时battery导致关闭
修改：frameworks/base/services/jni/com_android_server_BatteryService.cpp,补丁见附件。
在init.rc文件中添加
setprop battery.fake_ac 1 
```
#include <errno.h>
#include <unistd.h>
#include <cutils/properties.h>
#if HAVE_ANDROID_OS
#include <linux/ioctl.h>
#endif
@@ -173,6 +176,29 @@
static void android_server_BatteryService_update(JNIEnv* env, jobject obj)
{
    static int enable_fake_ac = -1;
    if( enable_fake_ac == -1 ){
        char value[PROPERTY_VALUE_MAX];
        enable_fake_ac = 0;
        if ( property_get("battery.fake_ac", value, 0) ){
            if( 0 == strcmp( value, "true") || 0 == strcmp(value, "1") ){
                enable_fake_ac = 1;
            }
        }
    }
 if( enable_fake_ac == 1 ){
        env->SetBooleanField(obj, gFieldIds.mAcOnline, true);
        env->SetBooleanField(obj, gFieldIds.mUsbOnline, false);
        env->SetBooleanField(obj, gFieldIds.mBatteryPresent, true );
        env->SetIntField(obj, gFieldIds.mBatteryStatus, gConstants.statusCharging);
        env->SetIntField(obj, gFieldIds.mBatteryLevel, 50);
        env->SetIntField(obj, gFieldIds.mBatteryVoltage, 4000);
        env->SetIntField(obj, gFieldIds.mBatteryTemperature, 20);
        env->SetIntField(obj, gFieldIds.mBatteryHealth, gConstants.healthGood);
        env->SetObjectField(obj, gFieldIds.mBatteryTechnology, env->NewStringUTF("Fake"));
  return;
 }
     setBooleanField(env, obj, AC_ONLINE_PATH, gFieldIds.mAcOnline);
     setBooleanField(env, obj, USB_ONLINE_PATH, gFieldIds.mUsbOnline);
     setBooleanField(env, obj, BATTERY_PRESENT_PATH, gFieldIds.mBatteryPresent);
```
```
#include <errno.h>
#include <unistd.h>
#include <cutils/properties.h>
#if HAVE_ANDROID_OS
#include <linux/ioctl.h>
#endif
@@ -173,6 +176,29 @@
static void android_server_BatteryService_update(JNIEnv* env, jobject obj)
{
    static int enable_fake_ac = -1;
    if( enable_fake_ac == -1 ){
        char value[PROPERTY_VALUE_MAX];
        enable_fake_ac = 0;
        if ( property_get("battery.fake_ac", value, 0) ){
            if( 0 == strcmp( value, "true") || 0 == strcmp(value, "1") ){
                enable_fake_ac = 1;
            }
        }
    }
 if( enable_fake_ac == 1 ){
        env->SetBooleanField(obj, gFieldIds.mAcOnline, true);
        env->SetBooleanField(obj, gFieldIds.mUsbOnline, false);
        env->SetBooleanField(obj, gFieldIds.mBatteryPresent, true );
        env->SetIntField(obj, gFieldIds.mBatteryStatus, gConstants.statusCharging);
        env->SetIntField(obj, gFieldIds.mBatteryLevel, 50);
        env->SetIntField(obj, gFieldIds.mBatteryVoltage, 4000);
        env->SetIntField(obj, gFieldIds.mBatteryTemperature, 20);
        env->SetIntField(obj, gFieldIds.mBatteryHealth, gConstants.healthGood);
        env->SetObjectField(obj, gFieldIds.mBatteryTechnology, env->NewStringUTF("Fake"));
  return;
 }
     setBooleanField(env, obj, AC_ONLINE_PATH, gFieldIds.mAcOnline);
     setBooleanField(env, obj, USB_ONLINE_PATH, gFieldIds.mUsbOnline);
     setBooleanField(env, obj, BATTERY_PRESENT_PATH, gFieldIds.mBatteryPresent);
```
二、目前缺陷
1．触摸屏没有校准
2．Android中时钟停止，结束zygote进程，系统重启，时钟更新，但再次停止不动
三、移植步骤
1、移植Android内核到TQ2440开发板上
1、从[http://code.google.com/p/android/downloads/list](http://code.google.com/p/android/downloads/list)上下载linux-2.6.25-android-1.0_r1.tar.gz
2、将内核解压到用户目录，如/root/kernel.git
3、进入kernel.git文件夹，如cd ~/kernel.git/
4、修改arch/arm/plat-s3c24xx/common-smdk.c文件中的nand flash分区设置，如gedit arch/arm/plat-s3c24xx/common-smdk.c
修改static struct mtd_partition smdk_default_nand_part[]结构体为：
```
static struct mtd_partition smdk_default_nand_part[] = {
        [0] = {
                .name        = "TQ2440_uboot",
                .size        = 0x00040000,
                .offset        = 0x00000000,
        },
        [1] = {
                .name        = "TQ2440_kernel",
                .offset        = 0x0004C000,
                .size        = 0x00200000,
        },
        [2] = {
                .name        = "TQ2440_yaffs2",
                .offset        = 0x0024C000,
                .size        = 0x03DB0000,
        }
};
```
5、在[http://www.codesourcery.com/gnu_toolchains/arm/portal/release644](http://www.codesourcery.com/gnu_toolchains/arm/portal/release644)网站上下载IA32 GNU/Linux TAR。
6、将文件解压到用户目录，如/usr/local/arm/arm-2008q3
7、进入arm-2008q3/bin文件夹，如cd ~/arm-2008q3/bin/
8、添加当前路径进入环境变量，如export PATH=$PWD:$PATH，或修改~/.bashrc文件并重新登录
9、进入内核文件夹，如cd ~/kernel.git/
10、修改Makefile文件，如gedit Makefile
更改ARCH和CROSS_COMPILE
#ARCH                ?= $(SUBARCH)
ARCH                ?= arm
#CROSS_COMPILE        ?= arm-eabi-
CROSS_COMPILE        ?= arm-none-gnueabi-
11、使用make s3c2410_defconfig，默认配置2410内核
12、运行make menuconfig
(1) 确保System Type ---> ARM system type ()中的内容为Samsung S3C2410, S3C2412, S3C2413, S3C2440, S3C2442, S3C2443
(2) 确保System Type ---> S3C2410 Machines ---> 
SMDK2410/A9M2410被勾选
(3) 取消选择与Goldfish相关的内容
Device Drivers ---> Character devices ---> < > Goldfish TTY Driver
Device Drivers ---> Power supply class support ---> < >   Goldfish battery driver (NEW)
Device Drivers ---> Real Time Clock ---> < >   GOLDFISH (NEW)
Device Drivers ---> Graphics support ---> Support for frame buffer devices ---> < >   Goldfish Framebuffer
(4) 选择S3C2410 LCD相关的内容
Device Drivers ---> Graphics support ---> Support for frame buffer devices ---> <*>   S3C2410 LCD framebuffer support
Device Drivers ---> Graphics support ---> 
Bootup logo ---> 
   Standard black and white Linux logo
Device Drivers ---> Graphics support ---> 
Bootup logo ---> 
   Standard 16-color Linux logo
Device Drivers ---> Graphics support ---> 
Bootup logo ---> 
   Standard 224-color Linux logo
(5) 选中Android内核必须选项
Kernel Features ---> 
Use the ARM EABI to compile the kernel
General setup ---> 
Use full shmem filesystem
General setup ---> 
Enable Android's Shared Memory Subsystem
System Type ---> 
Support Thumb user binaries
Device Drivers ---> Android ---> 
Android log driver
Device Drivers ---> Android ---> <*> Binder IPC Driver
(6) 尽量选中Android内核可选选项
Device Drivers ---> Android ---> 
RAM buffer console
Device Drivers ---> Android ---> 
Android timed gpio driver
Device Drivers ---> Android ---> 
Only allow certain groups to create sockets
(7) 其余CONFIG选项，如系统支持请一并选择
CONFIG_ANDROID_POWER =y
CONFIG_ANDROID_POWER_STAT =y
CONFIG_ANDROID_POWER_ALARM =y
（可以在Kconfig文件中查找ANDROID_POWER等字段进行选择，或者直接修改.config文件）
13、退出并保存.config
二、移植LCD驱动到Linux-2.6.25内核（骤二、三、四可以参照手册，也可以参照我的其他文章）
TQ2440的LCD移植可参照本人的另外的文章：[http://www.arm9.org.ru/viewthread.php?tid=3&extra=page%3D1](http://www.arm9.org.ru/viewthread.php?tid=3&extra=page%3D1)
三、移植触摸屏驱动到Linux-2.6.25内核
        TQ2440的触摸屏移植可参照本人的另外的文章：[http://www.arm9.org.ru/viewthread.php?tid=4&extra=page%3D1](http://www.arm9.org.ru/viewthread.php?tid=4&extra=page%3D1)
将256行
        ts.dev->evbit[0] = BIT(EV_SYN) | BIT(EV_KEY) | BIT(EV_ABS);
        ts.dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT(BTN_TOUCH);
改为：
ts.dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
ts.dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
四、移植网卡驱动到Linux-2.6.25内核
        TQ2440的网卡移植可参照本人的另外的文章：[http://www.arm9.org.ru/viewthread.php?tid=5&extra=page%3D1](http://www.arm9.org.ru/viewthread.php?tid=5&extra=page%3D1)
五、制作支持ARMv4的Android根文件系统
1、从[http://d.namipan.com/d/403e5eb4ec3af5d8029d994879808140f0c9222be3858a02](http://d.namipan.com/d/403e5eb4ec3af5d8029d994879808140f0c9222be3858a02)
或
[http://www.hiapk.com/bbs/download.php?keyid=f5d23685c772ca9eb4772a6a86274a1705ef58019c851c3972830c94b24c361d](http://www.hiapk.com/bbs/download.php?keyid=f5d23685c772ca9eb4772a6a86274a1705ef58019c851c3972830c94b24c361d)
下载Android的ARMv4版本根文件
2、在/dev下增加null和console两个设备节点
mknod -m 660 null c 1 3
mknod -m 660 console c 5 1
3、在U-boot中修改启动参数init为/init，完整的命令行如：
a,开机按住空格键
b,选[0] Set the boot parameters
c, [1] Set NFS boot parameter
d,输入192.168.1.3（这个是你的pc的ip，我的是这个），回车
e, 输入192.168.1.6（这个是板子ip，默认是这个），回车，输入255.255.255.0
f, 再选[3] Set parameter，这个很重要，输入bootargs,回车，输入console=ttySAC0 root=/dev/nfs init=/init nfsroot=192.168.1.3:/root_nfs ip=192.168.1.6:192.168.1.3:192.168.1.6:255.255.255.0:[www.arm9.org.ru:eth0:off](http://www.arm9.org.ru:eth0:off/)
g,保存重启
注：TQ2440自带的内核文件似乎在nfs启动上有问题，建议参照第五步驱动网卡；/root_nfs 这个是你的nfs目录，设置成你自己的
4、修改/init.rc文件，去除除mount tmpfs tmpfs /sqlite_stmt_journals size=4m之外的所有mount命令
系统制作完成，现在应该可以看到漂亮的大表了。首次启动需要创建文件，TQ2440启动还是比较快的，大约5分钟。然后每次启动只需要1分钟。
后记：至此，android能够驱动了，由于文件系统太大，做成文件系统有70多兆，2440只有64m空间，因此只能用nfs方式。哪天有时间将文件系统精简一下或许能到64m一下，以后再专门写吧！
注意：关于触摸屏，lcd，网卡的驱动上面的连接均是基于2.6.29的，而android内核目前是2.6.25，关于2.6.25的驱动可以参考TQ2440官方文档。
