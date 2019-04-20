# GNU ARM 汇编指令[转载] - h13 - 博客园
http://blog.sina.com.cn/s/blog_59b189220100au1k.html
第一部分 
Linux下ARM汇编语法尽管在Linux下使用C或C++编写程序很方便，但汇编源程序用于系统最基本的初始化，如初始化堆栈指针、设置页表、操作 ARM的协处理器等。初始化完成后就可以跳转到C代码执行。需要注意的是，GNU的汇编器遵循AT&T的汇编语法，可以从GNU的站点（www.gnu.org）上下载有关规范。
一. Linux汇编行结构
任何汇编行都是如下结构：
[:] [} @ comment
[:] [} @ 注释
Linux ARM 汇编中，任何以冒号结尾的标识符都被认为是一个标号，而不一定非要在一行的开始。
【例1】定义一个"add"的函数，返回两个参数的和。
.section .text, “x”
.global add @ give the symbol add external linkage
add:
ADD r0, r0, r1 @ add input arguments
MOV pc, lr @ return from subroutine
@ end of program
二. Linux 汇编程序中的标号
标号只能由a～z，A～Z，0～9，“.”，_等字符组成。当标号为0～9的数字时为局部标号，局部标号可以重复出现,使用方法如下：
标号f: 在引用的地方向前的标号
标号b: 在引用的地方向后的标号
【例2】使用局部符号的例子，一段循环程序
1:
 subs r0,r0,#1 @每次循环使r0=r0-1
 bne 1f @跳转到1标号去执行
局部标号代表它所在的地址,因此也可以当作变量或者函数来使用。
三. Linux汇编程序中的分段
（1）.section伪操作
用户可以通过.section伪操作来自定义一个段,格式如下:
.section section_name [, "flags"[, %type[,flag_specific_arguments]]]
每一个段以段名为开始, 以下一个段名或者文件结尾为结束。这些段都有缺省的标志（flags）,连接器可以识别这些标志。(与armasm中的AREA相同)。
下面是ELF格式允许的段标志
<标志> 含义
a 允许段
w 可写段
x 执行段
【例3】定义段
.section .mysection @自定义数据段，段名为 “.mysection”
.align 2
strtemp:
.ascii "Temp string \n\0"
（2）汇编系统预定义的段名
.text @代码段
.data @初始化数据段
.bss @未初始化数据段
.sdata @
.sbss @
需要注意的是，源程序中.bss段应该在.text之前。
四. 定义入口点
汇编程序的缺省入口是 start标号，用户也可以在连接脚本文件中用ENTRY标志指明其它入口点。
【例4】定义入口点
.section.data
< initialized data here>
.section .bss
< uninitialized data here>
.section .text
.globl _start
_start:
<instruction code goes here>
五. Linux汇编程序中的宏定义
格式如下:
.macro 宏名 参数名列表 @伪指令.macro定义一个宏
 宏体
