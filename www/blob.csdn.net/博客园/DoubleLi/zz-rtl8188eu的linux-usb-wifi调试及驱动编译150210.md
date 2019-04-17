# zz-rtl8188eu的linux-usb-wifi调试及驱动编译150210 - DoubleLi - 博客园






//zz//#######################################################################

zz-rtl8188eu的linux-usb-wifi调试及驱动编译150210

zz-Write:
    @2015-2-4 22:11:14
    @2015-2-11 00:32:07
    @

REF:
    嵌入式linux usb wifi移植参考文档.pdf
        嵌入式linux usb wifi移植.pdf
        http://blog.chinaunix.net/uid-8048969-id-3251135.html

    RTL8188EUS_linux_v4.3.0.7_12758.20141114.zip
        ftp3.realtek.com.tw (RTL8188E,M6b3Er)

KeyWord:

    以 ti-am335x 为例,使用 ti-sdk 6.0 平台内核及交叉编译器工具链

//zz//#######################################################################
0.
USB 硬件调试

1)
内核代码中, am335x 需要将接rtl8188eu的 USB0 口设置为 HOST 模式;
设为 OTG 主从可切换模式会无法检测到USB外设
原因是
    am335x 外接到 rtl8188eu 模块的USB口使用的4根线,没有引出 id 线到USB端口
    虽然am335x 的id线下拉为低电平,是为 OTG 之主机模式
    *但是 OTG 还需要用到 VBUS 线(即USB的VCC5V脚),这根线 OTG 检测主从要用到
        若 VBUS 检测到无电压,表示对方是B端设备,再通过使能脚给VBUS上电VCC5V
        若 VBUS 检测到5V,表示对方是A端(USB主机),本机为B端设备
    此板子是 USB0_VBUS 直接接 VCC5V ,所以无论插电脑还是U盘或rtl8188e,AM335X的OTG侦测结果都是自己为设备.

2)
USB0 口开机就接上 rtl8188 模块
提示枚举错误,各种 -74 错误号
参看内核得知是协议错误 => 时序错误 => D+ D- 数据线的走线,串接电阻,电容不对
解决办法:
    USB 差分高速信号,抵消反射处理,阻抗匹配可能不对
    去掉两个串接的 33ohm ,去掉两个100pf左右的电容

3)
USB0 口rtl8188e 模块出现断电后再上电(重启),主机无法检测到设备的问题
    USB协议提到,USB设备的 D+ D- 下拉了15K电阻,D+ 上拉1.5K电阻表示此设备为全速/高速
        USB2.0 全速 12Mbps,高速最高 480Mbps
    而 D- 上拉接1.5K电阻表示此设备只支持USB低速模式(USB1.0设备)

问题分析:
    因为是高速设备,用万用表测量 D+ 对地电平
    rtl8188e 模块在 am335x 整个开机过程中,先为3.0V左右高电平,枚举后因设备内部断开上拉电阻以作全速/高速通信,测得电平变为0.3左右
    此时再将rtl8188e 模块断电,再上电,发现整个过程中 D+ 脚对地电平都为 0.3V左右的低电平

结果:
    后面发现此情况为偶尔发生,正常情况下,USB设备断电再上电,我用万用表都能测到D+脚有个高电平,
    主机识别设备的速度的时间很短,只看到电压为1.6V左右..后续分析使用示波器吧

    *飞凌 OK335xD 说的需要开机后再插入 USB设备和观测到的现象不符
    实际是偶尔发生开机后供电rtl8188e无法识别到,而开机就供电反而一定能是被到,U盘开机就插好也能侦测到

4)
rtl8188e usb 模块使用 IO 口无法控制其VBUS接上电源
原因:
    NMOS 管使用错误
    其实 MOS 管内部有四个级,源级S 漏极D 栅极G 衬底级B
    常用 NMOS 管只有3个引脚,是因为源级S 与 衬底级B 连在一起的
    要使NMOS管D S 导通,需要在栅极G加高电平,而且是G 相对于S(B) 为高电平
    此处 D 接VCC S接VBUS 栅极G就算为高电平,也无法将 D S 连通,因为 G 与 B之间电压不够使 MOS 管中 P区的少子将两个N沟道导通
解决方法:
    将 S 接到 GND ,模块的GND脚与 NMOS 管的 D 级相连


//zz//#######################################################################
1.
驱动包中各目录说明,有driver,还有 wpa_supplicant hostapd 等应用层工具代码的
解压驱动包
rtl8188eus_linux_v4.3_141114
    driver/rtl8188eus_linux_v4.3.0.7drv
        驱动程序,编译得到 8188eu.ko

    wpa_supplicant_hostapd/wpa_supplicant_hostapd-0.8
        连接wifi用的客户端程序 wpa_supplicant wpa_cli等;AP热点程序 hostapd


    wireless_tools/wireless_tools.30.rtl
        查看wifi信号强度,速度,频段等信息用的 iwconfig iwlist 程序
        基于 /proc/net/wireless


