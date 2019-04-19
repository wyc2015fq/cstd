# U-boot给kernel传参数和kernel读取参数—struct tag - maopig的专栏 - CSDN博客
2012年03月17日 14:34:05[maopig](https://me.csdn.net/maopig)阅读数：854标签：[struct																[tags																[command																[linux																[linux内核																[平台](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=tags&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
个人分类：[c/C++																[Linux](https://blog.csdn.net/maopig/article/category/850267)](https://blog.csdn.net/maopig/article/category/847675)
U-boot 会给 Linux Kernel 传递很多参数，如：串口， RAM ， videofb 等。而 Linux kernel 也会读取和处理这些参数。两者之间通过 struct tag 来传递参数。
U-boot 把要传递给 kernel 的东西保存在 struct tag 数据结构中，启动 kernel 时，把这个结构体的物理地址传给 kernel ； Linux kernel 通过这个地址，用 parse_tags 分析出传递过来的参数。
本文主要以 U-boot 传递 RAM 和 Linux kernel 读取 RAM 参数为例进行说明。 
1 、u-boot 给kernel 传RAM 参数 
       ./common/cmd_bootm.c 文件中， bootm 命令对应的 do_bootm 函数，当分析 uImage 中信息发现 OS 是 Linux 时 ， 调用 ./lib_arm/bootm.c 文件中的 do_bootm_linux 函数来启动 Linux kernel 。
       在 do_bootm_linux 函数中： 
void do_bootm_linux (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[], 
                   ulong addr, ulong *len_ptr, int verify) 
{ 
...... 
#if defined (CONFIG_SETUP_MEMORY_TAGS) || \ 
    defined (CONFIG_CMDLINE_TAG) || \ 
    defined (CONFIG_INITRD_TAG) || \ 
    defined (CONFIG_SERIAL_TAG) || \ 
    defined (CONFIG_REVISION_TAG) || \ 
    defined (CONFIG_LCD) || \ 
    defined (CONFIG_VFD) 
       setup_start_tag (bd);      // 初始化 tag 结构体开始 
#ifdef CONFIG_SERIAL_TAG 
       setup_serial_tag (¶ms); 
#endif 
#ifdef CONFIG_REVISION_TAG 
       setup_revision_tag (¶ms); 
#endif 
#ifdef CONFIG_SETUP_MEMORY_TAGS 
       setup_memory_tags (bd);      // 设置 RAM 参数 
#endif 
#ifdef CONFIG_CMDLINE_TAG 
       setup_commandline_tag (bd, commandline); 
#endif 
#ifdef CONFIG_INITRD_TAG 
       if (initrd_start && initrd_end) 
              setup_initrd_tag (bd, initrd_start, initrd_end); 
#endif 
#if defined (CONFIG_VFD) || defined (CONFIG_LCD) 
       setup_videolfb_tag ((gd_t *) gd); 
#endif 
       setup_end_tag (bd);              // 初始化 tag 结构体结束 
#endif 
...... 
...... 
       theKernel (0, machid, bd->bi_boot_params); 
// 传给 Kernel 的参数＝ (struct tag *) 型的 bd->bi_boot_params 
//bd->bi_boot_params 在 board_init 函数中初始化如对于 at91rm9200 ，初始化在 at91rm9200dk.c 的 board_init 中进行： bd->bi_boot_params =PHYS_SDRAM + 0x100;
// 这个地址也是所有 taglist 的首地址，见下面的 setup_start_tag 函数 
} 
       对于 setup_start_tag 和 setup_memory_tags 函数说明如下。 
       函数 setup_start_tag 也在此文件中定义，如下： 
static void setup_start_tag (bd_t *bd) 
{ 
       params = (struct tag *) bd->bi_boot_params; 
// 初始化 (struct tag *) 型的全局变量 params 为bd->bi_boot_params 的地址，之后的setup tags 相关函数如下面的 setup_memory_tags 就把其它 tag 的数据放在此地址的偏移地址上。
       params->hdr.tag = ATAG_CORE; 
       params->hdr.size = tag_size (tag_core); 
       params->u.core.flags = 0; 
       params->u.core.pagesize = 0; 
       params->u.core.rootdev = 0; 
       params = tag_next (params); 
} 
RAM 相关参数在 bootm.c 中的函数 setup_memory_tags 中初始化： 
static void setup_memory_tags (bd_t *bd) 
{ 
       int i; 
       for (i = 0; i 
              params->hdr.tag = ATAG_MEM; 
              params->hdr.size = tag_size (tag_mem32); 
              params->u.mem.start = bd->bi_dram*.start;               params->u.mem.size = bd->bi_dram*.size;               params = tag_next (params);        }                   // 初始化内存相关 tag } 2 、Kernel 读取U-boot 传递的相关参数对于 Linux Kernel ， ARM 平台启动时，先执行 arch/arm/kernel/head.S ，此文件会调用 arch/arm/kernel/head-common.S 中的函数，并最后调用 start_kernel ：...... b     start_kernel ...... init/main.c 中的 start_kernel 函数中会调用 setup_arch 函数来处理各种平台相关的动作，包括了 u-boot 传递过来参数的分析和保存：start_kernel() { ......        setup_arch(&command_line); ...... }        其中， setup_arch 函数在 arch/arm/kernel/setup.c 文件中实现，如下： void __init setup_arch(char **cmdline_p) {        struct tag *tags = (struct tag *)&init_tags;        struct machine_desc *mdesc;        char *from = default_command_line;        setup_processor();        mdesc = setup_machine(machine_arch_type);        machine_name = mdesc->name;        if (mdesc->soft_reboot)               reboot_setup("s");        if (__atags_pointer)              // 指向各种 tag 起始位置的指针，定义如下： //unsigned int __atags_pointer  __initdata; // 此指针指向 __initdata 段，各种 tag 的信息保存在这个段中。               tags = phys_to_virt(__atags_pointer);        else if (mdesc->boot_params)               tags = phys_to_virt(mdesc->boot_params);        if (tags->hdr.tag != ATAG_CORE)               convert_to_tag_list(tags);        if (tags->hdr.tag != ATAG_CORE)               tags = (struct tag *)&init_tags;        if (mdesc->fixup)               mdesc->fixup(mdesc, tags, &from, &meminfo);        if (tags->hdr.tag == ATAG_CORE) {               if (meminfo.nr_banks != 0)                      squash_mem_tags(tags);               save_atags(tags);               parse_tags(tags);  // 处理各种 tags ，其中包括了 RAM 参数的处理。 // 这个函数处理如下 tags ： __tagtable(ATAG_MEM, parse_tag_mem32); __tagtable(ATAG_VIDEOTEXT, parse_tag_videotext); __tagtable(ATAG_RAMDISK, parse_tag_ramdisk); __tagtable(ATAG_SERIAL, parse_tag_serialnr); __tagtable(ATAG_REVISION, parse_tag_revision); __tagtable(ATAG_CMDLINE, parse_tag_cmdline);        }        init_mm.start_code = (unsigned long) &_text;        init_mm.end_code   = (unsigned long) &_etext;        init_mm.end_data   = (unsigned long) &_edata;        init_mm.brk       = (unsigned long) &_end;        memcpy(boot_command_line, from, COMMAND_LINE_SIZE);        boot_command_line[COMMAND_LINE_SIZE-1] = '\0';        parse_cmdline(cmdline_p, from);  // 处理编译内核时指定的 cmdline 或 u-boot 传递的 cmdline       paging_init(&meminfo, mdesc);        request_standard_resources(&meminfo, mdesc); #ifdef CONFIG_SMP        smp_init_cpus(); #endif        cpu_init();        init_arch_irq = mdesc->init_irq;        system_timer = mdesc->timer;        init_machine = mdesc->init_machine; #ifdef CONFIG_VT #if defined(CONFIG_VGA_CONSOLE)        conswitchp = &vga_con; #elif defined(CONFIG_DUMMY_CONSOLE)        conswitchp = &dummy_con; #endif #endif        early_trap_init(); } 对于处理 RAM 的 tag ，调用了 parse_tag_mem32 函数： static int __init parse_tag_mem32(const struct tag *tag) { ......        arm_add_memory(tag->u.mem.start, tag->u.mem.size); ...... } __tagtable(ATAG_MEM, parse_tag_mem32);        上述的 arm_add_memory 函数定义如下： static void __init arm_add_memory(unsigned long start, unsigned long size) {        struct membank *bank;        size -= start & ~PAGE_MASK;        bank = &meminfo.bank[meminfo.nr_banks++];        bank->start = PAGE_ALIGN(start);        bank->size  = size & PAGE_MASK;        bank->node  = PHYS_TO_NID(start); }        如上可见， parse_tag_mem32 函数调用 arm_add_memory 函数把 RAM 的 start 和 size 等参数保存到了 meminfo 结构的 meminfo 结构体中。最后，在 setup_arch 中执行下面语句：       paging_init(&meminfo, mdesc);        对有 MMU 的平台上调用 arch/arm/mm/nommu.c 中的 paging_init ，否则调用 arch/arm/mm/mmu.c 中的 paging_init 函数。这里暂不分析 mmu.c 中的 paging_init 函数。3 、关于U-boot 中的bd 和gd U-boot 中有一个用来保存很多有用信息的全局结构体－－ gd_t （ global data 缩写），其中包括了 bd 变量，可以说 gd_t 结构体包括了 u-boot 中所有重要全局变量。最后传递给内核的参数，都是从 gd 和 bd 中来的，如上述的 setup_memory_tags 函数作用就是用 bd 中的值来初始化 RAM 相应的 tag 。对于 ARM 平台这个结构体的定义大致如下： include/asm-arm/global_data.h typedef    struct      global_data {        bd_t        *bd;        unsigned long  flags;        unsigned long  baudrate;        unsigned long  have_console; /* serial_init() was called */        unsigned long  reloc_off;       /* Relocation Offset */        unsigned long  env_addr;       /* Address  of Environment struct */        unsigned long  env_valid;       /* Checksum of Environment valid? */        unsigned long  fb_base;  /* base address of frame buffer */        void        **jt;        /* jump table */ } gd_t; 在 U-boot 中使用 gd 结构之前要用先用宏 DECLARE_GLOBAL_DATA_PTR 来声明。这个宏的定义如下： include/asm-arm/global_data.h #define DECLARE_GLOBAL_DATA_PTR register volatile gd_t *gd asm ("r8") 从这个宏的定义可以看出， gd 是一个保存在 ARM 的 r8 寄存器中的 gd_t 结构体的指针。 说明：本文的版本为U-boot-1.3.4 、Linux-2.6.28 ，平台是ARM 。 //补充一下: 来自:http://hi.baidu.com/armfans/blog/item/306cd5035f24ff084afb514b.htmlbootloader巧妙地利用函数指针及传参规范将R0:0x0，R1:机器号，R2:参数地址传递给内核．由于R0，R1比较简单，不需要再作说明．需要花点时间了解的是R2寄存器．　　R2寄存器传递的是一个指针，这个指针指向一个TAG区域．UBOOT和Linux内核之间正是通过这个扩展了的TAG区域来进行复杂参数的传递，如 command line，文件系统信息等等，用户也可以扩展这个TAG来进行更多参数的传递．TAG区域存放的地址，也就是R2的值，是在/board /yourboard/youboard.c里的board_init函数中初始化的，如在UB4020中初始化为：gd->bd->bi_boot_params = 0x30000100;，这是一个绝对地址．　　TAG区的结构比较简单，可以视为一个一个TAG的排列（数组？），每一个TAG传递一种特定类型的参数．各种系统TAG的定义可以参考./include/asm-arm/setup.h．　　下面是一个TAG区的例子：　　0x30000100 00000005 54410001 00000000 00000000 　　0x30000110 00000000 0000000F 54410009 746F6F72 　　0x30000120 65642F3D 61722F76 7220306D 6F632077 　　0x30000130 6C6F736E 74743D65 2C305379 30303639 　　0x30000140 696E6920 6C2F3D74 78756E69 EA006372 　　0x30000150 00000004 54420005 30300040 00200000 　　0x30000160 00000000 00000000 　　我们可以看到一共有三个TAG：　　第一个TAG的长度是5个字，类型是ATAG_CORE（54410001），有三个元素，均为全零．TAG区必须以这个TAG开头．　　第二个TAG的长度是F个字，类型是ATAG_CMDLINE（54410009），这是一个字符串，是向内核传递的kernel command line　　第三个TAG的长度是4个字，类型是ATAG_INITRD2（54410005），有两个元素，第一个是start:30300040（30300000+40），第二个是size:200000（2M）　　如果说还有第四个TAG，那就是末尾的两个全零，这是TAG结束的标志．　　这些TAG是在./lib_arm/arm_linux.c中的do_bootm_linux函数中建立起来的．具体建立哪些TAG，由相应的控制宏决定．具体可以参考相应代码．例子中第一个TAG是起始TAG，如果环境变量中有bootargs，则建立第二个TAG，如果bootm有两个参数（引导文件系统），则会读取文件系统头部的必要信息，建立第三个TAG．　　内核启动后，将根据R2寄存器的值找到这些TAG，并根据TAG类型，调用相应的处理函数进行处理，从而获取内核运行的必要信息． 本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/lanmanck/archive/2009/06/24/4294685.aspx](http://blog.csdn.net/lanmanck/archive/2009/06/24/4294685.aspx)**
