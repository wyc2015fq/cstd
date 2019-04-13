
# 词性标注：基于MaxEnt的中文词性标注模型实现 - lpty的博客 - CSDN博客

2018年01月02日 10:51:59[lpty](https://me.csdn.net/sinat_33741547)阅读数：1216所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、前言
1、[中文词性标注](http://blog.csdn.net/sinat_33741547/article/details/78894163)
2、[最大熵模型](http://blog.csdn.net/sinat_33741547/article/details/78905635)
## 二、数据源
本文使用数据源未1998年的人民日报标注语料，手工进行处理，去除一些不符合标注规范（word/pos）的词语，词性标注见中文词性标注一文。
## 三、实战
### 1、特征提取
最大熵模型与一般机器学习算法不同之处在于特征是对x，y的联合分布提取，而不是只对x进行特征提取。
这是简单的设定几个模板：
`前一个词与前词词性
当前词与当前词词性
后一个词与当前词词性
前一个词词性与当前词词性
当前词部首与当前词词性`这里模板数量还是偏少，还可以再增加关于字的模板或者扩大window范围等，详情可以参考后文贴出的论文。
基于上述的模板，与语料进行匹配，得到输入的特征。
`cls.cal_pre_word_to_pos_feature()
    cls.cal_word_to_pos_feature()
    cls.cal_las_word_to_pos_feature()
    cls.cal_pre_pos_to_pos_feature()
    cls.cal_radical_to_pos_feature()`特征取值这里使用了每一个特征在语料中出现的概率，实验表明，这样的特征取值比单纯的二值方式效果要更好。
上述提到的特征并不能全部作为输入给到模型训练，需要进行筛选，这里简单的用特征出现的频率阈值作为筛选条件，一般选用5~20都可以。
`@classmethod
def process_feature(cls, counter, counts, feature_dict_path, feature_path):
    """
    特征筛选、特征值计算及保存 
    """
    limit = int(cls._config.get('postagger', 'feature_limit'))
    feature_dict = {k: {kk: vv/counter[k] for kk, vv in v.items() if vv > limit} for k, v in counts.items()}
    feature_dict = {k: v for k, v in feature_dict.items() if v}
    feature = cls.cal_feature_list(feature_dict)
    cls._feature.save_feature_to_file(feature_dict, feature_dict_path)
    cls._feature.save_feature_to_file(feature, feature_path)`提取出来的特征数量在十万左右，看起来很多，但实际上还是不够的。
### 2、语料处理
前面确定了输入的特征，接下来就是把原始语料转为特征的形式。这里采用tri-gram方式，将语料进行切分。
`@classmethod
def segment_by_window(cls, words_list=None, window=3):
    """
    窗口切分
    """
    words, pos = [], []
    pre_words = cls._words if not words_list else words_list
    begin, end = 0, window
    for _ in range(1, len(pre_words)):
        if end > len(pre_words): break
        words.append(pre_words[begin:end])
        if not words_list: pos.append(cls._pos[begin:end])
        begin = begin + 1 if words[-1] != u'<EOS>' else begin + window
        end = end + 1 if words[-1] != u'<EOS>' else end + window
    return words, pos`将切分后的语料按模板形式组合，与提取筛选后的特征进行比较，确定输入x的取值形式，构造生成器。
`@classmethod
def feature_generator(cls):
    """
    语料特征生成器 
    """
    words, pos = cls.segment_by_window()
    batch_size = int(cls._config.get('model', 'batch_size'))
    chunk_size = len(words)/batch_size
    generator_list = [(i*batch_size) - 1 for i in range(chunk_size) if i]
    features = cls.load_feature()
    batch_x, batch_y = [], []
    for index in range(len(words)):
        x = cls.get_batch_x(words, pos, index, features)
        batch_x.append(x)
        batch_y.append(cls._states.index(pos[index][1]))
        if index in generator_list:
            print('{} Feature_size{}, Batch_size:{}, Chunk_count:{}, Now_chunk:{}'.format(
                datetime.now().strftime('%c'), len(x), batch_size, chunk_size, (index+1)/batch_size))
            yield np.array(batch_x), np.array(batch_y)
            batch_x, batch_y = [], []`
### 3、模型搭建
最大熵模型实际上可以看成逻辑回归的一般化形式，或者是多项逻辑回归。前面将语料处理完成后，以一个batch的大小为256计算，大概存在百万级别的训练数据，这样的只能进行增量训练。
使用的sklearn的SGDClassifier就可以了，需要把损失函数设定为log，这样才是逻辑回归，如下：
`loss : str, 'hinge', 'log', 'modified_huber', 'squared_hinge',\
            'perceptron', or a regression loss: 'squared_loss', 'huber',\
            'epsilon_insensitive', or 'squared_epsilon_insensitive'
    The loss function to be used. Defaults to 'hinge', which gives a
    linear SVM.
    The 'log' loss gives logistic regression, a probabilistic classifier.
    'modified_huber' is another smooth loss that brings tolerance to
    outliers as well as probability estimates.
    'squared_hinge' is like hinge but is quadratically penalized.
    'perceptron' is the linear loss used by the perceptron algorithm.
    The other losses are designed for regression but can be useful in
    classification as well; see SGDRegressor for a description.`模型训练过程中准确率验证码方面，不能使用简单的交叉验证，这里使用了两个准确率对比，简单解释一下，estimator每次只会拿下一个待训练batch在本次做评估，然后训练完之后，再在这个batch上做一次评估，看看是否有优化。
`def train(self, epoch=None, show=None):
    """
    模型训练
    """
    self.initialize_model()
    g = self.corpus.feature_generator()
    batch_first_x, batch_first_y = next(g)
    self.model.partial_fit(batch_first_x, batch_first_y, classes=range(self.classes))
    train_score, progressive_validation_score = [], []
    epoch_count = 0
    for batch_x, batch_y in g:
        score = self.model.score(batch_x, batch_y)
        progressive_validation_score.append(score)
        print('{} Progressive_validation_score:{}'.format(datetime.now().strftime('%c'), score))
        self.model.partial_fit(batch_x, batch_y, classes=range(self.classes))
        score = self.model.score(batch_x, batch_y)
        train_score.append(score)
        print('{} Train_score:{}'.format(datetime.now().strftime('%c'), score))
        if epoch and epoch_count == epoch:break
        else: epoch_count += 1
    self.save_model()`
### 4、模型效果
参数：
`[model]
batch_size = 256
loss = log
alpha = 0.001
n_jobs = -1`准确率：
`01/02/18 04:25:28 Feature_size63033, Batch_size:256, Chunk_count:9121, Now_chunk:9120
01/02/18 04:25:29 Progressive_validation_score:0.6015625
01/02/18 04:25:31 Train_score:0.6015625`可以看到，到训练完毕时，两个对比准确率趋于稳定，但是都不高，可以判断这里是欠拟合，和上面特征提取时的判断一致，特征数量还是相对较少，需要增加特征模板，将特征数量提升到百万级别。
### 5、实例
`from postagger.api import *
predict = tagger(u'德国  大众  公司  是  欧洲  最  大  的  汽车  制造商  。')
origin = u'德国/ns  大众/nz  公司/n  是/v  欧洲/ns  最/d  大/a  的/u  汽车/n  制造商/n  。/w'
print u'Origin', origin
print u'Predict', predict`输出如下：
`Origin 德国/ns  大众/nz  公司/n  是/v  欧洲/ns  最/d  大/a  的/u  汽车/n  制造商/n  。/w
Predict 德国/n  大众/n  公司/n  是/v  欧洲/n  最/d  大/a  的/u  汽车/n  制造商/v  。/w`
## 三、参考
`1、汉语词性标注的特征工程  于江德等
2、一种基于改进的最大熵模型的汉语词性自动标注的新方法  赵伟等
3、基于最大熵的哈萨克语词性标注模型  桑海岩等`

