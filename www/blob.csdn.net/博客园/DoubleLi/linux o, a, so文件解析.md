# linux .o,.a,.so文件解析 - DoubleLi - 博客园






linux下文件的类型是不依赖于其后缀名的，但一般来讲：
.o,是目标文件,相当于windows中的.obj文件
.so 为共享库,是shared object,用于动态连接的,和dll差不多
.a为静态库,是好多个.o合在一起,用于静态连接
.la为libtool自动生成的一些共享库，vi编辑查看，主要记录了一些配置信息。可以用如下命令查看*.la文件的格式   $file *.la
      *.la: ASCII English text
所以可以用vi来查看其内容。
＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠
创建.a库文件和.o库文件：
[yufei@localhost perl_c2]$ pwd
/home/yufei/perl_c2

[yufei@localhost perl_c2]$ cat mylib.c
#include 
#include 
void hello(){
        printf("success call from perl to c library\n");
}
[yufei@localhost perl_c2]$ cat mylib.h
extern void hello();


[yufei@localhost perl_c2]$ gcc -c mylib.c
[yufei@localhost perl_c2]$ dir
mylib.c  mylib.h  mylib.o
[yufei@localhost perl_c2]$ ar -r mylib.a mylib.o
ar: 正在创建 mylib.a
[yufei@localhost perl_c2]$ dir
mylib.a  mylib.c  mylib.h  mylib.o


＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠＠
111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
动态链接库*.so的编译与使用- -


动态库*.so在linux下用c和c++编程时经常会碰到，最近在网站找了几篇文章介绍动态库的编译和链接，总算搞懂了这个之前一直不太了解得东东，这里做个笔记，也为其它正为动态库链接库而苦恼的兄弟们提供一点帮助。
1、动态库的编译

下面通过一个例子来介绍如何生成一个动态库。这里有一个头文件：so_test.h，三个.c文件：test_a.c、test_b.c、test_c.c，我们将这几个文件编译成一个动态库：libtest.so。

so_test.h：

#include 
#include 
void test_a();
void test_b();
void test_c();

test_a.c：

#include "so_test.h"
void test_a()
{
    printf("this is in test_a...\n");
}

test_b.c：

#include "so_test.h"
void test_b()
{
    printf("this is in test_b...\n");
}

test_c.c：

#include "so_test.h"
void test_c()
{
    printf("this is in test_c...\n");
}

将这几个文件编译成一个动态库：libtest.so
$ gcc test_a.c test_b.c test_c.c -fPIC -shared -o libtest.so

2、动态库的链接

在1、中，我们已经成功生成了一个自己的动态链接库libtest.so，下面我们通过一个程序来调用这个库里的函数。程序的源文件为：test.c。
test.c：

#include "so_test.h"
int main()
{
    test_a();
    test_b();
    test_c();
    return 0;

}
l         将test.c与动态库libtest.so链接生成执行文件test：
$ gcc test.c -L. -ltest -o test

l         测试是否动态连接，如果列出libtest.so，那么应该是连接正常了

$ ldd test
l         执行test，可以看到它是如何调用动态库中的函数的。
3、编译参数解析
最主要的是GCC命令行的一个选项:
          -shared 该选项指定生成动态连接库（让连接器生成T类型的导出符号表，有时候也生成弱连接W类型的导出符号），不用该标志外部程序无法连接。相当于一个可执行文件

l         -fPIC：表示编译为位置独立的代码，不用此选项的话编译后的代码是位置相关的所以动态载入时是通过代码拷贝的方式来满足不同进程的需要，而不能达到真正代码段共享的目的。

l         -L.：表示要连接的库在当前目录中

l         -ltest：编译器查找动态连接库时有隐含的命名规则，即在给出的名字前面加上lib，后面加上.so来确定库的名称

l         LD_LIBRARY_PATH：这个环境变量指示动态连接器可以装载动态库的路径。

l         当然如果有root权限的话，可以修改/etc/ld.so.conf文件，然后调用 /sbin/ldconfig来达到同样的目的，不过如果没有root权限，那么只能采用输出LD_LIBRARY_PATH的方法了。
4、注意
       调用动态库的时候有几个问题会经常碰到，有时，明明已经将库的头文件所在目录 通过 “-I” include进来了，库所在文件通过 “-L”参数引导，并指定了“-l”的库名，但通过ldd命令察看时，就是死活找不到你指定链接的so文件，这时你要作的就是通过修改 LD_LIBRARY_PATH或者/etc/ld.so.conf文件来指定动态库的目录。通常这样做就可以解决库无法链接的问题了。

makefile里面怎么正确的编译和连接生成.so库文件，然后又是在其他程序的makefile里面如何编译和连接才能调用这个库文件的函数????
答:
       你需要告诉动态链接器、加载器ld.so在哪里才能找到这个共享库,可以设置环境变量把库的路径添加到库目录/lib和/usr/lib，LD_LIBRARY_PATH=$(pwd),这种方法采用命令行方法不太方便,一种替代方法
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^注释^^^^^^^^^^^^^^^^^^^^^^^^^^^^
LD_LIBRARY_PATH可以在/etc/profile还是 ~/.profile还是 ./bash_profile里设置，或者.bashrc里，



