
# TensorFlow教程02：MNIST实验——Softmax回归 - lyc_yongcai的博客 - CSDN博客


2017年06月14日 21:22:11[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：259个人分类：[TensorFlow																](https://blog.csdn.net/lyc_yongcai/article/category/6970987)[深度学习																](https://blog.csdn.net/lyc_yongcai/article/category/6970795)[
							](https://blog.csdn.net/lyc_yongcai/article/category/6970987)


**首先声明，这个教程的目标读者是****[机器学习](http://lib.csdn.net/base/machinelearning)****和TensorFlow的新手。如果你熟悉MNIST和Softmax回归，有另外一篇快速教程你可以学习。开始学习这篇教程前，请确认你已正确安装TensorFlow,。**
我们知道MNIST中的每幅图像都是一个数字，不论它是0还是9。我们想在看到一幅图像时，能给出它是每一个数字的概率。比如，我们的明星在看到9的图像时，会得出它有80%的可能性是9、5%的可能性是8、以及是其它数字的更小的可能性。
在这样一个经典的例子上，Softmax回归是一种简单自然的模型。如果你想在多分类问题上指定某个类别的概率值，Softmax比较合适。即使后面我们要去训练更复杂的模型，最后一层依然是Softmax。
Softmax回归有两步：第一步我们在某一类别上计算输出值，然后我们将输出值转成概率。
给定一幅图像和一个类别，这个输出值是每个像素点亮度值的加权和。权值越正表明是该类别的可能性越大，权值越负表明不是该类别的可能性越大。
下面这个图表明了一个模型学习到的每个类别的权值。红色表明负方向，蓝色表明正方向。
![softmax-weights](https://img-blog.csdn.net/20160503133437703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们同时还在这个输出值上加上额外的Bias，这个Bias可以认为是和输入无关的一个数值。预测概率是基于此输出值计算Softmax函数值得到的。
这里Softmax可以认为是一个激活或连接函数，用途是把线性输出调整到我们希望的范围上——比如这个例子中，是在10个类别上的分布概率。
这样理解Softmax可能非常有帮助：先指数化输入，然后再归一化。指数化意味着输入维数增加时，对最终为乘性贡献；同样，减少维数时，对最终是除数贡献。我们规定权重不为0或负数。这样，Softmax的输出和为1，构成了一个有效的概率分布。
可以用下图来理解Softmax，对于每一个输出来说，计算输入xs的加权和，加上Bias，然后应用Softmax。
![softmax-regression-scalargraph](https://img-blog.csdn.net/20160503133541734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用公式可以写成下述形式：
![softmax-regression-scalarequation](https://img-blog.csdn.net/20160503133624472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们可以矢量化这个过程，转成矩阵乘和矢量加，这更有利于高效计算。
![softmax-regression-vectorequation](https://img-blog.csdn.net/20160503133657782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
更紧凑的话，可以写成：y={softmax}(Wx+b)


