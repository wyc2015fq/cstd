# linux -- 串口调试总结 - DoubleLi - 博客园






linux 串口输出调试


|在某些情况下，需要同时对两台或多台Linux主机进行管理和操作。如果手头缺少足够多的键盘和显示器，那么通过一台机器的串口对其余主机进行控制不失为一种快捷、有效的方法。下面就以两台主机为例，简单介绍一下配置方法。假设这两台主机分别为A和B，它们都运行Red Hat 9.0。**A****主机配置**A主机要选择一个合适的串口通信工具，本文中使用Linux自带的Minicom。以root身份登录，运行如下命令：|# minicom -s||----|屏幕上出现Minicom的主配置选单，移动键盘的方向键，选择“Serial Port Setup”选单项，会出现如下配置项：|A - Serial Device : /dev/ttyS0　　B - Lockfile Location : /var/lock　　C - Callin Program :　　D - Callout Program :　　E - Bps/Par/Bits : 9600 8N1　　F - Hardware Flow Control : No　　G - Software Flow Control : No　　Change which setting?||----|按照需要配置如下参数：◆ 串口设备 /dev/ttyS0；◆ 波特率 9600；◆ 帧格式 8N1（表示有8位数据、0位校验和1位停止位）；◆ 软、硬件流控 No。注意，串口设备号可以根据所连接的端口进行更改，比如，连接到COM2口就设置为/dev/ttyS1。波特率的设定要与B主机一致。配置完成后回到主配置选单，选择“Modem and Dialing”选单项，清除掉“A - Init string”、“B - Reset string”和“K - Hang-up string”选项的值。然后重新返回主选单，选择“Save As Dfl”选单项，保存为缺省设置。选择“Exit from Minicom”退出后，用如下命令重新启动Minicom：|# minicom||----|**B****主机配置**B主机需要修改以下几个文件。1.修改/etc/inittab文件该文件是init程序的配置文件，用于设置缺省状态和终端连接。在该文件中增加如下一行，用来初始化串口终端：|tty:2345:respawn:/sbin/agetty ttyS0 9600 vt100||----|“tty”为该行ID；“2345”指该行的运行级别是2、3、4、5级；“respawn”使命令退出后再执行一次，以便其它用户能够登录；“/sbin/agetty ttyS0 9600 vt100”表示具体的命令，该命令通过/sbin/agetty程序打开串口/dev/ttyS0（COM1），波特率设置为9600bps，终端模式为vt100。重启主机B后，在A主机中会显示B主机的登录界面。但是，此时还有两个问题：一个是无法在A主机以root身份登录B；另一个是A中无法显示B主机启动和关闭过程的相关提示信息。后者是因为Linux启动时的信息都是由内核显示的，默认输出到系统主控制台（tty1）。要解决这两个问题，还需要修改/etc/securetty文件和/etc/lilo.conf文件。2.修改/etc/securetty文件该文件是一个被允许以root身份登录的tty设备列表，这些设备由/bin/login程序读取。为了使用户能以root身份通过串口登录，需要在该文件中添加“ttyS0”，告诉系统COM1是安全的。3.修改/etc/lilo.conf文件本文假设系统使用LILO启动。为了使LILO和内核的启动信息能够输出到串口，必须修改/etc/lilo.conf文件，在“linear”一行后添加“serial=0,9600n8”，把“append="root=LABEL=/"”改为“append="root=LABEL=/ console=ttyS0,9600"”。serial选项使LILO的启动信息输出到串口，以便选择不同的系统或内核进行启动。其中，“0”表示串口使用COM1；“9600”表示波特率为9600bps；“n”表示无校验位；“8”表示有8位数据位。“append="root=LABEL=/ console=ttyS0,9600"”的作用是向内核传递参数，使系统启动信息输出到串口COM1，波特率设为9600bps。修改完成后，重新运行一下LILO命令使配置生效。至此，把键盘和显示器等外设连接在主机A上，主机A再通过串口连接主机B，就可以对B进行管理和控制了。所需的硬件就是一条串口线，当然还要求两台主机都具备串口。利用串口终端作为Linux控制台，可以免去额外的键盘，显示卡和显示器，同时可将Linux主机作为一个任意用途的嵌入式黑匣。 将串口终端连接到计算机的串口上并不困难，可以参考Linux的HOWTO文档和以及inittab和agetty的帮助信息。这里扼要地说一下。 　　首先，准备好一根null modem 电缆. 　　其次，在文件/etc/inittab 增加下面一行。[注：如果你不采用 agetty程序，采用其他的程序如like getty_ps ，应用正确的命令语法] 　　 ID:RUNLEVELS:respawn:/sbin/agetty -L SPEED TTY TERM 　　这里： ID =两字母的标识符，如s1或s2。 　　RUNLEVELS = 终端激活的运行级别 　　SPEED = 串口端口速率 　　TTY = 串口的设备名 　　TERM = TERM环境变量 　　范例如下： 　　s2:12345:respawn:/sbin/agetty -L 9600 ttyS1 vt100 　　表示串口 /dev/ttyS1 (COM2 )速率为 9600 bps，终端模式为vt100。 　　最后，重新启动机器。 　　如正确地按照上述三步进行，则就可以在终端屏幕上出现Login: 的提示符。你可以登录进系统，并能象在实际的控制台上或从远程Telnet登录一样进行工作。 　　下面简单介绍一下如何终端设置成控制台，主要涉及内核信息、启动脚本信息和LILO信息。 中国网管联盟 **一、内核信息**　　系统在启动时显示的信息总是输出到主控制台(tty1)。打开机器后，你只有等待Login: 出现在终端屏幕上，这意味着所有启动信息都无法获悉。你只有登录后用dmesg命令查看，但通常是想在login shell起来前看到这些信息。 　　还有其他信息出现在控制台上：/etc/rc.d目录下脚本命令执行时，启动和终止机器时运行的脚本命令等输出的信息。如果信息没有出现在屏幕上，怎样真正地知道"系统已终止"呢？ 　　你必须修改源码/usr/src/linux/drivers/char/console.c[必须已安装了内核源码]，这不是一个复杂得内核修改，按照下面三步进行： 　　首先，在程序前定义CONFIG_SERIAL_ECHO 　　#define CONFIG_SERIAL_ECHO 　　其次，修改串口地址 (仅当你使用得端口不同于默认定义的才有必要修改)。 　　#define SERIAL_ECHO_PORT 0x3f8 /* COM1 */ 　　或者： 　　#define SERIAL_ECHO_PORT 0x2f8 /* COM2 */ 　　 第三，重新编译内核[请参考相应的手册]，启动机器。在系统检测硬件设备时，你应该在终端屏幕上看到信息。 　　 请注意 ：console.c 补丁除了Alpha平台外，对所有的Linux 端口都是必要的。在Alpha平台上它是在运行make config ，选择下面的选项完成的：www_bitscn_com　　 Echo console messages on /dev/ttyS1 **二、/etc/rc.d/rc.*启动脚本信息**　　为了将这些信息显示在终端上，可以将这些文件中含有echo命令的行追加" > TTY "。 TTY 是终端的串口(与/etc/inittab 中串口终端行的一样)。 **三、 LILO 配置**　　如果想选择两个内核之一启动，你必须修改LILO 配置文件，/etc/lilo.conf。配置LILO，使提示信息出现在终端上，可以参考/usr/doc/lilo/README 文件 (查看SERIAL选项)。 这里给出两步正确设置的步骤： 　　首先，编辑/etc/lilo.conf file ，在BOOT选项行后，插入一个SERIAL选项 。 serial=SERIAL_LINE,SPEED PARITY BITS 　　这里： 　　SERIAL_LINE = 0 (串口1) 　　 　　　　　　 1 (串口2) 　　　　　　　　　2 (串口3) 　　　　　　　　　3 (串口4) 　　SPEED = 串口速度 　　PARITY = n (=无) 　　　　　　 o (= 奇校验) 　　　　　　 e (= 偶校验) 　　BITS = 数据位(8 or 7) 　　请注意：在SPEED, PARITY 和BITS参数间没有空格。这些参数必须与在terminal 设置时的参数一样。下面是LILO 配置的示例： 　　serial=1,9600n8 　　这一行表示COM2 ，速率9600bps，无校验位，数据位8。 www.bitsCN.com 　　第二，运行lilo 命令，刷新系统配置。 　　利用SERIAL 选项， LILO 在启动默认内核前，设置了2秒的延迟 。在这期间，你可以 在终端上按"SHIFT"键发送一个终止信号，终止boot进程，并取得LILO提示信息。 　　完成上述配置后，你的终端就可以作为一个控制台了。有一件事不能做的是用CTRL-ALT-DEL 重启动系统。grub 串口2007-03-23 20:03|t1:2345:respawn:/sbin/agetty -L ttyS0 115200 vt220default=0timeout=2serial --unit=0 --speed=115200 --word=8 --parity=no --stop=1terminal --timeout=5 serial consolesplashimage=(hd0,0)/grub/splash.xpm.gztitle Red Hat Linux (2.4.20-8) monitor         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/ console=ttyS0 console=tty0         initrd /initrd-2.4.20-8.imgtitle Red Hat Linux (2.4.20-8) serial console         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/ console=tty0 console=ttyS0         initrd /initrd-2.4.20-8.imgtitle Red Hat Linux (2.4.20-8)         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/         initrd /initrd-2.4.20-8.img||----|# 如何在红帽企业Linux下设置串口终端？2007-9-25 17:59:00[查看学习心得](http://edu.cnzz.cn/NewsComment/4303.aspx)在某些情况下，出于调试的目的需要给linux配置串口控制台，或者linux根本没有显卡，也需要配置串口终端。一个串口控制台将发送所有的终端显示到串口，而串口终端则可以实现通过串口登录到系统。你可以同时设置两个或者其中一个。为了控制内核输出所有控制台消息到串口，你需要在时向内核传递参数console=ttyS0,这可以通过GRUB来实现，下面的例子会将控制台消息发送到tty0（显示器控制台）和ttyS0（串口1），发送到串口控制台的速度为115200，但是需要注意的是一种类型设备只能定义一个控制台，例如不能把消息同时发送到串口1和串口2，编辑/boot/grub/grub.conf，添加如下内容：console=ttyS0,115200 console=tty0例如：# cat /boot/grub/grub.conf# grub.conf generated by anaconda## Note that you do not have to rerun grub after making changes to this file# NOTICE: You have a /boot partition. This means that# all kernel and initrd paths are relative to /boot/, eg.# root (hd0,0)# kernel /vmlinuz-version ro root=/dev/hda2# initrd /initrd-version.img#boot=/dev/hdadefault=0timeout=10splashimage=(hd0,0)/grub/splash.xpm.gztitle Red Hat Enterprise Linux AS (2.4.21-27.0.2.ELsmp) root (hd0,0) kernel /vmlinuz-2.4.21-27.0.2.ELsmp ro root=LABEL=/ console=ttyS0,115200 console=tty0 initrd /initrd-2.4.21-27.0.2.ELsmp.img串口终端 为了设置一个串口终端，需要为串口衍生(spawn)一个agetty，在/etc/inittab添加如下内容： co:2345:respawn:/sbin/agetty ttyS0 115200 vt100 init q 例如：# Run gettys in standard runlevels1:2345:respawn:/sbin/mingetty tty12:2345:respawn:/sbin/mingetty tty23:2345:respawn:/sbin/mingetty tty34:2345:respawn:/sbin/mingetty tty45:2345:respawn:/sbin/mingetty tty56:2345:respawn:/sbin/mingetty tty6co:2345:respawn:/sbin/agetty ttyS0 115200 vt100 init q也可以在/etc/securetty文件中添加串口设置，这样才可以以root身份从串口登录，在该文件最后添加一行，内容为ttyS0(如果不添加此行，则登录时会不能输入密码。)|# minicom -s|A - Serial Device : /dev/ttyS0　　B - Lockfile Location : /var/lock　　C - Callin Program :　　D - Callout Program :　　E - Bps/Par/Bits : 9600 8N1　　F - Hardware Flow Control : No　　G - Software Flow Control : No　　Change which setting?|# minicom|tty:2345:respawn:/sbin/agetty ttyS0 9600 vt100|t1:2345:respawn:/sbin/agetty -L ttyS0 115200 vt220default=0timeout=2serial --unit=0 --speed=115200 --word=8 --parity=no --stop=1terminal --timeout=5 serial consolesplashimage=(hd0,0)/grub/splash.xpm.gztitle Red Hat Linux (2.4.20-8) monitor         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/ console=ttyS0 console=tty0         initrd /initrd-2.4.20-8.imgtitle Red Hat Linux (2.4.20-8) serial console         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/ console=tty0 console=ttyS0         initrd /initrd-2.4.20-8.imgtitle Red Hat Linux (2.4.20-8)         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/         initrd /initrd-2.4.20-8.img|
|----|----|----|----|----|----|
|# minicom -s| | | | | |
|A - Serial Device : /dev/ttyS0　　B - Lockfile Location : /var/lock　　C - Callin Program :　　D - Callout Program :　　E - Bps/Par/Bits : 9600 8N1　　F - Hardware Flow Control : No　　G - Software Flow Control : No　　Change which setting?| | | | | |
|# minicom| | | | | |
|tty:2345:respawn:/sbin/agetty ttyS0 9600 vt100| | | | | |
|t1:2345:respawn:/sbin/agetty -L ttyS0 115200 vt220default=0timeout=2serial --unit=0 --speed=115200 --word=8 --parity=no --stop=1terminal --timeout=5 serial consolesplashimage=(hd0,0)/grub/splash.xpm.gztitle Red Hat Linux (2.4.20-8) monitor         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/ console=ttyS0 console=tty0         initrd /initrd-2.4.20-8.imgtitle Red Hat Linux (2.4.20-8) serial console         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/ console=tty0 console=ttyS0         initrd /initrd-2.4.20-8.imgtitle Red Hat Linux (2.4.20-8)         root (hd0,0)         kernel /vmlinuz-2.4.20-8 ro root=LABEL=/         initrd /initrd-2.4.20-8.img| | | | | |


