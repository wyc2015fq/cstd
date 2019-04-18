# 【IM】关于稀疏学习和鲁棒学习的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年10月08日 18:07:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：137
主要是理解L1和L2正则化，鲁棒学习中也是围绕L1残差和L2方差损失来提出模型。关于鲁棒学习，对于异常值的鲁棒性有两方面：一是对现有样本中异常点的鲁棒性；二是对新增异常点的鲁棒性。
关于L1和L2正则化，参考如下博文理解：
[https://blog.csdn.net/fjssharpsword/article/details/78842374](https://blog.csdn.net/fjssharpsword/article/details/78842374)
[https://blog.csdn.net/fjssharpsword/article/details/78850393](https://blog.csdn.net/fjssharpsword/article/details/78850393)
![](https://img-blog.csdn.net/20181008180525113?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再次回顾下，有监督回归是对实值函数的近似，最小二乘法是近似的一种方法，包括问题建模、优化求解。
