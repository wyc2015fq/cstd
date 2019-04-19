# 内核早期内存分配器：memblock - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [伯乐在线读者](http://www.jobbole.com/members/jobbole) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
> 
本文来自 **程雪涛** 的自荐投稿
Linux内核使用伙伴系统管理内存，那么在伙伴系统工作前，如何管理内存？答案是memblock。
memblock在系统启动阶段进行简单的内存管理，记录物理内存的使用情况。
在进一步介绍memblock之前，有必要先了解下系统内存的使用情况：
- 首先，内存中的某些部分是永久的分配给内核的，比如内核代码段和数据段，ramdisk和fdt占用的空间等，它们是系统内存的一部分，但是不能被侵占，也不参与内存分配，称之为静态内存；
- 其次，GPU，Camera等都需要预留大量连续内存，这部分内存平时不用，但是系统必须提前预留好，称之为预留内存；
- 最后，内存的其余部分称之为动态内存，是需要内核管理的宝贵资源。
memblock把物理内存划分为若干内存区，按使用类型分别放在memory和reserved两个集合（数组）中，memory即动态内存的集合，reserved集合包括静态内存和预留内存。
## 1. memblock关键数据结构
memblock数据结构定义如下：


```
struct memblock {
     bool bottom_up; 
     phys_addr_t current_limit;
     struct memblock_type memory;
     struct memblock_type reserved;
};
struct memblock_type {
     unsigned long cnt;           /* number of regions */
     unsigned long max;          /* size of the allocated array */
     phys_addr_t total_size;      /* size of all regions */
     struct memblock_region *regions;
};
struct memblock_region {
     phys_addr_t base;
     phys_addr_t size;
     unsigned long flags;
};
```
memblock相关数据结构十分的简单，内核还为memblock定义了一个全局变量，并为其赋初值，如下：


```
struct memblock memblock __initdata_memblock = {
     .memory.regions     = memblock_memory_init_regions,
     .memory.cnt           = 1,     /* empty dummy entry */
     .memory.max          = INIT_MEMBLOCK_REGIONS,
     .reserved.regions    = memblock_reserved_init_regions,
     .reserved.cnt          = 1,     /* empty dummy entry */
     .reserved.max         = INIT_MEMBLOCK_REGIONS,
     .bottom_up            = false,
     .current_limit          = MEMBLOCK_ALLOC_ANYWHERE,
};
```
memory类型的内存集合指向memblock_memory_init_regions数组，最多可以记录128个内存区。
reserved类型的内存集合指向memblock_reserved_init_regions数组，最多可以记录128个内存区。
注：内核代码经常用到类似”__initdata_memblock”的宏定义，通常用来指定变量或函数所在的section，该宏的定义如下：


```
#define __meminitdata    __attribute__ ((__section__(".meminit.data")))
```
## 2. memblock基本操作
### 1) 添加内存区


```
int memblock_add(phys_addr_t base, phys_addr_t size);
int memblock_reserve(phys_addr_t base, phys_addr_t size);
```
分别为memory和reserved集合添加内存区，如果新加入的内存区与原有内存区重叠，则合并到原有内存区，否则插入新内存区。
实际工作由memblock_add_range()完成，type参数指定内存集合类型。
需要注意的是该函数内部会执行两次：
第一次计算需要插入几个内存区，如果超过允许的最大内存区个数，则double内存区数组；
第二次执行内存区的实际插入与合并操作。


