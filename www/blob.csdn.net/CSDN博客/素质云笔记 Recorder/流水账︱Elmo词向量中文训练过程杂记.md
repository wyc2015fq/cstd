
# 流水账︱Elmo词向量中文训练过程杂记 - 素质云笔记-Recorder... - CSDN博客

2018年08月21日 22:53:21[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：13378



## 文章目录
## [@[toc]](#toc_0)
## [1 elmo是什么？](#1_elmo_4)
## [ELMo的 特点：](#ELMo__21)
## [2 Elmo训练有哪些好项目？](#2_Elmo_30)
## [**有训练过程的项目**](#_33)
## [**预训练模型：**](#_40)
## [3 Elmo训练流程](#3_Elmo_57)
## [3.1 elmo训练流程](#31_elmo_59)
## [3.2 elmo如何fine-tune到其他领域？？](#32_elmofinetune_66)
## [3.3 elmo具体使用的方式](#33_elmo_69)
## [4 英文预训练模型](#4__81)
## [4.1 首推[Elmo Embeddings in Keras with TensorFlow hub](https://towardsdatascience.com/elmo-embeddings-in-keras-with-tensorflow-hub-7eb6f0145440)](#41_Elmo_Embeddings_in_Keras_with_TensorFlow_hubhttpstowardsdatasciencecomelmoembeddingsinkeraswithtensorflowhub7eb6f0145440_85)
## [4.2 allenai/bilm-tf官方使用方式](#42_allenaibilmtf_140)
## [4.3 UKPLab/elmo-bilstm-cnn-crf](#43_UKPLabelmobilstmcnncrf_188)
## [4.4 Using ELMo programmatically](#44_Using_ELMo_programmatically_276)
## [5 中文训练与相关经验](#5__305)
## [5.1 相关训练项目](#51__307)
## [5.2 elmo实战经验小结](#52_elmo_365)
## [5.2.1 一则](#521__366)
## [5.2.2 二则](#522__374)
## [5.2.3 三则](#523__386)
## [5.2.4 四则](#524__401)

# 1 elmo是什么？
参考：[《文本嵌入的经典模型与最新进展》](https://mp.weixin.qq.com/s?__biz=MjM5ODU3OTIyOA==&mid=2650671849&idx=3&sn=28c4f3cc3192380cfe0a12dd759997ee&chksm=bec2379a89b5be8c444dc900254c46d21852a797337e414311100c2a090342be183234019c25&mpshare=1&scene=1&srcid=0621BfZjAbAPfeYLlc9DGM6l#rd)
人们已经提出了大量可能的词嵌入方法。最常用的模型是 word2vec 和 GloVe，它们都是基于分布假设的无监督学习方法（在相同上下文中的单词往往具有相似的含义）。
虽然有些人通过结合语义或句法知识的有监督来增强这些无监督的方法，但纯粹的无监督方法在 2017-2018 中发展非常有趣，最著名的是 FastText（word2vec的扩展）和 ELMo（最先进的上下文词向量）。
![这里写图片描述](https://img-blog.csdn.net/20180821201207935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在ELMo 中，每个单词被赋予一个表示，它是它们所属的整个语料库句子的函数。所述的嵌入来自于计算一个两层双向语言模型（LM）的内部状态，因此得名「ELMo」：Embeddings from Language Models。
[ELMo embeddings论文路径](https://towardsdatascience.com/elmo-embeddings-in-keras-with-tensorflow-hub-7eb6f0145440)
![这里写图片描述](https://img-blog.csdn.net/2018082123011119?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180821230116901?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### ELMo的 特点：
ELMo 的输入是字母而不是单词。因此，他们可以利用子字词单元来计算有意义的表示，即使对于词典外的词（如 FastText这个词）也是如此。
ELMo 是 biLMs   几层激活的串联。语言模型的不同层对单词上的不同类型的信息进行编码（如在双向LSTM神经网络中，词性标注在较低层编码好，而词义消歧义用上层编码更好）。连接所有层可以自由组合各种文字表示，以提高下游任务的性能。
---2 Elmo训练有哪些好项目？

---闲话不多数，理论自己补，因为笔者懒得复现，于是就去找捷径。。。

---有训练过程的项目

---就是两个项目：

---[allenai/bilm-tf](https://github.com/allenai/bilm-tf)

---和

---[UKPLab/elmo-bilstm-cnn-crf](https://github.com/UKPLab/elmo-bilstm-cnn-crf/blob/b08e170d91ecbd8e2b34101815e83e564449e39d/README.md)

---。

---elmo是原生于该项目

---[allenai/bilm-tf](https://github.com/allenai/bilm-tf)

---，py3.5

---tf1.2就可以运行，当然有些时间需要预装他们的

---[allennlp](https://github.com/allenai/allennlp)

---，原生的是自带训练模块。

---那么基于此，UKPlab（deeplearning4）改编了一个版本

---[UKPLab/elmo-bilstm-cnn-crf](https://github.com/UKPLab/elmo-bilstm-cnn-crf/blob/b08e170d91ecbd8e2b34101815e83e564449e39d/README.md)

---，配置为py3 + tf1.8，而且应用在了bilstm-cnn-crf任务之中。两个版本因为对tf版本要求不一，所以最好用他们的docker。

---预训练模型：

---还有tensorflow hub之中（双版本，

---[1版](https://www.tensorflow.org/hub/modules/google/elmo/1)

---、

---[2版](https://github.com/tensorflow/hub/blob/master/docs/modules/google/elmo/2.md)

---），有英文的预训练模型，可以直接拿来用的那种，于是有很多延伸：

---项目一：

---[PrashantRanjan09/WordEmbeddings-Elmo-Fasttext-Word2Vec](https://github.com/PrashantRanjan09/WordEmbeddings-Elmo-Fasttext-Word2Vec)

---，该项目对比了

---0 – Word2vec, 1 – Gensim FastText, 2- Fasttext (FAIR), 3-

---ELMo，几种词向量。但是引用的是hub中预训练的模型，没有自带训练模块；

---项目二：

---[strongio/keras-elmo](https://github.com/strongio/keras-elmo/blob/master/Elmo%20Keras.ipynb)

---的 Elmo Embeddings in Keras with TensorFlow

---hub，在hub基础上用keras训练了一个简单二分类情感，非常赞的教程，但是没有提供训练模块，调用的是hub。

---还有挺多小项目的，大多基于tf-hub（提示，hub要tf 1.7以上）预训练模型做一些小应用，那么问题就来了，英文有预训练，

---中文呢？

---笔者找到了

---[searobbersduck/ELMo_Chin](https://github.com/searobbersduck/ELMo_Chin)

---，该作用小说训练了一套，笔者按照提示也是能够训练，只不过该作者只写了训练过程没有写训练完怎么用起来，所以需要和前面几个项目对起来看。

---3 Elmo训练流程

---3.1 elmo训练流程

---allenNLP给出的解答，计算elmo的流程：

---Prepare input data and a vocabulary file.

---Train the biLM.

---Test (compute the perplexity of) the biLM on heldout data.

---Write out the weights from the trained biLM to a hdf5 file.(checkpoint -> hdf5)

---3.2 elmo如何fine-tune到其他领域？？

---First download the checkpoint files above. Then prepare the dataset as described in the section “Training a biLM on a new corpus”, with the exception that we will use the existing vocabulary file instead of creating a new one. Finally, use the script bin/restart.py to restart training with the existing checkpoint on the new dataset. For small datasets (e.g. < 10 million tokens) we only recommend tuning for a small number of epochs and monitoring the perplexity on a heldout set, otherwise the model will overfit the small dataset.

---3.3 elmo具体使用的方式

---来自

---[allennlp/Using pre-trained models](https://github.com/allenai/bilm-tf#how-to-do-fine-tune-a-model-on-additional-unlabeled-data)

---，三种使用方式，其中提到的使用方式为整段/整个数据集一次性向量化并保存，There are three ways to integrate ELMo representations into a downstream task, depending on your use case.

---Compute representations on the fly from raw text using character  input. This is the most general method and will handle any input  text. It is also the most computationally expensive.

---Precompute and cache the context independent token representations,  then compute context dependent representations using the biLSTMs for  input data. This method is less computationally expensive then \#1,

---but is only applicable with a fixed, prescribed vocabulary.

---Precompute the representations for your entire dataset and save to a file.

---4 英文预训练模型

---笔者抛砖引玉，给有心人整理一下英文预训练模型使用方式。

---4.1 首推

---[Elmo Embeddings in Keras with TensorFlow hub](https://towardsdatascience.com/elmo-embeddings-in-keras-with-tensorflow-hub-7eb6f0145440)

---code来自：

---[strongio/keras-elmo](https://github.com/strongio/keras-elmo/blob/master/Elmo%20Keras.ipynb)

---，只给出重点：

---\# Import our dependencies\# Import 
import tensorflow as tf
import pandas as pd
import tensorflow_hub as hub
import os
import re
from keras import backend as K
import keras.layers as layers
from keras.models import Model
import numpy as np
\# Initialize session
sess = tf.Session()
K.set_session(sess)
\# Now instantiate the elmo model
elmo_model = hub.Module("https://tfhub.dev/google/elmo/1", trainable=True)
sess.run(tf.global_variables_initializer())
sess.run(tf.tables_initializer())
\# Build our model
\# We create a function to integrate the tensorflow model with a Keras model
\# This requires explicitly casting the tensor to a string, because of a Keras quirk
def ElmoEmbedding(x):
    return elmo_model(tf.squeeze(tf.cast(x, tf.string)), signature="default", as_dict=True)["default"]
 
input_text = layers.Input(shape=(1,), dtype=tf.string)
embedding = layers.Lambda(ElmoEmbedding, output_shape=(1024,))(input_text)
dense = layers.Dense(256, activation='relu')(embedding)
pred = layers.Dense(1, activation='sigmoid')(dense)
model = Model(inputs=[input_text], outputs=pred)
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
model.summary()
\# Fit!
model.fit(train_text, 
          train_label,
          validation_data=(test_text, test_label),
          epochs=5,
          batch_size=32)
>>> Train on 25000 samples, validate on 25000 samples
>>> Epoch 1/5
>>>  1248/25000 [>.............................] - ETA: 3:23:34 - loss: 0.6002 - acc: 0.6795

---打开Hub中的模型

---hub.Module("https://tfhub.dev/google/elmo/1", trainable=True)

---，以及加载

---embedding = layers.Lambda(ElmoEmbedding, output_shape=(1024,))(input_text)

---elmo词向量。

---4.2 allenai/bilm-tf官方使用方式

---主要是第三章提到的三种使用方式：

---[usage_cached.py](https://github.com/allenai/bilm-tf/blob/master/usage_cached.py)

---、

---[usage_character.py](https://github.com/allenai/bilm-tf/blob/master/usage_character.py)

---、

---[usage_token.py](https://github.com/allenai/bilm-tf/blob/master/usage_token.py)

---import tensorflow as tf
import os
from bilm import TokenBatcher, BidirectionalLanguageModel, weight_layers, \
    dump_token_embeddings
\# Dump the token embeddings to a file. Run this once for your dataset.
token_embedding_file = 'elmo_token_embeddings.hdf5'
dump_token_embeddings(
    vocab_file, options_file, weight_file, token_embedding_file
)
tf.reset_default_graph()
\# Build the biLM graph.
bilm = BidirectionalLanguageModel(
    options_file,
    weight_file,
    use_character_inputs=False,
    embedding_weight_file=token_embedding_file
)
\# Get ops to compute the LM embeddings.
context_embeddings_op = bilm(context_token_ids)
elmo_context_input = weight_layers('input', context_embeddings_op, l2_coef=0.0)
\# run
with tf.Session() as sess:
    \# It is necessary to initialize variables once before running inference.
    sess.run(tf.global_variables_initializer())
    \# Create batches of data.
    context_ids = batcher.batch_sentences(tokenized_context)
    question_ids = batcher.batch_sentences(tokenized_question)
    \# Compute ELMo representations (here for the input only, for simplicity).
    elmo_context_input_, elmo_question_input_ = sess.run(
        [elmo_context_input['weighted_op'], elmo_question_input['weighted_op']],
        feed_dict={context_token_ids: context_ids,
                   question_token_ids: question_ids}
    )

---4.3 UKPLab/elmo-bilstm-cnn-crf

---来自

---[elmo-bilstm-cnn-crf/Keras_ELMo_Tutorial.ipynb](https://github.com/UKPLab/elmo-bilstm-cnn-crf/blob/master/Keras_ELMo_Tutorial.ipynb)

---，与首推一样也是keras写了一个二分类。训练步骤包含：

---we will include it into a preprocessing step:

---We read in the dataset (here the IMDB dataset)

---Text is tokenized and truncated to a fix length

---Each text is fed as a sentence to the AllenNLP ElmoEmbedder to get a 1024 dimensional embedding for each word in the document

---These embeddings are then fed to our neural network that we train

---import keras
import os
import sys
from allennlp.commands.elmo import ElmoEmbedder
import numpy as np
import random
from keras.models import Sequential
from keras.layers import Dense, Conv1D, GlobalMaxPooling1D, GlobalAveragePooling1D, Activation, Dropout
\# Lookup the ELMo embeddings for all documents (all sentences) in our dataset. Store those
\# in a numpy matrix so that we must compute the ELMo embeddings only once.
def create_elmo_embeddings(elmo, documents, max_sentences = 1000):
    num_sentences = min(max_sentences, len(documents)) if max_sentences > 0 else len(documents)
    print("\n\n:: Lookup of "+str(num_sentences)+" ELMo representations. This takes a while ::")
    embeddings = []
    labels = []
    tokens = [document['tokens'] for document in documents]
    
    documentIdx = 0
    for elmo_embedding in elmo.embed_sentences(tokens):  
        document = documents[documentIdx]
        \# Average the 3 layers returned from ELMo
        avg_elmo_embedding = np.average(elmo_embedding, axis=0)
             
        embeddings.append(avg_elmo_embedding)        
        labels.append(document['label'])
            
        \# Some progress info
        documentIdx += 1
        percent = 100.0 * documentIdx / num_sentences
        line = '[{0}{1}]'.format('=' * int(percent / 2), ' ' * (50 - int(percent / 2)))
        status = '\r{0:3.0f}%{1} {2:3d}/{3:3d} sentences'
        sys.stdout.write(status.format(percent, line, documentIdx, num_sentences))
        
        if max_sentences > 0 and documentIdx >= max_sentences:
            break
            
    return embeddings, labels

elmo = ElmoEmbedder(cuda_device=1) \#Set cuda_device to the ID of your GPU if you have one
train_x, train_y = create_elmo_embeddings(elmo, train_data, 1000)
test_x, test_y  = create_elmo_embeddings(elmo, test_data, 1000)
\# :: Pad the x matrix to uniform length ::
def pad_x_matrix(x_matrix):
    for sentenceIdx in range(len(x_matrix)):
        sent = x_matrix[sentenceIdx]
        sentence_vec = np.array(sent, dtype=np.float32)
        padding_length = max_tokens - sentence_vec.shape[0]
        if padding_length > 0:
            x_matrix[sentenceIdx] = np.append(sent, np.zeros((padding_length, sentence_vec.shape[1])), axis=0)
    matrix = np.array(x_matrix, dtype=np.float32)
    return matrix
train_x = pad_x_matrix(train_x)
train_y = np.array(train_y)
test_x = pad_x_matrix(test_x)
test_y = np.array(test_y)
print("Shape Train X:", train_x.shape)
print("Shape Test Y:", test_x.shape)
\# Simple model for sentence / document classification using CNN + global max pooling
model = Sequential()
model.add(Conv1D(filters=250, kernel_size=3, padding='same'))
model.add(GlobalMaxPooling1D())
model.add(Dense(250, activation='relu'))
model.add(Dense(1, activation='sigmoid'))
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
model.fit(train_x, train_y, validation_data=(test_x, test_y), epochs=10, batch_size=32)

---在

---ElmoEmbedder

---把tensorflow预训练模型加载。

---4.4 Using ELMo programmatically

---来自

---[allennlp   Using ELMo programmatically](https://github.com/allenai/allennlp/blob/master/tutorials/how_to/elmo.md)

---的片段

---from allennlp.modules.elmo import Elmo, batch_to_ids
options_file = "https://s3-us-west-2.amazonaws.com/allennlp/models/elmo/2x4096_512_2048cnn_2xhighway/elmo_2x4096_512_2048cnn_2xhighway_options.json"
weight_file = "https://s3-us-west-2.amazonaws.com/allennlp/models/elmo/2x4096_512_2048cnn_2xhighway/elmo_2x4096_512_2048cnn_2xhighway_weights.hdf5"
elmo = Elmo(options_file, weight_file, 2, dropout=0)
\# use batch_to_ids to convert sentences to character ids
sentences = [['First', 'sentence', '.'], ['Another', '.']]
character_ids = batch_to_ids(sentences)
embeddings = elmo(character_ids)
\# embeddings['elmo_representations'] is length two list of tensors.
\# Each element contains one layer of ELMo representations with shape
\# (2, 3, 1024).
\#   2    - the batch size
\#   3    - the sequence length of the batch
\#   1024 - the length of each ELMo vector

---If you are not training a pytorch model, and just want numpy arrays as output then use allennlp.commands.elmo.ElmoEmbedder.

---5 中文训练与相关经验

---5.1 相关训练项目

---一共有三个中文训练的源头。

---（1）可参考：

---[searobbersduck/ELMo_Chin](https://github.com/searobbersduck/ELMo_Chin)

---，不过好像过程中有些问题，笔者还没证实原因。

---（2）博文：

---[《如何将ELMo词向量用于中文》](http://www.linzehui.me/2018/08/12/%E7%A2%8E%E7%89%87%E7%9F%A5%E8%AF%86%E7%82%B9/%E5%A6%82%E4%BD%95%E5%B0%86ELMo%E8%AF%8D%E5%90%91%E9%87%8F%E7%94%A8%E4%BA%8E%E4%B8%AD%E6%96%87/)

---，该教程用glove作为初始化向量，思路如下：

---将预训练的词向量读入

---修改bilm-tf代码

---option部分

---添加给embedding weight赋初值

---添加保存embedding weight的代码


---开始训练，获得checkpoint和option文件

---运行脚本，获得language model的weight文件

---将embedding weight保存为hdf5文件形式

---运行脚本，将语料转化成ELMo embedding。

---（3）

---[HIT-SCIR/ELMoForManyLangs](https://github.com/HIT-SCIR/ELMoForManyLangs)

---，哈工大今年CoNLL评测的多国语ELMo，有繁体中文。

---其中教程中主要需要提取三个内容：

---词汇表

---vocab_seg_words_elmo.txt

---；词表文件的开头必须要有

---<S> </S> <UNK>

---，且大小写敏感。并且应当按照单词的词频降序排列。可以通过手动添加这三个特殊符号。

---立足
酸甜
冷笑
吃饭
市民
熟
金刚
日月同辉
光

---数据源进行分词

---vocab_seg_words.txt

---有 德克士 吃 [ 色 ] ， 心情 也 是 开朗 的
首选 都 是 德克士 [ 酷 ] [ 酷 ]
德克士 好样 的 ， 偶 也 发现 了 鲜萃 柠檬 饮 。
有 德克士 ， 能 让 你 真正 的 幸福 哦
以后 多 给 我们 推出 这么 到位 的 搭配 ， 德克士 我们 等 着
贴心 的 德克士 ， 吃货 们 分享 起来
又 学到 好 知识 了 ， 感谢 德克士 [ 吃惊 ]
德克士 一直 久存 于心

---参数配置表option.json

---其中有几个参数需要注意一下：

---n_train_tokens，训练集中总token数量

---max_characters_per_token，单个token最长字符串长度

---n_tokens_vocab，词汇表

---vocab_seg_words_elmo.txt

---n_characters，n_tokens_vocab + max_characters_per_token - 1 (笔者不确定)


---如何使用，笔者参考的是4.2 三种中的

---[usage_token.py](https://github.com/allenai/bilm-tf/blob/master/usage_token.py)

---，其他两种貌似总是报错。

---5.2 elmo实战经验小结

---5.2.1 一则

---一些回答来自知乎：

---[刘一佳](https://www.zhihu.com/question/288565744)

---。哈工大他们的算法ELMO用的20M词的生语料训练的，有的是他们自己IDE训练算法，比bilm-tf显存效率高一点，训练稳定性高一些。他们也给出以下几个经验：

---句法任务中，OOV比例高的数据ELMo效果好，多国语言中OOV rate与ELMo带来的提升 最为明显；

---训练数据少或接近zero-shot，one-shot，ELMo表现更好；

---训练数据较多，譬如dureader数据，elmo没啥效果；

---有些公司用了，觉得效果明显，甚至上生产环境，有的公司则效果不佳，具体任务来看。

---5.2.2 二则

---在博文

---[《吾爱NLP(5)—词向量技术-从word2vec到ELMo》](https://www.jianshu.com/p/a6bc14323d77)

---解释了一下

---ELMo与word2vec最大的不同

---：

---Contextual: The representation for each word depends on the entire context in which it is used.

---（即词向量不是一成不变的，而是根据上下文而随时变化，这与word2vec或者glove具有很大的区别）

---举个例子：针对某一词多义的词汇w="苹果"
文本序列1=“我 买了 六斤 苹果。”
文本序列2=“我 买了一个 苹果 7。”
上面两个文本序列中都出现了“苹果”这个词汇，但是在不同的句子中，它们我的含义显示是不同的，一个属于水果领域，一个属于电子产品呢领域，如果针对“苹果”这个词汇同时训练两个词向量来分别刻画不同领域的信息呢？答案就是使用ELMo。

---5.2.3 三则

---博文

---[《NAACL2018 一种新的embedding方法–原理与实验 Deep contextualized word representations (ELMo)》](https://cstsunfu.github.io/2018/06/ELMo/)

---提到：

---ELMo的效果非常好

---, 我自己在SQuAD数据集上可以提高3个左右百分点的准确率. 因为是上下文相关的embedding,

---所以在一定程度上解决了一词多义的语义问题.

---ELMo速度非常慢

---, 数据集中包含越10万篇短文, 每篇约400词, 如果将batch设置为32, 用glove词向量进行编码,

---过3个biLSTM, 3个Linear, 3个softmax/logsoftmax(其余dropout, relu这种忽略不计),

---在1080Ti(TiTan XP上也差不多)总共需要约15分钟训练完(包括bp)一个epoch. 而如果用ELMo对其进行编码,

---仅

---编码时间就近一个小时

---, 全部使用的话因为维度非常大, 显存占用极高, 需要使用多张卡,

---加上多张卡之间调度和数据传输的花销一个epoch需要2+小时(在4张卡上).

---文中提出的效率解决的方式：

---ELMo虽然对同一个单词会编码出不同的结果, 但是上下文相同的时候ELMo编码出的结果是不变的(这里不进行回传更新LM的参数)因为论文中发现不同任务对不同层的LM编码信息的敏感程度不同, 比如SQuAD只对第一和第二层的编码信息敏感, 那我们保存的时候可以只保存ELMo编码的一部分, 在SQuAD中只保存前两层, 存储空间可以降低1/3, 需要320G就可以了, 如果我们事先确定数据集对于所有不同层敏感程度(即上文提到的sj), 我们可以直接用系数超参sj对3层的输出直接用∑Lj=0staskjhLMk,j压缩成一个1024的向量, 这样只需要160G的存储空间即可满足需求.

---5.2.4 四则

---[Improving NLP tasks by transferring knowledge from large data](https://tsenghungchen.github.io/posts/elmo/)

---To conclude, both papers prove that NLP tasks can benefit from large data. By leveraging either parallel MT corpus or monolingual corpus, there are several killer features in contextual word representation:

---Model is able to disambiguate the same word into different

---representation based on its context.

---Thanks to the character-based convolution, representation of

---out-of-vocabulary tokens can be derived from morphological clues.

---However, ELMo can be a cut above CoVe not only because of the performance improvement in tasks, but the type of training data. Because eventually, data is what matters the most in industry. Monolingual data do not require as much of the annotation thus can be collected more easily and efficiently.

---一些测评：

![这里写图片描述](https://img-blog.csdn.net/20180826184131478?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

