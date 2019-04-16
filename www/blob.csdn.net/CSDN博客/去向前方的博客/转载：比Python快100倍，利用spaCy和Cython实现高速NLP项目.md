# 转载：比Python快100倍，利用spaCy和Cython实现高速NLP项目 - 去向前方的博客 - CSDN博客





2018年07月15日 17:58:39[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：302








Cython 是一个工具包，可以使你在 Python 中编译 C 语言，这就是为什么 numpy 和 pandas 很快的原因，Cython 就是 Python 的超集。在本文中，作者将为我们介绍他的 GitHub 项目 NeuralCoref v3.0，详解如何利用 spaCy 和 Cython 以约 100 倍于 Python 的速度实现 NLP 项目。

![这里写图片描述](https://img-blog.csdn.net/20180715175610400?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

相关 Jupyter Notebook 地址：[https://github.com/huggingface/100-times-faster-nlp](https://github.com/huggingface/100-times-faster-nlp)

去年我们发布 Python 包 coreference resolution package 后，我们收到了来自社区的精彩反馈，并且人们开始在很多应用中使用它，其中一些与我们原来的对话用例迥异。

我们发现，尽管对话信息的处理速度非常好，但对于长的新闻文章来说，处理速度可能会非常慢。

我决定详细研究这一问题，最终成果即 NeuralCoref v3.0，它在相同准确率的情况下比老版本快 100 倍左右（每秒几千字），同时兼顾 Python 库的易用性和兼容性。

NeuralCoref v3.0 ：[https://github.com/huggingface/neuralcoref/](https://github.com/huggingface/neuralcoref/)

我想在这篇文章中分享一些关于这个项目的经验，特别是：

如何用 Python 设计一个高速模块；

如何利用 spaCy 的内部数据结构来有效地设计超高速 NLP 函数。

所以我在这里有点作弊，因为我们会谈论 Python，但也谈论一些 Cython 的神奇作用。但是，你知道吗？Cython 是 Python 的超集，所以不要让它吓跑你！

你现在的 Python 程序已经是 Cython 程序。

有几种情况下你可能需要加速，例如：

你正在使用 Python 开发一个 NLP 的生产模块；

你正在使用 Python 计算分析大型 NLP 数据集；

你正在为深度学习框架，如 PyTorch / TensorFlow，预处理大型训练集，或者你的深度学习批处理加载器中的处理逻辑过于繁重，这会降低训练速度。

再强调一遍：我同步发布了一个 Jupyter Notebook，其中包含我在本文中讨论的例子。试试看！

Jupyter Notebook：[https://github.com/huggingface/100-times-faster-nlp](https://github.com/huggingface/100-times-faster-nlp)

加速第一步：剖析 
![这里写图片描述](https://img-blog.csdn.net/20180715175628904?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
首先要知道的是，你的大多数代码在纯 Python 环境中可能运行的不错，但是如果你多用点心，其中一些瓶颈函数可能让你的代码快上几个数量级。

因此，你首先应该分析你的 Python 代码并找出瓶颈部分的位置。使用如下的 cProfile 是一种选择：

import cProfile 

 import pstats 

 import myslowmodule 

 cProfile.run(‘myslowmodule.run()’, ‘restats’) 

 p = pstats.Stats(‘restats’) 

 p.sortstats(‘cumulative’).printstats(30)
如果你使用神经网络，你可能会发现瓶颈部分是几个循环，并且涉及 Numpy 数组操作。

那么，我们如何加速这些循环代码？

在 Python 中使用一些 Cython 加速循环

![这里写图片描述](https://img-blog.csdn.net/20180715175651775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

让我们用一个简单的例子来分析这个问题。假设我们有一大堆矩形，并将它们存储进一个 Python 对象列表，例如 Rectangle 类的实例。我们的模块的主要工作是迭代这个列表，以便计算有多少矩形的面积大于特定的阈值。

我们的 Python 模块非常简单，如下所示：

from random import random

class Rectangle: 

 def **init**(self, w, h): 

 self.w = w 

 self.h = h 

 def area(self): 

 return self.w * self.h
def check_rectangles(rectangles, threshold): 

 n_out = 0 

 for rectangle in rectangles: 

 if rectangle.area() > threshold: 

 n_out += 1 

 return n_out
def main(): 

 n_rectangles = 10000000 

 rectangles = list(Rectangle(random(), random()) for i in range(n_rectangles)) 

 n_out = check_rectangles(rectangles, threshold=0.25) 

 print(n_out)
check_rectangles 函数是瓶颈部分！它对大量的 Python 对象进行循环，这可能会很慢，因为 Python 解释器在每次迭代时都会做大量工作（寻找类中的求面积方法、打包和解包参数、调用 Python API …）。

Cython 将帮助我们加速循环。

Cython 语言是 Python 的超集，它包含两种对象：

Python 对象是我们在常规 Python 中操作的对象，如数字、字符串、列表、类实例…

Cython C 对象是 C 或 C ++ 对象，比如 double、int、float、struct、vectors。这些可以由 Cython 在超快速的底层代码中编译。

快速循环只是 Cython 程序（只能访问 Cython C 对象）中的一个循环。

设计这样一个循环的直接方法是定义 C 结构，它将包含我们在计算过程中需要的所有要素：在我们的例子中，就是矩形的长度和宽度。

然后，我们可以将矩形列表存储在这种结构的 C 数组中，并将这个数组传递给我们的 check_rectangle 函数。此函数现在接受一个 C 数组作为输入，因此通过 cdef 关键字而不是 def 将其定义为 Cython 函数（请注意，cdef 也用于定义 Cython C 对象）。

下面是我们的 Python 模块的快速 Cython 版：

from cymem.cymem cimport Pool 

from random import random

cdef struct Rectangle: 

 float w 

 float h
cdef int check_rectangles(Rectangle* rectangles, int n_rectangles, float threshold): 

 cdef int n_out = 0 

 # C arrays contain no size information => we need to give it explicitly 

 for rectangle in rectangles[:n_rectangles]: 

 if rectangles[i].w * rectangles[i].h > threshold: 

 n_out += 1 

 return n_out
def main(): 

 cdef: 

 int n_rectangles = 10000000 

 float threshold = 0.25 

 Pool mem = Pool() 

 Rectangle* rectangles = 










