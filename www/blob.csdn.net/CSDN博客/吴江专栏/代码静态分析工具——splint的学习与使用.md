# 代码静态分析工具——splint的学习与使用 - 吴江专栏 - CSDN博客





2017年03月25日 23:20:38[吴江2000](https://me.csdn.net/u014775175)阅读数：3043








**引言**


最近在项目中使用了静态程序分析工具[PC-Lint](http://www.gimpel.com/html/lintinfo.htm)，体会到它在项目实施中带给开发人员的方便。PC-Lint是一款针对C/C++语言、windows平台的静态分析工具，FlexeLint是针对其他平台的PC-Lint版本。由于PC-Lint/FlexeLint是商业的程序分析工具，不便于大家对其进行学习和使用，因而下面我将介绍一个针对C语言的开源程序静态分析工具——[splint](http://www.splint.org/)。

**静态程序分析**


先来说说什么是“静态程序分析(Static program analysis)”，静态程序分析是指使用自动化工具软件对程序源代码进行检查，以分析程序行为的技术，应用于程序的正确性检查、安全缺陷检测、程序优化等。它的特点就是不执行程序，相反，通过在真实或模拟环境中执行程序进行分析的方法称为“动态程序分析(Dynamic program analysis)”。


那在什么情况下需要进行静态程序分析呢？静态程序分析往往作为一个多人参与的项目中代码审查过程的一个阶段，因编写完一部分代码之后就可以进行静态分析，分析过程不需要执行整个程序，这有助于在项目早期发现以下问题：变量声明了但未使用、变量类型不匹配、变量在使用前未定义、不可达代码、死循环、数组越界、内存泄漏等。下图说明了静态程序分析在进行项目编码过程中所处的位置：

![静态分析工具位置](http://images.cnblogs.com/cnblogs_com/bangerlee/320012/r_555.jpg)


从上图可以知道，静态分析工具在代码通过编译之后再对代码进行分析。我们会问：静态分析工具与编译器相比，所做的工作有什么不同？静态分析工具相比编译器，对代码进行了更加严格的检查，像数组越界访问、内存泄漏、使用不当的类型转换等问题，都可以通过静态分析工具检查出来，我们甚至可以在分析工具的分析标准里定义代码的编写规范，在检测到不符合编写规范的代码时抛出告警，这些功能都是编译器没有的。




既然静态分析工具发挥了不小的作用，何不在编译器里兼备静态分析的功能？对于这个问题，S. C. Johnson（他是最古老的静态分析工具Lint的作者）在其1978年发表的论文[《Lint,
 a C Program Checker》](http://files.cnblogs.com/bangerlee/10.1.1.56.1841.pdf)中给出了他的答案：“Lint与C编译器在功能上的分离既有历史原因，也有现实的意义。编译器负责把C源程序快速、高效地转变为可执行文件，不对代码做类型检查（特别是对分别编译的程序），有益于做到快速与高效。而Lint没有“高效”的要求，可以花更多时间对代码进行更深入、仔细的检查。”




针对空指针提取、未定义变量使用、类型转换、内存管理、函数接口定义等，我们可以在静态分析工具里制定不同的检测标准，以下曲线图说明了在使用splint进行分析时，检测标准与splint运行的开销所对应的关系，从另一个角度看，也说明了静态分析工具与编译器的关系：

![splint effort-benefit curve](http://images.cnblogs.com/cnblogs_com/bangerlee/320012/r_Screenshot.png)

**splint**


掌握了“静态分析”等概念之后，我们再来看splint。


在Linux命令行下，splint的使用很简单，检测文件*.c，只要这样使用就可以了：

splint *.c

**1.splint消息**


我们通过以下例子来认识典型的splint告警信息：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1//splint_msg.c 2int func_splint_msg1(void) 3{ 4int a; 5return0; 6} 7int func_splint_msg2(void) 8{ 9int* a = (int*)malloc(sizeof(int));10    a = NULL;11return0;12}
![复制代码](http://common.cnblogs.com/images/copycode.gif)



运行splint splint_msg.c之后，我们来看输出的告警信息：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
splint_msg.c: (in function func_splint_msg1)splint_msg.c:4:6: Variable a declared but not used  A variable is declared but never used. Use /*@unused@*/ in front of  declaration to suppress message. (Use -varuse to inhibit warning)splint_msg.c: (in function func_splint_msg2)splint_msg.c:10:2: Fresh storage a (type int *) not released before assignment:                      a = NULL  A memory leak has been detected. Storage allocated locally is not released  before the last reference to it is lost. (Use -mustfreefresh to inhibit  warning)   splint_msg.c:9:37: Fresh storage a createdFinished checking --- 2 code warnings
![复制代码](http://common.cnblogs.com/images/copycode.gif)


蓝色字体部分：给出告警所在函数名，在函数的第一个警告消息报告前打印；

红色字体部分：消息的正文，文件名、行号、列号显示在的警告的正文前；


黑色字体部分：是有关该可疑错误的详细信息，包含一些怎样去掉这个消息的信息；

绿色字体部分：给出格外的位置信息，这里消息给出了是在哪里申请了这个可能泄露的内存。

**2.检查控制**


splint提供了三种方式可进行检查的控制，分别是.splintrc配置文件、flags标志和格式化注释。

**flags**:splint支持几百个标志用来控制检查和消息报告，使用时标志前加’+‘或’-’，'+'标志开启这个标志，'-'表示关闭此标志，下面例子展示了flags标志的用法：

splint -showcol a.c   //在检测a.c时，告警消息中列数不被打印splint -varuse  a.c   //在检测a.c时，告警消息中未使用变量告警不被打印****

**.splintrc配置文件**:在使用源码安装splint之后，.splintrc文件将被安装在主目录下，.splintrc文件中对一些标志作了默认的设定，命令行中指定的flags标志会覆盖.splintrc文件中的标志。

**格式化注释**：格式化注释提供一个类型、变量或函数的格外的信息，可以控制标志设置，增加检查效果，所有格式化注释都以/*@开始，@*/结束，比如在函数参数前加/*@null@*/，表示该参数可能是NULL，做检测时，splint会加强对该参数的值的检测。

**3.检测分析内容**

**1.解引用空指针(Null Dereferences)**


在Unix操作系统中，解引用空指针将导致我们在程序运行时产生段错误(Segmentation fault)，一个简单的解引用空指针例子如下：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
1//null_dereferences.c2int func_null_dereferences(void)3{4int* a = NULL;5return*a;6}
![复制代码](http://common.cnblogs.com/images/copycode.gif)



执行splint null_dereference.c命令，将产生以下告警消息：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
null_dereference.c: (in function func_null_dereferences)null_dereference.c:5:10: Dereference of null pointer a: *a  A possibly null pointer is dereferenced.  Value is either the result of a  function which may return null (in which case, code should check it is not  null), or a global, parameter or structure field declared with the null  qualifier. (Use -nullderef to inhibit warning)   null_dereference.c:4:11: Storage a becomes nullFinished checking --- 1 code warnin
![复制代码](http://common.cnblogs.com/images/copycode.gif)


**2.类型(Types)**


我们在编程中经常用到强制类型转换，将有符号值转换为无符号值、大范围类型值赋值给小范围类型，程序运行的结果会出无我们的预料。


![复制代码](http://common.cnblogs.com/images/copycode.gif)
1//types.c2void splint_types(void)3{4short a =0;5long b =32768;6    a = b;7return;8}
![复制代码](http://common.cnblogs.com/images/copycode.gif)



执行splint types.c命令，将产生以下告警消息：

types.c: (in function splint_types)types.c:6:2: Assignment of longint to shortint: a = b  To ignore type qualifiers in type comparisons use +ignorequals.Finished checking ---1 code warning

**3.内存管理(Memory Management)**


C语言程序中，将近半数的bug归功于内存管理问题，关乎内存的bug难以发现并且会给程序带来致命的破坏。由内存释放所产生的问题，我们可以将其分为两种：
- 当尚有其他指针引用的时候，释放一块空间


![复制代码](http://common.cnblogs.com/images/copycode.gif)
1//memory_management1.c2void memory_management1(void)3{4int* a = (int*)malloc(sizeof(int));5int* b = a;6    free(a);7*b =0;8return;9}
![复制代码](http://common.cnblogs.com/images/copycode.gif)



在上面这个例子中，指针a与b指向同一块内存，但在内存释放之后仍对b指向的内容进行赋值操作，我们来看splint 

memory_management1.c的结果：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
memory_management1.c: (in function memory_management1)memory_management1.c:7:3: Variable b used after being released  Memory is used after it has been released (either by passing as an only param  or assigning to an only global). (Use -usereleased to inhibit warning)   memory_management1.c:6:7: Storage b releasedmemory_management1.c:7:3: Dereference of possibly null pointer b: *b  A possibly null pointer is dereferenced.  Value is either the result of a  function which may returnnull (in which case, code should check it is not  null), or a global, parameter or structure field declared with the null  qualifier. (Use -nullderef to inhibit warning)   memory_management1.c:5:11: Storage b may become nullFinished checking ---2 code warnings
![复制代码](http://common.cnblogs.com/images/copycode.gif)



检查结果中包含了两个告警，第一个指出我们使用了b指针，而它所指向的内存已被释放；第二个是对解引用空指针的告警。
- 当最后一个指针引用丢失的时候，其指向的空间尚未释放


![复制代码](http://common.cnblogs.com/images/copycode.gif)
1//memory_management2.c2void memory_management2(void)3{4int* a = (int*)malloc(sizeof(int));5     a = NULL;6return;7}
![复制代码](http://common.cnblogs.com/images/copycode.gif)



这个例子中内存尚未释放，就将指向它的唯一指针赋值为NULL，我们来看splint memory_management2.c的检测结果：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
memory_management2.c: (in function memory_management2)memory_management2.c:5:2: Fresh storage a (type int*) not released before assignment:                 a = NULL  A memory leak has been detected. Storage allocated locally is not released  before the last reference to it is lost. (Use -mustfreefresh to inhibit  warning)   memory_management2.c:4:37: Fresh storage a createdFinished checking ---1 code warning
![复制代码](http://common.cnblogs.com/images/copycode.gif)



splint抛出一个告警：类型为int*的a在进行a = NULL赋值前没有释放新分配的空间。

**4.缓存边界(Buffer Sizes)**


splint会对数组边界、字符串边界作检测，使用时需要加上+bounds的标志，我们来看下面的例子：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
1//bounds1.c2void bounds1(void)3{4int a[10];5    a[10] =0;6return;7}
![复制代码](http://common.cnblogs.com/images/copycode.gif)



使用splint +bounds bounds1.c命令对其进行检测，结果如下：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
bounds1.c: (in function bounds1)bounds1.c:5:2: Likely out-of-bounds store: a[10]    Unable to resolve constraint:    requires 9>=10     needed to satisfy precondition:    requires maxSet(a @ bounds1.c:5:2) >=10  A memory write may write to an address beyond the allocated buffer. (Use  -likelyboundswrite to inhibit warning)Finished checking ---1 code warning
![复制代码](http://common.cnblogs.com/images/copycode.gif)



告警消息提示数组越界，访问超出我们申请的buffer大小范围。再看一个例子：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1//bounds2.c 2void bounds2(char* str) 3{ 4char* tmp = getenv("HOME"); 5if(tmp != NULL) 6    { 7        strcpy(str, tmp); 8    } 9return;10}
![复制代码](http://common.cnblogs.com/images/copycode.gif)



不对这个例子进行详细检查，可能我们不能发现其中隐含的问题，执行splint +bounds bounds2.c之后，会抛出如下告警：


![复制代码](http://common.cnblogs.com/images/copycode.gif)
bounds2.c: (in function bounds2)bounds2.c:7:3: Possible out-of-bounds store: strcpy(str, tmp)    Unable to resolve constraint:    requires maxSet(str @ bounds2.c:7:10) >= maxRead(getenv("HOME") @    bounds2.c:4:14)     needed to satisfy precondition:    requires maxSet(str @ bounds2.c:7:10) >= maxRead(tmp @ bounds2.c:7:15)     derived from strcpy precondition: requires maxSet(<parameter 1>) >=    maxRead(<parameter 2>)  A memory write may write to an address beyond the allocated buffer. (Use  -boundswrite to inhibit warning)Finished checking ---1 code warning
![复制代码](http://common.cnblogs.com/images/copycode.gif)



告警消息提示我们：在使用strcpy(str, tmp)进行字符串复制时，可能出现越界错误，因为str的大小可能不足以容纳环境变量“HOME”对应的字符串。绿色字体的内容指示了如何消除告警消息。

****

**小结**


这里仅给出了splint检查的4种检测：解引用空指针、类型、内存管理、缓存边界，除此之外，splint还对宏(Macros)、函数接口(Function Interfaces)、控制流(Control Flow)等内容作检测，很多检测标志和格式化注释都未在本文中提到，更详细的内容请查看[splint使用手册](http://www.splint.org/manual/manual.html#memory)。




不管pc-lint、splint等静态程序分析工具的功能多么强大，它们对程序的检查也有疏漏的地方，工具的使用并不能提高我们的编程能力，我们更应该通过它们学习各种编码错误和代码隐患，凭积累的编码知识把程序隐患扼杀在摇篮里。




你在项目实施中是否遇到过隐藏的bug，导致返工呢？在你的项目中是否使用了静态程序分析工具，它起到多大的作用？说出来与大家一块分享吧～








Reference:[《Splint Manual》](http://www.splint.org/manual/manual.html#memory)

[《Lint, a C Program Checker》](http://files.cnblogs.com/bangerlee/10.1.1.56.1841.pdf)



