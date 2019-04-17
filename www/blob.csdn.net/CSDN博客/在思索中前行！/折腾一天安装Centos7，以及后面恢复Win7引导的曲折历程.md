# 折腾一天安装Centos7，以及后面恢复Win7引导的曲折历程 - 在思索中前行！ - CSDN博客





2014年11月21日 10:30:03[_Tham](https://me.csdn.net/txl16211)阅读数：20266标签：[安装Centos7																[Centos 7引导Win7																[安装Centos 7没有Win7启动项](https://so.csdn.net/so/search/s.do?q=安装Centos 7没有Win7启动项&t=blog)
个人分类：[linux学习																[鸟哥的私房菜学习](https://blog.csdn.net/txl16211/article/category/2590995)](https://blog.csdn.net/txl16211/article/category/2231071)




一、下载centos 7 livecd iso
访问镜像网站，[http://mirrors.aliyun.com/centos/7.0.1406/isos/x86_64/](http://mirrors.aliyun.com/centos/7.0.1406/isos/x86_64/)
或者直接下载：[http://mirrors.aliyun.com/centos/7.0.1406/isos/x86_64/CentOS-7.0-1406-x86_64-livecd.iso](http://mirrors.aliyun.com/centos/7.0.1406/isos/x86_64/CentOS-7.0-1406-x86_64-livecd.iso)
为什么选择这个ISO？
因为足够简洁，该有的基本上有了。新手的话还是带图形界面更容易上手。高端玩家可以下载其他的ISO文件。
如果用于服务器实装，也可以用这个ISO安装完后设置为INIT 3。
二、制作USB引导盘
如果是linux系统，使用dd命令直接制作。
dd if=CentOS-7.0-1406-x86_64-livecd.iso of=/dev/sdc
注意：
dd命令制作的USB KEY无法在WIN中正常查看。但在LINUX系统中没有任何问题。
/dev/sdc为U盘的设备名。不要照抄。可以先用命令fdisk -l 查看对应的设备名
不要使用诸如/dev/sdc1这样的分区名，而是/dev/sdc这样的。因为是写入整个USB KEY。

如果是WIN系统，下载并安装Universal-USB-Installer应用程序来制作。因为RHEL、FEDROA、CENTOS都是血脉相连。该程序只能制作LIVE CD，这也是为什么我要选择该ISO的原因之一。

这个应用程序在二楼的打包文件中已经提供了。

下面贴一些实际安装过程中会出现的图片：



![](http://www.centoscn.com/uploads/allimg/140728/1-140HP15434119.jpg)

注意：对于U盘安装，我们需要修改镜像位置。

2.2、选择第一项：Install CentOS
 7 ，按 e 键进入编辑界面。

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP15444405.jpg)


将 vmlinuz initrd=initrd.img inst.stage2=hd:LABEL=CentOS\x207\x20x86_64 quiet

改为 vmlinuz initrd=initrd.img inst.stage2=hd:/dev/sdb4 quiet，这里sdb4为U盘所在分区，也可能是sdb1
 ，可根据实际情况修改（也可以用其他方法）（这个部分是网上说U盘安装时需要修改的参数部分，但实际上我并没修改直接选择第一项安装就顺利完成了）

2.3、修改后，按Ctrl+x执行修改，正常情况下，将进入安装界面，如下图：

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP1550I02.png)




选择想要安装的语言，点继续，如下图：

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP1562K93.png)

> 
2.3.1、时区的选择

> 

如果你安装的是英文版，需要将时区改为上海


2.3.2、键盘

> 

英文键盘和中文键盘布局是一样的！！


2.3.3、语言支持

> 

可同时选择支持多种语言


2.3.4、安装源（可以默认不动）

> 

程序将自动选择，进入可以手动制定，还可以直接指定为网络位置！！


2.3.5、软件选择（注意）

> 

进入后，可以看到有多个选项，根据需要选择，如下图：

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP15639B1.png)


注意：默认为最小安装，安装后是没有界面的哦！！！！


2.3.6、安装位置（重要）


 这里我没有截图，下图来自网络，与实际有些不同

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP1564U63.jpg)

> 

1、本地标准磁盘 项中，应该为 本地磁盘 和 U盘 ，选择本地硬盘

2、其他存储选项 中，选择 我要配置分区
**3、注意： 最下面的 完整磁盘摘要以及引导程序 ，打开，选择不添加引导（后面再添加），不知道是我这里的问题还是共性问题，先选择 安装引导 下一步将出错！！！，最后点击完成，进入如下界面**

**这里面的“引导程序”是一个坑，安装Centos6.4的时候是可以选择引导程序安装到哪个分区的，但是Centos7不能添加引导到默认的分区，因此安装Centos7会覆盖掉windows的MBR引导程序，所以安装Centos7后需要为Windows添加Grub引导，也是很麻烦的一件事儿、**

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP15P2628.png)


