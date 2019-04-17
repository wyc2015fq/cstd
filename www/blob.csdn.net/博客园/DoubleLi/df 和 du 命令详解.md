# df 和 du 命令详解 - DoubleLi - 博客园






df命令详细用法 

a：显示全部的档案系统和各分割区的磁盘使用情形 
i：显示i -nodes的使用量 
k：大小用k来表示 (默认值) 
t：显示某一个档案系统的所有分割区磁盘使用量 
x：显示不是某一个档案系统的所有分割区磁盘使用量 
T：显示每个分割区所属的档案系统名称 
常用命令：df -hi 

操作详解 

引用 
指令 df 可以显示目前所有档案系统的最大可用空间及使用情形，请看下列这个例子： 

# df -h 
Filesystem Size Used Avail Capacity Mounted on 
/dev/ad0s1a 1.9G 389M 1.4G 21% / 
devfs 1.0K 1.0K 0B 100% /dev 
/dev/ad0s1d 989M 54K 910M 0% /tmp 
/dev/ad0s1f 4.8G 3.8G 657M 86% /usr 
/dev/ad0s1e 1.9G 149M 1.6G 8% /var 
/dev/ad0s1g 26G 890K 24G 0% /volume2 
/dev/da0s1d 325G 261G 38G 87% /volume1 

我们加了参数 -h 表示使用「Human-readable」的输出，也就是在档案系统大小使用 GB、MB 等易读的格式。 

上面的指令输出的第一个字段及最后一个字段分别是档案系统及其挂入点。我们可以看到 /dev/ad0s1a 这个分割区被挂在根目录下。我们在上一小节提到过 ad 所代表的是 IDE 的硬盘，而 s1 表示第一个主要扇区。我另外有一个 SCSI 硬盘，它的代号是 da，它的容量很大，主要用来存放数据。devfs 是一个特别的档案系统，该档案系统并非真的磁盘，而是 FreeBSD 用来管理系统硬件装置的虚拟档案系统。 

接下来的四个字段 Size、Used、Avail、及 Capacity 分别是该分割区的容量、已使用的大小、剩下的大小、及使用的百分比。当硬盘容量已满时，您可能会看到已使用的百分比超过 100%，因为 FreeBSD 会留一些空间给 root，让 root 在档案系统满时，还是可以写东西到该档案系统中，以进行管理。 

另外，我们还可以使用参数 -i 来查看目前档案系统 inode 的使用情形。有的时候虽然档案系统还有空间，但若没有足够的 inode 来存放档案的信息，一样会不能增加新的档案。 

# df -ih 
Filesystem Size Used Avail Capacity iused ifree %iused Mounted on 
/dev/ad0s1a 1.9G 389M 1.4G 21% 20495 262127 7% / 
devfs 1.0K 1.0K 0B 100% 0 0 100% /dev 
/dev/ad0s1d 989M 62K 910M 0% 24 141286 0% /tmp 
/dev/ad0s1f 4.8G 3.8G 657M 86% 311439 348015 47% /usr 
/dev/ad0s1e 1.9G 149M 1.6G 8% 1758 280864 1% /var 
/dev/ad0s1g 26G 890K 24G 0% 12 3532786 0% /volume2 
/dev/da0s1d 325G 261G 38G 87% 707277 43311409 2% /volume1 
我们可以看到根目录的已经用掉的 inode 数量为 20495，还有 262127 的可用 inode。 

小提示 
还记得什么是 inode 吗？所谓的 inode 是用来存放档案及目录的基本信息 (metadata)，包含时间、档名、使用者及群组等。在分割扇区时，系统会先做出一堆 inode 以供以后使用，inode 的数量关系着系统中可以建立的档案及目录总数。如果要存的档案大部分都很小，则同样大小的硬盘中会有较多的档案，也就是说需要较多的 inode 来挂档案及目录。 


du：查询档案或目录的磁盘使用空间 

a：显示全部目录和其次目录下的每个档案所占的磁盘空间 
b：大小用bytes来表示 (默认值为k bytes) 
c：最后再加上总计 (默认值) 
s：只显示各档案大小的总合 (summarize) 
x：只计算同属同一个档案系统的档案 
L：计算所有的档案大小 
常用命令：du -a 

操作详解 

引用 
指令 du 能以指定的目录下的子目录为单位，显示每个目录内所有档案所占用的磁盘空间大小。例如： 

# du -h /etc 
104K /etc/defaults 
6.0K /etc/X11 
8.0K /etc/bluetooth 
4.0K /etc/gnats 
52K /etc/isdn 
388K /etc/mail 
68K /etc/mtree 
2.0K /etc/ntp 
38K /etc/pam.d 
44K /etc/periodic/daily 
6.0K /etc/periodic/monthly 
42K /etc/periodic/security 
16K /etc/periodic/weekly 
110K /etc/periodic 
6.0K /etc/ppp 
318K /etc/rc.d 
2.0K /etc/skel 
130K /etc/ssh 
10K /etc/ssl 
1.7M /etc 
我们目样使用 -h 参数来显示 human-readable 的格式。在应用时，我们可以使用 du 这个指令来查看哪个目录占用最多的空间。不过，du 的输出结果通常很长，我们可以加上 -s 参数来省略指定目录下的子目录，而只显示该目录的总合即可： 

# du -sh /etc 
1.7M /etc 
在查看目录的使用情形时，我们可以将输出结果导到 sort 指令进行排序，以了解哪个档案用了最多的空间： 

# du /etc | sort -nr | more 
1746 /etc 
388 /etc/mail 
318 /etc/rc.d 
130 /etc/ssh 
110 /etc/periodic 
104 /etc/defaults 
68 /etc/mtree 
52 /etc/isdn 
44 /etc/periodic/daily 
42 /etc/periodic/security 
38 /etc/pam.d 
16 /etc/periodic/weekly 
10 /etc/ssl 
8 /etc/bluetooth 
6 /etc/ppp 
6 /etc/periodic/monthly 
6 /etc/X11 
4 /etc/gnats 
2 /etc/skel 
2 /etc/ntp 
sort 的参数 -nr 表示要以数字排序法进行反向排序，因为我们要对目录大小做排序，所以不可以使用 human-readable 的大小输出，不然目录大小中会有 K、M 等字样，会造成排序不正确。









