# 嵌入式 uboot引导kernel，kernel引导fs - DoubleLi - 博客园





1、uboot引导kernel：
      u-boot中有个bootm命令，它可以引导内存中的应用程序映像（Kernel），bootm命令对应
common/cmd_bootm.c中的do_bootm（）函数，此函数实现下面几个功能：
    1)读flash中的内核映像文件
    2)解压内核
    3)校验内核
    4)跳到内核执行（调用do_bootm_linux（）函数）    {
1、Stage1 start.S代码结构 u-boot的stage1代码通常放在start.S文件中，他用汇编语言写成，其主要代码部分如下



（1） 定义入口。： 

该工作通过修改连接器脚本来完成。

（2）设置异常向量（Exception Vector）。 

（3）设置CPU的速度、时钟频率及终端控制寄存器。 

（4）初始化内存控制器。 

（5）将ROM中的程序复制到RAM中。 

（6）初始化堆栈。 

（7）转到RAM中执行，该工作可使用指令ldr pc来完成。



2、Stage2 C语言代码部分 lib_arm/board.c中的start arm boot是C语言开始的函数也是整个启动代码中C语言的主函数，同时还是整个u-boot（armboot）的主函数，该函数只要完成如下操作： 

（1）调用一系列的初始化函数。 

（2）初始化Flash设备。 

（3）初始化系统内存分配函数。 

（4）如果目标系统拥有NAND设备，则初始化NAND设备。 

（5）如果目标系统有显示设备，则初始化该类设备。 

（6）初始化相关网络设备，填写IP、MAC地址等。 

（7）进去命令循环（即整个boot的工作循环），接受用户从串口输入的命令，然后进行相应的工作。
}2、kernel引导fs：    1)获得可运行的Linux内核
    2)内核装载时的内存空间映射
    3)内核启始相关文件分析
    4)arch/i386/boot/bootsect.S
    5)arch/i386/boot/setup.S
    6)arch/i386/boot/compressed/head.S
    7)arch/i386/kernel/head.S
    8)start_kernel    {
** 在start_kernel()函数中：**

　　输出Linux版本信息（printk(_banner)）

　　设置与体系结构相关的环境（setup_arch()）

　　页表结构初始化（paging_init()）

　　使用"arch/alpha/kernel/entry.S"中的入口点设置系统自陷入口（trap_init()）

　　使用alpha_mv结构和entry.S入口初始化系统IRQ（init_IRQ()）

　　核心进程调度器初始化（包括初始化几个缺省的Bottom-half，sched_init()）

　　时间、定时器初始化（包括读取CMOS时钟、估测主频、初始化定时器中断等，time_init()）

　　提取并分析核心启动参数（从环境变量中读取参数，设置相应标志位等待处理，（parse_options()）

　　控制台初始化（为输出信息而先于PCI初始化，console_init()）

　　剖析器数据结构初始化（prof_buffer和prof_len变量）

　　核心Cache初始化（描述Cache信息的Cache，kmem_cache_init()）

　　延迟校准（获得时钟jiffies与CPU主频ticks的延迟，calibrate_delay()）

　　内存初始化（设置内存上下界和页表项初始值，mem_init()）

　　创建和设置内部及通用cache（"slab_cache"，kmem_cache_sizes_init()）

　　创建uid taskcount SLAB cache（"uid_cache"，uidcache_init()）

　　创建文件cache（"files_cache"，filescache_init()）

　　创建目录cache（"dentry_cache"，dcache_init()）

　　创建与虚存相关的cache（"vm_area_struct"，"mm_struct"，vma_init()）

　　块设备读写缓冲区初始化（同时创建"buffer_head"cache用户加速访问，buffer_init()）

　　创建页cache（内存页hash表初始化，page_cache_init()）

　　创建信号队列cache（"signal_queue"，signals_init()）

　　初始化内存inode表（inode_init()）

　　创建内存文件描述符表（"filp_cache"，file_table_init()）

　　检查体系结构漏洞（对于alpha，此函数为空，check_bugs()）

　　SMP机器其余CPU（除当前引导CPU）初始化（对于没有配置SMP的内核，此函数为空，smp_init()）

　　启动init过程（run_init_process() 创建第一个核心线程，调用init()函数，原执行序列调用cpu_idle() 等待调度，init()）

　　至此start_kernel()结束，基本的核心环境已经建立起来了。
    }
    9)第一个内核线程 - kernel_init

