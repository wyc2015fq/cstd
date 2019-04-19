# Linux 内核高-低端内存设置代码跟踪（ARM构架） - 文章 - 伯乐在线
原文出处： [tekkamanninja](http://blog.chinaunix.net/uid-20543672-id-2920689.html)
对于ARM中内核如何在启动的时候设置高低端内存的分界线（也是逻辑地址与虚拟地址分界线（虚拟地址）减去那个固定的偏移），这里我稍微引导下（内核分析使用Linux-3.0）：
首先定位设置内核虚拟地址起始位置（也就是内核逻辑地址末端+1的地址）的文件：init.c (arch\arm\mm)，在这个文件中的void __init bootmem_init(void)函数如下


```
void __init bootmem_init(void)
{
    unsigned long min, max_low, max_high;
    max_low = max_high = 0;
    find_limits(&min, &max_low, &max_high);
    arm_bootmem_init(min, max_low);
    /*
     * Sparsemem tries to allocate bootmem in memory_present(),
     * so must be done after the fixed reservations
     */
    arm_memory_present();
    /*
     * sparse_init() needs the bootmem allocator up and running.
     */
    sparse_init();
    /*
     * Now free the memory - free_area_init_node needs
     * the sparse mem_map arrays initialized by sparse_init()
     * for memmap_init_zone(), otherwise all PFNs are invalid.
     */
    arm_bootmem_free(min, max_low, max_high);
    high_memory = __va(((phys_addr_t)max_low << PAGE_SHIFT) - 1) + 1;
    /*
     * This doesn't seem to be used by the Linux memory manager any
     * more, but is used by ll_rw_block. If we can get rid of it, we
     * also get rid of some of the stuff above as well.
     *
     * Note: max_low_pfn and max_pfn reflect the number of _pages_ in
     * the system, not the maximum PFN.
     */
    max_low_pfn = max_low - PHYS_PFN_OFFSET;
    max_pfn = max_high - PHYS_PFN_OFFSET;
}
```
这个high_memory = __va(((phys_addr_t)max_low << PAGE_SHIFT) – 1) + 1;语句就是关键。从这里可以知道max_low就是高端内存的起始地址（物理地址）。那么这个max_low是如何得到的？其实看上面的代码可以推测出，他其实是在find_limits(&min, &max_low, &max_high);中（在同一个文件中）被设置的：


```
static void __init find_limits(unsigned long *min, unsigned long *max_low,
    unsigned long *max_high)
{
    struct meminfo *mi = &meminfo;
    int i;
    *min = -1UL;
    *max_low = *max_high = 0;
    for_each_bank (i, mi) {
        struct membank *bank = &mi->bank[i];
        unsigned long start, end;
        start = bank_pfn_start(bank);
        end = bank_pfn_end(bank);
        if (*min > start)
            *min = start;
        if (*max_high < end)
            *max_high = end;
        if (bank->highmem)
            continue;
        if (*max_low < end)
            *max_low = end;
    }
}
```
这个函数的意思很明显：通过扫描struct meminfo *mi = &meminfo;（结构体meminfo的数组）中的所有信息，设置三个指针所指的变量：
> 
min :内存物理地址起始
max_low :低端内存区物理地址末端
max_high :高端内存区物理地址末端
从上面可以看出，max_low和max_high所保存的地址不同就是由于bank->highmem造成的，它是内存bank被设为高端内存的依据：
> 
“如果这个内存bank是高端内存（bank->highmem ！= 0），跳过max_low = end;语句，max_low和max_high将不同（结果实际上是max_high > max_low）；
否则假设没有一个内存bank是高端内存（所有bank->highmem == 0）max_low和max_high必然一致（高端内存大小为0）”
当然要实现这个函数的功能，必须保证meminfo所指数组中的所有bank是按照地址数据从小到大排序好的哦～～。但是这个大家不用担心，后面会看到的：）
经过上面的跟踪，焦点集中到了全局变量（同一个文件中）：
> 
/*
* This keeps memory configuration data used by a couple memory
* initialization functions, as well as show_mem() for the skipping
* of holes in the memory map. It is populated by arm_add_memory().
*/
struct meminfo meminfo;
这个结构体的定义（setup.h (arch\arm\include\asm)）：


```
/*
 * Memory map description
 */
#define NR_BANKS 8  /*现在ARM最大只支持到8个bank哦～*/
struct membank {
    phys_addr_t start;
    unsigned long size;
    unsigned int highmem;  /*我们关心的变量*/
};
struct meminfo {
    int nr_banks;
    struct membank bank[NR_BANKS];  /*我们关心的数组*/
};
extern struct meminfo meminfo;
#define for_each_bank(iter,mi)                \
    for (iter = 0; iter < (mi)->nr_banks; iter++)
  #define bank_pfn_start(bank)	__phys_to_pfn((bank)->start)
  #define bank_pfn_end(bank)	__phys_to_pfn((bank)->start + (bank)->size)
  #define bank_pfn_size(bank)	((bank)->size >> PAGE_SHIFT)
  #define bank_phys_start(bank)	(bank)->start
  #define bank_phys_end(bank)	((bank)->start + (bank)->size)
  #define bank_phys_size(bank)	(bank)->size
```
只要找到初始化这个全局变量并完成排序的地方，就可以知道高端内存是如何配置的了！！OK，明确目标，go on～～～
通过查找代码，我们可以在setup.c (arch\arm\kernel)这个文件中找到相关的代码。在系统启动早期会运行的函数（具体的顺序你可以自行分析下ARM内核的启动流程，以后我也会写下）中有这样一个函数：


```
void __init setup_arch(char **cmdline_p)
{
    struct machine_desc *mdesc;
    unwind_init();
    setup_processor();
    mdesc = setup_machine_fdt(__atags_pointer);
    if (!mdesc)
        mdesc = setup_machine_tags(machine_arch_type);
    machine_desc = mdesc;
    machine_name = mdesc->name;
    if (mdesc->soft_reboot)
        reboot_setup("s");
    init_mm.start_code = (unsigned long) _text;
    init_mm.end_code = (unsigned long) _etext;
    init_mm.end_data = (unsigned long) _edata;
    init_mm.brk     = (unsigned long) _end;
    /* 填充cmd_line以备后用，维护boot_command_line */
    strlcpy(cmd_line, boot_command_line, COMMAND_LINE_SIZE);    /* 拷贝boot_command_line中的数据到cmd_line */
    *cmdline_p = cmd_line;
    parse_early_param();        /* 分析boot_command_line（内核启动参数字符串）中的数据，
                                 * 其中就分析了mem=size@start参数初始化了struct meminfo meminfo;
                                 * 同时如果有vmalloc=size参数也会初始化 vmalloc_min */  
    sanity_check_meminfo();    /* 在此处设置struct meminfo meminfo中每个bank中的highmem变量，
                                * 通过vmalloc_min确定每个bank中的内存是否属于高端内存 */
    arm_memblock_init(&meminfo, mdesc);   /* 在此处排序按地址数据从小到大排序 */
    paging_init(mdesc);
    request_standard_resources(mdesc);
    unflatten_device_tree();
#ifdef CONFIG_SMP
    if (is_smp())
        smp_init_cpus();
#endif
    reserve_crashkernel();
    cpu_init();
    tcm_init();
#ifdef CONFIG_MULTI_IRQ_HANDLER
    handle_arch_irq = mdesc->handle_irq;
#endif
#ifdef CONFIG_VT
#if defined(CONFIG_VGA_CONSOLE)
    conswitchp = &vga_con;
#elif defined(CONFIG_DUMMY_CONSOLE)
    conswitchp = &dummy_con;
#endif
#endif
    early_trap_init();
    if (mdesc->init_early)
        mdesc->init_early();
}
```
在上面的注释中，我已经表明了重点和解析，下面我细化下：
（1）获取参数部分
通过parse_early_param();函数可以解析内核启动参数中的许多字符串，但是对于我们这次分析内存的话主要是分析以下两个参数：
mem=size@start参数，她为初始化struct meminfo meminfo;（我们一直关注的内存信息哦～）提供信息。具体的获取信息的函数（同样位于setup.c (arch\arm\kernel)）：


```
int __init arm_add_memory(phys_addr_t start, unsigned long size)
{
    struct membank *bank = &meminfo.bank[meminfo.nr_banks];
    if (meminfo.nr_banks >= NR_BANKS) {
        printk(KERN_CRIT "NR_BANKS too low, "
            "ignoring memory at 0x%08llx\n", (long long)start);
        return -EINVAL;
    }
    /*
     * Ensure that start/size are aligned to a page boundary.
     * Size is appropriately rounded down, start is rounded up.
     */
    size -= start & ~PAGE_MASK;
    bank->start = PAGE_ALIGN(start);
    bank->size = size & PAGE_MASK;
    /*
     * Check whether this memory region has non-zero size or
     * invalid node number.
     */
    if (bank->size == 0)
        return -EINVAL;
    meminfo.nr_banks++;
    return 0;
}
/*
 * Pick out the memory size. We look for mem=size@start,
 * where start and size are "size[KkMm]"
 */
static int __init early_mem(char *p)
{
    static int usermem __initdata = 0;
    unsigned long size;
    phys_addr_t start;
    char *endp;
    /*
     * If the user specifies memory size, we
     * blow away any automatically generated
     * size.
     */
    if (usermem == 0) {
        usermem = 1;
        meminfo.nr_banks = 0;
    }
    start = PHYS_OFFSET;
    size = memparse(p, &endp);
    if (*endp == '@')
        start = memparse(endp + 1, NULL);
    arm_add_memory(start, size);
    return 0;
}
early_param("mem", early_mem);
```
vmalloc=size参数，她为初始化vmalloc_min（需要保留的内核虚拟地址空间大小，也就是这个内核虚拟地址空间中除去逻辑地址空间和必要的防止越界的保护空洞后最少要预留的地址空间）提供信息。具体的实现函数（位于mmu.c (arch\arm\mm)）：


```
static void * __initdata vmalloc_min = (void *)(VMALLOC_END - SZ_128M); /* 默认值，使得内核虚拟地址保留出128MB的空间以备后用 */
  /* 这里顺便提一下 VMALLOC_END  ，他和芯片构架相关，
   *不一定是0xffffffff，比如2410是0xF6000000UL，pxa是0xe8000000UL*/
/*
 * vmalloc=size forces the vmalloc area to be exactly 'size'
 * bytes. This can be used to increase (or decrease) the vmalloc
 * area - the default is 128m.
 */
static int __init early_vmalloc(char *arg)
{
    unsigned long vmalloc_reserve = memparse(arg, NULL);
    if (vmalloc_reserve < SZ_16M) {
        vmalloc_reserve = SZ_16M;
        printk(KERN_WARNING
            "vmalloc area too small, limiting to %luMB\n",
            vmalloc_reserve >> 20);
    } /* 数据检查，最小值=16MB */
    if (vmalloc_reserve > VMALLOC_END - (PAGE_OFFSET + SZ_32M)) {
        vmalloc_reserve = VMALLOC_END - (PAGE_OFFSET + SZ_32M);
        printk(KERN_WARNING
            "vmalloc area is too big, limiting to %luMB\n",
            vmalloc_reserve >> 20);
    }  /* 数据检查，最大值为从这个内核虚拟地址空间减去32MB，
        * 也就是只有32MB的逻辑地址空间，其他地址全部保留备用 */
    vmalloc_min = (void *)(VMALLOC_END - vmalloc_reserve);   
      /* vmalloc_min其实就是以后可用于映射分配的内核虚拟地址空间（不包含逻辑地址）最小值 */
    return 0;
}
early_param("vmalloc", early_vmalloc);
```
（2）在获得了必要的信息（初始化好struct meminfo meminfo和vmalloc_min）后，内核通过sanity_check_meminfo函数自动去通过vmalloc_min信息来初始化每个meminfo.bank[？]中的highmem成员。此过程中如果有必要，将可能会改变meminfo中的bank数组。处理函数位于mmu.c (arch\arm\mm)：


```
static phys_addr_t lowmem_limit __initdata = 0;
void __init sanity_check_meminfo(void)
{
    int i, j, highmem = 0;
    for (i = 0, j = 0; i < meminfo.nr_banks; i++) {
        struct membank *bank = &meminfo.bank[j];
        *bank = meminfo.bank[i];
#ifdef CONFIG_HIGHMEM
        if (__va(bank->start) >= vmalloc_min ||
         __va(bank->start) < (void *)PAGE_OFFSET)
            highmem = 1;
        bank->highmem = highmem;
        /*
         * Split those memory banks which are partially overlapping
         * the vmalloc area greatly simplifying things later.
         */
        if (__va(bank->start) < vmalloc_min &&
         bank->size > vmalloc_min - __va(bank->start)) {
            if (meminfo.nr_banks >= NR_BANKS) {
                printk(KERN_CRIT "NR_BANKS too low, "
                         "ignoring high memory\n");
            } else {
                memmove(bank + 1, bank,
                    (meminfo.nr_banks - i) * sizeof(*bank));
                meminfo.nr_banks++;
                i++;
                bank[1].size -= vmalloc_min - __va(bank->start);
                bank[1].start = __pa(vmalloc_min - 1) + 1;
                bank[1].highmem = highmem = 1;
                j++;
            }
            bank->size = vmalloc_min - __va(bank->start);
        }
#else
        bank->highmem = highmem;
        /*
         * Check whether this memory bank would entirely overlap
         * the vmalloc area.
         */
        if (__va(bank->start) >= vmalloc_min ||
         __va(bank->start) < (void *)PAGE_OFFSET) {
            printk(KERN_NOTICE "Ignoring RAM at %.8llx-%.8llx "
             "(vmalloc region overlap).\n",
             (unsigned long long)bank->start,
             (unsigned long long)bank->start + bank->size - 1);
            continue;
        }
        /*
         * Check whether this memory bank would partially overlap
         * the vmalloc area.
         */
        if (__va(bank->start + bank->size) > vmalloc_min ||
         __va(bank->start + bank->size) < __va(bank->start)) {
            unsigned long newsize = vmalloc_min - __va(bank->start);
            printk(KERN_NOTICE "Truncating RAM at %.8llx-%.8llx "
             "to -%.8llx (vmalloc region overlap).\n",
             (unsigned long long)bank->start,
             (unsigned long long)bank->start + bank->size - 1,
             (unsigned long long)bank->start + newsize - 1);
            bank->size = newsize;
        }
#endif
        if (!bank->highmem && bank->start + bank->size > lowmem_limit)
            lowmem_limit = bank->start + bank->size;
        j++;
    }
#ifdef CONFIG_HIGHMEM
    if (highmem) {
        const char *reason = NULL;
        if (cache_is_vipt_aliasing()) {
            /*
             * Interactions between kmap and other mappings
             * make highmem support with aliasing VIPT caches
             * rather difficult.
             */
            reason = "with VIPT aliasing cache";
        }
        if (reason) {
            printk(KERN_CRIT "HIGHMEM is not supported %s, ignoring high memory\n",
                reason);
            while (j > 0 && meminfo.bank[j - 1].highmem)
                j--;
        }
    }
#endif
    meminfo.nr_banks = j;
    memblock_set_current_limit(lowmem_limit);
}
```
（3）最后必须做的就是排序了，完成了这个工作就可以完全被我们上面提到的find_limits函数使用了，而这个工作就放在了接下来的arm_memblock_init(&meminfo, mdesc);中的一开头：


```
static int __init meminfo_cmp(const void *_a, const void *_b)
{
    const struct membank *a = _a, *b = _b;
    long cmp = bank_pfn_start(a) - bank_pfn_start(b);
    return cmp < 0 ? -1 : cmp > 0 ? 1 : 0;
}
void __init arm_memblock_init(struct meminfo *mi, struct machine_desc *mdesc)
{
    int i;
    sort(&meminfo.bank, meminfo.nr_banks, sizeof(meminfo.bank[0]), meminfo_cmp, NULL);   /* 极好用的排序函数 */
    memblock_init();
    for (i = 0; i < mi->nr_banks; i++)
        memblock_add(mi->bank[i].start, mi->bank[i].size);
    /* Register the kernel text, kernel data and initrd with memblock. */
#ifdef CONFIG_XIP_KERNEL
    memblock_reserve(__pa(_sdata), _end - _sdata);
#else
    memblock_reserve(__pa(_stext), _end - _stext);
#endif
#ifdef CONFIG_BLK_DEV_INITRD
    if (phys_initrd_size &&
     !memblock_is_region_memory(phys_initrd_start, phys_initrd_size)) {
        pr_err("INITRD: 0x%08lx+0x%08lx is not a memory region - disabling initrd\n",
         phys_initrd_start, phys_initrd_size);
        phys_initrd_start = phys_initrd_size = 0;
    }
    if (phys_initrd_size &&
     memblock_is_region_reserved(phys_initrd_start, phys_initrd_size)) {
        pr_err("INITRD: 0x%08lx+0x%08lx overlaps in-use memory region - disabling initrd\n",
         phys_initrd_start, phys_initrd_size);
        phys_initrd_start = phys_initrd_size = 0;
    }
    if (phys_initrd_size) {
        memblock_reserve(phys_initrd_start, phys_initrd_size);
        /* Now convert initrd to virtual addresses */
        initrd_start = __phys_to_virt(phys_initrd_start);
        initrd_end = initrd_start + phys_initrd_size;
    }
#endif
    arm_mm_memblock_reserve();
    arm_dt_memblock_reserve();
    /* reserve any platform specific memblock areas */
    if (mdesc->reserve)
        mdesc->reserve();
    memblock_analyze();
    memblock_dump_all();
}
```
通过上面的分析，整个高低端内存是如何确定的基本就清晰了，这里总结一下：
ARM构架中，高-低段内存是内核通过内核启动参数（ mem=size@start和vmalloc=size）来自动配置的，如果没有特殊去配置他，那么在普通的ARM系统中是不会有高端内存存在的。除非你系统的RAM很大或vmalloc配置得很大，就很可能出现高端内存。
以上是我对高-低端内存学习时跟踪代码的备忘，如果大家在其中发现什么不对的地方，欢迎拍砖、纠正～～谢谢～
