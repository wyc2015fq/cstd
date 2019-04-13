
# Recurrent Neural Network for Text Classification with Multi-Task Learning - lpty的博客 - CSDN博客

2018年12月05日 17:34:04[lpty](https://me.csdn.net/sinat_33741547)阅读数：174



# 引言
Pengfei Liu等人在2016年的IJCAI上发表的[论文](https://arxiv.org/abs/1605.05101)，论文提到已存在的网络都是针对单一任务进行训练，但是这种模型都存在问题，即缺少标注数据，当然这是任何机器学习任务都面临的问题。
为了应对数据量少，常用的方法是使用一个无监督的预训练模型，比如词向量，实验中也取得了不错的效果，但这样的方法都是间接改善网络效果。
作者提出了一种新的方法，将多个任务联合起来训练，用来直接对网络进行改善，基于RNN设计了三种不同的信息共享机制进行训练，并在四个基准的文本分类任务中获得了较好的效果。
# 内容
## 网络结构
#### Standard LSTM
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163545676.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
最为常见的LSTM结构，利用前面的LSTM网络进行特征提取，后接一个softmax进行分类。
[
](https://img-blog.csdnimg.cn/20181205163545676.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
#### Model-I: Uniform-Layer Architecture
[
](https://img-blog.csdnimg.cn/20181205163545676.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163820810.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
对于每个任务，共享一个LSTM层和Eembedding层，同时每个任务都拥有自己的Eembedding层，也就是说，对于任务m，输入x定义为以下形式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205164227723.png)
#### Model-II: Coupled-Layer Architecture
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205164321483.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
每个任务都拥有自己的LSTM层，但是下一时刻的输入中包含了下一时刻的char及所有任务在当前时刻的隐层输出h。
但是像这种情况就需要考虑，不同任务捕捉到的上下文信息作为特定任务的输入是否存在问题，这里作者修改了cell的计算公式以决定保存多少信息：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205164958925.png)
值得注意的是，这里实际上以一对任务进行输入，即(m，n)
[
](https://img-blog.csdnimg.cn/20181205164958925.png)
#### Model-III: Shared-Layer Architecture
[
](https://img-blog.csdnimg.cn/20181205164958925.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205165019764.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
每个任务都拥有一个共享的BI-LSTM层，同时各自有一个LSTM层，LSTM的输入包括char及BI-LSTM在该时刻的隐层输出，与模型2一样，作者也修改了cell的计算公式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205165444690.png)
## 训练
对于多任务的网络结构训练，比较实用的喂数据方式是随机选择一个任务数据，在任务数据中随机选择一个样本，以此循环。
#### Fine Tuning
对于模型1和模型3，因为存在共享层，所以可以额外增加一个微调的步骤，对模型进行进一步的优化。
#### Pre-training of the shared layer with neural language model
对于模型3，共享了层可以用所有任务数据使用语言模型进行预训练。
## 数据集
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120517071860.png)
SST-1：5个情绪类别的电影影评，来自斯坦福情感数据库
SST-2：2分类电影影评，来自斯坦福数据库
SUBJ：主观性数据集，任务目的是将句子分为主观和客观
IMDB：2分类的电影影评，大多数评价为长句子
## 结果
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170812309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
上述依次是模型1到模型3在各个数据集上的训练结果，其中的single task指的是使用Standard LSTM训练得到的结果。
作者还对比了模型3与当前state-of-art模型的差别，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205171111378.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## 案例
比较有意思的是，作者使用单层LSTM和模型3进行比较，对每一时刻的hidden state进行情感分类，直观显示每个词对模型的贡献，加深对模型3效果理解：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205172403396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
从图c可以看到，模型3在merry这个词中获得了更多的信息，从而得到一个正确的判断，这个信息流则是由全局门控函数g控制从共享层流向特定任务层。
[
](https://img-blog.csdnimg.cn/20181205172403396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## 错误分析
[
](https://img-blog.csdnimg.cn/20181205172403396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)对于任务中错误的case，基本可以总结为以下两种类型：
1、具有复杂的语句结构，比如双重否定或者倒装句等，这种情况可以考虑tree-base LSTM
2、需要进行一定的推理，比如“I tried to read the time on my watch.”，假设在电影评价中，实际上可以根据推理得到一个负面的评价。
## 总结
作者利用多个任务联合训练以提升实验结果，但是对于不同的任务，其信息能否共享或者那些信息能够共享还存在一定疑问，同时在实际任务中也比较少见这种做法。
当然，如果不同任务的标记数据共享后能够对结果提升，那对于工程来说也能解决一些问题，这种思想值得仔细研究。

[
  ](https://img-blog.csdnimg.cn/20181205172403396.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)