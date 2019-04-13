
# 命名实体：基于CRF的中文命名实体识别模型实现 - lpty的博客 - CSDN博客

2018年01月22日 17:09:15[lpty](https://me.csdn.net/sinat_33741547)阅读数：17423所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、前言
[1、中文命名实体识别](http://blog.csdn.net/sinat_33741547/article/details/79126446)
[2、条件随机场](https://www.jianshu.com/p/55755fc649b1)
## 二、实战
一般通用场景下，最常提取的是时间、人物、地点及组织机构名，因此本模型也将提取以上四种实体。
### 1、数据源
本模型数据使用人民日报1998年标注数据，并进行预处理。语料库词性标记中，对应的实体词依次为t，nr，ns，nt。对语料需要做以下处理：
（1）将语料全角字符统一转为半角
`def q_to_b(q_str):
    """全角转半角"""
    b_str = ""
    for uchar in q_str:
        inside_code = ord(uchar)
        if inside_code == 12288:  
            inside_code = 32
        elif 65374 >= inside_code >= 65281:  
            inside_code -= 65248
        b_str += unichr(inside_code)
    return b_str`（2）合并语料库分开标注的姓和名
（3）合并语料库中括号中的大粒度分词
（4）合并语料库分开标注的时间词
### 2、初始化语料序列
对语料中的句子，词性，实体分类标记进行区分。标签采用”BIO”体系，即实体的第一个字为B_*，其余字为I_*，非实体字统一标记为O。
大部分情况下，标签体系越复杂，准确度也越高，但这里采用简单的BIO体系也能达到相当不错的效果。
`@classmethod
  def init_sequence(cls, words_list):
      """
      初始化字序列、词性序列、标记序列 
      """
      words_seq = [[word.split(u'/')[0] for word in words] for words in words_list]
      pos_seq = [[word.split(u'/')[1] for word in words] for words in words_list]
      tag_seq = [[cls.pos_to_tag(p) for p in pos] for pos in pos_seq]
      cls.pos_seq = [[[pos_seq[index][i] for _ in range(len(words_seq[index][i]))]
                      for i in range(len(pos_seq[index]))] for index in range(len(pos_seq))]
      cls.tag_seq = [[[cls.tag_perform(tag_seq[index][i], w) for w in range(len(words_seq[index][i]))]
                      for i in range(len(tag_seq[index]))] for index in range(len(tag_seq))]
      cls.pos_seq = [[u'un']+[cls.pos_perform(p) for pos in pos_seq for p in pos]+[u'un'] for pos_seq in cls.pos_seq]
      cls.tag_seq = [[t for tag in tag_seq for t in tag] for tag_seq in cls.tag_seq]
      cls.word_seq = [[u'<BOS>']+[w for word in word_seq for w in word]+[u'<EOS>'] for word_seq in words_seq]`这里模型采用tri-gram形式，所以在字符列中，要在句子前后加上占位符。
### 3、特征模板选择
模型采用基于字的特征模板，包括当前字，前一个字，后一个字，及前后字的组合等，同时也可以加上表示词性的特征，效果上不加词性特征也可以接受，所以后边模型训练中没有加上。
`feature = {u'w-1': word_gram[0], u'w': word_gram[1], u'w+1': word_gram[2],
              u'w-1:w': word_gram[0]+word_gram[1], u'w:w+1': word_gram[1]+word_gram[2],
              # u'p-1': cls.pos_seq[index][i], u'p': cls.pos_seq[index][i+1],
              # u'p+1': cls.pos_seq[index][i+2],
              # u'p-1:p': cls.pos_seq[index][i]+cls.pos_seq[index][i+1],
              # u'p:p+1': cls.pos_seq[index][i+1]+cls.pos_seq[index][i+2],
              u'bias': 1.0}`
### 4、模型初始化
模型使用sklearn_crfsuite，文档可以[查看](https://sklearn-crfsuite.readthedocs.io/en/latest/)
`def initialize_model(self):
    """
    初始化
    """
    algorithm = self.config.get('model', 'algorithm')
    c1 = float(self.config.get('model', 'c1'))
    c2 = float(self.config.get('model', 'c2'))
    max_iterations = int(self.config.get('model', 'max_iterations'))
    self.model = sklearn_crfsuite.CRF(algorithm=algorithm, c1=c1, c2=c2,
                                      max_iterations=max_iterations, all_possible_transitions=True)`
### 5、训练
训练这里直接把语料都加载进行训练，但如果是大规模语料的话，这样是很难实现的，那就可以考虑用随机梯度下降或者小批量梯度下降。
`def train(self):
    """
    训练
    """
    self.initialize_model()
    x, y = self.corpus.generator()
    x_train, y_train = x[500:], y[500:]
    x_test, y_test = x[:500], y[:500]
    self.model.fit(x_train, y_train)
    labels = list(self.model.classes_)
    labels.remove('O')
    y_predict = self.model.predict(x_test)
    metrics.flat_f1_score(y_test, y_predict, average='weighted', labels=labels)
    sorted_labels = sorted(labels, key=lambda name: (name[1:], name[0]))
    print(metrics.flat_classification_report(y_test, y_predict, labels=sorted_labels, digits=3))
    self.save_model()`
### 6、效果
`precision    recall  f1-score   support
  B_LOC      0.944     0.827     0.882       266
  I_LOC      0.892     0.801     0.844      1203
  B_ORG      0.941     0.913     0.927       682
  I_ORG      0.932     0.869     0.899       997
  B_PER      0.985     0.918     0.951       440
  I_PER      0.983     0.939     0.961       824
    B_T      0.993     0.993     0.993       444
    I_T      0.995     0.995     0.995      1099
avg / total      0.953     0.904     0.928      5955`可以看到，只是单纯使用字序列的特征，基本的准确率还是可以接受的。
实际例子如下：
`from ner.api import recognize
sentence = u'新华社北京十二月三十一日电(中央人民广播电台记者刘振英、新华社记者张宿堂)今天是一九九七年的最后一天。' \
           u'辞旧迎新之际,国务院总理李鹏今天上午来到北京石景山发电总厂考察,向广大企业职工表示节日的祝贺,' \
           u'向将要在节日期间坚守工作岗位的同志们表示慰问'
predict = recognize(sentence)`命名实体提取结果：
`新华社北京十二月三十一日电(中央人民广播电台记者刘振英、新华社记者张宿堂)今天是一九九七年的最后一天。辞旧迎新之际,国务院总理李鹏今天上午来到北京石景山发电总厂考察,向广大企业职工表示节日的祝贺,向将要在节日期间坚守工作岗位的同志们表示慰问
新华社 北京 十二月三十一日  中央人民广播电台  刘振英  新华社  张宿堂  今天  一九九七年  国务院  李鹏 今天上午  北京石景山发电总厂`
### 7、其他
具体代码可以在我github上找到：[https://github.com/lpty/nlp_base](https://github.com/lpty/nlp_base)
## 三、参考
1、《统计自然语言处理》   宗成庆
2、基于规则和条件随机场的中文命名实体识别方法研究 程志刚
3、[https://www.jianshu.com/p/55755fc649b1](https://www.jianshu.com/p/55755fc649b1)

