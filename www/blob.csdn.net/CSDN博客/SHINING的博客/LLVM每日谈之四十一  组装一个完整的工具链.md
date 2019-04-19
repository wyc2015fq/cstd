# LLVM每日谈之四十一    组装一个完整的工具链 - SHINING的博客 - CSDN博客
置顶2019年01月13日 21:54:32[snsn1984](https://me.csdn.net/snsn1984)阅读数：409
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
写在前面的话：这是一篇LLVM的官方文档，英文文档地址：
[Assembling a Complete Toolchain](http://link.zhihu.com/?target=https%3A//clang.llvm.org/docs/Toolchain.html)
之前读文档的时候，陆陆续续的翻译过一些，周末花了点时间把这个文章整理了出来。因为都是随手翻的，翻译完了之后，也不愿意再花时间去斟酌，所以有不少用词都不那么严谨。
LLVM的官方文档很详细，有什么需要最好是直接读官方的英文文档。这个自己随便翻译的文档放出来，是希望能给有些不愿意读英文文档的同学一点参照。
------------------------------------------------------------------------------------
## 组装一个完整的工具链
**介绍：**
Clang只是完整工具链中的一个C系列编程语言的组件。为了组成一个完整的工具链，需要一些额外的工具和运行库。Clang被设计的可以和目标平台已有的工具和库进行交互操作，而且LLVM项目提供了很多这样可供选择的组件。
这个文档描述了一个完整的工具链所必须和可选的组件，提供了从何处获取这些组件，并且提供了支持版本和每个选择的限制。
警告：
这个文档现在描述的Clang的配置是在类POSIX操作系统上并且和GCC兼容的clang驱动。当目标平台变成Windows系统上与MSVC兼容的clang-cl驱动，一些细节会有不同。
**工具**
一个C系列的编程语言的完整编译过程，通常会调用接下来的流水线的工具，一些编译情况下有些工具会被省略。
- 预处理：这展示了C的预处理动作：展开#includes和#defeins。-E标识可以让Clang在这个步骤之后停止。
- 语法分析：这个语法分析和语义分析，分析源程序并且建立了一个源码层级的中间表示（”AST”），产生了一个预编译头（precompiled header, PCH）,前言，或预编译模块文件（precompiled module file, PCM）,这依赖于输入。-precompile 标识可以让Clang在这个步骤之后停止。如果输入是一个头文件的话，这就是默认设置（Clang在这个步骤之后停止）。
- 生成中间代码：这将源码级的中间标识转化为一个优化器专用的中间表示（IR）；对于Clang，这就是LLVM的中间表示。-emit-llvm标识可以让Clang在这个步骤之后停止。如果和-S联合起来，Clang可以生成LLVM IR的文本形式；另外，它可以生成LLVM IR的二进制形式。
- 编译器后端：这将中间表示转化为目标平台专用的汇编代码。-S标识可以让Clang在这个步骤之后停止。
- 汇编器：这将目标平台专用的汇编代码转化为目标专用的机器代码。-c标识可以让Clang在这个步骤之后停止。
- 链接器：这将多个对象文件合并成一个映象文件（一个共享对象或一个可执行文件）。
Clang提供了除链接器之外的所有部分。当多个步骤是被同一个工具执行的时候，通常把这些步骤都融合在一起，避免产生中间文件。
当把上述步骤中某个步骤的输出作为一个输入的话，在这个步骤之前的步骤都会被跳过。（例如，一个.s文件输入将会被汇编和链接。）
Clang驱动程序可以被-###标识调用（这个参数在大多数shell下需要被escaped），来查看上述的步骤中到底哪些命令将被运行，在不运行这些命令的情况下。-v（verbose）标识将在运行命令的基础之上打印运行的命令。
**Clang前端**
Clang前端（clang -cc1）用来编译C系列的编程语言。前端的命令行接口被认为是一个实现的细节，有意不提供外部接口，并且主观上可以不加提醒的修改。
**其他编程语言的前端**
Clang可以接受非C语言系列的编程语言的输入。在这种情况下，输入需要经过一个外部工具的编译。现在支持的语言有：
- Ada (-x ada, .ad[bs])
- Fortran (-x f95, .f, .f9[05], .for, .fpp, case-insensitive)
- Java (-x java)
在每个例子中，GCC将被调用，用来编译输入。
**汇编器**
Clang可以使用LLVM的集成的汇编器，或者使用一个外部的系统相关的工具（比如，GUN操作系统上的GNU汇编器）来从汇编语言产生机器代码。默认情况下，Clang在所有支持的目标平台上使用LLVM的集成的汇编器。如果你希望使用系统的汇编器替代LLVM的集成汇编器，使用-fno-intergrated-as 选项。
**链接器**
Clang可以被配置成使用一下几种不同的链接器：
- GNU ld
- GNU gold
- LLVM’s **[lld](http://link.zhihu.com/?target=http%3A//lld.llvm.org/)**
- MSVC’s link.exe
lld天然支持链接时优化，并且在使用gold的时候可以通过一个链接器插件支持。
不同的目标平台有不同的默认链接器，这些链接器的设置可以通过-fuse-ld=<linker name>标识重写。
**运行时库**
C家族的程序需要很多不同的运行时库来提供不同层面的支持。Clang会隐式链接每个运行时库的一个合适的实现，这种选择往往基于目标平台的默认选择，或者是通过-rtlib=和-stdlib=这种标识显式选择。
链接的库的设置，依赖于语言模型。作为一个结果而言，那么当你链接C++程序的时候，为了确保C++运行库被提供，你需要使用clang++。
注意：
或许存在这些部件的其他实现，而本文档的下述部分并没有描述它们。请让我们知道这些其他实现是怎么和Clang一起工作的，这样我们好将这些部分添加到这个清单中。
**编译器运行时**
编译器运行时库提供了一些函数定义，这些函数是编译器为了支持那些下层的硬件所没有原生支持的操作（例如，128位整型相乘）而隐式调用的，这些操作的内联扩展被认为是不合适的。
默认的运行时库是目标相关的。在GCC是主要编译器的目标平台上，Clang现在默认使用libgcc_s。在大多数其他目标平台上，默认使用的是compiler-rt。
compiler-rt(LLVM)
LLVM的编译器运行时库提供了一个完整的运行时库函数的集合，这个集合包含了所有Clang将隐式调用的函数，放在 libclang_rt.builtins.<arch>.a.
你可以使用-rtlib-compiler-rt这个标识去命令Clang使用compiler-rt。但是，并不是每个目标平台都可以支持的。
如果你使用libc++和/或libc++abi，你或许需要设置它们去使用compiler-rt而不是libgcc_s，设置方式为：传递-DLIBCXX_USE_COMPILER_RT=YES 和/或
-DLIBCXXABI_USE_COMPILER_RT=YES 给cmake。另外一方面，你或许需将以把运行时库链接到你的程序中作为结尾（严格的说这么做不会带来什么伤害，但是确是个浪费）。
libgcc_s(GNU)
GCC的运行时库可以用来取代compiler-rt。但是，它缺乏了几个函数，这几个函数是LLVM有可能会生成引用的，特别是当使用Clang的__builtin_*_overflow指令家族的时候。
你可以使用-rtlib=libgcc标识让Clang使用libgcc_s。这也不是所有平台都支持的。
**原子库**
如果你的程序使用了原子操作，并且编译器无法把它们直接转换为及其指令（因为或许是因为没有合适的机器指令，或者是操作数没办法很好的对齐），一个调用运行时库的_atomic_*函数将会被生成。一个运行时库包含了这些原子操作函数，这些函数是此类程序（使用原则操作的程序）所必须的。
compiler-rt(LLVM)
compiler-rt包含了一个原子库的实现。
libatomic(GNU)
libgcc_s没有提供一个原子库的实现。作为替代，GCC的libatomic库可以用来在使用libgcc_s的时候提供原子库的功能。
注意：
Clang目前在使用libgcc_s的时候，并不会自动连接libatomic。在使用非原生的原子操作的时候（如果你看到了链接错误指向_atomic_*函数），你或许需要手动添加-latomic去支持这个配置。
**Unwind 库**
unwind库提供了一系列的_Uwind_*函数，这些函数实现了语言中立的Itanium C++ ABI(Level I)的栈回滚部分。它是C++ ABI库的一个依赖，有时候也是其他运行时的依赖。
libunwind(LLVM)
LLVM的unwinder库可以从SVN获取：
llvm-src$ svn co [https://llvm.org/svn/llvm-project/libunwind/trunk](http://link.zhihu.com/?target=https%3A//llvm.org/svn/llvm-project/libunwind/trunk) projects/libunwind
当代码被LLVM的检出系统检出到projects/libunwind目录下时，LLVM的构建系统会自动的将libunwind纳入构建。
如果使用libc++ abi，你或许需要配置使用libunwind而不是libgcc_s，配置的方式是传递-DLIBCXXABI_USE_LLVM_UNWINDER=YES 给cmake。如果libc++ abi被设置为使用某个版本的libunwind，这个库将隐式的链接到已经链接libc++ abi的二进制文件。
libgcc_s(GNU)
libgcc_s有一个集成的unwinder，不需要额外提供一个unwind库。
libunwind([http://nongun.org](http://link.zhihu.com/?target=http%3A//nongun.org))
这是libunwind规范的另外一个实现。具体参见libunwind([http://nongnu.org](http://link.zhihu.com/?target=http%3A//nongnu.org))。
libunwind（PathScale）
这是libunwind规范的另外一个实现。具体参见libunwind(pathscale)。
**Sanitizer运行时**
Clang的sanitizers（-fsanitize=…）添加的指令隐式的调用了一个运行时库，为了保证程序执行的边界状态，和发现问题时候形成诊断信息。
这个运行时唯一支持的实现是LLVM的compiler-rt，并且这个库（libclang_rt.<sanitizer>.<arch>.a）的相关部分将会在使用-fsanitize=…标识进行链接的时候隐式链接。
**C标准库**
Clang支持C标准库实现的广泛种类。
**C++ ABI库**
C++ ABI 库提供了Itanium C++ ABI的一部分库的实现，覆盖了主要Itanium C++ ABI文档所要支持的功能和异常处理要支持的Level II。对这些函数的引用的生成，主要是Clang在编译C++代码的时候隐式生成的。
当链接使用libstdc++的C++代码和使用libc++的C++代码到一个程序成为可能的时候（只要你不试图去传递C++标准库对象穿过边界），通常不能在一个程序中使用一个以上的C++ ABI库。
Clang使用的C++ ABI 库的版本通常是挑选的C++标准库，这也是要链接的库。几种可用的实现：
libc++abi(LLVM)
libc++abi是这个规范的LLVM实现。
libsupc++(GNU)
libsupc++是这个规范的GCC实现。然而，这个库只有libstdc++是静态链接的时候才会被使用。libstdc++的动态库版本包含了一个libsupc++的拷贝。
注意：
Clang现在并不能自动在静态链接libstdc++的时候链接libstomic。当使用-static或者-static-libstdc++时候，你需要手动添加-lsupc++去支持这个设置。
libcxxrt(PathScale)
这是Itanium C++ ABI规范的另外一个实现。具体参见libcxxrt。
**C++标准库**
Clang支持使用基于C++标准库的LLVM的libc++或GCC的libstdc++实现。
libc++ (LLVM)
libc++是C++标准库的LLVM的实现，着眼于成为C++标准（从C++11往前）的一个完全实现。
你可以使用-stdlib=libc++标识去让Clang使用libc++。
libstdc++ (GNU)
libstdc++是C++标准库的GCC的实现。Clang支持一个宽范围的libstdc++版本，从version4.2往前，并且将会隐式的解决了libstdc++老版本中的一些问题。
你可以使用-stdlib=libstdc++标识让Clang使用libstdc++。
