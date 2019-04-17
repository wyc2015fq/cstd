# vivi代码分析(三) - 一世豁然的专栏 - CSDN博客





2015年06月14日 08:45:21[一世豁然](https://me.csdn.net/Explorer_day)阅读数：546








本博客转载于：[http://www.cnblogs.com/tureno/articles/4445401.html](http://www.cnblogs.com/tureno/articles/4445401.html)








step 5：



    MTD设备初始化。



    关于什么是MTD，为什么要使用MTD，MTD技术的架构是什么，等等，可以参考《Linux MTD源代码分析》（作者：Jim Zeus，2002-04-29）。这份文档的参考价值比较大，猜想作者在当时可能研究了很长时间，毕竟2002年的时候资料还比较缺乏。当然，因为完全分析透彻，方方面面都点透，这份文档还是没有做到。

      在分析代码前先介绍一下MTD(Memory Technology Device)相关的技术。在inux系统中，我们通常会用到不同的存储设备，特别是FLASH设备。为了在使用新的存储设备时，我们能更简便地提供它的驱动程序，在上层应用和硬件驱动的中间，抽象出MTD设备层。驱动层不必关心存储的数据格式如何，比如是FAT32、ETX2还是FFS2或其它。它仅仅提供一些简单的接口，比如读写、擦除及查询。如何组织数据，则是上层应用的事情。MTD层将驱动层提供的函数封装起来，向上层提供统一的接口。这样，上层即可专注于文件系统的实现，而不必关心存储设备的具体操作。

     在我们即将看到的代码中，使用mtd_info数据结构表示一个MTD设备，使用nand_chip数据结构表示一个nand flash芯片。在mtd_info结构中，对nand_flash结构作了封装，向上层提供统一的接口。比如，它根据nand_flash提供的read_data(读一个字节)、read_addr(发送要读的扇区的地址)等函数，构造了一个通用的读函数read，将此函数的指针作为自己的一个成员。而上层要读写flash

时，执行mtd_info中的read、write函数即可。



    vivi采用Linux kernel的架构，所以把Linux kernel的MTD子系统借用过来了，做了一些裁减。可以简单地看成：Flash硬件驱动层和MTD设备层。这样，最终以抽象的统一的接口向vivi提供。



    还是以nand flash启动这个情景为主线，对MTD初始化流程进行分析。下面先从入口开始。



|`ret= mtd_dev_init();`|
|----|




    利用source insight跟踪，看一下此函数的接口定义部分：



|```/* * VIVI Interfaces */#ifdef CONFIG_MTDint write_to_flash(loff_t ofs,size_t len,const u_char *buf,int flag);int mtd_dev_init(void);#else#define write_to_flash(a, b, c, d)(int)(1)#define mtd_dev_init()(int)(1)#endif```|
|----|




    可见，vivi在配置的时候是必须配置MTD功能部分的。如果不配置MTD，那么CONFIG_MTD就不存在定义。由此导致写flash的动作实际上是没有的。也就是说，无法完成写flash的动作。当然，在这里可以做测试，就是使用MTD子系统的vivi把分区等都设置好。然后重新编译一下vivi，把mtd功能去除，做简单的修改（把bon_cmd部分从【lib/command.c】中去掉，否则编译不通过），生成大小为35152字节。给开发板重新上电，利用老的vivi烧写nand flash的vivi分区，完成后做一下reset，于是没有MTD功能的vivi就跑起来了。但是，这样的bootloader仅仅适合于最终的产品阶段，不适合开发，没什么太大的价值。有兴趣倒是可以据此研究一下配置部分，整个引导时间相应的缩短。



    下面【drivers/mtd/mtdcore.c】，看看mtd_dev_init函数，核心部分就是调用mtd_init函数（【drivers/mtd/maps/s3c2410_flash.c】）。



|```int mtd_init(void){    int ret;#ifdef CONFIG_MTD_CFI    ret = cfi_init();#endif#ifdef CONFIG_MTD_SMC    ret = smc_init();#endif#ifdef CONFIG_S3C2410_AMD_BOOT    ret = amd_init();#endif```|
|----|




    可见，vivi现在支持三种类型的存储接口，一种是CFI，也就是Intel发起的一个flash的接口标准，主要就是intel的nor flash系列；一种是smc，智能卡系列接口，nand flash就是通过这个接口实现读写的；一种是AMD的flash系列。选择什么启动方式，就要选择相应的配置项。



    核心部分根据配置应该调用smc_init函数。-->【drivers/mtd/maps/s3c2410_flash.c】。这里最为核心的就是两个数据结构，一个是mtd_info，位于【include/mtd/mtd.h】，如下：

struct mtd_info {

 u_char type;

 u_int32_t flags;

 u_int32_t size;  // Total size of the MTD

 /* "Major" erase size for the device. Na飗e users may take this

  * to be the only erase size available, or may use the more detailed

  * information below if they desire

  */

 u_int32_t erasesize;

 u_int32_t oobblock;  // Size of OOB blocks (e.g. 512)

 u_int32_t oobsize;   // Amount of OOB data per block (e.g. 16)

 u_int32_t ecctype;

 u_int32_t eccsize;

 // Kernel-only stuff starts here.

 char *name;

 int index;

 /* Data for variable erase regions. If numeraseregions is zero,

  * it means that the whole device has erasesize as given above. 

  */

 int numeraseregions;

 struct mtd_erase_region_info *eraseregions; 

 /* This really shouldn't be here. It can go away in 2.5 */

 u_int32_t bank_size;

 struct module *module;

 int (*erase) (struct mtd_info *mtd, struct erase_info *instr);

 /* This stuff for eXecute-In-Place */

 int (*point) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char **mtdbuf);

 /* We probably shouldn't allow XIP if the unpoint isn't a NULL */

 void (*unpoint) (struct mtd_info *mtd, u_char * addr);


 int (*read) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);

 int (*write) (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf);

 int (*read_ecc) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf, u_char *eccbuf);

 int (*write_ecc) (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf, u_char *eccbuf);

 int (*read_oob) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);

 int (*write_oob) (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf);

 /* 

  * Methods to access the protection register area, present in some 

  * flash devices. The user data is one time programmable but the

  * factory data is read only. 

  */

 int (*read_user_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);

 int (*read_fact_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);

 /* This function is not yet implemented */

 int (*write_user_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);


 /* Chip-supported device locking */

 int (*lock) (struct mtd_info *mtd, loff_t ofs, size_t len);

 int (*unlock) (struct mtd_info *mtd, loff_t ofs, size_t len);

 void *priv;

};



