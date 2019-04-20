# 使用 SELinux 和 Smack 增强轻量级容器 - h13 - 博客园
http://www.bitscn.com/os/linux/200904/158771.html
安全 Linux 容器实现指南
    轻量级容器 又称作 Virtual Private Servers (VPS) 或 Jails，它们是经常用于限制不可信应用程序或用户的工具。但是最近构造的轻量级容器没有提供充分的安全保证。使用 SELinux 或 Smack 策略增强这些容器之后，就可以在 Linux中实现更加安全的容器。本文介绍如何创建受 Linux 安全模块保护的更加安全的容器。SELinux 和 Smack 策略的开发都在进行当中，并且在各自社区的帮助下不断得到改善。
    人们听到容器时的第一反应是 “如何才能创建安全的容器？”。本文通过使用 Linux 安全模块（Linux Security Modules，LSM）增强容器的安全性来解决这个问题。本文特别演示了如何设定安全目标，并通过 Smack 和 SELinux 安全模块实现目标。
    要了解 Linux 容器的背景知识，请阅读 “linux.chinaitlab.com/administer/777960_2.html" target=_blank>LXC：Linux 容器工具”（developerWorks，2009 年 2 月）。
    Linux 容器是根据几种 Linux 技术构建的概念性工件：
    资源名称空间 允许在容器内部查找进程、文件、SYSV IPC 资源、网络接口等等。
    控制组（Control groups）允许限制放置到容器中的资源。
    功能绑定（Capability bounding）设置 限制容器的访问特权。
    必须协调使用这些技术，以实现符合设想的容器。目前已有两个项目提供这个功能：
    Libvirt 是能够使用 Xen 管理程序、qemu 模拟器、kvmis 甚至是轻量级容器创建虚拟机的大型项目。
    Liblxc 是一个较小的库和用户空间命令集合，它们的目的之一是帮助内核开发人员快速轻松地测试容器的功能。
    因为 “LXC：Linux 容器工具” 是基于 liblxc 编写的，所以我在这里继续使用 liblxc；不过这里完成的操作也能够使用 libvirt 的容器支持轻松完成。
    主要元素 1：LSM
    在开始之前，如果不太了解 LSM，现在可以快速浏览一下。根据 Wikipedia 中的定义：Linux Security Modules (LSM) 是一个允许 Linux 内核支持各种计算机安全模型的框架，同时也避免依赖于特定安全实现。这个框架由 GNU General Public License 条款授权使用，并且是 Linux 2.6 之后的 Linux 内核的标准部分。设计 LSM 的目的是为成功实现强制访问控制模块提供一切必要元素，同时最小化对 Linux 内核的更改。LSM 避免了 Systrace 中的系统调用插入，因为它不支持多处理器内核，并且容易受 TOCTTOU (race) 攻击。相反，当某个用户级别的系统将要访问重要的内部内核对象（比如 inode 和任务控制块）时，LSM 将在内核中插入 “钩子（hook）”（向上调用模块）。这个项目专门用于解决访问控制问题，以避免对主流内核进行大量的复杂修改。该项目并不打算成为通用的 “钩子” 或 “向上调用” 机制，也不支持虚拟化。LSM 访问控制的目标与解决系统审计问题密切相关，但又有所区别。审计要求记录每次访问尝试。LSM 不能解决这个问题，因为这需要大量的钩子，以检测内核 “短路” 故障系统在什么地方发出调用，并在接近重要对象时返回错误代码。
    系统安全包括两个有些冲突的目标。第一个目标是实现完整的细粒度访问控制。必须对有可能泄露或损坏信息的位置实施控制。过于粗粒度的控制和不进行控制没有区别。例如，如果必须将所有文件归为一种类型，并且有任何一个文件是公开的，则所有文件都是公开的。
    另一方面，配置必须简单，否则管理员就需要管理很多访问（但是再次强调，这和不进行控制是一样的）。例如，如果使程序正常工作需要大量访问规则，那么管理员就会为程序添加许多访问权限，而不是测试这些访问规则是否有必要。
    Linux 中的两个基本安全模块使用不同的方法来平衡这个矛盾。
    SELinux 首先对所有东西实施控制，同时使用强大的策略语言简化策略的管理。
    Smack 主要提供简单的访问控制。
    主要元素 2：SELinux
    到目前为止，SELinux 是针对 Linux 的最有名的 MAC 系统（强制访问控制）。尽管仍然有人反对它，但流行的 Fedora? 发行版从几年前开始就和 SELinux 一起部署，这是它取得成功的有力证明。
    SELinux 使用模块化策略语言配置，因此用户可以轻松更新已安装的策略。这种语言还提供一些接口，允许使用更高级的语句表达一组低级的语句。
    在本文中，我们将使用一个新的接口来定义容器。虽然为容器添加许多访问权限使接口本身变得非常大，但是使用接口创建新的容器却很简单。这个接口很有希望成为核心发布策略的一部分。
    主要元素 3：Smack
    Smack 是简化的强制访问控制内核（Simplified Mandatory Access Control Kernel）的缩写。它首先使用简单的文本标签标记所有进程、文件和网络流量。使用创建进程的标签创建最新的文件。通常存在一些带有明确定义的访问规则的默认类型。进程常常可以对具有同一标签的对象进行读写。绕过 Smack 访问规则的特权由 POSIX 功能控制，因此带有 CAP_MAC_OVERRIDE 的任务可以覆盖规则；带有 CAP_MAC_ADMIN 的任务可以更改规则和标签。“POSIX file capabilities: Parceling the power of root”（参考资料）演示了这些特权。
