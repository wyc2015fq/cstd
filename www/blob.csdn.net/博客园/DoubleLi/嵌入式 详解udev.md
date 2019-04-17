# 嵌入式 详解udev - DoubleLi - 博客园






如果你使用Linux比较长时间了，那你就知道，在对待设备文件这块，Linux改变了几次策略。在Linux早期，设备文件仅仅是是一些带有适当的属性集的普通文件，它由mknod命令创建，文件存放在/dev目录下。后来，采用了devfs, 一个基于内核的动态设备文件系统，他首次出现在2.3.46内核中。Mandrake，Gentoo等Linux分发版本采用了这种方式。devfs创建 的设备文件是动态的。但是devfs有一些严重的限制，从2.6.13版本后移走了。目前取代他的便是文本要提到的udev－－一个用户空间程序。

目前很多的Linux分发版本采纳了udev的方式，因为它在Linux设备访问，特别是那些对设备有极端需求的站点(比如需要控制上千个硬盘)和热插拔设备(比如USB摄像头和MP3播放器)上解决了几个问题。下面我我们来看看如何管理udev设备。
实际上，对于那些为磁盘，终端设备等准备的标准配置文件而言，你不需要修改什么。但是，你需要了解udev配置来使用新的或者外来设备，如果不修改配置， 这些设备可能无法访问，或者说Linux可能会采用不恰当的名字，属组或权限来创建这些设备文件。你可能也想知道如何修改RS－232串口，音频设备等文件的属组或者权限。这点在实际的Linux实施中是会遇到的。

为什么使用udev

在此之前的设备文件管理方法(静态文件和devfs)有几个缺点：

*不确定的设备映射。特别是那些动态设备，比如USB设备，设备文件到实际设备的映射并不可靠和确定。举一个例子：如果你有两个USB打印机。一个可能称 为/dev/usb/lp0,另外一个便是/dev/usb/lp1。但是到底哪个是哪个并不清楚，lp0,lp1和实际的设备没有一一对应的关系，因为 他可能因为发现设备的顺序，打印机本身关闭等原因而导致这种映射并不确定。理想的方式应该是：两个打印机应该采用基于他们的序列号或者其他标识信息的唯一 设备文件来映射。但是静态文件和devfs都无法做到这点。

*没有足够的主/辅设备号。我们知道，每一个设备文件是有两个8位的数字：一个是主设备号 ，另外一个是辅设备号来分配的。这两个8位的数字加上设备类型(块设备或者字符设备)来唯一标识一个设备。不幸的是，关联这些身边的的数字并不足够。

*/dev目录下文件太多。一个系统采用静态设备文件关联的方式，那么这个目录下的文件必然是足够多。而同时你又不知道在你的系统上到底有那些设备文件是激活的。

*命名不够灵活。尽管devfs解决了以前的一些问题，但是它自身又带来了一些问题。其中一个就是命名不够灵活；你别想非常简单的就能修改设备文件的名字。缺省的devfs命令机制本身也很奇怪，他需要修改大量的配置文件和程序。

*内核内存使用，devfs特有的另外一个问题是，作为内核驱动模块，devfs需要消耗大量的内存，特别当系统上有大量的设备时(比如上面我们提到的系统一个上有好几千磁盘时)

udev的目标是想解决上面提到的这些问题，他通采用用户空间(user-space)工具来管理/dev/目录树，他和文件系统分开。知道如何改变缺省配置能让你之大如何定制自己的系统，比如创建设备字符连接，改变设备文件属组，权限等。

udev配置文件

主要的udev配置文件是/etc/udev/udev.conf。这个文件通常很短，他可能只是包含几行#开头的注释，然后有几行选项：

udev_root=“/dev/”
udev_rules=“/etc/udev/rules.d/”
udev_log=“err“

上面的第二行非常重要，因为他表示udev规则存储的目录，这个目录存储的是以.rules结束的文件。每一个文件处理一系列规则来帮助udev分配名字给设备文件以保证能被内核识别。
你的/etc/udev/rules.d下面可能有好几个udev规则文件，这些文件一部分是udev包安装的，另外一部分则是可能是别的硬件或者软件包 生成的。比如在Fedora Core 5系统上，sane-backends包就会安装60-libsane.rules文件，另外initscripts包会安装60-net.rules文 件。这些规则文件的文件名通常是两个数字开头，它表示系统应用该规则的顺序。

