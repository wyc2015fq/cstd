# configure 中常见的几个命令 - sxf_123456的博客 - CSDN博客
2017年02月23日 16:40:00[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：473
configure 脚本中有大量的命令行选项，对于不同的软件包来说，这些可能有些变化。
1、configure    --help
输出帮助信息
2、configure    --version
输出产生的configure脚本的Autoconf的版本号
3、configure    --prefix=PATH
PATH 为安装的路径，如/usr/local
--prefix是最常用的选项，制作出的makefile会查看此选项传递的参数，
4、configure   --bindir=DIR
指定二进制文件的安装位置，这里的二进制文件定义为可以被用户直接执行的程序
5、configure   --libexecdir=DIR
指定可执行支持文件的安装位置，与二进制文件相反，这些文件从来不直接由用户执行，可以被上面提到的二进制文件所执行
6、configure   --datadir=DIR
指定通用数据文件的安装位置
7、configure   --sharedstatedir=DIR
指定可以在多个机器上共享的可写数据的安装位置
8、configure   --localstatedir=DIR
指定只能在单机使用的可写数据的安装位置
9、configure  --libdir=DIR
指定库文件的安装位置
10、configure  --includedir=DIR
指定C头文件的安装位置，其他语言如c++的头文件也可以使用此选项
11、configure   --oldincludedir=DIR
指定除GCC外编译器安装的C头文件的安装位置
12、configure   --mandir=DIR
指定手册页的安装位置
13、configure  --build=BUILD
指定软件包安装的系统平台。如果没有指定，默认的值是--host选项的值
14、configure   --disable-FEATURE
禁用一些配置,例如 # ./configure  --disable-gui
15、configure    --enable-FEATURE[=ARG]
启用一些配置。例如 # ./configure  --enable-buffers=123
16、configure   --with-tcl=/usr/local   --with-tk=/usr/local
--with 依赖的安装包路径
17、configure  --without-gnu-ld
使编译器不使用GNU ld
在源码树中运行configure是不必要的同时也是不好的。一个由configure产生的良好的makefile可以构筑源码属于另一棵树的软件包。
在源码树中构建派生文件的好处很明显。派生文件如目标文件，不会和源文件放在一起。
采用构建三棵树的策略：
一棵源码树
一棵构筑树
一棵安装树
例子：
# tar   zxvf   mmalloc-1.0.tar.gz
#mkdir  build  && cd build
#../mmalloc-1.0/configure
creatingcache ./config.cache
checkingfor gcc... gcc
checkingwhether the C compiler (gcc ) works... yes
checkingwhether the C compiler (gcc ) is a cross-compiler... no
checkingwhether we are using GNU C... yes
checkingwhether gcc accepts -g... yes
checkingfor a BSD compatible install... /usr/bin/install -c
checkinghost system type... i586-pc-linux-gnu
checkingbuild system type... i586-pc-linux-gnu
checkingfor ar... ar
checkingfor ranlib... ranlib
checkinghow to run the C preprocessor... gcc -E
checkingfor unistd.h... yes
checkingfor getpagesize... yes
checkingfor working mmap... yes
checkingfor limits.h... yes
checkingfor stddef.h... yes
updatingcache ../config.cache
creating./config.status
这棵构筑树被配置，可以继续构筑和安装这个包到默认的位置/usr/local
#make   all  && make   install
软件包的卸载
很多source的makefile都有写uninstall规则，直接在source里make  uninstall就行。
安装错误解决办法：
#make  uninstall&&make clean
之后重新configure、make、make install操作
