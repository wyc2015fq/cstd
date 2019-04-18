# linux操作系统下c语言编程入门--Linux下C开发工具介绍  - 深之JohnChen的专栏 - CSDN博客

2007年02月28日 18:23:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：10594


 linux操作系统下c语言编程入门--Linux下C开发工具介绍 

Linux的发行版中包含了很多软件开发工具. 它们中的很多是用于 C 和 [C++](http://dev.rdxx.com/C/)应用程序开发
的. 本文介绍了在 Linux 下能用于 C 应用程序开发和调试的工具. 本文的主旨是介绍如
何在 Linux 下使用 C 编译器和其他 C 编程工具, 而非 C 语言编程的教程.

GNU C 编译器
GNU C 编译器(GCC)是一个全功能的 ANSI C 兼容编译器. 如果你熟悉其他操作系统或硬
件平台上的一种 C 编译器, 你将能很快地掌握 GCC. 本节将介绍如何使用 GCC 和一些 
GCC 编译器最常用的选项.

使用 GCC
通常后跟一些选项和文件名来使用 GCC 编译器. gcc 命令的基本用法如下:

gcc [options] [filenames]
命令行选项指定的操作将在命令行上每个给出的文件上执行. 下一小节将叙述一些你会最
常用到的选项.

GCC 选项
GCC 有超过100个的编译选项可用. 这些选项中的许多你可能永远都不会用到, 但一些主
要的选项将会频繁用到. 很多的 GCC 选项包括一个以上的字符. 因此你必须为每个选项
指定各自的连字符, 并且就象大多数 Linux 命令一样你不能在一个单独的连字符后跟一
组选项. 例如, 下面的两个命令是不同的:

gcc -p -g test.c

gcc -pg test.c
第一条命令告诉 GCC 编译 test.c 时为 prof 命令建立剖析(profile)信息并且把调试信
息加入到可执行的文件里. 第二条命令只告诉 GCC 为 gprof 命令建立剖析信息.

当你不用任何选项编译一个程序时, GCC 将会建立(假定编译成功)一个名为 a.out 的可
执行文件. 例如, 下面的命令将在当前目录下产生一个叫 a.out 的文件:

gcc test.c
你能用 -o 编译选项来为将产生的可执行文件指定一个文件名来代替 a.out. 例如, 将一
个叫 count.c 的 C 程序编译为名叫 count 的可执行文件, 你将输入下面的命令:

gcc -o count count.c

------------------------------------------------------------------------------
--

注意: 当你使用 -o 选项时, -o 后面必须跟一个文件名.

------------------------------------------------------------------------------
--

GCC 同样有指定编译器处理多少的编译选项. -c 选项告诉 GCC 仅把源代码编译为目标代
码而跳过[汇编](http://dev.rdxx.com/ASM/)和连接的步骤. 这个选项使用的非常频繁因为它使得编译多个 C 程序时速
度更快并且更易于管理. 缺省时 GCC 建立的目标代码文件有一个 .o 的扩展名.

-S 编译选项告诉 GCC 在为 C 代码产生了汇编语言文件后停止编译. GCC 产生的汇编语
言文件的缺省扩展名是 .s . -E 选项指示编译器仅对输入文件进行预处理. 当这个选项
被使用时, 预处理器的输出被送到标准输出而不是储存在文件里.

优 化 选 项
当你用 GCC 编译 C 代码时, 它会试着用最少的时间完成编译并且使编译后的代码易于调
试. 易于调试意味着编译后的代码与源代码有同样的执行次序, 编译后的代码没有经过优
化. 有很多选项可用于告诉 GCC 在耗费更多编译时间和牺牲易调试性的基础上产生更小
更快的可执行文件. 这些选项中最典型的是-O 和 -O2 选项.

-O 选项告诉 GCC 对源代码进行基本优化. 这些优化在大多数情况下都会使程序执行的更
快. -O2 选项告诉 GCC 产生尽可能小和尽可能快的代码. -O2 选项将使编译的速度比使
用 -O 时慢. 但通常产生的代码执行速度会更快.

除了 -O 和 -O2 优化选项外, 还有一些低级选项用于产生更快的代码. 这些选项非常的
特殊, 而且最好只有当你完全理解这些选项将会对编译后的代码产生什么样的效果时再去
使用. 这些选项的详细描述, 请参考 GCC 的指南页, 在命令行上键入 man gcc .

调试和剖析选项
GCC 支持数种调试和剖析选项. 在这些选项里你会最常用到的是 -g 和 -pg 选项.
-g 选项告诉 GCC 产生能被 GNU 调试器使用的调试信息以便调试你的程序. GCC 提供了
一个很多其他 C 编译器里没有的特性, 在 GCC 里你能使 -g 和 -O (产生优化代码)联用
.. 这一点非常有用因为你能在与最终产品尽可能相近的情况下调试你的代码. 在你同时使
用这两个选项时你必须清楚你所写的某些代码已经在优化时被 GCC 作了改动. 关于调试 
C 程序的更多信息请看下一节"用 gdb 调试 C 程序" .
-pg 选项告诉 GCC 在你的程序里加入额外的代码, 执行时, 产生 gprof 用的剖析信息以
显示你的程序的耗时情况. 关于 gprof 的更多信息请参考 "gprof" 一节.

用 gdb 调试 GCC 程序
Linux 包含了一个叫 gdb 的 GNU 调试程序. gdb 是一个用来调试 C 和 C++ 程序的强力
调试器. 它使你能在程序运行时观察程序的内部结构和内存的使用情况. 以下是 gdb 所
提供的一些功能:

它使你能监视你程序中变量的值.
它使你能设置断点以使程序在指定的代码行上停止执行.
它使你能一行行的执行你的代码.

在命令行上键入 gdb 并按回车键就可以运行 gdb 了, 如果一切正常的话, gdb 将被启动
并且你将在屏幕上看到类似的内容:

GNU gdb 5.0
Copyright 2000 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type "show copying" to see the conditions.
There is absolutely no warranty for GDB. Type "show warranty" for details.
This GDB was configured as "i386-redhat-linux".
(gdb)
当你启动 gdb 后, 你能在命令行上指定很多的选项. 你也可以以下面的方式来运行 gdb 
:

gdb <fname>
当你用这种方式运行 gdb , 你能直接指定想要调试的程序. 这将告诉gdb 装入名为 
fname 的可执行文件. 你也可以用 gdb 去检查一个因程序异常终止而产生的 core 文件, 
或者与一个正在运行的程序相连. 你可以参考 gdb 指南页或在命令行上键入 gdb -h 得
到一个有关这些选项的说明的简单列表.

为调试编译代码(Compiling Code for Debugging)
为了使 gdb 正常工作, 你必须使你的程序在编译时包含调试信息. 调试信息包含你程序
里的每个变量的类型和在可执行文件里的地址映射以及源代码的行号. gdb 利用这些信
息使源代码和机器码相关联.

在编译时用 -g 选项打开调试选项.

gdb 基本命令
gdb 支持很多的命令使你能实现不同的功能. 这些命令从简单的文件装入到允许你检查所
调用的堆栈内容的复杂命令, 表27.1列出了你在用 gdb 调试时会用到的一些命令. 想了
解 gdb 的详细使用请参考 gdb 的指南页.

基本 gdb 命令.

命 令 描 述
file 装入想要调试的可执行文件.
kill 终止正在调试的程序.
list 列出产生执行文件的源代码的一部分.
next 执行一行源代码但不进入函数内部.
step 执行一行源代码而且进入函数内部.
run 执行当前被调试的程序
quit 终止 gdb
watch 使你能监视一个变量的值而不管它何时被改变.
print 显示表达式的值
break 在代码里设置断点, 这将使程序执行到这里时被挂起.
make 使你能不退出 gdb 就可以重新产生可执行文件.
shell 使你能不离开 gdb 就执行 UNIX shell 命令.

gdb 支持很多与 UNIX shell 程序一样的命令编辑特征. 你能象在 bash 或 tcsh里那样
按 Tab 键让 gdb 帮你补齐一个唯一的命令, 如果不唯一的话 gdb 会列出所有匹配的命
令. 你也能用光标键上下翻动历史命令.

gdb 应用举例
本节用一个实例教你一步步的用 gdb 调试程序. 被调试的程序相当的简单, 但它展示了 
gdb 的典型应用.

下面列出了将被调试的程序. 这个程序被称为 hello , 它显示一个简单的问候, 再用反
序将它列出.

#include <stdio.h>

static void my_print (char *);
static void my_print2 (char *);

main ()
{
char my_string[] = "hello world!";
my_print (my_string);
my_print2 (my_string);
}

void my_print (char *string)
{
printf ("The string is %s ", string);
}

void my_print2 (char *string)
{
char *string2;
int size, i;

size = strlen (string);
string2 = (char *) malloc (size + 1);
for (i = 0; i < size; i++)
string2[size - i] = string[i];
string2[size+1] = '';

printf ("The string printed backward is %s ", string2);
}
用下面的命令编译它:

gcc -g -o hello hello.c
这个程序执行时显示如下结果:
../hello
The string is hello world!

The string printed backward is
输出的第一行是正确的, 但第二行打印出的东西并不是我们所期望的. 我们所设想的输出
应该是:

The string printed backward is !dlrow olleh
由于某些原因, my_print2 函数没有正常工作. 让我们用 gdb 看看问题究竟出在哪儿, 
先键入如下命令:

gdb hello

------------------------------------------------------------------------------
--

注意: 记得在编译 hello 程序时把调试选项打开.

------------------------------------------------------------------------------
--

如果你在输入命令时忘了把要调试的程序作为参数传给 gdb , 你可以在 gdb 提示符下用 
file 命令来载入它:

(gdb) file hello
这个命令将载入 hello 可执行文件就象你在 gdb 命令行里装入它一样.

这时你能用 gdb 的 run 命令来运行 hello 了. 当它在 gdb 里被运行后结果大约会象这
样:

(gdb) run

Starting program: /root/hello

The string is hello world!

The string printed backward is

Program exited with code 040
这个输出和在 gdb 外面运行的结果一样. 问题是, 为什么反序打印没有工作? 为了找出
症结所在, 我们可以在 my_print2 函数的 for 语句后设一个断点, 具体的做法是在 gdb 
提示符下键入 list 命令三次, 列出源代码:

(gdb) list

(gdb) list

(gdb) list

------------------------------------------------------------------------------
--

技巧: 在 gdb 提示符下按回车健将重复上一个命令.

------------------------------------------------------------------------------
--

第一次键入 list 命令的输出如下:

1 #include <stdio.h>
2
3 static void my_print (char *);
4 static void my_print2 (char *);
5
6 main ()
7 {
8 char my_string[] = "hello world!";
9 my_print (my_string);
10 my_print2 (my_string);

如果按下回车, gdb 将再执行一次 list 命令, 给出下列输出:

11 }
12
13 void my_print (char *string)
14 {
15 printf ("The string is %s ", string);
16 }
17
18 void my_print2 (char *string)
19 {
20 char *string2;
再按一次回车将列出 hello 程序的剩余部分:

21 int size, i;
22
23 size = strlen (string);
24 string2 = (char *) malloc (size + 1);
25 for (i = 0; i < size; i++)
26 string2[size - i] = string[i];
27 string2[size+1] = '';
28
29 printf ("The string printed backward is %s ", string2);
30 }
根据列出的源程序, 你能看到要设断点的地方在第26行, 在 gdb 命令行提示符下键入如
下命令设置断点:

(gdb) break 26
gdb 将作出如下的响应:

Breakpoint 1 at 0x804857c: file hello.c, line 26.

(gdb)

现在再键入 run 命令, 将产生如下的输出:

Starting program: /root/hello

The string is hello world!

Breakpoint 1, my_print2 (string=0xbffffab0 "hello world!") at hello.c:26
26 string2[size - i] = string[i];
你能通过设置一个观察 string2[size - i] 变量的值的观察点来看出错误是怎样产生的, 
做法是键入:

(gdb) watch string2[size - i]
gdb 将作出如下回应:

Hardware watchpoint 2: string2[size - i]
现在可以用 next 命令来一步步的执行 for 循环了:

(gdb) next
经过第一次循环后, gdb 告诉我们 string2[size - i] 的值是 `h`. gdb 用如下的显示
来告诉你这个信息:

Hardware watchpoint 2: string2[size - i]

Old value = 0 '00'
New value = 104 'h'
my_print2 (string=0xbffffab0 "hello world!") at hello.c:25
25 for (i = 0; i < size; i++)
这个值正是期望的. 后来的数次循环的结果都是正确的. 当 i=11 时, 表达式 
string2[size - i] 的值等于 `!`, size - i 的值等于 1, 最后一个字符已经拷到新串
里了.

如果你再把循环执行下去, 你会看到已经没有值分配给 string2[0] 了, 而它是新串的
第一个字符, 因为 malloc 函数在分配内存时把它们初始化为空(null)字符. 所以 
string2 的第一个字符是空字符. 这解释了为什么在打印 string2 时没有任何输出了.

现在找出了问题出在哪里, 修正这个错误是很容易的. 你得把代码里写入 string2 的第
一个字符的的偏移量改为 size - 1 而不是 size. 这是因为 string2 的大小为 12, 但
起始偏移量是 0, 串内的字符从偏移量 0 到 偏移量 10, 偏移量 11 为空字符保留.

改正方法非常简单. 这是这种解决办法的代码:

#include <stdio.h>

static void my_print (char *);
static void my_print2 (char *);

main ()
{
char my_string[] = "hello world!";
my_print (my_string);
my_print2 (my_string);
}

void my_print (char *string)
{
printf ("The string is %s ", string);
}

void my_print2 (char *string)
{
char *string2;
int size, i;

size = strlen (string);
string2 = (char *) malloc (size + 1);
for (i = 0; i < size; i++)
string2[size -1 - i] = string[i];
string2[size] = '';

printf ("The string printed backward is %s ", string2);
}
如果程序产生了core文件，可以用gdb hello core命令来查看程序在何处出错。如在函数
my_print2()中，如果忘记了给string2分配内存 string2 = (char *) malloc (size + 
1);，很可能就会core dump.

另外的 C 编程工具

xxgdb
xxgdb 是 gdb 的一个基于 X Window 系统的图形界面. xxgdb 包括了命令行版的 gdb 
上的所有特性. xxgdb 使你能通过按按钮来执行常用的命令. 设置了断点的地方也用图
形来显示.

你能在一个 Xterm 窗口里键入下面的命令来运行它:

xxgdb
你能用 gdb 里任何有效的命令行选项来初始化 xxgdb . 此外 xxgdb 也有一些特有的命
令行选项, 表 27.2 列出了这些选项.

表 27.2. xxgdb 命令行选项.

选 项 描 述
db_name 指定所用调试器的名字, 缺省是 gdb.
db_prompt 指定调试器提示符, 缺省为 gdb.
gdbinit 指定初始化 gdb 的命令文件的文件名, 缺省为 .gdbinit.

nx 告诉 xxgdb 不执行 .gdbinit 文件.
bigicon 使用大图标.

calls
你可以在 sunsite.unc.edu FTP 站点用下面的路径:
/pub/Linux/devel/lang/c/calls.tar.Z
来取得 calls , 一些旧版本的 Linux CD-ROM 发行版里也附带有. 因为它是一个有用的
工具, 我们在这里也介绍一下. 如果你觉得有用的话, 从 BBS, FTP, 或另一张CD-ROM 上
弄一个拷贝. calls 调用 GCC 的预处理器来处理给出的源程序文件, 然后输出这些文件
的里的函数调用树图.

注意: 在你的系统上安装 calls , 以超级用户身份登录后执行下面的步骤: 1. 解压和 
untar 文件. 2. cd 进入 calls untar 后建立的子目录. 3. 把名叫 calls 的文件移动
到 /usr/bin 目录. 4. 把名叫 calls.1 的文件移动到目录 /usr/man/man1 . 5. 删除 
/tmp/calls 目录. 这些步骤将把 calls 程序和它的指南页安装载你的系统上.

------------------------------------------------------------------------------
--

当 calls 打印出调用跟踪结果时, 它在函数后面用中括号给出了函数所在文件的文件名:

main [hello.c]
如果函数并不是向 calls 给出的文件里的, calls 不知道所调用的函数来自哪里, 则只
显示函数的名字:

printf
calls 不对递归和静态函数输出. 递归函数显示成下面的样子:

fact <<< recursive in factorial.c >>>
静态函数象这样显示:

total [static in calculate.c]
作为一个例子, 假设用 calls 处理下面的程序:

#include <stdio.h>

static void my_print (char *);
static void my_print2 (char *);

main ()
{
char my_string[] = "hello world!";
my_print (my_string);
my_print2 (my_string);
my_print (my_string);
}

void count_sum()
{
int i,sum=0;
for(i=0; i<1000000; i++)
sum += i;
}

void my_print (char *string)
{
count_sum();
printf ("The string is %s ", string);
}

void my_print2 (char *string)
{
char *string2;
int size, i,sum =0;

count_sum();
size = strlen (string);
string2 = (char *) malloc (size + 1);
for (i = 0; i < size; i++) string2[size -1 - i] = string[i];
string2[size] = '';
for(i=0; i<5000000; i++)
sum += i;

printf ("The string printed backward is %s ", string2);
}
将产生如下的输出:

1 __underflow [hello.c]
2 main
3 my_print [hello.c]
4 count_sum [hello.c]
5 printf
6 my_print2 [hello.c]
7 count_sum
8 strlen
9 malloc
10 printf
calls 有很多命令行选项来设置不同的输出格式, 有关这些选项的更多信息请参考 calls 
的指南页. 方法是在命令行上键入 calls -h .

calltree
calltree与calls类似，初了输出函数调用树图外，还有其它详细的信息。
可以从sunsite.unc.edu FTP 站点用下面的路径
:/pub/Linux/devel/lang/c/calltree.tar.gz得到calltree.

cproto
cproto 读入 C 源程序文件并自动为每个函数产生原型申明. 用 cproto 可以在写程序时
为你节省大量用来定义函数原型的时间.
如果你让 cproto 处理下面的代码(cproto hello.c):

#include <stdio.h>

static void my_print (char *);
static void my_print2 (char *);

main ()
{
char my_string[] = "hello world!";
my_print (my_string);
my_print2 (my_string);
}

void my_print (char *string)
{
printf ("The string is %s ", string);
}

void my_print2 (char *string)
{
char *string2;
int size, i;

size = strlen (string);
string2 = (char *) malloc (size + 1);
for (i = 0; i < size; i++)
string2[size -1 - i] = string[i];
string2[size] = '';

printf ("The string printed backward is %s ", string2);
}
你将得到下面的输出:

/* hello.c */

int main(void);

int my_print(char *string);

int my_print2(char *string);
这个输出可以重定向到一个定义函数原型的包含文件里.

indent
indent 实用程序是 Linux 里包含的另一个编程实用工具. 这个工具简单的说就为你的代
码产生美观的缩进的格式. indent 也有很多选项来指定如何格式化你的源代码.这些选项
的更多信息请看indent 的指南页, 在命令行上键入 indent -h .

下面的例子是 indent 的缺省输出:

运行 indent 以前的 C 代码:

#include <stdio.h>

static void my_print (char *);
static void my_print2 (char *);

main ()
{
char my_string[] = "hello world!";
my_print (my_string);
my_print2 (my_string);
}

void my_print (char *string)
{
printf ("The string is %s ", string);
}

void my_print2 (char *string)
{
char *string2; int size, i;

size = strlen (string);
string2 = (char *) malloc (size + 1);
for (i = 0; i < size; i++) string2[size -1 - i] = string[i];
string2[size] = '';

printf ("The string printed backward is %s ", string2);
}
运行 indent 后的 C 代码:

#include <stdio.h>
static void my_print (char *);
static void my_print2 (char *);
main ()
{
char my_string[] = "hello world!";
my_print (my_string);
my_print2 (my_string);
}
void
my_print (char *string)
{
printf ("The string is %s ", string);
}
void
my_print2 (char *string)
{
char *string2;
int size, i;
size = strlen (string);
string2 = (char *) malloc (size + 1);
for (i = 0; i < size; i++)
string2[size - 1 - i] = string[i];
string2[size] = '';
printf ("The string printed backward is %s ", string2);
}
indent 并不改变代码的实质内容, 而只是改变代码的外观. 使它变得更可读, 这永远是
一件好事.

gprof
gprof 是安装在你的 Linux 系统的 /usr/bin 目录下的一个程序. 它使你能剖析你的程
序从而知道程序的哪一个部分在执行时最费时间.

gprof 将告诉你程序里每个函数被调用的次数和每个函数执行时所占时间的百分比. 你如
果想提高你的程序性能的话这些信息非常有用.

为了在你的程序上使用 gprof, 你必须在编译程序时加上 -pg 选项. 这将使程序在每次
执行时产生一个叫 gmon.out 的文件. gprof 用这个文件产生剖析信息.

在你运行了你的程序并产生了 gmon.out 文件后你能用下面的命令获得剖析信息:

gprof <program_name>
参数 program_name 是产生 gmon.out 文件的程序的名字.

为了说明问题，在程序中增加了函数count_sum()以消耗CPU时间，程序如下
#include <stdio.h>

static void my_print (char *);
static void my_print2 (char *);

main ()
{
char my_string[] = "hello world!";
my_print (my_string);
my_print2 (my_string);
my_print (my_string);
}

void count_sum()
{
int i,sum=0;
for(i=0; i<1000000; i++)
sum += i;
}

void my_print (char *string)
{
count_sum();
printf ("The string is %s ", string);
}

void my_print2 (char *string)
{
char *string2;
int size, i,sum =0;

count_sum();
size = strlen (string);
string2 = (char *) malloc (size + 1);
for (i = 0; i < size; i++) string2[size -1 - i] = string[i];
string2[size] = '';
for(i=0; i<5000000; i++)
sum += i;

printf ("The string printed backward is %s ", string2);
}
$ gcc -pg -o hello hello.c
$ ./hello
$ gprof hello | more
将产生以下的输出
Flat profile:

Each sample counts as 0.01 seconds.
% cumulative self self total
time seconds seconds calls us/call us/call name
69.23 0.09 0.09 1 90000.00 103333.33 my_print2
30.77 0.13 0.04 3 13333.33 13333.33 count_sum
0.00 0.13 0.00 2 0.00 13333.33 my_print

% 执行此函数所占用的时间占程序总
time 执行时间的百分比

cumulative 累计秒数 执行此函数花费的时间
seconds （包括此函数调用其它函数花费的时间）

self 执行此函数花费的时间
seconds （调用其它函数花费的时间不计算在内）

calls 调用次数

self 每此执行此函数花费的微秒时间
us/call

total 每此执行此函数加上它调用其它函数
us/call 花费的微秒时间

name 函数名

由以上数据可以看出，执行my_print()函数本身没花费什么时间，但是它又调用了
count_sum()函数，所以累计秒数为0.13.

技巧: gprof 产生的剖析数据很大, 如果你想检查这些数据的话最好把输出重定向到一个
文件里.

