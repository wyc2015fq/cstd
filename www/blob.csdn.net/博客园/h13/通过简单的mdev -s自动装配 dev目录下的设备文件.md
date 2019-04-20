# 通过简单的mdev -s自动装配/dev目录下的设备文件 - h13 - 博客园
BusyBox已带有一个简化板的udev － mdev，通过简单的mdev -s，就能自动装配/dev目录下的设备文件： 
```
# ls /dev
console  null  ptmx  pts  shm
```
```
# mdev -s && ls /dev
apm_bios         ptypd            tty24            tty56
console            ptype            tty25            tty57
fb0                   ptypf             tty26            tty58
full                    ram0             tty27            tty59
kmem               ram1             tty28            tty6
kmsg                ram2             tty29            tty60
loop0                ram3             tty3             tty61
... ...
```
启动热拔插支持的话，用这一句 
`echo /sbin/mdev > /proc/sys/kernel/hotplug`
一切就是这么简单，相应的/dev目录下会出现一大堆设备文件，甚是难看。 
mdev的规则文件是/etc/mdev.conf ，与传统的udev规则文件写法上有点区别。
KERNEL=="tty[0-9]*", NAME="vc/%n"   //for udev
tty[0-9]* 0:5 660  //for mdev
如果选上FEATURE_MDEV_EXEC支持，规则后面可以执行相应的命令，如：
pts/ 0:0 755 $mount -t devpts /dev/pts /dev/pts
