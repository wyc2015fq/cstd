# 基于jieba、TfidfVectorizer、LogisticRegression的文档分类



潇洒坤

发表于

[简书专栏](https://cloud.tencent.com/developer/column/5209)订阅



jieba中文叫做**结巴**，是一款中文分词工具，官方文档链接：<https://github.com/fxsjy/jieba>  TfidfVectorizer中文叫做**词袋向量化模型**，是用来文章内容向量化的工具，官方文档链接：<http://sklearn.apachecn.org/cn/0.19.0/modules/generated/sklearn.feature_extraction.text.TfidfVectorizer.html>  LogisticRegression中文叫做**逻辑回归模型**，是一种基础、常用的分类方法。

建议读者安装anaconda，这个集成开发环境自带了很多包。  到2018年9月1日仍为最新版本的anaconda下载链接: <https://pan.baidu.com/s/1pbzVbr1ZJ-iQqJzy1wKs0A> 密码: g6ex  官网下载地址：<https://repo.anaconda.com/archive/Anaconda3-5.2.0-Windows-x86_64.exe>  下面代码的开发环境为jupyter notebook，使用在jupyter notebook中的截图表示运行结果。

## 0.打开jupyter

在桌面新建文件夹命名为**基于TfidfVectorizer的文档分类**，如下图所示:  

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/nw0ckn3px9.png?imageView2/2/w/1620)

image.png

   打开**基于TfidfVectorizer的文档分类**文件夹，在按住Shift键的情况下，点击鼠标右键，出现如下图所示。  选择**在此处打开PowerShell窗口**，之后会在此路径下打开PowerShell。  

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/6sy5qq4xt9.png?imageView2/2/w/1620)

image.png

   在PowerShell中输入命令并运行：**jupyter notebook**  

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/lj0fh4otn3.png?imageView2/2/w/1620)

image.png

   PowerShell运行命令后，会自动打开网页，点击如下图所示网页中的按钮：  

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/13ky84th6i.png?imageView2/2/w/1620)

image.png

   代码文件重命名为**tfidfVectorizerTest**，重命名按钮位置如下图所示：  

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/njond3ij06.png?imageView2/2/w/1620)

image.png

## 1.数据准备

训练集共有24000条样本，12个分类，每个分类2000条样本。  测试集共有12000条样本，12个分类，每个分类1000条样本。  数据集下载链接: <https://pan.baidu.com/s/1PY3u-WtfBdZQ8FsKgWo_KA> 密码: hq5v  下载完成后，将压缩文件包放到**基于TfidfVectorizer的文档分类**文件夹中，并将其解压到当前文件夹，如下图所示：  

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/fwau6vfr4p.png?imageView2/2/w/1620)

image.png

   加载训练集到变量train_df中，并打印训练集前5行，代码如下。  read_csv方法中有3个参数，第1个参数是加载文本文件的路径，第2个关键字参数sep是分隔符，第3个关键字参数header是文本文件的第1行是否为字段名。

```javascript
import pandas as pd

train_df = pd.read_csv('sohu_train.txt', sep='\t', header=None)
train_df.head()
```

上面一段代码的运行结果如下图所示：

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/tqoku8ogxb.png?imageView2/2/w/1620)

image.png

查看训练集每个分类的名字以及样本数量，代码如下：

```javascript
for name, group in train_df.groupby(0):
    print(name,len(group))
```

上面一段代码的运行结果如下图所示：

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/qp6y7rzejm.png?imageView2/2/w/1620)

image.png

加载测试集并查看每个分类的名字以及样本数量，代码如下：

```javascript
test_df = pd.read_csv('sohu_test.txt', sep='\t', header=None)
for name, group in test_df.groupby(0):
    print(name, len(group))
```

上面一段代码的运行结果如下图所示：

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/cy2sfgqo12.png?imageView2/2/w/1620)

image.png

载入停顿词赋值给变量stopWord_list，代码如下：

