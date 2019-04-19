# Core file 文件格式(Linux Coredump文件结构) - xqhrs232的专栏 - CSDN博客
2016年10月19日 00:13:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：192
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/_xiao/article/details/22389997](http://blog.csdn.net/_xiao/article/details/22389997)
[原创]转载请注明来源于CSDN _xiao。
[Linux](http://lib.csdn.net/base/linux)异常时会生成Coredump文件用于调试和分析，Coredump文件在嵌入式工作中对死机问题的帮助是非常大的。网上有很多生成core文件的配置方法，但却很少涉及core文件的数据格式，这里通过linux的源码走读尝试解读Coredump文件的格式。
Coredump文件的整体格式是ELF格式，但在ELF格式定义中对Core子类别的定义是开放的，由各[架构](http://lib.csdn.net/base/architecture)自己定义，因此Core文件在不同的体系中格式是有差别的。这里主要说明Linux下生成的Core文件，另外我们在工作中通常接触的指令架构是ARM和MIPS。
Coredump文件按ELF格式组织，不过它只有Executing View，没有Linking View，也就是只有Program
 Headers，没有Section Heders。所以用readelf命令查看时只能看到Segments而看不到Sections。异常产生时，Linux将死机进程的内存段的内容以Segment的形式保存到Coredump文件中（包括数据区，堆栈区，已分配的堆内容，以及死机时的帧结构数据等），对于代码段（属性为RX，即只读可执行的段），Linux不保存其内容，只保存代码段的地址信息；其它如线程信息、寄存器信息等辅助信息则保存在类型为PT_NOTE的Segment段中。所以使用GDB工具查看时，需要原始的可执行文件来恢复代码区的内容。
Linux生成Coredump的相关代码在3.1.10\fs\binfmt_elf.c中。
从函数elf_core_dump看起。首先申请空间存放elfhdr文件头，然后通过current->mm->map_count得到当前进程已映射的内存段数量，计算core文件中需要生成的segment数量，这里会多保留一个segment供PT_NOTE使用。然后调用fill_note_info填充note段信息。在fill_note_info里，先将所有的task放到info->thread_list链表中，然后遍历该链表，对每一个thread调用elf_dump_thread_status来保存该线程的状态。在elf_dump_thread_status中，先调用fill_prstatus获取线程状态，这包括收到的signal（对死机线程而言就是引起coredump的信号了），pending和hold的signal，线程的pid,pgrp,ppid等各种数据。再调用elf_core_copy_task_regs获取线程的寄存器，这里对ARM和MIPS会调用各自的函数来保存寄存器，例如MIPS会调用3.1.10\arch\mips\kernel\binfmt_elfo32.c里的elf32_core_copy_regs函数来获取MIPS寄存器的内容，再调用elf_core_copy_task_fpregs获取线程的浮点寄存器内容，在获取完信息后都会调用fill_note将这些信息放到类型为memelfnote的notes数组中（后面会利用这个notes数组再将所有信息保存到PT_NOTE段中）。回到elf_core_dump，现在获得了所有的note，所以可以计算出整个note段的大小，以及计算所有Program
 Header的地址信息了。填充elf头，为所有的Program header申请空间，调用fill_elf_note_phdr将note的地址放到phdr4note头中，其类型设置为PT_NOTE，然后遍历current->mm->mmap,为每一个已分配的内存段填充phdr，其类型设置为PT_LOAD，这里会调用vma_dump_size检查vma->vm_flags标志来决定哪些内存的内容需要保存，对IO的映射内存是不保存的，共享内存，私有内存，和已写入的内存都会保存。处理完所有内存segment的头数据后，再调用memcpy_note依次将前面搜集的所有note的信息写入到PT_NOTE的segment中，每个note信息一个个依次写入，按顺序排列在PT_NOTE段里，其头结构是elf32_note类型，即4字节的name长度，4字节内容长度，4字节的类型描述，紧跟其后就name的内容和note的内容（注意这里name长度和内容长度会按4的倍数向上取整）。之后elf_core_dump开始初始化gzip，先调用compress_coredump将之前准备好的elfhdr,phdr4note,Program
 header, 和PT_NOTE段的内容压缩写入，再重新遍历current->mm->mmap，对每一个内存段的每一页调用compress_coredump将其内容压缩到文件中，每次压缩一个PAGE。最后写入gzip的尾部和校验和，整个coredump过程就结束了。
根据以上代码，Core文件的整体布局如下图1所示，它与普通ELF文件的差别是多了一个特定的PT_NOTE类型的段，用于存放线程信息和寄存器信息。
![Core File 整体格式 (ELF)](https://img-blog.csdn.net/20140328192824218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvX3hpYW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（图1）Core File文件的整体布局
从Coredump文件的结构看到，保存现场信息的时候没有对一个很关键的信息进行处理，那就是载入的动态链接库文件(*.so文件)的信息。我们的工程运行时通常都加载很多库，问题也常出现在这些动态库里，那么将加载的库正确地映射到对应的地址上就是一件至关重要的事情(包括程序运行时通过dlopen动态加载的库)。通常调试工具GDB载入Coredump文件时能把动态库加载到正确的地址上（前提是设置了正确的库查找路径），是怎么做到的呢？这涉及到ELF文件的动态加载和ld.so动态加载器的工作流程，关键信息已随主执行程序和ld.so动态库的数据区一起保存到了PT_NOTE段里，详细过程请看另一篇《GDB如何从Coredump文件恢复动态库信息》。
下面用一个实际例子看Coredump文件的[数据结构](http://lib.csdn.net/base/datastructure)。
图2是一个在mips平台上生成的Coredump文件的头部的十六进制数据。
![Coredump](https://img-blog.csdn.net/20140328201142281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvX3hpYW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 （图2）一个实际的Coredump文件
文件开始是一个ELF Header，该头的结构定义为：
typedef struct elf32_hdr{
  unsigned char e_ident[EI_NIDENT];    // 16字节标识
  Elf32_Half e_type;
  Elf32_Half e_machine;
  Elf32_Word e_version;
  Elf32_Addr e_entry;
  Elf32_Off e_phoff;    // Program Headers的文件偏移地址
  Elf32_Off e_shoff;
  Elf32_Word e_flags;
  Elf32_Half e_ehsize;    // ELF Header头结构的大小
  Elf32_Half e_phentsize;    // 每个Program Header信息描述占用的大小
  Elf32_Half e_phnum;    // Program Header的数量
  Elf32_Half e_shentsize;
  Elf32_Half e_shnum;
  Elf32_Half e_shstrndx;
} Elf32_Ehdr;
根据该结构，解析Coredump文件的头信息如图3所示。
![Coredump ELF头信息解析](https://img-blog.csdn.net/20140328201928875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvX3hpYW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（图3）Coredump文件ELF头信息的解析
从图3的解析看到：e_type为0x04（ET_CORE），表示这是一个core文件；e_phoff为0x0034，表示Program
 Headers信息从文件的0x34地址开始；e_ehsize为0x34，表示此ELF Header文件头占用的字节为0x34；e_phentsize为0x20（32字节），表示每个Program
 Header占用的大小为32字节；e_phnum为0x23，表示这个core文件共含有35个segment段；根据e_phoff信息，Program
 Headers从0x34字节开始，即紧跟在ELF Header之后开始。
再来分析Program Headers信息，根据ELF Header，每个Program Header占据的字节数为32字节，说明这是一个32位的Elf32_Phdr（对应的还有64位的Elf64_Phdr，其size要大一些），Elf32_Phdr的定义如下：
typedef struct elf32_phdr{
  Elf32_Word p_type;    // segment的类型
  Elf32_Off p_offset;    // segment数据在文件中的偏移
  Elf32_Addr p_vaddr;    // segment加载到内存中的虚拟地址
  Elf32_Addr p_paddr;
  Elf32_Word p_filesz;    // segment数据在文件中的数据大小
  Elf32_Word p_memsz;    // segment数据在内存中占据的大小
  Elf32_Word p_flags;    // segment的属性标志
  Elf32_Word p_align;
} Elf32_Phdr;
根据这个定义，先分析第一个Program Header的信息，如图4所示。
![Coredump Program Header 0 分析](https://img-blog.csdn.net/20140328203910859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvX3hpYW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（图4）第1个Program Header信息的解析
从图4的解析看到：第1个Segment的p_type为0x04（PT_NOTE），表示这是一个描述note信息的段；p_offset为0x494，表示note信息段从文件的0x494字节开始；p_filesz为0x358，表示note信息共有856字节。由于note信息是辅助信息段，在原程序中并不存在于内存中，所以其p_vaddr，p_memsz，p_flags等均为0。
紧接着的是第二个Program Header信息（从地址0x34+0x20=0x54开始），其分析如图5所示。
![Coredump Program Header1 分析](https://img-blog.csdn.net/20140328204928250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvX3hpYW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（图5）第2个Program Header信息的解析
从图5的解析看到：第2个Segment的p_type为0x01（PT_LOAD），这是一个可加载的段；p_offset为0x1000，表示该段从文件的0x1000字节开始；p_vaddr为0x400000，表示该段在原程序的内存中的虚拟地址为0x400000（对mips而言，此地址是主程序的加载地址）；p_filesz为0，表示该段在文件中的大小为0，如前所述，由于该段是一个代码段，所以Core文件并没有保存其内容，因此该段在Core文件中的数据长度为0，调试时需要原程序的ELF档加载到0x400000地址才能分析此程序；p_memsz为0x10000，表示该段在原内存中占据4096的大小（即该段占据0x400000~0x40FFFF的地址空间）；p_flags为0x05，即PF_R
 | PF_X，表示该段是一个只读可执行的代码段。
按如上方法继续下去就可以将所有的段信息解析出来。
实际中，我们不需要自己去解析文件，通过工具如readelf或objdump等都可以轻松地将上面的信息解析出来。图6是上面Coredump文件对应的readelf运行结果。
![readelf -l Coredump](https://img-blog.csdn.net/20140328211006718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvX3hpYW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（图6）readelf读取Coredump文件
再来尝试分析PT_NOTE段的内容。PT_NOTE段的内容与体系架构有关，如前所述，它是由许多note信息段依次拼接而成的。每个note信息段的头部定义为：
typedef struct elf32_note {
  Elf32_Word n_namesz; /* Name size */
  Elf32_Word n_descsz; /* Content size */
  Elf32_Word n_type;  /* Content type */
} Elf32_Nhdr;
根据fill_note_info函数，第一个note信息是info->prstatus信息，在mips中，该结构体定义为(见文件binfmt_elfo32.c)：
#define elf_prstatus elf_prstatus32
struct elf_prstatus32
{
 struct elf_siginfo pr_info; /* Info associated with signal */
 short pr_cursig;  /* Current signal */
 unsigned int pr_sigpend; /* Set of pending signals */
 unsigned int pr_sighold; /* Set of held signals */
 pid_t pr_pid;
 pid_t pr_ppid;
 pid_t pr_pgrp;
 pid_t pr_sid;
 struct compat_timeval pr_utime; /* User time */
 struct compat_timeval pr_stime; /* System time */
 struct compat_timeval pr_cutime;/* Cumulative user time */
 struct compat_timeval pr_cstime;/* Cumulative system time */
 elf_gregset_t pr_reg; /* GP registers */
 int pr_fpvalid;  /* True if math co-processor being used.  */
};
其中的elf_siginfo结构定义为：
struct elf_siginfo
{
 int si_signo;   /* signal number */
 int si_code;   /* extra code */
 int si_errno;   /* errno */
};
寄存器信息的结构定义为：
#define ELF_NGREG 45
typedef elf_greg_t elf_gregset_t[ELF_NGREG];
另外根据elf32_core_copy_regs函数，在elf_gregset_t寄存器数组中，前6个寄存器都是0（这几个只用于64位mips），从索引6的寄存器开始，依次存放R0到R31的寄存器数据。
所以按照上面的结构解析第一个note信息如图7所示。第一个note信息从地址0x494开始，其中name字段长度为5个字节，由于按4的倍数向上取整，所以实际占据了8个字节；信息内容的长度为256，即从0x4A8到0x5A7都是其内容，注意其中pr_cursig由于结构体的字节对齐被填充了2个字节。
根据图7的解析，死机时，R1寄存器的值为0x2ad2b007，R2寄存器的值为0，R3寄存器的值为0x2ac21840……，死机时线程收到的信号为11，即segment
 fault，表示段错误，一般是操作非法内存地址导致的，这些都与GDB解析的结果相符。
从地址0x5A8开始是第二个note的信息，按同样的方法继续解析，见图7的下半部分。
![note信息的解析](https://img-blog.csdn.net/20140408130015781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvX3hpYW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（图7）note信息的解析
以上分析了Coredump文件的结构，由于note信息段的内容与体系架构有关，对于其它平台例如ARM平台的，需要根据fill_note_info函数再逐一分析，这里不再描述。如何从Coredump文件中恢复动态库文件信息请看另一篇《GDB如何从Coredump文件恢复动态库信息》。
