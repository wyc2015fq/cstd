# wifi详解（五） - DoubleLi - 博客园






# 1        Android平台的Wifi模块移植要点

## 1.1      Wifi结构

**user interface**



**Android WiFiService**

**WPA_Supplicant**

**DHD Driver**

**Dongle Binary**

**BCM43xxHardware**



蓝色：需要修改

黑色：不需要修改



蓝色部分是为了在android环境下，支持bcm43xx芯片需要修改的部分，各个部分的功能如下：

**1. User interface（用户接口层）**：控制和设置wifi的用户接口层

**2. Android wifi service：**它是android系统中的wifi服务部分，主要用来启动和停止wpa_supplicant，并作为用户接口层和wpa_supplicant交互的桥梁存在

**3. Wpa_supplicant：**支持wpa和wapi的外部库

**4. Dhd driver：**wifi模块的驱动

**5. Dongle binary：**BCM43xx的固件

**6. BCM43xx：**wifi的硬件芯片（是一个组合集成芯片）

## 1.2      Wifi模块环境

Wifi模块环境包括以下部分：

**1.      bcm43xx驱动源码**

**2.      wpa_supplicant和支持WAPI的wapilib库**

**3.      nvram.txt（BCM43xx芯片配置文件）**

**4.      wpa_supplicant.conf和wifi固件**

一般来说，第三和第四条件的文件在BCM43xx源码中都有包含。

## 1.3      Wifi模块的编译

### 1.3.1       Wifi驱动源码

这里不以某个类型的BCM43xx芯片和android版本作为特例来讲解wifi模块的编译过程，只是对编译的通用部分做简要的说明。

一般来说，wifi模块的编译可采用两种红方式：一是内部编译，二是模块编译。通常使用的是模块编译，这里也以模块编译wifi模块为例来说明wifi模块的编译过程。

在wifi模块驱动源码中，主要包含以下几个目录：

Firmware：里面会提供对应BCM43xx芯片的固件

Config：提供wpa_supplicant.conf和nvram.txt以及dhcpcd.conf，还有android.mk文件。如果没有上面的配置文件，需要找到放入该目录。

Android.mk：这是一个文件，也是编译android平台编译wifi模块的入口

Src：wifi驱动的源码目录

其实，你看到的broadcom提供的驱动源码可能的组织方式跟上面是有差别的，但大致的内容差不多，上面只是针对模块源码中目录和文件的不同用途来说明的。

### 1.3.2       在android平台添加BCM43xx驱动

要在android平台编译wifi模块，首先要将wifi模块的源码添加到android平台下的目录中。至于添加到什么地方也不是固定的，一般会添加到vendor目录下的某个目录下，下面我们把BCM43xx模块源码放在vendor/xxxx/wlan/bcm43xx下（XXXXX代表产品名称）。要知道，在添加源码之前，这个bcm43xx目录是不存在的，需要手动创建，并在该目录下创建android和src两个目录：

PC$ cd<ANDROID_ROOT>

PC$ mkdir ‐p vendor/xxxx/wlan/bcm4325/

PC$ cdvendor/xxxx/wlan/bcm4325/

PC$ mkdirandroid src

PC$ ls

android src

       在创建好上面的目录后，就可以在目录下添加BCM43xx的相关源码了，src存放的是驱动源码，android目录下主要存放固件（即二级制镜像），编译文件（Android.mk），配置文件（nvram.txt和wpa_supplicant.conf）等。

       添加好相关源码后，还需要做一定的修改工作（主要对编译文件的修改），不然模块无法被正常编译的，Android.mk的内容修改操作如下。

***PC$ cp<WORK>/bcm4325_source/open‐src/src/dhd/android/config/Android.mk***

***<ANDROID_ROOT>/vendor/xxxx/wlan/bcm4325/android***

***PC$ cd<ANDROID_ROOT>/vendor/xxxx/wlan/bcm4325/android***

***PC$ viAndroid.mk     //到这里，找到Android.mk文件，文件内容如下***

……

**#**

**# Install WLAN Driver, firmware, and configurationfiles.**

**#**

**local_target_dir :=$(TARGET_OUT_ETC)/firmware    //这里定义local_target_dir的路径，在后面会用到**

**########################**

**LOCAL_PATH := $(call my-dir)**

**include $(CLEAR_VARS)**

**LOCAL_MODULE :=sdio-g-cdc-reclaim-idsup-wme-ccx-wapi.bin**

**LOCAL_MODULE_TAGS := user development**

**LOCAL_MODULE_CLASS := ETC**

**LOCAL_MODULE_PATH := $(local_target_dir)**

