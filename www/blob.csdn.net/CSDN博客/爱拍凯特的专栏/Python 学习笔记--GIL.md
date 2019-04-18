# Python 学习笔记--GIL - 爱拍凯特的专栏 - CSDN博客
2017年04月19日 00:13:44[爱拍凯特](https://me.csdn.net/apacat)阅读数：170
1、python 全局翻译锁（GIL Global Interpreter Lock）
为了实现在多核环境下的运行时数据的一致性，Python采用加锁的思想来实现这种同步和一致性，这把锁就是GIL。这造成了一定量的性能损失，使用锁的原因是因为Python解释器内部是线程不安全的。
另外一点，首先需要明确的一点是 GIL 并不是Python的特性，它是在实现Python解析器(CPython)时所引入的一个概念。就好比C++是一套语言（语法）标准，但是可以用不同的编译器来编译成可执行代码。有名的编译器例如GCC，INTEL C++，Visual C++等。Python也一样，同样一段代码可以通过CPython，PyPy，Psyco等不同的Python执行环境来执行。像其中的JPython就没有GIL。然而因为CPython是大部分环境下默认的Python执行环境。所以在很多人的概念里CPython就是Python，也就想当然的把 GIL 归结为Python语言的缺陷。所以这里要先明确一点：GIL并不是Python的特性，Python完全可以不依赖于GIL
Python的多线程虽然是真正的多线程，并非虚拟出来的多线程，但是由于GIL的存在导致了python在多线程运行程序的时候，有可能会比单线程运行还要慢，但是可以采用多进程的方式来执行多任务，因为多进程的时候每个进程都有一个GIL，互相不影响。
