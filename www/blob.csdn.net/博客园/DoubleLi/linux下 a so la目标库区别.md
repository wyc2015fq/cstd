# linux下.a/.so/.la目标库区别 - DoubleLi - 博客园






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
**参考资料：**

[http://wiki.ubuntu.org.cn/index.php?title=Compiling_C&variant=zh-hans](http://wiki.ubuntu.org.cn/index.php?title=Compiling_C&variant=zh-hans)

[http://afreez.blog.51cto.com/59057/7338](http://afreez.blog.51cto.com/59057/7338)

[http://blog.csdn.net/lhf_tiger/article/details/7065289](http://blog.csdn.net/lhf_tiger/article/details/7065289)

[http://topic.csdn.net/u/20080124/13/c34ece2c-81a0-4818-bec4-d705aa4aecb1.html](http://topic.csdn.net/u/20080124/13/c34ece2c-81a0-4818-bec4-d705aa4aecb1.html)









