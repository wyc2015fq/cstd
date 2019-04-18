# 其他加速方案--Learning Structured Sparsity in Deep Neural Networks - wydbyxr的博客 - CSDN博客
2018年11月27日 10:17:24[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：64
# Learning Structured Sparsity in Deep Neural Networks
  亮点：如何把前几年盛行的Structured Sparisity和DNN结合？这篇文章给出了思路。
  这篇文章来自匹兹堡大学的研究人员，核心内容非常清晰，那就是要引入Structured Sparsity到DNN来，使得最终的DNN有一个比较紧凑的表示，加快运算速度，同时能够得到硬件友善的表现形式，让硬件能够比较快地执行。
  在这篇文章里，作者采用了三种方法：
  1）惩罚（Penalizing）不重要的Filter和Channel：把一些Filter和Channel置0；
  2）学习任意形状的Filter：在2D空间上学习通过置0，达到学习任意形状的要求；
  3）缩短DNN的层数：彻底移除整个层，通过增加Shortcut的方法来达到不产生断层的情况。
  文章并没有提供在SSL和DNN相结合的情况下的Learning算法。实验部分非常详实，用LeNet在MNIST、ConvNet和ResNet在CIFAR-10以及AlexNet在ImageNet上都做了实验。总体的感觉是，在很多例子的情况下，更加Sparsity的DNN反而带来了精度的提高。
  虽然之前已经有了一些工作来对DNN进行压缩，但这篇文章的作者认为，这些压缩方法（比如直接使用L1 Regularization），可能会让网络得到一些随机（Random）的链接，使得内存访问变得不常规（Iregular）。在这样的情况下，新的模型，虽然表面上有很大的Sparsity，但是并没有加快运算速度，有时候甚至还有减少。
  另外一种最近的思路，就是最近采用的Low-Rank Approximation方法。简单说来这种方法为先训练DNN，然后每层得到的Tensor被分解，并且被更小的Factor所替代逼近。这种方法的好处是能够达到加速，坏处则是需要最终重新Fine-Tune模型的精度。显然，这篇文章就是要解决上述缺点。文章作者把前几年火热的Structured Sparisty Learning（SSL）和DNN相结合。具体说来，就是采用了Group Lasso的方法来让DNN的参数能够结构化置0。