规则文件里的规则有一系列的键/值对组成，键/值对之间用逗号(,)分割。每一个键或者是用户匹配键，或者是一个赋值键。匹配键确定规则是否被应用，而赋 值键表示分配某值给该键。这些值将影响udev创建的设备文件。赋值键可以处理一个多值列表。匹配键和赋值键操作符解释见下表：

udev 键/值对操作符

操作符        匹配或赋值                             解释
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
==             匹配                        相等比较
!=              匹配                        不等比较
=                赋值                       分配一个特定的值给该键，他可以覆盖之前的赋值。
+=              赋值                       追加特定的值给已经存在的键
:=               赋值                       分配一个特定的值给该键，后面的规则不可能覆盖它。

这有点类似我们常见的编程语言，比如C语言。只是这里的键一次可以处理多个值。有一些键在udev规则文件里经常出现，这些键的值可以使用通配符(*,?,甚至范围，比如[0-9])，这些常用键列举如下：

常用udev键
键                 含义
ACTION                     一个时间活动的名字，比如add，当设备增加的时候
KERNEL                     在内核里看到的设备名字，比如sd*表示任意SCSI磁盘设备
DEVPATH              内核设备录进，比如/devices/*
SUBSYSTEM              子系统名字，比如sound,net
BUS                     总线的名字，比如IDE,USB
DRIVER                     设备驱动的名字，比如ide-cdrom
ID                       独立于内核名字的设备名字
SYSFS{ value}              sysfs属性值，他可以表示任意
ENV{ key}              环境变量，可以表示任意
PROGRAM              可执行的外部程序，如果程序返回0值，该键则认为为真(true)
RESULT                     上一个PROGRAM调用返回的标准输出。
NAME                     根据这个规则创建的设备文件的文件名。注意：仅仅第一行的NAME描述是有效的，后面的均忽略。 如果你想使用使用两个以上的名字来访问一个设备的话，可以考虑SYMLINK键。
SYMLINK              根据规则创建的字符连接名
OWNER                     设备文件的属组
GROUP                     设备文件所在的组。
MODE                     设备文件的权限，采用8进制
RUN                     为设备而执行的程序列表
LABEL                     在配置文件里为内部控制而采用的名字标签(下下面的GOTO服务)
GOTO                     跳到匹配的规则（通过LABEL来标识），有点类似程序语言中的GOTO
IMPORT{ type}           导入一个文件或者一个程序执行后而生成的规则集到当前文件
WAIT_FOR_SYSFS         等待一个特定的设备文件的创建。主要是用作时序和依赖问题。
PTIONS                     特定的选项： last_rule 对这类设备终端规则执行； ignore_device 忽略当前规则； ignore_remove 忽略接下来的并移走请求。all_partitions 为所有的磁盘分区创建设备文件。

我们给出一个列子来解释如何使用这些键。下面的例子来自Fedora Core 5系统的标准配置文件。



KERNEL=="*", OWNER="root" GROUP="root", MODE="0600" 
KERNEL=="tty", NAME="%k", GROUP="tty", MODE="0666", OPTIONS="last_rule"
KERNEL=="scd[0-9]*", SYMLINK+="cdrom cdrom-%k"
KERNEL=="hd[a-z]", BUS=="ide", SYSFS{removable}=="1",
               SYSFS{device/media}=="cdrom", SYMLINK+="cdrom cdrom-%k" 
ACTION=="add", SUBSYSTEM=="scsi_device", RUN+="/sbin/modprobe sg"上面的例子给出了5个规则，每一个都是KERNEL或者ACTION键开头：

*第一个规则是缺省的，他匹配任意被内核识别到的设备，然后设定这些设备的属组是root，组是root，访问权限模式是0600(-rw——-)。这也是一个安全的缺省设置保证所有的设备在默认情况下只有root可以读写

*第二个规则也是比较典型的规则了。它匹配终端设备(tty)，然后设置新的权限为0600，所在的组是tty。它也设置了一个特别的设备文件名:%K。在这里例子里，%k代表设备的内核名字。那也就意味着内核识别出这些设备是什么名字，就创建什么样的设备文件名。

*第三行开始的KERNEL==”scd[0-9]*”,表示 SCSI CD-ROM 驱动. 它创建一对设备符号连接：cdrom和cdrom-%k。

*第四行，开始的 KERNEL==”hd[a-z]“, 表示ATA CDROM驱动器。这个规则创建和上面的规则相同的符号连接。ATA CDROM驱动器需要sysfs值以来区别别的ATA设备，因为SCSI CDROM可以被内核唯一识别。.

*第五行以 ACTION==”add”开始，它告诉udev增加 /sbin/modprobe sg 到命令列表，当任意SCSI设备增加到系统后，这些命令将执行。其效果就是计算机应该会增加sg内核模块来侦测新的SCSI设备。

当然，上面仅仅是一小部分例子，如果你的系统采用了udev方式，那你应该可以看到更多的规则。如果你想修改设备的权限或者创建信的符号连接，那么你需要熟读这些规则，特别是要仔细注意你修改的那些与之相关的设备。

修改你的udev配置

在修改udev配置之前，我们一定要仔细，通常的考虑是：你最好不要修改系统预置的那些规则，特别不要指定影响非常广泛的配置，比如上面例子中的第一行。不正确的配置可能会导致严重的系统问题或者系统根本就无法这个正确的访问设备。

而我们正确的做法应该是在/etc/udev/rules.d/下创建一个信的规则文件。确定你给出的文件的后缀是rules文件名给出的数字序列应该比 标准配置文件高。比如，你可以创建一个名为99-my-udev.rules的规则文件。在你的规则文件中，你可以指定任何你想修改的配置，比如，假设你 修改修改floppy设备的所在组，还准备创建一个信的符号连接/dev/floppy，那你可以这么写：

KERNEL==”fd[0-9]*“, GROUP=“users“,   SYMLINK+=“floppy“

有些发行版本，比如Fedora，采用了外部脚本来修改某些特定设备的属组，组关系和权限。因此上面的改动可能并不见得生效。如果你遇到了这个问题，你就需要跟踪和修改这个脚本来达到你的目的。或者你可以修改PROGRAM或RUN键的值来做到这点。

某些规则的修改可能需要更深的挖掘。比如，你可能想在一个设备上使用sysfs信息来唯一标识一个设备。这些信息最好通过udevinfo命令来获取。

$ udevinfo –a –p $(udevinfo –q path      –n /dev/hda)上面的命令两次使用udevinfo：一次是返回sysfs设备路径(他通常和我们看到的Linux设备文件名所在路径－－/dev/hda－－不同)；第 二次才是查询这个设备路径，结果将是非常常的syfs信息汇总。你可以找到最够的信息来唯一标志你的设备，你可以采用适当的替换udev配置文件中的 SYSFS选项。下面的结果就是上面的命令输出



[root@localhost rules.d]# udevinfo -a -p $(udevinfo -q path        -n      /dev/hda1)
Udevinfo starts with the device specified by the devpath and then walks up the chain of
parent devices. It prints for every device found,all possible attributes in the udev rules
key format. A rule to match, can be composed by the attributes of the device and the
attributes from one single parent device.

looking at device '/block/hda/hda1':     
KERNEL=="hda1"     SUBSYSTEM=="block"     DRIVER==""        
ATTR{stat}=="        1133         2268            2            4"         ATTR{size}=="208782"
ATTR{start}=="63"         ATTR{dev}=="3:1"        looking at parent device '/block/hda':  

KERNELS=="hda"     SUBSYSTEMS=="block"     DRIVERS==""        
ATTRS{stat}=="28905 18814 1234781 302540 34087 133247 849708 981336 0 218340 1283968"
ATTRS{size}=="117210240"         ATTRS{removable}=="0"        
ATTRS{range}=="64"         ATTRS{dev}=="3:0"

looking at parent device '/devices/pci0000:00/0000:00:1f.1/ide0/0.0':    

KERNELS=="0.0"     SUBSYSTEMS=="ide"     DRIVERS=="ide-disk"        
ATTRS{modalias}=="ide:m-disk"         ATTRS{drivename}=="hda"       
ATTRS{media}=="disk"       
looking at parent device '/devices/pci0000:00/0000:00:1f.1/ide0':    

KERNELS=="ide0"     SUBSYSTEMS==""     DRIVERS==""       
looking at parent device '/devices/pci0000:00/0000:00:1f.1':     
KERNELS=="0000:00:1f.1"     SUBSYSTEMS=="pci"     DRIVERS=="PIIX_IDE"        
ATTRS{broken_parity_status}=="0"         ATTRS{enable}=="1"        
ATTRS{modalias}=="pci:v00008086d000024CAsv0000144Dsd0000C009bc01sc01i8a"
ATTRS{local_cpus}=="1"         ATTRS{irq}=="11"         ATTRS{class}=="0x01018a"
ATTRS{subsystem_device}=="0xc009"         ATTRS{subsystem_vendor}=="0x144d"
ATTRS{device}=="0x24ca"         ATTRS{vendor}=="0x8086"       
looking at parent device '/devices/pci0000:00':    

KERNELS=="pci0000:00"     SUBSYSTEMS==""     DRIVERS==""   
举一个例子：假设你想修改USB扫描仪的配置。通过一系列的尝试，你已经为这个扫描仪标识了Linux设备文件(每次打开扫描仪时，名字都会变)。你可以使 用上面的命令替换这个正确的Linux设备文件名，然后定位输出的采用SYSFS{idVendor}行和SYSFS{idProduct}行。最后你可 以使用这些信息来为这个扫描仪创建新的选项。

SYSFS{idVendor}=="0686",   SYSFS{idProduct}=="400e",  
SYMLINK+="scanner", MODE="0664",   group="scanner"上面的例子表示将扫描仪的组设置为scanner，访问权限设置为0664,同时创建一个/dev/scanner的符号连接。

udev-FAQ

问：udev是什么? 它的目的何在?

答：udev是一种工具，它能够根据系统中的硬件设备的状态动态更新设备文件，包括设备文件的创建，删除等。设备文件通常放在/dev目录下。使用udev后，在/dev目录下就只包含系统中真正存在的设备。

问：udev支持什么内核？

答：udev只支持linux-2.6内核，因为udev严重依赖于sysfs文件系统提供的信息，而sysfs文件系统只在linux-2.6内核中才有。

问：udev是一个内核程序还是用户程序？

答：udev是一个用户程序(user-mode daemon)。

问：udev和devfs有什么差别？

答：udev能够实现所有devfs实现的功能。但udev运行在用户模式中，而devfs运行在内核模式中。据称：devfs具有一些不太容易解决的先天缺陷。

问：udev的配置文件放在哪里？

答：udev是一个用户模式程序。它的配置文件是/etc/udev/udev.conf。这个文件一般缺省有这样几项：

udev_root=”/dev” ; udev产生的设备文件的根目录是/dev
udev_db=”/dev/.udevdb” ; 通过udev产生的设备文件形成的数据库
udev_rules=”/etc/udev/rules.d” ;用于指导udev工作的规则所在目录。
udev_log=”err” ;当出现错误时，用syslog记录错误信息。

问：udev的工作过程是怎样的？

答：由于没有研究过udev的源程序，不敢贸然就说udev的工作过程。我只是通过一些网上的资料和udev的说明文档，大致猜测它的工作过程可能是这样的。

当内核检测到在系统中出现了新设备后，内核会在sysfs文件系统中为该新设备生成一项新的记录，一般sysfs文件系统会被mount到/sys目录中。新记录是以一个或多个文件或目录的方式来表示。每个文件都包含有特定的信息。(信息是如何表述的，还要另外研究？)

udev在系统中是以守护进程的方式udevd在运行，它通过某种途径(到底什么途径，目前还没搞懂。)检测到新设备的出现，通过查找设备对应的sysfs中的记录得到设备的一些信息。

udev会根据/etc/udev/udev.conf文件中的udev_rules指定的目录，逐个检查该目录下的文件，这个目录下的文件都是针对某类或某个设备应该施行什么措施的规则文件。udev读取文件是按照文件名的ASCII字母顺序来读取的，如果udev一旦找到了与新加入的设备匹配的规则，udev就会根据规则定义的措施对新设备进行配置。同时不再读后续的规则文件。

问：udev的规则文件的语法是怎样的？

答：udev的规则文件以行为单位，以”#”开头的行代表注释行。其余的每一行代表一个规则。每个规则分成一个或多个“匹配”和“赋值”部分。“匹配”部分用“匹配“专用的关键字来表示，相应的“赋值”部分用“赋值”专用的关键字来表示。“匹配”关键字包括：ACTION，KERNEL，BUS，SYSFS等等，“赋值”关键字包括：NAME，SYMLINK，OWNER等等。具体详细的描述可以阅读udev的man文档。

下面举个例子来说明一下，有这样一条规则：SUBSYSTEM==”net”, ACTION==”add”, SYSFS{address}==”00:0d:87:f6:59:f3″, IMPORT=”/sbin/rename_netiface %k eth0″
这个规则中的“匹配”部分有三项，分别是SUBSYSTEM，ACTION和SYSFS。而”赋值”部分有一项，是IMPORT。这个规则就是说，当系统中出现的新硬件属于net子系统范畴，系统对该硬件采取的动作是加入这个硬件，且这个硬件在SYSFS文件系统中的“address”信息等于“00：0d…”时，对这个硬件在udev层次施行的动作是调用外部程序/sbin/rename_netiface，传递的参数有两个，一个是“%k”，代表内核对该新设备定义的名称。另一个是”eth0“。    从上面这个例子中可以看出，udev的规则的写法比较灵活的，尤其在“匹配”部分中，可以通过诸如”*“, ”?“,[a-c],[1-9]等shell通配符来灵活匹配多个匹配项。具体的语法可以参考udev的man文档。

问：udev怎样做到不管设备连接的顺序而维持一个统一的设备名？

答：实际上，udev是通过对内核产生的设备名增加别名的方式来达到上述目的的。前面说过，udev是用户模式程序，不会更改内核的行为。因此，内核依然会我行我素地产生设备名如sda,sdb等。但是，udev可以根据设备的其他信息如总线（bus），生产商（vendor）等不同来区分不同的设备，并产生设备文件。udev只要为这个设备文件取一个固定的文件名就可以解决这个问题。在后续对设备的操作中，只要引用新的设备名就可以了。但为了保证最大限度的兼容，一般来说，新设备名总是作为一个对内核自动产生的设备名的符号链接（link）来使用的。

例如：内核产生了sda设备名，而根据信息，这个设备对应于是我的内置硬盘，那我就可以制定udev规则，让udev除了产生/dev/sda设备文件外，另外创建一个符号链接叫/dev/internalHD。这样，我在fstab文件中，就可以用/dev/internalHD来代替原来的/dev/sda了。下次，由于某些原因，这个硬盘在内核中变成了sdb设备名了，那也不用着急，udev还会自动产生/dev/internalHD这个链接，并指向正确的/dev/sdb设备。所有其他的文件像fstab等都不用修改。

问：怎样才能找到这些设备信息，并把他们放到udev的规则文件中来匹配呢？

答：这个问题比较难，网上资料不多，我只找到一篇文章来介绍如何写udev的规则。他的基本方法是通过udevinfo这个实用程序来找到那些可以作为规则文件里的匹配项的项目。有这样两种情况可以使用这个工具：

第一种情况是，当你把设备插入系统后，系统为设备产生了设备名（如/dev/sda）。那样的
话，你先用udevinfo -q path -n/dev/sda，命令会产生一个该设备名对应的在sysfs下的路径，如/block/sda。然后，你再用udevinfo -a -p/sys/block/sda，这个命令会显示一堆信息，信息分成很多块。这些信息实际来自于操作系统维护的sysfs链表，不同的块对应不同的路径。你就可以用这些信息来作为udev规则文件中的匹配项。但需要注意的是，同一个规则只能使用同一块中显示的信息，不能跨块书写规则。

第二种情况是，不知道系统产生的设备名，那就只有到/sys目录下去逐个目录查找了，反复用udevinfo　-a -p/sys/path…这个命令看信息，如果对应的信息是这个设备的，那就恭喜你。否则就再换个目录。当然，在这种情况下，成功的可能性比较小。

问: udev和devfs是什么关系
答: udev完全在用户态(userspace)工作，利用设备加入或移除时内核所发送的hotplug事件(event)来工作。关于设备的详细信息是由内核输出(export)到位于/sys的sysfs文件系统的。所有的设备命名策略、权限控制和事件处理都是在用户态下完成的。与此相反，devfs是作为内核的一部分工作的。

问: 如果udev不能完成所有devfs的工作的话，为什么把devfs标记为OBSOLETE/removed?

答: 引用 Al Viro (Linux VFS 内核维护者):

-devfs所做的工作被确信可以在用户态来完成。
-devfs被加入内核之时，大家寄望它的质量可以迎头赶上。
-devfs被发现了一些可修复和无法修复的 bug。
-对于可修复的 bug，几个月前就已经被修复了，其维护者认为一切良好。
-对于后者，同样是相当常一段时间以来没有改观了。
-devfs的维护者和作者对它感到失望并且已经停止了对代码的维护工作。

问: 但是当一个并不存在的/dev节点被打开的时候，udev并不能如devfs一样自动加载驱动程序。
答: 的确如此，但Linux的设计是在设备被发现的时候加载模块，而不是当它被访问的时候。

问: 不过等等，我确实希望 udev 可以在不存在的节点被打开的时候自动加载驱动。这是我使用devfs的唯一原因了。给udev 增加这个功能吧。
答: 不，udev 是用来管理/dev 的，不是用来加载内核驱动的。

问: 嗨，求你们了。这不难做到的。
答: 这么个功能对于一个配置正确的计算机是多余的。系统中所有的设备都应该产生hotplug 事件、加载恰当的驱动，而 udev 将会注意到这点并且为它创建对应的设备节点。如果你不想让所有的设备驱动停留在内存之中，应该使用其它东西来管理你的模块(如脚本, modules.conf, 等等) 这不是udev 的工作。

问: 但是我真的喜欢那个功能，还是加上吧
答: devfs用的方法导致了大量无用的modprobe尝试，以此程序探测设备是否存在。每个试探性探测都新建一个运行 modprobe 的进程，而几乎所有这些都是无用的。

问: 我喜欢devfs的设备文件命名方式，udev 可以这样命名么?
答: 可以，udev 可以使用 /dev 的命名策略来创建节点。通过一个配置文件，可以把内核缺省的名字映射到 devfs 的名字。可以看看udev 中带的 udev.rules.devfs 文件。注意: devfs 的命名方式是不被建议并且不被官方支持的，因为它所用的简单枚举设备的方式在设备可能被随时加入或删除的情况下确实是一个比较笨的方法。这些编号代给你的将只有麻烦，而并不能用来确定设备。看看那个永久性磁盘 (persistentdisk) 的规则就知道如何在用户态下正确的做这件事，而不是傻傻地列出设备。

问: udev 可以为哪些设备创建节点?
答: 所有在 sysfs 中显示的设备都可以由 udev 来创建节点。如果内核中增加了其它设备的支持，udev 也就自动地可以为它们工作了。现在所有的块设备都在被支持之列，大部分的主字符设备也是被支持的。内核开发者们正致力于让所有的字符设备都被支持。可以到linux-kernel邮件列表上寻找补丁或是查看补丁的状态。

问: udev 是否会去掉匿名设备数量的限制?
答: udev 完全工作于用户态。如果内核支持了更多的匿名设备，udev就会支持。

问: udev 是否会支持符号链接?
答: udev 现在就支持符号链接，每个设备节点拥有多个符号链接也是被支持的。

问: udev如何处理/dev文件系统?
答: 建议使用一个每次启动系统的时候重新创建的 tmpfs 作为 /dev 的文件系统。不过实际上udev并不关心那种文件系统在被使用。

问: 在 init 运行之前，udev 如何处理设备?
答: udev 可以被放入 initramfs 之中，并在每个设备被发现的时候运行。也可以让udev 工作在一个真的根分区被加载之后根据 /sys 的内容创建的初始/dev目录之中。

问: 我是否可以利用 udev 在一个 USB 设备被加载的时候自动加载上这个设备?
答: 技术上讲是可以的，但是 udev 不是用于这个工作的。所有的主流发布版 (distro)都包含了 HAL ([http://freedesktop.org/wiki/Software_2fhal](http://freedesktop.org/wiki/Software_2fhal)) 用于这个工作，它也是专门用于监视设备变更的，并且集成进入了桌面软件。

换个角度说，这可以简单的通过 fstab 来实现:/dev/disk/by-label/PENDRIVE /media/PENDRIVE vfat user,noauto 0 0
这样，用户可以用如下命令来访问设备:
$mount /media/PENDRIVE
同样不需要管理员权限，但却拥有了设备的全部访问权限。使用永久性磁盘链接(label, uuid) 将可以指定同一设备，无论其实际上的内核名字是什么。

问: 有什么我需要注意的安全问题么?
答: 当使用动态设备编号的时候，一个给定的主/从设备号可能在不同时间对应不同的设备，如果一个用户拥有对这个节点的访问权限，并且可以创建一个到这个节点的硬链接，他就可以如此得到一个这个设备节点的拷贝。当设备被移除之后，udev 删除了设备节点，但硬链接依然存在。如果这个设备节点之后被重新使用不同的访问权限被创建的时候，其硬链接仍然可以使用先前的访问权限来访问。(同样的问题也存在在使用 PAM 改变访问权限的 login 上。)

简单的解决方案就是通过把 /dev 放在 tmpfs 这样的单独的文件系统之上来防止建立硬链接。