mtd_info是表示MTD设备的结构，每个分区也被表示为一个mtd_info，如果有两个MTD设备，每个设备有三个分区，那么在系统中就一共有6个mtd_info结构。关于mtd_info，在《Linux MTD源代码分析》中讲解非常透彻，不过需要注意的是，在vivi的实现中没有使用mtd_table，另外priv指向的是nand_chip，这些都是与Linux下不同的地方，主要是为了简化。**另一个是nand_chip，这个结构则包含了nand flash的所有信息。**

struct nand_chip {

#ifdef CONFIG_MTD_NANDY

 void (*hwcontrol)(int cmd);

 void (*write_cmd)(u_char val);

 void (*write_addr)(u_char val);

 u_char (*read_data)(void);

 void (*write_data)(u_char val);

 void (*wait_for_ready)(void);

 /*spinlock_t chip_lock;*/

 /*wait_queue_head_t wq;*/

 /*nand_state_t state;*/

 int page_shift;

 u_char *data_buf;

 u_char *data_cache;

 int cache_page;

 struct nand_smc_dev *dev;

 u_char spare[SMC_OOB_SIZE];

#else /* CONFIG_MTD_NANDY */

 unsigned long IO_ADDR_R;

 unsigned long IO_ADDR_W;

 void (*hwcontrol)(int cmd);

 int (*dev_ready)(void);

 int chip_delay;

 /*spinlock_t chip_lock;*/

 /*wait_queue_head_t wq;*/

 /*nand_state_t state;*/

 int page_shift;

 u_char *data_buf;

 u_char *data_cache;

 int cache_page;

#ifdef CONFIG_MTD_NAND_ECC

 u_char ecc_code_buf[6];

 u_char reserved[2];

#endif

#endif /* CONFIG_MTD_NANDY */

};



所谓的初始化，其实就是填充处理上述两个数据结构的过程。填充完毕之后，后续的工作都会基于此展开。下面开始看smc_init的代码。



|```mymtd= mmalloc(sizeof(struct mtd_info)+sizeof(struct nand_chip));this = (struct nand_chip *)(&mymtd[1]);```|
|----|




**在这里，第一句参考前面heap的实现代码，重点看第二句代码。这句代码是有一定的技巧性，但是也存在着很大的风险。其中，mymtd是指向struct mtd_info的指针，那么mymtd[1]实际上是等效于*(mymtd + 1)的数学计算模式，注意mymtd并非数组，这里仅仅利用了编译器翻译的特点。对于指针而言，加1实际上增加的指针对应类型的值，在这里地址实际上增加了sizeof(struct mtd_info)，因为前面分配了两块连续的地址空间，所以&(*(mymtd + 1))实际上就是mtd_info数据结构结束的下一个地址，然后实现强制转换，于是this就成为了nand_chip的入口指针了。但是，这里必须要把握好，因为这个地方是不会进行内存的检查的，也就是说，如果你使用了mymtd[2]，那么仍然按照上述公式解析，虽然可以运算，可是就是明显的指针泄漏了，可能会出现意料不到的结果。写了一个测试程序，对这点进行了探讨，要小心内存问题**。



