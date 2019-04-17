# DeepCTR：易用可扩展的深度学习点击率预测算法库 - 浅梦的博客 - CSDN博客





2018年12月25日 16:55:16[浅梦s](https://me.csdn.net/u012151283)阅读数：327








本文首发于知乎专栏：[https://zhuanlan.zhihu.com/p/53231955](https://zhuanlan.zhihu.com/p/53231955)

这个项目主要是对目前的一些基于深度学习的点击率预测算法进行了实现，并且对外提供了一致的调用接口。

关于每种算法的介绍这里就不细说了，大家可以看论文，看知乎，看博客，讲的都很清楚的。
这里简单从整体上介绍一下DeepCTR这个库。

首先这个不是一个框架，它不具有学术创新意义，目前也没有解决什么复杂的工程问题。

它面向的对象是那些对深度学习以及CTR预测算法感兴趣的同学，可以利用这个库：- 从一个统一的视角来看待各个模型
- 快速地进行简单的对比实验
- 利用已有的组件快速构建新的模型

# 统一视角

DeepCTR通过对现有的基于深度学习的点击率预测模型的结构进行抽象总结，在设计过程中采用模块化的思路，各个模块自身具有高复用性，各个模块之间互相独立。

基于深度学习的点击率预测模型按模型内部组件的功能可以划分成以下4个模块：输入模块，嵌入模块，特征提取模块，预测输出模块。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181225165310651.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)
# 快速实验

下面是一个简单的用DeepFM模型在criteo数据集上训练的的例子。

```python
import pandas as pd
from sklearn.preprocessing import LabelEncoder, MinMaxScaler
from deepctr.models import DeepFM
data = pd.read_csv('./criteo_sample.txt')

sparse_features = ['C' + str(i) for i in range(1, 27)]
dense_features = ['I'+str(i) for i in range(1, 14)]

data[sparse_features] = data[sparse_features].fillna('-1', )
data[dense_features] = data[dense_features].fillna(0,)
target = ['label']

# 1.Label Encoding for sparse features,and do simple Transformation for dense features
for feat in sparse_features:
    lbe = LabelEncoder()
    data[feat] = lbe.fit_transform(data[feat])
mms = MinMaxScaler(feature_range=(0, 1))
data[dense_features] = mms.fit_transform(data[dense_features])

# 2.count #unique features for each sparse field,and record dense feature field name

sparse_feature_dict = {feat: data[feat].nunique()
                       for feat in sparse_features}
dense_feature_list = dense_features

# 3.generate input data for model

model_input = [data[feat].values for feat in sparse_feature_dict] + \
    [data[feat].values for feat in dense_feature_list]  # + [data[target[0]].values]

# 4.Define Model,compile and train
model = DeepFM({"sparse": sparse_feature_dict,
                "dense": dense_feature_list}, final_activation='sigmoid')

model.compile("adam", "binary_crossentropy",
              metrics=['binary_crossentropy'], )

history = model.fit(model_input, data[target].values,
                    batch_size=256, epochs=10, verbose=2, validation_split=0.2, )
```

# 快速构建新模型

所有的模型都是严格按照4个模块进行搭建的，输入和嵌入以及输出基本都是公用的，每个模型的差异之处主要在体征提取部分。

下面是DeepFM模型的特征提取核心代码，大家也可以利用这些已有的组件去构建自己想要的模型。

```python
fm_input = Concatenate(axis=1)(embed_list)#将输入拼接成FM层需要的shape
deep_input = Flatten()(fm_input)#将输入拼接成Deep网络需要的shape
fm_out = FM()(fm_input)#调用FM组件
deep_out = MLP(hidden_size, activation, l2_reg_deep, keep_prob,
               use_bn, seed)(deep_input)
deep_logit = Dense(1, use_bias=False, activation=None)(deep_out)#调用Deep网络组件
```

# 相关资料

项目地址：[https://github.com/shenweichen/DeepCTR](https://github.com/shenweichen/DeepCTR)

感觉还还可以的话就star一下吧!

说明文档：[https://deepctr-doc.readthedocs.io/](https://deepctr-doc.readthedocs.io/)






