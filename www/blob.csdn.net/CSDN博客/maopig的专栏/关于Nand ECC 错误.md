# 关于Nand ECC 错误 - maopig的专栏 - CSDN博客
2012年12月10日 17:30:08[maopig](https://me.csdn.net/maopig)阅读数：1226
Nand flash ECC数据错误怎么办？
我编译过kernel
当时选了ECC校验
启动以后报错
去掉了，就不报错了
所以我会先考虑是不是这个问题
一般来说，硬件ECC校验出错可能性比较小吧？
## 我啥也不懂
-----摘自网络经典文章：“LINUX系统移植“
我们的内核都是通过UBOOT写到Nand Flash的, UBOOT通过的软件ECC算法产生ECC校验码, 这与内核校验的ECC码不一样, 内核中的ECC码是由S3C2410中Nand Flash控制器产生的. 所以, 我们在这里选择禁止内核ECC校验.
修改drivers/mtd/nand/s3c2410.c 文件:
[arm@localhost linux2.6.14]$
vi drivers/mtd/nand/s3c2410.c
找到s3c2410_nand_init_chip()函数，在该函数体最后加上一条语句:
chip->eccmode = NAND_ECC_NONE;
保存,退出。
ECC很重要，SPARE区域在NAND FLASH架构定义之初就是主要用来存放ECC编码的，因为NAND FLASH数据出错概率高需要加ECC进行校验
1.一个页读/写失败了，那么是否要将其所在块标识成坏块?
一般应用NAND FLASH时都会同时使用ECC纠错，无论是硬件纠错还是软件实现。在有ECC的情况下如果写入一个Page失败是可以将其读出以判断出现的错误是否可以被ECC校验纠正。若可以被ECC纠正则可以考虑仍将该页所在块视为有效块。若是出现页写入错误就将该块表为坏块，软件在实现上会更容易。根据具体需要可以选择不同的处理
2. ecc是做什么的？ecc校验失败是否可以确定该块已经变成了坏块呢？ecc校验是否可以关闭？
ECC代表Error Correction Code及纠错码，是NAND FLASH应用的关键。因为NAND FLASH的数据可靠性问题一般使用NAND FLASH都需要对其数据进行ECC校验。一般的用法是在写入NAND FLASH时在主数据区内写入正常数据，而住数据区数据的ECC校验码则写入Spare区。将住数据区数据读出时Spare区的ECC码也必须一起读出以便进行ECC校验及纠错。基本上若是读出数据后发现ECC校验失败则当前块必须被标记为坏块。软件实现的ECC校验算法一般都可以关闭，有硬件直接实现的ECC校验模块则需要看硬件是否支持关闭ECC。一般都可以关闭。
3. 是否只有当擦除一个块失败之后，才可以把该块视为坏块?
如前所述，仍然是软件策略选择问题。若是擦除失败，仍可尝试写入数据并读出看ECC能否纠错，若是可被ECC纠正，则可以将该块视为有效块。因为块擦除失败有相当概率是因为块内一两位无法被擦除造成，一般使用的硬件ECC纠错都可纠至少两位以上错误，纠错4-8位也不少见。在擦除失败之后直接将其标为坏块可以作为最简单的软件处理方法。
以上内容摘自网络
-------------------------------------------------
"mount_devfs_fs(): unable to mount devfs, err: -2"一个困扰了我很久的问题，主要是ecc的问题。在此我把我的理解说一下好了：
    搞清楚你在driver/mtd/nand/s3c2410.c文件中有没有把NAND_ECC_SOFT改成NAND_ECC_NONE，这个网上不少的人都会做（听说会与yaffs文件系统有冲突，但我发现反而和cramfs文件系统有冲突）。这个地方我那时候没有交代清楚，不好意思^_^
    假设你把NAND_ECC_SOFT改成NAND_ECC_NONE，那[*]     Lets Yaffs do its own ECC　这一步是必需的。
    最后，如果你把NAND_ECC_SOFT改成NAND_ECC_NONE的话，那你下载yaffs文件系统的时候就不应该加上-e的参数了。
    这个东西我可是经历了无数次的"mount_devfs_fs(): unable to mount devfs, err: -2"才悟出来的，本来想不讲出来的，但实在太多人问这个问题了，实在不忍^_^
    最后给点建议：先让内核挂载cramfs试试看（记得把NAND_ECC_SOFT改成NAND_ECC_NONE哦），因为这个文件系统只要用下载内核的命令下载就行，成功挂载cramfs的话将会是你最大的鼓舞
