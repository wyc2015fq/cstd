# 三十五、一个lstm单元让聊天机器人学会甄嬛体 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 14:00:40[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2392












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_450_default_big.jpeg)



当今比较流行的seq2seq基本都是用lstm组成的编码器解码器模型实现，开源的实现大都基于one-hot的embedding(没有词向量表达的信息量大)，因此打算自己实现一个基于word2vec词向量的强大的seq2seq模型，首先实现了只有一个lstm单元的机器人

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 下载《甄嬛传》小说原文

上网随便百度一个“甄嬛传 txt”，下载下来，首先要把文件转码成utf-8编码，然后要把windows的回车符都替换成\n，以便后续处理，最终效果如下：

```
[root@centos $] head zhenhuanzhuan.txt
序文--不过是「情」

    在键盘上敲落一个个文字的时候，窗外有大雨过后的清新。站在十二楼的落地玻璃窗前往外看，有大片大片开阔的深绿蔓延。

　　我喜欢这个有山有水的小城，所以在这样一个烦热的下午，背负着窒闷的心情不顾一切逃出暂居的城市，来到这里，在写完了一个整整写了三年多的故事之后。

　　终于，写完了《后宫：甄嬛传》的最后一本，第七本。七，是我喜欢的一个数字。甄嬛的故事，最后一个字，是我在初夏的某日坐在师大某个小宾馆的房间里写下的。这个故事，自我在母校时始，又于母校终，像一个有始有终的圆圈，终于完结了。

　　这是我的第一部长篇，自己也轻吁一口气，居然写了那么长，那么久。
　　可是完结的那一刻，我心里一点也不快活。因?是我自己，把我喜爱的清，把我理想中温润如玉的男子，写到玉碎斑驳。
```



## 对甄嬛传切词

