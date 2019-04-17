# Bochs上运行BIOS启动代码 - 小灰笔记 - CSDN博客





2017年08月23日 22:52:22[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：772








       安装好了Bochs，也运行了Demo系统没有问题。测试自己的BIOS启动代码的时候却一直不成功。最初，我以为这是我的镜像文件制作错了，花了整个晚上折腾镜像文件还是没有成功。于是又尝试修改启动配置文件，启动配置文件修改了之后依然不成功。睡眼惺忪的时候看了一眼代码发现代码错了！真是醉了！

       配置文件如下：

###############################################################

# bochsrc.txt filefor DLX Linux disk image.

###############################################################



# how much memorythe emulated machine will have

megs: 16



# filename of ROMimages

romimage:file=D:\software\Bochs-2.6.9\BIOS-bochs-latest

vgaromimage:file=D:\software\Bochs-2.6.9\VGABIOS-lgpl-latest



# what disk imageswill be used 

floppya:1_44=bios_demo, status=inserted

#floppyb:1_44=floppyb.img, status=inserted



# hard disk

#ata0: enabled=1,ioaddr1=0x1f0, ioaddr2=0x3f0, irq=14

#ata0-master:type=disk, path=new_boot, mode=flat, cylinders=520, heads=0, spt=63



# choose the bootdisk.

boot: floppy



# default configinterface is textconfig.

#config_interface:textconfig

#config_interface:wx



#display_library:x

# other choices:win32 sdl wx carbon amigaos beos macintosh nogui rfb term svga



# where do we sendlog messages?

log: bochsout.txt



# disable themouse, since DLX is text only

mouse: enabled=0



# set up IPS valueand clock sync

cpu: ips=15000000

clock: sync=both



# enable keymapping, using US layout as default.

#

# NOTE: In Bochs1.4, keyboard mapping is only 100% implemented on X windows.

# However, the keymapping tables are used in the paste function, so 

# in the DLX Linuxexample I'm enabling keyboard_mapping so that paste 

# will work.  Cut&Paste is currently implemented onwin32 and X windows only.



keyboard:keymap=D:\software\Bochs-2.6.9\keymaps\x11-pc-us.map

#keyboard:keymap=../keymaps/x11-pc-fr.map

#keyboard:keymap=../keymaps/x11-pc-de.map

#keyboard:keymap=../keymaps/x11-pc-es.map

       启动之后的效果：

![](https://img-blog.csdn.net/20170823225120289?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       终于看到了红色的启动提示字符，感觉终于有了一点成就感。其实，这个启动的配置文件写起来十分简单，最简单的方式自然还是使用dd命令。感觉Win10支持bash之后，折腾各种东西的时候都十分顺手啊！



