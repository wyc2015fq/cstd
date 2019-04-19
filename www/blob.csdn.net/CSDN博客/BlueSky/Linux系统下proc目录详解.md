# Linux系统下proc目录详解 - BlueSky - CSDN博客
2015年09月07日 15:26:25[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：322
1,、proc — 一个虚拟文件系统 
/proc 文件系统是一种内核和内核模块用来向进程 (process) 发送信息的机制 (所以叫做 /proc)。这个伪文件系统让你可以和内核内部数据结构进行交互，获取 有关进程的有用信息，在运行中 (on the fly) 改变设置 (通过改变内核参数)。 与其他文件系统不同，/proc 存在于内存之中而不是硬盘上。
2、/proc 由内核控制，没有承载 /proc 的设备。因为 /proc 主要存放由内核控制 的状态信息，所以大部分这些信息的逻辑位置位于内核控制的内存。对 /proc 进行 一次 'ls -l' 可以看到大部分文件都是 0 字节大的；不过察看这些文件的时候，确 实可以看到一些信息。这怎么可能？这是因为 /proc 文件系统和其他常规的文件系 统一样把自己注册到虚拟文件系统层 (VFS) 了。然而，直到当 VFS 调用它，请求 文件、目录的 i-node 的时候，/proc 文件系统才根据内核中的信息建立相应的文件
 和目录。 
注（inode 是 UNIX 操作系统中的一种数据结构，其本质是结构体，它包含了与文件系统中各个文件相关的一些重要信息。在 UNIX 中创建文件系统时，同时将会创建大量的 inode ）
3、查看/proc 的文件 
/proc 的文件可以用于访问有关内核的状态、计算机的属性、正在运行的进程的 状态等信息。大部分 /proc 中的文件和目录提供系统物理环境最新的信息。尽管 /proc 中的文件是虚拟的，但它们仍可以使用任何文件编辑器或像'more', 'less'或 'cat'这样的程序来查看。
4/proc 文件系统可以被用于收集有用的关于系统和运行中的内核的信息。下面是一些重要 的文件：
/proc/cpuinfo - CPU 的信息 (型号, 家族, 缓存大小等) 
/proc/meminfo - 物理内存、交换空间等的信息 
/proc/mounts - 已加载的文件系统的列表 
/proc/devices - 可用设备的列表 
/proc/filesystems - 被支持的文件系统 
/proc/modules - 已加载的模块 
/proc/version - 内核版本 
/proc/cmdline - 系统启动时输入的内核命令行参数
4、有关运行中的进程的信息 
/proc 文件系统可以用于获取运行中的进程的信息。在 /proc 中有一些编号的子目录。每个编号的目录对应一个进程 id (PID)。这样，每一个运行中的进程 /proc 中都有一个用它的 PID 命名的目录。这些子目录中包含可以提供有关进程的状态和环境的重要细节信息的文件。让我们试着查找一个运行中的进程。
5、通过 /proc 与内核交互 
上面讨论的大部分 /proc 的文件是只读的。而实际上 /proc 文件系统通过 /proc 中可读写的文件提供了对内核的交互机制。写这些文件可以改变内核 的状态，因而要慎重改动这些文件。/proc/sys 目录存放所有可读写的文件 的目录，可以被用于改变内核行为。 
/proc/sys/kernel - 这个目录包含反通用内核行为的信息。 /proc/sys/kernel/{domainname, hostname} 存放着机器/网络的域名和主机名。 这些文件可以用于修改这些名字。
$ hostname 
machinename.domainname.com 
$ cat /proc/sys/kernel/domainname 
domainname.com 
$ cat /proc/sys/kernel/hostname 
machinename 
$ echo “new-machinename” > /proc/sys/kernel/hostname 
$ hostname 
new-machinename.domainname.com 
这样，通过修改 /proc 文件系统中的文件，我们可以修改主机名。很多其 他可配置的文件存在于 /proc/sys/kernel/。 
另一个可配置的目录是 /proc/sys/net。这个目录中的文件可以 用于修改机器/网络的网络属性。比如，简单修改一个文件，你可以在网络 上瘾藏匿的计算机。
$ echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all 
这将在网络上瘾藏你的机器，因为它不响应 icmp_echo。主机将不会响应其 他主机发出的 ping 查询。要改回缺省设置，只要 
$ echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_all 
/proc/sys 下还有许多其它可以用于改变内核属性。
