# G++ 中文使用教程 - 建建的博客 - CSDN博客
2017年03月13日 13:41:33[纪建](https://me.csdn.net/u013898698)阅读数：392

gcc and g++分别是GNU的c & c++编译器　　gcc/g++在执行编译工作的时候，总共需要4步 
　　1.预处理,生成.i的文件[预处理器cpp] 
　　2.将预处理后的文件不转换成汇编语言,生成文件.s[编译器egcs] 
　　3.有汇编变为目标代码(机器代码)生成.o的文件[汇编器as] 
　　4.连接目标代码,生成可执行程序[链接器ld] 
操作指南　　[参数详解] 
　　-x language filename 
　　设定文件所使用的语言,使后缀名无效,对以后的多个有效.也就是根 
　　据约定C语言的后缀名称是.c的，而C++的后缀名是.C或者.cpp,如果 
　　你很个性，决定你的C代码文件的后缀名是.pig 哈哈，那你就要用这 
　　个参数,这个参数对他后面的文件名都起作用，除非到了下一个参数 
　　的使用。 
　　可以使用的参数吗有下面的这些 
　　`c', `objective-c', `c-header', `c++', `cpp-output', 
　　`assembler', and `assembler-with-cpp'. 
　　看到英文，应该可以理解的。 
　　例子用法: 
　　gcc -x c hello.pig 
　　-x none filename 
　　关掉上一个选项，也就是让gcc根据文件名后缀，自动识别文件类型 
　　例子用法: 
　　gcc -x c hello.pig -x none hello2.c 
　　-c 
　　只激活预处理,编译,和汇编,也就是他只把程序做成obj文件 
　　例子用法: 
　　gcc -c hello.c 
　　他将生成.o的obj文件 
　　-S 
　　只激活预处理和编译，就是指把文件编译成为汇编代码。 
　　例子用法 
　　gcc -S hello.c 
　　他将生成.s的汇编代码，你可以用文本编辑器察看 
　　-E 
　　只激活预处理,这个不生成文件,你需要把它重定向到一个输出文件里 
　　面. 
　　例子用法: 
　　gcc -E hello.c > pianoapan.txt 
　　gcc -E hello.c | more 
　　慢慢看吧,一个hello word 也要与处理成800行的代码 
　　-o 
　　制定目标名称,缺省的时候,gcc 编译出来的文件是a.out,很难听,如果 
　　你和我有同感，改掉它,哈哈 
　　例子用法 
　　gcc -o hello.exe hello.c (哦,windows用习惯了) 
　　gcc -o hello.asm -S hello.c 
　　-pipe 
　　使用管道代替编译中临时文件,在使用非gnu汇编工具的时候,可能有些问 
　　题 
　　gcc -pipe -o hello.exe hello.c 
　　-ansi 
　　关闭gnu c中与ansi c不兼容的特性,激活ansi c的专有特性(包括禁止一 
　　些asm inline typeof关键字,以及UNIX,vax等预处理宏, 
　　-fno-asm 
　　此选项实现ansi选项的功能的一部分，它禁止将asm,inline和typeof用作 
　　关键字。 
　　-fno-strict-prototype 
　　只对g++起作用,使用这个选项,g++将对不带参数的函数,都认为是没有显式 
　　的对参数的个数和类型说明,而不是没有参数. 
　　而gcc无论是否使用这个参数,都将对没有带参数的函数,认为城没有显式说 
　　明的类型 
　　-fthis-is-varialble 
　　就是向传统c++看齐,可以使用this当一般变量使用. 
　　-fcond-mismatch 
　　允许条件表达式的第二和第三参数类型不匹配,表达式的值将为void类型 
　　-funsigned-char 
　　-fno-signed-char 
　　-fsigned-char 
　　-fno-unsigned-char 
　　这四个参数是对char类型进行设置,决定将char类型设置成unsigned char(前 
　　两个参数)或者 signed char(后两个参数) 
　　-include file 
　　包含某个代码,简单来说,就是便以某个文件,需要另一个文件的时候,就可以 
　　用它设定,功能就相当于在代码中使用#include<filename> 
　　例子用法: 
　　gcc hello.c -include /root/pianopan.h 
　　-imacros file 
　　将file文件的宏,扩展到gcc/g++的输入文件,宏定义本身并不出现在输入文件 
　　中 
　　-Dmacro 
　　相当于C语言中的#define macro 
　　-Dmacro=defn 
　　相当于C语言中的#define macro=defn 
　　-Umacro 
　　相当于C语言中的#undef macro 
　　-undef 
　　取消对任何非标准宏的定义 
　　-Idir 
　　在你是用#include"file"的时候,gcc/g++会先在当前目录查找你所制定的头 
　　文件,如果没有找到,他回到缺省的头文件目录找,如果使用-I制定了目录,他 
　　回先在你所制定的目录查找,然后再按常规的顺序去找. 
　　对于#include<file>,gcc/g++会到-I制定的目录查找,查找不到,然后将到系 
　　统的缺省的头文件目录查找 
　　-I- 
　　就是取消前一个参数的功能,所以一般在-Idir之后使用 
　　-idirafter dir 
　　在-I的目录里面查找失败,讲到这个目录里面查找. 
　　-iprefix prefix 
　　-iwithprefix dir 
　　一般一起使用,当-I的目录查找失败,会到prefix+dir下查找 
　　-nostdinc 
　　使编译器不再系统缺省的头文件目录里面找头文件,一般和-I联合使用,明确 
　　限定头文件的位置 
　　-nostdin C++ 
　　规定不在g++指定的标准路经中搜索,但仍在其他路径中搜索,.此选项在创建 
　　libg++库使用 
　　-C 
　　在预处理的时候,不删除注释信息,一般和-E使用,有时候分析程序，用这个很 
　　方便的 
　　-M 
　　生成文件关联的信息。包含目标文件所依赖的所有源代码 
　　你可以用gcc -M hello.c来测试一下，很简单。 
　　-MM 
　　和上面的那个一样，但是它将忽略由#include<file>造成的依赖关系。 
　　-MD 
　　和-M相同，但是输出将导入到.d的文件里面 
　　-MMD 
　　和-MM相同，但是输出将导入到.d的文件里面 
　　-Wa,option 
　　此选项传递option给汇编程序;如果option中间有逗号,就将option分成多个选 
　　项,然后传递给会汇编程序 
　　-Wl.option 
　　此选项传递option给连接程序;如果option中间有逗号,就将option分成多个选 
　　项,然后传递给会连接程序. 
　　-llibrary 
　　制定编译的时候使用的库 
　　例子用法 
　　gcc -lcurses hello.c 
　　使用ncurses库编译程序 
　　-Ldir 
　　制定编译的时候，搜索库的路径。比如你自己的库，可以用它制定目录，不然 
　　编译器将只在标准库的目录找。这个dir就是目录的名称。 
　　-O0 
　　-O1 
　　-O2 
　　-O3 
　　编译器的优化选项的4个级别，-O0表示没有优化,-O1为缺省值，-O3优化级别最 
　　高　 
　　-g 
　　只是编译器，在编译的时候，产生调试信息。 
　　-gstabs 
　　此选项以stabs格式声称调试信息,但是不包括gdb调试信息. 
　　-gstabs+ 
　　此选项以stabs格式声称调试信息,并且包含仅供gdb使用的额外调试信息. 
　　-ggdb 
　　此选项将尽可能的生成gdb的可以使用的调试信息. 
　　-static 
　　此选项将禁止使用动态库，所以，编译出来的东西，一般都很大，也不需要什么 
　　动态连接库，就可以运行. 
　　-share 
　　此选项将尽量使用动态库，所以生成文件比较小，但是需要系统由动态库. 
　　-traditional 
　　试图让编译器支持传统的C语言特性 
　　GNU 的调试器称为 gdb，该程序是一个交互式工具，工作在字符模式。在 X Window 系统中，有一个 gdb 的 
　　前端图形工具，称为 xxgdb。gdb 是功能强大的调试程序，可完成如下的调试任务： 
　　* 设置断点； 
　　* 监视程序变量的值； 
　　* 程序的单步执行； 
　　* 修改变量的值。 
　　在可以使用 gdb 调试程序之前，必须使用 -g 选项编译源文件。可在 makefile 中如下定义 CFLAGS 变量： 
　　CFLAGS = -g 
　　运行 gdb 调试程序时通常使用如下的命令： 
　　gdb progname 
　　在 gdb 提示符处键入help，将列出命令的分类，主要的分类有： 
　　* aliases：命令别名 
　　* breakpoints：断点定义； 
　　* data：数据查看； 
　　* files：指定并查看文件； 
　　* internals：维护命令； 
　　* running：程序执行； 
　　* stack：调用栈查看； 
　　* statu：状态查看； 
　　* tracepoints：跟踪程序执行。 
　　键入 help 后跟命令的分类名，可获得该类命令的详细清单。 
　　#DENO# 
　　gdb 的常用命令 
　　表 1-4 常用的 gdb 命令 
　　命令 解释 
　　break NUM 在指定的行上设置断点。 
　　bt 显示所有的调用栈帧。该命令可用来显示函数的调用顺序。 
　　clear 删除设置在特定源文件、特定行上的断点。其用法为：clear FILENAME:NUM。 
　　continue 继续执行正在调试的程序。该命令用在程序由于处理信号或断点而 
　　导致停止运行时。 
　　display EXPR 每次程序停止后显示表达式的值。表达式由程序定义的变量组成。 
　　file FILE 装载指定的可执行文件进行调试。 
　　help NAME 显示指定命令的帮助信息。 
　　info break 显示当前断点清单，包括到达断点处的次数等。 
　　info files 显示被调试文件的详细信息。 
　　info func 显示所有的函数名称。 
　　info local 显示当函数中的局部变量信息。 
　　info prog 显示被调试程序的执行状态。 
　　info var 显示所有的全局和静态变量名称。 
　　kill 终止正被调试的程序。 
　　list 显示源代码段。 
　　make 在不退出 gdb 的情况下运行 make 工具。 
　　next 在不单步执行进入其他函数的情况下，向前执行一行源代码。 
　　print EXPR 显示表达式 EXPR 的值。 
　　1.8.5 gdb 使用范例 
　　----------------- 
　　清单 一个有错误的 C 源程序 bugging.c 
　　----------------- 
　　#include 
　　#include 
　　static char buff [256]; 
　　static char* string; 
　　int main () 
　　{ 
　　printf ("Please input a string: "); 
　　gets (string); 
　　printf ("\nYour string is: %s\n", string); 
　　} 
　　----------------- 
　　上面这个程序非常简单，其目的是接受用户的输入，然后将用户的输入打印出来。该程序使用了一个未经过初 
　　始化的字符串地址 string，因此，编译并运行之后，将出现 Segment Fault 错误： 
　　$ gcc -o test -g test.c 
　　$ ./test 
　　Please input a string: asfd 
　　Segmentation fault (core dumped) 
　　为了查找该程序中出现的问题，我们利用 gdb，并按如下的步骤进行： 
　　1．运行 gdb bugging 命令，装入 bugging 可执行文件； 
　　2．执行装入的 bugging 命令； 
　　3．使用 where 命令查看程序出错的地方； 
　　4．利用 list 命令查看调用 gets 函数附近的代码； 
　　5．唯一能够导致 gets 函数出错的因素就是变量 string。用 print 命令查看 string 的值； 
　　6．在 gdb 中，我们可以直接修改变量的值，只要将 string 取一个合法的指针值就可以了，为此，我们在第 
　　11 行处设置断点； 
　　7．程序重新运行到第 11 行处停止，这时，我们可以用 set variable 命令修改 string 的取值； 
　　8．然后继续运行，将看到正确的程序运行结果。
[](http://blog.csdn.net/gogiqp_jyh/article/details/17359107#)[](http://blog.csdn.net/gogiqp_jyh/article/details/17359107#)[](http://blog.csdn.net/gogiqp_jyh/article/details/17359107#)[](http://blog.csdn.net/gogiqp_jyh/article/details/17359107#)[](http://blog.csdn.net/gogiqp_jyh/article/details/17359107#)
