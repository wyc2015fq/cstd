
# Convolutional Neural Networks for Sentence Classification - lpty的博客 - CSDN博客

2018年11月15日 17:19:50[lpty](https://me.csdn.net/sinat_33741547)阅读数：68标签：[TextCNN																](https://so.csdn.net/so/search/s.do?q=TextCNN&t=blog)个人分类：[论文阅读																](https://blog.csdn.net/sinat_33741547/article/category/7727913)[深度学习																](https://blog.csdn.net/sinat_33741547/article/category/7028998)[
							](https://blog.csdn.net/sinat_33741547/article/category/7727913)



# 引言
这是Yoon Kim发表于2014年的EMNLP上的论文，也就是经常说到的TextCNN。
CNN一开始提出来是作为图像或者语音这种特征连续任务的处理方式，这篇论文则是尝试将CNN用于文本这种离散型的任务中。
# 模型
前面说到CNN是处理连续型数据的，所以对于文本而言，就需要进行离散特征连续化，目前比较流行的应该是word2vec的方式，另外对于长度不同的文本就需要考虑padding了，网络结构如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115163710927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
其中n为文本长度，k为向量维数
[
](https://img-blog.csdnimg.cn/20181115163710927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## Channels
[
](https://img-blog.csdnimg.cn/20181115163710927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)上图中，文本被表示成为两层，可以考虑不同的初始化方式及训练时进不进行向量的修改。
[
](https://img-blog.csdnimg.cn/20181115163710927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## Embedding
[
](https://img-blog.csdnimg.cn/20181115163710927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)这里就是将文本转为向量的过程，论文中使用了两种方式，随机初始化和使用预训练的词向量，最终结果来看，使用预训练的词向量对结果有显著的提升。
[
](https://img-blog.csdnimg.cn/20181115163710927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## Convolutional
[
](https://img-blog.csdnimg.cn/20181115163710927.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)卷积层采用(h, d)大小的filter，其中d为embedding后词向量的维数，h则选择不同不小构造不同的filter，示例图中则是构造了h分别为2和3的两个，最终得到shape分别为(7, 1)和(6, 1)两种类型的输出。
另一个是对卷积后的输出做一个非线性变换，也就是常说的激活函数，具体公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115164845761.png)
其中w，b为超参，x为分布式向量，f为非线性激活函数，c为最终的输出。
[
](https://img-blog.csdnimg.cn/20181115164845761.png)
## Pooling
[
](https://img-blog.csdnimg.cn/20181115164845761.png)pool这里采用了max-pool，选择c向量中值最大的维度，最后将结果连接成一个向量，也就是这里会得到filter数量乘以channel数量长度的一维向量。
[
](https://img-blog.csdnimg.cn/20181115164845761.png)
## Regularization
[
](https://img-blog.csdnimg.cn/20181115164845761.png)
### Dropout
[
](https://img-blog.csdnimg.cn/20181115164845761.png)这里对pooling后的向量进行了处理，加上了一层掩码，也就是下图中的r，r属于伯努利随机变量，在逆传播时随机将某个位置置0，防止过拟合。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115165958342.png)
### l2-norms
原文如下
> We additionally constrain l2-norms of the

> weight vectors by rescaling w to have
$$
||w||_2
$$
> = s

> whenever
$$
||w||_2
$$
> > s after a gradient descent step.

> Fully Connected

> 最后是全连接层，根据任务不同决定最终输出的维度，即分类数量。

> 结果

> 数据集

> 论文中提及数据集如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115170904104.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 模型对比
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181115171001175.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 其中，

> CNN-rand为single channel， embedding采用随机初始化的方式，并在训练中进行修改；

> CNN-static为single channel， embedding采用预训练的词向量，在训练中不做修改；

> CNN-non-static为single channel， embedding采用预训练的词向量，在训练中进行修改；

> CNN-multichannel为double channel，embedding均采用预训练的词向量，在训练中其中一个进行修改，另一个保持不变。

> 可以看到，CNN-multichannel在各个数据集中的表现均较为不错，而CNN-rand则表现相对较差。

> 引用

> 1、
> [Convolutional Neural Networks for Sentence Classification](http://www.aclweb.org/anthology/D14-1181)


