# CS 20SI|Lecture 4 Structure your TensorFlow model - 浅梦的博客 - CSDN博客





2017年02月06日 11:03:32[浅梦s](https://me.csdn.net/u012151283)阅读数：1399








前面一节中使用TensorFlow实现了简单的线性回归和逻辑斯谛回归。然而，复杂的模型是需要更好的总体设计，否则我们的模型将会变得非常混乱和很难调试。在接下来的两章中，将介绍如何有效地构建结构化的模型。 

本节将通过`word2vec`实例讲解。涉及到的部分NLP内容由于我本人理解有限所以就没有详细介绍，建议查阅相关资料 

本文只记录了部分代码，完整代码请查看[课程GitHub](https://github.com/chiphuyen/tf-stanford-tutorials/tree/master/examples)
# Agenda
- Overall structure of a model in TensorFlow 
- word2vec
- Name scope
- Embedding visualization

# Overall structure of a model in TensorFlow

## Phase 1: Assemble graph
- 定义输入输出的的容器placecholder
- 定义网络权值参数
- 定义推理模型 
- 定义损失函数 
- 定义优化器

## Phase 2: Compute

Created with Raphaël 2.1.0初始化模型参数输入训练数据在训练数据上执行推理计算损失调整模型参数

# Word Embedding

通过词嵌入可以得到词语之间语义(semantic)上的关系 

下图是使用TensorBoard中的t-SNE技术对词嵌入的结果进行可视化，可以看到和单词can语义相近的词语也均为情态动词。 
![词嵌入可视化](https://img-blog.csdn.net/20170206110705660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## word2vec

wrod2vec是词嵌入的一种方式，具体内容可以参阅论文 
[Distributed Representations of Words and Phrases and their Compositionality](http://papers.nips.cc/paper/5021-distributed-representations-of-words-and-phrases-and-their-compositionality.pdf)
[Efficient Estimation of Word Representations in Vector Space](http://arxiv.org/pdf/1301.3781.pdf)

我们需要词语的向量表示来输入到神经网络中进行后续的相关工作。 

本章中使用skip-gram model构建word2vec。 
[Word2Vec Tutorial - The Skip-Gram Model](http://mccormickml.com/2016/04/19/word2vec-tutorial-the-skip-gram-model/)

在skip-gram模型中，通过训练一个单隐层的神经网络来实现这个任务。但是我们关注的不是网络的输出，而是隐藏层的权重矩阵，这个权重矩阵就是我们需要的`word vector`或`embedding matrix`。 

在这个任务中，我们需要实现给定一句话中的中心词语(center word)来预测该词语附近的词语（上下文，语境）。 

给定一个特定的词语，查找附近的词语并随机选择一个。神经网络会输出单词表中每一个词语出现在选定词语附近的概率。
## Softmax, Negative Sampling, and Noise Contrastive Estimatio

这一小节主要介绍了word2vec的几种训练优化方式。 

在CS224N(一门关于自然语言处理与深度学习的课程)中，讲过了两种训练方式：hierachical softmax（层次softmax）和 negative sampling。由于计算softmat函数需要进行归一化，这涉及了遍历词汇表中的所有单词，计算开销大，所以排除softmax的训练方式。在CS224N中，使用的是negative sampling + skip-gram model实现的word2vec。 

Negative Sampling，负采样，是采样方法的一种，通过对样本进行采样来实现对原样本的近似。其他的还有importance sampling和target sampling。 

个人理解NegativeSampling方法是通过在单词表中抽样部分单词作为负样本来代替总体，这样在计算损失的时候，只计算在这些抽样出的样本上的损失，而避免了遍历整个单词表，由于每次抽样都是随机的。多次抽样后的结果可以近似总体分布。每次只更新正确词语和抽样出的错误词语对应的权重。这种方法很适合当样本类别数量非常多的时候使用。TF里可以使用`tf​.​nn​.​nce_loss`实现。 

Negative Sampling实际上是Noise Contrastive Estimation(NCE噪声对比估计)的一种简化版本，由于NCE还没有看，这里直接贴出一个简单解释
> 
negative sampling makes certain assumption about the number of noise samples to generate (k) and the distribution of noise samples (Q) (negative sampling assumes that kQ(w) = 1) to simplify computation 


相关文章 
[​On word embeddings - Part 2: Approximating the Softmax  -Sebastian Rudder](http://sebastianruder.com/word-embeddings-softmax/index.html)
[NotesonNoiseContrastiveEstimationandNegativeSampling](http://demo.clab.cs.cmu.edu/cdyer/nce_notes.pdf)

Mikolov等在论文[Distributed Representations of Words and Phrases and their Compositionality](http://papers.nips.cc/paper/5021-distributed-representations-of-words-and-phrases-and-their-compositionality.pdf)中提到使用Skip-gram model相比于复杂的hierarchical softmax来说能够更快的训练word2vec并在频繁出现的单词上获得更好的向量表示。 

当噪音样本数量增加时,NCE具有negative sampling所缺乏的理论保证。[Mnih and Teh(2012)](https://www.cs.toronto.edu/~amnih/papers/ncelm.pdf)说明了噪音样本数量取25的时候可以获得与常规softmax方法近似的表现，而训练速度却加快了约45倍。 

在本章中，由于NCE具有理论保证，使用NCE方法实现word2vec。 

最后注意，**基于采样的方法只在训练过程中有用，在实际预测的时候，仍然需要用完整的softmax来获得归一化的概率。**
## About the Dataset

本节使用的数据集合是2006年3月3日的经过数据清洗的英语维基百科语料库中的前100MB部分。 

然而100MB的训练数据其实并不能很好的训练出词向量，但是还是可以观察出一些有趣的现象。简单分词后大约有17,005,207个单词。 

想要获得更好的结果，可以使用[Matt Mahoney’s website​](https://cs.fit.edu/~mmahoney/compression/textdata.html)上的维基百科的dataset enwik9 的前10^9bytes 语料。
# 实战word2vec

## Phase 1: Assemble the graph
- 
Define placeholders for input and output 

输入为中心词，输出为目标词（上下文中的）。这里直接使用单词下标而不是用one-hot 向量。我们对预料库进行预处理使得每一个单词都对应一个唯一的索引下标。所有输入输出均为一个标量数字scalar。


```
center_words ​=​ tf​.​placeholder​(​tf​.​int32​,​ shape​=[​BATCH_SIZE​]) 

target_words ​=​ tf​.​placeholder​(​tf​.​int32​,​ shape​=[​BATCH_SIZE​])
```

- 
Define the weight(embedding matrix) 

我们使用一个[VOCAB_SIZE,EMBED_SIZE]的矩阵来表示，该矩阵的每一行就是词语表中每个单词的词向量，每个词向量的大小为`EMBED_SIZE`。权重矩阵初始化为-1到1的均匀分布。 
`embed_matrix ​=​ tf​.​Variable​(​tf​.​random_uniform​([​VOCAB_SIZE​,​ EMBED_SIZE​],​ ​-​1.0​,​ ​1.0​))`- Inference(计算图的前向计算) 

我们需要获得输入单词的词向量表示。事实上，对于一个独热编码后的单词输入来说，其向量与嵌入矩阵的乘积结果就是对应嵌入矩阵的某一行。也就是说，这里其实不需要做矩阵乘法运算，只需要选取对应的行即可。TF封装了`tf​.​nn​.​embedding_lookup`根据获取指定索引对应的词向量表示。 
![这里写图片描述](https://img-blog.csdn.net/20170206140427526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
`tf​.​nn​.​embedding_lookup​(​params​,​ ids​,​ partition_strategy​=​'mod'​,​ name​=​None​, validate_indices​=​True​,​ max_norm​=​None)`
`embed ​=​ tf​.​nn​.​embedding_lookup​(​embed_matrix​,​ center_words)`- 
Define the loss function 

TF封装好了`nce_loss`， 
`tf​.​nn​.​nce_loss​(​weights​,​ biases​,​ labels​,​ inputs​,​ num_sampled​,​ num_classes​,​ num_true​=​1​, sampled_values​=​None​,​ remove_accidental_hits​=​False​,​ partition_strategy​=​'mod'​, name​=​'nce_loss')`
`num_sampled`参数是训练时用来作负样本的噪声单词的数量。 

具体实现
```python
nce_bias ​=​ tf​.​Variable​(​tf​.​zeros​([​VOCAB_SIZE​])) 
loss ​=​ tf​.​reduce_mean​(​tf​.​nn​.​nce_loss​(​weights​=​nce_weight​,  biases​=​nce_bias​,  labels​=​target_words​,  inputs​=​embed​,  num_sampled​=​NUM_SAMPLED​,  num_classes​=​VOCAB_SIZE​))
```
- Define optimizer 
`optimizer ​=​ tf​.​train​.​GradientDescentOptimizer​(​LEARNING_RATE​).​minimize​(​loss)`

## Phase 2: Execute the computation

```python
with​ tf​.​Session​()​ ​as​ sess:     
    sess​.​run​(​tf​.​global_variables_initializer​()) 
    average_loss ​=​ ​0.0     ​
    batch ​=​ batch_gen​.​next​()         
    loss_batch​,​ _ ​=​ sess​.​run​([​loss​,​ optimizer​], feed_dict​={​center_words​:​ batch​[​0​],​ target_words​:​ batch​[​1​]})
    average_loss ​+=​ loss_batch         ​
    if​ ​(​index ​+​ ​1​)​ ​%​ ​2000​ ​==​ ​0:             
        ​print​(​'Average loss at step {}: {:5.1f}'​.​format​(​index ​+​ ​1​,average_loss ​/​ ​(​index ​+​ ​1​)))
```

# Name Scope

使用TensorBoard查看现在的网络结构 
![这里写图片描述](https://img-blog.csdn.net/20170206141359434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

现在的结构图可读性很差，结点遍布在各个地方。TensorBoard不知道哪些结点具有相似的功能而应该被聚集在一起。如我们可以把与input/output相关的ops联系起来，与NCE  loss相关的ops联系起来。TensorFlow提供了一种方式实现。
```python
with​ tf​.​name_scope​(​name_of_that_scope​): 
    # declare op_1 
    # declare op_2 
    # ...
```

举例来说，我们计算图可以分成3个op块，”Data”,”embed”,”NCE_LOSS”，

```python
with​ tf​.​name_scope​(​'data'​):         
    center_words ​=​ tf​.​placeholder​(​tf​.​int32​,​ shape​=[​BATCH_SIZE​],​ name​=​'center_words')         
    target_words ​=​ tf​.​placeholder​(​tf​.​int32​,​ shape​=[​BATCH_SIZE​,​ ​1​],​ name​=​'target_words') 

with​ tf​.​name_scope​(​'embed'​):     
    embed_matrix ​=​ tf​.​Variable​(​tf​.​random_uniform​([​VOCAB_SIZE​,​ EMBED_SIZE​],​ ​-​1.0​,​ ​1.0​), name​=​'embed_matrix') 
with​ tf​.​name_scope​(​'loss'​):     
    embed ​=​ tf​.​nn​.​embedding_lookup​(​embed_matrix​,​ center_words​,​ name​=​'embed')     
    nce_bias ​=​ tf​.​Variable​(​tf​.​zeros​([​VOCAB_SIZE​]),​ name​=​'nce_bias')     
    loss ​=​ tf​.​reduce_mean​(​tf​.​nn​.​nce_loss​(​weights​=​nce_weight​, biases​=​nce_bias​,​ labels​=​target_words​,​ inputs​=​embed​, num_sampled​=​NUM_SAMPLED​,​ num_classes​=​VOCAB_SIZE​), name​=​'loss')
```

看起来命名域`embed`似乎只有一个结点`embed_matrix`。事实上，它含有两个结点`tf.Variable`和`tf.random_uniform`，使用上述代码再查看计算图，更加清晰易读。点击每一个op块的右上角的+号可以查看该块包含的ops 
![这里写图片描述](https://img-blog.csdn.net/20170206142208675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

计算图中的实线表示数据流动的边。虚线表示依赖控制的边。如图中的`loss`依赖于`init`。控制依赖可以使用第2章讲的`tf.Graph.control_dependencies(control_inputs)`实现。 

图例 
![这里写图片描述](https://img-blog.csdn.net/20170206142840173?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

完整流程
```python
# Step 1: define the placeholders for input and output 
with​ tf​.​name_scope​(​"data"​):     
    center_words ​=​ tf​.​placeholder​(​tf​.​int32​,​ shape​=[​BATCH_SIZE​],​ name​=​'center_words')     
    target_words ​=​ tf​.​placeholder​(​tf​.​int32​,​ shape​=[​BATCH_SIZE​,​ ​1​],​ name​=​'target_words') 
with​ tf​.​device​(​'/cpu:0'​):     ​
    with​ tf​.​name_scope​(​"embed"​):         ​
        # Step 2: define weights. In word2vec, it's actually the weights that we care about
        embed_matrix ​=​ tf​.​Variable​(​tf​.​random_uniform​([​VOCAB_SIZE​,​ EMBED_SIZE​],​ ​-​1.0​,​ ​1.0​), name​=​'embed_matrix') 
     ​# Step 3 + 4: define the inference + the loss function     
     ​with​ tf​.​name_scope​(​"loss"​): 
         ​# Step 3: define the inference
         embed ​=​ tf​.​nn​.​embedding_lookup​(​embed_matrix​,​ center_words​,​ name​=​'embed') 
         ​# Step 4: construct variables for NCE loss
         nce_weight ​=​ tf​.​Variable​(​tf​.​truncated_normal​([​VOCAB_SIZE​,​ EMBED_SIZE​],stddev​=​1.0​ ​/​ math​.​sqrt​(​EMBED_SIZE​)), name​=​'nce_weight')         
         nce_bias ​=​ tf​.​Variable​(​tf​.​zeros​([​VOCAB_SIZE​]),​ name​=​'nce_bias') 
         ​# define loss function to be NCE loss function
         loss ​=​ tf​.​reduce_mean​(​tf​.​nn​.​nce_loss​(​weights​=​nce_weight​,biases​=​nce_bias​,​ labels​=​target_words​, inputs​=​embed​,num_sampled​=​NUM_SAMPLED​, num_classes​=​VOCAB_SIZE​),​ name​=​'loss') 
     ​# Step 5: define optimizer     
     optimizer ​=​ tf​.​train​.​GradientDescentOptimizer​(​LEARNING_RATE​).​minimize​(​loss)
```

# 面向对象编程

为了提升代码的重用性，使用面向对象的思想

```python
class​ ​SkipGramModel:     
    ​""" Build the graph for word2vec model """     
    ​def​ __init__​(​self​,​ ​params​):         
        ​pass 
    def​ _create_placeholders​(​self​):         ​
        """ Step 1: define the placeholders for input and output """         
        ​pass 
    ​def​ _create_embedding​(​self​):         ​
        """ Step 2: define weights. In word2vec, it's actually the weights that we care about """         ​
        pass 
    ​def​ _create_loss​(​self​):         ​
        """ Step 3 + 4: define the inference + the loss function """         
        ​pass 
    def​ _create_optimizer​(​self​):         
        ​""" Step 5: define optimizer """         ​
        pass
```

# t-SNE

> 
t​-​distributed stochastic neighbor embedding ​(​t​-​SNE​)​ ​is​ a machine learning algorithm ​for dimensionality reduction developed ​by​ ​Geoffrey​ ​Hinton​ ​and​ ​Laurens​ van der ​Maaten​.​ ​It​ ​is​ a nonlinear dimensionality reduction technique that ​is​ particularly well​-​suited ​for​ embedding high​-​dimensional data ​into​ a space of two ​or​ three dimensions​,​ which can ​then​ be visualized in​ a scatter plot​.​ ​Specifically​,​ it models each high​-​dimensional ​object​ ​by​ a two​-​ ​or three​-​dimensional point ​in​ such a way that similar objects are modeled ​by​ nearby points ​and dissimilar objects are modeled ​by​ distant points​. 

   The​ t​-​SNE algorithm comprises two main stages​.​ ​First​,​ t​-​SNE constructs a probability distribution over pairs of high​-​dimensional objects ​in​ such a way that similar objects have a high probability of being picked​,​ whilst dissimilar points have an extremely small probability of being picked​.​ ​Second​,​ t​-​SNE defines a similar probability distribution over the points ​in​ the low​-​dimensional map​,​ ​and​ it minimizes the ​Kullback​–​Leibler​ divergence between the two distributions ​with​ respect to the locations of the points ​in​ the map​.​ ​Note  

  that whilst the original algorithm uses the ​Euclidean​ distance between objects ​as​ the ​base of its similarity metric​,​ ​this​ should be changed ​as​ appropriate. 
使用t-SNE技术可以将高维数据投影到2维或3维进行可视化，使得在高维空间中相近点在低维空间中也相近。TensorBoard提供了t-SNE和PCA两种可视化技术。 

 最后的可视化结果就是本文最开始的部分已经给出。 

 相关代码
```python
from tensorflow.contrib.tensorboard.plugins import projector 

# 在训练好词向量后获取embed_matrix
final_embed_matrix ​=​ sess​.​run​(​model​.​embed_matrix) 
# 创建一个tf.Variable来容纳embeddings，这里不能用constans,也不能用之前模型里定义的embed_matrix.
# 获取前500个最流行的单词
embedding_var = tf.Variable(final_embed_matrix[:500],name='embedding') 
sess.run(embedding_var.initializer) 
config = projector.ProjectorConfig() 
summary_writer = tf.summary.FileWriter(LOGDIR) 

# 向config添加embedding
embedding = config.embeddings.add()
embedding.tensor_name = embedding_var.name 

# link the embeddings to their metadata file. In this case, the file that contains 
# the 500 most popular words in our vocabulary 
embedding.metadata_path = LOGDIR + '/vocab_500.tsv' 

# save a configuration file that TensorBoard will read during startup 
projector.visualize_embeddings(summary_writer, config) 

# save our embedding 
saver_embed = tf.train.Saver([embedding_var]) saver_embed.save(sess, LOGDIR + '/skip-gram.ckpt', 1)
```




















