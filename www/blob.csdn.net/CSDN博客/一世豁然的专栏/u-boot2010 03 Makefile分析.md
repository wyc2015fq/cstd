# u-boot2010.03 Makefile分析 - 一世豁然的专栏 - CSDN博客





2015年06月24日 22:48:31[一世豁然](https://me.csdn.net/Explorer_day)阅读数：489标签：[bootloader																[u-boot](https://so.csdn.net/so/search/s.do?q=u-boot&t=blog)](https://so.csdn.net/so/search/s.do?q=bootloader&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








本博客转载于：[http://blog.csdn.net/yyttiao/article/details/7899530](http://blog.csdn.net/yyttiao/article/details/7899530)






对于Makefile 由于源码特别长，所以，我在这里就一边摘录一边分析



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- # Include autoconf.mk before config.mk so that the config options are available
- # to all top level build files. We need the dummy all: target to prevent the
- # dependency target in autoconf.mk.dep from being the default.
- all:  
- sinclude $(obj)include/autoconf.mk.dep  
- sinclude $(obj)include/autoconf.mk  
- 
- # load ARCH, BOARD, and CPU configuration
- # 该文件为配置时生成的
- include $(obj)include/config.mk  
- export  ARCH CPU BOARD VENDOR SOC  
- 
- # set default to nothing for native builds
- ifeq ($(HOSTARCH),$(ARCH))  
- CROSS_COMPILE ?= arm-linux-  
- endif  
- 
- # load other configuration
- include $(TOPDIR)/config.mk  


这段代码中，上面的解释已经很到位了.我这里就不多说了，直接看一个相对比较重要的文件**include $(TOPDIR)/config.mk**

不知道怎么分析好.就挑几个重点讲下.

1:编译器的变量声明

2:连接脚本

3:连接选项


1:编译器的变量声明



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- #
- # Include the make variables (CC, etc...)
- #
- AS       = $(CROSS_COMPILE)as  
- LD       = $(CROSS_COMPILE)ld  
- CC       = $(CROSS_COMPILE)gcc  
- CPP      = $(CC) -E  
- AR       = $(CROSS_COMPILE)ar  
- NM       = $(CROSS_COMPILE)nm  
- LDR      = $(CROSS_COMPILE)ldr  
- STRIP    = $(CROSS_COMPILE)strip  
- OBJCOPY  = $(CROSS_COMPILE)objcopy  
- OBJDUMP  = $(CROSS_COMPILE)objdump  
- RANLIB   = $(CROSS_COMPILE)RANLIB  


这里就知道为什么交叉编译器只需要指定前缀的原因了。


2:连接脚本



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- ifndef LDSCRIPT  
-     #LDSCRIPT := $(TOPDIR)/board/$(BOARDDIR)/u-boot.lds.debug
- ifeq ($(CONFIG_NAND_U_BOOT),y)  
-     LDSCRIPT := $(TOPDIR)/board/$(BOARDDIR)/u-boot-nand.lds  
- else
-     LDSCRIPT := $(TOPDIR)/board/$(BOARDDIR)/u-boot.lds  
- endif  
- endif  

**LDSCRIPT**会根据是否启用nand_boot的选项,来选择连接脚本,连接脚本可以指定代码生成的先后位置,比如把nand相关的函数指定到最前面.

uboot就是通过该连接脚本来使start.S这段代码放到整个程序的最前面,这样才能保证arm能正常启动


3:连接选项



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- LDFLAGS += -Bstatic -T $(obj)u-boot.lds $(PLATFORM_LDFLAGS)  
- ifneq ($(TEXT_BASE),)  
-     LDFLAGS += -Ttext $(TEXT_BASE)  
- endif  

**LDFLAGS**指明在连接的时候，指定连接地址,还有连接脚本的参数之类的.

剩下的就自己分析吧....






再回到Makefile,指定要编译的对象了



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- #########################################################################
- # U-Boot objects....order is important (i.e. start must be first)
- 
- OBJS = cpu/$(CPU)/start.o  
- ifeq ($(CPU),i386)  
-     OBJS += cpu/$(CPU)/start16.o  
-     OBJS += cpu/$(CPU)/resetvec.o  
- endif  
- ifeq ($(CPU),ppc4xx)  
-     OBJS += cpu/$(CPU)/resetvec.o  
- endif  
- ifeq ($(CPU),mpc85xx)  
-     OBJS += cpu/$(CPU)/resetvec.o  
- endif  
- 
- OBJS := $(addprefix $(obj),$(OBJS))  
- 
- LIBS = lib_generic/libgeneric.a  
- LIBS += lib_generic/lzma/liblzma.a  
- LIBS += lib_generic/lzo/liblzo.a  
- LIBS += $(shell if [ -f board/$(VENDOR)/common/Makefile ]; then echo \  
- "board/$(VENDOR)/common/lib$(VENDOR).a"; fi)  
- LIBS += cpu/$(CPU)/lib$(CPU).a  
- ifdef SOC  
-     LIBS += cpu/$(CPU)/$(SOC)/lib$(SOC).a  
- endif  
- ifeq ($(CPU),ixp)  
-     LIBS += cpu/ixp/npe/libnpe.a  
- endif  
- LIBS += lib_$(ARCH)/lib$(ARCH).a  
- LIBS += fs/cramfs/libcramfs.a fs/fat/libfat.a fs/fdos/libfdos.a fs/jffs2/libjffs2.a \  
- fs/reiserfs/libreiserfs.a fs/ext2/libext2fs.a fs/yaffs2/libyaffs2.a \  
- fs/ubifs/libubifs.a  
- LIBS += net/libnet.a  
- LIBS += disk/libdisk.a  
- LIBS += drivers/bios_emulator/libatibiosemu.a  
- LIBS += drivers/block/libblock.a  
- LIBS += drivers/dma/libdma.a  
- LIBS += drivers/fpga/libfpga.a  
- LIBS += drivers/gpio/libgpio.a  
- LIBS += drivers/hwmon/libhwmon.a  
- LIBS += drivers/i2c/libi2c.a  
- LIBS += drivers/input/libinput.a  
- LIBS += drivers/misc/libmisc.a  
- LIBS += drivers/mmc/libmmc.a  
- LIBS += drivers/mtd/libmtd.a  
- LIBS += drivers/mtd/nand/libnand.a  
- LIBS += drivers/mtd/onenand/libonenand.a  
- LIBS += drivers/mtd/ubi/libubi.a  
- LIBS += drivers/mtd/spi/libspi_flash.a  
- LIBS += drivers/net/libnet.a  
- LIBS += drivers/net/phy/libphy.a  
- LIBS += drivers/pci/libpci.a  
- LIBS += drivers/pcmcia/libpcmcia.a  
- LIBS += drivers/power/libpower.a  
- LIBS += drivers/spi/libspi.a  
- ifeq ($(CPU),mpc83xx)  
-     LIBS += drivers/qe/qe.a  
- endif  
- ifeq ($(CPU),mpc85xx)  
-     LIBS += drivers/qe/qe.a  
-     LIBS += cpu/mpc8xxx/ddr/libddr.a  
-     LIBS += cpu/mpc8xxx/lib8xxx.a  
- endif  
- ifeq ($(CPU),mpc86xx)  
-     LIBS += cpu/mpc8xxx/ddr/libddr.a  
-     LIBS += cpu/mpc8xxx/lib8xxx.a  
- endif  
- LIBS += drivers/rtc/librtc.a  
- LIBS += drivers/serial/libserial.a  
- LIBS += drivers/twserial/libtws.a  
- LIBS += drivers/usb/gadget/libusb_gadget.a  
- LIBS += drivers/usb/host/libusb_host.a  
- LIBS += drivers/usb/musb/libusb_musb.a  
- LIBS += drivers/usb/phy/libusb_phy.a  
- LIBS += drivers/video/libvideo.a  
- LIBS += drivers/watchdog/libwatchdog.a  
- LIBS += common/libcommon.a  
- LIBS += libfdt/libfdt.a  
- LIBS += api/libapi.a  
- LIBS += post/libpost.a  
- 
- LIBS := $(addprefix $(obj),$(LIBS))  
- .PHONY : $(LIBS) $(TIMESTAMP_FILE) $(VERSION_FILE)  
- 
- LIBBOARD = board/$(BOARDDIR)/lib$(BOARD).a  
- LIBBOARD := $(addprefix $(obj),$(LIBBOARD))  
- 
- # Add GCC lib
- ifdef USE_PRIVATE_LIBGCC  
- ifeq ("$(USE_PRIVATE_LIBGCC)", "yes")  
-     PLATFORM_LIBGCC = -L $(OBJTREE)/lib_$(ARCH) -lgcc  
- else
-     PLATFORM_LIBGCC = -L $(USE_PRIVATE_LIBGCC) -lgcc  
- endif  
- else
-     PLATFORM_LIBGCC = -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc  
- endif  
- PLATFORM_LIBS += $(PLATFORM_LIBGCC)  
- export PLATFORM_LIBS  
- 
- # Special flags for CPP when processing the linker script.
- # Pass the version down so we can handle backwards compatibility
- # on the fly.
- LDPPFLAGS += \  
- -include $(TOPDIR)/include/u-boot/u-boot.lds.h \  
- $(shell $(LD) --version | \  
- sed -ne 's/GNU ld version \..*/-DLD_MAJOR=\1 -DLD_MINOR=\2/p')  
- 
- ifeq ($(CONFIG_NAND_U_BOOT),y)  
-     NAND_SPL = nand_spl  
-     U_BOOT_NAND = $(obj)u-boot-nand.bin  
- endif  
- 
- ifeq ($(CONFIG_ONENAND_U_BOOT),y)  
-     ONENAND_IPL = onenand_ipl  
-     U_BOOT_ONENAND = $(obj)u-boot-onenand.bin  
-     ONENAND_BIN ?= $(obj)onenand_ipl/onenand-ipl-2k.bin  
- endif  
- 
- __OBJS := $(subst $(obj),,$(OBJS))  
- __LIBS := $(subst $(obj),,$(LIBS)) $(subst $(obj),,$(LIBBOARD))  
- 
- #########################################################################
- #########################################################################


由上面的可以看出
**OBJS **对应的是 .o文件
LIBS对应的是 .a文件
**LIBBOARD **对应的是目录

等等.主要看下这3个是怎么被处理的



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- $(OBJS):    depend  
-     $(MAKE) -C cpu/$(CPU) $(if $(REMOTE_BUILD),$@,$(notdir $@))  
- 
- $(LIBS):    depend $(SUBDIRS)  
-     $(MAKE) -C $(dir $(subst $(obj),,$@))  
- 
- $(LIBBOARD):    depend $(LIBS)  
-     $(MAKE) -C $(dir $(subst $(obj),,$@))  


由这里可以看出，会逐个的make出来


最后就是连接的过程了.连接会调用



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- GEN_UBOOT = \  
-     UNDEF_SYM=`$(OBJDUMP) -x $(LIBBOARD) $(LIBS) | \  
-     sed -n -e 's/.*/-u\1/p'|sort|uniq`;\  
-     cd $(LNDIR) && $(LD) $(LDFLAGS) $$UNDEF_SYM $(__OBJS) \  
-     --start-group $(__LIBS) --end-group $(PLATFORM_LIBS) \  
-     -Map u-boot.map -o u-boot  
- $(obj)u-boot:   depend $(SUBDIRS) $(OBJS) $(LIBBOARD) $(LIBS) $(LDSCRIPT) $(obj)u-boot.lds  


最终会连接成功.后面只要通过



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- $(obj)u-boot.bin:   $(obj)u-boot  
- $(OBJCOPY) ${OBJCFLAGS} -O binary $< $@  


将二进制内容拷贝出来，那么uboot.bin就生成了


来看一下连接脚本吧



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")  
- OUTPUT_ARCH(arm)  
- ENTRY(_start)  
- SECTIONS  
- {  
-     . = 0x00000000;  
-     . = ALIGN(4);  
-     .text :  
-     {  
-         cpu/arm1176/start.o (.text)  
-         cpu/arm1176/s3c64xx/cpu_init.o (.text)  
-        *(.text)  
-     }  
-     . = ALIGN(4);  
-     .rodata :   
-     {   
-         *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))   
-     }  
-     . = ALIGN(4);  
-     .data : {  
-         *(.data)   
-     }  
-     . = ALIGN(4);  
-     .got :   
-     {  
-         *(.got)   
-     }  
-     __u_boot_cmd_start = .;  
-     .u_boot_cmd :   
-     {   
-         *(.u_boot_cmd)   
-     }  
-     __u_boot_cmd_end = .;  
-     . = ALIGN(4);  
-     .mmudata :   
-     {   
-         *(.mmudata)   
-     }  
-     . = ALIGN(4);  
-     __bss_start = .;  
-     .bss :   
-     {   
-         *(.bss) . = ALIGN(4);   
-     }  
-     _end = .;  
- }  


