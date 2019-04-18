# linux下c/c++编译器和调试器以及静态库、动态库使用汇总 - gauss的专栏 - CSDN博客
2014年06月01日 23:58:46[gauss](https://me.csdn.net/mathlmx)阅读数：555
个人分类：[linux](https://blog.csdn.net/mathlmx/article/category/2303095)

$ ./hello
Hello, world!
**********************gcc/egcs 的主要选项*********
gcc 命令的常用选项
选项 解释
-ansi 只支持 ANSI 标准的 C 语法。这一选项将禁止 GNU C 的某些特色，
例如 asm 或 typeof 关键词。
-c 只编译并生成目标文件。
-DMACRO 以字符串“1”定义 MACRO 宏。
-DMACRO=DEFN 以字符串“DEFN”定义 MACRO 宏。
-E 只运行 C 预编译器。
-g 生成调试信息。GNU 调试器可利用该信息。
-IDIRECTORY 指定额外的头文件搜索路径DIRECTORY。
-LDIRECTORY 指定额外的函数库搜索路径DIRECTORY。
-lLIBRARY 连接时搜索指定的函数库LIBRARY。
-m486 针对 486 进行代码优化。
-o FILE 生成指定的输出文件。用在生成可执行文件时。
-O0 不进行优化处理。
-O 或 -O1 优化生成代码。
-O2 进一步优化。
-O3 比 -O2 更进一步优化，包括 inline 函数。
-shared 生成共享目标文件。通常用在建立共享库时。
-static 禁止使用共享连接。
-UMACRO 取消对 MACRO 宏的定义。
-w 不生成任何警告信息。
-Wall 生成所有警告信息。 
＃＃＃＃＃＃＃SCO UNIX下面dbaxtra的调试技术＃＃＃＃＃＃＃＃＃
在sco unix下编程大多离不开C语言，即使是数据库应用也有很多是与c搭配使用的，例如informix esql/c 就可以在c语言中嵌入sql 语句。很多人认为在unix下写程序是件很痛苦的事情，其中一个很重要原因是不知道在unix下怎样调试程序。其实在sco unix源码调试器是dbxtra或dbXtra，linux下是gdb。它们类似turbo c的调试器,可以跟踪源码变量。在unix 下调试程序有如下传统方法
---- 一、在要调试语句之前，输出要调试的变量，利用printf()函数。 
---- 二、写日志文件，把结果输出到文件中避免屏幕混乱，利用fprintf()函数。 
---- 三、利用sco 内置调试器dbxtra或dbXtra。 
---- dbxtra 适用字符界面，在sco unix的图形界面用dbXtra。(编按：请注意大小写) 
以下是dbxtra基本命令：
c cont 在断点后继续执行
d delete 删除所设断点
h help 帮助
e edit 编辑源程序
n next 源程序区的内容向下翻一屏。
p print 显示变量
q quit 退出dbxtra
r run 运行程序，直到遇上设置的断点
rr rerun 再次运行
s step 单步运行
st stop 设置断点
j status 显示当前断点
t where 显示当前状态，列出所有设置的变量值
di display 开显示窗，用于查看变量
ud undisplay 删除显示窗的条目
f forward 源程序区的内容向上 翻一屏。
B backward 源程序区的内容向下 翻一屏。
Stopi stop inst 设置断点
tracei trace inst跟踪子程序 
dbxtra [options] [objectfile ] 
---- dbxtra 在启动时有个参数-Idir值得一提.我们在编写一个较大程序的时候，通常源程序和编译生成的可执行文件都放在不同的目录中，这样便于管理。默认dbxtra将在可执行文件所在的目录下找匹配c的源程序。当我们启动时，指定-I参数，dbxtra就会到我们指定的目录下找匹配的c程序。 例如：
---- dbxtra -I"workc" program1 
---- 源程序在用cc编译时要带上-g 参数，这样是加上符号表等调试信息。只有这样编译过的文件，dbxtra才可以调试。调试信息使源代码和机器码关联。 
---- 下面这个C程序输出结果和我们的预想结果不一样，说明某些地方有错误。我们用调试器来调试它： 
---- 程序一: 
t.c
main()
{ int i=10 ,*p1;
float j=1.5,*p2;
p1=&
p2=&
p2=p1;
printf("%d,%dn",*p1,*p2);
} 
首先带上-g参数编译 cc -g -o t t.c
启动调试器 dbxtra t
屏幕显示：
1.main()
2.{ int i=10 ,*p1;
3. float j=1.5,*p2;
4. p1=&
5. p2=&
6. p2=p1;
7. printf("%d,%dn",*p1,*p2);
8.}
C[browse] File:t.c Func.-
Readubg symbolic information
Type 'help' for help
(dbxtra)
(dbxtra) 
设置断点：
(dbxtra）stop at 5
运行：
(dbxtra) run
程序自动在第5行停下。
这时我们可以看变量的值。
(dbxtra) print *p1 
单步执行。
(dbxtra) step
程序将执行第5行源码，指针将移到第6行。
(dbxtra) print *p2 
(dbxtra） step
程序执行了第6行源码后，将指针移到第7行。
(dbxtra） print *p1 , *p2 
---- 我们发现 在执行了第6行源码后，*p1,*p2的值就不对了，所以问题就出在第6行上。仔细检查后发现指针p1指向整型，指针p2指向实型。它们之间的赋值要进行强制类型转换。这种错误在C程序中是很常见的。
---- 有时我们在调试一些程序时，要在整个程序运行中时刻监视莫些变量的值，例如程序一中我们要时刻了解*p1,*p2的值，除了在每一行程序执行完后，打print *p1,*p2外，还可以开一个显示窗口。
---- (dbxtra）display *p1,*p2 
---- 用undisplay 删掉不想要的变量。 
---- 有些程序运行时要带参数，mycat /etc/passwd 在调试时候 
---- (dbxtra) run '/etc/passwd' 
---- 再运行时，无需再写一遍参数。 
---- (dbxtra） rerun 
---- 在涉及到curses库编程或屏幕有大量的人机界面时，为了调试方便，我们可以把程序输出结果重定向到个虚屏。 
---- (dbxtra） run >/dev/tty03 
---- 当然要先把tty03 disable 掉。（disable tty03） 
＃＃＃＃＃＃＃创建和使用静态库＃＃＃＃＃＃＃＃＃ 
详细的使用情况，请大家man手册，这里只介绍一下。静态库相对的比较简单。 
创建一个静态库是相当简单的。通常使用 ar 程序把一些目标文件（.o）组合在一起，
成为一个单独的库，然后运行 ranlib，以给库加入一些索引信息。 
＃＃＃＃＃＃＃＃创建和使用共享库＃＃＃＃＃＃＃＃＃
特殊的编译和连接选项
-D_REENTRANT 使得预处理器符号 _REENTRANT 被定义，这个符号激活一些宏特性。
-fPIC 选项产生位置独立的代码。由于库是在运行的时候被调入，因此这个
选项是必需的，因为在编译的时候，装入内存的地址还不知道。如果
不使用这个选项，库文件可能不会正确运行。
-shared 选项告诉编译器产生共享库代码。
-Wl,-soname -Wl 告诉编译器将后面的参数传递到连接器。而 -soname 指定了
共享库的 soname。
＃ 可以把库文件拷贝到 /etc/ld.so.conf 中列举出的任何目录中，并以
root 身份运行 ldconfig；或者
＃ 运行 export LD_LIBRARY_PATH='pwd'，它把当前路径加到库搜索路径中去。 
＃＃＃＃＃＃＃使用高级共享库特性＃＃＃＃＃＃＃＃＃ 
1. ldd 工具
ldd 用来显示执行文件需要哪些共享库, 共享库装载管理器在哪里找到了需要的共享库. 
2. soname 
共享库的一个非常重要的，也是非常难的概念是 soname——简写共享目标名（short for shared object name）。这是一个为共享库（.so）文件而内嵌在控制数据中的名字。如前面提到的，每一个程序都有一个需要使用的库的清单。这个清单的内容是一系列库的 soname，如同 ldd 显示的那样，共享库装载器必须找到这个清单。
soname 的关键功能是它提供了兼容性的标准。当要升级系统中的一个库时，并且新库的 soname 和老的库的 soname 一样，用旧库连接生成的程序，使用新的库依然能正常运行。这个特性使得在 Linux 下，升级使用共享库的程序和定位错误变得十分容易。
在 Linux 中，应用程序通过使用 soname，来指定所希望库的版本。库作者也可以通过保留或者改变 soname 来声明，哪些版本是相互兼容的，这使得程序员摆脱了共享库版本冲突问题的困扰。
查看/usr/local/lib 目录，分析 MiniGUI 的共享库文件之间的关系 
3. 共享库装载器 
当程序被调用的时候，Linux 共享库装载器（也被称为动态连接器）也自动被调用。它的作用是保证程序所需要的所有适当版本的库都被调入内存。共享库装载器名字是 ld.so 或者是 ld-linux.so，这取决于 Linux libc 的版本，它必须使用一点外部交互，才能完成自己的工作。然而它接受在环境变量和配置文件中的配置信息。
文件 /etc/ld.so.conf 定义了标准系统库的路径。共享库装载器把它作为搜索路径。为了改变这个设置，必须以 root 身份运行 ldconfig 工具。这将更新 /etc/ls.so.cache 文件，这个文件其实是装载器内部使用的文件之一。
可以使用许多环境变量控制共享库装载器的操作（表1-4+）。 
表 1-4+ 共享库装载器环境变量
变量 含义
LD_AOUT_LIBRARY_PATH 除了不使用 a.out 二进制格式外，与 LD_LIBRARY_PATH 相同。
LD_AOUT_PRELOAD 除了不使用 a.out 二进制格式外，与 LD_PRELOAD 相同。
LD_KEEPDIR 只适用于 a.out 库；忽略由它们指定的目录。
LD_LIBRARY_PATH 将其他目录加入库搜索路径。它的内容应该是由冒号
分隔的目录列表，与可执行文件的 PATH 变量具有相同的格式。
如果调用设置用户 ID 或者进程 ID 的程序，该变量被忽略。
LD_NOWARN 只适用于 a.out 库；当改变版本号是，发出警告信息。
LD_PRELOAD 首先装入用户定义的库，使得它们有机会覆盖或者重新定义标准库。
使用空格分开多个入口。对于设置用户 ID 或者进程 ID 的程序，
只有被标记过的库才被首先装入。在 /etc/ld.so.perload 中指定
了全局版本号，该文件不遵守这个限制。 
4. 使用 dlopen 
另外一个强大的库函数是 dlopen()。该函数将打开一个新库，并把它装入内存。该函数主要用来加载库中的符号，这些符号在编译的时候是不知道的。比如 Apache Web 服务器利用这个函数在运行过程中加载模块，这为它提供了额外的能力。一个配置文件控制了加载模块的过程。这种机制使得在系统中添加或者删除一个模块时，都不需要重新编译了。
可以在自己的程序中使用 dlopen()。dlopen() 在 dlfcn.h 中定义，并在 dl 库中实现。它需要两个参数：一个文件名和一个标志。文件名可以是我们学习过的库中的 soname。标志指明是否立刻计算库的依赖性。如果设置为 RTLD_NOW 的话，则立刻计算；如果设置的是 RTLD_LAZY，则在需要的时候才计算。另外，可以指定 RTLD_GLOBAL，它使得那些在以后才加载的库可以获得其中的符号。
当库被装入后，可以把 dlopen() 返回的句柄作为给 dlsym() 的第一个参数，以获得符号在库中的地址。使用这个地址，就可以获得库中特定函数的指针，并且调用装载库中的相应函数。
一、编写合格的动态链接库头文件 
C语言的头文件,可供一个或多个程序引用,里面一般定义程序所需的常量,自定义类型及函数原型说明等.其中的函数原型说明,则供编译器检查语法,用于排除引用参数时类型不一致的错误.只有编写合格的动态链接库头文件,程序员才能正确使用动态链接库内的函数.
动态链接库头文件要采用C语言标准格式,其中的动态函数原型定义,不必象上文介绍的那样用(*动态函数名)的描述形式.请看下面的例子每行开始的数字为所在行行号,为笔者添加,供注解使用)
1 /* adatetime.h : 纵横软件制作中心雨亦奇(zhsoft@371.net)编写, 2002-03-06. */
2
3 #ifndef __DATETIME_H
4
5 #define __DATETIME_H
6
7 /* 日期结构 */
8 typedef struct
9 {
10 int year;
11 int mon;
12 int day;
13 }DATETYPE;
14
15 /* 时间结构 */
16 typedef struct
17 {
18 char hour;
19 char min;
20 char sec;
21 }TIMETYPE;
22
23 int getdate(DATETYPE *d); /* 取当前日期 */
24 int gettime(TIMETYPE *t); /* 取当前时间 */
25
26 #endif
27 
注:与上文的datetime.h文件比较,从该头文件第23,24行可以看到,动态函数getdate,gettime的原型定义改变了,不再使用(*getdate),(*gettime)的格式了(这种格式使用较为罗嗦).
二、正确编译与命名动态链接库 
为了让GCC编译器生成动态链接库,编译时须加选项-shared.(这点须牢记) 
LINUX系统中,为了让动态链接库能被系统中其它程序共享,其名字应符合“lib*.so*”这种格式.如果某个动态链接库不符合此格式,则LINUX的动态链接库自动装入程序(ld.so)将搜索不到此链接库,其它程序也无法共享之.
格式中,第一个*通常表示为简写的库名,第二个*通常表示为该库的版本号.如:在我的系统中,基本C动态链接库的名字为libc.so.6,线程pthread动态链接库的名字为libpthread.so.0等等.本文例子所生成的动态链接库的名字为libmy.so,虽没有版本号,但也符合所要求的格式.
生成该动态链接库的维护文件makefile-lib内容如下: 
1 # makefile : 纵横软件制作中心雨亦奇编写, 2002-03-07.
2
3 all : libmy.so
4
5 SRC = getdate.c gettime.c
6
7 TGT = $(SRC:.c=.o)
8
9 $(SRC) : adatetime.h
10 @touch $@
11
12 %.o : %.c
13 cc -c $?
14
15 # 动态链接库(libmy.so)生成
16 libmy.so : $(TGT)
17 cc -s -shared -o $@ $(TGT)
18 
运行命令: 
$ make -f makefile-lib
$ 
即生成libmy.so库. 
注: 维护文件中,第17行用-shared选项以生成动态链接库,用-s选项以去掉目标文件中的符号表,从而减小文件长度. 
三、共享动态链接库 
3.1 动态链接库配置文件 
为了让动态链接库为系统所使用,需要维护动态链接库的配置文件--/etc/ld.so.conf.此文件内,存放着可被LINUX共享的动态链接库所在目录的名字(系统目录/lib,/usr/lib除外),各个目录名间以空白字符(空格,换行等)或冒号或逗号分隔.一般的LINUX发行版中,此文件均含一个共享目录/usr/X11R6/lib,为X window窗口系统的动态链接库所在的目录.
下面看看我的系统中此文件的内容如何: 
# cat /etc/ld.so.conf
/usr/X11R6/lib
/usr/zzz/lib
# 
由上可以看出,该动态库配置文件中,增加了一个/usr/zzz/lib目录.这是我自己新建的共享库目录,下面存放我新开发的可供系统共享的动态链接库. 
3.2 动态链接库管理命令 
为了让动态链接库为系统所共享,还需运行动态链接库的管理命令--ldconfig.此执行程序存放在/sbin目录下. 
ldconfig命令的用途,主要是在默认搜寻目录(/lib和/usr/lib)以及动态库配置文件/etc/ld.so.conf内所列的目录下,搜索出可共享的动态链接库(格式如前介绍,lib*.so*),进而创建出动态装入程序(ld.so)所需的连接和缓存文件.缓存文件默认为/etc/ld.so.cache,此文件保存已排好序的动态链接库名字列表.
ldconfig通常在系统启动时运行,而当用户安装了一个新的动态链接库时,就需要手工运行这个命令. 
ldconfig命令行用法如下: 
ldconfig [-v|--verbose] [-n] [-N] [-X] [-f CONF] [-C CACHE] [-r ROOT] [-l] [-p|--print-cache] [-c FORMAT] [--format=FORMAT] [-V] [-?|--help|--usage] path...
ldconfig可用的选项说明如下: 
(1) -v或--verbose : 用此选项时,ldconfig将显示正在扫描的目录及搜索到的动态链接库,还有它所创建的连接的名字. 
(2) -n : 用此选项时,ldconfig仅扫描命令行指定的目录,不扫描默认目录(/lib,/usr/lib),也不扫描配置文件/etc/ld.so.conf所列的目录.
(3) -N : 此选项指示ldconfig不重建缓存文件(/etc/ld.so.cache).若未用-X选项,ldconfig照常更新文件的连接. 
(4) -X : 此选项指示ldconfig不更新文件的连接.若未用-N选项,则缓存文件正常更新. 
(5) -f CONF : 此选项指定动态链接库的配置文件为CONF,系统默认为/etc/ld.so.conf. 
(6) -C CACHE : 此选项指定生成的缓存文件为CACHE,系统默认的是/etc/ld.so.cache,此文件存放已排好序的可共享的动态链接库的列表.
(7) -r ROOT : 此选项改变应用程序的根目录为ROOT(是调用chroot函数实现的).选择此项时,系统默认的配置文件/etc/ld.so.conf,实际对应的为ROOT/etc/ld.so.conf.如用-r /usr/zzz时,打开配置文件/etc/ld.so.conf时,实际打开的是/usr/zzz/etc/ld.so.conf文件.用此选项,可以大大增加动态链接库管理的灵活性.
( -l : 通常情况下,ldconfig搜索动态链接库时将自动建立动态链接库的连接.选择此项时,将进入专家模式,需要手工设置连接.一般用户不用此项. 
(9) -p或--print-cache : 此选项指示ldconfig打印出当前缓存文件所保存的所有共享库的名字. 
(10) -c FORMAT 或 --format=FORMAT : 此选项用于指定缓存文件所使用的格式,共有三种ld(老格式),new(新格式)和compat(兼容格式,此为默认格式).
(11) -V : 此选项打印出ldconfig的版本信息,而后退出. 
(12) -? 或 --help 或 --usage : 这三个选项作用相同,都是让ldconfig打印出其帮助信息,而后退出. 
举三个例子: 
例1: 
# ldconfig -p
793 libs found in cache `/etc/ld.so.cache'
libzvt.so.2 (libc6) => /usr/lib/libzvt.so.2
libzvt.so (libc6) => /usr/lib/libzvt.so
libz.so.1.1.3 (libc6) => /usr/lib/libz.so.1.1.3
libz.so.1 (libc6) => /lib/libz.so.1
......
# 
注: 有时候用户想知道系统中有哪些动态链接库,或者想知道系统中有没有某个动态链接库,这时,可用-p选项让ldconfig输出缓存文件中的动态链接库列表,从而查询得到.例子中,ldconfig命令的输出结果第1行表明在缓存文件/etc/ld.so.cache中找到793个共享库,第2行开始便是一系列共享库的名字及其全名(绝对路径).因为实际输出结果太多,为节省篇幅,以......表示省略的部分.
例2: 
# ldconfig -v
/lib:
liby.so.1 -> liby.so.1
libnss_wins.so -> libnss_wins.so
......
/usr/lib:
libjscript.so.2 -> libjscript.so.2.0.0
libkspell.so.2 -> libkspell.so.2.0.0
......
/usr/X11R6/lib:
libmej-0.8.10.so -> libmej-0.8.10.so
libXaw3d.so.7 -> libXaw3d.so.7.0
......
# 
注: ldconfig命令在运行正常的情况下,默认不输出什么东西.本例中用了-v选项,以使ldconfig在运行时输出正在扫描的目录及搜索到的共享库,用户可以清楚地看到运行的结果.执行结束后,ldconfig将刷新缓存文件/etc/ld.so.cache.
例3: 
# ldconfig /usr/zhsoft/lib
# 
注: 当用户在某个目录下面创建或拷贝了一个动态链接库,若想使其被系统共享,可以执行一下"ldconfig 目录名"这个命令.此命令的功能在于让ldconfig将指定目录下的动态链接库被系统共享起来,意即:在缓存文件/etc/ld.so.cache中追加进指定目录下的共享库.本例让系统共享了/usr/zhsoft/lib目录下的动态链接库.需要说明的是,如果此目录不在/lib,/usr/lib及/etc/ld.so.conf文件所列的目录里面,则再度运行ldconfig时,此目录下的动态链接库可能不被系统共享了.
3.3 动态链接库如何共享 
了解了以上知识,我们可以采用以下三种方法来共享动态链接库注:均须在超级用户状态下操作,以我的动态链接库libmy.so共享过程为例) 
(1)拷贝动态链接库到系统共享目录下,或在系统共享目录下为该动态链接库建立个连接(硬连接或符号连接均可,常用符号连接).这里说的系统共享目录,指的是LINUX动态链接库存放的目录,它包含/lib,/usr/lib以及/etc/ld.so.conf文件内所列的一系列目录.
# cp libmy.so /lib
# ldconfig
# 
或: 
# ln -s `pwd`/libmy.so /lib
# ldconfig
# 
(2)将动态链接库所在目录名追加到动态链接库配置文件/etc/ld.so.conf中. 
# pwd >> /etc/ld.so.conf
# ldconfig
# 
(3)利用动态链接库管理命令ldconfig,强制其搜索指定目录,并更新缓存文件,便于动态装入. 
# ldconfig `pwd`
# 
需要说明的是,这种操作方法虽然有效,但效果是暂时的,供程序测试还可以,一旦再度运行ldconfig,则缓存文件内容可能改变,所需的动态链接库可能不被系统共享了.与之相比较,前两种方法是可靠的方法,值得业已定型的动态链接库共享时采用.前两种方法还有一个特点,即最后一条命令都是ldconfig,也即均需要更新一下缓存文件,以确保动态链接库的共享生效.
四、含有动态函数的程序的编译 
4.1 防止编译因未指定动态链接库而出错 
当一个程序使用动态函数时,编译该程序时就必须指定含所用动态函数的动态链接库,否则编译将会出错退出.如本文示例程序ady.c的编译(未明确引用动态链接库libmy.so):
# cc -o ady ady.c
/tmp/ccL4FsJp.o: In function `main':
/tmp/ccL4FsJp.o(.text+0x43): undefined reference to `gettime'
collect2: ld returned 1 exit status
# 
注: 因为ady.c所含的动态函数getdate,gettime不在系统函数库中,所以连接时出错. 
4.2 编译时引用动态链接库的几种方式 
(1)当所用的动态链接库在系统目录(/lib,/usr/lib)下时,可用编译选项-l来引用.即: 
# cc -lmy -o ady ady.c
# 
注:编译时用-l选项引用动态链接库时,库名须使用其缩写形式.本例的my,表示引用libmy.so库.若引用光标库libncurses.so,须用-lncurses.注意,-l选项与参数之间不能有空格,否则会出错.
(2)当所用的动态链接库在系统目录(/lib,/usr/lib)以外的目录时,须用编译选项-L来指定动态链接库所在的目录(供编译器查找用),同时用-l选项指定缩写的动态链接库名.即:
# cc -L/usr/zzz/lib -lmy -o ady ady.c
# 
(3)直接引用所需的动态链接库.即: 
# cc -o ady ady.c libmy.so
# 
或 
# cc -o ady ady.c /lib/libmy.so
# 
等等.其中,动态链接库的库名可以采用相对路径形式(文件名不以/开头),也可采用绝对路径形式(文件名以/开头). 
五、动态链接程序的运行与检查 
5.1 运行 
编译连接好含动态函数的程序后,就可以运行它了.动态链接程序因为共享了系统中的动态链接库,所以其空间占用很小.但这并不意味功能的减少,它的执行与静态连接的程序执行,效果完全相同.在命令提示符下键入程序名及相关参数后回车即可,如下例:
$ ady
动态链接库高级应用示范
当前日期: 2002-03-11
当前时间: 19:39:06
$ 
5.2 检查 
检查什么?检查动态链接程序究竟需要哪些共享库,系统中是否已有这些库,没有的话,用户好想办法把这些库装上. 
怎么检查呢?这里,告诉你一个实用程序--ldd,这个程序就是专门用来检查动态链接程序依赖哪些共享库的. 
ldd命令行用法如下: 
ldd [--version] [-v|--verbose] [-d|--data-relocs] [-r|--function-relocs] [--help] FILE...
各选项说明如下: 
(1) --version : 此选项用于打印出ldd的版本号. 
(2) -v 或 --verbose : 此选项指示ldd输出关于所依赖的动态链接库的尽可能详细的信息. 
(3) -d 或 --data-relocs : 此选项执行重定位,并且显示不存在的函数. 
(4) -r 或 --function-relocs : 此选项执行数据对象与函数的重定位,同时报告不存在的对象. 
(5) --help : 此选项用于打印出ldd的帮助信息. 
注: 上述选项中,常用-v(或--verbose)选项. 
ldd的命令行参数为FILE...,即一个或多个文件名(动态链接程序或动态链接库). 
例1: 
$ ldd ady
libmy.so => ./libmy.so (0x40026000)
libc.so.6 => /lib/libc.so.6 (0x40028000)
/lib/ld-linux.so.2 => /lib/ld-linux.so.2 (0x40000000)
$ 
注: 每行=>前面的,为动态链接程序所需的动态链接库的名字,而=>后面的,则是运行时系统实际调用的动态链接库的名字,所需的动态链接库在系统中不存在时,=>后面将显示"not found",括号所括的数字为虚拟的执行地址.本例列出ady所需的三个动态链接库,其中libmy.so为自己新建的动态链接库,而libc.so.6与/lib/ld-linux.so.2均为系统的动态链接库,前一个为基本C库,后一个动态装入库(用于动态链接库的装入及运行).
例2: 
$ ldd -v ady
libmy.so => ./libmy.so (0x40026000)
libc.so.6 => /lib/libc.so.6 (0x40028000)
/lib/ld-linux.so.2 => /lib/ld-linux.so.2 (0x40000000) 
Version information:
./ady:
libc.so.6 (GLIBC_2.1.3) => /lib/libc.so.6
libc.so.6 (GLIBC_2.0) => /lib/libc.so.6
./libmy.so:
libc.so.6 (GLIBC_2.1.3) => /lib/libc.so.6
libc.so.6 (GLIBC_2.0) => /lib/libc.so.6
/lib/libc.so.6:
ld-linux.so.2 (GLIBC_2.1.1) => /lib/ld-linux.so.2
ld-linux.so.2 (GLIBC_2.2.3) => /lib/ld-linux.so.2
ld-linux.so.2 (GLIBC_2.1) => /lib/ld-linux.so.2
ld-linux.so.2 (GLIBC_2.2) => /lib/ld-linux.so.2
ld-linux.so.2 (GLIBC_2.0) => /lib/ld-linux.so.2
$ 
注:本例用-v选项以显示尽可能多的信息,所以例中除列出ady所需要的动态链接库外,还列出了程序所需动态链接库版本方面的信息. 
小结: 在LINUX动态链接库的高级应用中,关键有两点,一是如何让动态链接库为LINUX系统所共享,二是编译连接程序时如何做.让动态链接库为系统所共享,主要是用ldconfig管理命令,维护好系统共享库的缓存文件/etc/ld.so.cache.编译连接时如何做?注意连接上所用的动态链接库就可以了.LINUX动态链接库的高级应用,用一用就明白:其实,就是这么简单!
[1 楼] | Posted: 2005-08-24 10:35 
ppking
级别: 论坛版主
精华: 1
发帖: 28
威望: 65 点
金钱: 136 RMB
贡献值: 0 点
注册时间:2005-08-16
最后登录:2005-08-26 
********几种不同UNIX系统常用的动态连接库建立的参数说明***** 
创建共享库和链接可执行文件类似：首先把源代码编译成目标文件， 然后把目标文件链接起来．目标文件需要创建成 位置无关码（position-independent code） （PIC），概念上就是在可执行程序装载它们的时候， 它们可以放在可执行程序的内存里的任何地方， （用于可执行文件的目标文件通常不是用这个方式编译的．） 链接动态库的命令包含特殊标志，与链接可执行文件的命令是有区别的． --- 至少理论上如此．在一些系统里的现实更恶心．
在下面的例子里，我们假设你的源程序代码在 foo.c 文件里并且将创建成名字叫 foo.so的共享库．中介的对象文件将叫做 foo.o，除非我们另外注明．一个共享库可以 包含多个对象文件，不过我们在这里只用一个．
BSD/OS
创建 PIC 的编译器标志是 -fpic．创建共享库的链接器标志是 -shared． 
gcc -fpic -c foo.c
ld -shared -o foo.so foo.o
上面方法适用于版本 4.0 的 BSD/OS． 
FreeBSD
创建 PIC 的编译器标志是 -fpic．创建共享库的链接器标志是 -shared． 
gcc -fpic -c foo.c
gcc -shared -o foo.so foo.o
上面方法适用于版本 3.0 的 FreeBSD. 
HP-UX
创建 PIC 的系统编译器标志是 +z．如果使用 GCC 则是 -fpic． 创建共享库的链接器标志是 -b．因此 
cc +z -c foo.c
或 
gcc -fpic -c foo.c
然后 
ld -b -o foo.sl foo.o
HP-UX 使用 .sl 做共享库扩展，和其它大部分系统不同． 
IRIX
PIC 是缺省，不需要使用特殊的编译器选项． 生成共享库的链接器选项是 -shared. 
cc -c foo.c
ld -shared -o foo.so foo.o 
Linux
创建 PIC 的编译器标志是 -fpic．在一些平台上的一些环境下， 如果 -fpic 不能用那么必须使用-fPIC． 参考 GCC 的手册获取更多信息． 创建共享库的编译器标志是 -shared．一个完整的例子看起来象：
cc -fpic -c foo.c
cc -shared -o foo.so foo.o 
NetBSD
创建 PIC 的编译器标志是 -fpic．对于 ELF 系统， 带 -shared 标志的编译命令用于链接共享库． 在老的非 ELF 系统里，使用ld -Bshareable．
gcc -fpic -c foo.c
gcc -shared -o foo.so foo.o 
OpenBSD
创建 PIC 的编译器标志是 -fpic. ld -Bshareable 用于链接共享库． 
gcc -fpic -c foo.c
ld -Bshareable -o foo.so foo.o 
Solaris
创建 PIC 的编译器命令是用 Sun 编译器时为 -KPIC 而用 GCC 时为 -fpic．链接共享库时两个编译器都可以用 -G 或者用 GCC 时还可以是 -shared．
cc -KPIC -c foo.c
cc -G -o foo.so foo.o
或 
gcc -fpic -c foo.c
gcc -G -o foo.so foo.o 
Tru64 UNIX
PIC 是缺省，因此编译命令就是平常的那个． 带特殊选项的 ld 用于链接： 
cc -c foo.c
ld -shared -expect_unresolved '*' -o foo.so foo.o
用 GCC 代替系统编译器时的过程是一样的；不需要特殊的选项． 
UnixWare
SCO 编译器创建 PIC 的标志是-KPIC GCC 是 -fpic． 链接共享库时 SCO 编译器用 -G 而 GCC 用-shared． 
cc -K PIC -c foo.c
cc -G -o foo.so foo.o
or 
gcc -fpic -c foo.c
gcc -shared -o foo.so foo.o 
技巧: 如果你想把你的扩展模块打包，用在更广的发布中，那么你应该考虑使用 GNU Libtool 制作共享库．它把平台之间的区别封装成 了一个通用的并且非常强大的接口．严肃的包还要求考虑有关库版本， 符号解析方法和一些其他的问题．
生成的共享库文件然后就可以装载到 PostgreSQL里面去了．在给 CREATE FUNCTION 命令声明文件名的时候，我们必须声明 共享库文件的名字而不是中间目标文件的名字．请注意你可以在 CREATE FUNCTION 命令上忽略 系统标准的共享库扩展 (通常是.so或.sl)， 并且出于最佳的兼容性考虑也应该忽略．
[2 楼] | Posted: 2005-08-24 10:36 
ppking
级别: 论坛版主
精华: 1
发帖: 28
威望: 65 点
金钱: 136 RMB
贡献值: 0 点
注册时间:2005-08-16
最后登录:2005-08-26 
源程序的显示和搜索 
程序出错一般来说不只是出错的那条语句本身造成的。事实上出现错误经常是前面或相关的代码执行了不正确的操作或少了某
些必要的处理。因此调试过程中经常要观察一下源程序中的语句，或者在程序中搜索某个符号出现在什么地方。其中字符串的
搜索功能同vi基本上是相同的，而文件的显示则同另外一个我们没有具体讨论的编辑器ed类似。下面我们将具体介绍这些命令。 
1.源程序的显示 
在用core进入sdb之后，在*提示符后输入w命令，该命令指示sdb显示源程序中的当前行为中心的前后10行的内容并保持当前行
不变： 
* w 
7：int 
8: TestInput(char * ValueInput) 
9: {while ( * ValueInput) 
10: if (! isdigit( * ValueInput)) return (! TESTOK); 
11: else ValueInput++; 
12: return ((100/atoi(ValueInput))? TESTOK:! TESTOK); 
13: } 
* 
我们看到，在进入sdb时，当前行是第12行，以该行为中心的10行内容正好就是上面所显示出来的。其他可以显示源程序语
句的sdb命令如下： 
P 显示当前行
l 显示对应于当前指令的那条语句
Z 显示当前行开始的下面10条语句
Ctrl+D 显示当前行之后（不包括当前行）的第10条语句
n 显示第n条语句，这里n是一个数
注意这些命令显示出的是源程序语句还是汇编语句（后面我们将要介绍)取决于最近一次显示出的是什么。 
2.改变当前行 
在用户显示语句时，当前行也会相应地发生变化。例如，Z命令将使当前行向程序尾移动9行，而Ctrl+D则使当前行向后移
动10行。 
在使用数字来显示某行语句时将使该行语句成为当前行。而在*提示符之后按一下回车，当前行将下移一行。例如，接着上面
的例子，输入： 
* 8p 
8: TEstInput(char * ValueInput) 
* 回车 
9： { while ( * ValueInput)} 
* 
这里8p实际上是两条命令的组合。它使当前行移至源文件的第八行，然后再显示出新的当前行。按回车键将使当前行后移一行。 
3.改变当前源文件 
在vi中我们可以用e命令对另外某个文件进行编辑。sdb也提供了e命令，可以用此命令来改变当前文件，如： 
* e myprog.c 
current file is now myprog.c 
* 8p 
8: main(int argc,char * argv[]) 
* 
我们看到，当前文件改变之后，sdb将第一行设为是当前行。如果此文件的第一行是个函数，那么该函数便成为当前函数。
否则将临时出现没有当前函数的情况。 
在上一节中，我们介绍过在命令行中可以指定源文件搜索目录名列表（缺省情况为当前目录）。如果某个文件不在此搜索
目录中，则可以用e命令将其加入： 
* e Another SourceDir 
这里Another SourceDir是一个目录名。如果要显示该目录下的某个文件，只需要输入： 
* e FileName.c 
当然直接使用： 
* e Another SourceDir/FileName.c 
也能达到同样的效果。 
使用： 
* e FunctionName 
将使包含函数FunctionName的文件名成为当前文件，而当前函数不言而喻将成为FunctionName。当前行则理所当然的是该
函数的第一行。同一程序中函数名在各模块中的唯一性保证了这一点是能够成功的，但如果包含指定函数的文件不在当前
搜索目录列表中，则必须用e命令将其加入。 
4.字符串的搜索 
在vi中，我们可以在命令方式下使用“/“或者“？”命令，从当前位置向后或者向前搜索某个字符串，在sdb中也同样可
以完成这一点。使用这两个命令我们可以查找源程序中某个或某类符号的出现。之所以说某类，是因为我们可以用正规表
达式来指定待搜索的串（也即在搜索串中可以使用*,?,[,],-,^这类特殊字符）。 
例如，为了查找myprog.c中argv出现在那些行上，可输入： 
* /argv/ 
8: main(ini argc,char * argv[]) 
sdb将从当前行开始向文件尾搜索，到达文件尾之后又从文件头开始直至搜索到某个匹配的串或到达当前行为止。 
与/相反，？命令将从当前行向文件头方向搜索，因此如果我们将上述/argv/换成： 
* ? argv? 
14: printf("The %dth value' %s'tis BAD! n",i,argv); 
* 
所得的结果一般是不同的。 
/或?命令之后的/或?并不是必须的。另外如果要在同一方向上继续搜索上次搜索过的串，只需要直接输入/或者?即可。
[3 楼] | Posted: 2005-08-24 10:38 
ppking
级别: 论坛版主
精华: 1
发帖: 28
威望: 65 点
金钱: 136 RMB
贡献值: 0 点
注册时间:2005-08-16
最后登录:2005-08-26 
***************ld是怎么连接的********************** 
由於静态与共享程式库两者间不相容的格式的差异性与动词*link*过量使用於指称*编译完成後的事情*与*当编译好的程式使用时所发生的事情*这两件事上头，使得这一章节变得复杂了许多。（ and, actually, the overloading of the word `load' in a comparable but opposite sense）不过，再复杂也就是这样了，所以阁下不必过於担心。
为了稍微减轻读者的困惑，我们称执行期间所发生的事为*动态载入*，这一主题会在下一章节中谈到。你也会在别的地方看到我把动态载入描述成*动态连结*，不过不会是在这一章节中。换句话说，这一章节所谈的，全部是指发生在编译结束後的连结。
6.1 共享程式库 vs静态程式库 
建立程式的最後一个步骤便是连结；也就是将所有分散的小程式组合起来，看看是否遗漏了些什麽。显然，有一些事情是很多程式都会想做的---例如，开启档案，接著所有与开档有关的小程式就会将储存程式库的相关档案提供给你的程式使用。在一般的Linux系统上，这些小程式可以在/lib与/usr/lib/目录底下找到。
当你用的是静态的程式库时，连结器会找出程式所需的模组，然後实际将它们拷贝到执行档内。然而，对共享程式库而言，就不是这样了。共享程式库会在执行档内留下一个记号，指明*当程式执行时，首先必须载入这个程式库*。显然，共享程式库是试图使执行档变得更小，等同於使用更少的记忆体与磁碟空间。Linux内定的行为是连结共享程式库，只要Linux能找到这些共享程式库的话，就没什麽问题；不然,Linux就会连结静态的了。如果你想要共享程式库的话，检查这些程式库（*.sa for a.out, *.so for ELF）是否住在它们该在的地方，而且是可读取的。
在Linux上，静态程式库会有类似libname.a这样的名称；而共享程式库则称为libname.so.x.y.z，此处的x.y.z是指版本序号的样式。共享程式库通常都会有连结符号指向静态程式库（很重要的）与相关联的.sa档案。标准的程式库会包含共享与静态程式库两种格式。
你可以用ldd（List Dynamic Dependencies）来查出某支程式需要哪些共享程式库。 $ ldd /usr/bin/lynx libncurses.so.1 => /usr/lib/libncurses.so.1.9.6 libc.so.5 => /lib/libc.so.5.2.18
这是说在我的系统上，WWW浏览器*lynx*会依赖libc.so.5 (the C library)与libncurses.so.1（终端机萤幕的控制）的存在。若某支程式缺乏独立性， ldd就会说‘statically linked’或是‘statically linked (ELF)’。
6.2 终极审判（‘sin() 在哪个程式库里？’） 
nm 程式库名称应该会列出此程式库名称所参考到的所有符号。这个指令可以应用在静态与共享程式库上。假设你想知道tcgetattr()是在哪儿定义的：你可以如此做，
$ nm libncurses.so.1 |grep tcget U tcgetattr 
*U*指出*未定义*---也就是说ncurses程式库有用到tegetattr()，但是并没有定义它。你也可以这样做， 
$ nm libc.so.5 | grep tcget 00010fe8 T __tcgetattr 00010fe8 W tcgetattr 00068718 T tcgetpgrp
*W*说明了*弱态(weak)*，意指符号虽已定义，但可由不同程式库中的另一定义所替代。最简单的*正常*定义（像是tcgetpgrp）是由*T*所标示： 
标题所谈的问题，最简明的答案便是libm.(so|a)了。所有定义在<math.h>的函数都保留在maths程式库内；因此，当你用到其中任何一个函数时，都需要以-lm的参数连结此程式库。
6.3 X档案？ 
ld: Output file requires shared library `libfoo.so.1` 
ld与其相类似的命令在搜寻档案的策略上，会依据版本的差异而有所不同，但是唯一一个你可以合理假设的内定目录便是/usr/lib了。如果你希望身处它处的程式库也列入搜寻的行列中，那麽你就必须以-L选项告知gcc或是ld。
要是你发现一点效果也没有，就赶紧察看看那档案是不是还乖乖的躺在原地。就a.out而言，以-lfoo参数来连结，会驱使ld去寻找libfoo.sa（shared stubs）；如果没有成功，就会换成寻找libfoo.a（static）。就ELF而言， ld会先找libfoo.so，然後是libfoo.a。libfoo.so通常是一个连结符号，连结至libfoo.so.x。
6.4 建立你自己的程式库 控制版本 
与其它任何的程式一样，程式库也有修正不完的bugs的问题存在。它们也可能产生出一些新的特点，更改目前存在的模组的功效，或是将旧的移除掉。这对正在使用它们的程式而言，可能会是一个大问题。如果有一支程式是根据那些旧的特点来执行的话，那怎麽办？
所以，我们引进了程式库版本编号的观念。我们将程式库*次要*与*主要*的变更分门别类，同时规定*次要*的变更是不允许用到这程式库的旧程式发生中断的现象。你可以从程式库的档名分辨出它的版本（实际上，严格来讲，对ELF而言仅仅是一场天大的谎言；继续读将下去，便可明白为什麽了）： libfoo.so.1.2的主要版本是1，次要版本是2。次要版本的编号可能真有其事，也可能什麽都没有---libc在这一点上用了*修正程度*的观念，而订出了像libc.so.5.2.18这样的程式库名称。次要版本的编号内若是放一些字母、底线、或是任何可以列印的ASCII字元，也是很合理的。
ELF与a.out格式最主要的差别之一就是在设置共享程式库这件事上；我们先看ELF，因为它比较简单一些。 
ELF？它到底是什麽东东ㄋㄟ？ 
ELF（Executable and Linking Format）最初是由USL（UNIX System Laboratories）发展而成的二进位格式，目前正应用於Solaris与System V Release 4上。由於ELF所增涨的弹性远远超过Linux过去所用的a.out格式，因此GCC与C程式库的发展人士於1995年决定改用ELF为Linux标准的二进位格式。
怎麽又来了？ 
这一节是来自於‘/news-archives/comp.sys.sun.misc’的文件。 
ELF（“Executable Linking Format”）是於SVR4所引进的新式改良目的档格式。ELF比起COFF可是多出了不少的功能。以ELF而言，它*是*可由使用者自行延伸的。ELF视一目的档为节区（sections），如串列般的组合；而且此串列可为任意的长度（而不是一固定大小的阵列）。这些节区与COFF的不一样，并不需要固定在某个地方，也不需要以某种顺序排列。如果使用者希望补捉到新的资料，便可以加入新的节区到目的档内。ELF也有一个更强而有力的除错法式，称为DWARF（Debugging
 With Attribute Record Format）□目前Linux并不完全支援。DWARF DIEs（Debugging Information Entries）的连结串列会在ELF内形成 .debug的节区。DWARF DIEs的每一个 .debug节区并非一些少量且固定大小的资讯记录的集合，而是一任意长度的串列，拥有复杂的属性，而且程式的资料会以有□围限制的树状资料结构写出来。DIEs所能补捉到的大量资讯是COFF的 .debug节区无法望其项背的。（像是C++的继承图。）
ELF档案是从SVR4（Solaris 2.0 ？）ELF存取程式库（ELF access library）内存取的。此程式库可提供一简便快速的介面予ELF。使用ELF存取程式库最主要的恩惠之一便是，你不再需要去察看一个ELF档的qua了。就UNIX的档案而言，它是以Elf*的型式来存取；呼叫elf_open()之後，从此时开始，你只需呼叫elf_foobar()来处理档案的某一部份即可，并不需要把档案实际在磁碟上的image搞得一团乱。
ELF的优缺点与升级至ELF等级所需经历的种种痛苦，已在ELF-HOWTO内论及；我并不打算在这儿涂浆糊。ELF HOWTO应该与这份文件有同样的主题才是。
ELF共享程式库 
若想让libfoo.so成为共享程式库，基本的步骤会像下面这样： 
$ gcc -fPIC -c *.c $ gcc -shared -Wl,-soname,libfoo.so.1 -o libfoo.so.1.0 *.o $ ln -s libfoo.so.1.0 libfoo.so.1 $ ln -s libfoo.so.1 libfoo.so $ LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH ; export LD_LIBRARY_PATH
这会产生一个名为libfoo.so.1.0的共享程式库，以及给予ld适当的连结（libfoo.so）还有使得动态载入程式（dynamic loader）能找到它（libfoo.so.1）。为了进行测试，我们将目前的目录加到LD_LIBRARY_PATH里。
当你津津乐道於程式库制做成功之时，别忘了把它移到如/usr/local/lib的目录底下，并且重新设定正确的连结路径。libfoo.so.1与libfoo.so.1.0的连结会由ldconfig依日期不断的更新，就大部份的系统来说，ldconfig会在开机过程中执行。libfoo.so的连结必须由手动方式更新。如果你对程式库所有组成份子（如标头档等）的升级，总是抱持著一丝不□的态度，那麽最简单的方法就是让libfoo.so -> libfoo.so.1；如此一来，ldconfig便会替你同时保留最新的连结。要是你没有这麽做，你自行设定的东东就会在数日後造成千奇百怪的问题出现。到时候，可别说我没提醒你啊！
$ su # cp libfoo.so.1.0 /usr/local/lib # /sbin/ldconfig # ( cd /usr/local/lib ; ln -s libfoo.so.1 libfoo.so
版本编号、soname与符号连结 
每一个程式库都有一个soname。当连结器发现它正在搜寻的程式库中有这样的一个名称，连结器便会将soname箝入连结中的二进位档内，而不是它正在运作的实际的档名。在程式执行期间，动态载入程式会搜寻拥有soname这样的档名的档案，而不是程式库的档名。因此，一个名为libfoo.so的程式库，就可以有一个libbar.so的soname了。而且所有连结到libbar.so的程式，当程式开始执行时，会寻找的便是libbar.so了。
这听起来好像一点意义也没有，但是这一点，对於了解数个不同版本的同一个程式库是如何在单一系统上共存的原因，却是关键之钥。Linux程式库标准的命名方式，比如说是libfoo.so.1.2，而且给这个程式库一个libfoo.so.1的soname。如果此程式库是加到标准程式库的目录底下（e.g. /usr/lib），ldconfig会建立符号连结libfoo.so.1 -> libfoo.so.1.2，使其正确的image能於执行期间找到。你也需要连结libfoo.so -> libfoo.so.1，使ld能於连结期间找到正确的soname。
所以罗，当你修正程式库内的bugs，或是添加了新的函数进去（任何不会对现存的程式造成不利的影响的改变），你会重建此程式库，保留原本已有的soname，然後更改程式库档名。当你对程式库的变更会使得现有的程式中断，那麽你只需增加soname中的编号---此例中，称新版本为libfoo.so.2.0，而soname变成libfoo.so.2。紧接著，再将libfoo.so的连结转向新的版本；至此，世界又再度恢复了和平！
其实你不须要以此种方式来替程式库命名，不过这的确是个好的传统。ELF赋予你在程式库命名上的弹性，会使得人气喘呼呼的搞不清楚状况；有这样的弹性在，也并不表示你就得去用它。
ELF总结：假设经由你睿智的观察发现有个惯例说：程式库主要的升级会破坏相容性；而次要的升级则可能不会；那麽以下面的方式来连结，所有的一切就都会相安无事了。 
gcc -shared -Wl,-soname,libfoo.so.major -o libfoo.so.major.minor 
a.out---旧旧的格式□ 
建立共享程式库的便利性是升级至ELF的主要原因之一。那也是说,a.out可能还是有用处在的。上ftp站去抓 ftp://tsx-11.mit.edu/pub/linux/packages/GCC/src/tools-2.17.tar.gz；解压缩後你会发现有20页的文件可以慢慢的读哩。我很不喜欢自己党派的偏见表现得那麽的淋璃尽致，可是从上下文间，应该也可以很清楚的嗅出我从来不拿石头砸自己的脚的脾气吧！
ZMAGIC vs QMAGIC 
QMAGIC是一种类似旧格式的a.out（亦称为ZMAGIC）的可执行档 格式，这种格式会使得第一个分页无法map。当0-4096的□围内没有mapping存在时，则可允许NULL dereference trapping更加的容易。所产生的边界效应是你的执行档会比较小（大约少1K左右）。
只有即将作废的连结器有支援ZMAGIC，一半已埋入棺材的连结器有支援这两种格式；而目前的版本仅支援QMAGIC而已。事实上，这并没有多大的影响，那是因为目前的核心两种格式都能执行。
*file*命令应该可以确认程式是不是QMAGIC的格式的。 
档案配置 
一a.out(DLL)的共享程式库包含两个真实的档案与一个连结符号。就*foo*这个用於整份文件做为□例的程式库而言，这些档案会是libfoo.sa与libfoo.so.1.2；连结符号会是libfoo.so.1，而且会指向libfoo.so.1.2。这些是做什麽用的？
在编译时，ld会寻找libfoo.sa。这是程式库的*stub*档案。而且含有所有执行期间连结所需的exported的资料与指向函数的指标。 
执行期间，动态载入程式会寻找libfoo.so.1。这仅仅是一个符号连结，而不是真实的档案。故程式库可更新成较新的且已修正错误的版本，而不会损毁任何此时正在使用此程式库的应用程式。在新版---比如说libfoo.so.1.3---已完整呈现时，ldconfig会以一极微小的操作，将连结指向新的版本，使得任何原本使用旧版的程式不会感到丝毫的不悦。
DLL程式库（我知道这是无谓的反覆---所以对我提出诉讼吧！）通常会比它们的静态副本要来得大多。它们是以*洞（holes）*的形式来保留空间以便日後的扩充。这种*洞*可以不占用任何的磁碟空间。一个简单的cp呼叫，或是使用makehole程式，就可以达到这样效果。因为它们的位址是固定在同一位置上，所以在建立程式库後，你可以把它们拿掉。不过，千万不要试著拿掉ELF的程式库。
**********************动态载入过程*************** 
Linux有共享程式库，如果之前你已坐著读完上一章节，想必现在一听到像这样的说词，便会立刻感到头昏。有一些照惯例而言是在连结时期便该完成的工作，必须延迟到载入时期才能完成。 7.2 错误讯息
把你连结的错误寄给我！我不会做任何的事，不过我可以把它们写起来** 
can't load library: /lib/libxxx.so, Incompatible version 
（a. out only） 这是指你没有xxx程式库的正确的主要版本。可别以为随随 便便弄个连结到你目前拥有的版本就可以了，如果幸运的话，就只会造成你的程式分页错误而已。去抓新的版本.ELF类似的情况会造成像下面这样的讯息：
ftp: can't load library 'libreadline.so.2' 
warning using incompatible library version xxx 
（a. out only）你的程式库的次要版本比起这支程式用来编译的还要旧。程式依然可以执行。只是可能啦！我想，升个级应该没什麽伤害吧！ 
7.3 控制动态载入器的运作 
有一组环境变数会让动态载入器有所反应。大部份的环境变数对ldd的用途要比起对一般users的还要来得更多。而且可以很方便的设定成由ldd配合各种参数来执行。这些变数包括，
LD_BIND_NOW --- 正常来讲，函数在呼叫之前是不会让程式寻找的。设定这个旗号会使得程式库一载入，所有的寻找便会发生，同时也造成起始的时间较慢。当你想测试程式，确定所有的连结都没有问题时，这项旗号就变得很有用。 LD_PRELOAD可以设定一个档案，使其具有*覆盖*函数定义的能力。例如，如果你要测试记忆体分配的方略，而且还想置换*malloc*，那麽你可以写好准备替换的副程式，并把它编译成mallolc.，然後： $ LD_PRELOAD=malloc.o; export LD_PRELOAD
 $ some_test_program LD_ELF_PRELOAD 与 LD_AOUT_PRELOAD 很类似，但是仅适用於正确的二进位型态。如果设定了 LD_something_PRELOAD 与 LD_PRELOAD ，比较明确的那一个会被用到。 LD_LIBRARY_PATH是一连串以分号隔离的目录名称，用来搜寻共享程式库。对ld而言，并没有任何的影响；这项只有在执行期间才有影响。另外，对执行setuid与setgid的程式而言，这一项是无效的。而LD_ELF_LIBRARY_PATH与LD_AOUT_LIBRARY_PATH这两种旗号可根据各别的二进位型式分别导向不同的搜寻路径。一般正常的运作下，不应该会用到LD_LIBRARY_PATH；把需要搜寻的目录加到/etc/ld.so.conf/里；然後重新执行ldconfig。
 LD_NOWARN 仅适用於a.out。一旦设定了这一项（LD_NOWARN=true; export LD_NOWARN），它会告诉载入器必须处理fatal-warnings（像是次要版本不相容等）的警告讯息。 LD_WARN仅适用於ELF。设定这一项时，它会将通常是致命讯息的“Can*t find library”转换成警告讯息。对正常的操作而言，这并没有多大的用处，可是对ldd就很重要了。 LD_TRACE_LOADED_OBJECTS仅适用於ELF。而且会使得程式以为它们是由ldd所执行的： $ LD_TRACE_LOADED_OBJECTS=true
 /usr/bin/lynx libncurses.so.1 => /usr/lib/libncurses.so.1.9.6 libc.so.5 => /lib/libc.so.5.2.18
7.4 以动态载入撰写程式 
如果你很熟悉Solaris 2.x所支援的动态载入的工作的话，你会发现Linux在这点上与其非常的相近。这一部份在H.J.Lu的ELF程式设计文件内与dlopen(3)的manual page（可以在ld.so的套件上找到）上有广泛的讨论
