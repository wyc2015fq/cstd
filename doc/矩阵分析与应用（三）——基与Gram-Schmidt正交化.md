# 矩阵分析与应用（三）——基与Gram-Schmidt正交化

2017年08月03日 11:53:28 [NirHeavenX](https://me.csdn.net/qsczse943062710) 阅读数：2597



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/qsczse943062710/article/details/76615618

  nn维Euclidean空间只有一个，但是nn维[向量空间](https://blog.csdn.net/qsczse943062710/article/details/76615618)却有无穷多个，如$x=\{0,0,\alpha,\beta,\gamma\}$和$y=\{1,5,\alpha,\beta,\gamma\}$就是两个完全不同的5维向量空间，虽然他们都在5阶Euclidean空间内。 
  我们知道，nn维空间的多个向量的线性组合也属于nn维空间（根据向量空间加法运算的闭合性）。因此，我们引出： 
  **由n维向量$x_1,x_2,...,x_m$所有的线性组合的集合WW称为由$x_1,x_2,...,x_m$张成的子空间。$x_1,x_2,...,x_m$称为WW的张成集。**

  可以看到，任意一个空间W的张成集都不是唯一的，这些集合称为[平凡张成集](https://blog.csdn.net/qsczse943062710/article/details/76615618)，对平凡张成集进行分析显然是没有意义的，为此我们需要寻找一种张成集， 该集合中包括张成空间W所需的最小个数的向量，也即[空间的基](https://blog.csdn.net/qsczse943062710/article/details/76615618)： 
  **生成子空间W的线性无关的向量集合称为W的基。**

  子空间的基也不是唯一的，但是它们包含的向量个数时相同的，等于子空间的维度。

- **对偶基**： 
  如果$\alpha_1,\alpha_2,...,\alpha_m$和$\beta_1,\beta_2,...,\beta_m$是两组不同的基，并且$\alpha_i^H\beta_i=0$，则称其中一组基是另外一组的对偶基。
- **正交基**： 
  如果一个基的所有向量之间相互正交，则称其为正交基。
- **标准正交基**： 
  如果一个正交基的每个向量的范数均为1，则成为标准正交基。

  在很多时候，我们需要对某个空间的标准正交基进行分析变换，这就要涉及到基的正交化，其中最常用的便是[Gram-Schmidt正交化](https://blog.csdn.net/qsczse943062710/article/details/76615618)。

### **Gram-Schmidt正交化**

  令$x_1,x_2,...,x_m$时子空间W的任何一组基，则通过以下变换生成的$\mu_1,\mu_2,...,\mu_m​$为一组标准正交基： 
$$
p_1 = x_1，\mu_1 = \frac{p_1}{\Vert p_1\Vert}\\
p_k = x_k-\sum_i^{k-1}(\mu_k^Hx_k)\mu_i，\mu_k = \frac{p_k}{\Vert p_k\Vert}
$$
  可以使用数学归纳法证明，经过上述过程生成的基是标准正交基。

  Gram-Schmidt正交化的主要缺点是，在某些情况下，数值性能不是很好，为此修正的Gram-Schmidt正交化法能很好地解决这个问题：
$$
\mu_1 = \frac{x_1}{\Vert x_1\Vert}\\
对所有的向量进行第一次修正x_i^{(1)} = x_i - \mu_1^Hx_i\mu_1\\
构造第二个向量\mu_2 =  \frac{x_2}{\Vert x_2\Vert}\\
对所有的向量进行第二次修正x_i^{(2)} = x_i^{(1)} - \mu_2^Hx_i^{(1)}\mu_2，依次类推
$$
