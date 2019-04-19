# 通过 GDB 学习 C 语言 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [myillusion3852](http://www.jobbole.com/members/myillusion3852) 翻译，[dopcn](http://www.jobbole.com/members/dopcn) 校稿。未经许可，禁止转载！
英文出处：[recurse](https://www.recurse.com/blog/5-learning-c-with-gdb)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
对于那些具有高级编程语言诸如： Ruby、Scheme、Haskell 等背景的人来说，学习 C 语言是具有挑战性的。除了纠结于 C  语言中像手动内存管理和指针等底层特性外，你必须在没有 REPL ( Read-Eval-Print Loop ) 的条件下完成工作。一旦你已经习惯于在 REPL 环境下进行探索性的编程，必须进行“编写-编译-运行”这样循环实在有点令人生厌。
最近我发现其实可以用 GDB 来作为 C 语言的伪 REPL。我一直尝试使用 GDB 作为学习 C 语言的工具，而不仅仅是用来调试 C 程序，事实上这非常有趣。
这篇文章我的目的就是向你展示 GDB 是一个非常好的学习 C 语言工具。下面我将会向你介绍一些我最喜欢的 GDB 命令，然后我会向你阐述怎样使用 GDB 来理解 C 语言中一个出了名的复杂问题：数组和指针的区别。
## GDB 简介
从创建一个简单的 C 程序开始，minimal.c:

C
```
int main()
{ 
   int i = 1337;
   return 0; 
}
```
注意这个程序并没有做任何事情，也没有一条输出指令。拥抱使用 GDB 学习 C 语言的美丽新世界吧！
使用 -g 参数进行编译，这样会生成一些有助于 debug，gdb 可以利用的信息，编译后用 GDB 运行起来：

Shell
```
$ gcc -g minimal.c -o minimal 
$ gdb minimal
```
你现在应该能看到明显的 GDB 提示行。我之前告诉你这是一个 REPL，下面我们就来试试：

C
```
(gdb) print 1 + 2 
$1 = 3
```
多么神奇！ print 是 GDB 的内置命令，他能够打印出一个 C 语言命令的返回值。如果你不确定一个 GDB 命令是做什么，尝试在 GDB 提示下运行命令 help。
然后是一个更有趣的例子：

C
```
(gbd) print (int) 2147483648
$2 = -2147483648
```
这里我先忽略为什么 2147483648 == -2147483648；我想要说明的是即使是算术运算在 C 语言中也是有很多坑的，GDB 能够理解运行 C 语言中的算术运算。
现在让我们在主函数中设置一个断点然后运行程序：

C
```
(gdb) break main
(gdb) run
```
现在程序在第 3 行处暂停，正好在 i 进行初始化之前。有趣的是，尽管 i 还没有被初始化，我们依然能够使用 print 命令看到它的值。

C
```
(gdb) print i
$3 = 32767
```
在 C 语言中，一个未被初始化的局部变量的值是没有定义的，所以你用 GDB 打印出的值可能与这里的不一样。
我们可以用 next 命令来执行当前断点这一行：

C
```
(gdb) next
(gdb) print i
$4 = 1337
```
## 使用 x 命令检查内存
在 C 语言中变量用来标示一块连续的内存区间。一个变量的内存区间由两个数字决定：
- 这块内存第一个字节数的数值地址
- 内存的大小，单位是字节。变量所占内容的大小取决于变量的类型。
C 语言中一个独特的特性是你能够直接访问变量所占的内存。操作符 & 可以计算一个变量的地址，操作符 sizeof 计算变量所占内存的大小。
你可以在 GDB 中测试以上两个概念：

C
```
(gdb) print &i
$5 = (int *) 0x7fff5fbff584
(gdb) print sizeof(i)
$6 = 4
```
字面上看，i 所占内存起始于地址 0x7fff5fbff5b4，占内存 4 个字节。
我前面提到的变量在内存中的大小取决于它的类型，所以操作符 sizeof 能够直接作用于类型：

C
```
(gdb) print sizeof(int)
$7 = 4
(gdb) print sizeof(double)
$8 = 8
```
以上显示意味着，至少在我的计算机上 int 变量占 4 个字节空间，double 变量占 8 个字节。
GDB 带来了一个功能强大的工具，能够直接检测内存：x 命令。x 命令从一个特定的地址开始检测内存。结合一些结构化的命令和这些已给的命令能精确控制你想检测多少字节，你想怎样打印它们。当你有疑问时，尝试在 GDB 提示下运行 help x。
& 操作符计算变量的地址，这意味着我们能将 &i 返回给 x，从而看到 i 值背后原始的字节。

C
```
(gdb) x/4xb &i
0x7fff5fbff584: 0x39    0x05    0x00    0x00
```
标识参数表示我想要检查 4 个值，格式是十六进制，一次显示一个字节。我选择检查 4 个字节，是因为 i 在内存中的大小是 4 字节；逐字节打印出 i 在内存中的表示。
在 Intel 机器上有一个坑应当记得，逐字节检测时字节数是以“小端”顺序保存：不像人类一般使用的标记方法，一个数字的低位在内存中排在前面（个位数在十位数之前）。
为了让这个问题更加明显，我们可以为 i 赋一个特别的值，然后重新检测所占内存。

C
```
(gdb) set var i = 0x12345678
(gdb) x/4xb &i
0x7fff5fbff584: 0x78 0x56 0x34 0x12
```
## 使用 ptype 检查类型
ptype 命令可能是我最喜爱的命令。它告诉你一个 C 语言表达式的类型。

C
```
(gdb) ptype i
type = int
(gdb) ptype &i
type = int *
(gdb) ptype main
type = int (void)
```
C 语言中的类型可以变得很复杂，但是好在 ptype 允许你交互式地查看他们。
## 指针和数组
数组在C语言中是非常难以捉摸的概念。这节的计划是写出一个简单的程序，然后在 GDB 中运行，直至它的意义变得清晰易懂。
编写如下的程序，array.c:

C
```
int main()
{
    int a[] = {1,2,3};
    return 0;
}
```
使用 -g 作为命令行参数进行编译，在 GDB 中运行，然后输入 next，执行初始化那一行

C
```
$ gcc -g arrays.c -o arrays
$ gdb arrays
(gdb) break main
(gdb) run
(gdb) next
```
在这里，你应该能够打印出 a 的内容并检查它的类型：

C
```
(gdb) print a
$1 = {1, 2, 3}
(gdb) ptype a
type = int [3]
```
现在我们的程序已经在 GDB 中运行起来了，我们应该做的第一件事是使用 x 看看 a 在内存中是什么样子。

C
```
(gdb) x/12xb &a
0x7fff5fbff56c: 0x01  0x00  0x00  0x00  0x02  0x00  0x00  0x00
0x7fff5fbff574: 0x03  0x00  0x00  0x00
```
以上意思是 a 所占内存开始于地址 0x7fff5fbff5dc。起始的四个字节存储 a[0], 随后的四个字节存储 a[1], 最后的四个字节存储 a[2]。事实上你可以通过 sizeof 得到，a 在内存中的大小是 12 字节。

C
```
(gdb) print sizeof(a)
$2 = 12
```
现在，数组好像确实有个数组的样子。他们有自己的数组类型，在连续的内存空间中存储自己的成员。然而在某些情况下，数组表现得更像指针。例如，我们能在 a 上进行指针运算。

C
```
= preserve do
  :escaped
    (gdb) print a + 1
    $3 = (int *) 0x7fff5fbff570
```
字面上看，a+1 是一个指向 int 的指针，占据地址 0x7fff5fbff570。这时，你应该反过来将指针传递给 x 命令，让我们看看会发生什么：

C
```
= preserve do
  :escaped
    (gdb) x/4xb a + 1
    0x7fff5fbff570: 0x02  0x00  0x00  0x00
```
注意 0x7fff5fbff570 比 0x7fff5fbff56c 大 4，后者是 a 在内存地址中的第一个字节。考虑到 int 值占 4 字节，这意味着 a+1 指向 a[1].
事实上，在 C 语言中数组索引是指针运算的语法糖：a[i] 等于 *(a+i)。你可以在 GDB 中尝试一下。

C
```
= preserve do
  :escaped
    (gdb) print a[0]
    $4 = 1
    (gdb) print *(a + 0)
    $5 = 1
    (gdb) print a[1]
    $6 = 2
    (gdb) print *(a + 1)
    $7 = 2
    (gdb) print a[2]
    $8 = 3
    (gdb) print *(a + 2)
    $9 = 3
```
我们已经看到在某些情况下，a 表现的像一个数组，在另一些情况下表现得像一个指向它首元素的指针。接下来会发生什么呢？
答案是当一个数组名在 C 语言表达式中使用时，它“退化”成指向这个数组首元素的指针。这个规则只有两个例外：当数组名传递给 sizeof 函数时，当数组名传递给操作数 & 时。
事实上，a 在传递给操作数 & 时并没有“退化”成一个指针，这就带来一个有趣的问题：由“退化”变成的指针和 &a 存在区别吗？
数值上讲，他们都表示相同的地址：

C
```
= preserve do
  :escaped
    (gdb) x/4xb a
    0x7fff5fbff56c: 0x01  0x00  0x00  0x00
    (gdb) x/4xb &a
    0x7fff5fbff56c: 0x01  0x00  0x00  0x00
```
然而，他们的类型是不同的。我们已经看到 a 退化的值是指向 a首元素的指针；这个必须是类型 int *。对于类型 &a，我们可以直接询问 GDB：

C
```
= preserve do
  :escaped
    (gdb) ptype &a
    type = int (*)[3]
```
从显示上看，&a 是一个指向 3 个整数数组的指针。这就说明：当传递给 & 时，a 没有退化，a 有了一个类型，是 int[3]。
通过测试他们在指针运算时的表现，你可以观察到 a 的退化值和 &a 的明显区别。

C
```
= preserve do
  :escaped
    (gdb) print a + 1
    $10 = (int *) 0x7fff5fbff570
    (gdb) print &a + 1
    $11 = (int (*)[3]) 0x7fff5fbff578
```
注意到对 a 增加 1 等于对 a 的地址增加 4，与此同时，对 &a 增加 1 等于对 a 的地址增加 12！
实际上 a 退化成的指针是 &a[0];

C
```
= preserve do
  :escaped
    (gdb) print &a[0]
    $11 = (int *) 0x7fff5fbff56c
```
## 结论
希望我已经向你证明 GDB 是学习 C 语言的一个灵巧而有富有探索性的环境。你能使用 print 打印表达式的值，使用 x 查看内存中原始字节，使用 ptype 配合类型系统进行问题修补。
如果你想要进一步对使用 GDB 学习 C 语言进行尝试，我有一些建议如下：
- 1.用 gdb 通过 Ksplice 指针挑战。
- 2.研究结构体是怎样在内存中存储的？ 他们与数组比较又有什么异同？
- 3.使用 GDB 的 disassemble 命令学习汇编语言！一个特别有趣的练习是研究函数调用栈是如何工作的。
- 4.试试 GDB 的 “ tui ”模式，这个模式在常规 GDB 顶层提供一个图像化的 ncurses 层(Ncurses 提供字符终端处理库，包括面板和菜单）。在 OS X 系统中，你可能需要用源代码安装 GDB。
Alan 是 Hacker School 的推广者。他想要感谢 David Albert、Tom Ballinger、Nicholas Bergson-Shilcock 和 Amy Dyer 给予非常有帮助的反馈。
