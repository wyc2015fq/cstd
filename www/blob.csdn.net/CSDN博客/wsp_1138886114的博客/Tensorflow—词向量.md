# Tensorflow—词向量 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 20:37:00[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：107标签：[Tensorflow—词向量](https://so.csdn.net/so/search/s.do?q=Tensorflow—词向量&t=blog)
个人分类：[深度学习](https://blog.csdn.net/wsp_1138886114/article/category/7729523)

所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)












### 文章目录
- [一、Tensorflow构建流程](#Tensorflow_1)
- [1.1 Tensorflow的基本总结](#11_Tensorflow_2)
- [1.2 Tensorflow构建流程运行—保存模型—加载模型](#12_Tensorflow_9)
- [二、语言模型和词向量](#_26)
- [1 随着模型不断更新](#1__31)
- [1.1 train文件中输入语句用index表示](#11_trainindex_32)
- [1.2 模型文件中index向量转化为词向量](#12_index_42)
- [2 直接使用预先训练好的词向量如word2vec， glove](#2_word2vec_glove_58)
- [2.1 train.py文件](#21_trainpy_60)
- [2.2 model.py文件](#22_modelpy_79)
- [3 测试的时候测试数据向量化](#3__99)
- [三、word_embedding的负采样算法,Negative Sampling 模型](#word_embeddingNegative_Sampling__108)




### 一、Tensorflow构建流程

##### 1.1 Tensorflow的基本总结

(1). 使用图来表示计算；

(2). 在session中执行图；

(3). 使用tensor来表示数据；

(4). Variable维护状态

(5). 使用feed和fetch可以为任意的操作赋值和获取数据。
##### 1.2 Tensorflow构建流程运行—保存模型—加载模型
- (1). 构建图
构图过程就是自己建立网络的过程。主要构建op和op之间的传递，通过tensor实现数据传输。Op右构造器构造（tf.constent、tf.Variable、tf.matmul等），构造器的输入是op的输入。- (2). 执行图
构造完成后，首先创建一个session，创建是无参数传入，则表示启动默认图。

通过session.run()来执行图，可以添加参数，参数为op，执行完后返回op的执行后的结果。- (3). 保存图
在训练创建图时，可以定义saver = tf.train.saver()，如果没有加参数，则会默认把所有变量全部保存，此时，保存的是图中的变量。

在执行完图后，做saver.save(session,save_path,…)即可把整个模型保存下来。- (4). 恢复模型
在恢复模型前，也要先建图，在图中部署使用模型做计算的op和计算过程，此时定义的op需要与训练时定义的op对应（可以不完全复制，但此时要用的op要包含在训练op中），然后任然定义saver=tf.train.saver()，保存变量。

然后用saver.restore(session,save_path,…)加载模型，接下来，可以用session.run()执行图。
### 二、语言模型和词向量

直接使用预先训练好的词向量如word2vec， glove

省去数据读取以及预处理模块

##### 1 随着模型不断更新

###### 1.1 train文件中输入语句用index表示

```python
```python
#表示最长的句子长度max_document_length
max_document_length=max([len(x.split(" "))for xin x_text])
vocab_processor = learn.preprocessing\
                       .VocabularyProcessor(max_document_length)

#拿到句子中的index向量表示（注意这里是词的index）
x = list(vocab_processor.fit_transform(x_text))
```
```

###### 1.2 模型文件中index向量转化为词向量

```python
```python
self.input_x = tf.placeholder(tf.int32, [None, sequence_length],name="input_x")

with tf.name_scope("embedding"):

#tf.random_uniform([vocab_size, embedding_size], -1.0,1.0)词向量采用均匀分布作为初始化

# trainable=Ture表示模型中不断迭代更新词向量的值，如果trainable=False表示采用第三方预先训练好的词向量结果

self.W = tf.Variable(tf.random_uniform([vocab_size, embedding_size], -1.0,1.0),trainable=True, name="W")

self.embedded_x = tf.nn.embedding_lookup(self.W,self.input_x)
```
```

##### 2 直接使用预先训练好的词向量如word2vec， glove

如下以glove为例,格式如下，词以及其对应的词向量

###### 2.1 train.py文件

```python
```python
# 读文件获取glove中的词汇，以及其对应的词向量
vocab, embd = data_helpers.loadGloVe(FLAGS.embedding_file, 
                                     FLAGS.embedding_dim)
embedding = np.asarray(embd)

def tokenizer(iterator):
    for value in iterator:
        yield re.split(r'\s', value)

# 将glove文件中的词转化为index
vocab_processor = learn.preprocessing.VocabularyProcessor(max_sequence_length,
                                                          tokenizer_fn=tokenizer)
vocab_processor.fit(vocab)

#拿到句子中的index向量表示（注意这里是词的index）
x = list(vocab_processor.fit_transform(x_text))
```
```

###### 2.2 model.py文件

```python
```python
#传入glove的初始值的占位符
self.embedding_placeholder = tf.placeholder(tf.float32,
                                            [self.vocab_size,self.embedding_size],
                                            name="embedding")
self.input_x = tf.placeholder(tf.int32, 
                              [None, sequence_length],
                              name="input_x")

with tf.variable_scope("embedding"):
#trainable=False的设置如1.2设置
       self.W = tf.Variable(tf.constant(0.0,
                                        shape=[self.vocab_size,self.embedding_size]),
                                        trainable=False, name="W")

# 把glove初始值给self.W
self.embedding_init = tf.assign(self.W,self.embedding_placeholder)
self.embedded_x = tf.nn.embedding_lookup(self.W,input_x )
```
```

##### 3 测试的时候测试数据向量化

```python
```python
#读取train时保存的vocab文件
vocab_path = os.path.join(FLAGS.checkpoint_dir, "..", "vocab")
vocab_processor = learn.preprocessing\
                       .VocabularyProcessor\
                       .restore(vocab_path)
x_combined_indexes = list(vocab_processor.transform(x_combined))
```
```

### 三、word_embedding的负采样算法,Negative Sampling 模型