.endm @.endm表示宏结束
如果宏使用参数,那么在宏体中使用该参数时添加前缀“\”。宏定义时的参数还可以使用默认值。
可以使用.exitm伪指令来退出宏。
【例5】宏定义
.macro SHIFTLEFT a, b
.if \b < 0
MOV \a, \a, ASR #-\b
.exitm
.endif
MOV \a, \a, LSL #\b
.endm
六. Linux汇编程序中的常数
（1）十进制数以非0数字开头,如:123和9876；
（2）二进制数以0b开头,其中字母也可以为大写；
（3）八进制数以0开始,如:0456,0123；
（4）十六进制数以0x开头,如:0xabcd,0X123f；
（5）字符串常量需要用引号括起来,中间也可以使用转义字符,如: “You are welcome!\n”；
（6）当前地址以“.”表示,在汇编程序中可以使用这个符号代表当前指令的地址；
（7）表达式:在汇编程序中的表达式可以使用常数或者数值, “-”表示取负数, “~”表示取补,“<>”表示不相等,其他的符号如:+、-、*、 /、%、<、<<、>、>>、|、&、^、!、==、>=、<=、&&、|| 跟C语言中的用法相似。
七. Linux下ARM汇编的常用伪操作
在前面已经提到过了一些为操作，还有下面一些为操作：
数据定义伪操作： .byte，.short，.long，.quad，.float，.string/.asciz/.ascii，重复定义伪操作.rept，赋值语句.equ/.set ；
函数的定义 ；
对齐方式伪操作 .align；
源文件结束伪操作.end；
.include伪操作；
if伪操作；
.global/ .globl 伪操作 ；
.type伪操作 ；
列表控制语句 ；
区别于gas汇编的通用伪操作,下面是ARM特有的伪操作 ：.reg ，.unreq ，.code ，.thumb ，.thumb_func ，.thumb_set， .ltorg ，.pool
1. 数据定义伪操作
（1） .byte:单字节定义，如：.byte 1,2,0b01,0x34,072,'s' ；
（2） .short:定义双字节数据，如:.short 0x1234,60000 ；
（3） .long:定义4字节数据，如:.long 0x12345678,23876565
（4） .quad:定义8字节，如:.quad 0x1234567890abcd
（5） .float：定义浮点数，如:
 .float 0f-314159265358979323846264338327\
 95028841971.693993751E-40 @ - pi
（6） .string/.asciz/.ascii：定义多个字符串，如:
 .string "abcd", "efgh", "hello!"
 .asciz "qwer", "sun", "world!"
 .ascii "welcome\0"
需要注意的是：.ascii伪操作定义的字符串需要自行添加结尾字符'\0'。
（7） .rept:重复定义伪操作, 格式如下:
 .rept 重复次数
 数据定义
 .endr @结束重复定义
 例如:
 .rept 3
 .byte 0x23
 .endr
（8） .equ/.set: 赋值语句, 格式如下:
 .equ(.set) 变量名,表达式
 例如:
 .equ abc 3 @让abc=3
2.函数的定义伪操作
（1）函数的定义,格式如下:
 函数名:
 函数体
 返回语句
一般的,函数如果需要在其他文件中调用, 需要用到.global伪操作将函数声明为全局函数。为了不至于在其他程序在调用某个C函数时发生混乱,对寄存器的使用我们需要遵循APCS准则。函数编译器将处理为函数代码为一段.global的汇编码。
（2）函数的编写应当遵循如下规则:
 a1-a4寄存器（参数、结果或暂存寄存器，r0到r3 的同义字）以及浮点寄存器f0-f3(如果存在浮点协处理器)在函数中是不必保存的；
 如果函数返回一个不大于一个字大小的值，则在函数结束时应该把这个值送到 r0 中；
 如果函数返回一个浮点数，则在函数结束时把它放入浮点寄存器f0中；
 如果函数的过程改动了sp（堆栈指针，r13）、fp（框架指针，r11）、sl（堆栈限制，r10）、lr（连接寄存器，r14）、v1-v8（变量寄存器，r4 到 r11）和 f4-f7,那么函数结束时这些寄存器应当被恢复为包含在进入函数时它所持有的值。
3. .align .end .include .incbin伪操作
（1）.align:用来指定数据的对齐方式,格式如下:
 .align [absexpr1, absexpr2]
 以某种对齐方式,在未使用的存储区域填充值. 第一个值表示对齐方式,4, 8,16或 32. 第二个表达式值表示填充的值。
（2）.end:表明源文件的结束。
（3）.include:可以将指定的文件在使用.include 的地方展开,一般是头文件,例如:
 .include “myarmasm.h”
（4）.incbin伪操作可以将原封不动的一个二进制文件编译到当前文件中,使用方法如下:
 .incbin "file"[,skip[,count]]
 skip表明是从文件开始跳过skip个字节开始读取文件,count是读取的字数.