//zz//#######################################################################
2.
驱动程序/内核模块 8188eu.ko 交叉编译移植
driver/rtl8188eus_linux_v4.3.0.7drv
    驱动程序,编译得到 8188eu.ko

确定芯片选中的是 8188E
    CONFIG_RTL8192C = n
    CONFIG_RTL8188E = y


修改 Makefile
1)
仿照 CONFIG_PLATFORM_ARM_S3C2K4 添加一个 ti am335x 的选项
注释掉 I386 电脑用的选项
添加 ARM_AM335X 选项

#zz# @dell @2015-0204 begin
#zz# CONFIG_PLATFORM_I386_PC = y
CONFIG_PLATFORM_ARM_AM335X = y
#zz# @dell @2015-0204 end

2)
仿照 CONFIG_PLATFORM_ARM_S3C2K4 添加如下这段
设置小端格式,设置内核代码路径等等

# @home @2015-0204 begin
ifeq ($(CONFIG_PLATFORM_ARM_AM335X), y)
EXTRA_CFLAGS += -DCONFIG_LITTLE_ENDIAN
ARCH := arm
CROSS_COMPILE := arm-linux-
KVER  := 3.2.0
KSRC := /home/am335x-tisdk6.0/linux-3.2.0-psp04.06ti-zz
endif

# @home @2015-0204 end

################################################
3)
去掉内核模块,驱动8188eu.ko中的调试输出信息
大部分调试信息是通过如下宏定义函数数出来的,使他们输出为空就行
    DBG_8192C
    MSG_8192C
    DBG_817X
    DBG_871X_LEVEL

在 include/autoconf.h 文件中
注释掉 CONFIG_DEBUG 宏定义,以减少不必要的调试信息输出


//zz// Too much debug info, Reduce something 
//  @dell @2015-02-04
//zz// #define DBG  1
#define DBG 0

/* DBG_871X, etc... */
//zz// #define CONFIG_DEBUG


在 rtw_debug.h 中
将 _dbgdump _seqdump 两个宏定义设置为空的(直接去掉会出错的,函数未定义)
这样调试信息就大部分不会有了,8188eu.ko也变小了
#elif defined PLATFORM_LINUX
    #define _dbgdump    //
    #define _seqdump    //

4)
执行 make 命令,即可编译得到 am335x 平台的 8188eu.ko 驱动模块

//zz//#######################################################################
3.
交叉编译 wpa_supplicant wpa_cli 程序,交叉编译 hostapd hostapd_cli 等程序
wpa_supplicant_hostapd/wpa_supplicant_hostapd-0.8
    连接wifi用的客户端程序 wpa_supplicant wpa_cli等
    AP热点程序 hostapd

#################
1)
wpa_supplicant/Makefile


设置编译器为交叉编译器

#zz############################
# zz @dell @2015-0204 begin
CC = arm-linux-gcc
# zz @dell @2015-0204 end


ifndef CC
CC=gcc
endif


在 src/lib.rules 中,最好也将编译器 CC 设置为交叉编译器
#zz############################
# zz @dell @2015-0204 begin
CC = arm-linux-gcc
# zz @dell @2015-0204 end

ifndef CC
CC=gcc
endif

#################
2)
hostapd/Makefile 中也是这样添加 CC 为交叉编译器

#zz############################
# zz @dell @2015-0204 begin
CC = arm-linux-gcc
# zz @dell @2015-0204 end

ifndef CC
CC=gcc
endif

//zz//#######################################################################
4.
交叉编译 iwlist iwconfig 等程序
wireless_tools/wireless_tools.30.rtl
    查看wifi信号强度,速度,频段等信息用的 iwconfig iwlist 程序
    基于 /proc/net/wireless


这里需要修改 Makefile 中 CC AR RANLIB 为交叉编译工具
同时 PREFIX 如果要安装的话,修改下路径,如下改为 _install 目录


#zz###############################################
# zz @dell @2015-0204 begin

PREFIX = _install

ifndef PREFIX
  PREFIX = /usr/local
endif

## Compiler to use (modify this for cross compile).

#zz# CC = gcc
CC = arm-linux-gcc

## Other tools you need to modify for cross compile (static lib only).

#zz# AR = ar
#zz# RANLIB = ranlib

AR = arm-linux-ar
RANLIB = arm-linux-ranlib

# zz @dell @2015-0204 end

#zz###############################################









