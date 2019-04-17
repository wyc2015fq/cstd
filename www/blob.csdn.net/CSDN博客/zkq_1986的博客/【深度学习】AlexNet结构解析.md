# 【深度学习】AlexNet结构解析 - zkq_1986的博客 - CSDN博客





2017年12月14日 21:26:37[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：170








AlexNet中间两层卷积（包括max pooling），filter为11*11和3*3, stride为4和2。然后接两层纯的卷积，接着又是卷积和pooling，最后三层的全连接，通过softmax输出结构。每层中的filter个数越来越多。

![](https://img-blog.csdn.net/20171214212526298)




