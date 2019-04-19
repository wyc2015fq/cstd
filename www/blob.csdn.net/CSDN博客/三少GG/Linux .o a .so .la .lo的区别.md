# Linux .o a .so .la .lo的区别 - 三少GG - CSDN博客
2014年08月19日 14:23:56[三少GG](https://me.csdn.net/scut1135)阅读数：688
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)

o: 编译的目标文件
a: 静态库，其实就是把若干o文件打了个包
so: 动态链接库（共享库）
lo: 使用libtool编译出的目标文件，其实就是在o文件中添加了一些信息
la: 使用libtool编译出的库文件，其实是个文本文件，记录同名动态库和静态库的相关信息
补充：libtool
1 libtool的工作原理 
libtool 是一个通用库支持脚本，将使用动态库的复杂性隐藏在统一、可移植的接口中；使用libtool的标准方法，可以在不同平台上创建并调用动态库。可以认为libtool是gcc的一个抽象，其包装了gcc（或者其他的编译器），用户无需知道细节，只要告诉libtool需要编译哪些库即可，libtool将处理库的依赖等细节。libtool只与后缀名为lo、la为的libtool文件打交道。 
libtool主要的一个作用是在编译大型软件的过程中解决了库的依赖问题；将繁重的库依赖关系的维护工作承担下来，从而释放了程序员的人力资源。libtool提供统一的接口，隐藏了不同平台间库的名称的差异等细节，生成一个抽象的后缀名为la高层库libxx.la（其实是个文本文件），并将该库对其它库的依赖关系，都写在该la的文件中。该文件中的dependency_libs记录该库依赖的所有库（其中有些是以.la文件的形式加入的）；libdir则指出了库的安装位置；library_names记录了共享库的名字；old_library记录了静态库的名字。 
当编译过程到link阶段的时候，如果有下面的命令： 
$libtool --mode=link gcc -o myprog -rpath /usr/lib –L/usr/lib –la 
libtool会到/usr/lib路径下去寻找liba.la，然后从中读取实际的共享库的名字（library_names中记录了该名字，比如liba.so）和路径(lib_dir中记录了，比如libdir=’/usr/lib’)，返回诸如/usr/lib/liba.so的参数给激发出的gcc命令行。 
如果liba.so依赖于库/usr/lib/libb.so，则在liba.la中将会有dependency_libs=’-L/usr/lib -lb’或者dependency_libs=’/usr/lib/libb.la’的行，如果是前者，其将直接把“-L/usr/lib –lb”当作参数传给gcc命令行；如果是后者，libtool将从/usr/lib/libb.la中读取实际的libb.so的库名称和路径，然后组合成参数“/usr/lib/libb.so”传递给gcc命令行。 
当要生成的文件是诸如libmylib.la的时候，比如： 
$libtool --mode=link gcc -o libmylib.la -rpath /usr/lib –L/usr/lib –la 
其依赖的库的搜索基本类似，只是在这个时候会根据相应的规则生成相应的共享库和静态库。 
注意：libtool在链接的时候只会涉及到后缀名为la的libtool文件；实际的库文件名称和库安装路径以及依赖关系是从该文件中读取的。 
2 为何使用 -Wl,--rpath-link -Wl,DIR？ 
使用libtool解决编译问题看上去没什么问题：库的名称、路径、依赖都得到了很好的解决。但下结论不要那么着急，一个显而易见的问题就是：并不是所有的库都是用libtool编译的。 
比如上面那个例子， 
$libtool --mode=link gcc -o myprog -rpath /usr/lib –L/usr/lib –la 
如果liba.so不是使用libtool工具生成的，则libtool此时根本找不到liba.la文件（不存在该文件）。这种情况下，libtool只会把“–L/usr/lib –la”当作参数传递给gcc命令行。 
考虑以下情况：要从myprog.o文件编译生成myprog，其依赖于库liba.so（使用libtool生成），liba.so又依赖于libb.so（libb.so的生成不使用libtool），而且由于某种原因，a对b的依赖并没有写入到liba.la中，那么如果用以下命令编译： 
$libtool --mode=link gcc -o myprog -rpath /usr/lib –L/usr/lib –la 
激发出的gcc命令行类似于下面： 
              gcc –o myprog /usr/lib/liba.so 