从这个链接脚本可以看出，uboot将整个程序进行了如下的布置

.text :

.rodata : 

.data :

.got : 

.u_boot_cmd : 

.mmudata : 

.bss :

代码段被首先链接，在代码段内第一个就是start.s 因为这就是uboot启动的第一条代码的地方，看一下map文件

Memory Configuration


Name                  Origin                        Length                Attributes

*default*             0x00000000             0xffffffff


Linker script and memory map


0x00000000  . = 0x0

0x00000000 . = ALIGN (0x4)


.text                 0x57e00000                  0x27b8c

cpu/arm1176/start.o(.text)

.text                 0x57e00000                  0x440              cpu/arm1176/start.o

                        0x57e00040                                           _end_vect

                        0x57e0004c                                           _bss_start

                        0x57e00050                                          _bss_end

                        0x57e00048                                          _armboot_start

                        0x57e00114                                          copy_from_nand

                        0x57e00000                                          _start


从map文件中，可以看到0x57e00000=_start 被放到了一条的位置（0x57e00000就是链接脚本指定的连接地址）

接着打开cpu/arm1176/start.S



**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7899530#)[copy](http://blog.csdn.net/yyttiao/article/details/7899530#)



- .globl _start  
- _start: b   reset  
- #ifndef CONFIG_NAND_SPL
- ldr pc, _undefined_instruction  
- ldr pc, _software_interrupt  
- ldr pc, _prefetch_abort  
- ldr pc, _data_abort  
- ldr pc, _not_used  
- ldr pc, _irq  
- ldr pc, _fiq  


第一条语句 b reset就被放到了_start的位置，这样uboot启动之后，就会开始执行这条语句，uboot的移植也会从这开始分析了


完。


Thanks a lot！



[](http://blog.csdn.net/yyttiao/article/details/7899530#)[](http://blog.csdn.net/yyttiao/article/details/7899530#)[](http://blog.csdn.net/yyttiao/article/details/7899530#)[](http://blog.csdn.net/yyttiao/article/details/7899530#)[](http://blog.csdn.net/yyttiao/article/details/7899530#)[](http://blog.csdn.net/yyttiao/article/details/7899530#)
- 上一篇[u-boot2010.03
 配置编译目标分析](http://blog.csdn.net/yyttiao/article/details/7899008)
- 下一篇[u-boot2010.03
 第一阶段--start.S](http://blog.csdn.net/yyttiao/article/details/7916495)









