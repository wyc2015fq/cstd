# automake使用 - V__KING__的专栏 - CSDN博客





2019年03月15日 01:43:39[v__king__](https://me.csdn.net/V__KING__)阅读数：21








参考：[https://www.ibm.com/developerworks/cn/linux/l-makefile/](https://www.ibm.com/developerworks/cn/linux/l-makefile/)

参考automake Example：[https://download.csdn.net/download/v__king__/11021046](https://download.csdn.net/download/v__king__/11021046)

## 命令顺序

autoscan; aclocal; autoconf; (autoheader); automake; ./configure --host=arm-linux; make;

**make dist**：打包，打包的命为 test2-1.0.tar.gz，保护packge名字和version。configure.in中定义。

**make distclean**：

## 需要编辑的文件

```
configure.in
Makefile.am
```

## 添加库

### 编译静态库

下面我们来说下编译静态库和编译动态库，我们说下静态库，下面这个例子比较简单。直接指定 XXXX_LTLIBRARIES或者XXXX_LIBRARIES就可以了。同样如果不需要安装到系统，将XXXX换成noinst就可以。

一般推荐使用libtool库编译目标，因为automake包含libtool，这对于跨平台可移植的库来说，是一个很好的事情。

看例子如下：

```
noinst_LTLIBRARIES = libutil.la

oinst_HEADERS = inaddr.h util.h compat.h pool.h xhash.h url.h device.h

ibutil_la_SOURCES = access.c config.c datetime.c hex.c inaddr.c log.c device.c pool.c rate.c sha1.c stanza.c str.c xhash.c

ibutil_la_LIBADD = @LDFLAGS@
```

第一行的noinst_LTLIBRARIES，这里要注意的是LTLIBRARIES，另外还有LIBRARIES，两个都表示库文件。前者表示libtool库，用法上基本是一样的。如果需要安装到系统中的话，用lib_LTLIBRARIES。

.la为libtool自动生成的一些共享库，vi编辑查看，主要记录了一些配置信息。可以用如下命令查看*.la文件的格式   $file *.la

.a为静态库,是好多个.o合在一起,用于静态连接

如果想编译 .a 文件，那么上面的配置就改成如下结果：

```
noinst_LTLIBRARIES = libutil.a

noinst_HEADERS = inaddr.h util.h compat.h pool.h xhash.h url.h device.h

ibutil_a_SOURCES = access.c config.c datetime.c hex.c inaddr.c log.c device.c pool.c rate.c sha1.c stanza.c str.c xhash.c

ibutil_a_LIBADD = @LDFLAGS@
```

注意：静态库编译连接时需要其它的库的话，采用XXXX_LIBADD选项，而不是前面的XXXX_LDADD。编译静态库是比较简单的，因为直接可以指定其类型。

### 编译动态库

如果想要编译XXX.so动态库文件，需要用到_PROGRAMS类型，有一个关于安装路径的问题，如果希望将动态库安装到lib目录下，按照前面所说的，只需要写成lib_PROGRAMS就可以了，lib表示安装的路径，但是automake不允许这样直接定义，所以可以采用下面的办法，同样是将动态库安装到lib目录下：

```
projectlibdir=$(libdir)//新建一个目录，就是该目录就是lib目录

projectlib_PROGRAMS=project.so

project_so_SOURCES=xxx.C

project_so_LDFLAGS=-shared -fpic//GCC编译动态库的选项
```

## SUBDIRS功能用法

SUBDIRS这是一个很重要的词，我们前面生成了一个目标文件，但是一个大型的工程项目是由许多个可执行文件和库文件组成，也就是包含多个目录，每个目录下都有用于生成该目录下的目标文件的Makefile.am文件，但顶层目录是如何调用，才能使下面各个目录分别生成自己的目标文件呢？就是SUBDIRS关键词的用法了。

看一下我的工程项目，这是顶层的Makefile.am文件

```
EXTRA_DIST = Doxyfile.in README.win32 README.protocol contrib UPGRADE

devicedir = ${prefix}/device

device_DATA = package

SUBDIRS = etc man

ifUSE_LIBSUBST

SUBDIRS += subst

endif

SUBDIRS += tools io sessions util client dispatch server hash storage sms
```

SUBDIRS表示在处理目录之前，要递归处理哪些子目录，要注意处理的顺序。比如配置中的client对sessions和utils这两上目标文件有依赖关系，就在client之前需要处理这两个目标文件。

## 如何交叉编译

./configure --host=arm-linux LDFLAGS="-lpthread -lrt"

## c中宏定义怎么加到automake中？
- CFLAGS = -DREALESE
- LDFLAGS = -DREALESE
- DEFS += -DREALESE    #注意+=符号
- 在执行./configure的时候将makefile参数重定义，如：./configure LDFLAGS="-lpthread -lrt"

## configure.in说明

参考：[https://www.cnblogs.com/silvermagic/p/7665868.html](https://www.cnblogs.com/silvermagic/p/7665868.html)

宏：AM_INIT_AUTOMAKE(test,1.0)，一定要有。

宏：AC_CONFIG_HEADER(config.h)可以去掉。

宏： AC_OUTPUT_COMMANDS (extra-cmds [, init-cmds])

在configure的过程中执行该命令。

指定在`config.status’末尾运行的附加的shell命令，以及用于初始化来自于configure 的所有变量的shell命令。本宏可以被调用多次。
宏：AC_OUTPUT

```
AC_OUTPUT([Makefile
        src/lib/Makefile
        src/ModuleA/apple/core/Makefile
        src/ModuleA/apple/shell/Makefile
        ])
```

## 多级目录工程构建

参考：[https://blog.csdn.net/rayyt/article/details/78876097?utm_source=blogxgwz8](https://blog.csdn.net/rayyt/article/details/78876097?utm_source=blogxgwz8)

工程目录结构如下：

├── include

│   └── module1.h

├── module1

│   ├── module1.c

│   └── module1.h

└── main.c

### 1. 运行命令：autoscan; cp configure.scan [configure.in](http://configure.in)
- [修改configure.in](http://xn--configure-z89nz78p.in)

```
AC_PREREQ([2.68])
AC_INIT([main], [1.0], [vk@qq.com])
AC_CONFIG_SRCDIR([main.c])
#AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(main, 1.0)

# Checks for programs.
AC_PROG_CC
#!!!多级目录的时候这个AC_PROG_RANLIB一定得加上
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

#!!!这里也可以将makefile移到AC_OUTPUT中(autoscan的时候生成的)
#AC_CONFIG_FILES(Makefile module1/Makefile)

# Checks for library functions.

AC_OUTPUT(Makefile module1/Makefile)
```

### 2. 编辑Makefile.am和module1/Makefile.am

Makefile.am如下

```
AUTOMAKE_OPTIONS=foreign
SUBDIRS=module1
bin_PROGRAMS=main
main_SOURCES=main.c
main_LDADD=module1/libmodule1.a

##################关于打包发布############
# Install my_script in $(bindir) and distribute it.
# dist_bin_SCRIPTS = my_script
# 上面相当于等于如下，没有dist说明是install的时候到prefix目录下
bin_SCRIPTS=my_script
EXTRA_DIST=my_script module1/libmodule1.a lib/libxml2.so

# make install的时候安装配置文件，@prefix@是./configure --prefix=path中的安装路径path
cfgdir=@prefix@/cfg
cfg_DATA=main.cfg

# clean自定义
CLEANFILES = main.o 

# 伪目标
move:
	cp $(bin_PROGRAMS) /path/nfs_dir
```

module1/Makefile.am

```
AUTOMAKE_OPTIONS=foreign
lib_LIBRARIES=libmodule1.a
libmodule1_a_SOURCES=module1.c module1.h
```

### 2. aclocal; autoconf

会得到一个aclocal.m4文件 和 configure文件

### 3. automake --add-missing

### 4. 编辑一个autogen.sh脚本来执行上面操作

```
autoscan
aclocal
automake --add-missing
./configure --prefix=/path/output
make
make dist
```

### automake打包发布

在Makefile.am中打包额外文件，如配置文件，脚本，so库，额外的lib/libxx.a库

```
# Install my_script in $(bindir) and distribute it.
# dist_bin_SCRIPTS = my_script
# 上面相当于等于如下，没有dist说明是install的时候到prefix目录下
bin_SCRIPTS=my_script
EXTRA_DIST=my_script path/file1 path/file2

# make install的时候安装配置文件
cfgdir=@prefix@/cfg
cfg_DATA=main.cfg

# clean自定义
CLEANFILES = main.o
```

### automake添加一个debug或者release选项

参考：[https://blog.csdn.net/liaoxinmeng/article/details/5468045](https://blog.csdn.net/liaoxinmeng/article/details/5468045)
- [在configure.in](http://xn--configure-kd5p.in)(或 [configure.ac](http://configure.ac))中添加：

```
#debug options support
AC_ARG_ENABLE([release],
    [AS_HELP_STRING([--enable-release],[release program(default have debug info)])],
    [CFLAGS="${CFLAGS} -DRELEASE -g"],
    [CFLAGS="-O2"])
```
- 用./configure  --enable-**release**
- 知识点：

在configure.in中${CFLAGS} 是目前CFLAGS的值，而CFLAGS是makefile中的变量

用双引号
### clean自定义

CLEANFILES = $(FILE_TO_RM)





