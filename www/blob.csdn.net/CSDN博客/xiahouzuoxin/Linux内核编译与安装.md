# Linux内核编译与安装 - xiahouzuoxin - CSDN博客





2012年10月03日 13:22:12[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2217标签：[linux内核																[menu																[终端																[工作																[x86](https://so.csdn.net/so/search/s.do?q=x86&t=blog)
个人分类：[Linux](https://blog.csdn.net/xiahouzuoxin/article/category/1286747)





操作环境：RHEL 5.1




**一、准备工作**

1、获取内核


[http://www.kernel.org/](http://www.kernel.org/)
2、解压缩内核文件，进入解压后内核源代码目录，切换至root用户

tar -xzvf linux-2.6.39.4.tar.bz2

cd /mnt/hgfs/linux-2.6.39.4


su root




**二、内核编译**

3、内核清理：清理源代码中的*.o等生成文件，获得纯净的内核。 直接官网上下载的代码可免去该步骤。
make distclean

4、内核配置

拷贝当前运行内核的配置文件：cp /boot/config-2.6.18-53.el5  ./.config

运行配置，make menuconfig ARCH=x86

按需配置后保存退出，配置选项参考网上资源。

【注1】默认各种架构的配置文件在arch/[cpu]/configs文件夹下；运行时终端要保证窗口足够大能容纳menu界面

【注2】关于make menuconfig 选项中“*”“M”“ ”3个符号的含义:
“*”—— a.c->a.o->生成uImage 包含编译和链接过程。最后生成内核文件

“M”—— b.c->a.o 只包含编译过程

“ ”—— d.c->不变化

5、内核编译：只编译配置时选择为“*”的选项

make zImage (<512K)

make bzImage (>512K) 


编译好的内核位于arch/<cpu>/boot/目录下

6、内核模块编译：编译配置时选择“M”的选项

make modules 

7、安装内核模块

make modules_install

将编译好的内核模块拷贝到系统/lib/modules下

8、制作init ramdisk

mkinitrd initrd-$version $version

initrd-$version表示制作的init ramdisk的名称，制作完后保存在内核根目录。

其中$version通过查询/lib/modules获得，表示内核版本号，此处为2.6.39.4。




**三、内核安装**


9、拷贝编译好的bzImage到系统/boot/目录

cp arch/x86/boot/bzImage /boot/vmlinuz-$version    $version为版本号，这里为2.6.39.4。

10、拷贝制作好的init ramdisk到系统/boot/目录

cpinitrd-2.6.39.4 /boot/

11、修改启动项，添加新的内核启动项

修改/etc/grub.conf（一般默认的都是grub启动）或者/etc/lilo.conf

注：将showmenu的注释去掉，启用选择操作系统菜单；

拷贝一份已有的启动项配置(4行)，修改bzImage和initrd

12、重启系统，OK。多了一个启动内核选项。](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=menu&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)




