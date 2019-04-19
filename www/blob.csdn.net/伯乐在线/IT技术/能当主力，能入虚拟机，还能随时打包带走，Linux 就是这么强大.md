# 能当主力，能入虚拟机，还能随时打包带走，Linux 就是这么强大 - 文章 - 伯乐在线
原文出处： [zasdfgbnm](https://zasdfgbnm.github.io/2017/06/29/%E8%83%BD%E5%BD%93%E4%B8%BB%E5%8A%9B%EF%BC%8C%E8%83%BD%E5%85%A5%E8%99%9A%E6%8B%9F%E6%9C%BA%EF%BC%8C%E8%BF%98%E8%83%BD%E9%9A%8F%E6%97%B6%E6%89%93%E5%8C%85%E5%B8%A6%E8%B5%B0%EF%BC%8CLinux%E5%B0%B1%E6%98%AF%E8%BF%99%E4%B9%88%E5%BC%BA%E5%A4%A7/)
这里介绍一下自己管理自己的Linux桌面的一点经验吧，我觉得还是有不少可取之处的。先来说一下大多数人管理Linux桌面的方法有哪些不方便的地方吧：
- 买新电脑了，又得在新电脑上安装Linux，安装各种软件，各种库，各种开发环境，配置各种服务，真麻烦。
- 最近一直在用电脑A，干了好多事情安装了好多软件，也配置了不少开发环境跟各种服务，然而处于某种原因，我又要开始使用好久没用过的电脑B了，难道我要把在A上的做的各种配置在B上再重新做一遍？
- 在Windows下做着PPT呢，发现需要调出自己之前的程序，然后根据若干组输入跑几个结果画张图好插到PPT里，然而这个程序是在Linux下写的，编译等的过程也严重依赖自己用的Linux环境，重启进Linux拿到结果再回Windows太不方便，想在Windows下配置好环境把自己的程序跑通更不容易。
- 要对系统安装某个软件，或者进行一些比较危险的更新操作（要知道Archlinux滚动更新滚挂了太正常了），担心把系统搞挂了，系统备份又实在太麻烦，要真挂了，系统恢复起来更麻烦。
- 我一直用Archlinux做主力，然而最近做的某件事情要用某个软件，这个软件官方只给了Ubuntu上的安装方式，Archlinux里面没有相应的包，在Archlinux上手动安装也太不方便。装个Ubuntu，然后暂时用几天Ubuntu吧，也是够折腾的。更何况有时候只是想用一小下而已，怎样才能最小化自己在折腾上浪费的时间呢？
- 有的软件官方软件仓库里面没有，而`make install`的话则会在系统中安装上不被包管理器所管理的文件，将来卸载也不方便，我还是更希望所有的文件都在一个包管理器中管理的。
- 听说新版本内核引入了某个牛逼的东西？我就想快速测试一下玩玩，我电脑还有计算在跑着呢，我可不想重启，那就只能用虚拟机尝试了。而且，一定要快速，我可不想为此特地装一个虚拟机。
上述的这些不方便之处是可以通过自己管理系统时的一些技巧来克服的，本文目的就是来介绍一下这些技巧。通过这些技巧，我们实现的功能是：一台机器上，可以同时安装Windows跟若干Linux系统，Windows下可以通过虚拟机来运行位于本地磁盘的这些Linux系统，而这些Linux系统下也可以通过容器或者虚拟机的方式互相运行。并且这些系统可以非常方便地备份跟删除，也可以随时创建以及运行快照。并且这些Linux系统可以随时打包带走，只需要经过很少的修改，就能直接在U盘或者其他机器上运行。如果要换电脑，或者新装一台电脑，也不需要重新安装系统，只需要把已有的系统同步到新电脑就行。这也正是这篇文章标题的意思。
为了行文的方便，我们假定读者有一台全新的机器，硬盘还没分区，也还没装任何系统。如果已经什么都装好了，而只是想迁移到我这种管理方式的话，我相信读者能够判断这个安装教程中哪些步骤是需要做的哪些步骤是不需要做的。 另外需要注意的是这不是一个手把手的一步一步的教程，中间有一些显然的步骤我就略去不写了，所以希望读者不要照着文章里的的命令不加思考地一条一条粘贴运行，而是要搞明白这些命令的目的是什么，然后根据你自己的情况来做相应的修改。
### 分区与子卷
具体怎么分区我就不说了，随便找个livecd启动进去，然后找到你自己最喜欢的分区程序，按照你的喜好把区分了就好。注意别忘了EFI分区。我这里需要说的是，分区的时候，不论有多少个发行版要安装，总共只给Linux划分两个分区：一个是swap，另一个则是一个大的btrfs分区。那个btrfs分区里面装着所有的文件，包括用户的个人数据，以及所有发行版的rootfs。这两个分区在格式化的时候，一定要给他们取Label，这么做的好处接下来我们很快就会看到。我的习惯是，swap分区的Label我就叫他“swap”，而那个btrfs分区我则叫他“linux”。创建好分区以后，如果格式化工作是在图形的分区管理程序下完成的，那么指定Label是个非常简单的工作，右键属性里面就有。如果是使用命令行工具格式化分区的，则可以使用`-L label`选项来指定label，比如：


```
mkswap -L swap /dev/sdb4
mkfs.btrfs -L linux /dev/nvme0n1p4
```
那个大的btrfs分区上的不同内容是通过btrfs的子卷来管理的，具体来讲就是为自己想安装的每个不同的Linux系统来创建一个单独的子卷。 比如说我电脑上同时安装了Archlinux、Ubuntu、Kali、Debian四个系统，那么的btrfs分区里面就有四个子卷：archlinux、ubuntu、kali、debian。 子卷的创建可以通过`btrfs subvolume create <name>`命令完成，比如说要创建我这五个子卷，需要做的事情就是：


```
mount /dev/disk/by-label/linux /mnt
cd /mnt
btrfs subvolume create archlinux
btrfs subvolume create ubuntu
btrfs subvolume create kali
btrfs subvolume create debian
```
如果你只想装一个发行版，比如archlinux，那么只需要archlinux子卷就够了。另外，如果你想把用户数据单独放在一个子卷里，也是完全可以的，不过这里不推荐多个Linux系统共享同一个家目录，因为不同系统上安装的软件不同，同样的软件版本也不相同，即使版本相同，不同发行版也可能应用了不同的patch，这就导致在一个系统上用户家目录里面产生的配置文件，在另一个系统里无法兼容，产生奇怪的行为。
### 系统安装
创建好分区与子卷，下一步就是安装操作系统了。这里分两种情况来讲：第一种情况是你想要全新安装一个Linux操作系统；第二种情况则是你已经有了某个可用的操作系统了，而只是想把这个操作系统迁移到文章所说的管理方式上。
#### 全新安装
如果想要全新安装一个操作系统，安装方式上，作者只推荐纯手工安装，而不是用官方给的安装光盘不断点着“下一步”来进行安装。这么做是为了防止官方安装程序做一些我们不想让他做的事情，比如说自动安装grub。对于Archlinux跟Gentoo来讲，唯一的安装方法就是纯手工安装，所以只要按照官方的教程来就好了。对于deb系的系统，可以使用debootstrap程序。对于其他的发行版，可能会找不到手工安装的教程，这时候可以新建一个虚拟机，在虚拟机中使用官方的安装程序不断点击“下一步”来完成安装，然后按照下一节即将介绍的现有系统迁移教程把系统从虚拟机中迁移到现实机器上；除此之外，读者还可以找到发行版官方提供的安装程序的源代码阅读一下，看明白这些安装程序都在干啥，就知道怎么手工安装了，安装程序的代码还是相对简单的，有时间的读者不妨尝试一下。下面来具体说一下安装过程，这里只介绍Archlinux跟deb系。如果有多个Linux系统需要安装，建议先安装并完全配置好其中一个，让这个系统处于可用并且方便使用的状态，然后再在这个可用的系统中安装其他系统。这里我们假设读者已经完成了分区，创建了对应的子卷，并且把那个btrfs分区挂载在了`/mnt`上。
##### Archlinux的手动安装
Archlinux的手动安装主要还是看[官方教程](https://wiki.archlinux.org/index.php/Installation_guide)。分区的时候注意按照上文介绍的方法。非常关键的`pacstrap`那一步注意使用如下命令安装到子卷里，而不是整个btrfs分区中:


```
pacstrap -d /mnt/archlinux base
```
至于fstab，就不要使用教程中的方法来生成了，我们的管理方式比较非常规，还是自己写fstab比较好。bootloader也要按照本文下文说的方式来安装跟配置。至于其他的设置键盘、设置网络、设置时区等操作，照着教程来就行。
##### deb系的手动安装
deb系的系统网上找到的教程都是使用发行版自带的安装程序的教程，并没有像Archlinux那么详细的手动安装教程。因为我们想要手动安装，所以我们就不参照网上的deb系的安装教程了。但是我们还是有教程可以参照的，那就是Archlinux的wiki里面[关于systemd-nspawn的教程](https://wiki.archlinux.org/index.php/Systemd-nspawn#Create_a_Debian_or_Ubuntu_environment)，这个教程里面有一节介绍如何使用debootstrap安装Debian或者Ubuntu。具体安装过程请参照上述教程，其中关键命令如下：


```
debootstrap --arch amd64 zesty /mnt/ubuntu http://archive.ubuntu.com/ubuntu/
```
值得一提的是，我们安装deb系的发行版并不一定要使用deb系的livecd，任何能够安装debootstrap程序的livecd都是可以的。比如说我们完全可以使用Archlinux的livecd来启动，然后安装debootstrap并通过debootstrap来安装Ubuntu。
注意的是，debootstrap并不会像官方安装程序那样安装一个完整齐全开袋即食的操作系统，而只是安装最基本的软件包，读者需要根据自己的情况单独安装桌面环境等的软件包。同时fstab跟bootloader也要根据本文的方法自己配置。
#### 现有系统迁移
Linux系统的迁移其实非常简单，无非就是把rootfs的文件全都拷贝到目的地即可。不过这个过程虽然看似简单，但是还是有一些需要注意的东西的。比如说对于符号链接，如果处理不当，则会不小心把符号链接搞成实体文件，这就不好了。再比如说，文件的权限等元数据的问题，如果处理不当，可能会导致拷贝过程中元数据的丢失。这两种问题，都有可能会导致系统不能正常运行。还有一个需要注意的地方就是，正常运行的操作系统里，会有/proc、/dev等目录，这些目录都是单独的虚拟文件系统，是不需要拷贝的，也是无法拷贝的。
我们现在假设用户想要把位于A的Ubuntu系统迁移到目标子卷`/mnt/ubuntu`去。其中，A可能位于虚拟机中，可能位于另一台电脑上，也可能位于本地磁盘。对系统进行迁移，大方向上来讲，需要做的有两步：
- 挂载相应分区，设置ssh，保证我们能够访问到A。
- 使用`rsync`或者`btrfs send`命令来把数据从A发送到目标子卷中去。
第一步具体怎么做就不说了，分三种情况简单几句话概括一下怎么做：
- 如果只是一个分区的话，mount就可以了
- 如果是另一台机器，把那台机器配置好ssh，保证root用户可以用ssh访问
- 如果是虚拟机，有两种选择，一种是想办法挂载虚拟机的磁盘镜像，然后像情况1那样处理；另一种则是配置好网络跟ssh，像情况2那样处理。具体采取哪种措施请读者根据自己的情况来自行决定。
第二步我们来分别介绍`rsync`跟`btrfs send`两种方法。
`rsync`的方法[这里有教程](https://wiki.archlinux.org/index.php/full_system_backup_with_rsync)可以参照。我们现在假设A的ip地址为`192.168.88.3`。则只需执行如下命令即可：


```
rsync -aAXv --exclude={"/dev/*","/proc/*","/sys/*","/tmp/*","/run/*","/mnt/*","/media/*","/lost+found"} root@192.168.88.3:/ /mnt/ubuntu
```
这里提醒读者注意自己系统上是否还有其他不想要同步的文件，记得一并排除掉。
`btrfs send`只在A的rootfs也是btrfs的情况下才能使用。[这个方法的教程参见这里](https://btrfs.wiki.kernel.org/index.php/Incremental_Backup)。首先需要做的是在A机器上给rootfs创建一个只读快照（注意下面命令是在A机器上执行的）：


```
btrfs subvolume snapshot -r / /ubuntu
```
注意上面命令中快照的名字要和目标子卷的名字相同，这样可以省去将来改名的麻烦。然后就可以使用`btrfs send`命令来把快照/ubuntu中的内容发送到目的地了，在这之前我们需要暂时删除我们分区的时候创建的ubuntu子卷，这个子卷会在接收过程中自动重新创建：


```
btrfs subvolume delete /mnt/ubuntu
ssh root@192.168.88.3 btrfs send /ubuntu | btrfs receive /mnt
```
最后在A机器上把刚刚创建的快照删除就可以了


```
btrfs subvolume delete /ubuntu
```
### bootloader与fstab
系统装好了，我们的fstab还没设置，启动管理器也还没安装配置。下面来讲讲怎么配置这两样东西。我们之前说过一定要给分区取一个Label，玄机在这里。如何在虚拟机中直接运行本地磁盘上安装的Linux，以及如何能把一个系统直接进行打包带走而不需要更改太多配置，关键也在这里。
#### fstab
先来说说fstab，fstab总共有五列，分别为fs、mountpoint、type、opts跟dump/pass。这五列分别为什么意思、以及fstab该怎么填，网上一查便知，在此不再赘述。这里只说我们需要做的跟常规不一样的地方。
第一个要注意的事情是，大家在填写fstab的时候，通常喜欢在fs那一列填写类似`/dev/sda4`或者`UUID=d5acc217-d524-4a2d-a937-bad945a047b2`，而在这里这样是不行的，这里我们填写的是形如`/dev/disk/by-label/linux`这样的东西。也就是说，我们的fstab里面是通过分区的Label来找分区的。这么做的原因是，我们希望我们的rootfs不光能在这台机器上启动，还希望它能在虚拟机的环境中，或者当我们把rootfs打包带走同步到别的机器上的时候，也能正常启动。在这台机器上rootfs所在的分区叫做`/dev/sda4`，在别的机器上或者虚拟机里就不一定还叫`/dev/sda4`了。但是我们只要遵守自己的命名规则，所有机器上的这些分区我们都取相同的Label，那么我们的fstab就是放之四海而皆准的，不需要为不同的环境而更改。
第二个需要注意的问题是，不要填写rootfs的条目。这种做法跟通常发行版或者其他用户的默认做法是非常不相同的。为了理解这一点，先来说说Linux系统的启动过程。通常情况下，Linux启动的时候，首先由bootloader把内核装载到内存，并向内核传递参数告诉内核rootfs的位置。接下来内核就会根据传递的参数，以只读方式挂载rootfs，并执行rootfs中的init程序。init程序会调用相应的初始化程序执行各种初始化操作。其中一项初始化操作就是根据fstab的配置，来重新以读写方式挂载rootfs，并且挂载fstab里面配置的其他各个分区到指定位置。明白了Linux启动的过程，我们就知道，fstab里面的rootfs那一行其实不是必须的。删掉了rootfs那一行，我们只需要通过修改bootloader传递给内核的参数，就可以告诉内核直接以读写而不是只读的方式挂载rootfs。
那么，我们在写fstab的时候不写rootfs那一项有啥好处呢？好处就是，我们不仅希望我们的系统能在裸机上用，还希望我们的系统能在虚拟机上用。在下文设置qemu虚拟机的时候，我们会以virtfs的方式把我们的子卷传递给虚拟机，这个时候rootfs就已经不再是`/dev/disk/by-label/linux`了，如果我们把rootfs的挂载方式硬编码到fstab里面，那么会导致init程序的失败，进而无法启动。
另外有一点值得一提的小技巧是，很多时候我们还有别的一些个分区想要自动挂载。问题在于，这些分区在虚拟机环境中，并不一定是存在的，这就会导致启动的时候由于无法挂载而启动失败。其实系统的设计者早就考虑到这个问题了。如果你不希望fstab中的某些条目自动挂载，在选项里面增加`noauto`即可。如果你希望一些条目自动挂载，但是这些条目不是那么重要，即使挂载失败也不希望这些条目导致启动失败，可以在选项中增加`nofail`。这两个选项真的是给我们的系统管理工作提供了非常大的方便。比如说我们可能会在fstab中增加`/dev/disk/by-label/swap`的条目，以便开机自动将这个分区设置为交换分区供系统使用。然而后面我们会看到，我们设置虚拟机的时候，这个分区在虚拟机环境下，并不一定是可用的。这种情况下，我们希望系统在找不到这个分区的时候直接忽略错误不用swap便是，而不是报错拒绝启动。
说了这么多，直接贴一个fstab的例子好了：


```
tmpfs                  		/tmp	tmpfs	defaults		0 0
/dev/disk/by-label/swap		none	swap	defaults,nofail		0 0
```
#### bootloader
再来说说启动管理器，这里作者推荐的启动管理器是refind，[安装教程官网有](http://www.rodsbooks.com/refind/installing.html)，在此不赘述。这里只讲一下启动项怎么写。先贴示例代码：


```
menuentry archlinux {
	icon EFI/refind/icons/os_arch.png
	volume linux
	loader archlinux/boot/vmlinuz-linux
	options "root=/dev/disk/by-label/linux rootflags=subvol=archlinux rw"
	initrd archlinux/boot/initramfs-linux.img
}
```
其中第三行的volume用来指定内核存放的分区，此分区可以通过多种方式来指定，比如通过分区的GUID，但是对我们来说最重要的是可以通过文件系统的Label来指定。我们的rootfs分区Label是”linux”，所以这一行写作`volume linux`。
接下来就是指定内核位置、内核参数跟initramfs的位置了。其中loader用来指定内核位置，options用来指定内核参数，initrd则用来指定initramfs的位置。示例中的是Archlinux系统，内核是archlinux子卷中的`boot/vmlinuz-linux`文件，所以写作`loader archlinux/boot/vmlinuz-linux`。类似，initrd那一行则写作`initrd archlinux/boot/initramfs-linux.img`。至于内核参数，`root=/dev/disk/by-label/linux`告诉内核我们的rootfs所在的分区，`rootflags=subvol=archlinux`告诉内核挂载名为archlinux的子卷，`rw`则告诉内核以读写方式挂载。对于Ubuntu系统，这三行应该写作：


```
loader ubuntu/vmlinuz
options "root=/dev/disk/by-label/linux rootflags=subvol=ubuntu rw"
initrd ubuntu/initrd.img
```
细心的读者可能已经发现，我们的refind的配置文件中在指定分区的时候用的全是他们的Label，这就保证了这个配置文件的普适性，换台电脑，只要你用同样的管理方式，同样的命名习惯，配置文件里面的东西动都不用动，直接拷贝过去就行。
### 系统的备份与恢复以及快照的应用
由于使用了btrfs的动态卷，所以备份恢复工作做起来非常简单。备份系统只需要创建快照即可：


```
cd /mnt
btrfs subvolume snapshot archlinux backup
```
至于恢复，其实我们根本不需要恢复，直接把快照作为rootfs用就行。我们只需要去refind的配置文件里面，把相应的启动项改改即可。比如说对于Archlinux而言，只需要改成：


```
menuentry archlinux {
	icon EFI/refind/icons/os_arch.png
	volume linux
	loader backup/boot/vmlinuz-linux
	options "root=/dev/disk/by-label/linux rootflags=subvol=backup rw"
	initrd backup/boot/initramfs-linux.img
}
```
如果有强迫症，觉得rootfs名字不叫archlinux很不爽，那其实改名也很简单:


```
cd /mnt
btrfs subvolume delete archlinux
btrfs subvolume snapshot backup archlinux
```
其实，btrfs的快照功能不仅可以用来备份与恢复系统，还有很多非常灵活的运用的。比如说我想在系统里面安装一个巨大而又混乱的软件，这个软件我只想用几天干一件事情，干完这件事情我就不想用了。问题是，这个软件在官方的软件仓库并没有，要安装，我只能使用软件提供的安装程序来安装，然而软件并没有提供卸载程序，或者卸载程序卸载的很不彻底，会在系统残留垃圾。我想用这软件，然而又不想脏了我的系统，这该怎么办？很简单：创建一个快照，新增加一条以快照为rootfs的启动项，要用软件了就启动到快照中去，用完这个软件以后把快照删除即可。再比如说，我想要搞个虚拟机跟实体机一起来测试某个东西（比如说测试某些网络协议、测试某些集群管理软件等），这个时候我根本没必要重新用安装光盘去装一个虚拟机，只需要创建一个快照，然后把快照作为虚拟机的rootfs启动即可，具体方法下文会介绍，在此不多说。当然，快照的应用还远远不止我说的这些，更多好玩的应用还待读者自己探索。
### Windows下访问Linux
从文章的刚开头我们就说，有时候我们是有在Windows下运行本地安装的Linux的需求的。这个需求可以通过VirtualBox来满足，只需要在VirtualBox中使用本地磁盘来作虚拟磁盘即可。说起来简单，但是实现起来还是需要折腾一下子的。
首先我们需要新建一个虚拟机，具体过程不多说，一路“下一步”就行了，唯一需要注意的是，在创建虚拟磁盘的那一步，选择“不添加虚拟硬盘”：
![virt_hdd.png](https://zasdfgbnm.github.io/2017/06/29/%E8%83%BD%E5%BD%93%E4%B8%BB%E5%8A%9B%EF%BC%8C%E8%83%BD%E5%85%A5%E8%99%9A%E6%8B%9F%E6%9C%BA%EF%BC%8C%E8%BF%98%E8%83%BD%E9%9A%8F%E6%97%B6%E6%89%93%E5%8C%85%E5%B8%A6%E8%B5%B0%EF%BC%8CLinux%E5%B0%B1%E6%98%AF%E8%BF%99%E4%B9%88%E5%BC%BA%E5%A4%A7/virt_hdd.png)
这里我的虚拟机取名为“Linux”。创建完虚拟机了以后，就需要把本地磁盘设置为虚拟磁盘了。VirtualBox只能通过命令来做这件事情，[教程可以在这里找到](http://www.serverwatch.com/server-tutorials/using-a-physical-hard-drive-with-a-virtualbox-vm.html)。首先要做的是寻找我们安装Linux的磁盘的编号，这个可以在系统自带的磁盘管理程序中找到，在我的机器上这个磁盘编号为2：
![dskmgr.png](https://zasdfgbnm.github.io/2017/06/29/%E8%83%BD%E5%BD%93%E4%B8%BB%E5%8A%9B%EF%BC%8C%E8%83%BD%E5%85%A5%E8%99%9A%E6%8B%9F%E6%9C%BA%EF%BC%8C%E8%BF%98%E8%83%BD%E9%9A%8F%E6%97%B6%E6%89%93%E5%8C%85%E5%B8%A6%E8%B5%B0%EF%BC%8CLinux%E5%B0%B1%E6%98%AF%E8%BF%99%E4%B9%88%E5%BC%BA%E5%A4%A7/dskmgr.png)
知道了磁盘的编号，就可以创建虚拟盘了。这里我们使用的命令如下，注意使用管理员身份运行：


```
VBoxManage internalcommands createrawvmdk -filename "C:\Users\gaoxiang\VirtualBox VMs\Linux\localdisk.vmdk" -rawdisk \\.\PhysicalDrive2
```
有了虚拟磁盘了，就可以将虚拟磁盘添加到虚拟机中去了：
![newdisk.png](https://zasdfgbnm.github.io/2017/06/29/%E8%83%BD%E5%BD%93%E4%B8%BB%E5%8A%9B%EF%BC%8C%E8%83%BD%E5%85%A5%E8%99%9A%E6%8B%9F%E6%9C%BA%EF%BC%8C%E8%BF%98%E8%83%BD%E9%9A%8F%E6%97%B6%E6%89%93%E5%8C%85%E5%B8%A6%E8%B5%B0%EF%BC%8CLinux%E5%B0%B1%E6%98%AF%E8%BF%99%E4%B9%88%E5%BC%BA%E5%A4%A7/newdisk.png)
虚拟磁盘设置好了，最后一步就是设置EFI了。由于我们之前在分区的时候给文件系统都赋予了Label，并且在refind设置的时候也是用的Label来指定分区，所以同一套refind的配置在虚拟机上也能用。因此我们不需要单独给虚拟机安装bootloader，而是直接用我们之前安装在物理磁盘上的EFI分区中的refind就行。VitualBox默认是不开启EFI的，我们需要在虚拟机的系统设置里面手动勾选EFI：
![efi.png](https://zasdfgbnm.github.io/2017/06/29/%E8%83%BD%E5%BD%93%E4%B8%BB%E5%8A%9B%EF%BC%8C%E8%83%BD%E5%85%A5%E8%99%9A%E6%8B%9F%E6%9C%BA%EF%BC%8C%E8%BF%98%E8%83%BD%E9%9A%8F%E6%97%B6%E6%89%93%E5%8C%85%E5%B8%A6%E8%B5%B0%EF%BC%8CLinux%E5%B0%B1%E6%98%AF%E8%BF%99%E4%B9%88%E5%BC%BA%E5%A4%A7/efi.png)
为了要让VirtualBox自动启动refind，还要对EFI的分区做一些简单的设置，[设置的教程参考这里](https://wiki.archlinux.org/index.php/VirtualBox#Installation_in_EFI_mode)。设置的时候一定要注意，这些设置一定要是通用的，即同一份文件既能在物理机上正常工作也能在虚拟机上正常工作，不要改完了设置以后虚拟机上能跑了物理机却挂了，这就不好玩了。VirtualBox的EFI在启动的时候会优先选择`/EFI/BOOT/BOOTX64.EFI`，如果找不到的话，才会启动EFI分区根目录下的`startup.nsh`中指定的bootloader。知道了这一点，为了实现自动启动refind，首先需要检查一下`/EFI/BOOT/BOOTX64.EFI`这个文件是否存在，若存在，备份并删除之：


```
cd EFI/BOOT
mv bootx64.efi bootx64-backup.efi
```
然后就是在EFI分区根目录下新建一个`startup.nsh`了，这个文件只需要一行，内容如下：


```
\EFI\refind\refind_x64.efi
```
一切设置完毕，运行虚拟机，就能看到我们熟悉的refind界面了：
![refind.png](https://zasdfgbnm.github.io/2017/06/29/%E8%83%BD%E5%BD%93%E4%B8%BB%E5%8A%9B%EF%BC%8C%E8%83%BD%E5%85%A5%E8%99%9A%E6%8B%9F%E6%9C%BA%EF%BC%8C%E8%BF%98%E8%83%BD%E9%9A%8F%E6%97%B6%E6%89%93%E5%8C%85%E5%B8%A6%E8%B5%B0%EF%BC%8CLinux%E5%B0%B1%E6%98%AF%E8%BF%99%E4%B9%88%E5%BC%BA%E5%A4%A7/refind.png)
打开其中的Ubuntu系统，测试一切正常就大功告成了：
![ubuntu.png](https://zasdfgbnm.github.io/2017/06/29/%E8%83%BD%E5%BD%93%E4%B8%BB%E5%8A%9B%EF%BC%8C%E8%83%BD%E5%85%A5%E8%99%9A%E6%8B%9F%E6%9C%BA%EF%BC%8C%E8%BF%98%E8%83%BD%E9%9A%8F%E6%97%B6%E6%89%93%E5%8C%85%E5%B8%A6%E8%B5%B0%EF%BC%8CLinux%E5%B0%B1%E6%98%AF%E8%BF%99%E4%B9%88%E5%BC%BA%E5%A4%A7/ubuntu.png)
当然，要在虚拟机中使用，还有一些细节性的工作要处理，比如安装VirtualBox的guest需要的相应的内核模块等等，这些在此不谈，读者使用过程中如果发现少啥了，自己装上便是。
### Linux下不同发行版的互相访问
我们已经成功地在Windows下运行Linux了，下一步就是想办法在一个Linux系统下访问其他Linux了。由于这些系统都是Linux，而且都在同一个文件系统里面，所以如果只是想要访问一下里面的文件的话，挂载了用就行了。但是很多时候我们还是有需要来运行其他系统里面安装的程序，或者对那个系统进行管理的。应对这种需求有两种解决方案：容器跟虚拟机。
可能很多读者并不了解这两者的区别，这里简单介绍一下。粗略来讲，虚拟机是通过软件的方式虚拟出一套硬件环境来，并在这套硬件环境中启动内核，然后内核会进行一个完整的开机过程，包括进行相应的初始化，加载init程序等。相比之下，容器则要轻量很多。容器并不会虚拟出自己的硬件环境，也不会额外加载一个内核。容器所做的，就是在现有内核上，运用namespace来创建出一套独立的进程PID、挂载点、网络接口、用户ID等等，由于不同namespace中的这些个ID之类的标识符都是独立的，所以不同namespace中的进程是互相之间看不到对方的，虚拟出来的环境乍看上去就跟在单独运行的一个系统一样，同样有PID为1的init进程，有自己一套独立的rootfs，等等。虚拟机的优点是更不容易被突破，安全性更好，可以使用自己的内核，但是效率也更低。容器的优点是轻便效率高，但是安全性就要稍差一些，也没法使用定制内核。
#### 容器
Linux下大家最熟悉的容器就是chroot了，但是作者并不喜欢chroot，主要原因有两点：
- /proc、 /dev等东西不会自动挂载，每次手动挂载挂的心好累
- 没有一个相对完整的开机过程，好多我希望自动启动的服务并不会运行起来
基于上面的原因，作者在这里推荐的容器是systemd-nspawn。关于systemd-nspawn的介绍跟使用教程，[推荐看这里](https://wiki.archlinux.org/index.php/Systemd-nspawn)。systemd-nspawn的使用非常简单，假设你的linux分区已经mount到了/mnt上去了，那么你只需要下面步骤就能启动一个systemd-nspawn容器（以Debian为例）：


```
cd /mnt/debian
systemd-nspawn -b
```
然后就能看到刷刷刷的开机界面了，真的是非常的方便快捷。这里还有一点小技巧是，如果嫌每次开容器都要把linux分区挂载到/mnt上太麻烦，可以在`/var/lib/machines`里面为每个系统新建一个目录，然后在fstab里面设置一下自动把相应的子卷挂载进去：


```
/dev/disk/by-label/linux        /var/lib/machines/kali          btrfs           defaults,nofail,noatime,discard,subvol=kali            0 0
/dev/disk/by-label/linux        /var/lib/machines/debian        btrfs           defaults,nofail,noatime,discard,subvol=debian          0 0
/dev/disk/by-label/linux        /var/lib/machines/ubuntu        btrfs           defaults,nofail,noatime,discard,subvol=ubuntu          0 0
```
这么做的好处是，根目录位于`/var/lib/machines`的系统，在启动systemd-nspawn的时候可以直接使用`-M`选项来指定系统，而不需要进入相应目录。比如如果想启动Ubuntu系统：


```
systemd-nspawn -b -M ubuntu
```
#### 虚拟机
如果只是想运行一下其他系统里面的程序，那么容器完全就够用了，但是有的时候我们还是需要玩玩不同的内核的，这就必须得用虚拟机了。通常情况下，大家用虚拟机，都是新建一个磁盘镜像，然后插入安装光盘，然后把光盘安装到镜像上。这么做的坏处，一个是访问镜像中的文件不方便，另一个是，我们在本地已经有安装过若干系统了，不去充分利用一下这些而去再重新往镜像里面安装那实在是舍近求远。那我们就来找一个把子卷当成虚拟机rootfs的方法。困难在于，虚拟机是个很独立的东西，是无法直接访问宿主机的文件系统的。然而幸运的是，Linux的内核虚拟化方案KVM提供了一个把本地文件系统传递给虚拟机的解决方案，用到的东西叫做VirtFS，[相关的文档见这里](http://wiki.qemu.org/Documentation/9psetup)。
好消息是，VirtFS是可以作为rootfs的。但是要能正常挂载VirtFS，内核必须要有相应的驱动才行。这里有两种方法可以做到这一点。如果你是自己编译内核的话，那么建议直接将相应的驱动编译进内核而不是模块。根据官网的指示，涉及到的内核配置如下：


```
CONFIG_NET_9P=y
CONFIG_NET_9P_VIRTIO=y
CONFIG_9P_FS=y
CONFIG_9P_FS_POSIX_ACL=y
```
如果使用的是发行版提供的内核的话，那么可以修改initramfs的相关设置保证9p、9pnet、9pnet_virtio三个modules能被安装到initramfs里面去。这里以Ubuntu做guest为例，具体做法是修改Ubuntu系统中的`/etc/initramfs-tools/modules`文件，增加下面三行：


```
9p
9pnet
9pnet_virtio
```
然后重新生成initramfs即可：


```
update-initramfs -u
```
内核驱动设置好了，就可以启动qemu虚拟机了，这里假定Ubuntu的rootfs已经被mount到了`/var/lib/machines/ubuntu`：


```
qemu-system-x86_64 -enable-kvm -m 16G -kernel /var/lib/machines/ubuntu/vmlinuz -initrd /var/lib/machines/ubuntu/initrd.img -virtfs local,id=root9p,path=/var/lib/machines/ubuntu,security_model=passthrough,mount_tag=root9p -nographic -append 'root=root9p rw rootfstype=9p rootflags=trans=virtio console=ttyS0 init=/lib/systemd/systemd'
```
最后放一张成功的截图：
![qemu-ubuntu.png](https://zasdfgbnm.github.io/2017/06/29/%E8%83%BD%E5%BD%93%E4%B8%BB%E5%8A%9B%EF%BC%8C%E8%83%BD%E5%85%A5%E8%99%9A%E6%8B%9F%E6%9C%BA%EF%BC%8C%E8%BF%98%E8%83%BD%E9%9A%8F%E6%97%B6%E6%89%93%E5%8C%85%E5%B8%A6%E8%B5%B0%EF%BC%8CLinux%E5%B0%B1%E6%98%AF%E8%BF%99%E4%B9%88%E5%BC%BA%E5%A4%A7/qemu-ubuntu.png)
全剧终
