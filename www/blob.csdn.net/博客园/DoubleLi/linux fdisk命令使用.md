# linux fdisk命令使用 - DoubleLi - 博客园






fdisk 对硬盘及分区的操作，进入fdisk 对硬盘操作阶段

　　我们可以对硬盘进行分区操作，前提是您把fdisk -l 弄明白了；通过fdisk -l ，我们能找出机器中所有硬盘个数及设备名称；比如上面的例子，我们会看到两个设备一个是/dev/hda ，另一个是/dev/sda ；

　　fdisk 操作硬盘的命令格式如下：

　　[root@localhost beinan]# fdisk 设备

　　比如我们通过 fdisk -l 得知 /dev/hda 或者 /dev/sda设备；我们如果想再添加或者删除一些分区，可以用

　　[root@localhost beinan]# fdisk /dev/hda

　　或

　　[root@localhost beinan]# fdisk /dev/sda

　　注 在以后的例子中，我们要以 /dev/sda设备为例，来讲解如何用fdisk 来操作添加、删除分区等动作；

**fdisk 的说明**

　　当我们通过 fdisk 设备，进入相应设备的操作时，会发现有如下的提示；以 fdisk /dev/sda 设备为例，以下同；

[root@localhost beinan]# fdisk /dev/sda
**Command (m for help): 在这里按m ，就会输出帮助；Command actiona toggle a bootable flagb edit bsd disklabelc toggle the dos compatibility flagd delete a partition 注：这是删除一个分区的动作；l list known partition types 注：l是列出分区类型，以供我们设置相应分区的类型；m print this menu 注：m 是列出帮助信息；n add a new partition 注：添加一个分区；o create a new empty DOS partition tablep print the partition table 注：p列出分区表；q quit without saving changes 注：不保存退出；s create a new empty Sun disklabelt change a partition's system id 注：t 改变分区类型；u change display/entry unitsv verify the partition tablew write table to disk and exit 注：把分区表写入硬盘并退出；x extra functionality (experts only) 注：扩展应用，专家功能；**

　　其实我们常用的只有注有中文的，其它的功能我们不常用（呵，主要是我不会用，否则早会卖弄一下了）；x扩展功能，也不是常用的；一般的情况下只要懂得 d l m p q t w 就行了；

　　下面以实例操作来详述，没有例子没有办法就，新手也看不懂；

**列出当前操作硬盘的分区情况，用p**

Command (m for help): p
Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes
Device Boot Start End Blocks Id System
/dev/sda1 1 25 201568+ c W95 FAT32 (LBA)
/dev/sda2 26 125 806400 5 Extended
/dev/sda5 26 50 201568+ 83 Linux
/dev/sda6 51 76 200781 83 Linux

**通过fdisk的d指令来删除一个分区**

Command (m for help): p 注：列出分区情况；
Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes
Device Boot Start End Blocks Id System
/dev/sda1 1 25 201568+ c W95 FAT32 (LBA)
/dev/sda2 26 125 806400 5 Extended
/dev/sda5 26 50 201568+ 83 Linux
/dev/sda6 51 76 200781 83 Linux
Command (m for help): d 注：执行删除分区指定；
Partition number (1-6): 6 注：我想删除 sda6 ，就在这里输入 6 ；
Command (m for help): p 注：再查看一下硬盘分区情况，看是否删除了？
Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes
Device Boot Start End Blocks Id System
/dev/sda1 1 25 201568+ c W95 FAT32 (LBA)
/dev/sda2 26 125 806400 5 Extended
/dev/sda5 26 50 201568+ 83 Linux
Command (m for help):

　　警告：删除分区时要小心，请看好分区的序号，如果您删除了扩展分区，扩展分区之下的逻辑分区都会删除；所以操作时一定要小心；如果知道自己操作错了，请不要惊慌，用q不保存退出；切记切记！！！！在分区操作错了之时，千万不要输入w保存退出！！！



**通过fdisk的n指令增加一个分区**

Command (m for help): p
Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes
Device Boot Start End Blocks Id System
/dev/sda1 1 25 201568+ c W95 FAT32 (LBA)
/dev/sda2 26 125 806400 5 Extended
/dev/sda5 26 50 201568+ 83 Linux
Command (m for help): n 注：增加一个分区；
Command action
l logical (5 or over) 注：增加逻辑分区，分区编号要大于5；为什么要大于5，因为已经有sda5了；
p primary partition (1-4) 注：增加一个主分区；编号从 1-4 ；但sda1 和sda2都被占用，所以只能从3开始；
p
Partition number (1-4): 3
No free sectors available 注：失败中，为什么失败？

注：我试图增加一个主分区，看来是失败了，为什么失败？因为我们看到主分区+扩展分区把整个磁盘都用光了，看扩展分区的End的值，再看一下 p输出信息中有125 cylinders；最好还是看前面部份；那里有提到；

　　所以我们只能增加逻辑分区了

Command (m for help): n
Command action
l logical (5 or over)
p primary partition (1-4)
l 注：在这里输入l，就进入划分逻辑分区阶段了；
First cylinder (51-125, default 51): 注：这个就是分区的Start 值；这里最好直接按回车，如果您输入了一个非默认的数字，会造成空间浪费；
Using default value 51
Last cylinder or +size or +sizeM or +sizeK (51-125, default 125): +200M 注：这个是定义分区大小的，+200M 就是大小为200M ；当然您也可以根据p提示的单位cylinder的大小来算，然后来指定 End的数值。回头看看是怎么算的；还是用+200M这个办法来添加，这样能直观一点。如果您想添加一个10G左右大小的分区，请输入 +10000M ；
Command (m for help):

