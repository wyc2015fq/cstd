# 机器学习之路：python 字典特征提取器 DictVectorizer - fighting！！！ - CSDN博客
2019年01月05日 10:56:25[dujiahei](https://me.csdn.net/dujiahei)阅读数：46
转自：[https://www.cnblogs.com/Lin-Yi/p/8973252.html](https://www.cnblogs.com/Lin-Yi/p/8973252.html)
```python
from sklearn.feature_extraction import DictVectorizer
'''
字典特征提取器：
    将字典数据结构抽和向量化
    类别类型特征借助原型特征名称采用0 1 二值方式进行向量化
    数值类型特征保持不变
'''
# 定义一个字典列表 用来表示多个数据样本
measurements = [
    {"city": "Dubai", "temperature": 33.0},
    {"city": "London", "temperature": 12.0},
    {"city": "San Fransisco", "temperature": 18.0},
]
# 初始化字典特征抽取器
vec = DictVectorizer()
data = vec.fit_transform(measurements).toarray()
# 查看提取后的特征值
print(data)
'''
[[ 1.  0.  0. 33.]
 [ 0.  1.  0. 12.]
 [ 0.  0.  1. 18.]]
'''
# 查看提取后特征的含义
print(vec.get_feature_names())
'''
['city=Dubai', 'city=London', 'city=San Fransisco', 'temperature']
'''
```
