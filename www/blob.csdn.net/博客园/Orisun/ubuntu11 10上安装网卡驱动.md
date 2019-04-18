# ubuntu11.10上安装网卡驱动 - Orisun - 博客园







# [ubuntu11.10上安装网卡驱动](https://www.cnblogs.com/zhangchaoyang/articles/2604951.html)





This is guidance for those with integrated Realtek RTL8111 series gigabit Ethernet, built in to motherboards such as the ASRock G41M-LE.


Ubuntu 8.10 (and may other versions) have been reporting problems with 
connectivity. Here is the solution in a walk-through format. You must be
 root (sudo su -).


1) Check to see if the r8169 module is loaded

-> lsmod | grep r816

r8168                  41104  0 

-> lspci -v

01:00.0 Ethernet controller: Realtek Semiconductor Co., Ltd. RTL8111/8168B PCI Express Gigabit Ethernet controller (rev 03)

	Subsystem: ASRock Incorporation Device 8168

	Kernel driver in use: r8169

	Kernel modules: r8169


2) Download the official Realtek driver

Realtek RTL8111/RTL8168 
(http://www.realtek.com.tw/downloads/downloadsView.aspx?Langid=1&PNid=13&PFid=5&Level=5&Conn=4&DownTypeID=3&GetDown=false#2)


3) Remove the r8169 module

-> rmmod r8169

-> mv /lib/modules/`uname -r`/kernel/drivers/net/r8169.ko ~/r8169.ko.backup

( the ` is a backtick, it is not an apostrophe or single quote )


4) Build the new r8168 module for the kernel

-> bzip2 -d r8168-8.009.00.tar.bz2

-> tar -xf r8168-8.009.00.tar

-> cd r8168-8.009.00

-> make clean modules

-> make install


5) Rebuild the kernel module dependencies

-> depmod -a

-> insmod ./src/r8168.ko


6) Remove the r8169 module from initrd

-> mv /initrd.img ~/initrd.img.backup

-> mkinitramfs -o /boot/initrd.img-`uname -r` `uname -r`


7) Add r8168 module to /etc/modules

-> echo "r8168" >> /etc/modules


8) Reboot, You are done!


9) Examine that ONLY the r8168 module is loaded for the interface

-> lspci -v

01:00.0 Ethernet controller: Realtek Semiconductor Co., Ltd. RTL8111/8168B PCI Express Gigabit Ethernet controller (rev 03)

	Subsystem: ASRock Incorporation Device 8168

	Kernel driver in use: r8168

	Kernel modules: r8168


If you need to, configure your /etc/network/interfaces for dhcp or static address then `sudo ifup eth0`












