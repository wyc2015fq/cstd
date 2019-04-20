# Linux内存管理(下) - h13 - 博客园
### 物理内存管理（页管理）
Linux内核管理物理内存是通过分页机制实现的，它将整个内存划分成无数4k(在i386体系结构中)大小页，从而分配和回收内存的基本单位便是内存页了。利用分页管理有助于灵活分配内存地址，因为分配时不必要求必须有大块的连续内存[[1]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftn1)，系统可以东一页、西一页的凑出所需要的内存供进程使用。虽然如此，但是实际上系统使用内存还是倾向于分配连续的内存块，因为分配连续内存时，页表不需要更改，因此能降低TLB的刷新率（频繁刷新会很大增加访问速度）。
鉴于上述需求，内核分配物理页为了尽量减少不连续情况，采用了“伙伴”关系来管理空闲页框。伙伴关系分配算法大家不应陌生——几乎所有操作系统书都会提到,我们不去详细说它了，如果不明白可以参看有关资料。这里只需要大家明白Linux中空闲页面的组织和管理利用了伙伴关系，因此空闲页面分配时也需要遵循伙伴关系，最小单位只能是2的幂倍页面大小。内核中分配空闲页框的基本函数是get_free_page/get_free_pages，它们或是分配单页或是分配指定的页框（2、4、8…512页）。
注意：get_free_page是在内核中分配内存，不同于malloc在用户空间中分配，malloc利用堆动态分配，实际上是调用brk()系统调用，该调用的作用是扩大或缩小进程堆空间（它会修改进程的brk域）。如果现有的内存区域不够容纳堆空间，则会以页面大小的倍数位单位，扩张或收缩对应的内存区域，但brk值并非以页面大小为倍数修改，而是按实际请求修改。因此Malloc在用户空间分配内存可以以字节为单位分配,但内核在内部仍然会是以页为单位分配的。
另外需要提及的是，物理页在系统中由页框结构struct paga描述，系统中所有的页框存储在数组mem_map[]中，可以通过该数组找到系统中的每一页（空闲或非空闲）。而其中的空闲页框则可由上述提到的以伙伴关系组织的空闲页链表（free_area[MAX_ORDER]）索引。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/kanghua/systemcall.bmp)
内核内存使用
**Slab**
所谓尺有所长，寸有所短。以页为最小单位分配内存对于内核管理系统物理内存来说的确比较方便，但内核自身最常使用的内存却往往是很小（远远小于一页）的内存块——比如存放文件描述符、进程描述符、虚拟内存区域描述符等行为所需的内存都不足一页。这些用来存放描述符的内存相比页面而言，就好比是面包屑与面包。一个整页中可以聚集多个这种这些小块内存；而且这些小块内存块也和面包屑一样频繁地生成/销毁。
为了满足内核对这种小内存块的需要，Linux系统采用了一种被称为slab分配器的技术。Slab分配器的实现相当复杂，但原理不难，其核心思想就是“存储池[[2]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftn2)”的运用。内存片段（小块内存）被看作对象，当被使用完后，并不直接释放而是被缓存到“存储池”里，留做下次使用，这无疑避免了频繁创建与销毁对象所带来的额外负载。
Slab技术不但避免了内存内部分片（下文将解释）带来的不便（引入Slab分配器的主要目的是为了减少对伙伴系统分配算法的调用次数——频繁分配和回收必然会导致内存碎片——难以找到大块连续的可用内存），而且可以很好利用硬件缓存提高访问速度。
Slab并非是脱离伙伴关系而独立存在的一种内存分配方式，slab仍然是建立在页面基础之上，换句话说，Slab将页面（来自于伙伴关系管理的空闲页框链）撕碎成众多小内存块以供分配，slab中的对象分配和销毁使用kmem_cache_alloc与kmem_cache_free。
**Kmalloc**
Slab分配器不仅仅只用来存放内核专用的结构体，它还被用来处理内核对小块内存的请求。当然鉴于Slab分配器的特点，一般来说内核程序中对小于一页的小块内存的求情才通过Slab分配器提供的接口Kmalloc来完成（虽然它可分配32 到131072字节的内存）。从内核内存分配角度讲kmalloc可被看成是get_free_page（s）的一个有效补充，内存分配粒度更灵活了。
有兴趣的话可以到/proc/slabinfo中找到内核执行现场使用的各种slab信息统计，其中你会看到系统中所有slab的使用信息。从信息中可以看到系统中除了专用结构体使用的slab外，还存在大量为Kmalloc而准备的Slab（其中有些为dma准备的）。
**内核非连续内存分配（****Vmalloc****）**
伙伴关系也好、slab技术也好，从内存管理理论角度而言目的基本是一致的，它们都是为了防止“分片”，不过分片又分为外部分片和内部分片之说，所谓内部分片是说系统为了满足一小段内存区（连续）的需要，不得不分配了一大区域连续内存给它，从而造成了空间浪费；外部分片是指系统虽有足够的内存，但却是分散的碎片，无法满足对大块“连续内存”的需求。无论何种分片都是系统有效利用内存的障碍。slab分配器使得含与一个页面内众多小块内存可独立被分配使用，避免了内部分片，节约了空闲内存。伙伴关系把内存块按大小分组管理，一定程度上减轻了外部分片的危害，因为页框分配不在盲目，而是按照大小依次有序进行，不过伙伴关系只是减轻了外部分片，但并未彻底消除。你自己笔画一下多次分配页框后，空闲内存的剩余情况吧。
所以避免外部分片的最终思路还是落到了如何利用不连续的内存块组合成“看起来很大的内存块”——这里的情况很类似于用户空间分配虚拟内存，内存逻辑上连续，其实影射到并不一定连续的物理内存上。Linux内核借用了这个技术，允许内核程序在内核地址空间中分配虚拟地址，同样也利用页表（内核页表）将虚拟地址影射到分散的内存页上。以此完美地解决了内核内存使用中的外部分片问题。内核提供vmalloc函数分配内核虚拟内存，该函数不同于kmalloc，它可以分配较Kmalloc大得多的内存空间（可远大于128K，但必须是页大小的倍数），但相比Kmalloc来说Vmalloc需要对内核虚拟地址进行重影射，必须更新内核页表，因此分配效率上要低一些（用空间换时间）
与用户进程相似内核也有一个名为init_mm的mm_strcut结构来描述内核地址空间，其中页表项pdg=swapper_pg_dir包含了系统内核空间（3G-4G）的映射关系。因此vmalloc分配内核虚拟地址必须更新内核页表，而kmalloc或get_free_page由于分配的连续内存，所以不需要更新内核页表。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/kanghua/systemcall2.bmp)
空闲页框
APP
内存区域 vm_area_structs
malloc、fork、excute、mmap
brk/do_map
get_free_page(s)
用户空间
内核空间
进程虚拟地址空间
系统调用
进程页表
请页异常
内核程序
物理内存影射区
Vmalloc分配区
slab
get_free_page(s)
内核页表
get_free_page(s)
请页异常
vmalloc分配的内核虚拟内存与kmalloc/get_free_page分配的内核虚拟内存位于不同的区间，不会重叠。因为内核虚拟空间被分区管理，各司其职。进程空间地址分布从０到３G(其实是到PAGE_OFFSET,在0x86中它等于0xC0000000)，从3G到vmalloc_start这段地址是物理内存映射区域（该区域中包含了内核镜像、物理页框表mem_map等等）比如我使用的系统内存是64M(可以用free看到)，那么(3G——3G+64M)这片内存就应该映射物理内存，而vmalloc_start位置应在3G+64M附近（说附近因为是在物理内存映射区与vmalloc_start期间还回存在一个8M大小的gap来防止跃界）,vmalloc_end的位置接近4G(说接近是因为最后位置系统会保留一片128k大小的区域用于专用页面映射，还由可能会由高端内存映射区，这些都是细节，这里我们不做纠缠)。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/kanghua/memspace.bmp)
进程地址空间
物理内存映射区
０
**3G**
内核虚拟空间
**Vmalloc_start**
**Vmalloc_end**
上图是内存分布的模糊轮廓
由get_free_page或Kmalloc函数所分配的连续内存都陷于物理映射区域，所以它们返回的内核虚拟地址和实际物理地址仅仅是相差一个偏移量（PAGE_OFFSET），你可以很方便的将其转化为物理内存地址，同时内核也提供了virt_to_phys（）函数将内核虚拟空间中的物理影射区地址转化为物理地址。要知道，物理内存映射区中的地址与内核页表是有序对应，系统中的每个物理页框都可以找到它对应的内核虚拟地址（在物理内存映射区中的）。
而vmalloc分配的地址则限于vmalloc_start与vmalloc_end之间。每一块vmalloc分配的内核虚拟内存都对应一个vm_struct结构体（可别和vm_area_struct搞混，那可是进程虚拟内存区域的结构），不同的内核虚拟地址被4k打大小空闲区的间隔，以防止越界——见下图）。与进程虚拟地址的特性一样，这些虚拟地址可与物理内存没有简单的位移关系，必须通过内核页表才可转换为物理地址或物理页。它们有可能尚未被映射，在发生缺页时才真正分配物理页框。
这里给出一个小程序帮助大家认请上面几种分配函数所对应的区域。
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/vmalloc.h>
unsigned char *pagemem;
unsigned char *kmallocmem;
unsigned char *vmallocmem;
int init_module(void)
{
pagemem = get_free_page(0);
printk("<1>pagemem=%s",pagemem);
kmallocmem = kmalloc(100,0);
printk("<1>kmallocmem=%s",kmallocmem);
vmallocmem = vmalloc(1000000);
printk("<1>vmallocmem=%s",vmallocmem);
}
void cleanup_module(void)
{
free_page(pagemem);
kfree(kmallocmem);
vfree(vmallocmem);
}
## 内存管理实例
### 代码功能介绍
我们希望能通过访问用户空间的内存达到读取内核数据的目的，这样便可进行内核空间到用户空间的大规模信息传输。
具体的讲，我们要利用内存映射功能，将系统内核中的一部分虚拟内存映射到用户空间，从而使得用户空间地址等同与被映射的内核内存地址。
### 代码结构体系介绍
#### 内核空间内存分配介绍
因此我们将试图写一个虚拟字符设备驱动程序，通过它将系统**内核空间映射到用户空间**——将内核虚拟内存映射到用户虚拟地址。当然映射地址时少不了定位内核空间对应的物理地址，并且还要建立新的用户页表项，以便用户进程寻址时能找到对应的物理内存。
从中应该看出，需要我完成既定目标，我们需要获得：*被映射内核空间物理地址**和**建立对应的用户进程页表*。
在内核空间中主要存在kmalloc分配的物理连续空间和vmalloc分配的非物理连续空间。kmalloc分配的空间往往被称为***内核逻辑地址***，由于它是连续分配（直接处理物理页框），而且分配首地址一定，所以其分配的内核虚拟地址对应的实际物理地址很容易获得：内核虚拟地址—PAGE_OFFSET（0xC0000000）（内核有对应例程virt_to_phys）即等于物理地址，而且其对应的页表属于内核页表（swapper_pg_dir）——在系统初始化时就以建立，因此省去了建立页表的工作。
而vmalloc分配的空间被称为***内核虚拟地址***，它的问题相对要复杂些，这是因为其分配的内核虚拟内存空间并非直接操作页框，而是分配的是vm_struct结构。该结构逻辑上连续但对应的物理内存并非连续，也就是说它vamlloc分配的内核空间地址所对应的物理地址并非可通过简单线性运算获得。从这个意义上讲，它的物理地址在分配前是不确定的，因此虽然vmalloc分配的空间与kmalloc一样都是由内核页表来映射的，但vmalloc分配内核虚拟地址时必须更新内核页表。
注释：vmalloc分配的内核虚拟内存与kmalloc/get_free_page分配的内核逻辑内存位于不同的区间，不会重叠。因为内核空间被分区管理，各司其职。进程空间地址分布从０到３G(其实是到PAGE_OFFSET,在0x86中它等于0xC0000000)，从3G到vmalloc_start这段地址是物理内存映射区域（该区域中包含了内核镜像、物理页框表mem_map等等）比如我使用的系统内存是64M(可以用free看到)，那么(3G——3G+64M)这片内存就应该映射物理内存，而vmalloc_start位置应在3G+64M附近（说附近因为是在物理内存映射区与vmalloc_start期间还回存在一个8M大小的gap来防止跃界）,vmalloc_end的位置接近4G(说接近是因为最后位置系统会保留一片128k大小的区域用于专用页面映射，还由可能会由高端内存映射区，这些都是细节，这里我们不做纠缠)。
另一个需要澄清的是，vmalloc分配的内核空间，其结构是vm_area，可千万别与用户空间malloc分配的vm_area_struct结构混淆。前者由内核页表映射，而后者则由用户页表映射。
|进程地址空间|进程地址空间|
|----|----|
|进程地址空间| |
|物理内存映射区kmalloc分配|物理内存映射区kmalloc分配|
|----|----|
|物理内存映射区kmalloc分配| |
|Vmalloc 分配区|Vmalloc 分配区|
|----|----|
|Vmalloc 分配区| |
|０|０|
|----|----|
|０| |
|**3G****（****page_offset****）**|**3G****（****page_offset****）**|
|----|----|
|**3G****（****page_offset****）**| |
|内核虚拟空间|内核虚拟空间|
|----|----|
|内核虚拟空间| |
|**Vmalloc_start**|**Vmalloc_start**|
|----|----|
|**Vmalloc_start**| |
|**Vmalloc_end**|**Vmalloc_end**|
|----|----|
|**Vmalloc_end**| |
上图是内存分布的模糊轮廓
#### 实例蓝图
为了近可能丰富我们的例子程序的场景，我们选择映射vmalloc分配的内核虚拟空间(下面我们简称为vk地址)到用户空间。
要知道用户进程操作的是虚拟内存区域vm_area_struct，我们此刻需要将用户vma区间利用用户页表映射到vk对应的物理内存上去（如下图所示）。这里主要工作便是建立用户也表项完成映射工作，而这个工作完全落在了vma->nopage[[3]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftn3)操作上，该方法会帮助我们在发生“缺页”时，动态构造映射所需物理内存的页表项。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/kanghua/memexample.bmp)
用户虚拟空间Vm_area_struct
Vk空间vm_struct
物理内存
Vma->nopage
我们需要实现nopage方法，动态建立对应页表，而在该方法中核心任务是寻找到vk地址对应的内核逻辑地址[[4]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftn4)。这必然需要我们做以下工作：
a)找到vmalloc虚拟内存对应的内核页表，并寻找到对应的内核页表项。
b)获取内核页表项对应的物理页框指针。
c)通过页框得到对应的内核逻辑地址**。**
#### 基本函数
我们实例将利用一个虚拟字符驱动程序，驱动负责将一定长的内核虚拟地址(vmalloc分配的)映射到设备文件上，以便可以通过访问文件内容来达到访问内存的目的。这样做的最大好处是提高了内存访问速度，并且可以利用文件系统的接口编程（设备在Linux中作为特殊文件处理）访问内存，降低了开发难度。
Map_driver.c就是我们的虚拟字符驱动程序，不用说它要实现文件操作表（file_operations——字符驱动程序主要做的工作便是实现该结构）中的，为了要完成内存映射，除了常规的open/release操作外，必须自己实现mmap操作，该函数将给定的文件映射到指定的地址空间上，也就是说它将负责把vmalloc分配的内核地址映射到我们的设备文件上。
我们下面就谈谈mmap操作的实现细节：
文件操作的mmap操作是在用户进行系统调用mmap[[5]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftn5)时被执行的，而且在调用前内核已经给用户找到并分配了合适的*虚拟内存区域*vm_area_struct，这个区域将代表文件内容，所以剩下要做的便是如何把虚拟区域和物理内存挂接到一起了，即构造页表。由于我门前面所说的原因，我们系统中页表需要动态分配，因此不可使用remap_page_range函数一次分配完成，而必须使用*虚拟内存区域*自带的nopage方法，在现场构造页表。这样以来，文件操作的mmap的方法只要完成“为它得到的*虚拟内存区域*绑定对应的操作表vm_operations”即可。于是主要的构造工作就落在了vm_operations中的nopage方法上了。
Nopage方法中核心内容上面已经提到了是“寻找到vk地址对应的内核逻辑地址”，这个解析内核页表的工作是需要自己编写辅助函数vaddr_to_kaddr来完成的，它所作的工作概括来讲就是上文提到的a/b/c三条。
有关整个任务执行路径请看下图。
![](http://p.blog.csdn.net/images/p_blog_csdn_net/kanghua/memexample2.bmp)
### STEP BY STEP
编译map_driver.c为map_driver.o模块,具体参数见Makefile
加载模块：insmod map_driver.o
生成对应的设备文件
1 在/proc/devices下找到map_driver对应的设备命和设备号：grep mapdrv /proc/devices
2 建立设备文件mknodmapfile c 254 0（在我系统里设备号为254）
利用maptest读取mapfile文件，将取自内核的信息（”ok”——我们在内核中在vmalloc分配的空间中填放的信息）打印到用户屏幕。
全部程序下载[mmap.tar](http://download.csdn.net/source/268397)（感谢Martin Frey，该程序主体出自他的灵感）

[[1]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftnref1)还有些情况必须要求内存连续，比如DMA传输中使用的内存，由于不涉及页机制所以必须连续分配。
[[2]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftnref2)这种存储池的思想在计算机科学里广泛应用，比如数据库连接池、内存访问池等等。
[[3]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftnref3)构建用户也表项，除了使用nopage一次一页的动态构造，还又一种方法remap_page_range可以一次构造一段内存范围的也表项，但显然这个方法时针对物理内存连续被分配时使用的，而我们vk对应的物理内存并非连续，所以这里使用nopage。
[[4]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftnref4)很多人一定会问，为什么不直接找到物理地址那，而要找内核逻辑地址呢？没错，我们本意应该是获得物理地址，但是为了利用内核提供的一些现成的例程，如virt_to_page等（它们都是针对内核逻辑地址而言的），我们不妨转化成内核逻辑地址来做，别忘了内核逻辑地址与理地址仅仅相差一个偏移量。
[[5]](http://writeblog.csdn.net/Editor/FCKeditor/editor/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#_ftnref5)系统调用mmap原形是void *mmap2(void *start, size_t length, int prot, int flags, int fd, off_t pgoff)。
