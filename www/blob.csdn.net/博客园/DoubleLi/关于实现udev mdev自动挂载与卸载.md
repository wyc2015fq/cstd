# 关于实现udev/mdev自动挂载与卸载 - DoubleLi - 博客园






在网上有很多关于讲mdev的自动挂载基本上都是一个版本，经过测试自动挂载确实可行，但是关于自动卸载mdev似乎不能很好的支持，经过修改已经可以做到与udev的效果相似。不能在挂载的目录中进行热插拔，否则会出现问题，不过此问题在下次插入U盘时不会造成影响，可能对U盘有损坏。

本文介绍了mdev与udev两种方法来实现自动挂载，读者可根据需要任选其一即可。

        首先介绍一下mdev与udev之间的关系：

        mdev是busybox中的一个udev管理程序的一个精简版，他也可以实现设备节点的自动创建和设备的自动挂载，只是在实现的过程中有点差异，在发生热插拔时间的时候，mdev是被hotplug直接调用，这时mdev通过环境变量中的 ACTION 和 DEVPATH，来确定此次热插拔事件的动作以及影响了/sys中的那个目录。接着会看看这个目录中是否有“dev”的属性文件，如果有就利用这些信息为这个设备在/dev 下创建设备节点文件。





/***********************************************************************************************************************************************************************************/

1.mdev支持

> 
①用busybox制作根文件系统的时候，要选择支持mdev机制

> 
Linux System Utilities  --->   
           [*] mdev      
           [*]   Support /etc/mdev.conf
           [*]     Support command execution at device addition/removal


②在文件系统/etc/init.d/rsC文件中添加如下内容

> 
Vi  /etc/init.d/rcS
        mount -t tmpfs mdev /dev 
        mount -t sysfs sysfs /sys
        mkdir /dev/pts
        mount -t devpts devpts /dev/pts

        echo /sbin/mdev>/proc/sys/kernel/hotplug
        mdev –s


这些语句的添加在busybox的/doc/mdev.txt中可以找到。



③添加对热插拔事件的响应，实现U盘和SD卡的自动挂载。

> 
Vi /etc/mdev.conf
       sd[a-z][0-9]      0:0 666        @/etc/mdev/udisk_insert                        

         sd[a-z]                   0:0 666          $/etc/mdev/udisk_remove


红色部分，是一个脚本，脚本内容可以根据我们的需要定制，可以实现挂载，卸载或其他一些功能。

注：@表示是在插入（创建设备结点）后执行后面的脚本，$表示在拔出（删除设备结点）前执行后面的脚本。

如下是自动挂载和卸载的脚本名称及其内容：

> 
> 
#!/bin/sh

