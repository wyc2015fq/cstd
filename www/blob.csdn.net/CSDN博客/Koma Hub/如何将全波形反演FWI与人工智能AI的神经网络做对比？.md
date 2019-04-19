# 如何将全波形反演FWI与人工智能AI的神经网络做对比？ - Koma Hub - CSDN博客
2018年09月15日 11:30:05[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：684
**目录**
[1.全波形反演](#1.%E5%85%A8%E6%B3%A2%E5%BD%A2%E5%8F%8D%E6%BC%94)
[2.神经网络](#2.%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C)
全波形反演可以说现在被地球物理界的小伙伴做烂了，一提到全波形反演大家先到的就是高精度...各种优点，可是他却有着与人工智能的同样的缺陷，那就是计算量的限制。但是在当下，随着计算技术的不但进步，虽然有人说摩尔定律已死，但是现在各种超级计算机横空出世，我们国家的曙光、天河等位居世界前列，相信，为了得到高精度的结果，这点计算花费是值得的，今天就来简单对地球物理的全波形反演（Full Waveform Inversion）和神经网络（Neural Network）进行简单的对比。
# **1.全波形反演**
总所周知，全波形反演其实就是数学方法的堆叠（其实大部分的科学研究最终都要归功于数学，但是为什么没有诺贝尔数学奖呢？这个故事留给大家自己搜索）。全波形反演的流程其实非常简单：建立数据和模型的非线性关系是要点。假如数据为D，模型为M，他们之间的关系是A，那么简单的关系就是：
![AM=D](https://private.codecogs.com/gif.latex?AM%3DD)
A说白了就是人类一直研究的自然规律（突然想起一句话：世间万物都是由分子构成“来自《大灌篮》”），地球物理学的常用规律是动力学和运动学理论，前者是波动理论，后者是旅行时理论，同时，当参数出现误差时候，方程就转变成了：
![A\Delta M=\Delta D](https://private.codecogs.com/gif.latex?A%5CDelta%20M%3D%5CDelta%20D)
这里的![\Delta D](https://private.codecogs.com/gif.latex?%5CDelta%20D)就是数据误差，其实就是波形反演中观测数据与模拟数据之间的误差，![\Delta M](https://private.codecogs.com/gif.latex?%5CDelta%20M)就是模型误差，就是我们需要更新的模型参数更新量。通常研究过程中加入修正量，叫做bias（偏置）：
![A\Delta M = \Delta D + bias](https://private.codecogs.com/gif.latex?A%5CDelta%20M%20%3D%20%5CDelta%20D%20&plus;%20bias)
这个变量的引入通常会产生一定的修正作用，我们常说到的井约束等信息等。
![](https://img-blog.csdn.net/20180915111146813?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
人类法则是人类从自然法则中获取的一个无线接近于自然法则的定律。
# **2.神经网络**
神经网络的原理大同小异，但是他的区别在于“人类法则”未知，需要通过大量的数据进行训练才能获取的一种规律。
![y = ax + b](https://private.codecogs.com/gif.latex?y%20%3D%20ax%20&plus;%20b)
将其写成神经网络的通用符形式为：
![y = wx + bias](https://private.codecogs.com/gif.latex?y%20%3D%20wx%20&plus;%20bias)
其中![w](https://private.codecogs.com/gif.latex?w)为权重，![bias](https://private.codecogs.com/gif.latex?bias)为偏置。
如何建立这个“人类法则”的初始状态呢？这里就引入了一个叫做sigmoid函数和softplus函数的随机变量，用这个随机变量建立初始权重和偏置。
![](https://img-blog.csdn.net/20180915112824760?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
给出一个GitHub开源项目中的sigmoid函数的例子，项目名称mnist-neural-network，请大家自行搜索：
```cpp
/*返回一个 正态分布的随机数*/
double
random_normal_distribution (void)
{
    return sqrt (-2.0 * log (drand ())) * cos (2.0 * M_PI * drand ());
}
/*返回一个此范围内的伪随机数*/
int
random_integer (int min, int max)
{
    return min + rand () / (RAND_MAX / (max - min + 1) + 1);
}
/*sigmoid函数*/
double
sigmoid_to_number (double number)
{
/*
a    sigmoid = 0     i=-9999...99
b            = 0.5   i=0
c            = 1     i=9999...99
*/
    /* exp (double) returns a "double" value, so casting is not required */
    return 1.0 / (1.0 + exp (-number));
}
/*sigmoid的导数*/
double
sigmoid_prime_to_number (double number)
{
    /* Avoid to call same function 2 times */
    double sigmoid_number = sigmoid_to_number (number);
    return sigmoid_number * (1.0 - sigmoid_number);
}
```
建立完整的初始状态后，就可以根据最速下降法或者其他一些数学方法计算梯度（权重）和修正量（偏置）了，这里需要一系列的神经网络划分，不同的神经网络对于训练结果也会有不同的结果。给出一个简单的神经网络实例：
![](https://img-blog.csdn.net/20180915112620435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个例子是手写数字识别的网络划分，每个手写字体有28*28=784个像素，最终的输出只有0~9数字。
