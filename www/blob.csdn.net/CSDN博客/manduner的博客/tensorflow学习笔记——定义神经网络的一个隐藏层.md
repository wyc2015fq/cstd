
# tensorflow学习笔记——定义神经网络的一个隐藏层 - manduner的博客 - CSDN博客


2018年06月13日 15:55:58[Manduner_TJU](https://me.csdn.net/manduner)阅读数：443


# 描述
常见的神经网络，如前馈神经网络，BP神经网络，自动编码器等，参数一般是权重W，偏置b以及激活函数activation function。
在具体应用一个神经网络模型前，通过tensorflow提前定义一个神经网络的添加层是很容易，也是很有必要的
# 源代码
```python
#coding:utf-8
```
```python
#导入tensorflow模块
```
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
```
```python
#定义一个添加层
```
```python
def
```
```python
add_layer
```
```python
(inputs,in_size,out_size,activation_function = None)
```
```python
:
```
```python
#初始化权重，一般是随机初始化
```
```python
Weights = tf.Variable(tf.random_normal([in_size,out_size]))
```
```python
#初始化偏置，一般会在0的基础上加0.1
```
```python
biases = tf.Variable(tf.zeros([
```
```python
1
```
```python
,out_size]) +
```
```python
0.1
```
```python
)
```
```python
#计算Wx+b
```
```python
feature = tf.matmul(inputs,Weights) + biases
```
```python
#判断是否需要激活函数
```
```python
if
```
```python
activation_function
```
```python
is
```
```python
None
```
```python
:
        outputs = feature
```
```python
else
```
```python
:
        outputs = activation_function(feature)
```
```python
#输出激活后的隐特征
```
```python
return
```
```python
outputs
```

