
# 推荐系统——Collaborative Memory Network for Recommendation Systems - manduner的博客 - CSDN博客


2018年07月18日 08:50:06[Manduner_TJU](https://me.csdn.net/manduner)阅读数：591


# 论文描述：
*协同过滤推荐系统可以分为三类：基于内存或者邻域的方法、潜在因子模型和混合模型
*基于邻域的方法通过从两个用户或商品之间的反馈交叉点中选择最多K个观测值，以获取用户和商品历史交互数据的局部结构，但这样做通常忽略了大多数可用的评分。
*潜在因子模型，例如矩阵分解模型，可以得到用户和商品之间的全局结构，但通常会忽略上述强关联的存在。
*混合模型，例如SVD++，通过整合基于领域的方法和潜在因子模型，克服了两种方法的缺点，提高了推荐系统的性能
*深度学习与潜在因子模型的成功结合已经证明了深度学习的复杂非线性变换和强大的表示学习能力能够提高推荐系统的性能。但是现有的利用深度学习的模型忽略了将非线性变换与基于领域的方法进行整合。
*我们提出了一种统一的混合模型，该模型整合了最先进的内存网络（Memory Networks）和具有隐式反馈的CF神经网络模型。内存网络通过读写操作来编码内存中复杂的用户和商品之间的关系。关联寻址方案（An associative addressing scheme）充当最近邻模型，它基于自适用用户和商品状态来发现语义上相似的用户。CF神经网络模型对具有相似偏好的用户的特定子集施加更高的权重，形成集体邻域概要（collective neighborhood summary）。最后，局部领域概要（local neighborhood summary）与全局潜在因子之间的非线性相互作用产生排名。模型可堆叠多个内存组件以进一步提高推荐性能。

# 参考文献：
*Ebesu T, Shen B, Fang Y. Collaborative Memory Network for Recommendation Systems[J]. arXiv preprint arXiv:1804.10862, 2018.*

