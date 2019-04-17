# 声学模型学习笔记（三） DNN-HMM hybrid system - xmdxcsj的专栏 - CSDN博客





2016年10月08日 19:54:18[xmucas](https://me.csdn.net/xmdxcsj)阅读数：8038








## architecture

![这里写图片描述](https://img-blog.csdn.net/20161008195355115)

声学信号使用HMM框架建模，每个状态的生成概率使用DNN替换原来的GMM进行估计，DNN每个单元的输出表示状态的后验概率。
## decoding

实际的语音识别解码的时候使用的是似然概率： 


$\hat{w}=argmax_{w}p(w|x)=argmax_{w}p(x|w)p(w)/p(x)=argmax_wp(x|w)p(w)$

其中声学部分概率为$p(x|w)$，使用的是似然概率。 

所以需要将DNN输出的后验概率转化为似然概率： 


$p(x_t|q_t=s)=p(q_t=s|x_t)*p(x_t)/p(s)$
$p(x_t)$表示观察值的概率，跟词序列无关可以忽略。 
$p(s)$表示状态的先验概率，可以使用训练语料的频率统计近似。实际使用时的先验概率有时无关紧要，但是可以缓解训练语料的标注偏移问题（比如训练语料包含大量的silience，从而导致silience的后验概率偏大）。 

最终的声学概率表示如下： 


$p(x|w)=\sum_{q}p(x|q,w)p(q|w)\approx max\pi(q_0)\prod_{t=1}^Ta_{q_{t-1}q_{t}}\prod_{t=0}^Tp(q_t|x_t)/p(q_t)$
## training

流程如下： 

- 训练CD-GMM-HMM 

- 使用CD-GMM-HMM对训练语料进行维特比解码，强制对齐特征和状态 

- dnn训练
dnn训练使用的准则是基于后验概率，而hmm训练的准则是基于似然概率。

## tricks

1.隐层个数 

隐层越多（具有更强的函数拟合能力），效果越好，超过9层基本饱和。 

2.contextual window 

一般使用左右相邻的特征拼接起来作为dnn的输入，一般9-13帧。 

在HMM中，有观察独立性假设（任意时刻的观测至于该时刻的状态有关，与其他观测和状态无关）： 


$logp(o_{t_n},...,o_{t_{n+1}-1}|s_n)\approx \sum_{t=t_n}^{t_{n+1}-1}[log(p(o_t|s_n)]$

实际上相邻帧是存在一定关系的，并不是完全独立的： 


$logp(o_{t_n},...,o_{t_{n+1}-1}|s_n)=\sum_{t=t_n}^{t_{n+1}-1}[log(p(o_t|s_n,o_{t_n},...,o_{t-1})]$

DNN的拼帧方法在一定程度上减弱了HMM的独立性假设，更符合实际关系。 

3.对senones建模 

使用cd-phone的状态比使用monophone的state建模效果更好。 

4.pretraining 

层数小于5的时候pretraining比较重要；当层数增加以后，pretraining收益变小，但是pretraining可以保证训练的鲁棒性，避免比较糟糕的参数初始化。 

5.better alignment 

更好的模型可以获得更准确的alignment，除了使用GMM-HMM的模型进行对齐，还可以使用DNN-HMM模型对训练数据进行对齐。
## 参考文献

《automatic speech recognition a deep learning approach》 chapter 6







