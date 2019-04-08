# python实现文本分类

2018年05月23日 11:13:38 [小白的进阶](https://me.csdn.net/laobai1015) 阅读数：5886



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/laobai1015/article/details/80415080

## 一、中文文本分类流程：

\1. 预处理

\2. 中文分词

\3. 结构化表示-构建词向量空间

4.权重策略-TF-IDF

\5. 分类器

\6. 评价

## 二、具体细节

### 1.预处理

1.1. 得到训练集语料库

​    本文采用复旦中文文本分类语料库，下载链接：https://download.csdn.net/download/laobai1015/10431543

1.2 得到测试集语料库

​    同样采用复旦中文文本分类语料库，下载链接：https://download.csdn.net/download/laobai1015/10431564



### 2.中文分词



第1小节预处理中的语料库都是没有分词的原始语料（即连续的句子，而后面的工作需要我们把文本分为一个个单词），现在需要对这些文本进行分词，只有这样才能在基于单词的基础上，对文档进行结构化表示。
中文分词有其特有的难点，最终完全解决中文分词的算法是基于概率图模型的条件随机场（CRF）。中文分词的工具有很多，但是比较著名的几个都是基于java的，这里推荐python的第三方库jieba（所采用的算法就是条件随机场）。
通过pip安装jieba：打开cmd，切换到Python所在目录下，执行命令：pip install jieba


然后通过Python编程，将训练语料库和测试语料库进行分词，分词后保存的路径可以自己设置。

```python
#!/usr/bin/env python  
# -*- coding: UTF-8 -*-  
 
import sys  
import os  
import jieba  
# 配置utf-8输出环境  
reload(sys)  
sys.setdefaultencoding('utf-8')  
# 保存至文件  
def savefile(savepath, content):  
    with open(savepath, "wb") as fp:  
        fp.write(content)  
    ''''' 
    上面两行是python2.6以上版本增加的语法，省略了繁琐的文件close和try操作 
    2.5版本需要from __future__ import with_statement 
    '''  
# 读取文件  
def readfile(path):  
    with open(path, "rb") as fp:  
        content = fp.read()  
    return content  
  
def corpus_segment(corpus_path, seg_path):  
    ''''' 
    corpus_path是未分词语料库路径 
    seg_path是分词后语料库存储路径 
    '''  
    catelist = os.listdir(corpus_path)  # 获取corpus_path下的所有子目录  
    ''''' 
    其中子目录的名字就是类别名，例如： 
    train_corpus/art/21.txt中，'train_corpus/'是corpus_path，'art'是catelist中的一个成员 
    '''  
  
    # 获取每个目录（类别）下所有的文件  
    for mydir in catelist:  
        ''''' 
        这里mydir就是train_corpus/art/21.txt中的art（即catelist中的一个类别） 
        '''  
        class_path = corpus_path + mydir + "/"  # 拼出分类子目录的路径如：train_corpus/art/  
        seg_dir = seg_path + mydir + "/"  # 拼出分词后存贮的对应目录路径如：train_corpus_seg/art/  
  
        if not os.path.exists(seg_dir):  # 是否存在分词目录，如果没有则创建该目录  
            os.makedirs(seg_dir)  
  
        file_list = os.listdir(class_path)  # 获取未分词语料库中某一类别中的所有文本  
        ''''' 
        train_corpus/art/中的 
        21.txt, 
        22.txt, 
        23.txt 
        ... 
        file_list=['21.txt','22.txt',...] 
        '''  
        for file_path in file_list:  # 遍历类别目录下的所有文件  
            fullname = class_path + file_path  # 拼出文件名全路径如：train_corpus/art/21.txt  
            content = readfile(fullname)  # 读取文件内容  
            '''''此时，content里面存贮的是原文本的所有字符，例如多余的空格、空行、回车等等， 
            接下来，我们需要把这些无关痛痒的字符统统去掉，变成只有标点符号做间隔的紧凑的文本内容 
            '''  
            content = content.replace("\r\n", "")  # 删除换行  
            content = content.replace(" ", "")#删除空行、多余的空格  
            content_seg = jieba.cut(content)  # 为文件内容分词  
            savefile(seg_dir + file_path, " ".join(content_seg))  # 将处理后的文件保存到分词后语料目录  
  
    print "中文语料分词结束！！！"  
  
''''' 
if __name__=="__main__": 
简单来说如果其他python文件调用这个文件的函数，或者把这个文件作为模块 
导入到你的工程中时，那么下面的代码将不会被执行，而如果单独在命令行中 
运行这个文件，或者在IDE（如pycharm）中运行这个文件时候，下面的代码才会运行。 
即，这部分代码相当于一个功能测试。 
 
'''  
if __name__=="__main__":
    #对训练集进行分词  
    corpus_path = "D:/work/train/train/"  # 未分词分类语料库路径  
    seg_path = "D:/work/train/train/train_corpus_seg/"  # 分词后分类语料库路径  
    corpus_segment(corpus_path,seg_path)  
  
    #对测试集进行分词  
    corpus_path = "D:/work/test/test/"  # 未分词分类语料库路径  
    seg_path = "D:/work/test/test/test_corpus_seg/"  # 分词后分类语料库路径  
    corpus_segment(corpus_path,seg_path)  
```

### 3. 结构化表示-构建词向量空间

现在我们得到了分词后的训练集语料库和测试集语料库，下面我们要把这两个数据集表示为变量，从而为下面的程序调用提供服务。采用Scikit-Learn库中的Bunch数据结构来表示这两个数据集。

Bunch就相当于python中的字典。你往里面传什么，它就存什么。就相当于python中的字典。你往里面传什么，它就存什么。



我们的数据集（训练集）有哪些信息：

> 1，类别，也就是所有分类类别的集合，即我们./train_corpus_seg/和./test_corpus_seg/下的所有子目录的名字。我们在这里不妨把它叫做target_name（这是一个列表）
>
> 2，文本文件名。例如./train_corpus_seg/art/21.txt，我们可以把所有文件名集合在一起做一个列表，叫做filenames
>
> 3，文本标签（就是文本的类别），不妨叫做label（与2中的filenames一一对应）
>
> > 例如2中的文本“21.txt”在./train_corpus_seg/art/目录下，则它的标签就是art。
>
> > 文本标签与1中的类别区别在于：文本标签集合里面的元素就是1中类别，而文本标签集合的元素是可以重复的，因为./train_corpus_seg/art/目录下有好多文本，不是吗？相应的，1中的类别集合元素显然都是独一无二的类别。
>
> 4，文本内容（contens）。
>
> > 上一步代码我们已经成功的把文本内容进行了分词，并且去除掉了所有的换行，得到的其实就是一行词袋。

那么，用Bunch表示，就是：



```python
from sklearn.datasets import base

bunch = base.Bunch(target_name=[], label=[], filenames=[], contents=[])  
```

我们在Bunch对象里面创建了有4个成员：
target_name：是一个list，存放的是整个数据集的类别集合。
label：是一个list，存放的是所有文本的标签。
filenames：是一个list，存放的是所有文本文件的名字。
contents：是一个list，分词后文本文件（一个文本文件只有一行）



如果你还没有明白，看一下下面这个图，你总该明白了：

Bunch:

![img](https://img-blog.csdn.net/20170206172045853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ2l0aHViXzM2MzI2OTU1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

下面，我们将文本文件转为Bunch类型：

```python
#!/usr/bin/env python  
# -*- coding: UTF-8 -*-  
 
import sys
reload(sys) # Python2.5 初始化后删除了 sys.setdefaultencoding 方法，我们需要重新载入 
sys.setdefaultencoding('utf-8') 
import os#python内置的包，用于进行文件目录操作，我们将会用到os.listdir函数  
import cPickle as pickle#导入cPickle包并且取一个别名pickle  
''''' 
事实上python中还有一个也叫作pickle的包，与这里的名字相同了，无所谓 
本文件代码下面会用到cPickle中的函数cPickle.dump 
'''
from sklearn.datasets import base  #一定是这样写的，下面注释的一行是错误示范
#from sklearn.datasets.base import Bunch  
 
 
  
def _readfile(path):  
    '''''读取文件'''  
    #函数名前面带一个_,是标识私有函数  
    # 仅仅用于标明而已，不起什么作用，  
    # 外面想调用还是可以调用，  
    # 只是增强了程序的可读性  
    with open(path, "rb") as fp:#with as句法前面的代码已经多次介绍过，今后不再注释  
        content = fp.read()  
    return content
  
def corpus2Bunch(wordbag_path,seg_path):
    catelist = os.listdir(seg_path)# 获取seg_path下的所有子目录，也就是分类信息  
    #创建一个Bunch实例  
    bunch = base.Bunch(target_name=[], label=[], filenames=[], contents=[])  
    bunch.target_name.extend(catelist)  
    ''''' 
    extend(addlist)是python list中的函数，意思是用新的list（addlist）去扩充 
    原来的list 
    '''  
    # 获取每个目录下所有的文件  
    for mydir in catelist:  
        class_path = seg_path + mydir + "/"  # 拼出分类子目录的路径  
        file_list = os.listdir(class_path)  # 获取class_path下的所有文件  
        for file_path in file_list:  # 遍历类别目录下文件  
            fullname = class_path + file_path  # 拼出文件名全路径  
            bunch.label.append(mydir)  
            bunch.filenames.append(fullname)  
            bunch.contents.append(_readfile(fullname))  # 读取文件内容  
            '''''append(element)是python list中的函数，意思是向原来的list中添加element，注意与extend()函数的区别'''  
    # 将bunch存储到wordbag_path路径中  
    with open(wordbag_path, "wb") as file_obj:  
        pickle.dump(bunch, file_obj)  
    print "构建文本对象结束！！！"  
  
 
wordbag_path = "D:/work/train/train_word_bag/train_set.dat"  # Bunch存储路径  
seg_path = "D:/work/train/train_corpus_seg/"  # 分词后分类语料库路径  
corpus2Bunch(wordbag_path, seg_path)  
  
    # 对测试集进行Bunch化操作：  
wordbag_path = "D:/work/test/test_word_bag/test_set.dat"  # Bunch存储路径  
seg_path = "D:/work/test/test_corpus_seg/"  # 分词后分类语料库路径  
corpus2Bunch(wordbag_path, seg_path)  
```

接下来我们要做的，就是把所有这些词统一到同一个词向量空间中。

为了节省空间，我们首先将训练集中每个文本中一些垃圾词汇去掉。所谓的垃圾词汇，就是指意义模糊的词，或者一些语气助词，标点符号等等，通常他们对文本起不了分类特征的意义。这些垃圾词汇我们称之为停用词。把所有停用词集合起来构成一张停用词表格，这样，以后我们处理文本时，就可以从这个根据表格，过滤掉文本中的一些垃圾词汇了。

你可以从这里下载停用词表：https://download.csdn.net/download/laobai1015/10431705

存放在这里路径中：train_word_bag/hlt_stop_words.txt



### 4.权重策略-TF-IDF

下面的程序，目的就是要将训练集所有文本文件统一到同一个词向量空间中。

下面的一节主要目标是希望得到两个东西：

1.词典（单词和单词对应的序号）

2.权重矩阵tdm，其中，权重矩阵是一个二维矩阵，tdm[i][j]表示，第j个词（即词典中的序号）在第i个类别中的IF-IDF值（下文有讲解）。

事实上，tdm的每一列都是一个单词在各个类别中的全职。我们把这每一列当作词向量。

```python
#!/usr/bin/env python
# -*- coding: UTF-8 -*-
 
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
 
from sklearn.datasets.base import Bunch
import cPickle as pickle
from sklearn.feature_extraction.text import TfidfVectorizer
 
def _readfile(path):
    with open(path, "rb") as fp:
        content = fp.read()
    return content
 
def _readbunchobj(path):
    with open(path, "rb") as file_obj:
        bunch = pickle.load(file_obj)
    return bunch
 
def _writebunchobj(path, bunchobj):
    with open(path, "wb") as file_obj:
        pickle.dump(bunchobj, file_obj)
 
def vector_space(stopword_path,bunch_path,space_path,train_tfidf_path=None):
 
    stpwrdlst = _readfile(stopword_path).splitlines()
    bunch = _readbunchobj(bunch_path)
    tfidfspace = Bunch(target_name=bunch.target_name, label=bunch.label, filenames=bunch.filenames, tdm=[], vocabulary={})
 
    if train_tfidf_path is not None:
        trainbunch = _readbunchobj(train_tfidf_path)
        tfidfspace.vocabulary = trainbunch.vocabulary
        vectorizer = TfidfVectorizer(stop_words=stpwrdlst, sublinear_tf=True, max_df=0.5,vocabulary=trainbunch.vocabulary)
        tfidfspace.tdm = vectorizer.fit_transform(bunch.contents)
 
    else:
        vectorizer = TfidfVectorizer(stop_words=stpwrdlst, sublinear_tf=True, max_df=0.5)
        tfidfspace.tdm = vectorizer.fit_transform(bunch.contents)
        tfidfspace.vocabulary = vectorizer.vocabulary_
 
    _writebunchobj(space_path, tfidfspace)
    print "if-idf词向量空间实例创建成功！！！" 
  
if __name__ == '__main__':  
  
    stopword_path = "D:/work/train/train_word_bag/hlt_stop_words.txt"  
    bunch_path = "D:/work/train/train_word_bag/train_set.dat"  
    space_path = "D:/work/train/train_word_bag/tfdifspace.dat"  
    vector_space(stopword_path,bunch_path,space_path)  
  
    bunch_path = "D:/work/test/test_word_bag/test_set.dat"  
    space_path = "D:/work/test/test_word_bag/testspace.dat"  
    train_tfidf_path="D:/work/train/train_word_bag/tfdifspace.dat"  
    vector_space(stopword_path,bunch_path,space_path,train_tfidf_path)
```

### 5. 分类器

这里我们采用的是朴素贝叶斯分类器

```python
#!/usr/bin/env python
# -*- coding: UTF-8 -*-
 
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
 
import cPickle as pickle
from sklearn.naive_bayes import MultinomialNB  # 导入多项式贝叶斯算法
 
 
# 读取bunch对象
def _readbunchobj(path):
    with open(path, "rb") as file_obj:
        bunch = pickle.load(file_obj)
    return bunch
 
# 导入训练集
trainpath = "D:/work/train/train_word_bag/tfdifspace.dat"
train_set = _readbunchobj(trainpath)
 
# 导入测试集
testpath = "D:/work/test/test_word_bag/testspace.dat"
test_set = _readbunchobj(testpath)
 
# 训练分类器：输入词袋向量和分类标签，alpha:0.001 alpha越小，迭代次数越多，精度越高
clf = MultinomialNB(alpha=0.001).fit(train_set.tdm, train_set.label)
 
# 预测分类结果
predicted = clf.predict(test_set.tdm)
 
for flabel,file_name,expct_cate in zip(test_set.label,test_set.filenames,predicted):
    if flabel != expct_cate:
        print file_name,": 实际类别:",flabel," -->预测类别:",expct_cate
 
print "预测完毕!!!"
 
# 计算分类精度：
from sklearn import metrics
def metrics_result(actual, predict):
    print '精度:{0:.3f}'.format(metrics.precision_score(actual, predict,average='weighted'))
    print '召回:{0:0.3f}'.format(metrics.recall_score(actual, predict,average='weighted'))
    print 'f1-score:{0:.3f}'.format(metrics.f1_score(actual, predict,average='weighted'))
 
metrics_result(test_set.label, predicted)
```

### 6.评价与小结

在实际操作过程中可以通过调节TF-IDF的阈值提高精确度或者使用其他的分类算法





## 完整代码

整个工程的完整代码下载：https://github.com/baixiaoyanvision/text_classify