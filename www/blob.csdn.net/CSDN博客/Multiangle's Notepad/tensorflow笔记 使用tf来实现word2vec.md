
# tensorflow笔记:使用tf来实现word2vec - Multiangle's Notepad - CSDN博客


2017年01月05日 14:50:21[multiangle](https://me.csdn.net/u014595019)阅读数：24347标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[tensorflow																](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)[word2vec																](https://so.csdn.net/so/search/s.do?q=word2vec&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[python																](https://blog.csdn.net/u014595019/article/category/5636921)[机器学习&深度学习																](https://blog.csdn.net/u014595019/article/category/3051069)[自然语言处理																](https://blog.csdn.net/u014595019/article/category/6183383)[
							](https://blog.csdn.net/u014595019/article/category/3051069)
[
																					](https://blog.csdn.net/u014595019/article/category/5636921)所属专栏：[深度学习&tensorflow笔记](https://blog.csdn.net/column/details/13414.html)[自然语言处理](https://blog.csdn.net/column/details/13461.html)[
							](https://blog.csdn.net/u014595019/article/category/5636921)
[
				](https://so.csdn.net/so/search/s.do?q=python&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=python&t=blog)

（一）[ tensorflow笔记：流程，概念和简单代码注释](http://blog.csdn.net/u014595019/article/details/52677412)
（二）[tensorflow笔记：多层CNN代码分析](http://blog.csdn.net/u014595019/article/details/52728886)
（三）[tensorflow笔记：多层LSTM代码分析](http://blog.csdn.net/u014595019/article/details/52759104)
（四）[tensorflow笔记：常用函数说明](http://blog.csdn.net/u014595019/article/details/52805444)
（五）[tensorflow笔记：模型的保存与训练过程可视化](http://blog.csdn.net/u014595019/article/details/53912710)
（六）[tensorflow笔记：使用tf来实现word2vec](http://blog.csdn.net/u014595019/article/details/54093161)
---
时隔若干个月，又绕到了word2vec。关于word2vec的原理我就不叙述了，具体可见[word2vec中的数学](http://download.csdn.net/detail/mzg12345678/7988741)，写的非常好。
我后来自己用Python实现了一遍word2vec,过程写在[自己动手写word2vec (一):主要概念和流程](http://blog.csdn.net/u014595019/article/details/51884529)以及后续的若干文章中
我当时使用的是Hierarchical Softmax+CBOW的模型。给我的感觉是比较累，既要费力去写huffman树，还要自己写计算梯度的代码，完了按层softmax速度还慢。这次我决定用tensorflow来写，除了极大的精简了代码以外，可以使用gpu对运算进行加速。此外，这次使用了负采样(negative sampling)+skip-gram模型，从而避免了使用Huffman树导致训练速度变慢的情况，适合大规模的文本。
一些相关的资料：
[word2vec 中的数学原理详解-基于 Negative Sampling 的模型](http://blog.csdn.net/u014595019/article/details/51970582)
[自己动手写word2vec (四):CBOW和skip-gram模型](http://blog.csdn.net/u014595019/article/details/51943428)
[tensorflow笔记：流程，概念和简单代码注释](http://blog.csdn.net/u014595019/article/details/52677412)
[tensorflow笔记 ：常用函数说明](http://blog.csdn.net/u014595019/article/details/52805444)
其实google已经实现过一遍word2vec了（[点这里](https://www.tensorflow.org/tutorials/word2vec/)）,我看了一下代码,感觉核心代码非常简介干练，我自己写的许多运算和函数调用也是参照它来的，但是关于外围的代码，包括数据集的生成等方面，我不是很喜欢，而且也与我的要求不符，所以我重新写了一下，并且进行了封装，增加了模型的存/取，训练过程可视化等功能，并且简化了流程。
我的模型主要分成两部分：由输入单词生成训练集的外围代码，以及用于描述模型，训练的核心代码。在训练的时候，外围代码收到一个分好词的句子，例如[‘我’,’中午’,’吃饭’],然后根据skip-gram模型，将其转化成输入集和标签集。例如
[我，中午，中午，吃饭]
[中午，我，吃饭，中午]
当然了，实际过程中输入集和标签集都是用id来表示的。生成输入集和标签集以后，将其输入核心代码进行训练。那就先从核心代码讲起吧。这篇文章中的代码是不完全的，想看完整版的可以移步[https://github.com/multiangle/tfword2vec](https://github.com/multiangle/tfword2vec)
---

# 核心代码
核心代码主要就是描述模型，计算loss,根据loss优化参数等步骤。这里计算loss直接使用了tf封装好的tf.nn.nce_loss方法，比较方便。优化方法这里也是选的最简单的梯度下降法。具体的描述就放在代码里说好了
```python
self
```
```python
.graph = tf.Graph()
```
```python
self
```
```python
.graph = tf.Graph()
        with
```
```python
self
```
```python
.graph.as_default():
```
```python
# 首先定义两个用作输入的占位符，分别输入输入集(train_inputs)和标签集(train_labels)
```
```python
self
```
```python
.train_inputs = tf.placeholder(tf.int32, shape=[
```
```python
self
```
```python
.batch_size])
```
```python
self
```
```python
.train_labels = tf.placeholder(tf.int32, shape=[
```
```python
self
```
```python
.batch_size,
```
```python
1
```
```python
])
```
```python
# 词向量矩阵，初始时为均匀随机正态分布
```
```python
self
```
```python
.embedding_dict = tf.Variable(
                tf.random_uniform([
```
```python
self
```
```python
.vocab_size,
```
```python
self
```
```python
.embedding_size],-
```
```python
1.0
```
```python
,
```
```python
1.0
```
```python
)
            )
```
```python
# 模型内部参数矩阵，初始为截断正太分布
```
```python
self
```
```python
.nce_weight = tf.Variable(tf.truncated_normal([
```
```python
self
```
```python
.vocab_size,
```
```python
self
```
```python
.embedding_size],
                                                              stddev=
```
```python
1.0
```
```python
/math.sqrt(
```
```python
self
```
```python
.embedding_size)))
```
```python
self
```
```python
.nce_biases = tf.Variable(tf.zeros([
```
```python
self
```
```python
.vocab_size]))
```
```python
# 将输入序列向量化，具体可见我的【常用函数说明】那一篇
```
```python
embed = tf.nn.embedding_lookup(
```
```python
self
```
```python
.embedding_dict,
```
```python
self
```
```python
.train_inputs)
```
```python
# batch_size
```
```python
# 得到NCE损失(负采样得到的损失)
```
```python
self
```
```python
.loss = tf.reduce_mean(
                tf.nn.nce_loss(
                    weights =
```
```python
self
```
```python
.nce_weight,
```
```python
# 权重
```
```python
biases =
```
```python
self
```
```python
.nce_biases,
```
```python
# 偏差
```
```python
labels =
```
```python
self
```
```python
.train_labels,
```
```python
# 输入的标签
```
```python
inputs = embed,
```
```python
# 输入向量
```
```python
num_sampled =
```
```python
self
```
```python
.num_sampled,
```
```python
# 负采样的个数
```
```python
num_classes =
```
```python
self
```
```python
.vocab_size
```
```python
# 类别数目
```
```python
)
            )
```
```python
# tensorboard 相关
```
```python
tf.scalar_summary(
```
```python
'loss'
```
```python
,
```
```python
self
```
```python
.loss)
```
```python
# 让tensorflow记录参数
```
```python
# 根据 nce loss 来更新梯度和embedding，使用梯度下降法(gradient descent)来实现
```
```python
self
```
```python
.train_op = tf.train.GradientDescentOptimizer(learning_rate=
```
```python
0.1
```
```python
).minimize(
```
```python
self
```
```python
.loss)
```
```python
# 训练操作
```
```python
# 计算与指定若干单词的相似度
```
```python
self
```
```python
.test_word_id = tf.placeholder(tf.int32,shape=[None])
            vec_l2_model = tf.sqrt(
```
```python
# 求各词向量的L2模
```
```python
tf.reduce_sum(tf.square(
```
```python
self
```
```python
.embedding_dict),
```
```python
1
```
```python
,keep_dims=
```
```python
True
```
```python
)
            )
            avg_l2_model = tf.reduce_mean(vec_l2_model)
            tf.scalar_summary(
```
```python
'avg_vec_model'
```
```python
,avg_l2_model)
```
```python
self
```
```python
.normed_embedding =
```
```python
self
```
```python
.embedding_dict / vec_l2_model
```
```python
# self.embedding_dict = norm_vec # 对embedding向量正则化
```
```python
test_embed = tf.nn.embedding_lookup(
```
```python
self
```
```python
.normed_embedding,
```
```python
self
```
```python
.test_word_id)
```
```python
self
```
```python
.similarity = tf.matmul(test_embed,
```
```python
self
```
```python
.normed_embedding, transpose_b=
```
```python
True
```
```python
)
```
```python
# 变量初始化操作
```
```python
self
```
```python
.init = tf.global_variables_initializer()
```
```python
# 汇总所有的变量记录
```
```python
self
```
```python
.merged_summary_op = tf.merge_all_summaries()
```
```python
# 保存模型的操作
```
```python
self
```
```python
.saver = tf.train.Saver()
```
---

# 外围代码
外围代码其实有很多，例如训练过程中变量的记录，模型的保存与读取等等，不过这与训练本身没什么关系，这里还是贴如何将句子转化成输入集和标签集的代码。对其他方面感兴趣的看官可以到[github](https://github.com/multiangle/tfword2vec)上看完整的代码。
```python
def train_by_sentence(self, input_sentence=[]):
```
```python
#  input_sentence: [sub_sent1, sub_sent2, ...]
```
```python
# 每个sub_sent是一个单词序列，例如['这次','大选','让']
```
```python
sent_num = input_sentence.__len__()
        batch_inputs = []
        batch_labels = []
        for sent
```
```python
in
```
```python
input_sentence:
```
```python
# 输入有可能是多个句子，这里每个循环处理一个句子
```
```python
for i
```
```python
in
```
```python
range(sent.__len__()):
```
```python
# 处理单个句子中的每个单词
```
```python
start = max(
```
```python
0
```
```python
,i-self
```
```python
.win
```
```python
_len)
```
```python
# 窗口为 [-win_len,+win_len],总计长2*win_len+1
```
```python
end = min(sent.__len__(),i+self
```
```python
.win
```
```python
_len+
```
```python
1
```
```python
)
```
```python
# 将某个单词对应窗口中的其他单词转化为id计入label，该单词本身计入input
```
```python
for index
```
```python
in
```
```python
range(start,end): 
                    if index == i:
                        continue
                    else:
                        input_id = self
```
```python
.word
```
```python
2id
```
```python
.get
```
```python
(sent[i])
                        label_id = self
```
```python
.word
```
```python
2id
```
```python
.get
```
```python
(sent[index])
                        if not (input_id
```
```python
and
```
```python
label_id):
```
```python
#　如果单词不在词典中，则跳过
```
```python
continue
                        batch_inputs
```
```python
.append
```
```python
(input_id)
                        batch_labels
```
```python
.append
```
```python
(label_id)
        if len(batch_inputs)==
```
```python
0
```
```python
:
```
```python
#　如果标签集为空，则跳过
```
```python
return
        batch_inputs = np
```
```python
.array
```
```python
(batch_inputs,dtype=np
```
```python
.int
```
```python
32)
        batch_labels = np
```
```python
.array
```
```python
(batch_labels,dtype=np
```
```python
.int
```
```python
32)
        batch_labels = np
```
```python
.reshape
```
```python
(batch_labels,[batch_labels.__len__(),
```
```python
1
```
```python
])
```
```python
#　生成供tensorflow训练用的数据
```
```python
feed_dict = {   
            self
```
```python
.train
```
```python
_inputs: batch_inputs,
            self
```
```python
.train
```
```python
_labels: batch_labels
        }
```
```python
# 这句操控tf进行各项操作。数组中的选项，train_op等，是让tf运行的操作，feed_dict选项用来输入数据
```
```python
_, loss_val, summary_str = self
```
```python
.sess
```
```python
.run
```
```python
([self
```
```python
.train
```
```python
_op,self
```
```python
.loss
```
```python
,self
```
```python
.merged
```
```python
_summary_op], feed_dict=feed_dict)
```
```python
# train loss，记录这次训练的loss值
```
```python
self
```
```python
.train
```
```python
_loss_records
```
```python
.append
```
```python
(loss_val)
```
```python
# self.train_loss_k10 = sum(self.train_loss_records)/self.train_loss_records.__len__()
```
```python
self
```
```python
.train
```
```python
_loss_k10 = np
```
```python
.mean
```
```python
(self
```
```python
.train
```
```python
_loss_records)
```
```python
# 求loss均值
```
```python
if self
```
```python
.train
```
```python
_sents_num %
```
```python
1000
```
```python
==
```
```python
0
```
```python
:
            self
```
```python
.summary
```
```python
_writer
```
```python
.add
```
```python
_summary(summary_str,self
```
```python
.train
```
```python
_sents_num)
            print(
```
```python
"{a} sentences dealed, loss: {b}"
```
```python
.format
```
```python
(a=self
```
```python
.train
```
```python
_sents_num,b=self
```
```python
.train
```
```python
_loss_k10))
```
```python
# train times
```
```python
self
```
```python
.train
```
```python
_words_num += batch_inputs.__len__()
        self
```
```python
.train
```
```python
_sents_num += input_sentence.__len__()
        self
```
```python
.train
```
```python
_times_num +=
```
```python
1
```

