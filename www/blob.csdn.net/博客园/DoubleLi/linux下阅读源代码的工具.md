# linux下阅读源代码的工具 - DoubleLi - 博客园






说来真是惭愧呀.一直在用VIM 做开发.却不知道VI 里还有这么好使的工具.以前一直都是用:

find -type f -print | xargs grep -i **** 在源代码里查找.

         原来Linux下也有类似于Souce Insight工具.以下是引用人家的文章.http://blog.chinaunix.net/u1/56619/showart_1822993.html



**linux源代码分析和阅读工具比较**



Windows下的源码阅读工具Souce Insight 凭 借着其易用性和多种编程语言的支持，无疑是这个领域的“带头大哥”。Linux/UNIX环境下呢？似乎仍然是处于百花齐放，各有千秋的春秋战国时代，实 际上，似乎其环境下的任何软件都是处于一种逐鹿中原的态势，也许这就是“集市”之于“大教堂”的优势所在吧！主要的交叉索引工具有：**ctags****、****cscope****、****global****、****lxr****、****KScope****、****sourcenav****、****calltree****、****CodeViz****、ncc 、gprof** 等。 下面将就各软件的优劣作一简单介绍。这里面当中，我当前分析过他们的源码的有lxr、sourcenav、calltree、ncc。现在正在 sourcenav的基础上进行我需要的改造以更合适分析内核需要。KScope需要进一部考察和分析，如果KScope的代码分析能力（也就是 scope的能力）跟得上sourcenav可能到时会放弃sourcenav转到KScope开发，因为sourcenav的界面和交互性都不怎么好。

# 1. ctags


     ctags应该是几个里面最古老的一个了，当然也是获得支持最多的一个，vim默认就支持ctags产生的tag数据库，不过原来的ctags名符其实， 只支持C语言，且分析不够细腻。其新版本名为exuberant ctags，但是它已经远远超出了它名字的范畴，支持包括C语言在内的33种编程语言，准确度上也不错，并且它的开发仍在如火如荼地进行着，前途无量啊！
配合vim使用，基本上能满足用户的要求。
常用命令:
建立数据库命令：

exuberant-ctags

在vim编辑器中浏览：


|:ta x跳转到符号x的定义处，如果有多个符号，直接跳转到第一处:ts x列出符号x的定义:tj x可以看做上面两个命令的合并，如果只找到一个符号定义，那么直接跳转到符号定义处，如果有多个，则让用户自行选择。Ctrl+]跳转到当前光标下符号的定义处，和ta类似。Ctrl+t跳转到上一个符号定义处，和上面的配合基本上就能自由跳转了。另外两个:tn, :tp是在符号的多个定义之间跳转|
|----|

美中不足，只能查找符号的定义，不能实现交叉引用查询等其它功能。

# 2. Cscope



     相比ctags的只能查找符号的定义，cscope则显得更加丰富，虽然他支持C语言，但是它除了能查找函数的定义，还能查找函数在哪里被调用过等，所以能在一定程度上弥补ctags的不足。
常用命令：
建立数据库的命令：

cscope和vim配合使用:

不再重复vim文档了，自己通过命令
:help cscope
去详细学习吧。

比较遗憾的是，cscope的最后一次更新是在2003年，这个项目似乎也是被人抛弃了，希望以后能被人收养，或者和其它项目合并。

# 3. Global


     Global是GNU出品的交叉索引工具，支持C, C++, Yacc, Java 和PHP4五种语言，命令行下表现不错，但是和vim配合使用，就不如前面两个更加亲切了。其特色在于分析得很细腻，我所研究过的，暂时只能 soucenav能超过它。能生成交叉索引的web页，很适合用来做程序的文档。数据库是从berkeley BD 3上改过来的。但和我的要需求却有比较大的差距。因为它只有一个WEB的前端，在功能上，他现有的前端也只实现符号查询和交叉引用查询。

# 4. LXR



     Linux超文本交叉代码检索工具LXR（Linux Cross Reference），是由挪威奥斯陆大学数学系Arne Georg Gleditsch和Per Kristian Gjermshus编写的。这个工具实际上运行在Linux或者Unix平台下，通过对源代码中的所有符号建立索引，从而可以方便的检索任何一个符号，包 括函数、外部变量、文件名、宏定义等等。不仅仅是针对Linux源代码，对于C语言的其他大型的项目，都可以建立其lxr站点，以提供开发者查询代码，以 及后继开发者学习代码。 　　
     目前的lxr是专门为Linux下面的Apache服务器设计的，通过运行perl脚本，检索源代码索引文件，将数据发送到网络客户端的Web浏览器上。 任何一种平台上的Web浏览器都可以访问，这就方便了习惯在Windows平台下工作的用户。 关于lxr的英文网站为http://lxr.linux.no/ ，在中国Linux论坛http://www.linuxforum.net 上有其镜象。
     好的说完了。说说缺点，可能我分析得不深入，它的代码不是用C写的，是网页脚本，程序也很短，没有看到有应用数据库。很多的东西都是你浏览的时候才生成的，所以拿过来用或改造的潜力有限，而且代码分析的不够Global细腻。