**LOCAL_SRC_FILES := $(LOCAL_MODULE)**

**include $(BUILD_PREBUILT)**

**########################**

**include $(CLEAR_VARS)**

**LOCAL_MODULE := nvram_4325b0.txt**

**LOCAL_MODULE_TAGS := user development**

**LOCAL_MODULE_CLASS := ETC**

**LOCAL_MODULE_PATH := $(local_target_dir)**

**LOCAL_SRC_FILES := $(LOCAL_MODULE)**

**include $(BUILD_PREBUILT)**

**########################**

**include $(CLEAR_VARS)**

**LOCAL_MODULE := dhd.ko**

**LOCAL_MODULE_TAGS := user development**

**LOCAL_MODULE_CLASS := ETC**

**LOCAL_MODULE_PATH :=$(TARGET_OUT)/lib/modules**

**LOCAL_SRC_FILES := $(LOCAL_MODULE)**

**include $(BUILD_PREBUILT)**

**########################**

**include $(CLEAR_VARS)**

**LOCAL_MODULE := wpa_supplicant.conf**

**LOCAL_MODULE_TAGS := user development**

**LOCAL_MODULE_CLASS := ETC**

**LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/wifi**

**LOCAL_SRC_FILES := $(LOCAL_MODULE)**

**include $(BUILD_PREBUILT)**

**########################**

**include $(CLEAR_VARS)**

**LOCAL_MODULE := dhcpcd.conf**

**LOCAL_MODULE_TAGS := user development**

**LOCAL_MODULE_CLASS := ETC**

**LOCAL_MODULE_PATH :=$(TARGET_OUT_ETC)/dhcpcd**

**LOCAL_SRC_FILES := android_dhcpcd.conf**

**include $(BUILD_PREBUILT)**

**########################**

       对上面的内容有必要说明一下：LOCAL_MODULE和LOCAL_MODULE_PATH的定义是这里需要修改的地方。对于LOCAL_MODULE_PATH的修改时一定的，但它主要依赖与android平台已经定义好的一些变量，如：TARGET_OUT _ETC和TARGET_OUT等。LOCAL_MODULE定义的是要操作的对象，也是前面提到的配置文件，编译好的驱动模块二进制文件和固件等。

       该文件的作用：将各个文件拷贝到定义的地方，以供驱动模块和wpa_supplicant等使用。、

       对于驱动的源码，用户可以通过定制的wifi芯片管理函数来关闭和开启wifi模块，在dhd/sys/dhd_custom_gpio.c文件中有如下的函数定义：

**#define CUSTOMER_HW**

**#ifdef CUSTOMER_HW**

**extern void bcm_wlan_power_off(int);**

**BCM43xx BCM43xx Porting Guide for Android System**

**Broadcom Corporation Proprietary and Confidential**

**9**

**extern void bcm_wlan_power_on(int);**

**#endif /* CUSTOMER_HW */**

       所以用户可以使用自己定制的开关函数，或者使用BCM43xx自带的开关函数来控制wifi模块的开关。如果使用定制的开关函数，则需要在平台相关的代码中定义这两个函数：

**Path：kernel/arch/arm/mach‐XXX/board‐XXXX.c**

**……**

**void bcm_wlan_power_off(int onoff);**

**void bcm_wlan_power_on(int onoff);**

**……**

       前面的Android.mk的修改中，有对dhd.ko的拷贝操作，这就是说源码编译后生成的模块二进制文件应该被暂时拷贝到Android.mk中指示的地方。这个暂时拷贝操作在驱动源码的编译文件dhd/linux/Makefile中被执行。

**PC$ cd<ANDROID_ROOT>/vendor/xxxx/wlan/bcm4325/src**

**PC$ vi dhd/linux/Makefile +315**

**modules: $(OFILES)**

**test -r ./Makefile || ln –s $(SRCBASE)/dhd/linux/makefile.26./Makefile**

**$(MAKE) -C $(LINUXDIR) M=$(shell pwd) $(if $(VERBOSE),V=1)modules**

**cp $(KMODULES) $(SRCBASE)/../android/.**

下一步需要做的就是将wifi模块源码的顶层目录下的android.mk文件放到bcm43xx目录下，它是android编译入口文件，表示在编译android时，这个wifi驱动模块被包含到整个android编译系统中。它的内容如下：

PC$ cd <ANDROID_ROOT>/vendor/xxxx/wlan/bcm4325/

PC$ vim Android.mk

**ifeq ($(BOARD_WLAN_DEVICE),bcm43xx)**

**include $(callall-subdir-makefiles)**

