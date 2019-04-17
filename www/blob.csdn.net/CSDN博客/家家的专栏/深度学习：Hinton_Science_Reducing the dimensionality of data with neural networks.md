# 深度学习：Hinton_Science_Reducing the dimensionality of data with neural networks - 家家的专栏 - CSDN博客





2012年11月16日 10:20:44[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：16502








近日，闲来得空，又不停的听到Deep learning （DL）相关的突破~

故来研究下Deep learning的相关东西~

在Deep learning 的学习资源中找到，关于Deep belief network的相关资源

[http://www.cs.toronto.edu/~hinton/MatlabForSciencePaper.html](http://www.cs.toronto.edu/~hinton/MatlabForSciencePaper.html)

这个资源的下载并运行，有一个需要注意的地方，就是数据文件：

要使用**gunzip **train-images-idx3-ubyte.gz 进行解压~

若使用winzip解压后，会损坏相关数据文件~

导致matlab code 中fopen文件 有错误~



本code涉及到的paper是2006 Science上的Hinton这篇:“reducing the dimensionality of data with neural networks”

 这篇paper来做什么的？（摘要）

通过一个小的中间层来重构高维输入向量，训练一个多层神经网络，最终使得高维数据可以转化为低维信号~

原有神经网络（Neural network, NN）求解权重时，**存在的问题**：利用梯度下降来求解权重~  但是严重依赖于初始化权重的好坏~

这篇paper描述一种有效地方法，来初始化权重，利用深度自解码网络（Deep autoencoder networks）来学习低维信号~

这种降维方法，比PCA(principal compenent analysis)效果要好的多~



降维有助于分类、可视化、交流和高维信号的存储~

一个简单常用的方法就是PCA,，找到数据集中最大方差方向.....

这篇paper描述一种非线性的PCA 的推广，利用一个自适应的、多层的**编码网络**，达到降维的目的。类似地，**解码网络**来重建数据~

在这两种网络中，随机初始化权重，通过最小化原始数据及其重建数据之间的差异，进行训练。

利用链规则来反馈错误，首先通过解码网络（Decoder），然后通过编码网络（Encoder），可以获得梯度。

![](https://img-my.csdn.net/uploads/201211/16/1353034465_3492.JPG)

多层RBM（Restricted Boltzmann machine）

像素对应着可见单元（**v**）~ 

特征描述子对应着隐单元**h**~

![](https://img-my.csdn.net/uploads/201211/16/1353036267_3307.JPG)

因为所有节点单元均为二进制的~  所以采用以下的激发函数来实现

可见层概率：

![](https://img-my.csdn.net/uploads/201211/16/1353036774_2826.JPG)![](https://img-my.csdn.net/uploads/201211/16/1353050284_8512.JPG)

权重更新：

![](https://img-my.csdn.net/uploads/201211/16/1353050499_4891.JPG)



疑问：这是如何把样本间建立联系呢？ 