```javascript
with open('stopwords.txt', encoding='utf8') as file:
    stopWord_list = [k.strip() for k in file.readlines()]
```

## 2.分词

需要安装jieba库，cmd中安装命令：**pip install jieba**  对训练集的24000条样本循环遍历，使用jieba库的cut方法获得分词列表赋值给变量cutWords。  判断分词是否为停顿词，如果不为停顿词，则添加进变量cutWords中。  代码如下：

```javascript
iimport jieba
import time

train_df.columns = ['分类', '文章']
stopword_list = [k.strip() for k in open('stopwords.txt', encoding='utf8').readlines() if k.strip() != '']
cutWords_list = []
i = 0
startTime = time.time()
for article in train_df['文章']:
    cutWords = [k for k in jieba.cut(article) if k not in stopword_list]
    i += 1
    if i % 1000 == 0:
        print('前%d篇文章分词共花费%.2f秒' %(i, time.time()-startTime))
    cutWords_list.append(cutWords)
```

上面一段代码的运行结果如下：

```
 前1000篇文章分词共花费67.62秒
 前2000篇文章分词共花费133.32秒
 前3000篇文章分词共花费272.28秒
 前4000篇文章分词共花费405.01秒
 前5000篇文章分词共花费529.79秒
 前6000篇文章分词共花费660.60秒
 前7000篇文章分词共花费696.51秒
 前8000篇文章分词共花费732.88秒
 前9000篇文章分词共花费788.51秒
 前10000篇文章分词共花费841.61秒
 前11000篇文章分词共花费903.35秒
 前12000篇文章分词共花费970.47秒
 前13000篇文章分词共花费1010.61秒
 前14000篇文章分词共花费1048.76秒
 前15000篇文章分词共花费1100.81秒
 前16000篇文章分词共花费1154.80秒
 前17000篇文章分词共花费1207.07秒
 前18000篇文章分词共花费1256.73秒
 前19000篇文章分词共花费1374.76秒
 前20000篇文章分词共花费1493.85秒
 前21000篇文章分词共花费1523.02秒
 前22000篇文章分词共花费1552.69秒
 前23000篇文章分词共花费1598.88秒
 前24000篇文章分词共花费1644.56秒
```

从上面的运行结果可以看出，对24000篇文章进行分词共使用1644秒，即27分24秒。  时间充裕的读者可以自己运行试试，将分词结果保存为本地文件**cutWords_list.txt**，代码如下：

```javascript
with open('cutWords_list.txt', 'w') as file: 
    for cutWords in cutWords_list:
        file.write(' '.join(cutWords) + '\n')
```

上面一段代码大概5秒左右运行完成，本文作者提供已经分词完成的文本文件。  读者节省时间可以下载，链接: <https://pan.baidu.com/s/1vCBeHNR6DEGSQQDvA7yQOw> 密码: j49q  下载文件是单个文本文件压缩的zip文件，文件大小为50M。  压缩的zip文件解压后的文本文件大小为118M。  载入分词文件的代码如下：

```javascript
with open('cutWords_list.txt') as file:
    cutWords_list = [k.split() for k in file.readlines()]
```

## 3.TfidfVectorizer模型

调用sklearn.feature_extraction.text库的TfidfVectorizer方法实例化模型对象。  TfidfVectorizer方法需要4个参数。  第1个参数是分词结果，数据类型为列表，其中的元素也为列表；  第2个关键字参数stop_words是停顿词，数据类型为列表；  第3个关键字参数min_df是词频低于此值则忽略，数据类型为int或float;  第4个关键字参数max_df是词频高于此值则忽略，数据类型为Int或float。  查看TfidfVectorizer方法的更多参数用法，官方文档链接：<http://sklearn.apachecn.org/cn/0.19.0/modules/generated/sklearn.feature_extraction.text.TfidfVectorizer.html>

```python
from sklearn.feature_extraction.text import TfidfVectorizer

tfidf = TfidfVectorizer(cutWords_list, stop_words=stopWord_list, min_df=40, max_df=0.3)
```

