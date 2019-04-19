# GCC参数详解 - xqhrs232的专栏 - CSDN博客
2016年10月20日 22:36:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：276
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.sina.com.cn/s/blog_531bb76301013lyq.html](http://blog.sina.com.cn/s/blog_531bb76301013lyq.html)
相关文章
1、
# [GCC常用参数解释](http://blog.csdn.net/friendbkf/article/details/45620627)----[http://blog.csdn.net/friendbkf/article/details/45620627](http://blog.csdn.net/friendbkf/article/details/45620627)

gcc是gnu compiler collection 的简称，他包含了多种语言的编译器，如C, C++, Objective-C, Objective-C++, Java, Fortran和Ada。但每种编译器的名字不一样，gcc and g++ 分别是 gnu 的 c & c++ 编译器
gcc/g++ 在执行编译工作的时候，总共需要 4 步
1. 预处理, 生成 .i 的文件 [ 调用预处理器 cpp]
2. 将预处理后的文件转换成汇编语言 , 生成文件 .s[ 调用编译器 gcc/egcs]
3. 有汇编变为目标代码 ( 机器代码 ) 生成 .o 的文件 [ 调用汇编器 as]
4. 连接目标代码 , 生成可执行程序 [ 调用链接器 ld]
常见的gcc参数介绍
-x language filename
　　 设定文件所使用的语言 , 使后缀名无效 , 对以后的多个有效 . 也就是根
　　 据约定 C 语言的后缀名称是 .c 的，而 C++ 的后缀名是 .C 或者 .cpp, 如果
　　 你很个性，决定你的 C 代码文件的后缀名是 .pig 哈哈，那你就要用这
　　 个参数 , 这个参数对他后面的文件名都起作用，除非到了下一个参数
　　 的使用。
　　 可以使用的参数吗有下面的这些
　　　　 `c', `objective-c', `c-header', `c++', `cpp-output',
　　　　 `assembler', and `assembler-with-cpp'.
　　 看到英文，应该可以理解的。
　　 例子用法 :
　　 gcc -x c hello.pig
-x none filename
　　关掉上一个选项，也就是让 gcc 根据文件名后缀，自动识别文件类型
　　例子用法 :
　　 gcc -x c hello.pig -x none hello2.c 　　
-c
　　只激活预处理 , 编译 , 和汇编 , 也就是他只把程序做成 obj 文件
　　例子用法 :
　　 gcc -c hello.c
　　他将生成 .o 的 obj 文件
-S
　　只激活预处理和编译，就是指把文件编译成为汇编代码。
　　例子用法
　　 gcc -S hello.c
　　他将生成 .s 的汇编代码，你可以用文本编辑器察看
-E
　　只激活预处理 , 这个不生成文件 , 你需要把它重定向到一个输出文件里
　　面 .
　　例子用法 :
　　 gcc -E hello.c > pianoapan.txt
　　 gcc -E hello.c | more
　　慢慢看吧 , 一个 hello word 也要与处理成 800 行的代码
-o
　　制定目标名称 , 缺省的时候 ,gcc 编译出来的文件是 a.out, 很难听 , 如果
　　你和我有同感，改掉它 , 哈哈
　　例子用法
　　 gcc -o hello.exe hello.c ( 哦 ,windows 用习惯了 )
　　 gcc -o hello.asm -S hello.c
-pipe
　　使用管道代替编译中临时文件 , 在使用非 gnu 汇编工具的时候 , 可能有些问
　　题
　　 gcc -pipe -o hello.exe hello.c
-ansi
　　关闭 gnu c 中与 ansi c 不兼容的特性 , 激活 ansi c 的专有特性 ( 包括禁止一
　　些 asm inline typeof 关键字 , 以及 UNIX,vax 等预处理宏 ,
-std c99
     表示编译器支持c99标准，主要有c89，c99，gnu89，gnu99，已经c++部分的c++98, c++0x, gnu++98, gnu++0x
-rdynamic
     通知链接器将所有符合添加到动态符号表中去
-lxx
    表示动态加载libxx.so库
-Lxx
    表示增加目录xx，让编译器可以在xx下寻找库文件
-Ixx
   表示增加目录xx，让编译器可以在xx下寻找头文件
优化选项
-o FILE
    生成指定的输出文件。用在生成可执行文件时。
-O0
    不进行优化处理。
-O 或 -O1
    优化生成代码。
-O2
    进一步优化。
-O3
    比 -O2 更进一步优化，包括 inline 函数。
-shared
    生成共享目标文件。通常用在建立共享库时。
-static
    禁止使用共享连接。
-w
    不生成任何警告信息。
-Wall
    生成所有警告信息。一下是具体的选项，可以单独使用
‘-Wcomment’  This option warns about nested comments.
‘-Wformat’   This option warns about the incorrect use of format strings in functions
               such as printf and scanf, where the format specifier does not agree with the  
               type of the corresponding function argument.
‘-Wunused’   This option warns about unused variables.
‘-Wimplicit’ This option warns about any functions that are used without being declared.
‘-Wreturn-type’ This option warns about functions that are defined without a return
                  type but not declared void. It also catches empty return statements in 
                  functions that are not declared void.
其他的一些warn编译选项（不包含在Wall中的）
‘-W’     This is a general option similar to ‘-Wall’ which warns about a selection of 
           common programming errors, such as functions which can return without a value 
           (also known as “falling off the end of the function body”), and comparisons
     一般情况下，-W 和 -Wall同时使用
‘-Wconversion’  This option warns about implicit type conversions that could cause
                  unexpected results.
‘-Wshadow’  This option warns about the redeclaration of a variable name in a scope where 
              it has already been declared.
‘-Wcast-qual’  This option warns about pointers that are cast to remove a type qualifier, 
                 such as const.
‘-Wwrite-strings’ This option implicitly gives all string constants defined in the program
                    a const qualifier, causing a compile-time warning if there is an attempt
                    to overwrite them.
‘-Wtraditional’   This option warns about parts of the code which would be interpreted
                    differently by an ANSI/ISO compiler and a “traditional” pre-ANSI 
                    compiler.(5)
上面的这些warn选项都仅仅产生warn，而不会停止编译过程，下面的选项可以将warn视为error并停止编译
‘-Werror’   changes the default behavior by converting warnings into errors, stopping
             the compilation whenever a warning occurs.
      between signed and unsigned values.
一般情况下，-W 和 -Wall同时使用
有关宏定义的选项
有两种定义的宏的方式：1：其他原文件中；2：在GCC的命令行中使用 -Dxxx
当这些宏被定义后（#define xxx）
在系统中使用时：#ifdef xxx 就会被预处理器扩展为有效代码；
在系统中已经定义了一些系统命名空间内的宏，都是以  __ 开头的 （两条下划线）
使用命令：cpp -dM /dev/null 可以查看到所有的预定义宏
（注：在这些宏中，有一些是GCC系统级的宏，它们都不是以__开头，这些非标准的宏可以使用
 GCC的编译选项 -ansi 使其无效）
（其实，利用-Dxxx来定义xxx，就是将xxx赋值为1 ）
-Dmacro
　　相当于C语言中的#define macro
-Umacro
　　相当于C语言中的#undef macro
-undef
　　取消对所有非标准宏的定义
也是利用 -Dxxx=value的方式来定义
假如利用-DNAME="" （空）来定义一个宏，则这个宏也被视为被定义的，但是如果按值展开的话，
则为空；
-Dmarco=3
     相当于在源文件中的#define macro 3
-g
　　只是编译器，在编译的时候，产生调试信息。
-m
  选择不同的硬件型号 或 配置 --- 例如, 68010 还是 68020, 有没有浮点协处理器. 通过指定选项, 安装编译器的一个版本能够为所有的型号或配置进行编译.有很多选项，这里不列举，如 -mshort，认为int类型为16bit
-Xlinker option
  传递option给链接器，如果option有两个参数，我们必须用两次-Xlinker，因为它每次只能传一个字符串。例如-assert definitions，我们需要 -Xlinker -assert -Xlinker definitions。
  如果是GNU的链接器，我们可以这样写 -Xlinker -assert=definitions，而其他链接器可能不支持这样的写法
程序性能测试工具-gprof
To use profiling, the program must be compiled and linked with the ‘-pg’
profiling option:
$ gcc -Wall -c -pg collatz.c
$ gcc -Wall -pg collatz.o
编译和链接的时候添加选项 -pg ，才能使用gprof测试程序性能
然后运行编译通过的程序，才能产生gprof需要的文件 gmon.out(在当前目录下)
然后执行：
$ gprof a.out （假设可执行文件是缺省生成的）
程序覆盖测试工具- gcov
The GNU coverage testing tool gcov analyses the number of times each
line of a program is executed during a run. This makes it possible to
find areas of the code which are not used, or which are not exercised
in testing. When combined with profiling information from gprof the
information from coverage testing allows efforts to speed up a program to
be concentrated on specific lines of the source code.
编译和链接必须使用相关选项，才可以使用gcov工具。
例如：
$ gcc -Wall -fprofile-arcs -ftest-coverage cov.c
其中
‘-ftest-coverage’ adds instructions for counting the number of times
                    individual lines are executed,
‘-fprofile-arcs’ incorporates instrumentation code for each branch of
                   the program. Branch instrumentation records how frequently
                   different paths are taken through‘if’ statements and
                   other conditionals.
运行通过编译的程序，产生供gcov使用的文件：
分别带有后缀：‘.bb’‘.bbg’ and ‘.da’在当前目录下
然后运行
$ gcov cov.c （注意：是源代码文件 ）
这样会产生一个带有标注的源文件的副本文件，后缀为 .gcov
在该文件中，标注了每一行代码的执行次数，标注为‘###### ’的语句为
未被执行到的语句。
可以通过命令：
grep ’######’ *.gcov 查找到所有未被执行到的语句