```
int memblock_add_range(struct memblock_type *type,
                    phys_addr_t base, phys_addr_t size,
                    int nid, unsigned long flags)
{
     bool insert = false;
     ... ...
     /* 特例: 如果内存集合为空，则不需要执行插入或合并操作，直接插入新的内存区就可以了 */
     if (type->regions[0].size == 0) {
          WARN_ON(type->cnt != 1 || type->total_size);
          type->regions[0].base = base;
          type->regions[0].size = size;
          type->regions[0].flags = flags;
          memblock_set_region_node(&type->regions[0], nid);
          type->total_size = size;
          return 0;
     }
repeat:
     base = obase;
     nr_new = 0;
     for (i = 0; i < type->cnt; i++) {
          struct memblock_region *rgn = &type->regions[i];
          phys_addr_t rbase = rgn->base;
          phys_addr_t rend = rbase + rgn->size;
          if (rbase >= end)
               break;
          if (rend <= base)
               continue;
          /* 如果内存区重叠，则先插入低地址部分[base~rbase]，然后重新计算base地址 */
          if (rbase > base) {
               nr_new++;
               if (insert)
                    memblock_insert_region(type, i++, base,
                                     rbase - base, nid,
                                     flags);
          }
          /* area below @rend is dealt with, forget about it */
          base = min(rend, end);
     }
     /* 插入内存区[base~end] */
     if (base < end) {
          nr_new++;
          if (insert)
               memblock_insert_region(type, i, base, end - base,
                                nid, flags);
     }
     /* 第一次执行检查是否需要调整内存区数组大小，第二次执行合并操作 */
     if (!insert) {
          while (type->cnt + nr_new > type->max)
               if (memblock_double_array(type, obase, size) < 0)
                    return -ENOMEM;
          insert = true;
          goto repeat;
     } else {
          memblock_merge_regions(type);
          return 0;
     }
}
```
### 2) 移除内存区


```
int memblock_remove(phys_addr_t base, phys_addr_t size);
```
从memory集合移除给定物理地址所指的内存区，如果是内存区域的一部分，则涉及到调整region大小，或者将一个region拆分成两个region。
系统将不会为移除的内存区建立内存映射，这部分内存区后续应该由DMA或CMA管理。
### 3) 分配内存


```
phys_addr_t memblock_alloc(phys_addr_t size, phys_addr_t align);
phys_addr_t memblock_alloc_range(phys_addr_t size, phys_addr_t align,  phys_addr_t start, phys_addr_t end);
```
使用该函数向kernel申请一块可用的物理内存，memblock使用自顶向下（取决于bottom_up的值）的方式查找空闲内存，实际操作是在memory region中查找合适的内存，并加入到reserved region中以标记这块内存已经被使用。
### 4) 释放内存


```
int memblock_free(phys_addr_t base, phys_addr_t size);
```
使用该函数来释放由memblock_alloc申请到的物理内存。
## 3. 探测系统可用内存
内核是如何知晓物理内存的拓扑结构呢？相信很多人都有过类似的疑问。
通过DDR的模式寄存器(MR8)，可以很容易获得内存密度，进而推断出内存容量，这部分工作通常由bootloader完成，然后使用fdt或者atag等方式传递给内核。
以fdt为例，内核解析memory节点，取得物理内存的拓扑结构（起始地址及大小），并添加到memblock中，代码如下：


```
setup_arch()->setup_machine_fdt()->early_init_dt_scan()->early_init_dt_scan_memory()
{
     ... ...
     reg = of_get_flat_dt_prop(node, "reg", &l);
     ... ...
     while ((endp - reg) >= (dt_root_addr_cells + dt_root_size_cells)) {
          u64 base, size;
          base = dt_mem_next_cell(dt_root_addr_cells, ®);
          size = dt_mem_next_cell(dt_root_size_cells, ®);
          ... ...
          early_init_dt_add_memory_arch(base, size);
     }
}
```
该函数扫描memory节点，并解析reg属性，注意此时DeviceTree还没有执行unflattern操作，需要使用”fdt”类型接口解析dtb。
以4G DDR为例，输出的调试信息如下：


```
[    0.000000] memory scan node memory, reg size 32, data: 0 80 0 80, 1000000 0 0 557e
[    0.000000]  - 80000000 ,  80000000
[    0.000000]  - 100000000 ,  7e550000
```
reg属性由addr和size组成，分别占用2个cell(u32类型数据)，上面的reg data可以看成：“0 00000080 0 00000080, 01000000 0 0 00557e”。
dtb使用big endian方式存储数据，需要转换成cpu字节序。
解析出来的内存包含两个Rank，起始地址分别是0x80000000和0x100000000，这是系统的可用内存，用来初始化memory region。


