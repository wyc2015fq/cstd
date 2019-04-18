# Improving speech recognition by revising gated recurrent units - wydbyxr的博客 - CSDN博客
2018年11月28日 10:10:47[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：27
个人分类：[深度学习论文阅读																[RNN](https://blog.csdn.net/wydbyxr/article/category/8286658)](https://blog.csdn.net/wydbyxr/article/category/7151097)
# Improving speech recognition by revising gated recurrent units
  通过修改门控循环单元改善语音识别。
## 一般的 GRU 架构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128100945959.png)
## 本论文提出的M-reluGRU
  移除重置门并且使用 ReLU 激活函数替代 tanh 后，我们可以得到新的公式：我们将这个架构称为 M-reluGRU。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128101030450.png)
