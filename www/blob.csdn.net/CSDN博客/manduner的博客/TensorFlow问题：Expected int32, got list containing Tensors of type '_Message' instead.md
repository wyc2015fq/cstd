
# TensorFlow问题：Expected int32, got list containing Tensors of type '_Message' instead. - manduner的博客 - CSDN博客


2018年07月18日 19:28:25[Manduner_TJU](https://me.csdn.net/manduner)阅读数：137


# 1，前言
查阅了很多资料，发现出现这个报错的始作俑者就是tf.concat()，话不多说，直接上错误“Expected int32, got list containing Tensors of type '_Message' instead.”的原因和解决办法
# 2，原因
TensorFlow版本带来的错误，具体分析如下：
TensorFlow 1.0之前的版本，concat()的参数顺序是，数字在前，tensors在后，举例如下：
```python
tf.concat(3,pooled_outputs_i)
```
TensorFlow 1.0之后的版本，concat()的参数顺序是，tensors在前，数字在后，举例如下：
```python
tf.concat(pooled_outputs_i,3)
```
# 3，解决办法
聪明如你，看到原因里面的两个解释，你就应该知道怎么调试了吧、、、、、就是把concat的参数顺序调整一下就一切ok啦