4. .if伪操作
根据一个表达式的值来决定是否要编译下面的代码, 用.endif伪操作来表示条件判断的结束, 中间可以使用.else来决定.if的条件不满足的情况下应该编译哪一部分代码。
.if有多个变种:
.ifdef symbol @判断symbol是否定义
.ifc string1,string2 @字符串string1和string2是否相等,字符串可以用单引号括起来
.ifeq expression @判断expression的值是否为0
.ifeqs string1,string2 @判断string1和string2是否相等,字符 串必须用双引号括起来
.ifge expression @判断expression的值是否大于等于0
.ifgt absolute expression @判断expression的值是否大于0
.ifle expression @判断expression的值是否小于等于0
.iflt absolute expression @判断expression的值是否小于0
.ifnc string1,string2 @判断string1和string2是否不相等, 其用法跟.ifc恰好相反。
.ifndef symbol, .ifnotdef symbol @判断是否没有定义symbol, 跟.ifdef恰好相反
.ifne expression @如果expression的值不是0, 那么编译器将编译下面的代码
.ifnes string1,string2 @如果字符串string1和string2不相 等, 那么编译器将编译下面的代码.
5. .global .type .title .list
（1）.global/ .globl ：用来定义一个全局的符号，格式如下:
 .global symbol 或者 .globl symbol
（2）.type：用来指定一个符号的类型是函数类型或者是对象类型, 对象类型一般是数据, 格式如下:
 .type 符号, 类型描述
【例6】
.globl a
.data
.align 4
.type a, @object
.size a, 4
a:
.long 10
【例7】
.section .text
.type asmfunc, @function
.globl asmfunc
asmfunc:
mov pc, lr
（3）列表控制语句:
.title：用来指定汇编列表的标题,例如:
 .title “my program”
.list：用来输出列表文件.
6. ARM特有的伪操作
（1） .reg: 用来给寄存器赋予别名,格式如下:
 别名 .req 寄存器名
（2） .unreq: 用来取消一个寄存器的别名,格式如下:
　　　　　　　.unreq 寄存器别名
　　注意被取消的别名必须事先定义过,否则编译器就会报错,这个伪操作也可以用来取消系统预制的别名, 例如r0, 但如果没有必要的话不推荐那样做。
（3） .code伪操作用来选择ARM或者Thumb指令集,格式如下:
　　　　　　　　　　　.code 表达式
　　如果表达式的值为16则表明下面的指令为Thumb指令,如果表达式的值为32则表明下面的指令为ARM指令.
（4） .thumb伪操作等同于.code 16, 表明使用Thumb指令, 类似的.arm等同于.code 32
（5） .force_thumb伪操作用来强制目标处理器选择thumb的指令集而不管处理器是否支持
（6） .thumb_func伪操作用来指明一个函数是thumb指令集的函数
（7） .thumb_set伪操作的作用类似于.set, 可以用来给一个标志起一个别名, 比.set功能增加的一点是可以把一个标志标记为thumb函数的入口, 这点功能等同于.thumb_func
（8） .ltorg用于声明一个数据缓冲池(literal pool)的开始,它可以分配很大的空间。
（9） .pool的作用等同.ltorg
（9）.space <number_of_bytes> {,<fill_byte>}
分配number_of_bytes字节的数据空间，并填充其值为fill_byte，若未指定该值，缺省填充0。（与armasm中的SPACE功能相同）
（10）.word <word1> {,<word2>} …
插入一个32-bit的数据队列。（与armasm中的DCD功能相同）
可以使用.word把标识符作为常量使用
例如：
 Start:
 valueOfStart:
 .word Start