改完后运行source /etc/profile或 . /etc/profile
更好的办法是添入/etc/ld.so.conf, 然后执行 /sbin/ldconfig


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^注释^^^^^^^^^^^^^^^^^^^^^^^^^^^^
是把库路径添加到/etc/ld.so.conf，然后以root身份运行ldconfig
      也可以在连接的时候指定文件路径和名称 -I  -L.

      GCC=gcc
CFLAGS=-Wall -ggdb -fPIC
#CFLAGS=
all: libfunc test
libfunc:func.o func1.o
        $(GCC) -shared -Wl,-soname,libfunc.so.1 -o libfunc.so.1.1 $<
        ln -sf libfunc.so.1.1 libfunc.so.1
        ln -sf libfunc.so.1 libfunc.so
***********************************************注释************************************************
ln -s是用来创建软链接，也就相当于windows中的快捷方式，在当前目录中创建上一级目录中的文件ttt的命名为ttt2软链接的命令是ln -s ../ttt ttt2,如果原文件也就是ttt文件删除的话，ttt2也变成了空文件。
ln -d是用来创建硬链接，也就相当于windows中文件的副本，当原文件删除的时候，并不影响“副本”的内容。

编译目标文件时使用gcc的-fPIC选项，产生与位置无关的代码并能被加载到任何地址：
gcc –fPIC –g –c liberr.c –o liberr.o


使用gcc的-shared和-soname选项；
使用gcc的-Wl选项把参数传递给连接器ld；
使用gcc的-l选项显示的连接C库，以保证可以得到所需的启动（startup）代码，从而避免程序在使用不同的，可能不兼容版本的C库的系统上不能启动执行。
gcc –g –shared –Wl,-soname,liberr.so –o liberr.so.1.0.0 liberr.o –lc


建立相应的符号连接：
ln –s liberr.so.1.0.0 liberr.so.1;
ln –s liberr.so.1.0.0 liberr.so;

在MAKEFILE中：
$@
    表示规则中的目标文件集。在模式规则中，如果有多个目标，那么，"$@"就是匹配于目标中模式定义的集合。
$%
    仅当目标是函数库文件中，表示规则中的目标成员名。例如，如果一个目标是"foo.a(bar.o)"，那么，"$%"就是"bar.o"，"$@"就是 "foo.a"。如果目标不是函数库文件（Unix下是[.a]，Windows下是[.lib]），那么，其值为空。
$<
    依赖目标中的第一个目标名字。如果依赖目标是以模式（即"%"）定义的，那么"$<"将是符合模式的一系列的文件集。注意，其是一个一个取出来的。
$?
    所有比目标新的依赖目标的集合。以空格分隔。
$^
    所有的依赖目标的集合。以空格分隔。如果在依赖目标中有多个重复的，那个这个变量会去除重复的依赖目标，只保留一份。
*********************************************注释***********************************************************************
test: test.o libfunc
        $(GCC) -o test test.o -L. -lfunc
%.o:%.c
        $(GCC) -c $(CFLAGS) -o $@ $<
clean:
        rm -fr *.o
        rm -fr *.so*
        rm -fr test

要 生成.so文件，cc要带-shared 参数；要调用.so的文件，比如libfunc.so,可以在cc命令最后加上-lfunc，还要视情况加上 -L/usr/xxx 指出libfunc.so的路径；这样，在你要编译的源文件中就可以调用libfunc.so这个库文件的函数.
       前面的都说的差不多了，最后提醒一下最好提供一个接口头文件
       动态加载,用dlopen,dlclose,dlsym 



ref:http://niefei.blog.ccidnet.com/blog/ccid/do_showone/tid_42855.html
1. 介绍

　 　使用GNU的工具我们如何在Linux下创建自己的程序函数库?一个“程序 函数库”简单的说就是一个文件包含了一些编译好的代码和数据，这些编 译好的代码和数据可以在事后供其他的程序使用。程序函数库可以使整个程序更加模块化，更容易重新编译，而且更方便升级。程序函数库可分为3种类型：静态函 数库（static libraries）、共享函数库（shared libraries）和动态加载函数库（dynamically loaded libraries）。

　　静态函数库是在程序执行前就加入到目标程序中去了；而共享函数库则是在程序启动的时候加载到程序中，它可以被 不同的程序共享；动态加载函数库则可以在程序运行的任何时候动态的加载。实际上，动态函数库并非另外一种库函数格式，区别是动态加载函数库是如何被程序员 使用的。后面我们将举例说明。

