# 斯特林公式（Stirling's approximation） - 紫芝的博客 - CSDN博客





2018年09月10日 20:33:21[紫芝](https://me.csdn.net/qq_40507857)阅读数：127








斯特林公式（Stirling's approximation）是一条用来取n的[阶乘](https://baike.baidu.com/item/%E9%98%B6%E4%B9%98)的[近似值](https://baike.baidu.com/item/%E8%BF%91%E4%BC%BC%E5%80%BC)的数学公式。一般来说，当n很大的时候，n阶乘的计算量十分大，所以斯特林公式十分好用，而且，即使在n很小的时候，斯特林公式的取值已经十分准确。

![](https://img-blog.csdn.net/20140807103820328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjMxMzMzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

公式为：   ![n! \approx \sqrt{2\pi n}\, \left(\frac{n}{e}\right)^{n}.](http://upload.wikimedia.org/math/e/6/5/e656c0c9a9bd7e763310012120a00317.png)

 从图中看出，对于足够大的整数n，这两个数互为近似值。更加精确地：   （e = 2.718）

![\lim_{n \rightarrow \infty} {\frac{e^n\, n!}{n^n \sqrt{n}}} = \sqrt{2 \pi}.](http://upload.wikimedia.org/math/6/c/f/6cf8c6b4a0e3df9fde5bd5d2524064ac.png)       或者        ![\lim_{n \rightarrow \infty} {\frac{n!}{\sqrt{2\pi n}\, \left(\frac{n}{e}\right)^{n}}} = 1](http://upload.wikimedia.org/math/5/9/7/597ea5ebaf3e7669de36494cd2554f3e.png)

按一般方法计算N的阶乘，其时间复杂度为O（N）：    N！= 1 * 2 * 3 * 4 * 5 * ............ * N；

 这个公式，以及误差的估计，可以推导如下。我们不直接估计n!，而是考虑它的[自然对数](http://zh.wikipedia.org/wiki/%E8%87%AA%E7%84%B6%E5%AF%B9%E6%95%B0)：

![\ln(n!) = \ln 1 + \ln 2 + \cdots + \ln n.](http://upload.wikimedia.org/math/b/d/e/bde6df6f84382a92e279f5626bed3606.png)

例题： [HDU1018](https://blog.csdn.net/qq_40507857/article/details/82594111)

题目给出的N的范围是: 1<= N <= 107  

用普通方法肯定算不出N的阶乘后的出的数字位数，但运用斯特林公式则很好解决.

Stirling 公式

![](https://img-blog.csdn.net/20150623213226157?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZnV5ZW0=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

即：

![](https://img-blog.csdn.net/20150623213347066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZnV5ZW0=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    Stirling公式的意义在于：当n足够大时，n!计算起来十分困难，虽然有很多关于n!的等式，但并不能很好地对阶乘结果进行估计，尤其是n很大之后，误差将会非常大。但利用Stirling公式可以将阶乘转化成幂函数，使得阶乘的结果得以更好的估计。而且n越大，估计得越准确。



