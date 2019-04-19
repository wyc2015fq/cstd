# LLVM每日谈之四 Pass初探 - SHINING的博客 - CSDN博客
2012年11月08日 09:29:22[snsn1984](https://me.csdn.net/snsn1984)阅读数：12434
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
LLVM 的Pass框架是LLVM系统的一个很重要的部分。每个Pass都是做优化或者转变的工作，LLVM的优化和转换工作就是由很多个Pass来一起完成的。
所以按照我的理解，Pass就是LLVM系统转化和优化的工作的一个节点，每个节点做一些工作，这些工作加起来就构成了LLVM整个系统的优化和转化。Pass架构这么做的话，可重用性非常好，你可以选择已有的一些Pass，自己去构建出自己想要的优化和转化效果。并且自己也可以重新写Pass去做自己想要的优化和转变，因为每个Pass都可以独立存在，所以新建Pass并不用考虑LLVM之前的优化和转化是怎么做的，自己可以只运行自己新建的Pass，这样可以方便的实现自己想要的效果。
Pass最初的例子就在：llvm源码/lib/Transform/Hello/Hello.cpp。
可以采用命令
$ opt -load ../../../Debug+Asserts/lib/Hello.so -hello < hello.bc > /dev/null
去运行这个Pass，并且查看效果。
../../../Debug+Asserts/lib/Hello.so是so的目录，在llvm3.1中，这个例子的名称已经变成了LLVMHello.so. 存放在build目录/Release+Asserts/lib目录之下。如果编译的时候是Debug模式，则是在build目录/Debug+Asserts/lib目录之下。
-hello 是注册的使用Pass的时候的参数。可以在llvm源码/lib/Transform/Hello/Hello.cpp中看到。
hello.bc 是给Pass输入的二进制文件，也就是Pass要处理的二进制文件。至于如何生成该文件（使用命令：clang -emit-llvm -c hello.c -o hello.bc, hello.c就是个输出helloworld的简单C语言程序），在llvm每日谈之二 LLVM IR中有介绍，后续介绍clang的时候还会继续介绍。
所有的Pass都是继承自Pass类的。Pass类的API文档地址：
http://llvm.org/docs/doxygen/html/classllvm_1_1Pass.html
如果想仔细了解这个类的同学可以去这里看下。这里的函数都是可以通过点击跳转到源码中去的。
几个常用的Pass类的子类：
The ImmutablePass class
The ModulePass class
The CallGraphSCCPass class
The FunctionPass class
The LoopPass class
The RegionPass class
The BasicBlockPass class
The MachineFunctionPass class
 推荐文档： http://llvm.org/docs/WritingAnLLVMPass.html该文档是LLVM官方介绍Pass的文档，英文的，英文不错的同学推荐直接看这个文档。
