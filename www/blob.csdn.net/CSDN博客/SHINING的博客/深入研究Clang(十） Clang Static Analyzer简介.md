# 深入研究Clang(十）  Clang Static Analyzer简介 - SHINING的博客 - CSDN博客
2017年03月29日 16:27:49[snsn1984](https://me.csdn.net/snsn1984)阅读数：3433
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
Clang Static Analyzer 官网地址：[http://clang-analyzer.llvm.org/](http://clang-analyzer.llvm.org/)
Clang Static Analyer是一个源码分析工具，它可以发现C、C++和Objective-C程序中的bug。现在Clang Static Analyer既可以作为一个单独的工具使用，也可以集成在Xcode中使用。作为单独的工具使用的时候，可以通过命令行调用，作为一个代码库编译的时候的一个串行的动作运行。Clang Static Aanlyer是百分百开源的项目，是Clang项目的一部分。像Clang项目剩下的部分一样，它是被实现成了一个C++的库的形式，这样的话它可以被其他的工具和应用调用。（注：该段文字基本上是对官网的一个介绍的翻译，并无太多个人 的理解，因为这块官网已经介绍的比较简明了。）
Clang Static Analyer作为单独工具被调用的时候，就涉及到了scan-build。
scan-build的官网地址：[http://clang-analyzer.llvm.org/scan-build.html](http://clang-analyzer.llvm.org/scan-build.html)
官网对scan-build的解释就是一个确保用户可以在他们的代码库上运行static analyzer的命令行实体。scan-build在正常编译clang之后就会生成，是和clang同目录的一个可执行文件，如果已经make install的话，那么在系统里可以随意直接调用scan-build。我在虚拟机上针对我的一个小程序，简单调用了一下scan-build，在我自己的待分析的项目目录下 输入命令行：
scan-build make 
得到了一个结果如下：
scan-build: Using '/usr/local/bin/clang-3.9' for static analysis
scan-build: Removing directory '/tmp/scan-build-2017-03-29-152946-3220-1' because it contains no reports.
从结果也可以看出，并没有发现什么错误，并且声称了一个未包含错误报告的目录。
