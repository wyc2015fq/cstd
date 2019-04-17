# 【IM】关于参数模型和核模型的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年09月28日 14:22:28[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：170








学习模型按照参数与维度还是样本相关，可分为参数模型（参数与维度相关）和核模型（参数与样本相关）。

对于核模型的核方法或核函数说明有如下博文，参考《图解机器学习》理解如下两页。

[https://blog.csdn.net/fjssharpsword/article/details/81661602](https://blog.csdn.net/fjssharpsword/article/details/81661602)

[https://blog.csdn.net/fjssharpsword/article/details/79092082](https://blog.csdn.net/fjssharpsword/article/details/79092082)

![](https://img-blog.csdn.net/20180928141745666?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018092814175966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 这个理解包括两方面：

1）非线性SVM中核函数的作用，起到高维线性可分同时又在低维进行计算，避免高维运算；

2）SVM和神经网络的比较，神经网络参数学的更多，模型更复杂，计算更大。

还有一些理解的不是很透彻，如高斯核模型中，只在输入样本附近对函数近似。按照参数模型中的加法乘法两种思路拓展多维，可以很好理解高维和低维的关系，以及模型复杂度、参数多寡、计算量大小、拟合度等等。



