# brk和sbrk及内存分配函数介绍 - h13 - 博客园
brk和sbrk主要的工作是实现虚拟内存到内存的映射.在GNUC中,内存分配是这样的:
       每个进程可访问的虚拟内存空间为3G，但在程序编译时，不可能也没必要为程序分配这么大的空间，只分配并不大的数据段空间，程序中动态分配的空间就是从这一块分配的。如果这块空间不够，malloc函数族（realloc，calloc等）就调用sbrk函数将数据段的下界移动，sbrk函数在内核的管理下将虚拟地址空间映射到内存，供malloc函数使用。（参见linux内核情景分析）
#include <unistd.h>
       int brk(void *end_data_segment);
       void *sbrk(ptrdiff_t increment);
DESCRIPTION
       brk   sets   the   end   of   the   data   segment   to   the value specified by
       end_data_segment, when that value is reasonable, the system   does   have
       enough   memory   and   the process does not exceed its max data size (see
       setrlimit(2)).
       sbrk increments the program's data   space   by   increment   bytes.    sbrk
       isn't a system call, it is just a C library wrapper.   Calling sbrk with
       an increment of 0 can be used to find the current location of the   pro-
       gram break.
RETURN VALUE
       On   success,   brk returns zero, and sbrk returns a pointer to the start
       of the new area.   On error, -1 is returned, and errno is set to ENOMEM.
