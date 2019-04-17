# 语言模型（三） RNN - xmdxcsj的专栏 - CSDN博客





2015年11月29日 18:09:46[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2013








## 概况

### feedforward NN based LM

单词使用1-of-V表示，其中V表示词典的大小，单词对应的位置为1，其他为0 

输入：历史词序列，输出是当前词。比如词典大小为50k，使用5-gram，那么输入维度为50k*4 

projection：将50k降维到30，即为30*4 

隐层：100-300 

输出：大小为V，代表概率值$P(w_t|w_{t-4},w_{t-3},w_{t-2},w_{t-1})$
### recurrent NN based LM

相比于feedforward，RNN可以利用到更多的历史信息，但是需要更多的空间用来保存历史信息。 

输入：当前词$w_t$，使用1-of-v表示 

输出：下一个词$w_{t+1}$的概率，$P(w_{t+1}|w_{t},s(t-1))$，$s(t-1)$表示上一时刻隐层的输出。
## trick

### loss function

使用CE，使用MSE容易陷入局部最优，有待论证。

### learninng rate

开始学习率设为0.1，如果在validation set上面提升明显，学习率不变；如果不明显，学习率减半。

### L2 regularization

使用doubleprecision，更新权重的时候的系数，有待论证。

### explode

BPTT会导致梯度过大，导致权重过大，使得训练失败。可以使用限制梯度值的方法，将大小限制在[-15,15]，否则大数据量无法训练成功。

### computational complexity

如果每一个example都unfolding的话，会导致训练复杂度过大，可以选择N个example进行一次unfolding；参数更新online比batch要好，最实用的是使用min-batch（大小设置为10-20）。

### vocabulary trunction

减小词典的大小，Bengio将非常用词归结到一类，然后根据ngram模型对这些词分配概率。可以降低计算量，但是准确率下降较大。

### Factorization of the Output Layer

可以采用对输出结果进行分类的方式。可以根据词频对单词进行分类以减小输出层的大小，称为frequenct binning.

## dynamic evaluation

使用测试数据实时更新模型参数，所以在计算前向的同时，还需要计算BPTT，计算量太大。

## Combination of Neural Network Models

根据不同的训练策略训练处的RNN模型可能收敛于不同的局部最优，可以使用多个模型同时计算前向，然后做一下平均。

## reference

[STATISTICAL LANGUAGE MODELS BASED ON NEURAL NETWORKS](http://www.fit.vutbr.cz/~imikolov/rnnlm/thesis.pdf)





