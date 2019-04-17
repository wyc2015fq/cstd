# u-boot向linux内核传递启动参数(详细) - DoubleLi - 博客园






U-BOOT 在启动内核时,会向内核传递一些参数.BootLoader 可以通过两种方法传递参数给内核，一种是旧的参数结构方式（parameter_struct)，主要是 2.6 之前的内核使用的方式。另外一种就是现在的 2.6内核在用的参数链表 (tagged list)  方式。这些参数主要包括，系统的根设备标志，页面大小，内存的起始地址和大小，RAMDISK的起始地址和大小，压缩的RAMDISK根文件系统的起始地址和大小，当前内核命令参数等而这些参数是通过 struct tag来传递的。U-boot 把要传递给 kernel 的东西保存在 struct tag 数据结构中，启动 kernel 时，把这个结构体的物理地址传给kernel；Linux kernel 通过这个地址分析出u-boot传递的参数。大家都知道U-Boot启动的时候会将启动参数的地址放入R2中，然后再启动内核。


首先看两个重要的数据结构：
第一个是global_data，定义在include/asm-arm/global_data.h文件中：

typedef    struct    global_data {
    bd_t        *bd;
    unsigned long    flags;
    unsigned long    baudrate;
    unsigned long    have_console;    /* serial_init() was called */
    unsigned long    reloc_off;    /* Relocation Offset */
    unsigned long    env_addr;    /* Address of Environment struct */
    unsigned long    env_valid;    /* Checksum of Environment valid? */
    unsigned long    fb_base;    /* base address of frame buffer */
#ifdef CONFIG_VFD
    unsigned char    vfd_type;    /* display type */
#endif
#if 0
    unsigned long    cpu_clk;    /* CPU clock in Hz!        */
    unsigned long    bus_clk;
    unsigned long    ram_size;    /* RAM size */
    unsigned long    reset_status;    /* reset status register at boot */
#endif
    void        **jt;        /* jump table */
} gd_t;


在同一个文件中有如下定义：

#define DECLARE_GLOBAL_DATA_PTR register volatile gd_t *gd asm ("r8")


在需要使用gd指针的时候，只需要加入DECLARE_GLOBAL_DATA_PTR这句话就可以了。可以知道，gd指针始终是放在r8中的。

其中的第一个变量，bd_t结构体，定义于include/asm-arm/u-boot.h中：



typedef struct bd_info {
    int            bi_baudrate;    /* serial console baudrate */
    unsigned long    bi_ip_addr;    /* IP Address */
    unsigned char    bi_enetaddr[6]; /* Ethernet adress */
    struct environment_s     *bi_env;
    ulong     bi_arch_number;    /* unique id for this board */
    ulong     bi_boot_params;    /* where this board expects params */
    struct                /* RAM configuration */
    {
    ulong start;
    ulong size;
    }             bi_dram[CONFIG_NR_DRAM_BANKS];
#ifdef CONFIG_HAS_ETH1
    /* second onboard ethernet port */
    unsigned char bi_enet1addr[6];
#endif
} bd_t;


bd_t中的变量bi_boot_params，表示传递给内核的参数的位置。

然后看看gd和bd的初始化，在lib_arm/board.c中：

gd = (gd_t*)(_armboot_start - CFG_MALLOC_LEN - sizeof(gd_t));
memset ((void*)gd, 0, sizeof (gd_t));
gd->bd = (bd_t*)((char*)gd - sizeof(bd_t));
memset (gd->bd, 0, sizeof (bd_t));


说明这两个结构体在内存中的位置是在uboot的代码在往下的地址处，所以进行操作的时候不要覆盖了这个位置！

在board/smdk2410/smdk2410.c中，有如下初始化：

gd->bd->bi_boot_params = 0x30000100; 说明参数位置在0x30000100。




现在，具体看看uboot是如何（按什么格式）把参数放入内存中。

内核参数链表的格式和说明可以从内核源代码目录树中的 include/asm-arm/setup.h中找到，参数链表必须以ATAG_CORE 开始，以ATAG_NONE结束。这里的 ATAG_CORE，ATAG_NONE是各个参数的标记，本身是一个32位值，例如：ATAG_CORE=0x54410001。
其它的参数标记还包括： ATAG_MEM32 ， ATAG_INITRD ， ATAG_RAMDISK ，ATAG_COMDLINE 等。每个参数标记就代表一个参数结构体，由各个参数结构体构成了参数链表。参数结构体的定义如下：



struct tag { 
      struct  tag_header  hdr; 
      union { 
                 struct tag_core  core; 
                 struct tag_mem32   mem; 
                 struct tag_videotext videotext; 
                 struct tag_ramdisk  ramdisk; 
                 struct tag_initrd     initrd; 
                 struct tag_serialnr     serialnr; 
                 struct tag_revision  revision; 
                 struct tag_videolfb  videolfb; 
                 struct tag_cmdline  cmdline; 
                 struct tag_acorn       acorn; 
                 struct tag_memclk    memclk; 
              } u; 
};

