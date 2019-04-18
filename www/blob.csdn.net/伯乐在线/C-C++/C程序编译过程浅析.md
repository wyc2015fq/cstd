# C程序编译过程浅析 - 文章 - 伯乐在线
原文出处： [笑遍世界，2012-01-08](http://smilejay.com/2012/01/c_compilation_stages/)
前几天看了《程序员的自我修养——链接、装载与库》中的第二章“编译和链接”，主要根据其中的内容简单总结一下C程序编译的过程吧。
我现在一般都是用gcc，所以自然以GCC编译hellworld为例，简单总结如下。
hello.c源代码如下：


```
#include <stdio.h>
int main()
{
	printf(“Hello, world.\n”);
	return 0;
}
```
通常我们使用gcc来生成可执行程序，命令为：gcc hello.c，默认生成可执行文件a.out
其实编译（包括链接）的命令：gcc hello.c 可分解为如下4个大的步骤：
- **预处理**(Preprocessing)
- **编译**(Compilation)
- **汇编**(Assembly)
- **链接**(Linking)
![gcc compilation](http://jbcdn2.b0.upaiyun.com/2016/12/e23dc61482a3e414bc20324fdd6c7d4b.jpg)
gcc compilation
**1.       ****预处理(Preproceessing)**
**预处理的过程主要处理包括以下过程：**
- 将所有的#define删除，并且**展开所有的宏定义**
- **处理**所有的**条件预编译指令**，比如#if #ifdef #elif #else #endif等
- **处理#include** 预编译指令，将被包含的文件插入到该预编译指令的位置。
- **删除所有注释 **“//”和”/* */”.
- **添加行号和文件标识**，以便编译时产生调试用的行号及编译错误警告行号。
- 保留所有的**#pragma****编译器指令**，因为编译器需要使用它们
通常使用以下命令来进行预处理：
gcc -E hello.c -o hello.i
参数**-E**表示只进行预处理 或者也可以使用以下指令完成预处理过程
cpp hello.c > hello.i      /*  cpp – The C Preprocessor  */
直接cat hello.i 你就可以看到预处理后的代码
**2.       ****编译(Compilation)**
编译过程就是把预处理完的文件进行一系列的词法分析，语法分析，语义分析及优化后生成相应的汇编代码。
$gcc –S hello.i –o hello.s
或者
$ /usr/lib/gcc/i486-linux-gnu/4.4/cc1 hello.c
注：现在版本的GCC把预处理和编译两个步骤合成一个步骤，用cc1工具来完成。gcc其实是后台程序的一些包装，根据不同参数去调用其他的实际处理程序，比如：预编译编译程序cc1、汇编器as、连接器ld
可以看到编译后的汇编代码(hello.s)如下：


```
.file   "hello.c"
    .section    .rodata
.LC0:
    .string "Hello, world."
    .text
.globl main
    .type   main, @function
main:
    pushl   %ebp
    movl    %esp, %ebp
    andl    $-16, %esp
    subl    $16, %esp
    movl    $.LC0, (%esp)
    call    puts
    movl    $0, %eax
    leave
    ret
    .size   main, .-main
    .ident  "GCC: (Ubuntu 4.4.3-4ubuntu5) 4.4.3"
    .section    .note.GNU-stack,"",@progbits
```
**3.       ****汇编(Assembly)**
汇编器是将汇编代码转变成机器可以执行的命令，每一个汇编语句几乎都对应一条机器指令。汇编相对于编译过程比较简单，根据汇编指令和机器指令的对照表一一翻译即可。
$ gcc –c hello.c –o hello.o
或者
$ as hello.s –o hello.co
由于hello.o的内容为机器码，不能以普通文本形式的查看（vi 打开看到的是乱码）。
**4.       ****链接(Linking)**
通过调用链接器ld来链接程序运行需要的一大堆目标文件，以及所依赖的其它库文件，最后生成可执行文件。
ld -static crt1.o crti.o crtbeginT.o hello.o -start-group -lgcc -lgcc_eh -lc-end-group crtend.o crtn.o (省略了文件的路径名)。
helloworld的大体编译和链接过程就是这样了，那么编译器和链接器到底做了什么呢？
**编译过程可分为6****步：扫描（词法分析）、语法分析、语义分析、源代码优化、代码生成、目标代码优化。**
词法分析：扫描器（Scanner）将源代的字符序列分割成一系列的记号（Token）。lex工具可实现词法扫描。
语法分析：语法分析器将记号（Token）产生语法树（Syntax Tree）。yacc工具可实现语法分析(yacc: Yet Another Compiler Compiler)。
语义分析：静态语义（在编译器可以确定的语义）、动态语义（只能在运行期才能确定的语义）。
源代码优化：源代码优化器(Source Code Optimizer)，将整个语法书转化为中间代码（Intermediate Code）（中间代码是与目标机器和运行环境无关的）。中间代码使得编译器被分为前端和后端。编译器前端负责产生机器无关的中间代码；编译器后端将中间代码转化为目标机器代码。
目标代码生成：代码生成器(Code Generator).
目标代码优化：目标代码优化器(Target Code Optimizer)。
链接的主要内容是把各个模块之间相互引用的部分处理好，使得各个模块之间能够正确地衔接。
链接的主要过程包括：地址和空间分配（Address and Storage Allocation），符号决议（Symbol Resolution），重定位（Relocation）等。
**链接分为静态链接和动态链接。**
**静态链接**是指在编译阶段直接把静态库加入到可执行文件中去，这样可执行文件会比较大。
而**动态链接**则是指链接阶段仅仅只加入一些描述信息，而程序执行时再从系统中把相应动态库加载到内存中去。
静态链接的大致过程如下图所示：
![static linking](http://jbcdn2.b0.upaiyun.com/2016/12/a838049a76d973c04bbfec9e09b7e6da.jpg)
static linking
参考资料：
《程序员的自我修养——链接、装载与库》
[http://www.stackpop.org/blog/html/y2011/53_cpp_compile_linking.html](http://www.stackpop.org/blog/html/y2011/53_cpp_compile_linking.html)
[http://blog.chinaunix.net/space.php?uid=20196318&do=blog&id=28797](http://blog.chinaunix.net/space.php?uid=20196318&do=blog&id=28797)
