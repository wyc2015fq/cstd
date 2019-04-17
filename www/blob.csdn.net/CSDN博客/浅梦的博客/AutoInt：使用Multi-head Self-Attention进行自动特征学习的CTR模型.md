# AutoInt：使用Multi-head Self-Attention进行自动特征学习的CTR模型 - 浅梦的博客 - CSDN博客





2018年12月27日 22:04:25[浅梦s](https://me.csdn.net/u012151283)阅读数：469
个人分类：[深度学习](https://blog.csdn.net/u012151283/article/category/6698461)









本文首发于知乎专栏 [https://zhuanlan.zhihu.com/p/53462648](https://zhuanlan.zhihu.com/p/53462648)

# 简介

这篇论文提出使用multi-head self attention(类似Transformer里的那个) 机制来进行自动特征交叉学习以提升CTR预测任务的精度。

废话不多说，先看下主要结构。典型的四段式深度学习CTR模型结构：输入，嵌入，特征提取，输出。这里我们重点看下嵌入和特征提取部分
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227215755273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)
# 核心结构

## 输入和嵌入

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227215743303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)

针对类别特征，通过embedding方式转换为低维稠密向量
$e_i=V_ix_i$

其中，$V_i$是特征组$i$对应的嵌入字典(嵌入矩阵),$x_i$是特征组特征的独热编码表示向量(通常处于节省空间的考虑，只存储非零特征对应的索引)
对于连续特征有，
$e_m=v_mx_m$

其中$v_m$是嵌入向量，$x_m$是一个标量值
通常在CTR任务中我们对连续值特征对处理方式有三种：
- 进行归一化处理拼接到embedding向量侧
- 进行离散化处理作为类别特征
- 赋予其一个embedding向量，每次用特征值与embedding向量的乘积作为其最终表示

本文采取的是第三种方式，具体这三种方式孰优孰劣，要在具体场景具体任务下大家自己去验证了~

从实现的角度看第三种是比较便捷的。

## InteractingLayer（交互层）

交互层使用多头注意力机制将特征投射到多个子空间中，在不同的子空间中可以捕获不同的特征交互模式。通过交互层的堆叠，可以捕获更高阶的交互模式。

下面展示在特定子空间$h$下，对于特征组$m$下的特征$e_m$，交互层是如何计算与其相关的交互特征$\tilde{e}^{(h)}_m$的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227215721826.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)- 
首先输入特征通过矩阵乘法线性变换为在注意力空间下的向量表示，对于每个特征$e_m$在特定的注意力空间$h$中，都有三个表示向量$E^{h:Query}_m=W^{(h)}_{Query}e_m$,$E^{h:Key}_m=W^{(h)}_{Key}e_m$,$E^{h:Value}_m=W^{(h)}_{Value}e_m$

- 
计算$e_m$与其他特征$e_k$的相似度，本文使用向量内积表示：
$\phi^{(h)}(e_m,e_k)=&lt;E^{h:Query}_m,E^{h:Key}_k&gt;$

- 
计算softmax归一化注意力分布：$a^{(h)}_{m,k}=\frac{exp(\phi^{(h)}(e_m,e_k))}{\sum_{l=1}^M{exp(\phi^{(h)}(e_m,e_l))}}$

- 
通过加权求和的方式得到特征m及其相关的特征组成的一个新特征$\tilde{e}^{(h)}_m=\sum_{k=1}^Ma^{(h)}_{m,k}E^{h:Value}_m$


假设有$H$个注意力空间，对每个注意力空间下的结果进行拼接，得到特征$m$最终的结果表示

：
$\tilde{e}_m=\tilde{e}^{(1)}_m\oplus\tilde{e}^{(2)}_m\oplus\dots\oplus\tilde{e}^{(H)}_m$

我们可以选择使用残差网络保留一些原始特征的信息留给下一层继续学习
$e^{Res}_m=ReLU(\tilde{e}_m+W_{Res}e_m)$
最后，将每个特征的结果拼接，计算最终的输出值
$\hat{y}=\sigma(w^T(e^{Res}_1\oplus e^{Res}_2 \dots e^{Res}_M)+b)$

# 我不想看数学，我想看代码：OK

