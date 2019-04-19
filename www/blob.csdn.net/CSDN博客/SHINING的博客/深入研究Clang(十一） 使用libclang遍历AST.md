# 深入研究Clang(十一） 使用libclang遍历AST - SHINING的博客 - CSDN博客
2018年05月13日 10:55:34[snsn1984](https://me.csdn.net/snsn1984)阅读数：2287
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
 之前在本系列的第三篇（[深入研究Clang（三） libclang](https://blog.csdn.net/snsn1984/article/details/25888949)）介绍过libclang，内容相对简单，也没有实际的代码举例。其实，在看libclang的时候，我自己动手写了一个小工具：screader，想取名source code reader的意思。
       screader放在了github上很长一段时间了，我一直以为这个资源已经在写第三篇（[深入研究Clang（三） libclang](https://blog.csdn.net/snsn1984/article/details/25888949)）的时候介绍过了，一直没在意。今天回头看，才发现这个东西没贴出来。
screader的github地址：[shining1984/screader](https://link.zhihu.com/?target=https%3A//github.com/shining1984/screader)
       这里是用C语言写的。主要是根据输入的待分析文件或待分析目录，去分析所有输入的文件，列出文件的AST节点信息。同时，screader还可以在文件后面输入要查找的关键字，然后输出关键字所在节点的相关信息。
       这个小工具很简单，几乎没有任何实际使用价值，价值主要在演示了如何调用libclang去做AST分析。有初学者想学习这块内容的话，可以作为参考。
