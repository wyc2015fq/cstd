
# TensorFlow问题：‘module’ object has no attribute 'multiply' - manduner的博客 - CSDN博客


2018年06月20日 09:45:09[Manduner_TJU](https://me.csdn.net/manduner)阅读数：190


# 1，问题的引出
最近在调试一段论文代码时，编辑器提示如标题上的错误，原因如下：
# 2，原因及解决办法
TensorFlow版本不一致问题，TensorFlow发布的新版本的API做了如下修改：
旧版本：tf.mul、tf.sub、tf.neg
新版本：tf.multiply、tf.subtract、tf.negative
# 3.注意
根据你TensorFlow的版本配置选择合适的方法即可

