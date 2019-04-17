# 高阶奇异值分解(HOSVD)理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年02月23日 12:59:47[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2447








在基于情境上下文的推荐系统中，HOSVD是常用方法，这里通过一篇文章简单理解下HOSVD。

1、适用场景：

This decomposition plays an important role in various domains, such as:
? Spectral analysis,
? Non-linear modeling,
? Communication and Radar processing,
? blind source separation,
? image processing,
? biomedical applications (magnetic resonance imaging and electrocardiography),
? web search,
? computer facial recognition,
? handwriting analysis,



2、HOSVD定义：

![](https://img-blog.csdn.net/20180223124916495)

3、张量分解

1）standard unfoldings



![](https://img-blog.csdn.net/20180223125049584)

巨大的计算成本。

2）Higer PCA

![](https://img-blog.csdn.net/20180223125332143)


3）Kernel decomposition in Volterra series 


![](https://img-blog.csdn.net/2018022312543016)

4）1-D harmonic retrieval problem 


![](https://img-blog.csdn.net/20180223125523502)

5）Oblique unfoldings to decrease the complexity

![](https://img-blog.csdn.net/20180223125652470)





6）Complexities of the HOSVD algorithms



![](https://img-blog.csdn.net/20180223125758450)

HOSVD主要在于张量分解方法。


4、展望

•Structured tensors imply strongly structured modes if oblique unfoldings are used. Not true for standard unfoldings !
• Increasing the structure of the modes allows to exploit fast techniques from numerical linear algebra based on - the column-redundancy property
- fast products vector/matrix for Toeplitz or Hankel matrices.
• Fastest implementation of the rank-truncated HOSVD (dedicated to Hankel tensors) has a quasilinear complexity w.r.t. the tensor dimensions.


• Generalize to tensors of order > 3.

 • Extend to other HOSVD (constrained HOSVD, cross-HOSVD,...)?




最基础的可以参考维基https://en.wikipedia.org/wiki/Higher-order_singular_value_decomposition




