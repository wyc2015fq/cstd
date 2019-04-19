# linux内核的编译与安装 - xqhrs232的专栏 - CSDN博客
2017年03月08日 16:03:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：308
原文地址::[http://blog.csdn.net/liuqinstudy/article/details/9055935](http://blog.csdn.net/liuqinstudy/article/details/9055935)
相关文章
1、Linux内核编译安装过程----[http://www.linuxidc.com/Linux/2016-12/137794.htm](http://www.linuxidc.com/Linux/2016-12/137794.htm)
2、
# [linux内核编译步骤](http://blog.csdn.net/liuqinstudy/article/details/9056439)----[http://blog.csdn.net/liuqinstudy/article/details/9056439](http://blog.csdn.net/liuqinstudy/article/details/9056439)
1、实验背景：主机WIN7     虚拟机VMware (ubuntu12.04)     内核版本[Linux](http://lib.csdn.net/base/linux)-3.2.0-23-generic-pae
2、下载了新的内核版本linux-3.2.1来进行编译，安装
步骤：
     1、首先下载你想要编译的内核版本:linux-3.2.1.tar.bz2。
     2、进入root权限把刚下载的内核代码复制到/usr/src目录下面去。
     3、在/usr/src目录下面解压刚才下载的内核代码：tar   -xjvf  linux-3.2.1.tar.bz2，解压完成后进入相应的linux-3.2.1           目录，以后的工作都会在这个目录下面完成。
     4、开始编译前的工作：
          4.1  使用命令：make mrproper
              作用是在每次配置并重新编译内核前需要先执行”make mrproper”命令清理源代码树，包括过去曾经配置的内核配置文件”.config”都将被清除。即进行新的编译工作时将原来老的配置文件给删除，以免影响新的内核编译。(但是一般情况也可以不执行这条命令,因为内核编译是增量编译,清除之后又要从头开始编译,浪费时间).
         4.2   使用命令：make menuconfig，进行内核定制，配置编译信息。
              输入命令：make menuconfig  。然后再出现的界面中移动鼠标选中file system项目  然后再出现的界面中移动鼠标选中file system项目，进入其中，把second extended fs suppor前面的 * 好改成M（修改这个的目的是让在编译内核的时候以模块的方式进行编译，而不是编译进内核）。这里可能会提醒安装ncurses相应的包。安装这个包的命令如下：sudo
 apt-get install libncurses5-dev
        4.3    保存退出
    5、  make dep命令生成内核功能间的依赖关系。一般会提示unecessary.
    6、  make命令 编译内核。(若过程不出现问题,大约1-2个小时编译完成) 。
           这个相当于手工执行下面两步：
           #make bzImage(编译出核心)
           #make modules(编译可加载模块)
    7、  等编译内核完成之后就是安装模块。使用命令：make modules_install
    8、 安装核心。 使用命令：make install
    9、  创建initrd文件  使用命令：mkinitramfs -o /boot/initrd.img-3.2.1（后面的数字是自己需要编译的内核的版本，根据自己的版本不同来书写）.*****后面出现的一个错误在重新用另外的方法生成initrd.img-3.2.1文件后解决，不知道是不是这里的生成方法有错，但是可以用这种方法试一试，出错了再改。因为我也只试了一次，说不定是我操作失误了。*****
   10、 更新grub来显示自己安装的内核。进入/boot/grub目录，然后输入命令：update-grub即可。
    经过以上的步骤，内核基本上已经编译成功并且已经安装上了。
为了看自己的内核是否安装上了可以通过以下步骤来看：进入/boot/grub/目录，然后打开grub.cfg文件，看你自己编译的版本是否已经在里面了，如果在里面了，就说明你自己编译的内核就完成了。这是后重启就可以进入你自己编译的内核里面了。（但是为了选择自己安装的内核或者是选择进入以前系统自带的内核还必须更改/boot/grub/目录下面的文件grub.cfg，把里面的所有的timeout的值都更改成10）。然后重新启动就可以选择了。
以上就是编译和安装自己内核的步骤。
过程中出现的错误：
  1、提示虚拟机磁盘空间不足，如下：
![](https://img-blog.csdn.net/20130608144907781)
     原因是我分配给虚拟系统的磁盘只有10G，而已经使用完了，查看磁盘使用率如下图：
![](https://img-blog.csdn.net/20130608145037812)
  解决方法：我按照网上的方法使用vmware-manager.exe扩充磁盘结果导致虚拟机无法进入系统。所以只好重装系统，分配更大的空间。
    重装之后，编译顺利通过。但是在进入新编译的内核是出现错误2.
   2、could not load /lib/modules/3.2.1/modules.dep。
      但是 在/lib/modules/3.2.1/下又能找到modules.dep文件。于是我尝试了网上的方法，结果能够正常进入新编译的内核了。方法如下：
         1、备份之前生成的/boot/initrd.img-3.2.1，执行
             使用如下命令:mv   initrd.img-3.2.1  initrd.img-3.2.1.old(重命名)
         2、cd  /usr/src/linux-3.2.1.
         3、执行：update-initramfs  –c  –k  3.2.1。这样，重新在/boot下创建initrd.img-3.2.1
        4 、sudo update-grub
        5、将/boot/grub/grub.cfg中的timeout改为10
重启后，进入了新内核，内核编译升级成功。