这样程序的开头Start便被存入了内存变量valueOfStart中。
（11）.hword <short1> {,<short2>} …
插入一个16-bit的数据队列。（与armasm中的DCW相同）
八. GNU ARM汇编特殊字符和语法
代码行中的注释符号: ‘@’
整行注释符号: ‘#’
语句分离符号: ‘;’
直接操作数前缀: ‘#’ 或 ‘$’
第二部分 GNU的编译器和调试工具
一. 编译工具
1．编辑工具介绍
GNU 提供的编译工具包括汇编器as、C编译器gcc、C++编译器g++、连接器ld和二进制转换工具objcopy。基于ARM平台的工具分别为arm- linux-as、arm-linux-gcc、arm-linux-g++、arm-linux-ld和arm-linux- objcopy。GNU的编译器功能非常强大，共有上百个操作选项，这也是这类工具让初学者头痛的原因。不过，实际开发中只需要用到有限的几个，大部分可以采用缺省选项。GNU工具的开发流程如下：编写C、C++语言或汇编源程序，用gcc或g++生成目标文件，编写连接脚本文件，用连接器生成最终目标文件（elf格式），用二进制转换工具生成可下载的二进制代码。
（1）编写C、C++语言或汇编源程序
通常汇编源程序用于系统最基本的初始化，如初始化堆栈指针、设置页表、操作ARM的协处理器等。初始化完成后就可以跳转到C代码执行。需要注意的是，GNU的汇编器遵循AT&T的汇编语法，读者可以从GNU的站点（www.gnu.org）上下载有关规范。汇编程序的缺省入口是 start标号，用户也可以在连接脚本文件中用ENTRY标志指明其它入口点（见下文关于连接脚本的说明）。
（2）用gcc或g++生成目标文件
如果应用程序包括多个文件，就需要进行分别编译，最后用连接器连接起来。如笔者的引导程序包括3个文件：init.s（汇编代码、初始化硬件）xmrecever.c（通信模块，采用Xmode协议）和flash.c（Flash擦写模块）。
分别用如下命令生成目标文件： arm-linux-gcc-c-O2-oinit.oinit.s arm-linux-gcc-c-O2-oxmrecever.oxmrecever.c arm-linux-gcc-c-O2-oflash.oflash.c 其中-c命令表示只生成目标代码，不进行连接；-o命令指明目标文件的名称；-O2表示采用二级优化，采用优化后可使生成的代码更短，运行速度更快。如果项目包含很多文件，则需要编写makefile文件。关于makefile的内容，请感兴趣的读者参考相关资料。
（3）编写连接脚本文件
gcc 等编译器内置有缺省的连接脚本。如果采用缺省脚本，则生成的目标代码需要操作系统才能加载运行。为了能在嵌入式系统上直接运行，需要编写自己的连接脚本文件。编写连接脚本，首先要对目标文件的格式有一定了解。GNU编译器生成的目标文件缺省为elf格式。elf文件由若干段（section）组成，如不特殊指明，由C源程序生成的目标代码中包含如下段：.text（正文段）包含程序的指令代码；.data(数据段)包含固定的数据，如常量、字符串；.bss（未初始化数据段）包含未初始化的变量、数组等。C++源程序生成的目标代码中还包括.fini（析构函数代码）和. init（构造函数代码）等。连接器的任务就是将多个目标文件的.text、.data和.bss等段连接在一起，而连接脚本文件是告诉连接器从什么地址开始放置这些段。例如连接文件link.lds为：
ENTRY(begin)
SECTION
{
.=0x30000000;
.text:{*(.text)}
.data:{*(.data)}
.bss:{*(.bss)}
}
其中，ENTRY(begin)指明程序的入口点为begin标号；.=0x00300000指明目标代码的起始地址为0x30000000，这一段地址为 MX1的片内RAM；.text:{*(.text)}表示从0x30000000开始放置所有目标文件的代码段，随后的.data:{* (.data)}表示数据段从代码段的末尾开始，再后是.bss段。
（4）用连接器生成最终目标文件
有了连接脚本文件，如下命令可生成最终的目标文件：
arm-linux-ld –no stadlib –o bootstrap.elf -Tlink.lds init.o xmrecever.o flash.o
其中，ostadlib表示不连接系统的运行库，而是直接从begin入口；-o指明目标文件的名称；-T指明采用的连接脚本文件（也可以使用-Ttext address，address表示执行区地址）；最后是需要连接的目标文件列表。
（5）生成二进制代码
连接生成的elf文件还不能直接下载执行，通过objcopy工具可生成最终的二进制文件：
arm-linux-objcopy –O binary bootstrap.elf bootstrap.bin
其中-O binary指定生成为二进制格式文件。Objcopy还可以生成S格式的文件，只需将参数换成-O srec。还可以使用-S选项，移除所有的符号信息及重定位信息。如果想将生成的目标代码反汇编，还可以用objdump工具：
arm-linux-objdump -D bootstrap.elf
至此，所生成的目标文件就可以直接写入Flash中运行了。
2．Makefile实例
example: head.s main.c
arm-linux-gcc -c -o head.o head.s
arm-linux-gcc -c -o main.o main.c
arm-linux-ld -Tlink.lds head.o ain.o -o example.elf
arm-linux-objcopy -O binary -S example_tmp.o example
arm-linux-objdump -D -b binary -m arm example >ttt.s
二. 调试工具
Linux 下的GNU调试工具主要是gdb、gdbserver和kgdb。其中gdb和gdbserver可完成对目标板上Linux下应用程序的远程调试。 gdbserver是一个很小的应用程序，运行于目标板上，可监控被调试进程的运行，并通过串口与上位机上的gdb通信。开发者可以通过上位机的gdb输入命令，控制目标板上进程的运行，查看内存和寄存器的内容。gdb5.1.1以后的版本加入了对ARM处理器的支持，在初始化时加入－ target==arm参数可直接生成基于ARM平台的gdbserver。gdb工具可以从ftp: //ftp.gnu.org/pub/gnu/gdb/上下载。
对于Linux内核的调试，可以采用kgdb工具，同样需要通过串口与上位机上的gdb通信，对目标板的Linux内核进行调试。可以从http://oss.sgi.com/projects/kgdb/上了解具体的使用方法。
参考资料：
1. Richard Blum，Professional Assembly Language
2. GNU ARM 汇编快速入门，http://blog.chinaunix.net/u/31996/showart.php?id=326146
3. ARM GNU 汇编伪指令简介，http://www.cppblog.com/jb8164/archive/2008/01/22/41661.aspx
4. GNU汇编使用经验，http://blog.chinaunix.net/u1/37614/showart_390095.html
5. GNU的编译器和开发工具，http://blog.ccidnet.com/blog-htm-do-showone-uid-34335-itemid-81387-type-blog.html
6. 用GNU工具开发基于ARM的嵌入式系统，http://blog.163.com/liren0@126/blog/static/32897598200821211144696/
7. objcopy命令介绍，http://blog.csdn.net/junhua198310/archive/2007/06/27/1669545.aspx
ADS汇编和Gnu汇编的转换转自[http://zhangwenxin82.blog.163.com/blog/static/1145959562009324111541596/](http://zhangwenxin82.blog.163.com/blog/static/1145959562009324111541596/)
ADS汇编格式转自[http://blog.csdn.net/jiacky/archive/2009/06/05/4245087.aspx](http://blog.csdn.net/jiacky/archive/2009/06/05/4245087.aspx)
将ARM ADS下的汇编码移植到GCC for ARM编译器时，有如下规则：
1, 注释行以"@"或""代替";"
2, GET或INCLUDE => .INCLUDE
如：get option.a =>     .include "option.a"
3, EQU => .equ
TCLK2   EQU   PB25    =>         .equ   TCLK2, PB25
     SETA ==> .equ
     SETL ==> .equ
BUSWIDTH SETA 16   => .equ BUSWIDTH, 16
4, EXPORT => .global
     IMPORT => .extern
     GBLL => .global
     GBLA => .global
5, DCD => .long
6, IF :DEF: => .IFDEF
     ELSE => .ELSE
     ENDIF => .ENDIF
     :OR:    => |
     :SHL:    =>      <<
7,   END    =>.end
NOTE:在被include的头文件中，如"option.a"中，不再需要.end，否则会导致主汇编程序结束。
8,   符号定义加"："号
Entry =>    Entry:
AREA Word, CODE, READONLY ==> .text
AREA Block, DATA, READWRITE ==> .data
CODE32     ==> .arm
CODE16     ==> .thumb
9,   MACRO ==>   .macro
     MEND   ==> .endm

