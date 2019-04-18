# 基于NLP自然语言构建的文档自动分类系统(搜狐娱乐)—word-of-bag模型 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 15:09:48[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：59











### 文章目录
- [1 加载数据](#1__5)
- [2 计算每个文章的词袋](#2__23)
- [3 训练分类器](#3__36)
- [4 模型效果评估](#4__61)
- [5 模型保存](#5__117)
- [6 对新文档预测](#6__130)



开发环境`jupyter notebook`

项目数据下载地址：[https://download.csdn.net/download/wsp_1138886114/10612733](https://download.csdn.net/download/wsp_1138886114/10612733)

项目数据下载地址：[https://download.csdn.net/download/wsp_1138886114/10612746](https://download.csdn.net/download/wsp_1138886114/10612746)

### 1 加载数据

```python
```python
import numpy as np
import pandas as pd

# 查看训练数据
train_data = pd.read_csv('data/sohu_train.txt', sep='\t', header=None, 
                         dtype=np.str_, encoding='utf8', names=[u'频道', u'文章'])
train_data.head()

# 载入停用词
stopwords = set()
with open('data/stopwords.txt', 'rb') as infile:
    for line in infile:
        line = line.rstrip('\n')
        if line:
            stopwords.add(line.lower())
```
```

### 2 计算每个文章的词袋

```python
```python
import jieba
from sklearn.feature_extraction.text import TfidfVectorizer

tfidf = TfidfVectorizer(tokenizer=jieba.lcut, 
                        stop_words=stopwords, 
                        min_df=50, 
                        max_df=0.3)
x = tfidf.fit_transform(train_data[u'文章'])

print (u'词表大小: {}'.format(len(tfidf.vocabulary_)))
```
```

### 3 训练分类器

```python
```python
# 编码目标变量
from sklearn.preprocessing import LabelEncoder
y_encoder = LabelEncoder()
y = y_encoder.fit_transform(train_data[u'频道']) 

# 编码X变量
x = tfidf.transform(train_data[u'文章'])

from sklearn.model_selection import train_test_split

#划分训练测试数,据根据y分层抽样，测试数据占20%
train_idx, test_idx = train_test_split(range(len(y)), test_size=0.2, stratify=y)
train_x = x[train_idx, :]
train_y = y[train_idx]
test_x = x[test_idx, :]
test_y = y[test_idx]
```
```

```python
```python
from sklearn.linear_model import LogisticRegression 

model = LogisticRegression(multi_class='multinomial', solver='lbfgs')
model.fit(train_x, train_y)
```
```

### 4 模型效果评估

```python
```python
from sklearn.metrics import confusion_matrix, precision_recall_fscore_support 

# 在测试集上计算模型的表现
test_y_pred = model.predict(test_x)

# 计算混淆矩阵
pd.DataFrame(confusion_matrix(test_y, test_y_pred), 
             columns=y_encoder.classes_, 
             index=y_encoder.classes_)
```
```
|$~$|体育|健康|女人|娱乐|房地产|教育|文化|新闻|旅游|汽车|科技|财经|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|体育|389|0|3|0|0|1|4|1|1|0|1|0|
|健康|0|362|15|0|0|1|4|9|0|0|4|5|
|女人|1|7|351|14|1|0|16|3|5|1|1|0|
|娱乐|1|0|4|346|0|6|36|2|2|0|3|0|
|房地产|0|0|1|3|366|3|0|7|4|0|1|15|
|教育|0|7|5|1|0|347|8|22|1|0|4|5|
|文化|1|3|13|34|1|3|321|7|8|2|7|0|
|新闻|6|5|7|0|14|21|15|271|8|8|19|26|
|旅游|0|0|7|1|0|4|7|11|364|2|2|2|
|汽车|1|1|2|1|0|1|1|1|2|384|0|6|
|科技|0|7|2|2|2|4|4|17|9|3|336|14|
|财经|0|10|0|1|19|0|3|31|3|10|19|304|

```python
```python
# 计算各项评价指标
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
        u'Label': [u'总体'],
        u'Precision': [tot_p],
        u'Recall': [tot_r],
        u'F1': [tot_f1],
        u'Support': [tot_s]
    })
    res2.index = [999]
    res = pd.concat([res1, res2])
    return res[[u'Label', u'Precision', u'Recall', u'F1', u'Support']] 

eval_model(test_y, test_y_pred, y_encoder.classes_)
```
```

### 5 模型保存

```python
```python
# 保存模型到文件
import dill
import pickle
model_file = os.path.join(output_dir, u'model.pkl')
with open(model_file, 'wb') as outfile:
    pickle.dump({
        'y_encoder': y_encoder,
        'tfidf': tfidf,
        'lr': model
    }, outfile)
```
```

### 6 对新文档预测

```python
```python
# 加载新文档数据
new_data = pd.read_csv('data/sohu_test.txt', sep='\t', header=None, 
                       dtype=np.str_, encoding='utf8', names=[u'频道', u'文章'])
new_data.head() 

# 加载模型
import pickle

with open(model_file, 'rb') as infile:
    model = pickle.load(infile) 

# 对新文档预测（这里只对前10篇预测）
# 1. 转化为词袋表示
new_x = model['tfidf'].transform(new_data[u'文章'][:10]) 

# 2. 预测类别
new_y_pred = model['lr'].predict(new_x) 

# 3. 解释类别
pd.DataFrame({u'预测频道': model['y_encoder'].inverse_transform(new_y_pred), 
              u'实际频道': new_data[u'频道'][:10]}) 

输出：
|      | 实际频道 | 预测频道 |
| ---- | -------- | -------- |
| 0    | 娱乐     | 娱乐     |
| 1    | 娱乐     | 体育     |
| 2    | 娱乐     | 娱乐     |
| 3    | 娱乐     | 娱乐     |
| 4    | 娱乐     | 教育     |
| 5    | 娱乐     | 娱乐     |
| 6    | 娱乐     | 娱乐     |
| 7    | 娱乐     | 娱乐     |
| 8    | 娱乐     | 娱乐     |
| 9    | 娱乐     | 娱乐     |
```
```



