# GNU gcc的编译工具用法(转) - maopig的专栏 - CSDN博客
2011年09月28日 16:56:34[maopig](https://me.csdn.net/maopig)阅读数：829标签：[gcc																[工具																[linux																[library																[path																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
对大多数不从事Linux平台C语言开发的人来说，GNU gcc的一套工具和Linux平台的共享库的使用还是十分陌生的，其实我也不太熟悉，姑且写点基础知识，权当做备忘吧。
## 一、GNU gcc的编译工具用法
我们先来写一个简单的C程序：hello.c 
C代码 
|`01`|`#include <stdio.h>`|
|`02`||
|`03`|`void``print_hello() {`|
|`04`|`  ``printf``(``"Hello World\n"``);`|
|`05`|`}`|
|`06`||
|`07`|`int``main(``int``argc, ``char````argv[]) {```|
|`08`|`  ``print_hello();`|
|`09`|`  ``return``0;`|
|`10`|`}`|
定义了一个print_hello函数，调用main函数打印Hello World。 
如何编译它呢？ 
C代码 
|`1`|`gcc -o hello -O2 hello.c`|
-o参数指定生成的可执行程序的文件名， -O2是优化级别。该命令会编译生成hello可执行程序，看看这个文件：ls -l hello
C代码 
|`1`|`-rwxr-xr-x  1 robbin ``users``11939 2008-11-02 13:48 hello`|
有11KB大小。 
看看他链接了哪些系统动态链接库，用ldd命令：
C代码 
|`1`|`ldd hello`|
输出信息为：
C代码 
|`1`|`libc.so.6 => /lib64/tls/libc.so.6 (0x0000002a9566d000)`|
|`2`|`/lib64/ld-linux-x86-64.so.2 (0x0000002a95556000)`|
libc是C语言标准函数库，ld是动态链接器。 
接着我们看看hello这个程序里面有哪些符号，用nm命令：
C代码 
|`1`|`nm hello`|
输出：
C代码 
|`1`|`00000000005008f8 A __bss_start`|
|`2`|`000000000040043c t call_gmon_start`|
|`3`|`......`|
|`4`|`00000000004004f0 T main`|
|`5`|`0000000000500658 d p.0`|
|`6`|`00000000004004e0 T print_hello`|
|`7`|`                 ``U puts@@GLIBC_2.2.5`|
|`8`|`0000000000400410 T _start`|
中间省略了一些，不过我们还是可以在符号表里面找到函数定义。 
hello有11KB，体积偏大，去处符号表可以给它瘦身，我们用strip命令：
C代码 
|`1`|`strip hello`|
然后再ls -l hello，输出为：
C代码 
|`1`|`-rwxr-xr-x  1 webuser ``users``4464 2008-11-02 13:56 hello`|
只有4.4KB了，瘦身效果明显！ 不过这次符号表再也看不到了，nm hello，输出为：nm: hello: no symbols。 
最后如果我们想从可执行程序里面提取出来一点什么文本信息的话，还可以用strings命令：
C代码 
|`1`|`strings hello`|
输出信息为：
C代码 
|`1`|`/lib64/ld-linux-x86-64.so.2`|
|`2`|`SuSE`|
|`3`|`libc.so.6`|
|`4`|`puts`|
|`5`|`__libc_start_main`|
|`6`|`__gmon_start__`|
|`7`|`GLIBC_2.2.5`|
|`8`|`t fff`|
|`9`|`Hello World`|
友情提醒一下，如果你用Java写一个HelloWorld.java，编译以后你也可以用strings窥探一番。 
## 二、动态共享库怎么使用
这次我们把hello.c拆开成为两个文件：hello.c和main.c。hello.c的代码是：
C代码 
|`1`|`#include <stdio.h>`|
|`2`||
|`3`|`void``print_hello() {`|
|`4`|`  ``printf``(``"Hello World\n"``);`|
|`5`|`}`|
而main.c的代码是：
C代码 
|`1`|`int``main(``int``argc, ``char````argv[]) {```|
|`2`|`  ``print_hello();`|
|`3`|`  ``return``0;`|
|`4`|`}`|
hello.c是我们的动态共享库，在hello.c里面我们声明和实现了各种公用的函数，最后main.c可以去调用这些公用函数。首先我们要把hello.c编译成为动态共享库：
C代码 
|`1`|`gcc -o libhello.so -O2 -fPIC -shared hello.c`|
-fPIC参数声明链接库的代码段是可以共享的，-shared参数声明编译为共享库。请注意这次我们编译的共享库的名字叫做libhello.so，这也是Linux共享库的一个命名的惯例了：后缀使用so，而名称使用libxxxx格式。
然后编译main.c的时候，我们需要更多的参数让gcc知道如何寻找共享库：
C代码 
|`1`|`gcc -o main -O2 -L. -lhello main.c`|
-L参数指定到哪个附加路径下面去寻找共享库，现在我们指定在当前目录下面寻找； 
-l参数指定链接到哪个共享库上面，我们传的参数hello，那么gcc就会自动链接到libhello.so这个共享库上面(注意我们上面说的libXXXX.so命名规则)；
-I参数指定到哪个附加路径下面去寻找h文件，这个我们没有使用。 
最后我们成功编译好了main，执行一下，报错：
引用
|`1`|`./main: error ``while``loading shared libraries: libhello.so: cannot ``open``shared object ``file````: No such````file``or directory`|
找不到libhello.so这个共享库，怎么回事？这是因为libhello.so并不在操作系统默认的共享库的路径下面，我们可以临时指定一下链接路径： 
C代码 
|`1`|`export``LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH`|
这样就成功了。我们用ldd main看一下：
C代码 
|`1`|`libhello.so => ./libhello.so (0x0000002a9566d000)`|
|`2`|`libc.so.6 => /lib64/tls/libc.so.6 (0x0000002a9576e000)`|
|`3`|`/lib64/ld-linux-x86-64.so.2 (0x0000002a95556000)`|
这次main程序链接到了libhello.so这个共享库上面。 
## 三、关于Linux的动态共享库的设置
可执行程序找不到要链接的动态共享库，这是Linux上面编译和运行程序很容易碰到的问题，通过上面的小例子，我们已经大致了解共享库的一点基本原理，接下来我们要探讨一下怎么设置程序寻找动态共享库的行为。
Linux操作系统上面的动态共享库大致分为三类： 
### 1、操作系统级别的共享库和基础的系统工具库
比方说libc.so, libz.so, libpthread.so等等，这些系统库会被放在/lib和/usr/lib目录下面，如果是64位操作系统，还会有/lib64和/usr /lib64目录。如果操作系统带有图形界面，那么还会有/usr/X11R6/lib目录，如果是64位操作系统，还有/usr/X11R6 /lib64目录。此外还可能有其他特定Linux版本的系统库目录。
这些系统库文件的完整和版本的正确，确保了Linux上面各种程序能够正常的运行。 
### 2、应用程序级别的系统共享库
并非操作系统自带，但是可能被很多应用程序所共享的库，一般会被放在/usr/local/lib和/usr/local/lib64这两个目录下面。很多你自行编译安装的程序都会在编译的时候自动把/usr/local/lib加入gcc的-L参数，而在运行的时候自动到/usr/local /lib下面去寻找共享库。
以上两类的动态共享库，应用程序会自动寻找到他们，并不需要你额外的设置和担心。这是为什么呢？因为以上这些目录默认就被加入到动态链接程序的搜索路径里面了。Linux的系统共享库搜索路径定义在/etc/ld.so.conf这个配置文件里面。这个文件的内容格式大致如下：
C代码 
|`1`|`/usr/X11R6/lib64`|
|`2`|`/usr/X11R6/lib`|
|`3`|`/usr/``local``/lib`|
|`4`|`/lib64`|
|`5`|`/lib`|
|`6`|`/usr/lib64`|
|`7`|`/usr/lib`|
|`8`|`/usr/``local``/lib64`|
|`9`|`/usr/``local``/ImageMagick/lib`|
假设我们自己编译安装的ImageMagick图形库在/usr/local/ImageMagick目录下面，并且希望其他应用程序都可以使用 ImageMagick的动态共享库，那么我们只需要把/usr/local/ImageMagick/lib目录加入/etc/ld.so.conf文件里面，然后执行：ldconfig 命令即可。
ldcofig将搜索以上所有的目录，为共享库建立一个缓存文件/etc/ld.so.cache。为了确认ldconfig已经搜索到ImageMagick的库，我们可以用上面介绍的strings命令从ld.so.cache里面抽取文本信息来检查一下：
C代码 
|`1`|`strings /etc/ld.so.cache | ``grep``ImageMagick`|
输出结果为：
C代码 
|`1`|`/usr/``local``/ImageMagick/lib/libWand.so.10`|
|`2`|`/usr/``local``/ImageMagick/lib/libWand.so`|
|`3`|`/usr/``local``/ImageMagick/lib/libMagick.so.10`|
|`4`|`/usr/``local``/ImageMagick/lib/libMagick.so`|
|`5`|`/usr/``local``/ImageMagick/lib/libMagick++.so.10`|
|`6`|`/usr/``local``/ImageMagick/lib/libMagick++.so`|
已经成功了！ 
### 3、应用程序独享的动态共享库
有很多共享库只被特定的应用程序使用，那么就没有必要加入系统库路径，以免应用程序的共享库之间发生版本冲突。因此Linux还可以通过设置环境变量LD_LIBRARY_PATH来临时指定应用程序的共享库搜索路径，就像我们上面举的那个例子一样，我们可以在应用程序的启动脚本里面预先设置 LD_LIBRARY_PATH，指定本应用程序附加的共享库搜索路径，从而让应用程序找到它。