受 SELinux 保护的容器
    我们将在容器上使用的 SELinux 策略包含一个 策略模块；这个模块已经发布到 refpolicy -- SELinux Reference Policy 开发邮件列表。将这个策略分别下载到 /root/vs 目录下的 vs.if、vs.fc 和 vs.te 文件中。像下面这样编译和安装新的模块：
  
cp vm.img selinux.img
cp vm.img smack.img
    然后使用 lxc-debian 创建 /vs1 and /vs2 容器，并且使用
mkdir /vs1; cd /vs1
lxc-debian create
	container name: vs1
	hostname: vs1
	address: 10.0.2.21
	gateway: 10.0.2.2
	arch: 2 (i386)
mkdir /vs2; cd /vs2
lxc-debian create
	container name: vs2
	hostname: vs2
	address: 10.0.2.22
	gateway: 10.0.2.2
	arch: 2 (i386)
fixfiles relabel /vs1
fixfiles relabel /vs2
    重新标记它们的文件系统。
    在启动容器时（例如通过使用命令 lxc-start -n vs1），很可能会收到一些关于 SELinux 访问拒绝的审计消息。但不要担心 —— 容器将正常启动，并且会启用网络服务并 隔离容器。如果在启动容器之前使用 mount --bind / /vs1/rootfs.vs1/mnt 帮助容器 vs1 进行伪装，那么您将会发现，即使是根用户，也会重用 ls /mnt/root。
    为了了解其中的原理，我们看看 vs.if 接口文件。这个文件定义一个称为 container 的接口，它带有一个参数（即容器将要定义的基本名称）。vs.te 文件使用容器名 vs1 和 vs2 两次调用这个函数。在这个接口中，$1 被扩展到这个参数，因此当我们调用 container(vs1) 时，$1_t 就会变成 vs1_t（从这里开始，假设我们定义的是 vs1）。
    包含 vs1_exec_t 内容的行是最重要的。这个容器以 vs1_t 类型运行。当 unconfined_t 执行容器的 /sbin/init（类型为 vs1_exec_t）时，它将进入这种类型。
    剩余的策略主要是授与容器充分的特权，以访问系统的各个部分：网络端口、设备和控制台等。该接口很长，这是由现有 SELinux 引用策略的细粒度特性决定的。正如我们将要看到的一样，受 Smack 保护的容器具有更加简单的策略；但是它在系统服务行为失误时提供的灵活保护要少得多。
    还有一件事情需要做。需要注意的是，虽然容器不能够重写它的 $1_exec_t（即 /sbin/init），但它能够执行
   
mv /sbin /sbin.bak
mkdir /sbin
touch /sbin/init
    生成的 /sbin/init 的类型为 vs1_file_t。容器管理员为什么需要这样做呢？因为它会在 unconfined_t 域中启动容器，包括 ssh daemon，这使他能够获得有特权的 shell，并且能够绕过我们将要实施的 SELinux 限制。
    为了避免这样做，需要通过定制脚本实际启动容器，并在启动容器前将 sbin/init 重新标记为 vs1_exec_t。事实上，如果容器管理员不介意的话，可以将一个 init 原始副本复制回容器中并重新标记它。但我们仅重新标记现有的 init：
   
