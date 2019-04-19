# 七天LLVM零基础入门（Linux版本）------第四天 - SHINING的博客 - CSDN博客
2013年02月25日 09:31:35[snsn1984](https://me.csdn.net/snsn1984)阅读数：8277
所属专栏：[LLVM零基础学习](https://blog.csdn.net/column/details/llvm.html)
作者：[snsn1984](snsn1984)
第一步：复习
第三天的时候，我们学习了LLVM的编程指引。在开始第四天的学习之前，需要复习之前学习过的两篇文档：
LLVM IR的文档（[http://llvm.org/docs/LangRef.html](http://llvm.org/docs/LangRef.html)）
编程指引文档 （[http://llvm.org/docs/ProgrammersManual.html](http://llvm.org/docs/ProgrammersManual.html)）
这两篇文档是LLVM的重中之重，希望学习的时候认真一点，能掌握这些文档的核心。
第二步：阅读文档 Write an LLVM pass
[http://llvm.org/docs/WritingAnLLVMPass.html](http://llvm.org/docs/WritingAnLLVMPass.html)
在阅读的过程中，文档中提到的例子要自己认真运行一下，并且对提到的实际代码，要认真的阅读。
对Pass的简要介绍，可参加我之前的一个博文：
http://blog.csdn.net/snsn1984/article/details/8160696
同时，还有一个对Pass例子的分析：
http://blog.csdn.net/snsn1984/article/details/8568828
第三步：自己根据HelloWorld的例子，写一个输出一个函数中所有的BasicBlock的name的Pass.
这个Pass是FunctionPass的子类，通过自己实现runOnFunction，来实现输出所有的BasicBlock的name.
在这个过程中，要善于利于google搜索，通过自己查找Function类的成员函数，来找到自己需要的成员函数。
这个Pass的难度不大，只是和HelloWorld稍微不同。
            
