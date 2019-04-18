# 拉普拉斯变换中的S是个什么鬼？ - 知乎
# 

> **A good way of thinking of where the Laplace transform comes from, and a way which dispels some of its mystery is by thinking of power series.(一个比较好的关于Laplace变换的解释方法是从幂级数(Power Series)入手。)**
**    — —Arthur Mattuck (原MIT数学系主任)**

    学过控制的都知道拉氏变换(Laplace Transform)，其可以将微分方程转化为代数方程进行运算，使得求解大为简化。

    但你们是不是也有这样的疑问：拉氏变换中的 ![S](https://www.zhihu.com/equation?tex=S) 是怎么来的？皮埃尔-西蒙·拉普拉斯当年为啥就能想出个这样的数学变换公式？


![](https://pic1.zhimg.com/v2-b465888792a8f26241f09f8d2527729c_b.jpg)Pierre-Simon Laplace (1749–1827)图片来源：（Wikipedia）
     我是自从接触拉氏变换就一直有这样的疑问，直到有一天，看了Arthur Mattuck的微分方程才恍然大悟。更有意思的是，导师有一天也问了这样一个看似无厘头的问题，还好当时有所准备。


![](https://pic3.zhimg.com/v2-f6211c3ba35f2969766a5f91e965449e_b.jpg)Arthur Mattuck

    如果学过高等数学，都应该知道：一个**幂级数**可以写为如下形式：

![](https://pic4.zhimg.com/v2-4e6cfdca7a816bd5d581f5f2fd4e4c7b_b.jpg)(1)

    将其展开其实就是： ![A(x) = a_0+a_1x^1+a_2x^2+...+a_nx^n](https://www.zhihu.com/equation?tex=A%28x%29+%3D+a_0%2Ba_1x%5E1%2Ba_2x%5E2%2B...%2Ba_nx%5En)

    如果将其中幂级数的系数 ![a_n](https://www.zhihu.com/equation?tex=a_n) 看成一组离散的函数，则上式 ![(1)](https://www.zhihu.com/equation?tex=%281%29) 也可以写为：

![](https://pic2.zhimg.com/v2-283e5e5e87f73d02766e8a8657bd82e9_b.jpg)(2)

    通过把 ![a(n)](https://www.zhihu.com/equation?tex=a%28n%29) 看作一组关于变量 ![n](https://www.zhihu.com/equation?tex=n) 的离散函数，式 ![(2)](https://www.zhihu.com/equation?tex=%282%29) 相当于描述了函数 ![A(x)](https://www.zhihu.com/equation?tex=A%28x%29) 的构造过程。

    输入是离散函数数列 ![\{a_0, a_1, a_2, ⋯, a_n\}](https://www.zhihu.com/equation?tex=%5C%7Ba_0%2C+a_1%2C+a_2%2C+%E2%8B%AF%2C+a_n%5C%7D) ，输出则是由多项式构成的函数 ![A(x)](https://www.zhihu.com/equation?tex=A%28x%29) 。即，只要输入一个 ![\{a_0, a_1, a_2, ⋯, a_n\}](https://www.zhihu.com/equation?tex=%5C%7Ba_0%2C+a_1%2C+a_2%2C+%E2%8B%AF%2C+a_n%5C%7D) 数列，就可以输出一个函数 ![A(x)](https://www.zhihu.com/equation?tex=A%28x%29) ，其中， ![x](https://www.zhihu.com/equation?tex=x)  是输出函数*A*(*x*)的自变量。

    现在，举一个例子，如果取 ![a(n)=1](https://www.zhihu.com/equation?tex=a%28n%29%3D1) ，即 ![\{a_0=1, a_1=1, a_2=1, ⋯, a_n=1\}](https://www.zhihu.com/equation?tex=%5C%7Ba_0%3D1%2C+a_1%3D1%2C+a_2%3D1%2C+%E2%8B%AF%2C+a_n%3D1%5C%7D) ，那么将得到输出为：

![](https://pic3.zhimg.com/v2-3897833ce4df55a3d054770397b9fcfa_b.jpg)(3)

    有人说式 ![(3)](https://www.zhihu.com/equation?tex=%283%29) 最后等于 ![\frac{1}{1-x}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7B1-x%7D) ，但这么说其实不准确，因为并不是对于所有的 ![x](https://www.zhihu.com/equation?tex=x) 都成立，只有当它是一个**收敛级数**时才成立！

    而式(3)中*x*的收敛域为 ![|x|∈(-1,1)](https://www.zhihu.com/equation?tex=%7Cx%7C%E2%88%88%28-1%2C1%29) ，所以当满足收敛条件时，式 ![(3)](https://www.zhihu.com/equation?tex=%283%29) 可以改写为：

![](https://pic2.zhimg.com/v2-4cb731a1e909ee2a08ad9c89f6d72ba1_b.jpg)(4)

    再举一个例子，如果 ![a(n)=\frac{1}{n!}](https://www.zhihu.com/equation?tex=a%28n%29%3D%5Cfrac%7B1%7D%7Bn%21%7D) ，即 ![\{a_0=1, a_1=\frac{1}{1!}, a_2=\frac{1}{2!}, ⋯, a_n=\frac{1}{n!}\}](https://www.zhihu.com/equation?tex=%5C%7Ba_0%3D1%2C+a_1%3D%5Cfrac%7B1%7D%7B1%21%7D%2C+a_2%3D%5Cfrac%7B1%7D%7B2%21%7D%2C+%E2%8B%AF%2C+a_n%3D%5Cfrac%7B1%7D%7Bn%21%7D%5C%7D) ，则有：

![](https://pic3.zhimg.com/v2-3b5c2fa5ab2af69ac2a802c8a3f084da_b.jpg)(5)

    在这个例子里，对于任意 ![x](https://www.zhihu.com/equation?tex=x) 均成立，即收敛域为 ![ℂ](https://www.zhihu.com/equation?tex=%E2%84%82) 。**其实式**![(5)](https://www.zhihu.com/equation?tex=%285%29)**就是函数**![e^x](https://www.zhihu.com/equation?tex=e%5Ex)**在**![x=0](https://www.zhihu.com/equation?tex=x%3D0)** 处的泰勒展开，或者说是函数**![e^x](https://www.zhihu.com/equation?tex=e%5Ex)**的麦克劳林级数**。

    从上面的例子可以看出，取一个定义在正整数上的离散函数，然后进行无穷次的相加操作，结果却能够产生一个连续函数。而且注意其中的离散函数 ![a_n](https://www.zhihu.com/equation?tex=a_n) 的变量为 ![n](https://www.zhihu.com/equation?tex=n) ，相加得出的却是关于变量 ![x](https://www.zhihu.com/equation?tex=x) 的连续函数。

    现在，让**离散求和**变成**连续求和**，即不再是变量 ![n=0, 1, 2, 3…](https://www.zhihu.com/equation?tex=n%3D0%2C+1%2C+2%2C+3%E2%80%A6) ，而是另外定义一个变量 ![t](https://www.zhihu.com/equation?tex=t) ，并且有 ![0≤t<∞](https://www.zhihu.com/equation?tex=0%E2%89%A4t%3C%E2%88%9E) ，即 ![t](https://www.zhihu.com/equation?tex=t) 可以为 ![[0，∞)](https://www.zhihu.com/equation?tex=%5B0%EF%BC%8C%E2%88%9E%29) 中的任意数。

    如果想用 ![t](https://www.zhihu.com/equation?tex=t) 取替代 ![n](https://www.zhihu.com/equation?tex=n) ，显然不能再用上面处理离散序列的办法进行求和，而是通过积分操作。即：

![](https://pic4.zhimg.com/v2-6852975295dd984be8630da944102a03_b.jpg)(6)

    式 ![(6)](https://www.zhihu.com/equation?tex=%286%29) 与式 ![(1)](https://www.zhihu.com/equation?tex=%281%29) 的区别在于：用 ![t](https://www.zhihu.com/equation?tex=t) 取替代了 ![n](https://www.zhihu.com/equation?tex=n) ；用积分符号替代了累加符号。

    我们可以保留这种形式，但是没有数学家喜欢这样做，而且工程师也很少会这样做。因为**在做微积分运算时，没有人希望其中有一个指数的底是**![x](https://www.zhihu.com/equation?tex=x)**之类的积分或微分项**，这看起来很头疼。而唯一方便的是取指数的底数为自然常数 ![e](https://www.zhihu.com/equation?tex=e) 。只有 ![e](https://www.zhihu.com/equation?tex=e)  才是人们喜欢用来积分或微分，因为 ![e^x](https://www.zhihu.com/equation?tex=e%5Ex) 在微积分时可以保证自身不变函数，详见：《[自然底数e怎么就“自然”了？](https://zhuanlan.zhihu.com/p/48391055)》和《[为什么e^x 的导数是还是其自身？](https://zhuanlan.zhihu.com/p/48678377)》。

    因此，将以*x*为底数的指数替换成以 ![e](https://www.zhihu.com/equation?tex=e) 为底数的指数形式：

![](https://pic2.zhimg.com/v2-3a72c4d485665d51913287b45af14879_b.jpg)(7)

    既然写出这个积分当然希望其可解，或者说收敛。而只有当 ![x](https://www.zhihu.com/equation?tex=x) 是一个小于 ![1](https://www.zhihu.com/equation?tex=1) 的数时，即**自然指数函数的幂为负数时**，该积分才有可能收敛，所以这里要求 ![x<1](https://www.zhihu.com/equation?tex=x%3C1) 。作为对数，还需要满足 ![x>0](https://www.zhihu.com/equation?tex=x%3E0) （对数的详细介绍请见：《[为什么说"对数"可以延长天文学家寿命？](https://zhuanlan.zhihu.com/p/48393525)》），所以这里有 ![0<x<1](https://www.zhihu.com/equation?tex=0%3Cx%3C1) 。显然，当 ![0<x<1](https://www.zhihu.com/equation?tex=0%3Cx%3C1) 时， ![lnx<0](https://www.zhihu.com/equation?tex=lnx%3C0) 。

![lnx ](https://www.zhihu.com/equation?tex=lnx+) 这个变量看起来貌似有点复杂，我们何不再用一个符号去代替它呢？

    那么就用*s*吧！

    令 ![s = -lnx ](https://www.zhihu.com/equation?tex=s+%3D+-lnx+) 或 ![-s = lnx](https://www.zhihu.com/equation?tex=-s+%3D+lnx) ，因为上面说了 ![lnx<0](https://www.zhihu.com/equation?tex=lnx%3C0) ，取 ![-s = lnx](https://www.zhihu.com/equation?tex=-s+%3D+lnx) 的话， ![s ](https://www.zhihu.com/equation?tex=s+) 就总为正数了，处理正数当然更符合人们的习惯。另外，用 ![f (x)](https://www.zhihu.com/equation?tex=f+%28x%29) 代替 ![a(x)](https://www.zhihu.com/equation?tex=a%28x%29) ，这样看上去更像我们熟悉的函数形式。这些替换**只是为了修(hao)饰(kan)**，现将这些替换代入式 ![(7)](https://www.zhihu.com/equation?tex=%287%29) 中，得：

![](https://pic1.zhimg.com/v2-0d83b8d024c2763d137d5ad6759946b4_b.jpg)(8)

    通过这种方式，我们得到了**Laplace Transform**！

    如果用符号表示这种变换，可以将式 ![(8)](https://www.zhihu.com/equation?tex=%288%29) 写为：

![](https://pic2.zhimg.com/v2-a4bee4078fb541d5db734970fd64004d_b.jpg)(9)
    这就是Laplace变换，当输入一个关于 ![t](https://www.zhihu.com/equation?tex=t) 的函数，将得到一个关于 ![s](https://www.zhihu.com/equation?tex=s) 的函数。

    最后提一句，这里说的是变换，而对于一个算子来说，就不会是这样，变换和算子的最本质区别在于，经过算子运算，变量没有变，比如**微分**就是一种典型的算子。经过变换则会改变变量的形式，类似的例子可见：《[如何给文科生解释傅里叶变换？](https://zhuanlan.zhihu.com/p/48305950)》。

**Reference**

[1] Pierre-Simon Laplace, [https://en.wikipedia.org/wiki/Pierre-Simon_Laplace](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Pierre-Simon_Laplace)

[2] Differential Equation,Arthur Mattuck

**推荐阅读**
[直观の数学​zhuanlan.zhihu.com![图标](https://pic1.zhimg.com/v2-9c87d956f28e27a418de774c1720e214_ipico.jpg)](https://zhuanlan.zhihu.com/c_1041689010053341184)
[李狗嗨：飞蛾真的是因为趋光，所以扑火？](https://zhuanlan.zhihu.com/p/48391829)

[李狗嗨：自然底数e怎么就“自然”了？](https://zhuanlan.zhihu.com/p/48391055)

[李狗嗨：拉普拉斯变换中的S是个什么鬼？](https://zhuanlan.zhihu.com/p/48314585)

[李狗嗨：虚数i真的很“虚”吗？](https://zhuanlan.zhihu.com/p/48392273)

[李狗嗨："上帝公式"(欧拉公式)真的神到无法触碰？](https://zhuanlan.zhihu.com/p/48392958)

[李狗嗨：为什么说"对数"可以延长天文学家寿命？](https://zhuanlan.zhihu.com/p/48393525)

[李狗嗨：0的0次方为何等于1？](https://zhuanlan.zhihu.com/p/48319167)

[李狗嗨：古人是如何寻找到π的？](https://zhuanlan.zhihu.com/p/48307582)

[李狗嗨：为什么e^x 的导数是还是其自身？](https://zhuanlan.zhihu.com/p/48678377)

[李狗嗨：一圈为何是360°？](https://zhuanlan.zhihu.com/p/48767522)

[李狗嗨：如何给文科生解释傅里叶变换？](https://zhuanlan.zhihu.com/p/48305950)

**更多精彩请扫码关注微信公众号，因为可能有重名的情况，最好是在微信主页面往下滑在搜索框内输入并搜索：“researchdog” 就能找到，公众号名字叫“科研狗”，公众号的头像是只戴着镜框的金毛犬。或扫下方二维码关注，谢谢噢~**

![](https://pic1.zhimg.com/v2-0fa971435cd7766d7ffdd44b7dff8458_b.jpg)

