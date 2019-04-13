
# valgrind工具使用详解 - 高科的专栏 - CSDN博客

2015年01月08日 11:33:45[高科](https://me.csdn.net/pbymw8iwm)阅读数：6265


zz自 http://blog.csdn.net/destina/article/details/6198443  感谢作者的分享！


## 一  valgrind是什么？
Valgrind是一套Linux下，开放源代码（GPL V2）的仿真调试工具的集合。Valgrind由内核（core）以及基于内核的其他调试工具组成。内核类似于一个框架（framework），它模拟了一个CPU环境，并提供服务给其他工具；而其他工具则类似于插件 (plug-in)，利用内核提供的服务完成各种特定的内存调试任务。Valgrind的体系结构如下图所示：

### valgrind的结构图
![点击看大图](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image001.jpg)

Valgrind包括如下一些工具：

Memcheck。这是valgrind应用最广泛的工具，一个重量级的内存检查器，能够发现开发中绝大多数内存错误使用情况，比如：使用未初始化的内存，使用已经释放了的内存，内存访问越界等。这也是本文将重点介绍的部分。
Callgrind。它主要用来检查程序中函数调用过程中出现的问题。
Cachegrind。它主要用来检查程序中缓存使用出现的问题。
Helgrind。它主要用来检查多线程程序中出现的竞争问题。
Massif。它主要用来检查程序中堆栈使用中出现的问题。
Extension。可以利用core提供的功能，自己编写特定的内存调试工具

### linux下内存空间布置：
![典型内存空间布局](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image002.jpg)
一个典型的Linux C程序内存空间由如下几部分组成：
代码段（.text）。这里存放的是CPU要执行的指令。代码段是可共享的，相同的代码在内存中只会有一个拷贝，同时这个段是只读的，防止程序由于错误而修改自身的指令。
初始化数据段（.data）。这里存放的是程序中需要明确赋初始值的变量，例如位于所有函数之外的全局变量：int val="100"。需要强调的是，以上两段都是位于程序的可执行文件中，内核在调用exec函数启动该程序时从源程序文件中读入。
未初始化数据段（.bss）。位于这一段中的数据，内核在执行该程序前，将其初始化为0或者null。例如出现在任何函数之外的全局变量：int sum;
堆（Heap）。这个段用于在程序中进行动态内存申请，例如经常用到的malloc，new系列函数就是从这个段中申请内存。
栈（Stack）。函数中的局部变量以及在函数调用过程中产生的临时变量都保存在此段中。
Memcheck 能够检测出内存问题，关键在于其建立了两个全局表。
Valid-Value 表：
对于进程的整个地址空间中的每一个字节(byte)，都有与之对应的 8 个 bits；对于 CPU 的每个寄存器，也有一个与之对应的 bit 向量。这些 bits 负责记录该字节或者寄存器值是否具有有效的、已初始化的值。
Valid-Address 表
对于进程整个地址空间中的每一个字节(byte)，还有与之对应的 1 个 bit，负责记录该地址是否能够被读写。
**检测原理：**
当要读写内存中某个字节时，首先检查这个字节对应的 A bit。如果该A bit显示该位置是无效位置，memcheck 则报告读写错误。
内核（core）类似于一个虚拟的 CPU 环境，这样当内存中的某个字节被加载到真实的 CPU 中时，该字节对应的 V bit 也被加载到虚拟的 CPU 环境中。一旦寄存器中的值，被用来产生内存地址，或者该值能够影响程序输出，则 memcheck 会检查对应的V bits，如果该值尚未初始化，则会报告使用未初始化内存错误。
## Valgrind
## 使用
用法:valgrind[options] prog-and-args [options]: 常用选项，适用于所有Valgrind工具
-tool=<name> 最常用的选项。运行valgrind中名为toolname的工具。默认memcheck。
h –help 显示帮助信息。
-version 显示valgrind内核的版本，每个工具都有各自的版本。
q –quiet 安静地运行，只打印错误信息。
v –verbose 更详细的信息, 增加错误数统计。
-trace-children=no|yes 跟踪子线程? [no]
-track-fds=no|yes 跟踪打开的文件描述？[no]
-time-stamp=no|yes 增加时间戳到LOG信息? [no]
-log-fd=<number> 输出LOG到描述符文件 [2=stderr]
-log-file=<file> 将输出的信息写入到filename.PID的文件里，PID是运行程序的进行ID
-log-file-exactly=<file> 输出LOG信息到 file
-log-file-qualifier=<VAR> 取得环境变量的值来做为输出信息的文件名。 [none]
-log-socket=ipaddr:port 输出LOG到socket ，ipaddr:port

LOG信息输出
-xml=yes 将信息以xml格式输出，只有memcheck可用
-num-callers=<number> show <number> callers in stack traces [12]
-error-limit=no|yes 如果太多错误，则停止显示新错误? [yes]
-error-exitcode=<number> 如果发现错误则返回错误代码 [0=disable]
-db-attach=no|yes 当出现错误，valgrind会自动启动调试器gdb。[no]
-db-command=<command> 启动调试器的命令行选项[gdb -nw %f %p]
适用于Memcheck工具的相关选项：
-leak-check=no|summary|full 要求对leak给出详细信息? [summary]
-leak-resolution=low|med|high how much bt merging in leak check [low]
-show-reachable=no|yes show reachable blocks in leak check? [no]



## Valgrind
## 使用
## 举例（一）
下面是一段有问题的C程序代码test.c
<span style="font-size: 16px;">＃i nclude <stdlib.h>
void f(void)
{
   int* x = malloc(10 * sizeof(int));
   x[10] = 0;  //问题1: 数组下标越界
}                  //问题2: 内存没有释放</span>
<span style="font-size: 16px;">int main(void)
{
   f();
   return 0;
 }</span>
<span style="font-size: 16px;">1、 编译程序test.c
gcc -Wall test.c -g -o test
2、 <span class="search">使用</span><span class="search">Valgrind</span>检查程序BUG
<span class="search">valgrind</span> --tool=memcheck --leak-check=full ./test</span>

## 使用未初始化内存问题
**问题分析：**
对于位于程序中不同段的变量，其初始值是不同的，全局变量和静态变量初始值为0，而局部变量和动态申请的变量，其初始值为随机值。如果程序使用了为随机值的变量，那么程序的行为就变得不可预期。
下面的程序就是一种常见的，使用了未初始化的变量的情况。数组a是局部变量，其初始值为随机值，而在初始化时并没有给其所有数组成员初始化，如此在接下来使用这个数组时就潜在有内存问题。
![清单 3](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image006.jpg)
**结果分析：**
假设这个文件名为：**badloop.c**，生成的可执行程序为**badloop**。用memcheck对其进行测试，输出如下。
![清单 4](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image007.jpg)
输出结果显示，在该程序第11行中，程序的跳转依赖于一个未初始化的变量。准确的发现了上述程序中存在的问题。

## 内存读写越界
**问题分析：**
这种情况是指：访问了你不应该/没有权限访问的内存地址空间，比如访问数组时越界；对动态内存访问时超出了申请的内存大小范围。下面的程序就是一个典型的数组越界问题。pt是一个局部数组变量，其大小为4，p初始指向pt数组的起始地址，但在对p循环叠加后，p超出了pt数组的范围，如果此时再对p进行写操作，那么后果将不可预期。
![清单 5](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image008.jpg)
**结果分析：**
假设这个文件名为badacc.cpp，生成的可执行程序为badacc，用memcheck对其进行测试，输出如下。
![清单 6](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image009.jpg)
输出结果显示，在该程序的第15行，进行了非法的写操作；在第16行，进行了非法读操作。准确地发现了上述问题。
## 内存覆盖
**问题分析：**
C 语言的强大和可怕之处在于其可以直接操作内存，C 标准库中提供了大量这样的函数，比如 strcpy, strncpy, memcpy, strcat 等，这些函数有一个共同的特点就是需要设置源地址 (src)，和目标地址(dst)，src 和 dst 指向的地址不能发生重叠，否则结果将不可预期。
下面就是一个 src 和 dst 发生重叠的例子。在 15 与 17 行中，src 和 dst 所指向的地址相差 20，但指定的拷贝长度却是 21，这样就会把之前的拷贝值覆盖。第 24 行程序类似，src(x+20) 与 dst(x) 所指向的地址相差 20，但 dst 的长度却为 21，这样也会发生内存覆盖。
![清单 7](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image010.jpg)
**结果分析：**
假设这个文件名为 badlap.cpp，生成的可执行程序为 badlap，用 memcheck 对其进行测试，输出如下。
![点击看大图](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image011.jpg)
输出结果显示上述程序中第15，17，24行，源地址和目标地址设置出现重叠。准确的发现了上述问题。
## 动态内存管理错误
**问题分析：**
常见的内存分配方式分三种：静态存储，栈上分配，堆上分配。全局变量属于静态存储，它们是在编译时就被分配了存储空间，函数内的局部变量属于栈上分配，而最灵活的内存使用方式当属堆上分配，也叫做内存动态分配了。常用的内存动态分配函数包括：malloc, alloc, realloc, new等，动态释放函数包括free, delete。
一旦成功申请了动态内存，我们就需要自己对其进行内存管理，而这又是最容易犯错误的。下面的一段程序，就包括了内存动态管理中常见的错误。
![清单 9](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image012.jpg)
常见的内存动态管理错误包括：
申请和释放不一致
由于 C++ 兼容 C，而 C 与 C++ 的内存申请和释放函数是不同的，因此在 C++ 程序中，就有两套动态内存管理函数。一条不变的规则就是采用 C 方式申请的内存就用 C 方式释放；用 C++ 方式申请的内存，用 C++ 方式释放。也就是用 malloc/alloc/realloc 方式申请的内存，用 free 释放；用 new 方式申请的内存用 delete 释放。在上述程序中，用 malloc 方式申请了内存却用 delete 来释放，虽然这在很多情况下不会有问题，但这绝对是潜在的问题。
申请和释放不匹配
申请了多少内存，在使用完成后就要释放多少。如果没有释放，或者少释放了就是内存泄露；多释放了也会产生问题。上述程序中，指针p和pt指向的是同一块内存，却被先后释放两次。
释放后仍然读写
本质上说，系统会在堆上维护一个动态内存链表，如果被释放，就意味着该块内存可以继续被分配给其他部分，如果内存被释放后再访问，就可能覆盖其他部分的信息，这是一种严重的错误，上述程序第16行中就在释放后仍然写这块内存。
**结果分析：**
假设这个文件名为badmac.cpp，生成的可执行程序为badmac，用memcheck对其进行测试，输出如下。
![清单 10](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image013.jpg)
输出结果显示，第14行分配和释放函数不一致；第16行发生非法写操作，也就是往释放后的内存地址写值；第17行释放内存函数无效。准确地发现了上述三个问题。

## 内存泄漏
**问题描述：**
内存泄露（Memory leak）指的是，在程序中动态申请的内存，在使用完后既没有释放，又无法被程序的其他部分访问。内存泄露是在开发大型程序中最令人头疼的问题，以至于有人说，内存泄露是无法避免的。其实不然，防止内存泄露要从良好的编程习惯做起，另外重要的一点就是要加强单元测试（Unit Test），而memcheck就是这样一款优秀的工具。
下面是一个比较典型的内存泄露案例。main函数调用了mk函数生成树结点，可是在调用完成之后，却没有相应的函数：nodefr释放内存，这样内存中的这个树结构就无法被其他部分访问，造成了内存泄露。
在一个单独的函数中，每个人的内存泄露意识都是比较强的。但很多情况下，我们都会对malloc/free 或new/delete做一些包装，以符合我们特定的需要，无法做到在一个函数中既使用又释放。这个例子也说明了内存泄露最容易发生的地方：即两个部分的接口部分，一个函数申请内存，一个函数释放内存。并且这些函数由不同的人开发、使用，这样造成内存泄露的可能性就比较大了。这需要养成良好的单元测试习惯，将内存泄露消灭在初始阶段。
![清单 1](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image014.jpg)
![清单 11.2](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image015.jpg)

![清单 11.3](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image016.jpg)
**结果分析：**
假设上述文件名位tree.h, tree.cpp, badleak.cpp，生成的可执行程序为badleak，用memcheck对其进行测试，输出如下。

![点击看大图](http://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/images/image017.jpg)
该示例程序是生成一棵树的过程，每个树节点的大小为12（考虑内存对齐），共8个节点。从上述输出可以看出，所有的内存泄露都被发现。Memcheck将内存泄露分为两种，一种是可能的内存泄露（Possibly lost），另外一种是确定的内存泄露（Definitely lost）。Possibly lost 是指仍然存在某个指针能够访问某块内存，但该指针指向的已经不是该内存首地址。Definitely lost 是指已经不能够访问这块内存。而Definitely
 lost又分为两种：直接的（direct）和间接的（indirect）。直接和间接的区别就是，直接是没有任何指针指向该内存，间接是指指向该内存的指针都位于内存泄露处。在上述的例子中，根节点是directly lost，而其他节点是indirectly lost。

