# 七天LLVM零基础入门（Linux版本）------第三天 - SHINING的博客 - CSDN博客
2013年02月22日 09:46:59[snsn1984](https://me.csdn.net/snsn1984)阅读数：12624
所属专栏：[LLVM零基础学习](https://blog.csdn.net/column/details/llvm.html)
作者：[snsn1984](snsn1984)
        第二天的任务中的LLVM IR的文档（[http://llvm.org/docs/LangRef.html](http://llvm.org/docs/LangRef.html)）对于初学者来说，看起来一定很费劲，内容繁杂，找不到头绪，看了后边的忘记了前面的，这些都是很正常的。但是，这里需要注意的是，不管多烦躁，一定要硬着头皮先把这个文档通读一遍，在读的同时，可以自己写一个小程序，根据前面学习的使用clang的命令将该程序转换成.ll格式的，然后在阅读文档的同时，对照.ll格式的文件，去实际的看一看，到底说的是什么样的东西，然后这样才能加深印象，才能让阅读文档变得不那么艰难。
下面开始今天的学习任务：
第一步：复习LLVM IR的文档（[http://llvm.org/docs/LangRef.html](http://llvm.org/docs/LangRef.html)）
复习该文档的时候，略微有些侧重点，就是该文档从开始的部分，一直到[http://llvm.org/docs/LangRef.html#other-values](http://llvm.org/docs/LangRef.html#other-values)  这个部分之前，都要仔细复习一遍。后面的内容，大致的过一遍，以后要用到的时候，可以找到地方就行。
第二步：阅读文档 （[http://llvm.org/docs/ProgrammersManual.html](http://llvm.org/docs/ProgrammersManual.html)）
这个文档从名字就可以看出来是干什么用的，这个就是LLVM的编程指引，相对来说这个文档没那么长，但是里面的东西很重要，希望在阅读的过程中认真阅读。同时，涉及到C++部分的知识，如果有不清楚的，要自己主动的把那些知识补充学习一下。在阅读这个文档的过程中，可以去LLVM的源码中找一些实际的代码，参照着看一下，效果会更好。可以从[http://androidxref.com/](http://androidxref.com/) 在线浏览和搜索代码，这个网站是在线浏览搜索Android源码的，LLVM的源码就在[http://androidxref.com/4.2.2_r1/xref/external/llvm/](http://androidxref.com/4.2.2_r1/xref/external/llvm/)
 这里，可以从这里搜索、跳转，都很方便。
补充：关于文档的阅读，其实是一个长期的过程，每次都认真读，每次都会有不同的收获。尤其是自己在实际使用的过程中，要多查文档，多阅读文档，才能让自己在这个领域里面更快的成长起来。
--------------------------------------------转载请注明出处----------------------------------------------------
