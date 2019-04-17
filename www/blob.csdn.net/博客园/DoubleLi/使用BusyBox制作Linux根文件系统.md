# 使用BusyBox制作Linux根文件系统 - DoubleLi - 博客园






STEP 1：构建目录结构 

创建根文件系统目录，主要包括以下目录
/dev  /etc /lib  /usr  /var /proc /tmp /home /root /mnt /bin  /sbin  /sys 

**#mkdir     /home/rootfs#cd        /home/rootfs**

**#mkdir  dev  etc  lib  usr  var  proc  tmp  home  root  mnt   sys**



STEP 2:    使用busybox构建/bin /sbin linuxrc

进入busybox-1.16.1目录，执行

**#make defconfig**

**#make menuconfig**

Busybox Setting ----->
    Build Options -----> 
**//1选择将busybox进行静态编译 **
        [*]Build BusyBox as a static binary (no shared libs) 
**//2.指定交叉编译器为 **
        (/usr/local/arm/4.3.2/bin/arm-linux-)Cross Compiler prefix

Installation Options ----->

**//3.选择上 Don’t use /usr **
Busybox Library Tuning---> 
    [*]Username completion 
    [*]Fancy shell prompts 
    [*]Query  cursor  position  from  terminal 
**//4.编译出的busybox的shell命令解释器支持显示当前路径及主机信息**

保存退出

#make   
#make install

在busybox目录下会看见 _install目录，里面有/bin /sbin linuxrc三个文件
将这三个目录或文件拷到第一步所建的rootfs文件夹下。

**#cp bin/ sbin/ linuxrc /home/rootfs -ra **

**   切记一定要带上-a的参数，因为bin目录里大部分都是链接，如果不带-a的参数，拷过去之后会做相应的复制，不再是链接的形式**



**STEP 3  构建etc目录：**

> 
**1)进入根文件系统rootfs的etc目录，执行如下操作：**


拷贝Busybox-1.16.1/examples/bootfloopy/etc/* 到当前目录下
        #cp –r busybox-1.16.1/examples/bootfloopy/etc/*  rootfs/etc

        修改inittab，把第二项改为::respawn:-/bin/login

**        删除第三、第四行代码   **

> 
**2)拷贝虚拟机上的/etc/passwd, /etc/group, /etc/shadow到rootfs/etc下**


**        # cp /etc/passwd   rootfs/etc        # cp /etc/group    rootfs/etc         # cp /etc/shadow   roofs/etc**

> 
> 
**对以下三个文件修改，只保存与root相关的项，根据具体情况内容会有所不同。**


> 
**修改passwd为root:x:0:0:root:/root:/bin/sh，即只保存与root相关项，而且最后改成/bin/ash。**


> 
**修改group为root:x:0:root**


> 
**修改shadow为root:$1$x9yv1WlB$abJ2v9jOlOc9xW/y0QwPs.:14034:0:99999:7:::**



> 
登陆开发板时需输入用户名密码，同虚拟机相同

**3)修改profile**


> 
> 
**PATH=/bin:/sbin:/usr/bin:/usr/sbin          //可执行程序 环境变量**

**export LD_LIBRARY_PATH=/lib:/usr/lib        //动态链接库 环境变量**


> 
**/bin/hostname sunplusedu**


> 
**USER="`id -un`"**


> 
**LOGNAME=$USER**


> 
**HOSTNAME='/bin/hostname'**


> 
**PS1='[\u@\h \W]# '                          //显示主机名、当前路径等信息：**



> 
**4)修改 etc/init.d/rc.S文件**


> 
> 
**/bin/mount -n -t ramfs ramfs /var**


> 
**/bin/mount -n -t ramfs ramfs /tmp**


> 
**/bin/mount -n -t sysfs none /sys**


> 
**/bin/mount -n -t ramfs none /dev**


> 
**/bin/mkdir /var/tmp**


> 
**/bin/mkdir /var/modules**


> 
**/bin/mkdir /var/run**


> 
**/bin/mkdir /var/log**


> 
**/bin/mkdir -p /dev/pts                    //telnet服务需要**


> 
**/bin/mkdir -p /dev/shm                    //telnet服务需要**


> 
**echo /sbin/mdev > /proc/sys/kernel/hotplug//USB自动挂载需要**


> 
**/sbin/mdev -s         //启动mdev在/dev下自动创建设备文件节点**


> 
**/bin/mount -a**

**5)修改etc/fstab文件，增加以下文件**

** none   /dev/pts    devpts   mode=0622      0 0       tmpfs  /dev/shm    tmpfs    defaults       0 0**



**STEP 4 构建lib目录：**

**1)****#cd /usr/local/arm/4.3.2/arm-none-linux-gnueabi/libc/armv4t/lib**

     将以下动态库拷贝到rootfs/lib下
**  #cp  *so*  roofs/lib  -a**

** 2)#cd /usr/local/arm/4.3.2/arm-none-linux-gnueabi/libc/armv4t/usr/lib**
     将以下动态库拷贝到rootfs/lib下 
** #cp ./libstdc++.so.*  rootfs/lib  -a**









