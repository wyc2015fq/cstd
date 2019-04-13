
# Doc2Vec实践 - Magician的博客 - CSDN博客


2018年05月30日 11:35:46[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：2625


[目录:](#目录)[前言：](#前言)
[第一步：首先我们需要拿到对应的数据，相关的代码如下：](#第一步首先我们需要拿到对应的数据相关的代码如下)
[第二步：拿到对应的数据后，就开始训练数据生成对应的model，对应的代码如下：](#第二步拿到对应的数据后就开始训练数据生成对应的model对应的代码如下)
[第三步：得到生成的model后，我们就可以输入相应的问题得到相似的问题。代码如下：](#第三步得到生成的model后我们就可以输入相应的问题得到相似的问题代码如下)
[第四步：将拿到的相似问题表示出来就OK了。](#第四步将拿到的相似问题表示出来就ok了)
[总结：](#总结)


# 目录:
## 前言：
最近由于要开发一款项目，就想用doc2vec来实现其中的推荐功能，根据用户输入的问题利用doc2vec返回相似的问题。
以下是整个Demo的实现过程，具体的详细代码请参考我的Git：[https://github.com/645187919/doc2vecDemo](https://github.com/645187919/doc2vecDemo)
对于doc2vec来说其实内部原理也是先拿到尽可能多的数据生成一个model然后根据输入再在model中进行匹配，得到对应的输入。
## 第一步：首先我们需要拿到对应的数据，相关的代码如下：
```python
def
```
```python
get_datasest
```
```python
()
```
```python
:
```
```python
fin = open(
```
```python
"questions.txt"
```
```python
,encoding=
```
```python
'utf8'
```
```python
).read().strip(
```
```python
' '
```
```python
)
```
```python
#strip()取出首位空格
```
```python
# print(fin)
```
```python
# print(type(fin))
```
```python
# 添加自定义的词库用于分割或重组模块不能处理的词组。
```
```python
jieba.load_userdict(
```
```python
"userdict.txt"
```
```python
)
```
```python
# 添加自定义的停用词库，去除句子中的停用词。
```
```python
stopwords = set(open(
```
```python
'stopwords.txt'
```
```python
,encoding=
```
```python
'utf8'
```
```python
).read().strip(
```
```python
'\n'
```
```python
).split(
```
```python
'\n'
```
```python
))
```
```python
#读入停用词
```
```python
text =
```
```python
' '
```
```python
.join([x
```
```python
for
```
```python
x
```
```python
in
```
```python
jieba.lcut(fin)
```
```python
if
```
```python
x
```
```python
not
```
```python
in
```
```python
stopwords])
```
```python
#去掉停用词中的词
```
```python
# print(text)
```
```python
print
```
```python
(type(text),len(text))
    x_train = []
    word_list = text.split(
```
```python
'\n'
```
```python
)
    print(word_list[
```
```python
0
```
```python
])
```
```python
for
```
```python
i,sub_list
```
```python
in
```
```python
enumerate(word_list):
        document = TaggededDocument(sub_list, tags=[i])
```
```python
# document是一个Tupple,形式为：TaggedDocument( 杨千嬅 现在 教育 变成 一种 生意 , [42732])
```
```python
# print(document)
```
```python
x_train.append(document)
```
```python
return
```
```python
x_train
```
## 第二步：拿到对应的数据后，就开始训练数据生成对应的model，对应的代码如下：
```python
def
```
```python
train
```
```python
(x_train, size=
```
```python
200
```
```python
, epoch_num=
```
```python
1
```
```python
)
```
```python
:
```
```python
# D2V参数解释：
```
```python
# min_count：忽略所有单词中单词频率小于这个值的单词。
```
```python
# window：窗口的尺寸。（句子中当前和预测单词之间的最大距离）
```
```python
# size:特征向量的维度
```
```python
# sample：高频词汇的随机降采样的配置阈值，默认为1e-3，范围是(0,1e-5)。
```
```python
# negative: 如果>0,则会采用negativesampling，用于设置多少个noise words（一般是5-20）。默认值是5。
```
```python
# workers：用于控制训练的并行数。
```
```python
model_dm = Doc2Vec(x_train,min_count=
```
```python
1
```
```python
, window =
```
```python
5
```
```python
, size = size, sample=
```
```python
1e-3
```
```python
, negative=
```
```python
5
```
```python
, workers=
```
```python
4
```
```python
,hs=
```
```python
1
```
```python
,iter=
```
```python
6
```
```python
)
```
```python
# total_examples：统计句子数
```
```python
# epochs：在语料库上的迭代次数(epochs)。
```
```python
model_dm.train(x_train, total_examples=model_dm.corpus_count, epochs=
```
```python
70
```
```python
)
    model_dm.save(
```
```python
'model_test'
```
```python
)
```
```python
return
```
```python
model_dm
```
## 第三步：得到生成的model后，我们就可以输入相应的问题得到相似的问题。代码如下：
```python
def
```
```python
test
```
```python
()
```
```python
:
```
```python
model_dm = Doc2Vec.load(
```
```python
"model_test"
```
```python
)
    test_ =
```
```python
'申请贷款需要什么条件？'
```
```python
#读入停用词
```
```python
stopwords = set(open(
```
```python
'stopwords.txt'
```
```python
,encoding=
```
```python
'utf8'
```
```python
).read().strip(
```
```python
'\n'
```
```python
).split(
```
```python
'\n'
```
```python
))
```
```python
#去掉停用词中的词
```
```python
test_text =
```
```python
' '
```
```python
.join([x
```
```python
for
```
```python
x
```
```python
in
```
```python
jieba.lcut(test_)
```
```python
if
```
```python
x
```
```python
not
```
```python
in
```
```python
stopwords])
    print(test_text)
```
```python
#获得对应的输入句子的向量
```
```python
inferred_vector_dm = model_dm.infer_vector(doc_words=test_text)
```
```python
# print(inferred_vector_dm)
```
```python
#返回相似的句子
```
```python
sims = model_dm.docvecs.most_similar([inferred_vector_dm], topn=
```
```python
10
```
```python
)
```
```python
return
```
```python
sims
```
## 第四步：将拿到的相似问题表示出来就OK了。
```python
if
```
```python
__name__ ==
```
```python
'__main__'
```
```python
:
    x_train = get_datasest()
```
```python
# print(x_train)
```
```python
# model_dm = train(x_train)
```
```python
sims = test()
```
```python
# sims:[(89, 0.730167031288147), (6919, 0.6993225812911987), (6856, 0.6860911250114441), (40892, 0.6508388519287109), (40977, 0.6465731859207153), (30707, 0.6388640403747559), (40160, 0.6366203427314758), (11672, 0.6353889107704163), (16752, 0.6346361637115479), (40937, 0.6337493062019348)]
```
```python
# sim是一个Tuple,内部包含两个元素，一个是对应的句子的索引号（之前自定义的tag）一个是对应的相似度
```
```python
# print(type(sims))
```
```python
# print('sims:'+str(sims))
```
```python
for
```
```python
count
```
```python
, sim in sims:
        sentence =
```
```python
str
```
```python
(x_train[
```
```python
count
```
```python
])
```
```python
# sentence = x_train[count]
```
```python
# print('sentence:'+sentence)
```
```python
# print('sim:'+str(sim))
```
```python
print(sentence, sim, len(sentence)
              )
```
**当然中间你也可以拿到对应的句子的向量如下：**
```python
def
```
```python
getVecs
```
```python
(model, corpus, size)
```
```python
:
```
```python
vecs = [np.array(model.docvecs[z.tags[
```
```python
0
```
```python
]].reshape(
```
```python
1
```
```python
, size))
```
```python
for
```
```python
z
```
```python
in
```
```python
corpus]
```
```python
return
```
```python
np.concatenate(vecs)
```
## 总结：
虽然在程序中自定义了停词库和词库但是整体的效果依旧不尽人意，甚至在刚开始未调参阶段碰到对于同一个输入运行多次得到不同结果的尴尬情况……虽然这个问题在后来通过调参解决了，但是发现发现这里面仍有许多问题：如输入问题A，model中也包含问题A，但是返回的相似问题中，A的相关度有些却不是最高的等等。后来查了一些资料发现其他的一些网友做这个实验的时候也是效果不理想（至于出现这些问题的原因目前不是特别清楚，按照Doc2Vec的理论来说效果应该不会很差的，可实践后却啪啪啪打脸。。。）。所以暂时得到的结论就是：doc2vec效果时好时坏，偶然性大，不稳定。目前有找到另一种方法来满足我的需求，同样采用的是句子向量，同样是用余弦定理来求相似句子，理论比doc2vec简单，效果也比doc2vec好。等整理好了，会在下一篇文章中做介绍。

