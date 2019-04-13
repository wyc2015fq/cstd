
# 人脸验证之SphereFace - intflojx的博客 - CSDN博客


2018年09月04日 09:55:45[paulpanwang](https://me.csdn.net/intflojx)阅读数：148


2017的一篇cvpr，SphereFace: Deep Hypersphere Embedding for Face Recognition，继centerloss之后又一大作。
文章主要提出了归一化权值（normalize weights and zero biases）和角度间距（angular margin），基于这2个点，对传统的softmax进行了改进，从而实现了，最大类内距离小于最小类间距离的识别标准。
原始的softmax的loss损失是一个互熵损失，
![](https://img-blog.csdn.net/20170726174154410)
其中，
![](https://img-blog.csdn.net/20170726174208410)
将其代入第一个式子，得出了损失函数如下，
![](https://img-blog.csdn.net/20170726174230243)![](https://img-blog.csdn.net/20170726174239271)
进行归一化操作，将其都映射到一个单位球面上，令||W||=1，b=0，并且引入夹角，得出Modified Softmax Loss公式如下，
![](https://img-blog.csdn.net/20170726174309402)
![](https://img-blog.csdn.net/20170726174327172)
在此基础上，再引入angular margin，用m表示，最终产生A-softmax的loss公式
![](https://img-blog.csdn.net/20170726174339889)
![](https://img-blog.csdn.net/20170726174349561)
在LFW和YTF数据集上的识别率分别如下图所示，可以看出LFW已经达到了99.42的识别率。
![](https://img-blog.csdn.net/20170726174400950)
这里分析一下，作者A-softmax 的参数，
![](https://img-blog.csdn.net/20170726174419221)
在margin_inner_product_layer.cpp的Forward_cpu函数中，
![](https://img-blog.csdn.net/20170726174439080)
也就是说，![](https://img-blog.csdn.net/20170726174452192)
计算出来也就是说，lambda=5（iteration>1658）lambda=大于5的值（iteration<1658）
训练过程loss下降如下，
![](https://img-blog.csdn.net/20170811130626272)
在LFW上10折测试的结果如下，
![](https://img-blog.csdn.net/20170811130548808)

