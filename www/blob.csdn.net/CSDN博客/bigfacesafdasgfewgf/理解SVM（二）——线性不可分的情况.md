# 理解SVM（二）——线性不可分的情况 - bigfacesafdasgfewgf - CSDN博客





2014年11月20日 11:06:28[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：19831标签：[svm																[kernel																[分类																[线性不可分																[维数](https://so.csdn.net/so/search/s.do?q=维数&t=blog)
个人分类：[Machine Learning&&Data Mining](https://blog.csdn.net/puqutogether/article/category/2254295)





# 理解SVM（二）——线性不可分的情况

    前面一篇介绍了SVM的基本原理和代码实现。其中提到了原始的SVM对数据对好要线性可分的假设太严格了，很多数据在一定程度上都不是线性可分的。那么这篇我们就要好好说说SVM对于线性不可分的情况是怎么处理的。

## 1. 什么是线性不可分

    线性不可分简单来说就是你一个数据集不可以通过一个线性分类器（直线、平面）来实现分类。这样子的数据集在实际应用中是很常见的，例如：人脸图像、文本文档等。下面的几个数据都是线性不可分的：

![](https://img-blog.csdn.net/20141120112836090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20141120111908702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





我们不可以使用一个直线或者一个直面把上面图像中的两类数据很好的划分。这就是线性不可分。

## 2. 解决线性不可分的思想

    现在流行的解决线性不可分的方法就是使用核函数（kernel）。这个技术加在任何分类算法上几乎都可以提高一些效果，不过也给程序带来了一些不便（例如多了模型的选择和参数的调节）。

    核函数解决线性不可分的本质思想就是把原始的样本通过核函数映射到高维空间中，让样本在高维特征空间中是线性可分的，然后再使用常见的线性分类器，如SVM就可以很好的分类了。

    那么什么是“通过核函数映射到高维空间”呢？针对上面两个线性不可分的数据例子我们说一下各自的映射过程，如下图：

![](https://img-blog.csdn.net/20141120112813954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


说明：图中开口向上的二次抛物线就是核函数，正类样本（红色）映射到该抛物线中得到的函数值都是小于0的，负类样本（蓝色）映射得到的函数值都是大于0的，这个时候就线性可分了。那么为什么说映射之后就是高维空间了，解释如下：

（

上述核函数是抛物线，函数模型为![](https://img-blog.csdn.net/20141120113231027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，我们需要通过g(x)=cy的向量乘的形式，这个时候得到的g(x)特征就是4维的，即为高维特征空间。

）

下面是另外一个例子的映射到高维空间的图示。




![](https://img-blog.csdn.net/20141120113531625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





## 3. 核函数

    首先，我们要联系到上一篇中，说道通过KT条件和对偶问题的转化，我们把原始的SVM目标函数转化成了下面的形式：

![](https://img-blog.csdn.net/20141120151141828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


可以发现其中要计算两个样本特征向量之间的内积。核函数就是从这里入手！

    根据这个切入点，有两种思路：

1）将输入的空间中样本映射到新的空间中，在新的空间中求内积；

2）找到一个核函数，它不需要将输入空间中的样本映射到新的空间中去，在输入空间中就可以直接计算出内积了。




    我们先看第一种，一个二维非线性空间要展开变成线性的，就会成为5维的，然后问题就变得线性可分。这种转化带来的问题就是维度变高了，后续的计算会变得非常复杂。对于学习期而言就是特征空间维数可能最终无法计算，而他的泛化能力会随着位数的增长而大大降低，最终可能会使得内积无法求出，后果也就失去了这种转化的优势。




    第二种方法，它其实是对输入空间向高维空间的一种隐式映射（注意，这也是低维空间到高维空间的一种映射）。他不需要给出那个映射，而在输入空间中就可以计算内积，这就是我们这节要讲的核函数方法。




    核函数K，对于所有的x1,x2满足：![](https://img-blog.csdn.net/20141120153828453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。




    再说的直接一点，核函数可以简化映射空间中的内积运算，而我们的SVM算法中最需要计算的地方恰恰就是特征向量之间的内积算法，这也是我们刚才说的切入点。

![](https://img-blog.csdn.net/20141120155006109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    引入了核函数之后，我们的目标函数就成为了：

![](https://img-blog.csdn.net/20141120155242437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中的内积直接替换成了核函数。**其实注意观察这两个方式就可以发现：显式映射就是先映射到高维空间，再求内积，此时往往由于过于高维，使得无法求得内积；隐式映射是先在核函数中求内积，然后再到高维，这个时候内积就是在输入空间中求的，不怕维数灾难了。**




## 4. 常见的核函数和如何选择核函数

多项式核函数：

![](https://img-blog.csdn.net/20141120160306890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    线性核函数：

![](https://img-blog.csdn.net/20141120160434084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    高斯核函数等。

![](https://img-blog.csdn.net/20141120160515190?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)](https://so.csdn.net/so/search/s.do?q=线性不可分&t=blog)](https://so.csdn.net/so/search/s.do?q=分类&t=blog)](https://so.csdn.net/so/search/s.do?q=kernel&t=blog)](https://so.csdn.net/so/search/s.do?q=svm&t=blog)




