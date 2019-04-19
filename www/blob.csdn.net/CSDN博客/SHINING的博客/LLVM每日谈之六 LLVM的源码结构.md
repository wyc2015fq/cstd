# LLVM每日谈之六 LLVM的源码结构 - SHINING的博客 - CSDN博客
2012年11月11日 09:07:44[snsn1984](https://me.csdn.net/snsn1984)阅读数：8661
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
 作者：[snsn1984](http://blog.csdn.net/snsn1984)
这里简单介绍下LLVM的源码结构，让大家知道LLVM的每个文件夹都放的是什么内容。
参考文档： http://llvm.org/docs/GettingStarted.html#getting-started
llvm/examples
这个目录主要是一些简单例子，演示如何使用LLVM IR 和 JIT。还有建立一个简单的编译器的例子的代码。
建立简单编译器的文档地址：http://llvm.org/docs/tutorial/
llvm/include
这个目录主要包含LLVM library的公共头文件。
llvm/lib
这个目录包含了大部分的LLVM的源码。在LLVM中大部分的源码都是以库的形式存在的，这样不同的工具之前就很容易共用代码。
llvm/projects
这个目录包含着一些依赖LLVM的工程，这些工程严格来说又不算LLVM一部分。
llvm/runtimes
这个目录包含了一些库，这些库会编译成LLVM的bitcode，然后当clang linking 程序的时候使用。
llvm/test
这个目录是LLVM的测试套件，包含了很多测试用例，这些测试用例是测试LLVM的所有基本功能的。
llvm/tools
这个目录理是各个工具的源码，这些工具都是建立在刚才上面的那些库的基础之上的。也是主要的用户接口。
llvm/utils
这个目录包含了一些和LLVM源码一起工作的应用。有些应用在LLVM的编译过程中是不可或缺的。
