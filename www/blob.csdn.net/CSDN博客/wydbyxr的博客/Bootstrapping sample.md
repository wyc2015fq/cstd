# Bootstrapping sample - wydbyxr的博客 - CSDN博客
2018年08月31日 17:59:27[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：418
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# Bootstrapping
　　bootstrap sample是什么? OOB(out of boostrap)是什么?  
　　boostrap sample: 是一种从给定训练集中有返回的均匀抽样, 也就是, 每当选中一个样本, 它等可能地再次被抽中, 并被添加到训练集中。 
　　机器学习中，Bootstrap 方法指的是借助替换的随机采样，它是一个重采样，允许模型或算法更好地理解存在于其中的偏差、方差和特征。数据的采样允许重采样包含不同的偏向，然后将其作为一个整体进行包含。如图 1 所示，其中每个样本群有不同的部分，而且各不相同。接着这会影响到数据集的整体均值、标准差和其他描述性指标。反过来，它可以发展出更多鲁棒的模型。
　　Bootstrapping 同样适用倾向于过拟合的小数据集。 
　　使用 Bootstrap 的原因是它可以测试解决方案的稳定性。使用多个样本数据集测试多个模型可以提高鲁棒性。或许一个样本数据集的平均值比其他数据集大，或者标准差不同。这种方式可以识别出过拟合且未使用不同方差数据集进行测试的模型。 
### 使用
　　Bootstrapping 越来越普遍的原因之一是计算能力的提升。出现比之前更多次数的重排列、重采样。Bagging 和 Boosting 都使用 Bootstrapping