　　本文档主要参考Program Library HOWTO，作者是luster（[hwang@ustc.edu](mailto:hwang@ustc.edu)），任何非商业目的的再次发行本文档都是允许的，但是请保留作者信息和本版权声明。本文档首先在[www.linuxaid.com.cn](http://www.linuxaid.com.cn/)发布。

　　2. 静态函数库

　 　静态函数库实际上就是简单的一个普通的目标文件的集合，一般来说习惯用“.a”作为文件的后缀。可以用ar这个程序来产生静态函数库文件。Ar 是archiver的缩写。静态函数库现在已经不在像以前用得那么多了，主要是共享函数库与之相比较有很多的优势的原因。慢慢地，大家都喜欢使用共享函数 库了。不过，在一些场所静态函数库仍然在使用，一来是保持一些与以前某些程序的兼容，二来它描述起来也比较简单。

　　静态库函数允许程序 员把程序link起来而不用重新编译代码，节省了重新编译代码的时间。不过，在今天这么快速的计算机面前，一般的程序的重新编译也花费不了多少时间，所以 这个优势已经不是像它以前那么明显了。静态函数库对开发者来说还是很有用的，例如你想把自己提供的函数给别人使用，但是又想对函数的源代码进行保密，你就 可以给别人提供一个静态函数库文件。理论上说，使用ELF格式的静态库函数生成的代码可以比使用共享函数库（或者动态函数 库）的程序运行速度上快一些，大概1－5％。

　　创建一个静态函数库文件，或者往一个已经存在地静态函数库文件添加新的目标代码，可以用下面的命令：



ar rcs my_library.a file1.o file2.o

　 　这个例子中是把目标代码file1.o和file2.o加入到my_library.a这个函数库文件中，如果my_library.a不存在 则创建一个新的文件。在用ar命令创建静态库函数的时候，还有其他一些可以选择的参数，可以参加ar的使用帮助。这里不再赘述。

　　一旦 你创建了一个静态函数库，你可以使用它了。你可以把它作为你编译和连接过程中的一部分用来生成你的可执行代码。如果你用gcc来编译产生可 执行代码的话，你可以用“-l”参数来指定这个库函数。你也可以用ld来做，使用它的“-l”和“-L”参数选项。具体用法，可以参考info:gcc。

　3. 共享函数库

　　共享函数库中的函数是在当一个可执行程序在启动的时候被加载。如果一个共享函数库正常安装，所有的程序在重新运行的时候都可以自动加载最新的函数库中的函数。对于Linux系统还有更多的可以实现的功能：

o 升级了函数库但是仍然允许程序使用老版本的函数库。 o 当执行某个特定程序的时候可以覆盖某个特定的库或者库中指定的函数。 o 可以在库函数被使用的过程中修改这些函数库。

　　3.1. 一些约定

　 　如果你要编写的共享函数库支持所有有用的特性，你在编写的过程中必须遵循一系列约定。你必须理解库的不同的名字间的区别，例如它的 “soname”和“real name”之间的区别和它们是如何相互作用的。你同样还要知道你应该把这些库函数放在你文件系统的什么位置等等。下面我们具体看看这些问题。

　　3.1.1. 共享库的命名

　　每个共享函数库都有个特殊的名字，称作“soname”。Soname名字命名必须以“lib”作为前缀，然后是函数库的名字，然后是“.so”，最后是版本号信息。不过有个特例，就是非常底层的C库函数都不是以lib开头这样命名的。

　　每个共享函数库都有一个真正的名字（“real name”），它是包含真正库函数代码的文件。真名有一个主版本号，和一个发行版本号。最后一个发行版本号是可选的，可以没有。主版本号和发行版本号使你可以知道你到底是安装了什么版本的库函数。

另外，还有一个名字是编译器编译的时候需要的函数库的名字，这个名字就是简单的soname名字，而不包含任何版本号信息。

　 　管理共享函数库的关键是区分好这些名字。当可执行程序需要在自己的程序中列出这些他们需要的共享库函数的时候，它只要用soname就可以了； 反过来，当你要创建一个新的共享函数库的时候，你要指定一个特定的文件名，其中包含很细节的版本信息。当你安装一个新版本的函数库的时候，你只要先将这些 函数库文件拷贝到一些特定的目录中，运行ldconfig这个实用就可以。Ldconfig检查已经存在的库文件，然后创建soname的符号链接到真正 的函数库，同时设置/etc/ld.so.cache这个缓冲文件。这个我们稍后再讨论。

　　Ldconfig并不设置链接的名字，通常 的做法是在安装过程中完成这个链接名字的建立，一般来说这个符号链接就简单的指向最新的soname 或者最新版本的函数库文件。最好把这个符号链接指向soname，因为通常当你升级你的库函数的后，你就可以自动使用新版本的函数库勒。

　　我们来举例看看：

　 　/usr/lib/libreadline.so.3 是一个完全的完整的soname，ldconfig可以设置一个符号链接到其他某个真正的函数库文件，例如是 /usr/lib/libreadline.so.3.0。同时还必须有一个链接名字，例如/usr/lib/libreadline.so 就是一个符号链接指向/usr/lib/libreadline.so.3。

3.1.2. 文件系统中函数库文件的位置

　 　共享函数库文件必须放在一些特定的目录里，这样通过系统的环境变量设置，应用程序才能正确的使用这些函数库。大部分的源码开发的程序都遵循 GNU的一些标准，我们可以看info帮助文件获得相信的说明，info信息的位置是：info: standards#Directory_Variables。GNU标准建议所有的函数库文件都放在/usr/local/lib目录下，而且建议命令 可执行程序都放在/usr/local/bin目录下。这都是一些习惯问题，可以改变的。

　　文件系统层次化标准FHS（Filesystem Hierarchy Standard）（[http://www.pathname.com/fhs](http://www.pathname.com/fhs)）规定了在一个发行包中大部分的函数库文件应该安装到/usr/lib目录 下，但是如果某些库是在系统启动的时候要加载的，则放到/lib目录下，而那些不是系统本身一部分的库则放到/usr/local/lib下面。

　　上面两个路径的不同并没有本质的冲突。GNU提出的标准主要对于开发者开发源码的，而FHS的建议则是针对发行版本的路径的。具体的位置信息可以看/etc/ld.so.conf里面的配置信息。

　　3.2. 这些函数库如何使用

　 　在基于GNU glibc的系统里，包括所有的linux系统，启动一个ELF格式的二进制可执行文件会自动启动和运行一个program loader。对于Linux系统，这个loader的名字是/lib/ld-linux.so.X（X是版本号）。这个loader启动后，反过来就会 load所有的其他本程序要使用的共享函数库。

　　到底在哪些目录里查找共享函数库呢？这些定义缺省的是放在 /etc/ld.so.conf文件里面，我们可以修改这个文件，加入我们自己的一些 特殊的路径要求。大多数RedHat系列的发行包的/etc/ld.so.conf文件里面不包括/usr/local/lib这个目录，如果没有这个目 录的话，我们可以修改/etc/ld.so.conf，自己手动加上这个条目。

　　如果你想覆盖某个库中的一些函数，用自己的函数替换它们，同时保留该库中其他的函数的话，你可以在/etc/ld.so.preload中加入你想要替换的库（.o结尾的文件），这些preloading的库函数将有优先加载的权利。

　 　当程序启动的时候搜索所有的目录显然会效率很低，于是Linux系统实际上用的是一个高速缓冲的做法。Ldconfig缺省情况下读出 /etc/ld.so.conf相关信息，然后设置适当地符号链接，然后写一个cache到/etc/ld.so.cache这个文件中，而这个 /etc/ld.so.cache则可以被其他程序有效的使用了。这样的做法可以大大提高访问函数库的速度。这就要求每次新增加一个动态加载的函数库的时 候，就要运行ldconfig来更新这个cache，如果要删除某个函数库，或者某个函数库的路径修改了，都要重新运行ldconfig来更新这个 cache。通常的一些包管理器在安装一个新的函数库的时候就要运行ldconfig。

　　另外，FreeBSD使用cache的文件不一样。FreeBSD的ELF cache是/var/run/ld-elf.so.hints，而a.out的cache责是/var/run/ld.so.hints。它们同样是通过ldconfig来更新。

　　3.3. 环境变量

　 　各种各样的环境变量控制着一些关键的过程。例如你可以临时为你特定的程序的一次执行指定一个不同的函数库。Linux系统中，通常变量 LD_LIBRARY_PATH就是可以用来指定函数库查找路径的，而且这个路径通常是在查找标准的路径之前查找。这个是很有用的，特别是在调试一个新的 函数库的时候，或者在特殊的场合使用一个肥标准的函数库的时候。环境变量LD_PRELOAD列出了所有共享函数库中需要优先加载的库文件，功能和 /etc/ld.so.preload类似。这些都是有/lib/ld-linux.so这个loader来实现的。值得一提的是， LD_LIBRARY_PATH可以在大部分的UNIX-linke系统下正常起作用，但是并非所有的系统下都可以使用，例如HP－UX系统下，就是用 SHLIB_PATH这个变量，而在AIX下则使用LIBPATH这个变量。

　　LD_LIBRARY_PATH在开发和调试过程中经常大量使用，但是不应该被一个普通用户在安装过程中被安装程序修改，大家可以去参考 [http://www.visi.com/~barr/ldpath.html](http://www.visi.com/~barr/ldpath.html),这里有一个文档专门介绍为什么不使用LD_LIBRARY_PATH这个 变量。

　　事实上还有更多的环境变量影响着程序的调入过程，它们的名字通常就是以LD_或者RTLD_打头。大部分这些环境变量的使用的文档都是不全，通常搞得人头昏眼花的，如果要真正弄清楚它们的用法，最好去读loader的源码（也就是gcc的一部分）。

　 　允许用户控制动态链接函数库将涉及到setuid/setgid这个函数如果特殊的功能需要的话。因此，GNU loader通常限制或者忽略用户对这些变量使用setuid和setgid。如果loader通过判断程序的相关环境变量判断程序的是否使用了 setuid或者setgid，如果uid和euid不同，或者gid和egid部一样，那么loader就假定程序已经使用了setuid或者 setgid，然后就大大的限制器控制这个老链接的权限。如果阅读GNU glibc的库函数源码，就可以清楚地看到这一点，特别的我们可以看elf/rtld.c和sysdeps/generic/dl-sysdep.c这两 个文件。这就意味着如果你使得uid和gid与euid和egid分别相等，然后调用一个程序，那么这些变量就可以完全起效。

3.4. 创建一个共享函数库

　 　现在我们开始学习如何创建一个共享函数库。其实创建一个共享函数库非常容易。首先创建object文件，这个文件将加入通过gcc –fPIC 参数命令加入到共享函数库里面。PIC的意思是“位置无关代码”（Position Independent Code）。下面是一个标准的格式：



gcc -shared -Wl,-soname,your_soname -o library_name file_list library_list

　　下面再给一个例子，它创建两个object文件（a.o和b.o），然后创建一个包含a.o和b.o的共享函数库。例子中”-g”和“－Wall”参数不是必须的。



gcc -fPIC -g -c -Wall a.cgcc -fPIC -g -c -Wall b.cgcc -shared -Wl,

-soname,liblusterstuff.so.1 -o liblusterstuff.so.1.0.1 a.o b.o -lc

　　下面是一些需要注意的地方：

· 不用使用-fomit-frame-pointer这个编译参数除非你不得不这样。虽然使用了这个参数获得的函数库仍然可以使用，但是这使得调试程序几乎 没有用，无法跟踪调试。 · 使用-fPIC来产生代码，而不是-fpic。 · 某些情况下，使用gcc 来生成object文件，需要使用“-Wl,-export-dynamic”这个选项参数。通常，动态函数库的符号表里面包含了这些动态的对象的符号。 这个选项在创建ELF格式的文件时候，会将所有的符号加入到动态符号表中。可以参考ld的帮助获得更详细的说明。

　　3.5. 安装和使用共享函数库

　　一旦你了一个共享函数库，你还需要安装它。其实简单的方法就是拷贝你的库文件到指定的标准的目录（例如/usr/lib），然后运行ldconfig。

　 　如果你没有权限去做这件事情，例如你不能修改/usr/lib目录，那么你就只好通过修改你的环境变量来实现这些函数库的使用了。首先，你需要 创建这些共享函数库；然后，设置一些必须得符号链接，特别是从soname到真正的函数库文件的符号链接，简单的方法就是运行ldconfig：



ldconfig -n directory_with_shared_libraries

　　然后你就可以设置你的LD_LIBRARY_PATH这个环境变量，它是一个以逗号分隔的路径的集合，这个可以用来指明共享函数库的搜索路径。例如，使用bash，就可以这样来启动一个程序my_program:



LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH my_program

　 　如果你需要的是重载部分函数，则你就需要创建一个包含需要重载的函数的object文件，然后设置LD_PRELOAD环境变量。通常你可以很 方便的升级你的函数库，如果某个API改变了，创建库的程序会改变soname。然而，如果一个函数升级了某个函数库而保持了原来的soname，你可以 强行将老版本的函数库拷贝到某个位置，然后重新命名这个文件（例如使用原来的名字，然后后面加.orig后缀），然后创建一个小的“wrapper”脚本 来设置这个库函数和相关的东西。例如下面的例子：



#!/bin/sh export LD_LIBRARY_PATH=/usr/local/my_lib:$LD_LIBRARY_PATH exec

/usr/bin/my_program.orig $*

　　我们可以通过运行ldd来看某个程序使用的共享函数库。例如你可以看ls这个实用工具使用的函数库：



ldd /bin/ls

    libtermcap.so.2 => /lib/libtermcap.so.2 (0x4001c000)

    libc.so.6 => /lib/libc.so.6 (0x40020000)

    /lib/ld-linux.so.2 => /lib/ld-linux.so.2 (0x40000000)

　　通常我么可以看到一个soname的列表，包括路径。在所有的情况下，你都至少可以看到两个库：



· /lib/ld-linux.so.N（N是1或者更大，一般至少2）。

这是这个用力加载其他所有的共享库的库。

· libc.so.N(N应该大于或者等于6)。这是C语言函数库。

　 　值得一提的是，不要在对你不信任的程序运行ldd命令。在ldd的manual里面写得很清楚，ldd是通过设置某些特殊的环境变量（例如，对 于ELF对象，设置LD_TRACE_LOADED_OBJECTS），然后运行这个程序。这样就有可能使得某地程序可能使得ldd来执行某些意想不到的 代码，而产生不安全的隐患。

3.6. 不兼容的函数库

　　如果一个新版的函数库要和老版本的二进制的库不兼容，则soname需要改变。对于C语言，一共有4个基本的理由使得它们在二进制代码上很难兼容：

　　o. 一个函数的行文改变了，这样它就可能与最开始的定义不相符合。

　　o. 输出的数据项改变了。

　　o. 某些输出的函数删除了。

　　o. 某些输出函数的接口改变了。

　　如果你能避免这些地方，你就可以保持你的函数库在二进制代码上的兼容，或者说，你可以使得你的程序的应用二进制接口（ABI：Application Binary Interface）上兼容。

　　4. 动态加载的函数库Dynamically Loaded (DL) Libraries

　 　动态加载的函数库Dynamically loaded (DL) libraries是一类函数库，它可以在程序运行过程中的任何时间加载。它们特别适合在函数中加载一些模块和plugin扩展模块的场合，因为它可以在 当程序需要某个plugin模块时才动态的加载。例如，Pluggable Authentication Modules(PAM)系统就是用动态加载函数库来使得管理员可以配置和重新配置身份验证信息。

　　Linux系统下，DL函数库与其 他函数库在格式上没有特殊的区别，我们前面提到过，它们创建的时候是标准的object格式。主要的区别就是 这些函数库不是在程序链接的时候或者启动的时候加载，而是通过一个API来打开一个函数库，寻找符号表，处理错误和关闭函数库。通常C语言环境下，需要包 含这个头文件。

　　Linux中使用的函数和Solaris中一样，都是dlpoen（） API。当时不是所有的平台都使用同样的接口，例如HP-UX使用shl_load()机制，而Windows平台用另外的其他的调用接口。如果你的目的 是使得你的代码有很强的移植性，你应该使用一些wrapping函数库，这样的wrapping函数库隐藏不同的平台的接口区别。一种方法是使用 glibc函数库中的对动态加载模块的支持，它使用一些潜在的动态加载函数库界面使得它们可以夸平台使用。具体可以参考http: //developer.gnome.org/doc/API/glib/glib-dynamic-loading-of-modules.html. 另外一个方法是使用libltdl，是GNU libtool的一部分，可以进一步参考CORBA相关资料。

　　4.1. dlopen()

　　dlopen函数打开一个函数库然后为后面的使用做准备。C语言原形是：



void * dlopen(const char *filename, int flag);

　　如果文件名filename是以“/”开头，也就是使用绝对路径，那么dlopne就直接使用它，而不去查找某些环境变量或者系统设置的函数库所在的目录了。否则dlopen（）

　　就会按照下面的次序查找函数库文件：

1. 环境变量LD_LIBRARY指明的路径。 2. /etc/ld.so.cache中的函数库列表。 3. /lib目录，然后/usr/lib。不过一些很老的a.out的loader则是采用相反的次序，也就是先查/usr/lib，然后是/lib。

　 　Dlopen()函数中，参数flag的值必须是RTLD_LAZY或者RTLD_NOW，RTLD_LAZY的意思是resolve undefined symbols as code from the dynamic library is executed，而RTLD_NOW的含义是resolve all undefined symbols before dlopen() returns and fail if this cannot be done'。

　　如果有好几个函数库，它们之间有一些依赖关系的话，例如X依赖Y，那么你就要先加载那些被依赖的函数。例如先加载Y，然后加载X。

　　dlopen（）函数的返回值是一个句柄，然后后面的函数就通过使用这个句柄来做进一步的操作。如果打开失败dlopen()就返回一个NULL。如果一个函数库被多次打开，它会返回同样的句柄。

　　如果一个函数库里面有一个输出的函数名字为_init,那么_init就会在dlopen（）这个函数返回前被执行。我们可以利用这个函数在我的函数库里面做一些初始化的工作。我们后面会继续讨论这个问题的。

　　4.2. dlerror()

　　通过调用dlerror()函数，我们可以获得最后一次调用dlopen()，dlsym()，或者dlclose（）的错误信息。

4.3. dlsym()

　　如果你加载了一个DL函数库而不去使用当然是不可能的了，使用一个DL函数库的最主要的一个函数就是dlsym()，这个函数在一个已经打开的函数库里面查找给定的符号。这个函数如下定义：



void * dlsym(void *handle, char *symbol);

　　函数中的参数handle就是由dlopen打开后返回的句柄，symbol是一个以NIL结尾的字符串。

　 　如果dlsym()函数没有找到需要查找的symbol，则返回NULL。如果你知道某个symbol的值不可能是NULL或者0，那么就很 好，你就可以根据这个返回结果判断查找的symbol是否存在了；不过，如果某个symbol的值就是NULL，那么这个判断就有问题了。标准的判断方法 是先调用dlerror()，清除以前可能存在的错误，然后调用dlsym（）来访问一个symbol，然后再调用dlerror（）来判断是否出现了错 误。一个典型的过程如下：



dlerror(); /* clear error code */

s = (actual_type) dlsym(handle, symbol_being_searched_for);

if ((err = dlerror()) != NULL)

{

/* handle error, the symbol wasn't found */

}

else

{

/* symbol found, its value is in s */

}

　　4.4. dlclose()

　 　dlopen()函数的反过程就是dlclose（）函数，dlclose（）函数用力关闭一个DL函数库。Dl函数库维持一个资源利用的计数 器，当调用dlclose的时候，就把这个计数器的计数减一，如果计数器为0，则真正的释放掉。真正释放的时候，如果函数库里面有_fini()这个函 数，则自动调用_fini（）这个函数，做一些必要的处理。Dlclose（）返回0表示成功，其他非0值表示错误。

　　4.5. DL Library Example

　　下面是一个例子。例子中调入math函数库，然后打印2.0的余弦函数值。例子中每次都检查是否出错。应该是个不错的范例：



  #include

  #include

  #include

  int main(int argc, char **argv)

  {

    void *handle;

    double (*cosine)(double);

    char *error;

    handle = dlopen ("/lib/libm.so.6", RTLD_LAZY);

    if (!handle) {

        fputs (dlerror(), stderr);

        exit(1);

    }

    cosine = dlsym(handle, "cos");

    if ((error = dlerror()) != NULL)

  {

        fputs(error, stderr);

        exit(1);

    }

    printf ("%f ", (*cosine)(2.0));

    dlclose(handle);

}

　　如果这个程序名字叫foo.c,那么用下面的命令来编译：



  gcc -o foo foo.c -ldl



5. 其他

　　5.1. nm命令

　 　nm命令可以列出一个函数库文件中的符号表。它对于静态的函数库和共享的函数库都起作用。对于一个给定的函数库，nm命令可以列出函数库中定义 的所有符号，包括每个符号的值和类型。还可以给出在原程序中这个函数（符号）是在多少行定义的，不过这必须要求编译该函数库的时候加“-l”选项。

　 　关于符号的类型，这里我们再多讨论一下。符号的类型是以一个字母的形式显示的，小写字母表示这个符号是本地（local）的，而大写字母则表示 这个符号是全局的（global,externel）。一般来说，类型有一下几种：T、D、B、U、W。各自的含义如下：T表示在代码段中定义的一般变量 符号；D表示时初始化过的数据段；B表示初始化的数据段；U表示没有定义的，在这个库里面使用了，但是在其他库中定义的符号；W，weak的缩写，表示如 果其他函数库中也有对这个符号的定义，则其他符号的定义可以覆盖这个定义。

　　如果你知道一个函数的名字，但是你不知道这个函数在什么库中定义的，那么可以用mn的“-o”选项和grep命令来查找库的名字。-o选项使得显示的每一行都有这个函数库文件名。例如，你要查找“cos”这个是在什么地方定义的，大致可以用下面的命令：



nm -o /lib/* /usr/lib/* /usr/lib/*/* /usr/local/lib/* 2> /dev/null

| grep 'cos$'

　　关于nm的更详细的用法我们可以参考info文档，位置是info:binutils#nm。

　　5.2. 特殊函数_init和_fini

　 　函数库里面有两个特殊的函数，_init和_fini，这个我们在前面已经说过了。主要是分别用来初始化函数库和关闭的时候做一些必要的处理， 我们可以把自己认为需要的代码放到这两个函数里面，它们分别在函数库被加载和释放的时候被执行。具体说，如果一个函数库里面有一个名字为“_init”的 函数输出，那么在第一次通过dlopen()函数打开这个函数库，或者只是简单的作为共享函数库被打开的时候，_init函数被自动调用执行。与之相对应 的就是_fini函数，当一个程序调用dlclose()去释放对这个函数库的引用的时候，如果该函数库的被引用计数器为0了，或者这个函数库是作为一般 的共享函数库被使用而使用它的程序正常退出的时候，_fini就会被调用执行。C语言定义它们的原型如下：



void _init(void); void _fini(void);

　　当用gcc编译源程序为“.o”文件的时候，需要加一个“-nostartfiles”选项。这个选项使得C编译器不链接系统的启动函数库里面的启动函数。否则，就会得到一个“multiple-definition”的错误。

　　5.3. 共享函数库也可以使脚本（Scripts）

　　GNU的loader允许使用特殊格式的脚本语言来写一个函数库。这对于那些需要间接包含其他函数库的情况还是有用的。例如，下面是一个/usr/lib/libc.so的例子：



/* GNU ld script Use the shared library, but some functions are only in

the static library, so try that secondarily. */GROUP ( /lib/libc.so.6

/usr/lib/libc_nonshared.a )

　　更多的信息可以参考texinfo文档中关于ld链接的脚本部分。一般的信息还可以参考: info:ld#Options 和info:ld#Commands，也可以参考info:ld#Option Commands。

　　5.4. GNU libtool

　 　如果你正在编译的系统相很方便的移植到其他操作系统下，你可以使用GNU libtool来创建和安装这个函数库。GNU libtool是一个函数库支持的典型的脚本。Libtool隐藏了使用一个可移植的函数库的负责性。Libtool提供了一个可以移植的界面来创建 object文件，链接函数库（静态或者共享的），并且安装这些库。它还包含了libltdl，一个可移植的动态函数库调入程序的wrapper。更多的 详细讨论，可以在[http://www.gnu.org/software/libtool/manual.html](http://www.gnu.org/software/libtool/manual.html)看到。

　　5.5. 删除一些符号

　　在一个生产的文件中很多符号都是为了debug而包含的，占用了不少空间。如果空间不够，而且这些符号也许不再需要，就可以将其中一些删除。

　 　最好的方法就是先正常的生成你需要的object文件，然后debug和测试你需要的一些东西。一旦你完全测试完毕了，就可以用strip去删 除一些不需要的符号了。Strip命令可以使你很方便的控制删除什么符号，而保留什么符号。Strip的具体用法可以参考其帮助文件。

　　另外的方法就是使用GNU ld的选项“-S”和“-s”;“-S”会删除一些debugger的符号，而“-s”则是将所有的符号信息都删除。通常我们可以在gcc中加这样的参数“-Wl,-S”和“-Wl,-s”来达到这个目的。

摘要

下 面是一些例子，例子中我们会使用三种函数库（静态的、共享的和动态加载的函数库）。文件libhello.c是一个函数库，libhello.h 是它的头文件；demo_use.c则是一个使用了libhello函数库的。Script_static和script_dynamic分别演示如何以 静态和共享方式使用函数库，而后面的demo_dynamic.c和script_dynamic则表示演示如何以动态加载函数库的方式来使用它。

(2002-08-25 17:38:37)

By Wing

　　6. 更多的例子

　 　下面是一些例子，例子中我们会使用三种函数库（静态的、共享的和动态加载的函数库）。文件libhello.c是一个函数库， libhello.h是它的头文件；demo_use.c则是一个使用了libhello函数库的。Script_static和 script_dynamic分别演示如何以静态和共享方式使用函数库，而后面的demo_dynamic.c和script_dynamic则表示演示 如何以动态加载函数库的方式来使用它。

　　6.1. File libhello.c



/* libhello.c - demonstrate library use. */

#include

void hello(void)

{

printf("Hello, library world.

");

}

　　6.2. File libhello.h



/* libhello.h - demonstrate library use. */

void hello(void);

　　6.3. File demo_use.c



/* demo_use.c -- demonstrate direct use of the "hello" routine */

#include "libhello.h"

int main(void)

{

hello();

return 0;

}

　　6.4. File script_static



#!/bin/sh

# Static library demo

# Create static library's object file, libhello-static.o.

# I'm using the name libhello-static to clearly

# differentiate the static library from the

# dynamic library examples, but you don't need to use

# "-static" in the names of your

# object files or static libraries.gcc -Wall -g -c -o libhello-static.o

libhello.c

# Create static library.ar rcs libhello-static.a libhello-static.o

# At this point we could just copy libhello-static.a

# somewhere else to use it.

# For demo purposes, we'll just keep the library

# in the current directory.

# Compile demo_use program file.gcc -Wall -g -c demo_use.c -o demo_use.o

# Create demo_use program; -L. causes "." to be searched during

# creation of the program. Note that this command causes

# the relevant object file in libhello-static.a to be

# incorporated into file demo_use_static.gcc -g -o demo_use_static

demo_use.o -L. -lhello-static

# Execute the program../demo_use_static

　　6.5. File script_shared



#!/bin/sh

# Shared library demo

# Create shared library's object file, libhello.o.gcc -fPIC -Wall

-g -c libhello.c

# Create shared library.

# Use -lc to link it against C library, since libhello

# depends on the C library.gcc -g -shared -Wl,-soname,libhello.so.0 -o

libhello.so.0.0 libhello.o -lc# At this point we could just copy

libhello.so.0.0 into

# some directory, say /usr/local/lib.

# Now we need to call ldconfig to fix up the symbolic links.

# Set up the soname. We could just execute:

# ln -sf libhello.so.0.0 libhello.so.0

# but let's let ldconfig figure it out./sbin/ldconfig -n .

# Set up the linker name.

# In a more sophisticated setting, we'd need to make

# sure that if there was an existing linker name,

# and if so, check if it should stay or not.ln -sf libhello.so.0

libhello.so

# Compile demo_use program file.gcc -Wall -g -c demo_use.c -o

demo_use.o

# Create program demo_use.

# The -L. causes "." to be searched during creation

# of the program; note that this does NOT mean that "."

# will be searched when the program is executed.gcc -g -o demo_use

demo_use.o -L. -lhello

# Execute the program. Note that we need to tell the program

# where the shared library is,

using LD_LIBRARY_PATH.LD_LIBRARY_PATH="." ./demo_use

　　6.6. File demo_dynamic.c



/* demo_dynamic.c -- demonstrate dynamic loading and

use of the "hello" routine */

/* Need dlfcn.h for the routines to

dynamically load libraries */

#include

#include

#include

/* Note that we don't have to include "libhello.h".

  However, we do need to specify something related;

  we need to specify a type that will hold the value

  we're going to get from dlsym(). */

/* The type "simple_demo_function" describes a function that

  takes no arguments, and returns no value: */

typedef void (*simple_demo_function)(void);

int main(void)

{

const char *error;

void *module;

simple_demo_function demo_function;

/* Load dynamically loaded library */

module = dlopen("libhello.so", RTLD_LAZY);

if (!module)

{

  fprintf(stderr, "Couldn't open libhello.so: %s

",dlerror());

  exit(1);

}

/* Get symbol */

dlerror();

demo_function = dlsym(module, "hello");

if ((error = dlerror()))

{

  fprintf(stderr, "Couldn't find hello: %s

", error);

  exit(1);

}

/* Now call the function in the DL library */

(*demo_function)();

/* All done, close things cleanly */

dlclose(module);

return 0;

}

　　6.7. File script_dynamic



#!/bin/sh

# Dynamically loaded library demo

# Presume that libhello.so and friends have

# been created (see dynamic example).

# Compile demo_dynamic program file into an object file.gcc

-Wall -g -c demo_dynamic.c

# Create program demo_use.

# Note that we don't have to tell it where to search

for DL libraries,

# since the only special library this program uses won't be

# loaded until after the program starts up.

# However, we DO need the option -ldl to include the library

# that loads the DL libraries.gcc -g -o demo_dynamic

demo_dynamic.o -ldl

# Execute the program. Note that we need to tell the
# program where get the dynamically loaded library,
# using LD_LIBRARY_PATH.LD_LIBRARY_PATH="." ./demo_dynamic