了解清楚了，mymtd指向mtd_info的入口，this指向nand_chip的入口。



|```memset((char*)mymtd, 0,sizeof(struct mtd_info));    memset((char*)this, 0,sizeof(struct nand_chip));```|
|----|




|```mymtd->priv=this;```|
|----|




    上述代码首先初始化这两个结构体，即均为0.然后利用priv把二者联系起来，也就是mymtd通过其成员priv指向this，那么mymtd中的抽闲操作函数，比如read、write等，真正的是通过this来实现的。很明显，this的实现部分属于**flash硬件驱动层**，而mymtd部分则属于MTD设备层，二者的联系就是通过成员priv实现的。



    接下来首先是初始化nand flash设备，这跟前面的基础实验一致。



|```/* set NAND Flash controller */    nfconf = NFCONF;    /* NAND Flash controller enable */    nfconf |= NFCONF_FCTRL_EN;    /* Set flash memory timing */    nfconf &=~NFCONF_TWRPH1;    /* 0x0 */    nfconf |= NFCONF_TWRPH0_3;    /* 0x3 */    nfconf &=~NFCONF_TACLS;     /* 0x0 */    NFCONF = nfconf;```|
|----|




    然后填充nand flash的数据结构的一个实例this，分成了两个部分，nand flash基本操作函数成员的初始化、其余信息的填写。



|```/* Set address of NAND IO lines */    this->hwcontrol= smc_hwcontrol;    this->write_cmd= write_cmd;    this->write_addr= write_addr;    this->read_data= read_data;    this->write_data= write_data;    this->wait_for_ready= wait_for_ready;    /* Chip Enable -> RESET -> Wait for Ready -> Chip Disable */    this->hwcontrol(NAND_CTL_SETNCE);    this->write_cmd(NAND_CMD_RESET);    this->wait_for_ready();    this->hwcontrol(NAND_CTL_CLRNCE);    smc_insert(this);```|
|----|




    上面这些都不难理解，感觉在结构体设计上还是比较出色的，把成员和相应的操作封装起来，面向对象的一种方法。下面看smc_insert，根据刚才填充的nand_flash结构，构造mtd_info结构无非还是按照结构体填写相应的信息，细节部分就不深入探讨了。



|```inline intsmc_insert(struct nand_chip*this){    /* Scan to find existance of the device */    if (smc_scan(mymtd)){        return -ENXIO;    }    /* Allocate memory for internal data buffer */    this->data_buf= mmalloc(sizeof(u_char)*             (mymtd->oobblock+ mymtd->oobsize));    if (!this->data_buf){        printk("Unable to allocate NAND data buffer for S3C2410./n");        this->data_buf=NULL;        return -ENOMEM;    }    return 0;}```|
|----|




    第一部分扫描填充mymtd数据结构。后面主要用于nand flash的oob缓冲处理。具体部分可以参考《s3c2410完全开发》。我们先来看看smc_scan函数的执行(drivers/mtd/nand/smc_core.c这个文件中包含的是nand flash中绝大多数真正进行操作的函数)：

int smc_scan(struct mtd_info *mtd)

