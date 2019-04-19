# 在Niosii上跑NiosIILinux！ - 三少GG - CSDN博客
2010年05月03日 01:04:00[三少GG](https://me.csdn.net/scut1135)阅读数：2646
*************************************************************************************************************
**  Project          :   Linux BSP for Embedded System Development Kit Cyclone III (3C120)
**  File Name      :   Linux_3c120_README.txt
**  Last Updated  :  4th August 2009
**  Author           :  System Level Solutions
*************************************************************************************************************
Quick Start Guide
--------------------------
We assume that you are acquainted with Linux enviornment.
1.  Install the following development packages at host side by giving following command.
**#apt-get install git-all git-gui tcsh make gcc ncurses-devel**bison byacc flex gawk gettext ccache zlib-devel gtk2-devel lzo-devel pax-utils
2.  Download the Linux BSP source (Nios2Linux-20090804-0.0.0.0.tar.bz2) and quick reference
    ( Quick_ref-20090805-0.0.0.0.tar.bz2) from http://www.slscorp.com/pages/bsp3c120.php
    Size      :        508.44 MB
     Source   :        Nios2Linux-20090804-0.0.0.0.tar.bz2
    MD5SUM :        4c54f86ef036591a53d18ac8bf6c7bf9
    Size      :        3.67 MB
     Source   :        Quick_ref-20090805-0.0.0.0.tar.bz2
    MD5SUM :        adee59e8100e3d113e40ed5fed529f4b
3.  Copy the BSP source 'Nios2Linux-20090804-0.0.0.0.tar.bz2' at the development folder on your
    linux PC and extract it.
     Ex:
         #cd /home/sls/
**   #tar -jxvf Nios2Linux-20090804-0.0.0.0.tar.bz2**
      The 'Nios2-Linux' folder will be created. It contains following three folders.
    BuildTools      :  Prebuild bin tools gcc 1.1.2 for nios2-linux
    Linux_source  :  Kernel and Application
    System-Board :  System file for specific board. It contains only for 3c120 board files
4.  Set the Bintools path on your terminal.
     Ex:
         #PATH=$PATH:/home/sls/Nios2-Linux/BinTools/toolchain-mmu/x86-linux2/bin
**PATH=$PATH:/home/grant/Nios2-Linux/BuildTools/toolchain-mmu/x86-linux2/bin**        (每次打开终端都要添加一次）  
查看：echo $PATH          
**sudo ln -s /lib/libncurses.so.5 /usr/lib/libtinfo.so.5    **               ---只需要这么一句话！原因参考下文转载
注意！：
　　　　　# nios2-wrs-linux-gnu-gcc hello.c -o hello                ---交叉编译
Step 1：
所有要**打包成image的檔案，都會放在/home/grant/Nios2-Linux/Linux_source/uClinux-dist/romfs下**，也就是說，我們只要將執行檔、函式庫、圖片....放在這個目錄下，將來就會打包成zImage下載到开发板的SDRAM內。
(或者將hello_world_linux放到/uClinux-dist/romfs/usr/bin下
类似[root@localhost src]# cp hello_world_uclinux /usr/local/src/uClinux-dist/romfs/usr/bin )
5.  Build the Linux image.
cd /home/grant/Nios2-Linux/Linux_source/uClinux-dist/
**   #make menuconfig       #make**
      After compilation, you will get different images in the image folder located at:
      /home/sls/Nios2-Linux/Linux_source/uClinux-dist/image/
      The linux.initramfs.gz file is an elf image with initramfs, built-in with u-boot images
      (images/vmImage and rootfs.initramfs.gz).
6.  Download the .elf and .sof files and then run the elf file.
       (a)  Download the sof file 'nios2_linux_3c120_125mhz_top.sof' located at 
             /home/sls/Nios2-Linux/System-Board/3c120_default/
       (b)  Download elf file 'linux.initramfs.gz' located at 
             /home/sls/Nios2-Linux/Linux_source/uClinux-dist/image/
       (c)  You will get Linux booting masseges on the nios2-terminal window.
                #nios2-configure-sof "路径名".sof         eg:#**nios2-configure-sof C:/nios2_linux_3c120_125mhz_top.sof**
             Ex:
        #nios2-download -g linux.initramfs.gz        eg:**nios2-download -g  C:/linux.initramfs.gz **
        #nios2-terminal
验证结果： 
            Welcome to      NiosIILinux
       ls
**执行：     ./hello**
Note:  
    -- Release is tested on fedora 8 only.