切词工具word_segment.py请到我的github下载，地址在[https://github.com/warmheartli/ChatBotCourse/blob/master/word_segment.py](https://github.com/warmheartli/ChatBotCourse/blob/master/word_segment.py)

执行
`python ./word_segment.py zhenhuanzhuan.txt zhenhuanzhuan.segment`
生成的文件如下：

```
[root@centos $] head zhenhuanzhuan.segment
  序文 - - 不过 是 「 情 」

 在 键盘 上 敲落 一个个 文字 的 时候 ， 窗外 有 大雨 过后 的 清新 。 站 在 十二楼 的 落地 玻璃窗 前往 外看 ， 有 大片大片 开阔 的 深绿 蔓延 。

 　 　 我 喜欢 这个 有山有水 的 小城 ， 所以 在 这样 一个 烦热 的 下午 ， 背负着 窒闷 的 心情 不顾一切 逃出 暂居 的 城市 ， 来到 这里 ， 在 写 完 了 一个 整整 写 了 三年 多 的 故事 之后 。

 　 　 终于 ， 写 完 了 《 后宫 ： 甄 嬛 传 》 的 最后 一本 ， 第七 本 。 七 ， 是 我 喜欢 的 一个 数字 。 甄 嬛 的 故事 ， 最后 一个 字 ， 是 我 在 初夏 的 某日 坐在 师大 某个 小 宾馆 的 房间 里 写下 的 。 这个 故事 ， 自我 在 母校 时始 ， 又 于 母校 终 ， 像 一个 有始有终 的 圆圈 ， 终于 完结 了 。

 　 　 这 是 我 的 第一部 长篇 ， 自己 也 轻吁 一口气 ， 居然 写 了 那么 长 ， 那么 久 。
 　 　 可是 完结 的 那一刻 ， 我 心里 一点 也 不 快活 。 因 ? 是 我 自己 ， 把 我 喜爱 的 清 ， 把 我 理想 中 温润 如玉 的 男子 ， 写 到 玉碎 斑驳 。
```



## 生成词向量

为了能应用词向量，我们利用word2vec来生成词向量，word2vec的源码可以从网上下载(或者可以在我的github上下载[https://github.com/warmheartli/ChatBotCourse/tree/master/word2vec](https://github.com/warmheartli/ChatBotCourse/tree/master/word2vec))，make编译即可

执行
`./word2vec -train ./zhenhuanzhuan.segment -output vectors.bin -cbow 1 -size 200 -window 8 -negative 25 -hs 0 -sample 1e-4 -threads 20 -binary 1 -iter 15`
这样会生成一个vectors.bin文件，这就是我们想要的基于甄嬛传原文生成的词向量文件

如果想知道vectors.bin文件里如何存储以及如何加载，可以参考我的另一篇文章《[三十二-用三千万影视剧字幕语料库生成词向量](http://www.shareditor.com/blogshow/?blogId=115)》



## 训练代码

下面是我的代码原文，仅供参考

```python
# -*- coding: utf-8 -*-

import sys
import math
import tflearn
import chardet
import numpy as np
import struct

seq = []

max_w = 50
float_size = 4
word_vector_dict = {}

def load_vectors(input):
    """从vectors.bin加载词向量，返回一个word_vector_dict的词典，key是词，value是200维的向量
    """
    print "begin load vectors"

    input_file = open(input, "rb")

    # 获取词表数目及向量维度
    words_and_size = input_file.readline()
    words_and_size = words_and_size.strip()
    words = long(words_and_size.split(' ')[0])
    size = long(words_and_size.split(' ')[1])
    print "words =", words
    print "size =", size

    for b in range(0, words):
        a = 0
        word = ''
        # 读取一个词
        while True:
            c = input_file.read(1)
            word = word + c
            if False == c or c == ' ':
                break
            if a < max_w and c != '\n':
                a = a + 1
        word = word.strip()

        vector = []
        for index in range(0, size):
            m = input_file.read(float_size)
            (weight,) = struct.unpack('f', m)
            vector.append(weight)

        # 将词及其对应的向量存到dict中
        word_vector_dict[word.decode('utf-8')] = vector

    input_file.close()
    print "load vectors finish"

def init_seq():
    """读取切好词的文本文件，加载全部词序列
    """
    file_object = open('zhenhuanzhuan.segment', 'r')
    vocab_dict = {}
    while True:
        line = file_object.readline()
        if line:
            for word in line.decode('utf-8').split(' '):
                if word_vector_dict.has_key(word):
                    seq.append(word_vector_dict[word])
        else:
            break
    file_object.close()

def vector_sqrtlen(vector):
    len = 0
    for item in vector:
        len += item * item
    len = math.sqrt(len)
    return len

def vector_cosine(v1, v2):
    if len(v1) != len(v2):
        sys.exit(1)
    sqrtlen1 = vector_sqrtlen(v1)
    sqrtlen2 = vector_sqrtlen(v2)
    value = 0
    for item1, item2 in zip(v1, v2):
        value += item1 * item2
    return value / (sqrtlen1*sqrtlen2)


def vector2word(vector):
    max_cos = -10000
    match_word = ''
    for word in word_vector_dict:
        v = word_vector_dict[word]
        cosine = vector_cosine(vector, v)
        if cosine > max_cos:
            max_cos = cosine
            match_word = word
    return (match_word, max_cos)

def main():
    load_vectors("./vectors.bin")
    init_seq()
    xlist = []
    ylist = []
    test_X = None
    #for i in range(len(seq)-100):
    for i in range(10):
        sequence = seq[i:i+20]
        xlist.append(sequence)
        ylist.append(seq[i+20])
        if test_X is None:
            test_X = np.array(sequence)
            (match_word, max_cos) = vector2word(seq[i+20])
            print "right answer=", match_word, max_cos

    X = np.array(xlist)
    Y = np.array(ylist)
    net = tflearn.input_data([None, 20, 200])
    net = tflearn.lstm(net, 200)
    net = tflearn.fully_connected(net, 200, activation='linear')
    net = tflearn.regression(net, optimizer='sgd', learning_rate=0.1,
                                     loss='mean_square')
    model = tflearn.DNN(net)
    model.fit(X, Y, n_epoch=500, batch_size=10,snapshot_epoch=False,show_metric=True)
    model.save("model")
    predict = model.predict([test_X])
    #print predict
    #for v in test_X:
    #    print vector2word(v)
    (match_word, max_cos) = vector2word(predict[0])
    print "predict=", match_word, max_cos

main()
```



解释一下上面的代码，load_vectors是从vectors.bin中加载词向量，init_seq是加载甄嬛传切词后的文本并存到一个序列里，vector2word是求距离某向量最近的词，模型中只有一个lstm单元

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

执行效果如下：

```
[root@centos $] python one_lstm_sequence_generate.py
begin load vectors
words = 16995
size = 200
load vectors finish
right answer= 站 1.0
---------------------------------
Run id: DQK34Q
Log directory: /tmp/tflearn_logs/
---------------------------------
Training samples: 10
Validation samples: 
--
Training Step: 500  | total loss: 0.33673
| SGD | epoch: 500 | loss: 0.33673 - acc: 0.1748 -- iter: 10/10
--
predict= 站 0.941794432002
```

可以看出：经过500个epoch的训练，均方损失降到0.33673，并能以0.941794432002的余弦相似度预测出下一个字

如果你有强大的gpu，可以调整上述参数，把整篇文章都训练进去，稍稍修改代码中predict的部分，让他不断的输出下一个字，就可以自动吐出甄嬛体

这段代码是基于tflearn实现的，在tflearn官方文档的examples中实现的seq2seq是直接调用了tensorflow中的tensorflow/python/ops/seq2seq.py，而这部分是基于one-hot的embedding方法，这是一定没有词向量效果好的，因此下一步打算基于上面的代码继续改造，实现基于词向量的seq2seq，相信能够让我的聊天机器人问答效果更好，敬请期待




