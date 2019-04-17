# Deep Speech笔记 - xmdxcsj的专栏 - CSDN博客





2017年02月03日 18:20:07[xmucas](https://me.csdn.net/xmdxcsj)阅读数：7321
个人分类：[声学模型](https://blog.csdn.net/xmdxcsj/article/category/6166365)









## Deep Speech 1

> 
Hannun, A., Case, C., Casper, J., Catanzaro, B., Diamos, G.,Elsen, E., Prenger, R., Satheesh, S., Sengupta, S., Coates,A., et al. (2014a).Deepspeech: Scaling up end-to-end speech recognition. arXiv preprint arXiv:1412.5567.


### 网络结构

![这里写图片描述](https://img-blog.csdn.net/20170203181754046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

网络输入是context特征，输出是char，训练准则是CTC，解码需要结合ngram语言模型。 

共五层，前三层是简单的DNN结构，第四层是双向RNN，第五层的输入是RNN的前向和后向单元，后面跟着softmax分类。|数据集|网络结构|
|----|----|
|300h switchboard|5隐层，2304节点|
|2300h SWB+FSH|4个RNN，每个5隐层，2304节点|
|100000h add noise|6个RNN，每个5隐层，2560节点|

### 训练数据

训练数据进行加噪处理，使用多种短时噪音。 

录制语音的时候增加噪声的场景.

### 训练优化
- Data parallelism 

训练语料按照长度排序，然后多句并行
- Model parallelism 

按照时间切分，前半段在GPU1上面计算，负责计算RNN的forward activation；后半段在GPU2上面计算，负责计算RNN的backward activation。在中间时间点交换角色。

## Deep Speech 2

> 
D. Amodei, R. Anubhai, E. Battenberg, C. Case, J. Casper, B. Catanzaro, J. Chen, M. Chrzanowski, A. Coates, G. Diamos et al., “Deep speech 2: End-to-end speech recognition in english and mandarin,” CoRR arXiv:1512.02595, 2015.


相比于Deep Speech，使用HPC技术，将训练时间由几周缩短到几天，尝试了更为复杂的网络结构。

### 网络结构

![这里写图片描述](https://img-blog.csdn.net/20170203181825172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

网络输入是context特征，输出是char（英文对应a/b/c，中文对应6000汉字），训练准则是CTC，解码需要结合ngram语言模型。
#### Batch Normalization

在网络层数更深的时候，效果更明显，收敛更快而且误差更小。 

有两种BatchNorm，在第一种上面没有收益 

第一种： $h_t^l=f(B(W^lh_T^{l-1}+U^lh_{t-1}^l))$

第二种： $h_t^l=f(B(W^lh_T^{l-1})+U^lh_{t-1}^l)$
#### SortaGrad

CTC训练的早期不稳定，长句子容易出现梯度异常（有些概率near-zero）。 

在第一个epoch，将训练句子按照长度排序，首先使用断句训练，后面的epoch再按照随机顺序。

#### GRU

GRU相比于vanilla RNN可以取得更好的结果，同时比LSTM更容易训练。

#### Convolution

在网络的最底层使用3层的CNN结构。

#### Lookahead Convolution

使用双向RNN可以获得更好的准确性，但是对on-line服务来讲，延时问题比较明显，为了解决这个问题，在RNN上面增加了一层Lookahead Convolution。 
![这里写图片描述](https://img-blog.csdn.net/20170203181841106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### adaptation

传统的Hybrid系统在语言之间迁移相对困难，end-to-end系统相对简单，只需要更换最后的输出节点就可以。

### 训练数据

英文11940h，中文9400h

### 训练优化

high performance computing

### 应用优化
- batch dispatch将线上不同的语音请求组成一个batch计算
- RNN计算使用16bit量化
- ngram查询只查询概率比较高的一些character
- 增加少量跟使用场景更匹配的语音或者文本，收益比较明显