三、start_kernel函数流程：asmlinkage void __init start_kernel(void)
{
	char * command_line;
	extern const struct kernel_param __start___param[], __stop___param[];


	smp_setup_processor_id();//首先判断是否是SMP （对称多处理器）对单核SOC来说，mpidr = 0;


	/*
	 * Need to run as early as possible, to initialize the
	 * lockdep hash:
	 */
	lockdep_init(); //只初始化该哈希表一次
	debug_objects_early_init();


	/*
	 * Set up the the initial canary ASAP:
	 */
	boot_init_stack_canary();//stack_canary的是带防止栈溢出攻击保护的堆栈


	/**
	 * cgroup_init_early - cgroup initialization at system boot
	 *
	 * Initialize cgroups at system boot, and initialize any
	 * subsystems that request early init.
	 */
	cgroup_init_early(); 


	local_irq_disable();
	early_boot_irqs_disabled = true;


/*
 * Interrupts are still disabled. Do necessary setups, then
 * enable them
 */
	//初始化time ticket，时钟
	tick_init(); 


	//用以启动的CPU进行初始化。也就是初始化CPU0
	boot_cpu_init();


	//初始化页面
	page_address_init();


	printk(KERN_NOTICE "%s", linux_banner);


	//CPU架构相关的初始化
	setup_arch(&command_line);
	
	//初始化内存管理		
	mm_init_owner(&init_mm, &init_task);
	mm_init_cpumask(&init_mm);
	
	//处理启动命令行
	setup_command_line(command_line);


	//可能多余的初始化可能去判断cpu的最大支持个数
	setup_nr_cpu_ids();


	//为每个CPU开辟一块区域？
	setup_per_cpu_areas();


	//准备boot_cpu.如果是SMP环境，则设置boot CPU的一些数据。在引导过程中使用的CPU称为boot CPU
	smp_prepare_boot_cpu();	/* arch-specific boot-cpu hooks */


	//Linux将所有物理内存分为三个区，ZONE_DMA, ZONE_NORMAM, ZONE_HIGHMEM
	build_all_zonelists(NULL);
	
	//初始化page allocation相关结构
	page_alloc_init();


	printk(KERN_NOTICE "Kernel command line: %s\n", boot_command_line);


	//解 析启动参数
	parse_early_param();
	parse_args("Booting kernel", static_command_line, __start___param,
		   __stop___param - __start___param,
		   &unknown_bootoption);
	/*
	 * These use large bootmem allocations and must precede
	 * kmem_cache_init()
	 */
	setup_log_buf(0);


	//初始化process ID hash表
	pidhash_init();


	//文件系统caches预初始化
	vfs_caches_init_early();


	//初始化exception table
	sort_main_extable();


	//初始化trap，用以处理错误执行代码
	trap_init();
	
	//初始化内存管理
	mm_init();


	/*
	 * Set up the scheduler prior starting any interrupts (such as the
	 * timer interrupt). Full topology setup happens at smp_init()
	 * time - but meanwhile we still have a functioning scheduler.
	 */
	
	//进程调度初始化
	sched_init();
	/*
	 * Disable preemption - early bootup scheduling is extremely
	 * fragile until we cpu_idle() for the first time.
	 */
	//	后当前进程将不能被强抢占
	preempt_disable();


	/*判断中断是否关闭，若打开则关闭中断*/	
	if (!irqs_disabled()) {
		printk(KERN_WARNING "start_kernel(): bug: interrupts were "
				"enabled *very* early, fixing it\n");
		local_irq_disable();
	}
	
	idr_init_cache();
	perf_event_init();
	
	//Read_Copy_Update机制初始 /*初始化互斥机制*/  
	rcu_init();
	radix_tree_init();
	
	/*中断向量的初始化*/  
	/* init some links before init_ISA_irqs() */
	early_irq_init();
	
	//初始化中断
	init_IRQ();
	prio_tree_init();


	/*初始化定时器*/  
	init_timers();


	//	初始化高精时钟
	hrtimers_init();


	//	初始化软中断
	softirq_init();


	//	初始化时钟源
	timekeeping_init();


	/*初始化系统时钟*/  
	time_init();


	/* 对内核的profile（一个内核性能调式工具）功能进行初始化 */    
	profile_init();
	
	call_function_init();//???
	if (!irqs_disabled())
		printk(KERN_CRIT "start_kernel(): bug: interrupts were "
				 "enabled early\n");
	early_boot_irqs_disabled = false;


	 /*打开IRQ中断*/ 
	local_irq_enable();


	/* Interrupts are enabled now so all GFP allocations are safe. */
	gfp_allowed_mask = __GFP_BITS_MASK;


	//初始化CPU Cache
	kmem_cache_init_late();


	/*
	 * HACK ALERT! This is early. We're enabling the console before
	 * we've done PCI setups etc, and console_init() must be aware of
	 * this. But we do want output early, in case something goes wrong.
	 */
	 
	//初始化console
	console_init();
	if (panic_later)
		panic(panic_later, panic_param);


	lockdep_info();


	/*
	 * Need to run this when irqs are enabled, because it wants
	 * to self-test [hard/soft]-irqs on/off lock inversion bugs
	 * too:
	 */
	 
	//自测试锁
	locking_selftest();


#ifdef CONFIG_BLK_DEV_INITRD
	if (initrd_start && !initrd_below_start_ok &&
	    page_to_pfn(virt_to_page((void *)initrd_start)) < min_low_pfn) {
		printk(KERN_CRIT "initrd overwritten (0x%08lx < 0x%08lx) - "
		    "disabling it.\n",
		    page_to_pfn(virt_to_page((void *)initrd_start)),
		    min_low_pfn);
		initrd_start = 0;
	}
#endif


	//页面初始
	page_cgroup_init();


	//页面分配debug启用
	enable_debug_pagealloc();
	debug_objects_mem_init();
	
	//memory leak 侦测初始化
	kmemleak_init();
	
	//设置每个CPU的页面集合
	setup_per_cpu_pageset();
	numa_policy_init();
	if (late_time_init)
		late_time_init();


	//初始化调度时钟
	sched_clock_init();


	/*校验延时函数的精确度*/  
	calibrate_delay();


	/*进程号位图初始化，一般用一个page来只是所有的进程PID占用情况*/ 
	pidmap_init();


	//anonymous page?什么意思？
	anon_vma_init();


#ifdef CONFIG_X86
	if (efi_enabled)
		efi_enter_virtual_mode();
#endif


	//初始化thread info
	thread_info_cache_init();


	//credential
	cred_init();
	
	//初始化fork
	fork_init(totalram_pages);
	
	//初始化/proc的cache?
	proc_caches_init();
	
	buffer_init();
	key_init();
	security_init();
	dbg_late_init();


	//文件系统cache初始化
	vfs_caches_init(totalram_pages);
	signals_init();
	
	/* rootfs populating might need page-writeback */
	page_writeback_init();


#ifdef CONFIG_PROC_FS
	proc_root_init();//本文件系统??
#endif


	cgroup_init();
	cpuset_init();
	taskstats_init_early();
	delayacct_init();


	check_bugs();


	acpi_early_init(); /* before LAPIC and SMP init */


	//simple firmware interface
	sfi_init_late();


	ftrace_init();


	/* Do the rest non-__init'ed, we're now alive */
	rest_init();
}








