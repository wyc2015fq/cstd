# LLVM每日谈之三十三 LLVM的DEBUG宏改名了 - SHINING的博客 - CSDN博客
2018年07月02日 13:58:41[snsn1984](https://me.csdn.net/snsn1984)阅读数：291
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
根据[rL332240](https://reviews.llvm.org/rL332240)提交的申请，LLVM的DEBUG宏将更名为LLVM_DEBUG，原因是为了避免其他工程出现问题。在转换期间，DEBUG宏作为别名依然可以使用。
这个更改虽然谈不上什么有技术含量的更新，只是一个更名，但是这个更名却对我们日常的使用影响较大。毕竟，debug宏使用的频率还是很高的。
具体修改内容见：[rL332240](https://reviews.llvm.org/rL332240)
2018年5月22日