```
void early_init_dt_add_memory_arch(base, size)
{
     const u64 phys_offset = __pa(PAGE_OFFSET);
     ... ...
     if (base < phys_offset) {
          pr_warning("Ignoring memory range 0x%llx - 0x%llx\n",
                  base, phys_offset);
          size -= phys_offset - base;
          base = phys_offset;
     }
     memblock_add(base, size);
}
```
从fdt解析的内存信息是否可信呢？内核有自己的判断，在启动阶段，内核会根据自身的运行地址计算内存基地址，即PHYS_OFFSET。
如果base地址小于phys_offset，则内核使用可信的phys_offset做为主存的基地址。
这里要注意区分PHYS_OFFSET, PAGE_OFFSET：
PAGE_OFFSET是内核虚拟地址空间的起始地址，PHYS_OFFSET是RAM在物理空间的起始地址，内核空间的地址映射通常具有固定的偏移量，即：


```
#define __virt_to_phys(x)     (((phys_addr_t)(x) - PAGE_OFFSET + PHYS_OFFSET))
```
## 4. 记录系统预留内存
这里说的系统预留内存，包括静态内存（内核Image，ramdisk，fdt等占用空间），以及系统为Camera，Display等子系统预留的大量连续内存。
另外，高通平台通常包含多核，还需要为Modem，TZ/TA等预留运行空间，这部分空间类似静态内存，都是永久分配给其它核心使用，根据节点属性，通常由DMA管理。
arm64_memblock_init()函数初始化系统预留内存，代码如下：


```
setup_arch()->arm64_memblock_init()
{
     // 1) 保留内核映像所在的内存空间（0x80080000~?）
     memblock_reserve(__pa(_text), _end - _text);
     // 2) 保留Ramdisk所在的内存空间（0x82200000~?）
     memblock_reserve(__virt_to_phys(initrd_start), initrd_end - initrd_start);
     // 3) 保留fdt所在内存空间（0x82000000~?），扫描reserved-memory节点，解析其子节点包含的预留内存信息；
     early_init_fdt_scan_reserved_mem();
     /* 4GB maximum for 32-bit only capable devices */
     if (IS_ENABLED(CONFIG_ZONE_DMA))
          dma_phys_limit = max_zone_dma_phys();
     // 4) 初始化Global CMA
     dma_contiguous_reserve(dma_phys_limit);
     memblock_allow_resize();
     memblock_dump_all();
}
void early_init_fdt_scan_reserved_mem(void)
{
     ... ...
     /* 1) 保留启动参数所在的内存空间，这里通常指的就是fdt占用的内存空间 */
     early_init_dt_reserve_memory_arch(__pa(initial_boot_params),
                           fdt_totalsize(initial_boot_params),
                           0);
     ... ...
     /* 2) 扫描reserved-memory节点 */
     of_scan_flat_dt(__fdt_scan_reserved_mem, NULL);
     /* 3) 为预留内存执行初始化操作 */
     fdt_init_reserved_mem();
}
```
“no-map”属性决定向reserved region添加内存区，还是从memory region移除内存区，二者差别在于内核不会给”no-map”属性的内存区建立内存映射，即该内存区不在动态内存管理范围。
预留内存还会被添加到reserved_mem数组，为后续的初始化做准备，”reg”属性指定内存区的起始地址和大小，如果没有”reg”属性，还需要为内存区分配空间。


```
__fdt_scan_reserved_mem()->__reserved_mem_reserve_reg()
{
     ... ...
     prop = of_get_flat_dt_prop(node, "reg", &len);
     if (!prop)
          return -ENOENT;
     ... ...
     nomap = of_get_flat_dt_prop(node, "no-map", NULL) != NULL;
     while (len >= t_len) {
          base = dt_mem_next_cell(dt_root_addr_cells, &prop);
          size = dt_mem_next_cell(dt_root_size_cells, &prop);
          if (size &&
              early_init_dt_reserve_memory_arch(base, size, nomap) == 0)
               pr_info("Reserved memory: reserved region for node '%s': base %pa, size %ld MiB\n",
                    uname, &base, (unsigned long)size / SZ_1M);
          else
               pr_info("Reserved memory: failed to reserve memory for node '%s': base %pa, size %ld MiB\n",
                    uname, &base, (unsigned long)size / SZ_1M);
          len -= t_len;
          if (first) {
               fdt_reserved_mem_save_node(node, uname, base, size);
               first = 0;
          }
     }
     return 0;
}
```
至此，memblock的初始化工作已经基本完成了，主要是记录系统内存的使用情况：
memory region记录系统了所有可用的动态内存；
reserved region记录了系统预留内存，这部分内存通常由CMA管理，也属于动态内存范畴；
reserved_mem数组则记录系统所有预留内存，包括”no-map”属性的内存区，为后续进一步初始化工作做准备。
## 5. 初始化预留内存区
内存向来是系统的宝贵资源，预留内存如果仅做为子系统的专用内存，就有点浪费了。
Linux内核引入CMA（Contiguous Memory Allocator，连续内存分配器）。
其工作原理是：为驱动预留一段内存，当驱动不用时，Memory Allocator（Buddy System）可以分配给用户进程使用；而当驱动需要使用时，就将进程占用的内存通过回收或者迁移的方式腾出来，供驱动使用。
但是并不是所有的预留内存都由CMA管理，像Modem，TA等永久分配给其它核心使用的内存空间，内核并不为这部分空间建立内存映射，而是交由DMA管理。
通过上面的分析，我们看到所有预留内存信息都记录在reserved_mem数组，下面先看看该结构体的定义：