if [ -d /sys/block/*/$MDEV ]  ; then

  mkdir -p /media/$MDEV

  mount /dev/$MDEV /media/$MDEV 

fi



根文件系统中的etc/mdev/udisk_remove文件内容:

> 
        #!/bin/sh

        umount -l /media/$MDEV

        rm -rf /media/$MDEV



        #!/bin/sh

        umount -l /media/sd*

        rm -rf /media/sd*


修改为红色部分后能够自动挂载

以上两个脚本需要可执行权限：chmod +x  /etc/mdev/udisk_insert

> 
> 
> 
               chmod +x etc/mdev/udisk_remove






/***********************************************************************************************************************************************************************************/

2.udev支持 

        linux传统上使用静态设备创建的方法，在dev下创建了大量的节点，而不管这些节点相应的硬件设备是否存在。采用udev的方法，系统检测到设备才会去创建这些设备对应的节点。

        这里我们简单的说一下udev的工作原理：

        udev是依赖于sysfs的，当系统中添加一个新的设备后，内核检测到后就会产生一个hotplug event并查找/proc/sys/kernel/hotplug去找出管理设备连接的用户空间程序，若udev已经启动，内核会通知udev去检测sysfs中关于这个新设备的信息并创建设备节点。如/dev/vcs,在/sys/class/tty/vcs/dev存放的是”7:0”，既/dev/vcs的主次设备号。并且udev还会根据/etc/udev/rules.d中的规则文件实现一些相应的功能。

下面我们介绍一下如何实现设备节点的自动创建及u盘或sd卡的自动挂载。

> 
因为文件系统中默认是没有对udev进行支持的，所以我们移植一个udev。

1.下载udev源码udev-100.tar.bz2，并解压

网址：[http://www.us.kernel.org/pub/linux/utils/kernel/hotplug](http://www.us.kernel.org/pub/linux/utils/kernel/hotplug)

2.交叉编译。

修改makefile,具体修改如下：

        cross = arm-linux-

保存退出。

然后执行命令：make 进行编译，然后执行arm-linux-strip udev udevd udevstart udevinfo udevtest，并拷贝这些文件到目标板根文件/bin目录下面。

3.添加udev的支持


下面三种方法功能相同

        (1)并修改etc/init.d/rcs脚本，然后添加如下命令：

        /bin/mount -t sysfs sysfs /sys 

        /bin/mount -t tmpfs tmpfs /dev

        /bin/udevd --daemon

        /bin/udevstart

        (2)如果linuxrc是二进制文件的话 

        rm /linuxrc

        vi /linuxrc

        添加如下内容

        /bin/mount -t sysfs sysfs /sys

        /bin/mount -t tmpfs tmpfs /dev

        /bin/udevd --daemon

        /bin/udevstart

        exec /sbin/init

        (3)修改/etc/fstab为

        #device mount-point type options dump fsck order

        proc /proc proc defaults 0 0

        tmpfs /tmp tmpfs defaults 0 0


        sysfs /sys sysfs defaults 0 0

        tmpfs /dev tmpfs defaults 0 0

        修改/etc/init.d/rcs,添加如下内容

> 
/bin/udevd --daemon

/bin/udevstart


        重新启动系统，文件系统就能够自动创建节点。



4.在/etc下创建udev目录

5.在/etc/udev下穿件目录rules.d和文件udev.conf

6.在udev.conf中添加如下内容

# udev.conf

        # the initial syslog(3) priority: "err", "info", "debug" or its

        # numerical equivalent. for runtime debugging, the daemons internal

        # state can be changed with: "udevcontrol log_priority=<value>".

        udev_log="err"

**7.在rules.d下创建规则文件**

**如实现u盘自动挂载**

        vim 11-add-usb.rules

添加如下内容

        action!="add",goto="farsight"

        kernel=="sd[a-z][0-9]",run+="/sbin/mount-usb.sh %k"

        label="farsight"

这个文件中action后是说明是什么事件，kernel后是说明是什么设备比如sda1，mmcblk0p1等，run这个设备插入后去执行哪个程序%k是传入这个程序的参数，这里%k=kernel的值也就是sda1等http://www.woaidiannao.com。

在/sbin/下创建mount-usb.sh文件添加如下内容 计算机

        #!/bin/sh

        /bin/mount -t vfat /dev/$1 /tmp

        sync

**修改文件权限为其添加可执行的权限。**

这样就实现了u盘的自动挂载，下面附上u盘的卸载规则文件和sd卡的文件

**usb卸载**

11-add-remove.rules

        action !="remove",goto="farsight"

        subsystem!="block",goto="farsight"

        kernel=="sd[a-z][0-9]",run+="/sbin/umount-usb.sh"

        label="farsight"

umount-usb.sh

        #!/bin/sh

        sync

        umount /tmp/

**sd卡挂载**

**12-add-sd.rules**

action!="add",goto="farsight"

        kernel=="mmcblk[0-9]p[0-9]",run+="/sbin/mount-sd.sh %k"

        label="farsight"

mount-sd.sh

        #!/bin/sh

        /bin/mount -t vfat /dev/$1 /tmp

        sync



**sd卸载**

**12-remove-sd.rules**

        action !="remove",goto="farsight"

        subsystem!="block",goto="farsight"

        kernel=="mmcblk*",run+="/sbin/umount-sd.sh"

        label="farsight"

umount-sd.sh

        #!/bin/sh

        sync

        /bin/umount /tmp/










