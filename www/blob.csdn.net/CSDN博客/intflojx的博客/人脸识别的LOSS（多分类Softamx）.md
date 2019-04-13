
# 人脸识别的LOSS（多分类Softamx） - intflojx的博客 - CSDN博客


2018年09月04日 09:47:14[paulpanwang](https://me.csdn.net/intflojx)阅读数：933


## 人脸1：N应用：
## [https://blog.csdn.net/intflojx/article/details/81278330](https://blog.csdn.net/intflojx/article/details/81278330)
## 超多分类的Softmax
早期深度人脸识别方法，框架为CNN + Softmax，**以“超多分类”这样一种比较难的任务训练CNN，强迫网络在第一个FC层形成比较紧凑的，判别力很强的深度人脸特征**，之后用于人脸识别。
## Softmax优缺点
![](https://pic1.zhimg.com/80/v2-95381ae9e2527858fffa2eca2f81c631_hd.jpg)
Softmax是soft（软化）的max。在CNN的分类问题中，我们的ground truth是one-hot形式，下面以四分类为例，理想输出应该是（1，0，0，0），或者说（100%，0%，0%，0%），这就是我们想让CNN学到的终极目标。
网络输出的幅值千差万别，输出最大的那一路对应的就是我们需要的分类结果。通常用百分比形式计算分类置信度，最简单的方式就是计算输出占比，假设输出特征是![(x_{1}, x_{2}, x_{3}, x_{4})](https://www.zhihu.com/equation?tex=%28x_%7B1%7D%2C+x_%7B2%7D%2C+x_%7B3%7D%2C+x_%7B4%7D%29)，这种最直接最最普通的方式，相对于soft的max，在这里我们把它叫做**hard的max**：
![](https://pic1.zhimg.com/80/v2-fbb793529c22bb09967015c4b7c0f939_hd.jpg)
而现在通用的是**soft的max**，将每个输出x非线性放大到exp(x)，形式如下：
![](https://pic4.zhimg.com/80/v2-19dacdbdca0f15965bcdcc61793f521f_hd.jpg)
hard的max和soft的max到底有什么区别呢？看几个例子
![](https://pic2.zhimg.com/80/v2-2450182a639cfe00d1d01b8eb4462cc5_hd.jpg)
相同输出特征情况，soft max比hard max更容易达到终极目标one-hot形式，或者说，softmax降低了训练难度，使得多分类问题更容易收敛。
到底想说什么呢？Softmax鼓励真实目标类别输出比其他类别要大，但并不要求大很多。对于人脸识别的特征映射（feature embedding）来说，**Softmax鼓励不同类别的特征分开，但并不鼓励特征分离很多**，如上表（5，1，1，1）时loss就已经很小了，此时CNN接近收敛梯度不再下降。
Softmax Loss训练CNN，MNIST上10分类的2维特征映射可视化如下：
![](https://pic1.zhimg.com/80/v2-1ce62133d0b445ec7348f33bd07c38c3_hd.jpg)
不同类别明显分开了，但这种情况并不满足我们人脸识别中特征向量对比的需求。人脸识别中特征向量相似度计算，常用欧式距离（L2 distance）和余弦距离（cosine distance），我们分别讨论这两种情况：
L2距离：L2距离越小，向量相似度越高。可能同类的特征向量距离（黄色）比不同类的特征向量距离（绿色）更大
![](https://pic2.zhimg.com/80/v2-6034febeacba9f5c10a9875e7ba4e573_hd.jpg)
cos距离：夹角越小，cos距离越大，向量相似度越高。可能同类的特征向量夹角（黄色）比不同类的特征向量夹角（绿色）更大
![](https://pic1.zhimg.com/80/v2-21f6b9816a46ee12b624feed740d4ea2_hd.jpg)
总结来说：
Softmax训练的深度特征，会把整个超空间或者超球，按照分类个数进行划分，保证类别是可分的，这一点对多分类任务如MNIST和ImageNet非常合适，因为测试类别必定在训练类别中。
但Softmax并不要求类内紧凑和类间分离，这一点非常不适合人脸识别任务，因为训练集的1W人数，相对测试集整个世界70亿人类来说，非常微不足道，而我们不可能拿到所有人的训练样本，更过分的是，一般我们还要求训练集和测试集不重叠。
所以需要改造Softmax，除了保证可分性外，还要做到特征向量类内尽可能紧凑，类间尽可能分离。
## 双约束的Loss Function
Sun Y, Chen Y, Wang X, et al.Deep learning face representation by joint identification-verification[C]// NIPS, 2014.
Sun Y, Wang X, Tang X.Deeply learned face representations are sparse, selective, and robust[C]// CVPR, 2015.
Sun Y, Liang D, Wang X, et al.Deepid3: Face recognition with very deep neural networks[J]. arXiv, 2015.
![](https://pic4.zhimg.com/80/v2-34960643bfdcfb654f70651f2b45144b_hd.jpg)
**DeepID2, DeepID2+, DeepID3**都在采用Softmax + Contrastive Loss，Contrast Loss是：
![](https://pic1.zhimg.com/80/v2-6b00dd9344dbe87b52cfa218761b07cc_hd.jpg)
**同类特征的L2距离尽可能小，不同类特征的L2距离大于margin(间隔) m**，Contrastive Loss同时约束类内紧凑和类间分离。25个patch训练25个CNN，特征联合后PAC降维，训练Joint Bayesian分类，在LFW上Softmax + Contrast Loss的DeepID2达到99.15%，多层加监督信息的DeepID2+达到99.47，采用更大的deep CNN的DeepID3达到99.53%。DeepID系列是早期的深度学习人脸识别方法，但代码都没有开源，而且深度特征是多patch联合，还要训练分类器，繁琐不实用。
工业届应用最广的还是Facenet的Triplet Loss:


