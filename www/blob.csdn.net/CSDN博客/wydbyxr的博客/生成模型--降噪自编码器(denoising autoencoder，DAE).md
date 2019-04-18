# 生成模型--降噪自编码器(denoising autoencoder，DAE) - wydbyxr的博客 - CSDN博客
2018年10月25日 13:46:53[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：539
# 降噪自编码器(denoising autoencoder，DAE)
  这里不是通过对损失函数施加惩罚项，而是通过改变损失函数的重构误差项来学习一些有用信息。
  向输入的训练数据加入噪声，并使自编码器学会去除这种噪声来获得没有被噪声污染过的真实输入。因此，这就迫使编码器学习提取最重要的特征并学习输入数据中更加鲁棒的表征，这也是它的泛化能力比一般编码器强的原因。目的是使特征很鲁棒。
  denoising autoencoder的表现好像比sparse autoencoder要强一些。
  降噪自编码模型避免了一般的自编码模型可能会学习得到无编码功能的恒等函数和需要样本的个数大于样本的维数的限制，尝试通过最小化降噪重构误差，从含随机噪声的数据中重构真实的原始输入。
## 具体实现和训练
  denoising其实就是在autoencoder的基础上，给输入的x加入噪声，就相当于dropout用在输入层。就是以一定概率分布（通常使用二项分布）去擦除原始input矩阵，即每个值都随机置0, 这样看起来部分数据的部分特征是丢失了。
  以这丢失的数据x’去计算y，计算z，并将z与原始x做误差迭代，这样，网络就学习了这个破损（原文叫Corruputed）的数据，这也就是所谓的改变损失函数的重构误差项。
  例如，在toolbox中的实现非常简单：
  batch_x = batch_x.*(rand(size(batch_x))>nn.inputZeroMaskedFraction)
  也就是随即把大小为(nn.inputZeroMaskedFraction)的一部分x赋成0。
  这种结构可以通过梯度下降算法来训练。
## 与dropout的比较
  当在训练深度网络时，且采用了无监督方法预训练权值，通常，Dropout和Denoise Autoencoder在使用时有一个小地方不同：
  1）Dropout在分层预训练权值的过程中是不参与的，只是后面的微调部分引入；而Denoise Autoencoder是在每层预训练的过程中作为输入层被引入，在进行微调时不参与。
  2）另外，一般的重构误差可以采用均方误差的形式，但是如果输入和输出的向量元素都是位变量，则一般采用交叉熵来表示两者的差异。
