# LLVM每日谈之十二 LLVM的源码分析之Pass相关 - SHINING的博客 - CSDN博客
2013年03月18日 14:41:28[snsn1984](https://me.csdn.net/snsn1984)阅读数：6181
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
题记：在学习LLVM的过程中，要想学的更加深入，掌握更多的技能，LLVM的源码是必须要读的，但是在这么多的源码中，从哪里下手？很容易让人找不到头脑，本文这里就先拿出几个Pass相关的源码文件进行分析。
1.Pass.h
源码中的位置：llvm源码/include/llvm/Pass.h
网络上位置： [http://llvm.org/docs/doxygen/html/Pass_8h_source.html](http://llvm.org/docs/doxygen/html/Pass_8h_source.html)
此头文件的内容：此头文件主要对和Pass相关的几个类进行了声明。主要有Pass、ModulePass、ImmutablePass、FunctionPass、BasicBlockPass这几个类。之前在关于Pass的博文里也介绍过一些和Pass相关的知识，而这几个类正是LLVM的Pass框架所最常用的几个类，在Pass的编写或者修改的工作中，基本上都是在和这几个类打交道，所以对于这几个类要想有一个深的了解，这个头文件是必须读的。
ModulePass、ImmutablePass、FunctionPass、BasicBlockPass这几个类都是Pass的子类，也是在写Pass的时候最容易直接用到的类。自己编写LLVM的Pass的时候，使用的最多是FunctionPass和ModulePass这两个。而这两个之中，FunctionPass要使用的比ModulePass更为多一点，因为如果不删除函数，一般都是处理函数内部的信息，那么使用FunctionPass就可以了。其实根据本人的经验，还有一种情况是需要使用ModulePass的，那就是在需要处理MetaData的时候，这种情况之下也会用到ModulePass的，因为Metadata是在函数之外的。
2.Pass.cpp
源码中的位置：llvm源码/lib/VMCore/Pass.cpp
网络上的位置：[http://llvm.org/docs/doxygen/html/Pass_8cpp_source.html](http://llvm.org/docs/doxygen/html/Pass_8cpp_source.html)
此文件主要是对Pass.h中声明的几个类的实现。除此之外，还有对RegisterAGBase、PassRegistrationListener、AnalysisUsage这三个类的实现。而这三个类的声明并不在刚才介绍的Pass.h之中，而是另有别的头文件，此处先不做介绍。
3.PassSupport.h
源码中的位置：llvm源码/include/llvm/PassSupport.h
网络上的位置：[http://llvm.org/docs/doxygen/html/PassSupport_8h_source.html](http://llvm.org/docs/doxygen/html/PassSupport_8h_source.html)
此头文件被包含在Pass.h这个头文件之中，所以如果文件已经包含了Pass.h，其实就也可以访问PassSupport.h。这个文件中主要是PassInfo、RegisterPass、RegisterAGBase、RegisterAnalysisGroup、PassRegistrationListener这几个类的声明。
PassInfo类的每一个对象都对应着一个实际存在的Pass，并且保存着这个Pass的信息。
RegisterPass这个类是一个模板类，这个模板类的类型就是Pass的名字。它是PassInfo的子类，主要用来注册Pass。完成注册之后，在PassManager管理的内部数据库里才能找到这个Pass。需要注意的是，这个模板类的使用必须是在全局范围之内的。可以从最简单的Pass例子--Hello（[http://llvm.org/docs/doxygen/html/Hello_8cpp_source.html](http://llvm.org/docs/doxygen/html/Hello_8cpp_source.html)）中去看这个模板类的使用方法。
RegisterAGBase是RegisterAnalysisGroup类的基类，而RegisterAGBase类又是PassInfo类的子类。其中RegisterAGBase类名字中的AG就是AnalysisGroup的缩写，这种命名方式在LLVM的源码中被大量的应用，比如MetaData在一些类的名字里就被缩写为MD。RegisterAnalysisGroup这个类的作用主要是将一个Pass注册成为一个分析组的成员，当然在进行此操作之前，这个Pass必须被首先注册Pass成功。一个Pass可以被注册到多个分析组中。同一个Pass在多个分析组中，依然是根据这个Pass的名字进行标识的。
PassRegistrationListener这个类主要负责在运行时时候Pass的注册与否，并且会在Pass被load和remove的时候，去调用回调函数。
4. PassAnalysisSupport.h
源码中的位置：llvm源码/include/llvm/PassAnalysisSupport.h
网络上的位置：[http://llvm.org/docs/doxygen/html/PassAnalysisSupport_8h_source.html](http://llvm.org/docs/doxygen/html/PassAnalysisSupport_8h_source.html)
此头文件被包含在Pass.h这个头文件的尾部，所以如果文件已经包含了Pass.h，其实就也可以访问PassAnalysisSupport.h。这个头文件之中，主要是AnalysisUsage和AlysisResolver两个类的声明以及Pass类的部分函数的实现。
其中，AnalysisUsage主要负责表示分析出来的使用信息。
AlysisResolver是一个Pass对象用来从Pass Manager获取所有的分析信息的简单接口。
另外，Pass类的函数getAnalysisIfAvailable()、getAnalysis()、getAnalysisID()这三个常用的函数，也都是在这个头文件中使用的。Pass之间的交互，是会经常用到这样的函数的。
--------------------------------------------------------------------------转载请注明出处---------------------------------------------------------------------------------------------
