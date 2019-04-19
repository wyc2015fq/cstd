# Make Your Own Neural Network（十五）-----权重更新实例 - Popeye_the_Sailor - CSDN博客
2018年05月06日 11:10:03[_Sailor_](https://me.csdn.net/lz0499)阅读数：607
**Make Your Own Neural Network**
**构建你自己的神经网络**
[https://blog.csdn.net/lz0499](https://blog.csdn.net/lz0499)
**作者：lz0499**
**声明：**
1）**Make Your Own Neural Network**翻译自[Tariq Rashid](https://book.douban.com/search/Tariq%20Rashid)编写的神经网络入门书籍。作者的目的是尽可能的少用术语和高深的数学知识，以图文并茂的方式讲解神经网络是如何工作的。任何拥有高中数学水平的人就能够理解神经网络的工作方式。强烈推荐初学者以这本书作为神经网络入门书籍。
2）本文仅供学术交流，非商用。翻译的初衷是一边翻译一边加深对神经网络的理解。
3）由于刚刚接触神经网络这方面的知识，翻译过程中难免有些错误。若发现错误，还请各位前辈指正。谢谢！
4）由于工作原因，我将有选择的按照原文的章节不定期的进行翻译更新。
5）此属于第一版本，若有错误，还需继续修正与增删。
目录：
第一部分：神经网络是如何工作的
[一种简单的预测机](https://mp.csdn.net/postedit/80069089)
[分类即是预测](https://blog.csdn.net/lz0499/article/details/80072948)
[训练一个简单的分类器](https://blog.csdn.net/lz0499/article/details/80086402)
[单个分类器似乎远不够](https://blog.csdn.net/lz0499/article/details/80099968)
[神经元，自然界的计算机](https://blog.csdn.net/lz0499/article/details/80138584)
[通过神经网络的信号](https://blog.csdn.net/lz0499/article/details/80138955)
[矩阵很有用](https://blog.csdn.net/lz0499/article/details/80160354)
[利用矩阵计算三层神经网络的输出结果](https://blog.csdn.net/lz0499/article/details/80160449)
[从多个节点更新权重](https://blog.csdn.net/lz0499/article/details/80172534)
[从多个节点中反向传递误差](https://blog.csdn.net/lz0499/article/details/80172568)
[多层神经网络层反向传输误差](https://blog.csdn.net/lz0499/article/details/80185692)
[利用矩阵乘法计算反向传输误差](https://blog.csdn.net/lz0499/article/details/80185923)
[实际上是如何更新权重（一）](https://blog.csdn.net/lz0499/article/details/80209928)
[实际上是如何更新权重（二）](https://blog.csdn.net/lz0499/article/details/80210590)
[权重更新实例](https://blog.csdn.net/lz0499/article/details/80212695)
**权重更新实例**
让我们实际计算下一个简单的神经网络中权重是如何更新的。
下图是我们之前遇到的一个神经网络。但是这次我们添加了每一个隐藏层的输出结果。这些输出结果只是为了演示如何更新权重而设置的，实际中并不一定是这个值。
![](https://img-blog.csdn.net/20180506110859107)
我们想更新隐藏层和输出之间的权重W1,1。W1,1当前的权重值为2.0.
让我们再一次写出误差斜率表达式
![](https://img-blog.csdn.net/20180506110930706)
我们一步一步开始计算：
l  第一部分tk-ok是误差e1=0.8
l  Sigmoid函数中的加权和为2.0*0.4+3.0*0.5=2.3
l  把2.3带入Sigmoid函数得到0.909.中间表达式为0.909*（1-0.909）=0.083
l  最后一部分Oj即使j=1的隐藏层输出，即为oj=0.4
把上述所有部分相乘，不要忘记前面的负号。我们将得到最后的结果为-0.0265。如果我们设置学习率为0.1，则我们需要改变权重W1，1 -（0.1*-0.0265）=0.002650大小，即W11=2.0+0.002650=2.00265。
这个改变值很小，但是经过上千次甚至上万次迭代之后，权重值将固定在某一个数值，表示的是神经网络已经训练好了。