**endif**

很简单，如果被BOARD_WLAN_DEVICE被定义为bcm43xx，调用call all-subdir-makefiles函数来包含所有子目录下的Android.mk文件，这是android编译系统的规则。

那么，最后一步就是在相关的平台配置文件中定义BOARD_WLAN_DEVICE为bcm43xx，文件路径：**vendor/XXXX/XXXX/BoardConfig.mk**。

### 1.3.3       编译wifi驱动源码

在编译wifi驱动源码之前，首先要建立kernel环境，即要在编译wifi模块之前编译kernel，因为模块的编译时依赖与内核的。

内核的编译如下：

**PC$ cd <ANDROID_ROOT>/kernel**

**PC$ make ARCH=arm CROSS_COMPILE=<ANDROID_ROOT>/prebuilt/**

**linuxx86/toolchain/arm‐eabi‐4.4.0/bin/arm‐eabi‐ XXXX_defconfig**

**PC$ make ARCH=armCROSS_COMPILE=<ANDROID_ROOT>/prebuilt/**

**linuxx86/toolchain/arm‐eabi‐4.4.0/bin/arm‐eabi‐ zImage**

如果设置好了ARCH和CROSS_COMPILE的环境变量，就可以省略这两个选项的定义，直接使用make命令生成zImage了。

在编译好kernel之后，就可以进入bcm43xx源码目录dhd/linux下，这里面有两个文件Makefile和Makefile.26，如果没有Makefile文件就将Makfeile.26拷贝成Makfile文件（实际上是建立了一个符号链接文件），当然也要做前面的dhd.ko暂时拷贝命令的添加了。在该目录下输入如下命令编译模块源码：

**PC$ cd<ANDROID_ROOT>/vendor/xxxx/wlan/bcm4325/src**

**PC$ cd dhd/linux/**

**PC$ make ARCH=armLINUXDIR=<ANDROID_ROOT>/kernel**

**CROSS_COMPILE=<ANDROID_ROOT>/prebuilt/linux‐x86/toolchain/arm‐eabi‐**

**4.4.0/bin/arm‐eabi‐ dhd‐cdc‐sdmmc‐gpl**

最后一条命令增加了一个命令选项LINUXDIR的传值，即告诉该编译文件kernel的所在位置，进入kernel的Makefile文件做设置工作，然后返回该目录下的Makefile文件进行编译工作。

到这里bcm43xx驱动源码已经编译完成，可以到android目录下查看有没有dhd.ko文件生成，如果有，即表明编译成功，否则查看编译过程的出错信息，解决错误问题再编译，直至编译成功。

### 1.3.4       在android中使用BCM43xx

前面的过程只是在android系统中添加编译了BCM43xx驱动源码，但是要想在android中使用它，还需要费一番功夫。需要修改的地方在开始的地方已经用蓝色背景标注了，在这里我们要看看到底要修改哪些文件。

#### 1.3.4.1        hardware/libhardware_legacy/wifi/wifi.c

为BCM43xx驱动模块做适当修改。Wifi.c作为加载wifi驱动模块和启动关闭wpa_supplicant的重要角色而存在，为了使wifi能更好的工作，该文件中的一些变量参数必须被适当的设置（根据wpa_supplicant.conf）。

**PC$ vi hardware/libhardware_legacy/wifi/wifi.c**

**......**

**#define WIFI_DRIVER_MODULE_PATH**

**"/system/lib/modules/dhd.ko"**

**......**

**#define WIFI_DRIVER_MODULE_NAME"dhd"**

**......**

**#define WIFI_DRIVER_MODULE_ARG**

**"firmware_path=/etc/firmware/sdio-g-cdc-reclaim-idsup-wme-ccxwapi.**

**bin nvram_path=/etc/firmware/nvram_4325b0.txt"**

**......**

**#define WIFI_TEST_INTERFACE "eth0"**

**......**

**static const char IFACE_DIR[] = "";**

**......**

**static const char SUPP_CONFIG_TEMPLATE[]=**

**"/data/misc/wifi/wpa_supplicant.conf";**

**……**

**static const char SUPP_CONFIG_FILE[] =**

**"/etc/wifi/wpa_supplicant.conf";**

**......**



上面的红色部分，就是对wifi驱动的特殊化定义，不同的wifi驱动会有不同的定义。修改还包括了固件的和配置文件的路径，以及接口名的相关定义。

**WIFI_TEST_INTERFACE**定义的是wpa_supplicant和UI的交互接口名，当通过property_get()函数获取wifi接口名错误时，就会使用此定义的接口名。Property_get()函数获取的接口名，就是init.xx.rc文件中的接口定义的wifi.interface：

**Setprop wifi.interface “eth0”**

**SUPP_CONFIG_TEMPLATE**定义的是wpa_supplicant配置文件的临时文件。而**SUPP_CONFIG_FILE**定义的是wpa_supplicant运行时使用的配置文件，并且对wifi配置的修改也会保持到这个文件里。

**IFACE_DIR**是wpa_supplicant控制接口的目录，该目录下的接口被UI用来连接到wpa_supplicant，这个目录是由wpa_supplicant决定的，其实，这个接口实际上是一个socket，在wpa_supplicant启动时被创建，wpa_supplicant_ctrl_iface_init()

中有两种方式来创建这个socket接口：

**（1）android系统的socket**

       它使用wpa_%ctrl_interface%组合来定义自己的名字，通过socketwpa_eth0 dgram ...命令来生成的（在init.rc中），如果使用这种socket，下面的代码中的接口名必须是一直的：

**ctrl_interface=eth0 (In wpa_supplicant.conf)**

**"socket wpa_eth0 ..." (In init.*.rc )**

**setprop wifi.interface "eth0" (In init.*.rc)**

**wpa_supplicant ‐ieth0 ... (In init.*.rc)**

       此时，IFACE_DIR的定义为NULL。

**（2）特殊socket**

       Wpa_supplicant会通过wpa_supplicant.conf中的ctrl_interface目录来创建控制接口socket，而这个值一般被定义为："ctrl_interface=DIR=/data/misc/...."，这个socket的名字由wpa_supplicant的“-i”参数传递，如果使用这种方式的socket，下面的变量定义必须一致：

**ctrl_interface=DIR=/data/misc/ (In wpa_supplicant.conf)**

**IFACE_DIR=/data/misc/ (In wifi.c)**



#### 1.3.4.2        kernel/arch/arm/mach‐XXX/board‐XXXX.c

为BCM43xx 驱动添加电源管理部分内容，这里不详细叙述。

#### 1.3.4.3        system/core/rootdir/etc/init.xxx.rc 

为支持BCM43xx驱动模块，修改wpa_supplicant服务。Init.xxx.rc文件的内容一般如下所示：

**PC$ vi system/core/rootdir/etc/init.xxx.rc**

**......         //添加android系统接口**

**# to enable wifi**

**setprop wifi.interface "eth0"**

**# end of wifi**

**......         //修改wpa_supplicant.conf的权限**

**chmod 0660 /etc/wifi/wpa_supplicant.conf**

**chown wifi wifi /etc/wifi/wpa_supplicant.conf**

**......         //wpa_supplicant服务的启动命令**

**service wpa_supplicant /system/bin/logwrapper**

**/system/bin/wpa_supplicant -Dwext -ieth0 -**

