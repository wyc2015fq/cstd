# LLVM每日谈之八 PASS例子分析 - SHINING的博客 - CSDN博客
2013年02月04日 14:48:37[snsn1984](https://me.csdn.net/snsn1984)阅读数：6502
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
之前已经 简单的介绍过怎么样去运行PASS（地址： [http://blog.csdn.net/snsn1984/article/details/8160696](http://blog.csdn.net/snsn1984/article/details/8160696)），并且以hello PASS为例进行了运行，可以输出hello 和函数的名字。下面咱们继续进一步的分析一下这个PASS的具体内容：
[http://llvm.org/docs/doxygen/html/Hello_8cpp_source.html](http://llvm.org/docs/doxygen/html/Hello_8cpp_source.html)
这里就是之前运行过的pass的源码，可以看出来，输入的主要是在下面的代码中实现的
00032       errs() << "Hello: ";
00033       errs().write_escaped(F.getName()) << '\n';
这里的首先输出了：Hello，然后通过LLVM：：Function类的getName()的成员函数取得了函数的名字进行输出。这两行代码是在
virtualbool runOnFunction([Function](http://llvm.org/docs/doxygen/html/classllvm_1_1Function.html) &[F](http://llvm.org/docs/doxygen/html/LLParser_8cpp.html#a33ece1ef8074506a15d7f86eb76dbae6))
这个函数里面的，而这个函数是结构体Hello的成员函数。并且我们也可以通过
struct Hello : public[FunctionPass](http://llvm.org/docs/doxygen/html/classllvm_1_1FunctionPass.html)
可以知道，Hello是继承了类FunctionPass.
virtualbool runOnFunction([Function](http://llvm.org/docs/doxygen/html/classllvm_1_1Function.html) &[F](http://llvm.org/docs/doxygen/html/LLParser_8cpp.html#a33ece1ef8074506a15d7f86eb76dbae6))
其实就是FunctionPass的一个虚函数，这里对它进行了实现。一个FunctionPass的子类要想做一些实际的工作，就必须对这个虚函数进行实现。
00040 static[RegisterPass<Hello>](http://llvm.org/docs/doxygen/html/structllvm_1_1RegisterPass.html)[X](http://llvm.org/docs/doxygen/html/namespacellvm.html#a6569d00d31a5d74e563e2f39f82271ab)("hello", "Hello World Pass");
这个语句的主要意思是对hello PASS进行注册，只有PASS进行了注册之后，才可以进行使用。
--------------------------------------------------转载请注明出处-----------------------------------------------------------------------------