由于liba.so依赖于libb.so（这种依赖可以用readelf读liba.so的ELF文件看到），而上面的命令行中，并没有出现libb.so，于是，可能会出现问题。 
      说“可能”，是因为如果在本地编译的情况下，gcc在命令行中找不到一个库（比如上面的liba.so）依赖的其它库（比如libb.so），链接器会按照某种策略到某些路径下面去寻找需要的共享库： 
1. 所有由'-rpath-link'选项指定的搜索路径. 
2. 所有由'-rpath'指定的搜索路径. '-rpath'跟'-rpath_link'的不同之处在于,由'-rpath'指定的路径被包含在可执行文件中,并在运行时使用, 而'-rpath-link'选项仅仅在连接时起作用. 
3. 在一个ELF系统中, 如果'-rpath'和'rpath-link'选项没有被使用, 会搜索环境变量'LD_RUN_PATH'的内容.它也只对本地连接器起作用. 
4. 在SunOS上, '-rpath'选项不使用, 只搜索所有由'-L'指定的目录. 
5. 对于一个本地连接器,环境变量'LD_LIBRARY_PATH'的内容被搜索. 
6. 对于一个本地ELF连接器,共享库中的`DT_RUNPATH'和`DT_RPATH'操作符会被需要它的共享库搜索. 如果'DT_RUNPATH'存在了, 那'DT_RPATH'就会被忽略. 
7. 缺省目录, 常规的,如'/lib'和'/usr/lib'. 
8. 对于ELF系统上的本地连接器, 如果文件'/etc/ld.so.conf'存在, 这个文件中有的目录会被搜索. 
从以上可以看出，在使用本地工具链进行本地编译情况下，只要库存在于某个位置，gcc总能通过如上策略找到需要的共享库。但在交叉编译下，上述八种策略，可以使用的仅仅有两个：-rpath-link，-rpath。这两个选项在上述八种策略当中优先级最高，当指定这两个选项时，如果链接需要的共享库找不到，链接器会优先到这两个选项指定的路径下去搜索需要的共享库。通过上面的描述可以看到：-rpath指定的路径将被写到可执行文件中；-rpath-link则不会；我们当然不希望交叉编译情况下使用的路径信息被写进最终的可执行文件，所以我们选择使用选项-rpath-link。 
      gcc的选项“-Wl,--rpath-link –Wl,DIR”会把-rpath-link选项及路径信息传递给链接器。回到上面那个例子，如果命令行中没有出现libb.so，但gcc指定了“-Wl,--rpath-link –Wl,DIR”，则链接器找不到libb.so的时候，会首先到后面-rpath-link指定的路径去寻找其依赖的库。此处我们使用的编译命令的示例是使用unicore平台的工具链。 
$ unicore32-linux-gcc –o myprog /usr/lib/liba.so \ 
-Wl,--rpath-link -Wl,/home/UNITY_float/install/usr/lib 
这样，编译器会首先到“/home/UNITY_float/install/usr/lib”下面去搜索libb.so 
      libtool如何把选项“-Wl,--rpath-link –Wl,DIR”传递给gcc？libtool中有一个变量“hardcode_libdir_flag_spec”，该变量本来是传递“-rpath”选项的，但我们可以修改它，添加我们需要的路径，传递给unicore32-linux-gcc。 
      “hardcode_libdir_flag_spec”原来的定义如下： 