**c/etc/wifi/wpa_supplicant.conf**

**......         //给wpa_supplicant修改socket接口**

**socket wpa_eth0 dgram 660 wifi wifi**

**......         //修改dhcp接口**

**service dhcpcd /system/bin/dhcpcd -BKL eth0**

**......**

#### 1.3.4.4        vendor/XXXX/XXXX/BoardConfig.mk

为BCM43xx驱动添加配置变量。

**PC$ vi vendor/XXXX/XXXX/BoardConfig.mk**

**……**

**ifneq ($(BUILD_TINY_ANDROID), true)**

**……**

**BOARD_WPA_SUPPLICANT_DRIVER := WEXT**

**BOARD_WLAN_DEVICE := bcm43xx**

**endif # !BUILD_TINY_ANDROID**

**……**

       对BCM43xx的WAPI的支持也需要做一系列的修改，这里不再对其进行详细的叙述了。

# 2        总结

最近一段时间里对wifi模块的学习，让我对wifi有了更多的了解，但这些也是不够的。对于wifi模块，需要了解的东西很多，这里只对其工作原理和工作流程以及移植过程进行了简单的阐述，还有很多方面没有概括到，如：协议层的实现方式和过程，以及数据传输中的细节和SDIO相关内容等等，这些在实际的wifi模块工作时可能都需要我们去进一步的分析和把握，虽然对wifi的理解可能不到位，但对这段时间学习做一次总结，也是大有裨益的，希望有机会可以更深入的学习wifi模块。

在wifi模块调试的过程中也会出现一系列的问题，这些问题可以归纳成不同的类型，对这些类型的问题做分析和总结，可以帮助我们在以后调试工作中快速定位问题所在。

对wifi的了解还远远不够，希望和wifi感兴趣的和已经成为高手的同仁能有更多的交流。









