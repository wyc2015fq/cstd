# 三维CNN：收集一些最近的3d卷积网络PointNet++ - wishchinYang的专栏 - CSDN博客
2018年03月22日 18:04:48[wishchin](https://me.csdn.net/wishchin)阅读数：5578
       受限于点云本身的无序性，不同设备在不同位置扫描物体所获得的数据不尽相同，数据难以通过端到端方法来处理；点云本身也面临数据缺失的问题，比如被扫描模型往往会被遮挡。
       针对无序点云数据的深度学习方法研究，目前进展还相对缓慢。在 CVPR 2017 上，斯坦福大学提出的 PointNet 是第一种直接处理无序点云数据的深度神经网络。团队通过在每个点训练一个 MLP，*把每个点投到一个 1024 维空间上*，并采用了 Max Pooling 层做为主要的对称函数，比较好地解决了顺序问题。随后斯坦福大学在 NIPS 2017 上的 Pointnet++，对 PointNet 做出了更多改进。
        PointNet++是在PointNet上做出了改进，考虑了点云局部特征提取，从而更好地进行点云分类和分割。
先简要说一下PointNet:
       PointNet，其本质就是一种网络结构，按一定的规则输入点云数据，经过一层层地计算，得出分类结果或者分割结果。其中比较特殊的地方在于两个转换矩阵（input transform & feature transform）的存在，根据文中所说，这两个转换矩阵可以在深度学习过程中**保持点云数据的空间不变性**。
       根据论文里所说，图中的input transform是一个3*3的矩阵，作为深度学习的一个参数存在。而feature transform由于维数较大（64*64），所以文中采用了正交约束的方法限制这个矩阵，从而使优化可以快速收敛。
         PointNet：PointNet Deep Learning on Point Sets for 3D Classification and Segmentation .
         简介：P[ointNet: Deep Learning on Point Sets for 3D Classification and Segmentation](https://www.meteorshub.com/machine-learning/2017/12/188/)-论文解读
![](https://img-blog.csdn.net/20180322180218846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         PointNet++：PointNet++: Deep Hierarchical Feature Learning onPoint Sets in a Metric Space
         简介：[PointNet++阅读](http://blog.csdn.net/yongxiebin9947/article/details/78706591)笔记   
![](https://img-blog.csdn.net/20180322180149774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意事项：
        无序点云融合模式识别一个重要的过程是有序化，笛卡尔坐标系化表示简单，但遍历与结构化描述比较困难。PointNet++进行，输入数据的标准化工作，输入一个梯级/逐渐稠密表示模型。
**方法**：从一点开始（可以是密度最高点，也可以随机选取），作为已选集合，使用测地线判断离已选集合最远点，然后更新已选集合，再次添加新的集合最远点，这样不管获得多少个点，都意味着模型的稀疏点表示。随着点集的增加，模型的表示越来越精确。PointNet++中使用了1024个维度，因此网络数据标准化时候可能面临点云填充问题，或者更好的方法是，在标准化之处先进行点云数据预填充工作。
        刘洪森同学对此网络进行了改进，暂时还没有仔细Get他的进展。
