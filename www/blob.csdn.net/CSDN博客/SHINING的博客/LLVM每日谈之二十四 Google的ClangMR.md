# LLVM每日谈之二十四  Google的ClangMR - SHINING的博客 - CSDN博客
2017年04月20日 10:57:50[snsn1984](https://me.csdn.net/snsn1984)阅读数：1572
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
ClangMR的论文《Large-Scale Automated Refactoring Using ClangMR》：http://www.hyrumwright.org/papers/icsm2013.pdf
ClangMR是Goolge的一个工具，严格的说，并不属于Clang家族，是属于基于Clang所构建的一个工具。Google秉承他们一贯的原则，在构建该工具并投入使用之后，将该工具的情况发了一篇论文《Large-Scale Automated Refactoring Using ClangMR》。论文本身对这个工具已经谈的很细致了，在这里我就简单聊聊读完这篇论文的一点感受，主要聚焦于几个点：
1、这是一个大规模的自动化的重构工具，或者说是平台；
2、该工具的应用范围并没有覆盖到多数的重构类型，主要还是聚焦在用新的API去重构旧的API这个重构上；
3、该工具的重构只支撑translation unit自包含的情况；（这种情况在重构里面属于比较便于自动化重构操作的）
4、因为ClangMR是大规模的自动化工具，所以使用了基于MapReduce的技术，便于处理大量的代码；
5、该工具的工作对象是C++代码库。
如果本身对自动化重构感兴趣的，可以仔细研究下这篇论文。
