# Clang Static Analyzer：源代码分析工具（C、C++和Objective-C） - 资源 - 伯乐在线

# Clang Static Analyzer：源代码分析工具（C、C++和Objective-C）

**[Clang Static Analyzer](http://hao.jobbole.com/tag/clang-static-analyzer/) , [C语言](http://hao.jobbole.com/tag/c/) , [静态代码分析](http://hao.jobbole.com/tag/%e9%9d%99%e6%80%81%e4%bb%a3%e7%a0%81%e5%88%86%e6%9e%90/)**[4](http://hao.jobbole.com/clang-static-analyzer/#comments)


本资源由 [伯乐在线](http://www.jobbole.com) - [Juliesand](http://www.jobbole.com/members/Juliesh) 整理

[![clang-analyzer](http://jbcdn1.b0.upaiyun.com/2016/12/1dfce5c475ac2617abe109b22c602d84.png)](http://jbcdn1.b0.upaiyun.com/2016/12/1dfce5c475ac2617abe109b22c602d84.png)

Clang Static Analyzer是一个源代码分析工具，可以找出C、C++和Objective-C程序中的错误。目前可以作为独立的工具运行，也可以在开发工具中运行。如果是独立的工具，在命令行调用，使它和一些基本代码库串联运行。

这个分析器是Clang项目的一部分，100%开源。同Clang的其他部分一样，分析器被开发为一个C++库，可以被其他工具和应用使用。

### 什么是静态分析

静态分析这个词是合并在一起的，但是这里我们使用它表示算法和技术的集合，这些技术可以分析源代码，并自动找出错误。这个思想和编译器的警告（它对于找出代码错误是有帮助的）相似，但是更进一步来说，查找错误传统意义上使用运行时调试技术，例如测试。

最近几十年，静态分析错误找出工具从基本分析检查器进化为通过推理代码的语义找出深层的错误。Clang Static Analyzer的目标是为分析C、C++和Objective-C程序提供一个工业品质静态分析框架，它可以免费获取、可扩展，并且高质量完成。

### Clang和LLVM的部分

正如其名称所示，Clang Static Analyzer在Clang和LLVM顶层生成。严格来说，这个分析器是Clang的一部分，因为Clang包含一套可重复使用的C++库，用来生成强大的源码级工具。Clang Static Analyzer使用的这个静态分析引擎是一个Clang库，可以在不同的环境和客户端中被重复使用。

## 要点和思考

当我们相信使用静态分析器寻找错误已经非常有效时，我们要求你在使用它的时候记住几点。

### 工作进展

分析器是一个连续的工作过程。现在有许多增强设计既可以提高分析算法的精度和范围，又可以找出各种错误。虽然关于静态分析器可以做什么有一些基本的限制，但是我们在到达那里之前仍然有很长一段路要走。

### 比编译慢

操作上，使用静态分析器自动找出深层程序错误是CPU时间和代码硬化的交易。由于深度分析被最先进的静态分析工具完成，静态分析可能比编译慢很多。

当Clang静态分析器被设计为尽可能快和轻量级时，请不要期望它和编译程序一样快（甚至启用最优化）。在最坏的情况下，一些被用来找出错误的算法执行时间成指数倍增长。

Clang静态分析器运行在合理的时间内，它将大量的检查工作捆绑在一起，同时使用智能算法降低大量寻找错误所必须做的工作。

### 误检

静态分析器是不完美的。它可能在代码正确的程序中错误的检测出漏洞。因为一些代码检测比另一些需要更精确的分析，所以误检率在不同的检测中变化很大。我们的长期目标是有一个分析器，它对大多数代码做所有的检测都有低的误检率。

### 更多检查

静态分析器不是魔法，一个静态分析器只能找出那些被明确设计为需要查找的漏洞。如果你希望Clang静态分析器查找一些特殊类型的漏洞，请在这里查找或者贡献你自己的补丁包：[http://clang-analyzer.llvm.org/filing_bugs.html](http://clang-analyzer.llvm.org/filing_bugs.html)

## 最新版本

在官网首页即可下载最新版本。
- 版本：10.8
- 发布日期：2016年11月14日
- Platform：Mac OS X

## 安装

### 软件包（Mac OS X）

半规则预编译二进制分析器可以在Mac OS X平台上使用，但是仅可以在OS X10.7或之后的版本上运行。

软件包会经常发布，不同版本号之间的差异是修复几个漏洞或者提升一些微小的性能。当使用分析器时，建议偶尔去官网查看是否有新的版本发布，尤其是你使用它超过几周时。

### 其他平台（编译分析器源文件）

对于其他平台，必须手动编译Clang和LLVM。请根据网站的提示操作：[http://clang.llvm.org/get_started.html#build](http://clang.llvm.org/get_started.html#build)

一旦Clang编译完成，需要在你的路径添加：
- Clang二进制包的位置。

例如，如果编译一个基于LLVM/Clang（默认情况）的Debug+Assert，合成的clang二进制文件将在$(OBJDIR)/Debug+Asserts/bin（这里的$(OBJDIR)通常在源文件根目录）。你也可以做make install安装LLVM/Clang库和二进制文件到你选择的安装目录（当你运行configure时指定的目录）。
- Scan-build和scan-view程序的位置。

在clang生成时通过make install将这两个程序安装在bin目录。

## 示例

想要使用软件包，只需简单的将它解压到任何地方。如果软件存档的名字是checker-XXX.tar.bz2，那么它将扩展出一个目录checker-XXX。你不需要在任何特殊的地方创建这个目录或者这个目录的内容。卸载这个分析器像删除这个目录一样简单。

在checker-XXX目录下的大部分文件将是分析器的支持文件，你可以简单地忽略它们，大部分用户只关心两个文件，它们位于checker-XXX目录的顶部。

Scan-build：scan-build是运行分析器的高级命令行工具。

Scan-view: scan-view是与scan-build相伴的命令行工具，用来查看scan-build产生的分析结果。有一个选项是可以传给scan-build一个命令使scan-view在分析完成时立刻运行。

Scan-build命令可以用于分析整个项目，本质上是介入项目的编译过程。这意味着使用scan-build运行分析器，你将使用scan-build分析在项目编译期间由gcc/clang编译的源文件。这也意味着任何没有编译的文件将不会被分析。

Scan-build的基本用法被设计的很简单：只是将字符“scan-build”放在编译命令前面：



























C++





$ scan-build make
$ scan-build xcodebuild

|12|$scan-build make$scan-build xcodebuild|
|----|----|



第一个命令scan-build分析make编译的项目代码；第二个命令是scan-build分析使用xcodebuild编译的项目。

这里是一个调用scan-build的通用格式



























C++





$ scan-build [scan-build options] <command> [command options]

|1|$scan-build[scan-build options]<command>[command options]|
|----|----|



在操作上，scan-build确实运行<command> 和所有在其之后传给它的选项。例如，一个命令可以是传给make命令-j4，实现超过4核的并行编译。



























C++





$ scan-build make -j4

|1|$scan-build make-j4|
|----|----|



Windows用户必须安装脚本才能使用scan-build。Scan-build.bat脚本允许用户启动scan-build，方式和上面介绍的基本用法相同。为了在任意位置调用scan-build，只需添加包含scan-build.bat的文件夹路径至环境变量即可。

## 资源链接
- [Clang](http://clang.llvm.org/)
- [LLVM](http://llvm.org/)
- [官方文档](http://clang-analyzer.llvm.org/scan-build)
- [检查项](http://clang-analyzer.llvm.org/available_checks.html)
- [常见问题](http://clang-analyzer.llvm.org/faq.html)

官方网站：[http://clang-analyzer.llvm.org/](http://clang-analyzer.llvm.org/)
开源地址：[https://github.com/search?p=1&q=Clang+Static+Analyzer&type=Repositories&utf8=%E2%9C%93](https://github.com/search?p=1&q=Clang+Static+Analyzer&type=Repositories&utf8=%E2%9C%93)

