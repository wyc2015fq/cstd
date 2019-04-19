# 七天LLVM零基础入门（Linux版本）------第二天 - SHINING的博客 - CSDN博客
2013年02月21日 09:15:49[snsn1984](https://me.csdn.net/snsn1984)阅读数：16126
所属专栏：[LLVM零基础学习](https://blog.csdn.net/column/details/llvm.html)
作者：[snsn1984](http://blog.csdn.net/snsn1984)
在第一天的时候，我们简单了解了llvm和clang。今天我们继续对LLVM进行学习。
第一步：学会如何查找LLVM的文档
LLVM的文档很多，也很全面，是开源软件中文档比较全面并且比较细致的，这是很难得的一个事情。所以只要和LLVM打交道，就一定要学会并且善于去使用这些LLVM的文档。
因为LLVM文档比较多，初学者可能不太容易搞清楚，我之前写过一个关于LLVM的文档的博文，可以参考：
[http://blog.csdn.net/snsn1984/article/details/8165529](http://blog.csdn.net/snsn1984/article/details/8165529)
这个博文把文档的地址和文档的分类进行了简单介绍。
第二步：熟悉LLVM的IR
IR是[intermediate representation](http://www.iciba.com/intermediate_representation)的缩写，顾名思义是中间表示的的缩写。中间表示已经被越来越多的编译器所采用，传统的编译器多采用汇编语言作为自己的中间语言，而现在大一些的编译器都有了自己专属的中间表示。LLVM IR的官方文档地址：[http://llvm.org/docs/LangRef.html](http://llvm.org/docs/LangRef.html)
简要介绍可参加我之前的博文：[http://blog.csdn.net/snsn1984/article/details/8037414](http://blog.csdn.net/snsn1984/article/details/8037414)
LLVM IR是整个LLVM框架中极其重要的一部分，一般与LLVM相关的项目都无法避开LLVM IR这个部分，因为它是整个LLVM框架中的枢纽，所以LLVM IR的官方文档必须要完全熟悉。初学者做不到完全熟悉的，至少也应该读几遍，知道整个LLVM IR的大部分内容，细节的内容记不住的可以在使用的时候进行查找文档。
另外，应该可以使用clang去获取一个程序的.ll格式和.bc格式的IR文件，并且能够让这两种格式的文件进行相互转换。
相关知识补充：如果编译原理相关知识比较差的话，需要在这个环节补充一下编译原理的相关知识，至少要知道从源码到可执行文件中的每一个大的环节。同时，也需要去补充一下为什么编译器都采用中间表示，这对LLVM IR的学习，以及后续的学习都很有帮助。
----------------------------------------------------------转载请注明出处---------------------------------------------------------------------------

