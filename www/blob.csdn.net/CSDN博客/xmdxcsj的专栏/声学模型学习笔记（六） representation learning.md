# 声学模型学习笔记（六） representation learning - xmdxcsj的专栏 - CSDN博客





2016年10月08日 20:07:23[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2239








## feature representation

### 特征抽象

![这里写图片描述](https://img-blog.csdn.net/20161008200522902)

DNN的前L-1层可以认为是特征提取部分，最后一层认为是简单的分类层。 

相比于人工设计的特征（比如MFCC），多层（每一层sigmoid都是一种非线性变换）连接起来具有很强的特征抽象能力。 

靠近输入层的表示low-level特征；靠近输出层的表示high-level的特征，high-level的特征更为抽象。 
![这里写图片描述](https://img-blog.csdn.net/20161008200552807)

如上图，一个特点是high-level的层包含有更多的饱和神经元（activation value>0.99或者<0.01），意味着越靠近输出层，对应的特征输出更为稀疏（因为最后的输出层是0和1的分类，本身就是稀疏的）。 

另外一个特点是high-level的层产生的特征具有更好的鲁棒性和区分度。 

GMM为了简化计算，方差使用的是对角矩阵，即假设输入的特征的各个维度之间是相互独立的，所以使用的是[MFCC特征](http://blog.csdn.net/xmdxcsj/article/details/51228791)，利用DCT运算去相关；相比之下，DNN没有这方面的限制，可以直接使用高维度的fbank特征直接计算。
### 特征鲁棒性

体现在两方面：说话人和环境。

#### speaker variations

为了解决说话人不同的问题，GMM-HMM引入了VTLN和fMLLR。当这种技术用在DNN-HMM模型上面的时候，收益相对小，说明了DNN-HMM对说话人信息具有更强的鲁棒性。

#### environment variations

GMM使用VTS和MLLR技术来归一化输入特征，减弱环境噪声的影响。DNN具有更好的健壮性。 

在不同的信噪比和语速下，DNN都会优于GMM。但是在畸变比较大的情况下（比如强噪声），DNN的性能也会很差。

## fuse DNN and GMM

### DNN提取特征训练GMM-HMM

![这里写图片描述](https://img-blog.csdn.net/20161008200638557)

### fuse recognition result

GMM-HMM和DNN-HMM的识别结果进行融合提升识别效果，包括： 

- ROVER(recognizer output voting error reduction) 

- SCARF(segmental conditional random field) 

- MBR(minimum bayesian risk)
### fuse frame-level acoustic scores

多个系统的声学得分线性插值作为最后的得分。

### multistream speech recognition

有三种架构： 

- early integration: 特征结合（比如拼接） 

- intermediate integration: 每个stream的特征单独处理（不同的DNN模型），然后进行结合 

- late integration: 每个stream使用不同的DNN-HMM，在最后的识别结果进行结合
stream的划分可以有多种形式，比如不同的频带，不同的特征PLP/MFCC等。

## adaptation of DNN

三种adaption方式

### linear transformation

可以针对不同层增加线性转化层 

- input feature 

- softmax layer 

- hidden layer
### conservative training

linear transformation是针对一层做的adaption，如果对所有层做adaption可以达到更好的效果，但是这样会引入一个问题：因为adaption的数据量相比比较小，如果对整个模型进行调整的话，可能会破坏模型的信息。 

为了避免上面问题的发生，引入了conservative training。通过增加regularization来避免模型被adaption数据训练跑偏了。 

典型的两种正则方法： 

- l2 正则，通过限制模型参数不要偏离原始模型太多来实现 

- KLD(Kullback-Leibler divergence)正则，通过限制输出概率不要偏离原来模型的输出概率太多来实现
如果每个人的模型都要单独保存的话，存储空间将会是个问题。有两种方法解决这个问题： 

- 保留si-model和sa-model转化的delta矩阵，并对delta矩阵进行svd简化 

- 通过将矩阵$W_{m*n}$分解为$W_{m*r}$和$W_{r*n}$，并在中间增加矩阵$W_{r*r}$，每个speaker只需要单独存放$W_{r*r}$即可
### subspace methods

1.PCA 

线性变换矩阵可以转化为向量$a=vec(W^AADP)$，向量构成说话人的空间，然后经过PCA可以获得eigenvectors。 


$a\approx \overline{a}+\tilde{U}\tilde{g_a}$

其中$\overline{a}$是自适应参数的均值，$\tilde{U}$是简化的eigenvector矩阵，$\tilde{g_a}$是需要估计的从自适应参数向量到主成方向的映射。 

2.Speaker-Aware 
![这里写图片描述](https://img-blog.csdn.net/20161008200703292)

如上图，通过扩展特征输入，让模型拥有根据speaker信息自动调整的能力。 

可以使用i-vector特征作为增加的speaker info。
## 参考

《automatic speech recognition a deep learning approach》 chapter 9-11










