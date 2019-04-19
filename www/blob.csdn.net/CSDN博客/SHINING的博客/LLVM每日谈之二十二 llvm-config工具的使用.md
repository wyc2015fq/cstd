# LLVM每日谈之二十二  llvm-config工具的使用 - SHINING的博客 - CSDN博客
2014年11月25日 10:18:52[snsn1984](https://me.csdn.net/snsn1984)阅读数：7944
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[史宁宁（snsn1984）](http://blog.csdn.net/snsn1984)
llvm-config作为LLVM的一个工具，是非常有用的，官方文档（[http://llvm.org/docs/CommandGuide/llvm-config.html](http://llvm.org/docs/CommandGuide/llvm-config.html)）关于它的介绍如下：
llvm-config makes it easier to build applications that use LLVM. It can print the compiler flags, linker flags and object libraries needed to link against LLVM.
这里介绍的非常清楚，llvm-config使得使用LLVM去构建自己的应用更加的简单。之所以可以更加简单，是因为它可以打印出编译器flags、连接器flags以及需要连接的LLVM库。简单点的说，就是llvm-config可以获取系统中LLVM的所有相关信息，这些信息可以方便的用于构建基于LLVM的项目。只是这么说的话，看起来并不明显，下面我就给出一个实例来。
```
LLVM_CONFIG = llvm-config
LLVM_CXXFLAGS += $(shell $(LLVM_CONFIG) --cxxflags)
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags)
LLVM_LIBS = $(shell $(LLVM_CONFIG) --libs bitwriter core support)
llvm_model_src = ModuleMaker.cpp
test_model:
	g++ $(llvm_model_src) $(LLVM_CXXFLAGS) $(LLVM_LIBS) $(LLVM_LDFLAGS) -lpthread -ldl -o ModuleMaker
```
这是一个Makefile。它是一个例子ModuleMaker的编译文件。这个ModuleMaker例子本身是LLVM源码中llvm/examples/ModuleMaker/目录下的一个例子，它演示的如果凭空构建一个LLVM IR的Module。我这里写了这个Makefile以后，可以在已经安装LLVM的系统（Linux）上单独的编译这个例子，而不需要依赖LLVM的源码，也不再需要在LLVM源码中编译这个例子。完整的包含Makefile的ModuleMaker例子的代码：[https://github.com/shining1984/llvm-examples](https://github.com/shining1984/llvm-examples)
从这个Makefile中可以看出，编译所需要的环境变量，包括LLVM_CXXFLAGS、LLVM_LDFLAGS和LLVM_LIBS都是直接通过llvm-config直接获取的，这就完全不需要用户在编译项目的时候设置环境变量或者传递变量，项目可以直接获取系统里的环境变量，大大方便了项目的构建。只有真正的构建过基于LLVM项目的人，才明白使用了llvm-config之后会多方便。
同时，llvm-config还可以以`llvm-config --libs`这样的形式在Makefile中使用，或者在命令行中使用，这样的使用形式是获取这个命令在shell执行后所输出的信息。这里需要指出的是“`”这个符号，并不是“‘”。这两者是有区别的，前者是和“~”同键的符号，后者是和“"” 同键的符号。这一点是一定要区别的，否者系统无法识别。例如：
```cpp
g++ `llvm-config --cxxflags` -o HowToUseJIT.o -c HowToUseJIT.cpp
g++ `llvm-config --ldflags` -o HowToUseJIT HowToUseJIT.o \
    `llvm-config --libs engine bcreader scalaropts`
```
llvm-config的主要参数如下：
**–version**
> Print the version number of LLVM.
**-help**
> Print a summary of **llvm-config** arguments.
**–prefix**
> Print the installation prefix for LLVM.
**–src-root**
> Print the source root from which LLVM was built.
**–obj-root**
> Print the object root used to build LLVM.
**–bindir**
> Print the installation directory for LLVM binaries.
**–includedir**
> Print the installation directory for LLVM headers.
**–libdir**
> Print the installation directory for LLVM libraries.
**–cxxflags**
> Print the C++ compiler flags needed to use LLVM headers.
**–ldflags**
> Print the flags needed to link against LLVM libraries.
**–libs**
> Print all the libraries needed to link against the specified LLVM *components*, including any dependencies.
**–libnames**
> Similar to **–libs**, but prints the bare filenames of the libraries without **-l** or pathnames. Useful for linking against a not-yet-installed copy of LLVM.
**–libfiles**
> Similar to **–libs**, but print the full path to each library file. This is useful when creating makefile dependencies, to ensure that a tool is relinked if any library it uses changes.
**–components**
> Print all valid component names.
**–targets-built**
> Print the component names for all targets supported by this copy of LLVM.
**–build-mode**
> Print the build mode used when LLVM was built (e.g. Debug or Release)
参考文档：[http://llvm.org/docs/CommandGuide/llvm-config.html](http://llvm.org/docs/CommandGuide/llvm-config.html)
