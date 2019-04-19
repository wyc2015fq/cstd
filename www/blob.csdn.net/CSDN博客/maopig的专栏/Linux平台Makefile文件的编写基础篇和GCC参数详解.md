# Linux平台Makefile文件的编写基础篇和GCC参数详解 - maopig的专栏 - CSDN博客
2011年09月22日 16:50:53[maopig](https://me.csdn.net/maopig)阅读数：7928
**问：gcc中的-I.是什么意思。。。。看到了有的是gcc -I. -I/usr/xxxxx..那个-I.是什么意思呢**
**答：-Ixxx 的意思是除了默认的头文件搜索路径(比如/usr/include等）外，同时还在路径xxx下搜索需要被引用的头文件。 所以你的gcc -I. -I/usr/xxxxx 的意思是，同时还要再 . 目录（即 执行gcc的当前目录） 以及 /usr/xxxxx 目录下搜索头文件。**
解释MAKEFILE    
EXEC = hello
OBJS = hello.o
CROSS= iwmmxt_le-
CC = $(CROSS)gcc
STRIP= $(CROSS)strip
CFLAGS = -Wall -g -O2
all: clean $(EXEC)
$(EXEC):$(OBJS)
$(CC) $(CFLAGS) -o $@ $(OBJS)
$(STRIP) $@
clean:
-rm -f $(EXEC) *.o
最佳答案
先说明下”all:“和”clean:“。这两个语句类似于：if (arg=="all") then.... 和 if (arg=="clean") then...
也就是说，如果你执行make all，all后面的语句就会被执行。
如果make clean，clean后面的语句就会被执行。
另外，all和clean后面的语句应该都是要缩进的。
EXEC = hello  // 生成的可执行文件名为hello
OBJS = hello.o // 编译产生的中间文件名为hello.o
CROSS= iwmmxt_le- //设置iwmmxt_Ie-为交叉编译环境
CC = $(CROSS)gcc // 编译器是gcc
STRIP= $(CROSS)strip // 设置优化器（用来删除debug信息）
CFLAGS = -Wall -g -O2 // 设置编译选项
all: clean $(EXEC) // 如果是make all，先掉用下clean后面的指令，再执行自己下面的指令
$(EXEC):$(OBJS)  // 创建依赖关系。这里表示hello依赖于hello.o。也就是说，先产生hello.o再产生hello
$(CC) $(CFLAGS) -o $@ $(OBJS)  // 编译
$(STRIP) $@ // 删除debug信息
clean: // make clean就会来这里
-rm -f $(EXEC) *.o // 删除产生的中间文件
# [Linux平台Makefile文件的编写基础篇](http://www.cnblogs.com/fence/archive/2009/12/18/1627147.html)
目的：
        基本掌握了 make 的用法，能在Linux系统上编程。
        环境：
        Linux系统，或者有一台Linux服务器，通过终端连接。一句话：有Linux编译环境。
        准备：
        准备三个文件：file1.c, file2.c, file2.h
               file1.c:
                      #include <stdio.h>
                      #include "file2.h"
                      int main()
                      {
                             printf("print file1$$$$$$$$$$$$$$$$$$$$$$$$\n");
                             File2Print();
                             return 0;
                      }
               file2.h:
                      #ifndef FILE2_H_
                      #define    FILE2_H_
                              #ifdef __cplusplus
                                    extern "C" {
                             #endif
                             void File2Print();
                             #ifdef __cplusplus
                                    }
                             #endif
                      #endif
               file2.c:
                      #include "file2.h"
                      void File2Print()
                      {
                             printf("Print file2**********************\n");
                      }
        基础：
        先来个例子：
        有这么个Makefile文件。（文件和Makefile在同一目录）
               === makefile 开始 ===
                      helloworld:file1.o file2.o
                             gcc file1.o file2.o -o helloworld
                      file1.o:file1.c file2.h
                             gcc -c file1.c -o file1.o
                       file2.o:file2.c file2.h
                             gcc -c file2.c -o file2.o
                      clean:
                             rm -rf *.o helloworld
               === makefile 结束 ===
        一个 makefile 主要含有一系列的规则，如下：
        A: B
        (tab)<command>
        (tab)<command>
        每个命令行前都必须有tab符号。
        上面的makefile文件目的就是要编译一个helloworld的可执行文件。让我们一句一句来解释：
               helloworld : file1.o file2.o：                 helloworld依赖file1.o file2.o两个目标文件。
               gcc File1.o File2.o -o helloworld：      编译出helloworld可执行文件。-o表示你指定 的目标文件名。
               file1.o : file1.c：    file1.o依赖file1.c文件。
               gcc -c file1.c -o file1.o：                  编译出file1.o文件。-c表示gcc 只把给它的文件编译成目标文件， 用源码文件的文件名命名但把其后缀由“.c”或“.cc”变成“.o”。在这句中，可以省略-o file1.o，编译器默认生成file1.o文件，这就是-c的作用。
                      file2.o : file2.c file2.h
                      gcc -c file2.c -o file2.o
        这两句和上两句相同。
               clean:
                      rm -rf *.o helloworld
        当用户键入make clean命令时，会删除*.o 和helloworld文件。
        如果要编译cpp文件，只要把gcc改成g++就行了。
        写好Makefile文件，在命令行中直接键入make命令，就会执行Makefile中的内容了。
        到这步我想你能编一个Helloworld程序了。
       上一层楼：使用变量
　　上面提到一句，如果要编译cpp文件，只要把gcc改成g++就行了。但如果Makefile中有很多gcc，那不就很麻烦了。
               第二个例子：
               === makefile 开始 ===
                      OBJS = file1.o file2.o
                      CC = gcc
                      CFLAGS = -Wall -O -g
                      helloworld : $(OBJS)
                             $(CC) $(OBJS) -o helloworld
                      file1.o : file1.c file2.h
                             $(CC) $(CFLAGS) -c file1.c -o file1.o
                      file2.o : file2.c file2.h
                             $(CC) $(CFLAGS) -c file2.c -o file2.o
                      clean:
                             rm -rf *.o helloworld
        === makefile 结束 ===
　　这里我们应用到了变量。要设定一个变量，你只要在一行的开始写下这个变量的名字，后 面跟一个 = 号，后面跟你要设定的这个变量的值。以后你要引用 这个变量，写一个 $ 符号，后面是围在括号里的变量名。
        CFLAGS = -Wall -O –g，解释一下。这是配置编译器设置，并把它赋值给CFFLAGS变量。
        -Wall：          输出所有的警告信息。
        -O：              在编译时进行优化。
        -g：               表示编译debug版本。
　　这样写的Makefile文件比较简单，但很容易就会发现缺点，那就是要列出所有的c文件。如果你添加一个c文件，那就需要修改Makefile文件，这在项目开发中还是比较麻烦的。
        再上一层楼：使用函数
　　学到这里，你也许会说，这就好像编程序吗？有变量，也有函数。其实这就是编程序，只不过用的语言不同而已。
               第三个例子：
               === makefile 开始 ===
                      CC = gcc
                      XX = g++
                      CFLAGS = -Wall -O –g
                      TARGET = ./helloworld
                      %.o: %.c
                             $(CC) $(CFLAGS) -c $< -o $@
                      %.o:%.cpp
                             $(XX) $(CFLAGS) -c $< -o $@
                      SOURCES = $(wildcard *.c *.cpp)
                      OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
                      $(TARGET) : $(OBJS)
                             $(XX) $(OBJS) -o $(TARGET)
                             chmod a+x $(TARGET)
        clean:
               rm -rf *.o helloworld
        === makefile 结束 ===
        函数1：wildcard
　　产生一个所有以 '.c' 结尾的文件的列表。
　　SOURCES = $(wildcard *.c *.cpp)表示产生一个所有以 .c，.cpp结尾的文件的列表，然后存入变量 SOURCES 里。
        函数2：patsubst
　　匹配替换，有三个参数。第一个是一个需要匹配的式样，第二个表示用什么来替换它，第三个是一个需要被处理的由空格分隔的列表。
　　OBJS = $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SOURCES)))表示把文件列表中所有的.c,.cpp字符变成.o，形成一个新的文件列表，然后存入OBJS变量中。
        %.o: %.c
               $(CC) $(CFLAGS) -c $< -o $@
        %.o:%.cpp
               $(XX) $(CFLAGS) -c $< -o $@
