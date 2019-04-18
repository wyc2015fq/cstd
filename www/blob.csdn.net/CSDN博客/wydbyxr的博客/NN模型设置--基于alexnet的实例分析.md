# NN模型设置--基于alexnet的实例分析 - wydbyxr的博客 - CSDN博客
2018年12月04日 10:45:25[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：100
# 1）学习率
  ReLu的线性不饱和激活端着相对于Tanh的双饱和端（经验规则0.1），肯定要降量级。
  Caffe和Alex给的Model基础都是0.001(W)/0.002(b)。
# 2）基本没有用的LRN层
  LRN是unsupervised learning流行时期的遗留物。有一定bio inspired成分。现在看来似乎对performance贡献不大。目前，更多的是使用BN。
  尽管Caffe中默认是加上去的，但是根据作者在知乎的说法，只是为了尊重前人的成果。examples只是简单的复现了Alex的结构。
  有的人说有用（对精度影响很大），有的说没用。
  比如2012的alex net当中，使用了LRN，然而在very deep convolutional networks for large-scale image recognition又说没用，具体看实验数据，在A和A-lrn的比较当中，仅仅相差0.1个百分点。
  又比如Hinton等人在ImageNet上的实验表明，添加LRN层可以分别把Top 1和Top 5的error rate降低1.4%和1.2%。
  LRN的几个超参数，local_size, alpha, beta等，需要在验证集上选择最优值。论文中推荐的值为local_size = 5, alpha=e-4, beta = 0.75。
  当然，LRN层的实际作用到底有多大，还可以进一步参考后续的相关研究文献。正如@贾扬清大神所说，为了增加caffe对AlexNet，CaffeNet的兼容性，就不得不在Layer类中派生LRN layer类。