cat >> /vs1/vs1.sh << EOF
#!/bin/sh
chcon -t vs1_exec_t /vs1/rootfs.vs1/sbin/init
lxc-start -n vs1
EOF
chmod u+x /vs1/vs1.sh
    现在需要使用 /vs1/vs1.sh 启动容器，而不是使用 lxc-start 手工启动。
   受 Smack 保护的容器
    在启用 Smack 时重新编译内核。您必须能够进入 /root/rpmbuild/BUILD/kernel*/linux* 目录的 make menuconfig，然后转到 security 菜单禁用 SELinux 并启用 Smack。接下来仅需重复步骤 make && make modules_install && make install。
    此外，也要停止用户空间对 SELinux 的配置。这可以在 SELinux 管理 GUI 上实现，或编辑 /etc/selinux/config 并设置 SELINUX=disabled。要在引导时安装 Smack 策略还需要几个步骤：
mkdir /smack
cd /usr/src
wget http://schaufler-ca.com/data/080616/smack-util-0.1.tar
tar xf smack-util-0.1.tar; cd smack-util-0.1
make && cp smackload /bin
    实际的 Smack 策略类似于清单 1：
    清单 1. smackaccesses
 vs1 _ rwa
_ vs1 rwa
vs2 _ rwa
_ vs2 rwa
_ host rwax
host _ rwax
    应该将它复制到一个名为 /etc/smackaccesses 的文件中。下次运行 /bin/container_setup.sh 时会将这个文件加载到 smackload。
    这个策略十分简单。默认情况下，任何标签都可以读取标记有 _ 的数据。我们为容器不能访问的主机的私有数据定义一个新标签 host；并且将这个标签应用到 container_setup.sh 脚本中的 cgroups 文件系统。其他敏感文件，比如 /etc/shadow，应该使用这个标签。
    我们对 vs1 和 vs2 进行定义以标记容器。默认情况下，它们能够访问自己的数据。我们添加一个规则使它们可以写 _，从而允许发送网络包。由于 vs1 不能访问 vs2 数据（反之亦然），因此容器之间是彼此独立的。
    如前所述，由 CAP_MAC_ADMIN 和 CAP_MAC_OVERRIDE 功能决定定义或绕过 Smack 规则的能力。因此，容器不应该具有这些能力。这可以通过 helper 程序 dropmacadmin.c 来实现（参见 下载 小节）。必须静态地编译它，因为来自主机的容器有不同的版本：
  
gcc -o dropmacadmin dropmacadmin.c -static
cp dropmacadmin /bin/
    创建一个称为 vs1 的新容器：
 
mkdir /vs1; cd /vs1
lxc-debian create
	container name: vs1
	hostname: vs1
	address: 10.0.2.21
	router: 10.0.2.2
	arch: 2 (i386)
    使用标签 vs1 标记 vs1 文件系统中的所有文件：
   
for f in `find /vs1/rootfs.vs1`; do
	attr -S -s SMACK64 -V vs1 $f
done
    现在需要创建一个能够安全启动容器的脚本。这意味着它能将自己的进程标签设置为 vs1，并通过 dropmacadmin 打包容器的 /sbin/init。如下所示：
cat >> /vs1/vs1.sh << EOF
#!/bin/sh
cp /bin/dropmacadmin /vs1/rootfs.vs1/bin/
attr -S -s SMACK64 -V vs1 /vs1/rootfs.vs1/bin/dropmacadmin
echo vs1 > /proc/self/attr/current
lxc-start -n vs1 /bin/dropmacadmin /sbin/init
EOF
chmod u+x /vs1/vs1.sh
    再做一件事情就可以让 vs1 在其即将装载的 tmpfs 文件系统上执行写操作：
sed -i 's/defaults/defaults,smackfsroot=vs1,smackfsdef=vs1/' \
	/vs1/rootfs.vs1/etc/fstab
    这导致在 /dev/shm 上装载 tmpfs 文件系统，以带上 vs1 标签，从而让 vs1 可以对它执行写操作。否则，vs1 init 脚本将不能在设置网络时创建需要使用的 /dev/shm/network 目录。类似地，如果希望使用基于 ram 的 /tmp，使用相同的选项即可。
    现在，我们再次帮助 vs1 进行伪装。像创建 vs1 那样创建 vs2，在每个步骤中将 vs1 替换为 vs2。然后在 vs1 的 /mnt 下绑定装载根文件系统：
 
mount --bind /vs1 /vs1
mount --make-runbindable /vs1
mount --rbind / /vs1/rootfs.vs1/mnt
    使用 vs1.sh 启动容器。注意，您还可以从 kvm 主机看到 vs1 和 vs2 上的 Web 页面。此外还要注意，vs1 不能通过网络访问 vs2。它也不能查看 vs2 的文件：
vs1:~# ls /mnt/
  (directory listing)
