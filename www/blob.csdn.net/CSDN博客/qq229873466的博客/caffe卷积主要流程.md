# caffe卷积主要流程 - qq229873466的博客 - CSDN博客

2017年02月23日 15:51:34[qq229873466](https://me.csdn.net/qq229873466)阅读数：185标签：[caffe](https://so.csdn.net/so/search/s.do?q=caffe&t=blog)
个人分类：[caffe](https://blog.csdn.net/qq229873466/article/category/6551642)



# 公式回顾

##   NN

## ![](https://img-blog.csdn.net/20170223112848226)

##   CNN

![](https://img-blog.csdn.net/20170223112852788)

![](https://img-blog.csdn.net/20170223150150563)

# 主要流程

caffe和matlab deep learning tool box 的卷积实现过程是不同。前者没有用CNN的第一条公式，而是将其拆成多个全连接然后使用NN的公式，并且激活函数的实现在ReLU层，所以实现起来就更简单了，就是矩阵相乘。

![](https://img-blog.csdn.net/20170223155043629)

