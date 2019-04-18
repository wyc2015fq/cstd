# KL散度 - wydbyxr的博客 - CSDN博客
2018年10月20日 11:36:03[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：29
个人分类：[机器学习具体算法](https://blog.csdn.net/wydbyxr/article/category/7945743)
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# KL散度
  相对熵（relative entropy）又称为KL散度（Kullback–Leibler divergence，简称KLD），信息散度（information divergence），信息增益（information gain）,KL距离。
![在这里插入图片描述](https://img-blog.csdn.net/20181020113644745?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 概念解释
  它是描述两个概率分布P和Q差异的非对称性的度量（注意与JS散度区别）。它是非对称的，这意味着D(P||Q) ≠ D(Q||P)。
  它衡量的是相同事件空间里的两个概率分布的差异情况。其物理意义是：在相同事件空间里，概率分布P(x)的事件空间，若用概率分布Q（x）编码时，平均每个基本事件（符号）编码长度增加了多少比特。我们用D（P||Q）表示KL距离，计算公式如下：	
![在这里插入图片描述](https://img-blog.csdn.net/20181020113310181?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
  常用于NLP。在SNE可视化时也使用了KL距离作为优化函数。
  另外，有人将KL散度称为KL距离，但事实上，KL散度并不满足距离的概念	虽然KL被称为距离，但是其不满足距离定义的三个条件：1）非负性；2）对称性（不满足，P到Q的距离，不等于Q到P的距离）；3）三角不等式（不满足，两边之和大于第三边，两边之差小于第三边）。
## 与相对熵的关系
  当两个概率分布完全相同时，即P(x)=Q(X)，其相对熵为0 。我们知道，概率分布P(X)的信息熵为：
![在这里插入图片描述](https://img-blog.csdn.net/20181020113341485?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
  其表示，概率分布P(x)编码时，平均每个基本事件（符号）至少需要多少比特编码。
  相对熵：Kullback–Leibler divergence
  交叉熵：cross entropy，通常“相对熵”也可称为“交叉熵”，虽然公式上看相对熵=交叉熵-信息熵，但由于真实分布p是固定的，D(p||q)由H(p,q)决定。当然也有特殊情况，彼时2者须区别对待。