　　这几句命令表示把所有的.c,.cpp编译成.o文件。
　　这里有三个比较有用的内部变量。$@ 扩展成当前规则的目的文件名， $< 扩展成依靠       列表中的第一个依靠文件，而 $^ 扩展成整个依靠的列表（除掉了里面所有重 复的文件名）。
               chmod a+x $(TARGET)表示把helloworld强制变成可执行文件。
　　到这里，我想你已经能够编写一个比较简单也比较通用的Makefile文件了，上面所有的例子都假定所有的文件都在同一个目录下，不包括子目录。
# [GCC参数详解](http://www.cnblogs.com/fence/archive/2009/12/18/1627138.html)
gcc and g++分别是gnu的c & c++编译器
　　gcc/g++在执行编译工作的时候，总共需要4步
　　1.预处理,生成.i的文件[预处理器cpp]
　　2.将预处理后的文件不转换成汇编语言,生成文件.s[编译器egcs]
　　3.有汇编变为目标代码(机器代码)生成.o的文件[汇编器as]
　　4.连接目标代码,生成可执行程序[链接器ld]
　　[参数详解]
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
　　只是编译器，在编译的时候，产生条是信息。
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
　　[参考资料]
　　-Linux/UNIX高级编程
　　中科红旗软件技术有限公司编著.清华大学出版社出版
　　-Gcc man page
