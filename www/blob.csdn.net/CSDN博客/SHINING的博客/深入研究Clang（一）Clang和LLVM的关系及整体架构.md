# 深入研究Clang（一）Clang和LLVM的关系及整体架构 - SHINING的博客 - CSDN博客
2013年12月31日 15:53:19[snsn1984](https://me.csdn.net/snsn1984)阅读数：36233
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)

作者：[史宁宁](http://weibo.com/u/1899097901)（[snsn1984](http://blog.csdn.net/snsn1984)）
Clang和LLVM的关系
        Clang和LLVM到底是什么关系，这是在研究Clang的过程中所不可避免的一个问题。如果要搞清楚Clang和LLVM之间的关系，首先先要知道宏观的LLVM和微观的LLVM。
        宏观的LLVM，指的是整个的LLVM的框架，它肯定包含了Clang，因为Clang是LLVM的框架的一部分，是它的一个C/C++的前端。虽然这个前端占的比重比较大，但是它依然只是个前端，LLVM框架可以有很多个前端和很多个后端，只要你想继续扩展。
        微观的LLVM指的是以实际开发过程中，包括实际使用过程中，划分出来的LLVM。比如编译LLVM和Clang的时候，LLVM的源码包是不包含Clang的源码包的，需要单独下载Clang的源码包。
        所以这里想讨论的就是微观的LLVM和Clang的关系。从编译器用户的角度，Clang使用了LLVM中的一些功能，目前所知道的主要就是对中间格式代码的优化，或许还有一部分生成代码的功能。从Clang和微观LLVM的源码位置可以看出，Clang是基于微观的LLVM的一个工具。而从功能的角度来说，微观的LLVM可以认为是一个编译器的后端，而Clang是一个编译器的前端，它们的关系就更加的明了了，一个编译器前端想要程序最终变成可执行文件，是缺少不了对编译器后端的介绍的。
        这样基本就确定了Clang和LLVM的关系。这个问题虽然并不复杂，但是对于后续的结构理解和深入研究确是一个很重要的出发点。这个出发点不理清楚的话，后续的研究没有办法继续深入下去。

Clang的整体架构
        如果要深入的研究Clang，那么首先需要知道Clang的整体架构。直接打开Clang的源码的话，里面目录较多，无从下手。可以直接从doxygen文档看，Clang的doxygen的地址是：http://clang.llvm.org/doxygen/index.html 。从这个首页选取Directories，可以直接进到文件目录列表：http://clang.llvm.org/doxygen/dirs.html 。在这个里面看目录，就比较清晰了，在clang目录下面一共就三个目录：
 docs、include和lib。为什么只有这三个呢？仔细研究和对比就会发现，如果你要研究Clang的内部实现，只需要这三个目录就够了，其他的目录和Clang核心没有任何关系，是一些基于Clang的工具，Clang的一些例子，或者是一些测试用例等。
          现在看起来，Clang的结构已经很明显了。docs很明显放置的都是一些文档相关的内容，include放置的都是一些头文件。就只剩下一个lib目录了，下面是分类的目录，这个时候也可以推断出Clang的总体架构了，它的总体架构是基于库的。在Clang的官方文档“Clang" CFE Internals Manual （http://clang.llvm.org/docs/InternalsManual.html ）中，对Clang内部的介绍也是依据库来进行分类的。

