# ldr和adr在使用标号表达式作为操作数的区别 - h13 - 博客园
 ARM汇编有ldr指令以及ldr、adr伪指令，他门都可以将标号表达式作为操作数，下面通过分析一段代码以及对应的反汇编结果来说明它们的区别。
     ldr     r0, _start
        adr     r0, _start
        ldr     r0, =_start
_start:
        b  _start
编译的时候设置 RO 为 0x30000000，下面是反汇编的结果：
   0x00000000: e59f0004  ldr r0, [pc, #4] ; 0xc
   0x00000004: e28f0000  add r0, pc, #0 ; 0x0
   0x00000008: e59f0000  ldr r0, [pc, #0] ; 0x10
   0x0000000c: eafffffe  b 0xc
   0x00000010: 3000000c  andcc r0, r0, ip
1．ldr     r0, _start
    这是一条指令，从内存地址 _start 的位置把值读入。
在这里_start是一个标号（是一个相对程序的表达式），汇编程序计算相对于 PC 的偏移量，并生成相对于 PC的前索引的指令：ldr r0, [pc, #4]。执行指令后，r0 = 0xeafffffe。
    ldr r0, _start是根据_start对当前PC的相对位置读取其所在地址的值，因此可以在和_start标号的相对位置不变的情况下移动。
2．adr     r0, _start
    这是一条伪指令，总是会被汇编程序汇编为一个指令。汇编程序尝试产生单个 ADD 或 SUB 指令来装载该地址。如果不能在一个指令中构造该地址，则生成一个错误，并且汇编失败。
    在这里是取得标号_start 的地址到 r0，因为地址是相对程序的，因此ADR产生依赖于位置的代码，在此例中被汇编成：add r0, pc, #0。因此该代码可以在和标号相对位置不变的情况下移动；
    假如这段代码在 0x30000000 运行，那么 adr r0, _start 得到 r0 = 0x3000000c；如果在地址 0 运行，就是 0x0000000c 了。
    通过这一点可以判断程序在什么地方运行。U-boot中那段relocate代码就是通过adr实现当前程序是在RAM中还是flash中，下面进行简要分析。
relocate: /* 把U-Boot重新定位到RAM */
    adr r0, _start /* r0是代码的当前位置 */
/* adr伪指令，汇编器自动通过当前PC的值算出 如果执行到_start时PC的值，放到r0中：
当此段在flash中执行时r0 = _start = 0；当此段在RAM中执行时_start = _TEXT_BASE(在board/smdk2410/config.mk中指定的值为0x30000000，即u-boot在把代码拷贝到RAM中去执行的代码段的开始) */
    ldr r1, _TEXT_BASE /* 测试判断是从Flash启动，还是RAM */
/* 此句执行的结果r1始终是0x30000000，因为此值是又编译器指定的(ads中设置，或-D设置编译器参数) */
    cmp r0, r1 /* 比较r0和r1，调试的时候不要执行重定位 */
3．ldr     r0, =_start
    这是一条伪指令，是一个相对程序的或外部的表达式。汇编程序将相对程序的标号表达式 label-expr 的值放在一个文字池中，并生成一个相对程序的 LDR 指令来从文字池中装载该值，在此例中生成的指令为：ldr r0, [pc, #0]，对应文字池中的地址以及值为：0x00000010: 3000000c。如果 label-expr 是一个外部表达式，或者未包含于当前段内，则汇编程序在目标文件中放置一个链接程序重定位命令。链接程序在链接时生成地址。
    因此取得的是标号 _start 的绝对地址，这个绝对地址（运行地址）是在连接的时候确定的。它要占用 2 个 32bit 的空间，一条是指令，另一条是文字池中存放_start 的绝对地址。因此可以看出，不管这段代码将来在什么地方运行，它的结果都是 r0 = 0x3000000c。由于ldr r0, =_start取得的是_start的绝对地址，这句代码可以在_start标号的绝对位置不变的情况下移动；如果使用寄存器pc在程序中可以实现绝对转移。
参考资料：
1． ARM DUI 0204BSC，RealView 编译工具 2.0 版 汇编程序指南，[http://infocenter.arm.com/help/index.jsp](http://infocenter.arm.com/help/index.jsp)
2． GNU汇编使用经验，[http://blog.chinaunix.net/u1/37614/showart_390095.html](http://blog.chinaunix.net/u1/37614/showart_390095.html)
3． 对.lds连接脚本文件的分析，[http://blog.chinaunix.net/u1/58780/showart.php?id=462971](http://blog.chinaunix.net/u1/58780/showart.php?id=462971)
转载于：[http://blog.csdn.net/denlee/archive/2008/05/31/2499542.aspx](http://blog.csdn.net/denlee/archive/2008/05/31/2499542.aspx)
