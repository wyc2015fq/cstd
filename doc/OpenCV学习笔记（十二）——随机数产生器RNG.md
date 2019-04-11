# OpenCV学习笔记（十二）——随机数产生器RNG

2011年11月03日 11:36:16

yang_xian521

阅读数：45649

 								标签： 																[compiler																](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[float																](https://so.csdn.net/so/search/s.do?q=float&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog) 							更多

 								个人分类： 																[OpenCV																](https://blog.csdn.net/yang_xian521/article/category/910716) 							

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

用OpenCV做算法的朋友们肯定为随机数烦恼过，新版本一直支持随机数产生器啦，而且还继续支持之前版本的c格式的函数，不过与时俱进，我这里介绍C++的RNG类。它可以压缩一个64位的i整数并可以得到scalar和array的随机数。目前的版本支持均匀分布随机数和Gaussian分布随机数。随机数的产生采用的是Multiply-With-Carry算法和Ziggurat算法。

其构造函数的初始化可以传入一个64位的整型参数作为随机数产生器的初值。next可以取出下一个随机数，uniform函数可以返回指定范围的随机数，gaussian函数返回一个高斯随机数，fill则用随机数填充矩阵。

这里介绍一个uniform的使用事项，就是比如利用它产生0~1的随机数的问题，具体代码如下：



```cpp
RNG rng;
// always produces 0
double a = rng.uniform(0, 1);
// produces double from [0, 1)
double a1 = rng.uniform((double)0, (double)1);
// produces float from [0, 1)
double b = rng.uniform(0.f, 1.f);
// produces double from [0, 1)
double c = rng.uniform(0., 1.);
// may cause compiler error because of ambiguity:
// RNG::uniform(0, (int)0.999999)? or RNG::uniform((double)0, 0.99999)?
double d = rng.uniform(0, 0.999999);
```

就是不能写成rng.uniform( 0 , 1)，因为输入为int型参数，会调用uniform（int，int），只能产生0。请大家注意使用^_^

还有一些随机数相关的函数，比如randu可以产生一个均匀分布的随机数或者矩阵，randn可以产生一个正态分布的随机数，randShuffle可以随机打乱矩阵元素

再简单介绍一下c版本的随机数产生器的相关函数，有cvRNG、cvRandArr、cvRandInt、cvRandReal