参数结构体包括两个部分，一个是 tag_header结构体,一个是u联合体。

tag_header结构体的定义如下： 
struct tag_header { 
                 u32 size;   
                 u32 tag; 
}; 
其中 size：表示整个tag 结构体的大小(用字的个数来表示，而不是字节的个数)，等于tag_header的大小加上u联合体的大小，例如，参数结构体 ATAG_CORE 的 size=(sizeof(tag->tag_header)+sizeof(tag->u.core))>>2，一般通过函数 tag_size(struct * tag_xxx)来获得每个参数结构体的size。

其中 tag：表示整个tag 结构体的标记，如：ATAG_CORE等。



联合体 u 包括了所有可选择的内核参数类型，包括：tag_core, tag_mem32，tag_ramdisk等。参数结构体之间的遍历是通过函数 tag_next(struct * tag)来实现的。本系统参数链表包括的结构体有：ATAG_CORE，ATAG_MEM，ATAG_RAMDISK，ATAG_INITRD32，ATAG_CMDLINE，ATAG_END。在整个参数链表中除了参数结构体 ATAG_CORE 和ATAG_END 的位置固定以外，其他参数结构体的顺序是任意的。本 BootLoader所传递的参数链表如下：第一个内核参数结构体，标记为ATAG_CORE，参数类型为 tag_core。每个参数类型的定义请参考源代码文件。


我们知道u-boot传递给内核的参数有很多个，如系统的根设备标志，页面大小，内存的起始地址和大小，RAMDISK的起始地址和大小，压缩的RAMDISK根文件系统的起始地址和大小等，而每个参数我们都是单独的采用一个struct tag来标识的，之前提到的参数标记如ATAG_MEM32，ATAG_INTRD等就是用来标识该tag结构是用来存放的哪种类型的参数。由于不同类型的参数传递的信息内容也不尽相同，为了综合不同参数的tag结构，所以在struct tag结构中定义了一个联合体union，根据不同的参数标记符来选择联合体中不同的结构体来存储参数的内容，如参数标记若为ATAG_MEM32，则联合体中采用struct tag_mem32来存储内存参数的内容。





然而内核是如何从gd->bd->bi_boot_params指定的地址上知道参数从哪里开始以及到哪里结束呢？

所以我们在构建各种参数tag时，在开始时先要构建一个参数标记为ATAG_CORE的tag结构标示从这个tag结构开始接下来就是参数





现来结合代码分析在u-boot中是如何来构建这多个参数的tag结构：



/common/cmd_bootm.c 文件中，bootm 命令对应的do_bootm函数，当分析 uImage 中信息发现 OS 是 Linux 时 ，调用 ./lib_arm/bootm.c 文件中的do_bootm_linux 函数来启动 Linux kernel 。



#if defined (CONFIG_SETUP_MEMORY_TAGS) || /
    defined (CONFIG_CMDLINE_TAG) || /
    defined (CONFIG_INITRD_TAG) || /
    defined (CONFIG_SERIAL_TAG) || /
    defined (CONFIG_REVISION_TAG) || /
    defined (CONFIG_LCD) || /
    defined (CONFIG_VFD)
 setup_start_tag (bd);  //通过bd结构体中参数在内存中的存放地址gd->bd->bi_boot_params来构建初始化的tag结构，表明参数结构的开始
#ifdef CONFIG_SERIAL_TAG
 setup_serial_tag (¶ms); //构建串口参数的tag结构
#endif
#ifdef CONFIG_REVISION_TAG
 setup_revision_tag (¶ms);
#endif
#ifdef CONFIG_SETUP_MEMORY_TAGS
 setup_memory_tags (bd);  //构建内存参数的tag结构
#endif
#ifdef CONFIG_CMDLINE_TAG
 setup_commandline_tag (bd, commandline);  //构建命令行参数的tag结构
#endif
#ifdef CONFIG_INITRD_TAG
 if (initrd_start && initrd_end)
  setup_initrd_tag (bd, initrd_start, initrd_end); //构建ramdisk参数的tag结构
#endif
#if defined (CONFIG_VFD) || defined (CONFIG_LCD)
 setup_videolfb_tag ((gd_t *) gd);
#endif
 setup_end_tag (bd);  //最后是构建参数tag结构结束的tag结构，标示参数已经结束，参数标记为ATAG_NONE
#endif



注意上面参数的tag结构的构建是有宏的约束的，再来看看具体是怎样构建每个tag结构的：



