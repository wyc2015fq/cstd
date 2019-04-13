
# CNN (Convolutional Neural Networks) Abstract - Wesley's Blog - CSDN博客


2018年05月07日 11:50:51[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：29标签：[深度学习																](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)[CNN																](https://so.csdn.net/so/search/s.do?q=CNN&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)个人分类：[深度学习																](https://blog.csdn.net/yinanmo5569/article/category/7645387)


CNN通常用来进行图像处理，经历一系列卷积层,非线性层,池化层和完全连接层,最终得到输出，输出通常是一个单独分类或者一组分类的概率.
![这里写图片描述](https://img-blog.csdn.net/20180507114707333?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## Fully Connected Layer & Convolution Layer & Pooling Layer & Rectified Linear Unit
卷积层与全连接层的主要区别是卷积层保留了基本的空间结构(Spatial Structure)
### 全连接层(FC Layer)
Fully Connected
![](https://img-blog.csdn.net/20180507114824107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 卷积层(Conv)
Conv通过filter输出一个激活映射(activation map) Conv经历相较全连接来说主要有以下几个特点:
局部连接
保留空间结构
参数共享 (filter)
![这里写图片描述](https://img-blog.csdn.net/20180507114910303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180507114910303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180507114932202?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[

](https://img-blog.csdn.net/20180507114910303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)池化层(POOL)
[
](https://img-blog.csdn.net/20180507114910303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)同时也被叫做downsampling层．最受欢迎的就是最大池化层(max-pooling,如图).
1. 减小输入卷空间维度，也可以控制过拟合
2. 对于每个激活层进行单独处理
![这里写图片描述](https://img-blog.csdn.net/20180507115002931?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
线性整流函数(ReLU)
在每个卷积层之后，通常会立即应用一个非线性层（或激活层）。其目的是给一个在卷积层中刚经过线性计算操作（只是数组元素依次（element wise）相乘与求和）的系统引入非线性特征。通常意义下，线性整流函数指代数学中的斜坡函数，即
f(x)=max(0,x)
f(x) = max(0, x)
CNN Typical architectures
![这里写图片描述](https://img-blog.csdn.net/20180507115017341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