{

 int i, nand_maf_id, nand_dev_id;  //定义flash的厂家ID和设备ID

 struct nand_chip *this = mtd->priv; //获得与mtd设备相联系的真正的flash设备结构

 /* Select the device */

 nand_select(); //**#define nand_select() this->hwcontrol(NAND_CTL_SETNCE);  nand_command(mtd, NAND_CMD_RESET, -1, -1);   udelay(10);这三句代码和我们之前在nand章节中讲解的片选flash是一个意思，先将使能nand的那位置1也就是片选nand，然后想nand发送reset命令，然后等待一段时间。**

 /* Send the command for reading device ID */

 nand_command(mtd, NAND_CMD_READID, 0x00, -1);//向nand发送读ID的命令

 this->wait_for_ready(); //等待nand结果数据准备好

 /* Read manufacturer and device IDs */

 nand_maf_id = this->read_data();//nand数据准备好后，通过read_data可以相继的读出厂家ID和设备ID

 nand_dev_id = this->read_data();

 /* Print and sotre flash device information */

 for (i = 0; nand_flash_ids[i].name != NULL; i++) { //在数组nand_flash_ids中查找与ID相符合的项，可以看到下面对数组说明

  if (nand_maf_id == nand_flash_ids[i].manufacture_id &&

      nand_dev_id == nand_flash_ids[i].model_id) {

#ifdef USE_256BYTE_NAND_FLASH

   if (!mtd->size) {      //下面都是根据在nand_flash_ids数组中找到相符的项，然后从对应的nand_flash_dev结构体中取出对应的属性值来填充mtd_info结构

    mtd->name = nand_flash_ids[i].name;

    mtd->erasesize = nand_flash_ids[i].erasesize;

    mtd->size = (1 << nand_flash_ids[i].chipshift); //

    mtd->eccsize = 256;

    if (nand_flash_ids[i].page256) {

     mtd->oobblock = 256;

     mtd->oobsize = 8;

     this->page_shift = 8;

    } else {

     mtd->oobblock = 512;

     mtd->oobsize = 16;

     this->page_shift = 9;

    }

    this->dev = &nand_smc_info[GET_DI_NUM(nand_flash_ids[i].chipshift)];

   }

#else

   if (!(mtd->size) && !(nand_flash_ids[i].page256)) {

    mtd->name = nand_flash_ids[i].name;

    mtd->erasesize = nand_flash_ids[i].erasesize;

    mtd->size = (1 << nand_flash_ids[i].chipshift);

    mtd->eccsize = 256;

    mtd->oobblock = 512;

    mtd->oobsize = 16;

    this->page_shift = 9;

    this->dev = &nand_smc_info[GET_DI_NUM(nand_flash_ids[i].chipshift)];

   }

#endif

   printk("NAND device: Manufacture ID:" /

    " 0x%02x, Chip ID: 0x%02x (%s)/n",

    nand_maf_id, nand_dev_id, mtd->name);

   break;

  }     

 }

 /* De-select the device */
nand_deselect();   //在对nand flash操作完后，需要禁止nand flash#define nand_deselect() this->hwcontrol(NAND_CTL_CLRNCE);也就是将对应的位置0

 /* Print warning message for no device */

 if (!mtd->size) {

  printk("No NAND device found!!!/n");

  return 1;

 }

 /* Fill in remaining MTD driver data */

 mtd->type = MTD_NANDFLASH;
mtd->flags = MTD_CAP_NANDFLASH | MTD_ECC; //填充mtd_info结构体中的函数指针，这些函数大多都是在smc_core.c定义

 mtd->module = NULL;

 mtd->ecctype = MTD_ECC_SW;

 mtd->erase = nand_erase;

 mtd->point = NULL;

 mtd->unpoint = NULL;

 mtd->read = nand_read;

 mtd->write = nand_write;

 mtd->read_ecc = nand_read_ecc;

 mtd->write_ecc = nand_write_ecc;

 mtd->read_oob = nand_read_oob;

 mtd->write_oob = nand_write_oob;

 mtd->lock = NULL;

 mtd->unlock = NULL;

 /* Return happy */

 return 0;

}



    这里重点是学习一种结构体的构造技巧。



首先构造一级数据结构，表示抽象实体。例如：



|```struct nand_flash_dev{    char * name; //设备名    int manufacture_id; //flash的厂家ID    int model_id; //flash的设备ID    int chipshift; //片选移位数，用来构建flash的大小    char page256; //    char pageadrlen;    unsigned long erasesize;};```|
|----|




    然后构造实例集合，表现形式就是一个大的数组,nand_flash_ids是nand_flash_dev结构的数组，里面存放的是世界上比较常用的nand flash型号的一些特性。



|```staticstruct nand_flash_dev nand_flash_ids[]={    {"Toshiba TC5816BDC", NAND_MFR_TOSHIBA, 0x64, 21, 1, 2, 0x1000},    // 2Mb 5V````    ... ....````{"Samsung K9D1G08V0M", NAND_MFR_SAMSUNG, 0x79, 27, 0, 3, 0x4000},    // 128Mb    {NULL,}};```|
|----|




**执行完mtd_dev_init后，我们得到了一个mtd_info结构的全局变量(mymtd指向它)，以后对nand flash的操作，直接通过mymtd提供的接口进行。**

    这样修改扩展等等后续的操作就简便多了。抽象的能力及其训练在读代码的时候是可以很好的学习的，在vivi中，多处都采用了这种设计原则，应该掌握并利用。



step 6：



    此部分的功能是把vivi可能用到的所有私有参数都放在预先规划的内存区域，大小为48K，基地址为0x33df0000。在内存的分配示意图方面，《s3c2410完全开发》已经比较详尽，就不放在这里了。到此为止，vivi作为bootloader的三大核心任务：initialise various devices, and eventually call the Linux kernel,passing information to
 the kernel.，现在只是完成第一方面的工作，设备初始化基本完成，实际上step 6是为**启动Linux内核**和**传递参数**做准备的，**把vivi的私有信息，内核启动参数，mtd分区信息等都放到特定的内存区域，**等待后面两个重要工作使用（在step 8完成，后面的step 7也是为step
 8服务的）。**这48K区域分为三个组成部分：MTD参数、vivi parameter、Linux启动命令。**每块的具体内容框架一致，以vivi param tlb这个情景为主线进行分析：



