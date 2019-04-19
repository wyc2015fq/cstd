# DAVINCI DM365-DM368开发攻略——U-BOOT-2010.12及UBL的移植 - maopig的专栏 - CSDN博客
2012年12月15日 17:48:05[maopig](https://me.csdn.net/maopig)阅读数：2300
个人分类：[DM368](https://blog.csdn.net/maopig/article/category/919673)

    从盛夏走到深秋，我们继续DAVINCI DM365-DM368的开发。说来惭愧，人家51CTO热情支持本博客，而本人却一直没有像其他博客之星一样频繁更新博客，心里确实说不过去。管理公司确实很累，有更急的客户的项目要做，我们成功先推出了DM6446-810MHz的核心板(
[http://www.tongyetech.com](http://www.tongyetech.com/) )，期间深圳安防展也接见了几个其他市的客户，然后继续我们的DM368开发。
**一、DM36X的BOOT过程介绍**
DM36x的BOOT过程和DM6446、DM6467完全是一样的，因为都是ARM926EJS架构，里边都有一个RBL，这RBL在芯片出厂的时候都烧写在ROM里，芯片上电复位后RBL在运行，然后读取BOOT MODE引脚的电平状态，决定是从NAND FLASH还是从 MMC/SD、UART 、USB、 SPI、 EMAC、HPI中之一的方式BOOT，具体见SPRS558A.pdf和SPRUSG5A.pdf的介绍，这两个文档对硬件工程师、软件驱动工程师来说非常重要。
![](http://img1.51cto.com/attachment/201111/220843691.jpg)
本篇及以后的DM36X文章基本以DM368为主的介绍，因为DM365--- dvsdk_dm365_4_02_00_06和DM368----dvsdk_dm368_4_02_00_06的UBL，U-BOOT， KERNEL的源码完全是一样的，如果不考虑2A算法，DM368就是DM365的超频超出来的高清！
**二、DM36X 的UBL移植**
关于UBL的移植，本人打算单独写一篇文章的，但是UBL和UBOOT太紧密了，是TI davinci芯片的特点，所以放到一起讲。我们重点放在NAND BOOT的移植，这个UBL的版本是V1.50，在dvsdk_dm368_4_02_00_06\psp\flash-utils\DM36x和flash-utils\Common目录下，Common目录里有非常多东西，包括UBL的驱动源码、工具、脚本等等。我们主要关注arch，drivers，src，ubl。UBL的main（）函数在dvsdk_dm368_4_02_00_06\psp\flash-utils\Common\ubl\src\ubl.c里，这几个文件夹打开看看就明白什么意思了，这里不罗嗦。
DM36x下有CCS、Common，GNU三个文件夹：
**CCS文件夹：**
这里边的程序需要在TI CCS下编译，通过仿真器和JTAG在DM36X的板子上调试和烧写NAND FLASH或NOR FLASH，有烧写Writer的应用程序，用CCS打开工程文件，会连接到dvsdk_dm368_4_02_00_06\psp\flash-utils\Common\ drivers里。
**Common文件夹：**Common里有核心的文件device.c和device_nand.c。device.c是最重要的文件，这里初始化很多系统的东西，见DEVICE_init（）：
1、屏蔽所有中断；
2、清除中断标志；
3、DEVICE_PSCInit（），Power and Sleep Controller；
4、主芯片管脚复用的设置DEVICE_pinmuxControl（），DM36X的管脚复用很多，很复杂，一共5个PINMUX寄存器需配置；
5、DEVICE_PLL1Init（），PPL1配置，见SPRUSG5A.pdf的35页开始的介绍，使用不同的频率的DM36X，这些值都不同，不过TI已经提供参数参考，我们的DM365核心板是：ARM297_DDR243_OSC24，DM368核心板是：ARM432_DDR340_OSC24；
6、DEVICE_PLL2Init（），PPL2的配置，使用同上，不同频率的值不同；
7、DEVICE_DDR2Init（）的配置，市场上不同的DDR2内存芯片需要不同的参数配置，就在这个函数内。
8、DEVICE_EMIFInit（），这个针对NAND FLASH接口或NOR FLASH接口的访问时序配置；
9、DEVICE_UART0Init（）的配置，这个就是我们调试DM36X串口的设置，我们使用UART0来调试LINUX，这里配置不好，后面的开发不用调试了。
10、DEVICE_TIMER0Init（）定时器TIMER0的设置；
11、DEVICE_I2C0Init（）的设置；
**GNU的文件夹：**
这个就是在LINUX环境下编译UBL的环境，修改dvsdk_dm368_4_02_00_06\psp\flash-utils\DM36x\GNU\ubl下的makefile，把
#$(MAKE) -C build TYPE=nor注释掉，只保留$(MAKE) -C build TYPE=nand，然后是make clean和 make生产ubl_DM36x_nand.bin的文件；
如果出现：device.c:(.text+0x2ec): undefined reference to `__aeabi_uidiv'和device.c:(.text+0x2e8): undefined reference to `__aeabi_uidivmod'的BUG，就是DDR_Get_Val（）函数里边的除法和求余的语句和你的编译器不配置造成的。
result = ((parm * freq) / 10000) - 1;和if(((parm * freq) % 10000))，我们对参数的求值直接：
    tRFC = xxx; //DDR_Get_Val(DDR_T_RFC, DDR_FREQ);
    tRP = xxx; //DDR_Get_Val(DDR_T_RP, DDR_FREQ);
    tRCD = xxx; //DDR_Get_Val(DDR_T_RCD, DDR_FREQ);
    tWR = xxx; //DDR_Get_Val(DDR_T_WR, DDR_FREQ);
    tRAS = xxx; //DDR_Get_Val(DDR_T_RAS, DDR_FREQ);
    tRC = xxx; //DDR_Get_Val(DDR_T_RC, DDR_FREQ);
    tRRD = xxx; //DDR_Get_Val(DDR_T_RRD, DDR_FREQ);
    tWTR = xxx; //DDR_Get_Val(DDR_T_WTR, DDR_FREQ);
不用DDR_Get_Val()函数求值，这样就可以编译通过了。后来发觉TI的[http://e2e.ti.com/support/dsp/davinci_digital_media_processors/f/100/p/115574/425950.aspx](http://e2e.ti.com/support/dsp/davinci_digital_media_processors/f/100/p/115574/425950.aspx)里也有人这人解决，呵呵，没想到本人的解决办法和一个网友一样的。本人记得调试三星2440
 UBOOT的时候，有人点到过，这里本人忘记在什么地方改了。
**三、介绍u-boot-2010.12的特点**
u-boot-2010.12的架构组织越来越向LINUX架构靠拢，这是U-BOOT的发展趋势。DM36x的UBOOT源码放在dvsdk_dm368_4_02_00_06\psp的下面，文件夹叫u-boot-2010.12-rc2-psp03.01.01.39，太长了，我们直接u-boot-2010.12。这个u-boot-2010.12支持YAFFS2、UBI等文件系统，支持SD卡、USB、VIDEO等驱动，支持lzma和lzo的内核镜像压缩，还有经典的linux shell命令支持模式，就是tab等的妙用，就是在”U-boot
 >”提示符下键入和LINUX 开发主机上一样的命令，当然这个命令很少，而且有些命令是用户自己加的。相对以前的u-boot-1.3.4版本等版本，这个新U-BOOT把很多平台的配置脚本给去掉了。
**四、u-boot-2010.12的移植**
DAVINCI芯片系列的UBOOT移植大同小异，比较简单。
1、首先修改：Makefile:
第140行:#examples/standalone \
第141行:#examples/api
第246行: #LIBS += api/libapi.o
等一下我们就可以把一些不相关的文件夹给删除掉，给u-boot-2010.12瘦身。
第323行：ALL += $(obj)u-boot.srec $(obj)u-boot.bin $(obj)System.map $(U_BOOT_NAND) $(U_BOOT_ONENAND)
u-boot.img
后面增加u-boot.img，这个u-boot.img就是我们要烧写到NAND里边的BIN文件，可以被UBL给BOOT起来的文件，因为u-boot.img是通过u-boot-2010.12\tools下的mkimage的工具生产的，带有头header的文件，这些头信息能被UBL给识别，所以才能烧写到NAND FLASH或NOR FLASH里边去。而u-boot.bin是没有增加文件头部信息的文件，不能被UBL直接给BOOT起来。u-boot.bin只能使用软件BOOT，比如: tftp 0x82080000
 u-boot.bin ;go 0x82080000。
最重要的地方修改：**第**350行：-a $(CONFIG_SYS_TEXT_BASE) -e 0x81080000 \****
**或者-a $(CONFIG_SYS_TEXT_BASE) -e $(CONFIG_SYS_TEXT_BASE)\**
**刚开始这个-e 0是错误的，我们把U-BOOT-2010.12的entrypoint定义到0x81080000，这个DDR的地址是在u-boot-2010.12\board\davinci\dm365evm\config.mk**
**CONFIG_SYS_TEXT_BASE = 0x81080000**
**这个-e(entrypoint)不能是0，**否则无法被UBL给BOOT起来。
第953行是不需要改动的：
davinci_dm365evm_config : unconfig
       @$(MKCONFIG) $(@:_config=) arm arm926ejs dm365evm davinci davinci
DM368也是使用同样的dm365evm名字。
2、开始删除一些不相关的文件夹
u-boot-2010.12\api文件夹
u-boot-2010.12\examples
u-boot-2010.12\nand_spl
u-boot-2010.12\onenand_ipl
u-boot-2010.12\arch\里的只保留arm文件夹
u-boot-2010.12\ arch\arm\cpu文件夹里，只保留arm926ejs
u-boot-2010.12\ arch\arm\cpu\arm926ejs文件夹里，只保留davinci文件夹，我们只删除文件夹，旁边的cpu.c和Makefile不要删除，等等；
u-boot-2010.12\board里边，只保留davinci文件夹，其他统统删除掉；
u-boot-2010.12\board\davinci\里，只保留dm365evm和common，其他删除；
u-boot-2010.12\include\configs里，只保留davinci_dm365evm.h，其他文件和文件夹删除掉；
通过上面的操作，我们的u-boot-2010.12简化多了。
3、修改最重要的davinci_dm365evm.h
在u-boot-2010.12\include\configs里，davinci_dm365evm.h
修改#define CONFIG_SYS_NAND_MAX_CHIPS    1，把以前的2改成1，表示本系统只支持1片NAND FLASH；
把#define CONFIG_BOOTCOMMAND      "if mmc rescan 0; then if fatload mmc 0 0x80600000 boot.scr; then source 0x80600000; else fatload mmc 0 0x80700000 uImage; bootm 80700000; fi; fi"给注释掉，我们不想用SD卡保存内核的BIN文件uImage；
然后定义#define CONFIG_BOOTCOMMAND "run nand_boot"，表示从NAND FLASH 读取内核；
修改UBOOT的BOOTARGS参数：#define CONFIG_BOOTARGS \
              "console=ttyS0,115200n8 mem=60M noinitrd rw ip=192.168.1.188:192.168.1.252:192.168.1.1:255.255.255.0 root=/dev/nfs nfsroot=192.168.1.252:/home/davinci/dm368/dvsdk_dm368_4_02_00_06/filesystem/dm368rootfs,nolock"
我们这里提到mem=60M，是有根据的，我们板子是128M-BTYE，因为后面的DVSDK4.02里边的encodedecode等例子需要用到CMEM，这个和DM6446类似，所以定义为60M。
见dvsdk_dm368_4_02_00_06\filesystem\dvsdk-dm368-evm-rootfs解压后的etc\init.d\loadmodule-rc文件：
# Default DM368 EVM Memory Map
#
# Start Addr    Size    Description
# -------------------------------------------
# 0x00001000    32K     ARM TCM memory
# 0x80000000    48 MB   Linux
# 0x83000000    12 MB   Video driver memory (Linux)
# 0x83C00000    68 MB   CMEM
# 0x88000000    BOTTOM  ADDRESS
load () {
    modprobe cmemk phys_start=0x83C00000 phys_end=0x88000000 allowOverlap=1 phys_start_1=0x00001000 phys_end_1=0x00008000 pools_1=1x28672 useHeapIfPoolUnavailable=1
    modprobe irqk 
    modprobe edmak
    modprobe dm365mmap
    mknod /dev/dm365mmap c `awk "\\$2==\"dm365mmap\" {print
\\$1}" /proc/devices` 0
}
有些人定义CMEM共享内存小一点的话，那么这个值可以比60M大。后面的参数就是使用静态固定IP的NFS SERVER路径，进行NFS调试；
增加
#define CONFIG_IPADDR       192.168.1.188
#define CONFIG_SERVERIP     192.168.1.252
#define CONFIG_ETHADDR       00:03:55:88:00:00
定义板子自己的IP和MAC，这个MAC在保证和其他网络设备MAC不冲突的前提下，自己调试的时候顺便分配。SERVERIP就是我们LINUX开发环境的主机IP地址，主要用做TFTP和NFS调试。
#define CONFIG_EXTRA_ENV_SETTINGS     \
"eraseenv=nand erase 0x00060000 0x00020000\0" \
"eraseall=nand erase 0x00000000 0x08000000\0" \
"updateuboot=tftp 0x84000000 dm368_uboot.bin;nand erase 80000 80000;nand write 84000000 80000 80000\0" \
"updatecore=tftp 0x84000000 dm368_kernel.bin;nand erase 0x00100000 0x00480000;nand write 84000000 100000 300000\0" \
"nand_boot=nboot 0x80700000 0 0x100000;bootm\0" \
"tftp_boot=tftpboot 0x80700000 dm368_kernel.bin;bootm\0"
这个很精典，我以前在DM6446上面U-BOOT-2009.03的移植上说过，在u-boot->的提示下，使用run 命令，就可以批量自动做一些操作。CONFIG_EXTRA_ENV_SETTINGS这个在u-boot-2010.12\common\env_common.c 和u-boot-2010.12\common\env_embedded.c有使用。你尽管在这里定义就可以了。
run eraseenv表示擦除u-boot的参数；
run eraseall表示擦除整片NAND；
run updateuboot表示通过TFTP烧写dm368_uboot.bin到板子上；
run updatecore表示烧写内核文件，300000表示内核BIN的大小，我们这里直接定死，3M的内核BIN文件长度够用了，当然，你可以根据内核大小的动态长度改变烧写方法；
TFTP先下载BIN文件(uImage)到DDR 的0x84000000地址，然后烧写到自己定义的分区上，这个和内核linux-2.6.32.17的分区是一一对应的，这点很重要；
"nand_boot=nboot 0x80700000 0 0x100000;bootm\0"；内核放在NAND的0x100000，我们使用run nand_boot进行板级内核启动；
"tftp_boot=tftpboot 0x80700000 dm368_kernel.bin;bootm\0"使用TFTP进行内核软件启动，编译调试内核；
在run eraseenv中，我们使用0x00060000作为U-BOOT参数存放的偏移地址，长度使用128K-BYTE，所以在第183行处开始：
#ifdef CONFIG_NAND_DAVINCI
#define CONFIG_ENV_SIZE  **(128 << 10) **/* 128KiB */  //128K-byte for evn
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OFFSET **0x00060000**#undef CONFIG_ENV_IS_IN_FLASH
#endif
顺便说一下，我们一般不建议在U-BOOT里边通过TFTP下载和烧写至少几十M的文件系统，我们尽量简化U-BOOT的功能，因为我们产品的重点是在LINUX内核，所有的开发、应用、升级程序都是基于LINUX内核上的，所以我们建议通过LINUX内核驱动来烧写文件系统，这是后话。
4、使能使用tab键功能
这个功能就是能在U-boot->的命令提示符下使用tab键，所以说
在davinci_dm365evm.h里一定要在“#define CONFIG_SYS_HUSH_PARSER”的上一行或下一行定义CONFIG_AUTO_COMPLETE；
在u-boot-2010.12\common\command.c里，第165行，把“#if 0”改成“#if 1”；
在u-boot-2010.12\common\env_common.c里，第247行把“#if 0”改成“#if 1”；
5、增加GPIO驱动的功能和修改一些BUG
复位外围芯片，比如采用GPIO控制网口芯片复位，在u-boot-2010.12\board\davinci\dm365evm\dm365evm.c的board_init()函数，增加GPIO的复位驱动，对一些外围的芯片复位一下；
在u-boot-2010.12\common\cmd_nvedit.c里，第227行，把“((strcmp (name, "ethaddr") == 0)”用“(0”，这样就能任意修改保存在网口MAC地址ethaddr；
6、开始编译
在u-boot-2010.12\arch\arm的config.mk里
#CROSS_COMPILE ?= arm-linux-
**CROSS_COMPILE = arm-none-linux-gnueabi-**
我们使用Code Sourcery ARM GCC Tool Chain的Sourcery G++ Lite 2009q1-203 for ARM GNU/Linux。
注意，除了UBL在UBL的文件夹内部编译外，其他的DVSDK的文件包u-boot，linux，dm365mm，irq ，edma，ceexamples，dmai，还有psp_examples，等等，都是在dvsdk_dm368_4_02_00_06的目录下编译，具体见dvsdk_dm368_4_02_00_06的Makefile。
u-boot的编译是:make u-boot，make u-boot_clean等等；
我们也可以在这个目录下自己生产一个build_uboot.sh的命令
#build u-boot in dvsdk4_02
make u-boot
chmod 777 /home/davinci/dm368/dvsdk_dm368_4_02_00_06/psp/u-boot-2010.12/u-boot.img
cp -f /home/davinci/dm368/dvsdk_dm368_4_02_00_06/psp/u-boot-2010.12/u-boot.img /tftpboot/dm368_uboot.bin
或者再增加build_uboot_all.sh
#build u-boot in dvsdk4_02
make u-boot_clean
make u-boot
chmod 777 /home/davinci/dm368/dvsdk_dm368_4_02_00_06/psp/u-boot-2010.12/u-boot.img
cp -f /home/davinci/dm368/dvsdk_dm368_4_02_00_06/psp/u-boot-2010.12/u-boot.img /tftpboot/dm368_uboot.bin
使用sh文件帮你做工，要多养成这样的习惯。
基本上，你的板子网口OK的话，可以通过TFTP把UBOOT烧进去吧，通过仿真器或者其他方式烧写，我们公司一般不用仿真器烧写，直接使用uart boot的方式烧写，自己写个uart boot的东西，好好看看dvsdk_dm368_4_02_00_06\psp\flash-utils的包里的东西，这里就保留吧。
到这里，整个u-boot-2010.12移植基本完成。有不足的地方还需其他网友指正。本人维护两个主芯片平台后，变得非常忙，但本人会坚持写下去，下一篇是linux-2.6.32.17的移植，以前写过三篇有关UBOOT的文章，包括S3C2440，本人的目的让大家缩短开发这些BOOT开发的时间，因为重点的开发是在内核和基于内核的应用程序（信息产业部也大力支持开源的linux及相关技术），让大家的音视频网络产品快点出来，改善人民的生活，推进这个社会的文明，文明是靠科技来推动的，让那些丑陋的、麻木缺德的、文明倒退的野蛮行为、愚民手段、空洞扭曲、侮辱国人智商的事情尽快暴露出来，碰到棘手的问题也能暴露出来，毕竟广大人民的智慧是无穷无尽的，是完全有能力解决问题的。但前提是我们要有这样的科技学术氛围，身上没有极其沉重的衣食住行的枷锁，也没把绝大部分时间浪费在外语和XX课上（看看人家德国吧），所以我们要坚持我们的技术之路。文明是靠科技来推动的，而科技需要我们千千万万的科技人员踏踏实实去动手做事的。
昨天才调试同TVP5158 D1的采集，终于看到图像了。心里很舒畅。顺便晒晒我们这段时间做的两款核心板：DM368和DM6446-810MHz核心板。核心板以最大限度方便客户进行功能扩展、集成应用，加快产品研发和上市，降低前期产品研发的风险。我们比较倾向项目合作，开发板不是我们开发的重点。
![](http://img1.51cto.com/attachment/201111/221217864.jpg)
**DM368核心板**
DM368核心板硬件接口描述：
■板载存储器：
  - 动态存储器（SDRAM DDR2）：1Gb(128M-byte)
  - 非易失存储器（NAND FLASH）：1Gb(128M-byte)
■板载接口：
  - 1个4位Boot模式选择接口
  - 16-BIT YCC数字视频输入接口
  - 2路可编程输出时钟CLOCK0和CLOCK2
  - 1路模拟标清视频输出接口TVOUT
  - 1路模拟高清视频输出接口YPbPr
  - 1路麦克风差分输入接口
  - 1路音频线性输出接口
  - 1路喇叭差分输出 
  - 3路AD采集接口
  - 1个SD卡接口SD0
  - 1个10/100M网络差分输入输出接口RX和TX
  - 2个RS232串口(UART0，UART1) 
  - 1个USB 2.0接口(HOST/DEVICE/OTG)
  - 1个 SPI口SPI1 
  - 1个标准 JTAG接口
  - 1个4×4键盘接口
 - 18个独立GPIO口
  - 3路电源输入接口+1.8V,+3.3V,+1.2V
■单板尺寸：
  46mm x 46mm x 9.5mm
其中整体高度是9.5mm
![](http://img1.51cto.com/attachment/201111/221002613.jpg)
**DM6446-810MHz核心板**
DM6446-810MHz核心板采用的是TI TMD320DM6446BZWT8的双核芯片，ARM9的频率达到405MHz，DSP-C64+核的频率达到810MHz，DDR2-667操作频率配置为189MHz，NAND FLASH为128M-BYTE。集成10M/100M网络芯片在核心板上，方便客户设计底板，70mm*47mm，整体高度9mm，比名片更小。
桐烨科技DM6446-810MHz核心板支持硬件接口：
1、VPFE16位总线及H、V信号，完全可以接视频采集芯片和CMOS SENSOR；
2、VPBE引出RGB24的控制信号，便于接RGB24的LCD屏；
3、CVBS模拟视频输出；
4、网口信号及网口LED、LINK信号；
5、I2C接口；
6、SPI接口；
7、SD卡接口；
8、ATA硬盘接口；
9、USB2.0 HOST接口；
10、ASP音频接口；
11、3个UART接口；
12、18个独立GPIO接口及其他复用的GPIO接口；
13、JTAG仿真器接口；
14、3个PWM控制线；
15、CLK_OUT0和CLK_OUT1可编程时钟输出信号；
16、TI的H264  ENCODE   LIB达到720P（1280*720）的实时；
本文出自 “[集成系统-踏上文明的征程](http://zjbintsystem.blog.51cto.com/)” 博客，请务必保留此出处[http://zjbintsystem.blog.51cto.com/964211/713240](http://zjbintsystem.blog.51cto.com/964211/713240)