```
static struct reserved_mem reserved_mem[MAX_RESERVED_REGIONS];
struct reserved_mem {
     const char             *name;
     unsigned long        fdt_node;
     unsigned long        phandle;
     const struct reserved_mem_ops     *ops;
     phys_addr_t            base;
     phys_addr_t            size;
     void                        *priv;
     int                          fixup;
};
```
reserved-memory子节点包含预留内存属性，典型定义如下：


```
reserved-memory {
    #address-cells = <2>;
    #size-cells = <2>;
    ranges;
    removed_regions: removed_regions@85800000 {
        compatible = "removed-dma-pool";
        no-map;
        reg = <0 0x85800000 0 0x3000000>;
    };
    adsp_mem: adsp_region {
        compatible = "shared-dma-pool";
        alloc-ranges = <0 0x00000000 0 0xffffffff>;
        reusable;
        alignment = <0 0x100000>;
        size = <0 0x400000>;
    };
    ... ...
};
```
“reg”和”no-map”属性前面介绍过，详细可以参考reserved-memory.txt，”compatible”属性使用标准定义，内核注册两种不同的处理方法：


```
RESERVEDMEM_OF_DECLARE(dma, "removed-dma-pool", removed_dma_setup);
RESERVEDMEM_OF_DECLARE(cma, "shared-dma-pool", rmem_cma_setup);
```
“removed-dma-pool”表示该内存区位于DMA管理区，内核不可见（没有页表）。
“shared-dma-pool”表示该内存区位于CMA管理区，平时是可用的，只有需要时才分配给驱动使用。
如果没有”reg”属性，即没有指定预留内存的起始地址，则需要由系统分配预留内存，然后初始化reserved_mem的ops成员：


```
arm64_memblock_init()->early_init_fdt_scan_reserved_mem()->fdt_init_reserved_mem()
{
     ... ...
     for (i = 0; i < reserved_mem_count; i++) {
          struct reserved_mem *rmem = &reserved_mem[i];
          ... ...
          if (rmem->size == 0)
               err = __reserved_mem_alloc_size(node, rmem->name,
                              &rmem->base, &rmem->size);
          if (err == 0)
               __reserved_mem_init_node(rmem);
     }
}
```
以”shared-dma-pool”为例，它的初始化函数如下：


```
static int rmem_cma_setup(struct reserved_mem *rmem)
{
    struct cma *cma;
    ... ...
    err = cma_init_reserved_mem(rmem->base, rmem->size, 0, &cma);
    if (err) {
        pr_err("Reserved memory: unable to setup CMA region\n");
        return err;
    }
    ... ....
    rmem->ops = &rmem_cma_ops;
    rmem->priv = cma;
    ... ...
    return 0;
}
```
此处为”shared-dma-pool”类型的内存注册操作方法，cma_init_reserved_mem初始化cma_area(CMA管理区)。
reserved_mem的ops成员包括init和release两个操作方法：


```
struct reserved_mem_ops {
    int     (*device_init)(struct reserved_mem *rmem,  struct device *dev);
    void   (*device_release)(struct reserved_mem *rmem,   struct device *dev);
};
```
驱动注册预留内存区时调用device_init方法，为设备指定预留内存操作方法(DMA)或预留内存区域(CMA)，这些方法包括预留内存的申请，释放和mmap等。
## 6. 连续内存分配器（CMA）
CMA的初始化必须在buddy系统工作之前和memblock分配器初始化完成之后。
在ARM中，初始化CMA的接口是：


