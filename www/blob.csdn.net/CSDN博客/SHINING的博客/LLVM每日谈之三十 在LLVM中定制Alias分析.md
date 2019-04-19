# LLVM每日谈之三十 在LLVM中定制Alias分析 - SHINING的博客 - CSDN博客
2018年05月12日 20:58:34[snsn1984](https://me.csdn.net/snsn1984)阅读数：545
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
Alias分析是一个决定两个指针是否指向同一个对象（alias）的过程。这在变量优化的过程中十分重要。
近日，Simon Brand写了一篇博客介绍如何在LLVM中定制Alias分析，这篇博客非常简明，并且附上了示例代码，让人可以很快的掌握如何在LLVM中定制Alias分析。其最终目标，也是为了添加一个可以做定制化Alias分析的PASS。这篇博客的地址为：[Custom Alias Analysis in LLVM](https://link.zhihu.com/?target=https%3A//blog.tartanllama.xyz/llvm-alias-analysis/)
与此同时，网络上也有几篇比较经典的介绍如何在LLVM中定制Alias分析的资料，LLVM官方也有介绍Alias分析的文档，这些资料都已在附录中列出。
参考文献及相关资源：
1、Custom Alias Analysis in LLVM
[Custom Alias Analysis in LLVM](https://link.zhihu.com/?target=https%3A//blog.tartanllama.xyz/llvm-alias-analysis/)
2、Alias Analysis in LLVM  
[http://www.cse.psu.edu/~gxt29/paper/ShengHsiuLin_thesis.pdf](https://link.zhihu.com/?target=http%3A//www.cse.psu.edu/~gxt29/paper/ShengHsiuLin_thesis.pdf)
3、LLVM Alias Analysis Infrastructure
[LLVM Alias Analysis Infrastructure](https://link.zhihu.com/?target=http%3A//llvm.org/docs/AliasAnalysis.html)
4、Enhancing the Alias Analysis Passes in LLVM
[http://james0zan.github.io/resource/GSoC15-Proposal-AA.pdf](https://link.zhihu.com/?target=http%3A//james0zan.github.io/resource/GSoC15-Proposal-AA.pdf)
