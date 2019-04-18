# Kernel Method的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年08月14日 10:18:09[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：711
kernel method是针对低维线性不可分而提出的一种解决方法，在PRML中有一章节的介绍，对其理解，也是迭代更进的过程。
简单来说，kernel method是一种低维和高维特征空间映射的方法，利用低维内积的函数来表征高维内积，即高维的内积用低维内积的函数来表示，这个低维内积的函数就是kernel function。
1、首先，说明kernel method的示例？
1）问题提出：
![](https://img-blog.csdn.net/20180814100512568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
将二维线性不可分映射到三维，就可以实现超平面的线性可分。
2）如何映射呢？
![](https://img-blog.csdn.net/20180814100632616?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
利用原空间中的已知信息来计算新空间中的内积，新空间是高维，直接计算内积运算量大，而通过原空间低维的内积函数运算则可避免这一问题，同时又能起到高维线性可分下的内积效果。
3）内积是一种相似性度量（正交的，内积为零），如此，可定义内积函数为：
![](https://img-blog.csdn.net/20180814101219543?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过核函数，可实现：
![](https://img-blog.csdn.net/20180814101251393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、其次，我们说怎么样的内积函数符合核函数要求？
Mercer定理，任何半正定的函数都可以作为核函数。
其论证过程，包括内积矩阵，待进一步学习。
3、目前核函数有哪些？
![](https://img-blog.csdn.net/20180814101550553?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
径向基和高斯核是比较常用的，至于高斯核可以映射到无限维这个推理也有待进一步看相关资料。
4、关于DL和Kernel的思考
DL在高维、非线性上是很好的模型，而kernel渐渐被冷落，其原因是否还在于kernel核函数的普适性以及高维计算的问题，这个困惑有待进一步求索。
图片参考：[https://blog.csdn.net/baimafujinji/article/details/79372911](https://blog.csdn.net/baimafujinji/article/details/79372911)
