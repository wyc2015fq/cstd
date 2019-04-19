# 深入研究Clang（二）Abstract Syntax Tree - SHINING的博客 - CSDN博客
2014年01月07日 16:01:11[snsn1984](https://me.csdn.net/snsn1984)阅读数：8639
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
要想深入的研究Clang，抽象语法树是必须要完全理解的。关于抽象语法树的资料，wiki介绍的很详细：[http://en.wikipedia.org/wiki/Abstract_syntax_tree](http://en.wikipedia.org/wiki/Abstract_syntax_tree)
如果觉的这个英文文档不是太好理解，也可以找点别的中文资料，但是这个文档还是要认真读的。AST作为语法分析的输出，是接下来的语义分析和生成中间代码所需要的基础。现在基于AST可以进行程序分析和程序转换，程序分析就不说了。程序转换，现在Clang下面已经有一些额外的工具：比如clang-modernize实现的就是对C++程序中可以使用C++11的地方进行转换，转换成在这些地方使用C++11特性的程序，等于直接在程序上进行的修改，是一个source-to-source的转换工具。
同时，AST也是编译器构建符号表的基础。
在这里先提出对AST一定要熟悉，因为它是编译器的几大基础之一，尤其是前端中的一个很重要的一部分，如果连AST也不熟悉，后续的所有工作都无法进行下去了。同时，也是为了后续从工具clang-modernize入手去深入研究Clang打下基础。在另外一篇博文中曾经提到过打算深入研究Clang的方法，就是从这些工具入手，不断的去挖掘Clang的源码。因为工具本身的代码量不大，方便阅读代码和调试，而从这些代码入手去挖掘Clang的代码，既不会觉的面对的东西太大无法入手，也比较容易看到成果。
这种方法的思路是不同的从外围去入手，然后渐进的向核心逼近，等于是农村包围城市的策略。如果能耐得住寂寞，同时也有大把的整块时间，可以直接去hack Clang的那些库的源码，这样更直接，但是会很累。
关于Clang AST，有一个官方文档：[http://clang.llvm.org/docs/IntroductionToTheClangAST.html](http://clang.llvm.org/docs/IntroductionToTheClangAST.html)
通过对这个文档的阅读，可以熟悉Clang的AST是什么样的，它和其他的编译器产生的AST是有不同点的，这些需要在阅读文档的过程中进行把握。而不应该想当然的去以为AST都是相同的。