vs1:~# ls /mnt/vs2/rootfs.vs2
  ls:/mnt/vs2/rootfs.vs2: Permission denied
vs1:~# mkdir /cgroup
vs1:~# mount -t cgroup cgroup /cgroup
vs1:~# ls /cgroup
  ls:/mnt/vs3: Permission denied
vs1:~# mknod /dev/sda1 b 8 1
  mknod: `/dev/sda1': Operation not permitted
vs1:~# mount /mnt/dev/sda1 /tmp
  mount: permission denied
    它能查看主机文件系统。对于需要保护的任何东西，可以使用 host 标签进行标记。在 cgroup 文件系统上就采用了这种做法，这正是 ls /cgroup 失败的原因。
    最后，设备白名单 cgroup 防止我们创建磁盘设备，或在它存在的情况下装载它（因为这需要通过 /mnt 来完成）。
    当然，我们的设置方式让容器管理员可以删除 /mnt/dev/sda1，或用其他方法扰乱主机，因此除了用于演示外，这种绑定装载是不如人意的！
    注意，在 SELinux 系统上，默认（且容易的）路由允许容器通过网络彼此进行对话，而在 Smack 中则恰好相反。目前，允许容器彼此对话还是比较困难的。不久以后，将可以在 IP 地址上设置标签，并且允许建立策略以实现容器之间的通信。
    在如何建立 Smack 网络方面还有另一个问题。命令 kill -9 -1 终止系统上的每个任务。当这个操作由容器中的任务执行时，它将仅终止同一容器中的任务。这种行为已经在上游内核中得到修复，但我们使用的 Fedora 10 内核还存在该问题。因此，每个任务都会发出一个 -9 信号。
    在受 SELinux 保护的容器中，SELinux 阻止该信号通过容器边界，因此 kill -9 -1 实际上是安全的。但在 Smack 中，默认情况下任务被标记为 _（就像网络一样），因此由于我们允许容器执行 _ 写操作以写到网络中，并且终止任务在 Smack 中被认为是写访问，所以允许容器管理员在整个系统上终止任何任务。
    另一个缺点（SELinux 容器仍然存在该缺点）与 Unix98 伪终端有关。打开两个图形化终端。在第一个终端中，启动 vs1 并查看 /dev/pts。您将看见至少两个条目（0 和 1），它们分别属于每个终端。可以从 vs1 容器写入到与另一个终端对应的条目。
    对于 Fedora 内核，有两个解决方案。可以使用设备白名单 cgroup 拒绝容器打开设备。但是这必须在容器每次启动时手动操作，以允许它访问自己的终端；或者应用 SELinux 和 Smack 标签，结果是一样的。
    更新的 2.6.29 内核支持 devpts 名称空间。容器必须重新装载 /dev/pts，在这个操作之后，它将不能访问属于主机或其他容器的 devpts 条目。
    结束语
    本文介绍了构建受 LSM 保护的容器所需的工具，但还有很多工作需要做：
    对于 Smack，必须选择需要标记为 host 的文件。
    对于 SELinux，应该对其进行调优，然后将一个 container 接口放置到上游引用策略。
    尽管这些工作正在进行当中，在获得更多关于受 LSM 保护的容器的经验之前，您不应该完全信赖这些机制来阻止不可信的根用户。
    尽管目前在创建容器方面还没有最佳实践，但仍然有一些想法是值得考虑的。首先，记住您正试图实现两个有些矛盾的目标：最小化容器（以及主机）之间的复制，同时需要确保安全隔离。实现这些目标的方法之一是，创建一个最小的完整 rootfs，其中不运行任何容器，并且将它的类型标记为所有容器都可以读取的类型。然后使用 lxc-sshd 脚本的定制版本创建每个基于原型的实际容器，以为容器的大部分文件系统创建只读装载，同时为容器提供一个可以存储文件的私有可写位置（比如 /scratch）。由于每个容器都有一个私有的装载名称空间，所以它能够绑定装载任何私有的和/或对于其私有共享目录可写的文件或目录。例如，如果它需要一个私有 /lib，则可以执行 mount --bind /scratch/rootfs/lib /lib。同样地，管理员也能确保每个容器都在启动时执行 mount --bind /scratch/shadow /etc/shadow。
    对于 SELinux 和 Smack，我演示的这个方法的一个明显缺点就是容器管理员不能在自己的容器的内部利用 LSM 控制信息流。并且为简单起见，容器中的所有任务都使用 MAC 策略统一处理。在另一篇文章中，我将探讨如何允许容器管理员指定自己的 LSM 策略，同时又能够约束它们。
