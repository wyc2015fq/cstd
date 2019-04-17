# 嵌入式Linux USB WIFI驱动的移植 - DoubleLi - 博客园






硬件平台：飞思卡尔MX258开发板

操作系统：Linux2.6.31

WIFI：    RT2860 USB WIFI模组

交叉编译环境：gcc version 4.1.2



调试步骤：

第一步：测试USB　HOST接口

在menuconfig中将USB HOST设置为内核模式：





重新编译内核后启动开发板，插入U盘并挂载：

mount /dev/sda1 /tmp

ls /tmp

可以看到U盘已经正常挂载，测试USB　HOST OK!

第二步：网上下载雷凌最新的USB驱动，

2011_0107_RT3070_RT3370_Linux_STA_v2[1].5.0.1_DPO.tar.bz2

拷备到Linux目录并解压：

tar  jxf  2011_0107_RT3070_RT3370_Linux_STA_v2[1].5.0.1_DPO.tar.bz2

由于上面名字很长，可以修改为简短的名字：

mv  2011_0107_RT3070_RT3370_Linux_STA_v2[1].5.0.1_DPO  RT3070_Linux_STA

第三步：进入RT3070_Linux_STA目录，看到有一个README_STA_usb文件，里面介绍了如何加载该驱动：

=======================================================================

Build Instructions: 

====================



1> $tar -xvzf DPB_RT2870_Linux_STA_x.x.x.x.tgz

    go to "./DPB_RT2870_Linux_STA_x.x.x.x" directory.



2> In Makefile

          set the "MODE = STA" in Makefile and chose the TARGET to Linux by set "TARGET = LINUX"

          define the linux kernel source include file path LINUX_SRC

          modify to meet your need.



3> In os/linux/config.mk

         define the GCC and LD of the target machine

         define the compiler flags CFLAGS

         modify to meet your need.

         ** Build for being controlled by NetworkManager or wpa_supplicant wext functions

            Please set 'HAS_WPA_SUPPLICANT=y' and 'HAS_NATIVE_WPA_SUPPLICANT_SUPPORT=y'.

            => #>cd wpa_supplicant-x.x

            => #>./wpa_supplicant -Dwext -ira0 -c wpa_supplicant.conf -d

         ** Build for being controlled by WpaSupplicant with Ralink Driver

            Please set 'HAS_WPA_SUPPLICANT=y' and 'HAS_NATIVE_WPA_SUPPLICANT_SUPPORT=n'.

            => #>cd wpa_supplicant-0.5.7

            => #>./wpa_supplicant -Dralink -ira0 -c wpa_supplicant.conf -d



4> $make

         # compile driver source code

         # To fix "error: too few arguments to function ˉiwe_stream_add_event"

           => $patch -i os/linux/sta_ioctl.c.patch os/linux/sta_ioctl.c



5> $cp RT2870STA.dat  /etc/Wireless/RT2870STA/RT2870STA.dat



6> load driver, go to "os/linux/" directory.

    #[kernel 2.4]

    #    $/sbin/insmod rt2870sta.o

    #    $/sbin/ifconfig ra0 inet YOUR_IP up



    #[kernel 2.6]

    #    $/sbin/insmod rt2870sta.ko

    #    $/sbin/ifconfig ra0 inet YOUR_IP up



7> unload driver   

    $/sbin/ifconfig ra0 down

         $/sbin/rmmod rt2870sta

这里go to "./DPB_RT2870_Linux_STA_x.x.x.x" directory.即我们刚解压的RT3070_Linux_STA目录。修改该目录下的makefile文件，以下只给出修改部分：

#PLATFORM = PC

PLATFORM = RALINK_2880



ifeq ($(PLATFORM), RALINK_2880)

LINUX_SRC = /home/lqm/share/G360/kernel_kfb

CROSS_COMPILE = /opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-

endif

ifeq ($(RT28xx_MODE),APSTA)

         cp -f $(RT28xx_DIR)/os/linux/rt$(CHIPSET)apsta.ko /tftpboot

ifeq ($(OSABL),YES)

         cp -f $(RT28xx_DIR)/os/linux/rtutil$(CHIPSET)apsta.ko /tftpboot

         cp -f $(RT28xx_DIR)/os/linux/rtnet$(CHIPSET)apsta.ko /tftpboot

