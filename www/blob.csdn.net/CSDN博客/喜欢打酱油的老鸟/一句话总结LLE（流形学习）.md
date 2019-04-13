
# 一句话总结LLE（流形学习） - 喜欢打酱油的老鸟 - CSDN博客


2018年09月19日 12:57:04[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：191


一句话总结LLE（流形学习）
核心：用一个样本点的邻居的线性组合近似重构这个样本，将样本投影到低维空间中后依然保持这种线性组合关系。
局部线性嵌入（简称LLE）将高维数据投影到低维空间中，并保持数据点之间的局部线性关系。其核心思想是每个点都可以由与它相近的多个点的线性组合来近似，投影到低维空间之后要保持这种线性重构关系，并且有相同的重构系数。
算法的第一步是求解重构系数，每个样本点xi可以由它的邻居线性表示，即如下最优化问题：
![](https://img-blog.csdn.net/20180919125633453?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样可以得到每个样本点与它邻居节点之间的线性组合系数。接下来将这个组合系数当做已知量，求解下面的最优化问题完成向量投影：
![](https://img-blog.csdn.net/20180919125650155?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样可以得到向量y，这就是投影之后的向量。
LLE是一种无监督的机器学习算法，它是一种非线性降维算法，不能直接用于分类或者回归问题。