#if defined (CONFIG_SETUP_MEMORY_TAGS) || /
    defined (CONFIG_CMDLINE_TAG) || /
    defined (CONFIG_INITRD_TAG) || /
    defined (CONFIG_SERIAL_TAG) || /
    defined (CONFIG_REVISION_TAG) || /
    defined (CONFIG_LCD) || /
    defined (CONFIG_VFD)
static void setup_start_tag (bd_t *bd)
{
 params = (struct tag *) bd->bi_boot_params;//将指定的内存中存放参数列表的地址强制转化为struct tag的结构，这样便于内核存取各个参数

 params->hdr.tag = ATAG_CORE; //标示这个tag结构是用来标示参数结构的开始
 params->hdr.size = tag_size (tag_core); //存放整个tag结构的大小

 params->u.core.flags = 0;
 params->u.core.pagesize = 0;
 params->u.core.rootdev = 0;

 params = tag_next (params);
}

其中用到了一个重要的指针:params，这是一个指向struct tag的指针，在文件的开始处声明，可以被这个文件中的所有函数访问：static struct tag *params;


 tag和tag_header和内核中的结构一模一样。tag_header中的tag字段表示的是这个tag的类型，在内核和Bootloader中通过一些固定的整形常量来表示：

#define ATAG_CORE 0x54410001
#define ATAG_NONE 0x00000000
#define ATAG_CORE 0x54410001
#define ATAG_MEM 0x54410002
#define ATAG_VIDEOTEXT 0x54410003
#define ATAG_RAMDISK 0x54410004
#define ATAG_INITRD 0x54410005
#define ATAG_INITRD2 0x54420005
#define ATAG_SERIAL 0x54410006
#define ATAG_REVISION 0x54410007
#define ATAG_VIDEOLFB 0x54410008
#define ATAG_CMDLINE 0x54410009
#define ATAG_ACORN 0x41000101
#define ATAG_MEMCLK 0x41000402



上面是初始化tag链表（在SDRAM里），最后一句是作为链表的最关键部分，它的定义是：
#define tag_next(t)    ((struct tag *)((u32 *)(t) + (t)->hdr.size))  作用是指向下一个tag结构体。一般在每个参数的tag结构体的最后都要调用这个宏，内核在遇到这个宏就可以直接跳转到下一个参数tag结构体的地址上来存取。





再来看看其他参数种类的tag结构的构建

#ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd)
{
 int i;

 for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
  params->hdr.tag = ATAG_MEM;
  params->hdr.size = tag_size (tag_mem32);

  params->u.mem.start = bd->bi_dram[i].start;
  params->u.mem.size = bd->bi_dram[i].size;

  params = tag_next (params);
 }
}



其中 defined (CONFIG_SETUP_MEMORY_TAGS) 和 defined (CONFIG_CMDLINE_TAG) 是必不可少的。前者是标记内存的信息，而后者是设置命令行标记（比如“root=/dev/mtdblock2 init=/linuxrc console=ttySAC0”）





到最后可以看到调用：theKernel (0, machid, bd->bi_boot_params);





当然,有很多的宏来选择是否传递相应的tag到linux kenel.实际是这些所以针对于 bd->bi_boot_params 这个变量.这个变量是个整形变量,代表存放所有tag的buffer的地址.
例如,在 smdk2410.c 中的 board_init() 函数中,对于这个变量进行了如下赋值:

    gd->bd->bi_boot_params = 0x30000100;
0x30000100 这个值可以随意指定, 但是要保证和内核中相应的mach_type 一致.以smdk2410为例:
在内核中始终这个值的地方是: arch/arm/mach-s3c2410/mach-smdk2410.c的最后
MACHINE_START(SMDK2410, "SMDK2410")

    .phys_ram    = S3C2410_SDRAM_PA,
    .phys_io    = S3C2410_PA_UART,
    .io_pg_offst    = (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
    .boot_params    = S3C2410_SDRAM_PA + 0x100,
    .map_io        = smdk2410_map_io,
    .init_irq    = smdk2410_init_irq,
    .timer        = &s3c24xx_timer,
MACHINE_END
红色部分的值, 必须等于0x30000100, 否者将会出现无法启动的问题.
内核启动后,会读取0x300000100位置的值, 当然,内核会把这个地址转换成逻辑地址在操作. 因为内核跑起来后,MMU已经工作, 必须要把0x300000100这个物理地址转成逻辑地址然后在操作.对于u- boot传给内核的参数中(tag), 内核比较关系memory的信息,比如memory地址的起始,大小等.如果没有得到,那么内核无法启 动,内核会进入BUG()函数,然后死在那里.
而memory的信息是由 CONFIG_SETUP_MEMORY_TAGS 宏决定的. 因此当这个宏没有被定义时,内核跑不起来. 初始化meminfo时会失败. 现象就是: 
Starting Kernel ...
死掉.
一般需要定义:
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG





// 传给 Kernel 的参数＝ (struct tag *) 型的 bd->bi_boot_params