```
dma_contiguous_reserve(phys_addr_t limit)；
```
@limit: 指定CMA区域的上限，在64位系统上@limit的值通常是0x100000000。


```
arm64_memblock_init()->dma_contiguous_reserve(phys_addr_t limit)
{
    ... ...
    /* 可以在cmdline或者kernel config中配置global cma size */
    if (selected_size && !dma_contiguous_default_area) {
        pr_debug("%s: reserving %ld MiB for global area\n", __func__,
             (unsigned long)selected_size / SZ_1M);
        dma_contiguous_reserve_area(selected_size, selected_base,
                        selected_limit,
                        &dma_contiguous_default_area,
                        fixed);
    }
}
```
以命令行参数”cma=32M@0-0xfffffff”为例: size_cmdline = 32M, base_cmdline = 0x0, limit_cmdline = 0xffffffff 。
计算好CMA的size等值以后就进入cma_declare_contiguous中：


```
dma_contiguous_reserve_area()->cma_declare_contiguous()
{
    ... ...
    /* Reserve memory */
    if (fixed) {
        if (memblock_is_region_reserved(base, size) ||
            memblock_reserve(base, size) < 0) {
            ret = -EBUSY;
            goto err;
        }
    } else {
        phys_addr_t addr = 0;
        ... ...
        if (!addr) {
            addr = memblock_alloc_range(size, alignment, base,  limit);
            if (!addr) {
                ret = -ENOMEM;
                goto err;
            }
        }
        ... ...
        base = addr;
    }
    ret = cma_init_reserved_mem(base, size, order_per_bit, res_cma);
    ... ...
    return 0;
}
```
dma_contiguous_default_are是用户自定义CMA管理区，定义如下：


```
struct cma *dma_contiguous_default_area;
struct cma {
    unsigned long   base_pfn;
    unsigned long   count;
    unsigned long  *bitmap;
    unsigned int      order_per_bit;    /* Order of pages represented by one bit */
    struct mutex     lock;
};
```
下面进入cma_init_reserved_mem初始化用户自定义CMA管理区：


```
int __init cma_init_reserved_mem(phys_addr_t base, phys_addr_t size,
                 int order_per_bit, struct cma **res_cma)
{
    ... ...
    /* Sanity checks */
    if (cma_area_count == ARRAY_SIZE(cma_areas)) {
        pr_err("Not enough slots for CMA reserved regions!\n");
        return -ENOSPC;
    }
    ... ...
    /*
     * Each reserved area must be initialised later, when more kernel
     * subsystems (like slab allocator) are available.
     */
    cma = &cma_areas[cma_area_count];
    cma->base_pfn = PFN_DOWN(base);
    cma->count = size >> PAGE_SHIFT;
    cma->order_per_bit = order_per_bit;
    *res_cma = cma;
    cma_area_count++;
    return 0;
}
```
以上只是将CMA区域预留下来，并记录到相关数组，进一步初始化和使用需要等slab等子系统初始化完成后了。
CMA并不直接开放给驱动开发人员，在注册设备时可以使用”memory-region”属性指定要操作的内存区域，需要分配DMA内存时，调用DMA相关函数就可以了。
例如dma_alloc_coherent()，最终DMA相关的分配函数会到达CMA的分配函数dma_alloc_from_contiguous()。
## 7. 进阶阅读
- [memblock和bootmem的区别](http://blog.chinaunix.net/uid-20378444-id-3189700.html)
- [CMA（连续内存分配器）](http://baike.baidu.com/link?url=THnntnMMugTgnyRJEYK3MxmIQmczeY_hdIhNzittyMaE_TVP96rSfmHnrLqBORdQ_QgPwT9-TVW3WLhEoWX5F_)
- [Contiguous Memory Allocator (CMA) 源码分析](http://blog.tek-life.com/contiguous-memory-allocator-cma-%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/)
- [Linux内核最新连续内存分配器(CMA) — 避免预留大块内存](http://blog.csdn.net/21cnbao/article/details/7309757)
- [内存管理笔记（CMA）](http://blog.chinaunix.net/uid-26859697-id-4999985.html)
