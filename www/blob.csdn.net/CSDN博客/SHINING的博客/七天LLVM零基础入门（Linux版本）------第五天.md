# 七天LLVM零基础入门（Linux版本）------第五天 - SHINING的博客 - CSDN博客
2013年02月26日 09:38:32[snsn1984](https://me.csdn.net/snsn1984)阅读数：5982
所属专栏：[LLVM零基础学习](https://blog.csdn.net/column/details/llvm.html)
作者：[snsn1984](snsn1984)
第一步：复习文档 Write an LLVM pass
[http://llvm.org/docs/WritingAnLLVMPass.html](http://llvm.org/docs/WritingAnLLVMPass.html)
第二步：阅读LLVM编程规范
[http://llvm.org/docs/CodingStandards.html](http://llvm.org/docs/CodingStandards.html)
编程规范是编程中需要注意的基础点，代码风格符合不符合规范，将会直接影响整个代码的可读性和代码质量。
所以一定要认真阅读，并且在自己编写代码的过程中要遵守规范。最容易犯错误的两点就是缩进和空格。不同
的编程规范的缩进是不同的，但是对TAB的限制是大多数规范都有的。任何不是必须的空格，都是不应该出现
的。
第三步：参照LLVM编程规范写Pass例子
编写一个Pass，删除掉程序中所有的没有使用的指令。比如：
%2 = add i32 %1, 1;
%3 = add i32 %2, 1;
ret;
那么因为%3并没有被使用，所以指令%3 = add i32 %2, 1; 就可以被删除了，删除之后，如果%2没有别的地方使用，
那么这条语句也可以删除掉了。依次循环下去，最后得到的是一个没有无用的代码的程序。
延伸阅读：
[http://en.wikipedia.org/wiki/Dead_code_elimination](http://en.wikipedia.org/wiki/Dead_code_elimination)
-------------------------------------------转载请注明出处-------------------------------------------------------------------------------------