endif

else

         cp -f $(RT28xx_DIR)/os/linux/rt$(CHIPSET)sta.ko /home/lqm/share/NFS/tmp

注意，虽然我们使用的是2860模组，我们这里仍然可以定义PLATFORM为RALINK_2880，只要后面对应的编译环境正确就可以了。LINUX_SRC表示内核的目录，CORSS_COMPILE为交叉编译环境，最末一行为编译完后将生成的KO文件复制到NFS文件系统的tmp目录。

第四步：按照第三步README_STA_usb给出的提示，修改config.mk文件，这里也只给出修改部分：

# Support Wpa_Supplicant

HAS_WPA_SUPPLICANT=y

# Support Native WpaSupplicant for Network Maganger

HAS_NATIVE_WPA_SUPPLICANT_SUPPORT=n

CC := /opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-gcc

LD := /opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-ld

ifeq ($(PLATFORM), RALINK_2880)

EXTRA_CFLAGS := -D__KERNEL__ -I$(LINUX_SRC)/include -I$(RT28xx_DIR)/include -Wall -Wstrict-prototypes -Wno-trigraphs -O2 -fno-strict-aliasing -fno-common -Uarm -fno-common -pipe -D__LINUX_ARM_ARCH__=5 -march=armv5te -msoft-float -Uarm -DMODULE -DMODVERSIONS -include $(LINUX_SRC)/include/config/modversions.h $(WFLAGS)

export EXTRA_CFLAGS

endif

注意，这里CC为交叉编译环境，LD为交叉编译的链接。默认EXTRA_CFLAGS为CFLAGS，这里需要修改为EXTRA_CFLAGS，否则编译时会提示如下错误：

scripts/Makefile.build:49: *** CFLAGS was changed in "/home/lqm/share/RT3070_Linux_STA/os/linux/Makefile". Fix it to use EXTRA_CFLAGS。 停止。

LINUX_ARM_ARCH一定要设置为5，-march一定要设置得和CPU匹配，由于这里为MX258，因此设置为armv5te，不可设置为armv5t或armv5，否则出现如下错误：

  CC [M]  /home/lqm/share/RT3070_Linux_STA/os/linux/../../os/linux/rt_main_dev.o

{standard input}: Assembler messages:

