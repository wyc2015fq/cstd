# python 随机打乱 图片和对应的标签 - 别说话写代码的博客 - CSDN博客





2018年01月19日 09:40:33[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：950








```python
# -*- coding: utf-8 -*-

import os
import numpy as np
import pandas as pd
import h5py
import pylab
import matplotlib.pyplot as plt


trainpath = str('C:/Users/49691/Desktop/数据集/train/')
testpath = str('C:/Users/49691/Desktop/数据集/test/')
n_tr = len(os.listdir(trainpath))
print('num of training files: ', n_tr)

train_labels = pd.read_csv('C:/Users/49691/Desktop/数据集/sample_submission.csv')
train_labels.head()

from skimage import io, transform


x = np.empty(shape=(n_tr, 224, 224, 3))
y = np.empty(n_tr)

labels = train_labels.invasive.values
name = train_labels.name.values

permutation=np.random.permutation(name.shape[0])
print(permutation)
print(labels[permutation])
save_data = pd.DataFrame({'name':permutation,'invasive':labels[permutation]})
save_data.to_csv('C:/Users/49691/Desktop/数据集/b.csv')


for k,v in enumerate(np.random.permutation(n_tr)):
    print(k,v)
    path = '{0}{1}.jpg'.format(trainpath, v)
    tr_im = io.imread(path)
    x[k] = transform.resize(tr_im, output_shape=(224, 224, 3))
    y[k] = float(labels[v-1])
```




