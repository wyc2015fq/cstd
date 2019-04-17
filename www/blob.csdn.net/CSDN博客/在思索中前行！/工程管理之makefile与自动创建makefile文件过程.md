# 工程管理之makefile与自动创建makefile文件过程 - 在思索中前行！ - CSDN博客





2016年03月27日 15:28:13[_Tham](https://me.csdn.net/txl16211)阅读数：2739









（风雪之隅 http://www.laruence.com/2009/11/18/1154.html）

## Linux Makefile自动编译和链接使用的环境




想知道到Linux Makefile系统的真相么，想知道Linux Makefile系统中藏有的内在奥义么，只有我来给大家全面讲解介绍Linux Makefile系统作为Linux下的程序开发人员，大家一定都遇到过Linux Makefile，用make命令来编译自己写的程序确实是很方便。一般情况下，大家都是手工写一个简单Linux Makefile，如果要想写出一个符合自由软件惯例的Linux Makefile就不那么容易了。

在本文中，将给大家介绍如何使用autoconf和automake两个工具来帮助我们自动地生成符合自由软件惯例的Linux Makefile，这样就可以象常见的GNU程序一样，只要使用“./configure”，“make”，“make instal”就可以把程序安装到Linux系统中去了。这将特别适合想做开放源代码软件的程序开发人员，又或如果你只是自己写些小的Toy程序，那么这个文章对你也会有很大的帮助。

一、Linux Makefile介绍

Linux Makefile是用于自动编译和链接的，一个工程有很多文件组成，每一个文件的改变都会导致工程的重新链接，但是不是所有的文件都需要重新编译，Linux Makefile中纪录有文件的信息，在Linux Makefile时会决定在链接的时候需要重新编译哪些文件。

Linux Makefile的宗旨就是：让编译器知道要编译一个文件需要依赖其他的哪些文件。当那些依赖文件有了改变，编译器会自动的发现最终的生成文件已经过时，而重新编译相应的模块。

Linux Makefile的基本结构不是很复杂，但当一个程序开发人员开始写Linux Makefile时，经常会怀疑自己写的是否符合惯例，而且自己写的 Linux Makefile经常和自己的开发环境相关联，当系统环境变量或路径发生了变化后，Linux Makefile可能还要跟着修改。这样就造成了手工书写 Linux Makefile的诸多问题，automake恰好能很好地帮助我们解决这些问题。

使用automake，程序开发人员只需要写一些简单的含有预定义宏的文件，由autoconf根据一个宏文件生成configure，由automake根据另一个宏文件生成Linux Makefile.in，再使用configure依据Linux Makefile.in来生成一个符合惯例的Linux Makefile。下面我们将详细介绍Linux Makefile的automake生成方法。

二、使用的环境

本文所提到的程序是基于Linux发行版本：Fedora Core release 1，它包含了我们要用到的autoconf，automake。

三、从helloworld入手

我们从大家最常使用的例子程序helloworld开始。下面的过程如果简单地说来就是：新建三个文件：
- helloworld.c  
- configure.in  
- Linux Makefile.am 

然后执行：

aclocal; autoconf; automake --add-missing; ./configure; make; ./helloworld

就可以看到Linux Makefile被产生出来，而且可以将helloworld.c编译通过。很简单吧，几条命令就可以做出一个符合惯例的Linux Makefile，感觉如何呀。现在开始介绍详细的过程：

1、建目录

在你的工作目录下建一个helloworld目录，我们用它来存放helloworld程序及相关文件，如在/home/my/build下：

$ mkdir helloword

   $ cd helloworld

2、 helloworld.c

然后用你自己最喜欢的编辑器写一个hellowrold.c文件，如命令：vi helloworld.c。使用下面的代码作为helloworld.c的内容。
- int main(int argc, char** argv)  
- {  
- printf("Hello, Linux World!/n");  
- return 0;  
- }  

完成后保存退出。现在在helloworld目录下就应该有一个你自己写的helloworld.c了。

3、生成configure

我们使用autoscan命令来帮助我们根据目录下的源代码生成一个configure.in的模板文件。

命令：
- $ autoscan  
- $ ls  
- configure.scan helloworld.c  

执行后在hellowrold目录下会生成一个文件：configure.scan，我们可以拿它作为configure.in的蓝本。现在将configure.scan改名为configure.in，并且编辑它，按下面的内容修改，去掉无关的语句：

configure.in内容开始

# -*- Autoconf -*-

# Process this file with autoconf to produce a configure script.

AC_INIT(helloworld.c)

AM_INIT_AUTOMAKE(helloworld, 1.0)

# Checks for programs.

AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT(Linux Makefile)

configure.in内容结束
然后执行命令aclocal和autoconf，分别会产生aclocal.m4及configure两个文件：

- $ aclocal   
- $ls   
- aclocal.m4 configure.in helloworld.c   
- $ autoconf   
- $ ls   
- aclocal.m4 autom4te.cache configure configure.in helloworld.c  



大家可以看到configure.in内容是一些宏定义，这些宏经autoconf处理后会变成检查系统特性、环境变量、软件必须的参数的shell脚本。autoconf 是用来生成自动配置软件源代码脚本（configure）的工具。

configure脚本能独立于autoconf运行，且在运行的过程中，不需要用户的干预。要生成configure文件，你必须告诉autoconf如何找到你所用的宏。方式是使用aclocal程序来生成你的aclocal.m4。aclocal根据configure.in文件的内容，自动生成aclocal.m4文件。

aclocal是一个perl 脚本程序，它的定义是：“aclocal - create aclocal.m4 by scanning configure.ac”。autoconf从configure.in这个列举编译软件时所需要各种参数的模板文件中创建configure。autoconf需要GNU m4宏处理器来处理aclocal.m4，生成configure脚本。

m4是一个宏处理器。将输入拷贝到输出，同时将宏展开。宏可以是内嵌的，也可以是用户定义的。除了可以展开宏，m4还有一些内建的函数，用来引用文件，执行命令，整数运算，文本操作，循环等。m4既可以作为编译器的前端，也可以单独作为一个宏处理器。

4、新建Linux Makefile.am

新建Linux Makefile.am文件，命令：$ vi Linux Makefile.am 内容如下:
- AUTOMAKE_OPTIONS=foreign
- bin_PROGRAMS=helloworld
- helloworldhelloworld_SOURCES=helloworld.c  

automake会根据你写的Linux Makefile.am来自动生成Linux Makefile.in。Linux Makefile.am中定义的宏和目标,会指导automake生成指定的代码。例如，宏bin_PROGRAMS将导致编译和连接的目标被生成。

5、运行automake

命令：
- $ automake --add-missing  
- configure.in: installing `./install-sh'  
- configure.in: installing `./mkinstalldirs'  
- configure.in: installing `./missing'  
- Linux Makefile.am: installing `./depcomp'  

automake会根据Linux Makefile.am文件产生一些文件，包含最重要的Linux Makefile.in。

6、执行configure生成Linux Makefile
- $ ./configure   
- checking for a BSD-compatible install... /usr/bin/install -c  
- checking whether build environment is sane... yes  
- checking for gawk... gawk  
- checking whether make sets $(MAKE)... yes  
- checking for gcc... gcc  
- checking for C compiler default output... a.out  
- checking whether the C compiler works... yes  
- checking whether we are cross compiling... no  
- checking for suffix of executables...   
- checking for suffix of object files... o  
- checking whether we are using the GNU C compiler... yes  
- checking whether gcc accepts -g... yes  
- checking for gcc option to accept ANSI C... none needed  
- checking for style of include used by make... GNU  
- checking dependency style of gcc... gcc3  
- configure: creating ./config.status  
- config.status: creating Linux Makefile  
- config.status: executing depfiles commands  
- $ ls -l Linux Makefile  
- -rw-rw-r-- 1 yutao yutao 15035 Oct 15 10:40 Linux Makefile  

你可以看到，此时Linux Makefile已经产生出来了。

7、使用Linux Makefile编译代码

$ make if gcc -DPACKAGE_NAME="" -DPACKAGE_TARNAME="" -DPACKAGE_VERSION="" -DPACKAGE_STRING="" -DPACKAGE_BUGREPORT="" -DPACKAGE="helloworld" -DVERSION="1.0" 

-I. -I. -g -O2 -MT helloworld.o -MD -MP -MF ".deps/helloworld.Tpo" /-c -o helloworld.o `test -f 'helloworld.c' || echo './'`helloworld.c; /then mv -f ".deps/helloworld.Tpo" ".deps/helloworld.Po"; /else rm -f ".deps/helloworld.Tpo"; exit 1; /figcc -g -O2 -o
 helloworld helloworld.o  运行helloworld$ ./helloworld Hello, Linux World!

这样helloworld就编译出来了，你如果按上面的步骤来做的话，应该也会很容易地编译出正确的helloworld文件。你还可以试着使用一些其他的make命令，如make clean，make install，make dist，看看它们会给你什么样的效果。感觉如何？自己也能写出这么专业的Linux Makefile，老板一定会对你刮目相看。

四、深入浅出

针对上面提到的各个命令，我们再做些详细的介绍。

1、 autoscan

autoscan是用来扫描源代码目录生成configure.scan文件的。autoscan可以用目录名做为参数，但如果你不使用参数的话，那么 autoscan将认为使用的是当前目录。autoscan将扫描你所指定目录中的源文件，并创建configure.scan文件。

2、 configure.scan

configure.scan包含了系统配置的基本选项，里面都是一些宏定义。我们需要将它改名为configure.in

3、 aclocal

aclocal是一个perl 脚本程序。aclocal根据configure.in文件的内容，自动生成aclocal.m4文件。aclocal的定义是：“aclocal - create aclocal.m4 by scanning configure.ac”。

4、 autoconf

autoconf是用来产生configure文件的。configure是一个脚本，它能设置源程序来适应各种不同的操作系统平台，并且根据不同的系统来产生合适的Linux Makefile，从而可以使你的源代码能在不同的操作系统平台上被编译出来。

configure.in文件的内容是一些宏，这些宏经过autoconf 处理后会变成检查系统特性、环境变量、软件必须的参数的shell脚本。configure.in文件中的宏的顺序并没有规定，但是你必须在所有宏的最前面和最后面分别加上AC_INIT宏和AC_OUTPUT宏。

在configure.ini中：#号表示注释，这个宏后面的内容将被忽略。AC_INIT(FILE) 这个宏用来检查源代码所在的路径。AM_INIT_AUTOMAKE(PACKAGE, VERSION) 这个宏是必须的，它描述了我们将要生成的软件包的名字及其版本号：PACKAGE是软件包的名字，VERSION是版本号。

当你使用make dist命令时，它会给你生成一个类似helloworld-1.0.tar.gz的软件发行包，其中就有对应的软件包的名字和版本号。AC_PROG_CC这个宏将检查系统所用的C编译器。 AC_OUTPUT(FILE)这个宏是我们要输出的Linux Makefile的名字。

我们在使用automake时，实际上还需要用到其他的一些宏，但我们可以用aclocal 来帮我们自动产生。执行aclocal后我们会得到aclocal.m4文件。产生了configure.in和aclocal.m4 两个宏文件后，我们就可以使用autoconf来产生configure文件了。

5、 Linux Makefile.am

Linux Makefile.am是用来生成Linux Makefile.in的，需要你手工书写。Linux Makefile.am中定义了一些内容：AUTOMAKE_OPTIONS 这个是automake的选项。在执行automake时，它会检查目录下是否存在标准GNU软件包中应具备的各种文件，例如AUTHORS、ChangeLog、NEWS等文件。我们将其设置成foreign时，automake会改用一般软件包的标准来检查。

bin_PROGRAMS这个是指定我们所要产生的可执行文件的文件名。如果你要产生多个可执行文件，那么在各个名字间用空格隔开。 helloworld_SOURCES 这个是指定产生“helloworld”时所需要的源代码。

如果它用到了多个源文件，那么请使用空格符号将它们隔开。比如需要 helloworld.h，helloworld.c那么请写成helloworld_SOURCES= helloworld.h helloworld.c。如果你在bin_PROGRAMS定义了多个可执行文件，则对应每个可执行文件都要定义相对的filename_SOURCES。

6、 automake

我们使用automake --add-missing来产生Linux Makefile.in。选项--add-missing的定义是“add missing standard files to package”，它会让automake加入一个标准的软件包所必须的一些文件。我们用automake产生出来的Linux Makefile.in文件是符合GNU Linux Makefile惯例的，接下来我们只要执行configure这个shell
 脚本就可以产生合适的 Linux Makefile 文件了。

7、 Linux Makefile

在符合GNU Makefiel惯例的Linux Makefile中，包含了一些基本的预先定义的操作：make根据Linux Makefile编译源代码，连接，生成目标文件，可执行文件。make clean清除上次的make命令所产生的object文件（后缀为“.o”的文件）及可执行文件。

make install将编译成功的可执行文件安装到系统目录中，一般为/usr/local/bin目录。make dist产生发布软件包文件（即distribution package）。这个命令将会将可执行文件及相关文件打包成一个tar.gz压缩的文件用来作为发布软件的软件包。它会在当前目录下生成一个名字类似“PACKAGE-VERSION.tar.gz”的文件。

PACKAGE和VERSION，是我们在configure.in中定义的AM_INIT_AUTOMAKE(PACKAGE, VERSION)。make distcheck生成发布软件包并对其进行测试检查，以确定发布包的正确性。这个操作将自动把压缩包文件解开，然后执行configure命令。

并且执行make，来确认编译不出现错误，最后提示你软件包已经准备好，可以发布了。helloworld-1.0.tar.gz is ready for distributionmake distclean 类似make clean，但同时也将configure生成的文件全部删除掉，包括Linux Makefile。

五、结束语

通过上面的介绍，你应该可以很容易地生成一个你自己的符合GNU惯例的Linux Makefile文件及对应的项目文件。如果你想写出更复杂的且符合惯例的Linux Makefile，你可以参考一些开放代码的项目中的configure.in和Linux Makefile.am文件，比如：嵌入式数据库sqlite，单元测试cppunit。

【推荐】
- [Linux Makefile介绍使用的环境深入浅出](http://os.51cto.com/art/201002/184987.htm)
- [阐述Linux Makefile文件概念](http://os.51cto.com/art/200912/169213.htm)
- [Linux Makefile由浅入深剖析](http://os.51cto.com/art/200910/158465.htm)
- [linux makefile文件心得笔记](http://os.51cto.com/art/200910/158416.htm)
- [Linux系统手工挂载和自动挂载](http://os.51cto.com/art/201002/185012.htm)


## [例解 Linux 下 make 命令](http://blog.csdn.net/hazir/article/details/18408007)（http://blog.csdn.net/hazir/article/details/18408007）



Linux 下 make 命令是系统管理员和程序员用的最频繁的命令之一。管理员用它通过命令行来编译和安装很多开源的工具，程序员用它来管理他们大型复杂的项目编译问题。本文我们将用一些实例来讨论 make 命令背后的工作机制。

## [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#make-%E5%A6%82%E4%BD%95%E5%B7%A5%E4%BD%9C%E7%9A%84)Make 如何工作的


对于不知道背后机理的人来说，make 命令像命令行参数一样接收目标。这些目标通常存放在以 “Makefile” 来命名的特殊文件中，同时文件也包含与目标相对应的操作。更多信息，阅读关于 Makefiles 如何工作的系列文章。


当 make 命令第一次执行时，它扫描 Makefile 找到目标以及其依赖。如果这些依赖自身也是目标，继续为这些依赖扫描 Makefile 建立其依赖关系，然后编译它们。一旦主依赖编译之后，然后就编译主目标（这是通过 make 命令传入的）。


现在，假设你对某个源文件进行了修改，你再次执行 make 命令，它将只编译与该源文件相关的目标文件，因此，编译完最终的可执行文件节省了大量的时间。

## [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#make-%E5%91%BD%E4%BB%A4%E5%AE%9E%E4%BE%8B)Make 命令实例


下面是本文所使用的测试环境：

```
OS —— Ubunut 13.04
Shell —— Bash 4.2.45
Application —— GNU Make 3.81
```


下面是工程的内容：

$ ls 
anotherTest.c Makefile test.c test.h



下面是 Makefile 的内容：

all:test test:test.o anotherTest.o 
    gcc -Wall test.o anotherTest.o -o testtest.o:test.c 
    gcc -c -Wall test.c 

anotherTest.o:anotherTest.c 
    gcc -c -Wall anotherTest.c 

clean: 
    rm -rf *.o test


现在我们来看 Linux 下一些 make 命令应用的实例：

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#1-%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E4%BE%8B%E5%AD%90)1. 一个简单的例子


为了编译整个工程，你可以简单的使用 `make` 或者在 make 命令后带上目标 `all`。

$ make 
gcc -c -Wall test.c 
gcc -c -Wall anotherTest.c 
gcc -Wall test.o anotherTest.o -o test


你能看到 make 命令第一次创建的依赖以及实际的目标。


如果你再次查看目录内容，里面多了一些 .o 文件和执行文件：

$ ls 
anotherTest.c anotherTest.o Makefile test test.c test.h test.o



现在，假设你对 test.c 文件做了一些修改，重新使用 make 编译工程：

$ make 
gcc -c -Wall test.c 
gcc -Wall test.o anotherTest.o -o test


你可以看到只有 test.o 重新编译了，然而另一个 Test.o 没有重新编译。


现在清理所有的目标文件和可执行文件 test，你可以使用目标 `clean`:

$ make clean
rm -rf *.o test$ ls
anotherTest.c Makefile test.c test.h



你可以看到所有的 .o 文件和执行文件 test 都被删除了。

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#2-%E9%80%9A%E8%BF%87--b-%E9%80%89%E9%A1%B9%E8%AE%A9%E6%89%80%E6%9C%89%E7%9B%AE%E6%A0%87%E6%80%BB%E6%98%AF%E9%87%8D%E6%96%B0%E5%BB%BA%E7%AB%8B)2. 通过 -B 选项让所有目标总是重新建立


到目前为止，你可能注意到 make 命令不会编译那些自从上次编译之后就没有更改的文件，但是，如果你想覆盖 make 这种默认的行为，你可以使用 -B 选项。


下面是个例子：

$ make
make: Nothing to be done for`all’.

$ make -B
gcc -c -Wall test.c
gcc -c -Wall anotherTest.c
gcc -Wall test.o anotherTest.o -o test


你可以看到尽管 make 命令不会编译任何文件，然而 `make -B` 会强制编译所有的目标文件以及最终的执行文件。

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#3-%E4%BD%BF%E7%94%A8--d-%E9%80%89%E9%A1%B9%E6%89%93%E5%8D%B0%E8%B0%83%E8%AF%95%E4%BF%A1%E6%81%AF)3. 使用 -d 选项打印调试信息


如果你想知道 make 执行时实际做了什么，使用 -d 选项。


这是一个例子：

$ make -d | more
GNU Make 3.81
Copyright (C) 2006 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.

This program built for x86_64-pc-linux-gnu
Reading makefiles…
Reading makefile `Makefile’…
Updating makefiles….
Considering target file `Makefile’.
Looking for an implicit rule for`Makefile’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.o’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.c’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.cc’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.C’.
Trying pattern rule with stem `Makefile’.
Trying implicit prerequisite `Makefile.cpp’.
Trying pattern rule with stem `Makefile’.
--More--




这是很长的输出，你也看到我使用了 `more` 命令来一页一页显示输出。

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#4-%E4%BD%BF%E7%94%A8--c-%E9%80%89%E9%A1%B9%E6%94%B9%E5%8F%98%E7%9B%AE%E5%BD%95)4. 使用 -C 选项改变目录


你可以为 make 命令提供不同的目录路径，在寻找 Makefile 之前会切换目录的。


这是一个目录，假设你就在当前目录下:

```
$ ls 
file file2 frnd frnd1.cpp log1.txt log3.txt log5.txt
file1 file name with spaces frnd1 frnd.cpp log2.txt log4.txt
```


但是你想运行的 make 命令的 Makefile 文件保存在 ../make-dir/ 目录下，你可以这样做：

$ make -C ../make-dir/ 
make: Entering directory `/home/himanshu/practice/make-dir’ 
make: Nothing to be done for`all’. 
make: Leaving directory `/home/himanshu/practice/make-dir



你能看到 make 命令首先切到特定的目录下，在那执行，然后再切换回来。

### [](https://github.com/hazirguo/Linoxide/blob/master/Linux%20%E2%80%98make%E2%80%99%20Command%20Explained%20With%20Examples.md#5-%E9%80%9A%E8%BF%87--f-%E9%80%89%E9%A1%B9%E5%B0%86%E5%85%B6%E5%AE%83%E6%96%87%E4%BB%B6%E7%9C%8B%E4%BD%9C-makefile)5. 通过 -f 选项将其它文件看作 Makefile


如果你想将重命名 Makefile 文件，比如取名为 my_makefile 或者其它的名字，我们想让 make 将它也当成 Makefile，可以使用 -f 选项。

make -f my_makefile



通过这种方法，make 命令会选择扫描 my_makefile 来代替 Makefile。


原文链接：[http://linoxide.com/how-tos/linux-make-command-examples/](http://linoxide.com/how-tos/linux-make-command-examples/)





## [Makefile自动生成工具-----autotools的使用（详细）](http://blog.csdn.net/memoryjs/article/details/7860783)

相信每个学习Linux的人都知道Makefile，这是一个很有用的东西，但是编写它是比较复杂，今天介绍一个它的自动生成工具，autotools的使用。很多GNULinux的的软件都是用它生成Makefile的，包括我们非常熟悉的Linux内核源代码。
  1、准备:

  需要工具

  autoscan

  aclocal

  autoheader 

  automake

  autoconf

  auto make 

  在终端敲入命令，哪个没有安装哪个，一般是第一个autoscan没有，其它的我用的Ubuntu10.04下全部都有
  2、测试程序编写：

     建立目录：mkdir include src

     编写程序：include/str.h





**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- #include <stdio.h>
- int str(char *string);
  

    编写程序：src/str.c



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- #include "str.h"
- //print string
- int str(char *string){  
-         printf("\n----PRINT STRING----\n\"%s\"\n",string);  
- return 0;  
- }  
- 
- //interface of this program
- int main(int argc , char **argv){  
- char str_read[1024];  
-         printf("Please INPUT something end by [ENTER]\n");  
-         scanf("%s",str_read);  
- return str(str_read );  
- }  
- 
- 

  3、生成configure.ac
  configure.ac是automake的输入文件，所以必须先生成该文件。

    执行命令：





**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# ls  
- include  src  
- [root@localhost str]# autoscan  
- autom4te: configure.ac: no such file or directory  
- autoscan: /usr/bin/autom4te failed with exit status: 1  
- [root@localhost str]# ls  
- autoscan.log  configure.scan  include  src  
- [root@localhost str]# cp configure.scan configure.ac   

    改 configure.ac 


**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- #                                               -*- Autoconf -*-
- # Process this file with autoconf to produce a configure script.
- 
- AC_PREREQ(2.59)  
- AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)  
- AC_CONFIG_SRCDIR([include/str.h])  
- AC_CONFIG_HEADER([config.h])  
- 
- # Checks for programs.
- AC_PROG_CC  
- 
- # Checks for libraries.
- 
- # Checks for header files.
- 
- # Checks for typedefs, structures, and compiler characteristics.
- 
- # Checks for library functions.
- AC_OUTPUT  

修改



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)  

改为：


**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- AC_INIT(str,0.0.1, [bug@sounos.org])  

其中：FULL-PACKAGE-NAME 为程序名称，VERSION为当前版本， BUG-REPORT-ADDRESS为bug汇报地址
  然后添加两句话：

AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile])

结果如下：（两句话不是在一起的）



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- #                                               -*- Autoconf -*-
- # Process this file with autoconf to produce a configure script.
- 
- AC_PREREQ(2.59)  
- #AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
- AC_INIT(str, 0.0.1, [bug@sounos.org])  
- AM_INIT_AUTOMAKE  
- AC_CONFIG_SRCDIR([include/str.h])  
- AC_CONFIG_HEADER([config.h])  
- 
- # Checks for programs.
- AC_PROG_CC  
- 
- # Checks for libraries.
- 
- # Checks for header files.
- 
- # Checks for typedefs, structures, and compiler characteristics.
- 
- # Checks for library functions.
- AC_CONFIG_FILES([Makefile])  
- AC_OUTPUT  

4、执行aclocal



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# aclocal  
- /usr/share/aclocal/libfame.m4:6: warning: underquoted definition of AM_PATH_LIBFAME  
-   run info '(automake)Extending aclocal'
-   or see http://sources.redhat.com/automake/automake.html#Extending-aclocal

5、制作Makefile.am



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# vi Makefile.am  
- #Makefile.am
- bin_PROGRAMS    = str  
- str_SOURCES     = include/str.h src/str.c  
- str_CPPFLAGS    = -I include/  

automake 这个命令需要用到这个配置文件。各个选项意思比较直观，不多说。



6、autoheader





**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# autoheader  

7、automake必须文件：



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- *  install-sh  
- * missing  
- * INSTALL  
- * NEWS  
- * README  
- * AUTHORS  
- * ChangeLog  
- * COPYING  
- * depcomp   

其中，以下文件在执行automake -a的时候会自动生成


**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- * install-sh  
- * missing  
- * INSTALL  
- * COPYING  
- * depcomp   

所以，接下来手动生成剩下的文件


**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# touch NEWS README AUTHORS ChangeLog  

8、执行automake -a


**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# automake -a  
- configure.ac: installing `./install-sh'  
- configure.ac: installing `./missing'  
- Makefile.am: installing `./INSTALL'  
- Makefile.am: installing `./COPYING'  
- Makefile.am: installing `./compile'  
- Makefile.am: installing `./depcomp'  

9、autoconf



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# autoconf  
- [root@localhost str]# ls  
- aclocal.m4      autoscan.log  config.h.in   configure.scan  include     Makefile.am  NEWS  
- AUTHORS         ChangeLog     configure     COPYING         INSTALL     Makefile.in  README  
- autom4te.cache  compile       configure.ac  depcomp         install-sh  missing      src  

10、执行测试：
      执行./configure



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# ./configure --prefix=/u  
- checking for a BSD-compatible install... /usr/bin/install -c  
- checking whether build environment is sane... yes  
- checking for gawk... gawk  
- checking whether make sets $(MAKE)... yes  
- checking for gcc... gcc  
- checking for C compiler default output file name... a.out  
- checking whether the C compiler works... yes  
- checking whether we are cross compiling... no  
- checking for suffix of executables...  
- checking for suffix of object files... o  
- checking whether we are using the GNU C compiler... yes  
- checking whether gcc accepts -g... yes  
- checking for gcc option to accept ANSI C... none needed  
- checking for style of include used by make... GNU  
- checking dependency style of gcc... gcc3  
- configure: creating ./config.status  
- config.status: creating Makefile  
- config.status: creating config.h  
- config.status: config.h is unchanged  
- config.status: executing depfiles commands  

执行 make



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# make  
- make  all-am  
- make[1]: Entering directory `/data/devel/c/str'  
- if gcc -DHAVE_CONFIG_H -I. -I. -I.  -I include/   -g -O2 -MT str-str.o -MD -MP -MF ".deps/str-str.Tpo" -c -o str-str.o `test -f 'src/str.c' || echo './'`src/str.c; \  
- then mv -f ".deps/str-str.Tpo"".deps/str-str.Po"; else rm -f ".deps/str-str.Tpo"; exit 1; fi  
- gcc  -g -O2   -o str  str-str.o  
- make[1]: Leaving directory `/data/devel/c/str'  

此时已经生成了 str（可执行文件名字在前面设置Makefile.am的参数时候去顶）这个，可以通过./str直接看到运行结果



**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# ./str  
- Please INPUT something end by [ENTER]  
- abcksdhfklsdklfdjlkfd  
- 
- ----PRINT STRING----  
- "abcksdhfklsdklfdjlkfd"

不过这里我们都做一步，把它安装到系统里面，这样我们只要在终端输入str就可以运行程序了。
执行 make install：





**[cpp]**[view
 plain](http://blog.csdn.net/memoryjs/article/details/7860783#)[copy](http://blog.csdn.net/memoryjs/article/details/7860783#)



- [root@localhost str]# make install  
- make[1]: Entering directory `/data/devel/c/str'  
- test -z "/u/bin" || mkdir -p -- "/u/bin"
-   /usr/bin/install -c 'str''/u/bin/str'
- make[1]: Nothing to be done for `install-data-am'.  
- make[1]: Leaving directory `/data/devel/c/str'       

接下来你可以make clean 清除安装的那些.o 文件了。这样生成了一个自动的Makefile。


## Easymake 使用说明

## 介绍




Easymake 是一个在linux系统中 C/C++ 开发的通用 makefile。在一个简单的 C/C++ 程序中使用 easymake，你甚至可以不写一行 makefile 代码来生成目标文件。


Easymake 包含以下功能：
- 自动扫描 C/C++ 源文件。
- 自动生成和维护依赖关系，加快编译时间。
- 支持简单的单元测试，可以很方便地管理多个程序入口（main 函数）。
- 完美地支持 `VPATH` 变量。


我将在后面的例子中一步步地向你展示如何使用 easymake 来构建你的应用程序。别看文档这么长，下面一节的内容中大部分是在讲一个简单的 C/C++ 程序的开发。就像 easymake 的名字一样，easymake 是非常易学易用的。

## 开始学习 Easymake


在这一节中将展示如何在一个简单的程序中使用 easymake。接下来让我们一个加法程序，用户输入两个数字，然后程序输出这两个数字相加的结果。这个程序的源代码可以在 `samples/basics` 目录中找到。

### C/C++ 代码


这个程序很简单，所以这里跳过程序设计环节。这里直接展示程序的 C/C++ 代码，然后再转入我们的正题。


File: main.cpp

```cpp
#include <iostream>

#include "math/add.h"

using namespace std;

int main(){
        cout<<"please enter two integer:"<<endl;

        int a,b;
        cin>>a>>b;

        cout<<"add("<<a<<","<<b<<") returns "<<add(a,b)<<endl;
}
```


File: math/add.h

```cpp
#ifndef ADD_H
#define ADD_H

int add(int,int);

#endif
```


File: math/add.cpp

```cpp
#include "math/add.h"

int add(int a,int b){
        return a+b;
}
```

### 使用 Easymake 来构建程序


代码很简单，可以直接使用命令行来构建程序。如果你对 makefile 的语法熟悉，你也可以很快地写出一个 makefile 来做完成这个事情。那么如何使用 easymake 来构建这个程序呢？先别急，接下来将使用刚才提到的三种方法来构建程序，希望你能清晰地了解和比较这三种方式。

#### 使用命令行构建

```
g++ -c -o main.o main.cpp
g++ -c -o add.o math/add.cpp -I.
g++ -o target main.o add.o
```


或者也可以只用一条命令 
```
g++
 -o target main.cpp math/add.cpp -I.
```
 来构建程序。


然后输入 `ls` 和 `./target`，就可以观察到程序的执行结果了：

```
[root@VM_6_207_centos basics]# ls
add.o  bin  main.cpp  main.o  makefile  math  target
[root@VM_6_207_centos basics]# ./target
please enter two integer:
5
3
add(5,3) returns 8
```

#### 自己写一个 makefile 构建程序


创建一个新的 Makefile 文件，代码如下：


```
target: main.o add.o
        g++ -o target main.o add.o

main.o: main.cpp
        g++ -c -o main.o main.cpp -I.

add.o: math/add.cpp
        g++ -c -o add.o math/add.cpp -I.
```



结果基本是一样的：

```
[root@VM_6_207_centos basics]# make
g++ -c -o main.o main.cpp -I.
g++ -c -o add.o math/add.cpp -I.
g++ -o target main.o add.o
[root@VM_6_207_centos basics]# ls
add.o  main.cpp  main.o  makefile  math  target
[root@VM_6_207_centos basics]# ./target
please enter two integer:
8
9
add(8,9) returns 17
```


使用 makefile 的好处就是，如果能很好地确定依赖关系，那么就不需要在每次构建时把所有的源文件都重新编译一次。但是随着项目的代码的增长，即使在一个良好的模块化设计中，手工维护依赖关系也是一件很繁琐而且很容易出错的工作。例如，假设我们需要增加一个 `multiply.cpp` 和 `multiply.h` 文件，让程序支持乘法计算的功能，那么我必须修改我们的
 makefile 才能构建新的程序。另外，如果头文件 `add.h` 被修改了，`multiply.cpp` 就不需要重新编译，所以我们应该在
 makefile 中增加 .cpp 文件和 .h 文件之间的依赖关系的代码。到这里，我想你也会觉得我们应该有一个通用的 makefile 来帮助我们自动维护依赖关系了吧。

#### 使用 easymake 构建程序


在这个例子中，包含 `easymake.mk` 文件就足够了。把我们的
 Makefile 修改成下面的代码：

`include ../../easymake.mk`


在命令行中输入 `make` 构建我们的程序。接下来我们给你展示一些细节来帮助你理解
 makefile 是如何构建程序的。

```
[root@VM_6_207_centos basics]# ls
main.cpp  makefile  math
[root@VM_6_207_centos basics]# make
g++ -c -o bin/main.o main.cpp  -I.
entry detected
g++ -c -o bin/math/add.o math/add.cpp  -I.
g++ -o bin/target bin/main.o bin/math/add.o
BUILD_ROOT/TARGET: bin/target
ENTRY: main.cpp
[root@VM_6_207_centos basics]# ./bin/target
please enter two integer:
3
5
add(3,5) returns 8
```


你也许也已经注意到，和之前的方式相比，主要的不同就是输出中的 
```
entry
 detected
```
，
```
BUILD_ROOT/TARGET:
 bin/target
```
 和 
```
ENTRY:
 main.cpp
```
。`bin/target` 就是我们的程序。至于这里的entry，会在后面讲到。


现在可以看一下当前的目录结构：

```
[root@VM_6_207_centos basics]# tree .
.
├── bin
│   ├── easymake_current_entry_file
│   ├── easymake_detected_entries
│   ├── easymake_entries_tmp.d
│   ├── main.d
│   ├── main.o
│   ├── math
│   │   ├── add.d
│   │   └── add.o
│   └── target
├── main.cpp
├── makefile
└── math
 ├── add.cpp
 └── add.h

3 directories, 12 files
```


Easymake 使用 `bin` 目录作为 `BUILD_ROOT`，用来存放生成的文件，这样一来我们的源文件目录也不会被污染。这里面的 `*.d` 和 `easy_make_*` 文件都是由
 easymake 额外生成用来维护依赖关系的。`*.d` 的文件其实也算是
 makefile 的一部分，例如 main.d 文件的内容如下：

```
[root@VM_6_207_centos basics]# cat bin/main.d
bin/main.o: main.cpp math/add.h

math/add.h:
```


这些依赖关系是 easymake 自动生成的，所以每当 `math/add.h` 被修改了，`main.o` 就会重新生成。事实上，你不需要关注这些细节来使用
 easymake，所以我们就忽略这些额外生成的文件吧。如果你有兴趣，可以查看 `easymake.mk` 的源代码，我觉得代码的注释得已经足够帮助你理解了。

### 用户选项


如果你想使用 gcc 编译器的 `-O2` 优化选项和链接器的 `-static` 选项来构建这个程序。那么你需要增加几行代码来修改编译和链接选项。下面是修改后的
 makefile：


```
COMPILE_FLAGS   += -O2
LINK_FLAGS      += -static

include ../../easymake.mk
```



然后重新构建程序：

```
[root@VM_6_207_centos basics]# make clean
rm -f \$(find bin -name \*.o)
rm -f \$(find bin -name \*.d)
rm -f \$(find bin -name \*.a)
rm -f \$(find bin -name \*.so)
rm -f \$(find bin -name \*.out)
rm -f bin/target
[root@VM_6_207_centos basics]# make
g++ -c -o bin/main.o main.cpp -O2  -I.
 entry detected
g++ -c -o bin/math/add.o math/add.cpp -O2  -I.
g++ -o bin/target bin/main.o bin/math/add.o  -static
BUILD_ROOT/TARGET: bin/target
ENTRY: main.cpp
```


除些以外，还有更多可供设置的选项，使用 
```
make
 help
```
 命令你就可以看到它们。注意 basic settings 和user settings 两部分的内容即可，其他部分可以忽略。

```
[root@VM_6_207_centos basics]# make help
---------------------
basic settings:
SETTINGS_ROOT       : build_settings
BUILD_ROOT          : bin
TARGET              : target
VPATH               :
CPPEXT              : cpp
CEXT                : c
GCC                 : gcc
GXX                 : g++
LINKER              : g++
---------------------
user settings files:
build_settings/entry_list
build_settings/compile_flags
build_settings/compile_search_path
build_settings/link_flags
build_settings/link_search_path
---------------------
user settings:
ENTRY_LIST          :
ENTRY               :
COMPILE_FLAGS       : -O2
COMPILE_SEARCH_PATH :  .
LINK_FLAGS          : -static
LINK_SEARCH_PATH    :
CPPSOURCES          : main.cpp math/add.cpp
CSOURCES            :
---------------------
internal informations:
   ...
   ...
   ...
```

## 用来测试的程序入口


现在我们需要给程序增加一个乘法运算功能，首先写一个 C++ 函数来做乘法运算，然后，在我们修改 `main.cpp` 的代码之前，我们应该测试一下这个这个
 C++ 函数的功能，确保新增加的乘法模块的逻辑是正确的。下面的例子会告诉你如果使用 easymake 来完成这项工作，你可以在 `samples/entries` 文件夹中找到这个例子的代码。

### 编写乘法模块的代码


File `math/multiply.h`:

```cpp
#ifndef MULTIPLY_H
#define MULTIPLY_H

#include "stdint.h"

int64_t multiply(int32_t,int32_t);

#endif
```


File `math/multiply.cpp`:

```cpp
#include "math/multiply.h"

int64_t multiply(int32_t a,int32_t b){
        return (int64_t)a*(int64_t)b;
}
```

### 编写测试代码


在命令行中输入 
```
mkdir
 test
```
 和 
```
vim
 test/multiply.cpp
```
 然后编写我们的代码。为了简单起见，这里仅仅是在 `main` 函数中打印了
 8 乘 8 的结果。

```cpp
#include "math/multiply.h"

#include <iostream>

using namespace std;

int main(){
        cout<<"multiply(8,8)="<<multiply(8,8)<<endl;
}
```

### 构建测试程序


现在直接输入命令 `make` 和 `./bin/target` 就可以看到测试程序的输出了。

```
[root@VM_6_207_centos entries]# make
g++ -c -o bin/main.o main.cpp -O2  -I.
    entry detected
g++ -c -o bin/math/add.o math/add.cpp -O2  -I.
g++ -c -o bin/math/multiply.o math/multiply.cpp -O2  -I.
g++ -c -o bin/test/multiply.o test/multiply.cpp -O2  -I.
    entry detected
g++ -o bin/target bin/math/add.o bin/math/multiply.o bin/test/multiply.o  -static
BUILD_ROOT/TARGET: bin/target
ENTRY: test/multiply.cpp
[root@VM_6_207_centos entries]# ./bin/target
multiply(8,8)=64
[root@VM_6_207_centos entries]#
```


注意到 `main.cpp` 和 `test/multiply.cpp` 都有被成功编译，但是只有 `test/multiply.cpp` 被链接到目标文件中，而且输出中 `ENTRY` 对应的值也变成了 `test/multiply.cpp`。在
 easymake，全体一个包含 `main`函数定义的源文件都会被自动检测到，并且被当作程序入口文件（`ENTRY`）。在众多入口文件当中，只有一个会被选中，其他文件不会被链接到目标文件中。另外注意这里的 `ENTRY` 所表示的文件名对应的文件也可以不存在，在某些场景中，例如生成动态库
 so 文件，就需要选择这个 `ENTRY` 来阻止其他入口文件被链接到目标文件中。


现在你肯定是在纳闷，easymake 是如何知道要选择 `test/multiply.cpp` 而不是 `main.cpp` 的？是不是很神奇？其实这里使用的是入口文件的最后修改时间。如果有多个入口文件，而且用户没有显式地声明使用哪个入口，那么
 easymake 就会自动选择最新的那个计算器文件。


如果你需要显式地声明 `ENTRY`，以选择 `main.cpp` 为例，可以输入命令 
```
make
 ENTRY=main.cpp
```
 或者 
```
make
 ENTRY=m
```
 ：

```
[root@VM_6_207_centos entries]# make ENTRY=main.cpp
g++ -o bin/target bin/main.o bin/math/add.o bin/math/multiply.o  -static
BUILD_ROOT/TARGET: bin/target
ENTRY: main.cpp
```


到这里已经完成了乘法模块的测试，接下来可以修改 `main.cpp` 的代码来整合我们的新模块了。为了简洁，接下来的步骤就不在这里赘述了，如果有需要，可以查看 `samples/entries` 目录中的代码。

## 原文及代码下载


最新的代码和文档请前往此处下载 [https://github.com/roxma/easymake](https://github.com/roxma/easymake) 。









