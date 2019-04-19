# 【原译】汇编编程之：Hello World！详解- 好文！！！ - Koma Hub - CSDN博客
2019年03月16日 13:55:49[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：103
**Table of Contents**
[准备工作](#%E5%87%86%E5%A4%87%E5%B7%A5%E4%BD%9C)
[CPU指令的执行过程](#CPU%E6%8C%87%E4%BB%A4%E7%9A%84%E6%89%A7%E8%A1%8C%E8%BF%87%E7%A8%8B)
[1.取指](#1.%E5%8F%96%E6%8C%87)
[2.译码](#2.%E8%AF%91%E7%A0%81)
[3.执行](#3.%E6%89%A7%E8%A1%8C)
[4.写回](#4.%E5%86%99%E5%9B%9E)
[理解寄存器](#%E7%90%86%E8%A7%A3%E5%AF%84%E5%AD%98%E5%99%A8)
[分析我们的源代码](#%E5%88%86%E6%9E%90%E6%88%91%E4%BB%AC%E7%9A%84%E6%BA%90%E4%BB%A3%E7%A0%81)
[总结一下](#%E6%80%BB%E7%BB%93%E4%B8%80%E4%B8%8B)
原文链接地址：[http://os-blog.com/x86-64-assembly-programming-hello-world/](http://os-blog.com/x86-64-assembly-programming-hello-world/)
译文地址：[https://www.cnblogs.com/lazycoding/archive/2012/01/02/2310049.html](https://www.cnblogs.com/lazycoding/archive/2012/01/02/2310049.html)
第一次翻译，错误之处希望园友们不吝赐教。
如果你打算构建自己的操作系统（你马上就要做了，是吗？），你将需要熟悉汇编编程，一旦你了解了一个汇编语言，你也许甚至会用它编写一个完整的OS，不论你如何选择，本教程将会介绍给你x86—64汇编语言，以后会推出"汇编编程"系列文章，会将一些更加高级的话题。
为了能够接收到本系列的文章，欢迎你通过rss或者是email订阅我的博客。
# 准备工作
 在我们开始之前，你需要个一台x86_64的linux机器，并且已经安装nasm程序，我想你可以下载并安装好nasm的。如果你还没有linux机器，请参照本文在虚拟机里安装一个linux。
`"Hello World!"`
  如大多数程序语言的开始教程一样，我们将会以一个最基础的hello world程序开始，我将通过展示代码，并且我建议你手工输入，不要直接复制粘贴，以便更好地记住它，
首先，我们来创建一个目录存储我们的工作文件
```
$ mkdir asm-tutorial
$ cd asm-tutorial
$ gedit hello-world.asm
```
在上面的例子中，我用gedit打开了hello-world.asm，这个好用，通用的文本编辑器，不过，你如果更喜欢emacs，vim或其他的文本编辑器也随意。。
好了，现在我们为我们的hello world程序输入代码，当你已经做完并且成功编译并且运行了以后我将会解释代码是如何工作的。
```
[bits 64]
    global _start
    section .data
    message db "Hello, World!"
    section .text
_start:
    mov rax, 1
    mov rdx, 13
    mov rsi, message
    mov rdi, 1
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
```
创建可执行文件
一旦你已经输入完了，保存文件，然后在终端输入下面的指令。
```
$ nasm -f elf64 hello-world.asm
$ ld hello-world.o -o hello-world
$ ./hello-world
Hello, World!%
```
第一行 nasm -f elf64 hello-world.asm 告诉nasm程序汇编我们的文件，-f elf64则是说明我们想让nasm生成一个elf64格式的目标文件。
nasm如我们所知是汇编器，汇编器就是把一个用汇编语言写好的文件，就像我们的hello-world.asm转换成机器码，机器码告诉计算机执行什么操作，nasm生成的文件就叫做目标文件，在我们这个小例子中，nasm产生一个叫做hello-world.o的文件
我们用hexdump工具看看我们的目标文件的内容
```
$ hexdump hello-world.o
...
0000080 0001 0000 0001 0000 0003 0000 0000 0000
0000090 0000 0000 0000 0000 0200 0000 0000 0000
00000a0 000f 0000 0000 0000 0000 0000 0000 0000
00000b0 0004 0000 0000 0000 0000 0000 0000 0000
00000c0 0007 0000 0001 0000 0006 0000 0000 0000
00000d0 0000 0000 0000 0000 0210 0000 0000 0000
...
```
看到了吧，机器码果断是为机器看的，而不是为人看的。
让这个程序运行起来的最后一步就是linking，链接了，它由系统链接器完成，在linux上，这个工具叫做ld，linking就是把目标文件组合，转换成可执行文件。
-o hello-world 这个选项就是告诉ld我们想要生成的可执行文件名为hello-world。
最后我们通过在我们的文件名之前加"./"来运行我们的程序，程序返回Hello, World!
# CPU指令的执行过程
在我们开始深入挖掘分析我们的程序之前，了解一下CPU如果执行是很有好处的，一般来说，CPU的目的是执行一个有意义的指令序列，通常分为四步，取指，译码，执行，写回
## 1.取指
 第一步，取指，包含从程序内存中取出一条指令，指令在内存中的位置由程序计数器（PC）指定，一旦一条指令取到，程序计数器就会指向下一条指令。
## 2.译码
   译码，确定了CPU将会执行什么操作，指令通常分为两部分，操作码，指定了执行的操作，剩下的那部分通过提供了请求执行该操作的信息，可能是常量，寄存器或者内存地址，（操作数引用）
## 3.执行
   上一步完成后，开始进入执行步，CPU的各个部分相互联系以使他们可以执行由操作码指定的操作。然后操作执行
## 4.写回
   最后一步，写回，仅仅是把执行的结果写到一种存储地址中，比如寄存器或是内存地址，并不是所有的指令都有输出值，有些指令操作程序计数器，这些地址叫做跳转指令，jump指令使得循环，，条件语句和函数调用变得简单。
# 理解寄存器
 寄存器就是cpu内部的小容量的存储器，我们关心的主要是三类寄存器，数据寄存器，地址寄存器和通用寄存器。
- •数据寄存器保存数值，（比如整数和浮点值）
- •地址寄存器，保存内存中的地址
- •通用寄存器，既可以用过数据寄存器也可以用做地址寄存器。
汇编程序员大部分工作都是在操作这些寄存器。
# 分析我们的源代码
 有了上面的背景知识，我们现在可是分析我们的代码，我将把程序分成很多小段，并解释每步完成了什么操作。
`[bits 64]`
我们程序的第一行是一个汇编指令，如你所猜的，这是告诉nasm我们想要得到可以运行在64位处理器上的代码。
` global _start`
这一行是另一条汇编指令，告诉nasm 用_start 标记的代码段（section）应该被看成全局的，全局的部分通常允许其他的目标文件引用它，在我们的这个例子中，我们把_start 段标记为全局的，以便让链接器知道我们的程序从哪里开始。
```
section .data
 message db "Hello, World!"
```
上面代码的第一行又是一条汇编指令，告诉nasm后面跟着的代码是data段，data段包含全局和静态变量。
下一句，我们有这样的静态变量。message db "Hello, World!"，db被用来声明初始化数据，message是一个变量名，与"Hello, World!"关联。
`  section .text`
上面这句是另一个段指令section，但是这次它是告诉nasm把紧跟着的代码存到text段，text段有时候也叫做code段，它是包含可执行代码的目标文件的一部分。
最后，我们要到这个程序最重要的部分了。
```
_start:
    mov rax, 1
    mov rdx, 13
    mov rsi, message
    mov rdi, 1
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
```
第一行。_start:，把它后面的代码和_start标记相关联起来。
```
mov rax, 1
    mov rdx, 13
    mov rsi, message
    mov rdi, 1
```
上面这四行都是加载值到不同的寄存器里，RAX和RDX都是通用寄存器，我们使用他们分别保存1和13，RSI和RDI是源和目标数据索引寄存器，我们设置源寄存器RSI指向message，而目标寄存器指向1。
现在，当寄存器加载完毕后，我们有syscall指令，这是告诉计算机我们想要使用我们已经加载到寄存器的值执行一次系统调用，我们加载的第一个数也就是RAX寄存器的值，告诉计算机我们想使用哪一个系统调用，syscalls表和其对应的数字可以查询这里
就像你从那张表里看到的。RAX里的1意味着我们想要调用write(int fd, const void* buf, size_t bytes). 下一条指令 mov rdx,13，则是我们想要调用的函数write()的最后一个参数值，最后一个参数size_t bytes指令了message的大小，此处 13也就是"Hello, World!"的长度，
下两条指令 mov rsi, message 和 mov rdi,1则分别作了其他两个参数，因此，当我们把他们放在一起，当要执行syscall的时候，我们是告诉计算机执行write(1, message, 13)，1就是标准输出stdout，因此本质上，我们是告诉计算机从message里取13个字节输出到stdout。
```
mov rax, 60
    mov rdi, 0
    syscall
```
现在，可能你想知道我们已经执行完了所有的功能，为什么后面还有个syscall，你不用猜了，直接去查前面所说的那个表 我们知道了，60引用exit().因为，syscall其实就是调用exit(0)了。
# 总结一下
所以啊，我们汇编程序的第一部分就是把message变量和"Hello, World!"对应起来，然后，程序的关键部分是写两个syscall，一个是write(),另一个是exit()，把他们放在一起，如果你想把汇编程序翻译成c语言的话，大概看起来会是这个样子。
```cpp
int main() 
{
    char* message = "Hello, World!"
    
    write(1, message, 13);
    exit(0);
}
```
> 
著作权声明：本文由http://www.cnblogs.com/lazycoding翻译，欢迎转载分享。请尊重作者劳动，转载时保留该声明和作者博客链接，谢谢！
