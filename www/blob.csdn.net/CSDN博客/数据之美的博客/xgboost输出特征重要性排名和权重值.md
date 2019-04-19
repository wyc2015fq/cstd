# xgboost输出特征重要性排名和权重值 - 数据之美的博客 - CSDN博客
2018年07月29日 20:18:48[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：7305
在机器学习实践中，我们要经常用到xgboost框架去训练数据，然后用训练得到的模型再去预测其他未知的数据的标签。
在判断训练得到的模型是否合理时，一个很重要的步骤就是查看xgboost模型的特征重要性排序。如果观察得到模型的排名前几的特征都不符合我们正常的思维，那么模型很可能是不稳定或者有问题的。在训练得到模型文件及特征映射文件后，可以通过下面一段代码查看特征排名、特征名称、特征权重值
```python
#-*-encoding: utf-8-*-
import xgboost as xgb
import pandas as pd
import operator
from matplotlib import pylab as plt
from pylab import *
fm = mpl.font_manager
fm.get_cachedir()
model_dir = 'your_model_directory_name/'
model_file = 'your_model_file_name'
fmap_file = 'your_fmap_file_name'
model =xgb.Booster(model_file=model_dir + model_file)
importance = model.get_fscore(fmap=model_dir + fmap_file)
f = open(model_dir + fmap_file)
fmap_dict = {}
for line in f:
    line = line.strip().split()
    fmap_dict[line[1]] = line[0]
feature_num = 10 #想要查看top10的特征名称及权重，这里设置想要查看前多少个特征及其权重
importance = sorted(importance.items(), key=operator.itemgetter(1), reverse=True)
importance = importance[:feature_num]
index = 1
print '特征排名', '特征名称', '特征权重值'
for line in importance:
    print index, fmap_dict[str(line[0])[1:]], line[1]
    index += 1
```
