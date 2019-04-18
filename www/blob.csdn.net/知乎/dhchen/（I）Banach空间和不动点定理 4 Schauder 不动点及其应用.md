# （I）Banach空间和不动点定理 4: Schauder 不动点及其应用 - 知乎
# 

在[（I）Banach空间和不动点定理 2 : 不动点定理和钟摆问题](https://zhuanlan.zhihu.com/p/26365534)中我们介绍了Banach不动点和它的应用，这个不动点的特点是需要算子具有很强的Lipschitz连续性而且它的Lipschitz的系数要小于1，这是一个非常强的结论。即使在一般的欧式空间中很多函数都是不满足这个条件的。**我们希望得到更一般的“不动点”定理：**在这一节中，我们我主要的目的是介绍Schauder不动点及其在微分方程／积分方程方面的应用。我们的目标是解下面两种方程：
![](https://pic2.zhimg.com/v2-6b2acad1a46bd362a4c59b87f1788985_b.png)

## 一、Brouwer不动点 （有限维的Schauder不动点）
![](https://pic1.zhimg.com/v2-d5a758ebc77b02b927d2a53f7570cd90_b.png)
知乎上不是特别喜欢问：**有什么形象理解方法吗？**

![](https://pic4.zhimg.com/v2-41bc2d269f4ba98bba991160743c30a7_b.gif)
一维：设![M=[0,1]](https://www.zhihu.com/equation?tex=M%3D%5B0%2C1%5D)，![h(x)=f(x)-x](https://www.zhihu.com/equation?tex=h%28x%29%3Df%28x%29-x)。 则我们可以发现![h(0)\geq 0](https://www.zhihu.com/equation?tex=h%280%29%5Cgeq+0) 而![h(1)\leq 0](https://www.zhihu.com/equation?tex=h%281%29%5Cleq+0)。显然根据函数的联系性我们可以知道至少存在一个点使得![h(x)=x](https://www.zhihu.com/equation?tex=h%28x%29%3Dx)。
![](https://pic1.zhimg.com/v2-e8361180c5e7ec03c4646e02e59f45ec_b.png)

二维：我们想象两张一摸一样的带坐标的橡皮，一个摊平放在桌上，一个在这个橡皮的正上方，使得下面橡皮上的每一点正上方都有对应一个点。你可以把上面的皮揉搓，拉长，皱起。但是不允许撕开或者超出下面橡皮的范围。那么至少存在一个点使得它对应的点和自己的坐标是一样的。

三维： 你有一杯茶，你拿着小勺子在里面慢慢搅动（保证不溢出），那么至少存在一个点是不会动。 

这个定理的具体证明方法有很多种，可以借助代数拓扑，可以借助组合论Sperner Lemma，也可以用“stokes定理”就能证明出某个特殊情况。我会在下一节附上一个“简单的证”明。实际上Brouwer定理的证明都要用到一些“奇奇怪怪的”东西。 


**好了，少年们？你们是不是觉得无限维这样的结论也成立呢？NO NO NO。**

**一个反例： **

![](https://pic1.zhimg.com/v2-c0ec70477ab8debbb8980a630bd85b20_b.png)
好了，怎么办？难道就没有别的方法了吗？我们在上一节不是说过“紧性”和“有限维”很像吗？我们将引入一类新的算子，并且把Brouwer不动点推广到无限维空间的Schauder不动点。

## 二、紧算子和Schauder不动点  

![](https://pic4.zhimg.com/v2-4144808ebea1fc54b2e6fbcdc4dd0a5f_b.png)
对于这种紧算子，我们有下面的不动点定理。 
![](https://pic3.zhimg.com/v2-9d5194d00ae0e2dada916239b8e9a986_b.png)
上一节中，我们学过一个点：“紧集”和“有限维空间”差不多，**下面的结果说明“紧算子”和“有限维空间算子”差不多。 **

![](https://pic3.zhimg.com/v2-68b4064fe062a1a683fbea6ee18101aa_b.png)

下面我们给出Shauder不动点的一个证明，**这个证明是建立在Brouwder不动点的基础上的。**
![](https://pic1.zhimg.com/v2-c18838085078659f409322479e361538_b.png)

## 三、不动点的应用

**好了，那么这个Schauder不动点有什么用呢？这大概是不动点定理里面最常用的一个。我们利用它的思路和之前的差不多，对于一个存在性问题，我们构造一个算子，把存在性问题变成不动点问题，然后解决这个不动点。 **

我们会在空间![C[a,b]](https://www.zhihu.com/equation?tex=C%5Ba%2Cb%5D)里面讨论紧算子，所以，我们第一个问题是：![C[a,b]](https://www.zhihu.com/equation?tex=C%5Ba%2Cb%5D)的预紧集是什么样子的？这个问题可以通过下面的定理回答：

![](https://pic2.zhimg.com/v2-a40329d2275683b76a72c76f954157dd_b.png)
**1.积分方程：**

![](https://pic4.zhimg.com/v2-39ce78bbc5689286f486af2dc3bc9f8f_b.png)
在[0.2 泛函分析简史(1) - 知乎专栏](https://zhuanlan.zhihu.com/p/25935855)中我提及了这个方程的起源，我们在这里给出一个解答。

![](https://pic4.zhimg.com/v2-b3e298ec25049c5fb0c4d0e39ec7caeb_b.png)

![](https://pic2.zhimg.com/v2-fa8be37db8cd366726eb739d318eea31_b.png)


**    2.微分方程：**

我们虽然用Banach不动点证明过微分方程的存在性定理，但是那个定理要求![F](https://www.zhihu.com/equation?tex=F)是Lipschitz的，但是这个连续性有点过强了。下面的定理只需要它是连续的。 
![](https://pic4.zhimg.com/v2-c9b416b44d6729c84da5cecdecc2cfbb_b.png)



## 下一节：

## [Banach空间和不动点定理（完）：有趣的Brouwer不动点 - 知乎专栏](https://zhuanlan.zhihu.com/p/26764450)

