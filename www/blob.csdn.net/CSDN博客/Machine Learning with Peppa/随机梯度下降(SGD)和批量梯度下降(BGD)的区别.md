# 随机梯度下降(SGD)和批量梯度下降(BGD)的区别 - Machine Learning with Peppa - CSDN博客





2018年05月26日 11:02:44[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：349








梯度下降（GD, gradient descent）：梯度下降法是求解无约束最优化问题的一种常用方法，比较适用于控制变量较多，受控系统比较复杂，无法建立准确数学模型的最优化控制过程。是一种迭代算法，每一步需要求解目标函数的梯度向量。     

在机器学习中，GD主要用于降低模型输出和真实输出之间的损失/误差，迭代模型结构。随机梯度下降是随机取样替代完整的样本，主要作用是提高迭代速度，避免陷入庞大计算量的泥沼。对于整个样本做GD又称为批梯度下降（BGD，batch gradient descent）。

![](https://img-blog.csdn.net/201805261101320?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





随机梯度下降（SGD, stochastic gradient descent）：名字中已经体现了核心思想，随机选取一个店做梯度下降，而不是遍历所有样本后进行参数迭代。因为梯度下降法的代价函数计算需要遍历所有样本，而且是每次迭代都要遍历，直至达到局部最优解，在样本量庞大时就显得收敛速度比较慢了，计算量非常庞大。      

随机梯度下降仅以当前样本点进行最小值求解，通常无法达到真正局部最优解，但可以比较接近。属于大样本兼顾计算成本的折中方案。
![](https://img-blog.csdn.net/2018052611013930?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




