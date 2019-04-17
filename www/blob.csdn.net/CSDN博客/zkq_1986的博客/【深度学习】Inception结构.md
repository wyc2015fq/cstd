# 【深度学习】Inception结构 - zkq_1986的博客 - CSDN博客





2017年12月22日 21:30:13[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：312








你不用考虑conv层到底是用1*1，3*3，还是5*5。Inception把这些conv的各filter大小类型合在一起，再加上pool，将结果合并输出。其缺点参数量增多。

![](https://img-blog.csdn.net/20171222212926504)




