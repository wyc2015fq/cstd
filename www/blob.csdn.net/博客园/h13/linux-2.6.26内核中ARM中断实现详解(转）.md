# linux-2.6.26内核中ARM中断实现详解(转） - h13 - 博客园
**作者：**刘洪涛，[华清远见嵌入式学院](http://www.embedu.org/)金牌讲师，ARM ATC授权培训讲师。
看了一些网络上关于linux中断实现的文章，感觉有一些写的非常好，在这里首先感谢他们的无私付出，然后也想再补充自己对一些问题的理解。先从函数注册引出问题吧。
**一、中断注册方法**
在linux内核中用于申请中断的函数是request_irq（），函数原型在Kernel/irq/manage.c中定义：
int request_irq(unsigned int irq, irq_handler_t handler,
                         unsigned long irqflags, const char *devname, void *dev_id)
**irq**是要申请的硬件中断号。
**handler**是向系统注册的中断处理函数，是一个回调函数，中断发生时，系统调用这个函数，dev_id参数将被传递给它。
**irqflags**是中断处理的属性，若设置了IRQF_DISABLED （老版本中的SA_INTERRUPT，本版zhon已经不支持了），则表示中断处理程序是快速处理程序，快速处理程序被调用时屏蔽所有中断，慢速处理程序不屏蔽；若设置了IRQF_SHARED （老版本中的SA_SHIRQ），则表示多个设备共享中断，若设置了IRQF_SAMPLE_RANDOM（老版本中的SA_SAMPLE_RANDOM），表示对系统熵有贡献，对系统获取随机数有好处。（这几个flag是可以通过或的方式同时使用的）
**dev_id**在中断共享时会用到，一般设置为这个设备的设备结构体或者NULL。
**devname**设置中断名称，在cat /proc/interrupts中可以看到此名称。
request_irq()返回0表示成功，返回-INVAL表示中断号无效或处理函数指针为NULL，返回-EBUSY表示中断已经被占用且不能共享。
关于中断注册的例子，大家可在内核中搜索下request_irq。
在编写驱动的过程中，比较容易产生疑惑的地方是：
1、中断向量表在什么位置？是如何建立的？
        2、从中断开始，系统是怎样执行到我自己注册的函数的？
        3、中断号是如何确定的？对于硬件上有子中断的中断号如何确定？
        4、中断共享是怎么回事，dev_id的作用是？
本文以2.6.26内核和S3C2410处理器为例，为大家讲解这几个问题。
**二、异常向量表的建立**
在ARM V4及V4T以后的大部分处理器中，中断向量表的位置可以有两个位置：一个是0，另一个是0xffff0000。可以通过CP15协处理器c1寄存器中V位(bit[13])控制。V和中断向量表的对应关系如下：
V=0        ～        0x00000000~0x0000001C
        V=1        ～        0xffff0000~0xffff001C
arch/arm/mm/proc-arm920.S中
.section ".text.init", #alloc, #execinstr
        __arm920_setup:
        …… orr       r0, r0, #0x2100              @ ..1. ...1 ..11 ...1
//bit13=1 中断向量表基址为0xFFFF0000。R0的值将被付给CP15的C1.
在linux中，向量表建立的函数为：
init/main.c->start_kernel()->trap_init()
void __init trap_init(void)
        { 
                 unsigned long vectors = CONFIG_VECTORS_BASE;
                 …… 
                 memcpy((void *)vectors, __vectors_start, __vectors_end - __vectors_start);
                memcpy((void *)vectors + 0x200, __stubs_start, __stubs_end - __stubs_start);
                 ....
        }
在2.6.26内核中CONFIG_VECTORS_BASE最初是在各个平台的配置文件中设定的，如：
arch/arm/configs/s3c2410_defconfig中
CONFIG_VECTORS_BASE=0xffff0000
__vectors_end 至 __vectors_start之间为异常向量表。
位于arch/arm/kernel/entry-armv.S
.globl __vectors_start
__vectors_start:
        swi SYS_ERROR0:
        b vector_und + stubs_offset //复位异常:
        ldr pc, .LCvswi + stubs_offset        //未定义指令异常:
        b vector_pabt + stubs_offset        //软件中断异常:
        b vector_dabt + stubs_offset        //数据异常:
        b vector_addrexcptn + stubs_offset        //保留:
        b vector_irq + stubs_offset        //普通中断异常:
        b vector_fiq + stubs_offset        //快速中断异常:
        .globl __vectors_end:
__vectors_end:
__stubs_end 至 __stubs_start之间是异常处理的位置。也位于文件arch/arm/kernel/entry-armv.S中。vector_und、vector_pabt、vector_irq、vector_fiq都在它们中间。
stubs_offset值如下：
.equ stubs_offset, __vectors_start + 0x200 - __stubs_start
stubs_offset是如何确定的呢？（引用网络上的一段比较详细的解释）
当汇编器看到B指令后会把要跳转的标签转化为相对于当前PC的偏移量（±32M）写入指令码。从上面的代码可以看到中断向量表和stubs都发生了代码搬移，所以如果中断向量表中仍然写成b vector_irq，那么实际执行的时候就无法跳转到搬移后的vector_irq处，因为指令码里写的是原来的偏移量，所以需要把指令码中的偏移量写成搬移后的。我们把搬移前的中断向量表中的irq入口地址记irq_PC,它在中断向量表的偏移量就是irq_PC-vectors_start, vector_irq在stubs中的偏移量是vector_irq-stubs_start，这两个偏移量在搬移前后是不变的。搬移后 vectors_start在0xffff0000处，而stubs_start在0xffff0200处，所以搬移后的vector_irq相对于中断 向量中的中断入口地址的偏移量就是，200+vector_irq在stubs中的偏移量再减去中断入口在向量表中的偏移量，即200+ vector_irq-stubs_start-irq_PC+vectors_start = (vector_irq-irq_PC) + vectors_start+200-stubs_start,对于括号内的值实际上就是中断向量表中写的vector_irq，减去irq_PC是由汇编器完成的，而后面的 vectors_start+200-stubs_start就应该是stubs_offset，实际上在entry-armv.S中也是这样定义的。
**linux-2.6.26****内核中ARM中断实现详解（2）**
**作者：**刘洪涛，[华清远见嵌入式学院](http://www.embedu.org/)金牌讲师，ARM公司ATC授权培训讲师。
**三、中断处理过程 **
这一节将以S3C2410为例，描述linux-2.6.26内核中，从中断开始，中断是如何一步一步执行到我们注册函数的。
3.1 中断向量表 arch\arm\kernel\entry-armv.S
__vectors_start:
             swi SYS_ERROR0
             b    vector_und + stubs_offset
             ldr pc, .LCvswi + stubs_offset
             b    vector_pabt + stubs_offset
             b    vector_dabt + stubs_offset
             b    vector_addrexcptn + stubs_offset
**             b    vector_irq + stubs_offset **
             b    vector_fiq + stubs_offset
             .globl   __vectors_end
        __vectors_end:
中断发生后，跳转到***b vector_irq + stubs_offset***的位置执行。注意现在的向量表的初始位置是0xffff0000。
3.2 中断跳转的入口位置 arch\arm\kernel\entry-armv.S
      .globl   __stubs_start
    __stubs_start:
     /*
     * Interrupt dispatcher
     */
**             vector_stub    irq, IRQ_MODE, 4 **@**IRQ_MODE****在include\asm\ptrace.h中定义：0x12**
             .long    __irq_usr @ 0 (USR_26 / USR_32)
             .long    __irq_invalid @ 1 (FIQ_26 / FIQ_32)
             .long    __irq_invalid @ 2 (IRQ_26 / IRQ_32)
             .long    __irq_svc @ 3 (SVC_26 / SVC_32)
             .long    __irq_invalid @ 4
             .long    __irq_invalid @ 5
             .long    __irq_invalid @ 6
             .long    __irq_invalid @ 7
             .long    __irq_invalid @ 8
             .long    __irq_invalid @ 9
             .long __irq_invalid @ a
             .long __irq_invalid @ b
             .long __irq_invalid @ c
             .long __irq_invalid @ d
             .long __irq_invalid @ e
             .long __irq_invalid @ f
上面代码中vector_stub宏的定义为：
.macro vector_stub, name, mode, correction=0
             .align 5
**        vector_\name:**
             .if \correction
             sub lr, lr, #\correction
             .endif
             @
             @ Save r0, lr_<exception> (parent PC) and spsr_<exception>
             @ (parent CPSR)
             @
             stmia sp, {r0, lr} @ save r0, lr
             mrs lr, spsr
             str lr, [sp, #8] @ save spsr
             @
             @ Prepare for SVC32 mode. IRQs remain disabled.
             @
             mrs r0, cpsr
             eor r0, r0, #(\mode ^ SVC_MODE)
             msr spsr_cxsf, r0 @为后面进入svc模式做准备
@
             @ the branch table must immediately follow this code
              @
             and lr, lr, #0x0f @进入中断前的mode的后4位
             @#define USR_MODE 0x00000010
             @#define FIQ_MODE 0x00000011
             @#define IRQ_MODE 0x00000012
             @#define SVC_MODE 0x00000013
             @#define ABT_MODE 0x00000017
             @#define UND_MODE 0x0000001b
             @#define SYSTEM_MODE 0x0000001f
             mov r0, sp
             ldr lr, [pc, lr, lsl #2] @如果进入中断前是usr，则取出PC+4*0的内容，即__irq_usr @如果进入中断前是svc，则取出PC+4*3的内容，即__irq_svc
             movs pc, lr @ 当指令的目标寄存器是PC，且指令以S结束，则它会把@ spsr的值恢复给cpsr branch to handler in SVC mode
             .endm
             .globl __stubs_start
       __stubs_start:
       /*
       * Interrupt dispatcher
       */
             vector_stub irq, IRQ_MODE, 4
             .long __irq_usr @ 0 (USR_26 / USR_32)
             .long __irq_invalid @ 1 (FIQ_26 / FIQ_32)
             .long __irq_invalid @ 2 (IRQ_26 / IRQ_32)
             .long __irq_svc @ 3 (SVC_26 / SVC_32)
用“irq, IRQ_MODE, 4”代替宏vector_stub中的“name, mode, correction”，找到了我们中断处理的入口位置为vector_irq（宏里面的**vector_\name**）。
从上面代码中的注释可以看出，根据进入中断前的工作模式不同，程序下一步将跳转到_irq_usr 、或__irq_svc等位置。我们先选择__irq_usr作为下一步跟踪的目标。
**3.3 __irq_usr****的实现 arch\arm\kernel\entry-armv.S**
**__irq_usr:             usr_entry @****后面有解释**kuser_cmpxchg_check
       #ifdef CONFIG_TRACE_IRQFLAGS
bl trace_hardirqs_off
       #endif
get_thread_info tsk @获取当前进程的进程描述符中的成员变量thread_info的地址，并将该地址保存到寄存器tsk等于r9（在entry-header.S中定义）
       #ifdef CONFIG_PREEMPT//如果定义了抢占，增加抢占数值
             ldr r8, [tsk, #TI_PREEMPT] @ get preempt count
             add r7, r8, #1 @ increment it
             str r7, [tsk, #TI_PREEMPT]
       #endif
**irq_handler @****中断处理，我们最关心的地方，3.4节有实现过程。 **
      #ifdef CONFIG_PREEMPT
             ldr r0, [tsk, #TI_PREEMPT]
             str r8, [tsk, #TI_PREEMPT]
             teq r0, r7
             strne r0, [r0, -r0]
      #endif
      #ifdef CONFIG_TRACE_IRQFLAGS
             bl trace_hardirqs_on
       #endif
mov why, #0
b **ret_to_user @****中断处理完成，返回中断产生的位置，3.7节有实现过程**
上面代码中的**usr_entry**是一个宏，主要实现了将usr模式下的寄存器、中断返回地址保存到堆栈中。
.macro **usr_entry**
       sub sp, sp, #S_FRAME_SIZE @ S_FRAME_SIZE的值在arch\arm\kernel\asm-offsets.c
       @ 中定义 DEFINE(S_FRAME_SIZE, sizeof(struct pt_regs));实际上等于72
     stmib sp, {r1 - r12}
             ldmia r0, {r1 - r3}
             add r0, sp, #S_PC @ here for interlock avoidance
             mov r4, #-1 @ "" "" "" ""
             str r1, [sp] @ save the "real" r0 copied
             @ from the exception stack
     @
             @ We are now ready to fill in the remaining blanks on the stack:
             @
             @ r2 - lr_<exception>, already fixed up for correct return/restart
             @ r3 - spsr_<exception>
             @ r4 - orig_r0 (see pt_regs definition in ptrace.h)
             @
             @ Also, separately save sp_usr and lr_usr
             @
             stmia r0, {r2 - r4}
             stmdb r0, {sp, lr}^
      @
             @ Enable the alignment trap while in kernel mode
             @
alignment_trap r0
     @
             @ Clear FP to mark the first stack frame
             @
             zero_fp
             .endm
上面的这段代码主要在填充结构体pt_regs ，这里提到的struct pt_regs，在include/asm/ptrace.h中定义。此时sp指向struct pt_regs。
*      struct pt_regs {*
*                          long uregs[18];*
*             };*
*       #define ARM_cpsr uregs[16]*
*       #define ARM_pc uregs[15]*
*       #define ARM_lr uregs[14]*
*       #define ARM_sp uregs[13]*
*       #define ARM_ip uregs[12]*
*       #define ARM_fp uregs[11]*
*       #define ARM_r10 uregs[10]*
*       #define ARM_r9 uregs[9]*
*       #define ARM_r8 uregs[8]*
*       #define ARM_r7 uregs[7]*
*       #define ARM_r6 uregs[6]*
*       #define ARM_r5 uregs[5]*
*       #define ARM_r4 uregs[4]*
*       #define ARM_r3 uregs[3]*
*       #define ARM_r2 uregs[2]*
*       #define ARM_r1 uregs[1]*
*       #define ARM_r0 uregs[0]*
*       #define ARM_ORIG_r0 uregs[17]*
**3.4 irq_handler****的实现过程**，**arch\arm\kernel\entry-armv.S**
  .macro **irq_handler**
              get_irqnr_preamble r5, lr
              @在include/asm/arch-s3c2410/entry-macro.s中定义了宏get_irqnr_preamble为空操作，什么都不做
              1: get_irqnr_and_base r0, r6, r5, lr @判断中断号，通过R0返回，3.5节有实现过程
              movne r1, sp
              @
              @ routine called with r0 = irq number, r1 = struct pt_regs *
              @
              adrne lr, 1b
**bne asm_do_IRQ @****进入中断处理。 **
       …… 
              .endm
**3.5 get_irqnr_and_base****中断号判断过程**，include/asm/arch-s3c2410/entry-macro.s
.macro get_irqnr_and_base, irqnr, irqstat, base, tmp
              mov \base, #S3C24XX_VA_IRQ
              @@ try the interrupt offset register, since it is there
              ldr \irqstat, [ \base, #INTPND ]
              teq \irqstat, #0
              beq 1002f
              ldr \irqnr, [ \base, #INTOFFSET ]** @****通过判断INTOFFSET寄存器得到中断位置**
              mov \tmp, #1
              tst \irqstat, \tmp, lsl \irqnr
              bne 1001f
              @@ the number specified is not a valid irq, so try
              @@ and work it out for ourselves
              mov \irqnr, #0 @@ start here
              @@ work out which irq (if any) we got
              movs \tmp, \irqstat, lsl#16
              addeq \irqnr, \irqnr, #16
              moveq \irqstat, \irqstat, lsr#16
              tst \irqstat, #0xff
              addeq \irqnr, \irqnr, #8
              moveq \irqstat, \irqstat, lsr#8
              tst \irqstat, #0xf
              addeq \irqnr, \irqnr, #4
              moveq \irqstat, \irqstat, lsr#4
              tst \irqstat, #0x3
              addeq \irqnr, \irqnr, #2
              moveq \irqstat, \irqstat, lsr#2
              tst \irqstat, #0x1
              addeq \irqnr, \irqnr, #1
              @@ we have the value
      1001:
       adds \irqnr, \irqnr, #IRQ_EINT0 @**加上中断号的基准数值，得到最终的中断号，注意：此时没有考虑子中断的具体情况，（子中断的问题后面会有讲解）。IRQ_EINT0在include/asm/arch-s3c2410/irqs.h中定义.从这里可以看出，中断号的具体值是有平台相关的代码决定的，和硬件中断挂起寄存器中的中断号是不等的。**
1002:
              @@ exit here, Z flag unset if IRQ
          .endm
3.6 **asm_do_IRQ**实现过程，arch/arm/kernel/irq.c
asmlinkage void __exception **asm_do_IRQ(**unsigned int irq, struct pt_regs *regs)
        {
              struct pt_regs *old_regs = set_irq_regs(regs);
**              struct irq_desc *desc = irq_desc + irq;//****根据中断号找到对应的irq_desc**
              /*
              * Some hardware gives randomly wrong interrupts. Rather
              * than crashing, do something sensible.
              */
              if (irq >= NR_IRQS)
              desc = &bad_irq_desc;
              irq_enter();//没做什么特别的工作，可以跳过不看
**              desc_handle_irq(irq, desc);//** 根据中断号和desc进入中断处理
              /* AT91 specific workaround */
              irq_finish(irq);
              irq_exit();
              set_irq_regs(old_regs);
       }
static inline void **desc_handle_irq**(unsigned int irq, struct irq_desc *desc)
         {
**                     desc->handle_irq(irq, desc);//****中断处理 **
          }
上述asmlinkage void __exception asm_do_IRQ(unsigned int irq, struct pt_regs *regs)使用了asmlinkage标识。那么这个标识的含义如何理解呢？
该符号定义在kernel/include/linux/linkage.h中，如下所示：
#include <asm/linkage.h>//各个具体处理器在此文件中定义asmlinkage
        #ifdef __cplusplus
        #define CPP_ASMLINKAGE extern "C"
        #else
        #define CPP_ASMLINKAGE
        #endif
#ifndef asmlinkage//如果以前没有定义asmlinkage
        #define asmlinkage CPP_ASMLINKAGE
        #endif
对于ARM处理器的<asm/linkage.h>，没有定义asmlinkage，所以没有意义（不要以为参数是从堆栈传递的，对于ARM平台来说还是符合ATPCS过程调用标准，通过寄存器传递的）。
但对于X86处理器的<asm/linkage.h>中是这样定义的：
#define asmlinkage CPP_ASMLINKAGE __attribute__((regparm(0)))
表示函数的参数传递是通过堆栈完成的。
3.7 描述3.3节中的**ret_to_user **中断返回过程，/arch/arm/kernel/entry-common.S
ENTRY(ret_to_user)
        ret_slow_syscall:
              disable_irq @ disable interrupts
              ldr r1, [tsk, #TI_FLAGS]
              tst r1, #_TIF_WORK_MASK
              bne work_pending
        no_work_pending:
              /* perform architecture specific actions before user return */
              arch_ret_to_user r1, lr
      @ slow_restore_user_regs
              ldr r1, [sp, #S_PSR] @ get calling cpsr
              ldr lr, [sp, #S_PC]! @ get pc
              msr spsr_cxsf, r1 @ save in spsr_svc
              ldmdb sp, {r0 - lr}^ @ get calling r0 - lr
              mov r0, r0
              add sp, sp, #S_FRAME_SIZE - S_PC
              movs pc, lr @ return & move spsr_svc into cpsr
第三章主要跟踪了从中断发生到调用到对应中断号的**desc->handle_irq(irq, desc)**中断函数的过程。后面的章节还会继续讲解后面的内容。
**linux-2.6.26内核中ARM中断实现详解（3）**
**作者：**刘洪涛，[华清远见嵌入式学院](http://www.embedu.org/)金牌讲师。
四、中断处理模型
要想弄清楚desc->handle_irq(irq, desc)和我们注册的中断有什么关联，就要了解中断处理模型了。
4.1 中断处理模型结构
中断处理模型如下图所示，
其中NR_IRQS表示最大的中断号，在include/asm/arch/irq.h中定义。
irq_desc［］是一个指向irq_desc_t结构的数组， irq_desc_t结构是各个设备中断服务例程的描述符。Irq_desc_t结构体中的成员action指向该中断号对应的irqaction结构体链表。Irqaction结构体定义在include/linux/interrupt.h中，如下：
truct irqaction {
irq_handler_t handler; //中断处理函数，注册时提供
unsigned long flags; //中断标志，注册时提供
cpumask_t mask; //中断掩码
const char *name; //中断名称
void *dev_id; //设备id，本文后面部分介绍中断共享时会详细说明这个参数的作用
struct irqaction *next; //如果有中断共享，则继续执行，
int irq; //中断号，注册时提供
struct proc_dir_entry *dir; //指向IRQn相关的/proc/irq/n目录的描述符
};
在注册中断号为irq的中断服务程序时，系统会根据注册参数封装相应的irqaction结构体。并把中断号为irq的irqaction结构体写入irq_desc [irq]->action。这样就把设备的中断请求号与该设备的中断服务例程irqaction联系在一起了。样当CPU接收到中断请求后，就可以根据中断号通过irq_desc []找到该设备的中断服务程序。
4.2 中断共享的处理模型
共享中断的不同设备的iqraction结构体都会添加进该中断号对应的irq_desc结构体的action成员所指向的irqaction链表内。当内核发生中断时，它会依次调用该链表内所有的handler函数。因此，若驱动程序需要使用共享中断机制，其中断处理函数必须有能力识别是否是自己的硬件产生了中断。通常是通过读取该硬件设备提供的中断flag标志位进行判断。也就是说不是任何设备都可以做为中断共享源的，它必须能够通过的它的中断flag判断出是否发生了中断。
中断共享的注册方法是：
int request_irq(unsigned int irq, irq_handler_t handler,
**IRQF_SHARED**, const char *devname, void *dev_id)
很多权威资料中都提到，中断共享注册时的注册函数中的dev_id参数是必不可少的，并且dev_id的值必须唯一。那么这里提供唯一的dev_id值的究竟是做什么用的？
根据我们前面中断模型的知识，可以看出发生中断时，内核并不判断究竟是共享中断线上的哪个设备产生了中断，它会循环执行所有该中断线上注册的中断处理函数（即irqaction->handler函数）。因此irqaction->handler函数有责任识别出是否是自己的硬件设备产生了中断，然后再执行该中断处理函数。通常是通过读取该硬件设备提供的中断flag标志位进行判断。那既然kernel循环执行该中断线上注册的所有irqaction->handler函数，把识别究竟是哪个硬件设备产生了中断这件事交给中断处理函数本身去做，那request_irq的dev_id参数究竟是做什么用的？
很多资料中都建议将设备结构指针作为dev_id参数。在中断到来时，迅速地根据硬件寄存器中的信息比照传入的dev_id参数判断是否是本设备的中断，若不是，应迅速返回。这样的说法没有问题，也是我们编程时都遵循的方法。但事实上并不能够说明为什么中断共享必须要设置dev_id。
下面解释一下dev_id参数为什么必须的，而且是必须唯一的。
当调用free_irq注销中断处理函数时（通常卸载驱动时其中断处理函数也会被注销掉），因为dev_id是唯一的，所以可以通过它来判断从共享中断线上的多个中断处理程序中删除指定的一个。如果没有这个参数，那么kernel不可能知道给定的中断线上到底要删除哪一个处理程序。
注销函数定义在Kernel/irq/manage.c中定义：
void free_irq(unsigned int irq, void *dev_id)
五、S3C2410子中断的注册的实现
5.1 S3C2410子中断注册问题的提出
参看3.5节中判断中断号的方法，可以看到只是通过S3C2410中断控制器中的INTOFFSET寄存器来判断的。对于**INTPND**中的EINT4_7、EINT8_23、INT_UART0、INT_ADC 等带有子中断的向量，INTOFFSET无法判断出具体的中断号。平台留给我们的注册方法如下：
在include/asm/arch/irqs.h中有类似如下定义：
/* interrupts generated from the external interrupts sources */
#define IRQ_EINT4 S3C2410_IRQ(32) /* 48 */
#define IRQ_EINT5 S3C2410_IRQ(33)
#define IRQ_EINT6 S3C2410_IRQ(34)
#define IRQ_EINT7 S3C2410_IRQ(35)
#define IRQ_EINT8 S3C2410_IRQ(36)
#define IRQ_EINT9 S3C2410_IRQ(37)
#define IRQ_EINT10 S3C2410_IRQ(38)
#define IRQ_EINT11 S3C2410_IRQ(39)
#define IRQ_EINT12 S3C2410_IRQ(40)
#define IRQ_EINT13 S3C2410_IRQ(41)
#define IRQ_EINT14 S3C2410_IRQ(42)
#define IRQ_EINT15 S3C2410_IRQ(43)
#define IRQ_EINT16 S3C2410_IRQ(44)
#define IRQ_EINT17 S3C2410_IRQ(45)
#define IRQ_EINT18 S3C2410_IRQ(46)
#define IRQ_EINT19 S3C2410_IRQ(47)
#define IRQ_EINT20 S3C2410_IRQ(48) /* 64 */
#define IRQ_EINT21 S3C2410_IRQ(49)
#define IRQ_EINT22 S3C2410_IRQ(50)
#define IRQ_EINT23 S3C2410_IRQ(51)
可以看到平台为每种子中断都定义了中断号，如果你想实现EINT10的中断注册，直接按照IRQ_EINT10这个中断号注册都可以了。那么平台代码是如何实现这部分中断注册的呢？
5.2 S3C2410子中断注册问题的解决
/*arch/arm/plat-s3c24xx/irq.c*/
void __init s3c24xx_init_irq(void)
{……
set_irq_chained_handler(IRQ_EINT4t7, s3c_irq_demux_extint4t7);
set_irq_chained_handler(IRQ_EINT8t23, s3c_irq_demux_extint8);
set_irq_chained_handler(IRQ_UART0, s3c_irq_demux_uart0);
set_irq_chained_handler(IRQ_UART1, s3c_irq_demux_uart1);
set_irq_chained_handler(IRQ_UART2, s3c_irq_demux_uart2);
set_irq_chained_handler(IRQ_ADCPARENT, s3c_irq_demux_adc);
……
}
平台在初始化时会调用到s3c24xx_init_irq，在此函数中实现了对EINT4_7、EINT8_23、INT_UART0、INT_ADC等中断的注册。下面看看这些带有子中断的中断号对应的处理函数的内容。以IRQ_EINT4t7为例，其它情况类似。
/*arch/arm/plat-s3c24xx/irq.c*/
s3c_irq_demux_extint4t7(unsigned int irq,
struct irq_desc *desc)
{
unsigned long eintpnd = __raw_readl(S3C24XX_EINTPEND);
unsigned long eintmsk = __raw_readl(S3C24XX_EINTMASK);
eintpnd &= ~eintmsk;
eintpnd &= 0xff; /* only lower irqs */
/* eintpnd中可以有多个位同时置1，这一点和intpnd的只能有1个位置1是不一样的 */
while (eintpnd) { //循环执行所有置位的子中断
irq = __ffs(eintpnd); //算出第一个不为0的位，类似arm v5后的clz前导0的作用
eintpnd &= ~(1<<irq);//清除相应的位
irq += (IRQ_EINT4 - 4);//算出对应的中断号
desc_handle_irq(irq, irq_desc + irq);//执行对应子中断的注册函数
}
}
从上面的函数可以看出子中断是如何注册及被调用到的。有人可能会问为何不在include/asm/arch-s3c2410/entry-macro.s 文件中get_irqnr_and_base函数判断中断号时，直接算出对应的子中断号，就可以直接找到子中断处理了呢？
原因是: get_irqnr_and_base是平台给系统提供的函数，对于多个子中断同时置位的情况无法通过一个值返回（因为子中断中，如eintpnd是可以多个位同时置位的））。而intpnd则没有这个问题。