入口：



|`init_priv_data();`|
|----|




进入【lib/priv_data/rw.c】--init_priv_data()



|```intinit_priv_data(void){    int ret_def;#ifdef CONFIG_PARSE_PRIV_DATA    int ret_saved;#endif    ret_def = get_default_priv_data();#ifdef CONFIG_PARSE_PRIV_DATA    ret_saved = load_saved_priv_data();    if (ret_def&& ret_saved){        printk("Could not found vivi parameters./n");        return -1;    } else if (ret_saved && !ret_def){        printk("Could not found stored vivi parameters.");        printk(" Use default vivi parameters./n");    } else {        printk("Found saved vivi parameters./n");    } #else    if (ret_def){        printk("Could not found vivi parameters/n");        return -1;    } else {        printk("Found default vivi parameters/n");    }#endif    return 0;```|
|----|




  此函数将启动内核的命令参数取出，存放在内存特定的位置中。这些参数来源有两个：vivi预设的默认参数，用户设置的参数(用户设置的参数存放在nand flash上，也就是我们进入到vivi控制端界面利用setenv来进行设置的参数，主要是以这个为主)。**init_priv_data先读出默认参数，存放在“VIVI_PRIV_RAM_BASE”开始的内存上；然后读取用户参数，若成功则用用户参数覆盖默认参数，否则使用默认参数。**

    init_priv_data函数分别调用 **get_default_priv_data** 函 数 和**load_saved_priv_data**函数来读取**默认参数**和**用户参数**。这些参数分为3类：

  a．vivi自身使用的一些参数，比如传输文件时的使用的协议等 

  b．linux启动命令 

  c．nand flash的分区参数      

下面分为两步：首先读取默认设置到特定的内存区域，然后读取nand flash的param区域的信息，如果读取成功，就覆盖掉前面的默认设置。首先看第一步，get_default_priv_data--get_default_param_tlb-->



|```static inline int get_default_priv_data(void){ if (get_default_param_tlb()) //获取默认的参数表  return NO_DEFAULT_PARAM; if (get_default_linux_cmd()) //获取到在vivi设置的linux启动命令  return NO_DEFAULT_LINUXCMD; if (get_default_mtd_partition()) //这步很重要，主要用来获取到nand flash分区表  return NO_DEFAULT_MTDPART;``````return 0;}``````int get_default_param_tlb(void){    char *src=(char*)&default_vivi_parameters;//这个默认的参数表也就是在程序中自定义的vivi_parameter_t数组，每个参数都用这样一个vivi_parameter_t来表示，default_vivi_parameters则为默认参数数组名，可以参考下面    char *dst=(char*)(VIVI_PRIV_RAM_BASE+ PARAMETER_TLB_OFFSET); //48KB的参数区域从下到上依次是16KB的mtd参数信息，16KB的参数表，16KB的linux启动命令，所以这里PARAMETER_TLB_OFFSET=16KB    int num = default_nb_params;    if (src==NULL) return-1;    /*printk("number of vivi parameters = %d/n", num); */    *(nb_params)= num;    //参数表的长度不可以超过预设内存的大小，可以看到这里每个参数都是由vivi_parameter_t结构体来定义的，这里总共有num个参数，也就对应有num个结构体``````if((sizeof(vivi_parameter_t)*num)> PARAMETER_TLB_SIZE){        printk("Error: too large partition table/n");        return -1;    }``````//首先复制magic number    memcpy(dst, vivi_param_magic, 8);``````//预留下8个字节作为扩展    dst += 16;``````//复制真正的parameter    memcpy(dst, src,(sizeof(vivi_parameter_t)*num));    return 0;}```|
|----|




**内存的入口地址为VIVI_PRIV_RAM_BASE+PARAMETER_TLB_OFFSET****，开始的8个字节放magic number，这里vivi定义为“VIVIPARA”，后面空下8个字节，留作扩展**，从第17个字节开始放置真正的param。这里用到了多处技巧，第一处就是上面刚刚介绍过的数据结构构造技巧，这里的vivi_parameter_t就是一级数据结构：



|```typedefstruct parameter{    char name[MAX_PARAM_NAME];    param_value_t value;    void (*update_func)(param_value_t value);} vivi_parameter_t;```|
|----|




    利用其构造了默认的成员表(下面就是在程序中定义的默认参数表，传递给内核启动)：