## 4.特征工程

程序运行时占电脑内存的情况如下图所示：

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/71u5yjpqes.png?imageView2/2/w/1620)

image.png

从上图可以看出，此程序占3384MB内存，所以电脑需要较高的内存配置，  第1行代码查看向量化的维数，即特征的维数；  第2行代码调用TfidfVectorizer对象的fit_transform方法获得特征矩阵赋值给X；  第3行代码查看特征矩阵的形状。

```javascript
print('词表大小:', len(tfidf.vocabulary_))
X = tfidf.fit_transform(train_df[1])
print(X.shape)
```

上面一段代码的运行结果如下：

>  词表大小: 3946  (24000, 3946)  

## 5.模型训练

### 5.1 标签编码

调用sklearn.preprocessing库的LabelEncoder方法对**文章分类**做**标签编码**。  最后一行代码查看预测目标的形状。

```javascript
from sklearn.preprocessing import LabelEncoder
import pandas as pd

train_df = pd.read_csv('sohu_train.txt', sep='\t', header=None)
labelEncoder = LabelEncoder()
y = labelEncoder.fit_transform(train_df[0])
y.shape
```

### 5.2 逻辑回归模型

调用sklearn.linear_model库的LogisticRegression方法实例化模型对象。  调用sklearn.model_selection库的train_test_split方法划分训练集和测试集。

```javascript
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split

train_X, test_X, train_y, test_y = train_test_split(X, y, test_size=0.2)
logistic_model = LogisticRegression(multi_class='multinomial', solver='lbfgs')
logistic_model.fit(train_X, train_y)
logistic_model.score(test_X, test_y)
```

上面一段代码的运行结果如下：

>  0.8754166666666666  

### 5.3 保存模型

保存模型需要先安装pickle库，安装命令：**pip install pickle**  调用pickle库的dump方法保存模型，需要2个参数。  第1个参数是保存的对象，可以为任意数据类型，因为有3个模型需要保存，所以下面代码第1个参数是字典。  第2个参数是保存的文件对象，数据类型为**_io.BufferedWriter**

```javascript
import pickle

with open('tfidf.model', 'wb') as file:
    save = {
        'labelEncoder' : labelEncoder,
        'tfidfVectorizer' : tfidf,
        'logistic_model' : logistic_model
    }
    pickle.dump(save, file)
```

本文作者提供自己完成的模型持久化文件，下载链接: <https://pan.baidu.com/s/1JIA_E-S3PotAGY4oLqy93w> 密码: e3yk  压缩文件大小：188.8M  解压后的模型文件大小：498.9M

### 5.4 交叉验证

在进行此步的时候，不需要运行此步之前的所有步骤，即可以重新运行jupyter notebook。  调用pickle库的load方法加载保存的模型对象，代码如下：

```javascript
import pickle

with open('tfidf.model', 'rb') as file:
    tfidf_model = pickle.load(file)
    tfidfVectorizer = tfidf_model['tfidfVectorizer']
    labelEncoder = tfidf_model['labelEncoder']
    logistic_model = tfidf_model['logistic_model']
```

调用pandas的read_csv方法加载训练集数据。  调用TfidfVectorizer对象的transform方法获得特征矩阵。  调用LabelEncoder对象的transform方法获得预测目标值。  代码如下：

```javascript
import pandas as pd

train_df = pd.read_csv('sohu_train.txt', sep='\t', header=None)
X = tfidfVectorizer.transform(train_df[1])
y = labelEncoder.transform(train_df[0])
```

调用sklearn.linear_model库的LogisticRegression方法实例化逻辑回归模型对象。  调用sklearn.model_selection库的ShuffleSplit方法实例化交叉验证对象。  调用sklearn.model_selection库的cross_val_score方法获得交叉验证每一次的得分。  最后打印每一次的得分以及平均分，代码如下：