下面就是核心代码啦，可以看到其实很短。

我们使用tensorflow进行实现的时候，可以充分利用矩阵运算的特性来简化实现。

先说明一些定义，`att_embedding_size`为注意力空间下隐向量的长度，`head_num`为注意力空间的个数，`use_res`为一个布尔变量，表示是否使用残差连接。

首先假设输入`inputs`的shape为`(batch_size,field_size,embedding_size)`，四个投影矩阵(W_Query,W_Key,W_Value,W_Res)的shape均为`(embedding_size, att_embedding_size * head_num)`
- 通过矩阵乘法得到注意力空间下的三组向量表示

```
querys = tf.tensordot(inputs, self.W_Query, axes=(-1, 0))  # (batch_size,field_size,att_embedding_size*head_num)
    keys = tf.tensordot(inputs, self.W_key, axes=(-1, 0))
    values = tf.tensordot(inputs, self.W_Value, axes=(-1, 0))
```
- 为了同时在不同的子空间下计算特征相似度，需要先进行一些变换

```
querys = tf.stack(tf.split(querys, self.head_num, axis=2))  # (head_num,batch_size,field_size,att_embedding_size)
    keys = tf.stack(tf.split(keys, self.head_num, axis=2))
    values = tf.stack(tf.split(values, self.head_num, axis=2))
```
- 计算相似度及归一化注意力分布

```
inner_product = tf.matmul(querys, keys, transpose_b=True)  # (head_num,batch_size,field_size,field_size)
    self.normalized_att_scores = tf.nn.softmax(inner_product)
```
- 计算加权和

```
result = tf.matmul(self.normalized_att_scores, values)  # (head_num,batch_size,field_size,att_embedding_size)
```
- 将不同子空间下的结果进行拼接

```
result = tf.concat(tf.split(result, self.head_num, ), axis=-1)
    result = tf.squeeze(result, axis=0)# (batch_size,field_size,att_embedding_size*head_num)
```
- 使用残差连接保留原始信息，

```
if use_res:
        result += tf.tensordot(inputs, self.w_res, axes=(-1, 0))
    result = tf.nn.relu(result)# (batch_size,field_size,att_embedding_size*head_num)
```

# 我不想看代码，我想直接拿来用：没问题

首先确保你的python版本>=3.4，然后`pip install deepctr`，

再去下载一下[demo数据](https://github.com/shenweichen/DeepCTR/blob/master/examples/criteo_sample.txt)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227215642619.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)

然后直接运行下面的代码吧!
```python
from deepctr.models import AutoInt
import pandas as pd
from sklearn.preprocessing import LabelEncoder, MinMaxScaler
if __name__ == "__main__":
    data = pd.read_csv('./criteo_sample.txt')
    sparse_features = ['C' + str(i) for i in range(1, 27)]
    dense_features = ['I'+str(i) for i in range(1, 14)]

    data[sparse_features] = data[sparse_features].fillna('-1', )
    data[dense_features] = data[dense_features].fillna(0,)
    target = ['label']

    for feat in sparse_features:
        lbe = LabelEncoder()
        data[feat] = lbe.fit_transform(data[feat])
    mms = MinMaxScaler(feature_range=(0, 1))
    data[dense_features] = mms.fit_transform(data[dense_features])

    sparse_feature_dict = {feat: data[feat].nunique() for feat in sparse_features}
    dense_feature_list = dense_features

    model_input = [data[feat].values for feat in sparse_feature_dict] + \
        [data[feat].values for feat in dense_feature_list]  # + [data[target[0]].values]

    model = AutoInt({"sparse": sparse_feature_dict,"dense": dense_feature_list})

    model.compile("adam", "binary_crossentropy",metrics=['binary_crossentropy'], )

    history = model.fit(model_input, data[target].values,batch_size=256, epochs=10,verbose=2, validation_split=0.2, )
```

# deepctr是个什么玩意？？

请移步~

# 参考资料
- [AutoInt:Automatic Feature Interaction Learning via Self-Attentive Neural Networks](https://arxiv.org/pdf/1810.11921.pdf)(本文贴图来自于该论文)
- [Attention Is All You Need](https://arxiv.org/pdf/1706.03762.pdf)