sbrk不是系统调用，是C库函数。系统调用通常提供一种最小界面，而库函数通常提供比较复杂的功能。
在Linux系统上，程序被载入内存时，内核为用户进程地址空间建立了代码段、数据段和堆栈段，在数据段与堆栈段之间的空闲区域用于动态内存分配。
内核数据结构mm_struct中的成员变量start_code和end_code是进程代码段的起始和终止地址，start_data和 end_data是进程数据段的起始和终止地址，start_stack是进程堆栈段起始地址，start_brk是进程动态内存分配起始地址（堆的起始地址），还有一个 brk（堆的当前最后地址），就是动态内存分配当前的终止地址。
C语言的动态内存分配基本函数是malloc()，在Linux上的基本实现是通过内核的brk系统调用。
brk()是一个非常简单的系统调用，只是简单地改变mm_struct结构的成员变量brk的值。
mmap系统调用实现了更有用的动态内存分配功能，可以将一个磁盘文件的全部或部分内容映射到用户空间中，进程读写文件的操作变成了读写内存的操作。
在linux/mm/mmap.c文件的do_mmap_pgoff()函数，是mmap系统调用实现的核心。
do_mmap_pgoff()的代码，只是新建了一个vm_area_struct结构，并把file结构的参数赋值给其成员变量vm_file，并没有把文件内容实际装入内存。
Linux内存管理的基本思想之一，是只有在真正访问一个地址的时候才建立这个地址的物理映射。
//////////////////////////////////////////
C语言跟内存分配方式
（1） 从静态存储区域分配。内存在程序编译的时候就已经分配好，这块内存在程序的整个运行期间都存在。例如全局变量，static变量。
（2） 在栈上创建。在执行函数时，函数内局部变量的存储单元都可以在栈上创建，函数执行结束时这些存储单元自动被释放。栈内存分配运算内置于处理器的指令集中，效率很高，但是分配的内存容量有限。
（3）从堆上分配，亦称动态内存分配。程序在运行的时候用malloc或new申请任意多少的内存，程序员自己负责在何时用free或delete释放内存。动态内存的生存期由我们决定，使用非常灵活，但问题也最多
C语言跟内存申请相关的函数主要有 alloca,calloc,malloc,free,realloc,sbrk等.
其中alloca是向栈申请内存,因此无需释放. malloc分配的内存是位于堆中的,并且没有
初始化内存的内容,因此基本上malloc之后,调用函数memset来初始化这部分的内存空间.
calloc则将初始化这部分的内存,设置为0. 而realloc则对malloc申请的内存进行大小的
调整.申请的内存最终需要通过函数free来释放. 而sbrk则是增加数据段的大小;
malloc/calloc/free基本上都是C函数库实现的,跟OS无关.C函数库内部通过一定的
结构来保存当前有多少可用内存.如果程序malloc的大小超出了库里所留存的空间,那么
将首先调用brk系统调用来增加可用空间,然后再分配空间.free时,释放的内存并不立即
返回给os,而是保留在内部结构中. 可以打个比方: brk类似于批发,一次性的向OS申请大
的内存,而malloc等函数则类似于零售,满足程序运行时的要求.这套机制类似于缓冲.
使用这套机制的原因: 系统调用不能支持任意大小的内存分配(有的系统调用只支持固定大小以及其倍数的内存申请,这样的话,对于小内存的分配会造成浪费; 系统调用申请内存代价昂贵,涉及到用户态和核心态的转换.
函数malloc()和calloc()都可以用来分配动态内存空间，但两者稍有区别。   
     malloc()函数有一个参数，即要分配的内存空间的大小：    
     void *malloc(size_t size);  
     calloc()函数有两个参数，分别为元素的数目和每个元素的大小，这两个参数的乘积就是要分配的内存空间的大小：   
     void *calloc(size_t numElements，size_t sizeOfElement)；
     如果调用成功，函数malloc()和calloc()都将返回所分配的内存空间的首地址。
     malloc() 函数和calloc()函数的主要区别是前者不能初始化所分配的内存空间，而后者能。如果由malloc()函数分配的内存空间原来没有被使用过，则其中的每一位可能都是0；反之，如果这部分内存空间曾经被分配、释放和重新分配，则其中可能遗留各种各样的数据。也就是说，使用malloc()函数的程序开始时(内存空间还没有被重新分配)能正常运行，但经过一段时间后(内存空间已被重新分配)可能会出现问题。
     calloc() 函数会将所分配的内存空间中的每一位都初始化为零，也就是说，如果你是为字符类型或整数类型的元素分配内存，那么这些元素将保证会被初始化为零；如果你是为指针类型的元素分配内存，那么这些元素通常(但无法保证)会被初始化为空指针；如果你是为实数类型的元素分配内存，那么这些元素可能(只在某些计算机中)会被初始化为浮点型的零。
     malloc() 函数和calloc()函数的另一点区别是calloc()函数会返回一个由某种对象组成的数组，但malloc()函数只返回一个对象。为了明确是为一个数组分配内存空间，有些程序员会选用calloc()函数。但是，除了是否初始化所分配的内存空间这一点之外，绝大多数程序员认为以下两种函数调用方式没有区别：
     calloc(numElements ，sizeOfElement)；
     malloc(numElements *sizeOfElement) ；
     需要解释的一点是，理论上(按照ANSIC标准)指针的算术运算只能在一个指定的数组中进行，但是在实践中，即使C编译程序或翻译器遵循这种规定，许多C 程序还是冲破了这种限制。因此，尽管malloc()函数并不能返回一个数组，它所分配的内存空间仍然能供一个数组使用(对realloc()函数来说同样如此，尽管它也不能返回一个数组)。
     总之，当你在calloc()函数和malloc()函数之间作选择时，你只需考虑是否要初始化所分配的内存空间，而不用考虑函数是否能返回一个数组。
     当程序运行过程中malloc了,但是没有free的话,会造成内存泄漏.一部分的内存没有
被使用,但是由于没有free,因此系统认为这部分内存还在使用,造成不断的向系统申请内
存,是的系统可用内存不断减少.但是,内存泄漏仅仅指程序在运行时,程序退出时,OS将回
收所有的资源.因此,适当的重起一下程序,有时候还是有点作用.
sbrk(int incr) 本函数用来增加分配给调用程序的数据段的空间数量,增加incr个字节的空间
brk函数的原形是：int    brk(void    *endds)  
   它的功能是：更改数据段空间的分配  
   char    *p;  
   p=malloc(1);  
   这时p指向的内存空间大小是1    byte  
   brk(p+100)  
   这时p指向的内存空间大小是101    bytes   
