# LLVM 3.3完全兼容C++11 - SHINING的博客 - CSDN博客
2013年12月08日 15:46:16[snsn1984](https://me.csdn.net/snsn1984)阅读数：1796
作者 [Jeff
 Martin](http://www.infoq.com/cn/author/Jeff-Martin) ，译者 [臧秀涛](http://www.infoq.com/cn/author/%E8%87%A7%E7%A7%80%E6%B6%9B)发布于
 七月 01, 2013
最近发布的LLVM 3.3带来了大量新特性，其中包括通过[Clang子项目](http://www.infoq.com/news/2013/04/clang_CPP11)实现的完全兼容C++11的编译器。最重要的新特性是支持一些新的CPU架构，包括AArch64（64位ARM）、[AMD的R600
 GPU](http://en.wikipedia.org/wiki/R600_%28ASIC%29)和IBM的z/Architecture（s390x），再就是改进了对MIPS和PowerPC的支持。
LLVM有两种向量识别器（vectorizer）：循环向量识别器和新增的SLP向量识别器。在3.3版本中，得益于一些改进，循环向量识别器能够生成更好的代码，在使用–O3标记时会默认启用该特性。新添加的特性包括指针的运行时检查、反向迭代器和If转换。SLP（superword-level parallelism）向量识别器可以将“类似的独立指令合并为向量指令”。SLP向量识别器通过“-fslp-vectorize”标记启用。
[Dragon Egg](http://dragonegg.llvm.org/)是LLVM的子项目，它将LLVM技术带向了GCC。DragonEgg将GCC的优化器和代码生成器替换成了LLVM的。在3.3版本中还有一些值得注意的改进，如支持gcc-4.8.1及更高版本，提供了更好的调试信息等。LLDB（Low
 Level Debugger）添加了一些Linux特性：
> - 
支持观察点
- 
通过一个vim插件，在vim中集成了lldb命令和程序状态
- 
改进了寄存器支持，其中包括向量寄存器
- 
可以使用make/ninja/auto-tools/clang 3.3/gcc 4.6构建
使用C++11的开发者，或考虑将C++11应用于现有项目的开发者，可以研究一下LLVM的C++11迁移工具——cpp11-migrate。按照[LLVM博客](http://blog.llvm.org/2013/04/status-of-c11-migrator.html)上的说法，“C++11迁移程序的意图是通过从源代码到源代码的翻译，将现有的C++代码迁移为使用C++11特性的代码，以增强可维护性、可读性、运行时性能和编译时性能。”
[LLVM 3.3](http://llvm.org/releases/3.3/docs/ReleaseNotes.html)以多种格式发布，其中包括源代码和预先构建好的二进制包。Linux、FreeBSD和Mac OS X用户可以直接从项目网站[下载安装程序](http://llvm.org/releases/download.html#3.3)。作为选择，也可以检查操作系统发行包，或从源代码编译。LLVM团队提供了[详细的编译说明](http://llvm.org/docs/GettingStarted.html)。
查看英文原文：[LLVM 3.3 Achieves
 Full C++11 Compliance](http://www.infoq.com/news/2013/06/llvm33)
