# LLVM每日谈之十 LLVM Test简介 - SHINING的博客 - CSDN博客
2013年02月27日 11:55:04[snsn1984](https://me.csdn.net/snsn1984)阅读数：5324
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
就像很多优秀的软件都有自己的测试框架一样，LLVM也有自己的测试框架。LLVM的测试主要分为两
部分：regression tests 和 whole programs。其中，regression tests主要位于LLVM源码/test目录下，主要是
用来测试LLVM的一些特性或者是用来测试一些bug的。我们在平时的工作中，一般使用的就是这个类型的test。
whole programs这个类型的测试，不在LLVM的源码中，在SVN库上有自己的单独目录，一般称之为test-suit，
它的测试范围要广的多，因为我们目前这里不涉及，所以这里不做过多介绍。
        运行regression tests的方法比较简单，但是必须有已经编译好的LLVM源码。在build目录下，运行make check，
这样就可以运行所有的LLVM源码/test目录下的regression tests。如果已经编译好的LLVM源码下还包含了Clang，也
就是说在编译LLVM之前，已经将Clang的远方放置到LLVM源码/tools目录下了，那么在编译成功之后，就可以使用
make check-all，这样可以运行LLVM和Clang的所有的regression tests。
        如果需要运行单个的测试用例，可以使用llvm-lit工具。直接使用llvm-lit工具，后面跟上测试用例的位置和名字，
就可以单独运行这个测试。或者在llvm-lit后面跟上一个测试目录，llvm-lit可以运行这个目录下的所有测试用例。
如果在运行llvm-lit的三个时候碰到了bash: ./llvm-lit: Permission denied 这个错误，那么可以使用$>chmod +x llvm-lit命令。
LLVM官方介绍测试的文档：[http://llvm.org/docs/TestingGuide.html](http://llvm.org/docs/TestingGuide.html)
LLVM官方介绍lit的文档：[http://llvm.org/docs/CommandGuide/lit.html](http://llvm.org/docs/CommandGuide/lit.html)

---------------------------------------------------转载请注明出处----------------------------------------------------------------------------