```javascript
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import ShuffleSplit
from sklearn.model_selection import cross_val_score

logistic_model = LogisticRegression(multi_class='multinomial', solver='lbfgs')
cv_split = ShuffleSplit(n_splits=5, test_size=0.3)
score_ndarray = cross_val_score(logistic_model, X, y, cv=cv_split)
print(score_ndarray)
print(score_ndarray.mean())
```

上面一段代码的运行结果如下：

>  [0.86819444 0.87430556 0.86861111 0.87       0.87430556]  0.8710833333333333  

## 6.模型评估

绘制混淆矩阵，代码如下：

```javascript
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegressionCV
from sklearn.metrics import confusion_matrix
import pandas as pd

train_X, test_X, train_y, test_y = train_test_split(X, y, test_size=0.2)
logistic_model = LogisticRegressionCV(multi_class='multinomial', solver='lbfgs')
logistic_model.fit(train_X, train_y)
predict_y = logistic_model.predict(test_X)
pd.DataFrame(confusion_matrix(test_y, predict_y), 
             columns=labelEncoder.classes_, 
             index=labelEncoder.classes_)
```

上面一段代码的运行结果如下图所示：

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/s8zs04k0x9.png?imageView2/2/w/1620)

image.png

绘制precision、recall、f1-score、support报告表，代码如下：

```javascript
def eval_model(y_true, y_pred, labels):
    # 计算每个分类的Precision, Recall, f1, support
    p, r, f1, s = precision_recall_fscore_support(y_true, y_pred)
    # 计算总体的平均Precision, Recall, f1, support
    tot_p = np.average(p, weights=s)
    tot_r = np.average(r, weights=s)
    tot_f1 = np.average(f1, weights=s)
    tot_s = np.sum(s)
    res1 = pd.DataFrame({
        u'Label': labels,
        u'Precision': p,
        u'Recall': r,
        u'F1': f1,
        u'Support': s
    })
    res2 = pd.DataFrame({
        u'Label': ['总体'],
        u'Precision': [tot_p],
        u'Recall': [tot_r],
        u'F1': [tot_f1],
        u'Support': [tot_s]
    })
    res2.index = [999]
    res = pd.concat([res1, res2])
    return res[['Label', 'Precision', 'Recall', 'F1', 'Support']]

predict_y = logistic_model.predict(test_X)
eval_model(test_y, predict_y, labelEncoder.classes_)
```

上面一段代码的运行结果如下图所示：

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/3imeun8i0o.png?imageView2/2/w/1620)

image.png

## 7.模型测试

模型测试，即对一个全新的测试集进行预测。  调用pandas库的read_csv方法读取测试集文件。  调用TfidfVectorizer对象的transform方法获得特征矩阵。  调用LabelEncoder对象的transform方法获得预测目标值。  下面一段代码能够成功运行的前提，是本文第5.4节和第6节已经运行。

```javascript
import pandas as pd

test_df = pd.read_csv('sohu_test.txt', sep='\t', header=None)
test_X = tfidfVectorizer.transform(test_df[1])
test_y = labelEncoder.transform(test_df[0])
predict_y = logistic_model.predict(test_X)
eval_model(test_y, predict_y, labelEncoder.classes_)
```

上面一段代码的运行结果如下图所示：

![img](https://ask.qcloudimg.com/http-save/yehe-2318291/3jv6a0xpmp.png?imageView2/2/w/1620)

模型测试结果.png

## 8.结论

本文是作者第2个NLP小型项目，训练集数据共有24000条，测试集数据共有12000条。  经过交叉验证，模型平均得分为**0.8711**。  模型评估时，使用LogisticRegressionCV模型，得分提高了3%，为**0.9076**。  最后在测试集上的f1-score指标为**0.8990**，总体来说这个分类模型较优秀，能够投入实际应用。

本文参与[腾讯云自媒体分享计划](https://cloud.tencent.com/developer/support-plan)，欢迎正在阅读的你也加入，一起分享。

发表于 2018-09-10