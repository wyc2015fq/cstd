# 基于NLP自然语言构建的文档自动分类系统(搜狐娱乐)—word2vec模型 - wsp_1138886114的博客 - CSDN博客





2018年09月22日 18:26:15[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：89











### 文章目录
- [1 加载数据](#1__5)
- [2 计算每个文章的词向量](#2__23)
- [3 训练分类器](#3__47)
- [4 .训练与评估](#4__63)
- [5 模型保存](#5__150)
- [6 加载模型对新文档进行预测](#6__162)



 开发环境 jupyter notebook 

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

### 2 计算每个文章的词向量

```python
```python
# 加载训练好的Word2Vec模型
# 需要 4.0_训练word2vec模型.ipynb 的执行结果
from gensim.models import Word2Vec
w2v = Word2Vec.load('output_word2vec/model.w2v') 

# 使用文章中所有词的平均词向量作为文章的向量
import jieba
def compute_doc_vec_single(article):
    vec = np.zeros((w2v.layer1_size,), dtype=np.float32)
    n = 0
    for word in jieba.cut(article):
        if word in w2v:
            vec += w2v[word]
            n += 1
    return vec / n

def compute_doc_vec(articles):
    return np.row_stack([compute_doc_vec_single(x) for x in articles]) 
    
x = compute_doc_vec(train_data[u'文章']) 
print (x.shape)
```
```

### 3 训练分类器

```python
```python
from sklearn.preprocessing import LabelEncoder
from sklearn.model_selection import train_test_split 

# 编码目标变量
y_encoder = LabelEncoder()
y = y_encoder.fit_transform(train_data[u'频道']) 

# 划分训练测试数据,根据y分层抽样，测试数据占20%
train_idx, test_idx = train_test_split(range(len(y)), test_size=0.2, stratify=y)
train_x = x[train_idx, :]
train_y = y[train_idx]
test_x = x[test_idx, :]
test_y = y[test_idx]
```
```

### 4 .训练与评估

```python
```python
# 训练逻辑回归模型 
from sklearn.linear_model import LogisticRegression
"""
	常用参数说明:
	penalty: 正则项类型，l1还是l2
	C:       正则项惩罚系数的倒数，越大则惩罚越小
	fit_intercept: 是否拟合常数项
	max_iter:      最大迭代次数
	multi_class:   以何种方式训练多分类模型
	     ovr =     对每个标签训练二分类模型
	     multinomial = 直接训练多分类模型，仅当solver={newton-cg, sag, lbfgs}时支持
	 solver: 用哪种方法求解，可选有{liblinear, newton-cg, sag, lbfgs}
	         小数据liblinear比较好，大数据量sag更快
	         多分类问题，liblinear只支持ovr模式，其他支持ovr和multinomial
	         liblinear支持l1正则，其他只支持l2正则
"""

model = LogisticRegression(multi_class='multinomial', solver='lbfgs')
model.fit(train_x, train_y)
```
```

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
|体育|385|0|4|3|2|1|1|0|2|0|1|1|
|健康|0|314|22|0|0|5|6|27|2|0|6|18|
|女人|7|13|323|19|2|2|17|4|3|3|7|0|
|娱乐|1|1|15|308|0|3|60|3|2|1|6|0|
|房地产|1|3|4|3|357|0|0|13|4|1|0|14|
|教育|0|5|4|5|0|335|6|32|3|1|5|4|
|文化|1|3|25|67|2|7|232|30|15|2|13|3|
|新闻|8|14|13|7|22|26|25|227|12|4|16|26|
|旅游|1|5|15|1|5|5|15|15|312|6|11|9|
|汽车|0|3|5|0|1|0|0|7|5|365|0|14|
|科技|2|9|4|3|2|3|12|20|8|3|319|15|
|财经|3|4|2|2|29|1|1|40|2|12|25|279|

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

eval_model(test_y, test_y_pred, y_encoder.classes_) #查看训练结果
```
```

**查看前五条**
|$~$|Label|Precision|Recall|F1|Support|
|----|----|----|----|----|----|
|0|体育|0.941320|0.9625|0.951792|400|
|1|健康|0.839572|0.7850|0.811370|400|
|2|女人|0.740826|0.8075|0.772727|400|
|3|娱乐|0.736842|0.7700|0.753056|400|
|4|房地产|0.845972|0.8925|0.868613|400|

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
        'lr': model
    }, outfile)
```
```

### 6 加载模型对新文档进行预测

```python
```python
from gensim.models import Word2Vec
import dill
import pickle
import jieba

# 把预测相关的逻辑封装在一个类中，使用这个类的实例来对新文档进行分类预测
class Predictor(object):
    
    def __init__(self, w2v_model_file, lr_model_file):
        self.w2v = Word2Vec.load(w2v_model_file)
        with open(lr_model_file, 'rb') as infile:
            self.model = pickle.load(infile)
    
    def predict(self, articles):
        x = self._compute_doc_vec(articles)
        y = self.model['lr'].predict(x)
        y_label = self.model['y_encoder'].inverse_transform(y)
        return y_label
    
    def _compute_doc_vec(self, articles):
        return np.row_stack([compute_doc_vec_single(x) for x in articles])

    def _compute_doc_vec_single(self, article):
        vec = np.zeros((w2v.layer1_size,), dtype=np.float32)
        n = 0
        for word in jieba.cut(article):
            if word in w2v:
                vec += w2v[word]
                n += 1
        return vec / n
```
```

```python
```python
# 加载新文档数据
new_data = pd.read_csv('data/sohu_test.txt', sep='\t', header=None, 
                       dtype=np.str_, encoding='utf8', names=[u'频道', u'文章'])
new_data.head() 

# 加载模型
predictor = Predictor('output_word2vec/model.w2v', model_file)

# 预测前10篇的分类
new_y_pred = predictor.predict(new_data[u'文章'][:10])

# 对比预测
pd.DataFrame({u'预测频道': new_y_pred, u'实际频道': new_data[u'频道'][:10]})

输出：
|      | 实际频道 | 预测频道 |
| ---- | --------| -------- |
| 0    | 娱乐     | 娱乐     |
| 1    | 娱乐     | 体育     |
| 2    | 娱乐     | 娱乐     |
| 3    | 娱乐     | 文化     |
| 4    | 娱乐     | 女人     |
| 5    | 娱乐     | 新闻     |
| 6    | 娱乐     | 娱乐     |
| 7    | 娱乐     | 娱乐     |
| 8    | 娱乐     | 娱乐     |
| 9    | 娱乐     | 娱乐     |
```
```