hardcode_libdir_flag_spec="\${wl}--rpath \${wl}\$libdir" 
我们修改后的定义如下： 
hardcode_libdir_flag_spec="\${wl}—rpath-link \${wl}\$libdir \ 
-Wl,--rpath-link -Wl,/home/UNITY_float/install/usr/lib \ 
-Wl,--rpath-link -Wl,/home/UNITY_float/install/usr/X11R6/lib " 
这样，当libtool在“--mode=link”的模式下，就会把选项“-Wl,--rpath-link –Wl,DIR”传递给gcc编译器了。
在linux平台上编译时，常会遇到目标库的疑问，有静态库也有动态库，单个理解都不太难，但是对复杂的工程而言，一旦混合到一起去，对整个工程的理解和调用，将会造成很大困扰，本文就汇总这几种常见编译结果文件的区别。
## 一、格式说明
linux下编译，常会遇到后缀为：.o .so .a .la .ko等格式文件，尽管linux并不以扩展名作为识别文件格式的唯一依据，但规范约定还是有的，如下：
- .o 是目标对象文件,相当于windows中的.obj文件
- .a 为静态库，可以是一个或多个.o合在一起,用于静态连接
- .la 为libtool生成的共享库，其实是个配置文档。可以用$file *.la查看*.la文件，或用vi来查看。
- .so 为共享库，类似windows平台的dll文件
补充： 还有一种扩展名为.ko 文件，不过它是Linux内核使用的动态链接文件后缀，属于模块文件，用来在Linux系统启动时加载内核模块。
## 二、创建实例
### 1、创建.o对象文件
$ gcc -c test.c
生成test.o，跳过链接对象，所以不是可执行文件。
### 2、创建.a静态库文件
$ ar -r libtest.a test1.o test2.o
### 3、创建动态库.so
$ gcc -Wall -fpic -shared test1.c test2.c -o libtest.so
上一句执行，将test1.c和test2.c编译生成动态库文件libtest.so
### 4、链接库文件
$ gcc -Wall -fpic -shared -Ltest test3.c -o libtest.so
编译test3.c后并与静态libtest.a链接（默认会到/usr/lib下找该文件）生成libtest.so动态库。
### 5、生成.la库
.la库一般通过makefile进行，当然也可以通过命令行进行，参考命令：
$libtool --mode=link gcc -o libmylib.la -rpath /usr/lib –L/usr/lib –la
libtool将会搜索libmylib.a文件，并传家libmylib.la。更多libtool帮助如下：
Tst@Tst-PC /cygdrive/d/
$ libtool --help
Usage: libtool [OPTION]... [MODE-ARG]...
Provide generalized library-building support services.
--config show all configuration variables
--debug enable verbose shell tracing
-n, --dry-run display commands without modifying any files
--features display basic configuration information and exit
--mode=MODE use operation mode MODE
--preserve-dup-deps don't remove duplicate dependency libraries
--quiet, --silent don't print informational messages
--no-quiet, --no-silent
print informational messages (default)
--tag=TAG use configuration variables from tag TAG
-v, --verbose print more informational messages than default
--no-verbose don't print the extra informational messages
--version print version information
-h, --help, --help-all print short, long, or detailed help message
MODE must be one of the following:
clean remove files from the build directory
compile compile a source file into a libtool object
execute automatically set library path, then run a program
finish complete the installation of libtool libraries
install install libraries or executables
link create a library or an executable
uninstall remove libraries from an installed directory
MODE-ARGS vary depending on the MODE. When passed as first option,
`--mode=MODE' may be abbreviated as `MODE' or a unique abbreviation of that.
Try `libtool --help --mode=MODE' for a more detailed description of MODE.
When reporting a bug, please describe a test case to reproduce it and
include the following information:
host-triplet: i686-pc-cygwin
shell: /bin/sh
compiler: gcc
compiler flags: -g -O2 -pipe
linker: /usr/i686-pc-cygwin/bin/ld.exe (gnu? yes)
libtool: (GNU libtool) 2.4
automake: automake (GNU automake) 1.11.1
autoconf: autoconf (GNU Autoconf) 2.68
Report bugs to <bug-libtool@gnu.org>.
GNU libtool home page: <http://www.gnu.org/software/libtool/>.
General help using GNU software: <http://www.gnu.org/gethelp/>.
参考资料：
[http://wiki.ubuntu.org.cn/index.php?title=Compiling_C&variant=zh-hans](http://wiki.ubuntu.org.cn/index.php?title=Compiling_C&variant=zh-hans)
[http://afreez.blog.51cto.com/59057/7338](http://afreez.blog.51cto.com/59057/7338)
[http://blog.csdn.net/lhf_tiger/article/details/7065289](http://blog.csdn.net/lhf_tiger/article/details/7065289)
[http://topic.csdn.net/u/20080124/13/c34ece2c-81a0-4818-bec4-d705aa4aecb1.html](http://topic.csdn.net/u/20080124/13/c34ece2c-81a0-4818-bec4-d705aa4aecb1.html)
