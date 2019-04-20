# sqlite在arm-linux下的编译 - h13 - 博客园
开发环境：ubuntu10.04  tq2440 
第一步 sqlite在arm-linux下的编译
1、 下载sqlite2.8.17：请到http://www.sqlite.org/download.html，将下载的代码包解开，将生成sqlite目录，另外新建一个build目录，如sqlite-arm,再在此目录下建build目录，sqlite-arm应该是和sqlite-2.8.17目录平行的同级目录
注：下载的是sqlite-2.8.17.tar.gz 
2、确认arm-linux-toolchains.tgz 已经安装到你的linux系统.
3、为了在arm-linux下能正常运行sqlite，我们需要修改一处代码，否则在arm板上运行sqlite时会出现下面的东东：
===============================
在文件btree.c中抛出断言，
assert( sizeof(ptr)==sizeof(char*) );
===============================
此断言是为了保证btree（B树）有正确的变量大小，如“ptr”和“char*”。 在不同的体系结构的linux，如x86和arm，会有些差别。刚好让我们在arm-linux下遇到了:-)。那么我们可以做一定的修改。
请修改sqlite/src/sqliteInt.h，找到如下部分：
#ifndef INTPTR_TYPE
# if SQLITE_PTR_SZ==4
# define INTPTR_TYPE int
# else
# define INTPTR_TYPE long long
# endif
在上面的代码前加上一句：
#define SQLITE_PTR_SZ 4
这样后面的“typedef INTPTR_TYPE ptr;”就是定义的“int”类型，而不是“long long”。 
4、修改configure 
注释掉如下几行 
19153行-19157行 
#  if test "$cross_compiling" = "yes"; then 
#    { { echo "$as_me:$LINENO: error: unable to find a compiler for building build tools" >&5 
#echo "$as_me: error: unable to find a compiler for building build tools" >&2;} 
#   { (exit 1); exit 1; }; } 
#  fi 
19989行-19995行 
#if eval "test \"\${$as_ac_File+set}\" = set"; then 
#  echo $ECHO_N "(cached) $ECHO_C" >&6 
#else 
#  test "$cross_compiling" = yes && 
#  { { echo "$as_me:$LINENO: error: cannot check for file existence when cross compiling" >&5 
#echo "$as_me: error: cannot check for file existence when cross compiling" >&2;} 
#   { (exit 1); exit 1; }; } 
20001行 
#fi 
20392行-20398行 
#if eval "test \"\${$as_ac_File+set}\" = set"; then 
#  echo $ECHO_N "(cached) $ECHO_C" >&6 
#else 
#  test "$cross_compiling" = yes && 
#  { { echo "$as_me:$LINENO: error: cannot check for file existence when cross compiling" >&5 
#echo "$as_me: error: cannot check for file existence when cross compiling" >&2;} 
#   { (exit 1); exit 1; }; } 
20404行 
#fi 
20418-20424行 
#if eval "test \"\${$as_ac_File+set}\" = set"; then 
#  echo $ECHO_N "(cached) $ECHO_C" >&6 
#else 
#  test "$cross_compiling" = yes && 
#  { { echo "$as_me:$LINENO: error: cannot check for file existence when cross compiling" >&5 
#echo "$as_me: error: cannot check for file existence when cross compiling" >&2;} 
#   { (exit 1); exit 1; }; } 
20430行 
#fi 
如果编译pc版本：
5. 注释掉后，就可以执行configure了。在sql-arm目录下，输入如下命令：
../sqlite/configure --host=arm-linux
这样在你的build目录中就将生成Makefile和一个libtool脚本，这些将在make时用到。 
6、make , make install;
如果编译arm版本：
5、注释掉后，就可以执行configure了。在sql-arm目录下，输入如下命令：
../sqlite/configure --host=arm-linux –prefix=/home/tool/sqlite-arm/build
这样在你的build目录中就将生成Makefile和一个libtool脚本，这些将在make时用到。 
6、修改Makefile文件
请修改Makefile文件，将下面的这行
BCC = arm-linux-gcc -g -O2
改掉，改成：
BCC = gcc -g -O2
一般地，我们都是将sqlite放到arm-linux的硬件板子上运行，所以我们一般将其编译成静态链接的形式。如果是共享so库的话，比较麻烦。
所以继续修改Makefile，找到如下地方：
sqlite:
将有其后的“libsqlite.la”改成
“.libs/libsqlite.a”
7、make  make install;
8、将生成在build下面的文件拷到交叉编译器目录下面
