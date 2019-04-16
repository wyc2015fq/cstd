# Linux文件系统中各个目录的作用 - 一个跳popping的quant的博客 - CSDN博客





2018年09月02日 21:34:46[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：977








转自：

[https://blog.csdn.net/hmh13548571896/article/details/78312518](https://blog.csdn.net/hmh13548571896/article/details/78312518)

[https://www.cnblogs.com/yinheyi/p/6279974.html](https://www.cnblogs.com/yinheyi/p/6279974.html)

[https://blog.csdn.net/qq_15766181/article/details/80755786](https://blog.csdn.net/qq_15766181/article/details/80755786)



先直观看一下linux的根目录下的文件有哪些

![](https://img-blog.csdn.net/20180902202554692?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**Linux各个目录的作用及内容**

1）根目录“/”

> 
根目录位于目录结构的最顶层，用斜线（/）表示，类似于Windows操作系统的“C:\“，包含[Fedora](http://www.linuxidc.com/topicnews.aspx?tid=5)操作系统中所有的目录和文件。


2）/bin

> 
bin是**binary**的缩写。目录又称为二进制目录，包含了那些供**系统管理员和普通用户使用的重要linux命令的二进制映像**。该目录存放的内容包括各种可执行文件，还有某些可执行文件的符号连接。常用的命令有：cp、dmesg、kill、login、rm、ping、chomd、bash、cat、echo、ls、 mail、vi等。


3）/boot

> 
目录存放系统核心文件以及启动时必须读取的文件，包括Linux内核的二进制映像。


4）/dev

> 
dev是**device**的缩写。目录保存着外部设备代码的文件，这些文件比较特殊，实际上它们都指向所代表的外围设备，如终端、磁盘驱动器、光驱、打印机等。你可以访问这些外部设备，与访问一个文件或一个目录没有区别。其中该目录下有：
- 子目录/dev/cdrom表示光驱目录；
- 子目录/dev/console表示控制台；
- 子目录 /dev/fd表示软驱；
- 子目录/dev/hd表示硬盘上的一个分区；
- lp0表示打印机；
- ttyS0表示系统的串口设备；
- dsp表示系统的音箱设备；

例如在系统中键入“cd /dev/cdrom”，就可以看到光驱中的文件；键入“cd /dev/mouse”即可看鼠标的相关文件。


/cdrom 该目录在刚安装系统时是空的，你可以将光驱文件系统挂在这个目录下， 例如“mount /dev/cdrom /cdrom”。

5）/etc

> 
目录是整个Linux系统的中心，其中包含所有系统管理和维护方面的配置文件，如dhcpd.conf、host.conf、logrotate.conf、man.config、mke2fs.conf、modprobe.conf、resolv.conf、sysctl.conf、syslog.conf（或rsyslog.conf）、xinetd.conf和yum.conf等，其他的配置文件分别位于单独的子目录中。

**通常应注意备份这个目录中的重要配置文件**，以便需要是能够快速的恢复系统。用于存放系统的配置文件和特定主机的相关文件。例如，记录用户帐号名称的 password 文件、投影密码的 shadow 文件等。该目录不包含任何二进制文件，**并且该目录下的所有文件主要由管理员使用，普通用户只对其具有阅读权限**。

该目录还包含一些网络配置文件、文件系统、 x系统配置文件、设备配置信息、设置用户信息等，如：

 /etc/cron.d  

用于存储cron进程调度运行后台进程所用的配置和控制文件。其他目录包括/etc/crontab文件定义的cron.hourly、cron.daily、cron.weekly和cron.monthly等四个目录。

/etc/cups

用于存储通用UNIX打印系统（Commom UNIX Printing System，CUPS）使用的各种配置文件。

/etc/default

其中的文件用于提供部分工具软件（如useradd程序）使用的变量及其默认值。

/etc/httpd  

Apache配置文件的根目录。Apache是一个通用的、高性能的HTTP服务器，也是世界上最流行的Web服务器。Apache采用模块化的设计方式，支持运行时的动态模块选择、虚拟主机，以及服务进程数量的动态调整等。

/etc/init.d   

用于存储进入相应运行级时需要由init调度执行的脚本文件。在Fedora Linux系统中，这只是一个符号链接文件，实际的目录应为/etc/rc.d/init.d。

/etc/ipsec.d   

用于存储IPSee使用的配置文件等。

/etc/kde     

其中含有部分KDE初始化文件和KDM配置文件。

/etc/pki  

用于存储各种密匙，如用于安装软件包的密匙等。


/etc/ppp   

用于存储PPP的脚本和配置文件。


/etc/profile.d 

用于存储/etc/profile等使用的辅助初始化文件，如lang.sh脚本文件等。


/etc/rc.d  

用于存储进入相应运行级是由init进程调度执行的脚本文件。其中含有init.d和rcN.d等子目录（其中的N为0、1、2、3、4、5和6，表示系统的运行级）。


/etc/samba   

Samba配置文件的根目录。Samba是一个网络共享软件的总称，Linux系统中实现的SMB协议，允许Linux系统为Windows系统提供文件和打印共享服务。

/etc/security   

用户存储的基本安全控制文件，包括注册控制文件、控制访问控制文件，以及资源限制控制文件等。

/etc/selinux   

SElinux(Security Enhanced Linux，由美国国家安全部(National Security Agency)领导开发的GPL项目，它拥有一个灵活而强制性的访问控制结构，旨在提高Linux系统的安全性，提供强健的安全保证，可防御未知攻击，据称相当于B1级的军事安全性能。比MSNT所谓的C2等高得多。应用SELinux后，可以减轻恶意攻击或恶意软件带来的灾难，并提供对机密性和完整性有很高要求的信息很高的安全保障。)配置文件的根目录。


/etc/skel   

其中存有默认的初始化文件，如.bash_logout、.bash_profile、.bashrc、.emacs、.kde和.zshrc等。每当新增一个新用户时，系统将会把其中的部分初始化文件复制到用户的主目录中。注意：上述文件均为隐藏文件。(即以“.“开头的文件名)

/etc/ssh   

这个目录含有系统配置过程，以及系统引导过程中需要用到的各种配置文件，也是chkconfig维护的各种后台服务进程配置文件的根目录。例如，其中的clock文件包含系统的时区设置，keyboard文件包含键盘的类型定义，init文件包含系统引导过程使用的参数定义等。

/etc/tomcat5   

tomcat的根目录

/etc/vsftpd   

包含FTP服务器的配置文件，其中包括vsftpd.conf、ftpusers和user_list等重要文件。

/etc/xinetd.d   

其中包含xinetd服务进程控制的所有传统网络服务使用的配置文件，尤其包含telnet等网络应用的配置文件。在Linux系统中，原有的inetd已由功能更强的xinetd服务进程取代，原有的inetd.conf配置文件也由xinetd.d目录中的一系列单独的配置文件取代。

/etc/yum   

其中包含yum软件更新工具使用的配置文件。

/etc/yum.repos.d

其中包含每个软件仓库的配置文件。


6) /home

> 
用户的主目录，在Linux中，每个用户都有一个自己的目录，一般该目录名是以用户的账号命名的。


7）/lib

> 
目录下存放必要的运行库，主要是编程语言的库。典型的 Linux 操作系统中包含了C、C++和 Fortran 的库文件。用这些语言开发的应用程序可以使用这些编程语言库文件。这使软件开发者能够利用那些预先写好并通过测试的函数。库文件包含了标准的C库/lib/libc.so.*，数学库libm.so.*，共享的动态链接库/lib/ld/so以及目录/bin和/sbin下用到的其他共享库。/lib/modules目录存放系统的核心模块，某些可被模块化的部分并不需要在编译系统核心时放入核心本体，避免本体过于庞大而导致效率降低。




8）/lost+found

> 
该目录存放所有和其它目录都没有关联的文件。系统出现错误或发生问题时，Fedora会自动扫描磁盘驱动器，修正错误，如果找到遗失或错误的片段，将这些片段转化成文件存放于此，等待管理员的进一步处理。




9) /media

> 
Linux系统会自动识别一些设备，例如U盘、光驱等等，当时别后，Linux会把识别的设备挂载到这个目录下。




10）/mnt

> 
该目录是默认的文件系统临时装载点，这是一个通用的安装点，可以临时安装任何文件系统或远程资源。系统管理员执行 mount命令完成装载工作。在系统中，该目录包含了光驱、磁盘和软驱的挂载点。




11) /opt

> 
这是给主机额外安装软件所摆放的目录。比如你安装一个Oracle数据库就可以放到这个目录下。默认是空的。

目录用来安装附加软件包，用户调用软件包程序放在目录/opt/package_name/bin下，package_name是安装软件包的名称。




12）/proc

> 
**process**的缩写。进程文件系统proc的根目录，**其中的部分文件分别对应正在运行的进程**，可用于访问当前进程的地址空间。它是一个非常特殊的虚拟文件系统，其中并不包含“实际的”文件，而是可用以引用当前运行系统的系统信息，如CPU、内存、运行时间、软件配置以及硬件配置的信息，这些信息是在内存中由系统自己产生的。

/proc/net   

其中的文件分别表示各种网络协议（如TCP、UDP以及ARP等）的状态与统计信息。

/proc/sys   

这个目录不仅存有各种系统信息，而且也包含系统内核与TCP/IP网络的可调参数。其中的kernel子目录含有共享内存和消息队列的可调参数，net子目录中含有TCP/IP的各种可调参数。例如，shmmax文件中含有系统的最大共享内存定义，如果使用“echo somevalue >/proc/sys/kernel/shmmax”命令，可以直接修改运行系统的内核参数，而无需重新引导系统。这一做法要谨慎，有的文件可能包含多个数值，或不同类型的数值，因此，在修改以前一定要弄清参数的意义和实际的数值。为了在每次启动系统时都能使用定制的系统可调参数，可以设置sysctl.conf配置文件，或编写自己的Shell启动脚本。






13）/root

> 
超级用户root的主目录（在Linux系统中，**斜杠字符“/”是整个系统的根目录，而非超级用户的主目录。**）




14）/sbin

> 
s就是super user的意思，这里存放的是系统管理员使用的系统管理程序。

目录/sbin、/usr/sbin和/usr/local/sbin存放了该目录启动系统时需执行的程序，如管理工具、应用软件和通用的根用户权限命令等内容。如包含getty、init、update。




15）/usr

> 
/usr:这是个**最庞大的目录,**我们要用到的很多应用程序和文件几乎都存放在这个目录了。具体来说:

/usr/X11R6:存放X-Windows的目录。

/usr/bin:存放着许多应用程序.

/usr/sbin:给超级用户使用的一些管理程序就放在这.

/usr/doc:这就是Linux文档的大本营.

/usr/include:Linux下开发和编译应用程序需要的头文件在这里找.

/usr/lib:存放一些常用的动态链接共享库和静态档案库.

/usr/local:这是提供给一般用户的/usr目录,在这安装软件最适合.

/usr/man:是帮助文档目录.

/usr/src:Linux开放的源代码,就存在这个目录,爱好者们别放过哦!

/usr/X11R6 存放X-Windows的目录；

/usr/games     存放着XteamLinux自带的小游戏；

/usr/bin       用户和管理员的标准命令；

/usr/sbin   存放root超级用户使用的管理程序；

/usr/doc     Linux技术文档；

/usr/include  来存放Linux下开发和编译应用程序所需要的头文件，for c 或者c++；

/usr/lib    应用程序和程序包的连接库；

/usr/local  系统管理员安装的应用程序目录；

/usr/man   帮助文档所在的目录；

/usr/src　 Linux开放的源代码；

/usr/share      用于存放一些共享的数据，比如音乐文件或者图标等等




14) /var   

> 
/var--------- 用于存放很多不断变化的文件，例如日志文件等。包含了日志文件、计划性任务和邮件等内容。　　　　　　

/var/cache   应用程序缓存目录；

/var/crash   系统错误信息；

/var/games 游戏数据；

/var/log   日志文件；

/var/mail 电子邮件；

/var/tmp   临时文件目录；




ps:如何选择安装软件的位置

Linux 的软件安装目录是也是有讲究的，理解这一点，在对系统管理是有益的

`/usr`：系统级的目录，可以理解为`C:/Windows/`，`/usr/lib`理解为`C:/Windows/System32`。
`/usr/local`：用户级的程序目录，可以理解为`C:/Progrem Files/`。用户自己编译的软件默认会安装到这个目录下。
`/opt`：用户级的程序目录，可以理解为`D:/Software`，opt有可选的意思，这里可以用于放置第三方大型软件（或游戏），当你不需要时，直接`rm -rf`掉即可。在硬盘容量不够时，也可将/opt单独挂载到其他磁盘上使用。

源码放哪里？
`/usr/src`：系统级的源码目录。
`/usr/local/src`：用户级的源码目录。



-----------------翻译-------------------

/opt

> Here’s where optional stuff is put. Trying out the latest Firefox beta? Install it to /opt where you can delete it without affecting other settings. Programs in here usually live inside a single folder whick contains all of their data, libraries, etc.

这里主要存放那些可选的程序。你想尝试最新的firefox测试版吗?那就装到/opt目录下吧，这样，当你尝试完，想删掉firefox的时候，你就可 以直接删除它，而不影响系统其他任何设置。安装到/opt目录下的程序，它所有的数据、库文件等等都是放在同个目录下面。

举个例子：刚才装的测试版firefox，就可以装到/opt/firefox_beta目录下，/opt/firefox_beta目录下面就包含了运 行firefox所需要的所有文件、库、数据等等。要删除firefox的时候，你只需删除/opt/firefox_beta目录即可，非常简单。



/usr/local

> 
This is where most manually installed(ie. outside of your package manager) software goes. It has the same structure as /usr. It is a good idea to leave /usr to your package manager and put any custom scripts and things into /usr/local, since nothing important normally lives in /usr/local.

这里主要存放那些手动安装的软件，即不是通过“新立得”或apt-get安装的软件。它和/usr目录具有相类似的目录结构。让软件包管理器来管理/usr目录，而把自定义的脚本(scripts)放到/usr/local目录下面，我想这应该是个不错的主意。





