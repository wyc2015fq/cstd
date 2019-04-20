# 机器学习之学习率 Learning Rate - Spark高级玩法 - CSDN博客
2018年05月08日 11:57:46[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：786
本文从梯度学习算法的角度中看学习率对于学习算法性能的影响，以及介绍如何调整学习率的一般经验和技巧。
在机器学习中，监督式学习（Supervised Learning）通过定义一个模型，并根据训练集上的数据估计最优参数。梯度下降法（Gradient Descent）是一个广泛被用来最小化模型误差的参数优化算法。梯度下降法通过多次迭代，并在每一步中最小化成本函数（cost function）来估计模型的参数（weights）。
梯度下降的伪代码如下：
重复已下过程，直到收敛为止{
　　　　ωj = ωj - λ ∂F(ωj) / ∂ωj
　　}
说明：（1） ωj 是模型参数， F()是成本函数， ∂F(ωj) / ∂ωj 是ωj  的一阶导数，λ 是学习率
　　　（2）如果F()是单调函数，经过多次迭代会得到最小的成本函数；如果F()非单调，那么我们有可能陷入局部最优，一个简单的解决办法是通过多次尝试不同的ωj 初始值，对比不同估计参数下的成本函数的值是否一致，来发现是否陷入局部最优。
　　　（3）梯度下降法未必是最优的计算权重参数的方法，但是作为一种简单快速的方法，常常被使用。参照Andrew Ng的Stanford公开课程。
梯度下降过程的图示如下：
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUl0noGuKab1fQcibsd8sH3cY2hwBxZtPQXj7tUkjM4O6FnjngVgOFXxJhmmJWwawBd49BMjPWhB8A/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**学习率的调整**
为了能够使得梯度下降法有较好的性能，我们需要把学习率的值设定在合适的范围内。学习率决定了参数移动到最优值的速度快慢。如果学习率过大，很可能会越过最优值；反而如果学习率过小，优化的效率可能过低，长时间算法无法收敛。所以学习率对于算法性能的表现至关重要。
**对于不同大小的数据集，调节不同的学习率**
根据我们选择的成本函数F()不同，问题会有区别。当平方误差和（Sum of Squared Errors）作为成本函数时， ∂F(ωj) / ∂ωj 会随着训练集数据的增多变得越来越大，因此学习率需要被设定在相应更小的值上。
解决此类问题的一个方法是将学习率λ 乘上1/N，N是训练集中数据量。这样每部更新的公式变成下面的形式：
ωj = ωj - (λ/N) * ∂F(ωj) / ∂ωj
相关内容可参考： Wilson et al. paper “The general inefﬁciency of batch training for gradient descent learning”
另外一种解决方法是：选择一个不被训练集样本个数影响的成本函数，如均值平方差（Mean Squared Errors）。
**在每次迭代中调节不同的学习率**
在每次迭代中去调整学习率的值是另一种很好的学习率自适应方法。此类方法的基本思路是当你离最优值越远，你需要朝最优值移动的就越多，即学习率就应该越大；反之亦反。
但是这里有一个问题，就是我们并不知道实际上的最优值在哪里，我们也不知道每一步迭代中我们离最优值有多远。
解决办法是，我们在每次迭代的最后，使用估计的模型参数检查误差函数（error function）的值。如果相对于上一次迭代，错误率减少了，就可以增大学习率，以5%的幅度；如果相对于上一次迭代，错误率增大了（意味着跳过了最优值），那么应该重新设置上一轮迭代ωj 的值，并且减少学习率到之前的50%。这种方法叫做 Bold Driver.
**建议：归一化输入向量**
归一化输入向量在机器学习问题中是一个通用的方法。在一些应用中，由于使用距离或者特征方差，要求必须归一化输入向量，因为如果不归一化将导致结果会严重被具有大方差的特征和不同的尺度影响。归一化输入能够帮助数值最优方法（例如，梯度下降法）更快，更准确地收敛。
尽管有一些不同的归一化变量的方法，[0,1]归一化（也叫做min-max）和z-score归一化是两种最为广泛应用的。
XminmaxNorm = (X - min(X)) / (max(X) - min(X));
XzscoreNorm = (X - mean(X)) / std(X);
说明：本文为http://blog.datumbox.com/tuning-the-learning-rate-in-gradient-descent/ 的译文版，原文作者 Vasilis Vryniotis.
希望有助于大家理解与使用！
**推荐阅读 ：**
1，[数据揭秘：低学历成功逆袭概率有多少？感谢父母送我读书！](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484801&idx=1&sn=5edbd121fe8e55f50475857189401f86&chksm=9f38e6a9a84f6fbf21c98c019bc26eb7262a106012ca48aa41e062f75b0b9e6d930ca193fc1a&scene=21#wechat_redirect)
2，[【干货】树算法对比：RF、GBDT、XGBoost](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484795&idx=1&sn=d3dd62fcf5d5740f5d7d10b1cbea72fc&chksm=9f38e653a84f6f458edfea9b13cb3319eec3bd8dab64d21358a71ddac34a1360bd443ea5a26d&scene=21#wechat_redirect)
3，[机器学习萌新必学的Top10算法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484792&idx=1&sn=74185f316b384c72aff130694b0ae1b6&chksm=9f38e650a84f6f4630a1e004206e16a8fe9994ea601062ba920752c91feb3d35a0a0ae18912a&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
