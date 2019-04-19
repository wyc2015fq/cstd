# linux_NandFlash_driver_超详细分析 . - maopig的专栏 - CSDN博客
2012年12月10日 18:44:22[maopig](https://me.csdn.net/maopig)阅读数：8692
分类： [Linux 驱动](http://blog.csdn.net/flymachine/article/category/902715)[Nand 驱动](http://blog.csdn.net/flymachine/article/category/922909)2011-11-06 23:16
474人阅读[评论](http://blog.csdn.net/flymachine/article/details/6941854#comments)(0)[收藏]()[举报](http://blog.csdn.net/flymachine/article/details/6941854#report)
今天学习了NandFlash的驱动，硬件操作非常简单，就是这个linux下的驱动比较复杂，主要还是MTD层的问题，用了一下午时间整理出来一份详细的分析，只是分析函数结构和调用关系，具体代码实现就不看了，里面有N个结构体，搞得我头大。
我用linux2.6.25内核，2440板子，先从启动信息入手。
**内核启动信息，NAND部分：**
S3C24XX NAND Driver, (c) 2004 Simtec Electronics
s3c2440-nand s3c2440-nand: Tacls=2, 20ns Twrph0=3 30ns, Twrph1=2 20ns
NAND device: Manufacturer ID: 0xec, Chip ID: 0x76 (Samsung NAND 64MiB 3,3V 8-bit)
Scanning device for bad blocks
Creating 3 MTD partitions on "NAND 64MiB 3,3V 8-bit":
0x00000000-0x00040000 : "boot"
0x0004c000-0x0024c000 : "kernel"
0x0024c000-0x03ffc000 : "yaffs2"
第一行，在driver/mtd/nand/s3c2410.c中第910行，s3c2410_nand_init函数：
printk("S3C24XX NAND Driver, (c) 2004 Simtec Electronics\n");
行二行，同一文件，第212行，s3c2410_nand_inithw函数：
dev_info(info->device, "Tacls=%d, %dns Twrph0=%d %dns, Twrph1=%d %dns\n", tacls, to_ns(tacls, clkrate), twrph0, to_ns(twrph0, clkrate), twrph1, to_ns(twrph1, clkrate));
第三行，在driver/mtd/nand/nand_base.c中第2346行，
printk(KERN_INFO "NAND device: Manufacturer ID:" " 0x%02x, Chip ID: 0x%02x (%s %s)\n", *maf_id, dev_id, nand_manuf_ids[maf_idx].name, type->name);
第四行，在driver/mtd/nand/nand_bbt.c中第380行，creat_bbt函数：
Printk(KERN INFO " Scanning device for bad blocks \n");
第五行，在driver/mtd/mtdpart.c中第340行，add_mtd_partitions函数：
printk (KERN_NOTICE "Creating %d MTD partitions on \"%s\":\n", nbparts, master->name);
下面三行，是flash分区表，也在mtdpart.c同一函数中，第430行：
printk (KERN_NOTICE "0x%08x-0x%08x : \"%s\"\n", slave->offset, slave->offset + slave->mtd.size, slave->mtd.name);
**MTD体系结构：**
在linux中提供了MTD（Memory Technology Device，内存技术设备）系统来建立Flash针对linux的统一、抽象的接口
引入MTD后，linux系统中的Flash设备驱动及接口可分为4层:
设备节点
MTD设备层
MTD原始设备层
硬件驱动层
**硬件驱动层：**Flash硬件驱动层负责底层硬件设备实际的读、写、擦除，Linux MTD设备的NAND型Flash驱动位于driver/mtd/nand子目录下
s3c2410对应的nand Flash驱动为s3c2410.c
**MTD原始设备层：**MTD原始设备层由两部分构成，一部分是MTD原始设备的通用代码，另一部分是各个特定Flash的数据，比如分区
主要构成的文件有：
drivers/mtd/mtdcore.c 支持mtd字符设备
driver/mtd/mtdpart.c  支持mtd块设备
**MTD设备层：**基于MTD原始设备，Linux系统可以定义出MTD的块设备（主设备号31) 和字符设备（设备号90），构成MTD设备层
简单的说就是：使用一个mtd层来作为具体的硬件设备驱动和上层文件系统的桥梁。mtd给出了系统中所有mtd设备（nand，nor，diskonchip）的统一组织方式。
mtd层用一个数组struct mtd_info *mtd_table[MAX_MTD_DEVICES]保存系统中所有的设备，mtd设备利用struct mtd_info 这个结构来描述，该结构中描述了存储设备的基本信息和具体操作所需要的内核函数，mtd系统的那个机制主要就是围绕这个结构来实现的。结构体在include/linux/mtd/mtd.h中定义：
struct mtd_info {
u_char type;            //MTD 设备类型
u_int32_t flags;        //MTD设备属性标志
u_int32_t size;         //标示了这个mtd设备的大小
u_int32_t erasesize;    //MTD设备的擦除单元大小，对于NandFlash来说就是Block的大小
u_int32_t oobblock;      //oob区在页内的位置，对于512字节一页的nand来说是512
u_int32_t oobsize;      //oob区的大小，对于512字节一页的nand来说是16
u_int32_t ecctype;      //ecc校验类型
u_int32_t eccsize;      //ecc的大小
char *name;             //设备的名字
int index;              //设备在MTD列表中的位置
struct nand_oobinfo oobinfo; //oob区的信息，包括是否使用ecc，ecc的大小
//以下是关于mtd的一些读写函数，将在nand_base中的nand_scan中重载
int (*erase)
int (*read)
int (*write)
int (*read_ecc)
int (*write_ecc)
int (*read_oob)
int (*read_oob)
void *priv;//设备私有数据指针，对于NandFlash来说指nand芯片的结构
下面看nand_chip结构，在include/linux/mtd/nand.h中定义：
struct nand_chip {
void  __iomem   *IO_ADDR_R;    //这是nandflash的读写寄存器
void  __iomem     *IO_ADDR_W;  
//以下都是nandflash的操作函数，这些函数将根据相应的配置进行重载
u_char    (*read_byte)(struct mtd_info *mtd);
void      (*write_byte)(struct mtd_info *mtd, u_char byte);
u16       (*read_word)(struct mtd_info *mtd);
void      (*write_word)(struct mtd_info *mtd, u16 word);
void     (*write_buf)(struct mtd_info *mtd, const u_char *buf, int len);
void     (*read_buf)(struct mtd_info *mtd, u_char *buf, int len);
int     (*verify_buf)(struct mtd_info *mtd, const u_char *buf, int len);
void     (*select_chip)(struct mtd_info *mtd, int chip);
int     (*block_bad)(struct mtd_info *mtd, loff_t ofs, int getchip);
int      (*block_markbad)(struct mtd_info *mtd, loff_t ofs);
void     (*hwcontrol)(struct mtd_info *mtd, int cmd);
int      (*dev_ready)(struct mtd_info *mtd);
void     (*cmdfunc)(struct mtd_info *mtd, unsigned command, int column, int page_addr);
int      (*waitfunc)(struct mtd_info *mtd, struct nand_chip *this, int state);
int     (*calculate_ecc)(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code);
int   (*correct_data)(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc);
void    (*enable_hwecc)(struct mtd_info *mtd, int mode);
void    (*erase_cmd)(struct mtd_info *mtd, int page);
int     (*scan_bbt)(struct mtd_info *mtd);
int       eccmode;     //ecc的校验模式（软件，硬件）
int       chip_delay;  //芯片时序延迟参数
int       page_shift;  //页偏移，对于512B/页的，一般是9
u_char    *data_buf;   //数据缓存区
**跟NAND操作相关的函数：**
1、 nand_base.c：
定义了NAND驱动中对NAND芯片最基本的操作函数和操作流程，如擦除、读写page、读写oob等。当然这些函数都只是进行一些常规的操作，若你的系统在对NAND操作时有一些特殊的动作，则需要在你自己的驱动代码中进行定义。
2、 nand_bbt.c：
定义了NAND驱动中与坏块管理有关的函数和结构体。
3、 nand_ids.c：
定义了两个全局类型的结构体：struct nand_flash_dev nand_flash_ids[ ]和struct nand_manufacturers nand_manuf_ids[ ]。其中前者定义了一些NAND芯片的类型，后者定义了NAND芯片的几个厂商。NAND芯片的ID至少包含两项内容：厂商ID和厂商为自己的NAND芯片定义的芯片ID。当NAND加载时会找这两个结构体，读出ID，如果找不到，就会加载失败。
4、 nand_ecc.c：
定义了NAND驱动中与softeware ECC有关的函数和结构体，若你的系统支持hardware ECC，且不需要software ECC，则该文件也不需理会。
我们需要关心的是/nand/s3c2410,这个文件实现的是s3c2410/2440nandflash控制器最基本的硬件操作，读写擦除操作由上层函数完成。
**s3c2410.c分析：**
首先看一下要用到的结构体的注册：
struct s3c2410_nand_mtd {
    struct mtd_info      mtd;   //mtd_info的结构体
    struct nand_chip     chip;  //nand_chip的结构体
    struct s3c2410_nand_set     *set;
    struct s3c2410_nand_info    *info;
    int           scan_res;
};
enum s3c_cpu_type {  //用来枚举CPU类型
    TYPE_S3C2410,
    TYPE_S3C2412,
    TYPE_S3C2440,
};
struct s3c2410_nand_info {  
    /* mtd info */
    struct nand_hw_control      controller;
    struct s3c2410_nand_mtd     *mtds;
    struct s3c2410_platform_nand    *platform;
    /* device info */
    struct device        *device;
    struct resource          *area;
    struct clk        *clk;
    void __iomem         *regs;
    void __iomem         *sel_reg;
    int           sel_bit;
    int           mtd_count;
    unsigned long        save_nfconf;
    enum s3c_cpu_type    cpu_type;
};
设备的注册：
static int __init s3c2410_nand_init(void)
{
    printk("S3C24XX NAND Driver, (c) 2004 Simtec Electronics\n");
    platform_driver_register(&s3c2412_nand_driver);
    platform_driver_register(&s3c2440_nand_driver);
    return platform_driver_register(&s3c2410_nand_driver);
}
platform_driver_register向内核注册设备，同时支持这三种CPU。
&s3c2440_nand_driver是一个platform_driver类型的结构体：
static struct platform_driver s3c2440_nand_driver = {
    .probe     = s3c2440_nand_probe,
    .remove   = s3c2410_nand_remove,
    .suspend   = s3c24xx_nand_suspend,
    .resume    = s3c24xx_nand_resume,
    .driver    = {
       .name  = "s3c2440-nand",
       .owner = THIS_MODULE,
    },
};
最主要的函数就是s3c2440_nand_probe，（调用s3c24XX_nand_probe）,完成对nand设备的探测，
static int s3c24xx_nand_probe(struct platform_device *pdev,
                 enum s3c_cpu_type cpu_type)
{
       /*主要完成一些硬件的初始化，其中调用函数：*/
**s3c2410_nand_init_chip**(info, nmtd, sets);
       /*init_chip结束后，调用nand_scan完成对flash的探测及mtd_info读写函数的赋值*/
nmtd->scan_res = **nand_scan**(&nmtd->mtd, (sets) ? sets->nr_chips : 1);
       if (nmtd->scan_res == 0) {
**s3c2410_nand_add_partition**(info, nmtd, sets);
       }
}
Nand_scan是在初始化nand的时候对nand进行的一步非常好重要的操作，在nand_scan中会对我们所写的关于特定芯片的读写函数重载到nand_chip结构中去，并会将mtd_info结构体中的函数用nand的函数来重载，实现了mtd到底层驱动的联系。
并且在nand_scan函数中会通过读取nand芯片的设备号和厂家号自动在芯片列表中寻找相应的型号和参数，并将其注册进去。
static void s3c2410_nand_init_chip(struct s3c2410_nand_info *info,
                 struct s3c2410_nand_mtd *nmtd,
                 struct s3c2410_nand_set *set)
{
    struct nand_chip *chip = &nmtd->chip;
    void __iomem *regs = info->regs;
    /*以下都是对chip赋值，对应nand_chip中的函数*/
    chip->write_buf    = s3c2410_nand_write_buf;  //写buf
    chip->read_buf     = s3c2410_nand_read_buf;   //读buf
    chip->select_chip  = s3c2410_nand_select_chip;//片选
    chip->chip_delay   = 50;
    chip->priv    = nmtd;
    chip->options    = 0;
    chip->controller   = &info->controller; //？？
    switch (info->cpu_type) {
    case TYPE_S3C2440:
       chip->IO_ADDR_W = regs + S3C2440_NFDATA;  //数据寄存器
       info->sel_reg   = regs + S3C2440_NFCONT;  //控制寄存器
       info->sel_bit = S3C2440_NFCONT_nFCE;
       chip->cmd_ctrl  = s3c2440_nand_hwcontrol; //硬件控制
       chip->dev_ready = s3c2440_nand_devready;  //设备就绪
       chip->read_buf  = s3c2440_nand_read_buf;  //读buf
       chip->write_buf   = s3c2440_nand_write_buf;//写buf
       break;
    }
    chip->IO_ADDR_R = chip->IO_ADDR_W; //读写寄存器都是同一个
    nmtd->info    = info;
    nmtd->mtd.priv       = chip; //私有数据指针指向chip
    nmtd->mtd.owner    = THIS_MODULE;
    nmtd->set     = set;
    /*后面是和ECC校验有关的，省略*/
}
初始化后，实现对nand的基本硬件操作就可以了，包括以下函数：
s3c2410_nand_inithw  //初始化硬件，在probe中调用
s3c2410_nand_select_chip  //片选
s3c2440_nand_hwcontrol  //硬件控制，其实就是片选
s3c2440_nand_devready  //设备就绪
s3c2440_nand_enable_hwecc  //使能硬件ECC校验
s3c2440_nand_calculate_ecc  //计算ECC
s3c2440_nand_read_buf  s3c2440_nand_write_buf
**注册nand设备到MTD原始设备层：（这个函数由probe调用）**
#ifdef CONFIG_MTD_PARTITIONS   //如果定义了MTD分区
static int s3c2410_nand_add_partition(struct s3c2410_nand_info *info,
                    struct s3c2410_nand_mtd *mtd,
                    struct s3c2410_nand_set *set)
{
    if (set == NULL)
       return add_mtd_device(&mtd->mtd);
    if (set->nr_partitions > 0 && set->partitions != NULL) {
       return add_mtd_partitions(&mtd->mtd, set->partitions, set->nr_partitions);
    }
    return add_mtd_device(&mtd->mtd);
}
#else
注册设备用这两个函数：
add_mtd_device  //如果nand整体不分区，用这个，
//该函数在mtdcore.c中实现
add_mtd_partitions  //如果nand是分区结构，用这个，
//该函数在mtdpart.c中实现
同样，注销设备也有两个函数：
del_mtd_device
del_mtd_partitions
NandFlash还有一个分区表结构体，mtd_partition，这个是在arch/arm/plat-s3c24XX/common-smdk.c中定义的。
static struct mtd_partition smdk_default_nand_part[] = {
    [0] = {
       .name  = "boot",
       .size  = 0x00040000,
       .offset    = 0,
    },
    [1] = {
       .name  = "kernel",
       .offset = 0x0004C000,
       .size  = 0x00200000,
    },
    [2] = {
       .name  = "yaffs2",
       .offset = 0x0024C000,
       .size  = 0x03DB0000,
    },
};
记录了当前的nand flash有几个分区，每个分区的名字，大小，偏移量是多少
系统就是依靠这些分区表找到各个文件系统的
这些分区表nand中的文件系统没有必然关系，分区表只是把flash分成不同的部分
如果自己编写一个nandflash驱动，只需要填充这三个结构体：
Mtd_info     nand_chip     mtd_partition
并实现对物理设备的控制，上层的驱动控制已由mtd做好了，不需要关心
**2410NandFlash控制器**
**管脚配置**
D[7:0]: DATA0-7 数据/命令/地址/的输入/输出口（与数据总线共享）
CLE : GPA17  命令锁存使能 (输出)
ALE : GPA18  地址锁存使能（输出）
nFCE : GPA22 NAND Flash 片选使能（输出）
nFRE : GPA20 NAND Flash 读使能 (输出)
nFWE : GPA19 NAND Flash 写使能 (输出)
R/nB : GPA21 NAND Flash 准备好/繁忙（输入）
**相关寄存器：**
NFCONF   NandFlash控制寄存器
 ［15］NandFlash控制器使能/禁止     0 = 禁止   1 = 使能
 ［14：13］保留
 ［12］初始化ECC解码器/编码器    0 = 不初始化   1 = 初始化
 ［11］芯片使能  nFCE控制        0 = 使能       1 = 禁止
 ［10：8］TACLS   持续时间 = HCLK*（TACLS+1）
 ［6：4］ TWRPH0
 ［2：0］ TWRPH1
NFCMD  命令设置寄存器
 ［7：0］ 命令值
NFADDR 地址设置寄存器
 ［7：0］ 存储器地址
NFDATA 数据寄存器
 ［7：0］ 存放数据
NFSTAT 状态寄存器
 ［0］    0 = 存储器忙     1 = 存储器准备好
NFECC  ECC寄存器
 ［23：16］ECC校验码2
 ［15：8］ ECC校验码1
 ［8：0］  ECC校验码0
**写操作：**
写入操作以页为单位。写入必须在擦除之后，否则写入将出错。
页写入周期中包括以下步骤：
写入串行数据输入指令（80h）。然后写入4个字节的地址，最后串行写入数据（528Byte）。串行写入的数据最多为528byte。
串行数据写入完成后，需要写入“页写入确认”指令10h，这条指令将初始化器件内部写入操作。
10h写入之后，nand flash的内部写控制器将自动执行内部写入和校验中必要的算法和时序，
系统可以通过检测R/B的输出，或读状态寄存器的状态位（I/O 6）来判断内部写入是否结束 
**擦除操作：**
擦除操作时以块(16K Byte)为单位进行的
擦除的启动指令为60h,随后的3个时钟周期是块地址。其中只有A14到A25是有效的，而A9到A13是可以忽略的。
块地址之后是擦除确认指令D0h，用来开始内部的擦除操作。
器件检测到擦除确认命令后，在/WE的上升沿启动内部写控制器，开始执行擦除和擦除校验。内部擦除操作完成后，应该检测写状态位（I/O 0），从而了解擦除操作是否成功完成。
**读操作**有两种读模式：
读方式1用于读正常数据；
读方式2用于读附加数据
在初始上电时，器件进入缺省的“读方式1模式”。在这一模式下，页读取操作通过将00h指令写入指令寄存器，接着写入3个地址（一个列地址和2个行地址）来启动。一旦页读指令被器件锁存，下面的页操作就不需要再重复写入指令了。
写入指令和地址后，处理器可以通过对信号线R//B的分析来判断该才作是否完成。
外部控制器可以再以50ns为周期的连续/RE脉冲信号的控制下，从I/O口依次读出数据
备用区域的从512到527地址的数据，可以通过读方式2指令进行指令进行读取（命令为50h）。地址A0～A3设置了备用区域的起始地址，A4～A7被忽略掉 
**时序要求：**
写地址、数据、命令时，nCE、nWE信号必须为低电平，它们在nWE信号的上升沿被锁存。命令锁存使能信号CLE和地址锁存信号ALE用来区分I/O引脚上传输的是命令还是地址。
**寻址方式：**
NAND Flash的寻址方式和NAND Flash的memory组织方式紧密相关。NAND Flash的数据以bit的方式保存在memory cell，一个cell中只能存储一个bit。这些cell以8个或者16个为单位，连成bit line，形成byte(x8)/word(x16)，这就是NAND的数据宽度。
       这些Line会再组成Page，**典型情况下：**通常是528Byte/page或者264Word/page。然后，每32个page形成一个Block，Sizeof(block)=16.5kByte。其中528Byte = 512Byte+16Byte，前512Byte为数据区，后16Byte存放数据校验码等，因此习惯上人们称1page有512个字节，每个Block有16Kbytes；
      现在在一些大容量的FLASH存贮设备中也采用以下配置：2112 Byte /page 或 1056 Word/page；64page/Block；Sizeof(block) = 132kByte；同上：2112 = 2048 +64，人们习惯称一页含2k个字节，一个Block含有64个页，容量为128KB；
**Block是NAND Flash中最大的操作单元，擦除可以按照block或page为单位完成，而编程/读取是按照page为单位完成的**
。
**所以，按照这样的组织方式可以形成所谓的三类地址：**
         -Block  Address   块地址
         -Page   Address   页地址
         -Column Address  列地址
      首先，必须清楚一点，对于NAND Flash来讲，地址和命令只能在I/O[7:0]上传递，数据宽度可以是8位或者16位，但是，对于x16的NAND Device，I/O[15:8]只用于传递数据。
    清楚了这一点，我们就可以开始分析NAND Flash的寻址方式了。
以528Byte/page 总容量64M Byte+512kbyte的NAND器件为例：
因为
**1page=528byte=512byte(Main Area)+16byte(Spare Area)**
**1block=32page = 16kbyte**
**64Mbyte = 4096 Block **
用户数据保存在main area中。
**512byte需要9bit来表示，**对于528byte系列的NAND，这512byte被分成1st half和2nd half,各自的访问由所谓的pointer operation命令来选择，也就是选择了bit8的高低。因此A8就是halfpage pointer，A[7:0]就是所谓的column address。
**32个page需要5bit来表示，占用A[13:9]，**即该page在块内的相对地址。
**Block的地址是由A14以上的bit来表示，**例如64MB的NAND，共4096block，因此，需要12个bit来表示，即A[25:14]，如果是1Gbit的528byte/page的NAND Flash，共8192个block，则block address用A[30:14]表示。
NAND Flash的地址表示为：
**Block Address  **|  **Page Address in block**  |  **half page pointer** | ** Column Address**
地址传送顺序是Column Address , Page Address , Block Address。
例如一个地址：0x00aa55aa
         0000 0000  **1010  1010  0101  0101  1010  1010 **
由于地址只能在I/O[7:0]上传递，因此，必须采用移位的方式进行。
例如，对于64MBx8的NAND flash，地址范围是0~0x3FF_FFFF，只要是这个范围内的数值表示的地址都是有效的。
**以NAND_ADDR为例：**
       第1步是传递column address，就是NAND_ADDR[7:0]，不需移位即可传递到I/O[7:0]上， 而halfpage pointer即bit8是由操作指令决定的，即指令决定在哪个halfpage上进行读写，而真正的bit8的值是don't care的。
       第2步就是将NAND_ADDR右移9位，将NAND_ADDR[16:9]传到I/O[7:0]上;
       第3步将NAND_ADDR[24:17]放到I/O上;
       第4步需要将NAND_ADDR[25]放到I/O上;
       因此，整个地址传递过程需要4步才能完成，即4-step addressing。
       如果NAND Flash的容量是32MB以下，那么，block adress最高位只到bit24，因此寻址只需要3步。
