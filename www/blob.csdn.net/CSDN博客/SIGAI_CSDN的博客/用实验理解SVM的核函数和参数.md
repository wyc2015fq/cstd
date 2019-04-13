
# 用实验理解SVM的核函数和参数 - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2018年06月14日 16:02:58[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：4591


原创声明：本文为SIGAI原创文章，仅供个人学习使用，未经允许，不能用于商业目的。
欢迎搜索关注微信公众号SIGAICN，获取更多原创干货
![](https://img-blog.csdn.net/20180614150316372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 导言
支持向量机（SVM）在很多分类问题上曾经取得了当时最好的性能，使用非线性核的支持向量机可以处理线性不可分的问题。仅仅通过一个简单的核函数映射，就能达到如此好的效果，这让我们觉得有些不可思议。核函数过于抽象，在本文中，SIGAI将通过一组实验来演示核函数的有效性，并解释支持向量机各个参数所起到的作用，让大家对此有一个直观的认识。如果要了解SVM的理论，请阅读我们之前的公众号文章“用一张图理解SVM的脉络”。

### 核映射与核函数
通过核函数，支持向量机可以将特征向量映射到更高维的空间中，使得原本线性不可分的数据在映射之后的空间中变得线性可分。假设原始向量为x，映射之后的向量为z，这个映射为：
![](https://img-blog.csdn.net/20180614155304588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在实现时不需要直接对特征向量做这个映射，而是用核函数对两个特征向量的内积进行变换，这样做等价于先对向量进行映射然后再做内积：
![](https://img-blog.csdn.net/20180614155317411?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在这里K为核函数。常用的非线性核函数有多项式核，高斯核（也叫径向基函数核，RBF）。下表列出了各种核函数的计算公式：
各种核函数与它们的计算公式
|核函数
|计算公式
|
|线性核

|
|多项式核

|
|径向基函数核|/|高斯核

|
|sigmoid|核

|
其中![](https://img-blog.csdn.net/20180614160046118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)，b，d为人工设置的参数，d是一个正整数，![](https://img-blog.csdn.net/20180614160046118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)为正实数，b为非负实数。
使用核函数后，支持向量机在训练时求解的对偶问题为：
![](https://img-blog.csdn.net/20180614155338183?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180614155428747?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中sgn为符号函数，定义为：
![](https://img-blog.csdn.net/20180614155450241?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 实验
支持向量机真的能对非线性的数据进行分类吗？不同的训练参数会对分类结果产生什么样的影响？下面我们用一个小实验来验证。在这里，我们对二维平面上512x512像素的图像中的所有点进行分类，有蓝色和红色两类。先用一批样本训练一个模型，然后对平面内所有的点进行分类，分类结果的颜色和训练样本的颜色相同。
首先来看支持向量机能否处理异或问题，这是人工智能里一个很经典的分类问题，两类训练样本分别落在两个对角线上：
![](https://img-blog.csdn.net/20180614155524277?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
显然，用一条直线无论怎样划分都无法将这两类样本正确的分开。下面来看SVM的表现，我们使用线性核，多项式核，高斯核三种核函数进行训练。
首先选用线性核，结果是这样的：
![](https://img-blog.csdn.net/20180614155532755?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所有样本都被判定成红色。不出所料，使用线性核的SVM是无法解决异或问题的。
接下来选用多项式核。首先将参数设置为：
![](https://img-blog.csdn.net/20180614155555461?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
分类效果非常差：
![](https://img-blog.csdn.net/20180614155605395?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
蓝色的样本只有少数被分对了。下面调整训练参数：
![](https://img-blog.csdn.net/20180614155621347?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里只加大了惩罚因子C的子，分类效果如下：
![](https://img-blog.csdn.net/20180614155628853?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这比之前好了，蓝色的样本有一半被分对。接着调整参数：
![](https://img-blog.csdn.net/20180614155650415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
分类效果如下：
![](https://img-blog.csdn.net/20180614155705590?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
现在是见证奇迹的时刻！所有训练样本都被正确分类，看来加大C的值非常有效。
下面来看高斯核的表现，如果参数设置为：
![](https://img-blog.csdn.net/20180614155729492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
分类效果也是非常差：
![](https://img-blog.csdn.net/20180614155736475?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所有的点都被分成了红色。下面加大惩罚因子的值：
![](https://img-blog.csdn.net/20180614155751638?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
大部分训练样本都可以正确分类：
![](https://img-blog.csdn.net/2018061415575914?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
进一步加大C的值：
![](https://img-blog.csdn.net/20180614155853325?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018061415590192?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
效果比刚才更好，所有样本基本上都被正确分类了。
继续调整，加大C的值：
![](https://img-blog.csdn.net/2018061415592395?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180614155932600?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所有样本都被正确分类。
如果我们只加大![](https://img-blog.csdn.net/20180614160046118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的值，也能达到很好的效果：
![](https://img-blog.csdn.net/20180614155951955?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180614155958945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所有样本同样被正确分类。

结论
通过上面的实验我们发现使用多项式核、高斯核的SVM确实是可以解决线性不可分问题的。不同的参数对精度的影响非常大，一般来说，C越大，训练得到的模型越准确。如果采用高斯核，参数![](https://img-blog.csdn.net/20180614160046118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的值对精度影响也非常大。因此，在实际应用时调一组好的参数对使用效果非常重要！

推荐文章
[1]机器学习-波澜壮阔40年SIGAI 2018.4.13.
[2]学好机器学习需要哪些数学知识？SIGAI 2018.4.17.
[3]人脸识别算法演化史SIGAI 2018.4.20.
[4]基于深度学习的目标检测算法综述SIGAI 2018.4.24.
[5]卷积神经网络为什么能够称霸计算机视觉领域？SIGAI 2018.4.26.
[6]用一张图理解SVM的脉络SIGAI 2018.4.28.
[7]人脸检测算法综述SIGAI 2018.5.3.
[8]理解神经网络的激活函数SIGAI 2018.5.5.
[9]深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读SIGAI 2018.5.8.
[10]理解梯度下降法SIGAI 2018.5.11.
[11]循环神经网络综述—语音识别与自然语言处理的利器SIGAI 2018.5.15
[12]理解凸优化SIGAI 2018.5.18.

原创声明
本文为SIGAI原创文章，仅供个人学习使用，未经允许，不能用于商业目的。
![](https://img-blog.csdn.net/20180614150316372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