|```vivi_parameter_t**default_vivi_parameters**[]={    { "mach_type",            MACH_TYPE,    NULL},    { "media_type",            MT_S3C2410,    NULL},    { "boot_mem_base",        0x30000000,    NULL},    { "baudrate",            UART_BAUD_RATE,    NULL},    { "xmodem_one_nak",        0,        NULL},    { "xmodem_initial_timeout",    300000,        NULL},    { "xmodem_timeout",        1000000,    NULL},    { "ymodem_initial_timeout",    1500000,    NULL},    { "boot_delay",            0x1000000,    NULL}};```|
|----|




    我们这时就可以很清楚的看到param show列出的配置参数了。



另外一个技巧就是利用宏计算数组长度。



|`int default_nb_params= ARRAY_SIZE(default_vivi_parameters);`|
|----|




    其中ARRAY_SIZE为：



|```#define ARRAY_SIZE(x)(sizeof(x)/sizeof((x)[0]))```|
|----|




    这是从Linux kernel中拿来的，也是值得学习和利用的地方。

我们再来看看get_default_linux_cmd()用来获取vivi中设置的linux启动命令：

int get_default_linux_cmd(void)

{

 char *src = **linux_cmd**;

 char *dst = (char *)(VIVI_PRIV_RAM_BASE + LINUX_CMD_OFFSET);

 if (src == NULL) return -1;

 memcpy((char *)dst, (char *)linux_cmd_magic, 8);

 dst += 8;

 memcpy(dst, src, (strlen(src) + 1));

 return 0;

}

 其实这三个函数大部分都是一样的，只不过赋值的源地址指向不同而已，在这里指向的是linux_cmd，也就是定义启动命令指针，然后将其拷贝到VIVI_PRIV_RAM_BASE + LINUX_CMD_OFFSET处，在这里linux_CMD_OFFSET为32KB，同样也是先拷贝magic number，然后留下8个字节扩展。可以看看linux_cmd的定义（这个定义是在arch/s3c2410/smdk.c中定义）：

**char linux_cmd[] = "noinitrd root=/dev/bon/2 init=/linuxrc console=ttyS0";这个说白了也不是什么启动命令，也是传递给linux内核启动时候的参数，也就相当于vivi中的bootargs参数。**



**主要来看看很重要的nand flash分区信息的获取get_default_mtd_partition()：**

**具体的函数就不用分析啦，因为跟上面分析的两个函数一样，只是默认分区信息的源地址不同而已**

**char *src_parts = (char *)&default_mtd_partitions;这里看到将分区的信息存放在了default_mtd_partitions数组中，同样在smdk.c中可以找到这个数组的定义：**

#ifdef CONFIG_S3C2410_NAND_BOOT
**mtd_partition_t** default_mtd_partitions[] = {

 {

  name:  "vivi",

  offset:  0,

  size:  0x00020000,

  flag:  0

 }, {

  name:  "param",

  offset:  0x00020000,

  size:  0x00010000,

  flag:  0

 }, {

  name:  "kernel",

  offset:  0x00030000,

  size:  0x001d0000,  // 2M sector  

  flag:  0

 }, {

  name:  "root",

  offset:  0x00200000, 

  size:  0x03000000, 

  flag:  MF_BONFS

 }

};在这里在flash分为vivi区，参数区，内核区，文件系统区，我们在vivi期间会将对应的内核，文件系统都会放到指定的区域，然后将信息传递给内核，这样内核在启动时会知道从哪地方加载内核，和文件系统。

同样我们也可以来看看，vivi是怎样去nand上读取用户设置的参数信息的:（下面的宏变量都是在smdk2410.h中定义）

int load_saved_priv_data(void)

{

 char *buf = (char *)(DRAM_BASE);

 char *dst = (char *)(VIVI_PRIV_RAM_BASE);

 if (**read_saved_priv_data_blk(buf))**{

  printk("invalid (saved) parameter block/n");

  return -1;

 }

 /* load parameter table */

 if (strncmp((buf + PARAMETER_TLB_OFFSET), vivi_param_magic, 8) != 0)

  return WRONG_MAGIC_PARAM;

 memcpy(dst + PARAMETER_TLB_OFFSET, buf + PARAMETER_TLB_OFFSET, 

  PARAMETER_TLB_SIZE);

 /* load linux command line */

 if (strncmp((buf + LINUX_CMD_OFFSET), linux_cmd_magic, 8) != 0)

  return WRONG_MAGIC_LINUXCMD;

 memcpy((dst + LINUX_CMD_OFFSET), buf + LINUX_CMD_OFFSET, LINUX_CMD_SIZE);

 /* load mtd partition table */ 

 if (strncmp(buf + MTD_PART_OFFSET, mtd_part_magic, 8) != 0)

  return WRONG_MAGIC_MTDPART;

 memcpy(dst + MTD_PART_OFFSET, buf + MTD_PART_OFFSET, MTD_PART_SIZE);

 return 0;

}

