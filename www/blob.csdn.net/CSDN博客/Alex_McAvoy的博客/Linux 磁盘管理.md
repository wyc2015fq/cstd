# Linux 磁盘管理 - Alex_McAvoy的博客 - CSDN博客





2018年07月01日 01:37:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：111
个人分类：[Linux 学习笔记](https://blog.csdn.net/u011815404/article/category/7715914)









# 【查看磁盘空间情况】

## 1.df命令

1）作用：检查文件系统的磁盘空间占用情况。可以利用该命令来获取硬盘被占用了多少空间，目前还剩下多少空间等信息。

2）格式：**df [选项] [目录或文件名]**

3）常见选项：


|-a|列出所有的文件系统|
|----|----|
|-h|各文件系统以较易阅读的 G Bytes, M Bytes, K Bytes 等格式显示|
|-k|以 K Bytes 的容量显示|
|-m|以 M Bytes 的容量显示各文件系统|
|-H|以 M=1000K 取代 M=1024K 的进位方式|
|-T|显示文件系统类型|
|-i|不用硬盘容量，而以 inode 的数量显示|

**注：inode，称为"索引节点"，是储存文件元信息的区域**

4）实例

① 将系统内所有的文件系统列出

![](https://img-blog.csdn.net/2018070101433398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：**

**    ① /dev/sd 是对应主机硬盘的分区，以上图** /dev/sda1 为例，**数字表示分区号，数字前面的字母 a 表示第几块硬盘。若主机上有多块硬盘则会出现 /dev/sdb，/dev/sdc 等，这些磁盘设备都会在 /dev 目录下以文件形式存在。**

**    ② 1k-块，表示以磁盘块大小的方式显示容量，后面为相应的以块大小表示的已用和可用容量。**

② 将容量结果以易读的容量格式显示出来

![](https://img-blog.csdn.net/20180701014416852?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

③ 将 /etc 下可用的磁盘容量以易读的容量格式显示

![](https://img-blog.csdn.net/20180701014445713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 2.du命令

1）作用：对文件和目录磁盘使用的空间的查看。

2）格式：**du [选项] [目录或文件名]**

3）常见选项：
|-a|列出所有的文件与目录容量|
|----|----|
|-h|以较易阅读的 G Bytes, M Bytes, K Bytes 等格式显示|
|-d|指定查看目录的深度|
|-k|以 KBytes 列出容量显示|
|-m|以 MBytes 列出容量显示|
|-s|列出目录总量|
|-S|不包括子目录下的总量|

**注：与 df 命令不同，du 命令会直接到文件系统内去搜寻所有的文件数据。**



4）实例：

① 列出当前目录下的所有文件的容量

![](https://img-blog.csdn.net/20180701015717498?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

② 检查根目录所占用的容量

![](https://img-blog.csdn.net/2018070102014832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

③ 查看1级目录、二级目录的信息

![](https://img-blog.csdn.net/20180701020507821?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 【创建虚拟磁盘】

## 1.dd 命令

1）作用:

dd 命令常用于复制文件并对原文件的内容进行转换和格式化处理。

在 Linux 上，硬件的设备驱动（如硬盘）和特殊设备文件（如/dev/zero和/dev/random）与普通文件一样，只是在各自的驱动程序中实现了对应功能，dd 命令可以读取文件或写入这些文件。因此，dd 命令可以用在备份硬件的引导扇区、获取一定数量的随机数据或者空数据等任务中。

dd 命令还可以在复制时处理数据，如：转换字节序、在 ASCII 编码与 EBCDIC 编码间互换等。

2）格式：**dd 选项=值**

3）常见选项：
|bs=<字节数>|指定块大小（缺省单位为 Byte）|
|----|----|
|if=<文件>|输入到文件|
|of=<文件>|输出到文件|
|ibs=<字节数>|每次读取指定的字节数|
|obs=<字节数>|每次输出指定的字节数|
|count=<区块数>|仅读取指定的区块数|
|conv=<关键字>|指定文件转换的方式|

**注：dd 命令默认从标准输入中读取，并写入到标准输出中，如想利用文件输入输出，可以用选项 if（input file）和 of（output file）改变。**

4）实例：

①从标准输入读入用户的输入到文件中

![](https://img-blog.csdn.net/20180701143739599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

②将输出的英文字符转换为大写再写入文件

![](https://img-blog.csdn.net/20180701144250274?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 2.创建虚拟磁盘

如下图，使用 dd 命令从 /dev/zero 设备创建一个容量为 256M ，名称为 test 的空镜像文件。

![](https://img-blog.csdn.net/20180701144643512?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：img格式是镜像文件的一种。**



## 3.格式化磁盘

1）作用：物理硬盘安装完毕后，要进行文件系统的格式化（写入文件系统），使用 mkfs（make filesystem）命令即可。

2）格式：mkfs [选项] 装置文件名

3）常见选项：**-t 文件系统格式（例：ext3、ext4、vfat等）：将文件系统格式化为相应的文件系统格式**

4）实例：格式化刚创建的虚拟磁盘镜像 test.img 文件

① 输入 mkfs 后，连按两次 Tab 键，按下两个[tab]，会显示 mkfs 支持的文件格式

![](https://img-blog.csdn.net/20180701023247270?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

② 将虚拟磁盘 test.img 格式化为 ext4 文件系统

![](https://img-blog.csdn.net/2018070115553023?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 4.磁盘挂载

1）挂载说明：

在 Linux 打开一个文件前，包含该文件的文件系统必须先进行挂载的动作，此时用户要对该文件系统执行 mount 的指令以进行挂载。

mount 命令会告诉操作系统，对应的文件系统已准备好可以使用，该文件系统会对应到一个特定的点（挂载点），挂载好的文件、目录、设备以及特殊文件即可提供用户使用。

因为 Linux 访问外设都是通过挂载来实现的，因此只有挂载到目录下，才能正常使用硬盘。

2）mount 命令格式：**mount [-t 文件系统] [-L Label名] [-o 额外选项] [-n]  装置文件名  挂载点**

3）实例：挂载创建的虚拟磁盘镜像到 /mnt 目录

① 查看主机已挂载的文件系统

![](https://img-blog.csdn.net/20180701161406515?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：输出的结果中，每一行表示一个设备或虚拟设备，最前面是设备名 ，然后是 on + 挂载点，其次是 type + 文件系统类型，再后面是挂载选项（例：在挂载时设定以只读方式挂载等）。**

② 将虚拟磁盘镜像挂载到 /mnt 目录

![](https://img-blog.csdn.net/20180701161352351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 4.磁盘卸载

1）使用 umount 命令可以将以挂载的设备。

2）命令格式：**umount 已挂载设备名或者挂载点**

3）实例：卸载已挂载在 /mnt 上的虚拟磁盘镜像 test.img 文件

![](https://img-blog.csdn.net/20180701162207162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# **【磁盘分区与格式化】**

## 1.磁盘分区

1）fdisk 命令：磁盘分区表操作工具，用于磁盘分区。

2）格式：fdisk** [选项] 装置名称**

3）常见选项：**-l：输出后接的装置所有的分区内容**

**注：仅有 fdisk -l 时，系统会把整个系统内能够搜寻到的装置的分区均列出来。**

4）实例操作

① 使用 fdisk -l 命令，查看硬盘分区表信息。

如下图，可以看到磁盘的一些信息，包括容量扇区数，扇区大小，I/O 大小等。

![](https://img-blog.csdn.net/20180701162434201?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

② 使用 fdisk 命令，进入磁盘分区模式，按 m 获取帮助

![](https://img-blog.csdn.net/20180701152628774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

③ 用虚拟磁盘镜像创建新分区

![](https://img-blog.csdn.net/20180701153753978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

④ 重复上述操作，创建 3 个大小为 30M 的主分区，剩余空间创建为扩展分区，按 p 查看分区表情况

![](https://img-blog.csdn.net/20180701153917862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

⑤ 按 w 保存写入分区并退出 fdisk 工具

![](https://img-blog.csdn.net/20180701154105597?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.loop 设备与 losetup 命令

1）loop 设备介绍

在 Linux 系统中，loop 设备（回环/循环设备）是一种伪设备，或可以说是仿真设备，它能使我们像块设备一样访问一个文件。

在使用 loop 设备之前，其必须要与一个文件进行连接，这种结合方式给用户提供了一个替代块特殊文件的接口，因此，如果这个文件包含有一个完整的文件系统，那么这个文件就可以像一个磁盘设备一样被挂载起来。

loop 设备可把文件虚拟成块设备，籍此来模拟整个文件系统，让用户得以将其视为硬盘驱动器，光驱或软驱等设备，并挂入当作目录来使用。

2）loop 的含义

对于第一层文件系统，它直接安装在我们计算机的物理设备之上，而对于这种被挂载起来的镜像文件（它也包含有文件系统），是建立在第一层文件系统之上，这样看来，就像是在第一层文件系统之上再绕了一圈的文件系统，所以称为 loop。

3）losetup 命令

①作用：用于设置回环设备。

②格式：**losetup [选项] 回环设备名 要与回环设备相关联的文件名**

③常见选项：
|-e|表示加密的方式|
|----|----|
|-o|设置数据偏移量|
|-d|卸载设备|

④实例：建立 test.img 镜像文件与回环设备的关联

![](https://img-blog.csdn.net/20180701163917907?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：若提示设备忙，可以使用 ls /dev/loop* 命令参看所有回环设备，使用其他回环设备。**



## 3.格式化各分区与挂载虚拟磁盘

1）使用 kpartx 工具，为各分区建立虚拟设备的映射

①工具安装

![](https://img-blog.csdn.net/2018070116440341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

②建立映射

![](https://img-blog.csdn.net/20180701164350114?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注：

①关于 kpartx 工具

在 Linux 中，如果映像文件（*.img）含有分区表，那么用 losetup 来挂载文件系统就有些力不从心了，因为 losetup 只能挂载无分区的文件系统映像文件。但使用 kpartx 工具可以配合 losetup 命令来挂载带有分区表的映像文件。

②kpartx 工具常用的选项有三个：
|-a|指定去添加的映像文件（add）|
|----|----|
|-v|指定要挂载到的 loop 设备（verbose）|
|-d|断掉与loop设备的映射（delete）|

例：取消各分区虚拟设备的映射

![](https://img-blog.csdn.net/20180701165809853?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2）对各主分区格式化

![](https://img-blog.csdn.net/20180701164647535?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3）新建空目录用于挂载磁盘

![](https://img-blog.csdn.net/20180701164948770?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4）挂载磁盘分区

![](https://img-blog.csdn.net/20180701165106685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5）使用 df -h 命令检查文件系统的磁盘空间占用情况，可以看到磁盘分区挂载完毕后的情况

![](https://img-blog.csdn.net/20180701165222661?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注：卸载磁盘分区

![](https://img-blog.csdn.net/20180701165333500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



