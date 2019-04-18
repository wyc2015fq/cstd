# 为什么e^x 的导数是还是其自身？ - 知乎
# 

前年看了一部家喻户晓的国产科幻小说——《三体》，刷新了自己的世界观，我不仅对“黑暗森林”理论感到惊异，还对作者刘慈欣充满了敬佩。能写出这样的科幻作品势必需要丰厚的数学、物理、天文、地理等知识。

书中留给我最深刻印象、让我觉得最不可思议的当属“歌者”使用的“二向箔”武器，其厉害之处在于可以将“三维”中的一个维度坍缩，最后变成“二维”，例如下图。

![](https://pic1.zhimg.com/v2-34c3fe9ec9869afb2d2ce78f81ecd168_b.jpg)（图片来源于网络）

就好像一个三维物体变成了一幅画。

“歌者”作为未知高等文明的底层人员，担任的工作是监听并分析太空信号，他一直监视着地球文明和三体文明，并毁灭其发现的文明。他这样做的原因作者已经通过“黑暗森林”理论进行了阐述。书中提到了宇宙社会学中的两条公理。**第一，生存是文明的第一需要；第二，文明不断扩张，但宇宙中物质的总量保持不变。**

**因此，宇宙就像一个“黑暗森林”，每个文明都是带枪的猎人，潜行于这林间，一旦发现其他生命，不管是天使还是魔鬼，都要将其消灭，因为其他生命是永恒的威胁、永恒的资源争夺者，任何暴露自己存在的文明都会被消灭。**

在罗辑发出的坐标星体和三体母星被其他监听人员毁灭之后，“歌者”发现并开始清理太阳系文明，但在清理的时候他发现了盲点（地球文明采取了掩体计划），所以放弃采用光粒而使用降维武器“二向箔”进行攻击，太阳最终被永远地定格成了二维。而且由于二向箔打击范围内的逃逸速度为光速，因此除非以光速逃逸，所有三维事物都无法逃脱“二向箔”的打击。

那高等文明如果想避免受到“二向箔”的打击，就要**自己将自己变为低维**，那样就能在遭受“二向箔”攻击时保持自身不变，以保全自己的“文明”。

其实，数学中也有一种东西可以看做是“降维操作”，那就是“求导”，而且**数学中有一个特例，能够在这种“降维操作”中，保持自身不变，那就是以**![e](https://www.zhihu.com/equation?tex=e)**为底的指数，或者说自然指数**。

![](https://pic1.zhimg.com/v2-6898ff91f77234ec2afcae22ca5cbe58_b.jpg)

**自然对数/指数求导**

前面已经说到了 ![e](https://www.zhihu.com/equation?tex=e) 的来源，以及以自然对数为底的指数、对数，相关文章有《[自然底数e怎么就“自然”了？》](https://zhuanlan.zhihu.com/p/48391055)、《[0的0次方为何等于1？](https://zhuanlan.zhihu.com/p/48319167)》、《[对数可以延长人类寿命？](https://zhuanlan.zhihu.com/p/48393525)》、《[拉普拉斯变换中的S是个什么鬼？](https://zhuanlan.zhihu.com/p/48314585)》等。但是因为其特殊性，想必有人对其中一个问题比较好奇，就是为什么对自然指数函数求导的结果还是其本身，所以针对这种特殊性质做了一下推导。

我们知道 ![e](https://www.zhihu.com/equation?tex=e) 是通过下面的公式得到的，推导过程可见《[自然底数e怎么就“自然”了？》](https://zhuanlan.zhihu.com/p/48391055)：

![](https://pic3.zhimg.com/v2-d65a082374bff21b80f8441262cd9c2a_b.jpg)

为了得到更加普适的公式，先从对以 ![a](https://www.zhihu.com/equation?tex=a) 为底的对数函数求导入手，来研究对指数函数的求导，进而找到对自然指数求导这种特殊情况下的求导结果。

## **1、对数求导**

令对数函数：
![](https://pic2.zhimg.com/v2-9400580dafdbbca11cf86165b58406f5_b.jpg)
由导数的定义有：
![](https://pic1.zhimg.com/v2-982ad37d2e29ebde83a37c00c1dfb2b4_b.jpg)

这样，我们就得到了对数函数的通用求导公式。显然，**当**![a=e](https://www.zhihu.com/equation?tex=a%3De)**时有**![(lnx)’=\frac{1}{x}.](https://www.zhihu.com/equation?tex=%28lnx%29%E2%80%99%3D%5Cfrac%7B1%7D%7Bx%7D.)

以上就是对数及自然对数的求导过程及结果。

## **2、反函数的导数**

由于**指数函数和对数函数互为反函数**，所以先看看反函数的求导结果与原函数求导结果的关系：

设 ![y=f(x)](https://www.zhihu.com/equation?tex=y%3Df%28x%29) 为 ![x=f^{-1}(y)](https://www.zhihu.com/equation?tex=x%3Df%5E%7B-1%7D%28y%29) 的反函数， ![f^{-1}(y)](https://www.zhihu.com/equation?tex=f%5E%7B-1%7D%28y%29) 在 ![y](https://www.zhihu.com/equation?tex=y) 的某领域内严格单调可导，且满足 ![[f^{-1}(y)]’ ≠ 0](https://www.zhihu.com/equation?tex=%5Bf%5E%7B-1%7D%28y%29%5D%E2%80%99+%E2%89%A0+0) .

在 ![x](https://www.zhihu.com/equation?tex=x) 处给增量 ![Δx ≠0](https://www.zhihu.com/equation?tex=%CE%94x+%E2%89%A00) ，由反函数的单调性知： ![Δy=f(x+Δx)–f(x) ≠ 0](https://www.zhihu.com/equation?tex=%CE%94y%3Df%28x%2B%CE%94x%29%E2%80%93f%28x%29+%E2%89%A0+0)

又反函数的连续性知， ![Δx→0](https://www.zhihu.com/equation?tex=%CE%94x%E2%86%920) 时必有 ![Δy→0](https://www.zhihu.com/equation?tex=%CE%94y%E2%86%920) ，因此：

![](https://pic1.zhimg.com/v2-9f932ec125da57b856825adfa8488594_b.jpg)

即，**反函数的导数等于原函数导数的倒数**。

## **3、指数求导**

既然如此，利用上述的对数函数求导结果和反函数的导数，可以证明对指数函数的微分进行证明。

令 ![y=f(x)=a^x](https://www.zhihu.com/equation?tex=y%3Df%28x%29%3Da%5Ex)

则有
![](https://pic1.zhimg.com/v2-04af82c6ccc1de3bb4568e88de89a330_b.jpg)

**当**![a=e](https://www.zhihu.com/equation?tex=a%3De)**时，有**![(e^x)’=e^{x}\cdot lne=e^x](https://www.zhihu.com/equation?tex=%28e%5Ex%29%E2%80%99%3De%5E%7Bx%7D%5Ccdot+lne%3De%5Ex) 。

这就是为什么以自然底数 ![e](https://www.zhihu.com/equation?tex=e) 为底的指数求导之后还是其本身。

![](https://pic2.zhimg.com/v2-56d7fd64f434f76245595830a9614349_b.jpg)

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

![](https://pic3.zhimg.com/v2-13d90069591d4dd15c9998d6d8889216_b.jpg)

