
# Neural Machine Translation by Jointly Learning to Align and Translate - lpty的博客 - CSDN博客

2018年12月19日 17:33:04[lpty](https://me.csdn.net/sinat_33741547)阅读数：128



# 引言
Dzmitry Bahdanau等人在2016年arXiv上的paper，原文地址[arXiv:1409.0473v7 [cs.CL] 19 May 2016](https://arxiv.org/pdf/1409.0473v2.pdf)
这篇应该是最早将Attention机制用到NLP中的论文，作者提到神经网络机器翻译的目标是训练一个单一庞大的网络去直接生成翻译结果，相比统计机器翻译由各个小组件进行结合有很大不同。
而一般的神经网络机器翻译模型都是由一个encoder与一个decoder构成，其中encoder负责将源语句编码形成一个固定长度的向量，而decoder根据这个向量生成对应的目标语句。
但这种结构存在一些问题，对于不同长度的句子，encoder都需要将句子里面的有效信息抽取压缩为一个固定长度的向量（也有其他工作研究变长向量），这样会导致模型在应对长文本时表现下降得很快；另一个则是对齐的问题，源语句与目标语句的词并不是一一对应的。
因此作者提出基于Attention机制的模型，对于源语句，encoder将语句编码成一个向量组，每个源语句中的词对应一个向量；decoder进行目标语句生成时，根据向量组的某种加权结果进行生成。这在一定程度上处理了上述的问题，并取得了相当不错的结果。
# 模型
### 网络结构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219160536408.png)
模型方面用了Bi-RNN，RNN需要递归的获取句子的全局信息，实际上很类似马尔可夫模型，所以这中情况下能够获取到的只是前文信息，因此需要增加一个反向的RNN获取后文信息，最后将两者做个拼接，当然相加可能也是一种可取的方案，只是从直觉上来说相加可能会丢掉一些信息。
[
](https://img-blog.csdnimg.cn/20181219160536408.png)
### Encoder
[
](https://img-blog.csdnimg.cn/20181219160536408.png)编码方向与一般神经网络翻译模型没有区别，文章里提到的Attention应该是Decoder Attention，只是将每个t时间步输出的隐藏层h保存下来，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219163615155.png)
### Decoder
在解码器中，一般定义目标函数为目标词$y_t$在给定上下文c和前面的所有词$\{y_1, ..., y_{t-1}\}$的情况下的最大化条件概率，作者定义如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219164636129.png)
其中$y_{i-1}$为上一时间步的目标词，$s_i$为上一时间步的隐藏层输出，而$c_i$就是本文重点需要注意的地方，$c_i$由encoder生成的隐藏层向量矩阵及上一时间步的隐藏层输出$s_{i-1}$决定，具体如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219165149620.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
其中需要注意的是，函数a可以有不用的选择，在本文中作者使用了感知机的形式，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219170028953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
最终得到的权重$a_{ij}$是对所有值的归一化概率，将所有$a_{ij}$与encoder中对应的隐藏层做加权和就得到decoder中的输入$c_i$。
[
](https://img-blog.csdnimg.cn/20181219170028953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 实验
[
](https://img-blog.csdnimg.cn/20181219170028953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)作者在英语转法语的翻译任务上进行了实验，共对比了四个模型，BLUE评分如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219171139514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
其中RNNsearch为文中提出模型，RNNenc为一般的圣经网络机器翻译模型，可以看到随着句子长度的增加，传统的模型效果大幅下降而加了Attention的模型表现均高于传统模型。
[
](https://img-blog.csdnimg.cn/20181219171139514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 结果
[
](https://img-blog.csdnimg.cn/20181219171139514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### 定量结果
[
](https://img-blog.csdnimg.cn/20181219171139514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219172355519.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
其中moses是统计机器翻译模型，运用了很多语言学形态学的知识，而神经网络翻译模型则只是简单的进行encoder和decoder转换。
[
](https://img-blog.csdnimg.cn/20181219172355519.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### 定性分析
[
](https://img-blog.csdnimg.cn/20181219172355519.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)比较有意思的是，使用了Attention的方法后，可以很直观的看到，在目标词生成的过程中，是哪些源词起到了作用：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181219172638872.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 总结
Attention在NLP中的开山之作，总的来说是一种自动加权方式，将希望联系在一起的两个部分进行‘soft’连接。
与人类的注意力不同的地方是，它实际上还是需要计算所有的位置，而不是简单的忽略，所以Attention是增加了计算的开销，但也以一种可视化的方式展示源词在生成目标词时起到的作用。
# 引用
1、Neural Machine Translation by Jointly Learning to Align and Translate

[
  ](https://img-blog.csdnimg.cn/20181219172355519.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)