**通过fdisk的t指令指定分区类型**

Command (m for help): t 注：通过t来指定分区类型；
Partition number (1-6): 6 注：要改变哪个分区类型呢？我指定了6，其实也就是sda6
Hex code (type L to list codes):L 注：在这里输入L，就可以查看分区类型的id了；
Hex code (type L to list codes): b 注：如果我想让这个分区是 W95 FAT32 类型的，通过L查看得知 b是表示的是，所以输入了b；
Changed system type of partition 6 to b (W95 FAT32) 注：系统信息，改变成功；是否是改变了，请用p查看；
Command (m for help): p
Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes
Device Boot Start End Blocks Id System
/dev/sda1 1 25 201568+ c W95 FAT32 (LBA)
/dev/sda2 26 125 806400 5 Extended
/dev/sda5 26 50 201568+ 83 Linux
/dev/sda6 51 75 201568+ b W95 FAT32

　　6、fdisk 的退出，用q或者 w

　　其中 q是 不保存退出，w是保存退出

　　Command (m for help): w
　　或
　　Command (m for help): q

**四，一个添加分区的例子**

　　本例中我们会添加两个200M的主分区，其它为扩展分区，在扩展分区中我们添加两个200M大小的逻辑分区

Command (m for help): p 注：列出分区表；
Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes
Device Boot Start End Blocks Id System
Command (m for help): n 注：添加分区；
Command action
e extended
p primary partition (1-4)
p 注：添加主分区；
Partition number (1-4): 1 注：添加主分区1；
First cylinder (1-125, default 1): 注：直接回车，主分区1的起始位置；默认为1,默认就好；
Using default value 1
Last cylinder or +size or +sizeM or +sizeK (1-125, default 125): +200M 注：指定分区大小，用+200M来指定大小为200M
Command (m for help): n 注：添加新分区；
Command action
e extended
p primary partition (1-4)
p 注：添加主分区
Partition number (1-4): 2 注：添加主分区2；
First cylinder (26-125, default 26):
Using default value 26
Last cylinder or +size or +sizeM or +sizeK (26-125, default 125): +200M 注：指定分区大小，用+200M来指定大小为200M
Command (m for help): n
Command action
e extended
p primary partition (1-4)
e 注：添加扩展分区；
Partition number (1-4): 3 注：指定为3 ，因为主分区已经分了两个了，这个也算主分区，从3开始；
First cylinder (51-125, default 51): 注：直接回车；
Using default value 51
Last cylinder or +size or +sizeM or +sizeK (51-125, default 125): 注：直接回车，把其余的所有空间都给扩展分区；
Using default value 125
Command (m for help): p
Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes
Device Boot Start End Blocks Id System
/dev/sda1 1 25 201568+ 83 Linux
/dev/sda2 26 50 201600 83 Linux
/dev/sda3 51 125 604800 5 Extended
Command (m for help): n
Command action
l logical (5 or over)
p primary partition (1-4)
l 注：添加逻辑分区；
First cylinder (51-125, default 51):
Using default value 51
Last cylinder or +size or +sizeM or +sizeK (51-125, default 125): +200M 注：添加一个大小为200M大小的分区；
Command (m for help): n
Command action
l logical (5 or over)
p primary partition (1-4)
l 注：添加一个逻辑分区；
First cylinder (76-125, default 76):
Using default value 76
Last cylinder or +size or +sizeM or +sizeK (76-125, default 125): +200M 注：添加一个大小为200M大小的分区；
Command (m for help): p 列出分区表；
Disk /dev/sda: 1035 MB, 1035730944 bytes
256 heads, 63 sectors/track, 125 cylinders
Units = cylinders of 16128 * 512 = 8257536 bytes
Device Boot Start End Blocks Id System
/dev/sda1 1 25 201568+ 83 Linux
/dev/sda2 26 50 201600 83 Linux
/dev/sda3 51 125 604800 5 Extended
/dev/sda5 51 75 201568+ 83 Linux
/dev/sda6 76 100 201568+ 83 Linux

　　然后我们根据前面所说通过t指令来改变分区类型；

　　最后不要忘记w保存退出



五、对分区进行格式化，以及加载

　　先提示一下；用 mkfs.bfs mkfs.ext2 mkfs.jfs mkfs.msdos mkfs.vfatmkfs.cramfs mkfs.ext3 mkfs.minix mkfs.reiserfs mkfs.xfs 等命令来格式化分区，比如我想格式化 sda6为ext3文件系统，则输入；

　　[root@localhost beinan]# mkfs.ext3 /dev/sda6

　　如果我想加载 sda6到目前系统来存取文件，应该有mount 命令，但首先您得建一个挂载目录；比如 /mnt/sda6 ；

[root@localhost beinan]# mkdir /mnt/sda6
[root@localhost beinan]# mount /dev/sda6 /mnt/sda6
[root@localhost beinan]# df -lh
Filesystem 容量 已用 可用 已用% 挂载点
/dev/hda8 11G 8.4G 2.0G 81% /
/dev/shm 236M 0 236M 0% /dev/shm
/dev/hda10 16G 6.9G 8.3G 46% /mnt/hda10
/dev/sda6 191M 5.6M 176M 4% /mnt/sda6

　　这样我们就能进入 /mnt/sda6目录，然后存取文件了









