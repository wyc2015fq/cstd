# TensorFlow--新闻阅读与个性化搜索系统（代码） - wsp_1138886114的博客 - CSDN博客





2018年09月02日 09:45:56[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：317








##### 1.导入模块

```python
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import collections
import math
import os                        #操作系统:操作文件模块
import zipfile                   #给文件解压
import numpy as np
from tempfile import gettempdir
from six.moves import urllib
from six.moves import xrange     #循环

import tensorflow as tf
```

##### 2. 获取文件并解压、初步处理

```python
def zip_file(filename,expected_bytes):              # 对相应文件进行解压  
# expected_bytes 用于检验文件的完整性和正确性            /filename 用来训练模型 的语料文件，语料就是一片文章
    local_filename = os.path.join('.\\',filename)   # 输入文件路径
    statinfo = os.stat(local_filename)              # 获取文件的属性，stat statistics 文件统计信息
    if statinfo.st_size == expected_bytes:          # 文件大小是否与（属性）统计的大小一致，判断文件是否损坏
        print('goodfile',filename)
    else:
        raise Exception('bad file')
    #对文件解压
    with zipfile.ZipFile(local_filename) as f:      # 一种简化的处理异常exception的方法
        data = tf.compat.as_str(f,read(f.namelist()[0])).split() #解压转换成字符串，变换列表
    return data
```

###### 了解文章词

```php
vocabulary = zip_file('text8.zip',31344016) #得到训练语料包含的所有词（没有进行分词，输入是英文）
print('Data size',len(vocabulary))          #一共多少个词

vocabulary_size = 50000                     # 限定5万（输入层编码的维度）高频词，低频词意义不大
                                            # 具体做项目工程的时候，根据统计来确定
```

##### 3.构造输入数据 Batch

```python
#统计，原始词列表结构化，生成batch 样本 

def build_dataset(words,n_words):  # words：原始数据vocabulary；n_words：vocabulary_size
    count = [['UNK',-1]]           # count[]列表用来统计词频， 
    #UNK(unknow):代表被过滤掉的所有低频词，例如囧，鳌等被UNK替代；-1表示还未出现，即初始化
    count.extend(collections.Counter(words).most_common(n_words -1))     
                                   # UNK  extend添加到列表的尾部
    dictionary = {}                # 编码，排在前面的，词频越高，编码值越小
    i = 0
    for word, _ in cout:
        dictionary[word] = i
        i += 1
    data = []                      # 用来存储 原文编码
    unk_count = 0                  # 用来统计被过滤的低频词
    for word in words:             # words:原文
        index = dictionary.get(word,0)  #default:若key不存在，则返回默认值
        unk_count += 1
        data.append(index)
    cout[0][1] = unk_count

    reversed_dictionary = dict(zip(dictionary.values():dictionary.key()))   
                                #把key和value 反转，zip用来转换行列
    return data,count,dictionary,reversed_dictionary
```

###### 查看统计词

```
data,count,dictionary,reversed_dictionary = build_dataset(vocabulary,vocabulary_size)
print('most common words:',count[:5])       #频数最高的五个词

data_index = 0 #对原文定位
```

##### 4. skip-gram方法：用于生成 Batch的样本

```python
def generate_batch(batch_size,num_skips,skip_windows): #skip_windows：总词长-1
    global data_index
    assert batch_size % num_skips ==0                  #随机从文中取num_skips个词
    assert num_skips <= 2 * skip_windows

    batch = np.ndarray(shape = (batch_size),dtype = np.int32)     #存放训练样本,输入，只有一行
    labels = np.ndarray(shape = (batch_size,1),dtype = np.int32)  #存放训练标注,输出，只有一列
    span = 2 * skip_windows+1                    #取词范围,长度
    buffer = collections.deque(maxlen = span)    #double-ended 双向队列，存放文本
    if (data_index + span > len(data)):
        data_index = 0                           #训练语料循环使用
    buffer.extend(data[data_index:data_index + span])

    data_index = span
    for i in range(batch_size// num_skips):         
        context_words = [w for w in range(span) if w != skip_windows]    
        #中间词的 上下文 例如【0，1，2，  4，5，6】
        random.shuffle(context_words)            #随机采样
        word_to_use = collections.deque(context_words)
        for j in range (num_skips):
            batch[i * num_skips + j] = buffer[skip_windows] #中心词
            context_words = word_to_use.pop()
            labels[i * num_skips + j,0] = buffer[context_words]
        if data_index == len(data):
            buffer[:] = data[:span]              #取到末尾数据的时候
            data_index = span
        else:
            buffer.append(data[data_index])
            data_index += 1

    data_index = (data_index + len(data) - span) % len(data)
    return batch,labels                         #输入和输出是分开放的
```

##### 5.定义神经网络初始化数据

```
batch_size = 128     #适当的偏向
embedding_size = 300 #中间层节点的个数，词向量（word2vec）的维度
skip_window = 2
num_skips = 2        #采样值 偏小（经验值）
num_sampled = 64     #采样 与soft max的计算有关

valid_size = 16      #测试集的大小
valid_window = 100   #生成随机列表的值<100
valid_examples = np.random.choice(valid_window,valid_size,replace = Fale)

gragh = tf.Gragh()   #直观理解，所在tensor 构成了一幅图
```

##### 6.构造神经网络

```
with gragh.as_default():
    train_input = tf.placebolder(tf.int32,shape = [batch_size])    #tensor 一行  placeholder占位符，存放batch样本
    train_labels = tf.placebolder(tf.int32, shape=[batch_size,1])  #tensor 一列  placeholder占位符


    with tf.device('/cpu:0'): 
        #存放所有的词,random_uniform()初始化,均匀分布  embeddings是weight矩阵 
        embeddings = tf.Variable(tf.random_uniform([vocabulary_size.embedding_size]),-1,0,1.0)    

        #找出一批子矩阵 其实就是找出batch样本对应的weight
        embed = tf.nn.embedding_lookup(embeddings,train_input)      

        valid_dataset = tf.constant(valid_examples, dtype=tf.int32)
        nce_weights = tf.Variable(
            tf.truncated_normal([vocabulary_size, embedding_size], stddev=1.0 / math.sqrt(embedding_size)))  #
        # truncated_normal是切掉左右尾巴的正态分布，stddev=1.0 / math.sqrt(embedding_size)很有名的初始化技巧
        nce_biases = tf.Variable(tf.zeros(vocabulary_size))         #定义常数bias

    # 构造损失函数 
    # nce：noise contranstive estimation 噪声对比估计  
    # 默认使用了 softmax函数（封装了平均交叉熵损失函数），损失函数近似计算
    loss = tf.reduce_mean(tf.nn_loss(weights = nce_weights,
                                    biases = nce_biases,
                                    labels = train_labels,
                                    inputs = embed,
                                    num_sampled = num_sampled,
                                    num_classes = vocabulary_size))


    # 优化器：梯度提升优化器(1.0: learning rate 学习率)
    optimizer = tf.train.GradientDcisionOptimizer(1.0).minimize(loss)   #小步长，防止抖动，梯度下降算法

    #测试用的：归一化后，计算词与词距离（夹角的余弦值）
    norm = tf.sqrt(tf.reduce_sum(tf.queare(embeddings),1,keep_dims = True))
    normalized_embeddings = embeddings/norm
    valid_embeddings = tf.nn.embedding_lookup(normalized_embeddings,valid_dataset)
    similarity = tf.matmul(valid_embeddings,
                            normalized_embeddings,
                            transpose_b = True)        #内积：夹角的余弦值

    init = tf.global_variable_initializer()            #初始化全局变量

num_steps = 100000                                     #训练迭代次数
```

##### 7. 训练

```
with tf.Session(gragh = gragh) as sesion:
    init.run()

    average_loss = 0    
    for step in arange(num_steps):
        batch_inputs,batch_labels = generate_batch(batch_size,num_skips,skip_window)
        feed_dict = {train_input:batch_inputs,train_labels:batch_labels}

        _.loss_val = session.run([optimizer,loss],feed_dict = feed_dict)
        #run()函数计算又先后
        average_loss += loss_val

        if (step % 2000 ==0):       # 每2000次 统计一下模型误差
            if(step > 0):
                average_loss /= 2000
            print('average loss at step is'.step,':',average_loss)
            average_loss = 0

    if step % 10000 ==0:
        sim = similarity.eval()
        for i in xrange(valid_size):
            valid_word = reversed_dictionary[valid_examples[1]]
            top_k = 8               # number of nearest neighbors
            nearest = (-sim[i,:]).argsort()[1:top_k + 1]
            log_str = 'Nearst to %s:' % valid_word
            for k in xrange(top_k):
                close_word = reversed_dictionary[nearest[k]]
                log_str = '%s %s,'% (log_str,close_word)
            print(log_str)

    final.embeddings = normalized_embeddings.eval() #归一化，输出词向量
```



