
# 词向量：对word2vec的理解 - lpty的博客 - CSDN博客

2017年12月11日 16:07:21[lpty](https://me.csdn.net/sinat_33741547)阅读数：5974所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、词向量
在自然语言处理中，需要把自然语言转化为计算机可以理解的形式，一般采用向量空间模型（VSM）进行表示，在这里就是词向量。
### 1、one-hot词向量
在中文处理中，将分词的结果经过降噪等预处理后，选择出来的特征在进行权重计算时，采用布尔权重，就能够得到one-hot词向量。
从表现形式上来看，向量的长度是词典的大小V，同时它的分量只有一个是1，其他全是0，1的位置对应词在词典中的索引。
但是这种词向量的长度是随着词典的大小增加而增加，同时数据非常稀疏。这样很容易受到维数灾难的困扰，而且由于是独立形式的词索引，无法体现出词与词之间的联系。
### 2、分布式词向量
分布式词向量被提出来解决one-hot向量的不足，它的基本思想是：通过某种训练将语料中的每一个词映射成一个固定长度的向量，所有的词向量组成一个向量空间，这样就可以把词向量对应成空间中的一个点。
分布式词向量的做法很好解决one-hot词向量的问题：词向量的长度可以自行确定，词与词直接的关系也可以由空间中点的距离进行衡量。机器学习中提到的word2vec或者深度学习中的word embedding都可以理解为训练分布式词向量的方法。
上面提到的训练词向量的方法，比较流行的都是使用语言模型。
## 二、语言模型
语言模型的作用实际上是确定字符串s作为一个句子出现的概率，词向量可以看成在训练语言模型的过程中出现的副产品。
统计语言模型的训练方式常见的有：n-gram模型、神经网络等方法，Word2vec是基于神经网络的方法来进行训练的，关于语言模型可以参考：[语言模型](http://blog.csdn.net/sinat_33741547/article/details/78744723)。
## 三、Word2Vec
### 1、什么是word2vec？
word2vec的本质是一个神经网络语言模型，基于语言模型进行分布式词向量的训练。
与传统神经概率语言模型不同的是，作者提出了新的方法进行训练：CBOW模型（Continuous Bag-of-Words Model）和Skip-gram模型（Continuous Skip-gram Model）。同时为了加快训练速度，使用Hierarchical Softmax和Negative Sampling这两种tricks。
### 2、CBOW模型和Skip-gram模型
![](https://img-blog.csdn.net/20171209160255382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上面可以看到，连个模型都包含三层：输入层，投影层和输出层。
#### （1）CBOW模型
CBOW模型是是在已知当前词的上下文，，，的情况下，预测当前词。
#### （2）Skip-gram模型
Skip-gram模型则是在已知当前词的情况下，预测其上下文，，，。
#### （3）模型其他知识
从形式上来看，这两种模型与n-gram模型的思想很相似。在统计语言模型中，n-gram通过引入马尔科夫假设，假设当前词只与在它之前出现的n个词有关。这实际上隐式的带入了一种思想，当前词可以在某种形式上由其上下文代替。而这种思想也是符合人的思维的，通过上下文猜测当前词（填空）、通过当前词猜测上下文（造句）。
word2vec用了一种称为window的说法，实现了类似的上下文范围划定功能。
### 3、基于Hierarchical Softmax的CBOW模型
![](https://img-blog.csdn.net/20171209163359809?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上述模型包括三层：输入层、投影层和输出层。简单的来说：
* 输入层包含了![](http://latex.codecogs.com/gif.latex?Context%28w%29)中2c个词的词向量![](http://latex.codecogs.com/gif.latex?v%28Context%28w%29_%7B1%7D%29),...,![](http://latex.codecogs.com/gif.latex?v%28Context%28w%29_%7B2c%7D%29%25u2208R_%7B2%7D%5E%7Bm%7D)![](http://latex.codecogs.com/gif.latex?%5Cin%20R%5E%7Bm%7D)。
* 投影层将输入层的2c个向量做求和累加，合并成![](http://latex.codecogs.com/gif.latex?x_%7Bw%7D)向量。
* 输出层对应一颗huffman二叉树，将V分类转换成多次二分类，得到输出。
具体流程，下面通过gensim.models.Word2vec源码，进行说明。
#### （1）遍历语料，构造词典
代码详见Word2vec.scan_vocab()，如下：

```python
for sentence_no, sentence in enumerate(sentences):
    ...
    for word in sentence:
       vocab[word] += 1
```
在Word2Vec模型中，默认sentences是可迭代类型。在中文自然语言处理中，常用分词降噪后的文本作为sentences输入，关于中文分词可以参考：[j浅谈中文分词与jieba源码](http://blog.csdn.net/sinat_33741547/article/details/78690440)
上述代码也非常简单，就是统计词频的操作。
#### （2）对语料词典进行预处理
代码详见Word2vec.scale_vocab()，
**1）低频词的处理**

```python
for word, v in iteritems(self.raw_vocab):
    if keep_vocab_item(word, v, min_count, trim_rule=trim_rule):
        retain_words.append(word)
        retain_total += v
```
代码中引入min_count参数，若某个词再语料中出现的次数小于它，则将其从词典中删除。
**2）高频词的处理**

```python
for w in retain_words:
    v = self.raw_vocab[w]
    word_probability = (sqrt(v / threshold_count) + 1) * (threshold_count / v)
```
在语料中，那些最常见的词如“”的“”等，出现次数太多以至于没有区分性，因此引入downsample方法，将词频高于某个阈值的词从字典中去除。
#### （3）构建huffman二叉树
代码详见Word2vec.create_binary_tree()，关于huffman二叉树可以参考：[Huffman Tree](http://blog.csdn.net/sinat_33741547/article/details/78770160)
**1）构建huffman二叉树**

```python
heap = list(itervalues(self.wv.vocab))
heapq.heapify(heap)
for i in xrange(len(self.wv.vocab) - 1):
    min1, min2 = heapq.heappop(heap), heapq.heappop(heap)
    heapq.heappush(heap, Vocab(count=min1.count + min2.count, index=i + len(self.wv.vocab), left=min1, right=min2))
```
基于词频构建huffman二叉树，词频越高离根节点越近，响应的搜索路劲也更短。
**2）生成叶子节点对应的huffman编码**

```python
max_depth, stack = 0, [(heap[0], [], [])]
while stack:
    node, codes, points = stack.pop()
    if node.index < len(self.wv.vocab):
        # leaf node => store its path from the root
        node.code, node.point = codes, points
        max_depth = max(len(codes), max_depth)
     else:
        # inner node => continue recursion
        points = array(list(points) + [node.index - len(self.wv.vocab)], dtype=uint32)
        stack.append((node.left, array(list(codes) + [0], dtype=uint8), points))
        stack.append((node.right, array(list(codes) + [1], dtype=uint8), points))
```
递归寻找二叉树，为每一个叶子节点，即语料词典中的每一个词生成huffman编码。

#### （4）初始化模型参数
模型中需要训练的参数包括huffman树种每一个非叶子节点对应的向量，以及词典中每个词的词向量，代码详见Word2vec.reset_weights()，
**1）初始化叶子节点向量**

```python
self.wv.syn0 = empty((len(self.wv.vocab), self.vector_size), dtype=REAL)
# randomize weights vector by vector, rather than materializing a huge random matrix in RAM at once
for i in xrange(len(self.wv.vocab)):
    # construct deterministic seed from word AND seed argument
    self.wv.syn0[i] = self.seeded_vector(self.wv.index2word[i] + str(self.seed))
```
叶子节点的向量，也就是词典中每一个词的词向量，这里采用随机初始化的方法。
**2）初始化非叶子节点向量**

```python
self.syn1 = zeros((len(self.wv.vocab), self.layer1_size), dtype=REAL)
```
非叶子节点向量，是模型训练过程中的中间向量，与神经网络模型中的隐藏变量相似，这里采用零初始化的方法。

#### （5）输入层处理
回到前面的模型图上，输入层包含了![](http://latex.codecogs.com/gif.latex?Context%28w%29)中2c个词的词向量![](http://latex.codecogs.com/gif.latex?v%28Context%28w%29_%7B1%7D%29),...,![](http://latex.codecogs.com/gif.latex?v%28Context%28w%29_%7B2c%7D%29%25u2208R_%7B2%7D%5E%7Bm%7D)![](http://latex.codecogs.com/gif.latex?%5Cin%20R%5E%7Bm%7D)。
这里的词向量就是上面在huffman树种的叶子节点初始化的向量，一般习惯把所有词向量构建成一张look-up table，把每一个词转化为对应table中的索引，在使用的时候直接查找，能够提高效率。
#### （6）投影层处理
投影层将输入层的2c个向量做求和累加，合并成![](http://latex.codecogs.com/gif.latex?x_%7Bw%7D)向量。
之前提到，模型中隐式的带入一种思想：当前词可以在某种形式上由其上下文代替。在投影层，处理方法是直接将上下文向量相加，在gensim的模型中，还有另一种方法，是将所有上下文向量求平均。
相比传统神经概率网络模型的做法（将当前的词的前n-1个词首尾相接），这样的做法是不需要考虑缺失词的情况，在这里只是少加上词而已，向量的长度不会改变。
#### （7）输出层处理
假设有字符串s=（我，喜欢，观看，巴西，足球，世界杯），其中每个字符出现的频数如下：
我：15
喜欢：8
观看：6
巴西：5
足球：3
世界杯：1
以上述为语料，假设w=足球，那么可构建模型流程图如下：
![](https://img-blog.csdn.net/20171211111642689?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图中，白色节点即是叶子节点，也就是语料中的词，黄色节点便是中间节点了。由红色边串起来的5个节点构成路径![](http://latex.codecogs.com/gif.latex?p%5E%7Bw%7D)，其长度![](http://latex.codecogs.com/gif.latex?l%5E%7Bw%7D)=5。![](http://latex.codecogs.com/gif.latex?p_%7B1%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?p_%7B2%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?p_%7B3%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?p_%7B4%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?p_%7B5%7D%5E%7Bw%7D)为路径上的5个结点。![](http://latex.codecogs.com/gif.latex?d_%7B2%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?d_%7B3%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?d_%7B4%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?d_%7B5%7D%5E%7Bw%7D)分别为1,0,0,1，即Huffman编码为（1,0,0,1）。![](http://latex.codecogs.com/gif.latex?%5CTheta%20_%7B1%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?%5CTheta%20_%7B2%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?%5CTheta%20_%7B3%7D%5E%7Bw%7D)，![](http://latex.codecogs.com/gif.latex?%5CTheta%20_%7B4%7D%5E%7Bw%7D)为路径上的非叶子节点对应的中间向量。
上面说到，利用Huffman Tree的的分类其实是把N分类转换了多个二分类。这里的二分类是在路径中的每一次分支，进行了一次logistic回归。每一次的二分类的正负标签由Huffman编码指定，这里规定：编码为1的结点定义为负类，编码为0的结点定义为正类，即：
![](http://latex.codecogs.com/gif.latex?Label%28p%20_%7Bi%7D%5E%7Bw%7D%29%3D1-d%20_%7Bi%7D%5E%7Bw%7D%2Ci%3D2%2C3%2C%5Ccdot%20%5Ccdot%20%5Ccdot%20%2Cl%5E%7Bw%7D)
根据逻辑回归的定义，对每一次的分支可以得到以下概率：
![](https://img-blog.csdn.net/20171211132232452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
利用极大似然，很容易推导得到：
![](https://img-blog.csdn.net/20171211132548704?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
到这里，就已经将N分类转化成了多次二分类，并最终得到给定上下文的条件下，w=足球的概率。
由上可以推导得到，条件概率更一般的公式如下：
![](http://latex.codecogs.com/gif.latex?p%28w%7Ccontext%28w%29%29%3D%5Cprod_%7Bi%3D2%7D%5E%7Bl%5Ew%7D%20p%28d_i%5Ew%7C%5Cboldsymbol%7BX_w%3B%5Ctheta_%7Bi-1%7D%7D%29%3D%5Cprod_%7Bi%3D2%7D%5E%7Bl%5Ew%7D%5B%5Csigma%28z_%7Bi-1%7D%29%5E%7B1-d_i%5Ew%7D%281-%5Csigma%28z_%7Bi-1%7D%29%29%5E%7Bd_i%5Ew%7D%5D)
其中，![](http://latex.codecogs.com/gif.latex?z%3D%5Cboldsymbol%7BX_w%5ET%5Ctheta%7D)
上述条件概率的公式，如logistic回归的似然函数公式形式上并没有什么不同。logistic回归中，输入x一般是固定的向量，根据每一次分类的误差对参数θ进行修正。而在这里无论是输入x还是参数θ，都是需要在迭代过程中更新的。
#### （8）梯度优化
**1）由上述条件概率公式，可以得到目标极大似然函数如下：**
![](http://latex.codecogs.com/gif.latex?%5Cmathcal%7BL%7D%3D%5Csum_%7Bw%20%5Cin%20%5Cmathcal%7BC%7D%7D%20%5Clog%20p%28w%7Ccontext%28w%29%29%5C%5C%20%3D%5Csum_%7Bw%20%5Cin%20%5Cmathcal%7BC%7D%7D%20%5Clog%20%5Cprod_%7Bi%3D2%7D%5E%7Bl%5Ew%7D%5B%5Csigma%28z_%7Bi-1%7D%29%5E%7B1-d_i%5Ew%7D%281-%5Csigma%28z_%7Bi-1%7D%29%29%5E%7Bd_i%7D%5D%5C%5C%20%3D%5Csum_%7Bw%20%5Cin%20%5Cmathcal%7BC%7D%7D%5Csum_%7Bi%3D2%7D%5E%7Bl%5Ew%7D%5B%281-d_i%5Ew%29%5Clog%5Csigma%28z_%7Bi-1%7D%29&plus;d_i%5Ew%20%5Clog%20%281-%5Csigma%28z_%7Bi-1%7D%29%29%5D)
即可得，
![](http://latex.codecogs.com/gif.latex?L%28w%2C%20i%29%20%3D%20%281-d_i%5Ew%29%5Clog%5Csigma%28%5Cboldsymbol%7BX_w%5ET%5Ctheta_%7Bi-1%7D%7D%29&plus;d_i%5Ew%20%5Clog%20%281-%5Csigma%28%5Cboldsymbol%7BX_w%5ET%5Ctheta_%7Bi-1%7D%7D%29)

**2）对上述表达式求求偏导，**
![](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20%5CL%28w%2C%20i%29%7D%7B%5Cpartial%20%5Cboldsymbol%7BX_w%7D%7D%20%3D%20%281-d_i%5Ew-%5Cfrac%7B1%7D%7B1&plus;e%5E%7B%5Cboldsymbol%7B-X_w%5ET%5Ctheta_i-1%7D%7D%7D%29%5Cboldsymbol%7B%5Ctheta_%7Bi-1%7D%7D%5C%5C%20%5Cfrac%7B%5Cpartial%20%5CL%28w%2C%20i%29%7D%7B%5Cpartial%20%5Cboldsymbol%7B%5Ctheta_%7Bi-1%7D%7D%7D%20%3D%20%281-d_i%5Ew-%5Cfrac%7B1%7D%7B1&plus;e%5E%7B%5Cboldsymbol%7B-X_w%5ET%5Ctheta_i-1%7D%7D%7D%29%5Cboldsymbol%7BX_w%5ET%7D)

**3）那么，参数θ的更新公式如下：**
![](http://latex.codecogs.com/gif.latex?%5Ctheta_%7Bi-1%7D%20%3A%3D%20%5Ctheta_%7Bi-1%7D%20&plus;%20%5Ceta%20%281-d_i%5Ew-%5Cfrac%7B1%7D%7B1&plus;e%5E%7B%5Cboldsymbol%7B-X_w%5ET%5Ctheta_i-1%7D%7D%7D%29%5Cboldsymbol%7BX_w%5ET%7D)

**4）词向量的更新公式如下：**
![](http://latex.codecogs.com/gif.latex?v%28w%29%3A%3Dv%28w%29&plus;%5Ceta%20%5Csum_%7Bi%3D2%7D%5E%7Bl_w%7D%281-d_i%5Ew-%5Cfrac%7B1%7D%7B1&plus;e%5E%7B%5Cboldsymbol%7B-X_w%5ET%5Ctheta_i-1%7D%7D%7D%29%5Cboldsymbol%7B%5Ctheta_%7Bi-1%7D%7D)

在每一个节点中，根据节点的输入x与中间向量的计算结果进行二分类，根据分类结果对θ进行修正并记录误差量。
直到到达叶子节点，再根据之前的累计误差e，对组成输入x的每一个词向量v(u)进行修正。由于输入x是由所有上下文词向量相加得到，所以这里的修正也是直接加上误差e。
详细代码，参见gensim.models.word2vec.train_batch_cbow()，

```python
for sentence in sentences:
            word_vocabs = [model.wv.vocab[w] for w in sentence if w in model.wv.vocab and
                           model.wv.vocab[w].sample_int > model.random.rand() * 2**32]  # 语料预检查，去除未在词典中出现及词频不满足要求的词
            for pos, word in enumerate(word_vocabs):
                reduced_window = model.random.randint(model.window)  # `b` in the original word2vec code
                start = max(0, pos - model.window + reduced_window)
                window_pos = enumerate(word_vocabs[start:(pos + model.window + 1 - reduced_window)], start)
                word2_indices = [word2.index for pos2, word2 in window_pos if (word2 is not None and pos2 != pos)]
                l1 = np_sum(model.wv.syn0[word2_indices], axis=0)  # 叶子节点向量
                if word2_indices and model.cbow_mean:
                    l1 /= len(word2_indices)
                train_cbow_pair(model, word, word2_indices, l1, alpha, compute_loss=compute_loss) 训练
```
对每份语料都进行一次训练，参见gensim.models.word2vec.train_cbow_pair(),

```python
l2a = model.syn1[word.point]  # 中间节点向量
        prod_term = dot(l1, l2a.T)
        fa = expit(prod_term)  # propagate hidden -> output
        ga = (1. - word.code - fa) * alpha  # vector of error gradients multiplied by the learning rate
        if learn_hidden:
            model.syn1[word.point] += outer(ga, l1)  # learn hidden -> output
        neu1e += dot(ga, l2a)  # save error
        # loss component corresponding to hierarchical softmax
        if compute_loss: #  损失
            sgn = (-1.0)**word.code  # ch function, 0-> 1, 1 -> -1
            model.running_training_loss += sum(-log(expit(-sgn * prod_term)))
```

### 4、基于Hierarchical Softmax的Skip-gram模型
Skip-gram的基本计算方式与CBOW基本一致，不同的是skip-gram输入的是当前词的词向量，利用当前词向量预测上下文。
所以实际上，skip-gram的模型在进行梯度优化时，相比CBOW要多遍历几次，每一次都是用输入x预测上下文中的每一个，直到结束，具体的推导方式这里就不展开了。
### 5、Negative Sampling模型
回顾上面说到的内容，word2vec提出的方法是用来加速神经网络训练速度的，NEG模型与Hierarchical模型相比，去掉了复杂的Huffman树，转而利用随机负采样进行训练。
#### （1）什么是随机负采样？
在训练神经网络时，如上述基于Hierarchical的CBOW模型，每次进行一个（上下文，目标词）的样本的训练，都会对整个网络的权重进行调整。
基于语料词典V的大小，网络中拥有大规模的权重矩阵，这样调整起来需要耗费大量的时间。而负采样每次让一个训练样本仅仅更新一小部分的权重，这样用来降低梯度下降中的计算量。
#### （2）怎么进行随机负采样？
假如（上下文，w）为一组训练样本，在w为正样本的情况下，其他的词都可以当成负样本。
而在语料中出现的词，不同的词拥有不同的频数。对于那些高频词，被选负样本的概率就应该比较大；反之，对于低频词，被选中的概率就会比较小，本质上是一个带权采样问题。
具体的公式如下：
![](https://img-blog.csdn.net/20171211154916284?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上述公式中，每个单词都具有一个权重f(w)，代表单词出现的频次；另一个3/4的根号则是基于经验，论文提及这个公式效果更为出色。
代码中具体的做法是初始化一个大小为M的table，这个table由语料词典中词的索引填充。具体填充方式基于词频确定，即：
计算出的负采样概率*M = 单词在表中出现的次数
之后在进行负采样时，只需要在0-M范围内生成一个随机数，将table中对应随机数索引的词取出作为负样本即可。由上述公式可以看出，一个词的负采样概率越大，那么在表中出现的次数越多，被选中的概率也越大。
在论文中，作者提到对于小规模的数据集，选择5-20个负样本会比较好，对于大规模数据集可以仅选择2-5个负样本。
#### （3）其他
最后根据采样出来的（词，负样本），进行随机梯度下降优化即可，这里就不展开了。
## 四、参考
1、《数据结构》(c语言版)  严蔚敏等
2、《统计自然语言处理》  宗成庆
3、http://www.cnblogs.com/peghoty/p/3857839.html
4、Distributed Representations of Sentences and Documents  Mikolov
5、Efficient estimation of word representations in vector space  Mikolov

