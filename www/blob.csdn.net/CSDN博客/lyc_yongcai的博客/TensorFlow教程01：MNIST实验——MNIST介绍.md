
# TensorFlow教程01：MNIST实验——MNIST介绍 - lyc_yongcai的博客 - CSDN博客


2017年06月14日 21:16:47[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：446



在我们学习任何一门编程语言的时候，我们做的第一件事情就是写一个“Hello World！”程序；机器学习的“Hello World！”就是MNIST。MNIST是一个简单的[计算机视觉](http://lib.csdn.net/base/computervison)数据集，它由下述的手写阿拉伯数字图像构成：
![MNIST](https://img-blog.csdn.net/20160502230009028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
MNIST同时也包括这些图像的标注，比如上述图像的标注分别是5，0，4和1。
在这篇教程中，我们会训练一个模型去预测图像是什么数字。我们当前的目标并不是精心训练一个业内领先的模型，而是让你先简单了解一下TensorFlow；但我们后面会为你提供如何训练一个业内领先模型的代码！首先，我们从一个非常简单的、叫做Softmax回归的模型开始。
这个教程的实际代码非常短，所有有趣的内容3行左右就够了。然而，对于我们最重要的是理解这些代码背后的含义：TensorFlow和核心机器学习理念都是如何工作的？所以，下面我们会非常仔细的讲解这些代码。
在Yann Lecun的网站上可找到MNIST数据。为方便起见，我们放置了自动下载并安装MNIST数据的[Python](http://lib.csdn.net/base/python)代码。你可以象下面一样下载并import这些代码，或者简单的剪刀加浆糊。
**[python]**[
view plain](#)[
copy](#)
[print](#)[?](#)
fromtensorflow.examples.tutorials.mnistimport input_data
mnist =input_data.read_data_sets("MNIST_data/", one_hot=True)

```python
from tensorflow.examples.tutorials.mnistimport input_data
mnist =input_data.read_data_sets("MNIST_data/", one_hot=True)
```
下载的数据可分成三部分：55000份训练数据(mnist.train)、10000份测试数据(mnist.test)和5000份开发数据(mnist.validation)。这个划分非常重要：我们已经分离出不用来训练的数据，这样可以确保我们的学习具有实际的推广性能。之前提到过，每份MNIST数据包含手写数字图像(后记为xs)和对应标注(后记为ys)2部分。训练集和[测试](http://lib.csdn.net/base/softwaretest)集均包含xs和ys，所以训练图像是mnist.train.images、训练标注是mnist.train.labels.
每份图像宽和高均为28像素点，我们可以理解为一个大数组。
![MNIST-Matrix](https://img-blog.csdn.net/20160502230107467?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们可以把这个数字平铺为一个28x28 = 784个点的矢量，只要我们保持一致，怎么平铺都无所谓。从这个角度来说，这些MNIST图像仅仅是784维矢量空间中的一些有丰富结构的点。
平铺这些数据导致了我们丢失了图像二维结构信息，这样做有什么坏处呢？好吧，后面会介绍的最好的计算机视觉方法会使用这个二维结构信息，但这儿用的Softmax回归方法不会使用。
结果就是mnist.train.images是一个形状为[55000, 784]的张量(Tensor)，第一维是图像数目、第二维是每幅图像的像素点。Tensor里的每个值对应某幅图像的某个像素点的亮度值，介于0和1之间。
![mnist-train-xs](https://img-blog.csdn.net/20160502230205453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
MNIST对应的标注是0到9的数字。我们这份教程中，会把标注设定为One-hot矢量，One-hot矢量的特点是绝大多数维的值都是0、只有某一维的值是1。这意味着数值是几，第几维就是1，如标注3对应的是[0,0,0,1,0,0,0,0,0,0]。这样，mnist.train.labels是个[55000, 10]的浮点数组。
![mnist-train-ys](https://img-blog.csdn.net/20160502230236031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
现在我们准备好实际构建我们的模型啦！


