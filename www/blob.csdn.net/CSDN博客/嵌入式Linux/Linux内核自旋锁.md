
# Linux内核自旋锁 - 嵌入式Linux - CSDN博客

2018年10月08日 09:51:00[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：18



# 自旋锁
自旋锁（spinlock）是用在多个CPU系统中的锁机制，当一个CPU正访问自旋锁保护的临界区时，临界区将被锁上，其他需要访问此临界区的CPU只能忙等待，直到前面的CPU已访问完临界区，将临界区开锁。自旋锁上锁后让等待线程进行忙等待而不是睡眠阻塞，而信号量是让等待线程睡眠阻塞。自旋锁的忙等待浪费了处理器的时间，但时间通常很短，在1毫秒以下。
自旋锁用于多个CPU系统中，在单处理器系统中，自旋锁不起锁的作用，只是禁止或启用内核抢占。在自旋锁忙等待期间，内核抢占机制还是有效的，等待自旋锁释放的线程可能被更高优先级的线程抢占CPU。
自旋锁基于共享变量。一个线程通过给共享变量设置一个值来获取锁，其他等待线程查询共享变量是否为0来确定锁现是否可用，然后在忙等待的循环中"自旋"直到锁可用为止。
## 通用自旋锁
自旋锁的状态值为1表示解锁状态，说明有1个资源可用；0或负值表示加锁状态，0说明可用资源数为0。Linux内核为通用自旋锁提供了API函数初始化、测试和设置自旋锁。API函数功能说明如表5。
表5 通用自旋锁API函数功能说明|宏定义|功能说明|
|spin_lock_init(lock)|初始化自旋锁，将自旋锁设置为1，表示有一个资源可用。|
|spin_is_locked(lock)|如果自旋锁被置为1（未锁），返回0，否则返回1。|
|spin_unlock_wait(lock)|等待直到自旋锁解锁（为1），返回0；否则返回1。|
|spin_trylock(lock)|尝试锁上自旋锁（置0），如果原来锁的值为1，返回1，否则返回0。|
|spin_lock(lock)|循环等待直到自旋锁解锁（置为1），然后，将自旋锁锁上（置为0）。|
|spin_unlock(lock)|将自旋锁解锁（置为1）。|
|spin_lock_irqsave(lock, flags)|循环等待直到自旋锁解锁（置为1），然后，将自旋锁锁上（置为0）。关中断，将状态寄存器值存入flags。|
|spin_unlock_irqrestore(lock, flags)|将自旋锁解锁（置为1）。开中断，将状态寄存器值从flags存入状态寄存器。|
|spin_lock_irq(lock)|循环等待直到自旋锁解锁（置为1），然后，将自旋锁锁上（置为0）。关中断。|
|spin_unlock_irq(lock)|将自旋锁解锁（置为1）。开中断。|
|spin_unlock_bh(lock)|将自旋锁解锁（置为1）。开启底半部的执行。|
|spin_lock_bh(lock)|循环等待直到自旋锁解锁（置为1），然后，将自旋锁锁上（置为0）。阻止软中断的底半部的执行。|
下面用一个使用自旋锁锁住链表的样例，代码列出如下（在arch/x386/mm/pgtable.c中）：
spinlock_t pgd_lock= SPIN_LOCK_UNLOCKED;*//锁初始化*void pgd_dtor(void*pgd, kmem_cache_t*cache,unsignedlong unused){unsignedlong flags;*//能从中断上下文中被调用*spin_lock_irqsave(&pgd_lock, flags);*//加锁*pgd_list_del(pgd);
	spin_unlock_irqrestore(&pgd_lock, flags);*//解锁*}

自旋锁用结构spinlock_t描述，在include/linux/spinlock.h中有类型 spinlock_t定义，列出如下：
typedefstruct{
	raw_spinlock_t raw_lock;\#ifdef CONFIG_GENERIC_LOCKBREAK    /*引入另一个自旋锁*/unsignedint break_lock;\#endif\#ifdef CONFIG_DEBUG_SPINLOCK   /*用于调试自旋锁*/unsignedint magic, owner_cpu;void*owner;\#endif\#ifdef CONFIG_DEBUG_LOCK_ALLOCstruct lockdep_map dep_map;*/*映射lock实例到lock-class对象\#endif} spinlock_t;*

由于自旋锁的性能严重地影响着操作系统的性能，Linux内核提供了Lock-class和Lockdep跟踪自旋锁的使用对象和锁的状态，并可从/proc文件系统查询自旋锁的状态信息。自旋锁的调试通过配置项CONFIG_DEBUG_*项打开。
对于对称多处理器系统（SMP），slock为一个int数据类型，对于单个处理器系统，slock定义为空。SMP的slock定义列出如下（在include/linux/spinlock_types.h）：
typedefstruct{volatileunsignedint slock;} raw_spinlock_t;

自旋锁的实现机制类型，下面仅分析自旋锁API函数spin_lock_init、spin_lock_irqsave和spin_unlock_irqrestore。
（1）spin_lock_init
函数spin_lock_init将自旋锁状态值设置为1，表示未锁状态。其列出如下（在include/linux/spinlock.h中）：
\# define spin_lock_init(lock)					/
	do { *(lock) = SPIN_LOCK_UNLOCKED; } while (0)

宏__SPIN_LOCK_UNLOCKED列出如下（在include/linux/spinlock_types.h中）：
\# define __SPIN_LOCK_UNLOCKED(lockname) /
	(spinlock_t)	{	.raw_lock = __RAW_SPIN_LOCK_UNLOCKED,	/
				SPIN_DEP_MAP_INIT(lockname) }\#define __RAW_SPIN_LOCK_UNLOCKED	{ 1 }

（2）函数spin_lock_irqsave
函数spin_lock_irqsave等待直到自旋锁解锁，即自旋锁值为1，它还关闭本地处理器上的中断。其列出如下（在include/linux/spinlock.h中）：
\#define spin_lock_irqsave(lock, flags)	flags = _spin_lock_irqsave(lock)

函数spin_lock_irqsave分析如下（在kernel/spinlock.c中）：
unsignedlong __lockfunc _spin_lock_irqsave(spinlock_t*lock){unsignedlong flags;
 
	local_irq_save(flags);*//将状态寄存器的值写入flags保存*preempt_disable();*//关闭内核抢占，内核抢占锁加1*spin_acquire(&lock->dep_map,0,0, _RET_IP_);\#ifdef CONFIG_LOCKDEP
	LOCK_CONTENDED(lock, _raw_spin_trylock, _raw_spin_lock);\#else
	_raw_spin_lock_flags(lock,&flags);\#endifreturn flags;}

宏定义local_irq_save保存了状态寄存器的内容到x中，同时关中断。这个宏定义列出如下：
\#define local_irq_save(x)	__asm__ __volatile__("pushfl ; popl %0 ; cli":"=g" (x): /* no input */ :"memory")

上述语句中，指令pushfl将当前处理器的状态寄存器的内容压入堆栈保护。指令popl %0 将状态寄存器的内容存入x中，其中%0这里指x。
函数_raw_spin_lock_flags空操作等待直到自旋锁的值为1，表示有资源可用，就跳出循环等待，准备执行本函数后面的操作。其列出如下：
\# define _raw_spin_lock_flags(lock, flags) /
		__raw_spin_lock_flags(&(lock)->raw_lock, *(flags�

函数__raw_spin_lock_flags列出如下（在include/asm-x86/spinlock.h中）：
\#define __raw_spin_lock_flags(lock, flags) __raw_spin_lock(lock)static __always_inlinevoid __raw_spin_lock(raw_spinlock_t*lock){int inc=0x00010000;int tmp;*/*指令前缀lock用来锁住内存控制器，不让其他处理器访问，保证指令执行的原子性*/*asmvolatile("lock ; xaddl %0, %1**/n**"*// lock->slock=lock->slock+inc*"movzwl %w0, %2**/n****/t**"*//tmp=inc*"shrl $16, %0**/n****/t**"*//inc >> 16 后，inc=1*"1:**/t**""cmpl %0, %2**/n****/t**"*//比较inc与lock->slock*"je 2f**/n****/t**"*//如果inc与lock->slock相等，跳转到2*"rep ; nop**/n****/t**"*//空操作*"movzwl %1, %2**/n****/t**"*//tmp=lock->slock**/* 这里不需要读内存屏障指令lfence，因为装载是排序的*/*"jmp 1b**/n**"*//跳转到1*"2:":"+Q"(inc),"+m"(lock->slock),"=r"(tmp)::"memory","cc");}

（3）函数spin_unlock_irqrestore
宏定义spin_unlock_irqrestore是解锁，开中断，并把flags值存入到状态寄存器中，这个宏定义分析如下：
\#define spin_unlock_irqrestore(lock, flags)	_spin_unlock_irqrestore(lock, flags)
函数_spin_unlock_irqrestore列出如下（在kernel/spinlock.c中）：
void __lockfunc _spin_unlock_irqrestore(spinlock_t*lock,unsignedlong flags){
	spin_release(&lock->dep_map,1, _RET_IP_);
	_raw_spin_unlock(lock);*//解锁*local_irq_restore(flags);*//开中断，将flag的值存入状态寄存器*preempt_enable();*//开启内核抢占*}\# define _raw_spin_unlock(lock)		__raw_spin_unlock(&(lock)->raw_lock)

函数__raw_spin_unlock将自旋锁状态值加1，表示有1个资源可用，从而释放自旋锁，其列出如下（在include/asm-x86/spinlock.h中）：
static __always_inlinevoid __raw_spin_unlock(raw_spinlock_t*lock){
	asmvolatile(UNLOCK_LOCK_PREFIX"incw %0"*// lock->slock= lock->slock +1*:"+m"(lock->slock)::"memory","cc");}

## 读/写自旋锁
"读/写自旋锁"用来解决读者/写者问题。如果有多个线程（进程、中断处理程序、底半部例程）以只读的方式访问一个临界区数据，读/写自旋锁允许多个线程同时读取数据。如果一个线程需要对临界区数据进行写操作，它必须获取写锁，只有在没有读者或写者进行操作时，写者才独占临界区数据进行写操作。读操作时需要获取读锁，写操作时需要获取写锁。
Linux内核为读/写自旋锁提供了操作API函数初始化、测试和设置自旋锁。API函数功能说明如表5。
表5 读/写自旋锁API函数功能说明|宏定义|功能说明|
|rwlock_init(lock)|初始化自旋锁值为0x01000000（未锁）。|
|read_lock(lock)|加读者锁，即将读者计数加1。|
|read_lock_irqsave(lock, flags)|加读者锁，即将读者计数加1。并且关中断，存储状态标识到flags中。|
|read_lock_irq(lock)|加读者锁，即将读者计数加1。并且关中断。|
|read_unlock(lock)|解读者锁，即将读者计数减1。|
|read_unlock_irqrestore(lock, flags)|解读者锁，即将读者计数减1。并且开中断，将状态标识从flags读到状态寄存器中。|
|read_unlock_irq(lock)|解读者锁，即将读者计数减1。并且开中断。|
|write_lock(lock)|加写者锁，即将写者锁置0。|
|write_lock_irqrestore(lock, flags)|加写者锁，即将写者锁置0。并且关中断，存储状态标识到flags中。|
|write_lock_irq(lock)|加写者锁，即将写者锁置0。并且关中断。|
|write_unlock(lock)|解写者锁，即将写者锁置1。|
|write_unlock_irqrestore(lock, flags)|解写者锁，即将写者锁置1。并且开中断，将状态标识从flags读到状态寄存器中。|
|write_unlock_irq(lock)|解写者锁，即将写者锁置1。并且开中断。|
用户使用读/写自旋锁，应先自旋锁的状态值初始化为锁初始化为RW_LOCK_BIAS，即0x01000000，表示为未锁状态。
读/写自旋锁用结构rwlock_t描述，它的主要成员为锁状态值变量lock，结构rwlock_t列出如下（在include/linux/spinlock_types.h中）：
typedefstruct{
	raw_rwlock_t raw_lock;
    ……} rwlock_t;typedefstruct{unsignedint lock;} raw_rwlock_t;

在结构raw_rwlock_t中，读/写自旋锁状态变量lock为32位，它分为2个部分，0~23位是一个24位计数器，表示对临界数据区进行并发读操作的线程数，线程数以补码形式存入计数器；第24位为表示"未锁"的状态位，在没有线程读或写临界区时，设置为1，否则，设置为0。
如果自旋锁设置了"未锁"状态且无读者，那么lock值为0x01000000；如果写者已获得自旋锁且无读者，则未锁状态位清0，lock值为0x00000000。如果有一个、2个或多个线程获取锁对临界数据区进行读操作，则lock值为0x00ffffff、0x00fffffe等（第24位清0表示未锁，第0~23位为读者个数的补码）。
下面说明读/写自旋锁API函数的实现：
### （1）函数rwlock_init
函数rwlock_init将读/写自旋锁状态值设为0x01000000，其列出如下（在include/linux/spinlock.h中）：
\# define rwlock_init(lock)					/
	do { *(lock) = RW_LOCK_UNLOCKED; } while (0)\#define RW_LOCK_UNLOCKED	__RW_LOCK_UNLOCKED(old_style_rw_init)\#define __RW_LOCK_UNLOCKED(lockname) /
	(rwlock_t)	{	.raw_lock = __RAW_RW_LOCK_UNLOCKED,	/
				RW_DEP_MAP_INIT(lockname) }\#define __RAW_RW_LOCK_UNLOCKED		{ RW_LOCK_BIAS }\#define RW_LOCK_BIAS		 0x01000000

### （2）函数read_lock和read_unlock
函数read_lock用于加读者锁，函数read_unlock用于解读者锁，两函数需要配对使用。下面分别进行说明：
#### 函数read_lock
读/写自旋锁lock空闲值为0x01000000，当有一个读者进行读操作时，它加读者锁，执行运算lock=lock-1，lock值为0x00ffffff；当接着有第二个读者进行读操作时，可以进行并发的读，再执行运算lock=lock-1，lock值为0x00fffffe；依此类推，可支持多个读者同时读操作。
如果在读操作正进行（如：有2个读者正进行操作，lock值为0x00fffffe）时，有一个写者请求写操作时，写操作必须等待读者全部完成操作，每个读者完成操作时，执行运算lock=lock+1，当2个读者的操作完成后，lock值为0x01000000，表示写锁空闲，可以进行写操作或并发的读操作。
如果一个写操作正进行时，执行运算lock=lock-0x01000000，lock值为0x00000000，表示写者锁已加锁，另一个写者无法对临界区数据进行访问。此时，如果有一个读者进行读操作请求时，执行运算lock=lock-1，结果为负数，则状态寄存器符号位置为1，加读者锁失败，将lock还原（lock=lock+1），读者循环等待，直到写操作完成（即lock值为0x01000000）时。
写操作完成时，lock值为0x01000000，表示写锁空闲，可以进行写操作或并发的读操作。这时，正等待的读者执行运算lock=lock-1，结果为0x00ffffff，则状态寄存器符号位置为0，跳出加读者锁的等待循环，加锁成功，读者进行读操作。
函数read_lock关闭内核抢占，加读者锁，即将读者数增加1，其列出如下（在include/linux/spinlock.h中）：
\#define read_lock(lock)			_read_lock(lock)

函数_read_lock列出如下（在kernel/spinlock.c中）：
void __lockfunc _read_lock(rwlock_t*lock){
	preempt_disable();*//关闭内核抢占*rwlock_acquire_read(&lock->dep_map,0,0, _RET_IP_);*/*用于自旋锁调试*/**/*下面语句相当于_raw_read_lock（lock）*/*LOCK_CONTENDED(lock, _raw_read_trylock, _raw_read_lock);}\# define _raw_read_lock(rwlock)		__raw_read_lock(&(rwlock)->raw_lock)

函数__raw_read_lock增加读锁，即锁状态值rw减1，由于读者计数以补码形式存放在锁状态值中，因此，减1表示读者计数增加1。其列出如下（在include/asm-x86/spinglock.h中）：
static**inline**void __raw_read_lock(raw_rwlock_t*rw){
	asmvolatile(LOCK_PREFIX" subl $1,(%0)**/n****/t**"*//*rw=*rw-1*"jns 1f**/n**"*//如果符号位为0，跳转到1*"call __read_lock_failed**/n****/t**""1:**/n**"::LOCK_PTR_REG(rw):"memory");}

函数__read_lock_failed进行加读者锁失败后的循环等待操作。加读者锁失败，说明有一个写者正在写操作，因此，锁状态值为*rw=0x00000000，函数__raw_read_lock在执行*rw=*rw-1后，rw值为0xffffffff，即传入函数__read_lock_failed的rw值为0xffffffff。
函数__read_lock_failed执行*rw=*rw+1后，锁状态值为*rw=0x00000000，然后，进入循环等待状态，直到，写者完成写操作后将锁状态值*rw置为0x01000000。这时，函数__read_lock_failed才跳出循环等待状态，加读者锁成功。
函数__read_lock_failed列出如下（在include/asm-x86/lib/rwlock_64.h中）：
/* rdi指向rwlock_t*/
ENTRY**(**__read_lock_failed**)**CFI_STARTPROC//即：\#define CFI_STARTPROC.cfi_startproc
	LOCK_PREFIX
	incl**(**%rdi**)**//*rw=*rw+1，值为0x000000001:**rep**//循环等待*rw值被写者修改为0x01000000**nop**cmpl $1,**(**%rdi**)**//*rw-1**js**1b//如果符号位为1，表明*rw值还为0x00000000，跳转到1进行循环等待
	LOCK_PREFIX/* 运行到这里，说明写者操作完成，*rw值为0x01000000*/
	decl**(**%rdi**)**//执行加读者锁操作*rw＝*rw-1**js**__read_lock_failed//如果符号位为1，表明*rw值为0x00000000，跳转到函数开头进行循环等待**ret**CFI_ENDPROC//即：\#define CFI_ENDPROC.cfi_endproc**END****(**__read_lock_failed**)**

由于汇编语言程序无法产生帧信息，由用户手动添加指示语句。上述代码中，指示语句.cfi_startproc用于调试时的调用帧信息处理，在每个函数的开始处使用，它在.eh_frame中生成一个条目，初始化一些内部数据结构，并发出构架依赖的初始CFI（Call Frame Information）指令。在函数结束处使用.cfi_endproc关闭该功能。
#### 函数read_unlock
函数read_unlock开读者锁，即将锁状态值减1，由于读者计数以补码形式存放在锁状态值中，因此，加1表示读者计数减1。其列出如下
\# define read_unlock(lock) /
do {__raw_read_unlock(&(lock)->raw_lock); __release(lock); } while (0)\# define __release(x)	__context__(x,-1)static**inline**void __raw_read_unlock(raw_rwlock_t*rw){*/* rw->lock= rw->lock +1*/*asmvolatile(LOCK_PREFIX"incl %0":"+m"(rw->lock)::"memory");}

### （3）函数write_lock和write_unlock
函数write_lock和write_unlock分别加写者锁和解写者锁，分别说明如下：
#### 函数write_lock
只有在没有读者或写者对临界区数据进行操作时，加写者锁才会成功，即：只有锁状态值lock值为0x01000000时，写者加锁才能成功，执行运行lock=lock-0x01000000运算。
当有读者或写者操作临界区数据时，lock值为比0x01000000小的正数，如果值为0x00000000表示有一个写者正在写操作，如果值为0x00ffffff，表示有1个读者在进行读操作，如果值为0x00fffffe，表示有2个读者在进行读操作，依此类推。此时，写者只能循环等待，直到lock值为0x01000000。
函数write_lock关闭内核抢占，加写者锁，其列出如下（在include/linux/spinlock.h中）：
\#define write_lock(lock)		_write_lock(lock)

函数_write_lock列出如下（在kernel/spinlock.c中）：
void __lockfunc _write_lock(rwlock_t*lock){
	preempt_disable();*/*关闭内核抢占*/*rwlock_acquire(&lock->dep_map,0,0, _RET_IP_);*/*用于自旋锁调试*/**/*下面语句相当于_raw_write_lock(lock)*/*LOCK_CONTENDED(lock, _raw_write_trylock, _raw_write_lock);}\# define _raw_write_lock(rwlock)	__raw_write_lock(&(rwlock)->raw_lock)static**inline**void __raw_write_lock(raw_rwlock_t*rw){
	asmvolatile(LOCK_PREFIX" subl %1,(%0)**/n****/t**"*/* RW_LOCK_BIAS-rw*/**/* 如果没有读者或写者，*rw为0x01000000，RW_LOCK_BIAS-rw为0  */*"jz 1f**/n**"*/*值为0，跳转到1*/*"call __write_lock_failed**/n****/t**"*/*加写者锁失败*/*"1:**/n**"*/* RW_LOCK_BIAS定义为0x01000000*/*::LOCK_PTR_REG(rw),"i"(RW_LOCK_BIAS):"memory");}

运行函数__write_lock_failed时，说明加写者锁失败。如果加写者锁失败，说明有读者或写者正在访问临界区数据，*rw值为一个小于0x01000000的正数。此时，函数__write_lock_failed循环等待直到，读者或写者完成操作，锁变为空闲，即*rw值为0x01000000。
函数__write_lock_failed列出如下（在include/asm-x86/lib/rwlock_64.h中）：
/* rdi:	pointer to rwlock_t*/
ENTRY**(**__write_lock_failed**)**CFI_STARTPROC/*用于调试时将调用帧信息写入
	LOCK_PREFIX
	addl $RW_LOCK_BIAS,**(**%rdi**)**//*rw=*rw+$RW_LOCK_BIAS，还原为尝试加锁前的状态值1:**rep****nop**cmpl $RW_LOCK_BIAS,**(**%rdi**)**//比较结果 =*rw-$RW_LOCK_BIAS**jne**1b//比较结果不为0，说明有写者或读者在访问临界区，跳转到1进行循环等待
	LOCK_PREFIX//锁内存管理器，确保原子操作/*运行到这里，说明锁空闲，*rw值为0x010000，执行加写者锁操作*/
	subl $RW_LOCK_BIAS,**(**%rdi**)**//*rw=*rw-RW_LOCK_BIAS**jnz**__write_lock_failed/*如果*rw不为0，说明加写者锁失败，跳转到函数头循环等待*/**ret**CFI_ENDPROC**END****(**__write_lock_failed**)**

#### 函数write_unlock
函数write_unlock在写者操作完后解写者锁，读/写自旋锁变为空闲，锁状态值lock变为: 0x00000000+0x01000000。以后，读者或写者可以访问临界区数据了。
函数write_unlock列出如下：
\# define write_unlock(lock) /
    do {__raw_write_unlock(&(lock)->raw_lock); __release(lock); } while (0)

函数_write_unlock列出如下（在kernel/spinlock.c中）：
void __lockfunc _write_unlock(rwlock_t*lock){
	rwlock_release(&lock->dep_map,1, _RET_IP_);
	_raw_write_unlock(lock);
	preempt_enable();*/*打开内核抢占*/*}\# define _raw_write_unlock(rwlock)	__raw_write_unlock(&(rwlock)->raw_lock)

函数__raw_write_unlock开写者锁，即将锁状态值加上RW_LOCK_BIAS，其列出如下（在include/asm-x86/spinlock.h中）：
static**inline**void __raw_write_unlock(raw_rwlock_t*rw){
	asmvolatile(LOCK_PREFIX"addl %1, %0"*/* RW_LOCK_BIAS+rw*/*:"+m"(rw->lock):"i"(RW_LOCK_BIAS):"memory");}

## 顺序锁
当使用读/写锁时，读者必须等待写者完成时才能读，写者必须等待读者完成时才能写，两者的优先权是平等的。顺序锁是对读/写锁的优化，它允许读写同时进行，提高了并发性，读写操作同时进行的概率较小时，其性能很好。顺序锁对读/写锁进行了下面的改进：
写者不会阻塞读者，即写操作时，读者仍可以进行读操作。
写者不需要等待所有读者完成读操作后才进行写操作。
写者与写者之间互斥，即如果有写者在写操作时，其他写者必须自旋等待。
如果在读者进行读操作期间，有写者进行写操作，那么读者必须重新读取数据，确保读取正确的数据。
要求临界区的共享资源不含指针，因为如果写者使指针失效，读者访问该指针，将导致崩溃。
顺序锁实际上由一个自旋锁和一个顺序计数器组成，有的应用已包括自旋锁，只需要一个顺序计数器配合就可以实现顺序锁。针对这两种情况，Linux内核给顺序锁提供了两套API函数。一套API函数为*seq*，完整地实现了顺序锁；另一套API函数为*seqcount*，只包含了顺序计数器，需要与用户的自旋锁配套实现顺序锁。顺序锁API函数的功能说明如表5所示。
表5 顺序锁API函数功能说明|函数名|功能说明|
|seqlock_init(x)|初始化顺序锁，将顺序计数器置0。|
|write_seqlock(seqlock_t *sl)|加顺序锁，将顺序号加1。写者获取顺序锁s1访问临界区，它使用了函数spin_lock。|
|write_sequnlock(seqlock_t *sl)|解顺序锁，使用了函数spin_unlock，顺序号加1。|
|write_tryseqlock(seqlock_t *sl)|功能上等同于spin_trylock，顺序号加1。|
|read_seqbegin(const seqlock_t *sl)|返回顺序锁s1的当前顺序号，读者没有开锁和释放锁的开销。|
|read_seqretry(const seqlock_t *sl, unsigned start)|检查读操作期间是否有写者访问了共享资源，如果是，读者就需要重新进行读操作，否则，读者成功完成了读操作。|
|seqcount_init(x)|初始化顺序号。|
|read_seqcount_begin(const seqcount_t *s)|读者在读操作前用此函数获取当前的顺序号。|
|read_seqcount_retry(const seqcount_t *s, unsigned start)|读者在访问完后调用此函数检查在读期间是否有写者访问临界区。如果有，读者需要重新进行读操作，否则，完成读操作。|
|write_seqcount_begin(seqcount_t *s)|写者在访问临界区前调用此函数将顺序号加1，以便读者检查是否在读期间有写者访问过。|
|write_seqcount_end(seqcount_t *s)|写者写完成后调用此函数将顺序号加1，以便读者能检查出是否在读期间有写者访问过。|
用户使用顺序锁时，写操作加锁方法与自旋锁一样，但读操作需要使用循环查询，使用顺序锁的读操作样例列出如下（在kernel/time.c中）：
u64 get_jiffies_64(void){unsignedlong seq;
	u64 ret;do{
		seq= read_seqbegin(&xtime_lock);*//获取当前的顺序号*ret= jiffies_64;*//读取临界区数据**/*检查seq值与当前顺序号是否相等，若不等，说明有写者开始工作，函数read_seqretry返回1，继续循环*/*}while(read_seqretry(&xtime_lock, seq));return ret;}

在非SMP系统上，自旋锁消失，但写者还必须递增顺序变量，因为中断例程可能改变数据的状态。
下面分析顺序锁的数据结构及API函数：
### （1）顺序锁结构seqlock_t
顺序锁用结构seqlock_t描述，它包括顺序计数器sequence和自旋锁lock。结构seqlock_t列出如下（在include/linux/seqlock.h中）：
typedefstruct{unsigned sequence;
	spinlock_t lock;} seqlock_t;

在结构seqlock_t中，顺序计数器sequence存放顺序号，每个读者在读数据前后两次读顺序计数器，并检查两次读到的顺序号是否相同。如果不相同，说明新的写者已经开始写并增加了顺序计数器，表明刚读到的数据无效。
写者通过调用函数write_seqlock获取顺序锁，将顺序号加1，调用函数write_sequnlock释放顺序锁，再将顺序号加1。这样，写者正在写操作时，顺序号为奇数，写完临界区数据后，顺序号为偶数。
读者应以循环查询方法读取临界区数据，读者执行的临界区代码的方法列出如下：
do{
	    seq= read_seqbegin(&foo);*//返回当前的顺序号*...*//临界区数据操作*}while(read_seqretry(&foo, seq));
在上述代码中，读者在读临界区数据之前，先调用函数read_seqbegin获致当前的顺序号，如果顺序号seq为奇数，说明写者正写临界区数据，或者seq值与顺序号当前值不等，表明读者正读时，写者开始写，函数read_seqretry返回1，读者继续循环等待写者完成。
### （2）顺序锁初始化函数seqlock_init
函数seqlock_init初始化顺序锁，顺序锁实际上由一个自旋锁和一个顺序计数器组成。其列出如下：
\#define seqlock_init(x)					/
	do {						/
		(x)->sequence = 0;			/
		spin_lock_init(&(x)->lock);		/
	} while (0)

### （3）写者加锁函数write_seqlock
函数write_seqlock加顺序锁。方法是：它先加自旋锁，然后将顺序号加1，此时，顺序号值为奇数。此函数不需要关闭内核抢占，因为自旋锁加锁时已关闭了内核抢占。其列出如下：
static**inline**void write_seqlock(seqlock_t*sl){
	spin_lock(&sl->lock);++sl->sequence;
	smp_wmb();}

### （4）写者解锁函数write_sequnlock
函数write_sequnlock表示写者解顺序锁，它将顺序号加1，然后解开自旋锁。此时，顺序号应为偶数。其列出如下（在include/linux/seqlock.h中）：
static**inline**void write_sequnlock(seqlock_t*sl){
	smp_wmb();*//加上SMP写内存屏障*sl->sequence++;*//顺序号加1*spin_unlock(&sl->lock);*//解开自旋锁*}

### （5）读操作开始时读顺序号函数read_seqbegin
函数read_seqbegin读取顺序号，如果顺序号为奇数，说明写者正在写操作，处理器执行空操作，进行循环等待，否则，函数返回读取的顺序号值。其列出如下：
static __always_inlineunsigned read_seqbegin(const seqlock_t*sl){unsigned ret;
 
repeat:
	ret= sl->sequence;
	smp_rmb();*//加上SMP读内存屏障*if(unlikely(ret&1)){*//如果ret & 1为true，表示顺序号为奇数，写者正在写操作*cpu_relax();*//空操作*goto repeat;}return ret;}

### （6）读操作完成时顺序号检查函数read_seqretry
函数read_seqretry用于读操作完成后检测读的数据是否有效。如果读操作完成后的顺序号与读操作开始前的顺序号不一致，函数返回1，说明有写者更改了临界区数据，因此，调用者必须重新读临界者数据。
函数read_seqretry列出如下：
static __always_inlineint read_seqretry(const seqlock_t*sl,unsigned start){
	smp_rmb();*//加上SMP读内存屏障*return(sl->sequence!= start);*//顺序锁的顺序号值与读操作开始时的顺序号值start不一致*}

## 大内核锁
Linux内核因历史原因还保留着大内核锁（Big Kernel Lock，BKL），它在内核中的用途越来越小。大内核锁用于同步整个内核，锁的保持时间较长，严重地影响延迟，不提倡使用。
大内核锁本质上是自旋锁，它由一个自旋锁和一个锁深度变量组成。自旋锁不能递归获得锁的，否则导致死锁。大内核锁进行了改进，它可以递归获得锁，还实现了抢占。整个内核只有一个大内核锁，因为内核只有一个，用于保护整个内核。
锁深度变量定义列出如下（在include/linux/sched.h）：
struct task_struct{
	……int lock_depth;*/* BKL锁深度*/*……}

自旋锁定义列出如下（在lib/kernel_lock.c中）：
static  __cacheline_aligned_in_smp DEFINE_SPINLOCK(kernel_flag);
大内核锁的API函数为lock_kernel(void)和unlock_kernel(void)，函数lock_kernel为获取大内核锁，可以递归调用而不导致死锁；函数unlock_kernel释放大内核锁。它们的用法与一般自旋锁类似。














posted on2018-10-08 09:51[公众号；嵌入式Linux](https://www.cnblogs.com/weiqifa/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9752713)[收藏](#)