接着来看看read_saved_priv_data_blk函数：

int read_saved_priv_data_blk(char *buf)

{

 char *src = (char *)(VIVI_PRIV_ROM_BASE); //VIVI_PRIV_ROM_BASE为flash上参数区域的起始地址

 size_t size = (size_t)(VIVI_PRIV_SIZE); //VIVI_PRIV_SIZE也就是这三种类型的参数大小也就是48KB

#ifdef CONFIG_USE_PARAM_BLK

 {    //如果是在nand flash上定义了param分区，我们则直接去得到param分区的mtd_partition_t结构体

  mtd_partition_t *part = get_mtd_partition("param");

  if (part == NULL) {

   printk("Could not found 'param' partition/n");

   return -1;

  }

  src = (char *)part->offset; //然后获取到该结构体中定义的该分区的偏移地址，也真正就是存放参数的地址

 }

#endif

 return read_mem(buf, src, size); //然后从参数的源地址中读取参数先存放到sdram的起始地址也就是0x30000000,然后再拷贝到sdram的参数区，也就是load_saved_priv_data函数中的操作。read_mem函数里面也就是nand_read_ll函数，读nand flash之前已经讲过。

}

    在load阶段内无非就是找到param分区，然后根据配置，找到相应的flash硬件驱动（这就是MTD层的作用所在，不过可以看出nand chip的databuf确实没有起到作用，现在也未看出这部分究竟用在何处）。然后就是读操作。当然，读取出来的信息先放到临时缓冲区，判断头部的magic number，如果符合则说明是正确的分区信息，然后把信息从临时缓冲区复制到对应的默认配置区，这样就完成了真正的配置。



    其实这个地方可以改进。首先看看param分区是否有合适的分区信息，如果有，直接读取到vivi parameter区域，不需要再读取默认的配置信息；如果没有合适的分区信息，然后读取默认的配置信息。这样在用户修正了分区信息时，不必再读取默认的配置信息，这也算是一处优化。



step 7：



调用add_command()函数，增加vivi作为终端时命令的相应处理函数。其实，这种机制还是比较简单的，就是利用了**链表**。



整个命令处理机制及其初始化的实现是在【lib/command.c】中完成的，包括添加命令、查找命令、执行命令、解析命令行等等。具体的命令函数则在相应的模块里面，这样形成了一个2层的软件架构：顶部管理层+底部执行层。维护的核心就是一个数据结构user_command：



|```typedefstruct user_command{    const char*name;    void (*cmdfunc)(int argc, const char **);    struct user_command *next_cmd;    const char*helpstr;} user_command_t;```|
|----|




 第一个成员是指向name字符串的指针表示命令的名字，第二个成员就是命令的处理函数，第三个成员是指向下一个命令，第四个成员是帮助信息。**如果你想添加一个命令，那么首先需要构造一个数据结构user_command的实例**，比如：



|```user_command_t help_cmd={    "help",    command_help,    NULL,    "help [{cmds}] /t/t/t-- Help about help?"};```|
|----|




    然后实现命令的真正处理函数command_help。



|```void command_help(int argc,constchar **argv){    user_command_t *curr;    /* help <command>. invoke <command> with 'help' as an argument */    if (argc== 2){        if (strncmp(argv[1],"help",strlen(argv[1]))== 0){            printk("Are you kidding?/n");            return;        }        argv[0]= argv[1];        argv[1]="help";        execcmd(argc, argv);        return;    }    printk("Usage:/n");    curr = head_cmd;    while(curr!=NULL) {        printk(" %s/n", curr->helpstr);        curr = curr->next_cmd;    }}```|
|----|




构造好之后，需要把它加入链表，也就是在init_builtin_cmds中增加add_command(&help_cmd);，其中add_command的实现如下：



|```void add_command(user_command_t*cmd){    if (head_cmd==NULL) { //对链表的操作，表示如果当前vivi中还没有任何命令，则将新加进来的user_command_t作为链表头指针        head_cmd = tail_cmd = cmd;    } else { //如果之前就存在，则将新加进来的user_command_t结构加载链表尾端就行        tail_cmd->next_cmd= cmd;        tail_cmd = cmd;    }    /*printk("Registered '%s' command/n", cmd->name);*/}```|
|----|




    这样，自己如果增加新的程序，就按照如上的步骤添加即可。



    其余具体命令的实现暂时不做解释。



step 8：



    根据情况，要么进入vivi的命令行交互界面，要么直接启动内核。关于此部分的流程分析，有了前面的基础和经验，是不难理解的。很容易通过vivi的打印信息得知进行到了第几步，《s3c2410完全开发》在过程上讲解的也很清楚。所以不打算具体分析了。还是来稍微分析下：

