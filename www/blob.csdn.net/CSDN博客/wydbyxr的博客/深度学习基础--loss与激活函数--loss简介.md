# 深度学习基础--loss与激活函数--loss简介 - wydbyxr的博客 - CSDN博客
2018年11月16日 09:46:06[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：66
# loss简介
  为了训练我们的模型，我们首先需要定义一个指标来评估这个模型是好的。反过来想，在机器学习，我们通常定义指标来表示一个模型是坏的，这个指标称为成本（cost）或损失（loss），然后尽量最小化这个指标。但是，这两种方式是相同的。
## 常见的损失函数
  1）Cross-Entropy；
  2）MSE，sigmoid和softmax；
  3）K-L散度
## 分类loss
  对于分类loss，是一个N+1路的softmax输出，其中的N是类别个数，1是背景，使用softmax分类器。
## 回归loss
  对于回归loss，是一个4xN路输出的bounding-box regressor（有4个元素(x,y,w,h)，左上角坐标(x,y)，宽w，高h），也就是说对于每个类别都会训练一个单独的regressor。
  线性回归只适用于因变量为连续型变量，多项逻辑回归是因变量有多个取值。
