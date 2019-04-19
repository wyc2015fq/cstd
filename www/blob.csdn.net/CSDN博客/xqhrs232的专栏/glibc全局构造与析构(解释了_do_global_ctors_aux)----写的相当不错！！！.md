# glibc全局构造与析构(解释了_do_global_ctors_aux)----写的相当不错！！！ - xqhrs232的专栏 - CSDN博客
2018年04月09日 14:56:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：189
原文地址::[http://www.xuebuyuan.com/839077.html](http://www.xuebuyuan.com/839077.html)
**11.4  C++全局构造与析构**
在C++的世界里，入口函数还肩负着另一个艰巨的使命，那就是在main的前后完成全局变量的构造与析构。本节将介绍在glibc和MSVCRT的努力下，这件事是如何完成的。
**11.4.1  glibc全局构造与析构（1）**
在前面介绍glibc的启动文件时已经介绍了".init"和".finit"段，我们知道这两个段中的代码最终会被拼成两个函数_init()和_finit()，这两个函数会先于/后于main函数执行。但是它们具体是在什么时候被执行的呢？由谁来负责调用它们呢？它们又是如何进行全局对象的构造和析构的呢？为了解决这些问题，这一节将继续沿着本章第一节从_start入口函数开始的那条线进行摸索，顺藤摸瓜地找到这些问题的答案。
为了表述方便，下面使用这样的代码编译出来的可执行文件进行分析：
class HelloWorld
{
public:
HelloWorld();
~HelloWorld();
};
HelloWorld Hw;
HelloWorld::HelloWorld()
{
......
}
HelloWorld::~HelloWorld()
{
......
}int main()
{
return 0;    
}
为了了解全局对象的构造细节，对程序的启动过程进行更深一步的研究是必须的。在本章的第一节里，由_start传递进来的init函数指针究竟指向什么？通过对地址的跟踪，init实际指向了__libc_csu_init函数。这个函数位于Glibc源代码目录的csu/Elf-init.c，让我们来看看这个函数的定义：
_start -> __libc_start_main -> __libc_csu_init:void __libc_csu_init (int argc, char **argv, char **envp)
{
…
_init ();
const size_t size = __init_array_end - 
__init_array_start;
for (size_t i = 0; i < size; i++)
(*__init_array_start [i]) (argc, argv, envp);
}
这段代码调用了_init函数。那么_init()是什么呢？是不是想起来前面介绍过的定义在crti.o的_init()函数呢？没错，__libc_csu_init里面调用的正是".init"段，也就是说，用户所有放在".init"段的代码就将在这里被执行。
看到这里，似乎我们的线索要断了，因为"_init"函数的实际内容并不定义在Glibc里面，它是由各个输入目标文件中的".init"段拼凑而来的。不过除了分析源代码之外，还有一个终极必杀就是反汇编目标代码，我们随意反汇编一个可执行文件就可以发现_init()函数的内容：
_start -> __libc_start_main -> __libc_csu_init -> _init：Disassembly of section .init:80480f4 <_init>:
80480f4:       55                      push   %ebp
80480f5:       89 e5                   mov    %esp,%ebp
80480f7:       53                      push   %ebx
80480f8:       83 ec 04                sub    $0x4,%esp
80480fb:       e8 00 00 00 00          call   8048100 <_init+0xc>
8048100:       5b                      pop    %ebx
8048101:       81 c3 9c 39 07 00       add    $0x7399c,%ebx
8048107:       8b 93 fc ff ff ff       mov    -0x4(%ebx),%edx
804810d:       85 d2                   test   %edx,%edx
804810f:       74 05                   je     8048116 <_init+0x22>
8048111:       e8 ea 7e fb f7          call   0 <_nl_current_LC_CTYPE>
8048116:       e8 95 00 00 00          call   80481b0 <frame_dummy>
804811b:       e8 b0 6e 05 00          call   809efd0 <__do_global_ctors_aux>
8048120:       58                      pop    %eax
8048121:       5b                      pop    %ebx
8048122:       c9                      leave
8048123:       c3                      ret
可以看到_init调用了一个叫做__do_global_ctors_aux的函数，如果你在glibc源代码里面查找这个函数，是不可能找到它的。因为它并不属于glibc，而是来自于GCC提供的一个目标文件crtbegin.o。我们在上一节中也介绍过，链接器在进行最终链接时，有一部分目标文件是来自于GCC，它们是那些与语言密切相关的支持函数。很明显，C++的全局对象构造是与语言密切相关的，相应负责构造的函数来自于GCC也非常容易理解。
即使它在GCC的源代码中，我们也把它揪出来。它位于gcc/Crtstuff.c，把它简化以后代码如下：
_start -> __libc_start_main -> __libc_csu_init 
-> _init -> __do_global_ctors_aux：void __do_global_ctors_aux(void)
{
/* Call constructor functions.  */
unsigned long nptrs = (unsigned long) __CTOR_LIST__[0]; 
unsigned i;    for (i = nptrs; i >= 1; i--)
__CTOR_LIST__[i] ();
}
上面这段代码首先将__CTOR_LIST__数组的第一个元素当做数组元素的个数，然后将第一个元素之后的元素都当做是函数指针，并一一调用。这段代码的意图非常明显，我们都可以猜到__CTOR_LIST__里面存放的是什么，没错，__CTOR_LIST__里面存放的就是所有全局对象的构造函数的指针。那么接下来的焦点很明显就是__CTOR_LIST__了，这个数组怎么来的，由谁负责构建这个数组？
__CTOR_LIST__
这里不得不暂时放下__CTOR_LIST__的身世来历，从GCC方面再追究__CTOR_LIST__未免有些乏味，我们不妨从问题的另一端，也就是从编译器如何生产全局构造函数的角度来看看全局构造函数是怎么实现的。
对于每个编译单元(.cpp)，GCC编译器会遍历其中所有的全局对象，生成一个特殊的函数，这个特殊函数的作用就是对本编译单元里的所有全局对象进行初始化。我们可以通过对本节开头的代码进行反汇编得到一些粗略的信息，可以看到GCC在目标代码中生成了一个名为_GLOBAL__I_Hw的函数，由这个函数负责本编译单元所有的全局/静态对象的构造和析构，它的代码可以表示为：
static void GLOBAL__I_Hw(void)
{
Hw::Hw(); // 构造对象
atexit(__tcf_1); // 一个神秘的函数叫做__tcf_1被注册到了exit
}
**11.4.1  glibc全局构造与析构（2）**
我们暂且不管这里的神秘函数__tcf_1，它将在本节的最后部分讲到。GLOBAL__I_Hw作为特殊的函数当然也享受特殊待遇，一旦一个目标文件里有这样的函数，编译器会在这个编译单元产生的目标文件(.o)的".ctors"段里放置一个指针，这个指针指向的便是GLOBAL__I_Hw。
那么把每个目标文件的复杂全局/静态对象构造的函数地址放在一个特殊的段里面有什么好处呢？当然不为别的，为的是能够让链接器把这些特殊的段收集起来，收集齐所有的全局构造函数后就可以在初始化的时候进行构造了。
在编译器为每个编译单元生成一份特殊函数之后，链接器在连接这些目标文件时，会将同名的段合并在一起，这样，每个目标文件的.ctors段将会被合并为一个.ctors段，其中的内容是各个目标文件的.ctors段的内存拼接而成。由于每个目标文件的.ctors段都只存储了一个指针（指向该目标文件的全局构造函数），因此拼接起来的.ctors段就成为了一个函数指针数组，每一个元素都指向一个目标文件的全局构造函数。这个指针数组不正是我们想要的全局构造函数的地址列表吗？如果能得到这个数组的地址，岂不是构造的问题就此解决了？
没错，得到这个数组的地址其实也不难，我们可以效仿前面".init"和".finit"拼凑的办法，对".ctor"段也进行拼凑。还记得在链接的时候，各个用户产生的目标文件的前后分别还要链接上一个crtbegin.o和crtend.o吧？这两个glibc自身的目标文件同样具有.ctors段，在链接的时候，这两个文件的.ctors段的内容也会被合并到最终的可执行文件中。那么这两个文件的.ctors段里有什么呢？
crtbegin.o：作为所有.ctors段的开头部分，crtbegin.o的.ctor段里面存储的是一个4字节的 1(0xFFFFFFFF)，由链接器负责将这个数字改成全局构造函数的数量。然后这个段还将起始地址定义成符号__CTOR_LIST__，这样实际上__CTOR_LIST__所代表的就是所有.ctor段最终合并后的起始地址了。
crtend.o：这个文件里面的.ctors内容就更简单了，它的内容就是一个0，然后定义了一个符号__CTOR_END__，指向.ctor段的末尾。
在前面的章节中已经介绍过了，链接器在链接用户的目标文件的时候，crtbegin.o总是处在用户目标文件的前面，而crtend.o则总是处在用户目标文件的后面。例如链接两个用户的目标文件a.o和b.o时，实际链接的目标文件将是（按顺序）ld crti.o crtbegin.o a.o b.o crtend.o crtn.o。在这里我们忽略crti.o和crtn.o，因为这两个目标文件和全局构造无关。在合并crtbegin.o、用户目标文件和crtend.o时，链接器按顺序拼接这些文件的.ctors段，因此最终形成.ctors段的过程将如图11-10所示。
（点击查看大图）图11-10  .ctor段的形成
在了解了可执行文件的.ctors段的结构之后，再回过头来看__do_global_ctor_aux的代码就很容易了。__do_global_ctor_aux从__CTOR_LIST__的下一个位置开始，按顺序执行函数指针，直到遇上NULL（__CTOR_END__）。如此每个目标文件的全局构造函数都能被调用。
【小实验】
在main前调用函数：
glibc的全局构造函数是放置在.ctors段里的，因此如果我们手动在.ctors段里添加一些函数指针，就可以让这些函数在全局构造的时候（main之前）调用：
#include <stdio.h>
void my_init(void) 
{
printf("Hello ");
}
typedef void (*ctor_t)(void); 
//在.ctors段里添加一个函数指针
ctor_t __attribute__((section (".ctors"))) my_init_p = &my_init;
int main() 
{
printf("World!/n");
return 0;
}
如果运行此程序，结果将打印出：Hello World!
当然，事实上，gcc里有更加直接的办法来达到相同的目的，那就是使用__attribute__((constructor))
示例如下：
#include <stdio.h>
void my_init(void) __attribute__ ((constructor));
void my_init(void) 
{
printf("Hello ");
}
int main() 
{
printf("World!/n");
return 0;
}
**11.4.1  glibc全局构造与析构（3）**
**析构**
对于早期的glibc和GCC，在完成了对象的构造之后，在程序结束之前，crt还要进行对象的析构。实际上正常的全局对象析构与前面介绍的构造在过程上是完全类似的，而且所有的函数、符号名都一一对应，比如".init"变成了".finit"、"__do_global_ctor_aux"变成了"__do_global_dtor_aux"、"__CTOR_LIST__"变成了"__DTOR_LIST__"等。在前面介绍入口函数时我们可以看到，__libc_start_main将"__libc_csu_fini"通过__cxa_exit()注册到退出列表中，这样当进程退出前exit()里面就会调用"__libc_csu_fini"。"_fini"的原理和"_init"基本是一样的，在这里不再一一赘述了。
不过这样做的好处是为了保证全局对象构造和析构的顺序（即先构造后析构），链接器必须包装所有的".dtor"段的合并顺序必须是".ctor"的严格反序，这增加了链接器的工作量，于是后来人们放弃了这种做法，采用了一种新的做法，就是通过__cxa_atexit()在exit()函数中注册进程退出回调函数来实现析构。
这就要回到我们之前在每个编译单元的全局构造函数GLOBAL__I_Hw()中看到的神秘函数。编译器对每个编译单元的全局对象，都会生成一个特殊的函数来调用这个编译单元的所有全局对象的析构函数，它的调用顺序与GLOBAL__I_Hw()调用构造函数的顺序刚好相反。例如对于前面的例子中的代码，编译器生成的所谓的神秘函数内容大致是：
static void __tcf_1(void) //这个名字由编译器生成
{
Hw.~HelloWorld();
}
此函数负责析构Hw对象，由于在GLOBAL__I_Hw中我们通过__cxa_exit()注册了__tcf_1，而且通过__cxa_exit()注册的函数在进程退出时被调用的顺序满足先注册后调用的属性，与构造和析构的顺序完全符合，于是它就很自然被用于析构函数的实现了。
当然在本节中介绍glibc/GCC的全局对象构造和析构时，省略了不少我们认为超出了本书所要强调的范围细节，真正的构造和析构过程比上面介绍的要复杂一些，并且在动态链接和静态链接不同的情况下，构造和析构还略有不同。但是不管哪种情况，基本的原理都是相通的，按照上面介绍的步骤和路径，相信读者也能够自己重新根据真实的情况梳理清楚这条调用路线。
由于全局对象的构建和析构都是由运行库完成的，于是在程序或共享库中有全局对象时，记得不能使用"-nonstartfiles"或"-nostdlib"选项，否则，构建与析构函数将不能正常执行（除非你很清楚自己的行为，并且手工构造和析构全局对象）。
**Collect2**
我们在第2章时曾经碰到过collect2这个程序，在链接时它代替ld成为了最终链接器，一般情况下就可以简单地将它看成ld。实际上collect2是ld的一个包装，它最终还是调用ld完成所有的链接工作，那么collect2这个程序的作用是什么呢？
在有些系统上，汇编器和链接器并不支持本节中所介绍的".init"".ctor"这种机制，于是为了实现在main函数前执行代码，必须在链接时进行特殊的处理。Collect2这个程序就是用来实现这个功能的，它会"收集"（collect）所有输入目标文件中那些命名特殊的符号，这些特殊的符号表明它们是全局构造函数或在main前执行，collect2会生成一个临时的.c文件，将这些符号的地址收集成一个数组，然后放到这个.c文件里面，编译后与其他目标文件一起被链接到最终的输出文件中。
在这些平台上，GCC编译器也会在main函数的开始部分产生一个__main函数的调用，这个函数实际上就是负责collect2收集来的那些函数。__main函数也是GCC所提供的目标文件的一部分，如果我们使用"-nostdlib"编译程序，可能得到__main函数未定义的错误，这时候只要加上"-lgcc"把它链接上即可。

