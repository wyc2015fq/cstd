# 七天LLVM零基础入门（Linux版本）------第六天 - SHINING的博客 - CSDN博客
2013年02月27日 09:43:52[snsn1984](https://me.csdn.net/snsn1984)阅读数：5675
所属专栏：[LLVM零基础学习](https://blog.csdn.net/column/details/llvm.html)
作者：[snsn1984](snsn1984)
第一步：对优化代码的pass继续进行深入的分析
首先检查一下第五天中的Pass练习，什么情况下可以进行优化，什么情况下不可以进行优化。
可以参照系统自带的Pass: [http://llvm.org/docs/doxygen/html/DCE_8cpp_source.html](http://llvm.org/docs/doxygen/html/DCE_8cpp_source.html)
需要将判断是否需要优化的主要函数
[isInstructionTriviallyDead](http://llvm.org/docs/doxygen/html/namespacellvm.html#abe283966f269761ee387642061c4451e)(I, TLI)
进行深入的分析。函数的具体内容位置：
[http://llvm.org/docs/doxygen/html/Local_8cpp_source.html#l00269](http://llvm.org/docs/doxygen/html/Local_8cpp_source.html#l00269)
将第五天要求的pass进一步进行完善。
第二步：学习LLVM Test
参照LLVM文档： [http://llvm.org/docs/TestingGuide.html](http://llvm.org/docs/TestingGuide.html)
此处需要注意的是，在llvm的build目录下，在已经运行make成功的情况下，直接进行make check，可以运行llvm
自带的所有测试。
此处可参考我的一个博文： [http://blog.csdn.net/snsn1984/article/details/8617068](http://blog.csdn.net/snsn1984/article/details/8617068)
第三步：写一个测试用例
写一个测试程序输出的测试用例。
可以测试最简单的hello world程序，测试该程序的输出的是不是程序本身期望输出的“hello world”。
并且可以使用llvm-lit去运行这个测试用例，保证测试用例PASS。

----------------------------------转载请注明出处----------------------------------------------------------------------------------
