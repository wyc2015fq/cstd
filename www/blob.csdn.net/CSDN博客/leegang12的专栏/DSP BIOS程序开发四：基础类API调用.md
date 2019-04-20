# DSP/BIOS程序开发四：基础类API调用 - leegang12的专栏 - CSDN博客
2013年12月22日 22:18:58[leegang12](https://me.csdn.net/leegang12)阅读数：591
个人分类：[DSP实现算法](https://blog.csdn.net/leegang12/article/category/816289)
【声明：内容源于网络】
一、时钟管理CLK
(1)Uns ncounts   = CLK_countspms(void)
返回每毫秒的定时器高分辨率时钟的计数值
(2)LgUns currtime = CLK_gethtime(void)
返回高分辨率时钟的计数值超过32bit归零
高分辨率时钟是DSP时钟除以(TDDR+1)
(3)LgUns currtime = CLK_getltime(void)
返回低分辨率时钟的计数值超过32bit归零
高分辨率时钟是DSP时钟除以(TDDR+1)再除以(PRD+1)
(4)Uns period   = CLK_getprd(void)
返回CLK管理器周期寄存器的值
二、周期函数PRD
(1)LgUns num   = PRD_getticks(void)
返回32位周期函数管理计数值
(2)void PRD_start(PRD_Obj* period)
启动PRD模块计数器，一般地，mode=one-shot
(3)void PRD_stop(PRD_Obj* period)
关闭PRD模块计数器
(4)void PRD_tick(void)
对周期模块的计数器加1，以便周期模块管理器确定哪个周期性函数得以运行
三、软件中断管理SWI
(1)void SWI_andn(SWI_Obj* swi, Uns mask)  mask 屏蔽字参数
将邮箱值与mask做与运算，并用结果代替以前的邮箱值，若为0，启动软件中断，恢复邮箱初始值
(2)void SWI_andn(SWI_Obj* swi)
将邮箱值减1，并用结果代替以前的邮箱值，若为0，启动软件中断，恢复邮箱初始值
(3)void SWI_disable(Void)
禁止软件中断
(4)void SWI_enable(Void)
使能软件中断
(5)Uns num = SWI_getmbox(void)
返回当前SWI对象在被执行的邮箱值，若中断已经开始执行，DSP/BIOS会先保存此值
(6)Uns key = SWI_getpri(SWI_Obj * swi)
返回指定的SWI对象的优先级
(7)void SWI_inc(SWI_Obj * swi)
将指定的SWI对象的邮箱值加1，同时启动软件中断，就算用户调用多次，最终执行一次。
(8)void SWI_or(SWI_Obj* swi, Uns mask)  mask 屏蔽字参数
将启动指定的软件中断，并将此软件中断的邮箱值与mask做与运算，并用结果代替以前的邮箱值，
等到中断完成之后，邮箱值恢复为初始值，一般地，调用SWI_getmbox获得触发此软件中断的邮箱值
(9)void SWI_post(SWI_Handle swi)
启动指定的SWI对象的软件中断，此函数不受邮箱值的影响，也不影响邮箱值。
一般地，周期性低启动一个软件中断，将_SWI_post填入PRD对象的函数设置栏，启动软件中断的参数写入arg0
(10)Uns key = SWI_raisepri(Uns mask)
提高SWI软件中断的优先级，一般地
  key = SWI_raisepri(SWI_getpri(&swi_1));
  --access shared resouces--
  SWI_restorepri(key)
(11)void SWI_restorepri(Uns key)
恢复原来的优先级
(12)SWI_Obj * swi = SWI_self(void)
当前执行的SWI对象的软件中断的地址
四、信息输出管理LOG
(1)void LOG_disable(LOG_Obj * log)
关闭指定对向的日志功能
(2)void LOG_enable(LOG_Obj * log)
开启指定对向的日志功能
(3)void LOG_error(String format, Arg arg0)
  void LOG_message(String format, Arg arg0)
前一个函数可以将一个事件、数据或出错信息按照指定的格式串写入系统日志，不受TRC跟踪管理模块的影响
后一个函数类似于前一个函数，但是受到TRC跟踪管理模块的影响
(4)void LOG_event(LOG_Obj * log, Arg arg0, Arg arg1, Arg arg2)
将未格式化的事件消息写入日志中
(5)void LOG_printf(LOG_Obj * log, String format, int arg0, int arg1)
指定的LOG窗口显示消息 %d %x %o %s
(6)void LOG_reset(LOG_Obj * log)
复位日志缓冲区
五、存储器管理MEM
(1)void * addr = MEM_alloc(int segid,Uns size,Uns align)
指定存储段分配连续块，返回起始地址
  segid = 存储段的标识符或者ID号
  size 块大小等于多少个字
  align 边界条件只为0或2的幂若align为0、1
则无约束
(2)void * addr = MEM_calloc(segid, size, align)
分配内存并初始化为0
(3)int segid = MEM_define(Ptr base, Uns length, MEM_Attrs* attrs)
定义一个新的存储段。返回值为存储段的ID标号
  attr参数为NULL，会按照默认参数进行配置，段的参数由结构体type MEM_Attrs规定
此函数仅在main函数中使用
  base: 新段的基地址
  length：段长度
  attrs：段属性
(4)bool status = MEM_free(segid,addr,size)
释放申请的动态内存不能在SWI和HWI中调用
调用之前，用LCK中API查看内存块的锁定情况
(5)void MEM_redefine(segid,base,length)
重新定义一个存储段，只能在main中调用
(6)bool status = MEM_stat(int segid, MEM_Stat * statbuf)
  segid 存储段标识符
  statbuf 状态缓存区指针
 struct mem_stat{
  Uns size;//存储段大小
  Uns used;//已经使用的数量
  Uns length;//最大连续存储块长度
 }
同理：若segid有效，则MEM_stat返回TRUE
不能在SWI和HWI中使用，应判断锁定情况
(7)void * addr = MEM_valloc(int segid, Uns size, Uns align, char value)
先申请内存，初始化为指定的值value
六、C6000专用模块C64x
(1)Uns oldmask = C64_diableIER(Uns mask)
关闭相应的中断返回值实际屏蔽的中断
(2)void C64_enableIER(Uns oldmask)
开放相应的中断
  Uns oldmask;
  oldmask = C64_disableIER(0x1); // disable INT0
  C64_enableIER(oldmask);     // enable INT0
(3)void C64_plug(int vecid, Fxn fxn, int dmachan)
插入一个中断向量
  int vecid: 中断序号
  Fxn fxn: 中断服务程序的函数指针
  int dmachan: 插入操作使用的DMA通道
将ISFP写到中断服务表IST中。若IST在外部RAM，则CPU复制代替DMA，此时DMA为-1
  IST在片内RAM时，使用DMA方式，是0，1，2，3
用户保证在函数调用时该DMA通道有效
七、统计模块STS
(1)void STS_add(STS_Obj * sts, LgInt value)
使用我们提供的数据更新STS统计对象的Total，Count，Max等参数
传入一个32位参数，Count加1，Total累加传递的32位参数，MAX记录传递参数的最大值
(2)void STS_delta(STA_Obj * sts, LgInt value)
每个对象都包含有初始值，该初始值由配置文件指定或者由STS_set函数设置
  STS_delta先计算当前传递参数与先前初始值只差，然后调用STS_add更新统计累加值
一般和STS_set一起用
  eg：
   STS_set（&sts,CLK_gethtime());
   STS_delta(&sts,CLK_gethtime());
(3)void STS_reset(STS_Obj * sts)
复位统计对象中的累加值。Count和Total设置为0.MAX成为最大的负值。
(4)void STS_set(STS_Obj * sts, LgInt value)
监视或统计程序的执行时间
八、统计类模块TRC
(1)TRC_LOGCLK 日志记录定时器中断
(2)TRC_LOGPRD 日志记录周期时隙和周期函数的开始
(3)TRC_LOGSWI 日志记录软件中断的启动和完成
(4)TRC_LOGTSK 日志记录任务处于准备好、开始、停止、继续执行、终止等事件
(5)TRC_STSHWI 收集关于HWI统计数据
(6)TRC_STSPIP 写管道和读管道帧进行计数
(7)TRC_STSPRD 执行期间CLK时隙个数的统计数据
(8)TRC_STSSWI SWI执行时间的统计数据
(9)TRC_STSTSK TSK执行时间的统计数据从TSK准备好到调用TSK
 deltatime都要收集
(10)TRC_USER0 AND TRC_USER1 和TRC_query一起用执行和忽略结果的设备调用
(11)TRC_GBLHOST 执行所有的隐藏仪器功能，此比特位能够开始或停止收集所有开启的跟踪类型
(12)TRC_GBLTARG 执行所有的隐藏仪器功能，此比特位只能被目标程序设置
九、任务管理 TSK任务管理模块
(1)void TSK_checkstacks(TSK_Handle oldtask,TSK_Handle newtask)
无论是新任务还是老任务，如果堆栈的最后位置上都没有RG_STACKSTAMP标识，那么TSK_checkstacks就会
报错SYS_abort，出现这种情况可能是由于上一个任务堆栈溢出或无效的存储占用了新任务的堆栈
一般地，用TSK_checkstacks(TSK_self(),TSK_self())来检查堆栈
定义Switch函数，这样在任务切换时自动调用堆栈检查函数：
  void myswitchfxn(TSK_Handle oldtask, TSK_Handle newtask)
  {
   ...
   TSK_checkstacks(oldtask,newtask);
   ...
  }
(2)TSK_Handle task = TSK_create(Fxn fxn, TSK_Attrs * attr, Arg [arg,]...)
创建一个调用函数fxn的任务对象，返回新对象的句柄，失败返回NULL。
调用此函数是动态创建，而在配置工具中创建是静态创建，二者效果一样。
静态创建的任务对象，创建函数将在BIOS_start函数中自动调用。BIOS_start函数在main函数之后，
在后台IDL循环之前运行。而动态创建的任务处于Ready状态。函数参数最多不超过8个。
任务对象函数fxn返回时，自动调用TSK_exit函数。
解析：TSK_Attrs * attrs
任务参数指针
  struct TSK_Attrs{
   int priority;
   Ptr stack;
   Uns stacksize;
  #ifdef _64_ // imitate C55 series. to check
   Uns sysstacksize;
  #endif
   Uns stackseg;
   Ptr environ;
   String name;
   bool exitflag;
  }
(3)void TSK_delete(TSK_Handle task)
从所有内部队列里面删除这个任务，并且调用MEM_free释放任务对象和堆栈。
只能删除处于结束状态的任务，也可调用删除的钩子函数。
  void myDeleteFxn(TSK_Handle task);
(4)void TSK_deltatime(TSK_Handle task)
累计从任务准备好到执行此函数时候的时间差
如果未调用此函数，那么就算打开任务统计累加器选项，统计对象也不会更新
一般地，统计时先用TSK_settime函数记录起始点，此函数记录终点。
(5)void TSK_settime(TSK_Handle task)
设定统计初始值
  void task()
  {
   --do some startup work--
   TSK_settime(TSK_self);
   for(;;){
    SIO_get(...);
    --process data--
    TSK_deltatime(TSK_self);
   }
  }
假如流式IO没有准备好，那么该API函数会阻塞(Blocked)，任务切换，一段时间后，流式IO数据
准备好了，此时发出READY信号，将本任务置为ready状态，此时TSK_settime会重新记录时间。
(6)void TSK_disable(void)
全局关闭内核调度机制，Busy-Shutting-Down状态。只有当前任务可行，其余所有任务禁止。
此函数不会禁止中断，所以在中断开始前需要调用此函数保证中断发生时不会发生任务切换。
可以嵌套，但是调用几次TSK_disable，就得相应调用几次TSK_enable
(7)void TSK_enable(void)
全局开启内核调度机制
(8)void TSK_exit(void)
终止当前任务运行。如果所有任务都被终止，则DSP/BIOS会调用SYS_exit终止程序。
无论什么时候，任务从顶层函数返回时，都是自动调用此函数。
可以注册一个退出辅助函数 void myExitFxn(void)
这样，在任务被设置为TSK_TERMINATED模式之前，会调用这个辅助函数。
(9)Ptr environ = TSK_getenv(TSK_Handle task)
返回任务环境指针，这个指针指向一个该任务可以访问的全局属性的结构。
若程序定义多个钩子对象，那么HOOK_getenv函数可以获取设置的环境指针。
(10)void TSK_setenv(TSK_Handle task, Ptr environ)
设置指定任务的环境指针。
若程序定义多个钩子对象，那么HOOK_setenv函数可以为每个钩子和任务对象的组合体设置独立的环境指针。
(11)int errno = TSK_geterr(TSK_Handle task)
每个任务对象都有一个包含任务错误号的存储单元。初始值为SYS_OK
(12)void TSK_seterr(TSK_Handle task, int errno)
改变错误号
(13)String name = TSK_getname(TSK_Handle task)
返回任务的名字。
对于静态对象来说，必须打开Allocate Task Name on Target
对于动态对象来说，TSK_getname返回attrs.name字段
(14)int priority = TSK_getpri(TSK_Handle task)
返回优先级
(15)int oldpri = TSK_setpri(TSK_Handle task, int newpri)
设置优先级
设置优先级对于TSK_BLOCKED状态任务只是优先级改变，而不会改变状态；
对于TSK_READY状态的任务而言，可能会改变运行状态。
(16)STS_Handle sts = TSK_getsts(TSK_Handle task)
获得统计对象句柄，以便查看数据
(17)void TSK_sleep(Uns nticks)
暂停任务的时钟个数，此时钟数可能比真实的暂停时钟少一个时钟（告警时钟）
(18)void TSK_itick(void)
对告警时钟加1，以便让TSK_sleep或者SEM_pend函数暂停执行的任务恢复到ready。
一些暂停的任务可能会随着告警时钟的增加而超时，从而就绪。
(19)void TSK_tick(void)
对告警时钟加1，以便让TSK_sleep或者SEM_pend函数暂停执行的任务恢复到ready。
一些暂停的任务可能会随着告警时钟的增加而超时，从而就绪。
可以在中断服务程序和当前任务中调用，后者在控制超时非常有用。
(20)Uns currtime = TSK_time(void)
返回系统告警时钟的当前值。(由于延迟，只能得到一个大概的系统时钟)
(21)TSK_Handle currtask = TSK_self(void)
返回当前任务对象的句柄
(22)void TSK_stat(TSK_Handle task,TSK_Stat * statbuf)
返回任务的属性参数和状态信息
  struct TSK_Stat{
   TSK_Attrs attrs;//任务参数
   TSK_Mode mode; //任务执行模式
   Ptr sp;     //任务当前堆栈指针
   Uns used;    //任务堆栈曾经使用的最大值
  }
注意：任务比HWI和SWI中断优先级要低，所以当任务被中断时，还是返回TSK_RUNNING，
因为中断完成后任务继续运行。
(23)void TSK_yield(void)
强制任务切换，请注意，任务可以被中断，但是，任务之间必须依靠切换来进行，就是说，
即便当前有高优先级任务就绪，它不能被执行，除非切换。
此函数用于任务之间的同步。
十、任务管理 HOOK钩子函数管理模块
 HOOK模块管理者一组涉及钩子函数的对象。DSP/BIOS初始化期间，每一个HOOK对象都分配了一个数字标识符。
每个HOOK模块都有一个初始化函数，该函数会在钩子函数调用前运行。在调用初始化函数是，DSP/BIOS内核
会将HOOK对象的标识符以参数的形式传给初始化函数。
 HOOK对象的数据是存放在.bss部分中的。
关于HOOK属性的设置，对应关系如下：
 Initialization function: TSK_create
 delete function: TSK_delete
 Exit function: TSK_exit
另外还有Call switch function, switch function, Call ready function, ready function
十一、DSP/BIOS后台管理 IDL模块
 IDL  模块管理着应用中最低等级的线程。除了用户自己编写的函数外，IDL模块执行DSP/BIOS的API函数，
 DSP/BIOS内核正是利用这些函数来处理主机通信和CPU负载计算的。
当DSP/BIOS分析工具的RTA控制面板激活时，应用程序会包含一个IDL_cpuLoad的IDL对象，其会调用一个提供
 CPU利用率的函数，绘出负载图。另外，还有LINL_dataPump函数在后台处理与主机的数据交换，RTDX，HST等。
程序模块执行状态图，就是RTA_dispatch函数处理的结果。
(1)void IDL_run(void)
依次调用IDL模块中定义的所有IDL函数，其在所有IDL函数执行一次后返回，由于IDL函数中有完成主机和目标
系统交换数据的函数，所有IDL函数必须定期调用。