注意：这里的分区最好选择
 为标准分区。因为：在用grub引导时，无法挂载LVMPV分区，根本不识别！！ 最好不要用自动创建


1、点击 + 号，分别添加 swap 和 /  两个分区

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP15I3544.png)


大小 ： swap 一般为内存的两倍


             /  为剩余的所有空间



这步需要格外注意：

（1）在选择自动创建分区时，分区信息将不能在更新，（这可能是我自己的问题，等待大家测试）

完成后，点完成，返回配置摘要界面

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP15929162.png)



![](http://www.centoscn.com/uploads/allimg/140728/1-140HP15940593.png)




注意：此时，我们再选择安装位置项，将上面没添加的分区引导一项选上，直接点完成，直到返回配置界面！！

2.3.6、网络和主机名

> 
默认网络是关着的，可以再此处打开



2.4、最后，点击开始安装，如下：



![](http://www.centoscn.com/uploads/allimg/140728/1-140HP159501U.png)



在安装过程中，可以设置 root 密码 和 新的用户 （安装后半部分不能再修改了！！）等待安装即可！！



注意： 安装过程中，如果密码太简单，需要点击两次完成来确认！

![](http://www.centoscn.com/uploads/allimg/140728/1-140HP20001C0.png)




![](http://www.centoscn.com/uploads/allimg/140728/1-140HP20013328.png)




![](http://www.centoscn.com/uploads/allimg/140728/1-140HP200305W.png)



至此，安装已完成，重启，就剩下引导了！！！！！！！！！



# 为Centos 7添加Windows引导程序




网上盛传多种解决方法，主要的有两种：

①、修改Centos 7的Grub2引导，添加Windows的启动项

②、进PE恢复Windows的mbr引导（当然Centos 7的Grub2引导就没了），再进Windows下使用easyBCD添加Centos 7的Grub2引导

解决方法总结：

CentOS7在安装的时候选择引导程序只能是整个硬盘了。



**网络方案1：**


安装CentOS 7后，原来的Windows 7不会出现在启动项目里，网上查找一番后解决办法是修改grub.cfg文件。因为CentOS 7已采用新式的grub2系统，所以需要进入/boot/grub2目录后使用vim编辑grub.cfg文件。:

将下面这段Windows 7的启动项声明：

```
### BEGIN /etc/grub.d/30_os-prober ###
menuentry 'Windows 7 (loader) (on /dev/sda1)' --class windows --class os $menuentry_id_option 'osprober-chain-XXX' {
insmod part_msdos
insmod ntfs
set root='hd0,msdos1'
chainloader +1
}
### END /etc/grub.d/30_os-prober ###
```


加在### END /etc/grub.d/00_header ###之后。其中的XXX为你的Windows 7系统分区的UUID。

经过此番修改后，重启动后，Windows 7启动项可以看到了，并且也可以成功进入Windows 7。（我这样去操作了但是未成功）
**网络方案②：**

先进入系统。我们可以设置grub2来添加win7启动项。
打开终端，su成root，
vi /etc/grub.d/40_custom
在最后加入以下行
menuentry 'win7'{
set root=(hd0,1)
chainloader +1
}

 添加的位置大家一般也能看出来启动项都是以menuentr开始，就直接放到两个menuentry之间就行了
然后重设grub2，执行以下命令：
grub2-mkconfig -o /boot/grub2/grub.cfg
重启电脑  （按照此步骤操作，Windows引导失败）:


2.安装完成centos7之后，启动项中没有win7启动项，解决方法，修改/boot/grub2/grub.cfg


menuentry 'win7'{

   insmod chain

   insmod ntfs

   set root=(hd0,msdos1)

   chainloader +1

}

menuentry 'win7 search'{

insmod chain

insmod ntfs

search -f /ntldr --set root

chainloader +1

} 
按照此步骤设置后还是Windows引导失败：




得到灵感的地方：grub2-mkconfig
 -o /boot/grub2/grub.cfg



重新检测本地硬盘上是否存在有操作系统，然后自动生成启动菜单。

不需要手动去编辑，而且不用担心手工编辑出错（这样设置后重启还是没有出现Windows的引导，仔细思考什么原因呢？ O ！明白了Linux只能认识Ext3/4，fat32格式，而Windows是NTFS格式磁盘分区，因此Linux的Grub2识别不了NTFS上的操作系统）。




**网络方案3：**




如果你不想重装WIN7的话，建议用原版win7安装盘修复win7引导。具体做法就是制备好原版win7安装盘（不知道非原版可不可以，我没有试过，你可以试试我估计没有多大的问题），选择从安装盘启动，到了某个步骤之后，选择高级修复（注意不要选择重装），然后打开命令行，输入 bootrec /fixmbr 然后回车，重启（如果你没有完全破坏掉你的win7的话，现在就应该可以进入win7了），进入win7。但是有一点还需要注意就是这个引导界面是没有你的linux版本的选项的怎么办呢。很简单，到官网(http://neosmart.net/EasyBCD/)上下载easyBCD软件，我在这里附上我下载的，安装后选择“添加新条目”，接下来“linux/BCD”,选择你你放置linux引导程序的位置，按照你的情况你应该是安装在驱动器0上的，选好以后，点击“添加条目”，然后就OK了。PS：easyBCD的功能非常的强大，不要乱搞，要不然损失你是不知道了。

或者：

用PE启动系统，然后进入WIN下的命令行，进入C盘符，用fdisk
 /mbr来擦除。这样只能进入win，而不能引导linux了。然后使用第三方软件EASY-BCD，可以重建多重引导系统。


老毛桃在PE也能修复win的MBR引导和Linux的grub2引导。




（以上两种方案未做尝试，不过安照原理应该能成功）







**最终解决方案④：**



**用grub引导win7，在用u盘装完Centos7后再安装一下ntfs-3g，然后在root下执行**

**grub2-mkconfig -o /boot/grub2/grub.cfg过程中会发现win7 loader，开机重启即可。**



## **[linux下ntfs硬盘的加载](http://blog.csdn.net/qingzilantu/article/details/6313592)（安装NTFS-3G）**



问题： 

      # mount –t ntfs /dev/sdb1 /mnt/

      mount: unknown filesystem type ‘ntfs’ 

      这是由于CentOS release 5.5(Final)上无法识别NTFS格式的分区。 


解决办法： 

      通过使用 ntfs-3g 来解决。 

      打开ntfs-3g的下载点[http://www.tuxera.com/community/ntfs-3g-download/](http://www.tuxera.com/community/ntfs-3g-download/) ，将最新稳定(当前最新版本为[ntfs-3g-2011.1.15](http://tuxera.com/opensource/ntfs-3g-2011.1.15.tgz) ）下载到CentOS，执行以下命令安装： 

      1) 编译安装 ntfs-3g：


# tar zxvf  ntfs-3g-2011.1.15.tgz


# cd ntfs-3g-2011.1.15


#./configure


#make


#make install


已经安装完成啦！！

1. mount命令
`mount -t 文件格式 -o 选项参数 磁盘分区挂载点目录名`

比如:mount -t vfat -o iocharset=utf8,umask=000 /dev/hda5 /mnt/D

再如:mount -t ntfs -o nls=utf8,umask=000 /dev/hdd1 /mnt/sysc

但到这里出现了问题,首先是我的系统不识别该文件格式NTFS,其次是你会问:"你怎么知道外挂的那个系统盘C就是hdd1?"是阿,请继续...
```

```
>>2.判断哪个分区是NTFS格式，加载NTFS分区；
>>[root@localhost kalon]# /sbin/fdisk -l l

Disk /dev/sda: 120.0 GB, 120034123776 bytes

255 heads, 63 sectors/track, 14593 cylinders

Units = cylinders of 16065 * 512 = 8225280 bytes

Sector size (logical/physical): 512 bytes / 512 bytes

I/O size (minimum/optimal): 512 bytes / 512 bytes

Disk identifier: 0x21d3ce88



Device Boot      Start         End      Blocks   Id  System

/dev/sda1   *           1          26      204800   83  Linux

Partition 1 does not end on cylinder boundary.

/dev/sda2            5223       14593    75272557+   f  W95 Ext'd (LBA)

/dev/sda3              26        4700    37544960   83  Linux

/dev/sda4            4700        5222     4194304   82  Linux swap / Solaris

/dev/sda5            5223       14593    75272526    7  HPFS/NTFS

Partition table entries are not in disk order

Disk /dev/sdb: 160.0 GB, 160041885696 bytes

255 heads, 63 sectors/track, 19457 cylinders

Units = cylinders of 16065 * 512 = 8225280 bytes

Sector size (logical/physical): 512 bytes / 512 bytes

I/O size (minimum/optimal): 512 bytes / 512 bytes

Disk identifier: 0xf0b1ebb0

Device Boot      Start         End      Blocks   Id  System

/dev/sdb1   *           1        2611    20972826    7  HPFS/NTFS

/dev/sdb2            2612       19456   135307462+   f  W95 Ext'd (LBA)

/dev/sdb5            2612        5875    26218048+   7  HPFS/NTFS

/dev/sdb6            5876       15539    77626048+   7  HPFS/NTFS

/dev/sdb7           15540       19456    31463271    7  HPFS/NTFS


.............................................................................................................

现在进入NTFS的分区的加载过程；通过上面的查看，我们知道/dev/sda5是NTFS格式的；

1］我们要建一个挂载点的目录，比如是在/mnt/目录下建一个win1的目录：
[root@localhost kalon]# mkdir /mnt/win1

2］把/dev/ sda5 挂载到 /mnt/win1中，这样我们如果查看/dev/ sda5磁盘内容时，就会在/mnt/win1中找得到；
`[root@localhost kalon]# `mount -t ntfs-3g -o nls=utf8,umask=000 /dev/sda5 /mnt/win1
```

```
注： -t ntfs 说明文件系统的格式是ntfs的；-o是选项 ，nls=utf8 ，意思是Native Language Support（本地语言支持）是utf8，也就是大家常说的语言编码,也可以换成iocharset=utf8；Fedora core 4.0 默认的语言编码就是utf8的，这样能显示出中文来；umask=000表示所有的用户组都可读可写，但因为NTFS在Linux中写入不安全，所以只能 读，不能写入;

是不是挂载好了呢？....
2.dh -lh 磁盘使用情况命令 

>>[root@localhost kalon]# df -lh


文件系统              容量  已用  可用 已用%% 挂载点

/dev/sda3              36G  4.4G   30G  13% /

tmpfs                1012M  1.7M 1010M   1% /dev/shm

/dev/sda1             194M   24M  161M  13% /boot


/dev/sda5              72G  1.4G   71G   2% /mnt/win1

..........................说明已挂载好,可以cd /mnt/sysc/去看看了,余下的我就不多说了 .................................

............................................................................................................................................................

>>开机分区自动挂载:应把一下命令放入/etc/fstab中

/dev/sda5 /mnt/win1ntfs-3g umask=000,iocharset=utf8 0 0

>>cp -r -f srcDir dstDir //复制目录文件

>>rm -rf dstDir //删除目录文件

>>.......
2.umount命令


如果想卸载已经挂载好的目录或盘符，可输入以下命令：

[root@localhost kalon]# umount  /mnt/win1](https://so.csdn.net/so/search/s.do?q=Centos 7引导Win7&t=blog)](https://so.csdn.net/so/search/s.do?q=安装Centos7&t=blog)




