# Wince下GetTickCount函数不能再计时的原因 - xqhrs232的专栏 - CSDN博客
2014年07月05日 10:45:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1274

    这几天遇到了一个怪现象，有段代码有用到GetTickCount函数，但代码执行一直不是自己想要的结果；追踪代码发现GetTickCount函数压根就没有再计时，前后两句GetTickCount函数居然读到的时间是一样的。
  通过分析BSP的一些相关代码发现，GetTickCount函数能够正确运行是依赖一个定时器中断的，而这段代码正好把这个IRQ形式的定时器中断关掉了，所以造成了GetTickCount函数不能再计时的现象。一般看IRQ的中断处理函数OEMInterruptHandler就可以发现这一点，跟OALTimerIntrHandler()函数有关的Timer定时器处理分支就是系统用于GetTickCount函数的定时器，这个定时器的功能就是It provides 1ms time tick at any power
 mode except sleep mode。一般看BSP包里面的OALTimerInit( )函数也可以看到BSP包将使用那个定时器来做系统的1ms Tick Timer.
  一般的BSP包里面有System_DisableIRQ( )/System_EnableIRQ( )/System_DisableFIQ( )/System_EnableFIQ( )这样的底层操作函数的。一般是用汇编语言写的函数，像我手头上用的BSP就是一个system.s汇编语言文件中定义了这些IRQ/FIQ的中断操作函数。
