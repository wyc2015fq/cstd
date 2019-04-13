
# 分词：基于HMM的中文分词模型实现 - lpty的博客 - CSDN博客

2017年12月22日 10:33:36[lpty](https://me.csdn.net/sinat_33741547)阅读数：2346所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、前言
本文主要是实现了一个纯粹的HMM中文分词模型，关于中文分词可以参考：[中文分词](http://blog.csdn.net/sinat_33741547/article/details/78690440)。分词的基本思想与该文基本一致，请确保已经了解分词的基本知识。
## 二、实战
### 1、语料源
语料来源于[Bakeoff 2005](http://sighan.cs.uchicago.edu/bakeoff2005/)的主页，这里选用了icwb2-data.rar语料，大概介绍如下：
*  /icwb2-data.rar/training/msr_training.txt    用以训练HMM，其中包含已分词汇约2000000个
*  /icwb2-data.rar/testing/msr_test.txt    测试集
* /icwb2-data.rar/gold/msr_gold.txt    测试集已分词文本
### 2、语料处理

```python
def read_corpus_from_file(cls, file_path):
        """
        读取语料
        """
        f = open(file_path, 'r')
        lines = f.readlines()
        for line in lines:
            cls._words.extend([word for word in line.decode('gbk').strip().split(' ') if word and not cls.is_puns(word)])
        f.close()
```
上面为了简单处理，将文本中的所有标点符号都去除了，如果不去除对于模型本身其实也没有什么影响。

```python
def gen_vocabs(cls):
        """
        生成词典
        """
        cls._vocab = list(set(cls._words))+[u'<UNK>']
```
在词典后面加上UNK标记，作为未知字的占位符。
### 3、生成隐藏状态序列

```python
def word_to_states(cls, word):
        """
        词对应状态转换 
        """
        word_len = len(word)
        if word_len == 1:
            cls._states.append('S')
        else:
            state = ['M'] * word_len
            state[0] = 'B'
            state[-1] = 'E'
            cls._states.append(''.join(state))
```
按照上文链接提到的中文分词思想，将语料文字序列转化为对应{‘S’， ‘M’， ‘B’， ‘E’}的隐藏状态序列
### 4、初始化三类概率

```python
def cal_state(cls):
        """
        计算三类状态概率 
        """
        for word in cls._words:
            cls.word_to_states(word)
        init_state = cls.cal_init_state()
        trans_state = cls.cal_trans_state()
        emit_state = cls.cal_emit_state()
        cls.save_state(init_state, trans_state, emit_state)
```
统计各类隐藏状态序列频率，分别计算出初始概率，转移概率，发射概率。
### 5、初始化模型

```python
def get_model(self):
        """
        初始化hmm模型
        """
        model = MultinomialHMM(n_components=len(self.states))
        model.startprob_ = self.init_p
        model.transmat_ = self.trans_p
        model.emissionprob_ = self.emit_p
        return model
```
这里采用了hmmlearn封装好的HMM模型，将我们上个步骤生成的概率传递到模型中
### 6、模型解码

```python
def cut(self, sentence):
        """
        分词
        """
        seen_n = np.array([[self.pre_process(w) for w in sentence]]).T
        log_p, b = self.model.decode(seen_n, algorithm='viterbi')
        states = map(lambda x: self.states[x], b)
        cut_sentence = ''
        for index in range(len(states)):
            if states[index] in ('S', 'E'):
                cut_sentence += sentence[index]+' '
            else:
                cut_sentence += sentence[index]
        return cut_sentence
```
利用上述模型就可以简单的进行分词了，模型进行最优隐藏序列计算时，使用了viterbi算法，具体可以参考：[维特比算法](http://blog.csdn.net/sinat_33741547/article/details/78867039)
### 7、效果

```python
准确率：0.7416711648494009
召回率：0.6686293783881109
F1：0.7032587944456672
```
效果并不是特别好，毕竟只是单纯的HMM模型，如果后续提升效果可以加上字典、规则，或者使用更丰富的语料等。
## 三、其他
具体代码可以在我的github上找到：[https://github.com/lpty/nlp_base](https://github.com/lpty/nlp_base)




