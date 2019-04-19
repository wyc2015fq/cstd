# 编译安装glibc - xqhrs232的专栏 - CSDN博客
2017年02月21日 11:33:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：975
原文地址::[http://blog.csdn.net/ldl22847/article/details/18702645](http://blog.csdn.net/ldl22847/article/details/18702645)
相关文章
1、[编译安装glibc](http://blog.csdn.net/u012927281/article/details/51704855)----[http://blog.csdn.net/u012927281/article/details/51704855](http://blog.csdn.net/u012927281/article/details/51704855)
2、[glibc的编译和调试](http://blog.chinaunix.net/uid-20786208-id-4980168.html) ----[http://blog.chinaunix.net/uid-20786208-id-4980168.html](http://blog.chinaunix.net/uid-20786208-id-4980168.html)
3、 glibc下载地址----[http://ftp.gnu.org/gnu/glibc/](http://ftp.gnu.org/gnu/glibc/)
4、各个版本linux下载地址----[http://blog.chinaunix.net/uid-18905703-id-3767700.html](http://blog.chinaunix.net/uid-18905703-id-3767700.html)
5、glibc的安装编译 --完整处理问题版----[https://my.oschina.net/startphp/blog/161825](https://my.oschina.net/startphp/blog/161825)
6、[使用指定glibc编译程序](http://blog.csdn.net/jefbai/article/details/47859335)----[http://blog.csdn.net/jefbai/article/details/47859335](http://blog.csdn.net/jefbai/article/details/47859335)
首先从gnu官网下载最新版的glibc，地址[http://www.gnu.org/software/libc/](http://www.gnu.org/software/libc/)
编译步骤：
下载glibc-2.15.tar.gz和补丁包glibc-ports-2.15.tar.gz
解压
$mv glibc-ports-2.15 glibc-2.15/ports
$mkdir glibc-build-2.15 &&cd glibc-build-2.15
$ ../glibc-2.15/configure \
    --prefix=/usr/local/glibc_mips \
    CC=mipsel-[Linux](http://lib.csdn.net/base/linux)-gcc \
    --host=mipsel-linux \
    --build=i686-pc-linux-gnu \
    --enable-add-on=nptl \
    libc_cv_forced_unwind=yes \  
    libc_cv_c_cleanup=yes \
    libc_cv_mips_tls=yes \
    libc_cv_gnu99_inline=yes 
ok,没问题
$make &&make install
大功告成
##########################################################################
下面是我编译时的过程和遇到的问题及解决：
##########################################################################
$tar xvf glibc-2.16.0.tar.bz2
$cd glibc-2.16.0
$./configure --prefix=/usr/local/glibc   //先不加其他选项，除了安装路径，一切默认，网上一般配置arm的选项如下 --prefix=$HOME/usr/arm --with-headers=$HOME/usr/arm/glibc/arm-linux-glibc/include --with-libs=$HOME/usr/arm/glibc/arm-linux-glibc/lib 
报错：
configure: error: you must configure in a separate build directory
很奇怪的问题，必须配置一个构建目录，刚开始以为是安装目录为创建
$mkdir /usr/local/glibc
问题仍然存在，百度之
$mkdir ../glibc-build && cd ../glibc-build
$../glibc-2.16.0/configure --prefix=/usr/local/glibc
出现新的问题：
configure: WARNING:
*** These auxiliary programs are missing or incompatible versions: msgfmt
*** some features will be disabled.
*** Check the INSTALL file for required versions.
checking LD_LIBRARY_PATH variable... contains current directory
configure: error:
*** LD_LIBRARY_PATH shouldn't contain the current directory when
*** building glibc. Please change the environment variable
*** and run configure again.
第一个警告不用管它，第二个LD_LIBRARY_PATY也会有错？我的这个路径用了多少天了。仔细看提示，不应包含当前路径。打开~/.bash_profile
$cat ~/.bash_profile
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib export LD_LIBRARY_PATH
这也没当前路径啊。还是百度吧。
一个兄弟的解释是这样“LD_LIBRARY_PATH不能以终结符作为开始和最后一个字符，不能有2个终结符连在一起，我的LD_LIBRARY_PATH为 :/usr/local/firefox:/usr/local/firefox,只要在前面加上一个路径，不让：出现在第一个字符就可以了 ”
原来如此，第一个字符不能是":"，修改~/.bash_profile
export LD_LIBRARY_PATH=/usr/local/lib export LD_LIBRARY_PATH 
$../glibc-2.16.0/configure --prefix=/usr/local/glibc
ls一下，发现，当前目录生成了Makefile等一堆东西
$make && make install
没问题
下一步开始交叉编译
$mkdir ../glibc-build-mips && cd ../glibc-build-mips
$ ../glibc-2.16.0/configure --prefix=/usr/local/glibc_mips CC=mipsel-linux-gcc --host=mips
出现新的问题：
configure: running configure fragment for add-on libidn
configure: running configure fragment for add-on nptl
*** The GNU C library is currently not available for this platform.
*** So far nobody cared to port it and if there is no volunteer it
*** might never happen.  So, if you have interest to see glibc on
*** this platform visit
***     http://www.gnu.org/software/libc/porting.html
*** and join the group of porters
看起来像是需要path，下载glibc-ports-2.16.tar.gz，放在源码包目录，解压
$ ../glibc-2.16.0/configure \
    --prefix=/usr/local/glibc_mips \
    CC=mipsel-linux-gcc \
    CXX=mipsel-linux-g++ \
    --host=mips \
    --enable-add-ons=/home/hb/code/glibc/glibc-ports-2.16.0/sysdeps/mips
仍然报错：
configure: error: fragment must set $libc_add_on_canonical
改为：
$ ../glibc-2.16.0/configure \
    --prefix=/usr/local/glibc_mips \
    CC=mipsel-linux-gcc \
    CXX=mipsel-linux-g++ \
    --host=mips \
    --enable-add-ons
报错：
configure: error: The mipsel is not supported.
这样不行，谷歌半天，总算知道补丁怎么用的了。把补丁目录拷到glibc目录下，改名为ports
$mv glibc-ports-2.16.0/ glibc-2.16.0/ports
$../glibc-2.16.0/configure \
    --prefix=/usr/local/glibc_mips \
    CC=mipsel-linux-gcc \
    CXX=mipsel-linux-g++ \
    --host=mipsel-linux \
    --build=i686-pc-linux-gnu \
    --enable-add-on
继续报错：
configure: error: 
*** These critical programs are missing or too old: ld as
*** Check the INSTALL file for required versions.
这个问题可折腾死我了。弄了好半天，就是不行，最后google发现，原来是ld和as版本不对，不是太高就是太低。
configure中找到$AS --version
发现版本是这么匹配的2.1*.*
$mipsel-linux-ld
GNU ld (GNU Binutils) 2.18.50.20080908
原来是这样，在configure版本号那一行修改，最后的括号前面加入
|2.18.50.×
as那一行也同样修改
然后
$make
开始编译，看起来不错
好半天后，编译也报错了
In file included from ../include/uchar.h:1,
                 from mbrtoc16.c:23:
../wcsmbs/uchar.h:47:5: error: #error "<uchar.h> requires ISO C11 mode"
In file included from ../include/uchar.h:1,
                 from mbrtoc16.c:23:
../wcsmbs/uchar.h:52: error: expected '=', ',', ';', 'asm' or '__attribute__' before 'char16_t'
../wcsmbs/uchar.h:53: error: expected '=', ',', ';', 'asm' or '__attribute__' before 'char32_t'
../wcsmbs/uchar.h:61: error: expected ')' before '*' token
../wcsmbs/uchar.h:66: error: expected declaration specifiers or '...' before 'char16_t'
../wcsmbs/uchar.h:73: error: expected ')' before '*' token
../wcsmbs/uchar.h:78: error: expected declaration specifiers or '...' before 'char32_t'
mbrtoc16.c:37: error: expected ')' before '*' token
make[2]: *** [/home/hb/code/glibc/glibc-build-mips/wcsmbs/mbrtoc16.o] 错误 1
make[2]:正在离开目录 `/home/hb/code/glibc/glibc-2.16.0/wcsmbs'
make[1]: *** [wcsmbs/subdir_lib] 错误 2
make[1]:正在离开目录 `/home/hb/code/glibc/glibc-2.16.0'
make: *** [all] 错误 2
看看这个头文件咋回事
$ vim ../glibc-2.16.0/wcsmbs/uchar.h
#if defined __GNUC__ && !defined __USE_ISOCXX11
/* Define the 16-bit and 32-bit character types.  Use the information
   provided by the compiler.  */
# if !defined __CHAR16_TYPE__ || !defined __CHAR32_TYPE__
#  if defined __STDC_VERSION__ && __STDC_VERSION__ < 201000L
#   error "<uchar.h> requires ISO C11 mode"
#  else
#   error "definitions of __CHAR16_TYPE__ and/or __CHAR32_TYPE__ missing"
#  endif
# endif
明白了，原来是需要c11支持，mipsel-linux-gcc -v一下，我的支持c99.原来如此。暂时没招了，我还做不到修改c11的支持，只剩两个办法，不用这个glibc版本或者重新编译一个支持c11的交叉编译器。编译器需要做的比较多，暂时先换个低点的版本吧。
下载galibc-2.15版本
重复上面步骤，解压tar包
解压ports包
$mv glibc-ports-2.15 glibc-2.15/ports
$mkdir glibc-build-2.15 &&cd glibc-build-2.15
$ ../glibc-2.15/configure \
    --prefix=/usr/local/glibc_mips \
    CC=mipsel-linux-gcc \
    --host=mipsel-linux \
    --build=i686-pc-linux-gnu \
    --enable-add-on=nptl \
    libc_cv_forced_unwind=yes \  
    libc_cv_c_cleanup=yes \
    libc_cv_mips_tls=yes \
    libc_cv_gnu99_inline=yes 
ok,没问题
$make &&make install
库已经编好了，但是不能直接使用，必须再用新的库重编一遍编译器才行。
