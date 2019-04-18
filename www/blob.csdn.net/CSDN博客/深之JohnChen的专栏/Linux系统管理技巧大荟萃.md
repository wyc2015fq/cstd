# Linux系统管理技巧大荟萃 - 深之JohnChen的专栏 - CSDN博客

2005年12月23日 11:40:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1714


**Linux系统管理技巧大荟萃**为桌面操作系统，Linux的人机界面可真是不敢恭维，但是，作为网络操作系统，其易用性(对于NOS而言)和高性能恐怕是很难有能出其右的。当然，这并不是说它的操作很简单，而是说若能掌握一些技巧，无论是系统设置还是网络管理都能变得轻松自如、随心所欲。 

禁止终端模式下的显示器自动节能 

大部分PC上安装Linux后，在终端模式下也能实现自动关闭显示器的功能，但是假如我们要在显示器上即时显示一些信息，如:网络流量、包分析等，这个功能就变得非常讨厌了。采用修改CMOS和系统属性都无法解决，解决的办法是用setterm，设置终端的命令，如setterm--blank，就能关闭自动节能这一特性。 

将man帮助文档存为普通txt文件 

Man的地位在Linux中可谓至关重要，比Windows里的中看不中用的Help好多了。但是它所显示的信息却不是普通的文本，如果直接将这些文字重定向到一个文本文件，你会发现，在man中高亮显示的文字就变成了两个，而且有不计其数的制表符，这使我们的打印、编辑都变得非常不便。不过使用下面这样一条语句就能得到tcsh命令的普通帮助文本:mantcshcol-b> tcsh.txt，再也不会有那些讨厌的重复字符了。 

IDE硬盘的优化 

Linux可能更加注重数据的安全性而非性能，这对NOS来说是完全必要的，但是打开IDE硬盘的一些有用的优化应该没什么害处吧。看看这个命令:hdparm，它的作用是显示、设置硬盘(包括光驱)的参数。我们可以先用hdparm-i/dev/hda查看hda这块硬盘的参数设置，再根据情况来修改，如:hdparm-c1可将硬盘的I/O读写支持由16位改为32位，hdparm-m16是开启硬盘的MultiCount。在执行这条命令之前最好先用-i参数确定你的硬盘所能支持的最大MultiCount数。主要优化的就是这两项了，其他的不敢确定也别乱来。可以用hdparm-t/dev/had在优化前后测试硬盘性能。 

允许root从远程登录 

Linux的默认安全设置是root不能从远程工作站登录，这使我们对Linux服务器进行远程管理造成了不小的麻烦，/etc/securtty正是控制这一机制的关键文件，在这个配置文件的root小节中加入pts/0、pts/1等你希望root能够登录的终端名就行了。 

恢复被覆盖的MBR(主引导扇区)的Linux引导程序LILO 

如果LILO安装在MBR，那么先装Linux再安装Windows，LILO就会被覆盖，这时我们将无法从硬盘引导Linux，怎么办？我们可以启动到Windows命令行模式，从Linux光盘上拷贝loadlin.exe和vmlinuz两个文件到Windows分区，然后执行loadlinvmlinuzroot=/dev/had？rw。这条命令的作用是从指定的分区启动Linux，并将根分区(1)设为读写模式，这样就可以修改Linux的大部分设置或者恢复LILO。要注意的是这种情况启动的Linux并不完整，如:MSDOS、VFAT文件系统的支持、网络等都无法使用，不过没关系，只要能这样启动，我们可以利用mkbootdisk来创建Linux启动盘，用这张启动盘启动Linux，就能完全使用Linux了。启动Linux以后，只要将/etc/lilo.conf文件的第一项boot=/dev/hda？改为boot=/dev/hda，然后执行lilo就能恢复从MBR启动Linux。配置文件一览表文件位置及名称作用/etc/rc.d/rc.sysinit系统启动时的公用服务配置/etc/rc.d/rc.local系统启动时的本地服务配置/etc/bashrc、/etc/profilebashshell的公用配置/home/username/.bashrc各用户私人bashshell配置/etc/securetty终端安全设置/etc/hosts主机名与ip地址的对应文件/etc/resolv.confdns服务器配置/etc/squid.conf或/etc/squid/squid.confsquid代理服务器软件配置/etc/smb.conf或/etc/samba/smb.conf基于smb的网上邻居配置/etc/dhcpd.conflinux下的dhcp服务器配置文件/etc/filesystems系统支持的文件系统列表/etc/fstab当前系统中已激活分区的信息/etc/host.conf指定系统搜索dns记录的配置文件/etc/lilo.conf启动程序lilo的配置文件 

Linux系统的大部分控制都是基于配置文件的，了解这些配置文件能更有利于使用Linux。 

将Linux加入到WinNT/2000的启动选单中 

Linux的启动选单不错，可惜不能显示中文，而且不能选择启动NT还是Win98，所以我认为启动选单还是用NT系列的好。其实做起来很简单，安装完所有Windows系列操作系统后再装Linux，并在安装时选择将LILO安装到Linux所在的分区而非MBR，然后利用启动盘启动Linux，将Windows分区mount为/mnt/dos；然后执行ddif=/dev/hda？bs=512count=1of=/mnt/dos/bootsect.lin，这样在Windows分区就有了一个bootsect.lin文件，它记录了Linux分区的启动信息，然后在WindowsNT的启动配置文件boot.ini中加入C:BOOTSECT.LIN="红帽子Linux7.0"，再启动电脑的时候，我们发现，在WindowsNT启动选单中多了"红帽子Linux7.0"这个选项，选择它就能启动我们的Linux。需要注意的是NT在启动时仍然只识别8.3格式的文件，所以不要把bootsect.lin改为长文件名。 

用Squid来控制文件类型的访问 

Linux的一大好处就是能够非常方便地对IP网络进行路由，很多Linux系统就是专门用作Internet接入路由器。但是，iptables或ipchain等包过滤软件只能控制用户访问哪些站点，而无法控制用户访问哪些文件。使用Linux下的代理服务器软件Squid可以很容易地做到这一点，Squid.conf文件中的urlpath_regex项就是控制每种注册的MIME文件访问的，例如以下语句: 

#给出需要控制访问的MIME文件类型(访问控制列表即acl) 

acldenymineurlpath_regex.exe.zip 

#禁止这类文件的访问 

http_accessdenydenymine 

这样就禁止了这条语句以后的所有用户对相应站点zip、exe文件的访问，当然你可以加入其他文件类型，如MP3等。再利用iptables之类的ReDirect重定向功能，强迫用户通过Squid代理上网，即所谓"透明代理"，就可完全控制用户对站点的访问了。 

让系统使用默认的彩色文件显示 

Linux终端下的不同类型文件的彩色显示让我们感觉非常方便，但有时可能不小心丢失这一功能，在配置文件中加入aliasls="ls--color-F-N"就能保证这一功能的正常使用。 

利用cron保持某些分区的激活状态 

cron相当于Win98的计划任务，它能根据用户的要求定时执行某些命令。如果你mount了一些NTFS分区，并经常访问这个分区(如WebServer对一些文件的访问)，很有可能因为时间过长而无法正确读写数据，这时可以利用cron的定时访问功能，保证该分区总是激活的。根据个人经验，网络上的NTFS分区最好让cron每15分钟访问一次，否则就有可能出现需要该分区数据时出现mount失败的情况。