# 5. KScope



     KScope是cscope的图形前端，他为那些不熟悉vim或者是不习惯终端作业的人提供了图形界面的选择，界面是这一款软件的一个亮点，比 source insight还好看，从windows转过来做Linux开发的人可能感觉比较亲切。这也是我最近动心的原因。另外，它还有个比较迷人的特点：能生成函 数调用树。但说百了，其实是调用graphviz画出来的。

# 6. sourcenav

     snavigator是一个不错的源代码编辑和项目管理软件。有Windows和Linux版本。网址http://sourceforge.net/projects/sourcenav 。
     基本介绍：Source-Navigator is a source code analysis tool. With it, you can edit your source code, display relationships between classes and functions and members, and display call trees.
     这种工具号称是source insight是linux对手，和确，经过我这一轮的GOOGLE后，从分析性能上，只有他和source insight的能平行的。但可操作性方面就远远没有source insight强了，很多功能用起来都没有source insight顺心。界面就比kScope差很远了，这也就是我对KScope心动的原因。和source insight比，还有一点不行的就是，需然生成交叉引用是在后台进行了，但它是一次性连续生成的，而source insight生成是，则更为优先生成客户当前的代码，source insight这一点对客户的考虑真不错。数据库方面采用的也是berkeley BD 4，他和Global一样，在数据库的使用方面，第个表只采用了两个字端，KEY和DATA，也就是说没采用Secondary Databases，这导致很多时候都是把全部的记录返回，然后再优程序来筛选，这种方式把数据库的优点全给埋没了，要知道对linux这类原码，交叉引 用数据库可能达几百M，这么干内存也不够，速度也更不用谈。我还没有研究KScope不知它在这方面怎么样，我觉得sourcenav这方面肯定要改掉。 把数据筛选的工作完全由数据库来做，他们会做得很好。
     可开发性方面，sourcenav做得还算不错，有用户文档和开发文档，代码结构也比较可能，TCL编程也直对比较容易。可以在它原来功能的基础上进行功能扩充。接下来的几编BLOG,我会对它的源码进行分析。   

# 7. 其它源码分析工具

     除了上述的体系比较完整的工具外，还有很多源源分析的小工具。这些小工具多数是用来绘制函数调用关系图的。

     绘制函数调用关系图对理解大型程序大有帮助。我想大家都有过一边读源码（并在头脑中维护一个调用栈），一边在纸上画函数调用关系，然后整理成图的经历。如 果运气好一点，借助调试器的单步跟踪功能和call stack窗口，能节约一些脑力。不过如果要分析的是脚本语言的代码，那多半只好老老实实用第一种方法了。如果在读代码之前，手边就有一份调用图，岂不妙 哉？下面举出我知道的几种免费的分析C/C++函数调用关系的工具。

    函数调用关系图（call graph）是图（graph），而且是有向图，多半还是无环图（无圈图）——如果代码中没有直接或间接的递归的话。Graphviz 是专门绘制有向图和无向图的工具，所以很多call graph分析工具都以它为后端(back end)。那么前端呢？就看各家各显神通了。

     调用图的分析分析大致可分为“静态”和“动态”两种，所谓静态分析是指在不运行待分析的程序的前提下进行分析，那么动态分析自然就是记录程序实际运行时的函数调用情况了。

    静态分析又有两种方法，一是分析源码，二是分析编译后的目标文件。

分析源码获得的调用图的质量取决于分析工具对编程语言的理解程度，比如能不能找出正确的C++重载函数。Doxygen 是源码文档化工具，也能绘制调用图，它似乎是自己分析源码获得函数调用关系的。GNU cflow也是类似的工具，不过它似乎偏重分析流程图（flowchart）。

     对编程语言的理解程度最好的当然是编译器了，所以有人想出给编译器打补丁，让它在编译时顺便记录函数调用关系。CodeViz （其灵感来自Martin Devera (Devik) 的工具 ）就属于此类，它（1.0.9版）给GCC 3.4.1打了个补丁。另外一个工具egypt 的思路更巧妙，不用大动干戈地给编译器打补丁，而是让编译器自己dump出调用关系，然后分析分析，交给Graphviz去绘图。不过也有人另起炉灶，自己写个C语言编译器（ncc ），专门分析调用图，勇气可嘉。不如要是对C++语言也这么干，成本不免太高了。分析C++的调用图，还是借助编译器比较实在。

     分析目标文件听起来挺高深，其实不然，反汇编的工作交给binutils的objdump去做，只要分析一下反汇编出来的文本文件就行了。下面是Cygwin下objdump -d a.exe的部分结果：

