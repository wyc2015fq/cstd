# vivi代码分析(二) - 一世豁然的专栏 - CSDN博客





2015年06月14日 08:42:22[一世豁然](https://me.csdn.net/Explorer_day)阅读数：552








本博客转载于：[http://www.cnblogs.com/tureno/articles/4445397.html](http://www.cnblogs.com/tureno/articles/4445397.html)





现在进入bootloader之vivi分析的第二阶段，这部分使用C语言实现，部分代码采取**内嵌汇编**的方式。这里需要用到GNU GCC内嵌汇编的知识，这部分基础还没有具备，需要学习。



    下面先按照流程进行分析。需要注意的是，此部分内容并非完全按照原版的vivi源代码，而是加入了自己的理解。另外，对非常简单、google出一片而且有分析正确的部分，在这里就简化了，不做详细分析，只是对网上没有分析到位而又影响理解的部分进行深入分析。我想，这部分内容应该是对《s3c2410完全开发》中vivi源代码分析部分的补充和完善。



stage 2：【init/main.c】



    第二阶段的入口就是init/main.c，按照源代码的组织流程，根据模块化划分的原则，应该分为**8个功能模块**，源代码注释以step区分，非常清晰。现在首先解决一个问题，就是关于main的形参。vivi源代码中对main的原型使用了：int main(int argc, char *argv[])的标准形式，在第一阶段的【arch/s3c2410/head.S】中，利用APCS设定了相应的入口参数，如下：



|```@ get read tocall C functions        ldr sp, DW_STACK_START @ setup stack pointer        mov fp, #0 @ no previous frame, so fp=0        mov a2, #0 @ set argv to NULL        bl main @ call main```|
|----|




**这里的sp、fp、a2都是APCS中名字，与之对应的寄存器分别为R13、R11、R2。**这里理解的重点在于fp（frame pointer，帧指针），也就是栈帧指针。这是比较复杂的一个地方，对栈需要有深入的分析。搜集了一些资料，看了APCS标准，后续会把关于fp和APCS的部分单独拿出，总结成文。不管怎样，通过其入口地址的设置也可以看出，main是不需要入口地址的。那么，为了理解上的方便，不妨把main原型改为int
 main(void)，这样，相应的入口地址就不需要设置了。更改后的head.S对应部分如下：



|```@ jump to ram        @ a technology about trampoline        ldr pc, =on_the_ramon_the_ram:        bl main        @ if main ever returns, reboot        mov pc, #FLASH_BASE```|
|----|




    清晰而且符合规则，前提是init/main.c中main原型修改为int main(void)。当然，对main还动了一些手术，现在把main的主流程部分放在这里，后面会对为什么如此改动详细说明。





|```int main(void){        int ret;        /*         * Step 1:         * Print Vivi version information         */        putstr("/r/n");        putstr(vivi_banner);        reset_handler();        /*         * Step 2:         * initialize board environment         */        ret = board_init();        if (ret){                putstr("Failed a board_init() procedure/r/n");                error();        }        /*         * Step 3:         * MMU management         * When it's done, vivi is running on the ram and MMU is enabled.         */        mem_map_init();        mmu_init();        putstr("Succeed memory mapping./r/n");        /*          * Step 4:         * initialize the heap area         */        ret = heap_init();        if (ret){                putstr("Failed initailizing heap region/r/n");                error();        }        /*         * Step 5:         * initialize the MTD device         */        ret = mtd_dev_init();        /*         * Step 6:         * initialize the private data         */        init_priv_data();        /*         * Step 7:         * initialize the humanmachine environment         */        misc();        init_builtin_cmds();        /*         * Step 8:         * boot kernel or step into vivi         */        boot_or_vivi();        return 0;}```|
|----|




（1）step 1：打印版本信息



    这一部分其实是作为调试和增强人机交互行而用的，如果不用，对vivi的主要功能也不会产生影响。本来是最为简单的一个部分，但是实际上却是我理解上问题最多的一个部分，对这块动的手术也最多。事实上，这个部分的reset_handler存在bug。具体分析一下。



    源代码step 1部分如下：

|```putstr("/r/n");putstr(vivi_banner);reset_handler();```|
|----|




    打印的vivi_banner在【init/version.c】中，如下：



|```#include"version.h"#include "compile.h"const char *vivi_banner =                       "VIVI version " VIVI_RELEASE" (" VIVI_COMPILE_BY"@"                       VIVI_COMPILE_HOST ") (" VIVI_COMPILER") " UTS_VERSION"/r/n";```|
|----|




   vivi_banner就是字符串，中间有五个未知的宏，这是非常明显的。显然就是头文件中给出的。但是find，没有version.h。compile.h，所以现在就要知道version.h,compile.h是怎样生成的，在compile.h中一次定义了VIVI_COMPILE_BY，VIVI_COMPILE_HOST，VIVI_COMPILER ，UTS_VERSION 这四个宏，这在前面Makefile分析时也详细讲解过了，我们可以在顶层的Makefile中找到在编译时是如何自动生成version.h以及compile.h这两个头文件的：

include/version.h:

 @echo /#define VIVI_RELEASE /"$(VIVIRELEASE)/" > .ver

 @echo /#define VIVI_VERSION_CODE `expr $(VERSION) //* 65536 + $(PATCHLEVEL) //* 256 + $(SUBLEVEL)` >> .ver

 @echo '#define VIVI_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))' >>.ver
@mv -f .ver $@

可以看到在version.h中定义了VIVI_RELEASE，VIVI_VERSION_CODE以及VIVI_VERSION（a,b,c）三个宏，同样在顶层Makefile中也定义了compile.h的生成规则，而且还把version.h合并入compile.h里面了。可以just for fun，增加一些个性化的打印信息，比如，我的version.c修改如下：



|```#include"compile.h"const char *vivi_banner =                        "/r/n/t^_^ Well done, boy! Go on -->/r/n"                        "VIVI version " VIVI_RELEASE" (" VIVI_COMPILE_BY"@"                        VIVI_COMPILE_HOST ") (" VIVI_COMPILER") " UTS_VERSION"/r/n";```|
|----|




    下面进入疑惑的reset_handler功能部分。作者的本意是**利用reset_handler()实现软复位跟硬复位的处理。**



|```【lib/reset_handle.c】voidreset_handler(void){    int pressed;    pressed = is_pressed_pw_btn();    if (pressed== PWBT_PRESS_LEVEL){        DPRINTK("HARD RESET/r/n");        hard_reset_handle();    } else {        DPRINTK("SOFT RESET/r/n");        soft_reset_handle();    }}```|
|----|




    首先看一下is_pressed_pw_btn，按照函数字面意思，应该是判断电源复位键是否按下，**如果按下，则证明是硬复位**；如果没有检测到键按下，那么就是软复位。具体代码如下：


|```staticintis_pressed_pw_btn(void){    return read_bt_status();}--> read_bt_statusstatic intread_bt_status(void){    ulong status;    //status = ((GPLR & (1 << GPIO_PWBT)) >> GPIO_PWBT);    **status = ((PWBT_REG & (1 << PWBT_GPIO_NUM)) >> PWBT_GPIO_NUM);**        if (status)        return HIGH;    else        return LOW;}```|
|----|




    可是，PWBT_REG是没有定义的，PWBT_GPIO_NUM也没有定义，也就是说，这个函数实际上是不可能编译通过的。从表面上分析，如同网上大部分讨论一样，我可以知道作者是什么意图，但是这段代码真正有效吗？从上面分析看，答案显然是这个reset_handle.c根本就是无效的。那么为什么vivi移植成功都没有注意这个问题，还只是按照表面意思分析代码呢？这个可以看reset_handle.h(在include文件夹中)的头文件。



|```#ifdef CONFIG_RESET_HANDLINGvoid reset_handler(void);#else#define reset_handler()    (void)(0)#endif```|
|----|




    很显然，在配置的时候，CONFIG_RESET_HANDLING是没有定义的，那么reset_handler()为空，也就是说这部分根本就是空代码，并没有实际执行功能。如果还不放心，那就做测试，如果把CONFIG_RESET_HANDLING选中（具体是把General setup部分的support reset handler选中），那么就会出现错误：


|```reset_handle.c: In function `read_bt_status':reset_handle.c:31: `PWBT_REG' undeclared (first use inthis function)reset_handle.c:31:(Each undeclared identifier is reported only oncereset_handle.c:31:for each function it appears in.)reset_handle.c:31: `PWBT_GPIO_NUM' undeclared (first use in this function)reset_handle.c:28: warning: `status' might be used uninitialized in this functionreset_handle.c: In function `hard_reset_handle':reset_handle.c:52: `USER_RAM_BASE' undeclared (first use in this function)reset_handle.c:52: `USER_RAM_SIZE' undeclared (first use in this function)reset_handle.c: In function `reset_handler':reset_handle.c:68: `PWBT_PRESS_LEVEL' undeclared (first use in this function)make[2]: *** [reset_handle.o] Error 1make[2]: Leaving directory `/home/armlinux/embedded_Linux/s3c2410/bootloader/m-boot-1.0.0/lib'make[1]:***[first_rule]Error 2make[1]: Leaving directory `/home/armlinux/embedded_Linux/s3c2410/bootloader/m-boot-1.0.0/lib```|
|----|



    可见这部分功能是多余的。可以选择把这部分设置完全去掉，方法如下：



·【init/main.c】去掉行reset_handler();，去掉#include <reset_handle.h>。
    ·删除【lib/reset_handle.c】，删除【include/reset_handle.h】
    ·【arch/config.in】，删除行bool 'support reset handler' CONFIG_RESET_HANDLING，这样就彻底把此项配置部分也删除了。如果还有原来的默认配置文件，可以把# CONFIG_RESET_HANDLING is not set删除。



    经过上面三步，就可以把reset handler功能去掉了。这些在你了解了vivi的配置机制后是很容易操作的，它们之间的关系并不复杂，就是一条链，顺着找就可以了。



    我现在第一步想做的是把vivi进行“瘦身”，只需要完成在EDUKIT-III上从nand flash启动引导内核的功能就可以，从中也可以了解核心技术和主要流程。但是，在整个的软件架构上是保持不变的。如果我想增加功能，因为对这个软件架构熟悉了，所以很容易扩展，而且也容易自己重新做一个功能更好一些的bootloader。



（2）step 2：



主要是初始化GPIO。这个在前面实验中做过了，基本的思路和方法就是在把握好整个系统硬件资源的前提下，根据datasheet把所有的初始值设定，在这里利用这个函数就可以完成初始化了。我们在这里稍微分析下board_init()函数(arch/s3c2410/smdk.c)

int board_init(void)

{

 init_time();   //arch/s3c2410/proc.c中

 set_gpios(); //arch/s3c2410/smdk.c

 return 0;

}

先来看看init_time():

void init_time(void)

{

 TCFG0 = (TCFG0_DZONE(0) | TCFG0_PRE1(15) | TCFG0_PRE0(0));

}

再来看看set_gpios()

void set_gpios(void)

{

 GPACON  = vGPACON;

 GPBCON  = vGPBCON;

 GPBUP   = vGPBUP; 

 GPCCON  = vGPCCON;

 GPCUP   = vGPCUP; 

 GPDCON  = vGPDCON;

 GPDUP   = vGPDUP; 

 GPECON  = vGPECON;

 GPEUP   = vGPEUP; 

 GPFCON  = vGPFCON;

 GPFUP   = vGPFUP; 

 GPGCON  = vGPGCON;

 GPGUP   = vGPGUP; 

 GPHCON  = vGPHCON;

 GPHUP   = vGPHUP; 

 EXTINT0 = vEXTINT0;

 EXTINT1 = vEXTINT1;

 EXTINT2 = vEXTINT2;

}也就是设置各个GPIO口的控制寄存器，和上拉寄存器



（3）step 3：



    MMU初始化。这部分在MMU基础实验中完成了。关于GNU GCC内嵌汇编部分还不是太清晰，还有待于在后续工作中加强。

   mem_map_init(); //arch/s3c2410/mmu.c

    mmu_init();



（4）step 4：

ret = heap_init(); //lib/heap.c



    heap——堆，内存动态分配函数mmalloc就是从heap中划出一块空闲内存的，mfree则将动态分配的某块内存释放回heap中。

heap_init函数在SDRAM中指定了一块1M大小的内存作为heap(起始地址HEAP_BASE  =  0x33e00000)，并在heap的开头定义了一个数据结构blockhead——事实上，heap就是使用一系列的blockhead数据结构来描述和操作的。每个blockhead数据结构对应着一块heap内存，**假设一个blockhead数据结构的存放位置为A，则它对应的可分配内存地址为“A + sizeof(blockhead)”到“A +sizeof(blockhead) + size - 1”。(因为A地址处存放了一个blockhead结构所以不能分配，从A+sizeof(blockhead)处开始，而blockhead结构体中的size定义了该结构控制的区域的大小，所以这块区域的尾地址也就是A+sizeof（blockhead）+size -1,后面的blockhead结构就要从这块的尾地址开始存放，这样依次存放，每个blockhead结构通过指针练成一个链表，这样利于分配)**

堆初始化。堆与栈的区别已经比较清晰了，在动手分析vivi的过程中，更为明确了。在这里，实际上就是**实现动态内存分配策略**。具体实现部分在【lib/heap.c】。因为以前自己没有写过动态内存分配，所以要仔细分析这部分是如何实现的。这部分的工作主要有两个：**一是分析封装调试宏的技巧和printk的实现方法，这部分在这里还是挺重要的。二是heap基本的原理是什么？具体如何实现？我们暂且来分析下heap_init函数：**

int heap_init(void)

{

 return mmalloc_init((unsigned char *)(HEAP_BASE), HEAP_SIZE); 

}

 我们在smdk2410.h中发现上面两个常量的定义：

#define HEAP_SIZE  SZ_1M //堆的分配大小为1M

#define HEAP_BASE  (VIVI_RAM_BASE - HEAP_SIZE)//我们可以看到一是高端顺序存放二是堆空间的分配挨着sdram中VIVI的分区

typedef struct blockhead_t {

 Int32 signature;  //固定为BLOCKHEAD_SIGNATURE 

 Bool allocated;  //此区域是否已经分配出去：0-N，1-Y

 unsigned long size;  //此区域大小

 struct blockhead_t *next;  //链表指针

 struct blockhead_t *prev;  //链表指针

} blockhead;

vivi对heap的操作比较简单，vivi中有一个全局变量static blockhead *gHeapBase，它是heap的链表头指针，通过它可以遍历所有blockhead数据结构。假设需要动态申请一块sizeA大小的内存，则mmalloc函数从gHeapBase开始搜索

blockhead数据结构，如果发现某个blockhead满足：

a.  allocated = 0  //表示未分配 

b.  size > sizeA， 

则找到了合适的blockhead，于是进行如下操作： 

a．allocated设为1 

b．如果size – sizeA > sizeof(blockhead)，则将剩下的内存组织成

一个新的blockhead，放入链表中 

c．返回分配的内存的首地址 

分析分析动态分配内存函数mmalloc函数：

void *mmalloc(unsigned long size) 

{

 blockhead *blockptr = gHeapBase; //将heap的链表头地址赋值，从头开始查找

 blockhead *newblock;

 Bool compacted = FALSE;

 size = (size+7)&~7; /* unsigned long align the size */

 DPRINTK("malloc(): size = 0x%08lx/n", size);

 while (blockptr != NULL) {

  if (blockptr->allocated == FALSE) { //判断是否被分配出去

   if (blockptr->size >= size) { //判断这个结构控制的区域够不够分配的大小

    blockptr->allocated=TRUE; //将这个区域定义为已经分出去的标志

    if ((blockptr->size - size) > sizeof(blockhead)) { //如果该区域除开分配的内存大小，余下的比结构体大小还要大，就重新创建一个新的blockhead结构体链接到该结构体后面

     newblock = (blockhead *)((unsigned char *)(blockptr) + sizeof(blockhead) + size);

     newblock->signature = BLOCKHEAD_SIGNATURE;

     newblock->prev = blockptr;

     newblock->next = blockptr->next; //链表操作

     newblock->size = blockptr->size - size - sizeof(blockhead);//这时候新建的结构体可分配的大小就是余下的内存减去新建结构体大小

     newblock->allocated = FALSE;

     blockptr->next = newblock;

     blockptr->size = size;

    } else {

    }

    break;

   } else {

    if ((blockptr->next == NULL) && (compacted == FALSE)) {

     if (compact_heap()) {

      compacted=TRUE;

      blockptr = gHeapBase;

      continue;

     }

    }

   }

  }

  blockptr = blockptr->next;

 }

 DPRINTK("malloc(): returning blockptr = 0x%08lx/n", blockptr);

 if (blockptr == NULL)

  printk("Error: malloc(), out of storage. size = 0x%x/n", size);

 return (blockptr != NULL) ? ((unsigned char *)(blockptr)+sizeof(blockhead)) : NULL;

}

再来看看动态内存的释放mfree函数：

void mfree(void *block) {

 blockhead *blockptr;

 if (block == NULL) return;

 blockptr = (blockhead *)((unsigned char *)(block) - sizeof(blockhead));

 if (blockptr->signature != BLOCKHEAD_SIGNATURE) return;

 blockptr->allocated=FALSE;//主要就是这句，将该区域的分配标志置为没分出去，就表示回收啦

 return;

}

释放内存的操作更简单，直接将要释放的内存对应的blockhead数据结构的allocated设为0即可。

static blockhead *gHeapBase = NULL; //分配堆空间的起始地址结构

static inline int mmalloc_init(unsigned char *heap, unsigned long size)

{

 if (gHeapBase != NULL) return -1;

 DPRINTK("malloc_init(): initialize heap area at 0x%08lx, size = 0x%08lx/n", heap, size);//#define DPRINTK(args...) printk(##args)

 gHeapBase = (blockhead *)(heap); //在堆的分配起始地址处定义一个blockhead的结构体

 gHeapBase->allocated=FALSE;

 gHeapBase->signature=BLOCKHEAD_SIGNATURE;

 gHeapBase->next=NULL;

 gHeapBase->prev=NULL;

 gHeapBase->size = size - sizeof(blockhead);

 return 0;

}

分配heap区域后，内存划分情况如下：

0x34000000-0x33f00000这1MB的空间存放的是vivi的stage1和stage2,0x33f00000-0x33e00000这1MB的空间就是堆空间。

    下面首先进行第一个重点分析。关于**调试手段**，在分析ARM的基本调试手段时也提到过，使用串口打印调试信息是一个非常有效且常用的手段，vivi中采取的也是这种方式。当然，如果你只是实现最为简单的打印字符串等，那么初始化串口后，封装一个基本的输出函数就可以了。但是，这个基本函数的功能是非常有限的。我们在Linux用的printk则要强大好用的多。vivi的思想就是把Linux
 kernel的printk拿过来，稍微裁减一下（因为vivi不需要打印级别，但是需要打印手段的多样化）。这样，自己的工作量并不大，但是调试手段则要完善得多了。在这里，关于printk的代码细节不作为重点，vivi也只是借用了Linux kernel的printk的实现，并做了简单的修改，把console映射到了串口0上。



    手头上暂时有linux-2.4.18的内核，暂时以这个为依据来探讨vivi中printk的实现。



    ·复制【lib/vsprintf.c】到vivi的lib目录下，更改名称为printk.c。然后只保留vsnprintf，及其用到的number函数、skip_atoi函数。skip_atoi中用到了isdigit，所以把【include/linux/ctype.h】复制到vivi的include目录下。另外，还要用到do_div和strnlen两个函数。其中do_div是宏，在【include/asm-arm/div64.h】中实现，直接复制到vivi的include文件夹中。strnlen应该在string.c中实现，可以从【lib/string.c】复制然后添加到vivi的lib下的string.c文件中，最后把声明加到include下的vivi_string.h中。这样，printk需要的基础部分就具备了。

    ·复制【kernel/printk.c】，然后把printk的实现部分摘出来，去掉打印等级等功能，参考vivi的就可以封装起来了。



    可见，vivi中的printk只是把Linux kernel中的代码拿过来，做了及其少量的修改。我现在已经重现了这个过程，并且对整个vivi工程文件做出一些修改，编译下载，测试功能稳定。



    实现了printk，往往需要封装一个调试宏。在【lib/heap.c】中和其他一些文件中，调试宏都是这样的形式：


|```#ifdef DEBUG_HEAP#define DPRINTK(args...)    printk(##args)#else#define DPRINTK(args...)#endif```|
|----|




    分析这样是不妥的。原因就在于stdarg.h（注意，这里只需要定义头文件<stdarg.h>就可以把变长参数表的功能引入了。该头文件的实现因为不同的机器而不同，但是提供的接口是一致的。具体可以看《The C Programming Language》。但是一个细节就是你如果find，会找不到stdarg.h这个头文件，原因就是gcc直接把stdarg.h放到编译器里。）规定的**变长参数表必须至少包括一个有名参数，va_start会将最后一个有名参数作为起点。**这里封装的printk缺少了有名参数，这里可以做测试。测试工程如下：

通过这个测试手段，发现如果选择方式**#define DPRINTK(fmt, args...)   printk(fmt, ##args)，**那么结果如下：



|```[armlinux@lqm printk_test]$ makegcc -Wall -g-O2-c-o printk.o printk.cgcc -Wall -g-O2-c-otest.otest.cgcc -Wall -g-O2 printk.otest.o-otest[armlinux@lqm printk_test]$ lsMakefile printk.c printk.h printk.otesttest.ctest.o[armlinux@lqm printk_test]$./testtest: i = 5, j = 10```|
|----|




    如果选择**#define DPRINTK(args...)        printk(##args)，**那么结果如下：



|```[armlinux@lqm printk_test]$ makegcc -Wall -g-O2-c-o printk.o printk.cgcc -Wall -g-O2-c-otest.otest.ctest.c:23:47: warning: pasting"("and""test: i=%d, j=%d/n"" doesnot give a valid preprocessing tokengcc -Wall -g-O2 printk.otest.o-otest[armlinux@lqm printk_test]$ lsMakefile printk.c printk.h printk.otesttest.ctest.o[armlinux@lqm printk_test]$./testtest: i = 5, j = 10```|
|----|




    可见第二种方式是不合适的。于是修改如下：


|```#ifdef DEBUG_HEAP#define DPRINTK(fmt, args...) printk(fmt,##args)#else#define DPRINTK(fmt, args...)#endif```|
|----|



    这样的调试宏就没有问题了。也算是宏的一个小技巧吧，在Linux内核中查看，可以看到不少的printk的宏封装都是这样的。




