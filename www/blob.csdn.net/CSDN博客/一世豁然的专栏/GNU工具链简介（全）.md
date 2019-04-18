# GNU工具链简介（全） - 一世豁然的专栏 - CSDN博客





2015年05月05日 09:50:26[一世豁然](https://me.csdn.net/Explorer_day)阅读数：740








本博客转载于：[http://blog.chinaunix.net/uid-2630593-id-2550527.html](http://blog.chinaunix.net/uid-2630593-id-2550527.html)








**Mips GNU工具链简介**



**1 . 总括 **

本文分三部分来介绍mips的工具链，首先工具链的总括，其次是每个工具链的简介，最后是各工具链的实际应用举例。我们的项目使用的工具链就如图1所示,
声明，本文所述的内容实乃GNU工具链的九牛之一毛, 日后会陆续更新。





图1

**2. 工具链简介 **

下边就以列表的方式，对每一个工具链的作用进行介绍，在下一章进行实例演示。



***mips-linux-gnu-addr2line :***

把程序地址转换为文件名和行号。在命令行中给它一个地址和一个可执行文件名，

它就会使用这个可执行文件的调试信息指出在给出的地址上是哪个文件以及行号。

***mips-linux-gnu-gcc:***

符合ISO标准的C编译器,
这个大家都在用，不再赘述。

***mips-linux-gnu-objcopy:***

把一种目标文件中的内容复制到另一种类型的目标文件中。

***mips-linux-gnu-ar:***

建立、修改、提取归档文件。归档文件是包含多个文件内容的一个大文件，其结构

保证了可以恢复原始文件内容。

***mips-linux-gnu-gcov:***

gcov是一个保险测试工具。当构建一个程序时，gcov会监视一个程序的执行，并且会标识出执行了哪一行源码，哪一行没有执行。

***mips-linux-gnu-objdump:***

显示一个或者更多目标文件的信息。使用选项来控制其显示的信息。它所显示的信息通常只有编写编译工具的人才感兴趣。

***mips-linux-gnu-as:***

是 GNU 汇编器，主要用来编译 GNU C
编译器 gcc 输出的汇编文件，它将汇编代码转换成二进制代码，并存放到一个object
文件中，该目标文件将由连接器 ld 连接

***mips-linux-gnu-gdb:***

GNU调试器。允许调试用C\C++和其他语言编写的应用程序。它的基本运行方式是在shell环境下用命令方式进行调试程序和显示数据。如果加上一些图形前端（如DDD等软件），则可以在一个更方便的图形环境下调试程序

***mips-linux-gnu-ranlib:***

产生归档文件索引，并将其保存到这个归档文件中。在索引中列出了归档文件各成员所定义的可重分配目标文件。

***mips-linux-gnu-c++:***

标准的C++编译器。

***mips-linux-gnu-gdbtui:***

                   GDB调试器的文本用户界面。

***mips-linux-gnu-readelf:***

显示 elf
格式可执行文件的信息

***mips-linux-gnu-c++filt:***

解码 C++ 符号名，连接器使用它来过滤 C++
和 Java 符号，防止重载函数冲突。

***mips-linux-gnu-gprof:***

显示程序调用段的各种数据,
包括时间和次数等。

***mips-linux-gnu-size:***

列出目标文件每一段的大小以及总体的大小。默认情况下，对于每个目标文件或者一个归档文件中的每个模块只产生一行输出。

***mips-linux-gnu-cpp:***

GNU预处理器，主要用于扩展用户源程序的头文件和宏定义。cpp是GNU C编译器的预处理器。cpp是一个宏指令处理器，GNU
 C编译器在编译前自动使用cpp对用户程序进行转换。cpp之所以被称为宏指令处理器是因为它允许用户定义宏指令（长结构的简写）。

***mips-linux-gnu-ld:***

GNU链接器，主要用于确定相对地址，把多个object文件、起始代码段、库等链接起来，并最终形成一个可执行文件。

***mips-linux-gnu-strings:***

打印某个文件的可打印字符串，这些字符串最少 4 个字符长，也可以使用选项 -n
设置字符串的最小长度。默认情况下，它只打印目标文件初始化和可加载段中的可打印字符；对于其它类型的文件它打印整个文件的可打印字符，这个程序对于了解非文本文件的内容很有帮助。

***mips-linux-gnu-g++:***

基本符合ISO标准的C++编译器。

***mips-linux-gnu-nm:***

列出object文件中的符号

***mips-linux-gnu-strip:***

丢弃目标文件中的全部或者特定符号。



**3. 工具使用举例:**



   对于GNU工具链中的GCC, G++, C++等不再举例。



***m**ips-linux-gnu-ar:***

ar 用于建立、修改、提取归档文件，一个归档文件，是包含多个被包含文件的单个文件（也可以认为归档文件是一个库文件）。

被包含的原始文件的内容、权限、时间戳、所有者等属性都保存在归档文件中，并且在提取之后可以还原



主要参数：

r - replace existing or insert new ﬁle(s) into the archive

v - be verbose



生成归档文件的方法:

         Mips-linux-gnu-ar –rv libtest.a inputa.o inputb.o



***mips-linux-gnu-ranlib:***

这又是一个历史遗留问题，在早期版本中，ar只是将.o打包到.a,
而不处理里边的符号表，linker需要一个完整的符号表，所以当时就单独写了一个ranlib来产生linker需要的符号表。后来产生完整符号表的功能被集成到ar中了，为了兼容，故ranlib还存在，总之以后尽量不要使用ranlib。



生成完整符号表的方法:

                   Mips-linux-gnu-ranlib libtest.a



***mips-linux-gnu-nm:***

nm 的主要功能是列出目标文件中的符号，这样就可以定位和分析执行程序和目标文件中的符号信息和它的属性。



生成符号前缀的含义:

A：符号的值是绝对值，并且不会被将来的链接所改变

B：符号位于未初始化数据部分（BSS
段）

C：符号是公共的。公共符号是未初始化的数据。在链接时，多个公共符号可能以相同的名字出现。如果符号在其他地方被定义，则该文件中的这个符号会被当作引用来处理

D：符号位于已初始化的数据部分

T：符号位于代码部分

U：符号未被定义？：符号类型未知，或者目标文件格式特殊



使用方法:

                            Mips-linux-gnu-nm –A libtest.a



***mips-linux-gnu-gcov:***

gcov会监视一个程序的执行，并且会标识出执行了哪一行源码，哪一行没有执行。更进一步，gcov可以标识出某一行源执行的次数，这对于执行配置很有用（程序在哪里花费了大多数的时间）



**gcov的参数:**

-b,-branch-probabilities向输出文件输出分支频度
-c,-branch-counts    打印分支计数而不是分支频度
-n,-no-output        不创建gcov输出文件
-l,-long-file-names    创建长文件名
-f,-function-summaries    打印每一个函数的概要
-o,-object-directory    .bb,.bbg,.da文件存放的目录



**Gcov实例(使用的gcc测试):**

         Mips-linux-gnu-gcc a.c -o a -ftesage -fprofile-arcs

         ./a                                       
执行a文件//

         Mips-linux-gnu-gcov a.c //会自动生成a.c.gcov文件

File 'a.c'

Lines executed:83.33% of 6

a.c:creating 'a.c.gcov'

vi a.c.gcov  //就可以看到代码的执行次数

mips-linux-gnu-gcov –b a.c  //可以查看gcov的分支执行频率



***mips-linux-gnu-c++filt:***

大家都知道在C++和JAVA中都有函数重载的机制，编译器在编译相同重载函数时，会使用name mangling机制对函数重新命名，而链接器连接的时候就需要对此进行解析。



C++filt常用参数:

-_        滤掉符号前的下划线
    -j        使用Java语法，默认为C++

    -n        不要滤掉符号前的下划线
    -p        不要打印函数的参数类型
    -s        指定解码方式，不同编译器的编码方式不同。



                   C++filt 使用实例:

                            dyron@harris-desktop:~/misc/module/test/tmp$ mips-linux-gnu-nm filt | grep func

080484d8 T _Z4funci

080484c4 T _Z4funcv



dyron@harris-desktop:~/misc/module/test/tmp$ mips-linux-gnu-nm filt | grep func | mips-linux-gnu-c++filt

080484d8 T func(int)

080484c4 T func()



***mips-linux-gnu-strip:***

strip经常用来去除目标文件中的一些符号表、调试符号表信息，以减小程序的

大小，在rpmbuild包的最后就用到。

值得注意的是，在动态库和静态库时去掉符号表可以给文件瘦身，但再链接时可能编译不过去，所以strip慎用。



         Strip使用实例:

                   dyron@harris-desktop:~/misc/module/test/tmp$ ls -l filt

-rwxr-xr-x 1 dyron dyron 7311 2011-08-31 10:29 filt



dyron@harris-desktop:~/misc/module/test/tmp$ mips-linux-gnu-strip filt



dyron@harris-desktop:~/misc/module/test/tmp$ ls -l filt

-rwxr-xr-x 1 dyron dyron 5552 2011-08-31 10:55 filt



***mips-linux-gnu-gprof:***

打印出程序运行中各个函数消耗的时间，可以帮助程序员找出众多函数中耗时最多的函数。产生程序运行时候的函数调用关系，包括调用次数，可以帮助程序员分析程序的运行流程。

Gprof的原理就是在加上-pg选项上，在编译和链接的时候，编译器在每个函数中都加入了一个名为gcount的函数，每个运行函数都会调用它，用来在内存中生成函数调用图。非常有用的一个工具，这里只是一个皮毛，等下次深入分析它的线程。



Gporf参数简介:

         -p得到每个函数占用的执行时间

         -q得到call graph，包含了每个函数的调用关系，调用次数，执行时间等信息

-A得到一个带注释的“源代码清单”，它会注释源码，指出每个函数的执行次数。这需要在编译的时候增加 -g选项。



         Gprof使用实例:



dyron@harris-desktop:~/misc/module/test/tmp$ gcc -pg a.c -o filt

dyron@harris-desktop:~/misc/module/test/tmp$ gprof -b filt gmon.out

Flat profile:



Each sample counts as 0.01 seconds.

 no time accumulated



  %   cumulative   self              self     total

 time   seconds   seconds    calls  Ts/call  Ts/call  name

  0.00      0.00     0.00        1     0.00     0.00  a

  0.00      0.00     0.00        1     0.00     0.00  b

  0.00      0.00     0.00        1     0.00     0.00  c



                        Call graph





granularity: each sample hit covers 4 byte(s) no time propagated



index % time    self  children    called     name

                0.00    0.00       1/1           b [2]

[1]      0.0    0.00    0.00       1         a [1]

-----------------------------------------------

                0.00    0.00       1/1           main [7]

[2]      0.0    0.00    0.00       1         b [2]

                0.00    0.00       1/1           c [3]

                0.00    0.00       1/1           a [1]

-----------------------------------------------

                0.00    0.00       1/1           b [2]

[3]      0.0    0.00    0.00       1         c [3]

-----------------------------------------------



Index by function name



   [1] a                       [2] b                       [3] c





***mips-linux-gnu-size :***

显示一个目标文件或者链接库文件中的目标文件的各个段的大小。



                   Size的参数简介:

                            -A –B:      
选择以sysv|berkeley的样式输出。

                            -o|-d|-x : 
选择输出数据的格式. 

                            -t :            
选择全部输出。



                  Size 的使用实例:

                            [tulip]dyron@harris-desktop:~/misc/module/test/tmp$ mips-linux-gnu-size filt

   text    data     bss     dec     hex filename

   1477     280      12    1769     6e9 filt



***mips-linux-gnu-as:***

GNU AS是GNU汇编器，主要用于把汇编代码转换成二进制代码，并存放到一个object文件中。

大家都知道从源码到程序分为4个过程，AS做的工作就是第3步，将汇编指令码转换为二进制的文件。



As 的使用实例(可用gcc-c hello.s –o hello.o代替)：

Mips-linux-gnu-as hello.s –o hello.o



***mips-linux-gnu-strings:***

strings的功参简单，就是显示文件中的可打印字符，常用来在[二进制](http://os.51cto.com/art/200709/56622.htm)文件中查找字符串。



                   Strings的参数简介:

-a          不只是扫描目标文件初始化和装载段,
而是扫描整个文件.

       -f              在显示字符串之前先显示文件名.

       -n min-len     打印至少min-len
字符长的字符串. 默认的是4.

-t {o,x,d}        在字符串前面显示其在文件中的偏移量.
选项中的单个字符指定偏移量的进制是八进制, 十六进制，还是十进制.

--target=bfdname        指定一种目标代码格式来代替你的系统的默认格式.




                   Strings 的使用实例:

[tulip]dyron@harris-desktop:~/misc/module/test/tmp$ mips-linux-gnu-strings /usr/bin/uptime  //将会显示uptime中的字符串，默认是以4的最少长度。





***mips-linux-gnu-objcopy:***

可以将一种格式的目标文件内容进行转换，并输出为另一种格式的目标文件。

它使用 GNU BFD(binary format description) 
库读 / 写目标文件，通过这个 BFD
库，objcopy 能以一种不同于源目标文件的格式生成新的目标文件



Objcopy命令的常用参数(太有用的命令了，参数一大堆):

-I bfdname或—input-target=bfdname用来指明源文件的格式,bfdname是BFD库中描述的标准格式名,如果没指明,则objcopy自己分析

-O bfdname 输出的格式

-F bfdname 同时指明源文件,目的文件的格式

-R sectionname 从输出文件中删除掉所有名为sectionname的段

-S 不从源文件中复制重定位信息和符号信息到目标文件中

-g 不从源文件中复制调试符号到目标文件中



                   Objcopy 的使用实例:

objcopy -I binary -O elf32-i386 -B i386 inputfile outputfile



***mips-linux-gnu-readelf:***

显示一个或多个 ELF 格式的目标文件信息。



         Readelf命令的参数简介:

-e                        显示头信息，包括ELF file header、section header、program header

-s                 显示符号表信息

-r                 显示重定位信息

-a                        显示所有信息

         Readelf的使用实例:

                   dyron@harris-desktop:~/misc/module/test/tmp$ mips-linux-gnu-readelf -h filt

                   dyron@harris-desktop:~/misc/module/test/tmp$ mips-linux-gnu-readelf -e filt

dyron@harris-desktop:~/misc/module/test/tmp$ mips-linux-gnu-readelf -s filt



***mips-linux-gnu-objdump：***

显示一个或多个目标文件的信息，由其选项来控制显示哪些信息。



                   Objdump的参数简介:

                            -f               
选项显示文件头的内容。

-d                只反汇编代码段

-D                反汇编代码段和数据段

-S                 混合输出源代码和汇编代码，编译时必须加-g选项



                   Objdump的使用实例:

                            Mips-linux-gnu-objdump –f filt         //只显示filt

                            Mips-linux-gnu-objdump –d filt        //只反汇编代码段



***mips-linux-gnu-addr2line：***

Addr2line 工具（它是标准的 GNU Binutils
中的一部分）是一个可以将指令的地址

和可执行镜象转换成文件名、函数名和源代码行数的工具。



         Addr2line 的参数简介:

         -e               选项来指定可执行映像是 test。

 -f               可以告诉工具输出函数名



                   Addr2line 的使用实例(android分析crash错误):

I/DEBUG   ( 3411):          #00  pc 00000000  

I/DEBUG   ( 3411):          #01  pc 000527e8  /system/lib/libandroid_runtime.so

I/DEBUG   ( 3411):          #02  pc 0000f1f4  /system/lib/libdvm.so

这就是出问题时候的栈，pc等于00000000
说明遇到了空指针，因为在栈里，所以查上一层指针000527e8，就可以分析到具体代码到哪一行了。

dyron@harris-desktop:~/project/test/android-2.2$ mips-linux-gnu-addr2line  -f -e out/target/product/tulip/symbols/system/lib/libandroid_runtime.so 000527e8

??

/home/dyron/project/test/android-2.2/frameworks/base/core/jni/android_database_SQLiteDebug.cpp:129



***mips-linux-gnu-ld:   ***

ld是GNU binutils工具集中的一个，是众多Linkers（链接器）的一种。完成的功能自然也就是链接器的基本功能：把各种目标文件和库文件链接起来，并重定向它们的数据，完成符号解析。Linking其实主要就是完成四个方面的工作：storage
 allocation、symbol management、libraries、relocation。

ld可以识别一种Linker command Language表示的linker scriopt文件来显式的控制链接的过程。通过BFD（Binary
 Format Description）库，ld可以读取和操作COFF（common object file format）、ELF（executable
 and linking format）、a.out等各种格式的目标文件。



         Ld的参数简介:

                   -b TARGET 
设置目标文件的文件格式

-e ADDRESS
设置目标文件的开始地址

-EB  
链接big-endian的目标文件

-EL  
链接small-endian的目标文件

-l LIBNAME   
创建执行程序时要链接的库文件（比如某个库为test，则可以为-ltest）

-L DIRECTORY 
寻找要链接的库文件时搜索的文件路径

-o FILE 
设置输出文件的名字

-s  
去除输出文件中的所有符号信息

-S  
去除输出文件中的调试符号信息

-T FILE 
读取链接描述脚本，以确定符号等的定位地址

-v  
输出ld的版本信息

-x  
去除所有的局部符号信息

-X  
去除临时的局部符号信息，默认情况下会设置这个选项

-Bstatic  
创建的输出文件链接静态链接库

-Bdynamic 
创建的输出文件链接动态链接库

-Tbss ADDRESS 
设置section bss的起始地址

-Tdata ADDRESS
设置section data的起始地址

-Ttext ADDRESS
设置section text的起始地址



         Ld的例用实例

举个内核的例子:

mips-linux-gnu-ld -T 
arch/mips/boot/compressed/ld.script -Ttext 0x80600000 -Bstatic -EL -o arch/mips/boot/compressed/vmlinuz arch/mips/boot/compressed/head.o arch/mips/boot/compressed/misc.o arch/mips/boot/compressed/piggy.o



         ld.script是链接脚本，-o
是输出的文件名, -Ttext是设定text段的地址为0x80600000, -EL链接为小端文件。



再举个小例子，我们的程序开始过程:

         gcc -c a.c                             //编译成.o文件

ld -dynamic-linker /lib/ld-linux.so.2 /usr/lib/crt1.o /usr/lib/crti.o -lc a.o /usr/lib/crtn.o //链接成elf可执行文件.







**4. 参考书籍:**

《程序员的自我修养》------------------------------俞甲子，石凡，潘爱民.