00401050 <_main>:
401050:       55                      push   %ebp
401051:       89 e5                   mov    %esp,%ebp
401053:       83 ec 18                sub    $0x18,%esp
   ......
40107a:       c7 44 24 04 00 20 40    movl   $0x402000,0x4(%esp)
401081:    &n

401082:       c7 04 24 02 20 40 00    movl   $0x402002,(%esp)
401089:       e8 f2 00 00 00          call   401180 <_fopen>

     从中可以看出，main()调用了fopen()。CodeViz带有分析目标文件的功能。

     动态分析是在程序运行时记录函数的调用，然后整理成调用图。与静态分析相比，它能获得更多的信息，比如函数调用的先后顺序和次数；不过也有一定的缺点，比如程序中语句的某些分支可能没有执行到，这些分支中调用的函数自然就没有记录下来。

     动态分析也有两种方法，一是借助gprof的call graph功能（参数-q），二是利用GCC的 `-finstrument-functions `参数。

gprof生成的输出如下：

index % time    self children    called     name
                0.00    0.00       4/4           foo [4]
[3]      0.0    0.00    0.00       4         bar [3]
-----------------------------------------------
                0.00    0.00       1/2           init [5]
                0.00    0.00       1/2           main [45]
[4]      0.0    0.00    0.00       2         foo [4]
                0.00    0.00       4/4           bar [3]
-----------------------------------------------
                0.00    0.00       1/1           main [45]
[5]      0.0    0.00    0.00       1         init [5]
                0.00    0.00       1/2           foo [4]
-----------------------------------------------

     从中可以看出，bar()被foo()调用了4次，foo()被init()和main()各调用了一次，init()被main()调用了一次。用 Perl脚本分析gprof的输出，生成Graphviz的dot输入，就能绘制call graph了。这样的脚本不止一个人写过：http://www.graphviz.org/Resources.php ，http://www.ioplex.com/~miallen/ 。

GCC的-finstrument-functions 参数的作用是在程序中加入hook，让它在每次进入和退出函数的时候分别调用下面这两个函数：


```
void __cyg_profile_func_enter( void *func_address, void *call_site )
                                __attribute__ ((no_instrument_function));
void __cyg_profile_func_exit ( void *func_address, void *call_site )
                                __attribute__ ((no_instrument_function));
```


     当然，这两个函数本身不能被钩住（使用no_instrument_function这个__attribute__），不然就反反复复万世不竭了:) 这里获得的是函数地址，需要用binutils中的addr2line这个小工具转换为函数名，如果是C++函数，还要用c++filt进行name demangle。具体方法在《用Graphviz 可视化函数调用 》中有详细介绍，这里不再赘述。

     从适应能力上看，源码分析法是最强的，即便源码中有语法错，头文件不全也没关系，它照样能分析个八九不离十。而基于编译器的分析法对源码的要求要高一些， 至 少能编译通过（gcc 参数 -c）——能产生object file，不一定要链接得到可执行文件。这至少要求源码没有语法错，其中调用的函数不一定有定义(definition)，但要有声明 (declaration)，也就是说头文件要齐全。当然，真的不全也没关系，自己放几个函数声明在前面就能糊弄编译器:) 至于动态分析，要求最高——程序需得运行起来。如果你要分析的是操作系统中某一部分，比如内存管理或网络协议栈，那么这里提到的两种动态分析法恐怕都不适 用了。

# 8. 网上的其它资源

[1] **使用Gnu gprof进行Linux平台下的程序分析 **[2] The DOT Language
http://www.graphviz.org/doc/info/lang.html 
[3] Graphviz - Graph Visualization Software
http://www.graphviz.org/ 
[4] Coverage Measurement and Profiling
http://www.linuxjournal.com/article/6758 
[5] **用Graphviz进行可视化操作──绘制函数调用关系图 **[6] cscope
http://cscope.sourceforge.net/ 
[7] KFT(Kernel Function Tracing)
http://elinux.org/Kernel_Function_Trace 
ftp://dslab.lzu.edu.cn/pub/kft 
[8] Call Graph -- Gprof
http://sourceware.org/binutils/docs-2.17/gprof/Call-Graph.html#Call-Graph 
[9] **各类分析函数调用关系图的工具 **http://oss.lzu.edu.cn/blog/blog.php?do_showone/tid_1636.html









