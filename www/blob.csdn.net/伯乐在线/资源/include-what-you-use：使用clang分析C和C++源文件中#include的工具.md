# include-what-you-use：使用clang分析C和C++源文件中#include的工具 - 资源 - 伯乐在线

# include-what-you-use：使用clang分析C和C++源文件中#include的工具

**[C语言](http://hao.jobbole.com/tag/c/) , [include-what-you-use](http://hao.jobbole.com/tag/include-what-you-use/) , [静态代码分析](http://hao.jobbole.com/tag/%e9%9d%99%e6%80%81%e4%bb%a3%e7%a0%81%e5%88%86%e6%9e%90/)


本资源由 [伯乐在线](http://www.jobbole.com) - [Juliesand](http://www.jobbole.com/members/Juliesh) 整理

[![include-what-you-use](http://jbcdn1.b0.upaiyun.com/2016/11/b45e1f31b534c15d3b546538967f761d.png)](http://jbcdn1.b0.upaiyun.com/2016/11/b45e1f31b534c15d3b546538967f761d.png)

Include-what-you-use是一个使用clang分析C和C++源文件中#include的工具。

“Include what you use”意味着：针对foo.cxx中使用的每一个符号（类型、函数变量或者宏），foo.cc或者foo.h应该#include一个.h文件，用于输出那些符号的声明。Include-what-you-use工具是一个程序，可以和clang库一起编译，目的是分析源文件的#includes，找出违反include-what-you-use规则的地方，然后给出它们的修改建议。

Include-what-you-use的主要目的是移除过多的#includes。它做了两件事，一个是解决哪个#includes实际上是这个文件不需要的（包含.cc和.h两个文件）；另一个是如果可能的话使用前置声明代替#includes。

这使我们处在这样一种状态：每个文件都包含一些头文件声明其使用的符号。当每个文件都包含它使用的头文件时，就可以编辑任何文件，移除不使用的头文件，不用害怕意外破坏文件向上的依赖性。也使得在源文件中自动跟踪和更新依赖性变得容易。

# 警告

这最多是一个alpha质量软件（自2011年2月起）。它被创造出来特定用于Google源码树中，可能有些差距，但是可以假设这在其他类型代码中是显然的，尽管有些直接和令人尴尬。例如，我们只在C++代码上运行它，而不在C或者Objective C中运行。甚至对于Google的代码，这个工具依然有许多错误。

当我们使IWYU的质量提升时，我们将限制新的特征，并优先报告许多已经存在的错误，即已知的错误。修复错误最好的机会是提交修复它的补丁（和确认这个修复的单元测试实例一起提交）。

# 最新版本 
- 版本：0.7
- 发布日期：2016年10月30日

# 安装

如果在源码树外编译IWYU或者安装预编译二进制包，需要确保编译器能找到Clang内嵌的头文件（stdarg.h和友元）。

Clang的默认路径是在path/to/clang-executable/../lib/clang/<clang ver>/include中查看。所以如果Clang 3.5.0被安装在/usr/bin，它将在/usr/lib/clang/3.5.0/include搜索内嵌头文件。

Clang工具有相同的默认设置，所以为了让IWYU分析有价值的代码，需要在path/to/iwyu/../lib/clang/3.5.0/include 找到内嵌头文件，其中3.5.0表示当前编译的IWYU所基于的Clang版本。

所以，为了使IWYU功能正常，需要在运行前将Clang头文件拷贝到恰当的位置。

这个怪异行为可以在issue100（https://github.com/include-what-you-use/include-what-you-use/issues/100）中追踪，希望随着时间的推移我们可以让它更易识别。

# 示例

运行IWYU检测基本代码最简单的方法是运行下面这个命令。



























C++





make -k CXX=/path/to/llvm/Debug+Asserts/bin/include-what-you-use

|1|make-kCXX=/path/to/llvm/Debug+Asserts/bin/include-what-you-use|
|----|----|



或者



























C++





make -k CXX=/path/to/llvm/Release/bin/include-what-you-use

|1|make-kCXX=/path/to/llvm/Release/bin/include-what-you-use|
|----|----|



（Include-what-you-use总是在退出时给出错误代码，所以编译系统知道它没有编译.o文件。因此需要命令-k。）

Include-what-you-use只分析被make编译的.cc（或者.cpp）文件和它们对应的.h文件。如果你的项目有.h文件，但是没有对应的.cc文件，IWYU将忽略它，除非使用–check_also开关使它和一个.cc文件一起分析。

在这个目录中，我们还包含一个工具，可以根据IWYU的建议自动修复源文件。这也是alpha质量软件一种功能。下面是怎么使用它（需要Python）：



























C++





make -k CXX=/path/to/llvm/Debug+Asserts/bin/include-what-you-use > /tmp/iwyu.out
python fix_includes.py < /tmp/iwyu.out

|12|make-kCXX=/path/to/llvm/Debug+Asserts/bin/include-what-you-use>/tmp/iwyu.outpython fix_includes.py</tmp/iwyu.out|
|----|----|



如果你不希望fix_includes.py修改文件中的#include行，可以通过标志控制它的行为。

fix_includes.py –help将列出完整的清单，下面是几个常用的命令：
- -b: 在系统和Google包含文件之间留出空行
- –nocomments: 不要在包含文件旁边添加‘why’评论。

# 资源链接
- [教程](https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/InstructionsForUsers.md)
- [下载地址](http://include-what-you-use.org/downloads/)
- [下载 Clang](http://clang.llvm.org/get_started.html)
- [开发文档](https://github.com/include-what-you-use/include-what-you-use/tree/master/docs)
- [为什么要使用Include What You Use](https://github.com/include-what-you-use/include-what-you-use/blob/master/docs/WhyIWYU.md)
- [讨论组](http://groups.google.com/group/include-what-you-use)

官方网站：[http://include-what-you-use.org/](http://include-what-you-use.org/)
开源地址：[https://github.com/include-what-you-use/include-what-you-use](https://github.com/include-what-you-use/include-what-you-use)