# 如何在红帽企业Linux下设置串口终端？

2007-9-25 17:59:00[查看学习心得](http://edu.cnzz.cn/NewsComment/4303.aspx)





在某些情况下，出于调试的目的需要给linux配置串口控制台，或者linux根本没有显卡，也需要配置串口终端。一个串口控制台将发送所有的终端显示到串口，而串口终端则可以实现通过串口登录到系统。你可以同时设置两个或者其中一个。

为了控制内核输出所有控制台消息到串口，你需要在时向内核传递参数console=ttyS0,这可以通过GRUB来实现，下面的例子会将控制台消息发送到tty0（显示器控制台）和ttyS0（串口1），发送到串口控制台的速度为115200，但是需要注意的是一种类型设备只能定义一个控制台，例如不能把消息同时发送到串口1和串口2，编辑/boot/grub/grub.conf，添加如下内容：


console=ttyS0,115200 console=tty0

例如：


# cat /boot/grub/grub.conf# grub.conf generated by anaconda## Note that you do not have to rerun grub after making changes to this file# NOTICE: You have a /boot partition. This means that# all kernel and initrd paths are relative to /boot/, eg.# root (hd0,0)# kernel /vmlinuz-version ro root=/dev/hda2# initrd /initrd-version.img#boot=/dev/hdadefault=0timeout=10splashimage=(hd0,0)/grub/splash.xpm.gztitle Red Hat Enterprise Linux AS (2.4.21-27.0.2.ELsmp) root (hd0,0) kernel /vmlinuz-2.4.21-27.0.2.ELsmp ro root=LABEL=/ console=ttyS0,115200 console=tty0 initrd /initrd-2.4.21-27.0.2.ELsmp.img

串口终端 为了设置一个串口终端，需要为串口衍生(spawn)一个agetty，在/etc/inittab添加如下内容： co:2345:respawn:/sbin/agetty ttyS0 115200 vt100 init q 例如：

# Run gettys in standard runlevels1:2345:respawn:/sbin/mingetty tty12:2345:respawn:/sbin/mingetty tty23:2345:respawn:/sbin/mingetty tty34:2345:respawn:/sbin/mingetty tty45:2345:respawn:/sbin/mingetty tty56:2345:respawn:/sbin/mingetty tty6co:2345:respawn:/sbin/agetty ttyS0 115200 vt100 init q

也可以在/etc/securetty文件中添加串口设置，这样才可以以root身份从串口登录，在该文件最后添加一行，内容为


ttyS0


(如果不添加此行，则登录时会不能输入密码。)










