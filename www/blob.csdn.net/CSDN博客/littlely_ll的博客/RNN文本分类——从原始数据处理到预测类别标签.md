# RNN文本分类——从原始数据处理到预测类别标签 - littlely_ll的博客 - CSDN博客





2018年02月19日 20:02:47[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：4617
所属专栏：[自然语言处理](https://blog.csdn.net/column/details/18554.html)









这两天做了一个小项目，是一个[文因互联](http://competition.ai100.com.cn/html/game_det.html?id=24&tab=1)文本分类的竞赛题目，但已经过期了，只是使用它的数据做一下。本次使用的RNN+LSTM模型，最终训练的正确率为87%，不过每次训练正确率有些差别，并且还有很多可调参数没有调整，只是当一个练手的了。由于训练时间很长，完整的代码以及持久化的模型和字典在我的[github](https://github.com/Alxe1/RNNClassification)上可以下载，当然也可以自己重新训练。 

本文的RNN结构主要使用了[finch](https://github.com/zhedongzheng/finch/blob/master/nlp-models/tensorflow/rnn_text_clf.py)的结构，并在此稍微做了修改。 

本文使用到的模块：
```python
import tensorflow as tf
import sklearn
import numpy as np
import pandas as pd
import math
import jieba
import pickle
import time
from collections import Counter
```

# RNN+LSTM

在此贴出实现RNN的代码：

```python
class RNNTextClassifier():
    def __init__(self,vocab_size, n_out, embedding_size=128, cell_size=128,
                 grad_clip=5.0,sess=tf.Session()):
        self.vocab_size = vocab_size
        self.embedding_size = embedding_size
        self.cell_size = cell_size
        self.grad_clip = grad_clip
        self.n_out = n_out
        self.sess = sess
        self._pointer = None
        self.buildgraph()

    def buildgraph(self):
        self.add_input_layer()
        self.add_wordembedding_layer()
        self.add_dynamic_rnn()
        self.add_output_layer()
        self.add_optimizer()

    def add_input_layer(self,):
        self.X = tf.placeholder(tf.int32, [None, None])
        self.Y = tf.placeholder(tf.int64, [None])
        self.X_seq_len = tf.placeholder(tf.int32, [None])
        self.keep_prob = tf.placeholder(tf.float32)
        self.lr = tf.placeholder(tf.float32)
        self._pointer = self.X

    def add_wordembedding_layer(self):
        embedding = tf.get_variable("encoder",
                                    [self.vocab_size,self.embedding_size],
                                    dtype=tf.float32,
                                    initializer=tf.random_uniform_initializer(-1.0,1.0))
        embedded = tf.nn.embedding_lookup(embedding, self._pointer)
        # self._pointer = tf.nn.dropout(embedded, keep_prob=self.keep_prob)
        self._pointer = embedded

    def lstm_cell(self):
        lstm_cell = tf.nn.rnn_cell.LSTMCell(num_units=self.cell_size,initializer=tf.orthogonal_initializer())
        return tf.nn.rnn_cell.DropoutWrapper(lstm_cell, output_keep_prob= self.keep_prob)

    def add_dynamic_rnn(self):
        self.outputs, self.last_state = tf.nn.dynamic_rnn(
            cell=self.lstm_cell(),
            inputs=self._pointer,
            sequence_length=self.X_seq_len,
            dtype=tf.float32
        )
    def add_output_layer(self):
        self.logits = tf.layers.dense(self.last_state.h, self.n_out)

    def add_optimizer(self):
        self.loss = tf.reduce_mean(
            tf.nn.sparse_softmax_cross_entropy_with_logits(
                logits=self.logits, labels=self.Y
            )
        )
        self.acc = tf.reduce_mean(tf.cast(tf.equal(tf.argmax(self.logits, axis=1),self.Y),dtype=tf.float32))
        #gradient clipping
        params = tf.trainable_variables()
        gradients = tf.gradients(ys=self.loss, xs=params)
        clipped_gradients, _ = tf.clip_by_global_norm(t_list=gradients, clip_norm=self.grad_clip)
        self.train_op = tf.train.AdamOptimizer(self.lr).apply_gradients(zip(clipped_gradients, params))

    def fit(self, X, Y, val_data=None, n_epoch=10, batch_size=128, exp_decay=True,
            isshuffle=True, keep_prob=0.5):
        if val_data is None:
            print("Train %d samples" % len(X))
        else:
            print("Train %d samples | Test %d samples" % (len(X), len(val_data[0])))
        log = {'loss':[], 'acc':[], 'val_loss':[], 'val_acc':[]}
        global_step = 0
        self.sess.run(tf.global_variables_initializer())
        saver = tf.train.Saver()
        for epoch in range(n_epoch):
            if isshuffle:
                X, Y = sklearn.utils.shuffle(X,Y)
            for local_step, ((X_batch, X_batch_lens), Y_batch) in enumerate(
                    zip(self.next_batch(X, batch_size), self.gen_batch(Y, batch_size))):
                lr = self.decrease_lr(exp_decay,global_step, n_epoch, len(X), batch_size)
                _, loss, acc = self.sess.run([self.train_op, self.loss, self.acc],
                                             feed_dict={self.X:X_batch,
                                                        self.Y:Y_batch,
                                                        self.X_seq_len:X_batch_lens,
                                                        self.lr:lr,
                                                        self.keep_prob:keep_prob})
                global_step += 1
                if local_step % 50 == 0:
                    print("Epoch %d | Step %d%d | Train loss: %.4f | Train acc: %.4f | lr: %.4f" % (
                        epoch+1, local_step, int(len(X)/batch_size), loss, acc, lr
                    ))
                log['loss'].append(loss)
                log['acc'].append(acc)

            if val_data is not None:
                val_loss_list, val_acc_list = [],[]
                for (X_test_batch,X_test_batch_lens), Y_test_batch in zip(self.next_batch(val_data[0], batch_size),
                                                                          self.gen_batch(val_data[1],batch_size)):
                    v_loss, v_acc = self.sess.run([self.loss, self.acc],feed_dict={
                        self.X: X_test_batch, self.Y: Y_test_batch,
                        self.X_seq_len:X_test_batch_lens, self.keep_prob:1.0
                    })
                    val_loss_list.append(v_loss)
                    val_acc_list.append(v_acc)
                val_loss, val_acc = self.list_avg(val_loss_list), self.list_avg(val_acc_list)
                log['val_loss'].append(val_loss)
                log['val_acc'].append(val_acc)
                print("val_data loss: %.4f | val_data acc: %.4f" % (val_loss, val_acc))
        saver.save(self.sess,"c:/users/ll/desktop/model/model.ckpt")
        return log

    def predict(self, X_test, batch_size=128):
        batch_pred_list = []
        for (X_test_batch, X_test_batch_lens) in self.next_batch(X_test, batch_size):
            batch_pred = self.sess.run(self.logits,feed_dict={
                self.X: X_test_batch,
                self.X_seq_len: X_test_batch_lens,
                self.keep_prob: 1.0
            })
            batch_pred_list.append(batch_pred)
        return np.argmax(np.vstack(batch_pred_list), 1)

    def pad_sentence_batch(self, sentence_batch, pad_int=0):
        max_lens = max([len(sentence) for sentence in sentence_batch])
        padded_seqs = []
        seq_lens = []
        for sentence in sentence_batch:
            padded_seqs.append(sentence + [pad_int] * (max_lens-len(sentence)))
            seq_lens.append(len(sentence))

        return padded_seqs, seq_lens

    def next_batch(self, arr, batch_size):
        for i in range(0, len(arr), batch_size):
            padded_seqs, seq_lens = self.pad_sentence_batch(arr[i:i+batch_size])
            yield padded_seqs, seq_lens

    def gen_batch(self, arr, batch_size):
        for i in range(0, len(arr), batch_size):
            yield arr[i: i+batch_size]

    def list_avg(self, l):
        return sum(l)/len(l)

    def decrease_lr(self, exp_decay, global_step, n_epoch, len_x, batch_size):
        if exp_decay:
            max_lr = 0.005
            min_lr = 0.001
            decay_rate = math.log(min_lr/max_lr) / (-n_epoch*len_x/batch_size)
            lr = max_lr*math.exp(-decay_rate*global_step)
        else:
            lr = 0.001
        return lr
```

# 文本的输入及分词

下面讲解的是数据的输入。它的训练数据是csv文件，第一列是类别标签，第二列是文本，即每行对应一个标签和相应的文本，总共11个类别。我是使用pandas读取，并进行文本的分词。由于分词时间比较长，所以我使用pickle把结果持久化。

```python
def load_data(file_in_path, pickle_text=True, pickle_out_path=None):
    '''
    file_in_path=".../input/training.csv"
    pickle_out_path=".../output/texting.txt"
    '''
    train_data = pd.read_csv(file_in_path, header=None, names=['ind', 'text'])
    texts = train_data['text']
    ind = train_data['ind']
    ind = np.asarray(ind)
    text1 = []
    for text in texts:
        text1.append(" ".join(jieba.cut(text)))
    text1 = [s.split(" ") for s in text1]
    if pickle_text:
        if pickle_out_path is not None:
            dictionary = {'ind':ind, 'texts': text1}
            with open(pickle_out_path, "wb") as f:
                pickle.dump(dictionary, f)
        else:
            print("you should provide pickle_out_path")

    return ind, text1
```

# 构建词典

构建词典，首先提取文本的关键词，我使用的方法是TF-IDF抽取关键词。其中，count是一个Counter字典，countlist是一个包含Counter字典的列表。

```python
def _tf(word, count):
    return count[word] / sum(count.values())

def _containing(word, countlist):
    return sum(1 for count in countlist if word in count)

def _idf(word,countlist):
    return math.log(len(countlist)/(1+_containing(word, countlist)))
```

我的想法是，对每行抽取一定数目的关键词（在代码中为row_key_word），把它加入到字典这个集合中，然后限制字典的大小（limits），我使用的为10000个词，并增加一个不在字典中的词的标志[UNK]，最后把它们变成索引的形式，至此，已完成字典的构建。

```python
def add_dict(texts,row_key_word=5, limits=3000):
    countlist = []
    dictionary = set()
    word2index = dict()
    for text in texts:
        countlist.append(Counter(text))
    for count in countlist:
        tfidf = dict()
        for word in count:
            tfidf[word] = _tf(word, count) * _idf(word, countlist)
        sorted_word = sorted(tfidf.items(), key=lambda x: x[1], reverse=True)[:row_key_word]
        word = [w[0] for w in sorted_word]
        for w in word:
            dictionary.add(w)
        if len(dictionary) > limits+1:
            break
    for i, word in enumerate(dictionary):
        word2index[word] = i+1 #need add the unknown word, index 0
    word2index['UNK'] = 0
    return word2index
```

# 文本转化为字典索引

这一步要把分词后的文本转化为索引以feed到RNN模型中去，这一步还是比较耗时的，所以我进行了序列化，在这一步中可调的参数是每行提取关键字的个数（row_key_word）和字典的大小（limits），当然字典是越大越好，但是耗时也非常长。最终得到的是一个文本列表。

```python
def convert_text(texts,row_key_word=5, limits=20000, ispickle=False, pickle_out_path=None):
    textlist = []
    word2index = add_dict(texts, row_key_word, limits)
    for text in texts:
        wordlist = []
        for word in text:
            if word in word2index:
                wordlist.append(word2index[word])
            else:
                wordlist.append(word2index["UNK"])
        textlist.append(wordlist)
    if ispickle is not None:
        with open(pickle_out_path, 'wb') as f:
            pickle.dump(textlist, f)
    return textlist
```

# 训练文本

```
#ind, texts = load_data(".../input/training.csv", True, ".../output/text.txt")
#id, test_texts = load_data(".../input/testing.csv",True, ".../output/test_texts.txt")
with open(".../output/text.txt", 'rb') as f:
    train_data = pickle.load(f)
with open(".../output/test_texts.txt", 'rb') as f:
    test_data = pickle.load(f)
```

这一步载入数据是比较耗时间的，因为它要进行分词，所以可以持久化。在这里展示的是使用序列化的文件。

```bash
ind, train_texts = train_data['ind'], train_data['texts']
ind -= 1
_, test_texts = test_data['ind'], test_data['texts']
```

这一步把类别标签和文本分开以容易处理文本。ind之所以减去1，是因为给定的数据标签是从1开始的，然而训练及预测的时候是从0开始的，所以为了对应应该减去1，但最后输出的时候还要变成原来从1开始的标签。

接下来是把文本转换为字典中对应的索引：

```
# textlist_train = convert_text(train_texts, row_key_word=7, limits=10000,
#                         ispickle=True, pickle_out_path=".../output/textlist_train.txt")
# textlist_test = convert_text(test_texts, row_key_word=7, limits=10000,
#                         ispickle=True, pickle_out_path=".../output/textlist_test.txt")
# print(textlist_train[:2])
with open(".../output/textlist_train.txt", 'rb') as f:
    textlist_train = pickle.load(f)
with open(".../output/textlist_test.txt", 'rb') as f:
    textlist_test = pickle.load(f)
```

同样是使用的序列化后的文件，当然也可以自己改变字典的大小和每行抽取的关键字数。

最后就是使用RNN进行训练，预测testset的类别，并写入csv文件

```
rnn = RNNTextClassifier(10004, 11)
t1 = time.time()
log = rnn.fit(textlist_train,ind)
t2 = time.time() - t1
print(t2)
result = rnn.predict(textlist_test)
result = result + 1 #to be the labels needed
t3 = time.time() - t2
print(t3)
print("training time: %f, testing time: %f" % (t2, t3))
print(result)
result = pd.DataFrame(list(result))
result.to_csv(".../output/result.csv", sep=",")
```

至此，一个完整的处理文本分类数据的过程就结束了。不过我没有进行参数的调整，因此该模型还可以进行优化。