void boot_or_vivi(void)

{

 char c;

 int ret;

 ulong boot_delay;

 boot_delay = get_param_value("boot_delay", &ret);     //从vivi的环境参数boot_delay中获取到等待的时间

  if (ret) boot_delay = DEFAULT_BOOT_DELAY;  //若没有设置该参数的值，则利用默认的等待时间

if (boot_delay == 0) vivi_shell();/* If a value of boot_delay is zero,

  * unconditionally call vivi shell */



 /*

  * wait for a keystroke (or a button press if you want.)

  */

 printk("Press Return to start the LINUX now, any other key for vivi/n");
c = awaitkey(boot_delay, NULL);  //这句话也很重要，在等待时间内获取到用户按下去的键盘码值
 if (((c != '/r') && (c != '/n') && (c != '/0'))) {

  printk("type /"help/" for help./n");

  vivi_shell();

 }
run_autoboot(); //启动内核

 return;

}

void vivi_shell(void)

{

#ifdef CONFIG_SERIAL_TERM
serial_term();

#else

#error there is no terminal.

#endif

}

//drivers/serial/term.c

void serial_term(void)

{

 char cmd_buf[MAX_CMDBUF_SIZE];

 for (;;) {

  printk("%s> ", prompt);

  getcmd(cmd_buf, MAX_CMDBUF_SIZE);

  /* execute a user command */

  if (cmd_buf[0])
exec_string(cmd_buf);//最终还是调用exec_string来进行命令的处理函数分配
 }

}

而对于启动内核的函数：

void run_autoboot(void)

{

 while (1) {

  exec_string("boot");

  printk("Failed 'boot' command. reentering vivi shell/n");

  /* if default boot fails, drop into the shell */

  vivi_shell();

 }

}

其实上也是调用exec_string来执行boot命令，真正启动内核的是在boot命令的处理函数中执行，则要来看看exec_string函数的实现：

void exec_string(char *buf)

{

 int argc;

 char *argv[128];

 char *resid;

 while (*buf) {

  memset(argv, 0, sizeof(argv));

  parseargs(buf, &argc, argv, &resid); //从得到的命令字符串解析出命令

  if (argc > 0)

   execcmd(argc, (const char **)argv);//然后执行命令，在这里其实也是查表，找到命令名和argc相同的user_command_t结构体，然后调用user_command_t结构体中对应的命令处理函数来执行。

  buf = resid;

 }

}

现在翻阅网上资料，有一个问题实际上模模糊糊，如下：



 vivi作为bootloader的一个重要的功能就是向Linux kernel传递启动参数，这个情景究竟是如何完成的呢？虽然网上讨论很多，但是因为vivi具有一点特殊性，所以使得理解上有一定的困难。现在已经比较清晰了，算是回答网友的一个问题，也算是总结，就bootloader如何向kernel传递参数，作为一个情景进行详尽的分析。事先需要说明的是，我们假定vivi为A，Linux
 kernel为B，A要传给B东西，这就是一个通信的过程。要想通信，至少我们得有一个约定，那就是协议。现在存在的协议有两种，一种是基于struct param_struct，不过这种因为其局限性即将作废；一种是基于tags技术。基本的情景框架就是A必须按照协议设置好参数，B呢，就需要来读取解析这些参数。它们之间必须配合好，如果配合不好，那么，kernel是无法引导成功的。现在嵌入式系统的移植，很多时候kernel引导不起来，部分原因就直接来自于参数传递问题。但是设计到这个问题，不能不分析Linux
 kernel的引导过程。现在还不想细致到代码层，只是根据部分代码把Linux kernel启动至获取引导参数的过程从整体上了解清楚，必要的时候辅助相应的代码。这部分内容的详细分析，专门在下篇总结中完成。







学习总结：



    学习一种技术，采用历史的观点是很好的方法。我们现在学习的技术并非最新的理论研究，所以有大量前人的工作经验可以借鉴。站在巨人的肩上，不做无谓的工作，是好的学习方法。我现在的学习观点就是事先要分析阅读前人的相关经验，包括经典书籍、网上资料、网友的经验等等，然后呢，需要对这些知识理解消化，深入，深入再深入，形成自己的认识，转化成自己的经验。正像网友所说，这些都是现成的技术，只要静下心来肯学，就一定能够学好。



    另外，一定要多思考，多动手，多给自己提出问题。没有问题说明你根本就没有深入，有问题才能在解决的过程中提升自己！学习首先从整体上把握流程，然后呢，需要具体的细节。只看整体，不看细节，容易“眼高手低”；只看细节，不看整体，容易“只见树木，不见森林”，提高不到一定的层次。



    这些都是学习过程中的经验总结。欢迎交流！









