# LLVM每日谈之二十一  一些关于编译器和LLVM/Clang的代码 - SHINING的博客 - CSDN博客
2014年11月18日 10:36:23[snsn1984](https://me.csdn.net/snsn1984)阅读数：4141
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[史宁宁（snsn1984）](http://blog.csdn.net/snsn1984)
自己收集了一些LLVM/Clang的代码，并且自己也在一点一点的写一些LLVM/Clang以及编译器的代码，在这里把这些代码库分享出来，欢迎大家交流探讨。
1.crange
[https://github.com/shining1984/crange](https://github.com/shining1984/crange)
这是一个python写的调用libclang去定位和交叉检索C/C++源码的工具。这个工具不是我写的，我给fork过来了，方便自己加一些自己的代码。
2. libclang-examples
[https://github.com/shining1984/libclang-examples](https://github.com/shining1984/libclang-examples)
这是我自己写的使用python去调用libclang的几个简单的例子，后续会逐渐添加。
3.screader
[https://github.com/shining1984/screader](https://github.com/shining1984/screader)
自己写的一个工具叫screader，是用C语言实现的调用libclang去实现代码交叉索引的工具，还有待进一步完善。
4.llvm-clang-samples
[https://github.com/shining1984/llvm-clang-samples](https://github.com/shining1984/llvm-clang-samples)
Eli写的一些关于如何使用LLVM/Clang中的库以及内部机制的一些例子。Eli对LLVM/Clang有很深的研究，并且有着大量的博文，他的博客地址：eli.thegreenplace.net
5. compiler_exercise_code
[https://github.com/shining1984/compiler_exercise_code](https://github.com/shining1984/compiler_exercise_code)
我自己写的简单编译器的代码。目前已经实现了一个简单版本的词法分析器，后续继续实现中。这个代码是用C++实现的，并不是为了真正的实现编译器，主要是为了通过简单的代码去展示编译器的大致处理过程。
6. llvm-examples
[https://github.com/shining1984/llvm-examples](https://github.com/shining1984/llvm-examples)
这个代码库也是我自己刚刚建立的，刚刚上传了一个例子。这里打算要放一些基于LLVM构建的例子或者简单的项目，主要是为了演示如何基于LLVM构建项目，以及如何使用LLVM的库和IR。
