# 【深度学习】RNN之GRU原理 - zkq_1986的博客 - CSDN博客





2018年05月17日 22:53:24[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：309








GRU（Gated recurrent unit，循环门单元）

c：memery cell，记忆单元

![](https://img-blog.csdn.net/20180517225221680)


![](https://img-blog.csdn.net/20180517225241273)


![](https://img-blog.csdn.net/20180517225257527)


之所以gamma值用sigmoid函数，是因为它基本是要么0要么1。当gamma为0时，就采用历史信息；当gamma为1时，就采用当前信息。



