# TensorFlow之八—优化器 - wsp_1138886114的博客 - CSDN博客





2019年02月12日 14:12:17[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：70











### 文章目录
- [一，优化器函数](#_1)
- [tf.train.GradientDescentOptimizer（梯度下降算法）](#tftrainGradientDescentOptimizer_16)
- [tf.train.MomentumOptimizer（动量梯度下降算法 ）](#tftrainMomentumOptimizer__19)
- [tf.train.AdamOptimizer（Adam优化算法）](#tftrainAdamOptimizerAdam_23)




### 一，优化器函数
- TensorFlow中提供了如下优化器函数：
tf.train.Optimizer

tf.train.GradientDescentOptimizer

tf.train.AdadeltaOptimizer

tf.train.AdagtadOptimizer

tf.train.AdagradDAOptimizer

tf.train.MomentumOptimizer

tf.train.AdamOptimizer

tf.train.FtrlOptimizer

tf.train.ProximalGradientDescentOptimizer

tf.train.ProximalAdagradOptimizer

tf.train.RMSProOptimizer
##### tf.train.GradientDescentOptimizer（梯度下降算法）

`tf.train.GradientDescentOptimizer()`使用随机梯度下降算法，使参数沿着 梯度的反方向，即总损失减小的方向移动，实现更新参数。

##### tf.train.MomentumOptimizer（动量梯度下降算法 ）

`tf.train.MomentumOptimizer()`在更新参数时，利用了超参数，参数更新公式是：

其中，𝛼为学习率，超参数为𝛽，𝜃为参数，𝑔(𝜃𝑖−1 )为损失函数的梯度。

##### tf.train.AdamOptimizer（Adam优化算法）

`tf.train.AdamOptimizer()`是利用自适应学习率的优化算法，Adam 算法和随 机梯度下降算法不同。随机梯度下降算法保持单一的学习率更新所有的参数，学习率在训练过程中并不会改变。而 Adam 算法通过计算梯度的一阶矩估计和二 阶矩估计而为不同的参数设计独立的自适应性学习率。

学习率：决定每次参数更新的幅度。

优化器中都需要一个叫做学习率的参数，使用时，如果学习率选择过大会出现震 荡不收敛的情况，如果学习率选择过小，会出现收敛速度慢的情况。我们可以选 个比较小的值填入，比如 0.01、0.001
- 观察图：
SGD随机梯度下降算法收敛的速度是最慢的；

Adadelta收敛得速度是最快的；

Momentum所走的路程是最远的，会冲出最低点

关于[梯度下降详情理论请点击](https://blog.csdn.net/wsp_1138886114/article/details/81328694)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190131165339643.gif)


