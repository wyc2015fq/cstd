# LLVM每日谈之二 LLVM IR - SHINING的博客 - CSDN博客
2012年10月02日 23:06:30[snsn1984](https://me.csdn.net/snsn1984)阅读数：22428
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
在介绍LLVM IR之前，我们需要先了解下LLVM的结构。传统的静态编译器分为三个阶段：前端、优化和后端。
![[Three Major Components of a Three-Phase Compiler]](https://img-my.csdn.net/uploads/201210/02/1349190533_4812.png)
LLVM的三阶段设计是这样的：
![[LLVM's Implementation of the Three-Phase Design]](https://img-my.csdn.net/uploads/201210/02/1349190617_1845.png)
这样做的优点是如果需要支持一种新的编程语言，那么我们只需要实现一种新的前端。如果我们需要支持一种新的硬件设备，那我们只需要实现一个新的后端。而优化阶段因为是针对了统一的LLVM IR，所以它是一个通用的阶段，不论是支持新的编程语言，还是支持新的硬件设备，这里都不需要对优化阶段做修改。所以从这里可以看出LLVM IR的作用。
LLVM IR主要有三种格式：一种是在内存中的编译中间语言；一种是硬盘上存储的二进制中间语言（以.bc结尾），最后一种是可读的中间格式（以.ll结尾）。这三种中间格式是完全相等的。
LLVM IR是LLVM优化和进行代码生成的关键。根据可读的IR，我们可以知道再最终生成目标代码之前，我们已经生成了什么样的代码。而且根据IR，我们可以选择使用不同的后端而生成不同的可执行代码。同时，因为使用了统一的IR，所以我们可以重用LLVM的优化功能，即使我们使用的是自己设计的编程语言。
如果想直观的看下llvm的IR到底是什么样的，可以先写一个helloworld的程序，文件名字叫做hello.c。
根据下列文档的步骤配置llvm:
http://clang.llvm.org/get_started.html
然后根据文档的内容使用下列命令可以得到二进制的.bc文件：
clang -emit-llvm -c hello.c -o hello.bc
然后使用下列命令可以得到对应的hello.ll文件，这个文件是可读的，可以通过文本编辑器打开。
llvm-dis hello.bc
这样我们就可以直观的得到IR的两种格式了，至于在内存中的那种格式，我们是无法通过文件的形式得到的。
我们一般都是查看.ll格式，因为这本身就是给人阅读的IR格式。
关于LLVM IR的介绍，LLVM专门有一个文档，文档的地址为：
[http://llvm.org/docs/LangRef.html](http://llvm.org/docs/LangRef.html)