{standard input}:340: Error: selected processor does not support `pld [r5,#0]'

{standard input}:349: Error: selected processor does not support `pld [r5,#0]'

make[2]: *** [/home/lqm/share/RT3070_Linux_STA/os/linux/../../os/linux/rt_main_dev.o] 错误 1

make[1]: *** [_module_/home/lqm/share/RT3070_Linux_STA/os/linux] 错误 2

make[1]: Leaving directory `/home/lqm/share/G360/kernel_kfb'

make: *** [LINUX] 错误 2

[root@lqm RT3070_Linux_STA]#

第五步：make

如果编译一切正常，将会在/RT3070_Linux_STA/os/linux目录下生成rt3070sta.ko文件。

第六步：将USB WIFI模组插到开发板的USB HOST端口，将rt3070sta.ko文件复制到文件系统的/tmp目录，重启开发板，进入文件系统后加载驱动：

cd  /tmp

insmod  rt3070sta.ko

这时会提示如下信息：

root@freescale ~$ cd /tmp

root@freescale /tmp$ insmod rt3070sta.ko

rtusb init rt2870 --->





=== pAd = c49eb000, size = 509592 ===



<-- RTMPAllocTxRxRingMemory, Status=0

<-- RTMPAllocAdapterBlock, Status=0

usbcore: registered new interface driver rt2870

root@freescale /tmp$

第七步：配置无线网卡

首先检测开发板的网络设备：

ifconfig

可以看到这时只有eth0和lo两个网络信号，使用如下命令设置无线网卡的网络地址：

ifconfig ra0 192.168.1.5

打印信息如下：

root@freescale /tmp$ ifconfig ra0 192.168.1.5

(Efuse for 3062/3562/3572) Size=0x2d [2d0-2fc]

RTMP_TimerListAdd: add timer obj c4a32d88!

RTMP_TimerListAdd: add timer obj c4a32db4!

RTMP_TimerListAdd: add timer obj c4a32de0!

RTMP_TimerListAdd: add timer obj c4a32d5c!

RTMP_TimerListAdd: add timer obj c4a32cd8!

RTMP_TimerListAdd: add timer obj c4a32d04!

RTMP_TimerListAdd: add timer obj c49fd91c!

RTMP_TimerListAdd: add timer obj c49ece5c!

RTMP_TimerListAdd: add timer obj c49ece90!

RTMP_TimerListAdd: add timer obj c49fd9b4!

RTMP_TimerListAdd: add timer obj c49fd8c4!

RTMP_TimerListAdd: add timer obj c49fd984!

-->RTUSBVenderReset

<--RTUSBVenderReset

Key1Str is Invalid key length(0) or Type(0)

Key2Str is Invalid key length(0) or Type(0)

Key3Str is Invalid key length(0) or Type(0)

Key4Str is Invalid key length(0) or Type(0)

1. Phy Mode = 5

2. Phy Mode = 5

NVM is Efuse and its size =2d[2d0-2fc]

phy mode> Error! The chip does not support 5G band 5!

RTMPSetPhyMode: channel is out of range, use first channel=1

(Efuse for 3062/3562/3572) Size=0x2d [2d0-2fc]

3. Phy Mode = 9

MCS Set = ff 00 00 00 01

<==== rt28xx_init, Status=0

0x1300 = 00064300

root@freescale /tmp$

再使用ifconfig查看网络设备：

root@freescale /tmp$ ifconfig

eth0      Link encap:Ethernet  HWaddr 00:04:9F:00:EB:33 

          inet addr:172.18.2.133  Bcast:172.18.255.255  Mask:255.255.0.0

          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1

          RX packets:3182 errors:0 dropped:0 overruns:0 frame:0

          TX packets:1380 errors:0 dropped:0 overruns:0 carrier:0

          collisions:0 txqueuelen:1000

          RX bytes:3079209 (2.9 MiB)  TX bytes:227540 (222.2 KiB)

          Base address:0xa000



lo        Link encap:Local Loopback 

          inet addr:127.0.0.1  Mask:255.0.0.0

          UP LOOPBACK RUNNING  MTU:16436  Metric:1

          RX packets:0 errors:0 dropped:0 overruns:0 frame:0

          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0

          collisions:0 txqueuelen:0

          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)



ra0       Link encap:Ethernet  HWaddr 00:E0:61:37:D0:10 

          inet addr:192.168.1.5  Bcast:192.168.1.255  Mask:255.255.255.0

          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1

          RX packets:631 errors:0 dropped:0 overruns:0 frame:0

          TX packets:83 errors:0 dropped:0 overruns:0 carrier:0

          collisions:0 txqueuelen:1000

          RX bytes:92470 (90.3 KiB)  TX bytes:8736 (8.5 KiB)



root@freescale /tmp$

可以看到这时多了一组ra0设备，它就是无线网卡的相关信息。

第八步：下载并编译无线网络工具wireless_tools.29.tar.gz

tar  zxf  wireless_tools.29.tar.gz

cd  wireless_tools.29

修改makefile，修改部分如下：

## Compiler to use (modify this for cross compile).

CC = arm-none-linux-gnueabi-gcc

## Other tools you need to modify for cross compile (static lib only).

AR = arm-none-linux-gnueabi-ar

RANLIB = arm-none-linux-gnueabi-ranlib

修改后make，成功后会在当前目录生成iwspy，iwpriv，iwlist以及iwconfig等文件。我们只需将这四个文件拷备到NFS文件系统的sbin目录，然后将当前目录的libiw.so.29库文件拷到NFS文件系统的lib目录，无线网络工具即安装完成。

第九步：查看无线网络：

iwlist ra0 scanning

这时如果设备周边有无线信号，将会搜索出这些信号并打印出来，例如：

dBm

                    Encryption key:on

                    Bit Rates:54 Mb/s

                    IE: WPA Version 1

                        Group Cipher : TKIP

                        Pairwise Ciphers (2) : CCMP TKIP

                        Authentication Suites (1) : PSK

                    IE: IEEE 802.11i/WPA2 Version 1

                        Group Cipher : TKIP

                        Pairwise Ciphers (2) : CCMP TKIP

                        Authentication Suites (1) : PSK

          Cell 23 - Address: 00:25:5E:1E:38:5A

                    Protocol:802.11b/g

                    ESSID:"ChinaNet-UWfq"

                    Mode:Managed

                    Frequency:2.412 GHz (Channel 1)

                    Quality=26/100  Signal level=-79 dBm  Noise level=-74 dBm

                    Encryption key:on

                    Bit Rates:54 Mb/s

                    IE: WPA Version 1

                        Group Cipher : TKIP

                        Pairwise Ciphers (1) : TKIP

                        Authentication Suites (1) : PSK

                    IE: Unknown: DD0E0050F204104A0001101044000102

          Cell 24 - Address: 00:25:5E:1D:2F:FF

                    Protocol:802.11b/g

                    ESSID:"ChinaNet-eSpK"

                    Mode:Managed

                    Frequency:2.437 GHz (Channel 6)

                    Quality=42/100  Signal level=-73 dBm  Noise level=-75 dBm

                    Encryption key:on

                    Bit Rates:54 Mb/s

                    IE: WPA Version 1

                        Group Cipher : TKIP

                        Pairwise Ciphers (1) : TKIP

                        Authentication Suites (1) : PSK

                    IE: Unknown: DD0E0050F204104A0001101044000102



root@freescale /tmp$



第九步：下载并解压wpa_supplicant需要的源码

tar xvfz wpa_supplicant-0.7.2.tar.gz

tar xvfz wpa_supplicant-0.5.8.tar.gz

tar zxvf openssl-0.9.8e.tar.gz

第十步：编译openssl库

将wpa_supplicant中的补丁拷贝到openssl中：

cp  wpa_supplicant-0.7.2/patches/openssl-0.9.8e-tls-extensions.patch  openssl-0.9.8e/

建立openssl编译目标路径：

mkdir  /usr/local/ssl

修改openssl-0.9.8e目录下的makefile文件，修改内容如下：

CC= arm-none-linux-gnueabi-gcc

AR= arm-none-linux-gnueabi-ar

RANLIB= arm-none-linux-gnueabi-ranlib

INSTALLTOP=/usr/local/ssl

OPENSSLDIR=/usr/local/ssl

然后编译openssl:

make & make install

正常编译后，在/usr/local/ssl中将会出现ssl库。

第十一步：编译wpa_supplicant

cd  wpa_supplicant-0.5.8

cp  defconfig  .config

在.config中添加如下代码：

CC=arm-none-linux-gnueabi-gcc -L /usr/local/ssl/lib/

CFLAGS += -I/usr/local/ssl/include/

LIBS += -L/usr/local/ssl/lib/

然后编译:

make

正常情况下，将会在wpa_supplicant-0.5.8目录生成wpa_supplicant文件。将生成的文件拷备到文件系统的sbin目录

第十二步：建立配置文件wpa_supplicant.conf

在开发板文件系统的/etc目录下建立配置文件wpa_supplicant.conf，编辑内容如下：

# WPA-PSK/TKIP



ctrl_interface=/var/run/wpa_supplicant



network={

         ssid="XGD-kjy"

         scan_ssid=1

         key_mgmt=WPA-EAP WPA-PSK IEEE8021X NONE

         pairwise=TKIP CCMP

         group=CCMP TKIP WEP104 WEP40

         psk="xgdoko2008"

}

注意上面ssid要和无线路由的名字相同，psk为无线路由的密码。无线路由使用WPA或WPA2的加密方式。

在开发板终端建立无线网卡运行目录：

mkdir –p /var/run/wpa_supplicant

第十三步：执行wpa_supplicant

wpa_supplicant  -Dwext  -ira0  -c  /etc/wpa_supplicant.conf  -dd  &



这时PING路由的IP：

root@freescale /tmp$ ping 192.168.1.1

PING 192.168.1.1 (192.168.1.1): 56 data bytes

64 bytes from 192.168.1.1: seq=0 ttl=64 time=169.375 ms

64 bytes from 192.168.1.1: seq=1 ttl=64 time=16.678 ms

64 bytes from 192.168.1.1: seq=2 ttl=64 time=60.179 ms



--- 192.168.1.1 ping statistics ---

3 packets transmitted, 3 packets received, 0% packet loss

round-trip min/avg/max = 16.678/82.077/169.375 ms

root@freescale /tmp$

可见，已经能够PING通路由了。



from:http://blog.csdn.net/armeasy/article/details/6252389









