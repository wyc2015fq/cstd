# 学习较底层编程：动手写一个C语言编译器 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [菜鸟浮出水](http://www.jobbole.com/members/caiyiheng) 翻译。未经许可，禁止转载！
英文出处：[Wilfred Hughes](http://www.wilfred.me.uk/blog/2014/08/27/baby-steps-to-a-c-compiler/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
动手编写一个编译器，学习一下较为底层的编程方式，是一种学习计算机到底是如何工作的非常有效方法。
编译器通常被看作是十分复杂的工程。事实上，编写一个产品级的编译器也确实是一个庞大的任务。但是写一个小巧可用的编译器却不是这么困难。
秘诀就是首先去找到一个最小的可用工程，然后把你想要的特性添加进去。这个方法也是Abdulaziz Ghuloum在他那篇著名的论文[“一种构造编译器的捷径”](http://scheme2006.cs.uchicago.edu/11-ghuloum.pdf)里所提到的办法。不过这个办法确实可行。你只需要按照这篇论文中的第一步来操作，就可以得到一个真正可用的编译器！当然，它只能编译程序语言中的非常小的子集，但是它确实是一个真实可用的编译器。你可以随意的扩展这个编译器，然后从中学到更多更深的知识。
受到这篇文章的鼓舞，我就写了一个C编译器。从某种意义上来说这比写一个scheme的编译器要困难一些（因为你必须去解析C那复杂的语法），但是在某些方面又很便利（你不需要去处理运行时类型）。要写这样一个编译器，你只需要从你那个可用的最小的编译器开始。
对于我写的编译器来说，我把它叫 [babyc](https://github.com/Wilfred/babyc)，我选了这段代码来作为我需要运行的第一个程序：

C
```
int main() {
    return 2;
}
```
没有变量，没有函数调用，没有额外的依赖，甚至连if语句，循环语句都没有，一切看起来是那么简单。
我们首先需要解析这段代码。我们将使用 Flex 和 Bison 来做到这点。这里有怎么用的[例子](https://gist.github.com/codebrainz/2933703)可以参考，幸好我们的语法是如此简单，下面就是词法分析器：


```
"{" { return '{'; }
"}" { return '}'; }
"(" { return '('; }
")" { return ')'; }
";" { return ';'; }
[0-9]+ { return NUMBER; }
"return" { return RETURN; }
"int" { return TYPE; }
"main" { return IDENTIFIER; }
```
这里是语法分析器：


```
function:
	TYPE IDENTIFIER '(' ')' '{' expression '}'
	;
expression:
	RETURN NUMBER ';'
	;
```
最终，我们需要生成一些汇编代码。我们将使用32位的X86汇编，因为它非常的通用而且可以很容易的运行在你的机器上。这里有[X86汇编的相关网站](http://x86.renejeschke.de/)。
下面就是我们需要生成的汇编代码：


```
.text
        .global _start # Tell the loader we want to start at _start.
_start:
        movl    $2,%ebx # The argument to our system call.
        movl    $1,%eax # The system call number of sys_exit is 1.
        int     $0x80 # Send an interrupt
```
然后加上上面的[词法语法分析代码](https://github.com/Wilfred/babyc/blob/dffc393f3254468acfbb3539c2e0f8c464b40464/minimal_c.y#L43)，把[这段汇编代码](https://github.com/Wilfred/babyc/blob/dffc393f3254468acfbb3539c2e0f8c464b40464/minimal_c.y#L17)写进一个文件里。恭喜你！你已经是一个编译器的编写者了！
Babyc 就是这样诞生的，你可以在这里看到[它最开始的样子](https://github.com/Wilfred/babyc/tree/dffc393f3254468acfbb3539c2e0f8c464b40464)。
当然，如果汇编代码没办法运行也是枉然。让我们来用编译器生成我们所希望的真正的汇编代码。

Shell
```
# Here's the file we want to compile.
$ cat return_two.c
#include <stdio.h>
int main() {
    return 2;
}
# Run the compiler with this file.
$ ./babyc return_two.c
Written out.s.
# Check the output looks sensible.
$ cat out.s
.text
    .global _start
_start:
    movl    $2, %ebx
    movl    $1, %eax
    int     $0x80
```
非常棒！接着让我们来真正的运行一下编译之后代码来确保它能得到我们所想的结果。

Shell
```
# Assemble the file. We explicitly assemble as 32-bit
# to avoid confusion on x86_64 machines.
$ as out.s -o out.o --32
# Link the file, again specifying 32-bit.
$ ld -m elf_i386 -s -o out out.o
# Run it!
$ ./out
# What was the return code?
$ echo $?
2 # Woohoo!
```
我们踏出了第一步，接下去怎么做就全看你了。你可以按照那篇文章所指导的全部做一遍，然后制作一个更加复杂的编译器。你需要去写一个更加精巧的语法树来生成汇编代码。接下去的几步分别是：（1）允许返回任意的值（比如，return 3; [一些可执行代码](https://github.com/Wilfred/babyc/commit/65f3a0171e25075db886f4d5cc4127ed04a77a88)）；（2）添加对“非”的支持（比如，return ～1; [一些可执行代码](https://github.com/Wilfred/babyc/commit/32a82562096873f67c4fb36198eba43abc4ea8d6)）。每一个额外的特性都可以教你关于C语言的更多知识，编译器到底是怎么执行的，以及世界上其他编写编译器的人是如何想的。
这是构建 babyc 的方法。Babyc 现在已经拥有了if语句，循环，变量以及最基础的数据结构。欢迎你来[check out它的代码](https://github.com/Wilfred/babyc)，但是我希望看完我的文章你能够自己动手写一个。
不要害怕底层的一些事情。这是一个非常奇妙的世界。
