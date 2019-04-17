# hi35183e增加exfat文件系统的支持 - DoubleLi - 博客园






64G-128G的tf卡文件系统格式为exfat，而hi3518e默认只支持fat32格式的tf卡。为了挂载64G以上的tf卡，只能将sd卡先格式化成FAT32。鉴于exfat性能比FAT32强，因此考虑移植exfat驱动到海思3518e平台,这样就不用强制格式化tf卡。

### 拷贝驱动源码到内核

# cp /home/dyx/exfat /home/dyx/sdk_hi3518e/Hi3518_SDK_V1.0.A.0/osdrv/kernel/linux-3.0.y/fs/exfat -frv

### 切换路径至内核编译顶层路径

# cd /home/dyx/sdk_hi3518e/Hi3518_SDK_V1.0.A.0/osdrv/kernel/linux-3.0.y

### 修改fs/Kconfig

# vi ./fs/Kconfig



if BLOCK
menu "DOS/FAT/NT Filesystems"


source "fs/fat/Kconfig"
source "fs/exfat/Kconfig"

source "fs/ntfs/Kconfig"

### 修改fs/Makefile

# vi ./fs/Makefile

obj-$(CONFIG_MINIX_FS)+= minix/
obj-$(CONFIG_FAT_FS) += fat/
obj-$(CONFIG_EXFAT_FS)  += exfat/

### exfat层makefile示例



**[plain]**[view plain](http://blog.csdn.net/u013286409/article/details/50519313#)[copy](http://blog.csdn.net/u013286409/article/details/50519313#)

[print](http://blog.csdn.net/u013286409/article/details/50519313#)[?](http://blog.csdn.net/u013286409/article/details/50519313#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <span style="font-size:14px;">#  
- # Makefile for the Linux fat filesystem support.  
- #  
- 
- obj-$(CONFIG_EXFAT_FS) += exfat.o  
- 
- exfat-y := exfat_core.o exfat_super.o exfat_api.o exfat_blkdev.o exfat_cache.o \  
-                exfat_data.o exfat_bitmap.o exfat_nls.o exfat_oal.o exfat_upcase.o</span></span></span>  




### menuconfig配置

# make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- menuconfig

进入图形化配置界面

File systems  ---> 

DOS/FAT/NT Filesystems  --->

<*> exFAT fs support                                                                                               
[*]   enable discard support                                                                                       
[*]   enable delayed sync                                                                                          
[ ]   enable kernel debug features via ioctl                                                                       
[ ]   print debug messages                                                                                         
(437) Default codepage for exFAT                                                                                   
(utf8) Default iocharset for exFAT 

保存退出

内核编译
# make ARCH=arm CROSS_COMPILE=arm-hisiv100nptl-linux- uImage

### 挂载

mount -t exfat /dev/mmcblk0p1   /media/

或者可以不指定挂载的文件系统类型，让系统自动识别

mount /dev/mmcblk0p1   /media/

[EXFAT] trying to mount...
[EXFAT] mounted successfully

# df -h
Filesystem                Size      Used Available Use% Mounted on
/dev/root                12.0M      4.6M      7.4M  38% /
tmpfs                    16.1M      4.0K     16.1M   0% /dev
/dev/mmcblk0p1          117.0G    512.0K    117.0G   0% /mnt









