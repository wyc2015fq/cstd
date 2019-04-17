# 使用Busybox-1.2.0制作根文件系统 - DoubleLi - 博客园






使用Busybox-1.2.0制作根文件系统

cross-3.3.2

make-3.8.1



STEP 1：

创建根文件系统目录，主要包括以下目录/bin，/etc，/dev，/mnt，/sbin，/usr，/sbin，/tmp

/proc，/lib. /usr下有bin, sbin, lib, local, etc   /mnt下有etc

创建多级目录可以使用mkdir –p usr/bin/ …….



STEP 2：

升级make到3.81版本，用RH9自己带的make 3.79会出错

进入make-3.81目录，执行：# ./configure    # make   #make install

会把make安装到/usr/local/bin目录下，要使新make立即生效，要设置环境变量

PATH=/usr/local/bin:$PATH。设置完成后，就可以编译Busybox了。



STEP 3：

进入busybox-1.2.0目录，执行# make defconfig

执行# make menuconfig



Busybox Setting -----> Build Options ----->

选择上 Build BusyBox as a static binary (no shared libs)

       Do you want to build BusyBox with a Cross Compiler?

指定交叉编译器为

(/usr/local/arm/3.3.2/bin/arm-linux-)Cross Compiler prefix



Installation Options ----->

选择上 Don’t use /usr

设置Busybox installation prefix为 ../rootfs即安装目录.



在Shells中,选择Choose your default shell为ash.

保存退出



STEP 4：

# make    # make install



安装完后，就会把busybox拷到自己指定的rootfs 路径下，同时会生成一个linuxrc文件，这个是busybox自带的启动文件，也可以自己编写启动文件



Linux内核使用的是2.4.18

用3.3.2交叉编译器和1.2.0版本的busybox编译通过

我开始用1.9.2版本的，用2.95.3，3.4.1，3.3.2编译均不通过



STEP 5：   这个是用来启动linux时输入用户和密码的

编译tinylogin-1.4。进入tinylogin-1.4目录，修改Makefile文件如下：

DOSTATIC = true

USE_SYSTEM_PWD_GRP = false

USE_SYSTEM_SHADOW = false

CROSS = /usr/local/arm/3.3.2/bin/arm-linux-



STEP 6：

执行# make PREFIX=../rootfs install      安装到指定路径



STEP 7：

进入根文件系统rootfs的etc目录，执行如下操作：

拷贝Busybox-1.2.0/examples/bootfloopy/etc/* 到当前目录下。

# cp –r ../../busybox-1.2.0/examples/bootfloopy/etc/* ./

拷贝/etc/passwd, /etc/group, /etc/shadow到当前目录下。

# cp /etc/passwd ./

# cp /etc/group ./

# cp /etc/shadow ./



修改inittab，把第二项改为::respawn:-/bin/login

修改profile，加入PATH环境变量：export PATH=/bin:/sbin:/usr/bin:/usr/sbin

设置链接库：export LD_LIBRARY_PATH=/lib:/usr/lib



对以下三个文件修改，只保存与root相关的项，根据具体情况内容会有所不同。

修改passwd为root:x:0:0:root:/root:/bin/sh，即只保存与root相关项，而且最后改成/bin/sh。

修改group为root:x:0:root

修改shadow为root:$1$x9yv1WlB$abJ2v9jOlOc9xW/y0QwPs.:14034:0:99999:7:::



STEP 8：

制作cramfs镜像，下载到实验箱上就可以引导起来了。

# mkcramfs rootfs rootfs.cramfs



这个是一个基本的移植过程，下面看一下busybox的启动过程

先进入vivi, 执行param show 会在最后一行看到：

Linux command line: noinitrd root=/dev/mtdblock/1 init=/linuxrc console=ttyS0

这个是vivi传给linux的命令行参数，root=表示root根文件系统挂在哪个设备下，这里是mtd分区的，如果是bon分区的，就会是root=/dev/bon/2  init=/linuxrc这个最重要，表示初始化启动脚本为根目录下的linuxrc，console=ttyS0 初始化控制台为串口



BusyBox的intit命令提供了类似传统init程序的系统启动和初始化功能，init程序（如/sbin/init）只是一个指向busybox程序(/bin/busybox)的符号链接，也就是说，busybox是系统第一个执行的应用程序。Busybox由文件名识别出要执行的命令是init, 然后跳转到init例程去运行



Busybox init 依次执行如下任务：

设置init的信号处理函数

初始化控制台

分析/etc/inittab配置文件

执行系统初始化脚本，如/etc/init.d/rcS



完成初始化控制台后，busybox检查/etc/inittab文件是否存在，如果不存在，将使用一个缺省的inittab，我可可以自己写一个inittab文件，格式如下：

id:runlevel:action:process

id指定要启动的进程   runlevel是运行级别，busybox不支持运行级别，所以为空

action是要执行的动作，process指定要运行的程序和启动的命令行参数



Action：

sysinit   指定初始化脚本路径

respawn  当某进程结束时重启该程序

askfirst  在启动程序前提示用户按回车键

wait  等待所启动的进程结束

once  只运行进程一次，不等待进程结束

crtlaltdel 按组合键时执行的进程

shotdown 系统关闭时运行的进程

restart 重启时运行的进程，通常是init本身



自己写的inittab

::sysinit:/etc/init.d/rcs

::respawn:-/bin/sh

tty2:askfirst:-/bin/sh

::ctrlaltdel:/bin/umount –a –r

第一句指定执行初始化脚本为 /etc/init.d/rcS

第二句就是当进程退出，如执行exit后，又重新出现命令提示符

      如果要用帐号密码登陆，则改成-/bin/login

第三句是在启动linux后，提示要按回车才能进入

第四句是在按组合键后，执行umount，把所有文件系统umount



再看/etc/init.d/rcS

#!/bin/sh          //指定shell

/bin/mount –a        //mount –a 为挂载全部文件系统，详见/etc/fstab

exec /usr/etc/.rc.local   //执行rc.local



rcS只完成挂载全部文件系统，然后执行rc.local，进入/usr/etc，看rc.local

#!/bin/sh

ifconfig eth0 192.168.220.10   //设置板子IP

echo “Set IP = 192.168.220.10”



mount –t nfs –o nolock ……………..  //挂载NFS

echo “mount nfs ok”

也可以写一些其他的挂载命令



到这里就完成了启动过程，有时也可以在/etc下写一个profile，这个是由shell执行的，启动后，按完回车才执行，这里也可以写一些初化脚本，如初始化环境变量



总结：linuxrc -> /sbin/init -> /etc/inittab -> /etc/init.d/rcS -> /usr/etc/rc.local   /etc/profile



也可以编写自己的linuxrc

#!/bin/sh

/bin/mount –n –t ramfs ramfs /etc

/bin/cp –a /mnt/etc/* /etc



/bin/mount –f –ramfs ramfs /tmp



exec /sbin/init



主要就是最后一句，执行init

把/etc中的文件放到/mnt/etc/下，挂载/etc/为ramfs，然后把/mnt/etc/下的所有文件拷到/etc下，这时/etc下就为可写的了，这样可以方便修改脚本



最好也将/tmp挂为ramfs，因为执行QT程序时，需要tmp目录为可写









