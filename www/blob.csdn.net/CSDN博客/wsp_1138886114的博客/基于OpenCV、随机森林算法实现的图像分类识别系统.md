# 基于OpenCV、随机森林算法实现的图像分类识别系统 - wsp_1138886114的博客 - CSDN博客





2018年08月11日 16:27:35[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1284










- - - [1 数据准备](#1-数据准备)
- [2 使用RGB颜色直方图做特征训练分类器](#2-使用rgb颜色直方图做特征训练分类器)- - [2.1 计算RGB颜色直方图](#21-计算rgb颜色直方图)
- [2.2 随机森林—训练分类器](#22-随机森林训练分类器)
- [2.3 评估随机森林分类器](#23-评估随机森林分类器)
- [2.4 逻辑回归—训练分类器](#24-逻辑回归训练分类器)
- [2.5 评估逻辑回归分类器](#25-评估逻辑回归分类器)









开发环境`jupyter notebook`


```python
import cv2
import os
import pickle                               #持久化
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import confusion_matrix, precision_recall_fscore_support 

output_dir = 'output'                       #图片保存目录
if not os.path.exists(output_dir):
    os.mkdir(output_dir)
```

### 1 数据准备

```
image_dir = '../data/images'                     #图片路径

images = []
labels = []
for fname in os.listdir(image_dir):

    if not fname.startswith('image'): continue   # 跳过不是目标图片的文件
    fpath = os.path.join(image_dir, fname)
    lab = fpath.split('_')[1]                    # 根据文件名，提取图片分类
    img = cv2.imread(fpath, cv2.IMREAD_COLOR)    # RGB模式读取图片
    images.append(img)                           #获取所有图片
    labels.append(lab)                           #


label_encoder = LabelEncoder()                  # 将图片标签ID化
y = label_encoder.fit_transform(labels) 

# 训练集，测试集划分
train_idx, test_idx = train_test_split(range(len(y)), test_size=0.2, stratify=y, random_state=1234)
train_y = y[train_idx]
test_y = y[test_idx]
```

### 2 使用RGB颜色直方图做特征训练分类器

##### 2.1 计算RGB颜色直方图

```python
# 定义特征转化函数
def transform(img):
    hist = cv2.calcHist([img], [0,1,2], None, [8]*3, [0,256]*3) # 每个通道等分为8组后计算直方图
    return hist.ravel()                                         # 将8x8x8的多维数组展平


x = np.row_stack([transform(img) for img in images])            # 提取每个图像的直方图特征
train_x = x[train_idx, :]
test_x = x[test_idx, :]
```

##### 2.2 随机森林—训练分类器

```python
"""
    RandomForestClassifier 常用参数

    n_estimators: 整数，树的个数，默认10，树越多拟合效果越好，也越容易过拟合
    criterion: 衡量树的某次分裂好坏的指标，可选值有{'gini', 'entropy'}
    max_features: 寻找最佳分裂时最多使用多少个特征
         如果为int，表示每次分裂时的最大特征数
         如果为float，则最大特征数为int(max_features * n_features)
         如果为'auto'，则max_features=sqrt(n_features)
         如果为'sqrt'，则max_features=sqrt(n_features)
         如果为'log2'，则max_features=log2(n_features)
         如果为None，则max_features=n_features
     max_depth: int或None，每个树最大的深度。默认None，表示每个树节点会一直分裂直到所有
         的叶子节点都是纯的，或者包含min_samples_split个样本。深度越深，越容易过拟合。
     min_samples_split: int或float，默认为2，对一个节点进行分裂时，至少需要这么多样本。
         int: 表示样本数
         float: 表示比例，即最小样本数为 ceil(min_samples_split * n_samples)
     min_samples_leaf: int或float，默认为1，叶子节点至少需要这么多样本
         int: 表示样本数
         float: 表示比例，样本数为 ceil(min_samples_leaf * n_samples)
"""
model = RandomForestClassifier(n_estimators=15, max_depth=3, random_state=1234)
model.fit(train_x, train_y)        #拟合训练
```

```python
# 保存模型
def save_model(model, label_encoder, output_file):
    try:
        with open(output_file, 'wb') as outfile:
            pickle.dump({
                'model': model,
                'label_encoder': label_encoder
            }, outfile)
        return True
    except:
        return False

save_model(model, label_encoder, os.path.join(output_dir, 'model_rf1.pkl'))
```

##### 2.3 评估随机森林分类器

```python
# 计算各项评价指标
def eval_model(y_true, y_pred, labels):
    # 计算每个分类的Precision, Recall, f1, support
    p, r, f1, s = precision_recall_fscore_support(y_true, y_pred)
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

# 计算混淆矩阵
conf_mat = pd.DataFrame(confusion_matrix(y_true, y_pred), columns=labels, index=labels)
return conf_mat, res[[u'Label', u'Precision', u'Recall', u'F1', u'Support']]

# 在测试集上计算每个图片的预测分类
y_pred = model.predict(test_x)

# 评估模型
conf_mat, evalues = eval_model(test_y, y_pred, label_encoder.classes_)
conf_mat                         #查看混淆矩阵
evalues                          # 各项评价指标
```

![这里写图片描述](https://img-blog.csdn.net/20180811160818748?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 2.4 逻辑回归—训练分类器

```
model = LogisticRegression(penalty='l2', C=1, random_state=1234)
model.fit(train_x, train_y) 

save_model(model, label_encoder, os.path.join(output_dir, 'model_lr1.pkl'))
```

##### 2.5 评估逻辑回归分类器

```
# 在测试集上计算每个图片的预测分类
y_pred = model.predict(test_x)

# 评估模型
conf_mat, evalues = eval_model(test_y, y_pred, label_encoder.classes_) 
conf_mat              #混淆矩阵
evalues               #各项评价指标
```



