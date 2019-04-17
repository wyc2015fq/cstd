# Linux下如何挂载FAT32格式USB设备 - DoubleLi - 博客园






挂u盘之前，运行命令cat /proc/partitions,看看现在系统中有哪些分区。插上u盘以后，再次运行上述命令，看看多出来什么分区.通常是sda1.

1、插入U盘

2、输入 fdisk -l /dev/sda 查看输出结果，比如我的是这样的：

[root@leason-laptop proc]# fdisk -l /dev/sda

Disk /dev/sda: 2004 MB, 2004877312 bytes

247 heads, 62 sectors/track, 255 cylinders

Units = cylinders of 15314 * 512 = 7840768 bytes

Device Boot Start End Blocks Id System

/dev/sda1 * 1 256 1957856 c W95 FAT32 (LBA)

Partition 1 has different physical/logical beginnings (non-Linux?):

phys=(0, 1, 1) logical=(0, 1, 3)

Partition 1 has different physical/logical endings:

phys=(505, 246, 0) logical=(255, 172, 42)

Partition 1 does not end on cylinder boundary.

[root@leason-laptop proc]# mount -t vfat /dev/sda1 /mnt/usb/

3、看了上面的输出就知道U盘所在的设备了，比如我的就是/dev/sda1，接着便是挂载了，假设我将U盘挂载到/mnt/usb目录(没有的话，新建)中，就是

如果是fat16就使用下面的命令

mount -t msdos /dev/sda1 /mnt/usb

如果是fat32

mount -t vfat /dev/sda1 /mnt/usb

如果是ext2格式，就用命令：

mount -t ext2 /dev/sda1 /mnt/usb

4、打开/mnt/usb 就可以看到你的U盘里的东西了！

cd /mnt/usb

具体命令：

1.加挂FAT32文件系统

最简单的用法

mount /dev/hda6 /mnt/d

/dev/hda6是笔者Windows的D盘，/mnt/d是目录加挂点。Linux会识别到/dev/hda6分区的文件系统类型是什么，然后加挂的。当然也可以指定分区的文件系统类型，命令如下：

mount -t vfat /dev/hda6 /mnt/d

在实际中操作中，直接加挂一个windows的分区，中文的文件名和目录名会出现乱码，为了避免这种情况可以指定字符集，命令如下：

mount /dev/hda6 /mnt/d -o codepage=936,iocharset=cp936

mount -t vfat /dev/hda6 /mnt/d -o codepage=936,iocharset=cp936

注意：cp936是指简体中文，cp950是指繁体中文。



2.加挂NTFS文件系统

在目前多数的Linux版本上，需要重编译Linux核心才能，加挂NTFS分区(编译方法请参阅其它文章)。核心支持NTFS后，可以用以下命令加挂：

mount -t ntfs /dev/hda2 /mnt/c

同样对于中文的文件名和目录名会出现乱码的问题可以指定字符集，不过和加挂vfat分区不同，实际中用以下命令是可行的：

mount -t ntfs -o iocharset=cp936 /dev/hda2 /mnt/c -r

mount -t ntfs -o iocharset=cp936,rw /dev/hda2 /mnt/c



3.加挂U盘上的文件系统

现在使用U盘的人越来越多了，其实在Linux下使用U盘也非常简单，Linux对USB设备有很好的支持，在你插入U盘后，U盘被识别为一个SCSI盘，通常您用以下命令就能加挂U盘上的文件系统

mount /dev/sda1 /usb

同样对于中文的文件名和目录名会出现乱码的问题可以指定字符集，命令类似于上面对FAT32介绍的：

mount /dev/sda1 /usb -o pagecode=936,iocharset=cp936

4.加挂Linux系统通过samba共享出来的目录

利用samba共享出来的目录其实很难说其原来是哪种文件系统，不过这一点不重要，只要它对用户是透明的就好啦。加挂时，我们指定类型为smbfs，在加挂samba共享的分区时，也会出现中文的文件名和目录名乱码情况，可以用以下命令加挂：

mount -t smbfs -o

username=terry,password=terry,codepage=936,iocharset=cp936

//terry-linux/terry /mp3/

mount -t smbfs -o

username=terry,password=terry,codepage=936,iocharset=cp936

//192.168.100.228/terry /mp3/

注意：你可以不直接写password=terry这个参数，系统界时会要你输入密码，这样就可以防止有人直接看到你的密码。视具体情况而定，-o后面的参数可以对应增减。

5.加挂Window系统共享出来的目录

在局域网中，常常需要去访问其它Windows系统共享出来的目录，在Linux下，安装了samba后就可以使用samba中带的命令来访问Windows机器的共享资源。

用smbclient来列出Windows机器的共享资源

smbclient -L 192.168.100.111

根据上面所列出Windows的共享资源后，可以选择需要加挂到本地Linux中去的Windows的共享资源，然后使用smbmount或mount来加挂，请参照下面几条命令：

smbmount //192.168.100.111/public /public/

mount //192.168.100.111/d /mnt/cdrom -o username=terry (这样要在命令行输入密码的)

mount //192.168.100.111/d /mnt/cdrom -o username=terry$1234 (这样则不必在命令行输入密码)



注：除了上面介绍的命令行方法之外，这时最好的方法就是使用其它的客户端，如LinNeighborhood、networkneighbours、ksmbshare等，请参考其它文章。



以上介绍的都是命令行的方式，每次使用时都要输入一次，如果你常常要加挂一些文件系统，想在启动时自动加挂应该怎样做呢？下面介绍两种方法：



方法1.

把加挂的命令放在/etc/rc.d/rc.local中。

方法2.

修改分区配置文件/etc/fstab，在启动时加挂文件系统，下面是我的/etc/fstab文件：

LABEL=/ / ext3 defaults

1 1

none /dev/pts devpts gid=5,mode=620

0 0

LABEL=/home /home ext3 defaults

1 2

none /proc proc defaults

0 0

none /dev/shm tmpfs defaults

0 0

/dev/hda4 swap swap defaults

0 0

/dev/cdrom /mnt/cdrom iso9660

noauto,owner,kudzu,ro 0 0

/dev/hda2 /ntfs ntfs defaults,iocharset=cp936 0 0

/dev/hda6 /win vfat defaults,codepage=936,iocharset= cp936 0 0

//192.168.100.228/terry /mp3 smbfs username=terry,password=terry,codepage=cp936,iocharset=cp936 0 0

请注意最后三行：

倒数第四行，加挂我的C盘，NTFS格式的

倒数第三行，加挂我的D盘，FAT32格式的

倒数第二行，加挂的是我的局域网中另一台安装samba的Linux文件服务器上的一个目录。

以上所有命令，必须以root身份才能执行

用完U盘之后，记得要卸载掉才能拔U盘，操作如下：

# umount -l /mnt/usb









